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
        void antiDrift() {
            while (leftMotors.isMoving() || rightMotors.isMoving())
            {
                double wheelRad = DrivetrainEnums::getWheelDiameter(leftMotors.group[0].getWheelType()) / 2.0;
                double leftDistance = leftMotors.getRotation() * 2 * pi * wheelRad;
                double rightDistance = rightMotors.getRotation() * 2 * pi * wheelRad;
                double drift = leftDistance - rightDistance;

                double correction = drift * 0.5; // Tune this factor for your robot
                double leftVelocity = 100 - correction;  // Base velocity minus correction
                double rightVelocity = 100 + correction; // Base velocity plus correction

                leftVelocity = minmax<double>(leftVelocity, -127, 127);
                rightVelocity = minmax<double>(rightVelocity, -127, 127);

                leftMotors.setVelocity(leftVelocity);
                rightMotors.setVelocity(rightVelocity);
                pros::delay(10);
            }
        }
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

        /*Attempts to get the current heading of the drivetrain. Uses encoders & gyro (if provided). */ 
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

            if (imu != nullptr) {
                double gyroAngle = imu->get_heading();
                return (headingDegrees + gyroAngle) / 2;
            }

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
            antiDrift();
        }
        /*
            Sets the velocity of both sides. An easier way of calling .setLeftVelocity and .setRightVelocity
        */
        void setVelocity(int leftVelocity, int rightVelocity) {
            setLeftVelocity(leftVelocity);
            setRightVelocity(rightVelocity);
        }

        /*Attempts to turn to a heading. Uses gyro.*/
        void rotateTo(double heading) {
            double difference = getHeading() - heading;
            DrivetrainEnums::Direction dir;
            const int MAX_VELOCITY = 127;
            const int MIN_VELOCITY = 20; 
            const int TIMEOUT = 5000;     
            int elapsedTime = 0;

            while (fabs(difference) > ROTATION_OFFSET_LIMIT && elapsedTime < TIMEOUT) {
                difference = getHeading() - heading;
                if (difference < 0) {
                    dir = DrivetrainEnums::Direction::REVERSE;
                } else {
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