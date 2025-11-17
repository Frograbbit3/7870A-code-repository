#pragma once
#include "custom/emulated_controller.hpp"
#include "custom/enums.hpp"
#include "custom/drivetrain.hpp"
#include "mathlib.h"

namespace ControllerLib
{
    class ControlScheme
    {
    public:
        ControllerEnums::ControllerSettings configuration;

    private:
        int current_action;
        int fr;
        DrivetrainLib::Drivetrain &drive;
        ControllerLib::EmulatedController &controller;
        int16_t leftVelocity;
        int16_t rightVelocity;
        Joystick *RightJoystick;
        Joystick *LeftJoystick;

        // cheese
        float linearCmd = 0.0f;
        bool turnInPlace = false;
        double prevThrottle;
        double prevTurn;
        int left;
        int right;
        double quickStopAccumlator = 0.0;
        double negInertiaAccumlator = 0.0;
        void _updateAccumulators()
        {
            if (negInertiaAccumlator > 1)
            {
                negInertiaAccumlator -= 1;
            }
            else if (negInertiaAccumlator < -1)
            {
                negInertiaAccumlator += 1;
            }
            else
            {
                negInertiaAccumlator = 0;
            }

            if (quickStopAccumlator > 1)
            {
                quickStopAccumlator -= 1;
            }
            else if (quickStopAccumlator < -1)
            {
                quickStopAccumlator += 1;
            }
            else
            {
                quickStopAccumlator = 0.0;
            }
        }

    public:
        ControlScheme(ControllerEnums::ControllerDriveTypes typ, DrivetrainLib::Drivetrain &driveRef, EmulatedController &controllerRef) : drive(driveRef), controller(controllerRef)
        {
            configuration.CONTROL_SCHEME = typ;
            LeftJoystick = &controller.joysticks.Left;
            RightJoystick = &controller.joysticks.Right;
        }
        void update()
        {
            controller.update(); // For the EmulatedController
            if (!configuration.ENABLED)
            {
                return;
            }
            float MAX_SPEED_FACTOR = 0.0f; // std::min(static_cast<double>(0.2f), (pros::millis() - configuration.timeSinceJoystickStop)/2000.0f);

            // Read joystick raw values through the pointer members and apply curve
            float LeftJoystickX = JoystickCurve(LeftJoystick->X);
            float LeftJoystickY = JoystickCurve(LeftJoystick->Y);
            float RightJoystickX = JoystickCurve(RightJoystick->X);
            float RightJoystickY = JoystickCurve(RightJoystick->Y);

            switch (configuration.CONTROL_SCHEME)
            {
            case ARCADE_DRIVE:
                leftVelocity = static_cast<int>(RightJoystickX * -(configuration.MAX_TURN_SPEED + MAX_SPEED_FACTOR)) - static_cast<int>(-LeftJoystickY * (configuration.MAX_FORWARD_SPEED + MAX_SPEED_FACTOR));
                rightVelocity = static_cast<int>(RightJoystickX * -(configuration.MAX_TURN_SPEED + MAX_SPEED_FACTOR)) + static_cast<int>(-LeftJoystickY * (configuration.MAX_FORWARD_SPEED + MAX_SPEED_FACTOR));
                break;
            case TANK_DRIVE:
                leftVelocity = static_cast<int>(LeftJoystickY * (configuration.MAX_FORWARD_SPEED + MAX_SPEED_FACTOR));
                rightVelocity = static_cast<int>(RightJoystickY * -(configuration.MAX_FORWARD_SPEED + MAX_SPEED_FACTOR));
                break;
            case GTA_DRIVE:
            {
                int forward = (controller.buttons.R2.pressed ? 1 : 0) -
                              (controller.buttons.L2.pressed ? 1 : 0);
                int turn = static_cast<int>(LeftJoystickX);

                leftVelocity = static_cast<int>(
                    (forward * (configuration.MAX_FORWARD_SPEED + MAX_SPEED_FACTOR)) -
                    (turn * (configuration.MAX_TURN_SPEED + MAX_SPEED_FACTOR)));

                rightVelocity = static_cast<int>(
                    (forward * (configuration.MAX_FORWARD_SPEED + MAX_SPEED_FACTOR)) +
                    (turn * (configuration.MAX_TURN_SPEED + MAX_SPEED_FACTOR)));
                break;
            }

            /*Credit where credit is due: made by Purdue ACM SIGBots. They also wrote the joystick curve I used.*/
            case CHEESE_DRIVE:
            {
                double ithrottle = LeftJoystickY / 127.0;
                double iturn = RightJoystickX / 127.0;
                double linearCmd = ithrottle;
                bool turnInPlace = false;

                if (fabs(ithrottle) < 0.1 && fabs(iturn) > 0.1)
                {
                    linearCmd = 0.0;
                    turnInPlace = true;
                }
                else if (ithrottle - prevThrottle > DRIVE_SLEW)
                {
                    linearCmd = prevThrottle + DRIVE_SLEW;
                }
                else if (ithrottle - prevThrottle < -(DRIVE_SLEW * 2))
                {
                    linearCmd = prevThrottle - (DRIVE_SLEW * 2);
                }

                double remappedTurn = iturn;
                double left, right;

                if (turnInPlace)
                {
                    left = remappedTurn * fabs(remappedTurn);
                    right = -remappedTurn * fabs(remappedTurn);
                }
                else
                {
                    double negInertiaPower = (iturn - prevTurn) * CD_NEG_INERTIA_SCALAR;
                    negInertiaAccumlator += negInertiaPower;

                    double angularCmd =
                        fabs(linearCmd) *
                            (remappedTurn + negInertiaAccumlator) *
                            CD_SENSITIVITY -
                        quickStopAccumlator;

                    right = left = linearCmd;
                    left += angularCmd;
                    right -= angularCmd;

                    _updateAccumulators();
                }

                // scale to [-127, 127]
                leftVelocity = static_cast<int>(std::clamp(left, -1.0, 1.0) * 127);
                rightVelocity = static_cast<int>(std::clamp(right, -1.0, 1.0) * 127);

                prevTurn = iturn;
                prevThrottle = ithrottle;
                break;
            }
            }

            if (fabs(leftVelocity) > 0 || fabs(rightVelocity) > 0)
            {
                leftVelocity = minmax(static_cast<int>(leftVelocity), -127, 127);
                rightVelocity = minmax(static_cast<int>(rightVelocity), -127, 127);
                drive.setVelocity(leftVelocity, rightVelocity);
                drive.drive(DRIVE_FORWARD);
            }
            else
            {
                configuration.timeSinceJoystickStop = pros::millis();
                drive.stop();
            }
            fr++;
        }
    };
}
