#include "custom/controls.hpp"
#include "custom/emulated_controller.hpp"
#include "custom/enums.hpp"
#include "custom/modded.hpp"
#include <variant>

namespace MKV5 {
ControlScheme::ControlScheme(Units::ControllerDriveTypeUnit typ,
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
		if (std::holds_alternative<MKV5::CustomMotor *>(
		        binding.motor)) {
			auto m = std::get<MKV5::CustomMotor *>(binding.motor);

			if (std::holds_alternative<ControllerInputs::Button *>(
			        binding.buttons)) {
				auto b = std::get<ControllerInputs::Button *>(
				    binding.buttons);
				b->update();

				if (b->pressed) {
					m->setVelocity(binding.speed);
					m->spin(Units::DirectionUnit::FORWARD);
				} else {
					m->brake();
				}
			}

			if (std::holds_alternative<
			        std::pair<ControllerInputs::Button *,
			                  ControllerInputs::Button *>>(
			        binding.buttons)) {
				auto [f, r] = std::get<
				    std::pair<ControllerInputs::Button *,
				              ControllerInputs::Button *>>(
				    binding.buttons);
				f->update();
				r->update();

				bool fp = f->pressed;
				bool rp = r->pressed;

				if (fp ^ rp) {
					m->setVelocity(binding.speed);
					m->spin(
					    fp ? Units::DirectionUnit::FORWARD
					       : Units::DirectionUnit::REVERSE);
				} else {
					m->brake();
				}
			}
		}

		if (std::holds_alternative<MKV5::Piston *>(binding.motor)) {
			auto v = std::get<MKV5::Piston *>(binding.motor);

			if (std::holds_alternative<ControllerInputs::Button *>(
			        binding.buttons)) {
				auto b = std::get<ControllerInputs::Button *>(
				    binding.buttons);
				b->update();
				v->setState(b->pressed);
			}

			if (std::holds_alternative<
			        std::pair<ControllerInputs::Button *,
			                  ControllerInputs::Button *>>(
			        binding.buttons)) {
				auto [f, r] = std::get<
				    std::pair<ControllerInputs::Button *,
				              ControllerInputs::Button *>>(
				    binding.buttons);
				f->update();
				r->update();

				bool fp = f->pressed;
				bool rp = r->pressed;

				if (fp ^ rp) {
					v->setState(fp);
				}
			}
		}

		if (std::holds_alternative<MKV5::MotorGroup *>(binding.motor)) {
			auto mg = std::get<MKV5::MotorGroup *>(binding.motor);

			if (std::holds_alternative<ControllerInputs::Button *>(
			        binding.buttons)) {
				auto b = std::get<ControllerInputs::Button *>(
				    binding.buttons);
				b->update();

				if (b->pressed) {
					mg->setVelocity(binding.speed);
					mg->spin(
					    Units::DirectionUnit::FORWARD);
				} else {
					mg->stop();
				}
			}

			if (std::holds_alternative<
			        std::pair<ControllerInputs::Button *,
			                  ControllerInputs::Button *>>(
			        binding.buttons)) {
				auto [f, r] = std::get<
				    std::pair<ControllerInputs::Button *,
				              ControllerInputs::Button *>>(
				    binding.buttons);
				f->update();
				r->update();

				bool fp = f->pressed;
				bool rp = r->pressed;

				if (fp ^ rp) {
					mg->setVelocity(binding.speed);
					mg->spin(
					    fp ? Units::DirectionUnit::FORWARD
					       : Units::DirectionUnit::REVERSE);
				} else {
					mg->stop();
				}
			}
		}
	}
}

void ControlScheme::doControllerInputs() {
	switch (configuration.CONTROL_SCHEME) {

	case MKV5::Units::ControllerDriveTypeUnit::DRIVE_MODE_ARCADE:
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

	case MKV5::Units::ControllerDriveTypeUnit::DRIVE_MODE_TANK:
		leftVelocity = static_cast<int>(
		    LeftJoystick->Y * (configuration.maxForwardSpeed));
		rightVelocity = static_cast<int>(
		    RightJoystick->Y * (configuration.maxForwardSpeed));
		break;

	case MKV5::Units::ControllerDriveTypeUnit::DRIVE_MODE_GTA: {
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

	case MKV5::Units::ControllerDriveTypeUnit::DRIVE_MODE_CHEESE: {
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
		drive.move(Units::DirectionUnit::FORWARD);
	} else {
		drive.stop();
	}
	fr++;
}
} // namespace MKV5
