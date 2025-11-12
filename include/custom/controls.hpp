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
        ControllerLib::EmulatedController& controller;
        int16_t leftVelocity;
        int16_t rightVelocity;
        Joystick* RightJoystick;
        Joystick* LeftJoystick;

    public:
        ControlScheme(ControllerEnums::ControllerDriveTypes typ, DrivetrainLib::Drivetrain &driveRef, EmulatedController &controllerRef) : drive(driveRef), controller(controllerRef)
        {
            configuration.CONTROL_SCHEME = typ;
            LeftJoystick = &controller.joysticks.Left;
            RightJoystick = &controller.joysticks.Right;
        }
        void update(){
            drive.configuration.AUTO_DRIVE_ENABLED = configuration.DRIVE_AUTO_CORRECTION;
            controller.update(); // For the EmulatedController
            if (!configuration.ENABLED) {return;}
            float MAX_SPEED_FACTOR = 0.0f; // std::min(static_cast<double>(0.2f), (pros::millis() - configuration.timeSinceJoystickStop)/2000.0f);

            // Read joystick raw values through the pointer members and apply curve
            float LeftJoystickX = JoystickCurve(LeftJoystick->X);
            float LeftJoystickY = JoystickCurve(LeftJoystick->Y);
            float RightJoystickX = JoystickCurve(RightJoystick->X);
            float RightJoystickY = JoystickCurve(RightJoystick->Y);

            switch (configuration.CONTROL_SCHEME) {
                case ARCADE_DRIVE:
                    leftVelocity = static_cast<int>(RightJoystickX * -(configuration.MAX_TURN_SPEED+MAX_SPEED_FACTOR)) - static_cast<int>(-LeftJoystickY* (configuration.MAX_FORWARD_SPEED+MAX_SPEED_FACTOR));
                    rightVelocity = static_cast<int>(RightJoystickX * -(configuration.MAX_TURN_SPEED+MAX_SPEED_FACTOR)) + static_cast<int>(-LeftJoystickY * (configuration.MAX_FORWARD_SPEED+MAX_SPEED_FACTOR));
                    break;
                case TANK_DRIVE:
                    leftVelocity = static_cast<int>(LeftJoystickY * (configuration.MAX_FORWARD_SPEED+MAX_SPEED_FACTOR));
                    rightVelocity = static_cast<int>(RightJoystickY * -(configuration.MAX_FORWARD_SPEED+MAX_SPEED_FACTOR));
                    break;
                case GTA_DRIVE:
                    int forward = (controller.buttons.R2.pressed ? 1 : 0) -
                                (controller.buttons.L2.pressed ? 1 : 0);
                    int turn = LeftJoystickX;

                    leftVelocity = static_cast<int>(
                        (forward * (configuration.MAX_FORWARD_SPEED + MAX_SPEED_FACTOR)) -
                        (turn * (configuration.MAX_TURN_SPEED + MAX_SPEED_FACTOR))
                    );

                    rightVelocity = static_cast<int>(
                        (forward * (configuration.MAX_FORWARD_SPEED + MAX_SPEED_FACTOR)) +
                        (turn * (configuration.MAX_TURN_SPEED + MAX_SPEED_FACTOR))
                    );
                    break;
                
                
            }
            
            if (LeftJoystick->moving || RightJoystick->moving)
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
