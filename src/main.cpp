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

bool drawingEnabled = false;
inline void enableDrawing() {FlappyBird::start();master.rumble("."); drawingEnabled=!drawingEnabled;}

void initialize(){
	control.createMacro({DIGITAL_A, DIGITAL_L1, DIGITAL_R1, DIGITAL_LEFT}, enableDrawing);
}



void disabled() {}
void competition_initialize() {}
void autonomous() {}


void opcontrol() {
	while (true) {
		if (drawingEnabled) {
			FlappyBird::update();
		}
		control.update();
		UILib::update();
		pros::delay(20);
	}
}