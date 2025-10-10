#include <main.h>
#include <numeric> 
#include "drivetrain.hpp"





void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Drive");

	pros::Motor bottom_right = pros::Motor(pros::v5::Device(10));
	pros::Motor bottom_left = pros::Motor(pros::v5::Device(3));

	pros::Motor center_right = pros::Motor(pros::v5::Device(5));
	pros::Motor center_left = pros::Motor(pros::v5::Device(9));
	pros::Motor upper_right = pros::Motor(pros::v5::Device(20));
	pros::Motor upper_left = pros::Motor(pros::v5::Device(13));
	
}


template <typename T>
double average_velocity(const std::vector<T>& values) {
    if (values.empty()) return 0.0;
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    return sum / static_cast<double>(values.size());
}
const char* double_to_char(double num) {
    static std::string m; // static keeps it alive after function ends
    m = std::to_string(num);
    return m.c_str();
}

const float dead_zone = 0.1f;

void disabled() {}
void competition_initialize() {}
void autonomous() {}
float MAX_TURN_SPEED = 0.6f;
float MAX_FORWARD_SPEED = 0.8f;


DriveUtils::Drivetrain drive();

void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	std::vector<pros::Motor> leftMotors = {pros::Motor(3),pros::Motor(9),pros::Motor(12)};
	std::vector<pros::Motor> rightMotors = {pros::Motor(10),pros::Motor(5),pros::Motor(20)};
	pros::MotorGroup left_mg({3,9,12});    // Creates a motor group with forwards ports 1 & 3 and reversed port 2
	pros::MotorGroup right_mg({10,5,20});  // Creates a motor group with forwards port 5 and reversed ports 4 & 6	
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


		
		if (abs(dir) < dead_zone && abs(turn) < dead_zone) {
			//Start custom break

		}else{
			left_mg.move(dir - turn );                      // Sets left motor voltage
			right_mg.move(dir + turn);   
		}

        telemetry_counter++;
        if (telemetry_counter >= 10) { // every 200ms
            telemetry_counter = 0;
			current_motor = 0;
			for (const pros::Motor motorObj : leftMotors) {
				motorObj.set_gearing(pros::E_MOTOR_GEAR_600);
				std::cout << "Current Motor: " << current_motor << ", Eff: " << motorObj.get_actual_velocity() << std::endl;
				current_motor += 1;
			}
        }
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
 			MAX_FORWARD_SPEED = 1.0f;
		}else{
			MAX_FORWARD_SPEED = 0.8f;
		}
		pros::delay(20);                               // Run for 20 ms then update
	}
}