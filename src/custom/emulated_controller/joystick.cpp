#include "custom/emulated_controller.hpp"
#include "custom/mathlib.h"

namespace MKV5 {
namespace ControllerInputs {
// JOYSTICKS
Joystick::Joystick(pros::Controller *ctrl, pros::controller_analog_e_t stkX,
                   pros::controller_analog_e_t stkY)
    : stickX(stkX), stickY(stkY), control(ctrl) {}

void Joystick::SetStick(float nx, float ny) {
	moving = (fabs(nx) > deadzone ||
	          fabs(ny) > deadzone); // FIXED: moving when OUTSIDE deadzone
			

	if (OnMoveCallback != nullptr && moving) {
		OnMoveCallback(nx - X, ny - Y);
	}
	X = linear_slew(X, fabs(nx) > deadzone ? nx : 0.0f);
	Y = linear_slew(Y, fabs(ny) > deadzone ? ny : 0.0f);
}
void Joystick::update() {
	if (!process || control == nullptr) {
		return;
	}
	if (static_cast<float>(control->get_analog(stickX)) != X ||
	    static_cast<float>(control->get_analog(stickY)) != Y) {
		SetStick(static_cast<float>(control->get_analog(stickX)),
		         static_cast<float>(control->get_analog(stickY)));
	} else {
		SetStick(static_cast<float>(control->get_analog(stickX)),
		         static_cast<float>(control->get_analog(stickY)));
	}
}
void Joystick::SetEnabled(bool enabled) { process = enabled; }
void Joystick::OnMove(const std::function<void(float, float)> &func) {
	OnMoveCallback = func;
}
} // namespace ControllerInputs
} // namespace ControllerLib
