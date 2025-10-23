#include "main.h"
#include <numeric> 

/// @brief Variable init
pros::Controller master(pros::E_CONTROLLER_MASTER);


void initialize(){
	//ControllerLib::Macro AMacro({pros::E_CONTROLLER_DIGITAL_A}, on_a_pressed, on_a_released, true);
	//Wcontrol.createMacro(AMacro);

}



void disabled() {}
void competition_initialize() {}
void autonomous() {}


void opcontrol() {
	DrivetrainLib::Drivetrain drivetrain ({-3,9,10}, {5,12,20}); //put motor ports here
	ControllerLib::ControlScheme control(
		ARCADE_DRIVE,
		drivetrain,
		master
	);
	while (true) {
		control.update();
		pros::delay(20);
	}
}