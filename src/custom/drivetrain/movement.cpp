#include "custom/drivetrain.hpp"
namespace DrivetrainLib
{
    void Drivetrain::setLeftVelocity(int velocity)
    {
        leftMotors.setVelocity(minmax(static_cast<int>(velocity), -127, 127));
    }
    void Drivetrain::setRightVelocity(int velocity)
    {
        rightMotors.setVelocity(minmax(static_cast<int>(velocity), -127, 127));
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
        stopDrive();
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
            startDrive(dir);

            pros::delay(5);
            elapsedTime += 5;
        }
        stopDrive();
    }
    void Drivetrain::stopDrive()
    {
        leftMotors.stopMove();
        rightMotors.stopMove();
    }
    void Drivetrain::startDrive(DrivetrainEnums::Direction direction)
    {
        leftMotors.startMove(direction);
        rightMotors.startMove(direction);
    }
}