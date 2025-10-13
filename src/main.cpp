#include "main.h"
#include <numeric> 
#include "drivetrain.hpp"
#include "enums.hpp"




DriveUtils::Drivetrain drivetrain ({3,9,12}, {10,5,20});

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



const float dead_zone = 0.1f;
void disabled() {}
void competition_initialize() {}
void autonomous() {}
float MAX_TURN_SPEED = 0.6f;
float MAX_FORWARD_SPEED = 0.8f;




void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	int current_motor = 0;
	double left_motor_real = 0.0f;
	int telemetry_counter = 0;
	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

		// Arcade control scheme
		int dir = master.get_analog(ANALOG_RIGHT_X) * -MAX_TURN_SPEED;    // Gets amount forward/backward from left joystick
		int turn = master.get_analog(ANALOG_LEFT_Y) * MAX_FORWARD_SPEED;  // Gets the turn left/right from right joystick


		
		if (abs(dir) > dead_zone || abs(turn) > dead_zone) {
			drivetrain.setLeftVelocity(dir - turn);                      // Sets left motor voltage
			drivetrain.setRightVelocity(dir + turn);
			drivetrain.drive(DrivetrainEnums::Direction::FORWARD);
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
 			MAX_FORWARD_SPEED = 1.0f;
		}else{
			MAX_FORWARD_SPEED = 0.8f;
		}
		pros::delay(20);                               // Run for 20 ms then update
	}
}