#include "custom/enums.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/motors.hpp"
#include <variant>
namespace MKV5
{

    // constructor
    CustomMotor::CustomMotor(int prt): motor(pros::Motor(prt))
    {
        motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
        motor.set_gearing(pros::E_MOTOR_GEAR_200);
        motor.set_brake_mode(pros::MotorBrake::coast);
    }

    // getters
    pros::MotorBrake CustomMotor::getBrakeMode()
    {
        return motor.get_brake_mode();
    }

    pros::MotorEncoderUnits CustomMotor::getEncoding()
    {
        return motor.get_encoder_units();
    }

    Units::WheelUnit CustomMotor::getWheelType()
    {
        if (wheel)
            return *wheel;
        return Units::WheelUnit::WHEEL_400;
    }

    bool CustomMotor::getMovement()
    {
        return abs(motor.get_actual_velocity()) < 1;
    }

    int32_t CustomMotor::getPosition()
    {
        return motor.get_position();
    }

    int32_t CustomMotor::getCurrentLimit()
    {
        return motor.get_current_limit();
    }

    pros::MotorGears CustomMotor::getGearing()
    {
        return motor.get_gearing();
    }

    int8_t CustomMotor::getPort()
    {
        return motor.get_port();
    }

    int32_t CustomMotor::getVoltageLimit()
    {
        return motor.get_voltage_limit();
    }

    bool CustomMotor::getReversed()
    {
        return motor.is_reversed();
    }

    double CustomMotor::getVelocity(Units::VelocityUnit unit)
    {
        switch (unit)
        {
        case Units::VelocityUnit::RAW:
            return velocity;
        case Units::VelocityUnit::mV:
            return (velocity / 12.7) * 1000.0;
        case Units::VelocityUnit::VOLTS:
            return (velocity / 127.0) * 12.0;
        case Units::VelocityUnit::PERCENT:
            return (velocity / 127.0) * 100.0;
        default:
            return 0.0;
        }
    }

    double CustomMotor::getTargetPosition()
    {
        return motor.get_target_position();
    }

    // setters
    void CustomMotor::setBrakeMode(pros::MotorBrake brake)
    {
        motor.set_brake_mode(brake);
    }

    void CustomMotor::setCurrentLimit(int32_t current)
    {
        motor.set_current_limit(current);
    }

    void CustomMotor::setVoltageLimit(int32_t voltage)
    {
        motor.set_voltage_limit(voltage);
    }

    void CustomMotor::setGearing(pros::MotorGears gears)
    {
        motor.set_gearing(gears);
    }

    void CustomMotor::setReversed(bool reversed)
    {
        motor.set_reversed(reversed);
    }

    void CustomMotor::setWheelType(Units::WheelUnit whl)
    {
        wheel = &whl;
    }

    void CustomMotor::setZero(std::optional<double> position)
    {
        if (position.has_value())
            motor.set_zero_position(position.value());
        else
            motor.tare_position();
    }

    void CustomMotor::setVelocity(float vel, Units::VelocityUnit unit)
    {
        switch (unit)
        {
        case Units::VelocityUnit::RAW:
            velocity = minmax<double>(vel, -127.0f, 127.0f);
            break;
        case Units::VelocityUnit::mV:
            velocity = minmax<double>((vel / 1000.0f) * 12.7f, -127.0f, 127.0f);
            break;
        case Units::VelocityUnit::VOLTS:
            velocity = minmax<double>((vel / 12.0f) * 127.0f, -127.0f, 127.0f);
            break;
        case Units::VelocityUnit::PERCENT:
            velocity = minmax<double>((vel / 100.0f) * 127.0f, -127.0f, 127.0f);
            break;
        default:
            break;
        }
    }

    void CustomMotor::setEncoding(pros::MotorEncoderUnits unit)
    {
        motor.set_encoder_units(unit);
    }

    // movement
    void CustomMotor::move(Units::DirectionUnit dir, std::optional<int32_t> voltage)
    {
        int mult = (dir == Units::DirectionUnit::FORWARD) ? 1 : (dir == Units::DirectionUnit::REVERSE) ? -1
                                                                                 : 0;

        int32_t out = voltage.has_value()
                          ? *voltage * mult
                          : velocity * mult;

        motor.move(out);
    }

    void CustomMotor::moveAbsolute(Units::DirectionUnit direction, double position,
                                   std::optional<int32_t> voltage)
    {
        int8_t mult = (direction == Units::DirectionUnit::FORWARD) ? 1 : (direction == Units::DirectionUnit::REVERSE) ? -1
                                                                                                : 0;

        int32_t out = voltage.has_value()
                          ? (*voltage) * mult
                          : velocity * mult;

        motor.move_absolute(position, out);
    }

    void CustomMotor::moveRelative(Units::DirectionUnit direction, double position)
    {
        motor.move_relative(position, velocity);
        double target = motor.get_position() + position;

        // Block until close enough
        while (true) {
            double current = motor.get_position();
            
            // Tunable threshold
            if (fabs(target - current) < 5) {
                break;
            }

            pros::delay(10);
        }
    }
    void CustomMotor::spin(Units::DirectionUnit direction) {
        move(direction, 127);
    }
    void CustomMotor::brake()
    {
        motor.brake();
    }
    

    // misc
    void CustomMotor::calibrate()
    {
        motor.move_voltage(0);
        motor.set_zero_position(0);
    }

} // namespace DrivetrainEnums
