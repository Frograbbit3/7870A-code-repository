#include "custom/drivetrain.hpp"
#include "config.h"
#include <algorithm>
#include <cstdlib>
namespace MKV5
{
void Drivetrain::antiDrift() {
    double lastLeft = leftMotors.getRotation();
    double lastRight = rightMotors.getRotation();

    const double kP = 0.4;
    const double deadband = 0.01;

    while (leftMotors.isMoving() && rightMotors.isMoving()) {
        double leftNow = leftMotors.getRotation();
        double rightNow = rightMotors.getRotation();

        double dLeft = leftNow - lastLeft;
        double dRight = rightNow - lastRight;

        lastLeft = leftNow;
        lastRight = rightNow;

        double drift = dLeft - dRight;

        if (std::abs(drift) < deadband) {
            pros::delay(10);
            continue;
        }

        double correction = drift * kP;

        double leftVel = 100 - correction;
        double rightVel = 100 + correction;


        leftVel  = minmax(leftVel,  -127.0, 127.0);
        rightVel = minmax(rightVel, -127.0, 127.0);

        if (abs(leftVel) > 10 && abs(rightVel) > 10) {
            leftMotors.setVelocity(leftVel);
            rightMotors.setVelocity(rightVel);

        }
        pros::delay(10);
    }
}


    double Drivetrain::getHeading()
    {


        if (imu != nullptr) //use the imu if it's there
        {
            double gyroAngle = imu->get_heading();
            
            return (gyroAngle);
        }

        double leftRotations = leftMotors.getRotation();
        double rightRotations = rightMotors.getRotation();
        double wheelRad = getWheelDiameter(leftMotors.group[0].getWheelType()) / 2.0;

        double leftDistance = leftRotations * D_PI * wheelRad;
        double rightDistance = rightRotations * D_PI * wheelRad;

        double deltaDistance = rightDistance - leftDistance;
        double headingRadians = deltaDistance / trackWidth;
        double headingDegrees = headingRadians * (180.0 / pi);
        return headingDegrees;
    }

}