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
#define VELOCITY_PERCENT DrivetrainEnums::VelocityUnit::PERCENT
#define VELOCITY_RAW DrivetrainEnums::VelocityUnit::RAW
#define VELOCITY_mV DrivetrainEnums::VelocityUnit::mV
#define VELOCITY_VOLTS DrivetrainEnums::VelocityUnit::VOLTS
namespace DrivetrainEnums
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
    
    enum class WheelType {
        WHEEL_275,
        WHEEL_325,
        WHEEL_400
    };

    struct CustomMotor
    {
    private:
        double velocity = 0;

    public:
        pros::Motor *motor;
        WheelType *wheel=nullptr;
        CustomMotor(pros::Motor *mtr) : motor(mtr)
        {
            motor->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
            motor->set_gearing(pros::E_MOTOR_GEAR_200);
        }

        /*Get functions*/

        /*Gets the brake mode of the motor. Returns a pros::MotorBrake object.*/
        pros::MotorBrake getBrakeMode()
        {
            return motor->get_brake_mode();
        }

        /*Gets the encoding. Defaults to degrees.*/
        pros::MotorEncoderUnits getEncoding()
        {
            return motor->get_encoder_units();
        }

        /*Gets the current type of wheel connected to this motor.*/
        WheelType getWheelType() {
            if (wheel) {
                return *wheel;
            }
            return WheelType::WHEEL_400; //fallback
        }

        /*Gets if the motor is moving.*/
        bool getMovement() {
            return abs(motor->get_actual_velocity()) < 1;
        }

        /*Gets the position of the motor. Returns a value in degrees.*/
        int32_t getPosition()
        {
            return motor->get_position();
        }

        /*Gets the current limit of the motor. Returns a value in mA*/
        int32_t getCurrentLimit()
        {
            return motor->get_current_limit();
        }
        /*Gets the motor's gearing. Returns a pros::MotorGears value.*/
        pros::MotorGears getGearing()
        {
            return motor->get_gearing();
        }

        /*Gets the motor's port. Returns an int8_t.*/
        int8_t getPort()
        {
            return motor->get_port();
        }
        /*Gets the motor's voltage limit. Returns a voltage in mV*/
        int32_t getVoltageLimit()
        {
            return motor->get_voltage_limit();
        }
        /*Gets the motor's reversed flag. Returns true if reversed.*/
        bool getReversed()
        {
            return motor->is_reversed();
        }

        /*Gets the velocity. Takes in a VelocityUnit*/
        double getVelocity(VelocityUnit unit = VELOCITY_RAW)
        {
            switch (unit)
            {
            case VELOCITY_RAW:
                return velocity;

            case VELOCITY_mV:
                return (velocity / 12.7) * 1000.0;

            case VELOCITY_VOLTS:
                return (velocity / 127.0) * 12.0;

            case VELOCITY_PERCENT:
                return (velocity / 127.0) * 100.0;

            default:
                return 0.0;
            }
        }
        double getTargetPosition()
        {
            return motor->get_target_position();
        }

        /*Set functions*/

        /*Sets the motor's brake mode.*/
        void setBrakeMode(pros::MotorBrake brake)
        {
            motor->set_brake_mode(brake);
        }
        /*Sets the motor's current limit in mA.*/
        void setCurrentLimit(int32_t current)
        {
            motor->set_current_limit(current);
        }
        /*Sets the motor's voltage limit in mV.*/
        void setVoltageLimit(int32_t voltage)
        {
            motor->set_voltage_limit(voltage);
        }
        /*Sets the motor's gearing.*/
        void setGearing(pros::MotorGears gears)
        {
            motor->set_gearing(gears);
        }

        /*Sets the reversed flag.*/
        void setReversed(bool reversed)
        {
            motor->set_reversed(reversed);
        }

        /*Sets the wheel type of the motor.*/
        void setWheelType(WheelType whl) {
            wheel = &whl;
        }
    
        /*Sets the zero of the motor. Pass in no args to use the current motor position.*/
        void setZero(std::optional<double> position = std::nullopt)
        {
            if (position.has_value())
            {
                motor->set_zero_position(position.value());
            }
            else
            {
                motor->tare_position();
            }
        }
        /*Sets the motors velocity. Defaults to a unit of RAW, meaning -127 -> 127*/
        void setVelocity(float vel, VelocityUnit unit = VELOCITY_RAW)
        {
            switch (unit)
            {
            case VELOCITY_RAW:
                velocity = minmax<double>(vel, -127.0f, 127.0f);
                break;
            case VELOCITY_mV:
                velocity = minmax<double>((vel / 1000.0f) * 12.7f, -127.0f, 127.0f);
                break;
            case VELOCITY_VOLTS:
                velocity = minmax<double>((vel / 12.0f) * 127.0f, -127.0f, 127.0f);
                break;
            case VELOCITY_PERCENT:
                velocity = minmax<double>((vel / 100.0f) * 127.0f, -127.0f, 127.0f);
                break;
            default:
                break;
            };
        }

        /*Sets the encoding of the motor. Defaults to DEGREES.*/
        void setEncoding(pros::MotorEncoderUnits unit) {
            motor->set_encoder_units(unit);
        }

        /*Movement Functions*/

        /*Drives the robot. Pass in an argument if you want to choose a voltage.*/
        void move(Direction dir, std::optional<int32_t> voltage = std::nullopt)
        {
            int mult = (dir == Direction::FORWARD)   ? 1
                       : (dir == Direction::REVERSE) ? -1
                                                     : 0;

            int32_t out = voltage.has_value()
                              ? *voltage * mult
                              : velocity * mult;

            motor->move(out);
        }
        void moveAbsolute(Direction direction, double position,
                          std::optional<int32_t> voltage = std::nullopt)
        {

            int8_t mult =
                (direction == Direction::FORWARD) ? 1 : (direction == Direction::REVERSE) ? -1
                                                                                          : 0;

            int32_t out = voltage.has_value()
                              ? (*voltage) * mult
                              : velocity * mult;

            motor->move_absolute(position, out);
        }

        void moveRelative(Direction direction, double position,
                          std::optional<int32_t> voltage = std::nullopt)
        {

            int8_t mult =
                (direction == Direction::FORWARD) ? 1 : (direction == Direction::REVERSE) ? -1
                                                                                          : 0;

            int32_t out = voltage.has_value()
                              ? (*voltage) * mult
                              : velocity * mult;

            motor->move_relative(position, out);
        }

        /*Stops the motor.*/
        void brake()
        {
            motor->brake();
        }
        

        /*OTHER*/

        /*Resets the motor position to 0dg.*/
        void calibrate() {
            motor->move_voltage(0);
            motor->set_zero_position(0);
        }
    };
    inline double getWheelDiameter(WheelType type) {
        switch (type) {
            case WheelType::WHEEL_275:
                return 2.75f;
            case WheelType::WHEEL_325:
                return 3.25f;
            case WheelType::WHEEL_400:
                return 4.00f;
            default:
                return 3.25f;
        }
    }
} // namespace DrivetrainEnums

