#pragma once
#include "api.h"
#include "config.h"
#include "mathlib.h"
#include "pros/motors.h"
#include <iostream>
#include <variant>
inline double INCH_TO_MM(double inch) { return 25.4 * inch; }
inline double MM_TO_INCH(double mm) { return mm / 25.4; }
template <typename T> inline void print(T m) { std::cout << m << std::endl; }

#define RED_MOTOR_CART pros::motor_gearset_e_t::E_MOTOR_GEAR_RED
#define GREEN_MOTOR_CART pros::motor_gearset_e_t::E_MOTOR_GEAR_GREEN
#define BLUE_MOTOR_CART pros::motor_gearset_e_t::E_MOTOR_GEAR_BLUE
#define __VERSION 1.3b
namespace MKV5 {
namespace Units {

enum class RotationUnit { CLOCKWISE = 1, COUNTER_CLOCK = 2, AUTO = 3 };
struct Distance {
	double meters;

	// accessors
	constexpr double m() const { return meters; }

	constexpr double mm() const { return meters * 1000.0; }

	constexpr double cm() const { return meters * 100.0; }

	constexpr double inches() const { return meters / 0.0254; }
};

constexpr Distance operator""_mm(unsigned long long v) {
	return Distance{static_cast<double>(v) / 1000.0};
}

constexpr Distance operator""_mm(long double v) {
	return Distance{static_cast<double>(v) / 1000.0};
}

constexpr Distance operator""_cm(unsigned long long v) {
	return Distance{static_cast<double>(v) / 100.0};
}

constexpr Distance operator""_cm(long double v) {
	return Distance{static_cast<double>(v) / 100.0};
}

constexpr Distance operator""_in(unsigned long long v) {
	return Distance{static_cast<double>(v) * 0.0254};
}

constexpr Distance operator""_in(long double v) {
	return Distance{static_cast<double>(v) * 0.0254};
}
constexpr Distance operator-(Distance v) { return Distance{-v.meters}; }
struct Angle {
	double degrees;

	// accessors
	constexpr double rad() const { return (M_PI / 180) * degrees; }

	constexpr double dg() const { return degrees; }
};

constexpr Angle operator""_dg(unsigned long long v) {
	return Angle{static_cast<double>(v)};
}

constexpr Angle operator""_rad(unsigned long long v) {
	return Angle{static_cast<double>(v) * (180 / M_PI)};
}

constexpr Angle operator-(Angle v) { return Angle{-v.degrees}; }

constexpr uint32_t operator""_s(unsigned long long v) { return v * 1000; }

constexpr uint32_t operator""_ms(unsigned long long v) { return v; }

constexpr uint32_t operator""_s(long double v) { return v * 1000; }

constexpr uint32_t operator""_ms(long double v) { return v; }
enum class VelocityUnit { PERCENT = 0, RAW = 1, mV = 2, VOLTS = 3 };
enum class DirectionUnit { FORWARD = 1, REVERSE = -1, STOP = 0 };
enum class DistanceUnit {
	INCHES = 1,
	MM = 0,
	ROTATION = 2,

};
enum class WheelUnit { WHEEL_275, WHEEL_325, WHEEL_400 };
enum class ControllerDriveTypeUnit {
	DRIVE_MODE_TANK = 1,
	DRIVE_MODE_ARCADE = 2,
	DRIVE_MODE_GTA = 3,
	DRIVE_MODE_CHEESE = 4,
};

} // namespace Units

struct CustomMotor {
      private:
	double velocity = 0;

      public:
	pros::Motor motor;
	Units::WheelUnit *wheel = nullptr;
	CustomMotor(int prt);

	/*Get functions*/

	/*Gets the brake mode of the motor. Returns a pros::MotorBrake object.*/
	pros::MotorBrake getBrakeMode();
	/*Gets the encoding. Defaults to degrees.*/
	pros::MotorEncoderUnits getEncoding();
	/*Gets the current type of wheel connected to this motor.*/
	Units::WheelUnit getWheelType();
	/*Gets if the motor is moving.*/
	bool getMovement();
	/*Gets the position of the motor. Returns a value in degrees.*/
	int32_t getPosition();
	/*Gets the current limit of the motor. Returns a value in mA*/
	int32_t getCurrentLimit();
	/*Gets the motor's gearing. Returns a pros::MotorGears value.*/
	pros::MotorGears getGearing();
	/*Gets the motor's port. Returns an int8_t.*/
	int8_t getPort();
	/*Gets the motor's voltage limit. Returns a voltage in mV*/
	int32_t getVoltageLimit();
	/*Gets the motor's reversed flag. Returns true if reversed.*/
	bool getReversed();
	/*Gets the velocity. Takes in a VelocityUnit*/
	double getVelocity(Units::VelocityUnit unit = Units::VelocityUnit::RAW);
	/*Gets the position the motor is *attempting* to turn to.*/
	double getTargetPosition();
	/*Set functions*/

