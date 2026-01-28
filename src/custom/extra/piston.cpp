#include "custom/piston.hpp"
#include "pros/adi.h"
#include "pros/ext_adi.h"
namespace MKV5 {
Piston::Piston(int prt, bool start) : port(prt), state(start) {
    pros::c::adi_port_set_config(port, pros::E_ADI_DIGITAL_OUT);
    pros::c::adi_digital_write(port, start);
}

bool Piston::getState() { return state; }
bool Piston::toggleState() {
	state = !state;
	pros::c::adi_digital_write(port, state);
	return state;
}
void Piston::setState(bool s) {
	state = s;
	pros::c::adi_digital_write(port, state);
};
void Piston::enable() {
	Piston::setState(true);
}
void Piston::disable() {
	Piston::setState(false);
}
} // namespace MKV5