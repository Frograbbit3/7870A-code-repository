#include "custom/drivetrain.hpp"
namespace MKV5
{
    void Drivetrain::antiDrift()
    {
        
        while (leftMotors.isMoving() || rightMotors.isMoving())
        {
            double wheelRad = getWheelDiameter(leftMotors.group[0].getWheelType()) / 2.0;
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

    double Drivetrain::getHeading()
    {
        double leftRotations = leftMotors.getRotation();
        double rightRotations = rightMotors.getRotation();
        double wheelRad = getWheelDiameter(leftMotors.group[0].getWheelType()) / 2.0;

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

}