	/*Sets the motor's brake mode.*/
	void setBrakeMode(pros::MotorBrake brake);
	/*Sets the motor's current limit in mA.*/
	void setCurrentLimit(int32_t current);
	/*Sets the motor's voltage limit in mV.*/
	void setVoltageLimit(int32_t voltage);
	/*Sets the motor's gearing.*/
	void setGearing(pros::MotorGears gears);
	/*Sets the reversed flag.*/
	void setReversed(bool reversed);
	/*Sets the wheel type of the motor.*/
	void setWheelType(Units::WheelUnit whl);
	/*Sets the zero of the motor. Pass in no args to use the current motor
	 * position.*/
	void setZero(std::optional<double> position = std::nullopt);
	/*Sets the motors velocity. Defaults to a unit of RAW, meaning -127 ->
	 * 127*/
	void setVelocity(float vel,
	                 Units::VelocityUnit unit = Units::VelocityUnit::RAW);

	/*Sets the encoding of the motor. Defaults to DEGREES.*/
	void setEncoding(pros::MotorEncoderUnits unit);
	/*Movement Functions*/

	/*Moves the motor forever until .brake is called. Pass in an argument if
	 * you want to choose a voltage.*/
	void spin(Units::DirectionUnit dir = Units::DirectionUnit::FORWARD,
	          std::optional<int32_t> voltage = std::nullopt);
	/*Moves the motor to a specific position. Pass in a voltage if you want
	 * to choose the speed.*/
	void spinAbsolute(Units::DirectionUnit direction, double position,
	                  std::optional<int32_t> voltage = std::nullopt);
	/*Moves the motor to an offset based off the current pos. Pass in a
	 * voltage if you want to choose the speed.*/
	void spinRelative(Units::DirectionUnit direction, double position, bool block=true);
	/*Stops the motor.*/
	void brake();

	/*OTHER*/

	/*Resets the motor position to 0dg.*/
	void calibrate();
};
/*Helper function to get the diameter of a wheel. Defaults to 4.0f. Returns in
 * inches.*/
double getWheelDiameter(Units::WheelUnit type);

struct ControllerSettings {
	Units::ControllerDriveTypeUnit CONTROL_SCHEME =
	    Units::ControllerDriveTypeUnit::DRIVE_MODE_ARCADE;
	float maxTurnSpeed = 0.8f;
	float maxForwardSpeed = 0.8f;
	bool enabled = true;
	bool useJoystickCurve = false;
};
} // namespace MKV5

// Drive modes
constexpr auto TANK_DRIVE =
    MKV5::Units::ControllerDriveTypeUnit::DRIVE_MODE_TANK;
constexpr auto ARCADE_DRIVE =
    MKV5::Units::ControllerDriveTypeUnit::DRIVE_MODE_ARCADE;
constexpr auto GTA_DRIVE = MKV5::Units::ControllerDriveTypeUnit::DRIVE_MODE_GTA;
constexpr auto CHEESE_DRIVE =
    MKV5::Units::ControllerDriveTypeUnit::DRIVE_MODE_CHEESE;

// Directions
#ifdef SHORTHAND_DIRECTIONALS
constexpr auto FORWARD = MKV5::Units::DirectionUnit::FORWARD;
constexpr auto REVERSE = MKV5::Units::DirectionUnit::REVERSE;
constexpr auto STOP = MKV5::Units::DirectionUnit::STOP;

// This was wrong before (SPIN_FORWARD = STOP??), but keeping semantics
constexpr auto SPIN_FORWARD = MKV5::Units::DirectionUnit::FORWARD;
constexpr auto SPIN_REVERSE = MKV5::Units::DirectionUnit::REVERSE;

// distances, outdated now but ill keep it
constexpr auto DIST_MM = MKV5::Units::DistanceUnit::MM;
constexpr auto DIST_INCHES = MKV5::Units::DistanceUnit::INCHES;
constexpr auto DIST_ROTATION = MKV5::Units::DistanceUnit::ROTATION;

constexpr auto COUNTER_CLOCKWISE = MKV5::Units::RotationUnit::COUNTER_CLOCK;
constexpr auto CLOCKWISE = MKV5::Units::RotationUnit::CLOCKWISE;
#endif
// drivetrain
constexpr float DRIVE_SLEW = 0.02f;
constexpr double CD_NEG_INERTIA_SCALAR = 4.0;
constexpr double CD_SENSITIVITY = 1.0;

// distances
