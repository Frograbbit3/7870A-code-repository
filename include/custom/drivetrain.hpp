#pragma once
#include "custom/enums.hpp"
#include "modded.hpp"
#include "pros/abstract_motor.hpp"
#include <limits>
#include <vector>
constexpr double pi = M_PI;
constexpr long long M_MAX = std::numeric_limits<long long>::max();
namespace MKV5
{
    class Drivetrain
    {
    public:
        MotorGroup leftMotors;
        MotorGroup rightMotors;
        double trackWidth;
        int isDriving = 0;

    private:
        std::vector<int8_t> leftSide;
        std::vector<int8_t> rightSide;
        pros::Imu *imu;
        double gearRatio;
        void antiDrift();
        double stationaryRate;
        long long timeout = M_MAX;//effeci

    static void _drivetrain_tick(void* drive);

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
            @param gearRatio a fraction / decimal ratio used primarily for rotation and driveDistance. Pass 1 in if you don't have a gear ratio.
        */
        Drivetrain(const std::vector<int8_t> &leftSide, const std::vector<int8_t> &rightSide, Units::Distance trc, double gearRatio, std::optional<pros::Imu *> i);

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
        void moveDistance(MKV5::Units::Distance distance, std::optional<MKV5::Units::DirectionUnit> direction =  std::nullopt, bool block = true);

        /*
            Sets the velocity of both sides. An easier way of calling .setLeftVelocity and .setRightVelocity.
            @param leftVelocity The left side velocity.
            @param rightVelocity The right side velocity.
        */
        void setVelocity(int leftVelocity, int rightVelocity);

        /*
            Attempts to turn to a heading. Uses gyro.
            @param heading A double of the degree you want to turn two. Accuracy is ~2dg by default.
            @param direction Allows manual override of a direction.
            @example
        */
        void rotateTo(MKV5::Units::Angle heading, MKV5::Units::RotationUnit direction = MKV5::Units::RotationUnit::AUTO);

        /*
            Turns a certain amount of degrees relative to the current heading.
            @param heading The angle to turn
            @param direction Allows manual override of a direction.
        */
        void turn(MKV5::Units::Angle heading, MKV5::Units::RotationUnit direction = MKV5::Units::RotationUnit::AUTO);
        /*
            Stops the robot.
        */
        void stop(pros::MotorBrake brakeType = pros::MotorBrake::brake);
        /*
            Moves the robot in a direction.
            @param direction Direction to move.
        */
        void move(MKV5::Units::DirectionUnit direction);
        /*
            Checks if the robot is currently moving based on the gyro.
            @return Returns true if no gyro is installed
        */
        bool isStationary();
        /*
            Waits until isStationary returns true.Immediately continues if a gyro is not found.
            @param timeout The amount of time to wait before continuing regardless
            @return A bool on if it failed or not.
        */
        bool waitForStationary(int timeout = 50);
        /*
            Resets the drivetrain / gyro heading.
            @param heading the new heading
        */
        void resetHeading(Units::Angle heading = {0});
        /*
            Sets the max amount of time any one drive call can last.
            @param timeout Passing 0 or a negative into this call will remove the timeout.
        */
        void setTimeout(int timeout);

        /*
            Gets the port of motors flagged as overheating.
            @returns an int from 0-<count of motors>
        */
        std::vector<uint8_t> getOverheatingMotors();
        /*
            Checks if the drivetrain is overheating. (If ANY motor is overheating, this will be true!)
        */
        bool isOverheating();
    };
}
