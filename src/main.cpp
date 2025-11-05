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
;
	control.buttons.A.OnButtonPress(OnAPress);
	control.buttons.A.OnButtonRelease(OnARelease);
	//control.createMacro(AMaAMacrocro);
	//control.createMacro(AMacro2);
	//control.createMacro(AMacro3);
	int frame = 0;
	while (true) {
		mainControl.update();
		if (frame % 60 == 0) {
			pros::screen::erase();
		}
		frame++;
		pros::delay(20);
	}
}
