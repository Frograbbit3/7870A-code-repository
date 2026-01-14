#pragma once
#include "custom/drivetrain.hpp"
#include "custom/emulated_controller.hpp"
#include "custom/enums.hpp"

namespace MKV5 {
class ControlScheme {
      public:
	MKV5::ControllerSettings configuration;
	std::vector<ControllerInputs::ControlBinding> bindings;

      private:
	int current_action;
	int fr;
	MKV5::Drivetrain &drive;
	MKV5::EmulatedController &controller;
	int16_t leftVelocity;
	int16_t rightVelocity;
	MKV5::ControllerInputs::Joystick *RightJoystick;
	MKV5::ControllerInputs::Joystick *LeftJoystick;

	float linearCmd = 0.0f;
	bool turnInPlace = false;
	double prevThrottle;
	double prevTurn;
	int left;
	int right;
	double quickStopAccumlator = 0.0;
	double negInertiaAccumlator = 0.0;
	float LeftJoystickX;
	float LeftJoystickY;
	float RightJoystickX;
	float RightJoystickY;
	float decreased_speed = 1.0f;

	void _updateAccumulators();
	void tankDrive();
	void arcadeDrive();
	void gtaDrive();
	void cheeseDrive();
	void doControllerInputs();

      public:
	ControlScheme(MKV5::Units::ControllerDriveTypeUnit typ,
	              MKV5::Drivetrain &driveRef,
	              EmulatedController &controllerRef);

	/*
	    Creates a binding for a motor. Useful for keybinding to a button.
	    @param binding a ControlBinding
	*/
	void registerMotor(ControllerInputs::ControlBinding &binding);

      public:
	/*
	    Processes controls. This is what you want to call.
	*/
	void update();
	void processBindings();
};
} // namespace MKV5
