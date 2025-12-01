#pragma once
#include "custom/enums.hpp"
#include "custom/modded.hpp"
#include "pros/misc.hpp"
#include <functional>
#include <math.h>
#include <vector>
#include "config.h"
#include "piston.hpp"
namespace MKV5 {

class EmulatedController;
namespace ControllerInputs {

struct Joystick {
	pros::controller_analog_e_t stickX;
	pros::controller_analog_e_t stickY;
	float X = 0;
	float Y = 0;
	float deadzone = 0.15f;
	bool process = true;
	bool moving = false;
	std::function<void(float, float)> OnMoveCallback = nullptr;
	pros::Controller *control = nullptr;

	Joystick() = default;
	Joystick(pros::Controller *ctrl, pros::controller_analog_e_t stkX,
	         pros::controller_analog_e_t stkY);

	/*Called automatically by the EmulatedController loop, but this is used
	 * if macro mode is not enabled to link the joystick to the physical
	 * joystick.*/
	void update();

	/*Sets the stick X and stick Y. If you want to set these values, call
	 * SetEnabled(false) to disable the joystick overwriting it.*/
	void SetStick(float nx, float ny);

	/*Enabled / disables Macro mode.*/
	void SetEnabled(bool enabled);

	/*
	    Pass in a function and it will call it with the DIFFERENCE between
	   the old joystick and the new joystick
	    @param func This is an std::function with two args, both being
	   float.The first one is the difference in the X and the second one is
	   the difference in the Y.
	*/
	void OnMove(const std::function<void(float, float)> &func);
};

struct Button {
	pros::controller_digital_e_t button;
	pros::Controller *control = nullptr;

	bool pressed = false;
	bool process = true;
	bool triggered_press_callback = false;
	bool triggered_release_callback = false;
	EmulatedController *parent = nullptr;
	std::function<void()> OnPressCallback = nullptr;
	std::function<void()> OnReleaseCallback = nullptr;

	Button() = default;

	Button(EmulatedController *parent_, pros::Controller *ctrl,
	       pros::controller_digital_e_t btn);

	/*Processes button events. Called by EmulatedController main loop,
	 * calling manually does next to nothing.*/
	void update();

	/*Changes the button's state. To manually call this run
	 * SetEnabled(false)*/
	void SetButton(bool state);

	/*Enabled / disables the button; disabling this will allow the user to
	 * control it*/
	void SetEnabled(bool enabled);

	/*
	    Callback for when the button is pressed.
	    @param func An std::function with no args.
	*/
	void OnButtonPress(const std::function<void()> &func);
	/*
	    Callback for when the button is released.
	    @param func An std::function with no args.
	*/
	void OnButtonRelease(const std::function<void()> &func);
};
struct ControlBinding {
    std::variant<MKV5::CustomMotor*, MKV5::MotorGroup*, MKV5::Piston*> motor;

    std::variant<
        MKV5::ControllerInputs::Button*,
        std::pair<MKV5::ControllerInputs::Button*, MKV5::ControllerInputs::Button*>
    > buttons;

    int speed = 127;
    bool toggle = false;
    bool _toggleState = false;
};

} // namespace ControllerInputs
class EmulatedController {
      private:
	pros::Controller *controller;
	std::vector<ControllerInputs::Button *> btns;

      public:
	struct {
		ControllerInputs::Button A;
		ControllerInputs::Button B;
		ControllerInputs::Button Y;
		ControllerInputs::Button X;
		ControllerInputs::Button Left;
		ControllerInputs::Button Right;
		ControllerInputs::Button Up;
		ControllerInputs::Button Down;
		ControllerInputs::Button L1;
		ControllerInputs::Button L2;
		ControllerInputs::Button R1;
		ControllerInputs::Button R2;
		ControllerInputs::Button Power;
	} buttons;

	struct {
		ControllerInputs::Joystick Left;
		ControllerInputs::Joystick Right;
	} joysticks;

	double battery = 100;
	bool enabled = true;
	bool connected = false;

	EmulatedController(pros::Controller *ctrl);
	/*
	    Vibrates a controller based off a pattern.
	    WARNING! Buggy & limited.
	    Use a pattern of this:
	     - Spaces: break
	     - Dash: long
	     - Dot/Period: short

	    --.--.
	*/
	void vibrate(std::string pattern);
	/*
	    Processes joystick inputs
	*/
	void update();
};
} // namespace MKV5