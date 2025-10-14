#include "main.h"
#include <numeric> 



/// @brief Variable init
constexpr float dead_zone = 0.1f;
constexpr float MAX_TURN_SPEED = 0.6f;
constexpr float MAX_FORWARD_SPEED = 0.8f;
DriveUtils::Drivetrain drivetrain ({3,9,12}, {10,5,20}); //put motor ports here

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
	pros::Motor bottom_right = pros::Motor(pros::v5::Device(10));
	pros::Motor bottom_left = pros::Motor(pros::v5::Device(3));

	pros::Motor center_right = pros::Motor(pros::v5::Device(5));
	pros::Motor center_left = pros::Motor(pros::v5::Device(9));
	pros::Motor upper_right = pros::Motor(pros::v5::Device(20));
	pros::Motor upper_left = pros::Motor(pros::v5::Device(13));
	calibrate_drivetrain_button();
	
}




void disabled() {}
void competition_initialize() {}
void autonomous() {}


void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	while (true) {
		int dir = master.get_analog(ANALOG_RIGHT_X) * -MAX_TURN_SPEED;   
		int turn = master.get_analog(ANALOG_LEFT_Y) * MAX_FORWARD_SPEED;
		if (abs(dir) > dead_zone || abs(turn) > dead_zone) {
			drivetrain.setLeftVelocity(dir - turn); 
			drivetrain.setRightVelocity(dir + turn);
			drivetrain.drive(DrivetrainEnums::Direction::FORWARD);
		}
		pros::delay(20);
	}
}