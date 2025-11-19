#include "custom/controls.hpp"

namespace MKV5 {
ControlScheme::ControlScheme(Enums::ControllerDriveTypes typ,
                             Drivetrain &driveRef,
                             EmulatedController &controllerRef)
    : drive(driveRef), controller(controllerRef) {
	configuration.CONTROL_SCHEME = typ;
	LeftJoystick = &controller.joysticks.Left;
	RightJoystick = &controller.joysticks.Right;
}
void ControlScheme::_updateAccumulators() {
	if (negInertiaAccumlator > 1) {
		negInertiaAccumlator -= 1;
	} else if (negInertiaAccumlator < -1) {
		negInertiaAccumlator += 1;
	} else {
		negInertiaAccumlator = 0;
	}

	if (quickStopAccumlator > 1) {
		quickStopAccumlator -= 1;
	} else if (quickStopAccumlator < -1) {
		quickStopAccumlator += 1;
	} else {
		quickStopAccumlator = 0.0;
	}
}
void ControlScheme::registerMotor(ControllerInputs::ControlBinding binding) {
	if (std::holds_alternative<MKV5::CustomMotor *>(binding.motor)) {
		auto m = std::get<MKV5::CustomMotor *>(binding.motor);
		if (std::holds_alternative<ControllerInputs::Button>(
		        binding.buttons)) {
			auto &btns =
			    std::get<ControllerInputs::Button>(binding.buttons);
			// we have a single button
			if (binding.toggle) {
				auto onpress = [m, &binding]() {
					if (m->getMovement()) {
						m->move(DRIVE_STOP);
					} else {
						m->move(DRIVE_FORWARD,
						        binding.speed);
					}
				};
				btns.OnButtonPress(onpress);
			} else {
				auto onpress = [&m, &binding]() {
					m->move(DRIVE_FORWARD, binding.speed);
				};
				auto onrelease = [&m, &binding]() {
					m->move(DRIVE_STOP, binding.speed);
				};
				btns.OnButtonPress(onpress);
				btns.OnButtonRelease(onrelease);
			}
		} else {
			if (std::holds_alternative<std::pair<
			        ControllerInputs::Button, ControllerInputs::Button>>(
			        binding.buttons)) {
				auto &btns =
				    std::get<std::pair<ControllerInputs::Button,
				                       ControllerInputs::Button>>(
				        binding.buttons);
				// we have two buttons
				// toggle doesnt work on this so
				auto onforwardpress = [&m, &binding]() {
					m->move(DRIVE_FORWARD, binding.speed);
				};
				auto onbackwardspress = [&m, &binding]() {
					m->move(DRIVE_REVERSE, binding.speed);
				};
				auto onrelease = [&m, &binding]() {
					m->move(DRIVE_STOP, binding.speed);
				};
				btns.first.OnButtonPress(onforwardpress);
				btns.second.OnButtonPress(onbackwardspress);
				btns.first.OnButtonRelease(onrelease);
				btns.second.OnButtonRelease(onrelease);
			}
		}
	} else {
		auto &mg = std::get<MotorGroup *>(binding.motor);
		// use mg
	}
}
void ControlScheme::doControllerInputs() {
	switch (configuration.CONTROL_SCHEME) {
	case ARCADE_DRIVE:
		leftVelocity =
		    static_cast<int>(RightJoystickX *
		                     -(configuration.maxTurnSpeed)) -
		    static_cast<int>(-LeftJoystickY *
		                     (configuration.maxForwardSpeed));
		rightVelocity =
		    static_cast<int>(RightJoystickX *
		                     -(configuration.maxTurnSpeed)) +
		    static_cast<int>(-LeftJoystickY *
		                     (configuration.maxForwardSpeed));
		break;

	case TANK_DRIVE:
		leftVelocity = static_cast<int>(
		    LeftJoystickY * (configuration.maxForwardSpeed));
		rightVelocity = static_cast<int>(
		    RightJoystickY * -(configuration.maxForwardSpeed));
		break;

	case GTA_DRIVE: {
		int forward = (controller.buttons.R2.pressed ? 1 : 0) -
		              (controller.buttons.L2.pressed ? 1 : 0);
		int turn = static_cast<int>(LeftJoystickX);

		leftVelocity =
		    static_cast<int>((forward * configuration.maxForwardSpeed) -
		                     (turn * configuration.maxTurnSpeed));
		rightVelocity =
		    static_cast<int>((forward * configuration.maxForwardSpeed) +
		                     (turn * configuration.maxTurnSpeed));
		break;
	}

	case CHEESE_DRIVE: {
		cheeseDrive();
		break;
	}

	default:
		break;
	}
}

void ControlScheme::update() {
	controller.update(); // For the EmulatedController
	if (!configuration.enabled) {
		return;
	}

	// Read joystick raw values through the pointer members and apply curve
	float LeftJoystickX = JoystickCurve(LeftJoystick->X);
	float LeftJoystickY = JoystickCurve(LeftJoystick->Y);
	float RightJoystickX = JoystickCurve(RightJoystick->X);
	float RightJoystickY = JoystickCurve(RightJoystick->Y);

	doControllerInputs();
	if (fabs(leftVelocity) > 0 || fabs(rightVelocity) > 0) {
		leftVelocity =
		    minmax(static_cast<int>(leftVelocity), -127, 127);
		rightVelocity =
		    minmax(static_cast<int>(rightVelocity), -127, 127);
		drive.setVelocity(leftVelocity, rightVelocity);
		drive.startDrive(DRIVE_FORWARD);
	} else {
		configuration.timeSinceJoystickStop = pros::millis();
		drive.stopDrive();
	}
	fr++;
}
} // namespace MKV5
