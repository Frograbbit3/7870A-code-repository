#include "main.h"
#include <numeric> 

/// @brief Variable init
pros::Controller master(pros::E_CONTROLLER_MASTER);
DriveUtils::Drivetrain drivetrain ({3,9,12}, {10,5,20}); //put motor ports here
ControllerLib::ControlScheme control(
	ControllerLib::ControllerEnums::DRIVE_MODE_ARCADE,
	drivetrain,
	master
);


void createMessageBox() {
	UILib::MessageBox msg("Example", "This is an example message box.", UILib::MessageBoxType::OK_BOX);
}

void initialize() {
	control.createMacro({DIGITAL_A, DIGITAL_L1}, createMessageBox);
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