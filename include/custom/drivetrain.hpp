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

    private:
        std::vector<int8_t> leftSide;
        std::vector<int8_t> rightSide;
    public:
        /*
            The core drivetrain.
            This drivetrain is primarily used as a replacement to a manual motor system; it supports unlimited motors.
            @param leftSide A list of left motor ports.
            @param rightSide A list of right motor ports.
        */
        Drivetrain(const std::vector<int8_t> &leftSide, const std::vector<int8_t> &rightSide) : leftMotors(leftSide), rightMotors(rightSide) {}

        /*Sets the left velocity to a value between 0 - 100. Does this by attempting to set max voltage.*/
        void setLeftVelocity(int velocity)
        {
            leftMotors.setVelocity(minmax(static_cast<int>(velocity), -127, 127));
        }
        /*Sets the right velocity to a value between 0 - 100. Does this by attempting to set max voltage.*/
        void setRightVelocity(int velocity){
            rightMotors.setVelocity(minmax(static_cast<int>(velocity), -127, 127));
        }

        /*Attempts to move forward a certain distance. DOES NOT WORK YET!*/
        void moveDistance(float distance, DrivetrainEnums::Distance dist)
        {
            double leftRotation = leftMotors.getRotation();
            double rightRotation = rightMotors.getRotation();

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