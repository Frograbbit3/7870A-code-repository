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


void on_triggers_hit() {

}


void calibrate_drivetrain_button() {
	pros::lcd::set_text(2, "Calibrating drivetrain.");
	drivetrain.calibrate();
	pros::lcd::set_text(2, "");
}

void test_drivetrain_button() {
	pros::lcd::set_text(2, "Testing drivetrain. This may take a while.");
	drivetrain.test();
	pros::lcd::set_text(2, "");
}
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Debug Menu");
	pros::lcd::register_btn0_cb(calibrate_drivetrain_button);
	pros::lcd::register_btn1_cb(test_drivetrain_button);
	calibrate_drivetrain_button();
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