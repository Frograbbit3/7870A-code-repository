#include "main.h"
#include "custom/controls.hpp"
#include "custom/enums.hpp"
#include <numeric> 

/// @brief Variable init
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor fly(20);

void on_a_pressed() {
	std::cout << "A PRESSED" << std::endl;
	fly.move(127);
}

void on_a_released() {
	std::cout << "A RELEASED" << std::endl;
	fly.move(0);
}

void on_ab_pressed() {
	std::cout << "AB PRESSED" << std::endl;
	
}

void on_ab_released() {
	std::cout << "AB RELEASED" << std::endl;
}


void on_b_pressed() {
	std::cout << "B PRESSED" << std::endl;
	fly.move(-127);
}

void on_b_released() {
	std::cout << "B RELEASED" << std::endl;
	fly.move(0);
}

void initialize(){

}



void disabled() {}
void competition_initialize() {}
void autonomous() {}


void opcontrol() {
	//new comment
	DrivetrainLib::Drivetrain drivetrain ({-3,9,10}, {5,12,20}); //put motor ports here
	//ControllerLib::ControlScheme control(
	//	ARCADE_DRIVE,
	//	drivetrain,
	//	master

	ControllerLib::EmulatedController control(&master);
	ControllerLib::ControlScheme mainControl(
		ARCADE_DRIVE,
		drivetrain,
		control
	);

	//control.createMacro(AMacro);
	//control.createMacro(AMacro2);
	//control.createMacro(AMacro3);
	while (true) {
		mainControl.update();
		pros::delay(20);
	}
}
