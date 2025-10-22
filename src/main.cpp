#include "main.h"
#include <numeric> 

/// @brief Variable init
pros::Controller master(pros::E_CONTROLLER_MASTER);
DriveUtils::Drivetrain drivetrain ({3,9,12}, {10,5,20}); //put motor ports here
ControllerLib::ControlScheme control(
	TANK_DRIVE,
	drivetrain,
	master
);



void on_a_pressed() {
	std::cout << "a pressed" << std::endl;
}

void on_a_released() {
	std::cout << "a released" << std::endl;
}
void initialize(){
	ControllerLib::Macro AMacro({pros::E_CONTROLLER_DIGITAL_A}, on_a_pressed, on_a_released, true);
	control.createMacro(AMacro);
}



void disabled() {}
void competition_initialize() {}
void autonomous() {}


void opcontrol() {
	while (true) {
		control.update();
		UILib::update();
		pros::delay(20);
	}
}