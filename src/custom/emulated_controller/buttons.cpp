#include "custom/emulated_controller.hpp"

namespace MKV5 {
namespace ControllerInputs {
// BUTTONS
Button::Button(EmulatedController *parent_, pros::Controller *ctrl,
               pros::controller_digital_e_t btn)
    : button(btn), control(ctrl), parent(parent_) {}
void Button::update() {
	if (!process || control == nullptr) {
		return;
	}
	SetButton(control->get_digital(button));
}
void Button::SetButton(bool state) {
	pressed = state;
	if (pressed) {
		if (OnPressCallback != nullptr && !triggered_press_callback) {
			triggered_press_callback = true;
			triggered_release_callback = false;
			OnPressCallback();
		}
	} else {
		if (OnReleaseCallback != nullptr &&
		    !triggered_release_callback) {
			triggered_release_callback = true;
			triggered_press_callback = false;
			OnReleaseCallback();
		}
	}
}
void Button::SetEnabled(bool enabled) { process = enabled; }
void Button::OnButtonPress(const std::function<void()> &func) {
	OnPressCallback = func;
}
void Button::OnButtonRelease(const std::function<void()> &func) {
	OnReleaseCallback = func;
}
} // namespace ControllerInputs
} // namespace MKV5