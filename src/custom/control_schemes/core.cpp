#include "custom/controls.hpp"
#include "custom/emulated_controller.hpp"
#include "custom/enums.hpp"
#include <string>
#include <variant>

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
void ControlScheme::registerMotor(ControllerInputs::ControlBinding &binding) {
	bindings.push_back(binding);
}
void ControlScheme::processBindings() {
	for (auto &binding : bindings) {

		// Determine motor target
		MKV5::CustomMotor *m = nullptr;
		MotorGroup *mg = nullptr;
		MKV5::Piston *v = nullptr;
        std::function<void(int)> startForward;
        std::function<void(int)> startReverse;
        std::function<void()> stopMotor;
		if (std::holds_alternative<MKV5::CustomMotor *>(
		        binding.motor)) {
			m = std::get<MKV5::CustomMotor *>(binding.motor);

			auto startForward = [&](int speed) {
				m->setVelocity(speed);
				m->move(Enums::Direction::FORWARD);
			};

			auto startReverse = [&](int speed) {
				m->setVelocity(speed);
				m->move(Enums::Direction::REVERSE);
			};
			auto stopMotor = [&]() {
				m->move(Enums::Direction::STOP);
			};
		}

		else if (std::holds_alternative<MKV5::Piston *>(
		             binding.motor)) {
			v = std::get<MKV5::Piston *>(binding.motor);

			auto startForward = [&](int speed) { v->setState(true); };

			auto startReverse = [&](int speed) { v->setState(false); };
			auto stopMotor = [&]() { v->setState(false); };
		} else {
			mg = std::get<MotorGroup *>(binding.motor);
			// Helper lambdas so logic is clean
			auto startForward = [&](int speed) {
				mg->setVelocity(speed);
				mg->startMove(Enums::Direction::FORWARD);
			};

			auto startReverse = [&](int speed) {
				mg->setVelocity(speed);
				mg->startMove(Enums::Direction::REVERSE);
			};

			auto stopMotor = [&]() {
				mg->startMove(Enums::Direction::STOP);
			};
		}

		// ----------------------
		// SINGLE BUTTON BINDING
		// ----------------------

		if (std::holds_alternative<ControllerInputs::Button *>(
		        binding.buttons)) {
			auto *btn = std::get<ControllerInputs::Button *>(
			    binding.buttons);

			if (btn->pressed) {
				if (binding.toggle) {
					// Toggle activates ONLY on rising edge
					if (!binding._toggleState) {
						binding._toggleState = true;

						// If motor currently moving →
						// stop
						if (m) {
							stopMotor();
						} else if (mg) {
							stopMotor();
						}
						// Otherwise → start forward
						else {
							startForward(
							    binding.speed);
						}
					}
				} else {
					// Non-toggle (hold-to-drive)
					startForward(binding.speed);
				}
			} else {
				// Button released
				if (!binding.toggle) {
					stopMotor();
				}
				binding._toggleState =
				    false; // reset rising-edge latch
			}
		}

		// ----------------------
		// TWO BUTTON BINDING
		// ----------------------

		else if (std::holds_alternative<
		             std::pair<ControllerInputs::Button *,
		                       ControllerInputs::Button *>>(
		             binding.buttons)) {
			auto &pair =
			    std::get<std::pair<ControllerInputs::Button *,
			                       ControllerInputs::Button *>>(
			        binding.buttons);

			if (pair.first->pressed) {
				startForward(binding.speed);
				std::cout << "first" << std::endl;
			}
			if (pair.second->pressed) {
				std::cout << "second" << std::endl;
				startReverse(binding.speed);
			}
			if (!pair.second->pressed && !pair.first->pressed) {
				stopMotor();
			}
		}
	}
}

void ControlScheme::doControllerInputs() {
	switch (configuration.CONTROL_SCHEME) {

	case MKV5::Enums::ControllerDriveTypes::DRIVE_MODE_ARCADE:
		leftVelocity =
		    static_cast<int>(LeftJoystick->Y *
		                     -(configuration.maxTurnSpeed)) -
		    static_cast<int>(-RightJoystick->X *
		                     (configuration.maxForwardSpeed));
		rightVelocity =
		    static_cast<int>(LeftJoystick->Y *
		                     -(configuration.maxTurnSpeed)) +
		    static_cast<int>(-RightJoystick->X *
		                     (configuration.maxForwardSpeed));
		break;

	case MKV5::Enums::ControllerDriveTypes::DRIVE_MODE_TANK:
		leftVelocity = static_cast<int>(
		    LeftJoystick->Y * (configuration.maxForwardSpeed));
		rightVelocity = static_cast<int>(
		    RightJoystick->Y * (configuration.maxForwardSpeed));
		break;

	case MKV5::Enums::ControllerDriveTypes::DRIVE_MODE_GTA: {
		int forward = (controller.buttons.R2.pressed ? 1 : 0) -
		              (controller.buttons.L2.pressed ? 1 : 0);
		int turn = static_cast<int>(LeftJoystick->X);

		leftVelocity =
		    static_cast<int>((forward * configuration.maxForwardSpeed) -
		                     (turn * configuration.maxTurnSpeed));
		rightVelocity =
		    static_cast<int>((forward * configuration.maxForwardSpeed) +
		                     (turn * configuration.maxTurnSpeed));
		break;
	}

	case MKV5::Enums::ControllerDriveTypes::DRIVE_MODE_CHEESE: {
		cheeseDrive();
		break;
	}

	default:
		break;
	}
}

void ControlScheme::update() {
	controller.update(); // For the EmulatedController
	// Read joystick raw values through the pointer members and apply curve
	doControllerInputs();
	if (fabs(leftVelocity) > 0 || fabs(rightVelocity) > 0) {
		leftVelocity =
		    minmax(static_cast<int>(leftVelocity), -127, 127);
		rightVelocity =
		    minmax(static_cast<int>(rightVelocity), -127, 127);
		drive.setVelocity(leftVelocity, rightVelocity);
		drive.startDrive(DRIVE_FORWARD);
	} else {
		drive.stopDrive();
	}
	fr++;
}
} // namespace MKV5
