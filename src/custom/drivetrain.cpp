#include "custom/drivetrain.hpp"
namespace DrivetrainLib
{
    Drivetrain::Drivetrain(const std::vector<int8_t> &leftSide, const std::vector<int8_t> &rightSide, double trc, std::optional<pros::Imu *> i) : leftMotors(leftSide), rightMotors(rightSide), trackWidth(trc), imu(i.value_or(nullptr))
    {
        calibrate();
    }
    void Drivetrain::antiDrift()
    {
        while (leftMotors.isMoving() || rightMotors.isMoving())
        {
            double wheelRad = DrivetrainEnums::getWheelDiameter(leftMotors.group[0].getWheelType()) / 2.0;
            double leftDistance = leftMotors.getRotation() * 2 * pi * wheelRad;
            double rightDistance = rightMotors.getRotation() * 2 * pi * wheelRad;
            double drift = leftDistance - rightDistance;

            double correction = drift * 0.5;         // Tune this factor for your robot
            double leftVelocity = 100 - correction;  // Base velocity minus correction
            double rightVelocity = 100 + correction; // Base velocity plus correction

            leftVelocity = minmax<double>(leftVelocity, -127, 127);
            rightVelocity = minmax<double>(rightVelocity, -127, 127);

            leftMotors.setVelocity(leftVelocity);
            rightMotors.setVelocity(rightVelocity);
            pros::delay(10);
        }
    }
    void Drivetrain::calibrate()
    {
        if (imu != nullptr)
        {
            imu->reset(false);
        }
        leftMotors.calibrate();
        rightMotors.calibrate();
        while (imu->is_calibrating())
        {
            pros::delay(20);
        }
    }
    void Drivetrain::setLeftVelocity(int velocity)
    {
        leftMotors.setVelocity(minmax(static_cast<int>(velocity), -127, 127));
    }
    void Drivetrain::setRightVelocity(int velocity)
    {
        rightMotors.setVelocity(minmax(static_cast<int>(velocity), -127, 127));
    }
    double Drivetrain::getHeading()
    {
        double leftRotations = leftMotors.getRotation();
        double rightRotations = rightMotors.getRotation();
        double wheelRad = DrivetrainEnums::getWheelDiameter(leftMotors.group[0].getWheelType()) / 2.0;

        double leftDistance = leftRotations * 2 * pi * wheelRad;
        double rightDistance = rightRotations * 2 * pi * wheelRad;

        double deltaDistance = rightDistance - leftDistance;
        double headingRadians = deltaDistance / trackWidth;
        double headingDegrees = headingRadians * (180.0 / pi);

        if (imu != nullptr)
        {
            double gyroAngle = imu->get_heading();
            return (headingDegrees + gyroAngle) / 2;
        }

        return headingDegrees;
    }
    void Drivetrain::moveDistance(float distance, DrivetrainEnums::Distance dist)
    {
        double wheelRad = DrivetrainEnums::getWheelDiameter(leftMotors.group[0].getWheelType()) / 2.0; // Convert diameter to radius
        double rotations = 0;
        switch (dist)
        {
        case DrivetrainEnums::Distance::INCHES:
            rotations = distance / (2 * pi * wheelRad);
            break;

        case DrivetrainEnums::Distance::MM:
            rotations = (distance / INCH_TO_MM(1)) / (2 * pi * wheelRad);
            break;

        default:
            break;
        }

        leftMotors.moveRelative(rotations);
        rightMotors.moveRelative(rotations);
        antiDrift();
        stop();
    }
    void Drivetrain::setVelocity(int leftVelocity, int rightVelocity)
    {
        setLeftVelocity(leftVelocity);
        setRightVelocity(rightVelocity);
    }
    void Drivetrain::rotateTo(double heading)
    {
        double difference = getHeading() - heading;
        DrivetrainEnums::Direction dir;
        const int MAX_VELOCITY = 127;
        const int MIN_VELOCITY = 20;
        const int TIMEOUT = 5000;
        int elapsedTime = 0;

        while (fabs(difference) > ROTATION_OFFSET_LIMIT && elapsedTime < TIMEOUT)
        {
            difference = getHeading() - heading;
            if (difference < 0)
            {
                dir = DrivetrainEnums::Direction::REVERSE;
            }
            else
            {
                dir = DrivetrainEnums::Direction::FORWARD;
            }
            double percent = fabs(difference) / 360.0;
            double vel = percent * MAX_VELOCITY;

            vel = minmax<double>(vel, MIN_VELOCITY, MAX_VELOCITY);
            setVelocity(vel, -vel);
            drive(dir);

            pros::delay(5);
            elapsedTime += 5;
        }
        stop();
    }
    void Drivetrain::stop()
    {
        leftMotors.brake();
        rightMotors.brake();
    }
    void Drivetrain::drive(DrivetrainEnums::Direction direction)
    {
        leftMotors.move(direction);
        rightMotors.move(direction);
    }
}