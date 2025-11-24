#pragma once
#include "api.h"
#include "pros/motors.h"
#include <iostream>
#include "mathlib.h"
inline double INCH_TO_MM(double inch) { return 25.4 * inch; }
inline double MM_TO_INCH(double mm) { return mm / 25.4; }
template <typename T>
inline void print(T m) { std::cout << m << std::endl; }

#define RED_MOTOR_CART pros::motor_gearset_e_t::E_MOTOR_GEAR_RED
#define GREEN_MOTOR_CART pros::motor_gearset_e_t::E_MOTOR_GEAR_GREEN
#define BLUE_MOTOR_CART pros::motor_gearset_e_t::E_MOTOR_GEAR_BLUE
#define __VERSION 1.3b
#define VELOCITY_PERCENT MKV5::Enums::VelocityUnit::PERCENT
#define VELOCITY_RAW MKV5::Enums::VelocityUnit::RAW
#define VELOCITY_mV MKV5::Enums::VelocityUnit::mV
#define VELOCITY_VOLTS MKV5::Enums::VelocityUnit::VOLTS
namespace MKV5
{
    namespace Enums
    {
        enum class VelocityUnit
        {
            PERCENT = 0,
            RAW = 1,
            mV = 2,
            VOLTS = 3
        };
        enum class Direction
        {
            FORWARD = 1,
            REVERSE = -1,
            STOP = 0
        };
        enum class Distance
        {
            INCHES = 1,
            MM = 0,
            ROTATION = 2,

        };

        enum class WheelType
        {
            WHEEL_275,
            WHEEL_325,
            WHEEL_400
        };
        enum class ControllerDriveTypes
        {
            DRIVE_MODE_TANK = 1,
            DRIVE_MODE_ARCADE = 2,
            DRIVE_MODE_GTA = 3,
            DRIVE_MODE_CHEESE = 4,
        };
    }

    struct CustomMotor
    {
    private:
        double velocity = 0;

    public:
        pros::Motor *motor;
        Enums::WheelType *wheel = nullptr;
        CustomMotor(pros::Motor *mtr);

        /*Get functions*/

        /*Gets the brake mode of the motor. Returns a pros::MotorBrake object.*/
        pros::MotorBrake getBrakeMode();
        /*Gets the encoding. Defaults to degrees.*/
        pros::MotorEncoderUnits getEncoding();
        /*Gets the current type of wheel connected to this motor.*/
        Enums::WheelType getWheelType();
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
        double getVelocity(Enums::VelocityUnit unit = VELOCITY_RAW);
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
        void setWheelType(Enums::WheelType whl);
        /*Sets the zero of the motor. Pass in no args to use the current motor position.*/
        void setZero(std::optional<double> position = std::nullopt);
        /*Sets the motors velocity. Defaults to a unit of RAW, meaning -127 -> 127*/
        void setVelocity(float vel, Enums::VelocityUnit unit = VELOCITY_RAW);

        /*Sets the encoding of the motor. Defaults to DEGREES.*/
        void setEncoding(pros::MotorEncoderUnits unit);
        /*Movement Functions*/

        /*Moves the motor forever until .brake is called. Pass in an argument if you want to choose a voltage.*/
        void move(Enums::Direction dir, std::optional<int32_t> voltage = std::nullopt);
        /*Moves the motor to a specific position. Pass in a voltage if you want to choose the speed.*/
        void moveAbsolute(Enums::Direction direction, double position,
                          std::optional<int32_t> voltage = std::nullopt);
        /*Moves the motor to an offset based off the current pos. Pass in a voltage if you want to choose the speed.*/
        void moveRelative(Enums::Direction direction, double position,
                          std::optional<int32_t> voltage = std::nullopt);
        /*Stops the motor.*/
        void brake();

        /*OTHER*/

        /*Resets the motor position to 0dg.*/
        void calibrate();
    };
    /*Helper function to get the diameter of a wheel. Defaults to 4.0f. Returns in inches.*/
    double getWheelDiameter(Enums::WheelType type);

    struct ControllerSettings
    {
        Enums::ControllerDriveTypes CONTROL_SCHEME = MKV5::Enums::ControllerDriveTypes::DRIVE_MODE_ARCADE;
        float maxTurnSpeed = -0.6f;
        float maxForwardSpeed = 0.8f;
        bool enabled = true;
        bool useJoystickCurve=false;
    };
}

#define TANK_DRIVE MKV5::Enums::ControllerDriveTypes::DRIVE_MODE_TANK
#define ARCADE_DRIVE MKV5::Enums::ControllerDriveTypes::DRIVE_MODE_ARCADE
#define GTA_DRIVE MKV5::Enums::ControllerDriveTypes::DRIVE_MODE_GTA
#define CHEESE_DRIVE MKV5::Enums::ControllerDriveTypes::DRIVE_MODE_CHEESE
#define DRIVE_FORWARD MKV5::Enums::Direction::FORWARD
#define DRIVE_REVERSE MKV5::Enums::Direction::REVERSE
#define DRIVE_STOP MKV5::Enums::Direction::STOP

// drivetrain
#define DRIVE_SLEW 0.02f
#define CD_NEG_INERTIA_SCALAR 4.0
#define CD_SENSITIVITY 1.0

// distances
#define DIST_MM MKV5::Enums::Distance::MM
#define DIST_INCHES MKV5::Enums::Distance::INCHES
#define DIST_ROTATION MKV5::Enums::Distance::ROTATION