#include "main.h"
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
	//DrivetrainLib::Drivetrain drivetrain ({-3,9,10}, {5,12,20}); //put motor ports here
	//ControllerLib::ControlScheme control(
	//	ARCADE_DRIVE,
	//	drivetrain,
	//	master
	//);
	
	ControllerLib::Macro AMacro({pros::E_CONTROLLER_DIGITAL_A}, on_a_pressed, on_a_released, true);
	ControllerLib::Macro AMacro2({pros::E_CONTROLLER_DIGITAL_A, pros::E_CONTROLLER_DIGITAL_B}, on_ab_pressed, on_ab_released, false);
	ControllerLib::Macro AMacro3({pros::E_CONTROLLER_DIGITAL_B}, on_b_pressed, on_b_released, true);
	UILib::Slider velocity(120, 60, -127, 127);
	UILib::Checkbox enabled(60,60,"Enabled"); 


	//control.createMacro(AMacro);
	//control.createMacro(AMacro2);
	//control.createMacro(AMacro3);
	while (true) {
		//control.update();
		std::cout << enabled.checked << std::endl;
		if (!enabled.checked) {
			fly.move(0);
		}else{
			fly.move(velocity.value);
		}
		
		pros::delay(20);
		UILib::update();
	}
}