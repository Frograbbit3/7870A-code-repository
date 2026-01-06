#pragma once
#include "custom/enums.hpp"
#include "modded.hpp"
#define pi M_PI

namespace MKV5
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
        pros::Imu *imu;
        void antiDrift();

    public:
        /*Resets all motors & calibrates the gyro.*/
        void doCalibration();
        /*
            The core drivetrain.
            This drivetrain is primarily used as a replacement to a manual motor system; it supports unlimited motors.
            @param leftSide A list of left motor ports.
            @param rightSide A list of right motor ports.
            @param trc A double with the distance between the left & right wheels in inches.
            @param imu A gyroscope for more accurate rotation.
        */
        Drivetrain(const std::vector<int8_t> &leftSide, const std::vector<int8_t> &rightSide, double trc, std::optional<pros::Imu *> i);

        /*
            Sets the left side velocity.
            @param velocity A value from 0 - 100.
        */
        void setLeftVelocity(int velocity);
        /*
            Sets the right side velocity.
            @param velocity A value from 0 - 100.
        */
        void setRightVelocity(int velocity);

        /*
            Attempts to get the current heading of the drivetrain. Uses encoders & gyro (if provided).
            @return A double with the approximate heading.
        */
        double getHeading();

        /*
            Attempts to move forward a certain distance.
            @param distance A float containing a distance.
            @param dist The unit for the distance.
        */
        void moveDistance(MKV5::Units::Distance distance);

        /*
            Sets the velocity of both sides. An easier way of calling .setLeftVelocity and .setRightVelocity.
            @param leftVelocity The left side velocity.
            @param rightVelocity The right side velocity.
        */
        void setVelocity(int leftVelocity, int rightVelocity);

        /*
            Attempts to turn to a heading. Uses gyro.
            @param heading A double of the degree you want to turn two. Accuracy is ~2dg by default.
            @example
        */
        void rotateTo(MKV5::Units::Angle heading);

        /*
            Stops the robot.
        */
        void stopDrive();
        /*
            Moves the robot in a direction.
            @param direction Direction to move.
        */
        void startDrive(MKV5::Enums::Direction direction);
    };
}