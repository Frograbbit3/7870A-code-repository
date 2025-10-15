#include "main.h"
#include <numeric> 
#include "pros/apix.h"

/// @brief Variable init
pros::Controller master(pros::E_CONTROLLER_MASTER);
DriveUtils::Drivetrain drivetrain ({3,9,12}, {10,5,20}); //put motor ports here
ControllerLib::ControlScheme control(
	ControllerLib::ControllerEnums::DRIVE_MODE_ARCADE,
	drivetrain,
	master
);



int count = 0;
void on_triggers_hit() {
	count++;
	master.print(0,0,std::to_string(count).c_str());
};

void a_b() {
	std::cout << "a+b hit" << std::endl;
}
void a() {
	std::cout << "just a" << std::endl;
}

void initialize() {

	control.createMacro({DIGITAL_A, DIGITAL_B}, a_b);
	control.createMacro({DIGITAL_A}, a);
	control.createMacro({pros::E_CONTROLLER_DIGITAL_L1,pros::E_CONTROLLER_DIGITAL_L2,pros::E_CONTROLLER_DIGITAL_R1,pros::E_CONTROLLER_DIGITAL_L2}, on_triggers_hit);
	
}




void disabled() {}
void competition_initialize() {}
void autonomous() {}


void opcontrol() {
	while (true) {
		control.update();
		pros::delay(20);
	}
}