#pragma once
#include "config.h"
#include "mathlib.h"
#include "modded.hpp"
#include "enums.hpp"
#include <math.h>
#define pi M_PI

namespace DrivetrainLib
{
    class Drivetrain
    {
    public:
        MotorGroup leftMotors;
        MotorGroup rightMotors;
        double trackWidth;

    private:
        std::vector<int8_t> leftSide;
        std::vector<int8_t> rightSide;
        pros::Imu* imu;
    public:
        /*Resets all motors & calibrates the gyro.*/
        void calibrate() {
            if (imu!=nullptr) {
                imu->reset(false);
            }
            leftMotors.calibrate();
            rightMotors.calibrate();
            while (imu->is_calibrating()) {
                pros::delay(20);
            }
        }
        /*
            The core drivetrain.
            This drivetrain is primarily used as a replacement to a manual motor system; it supports unlimited motors.
            @param leftSide A list of left motor ports.
            @param rightSide A list of right motor ports.
            @param trc A double with the distance between the left & right wheels in inches.
            @param imu A gyroscope for more accurate rotation.
        */
        Drivetrain(const std::vector<int8_t> &leftSide, const std::vector<int8_t> &rightSide, double trc, std::optional<pros::Imu*> i) 
        : leftMotors(leftSide), rightMotors(rightSide),trackWidth(trc),imu(i.value_or(nullptr)) {calibrate();}

        /*Sets the left velocity to a value between 0 - 100. Does this by attempting to set max voltage.*/
        void setLeftVelocity(int velocity)
        {
            leftMotors.setVelocity(minmax(static_cast<int>(velocity), -127, 127));
        }
        /*Sets the right velocity to a value between 0 - 100. Does this by attempting to set max voltage.*/
        void setRightVelocity(int velocity){
            rightMotors.setVelocity(minmax(static_cast<int>(velocity), -127, 127));
        }

        /*Attempts to get the current heading of the drivetrain. Uses encoders & gyro (if provided)*/ 
        double getHeading()
        {
            double leftRotations = leftMotors.getRotation();
            double rightRotations = rightMotors.getRotation();
            double wheelRad = DrivetrainEnums::getWheelDiameter(leftMotors.group[0].getWheelType()) / 2.0;

            double leftDistance = leftRotations * 2 * pi * wheelRad;
            double rightDistance = rightRotations * 2 * pi * wheelRad;

            double deltaDistance = rightDistance - leftDistance;
            double headingRadians = deltaDistance / trackWidth;
            double headingDegrees = headingRadians * (180.0 / pi);

            return headingDegrees;
        }
    
        /*Attempts to move forward a certain distance. DOES NOT WORK YET!*/
        void moveDistance(float distance, DrivetrainEnums::Distance dist)
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
        }

        /*Attempts to turn to a heading. Not really ready but I want to make a stable API so*/
        void rotateTo(int heading) {
            //pretend i did it
        }
        /*
            Sets the velocity of both sides. An easier way of calling .setLeftVelocity and .setRightVelocity
        */
        void setVelocity(int leftVelocity, int rightVelocity) {
            setLeftVelocity(leftVelocity);
            setRightVelocity(rightVelocity);
        }
        /*
            Stops the robot. This uses the breaks' weird hold method which makes it stop immediately. (currently disa)
        */
        void stop()
        {
            leftMotors.brake();
            rightMotors.brake();
        }
        void drive(DrivetrainEnums::Direction direction)
        {
            leftMotors.move(direction);
            rightMotors.move(direction);
        }
    };
}