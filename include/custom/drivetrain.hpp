#pragma once
#include "main.h"
#include "modded.hpp"
#include "motor_testing.hpp"
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
        DrivetrainEnums::DrivetrainSettings configuration;

    private:
        std::vector<int8_t> leftSide;
        std::vector<int8_t> rightSide;
        DrivetrainEnums::MotorProperties leftProperties;
        DrivetrainEnums::MotorProperties rightProperties;

        void internal_telementry_collector()
        {
            float average_vel;
            while (true)
            {
                // left side
                average_vel = 0.0f;
                for (pros::Motor &mtr : leftMotors.group)
                {
                    average_vel += mtr.get_actual_velocity();
                }
                leftProperties.ACTUAL_VELOCITY = average_vel / leftMotors.group.size();

                // right side
                average_vel = 0.0f;
                for (pros::Motor &mtr : rightMotors.group)
                {
                    average_vel += mtr.get_actual_velocity();
                }
                rightProperties.ACTUAL_VELOCITY = average_vel / rightMotors.group.size();
                pros::delay(250);
            }
        }

        void drive_correction()
        { // NOTE TO SELF! Correct this soon!
            float correction = 2.0;
            double leftSideDifference = 0;
            double rightSideDifference = 0;
            while (true)
            {
                if (configuration.AUTO_DRIVE_ENABLED)
                {
                    if (leftProperties.IS_DRIVING && rightProperties.IS_DRIVING)
                    {
                        leftSideDifference = leftProperties.SET_VELOCITY - leftProperties.ACTUAL_VELOCITY;
                        rightSideDifference = rightProperties.SET_VELOCITY - rightProperties.ACTUAL_VELOCITY;
                        if (leftSideDifference - rightSideDifference > correction)
                        {
                            rightProperties.SET_VELOCITY = (leftSideDifference - rightSideDifference) * leftProperties.SET_VELOCITY;
                        }
                        else if (rightSideDifference - leftSideDifference > correction)
                        {
                            leftProperties.SET_VELOCITY = (rightSideDifference - leftSideDifference) * rightProperties.SET_VELOCITY;
                        }
                    }
                    else
                    { // Process breaking as well; makes life easier.
                        leftMotors.update();
                        rightMotors.update();
                    }
                }
                // only correct if both sides are driving

                pros::delay(20);
            }
        }

        static void task_helper_telementry(void *ptr)
        {
            Drivetrain *self = static_cast<Drivetrain *>(ptr);
            self->internal_telementry_collector();
        }
        static void task_helper_drive_correction(void *ptr)
        {
            Drivetrain *self = static_cast<Drivetrain *>(ptr);
            self->drive_correction();
        }
        void calibrateMotors()
        {
            for (pros::Motor &mtr : leftMotors.group)
            {
                mtr.tare_position();
            }
            pros::delay(50);
            for (pros::Motor &mtr : rightMotors.group)
            {
                mtr.tare_position();
            }
            pros::delay(50);
        }

    public:
        Drivetrain(const std::vector<int8_t> &leftSide, const std::vector<int8_t> &rightSide) : leftMotors(leftSide), rightMotors(rightSide)
        {
         //   pros::Task telementry(task_helper_telementry, (void *)this);
         //   pros::Task auto_drive(task_helper_drive_correction, (void *)this);
        }
        void calibrate()
        {
            /*@brief  'Calibrates' the drivetrain. To test, run `DriveUtils::Drivetrain.test()`*/
            calibrateMotors();
        }
        bool test()
        {
            ///@brief Does a test of the drivetrain to confirm all motors are spinning and optimal. Can take a while.
            calibrateMotors();
            for (pros::Motor &mtr : leftMotors.group)
            {
                if (!testMotor(mtr))
                {
                    return false;
                }
            }
            pros::delay(50);
            for (pros::Motor &mtr : rightMotors.group)
            {
                if (!testMotor(mtr))
                {
                    return false;
                }
            }
            pros::delay(50);
            calibrateMotors();
            return true;
        }
        void setLeftVelocity(int velocity)
        {
            /// Sets the left velocity to a value between 0 - 100. Does this by attempting to set max voltage.
            leftProperties.SET_VELOCITY = std::min(127, std::max(-127, velocity));
        }
        void setRightVelocity(int velocity)
        {
            /// Sets the right velocity to a value between 0 - 100. Does this by attempting to set max voltage.
            rightProperties.SET_VELOCITY = std::min(127, std::max(-127, velocity));
        }

        void stop()
        {
            leftProperties.SET_VELOCITY = 0;
            rightProperties.SET_VELOCITY = 0;
            leftMotors.brake();
            rightMotors.brake();
            leftProperties.IS_DRIVING = false;
            rightProperties.IS_DRIVING = false;
            /// Breaks the robot. This is the same as calling DriveUtils::Drivetrain::Drive(DrivetrainEnums::Direction::STOP)
        }
        void drive(DrivetrainEnums::Direction direction)
        {
            /// Moves the robot. Simple.
            switch (direction)
            {
            case DrivetrainEnums::Direction::FORWARD:
                leftMotors.move(leftProperties.SET_VELOCITY);
                rightMotors.move(rightProperties.SET_VELOCITY);
                leftProperties.IS_DRIVING = true;
                rightProperties.IS_DRIVING = true;
                break;
            case DrivetrainEnums::Direction::REVERSE:
                leftMotors.move(leftProperties.SET_VELOCITY * -1);
                rightMotors.move(rightProperties.SET_VELOCITY * -1);
                leftProperties.IS_DRIVING = true;
                rightProperties.IS_DRIVING = true;
                break;
            }
        }
    };
}