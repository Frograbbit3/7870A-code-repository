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



void initialize(){
	control.configuration.CONTROL_SCHEME = ARCADE_DRIVE;
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