// controller stuff

namespace ControllerEnums
{
    enum class ControllerDriveTypes
    {
        DRIVE_MODE_TANK = 1,
        DRIVE_MODE_ARCADE = 2,
        DRIVE_MODE_GTA = 3,
        DRIVE_MODE_CHEESE = 4,
    };

    struct ControllerSettings
    {
        ControllerDriveTypes CONTROL_SCHEME = ControllerEnums::ControllerDriveTypes::DRIVE_MODE_ARCADE;
        float maxTurnSpeed = -0.6f;
        float maxForwardSpeed = 0.8f;
        bool enabled = true;
        double timeSinceJoystickStop = 0.0f;
    };
}



#define TANK_DRIVE ControllerEnums::ControllerDriveTypes::DRIVE_MODE_TANK
#define ARCADE_DRIVE ControllerEnums::ControllerDriveTypes::DRIVE_MODE_ARCADE
#define GTA_DRIVE ControllerEnums::ControllerDriveTypes::DRIVE_MODE_GTA
#define CHEESE_DRIVE ControllerEnums::ControllerDriveTypes::DRIVE_MODE_CHEESE
#define DRIVE_FORWARD DrivetrainEnums::Direction::FORWARD
#define DRIVE_REVERSE DrivetrainEnums::Direction::REVERSE
#define DRIVE_STOP DrivetrainEnums::Direction::STOP

// drivetrain
#define DRIVE_SLEW 0.02f
#define CD_NEG_INERTIA_SCALAR 4.0
#define CD_SENSITIVITY 1.0

// distances
#define DIST_MM DrivetrainEnums::Distance::MM
#define DIST_INCHES DrivetrainEnums::Distance::INCHES
#define DIST_ROTATION DrivetrainEnums::Distance::ROTATION