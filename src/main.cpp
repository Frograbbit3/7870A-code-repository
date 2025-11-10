#include "main.h"
#include "custom/controls.hpp"
#include "custom/enums.hpp"
#include "pros/screen.hpp"
#include <functional>
#include <numeric> 

/// @brief Variable init
pros::Controller master(pros::E_CONTROLLER_MASTER);

void initialize(){

}



void disabled() {}
void competition_initialize() {}
void autonomous() {}


void OnAPress() {
	pros::screen::print(pros::E_TEXT_MEDIUM, 1, "yes");	
}
void OnARelease() {
	pros::screen::print(pros::E_TEXT_MEDIUM, 1, "noo");
}

void opcontrol() {
	//new comment
	DrivetrainLib::Drivetrain drivetrain ({-1,-5,-20}, {-9,-10,-21}); //put motor ports here
	ControllerLib::EmulatedController control(&master);
	ControllerLib::ControlScheme mainControl(
		ARCADE_DRIVE,
		drivetrain,
		control
	);
	int frame = 0;
	while (true) {
		mainControl.update();
		frame++;
		pros::delay(20);
	}
}
