#pragma once
#include "custom/modded.hpp"
#include "custom/enums.hpp"
#include "custom/drivetrain.hpp"

namespace ControllerLib
{
    enum class ControllerEnums
    {
        DRIVE_MODE_TANK = 1,
        DRIVE_MODE_ARCADE = 2
    };

    class ControlScheme {
        private:
            DriveUtils::Drivetrain& drive;
            pros::Controller& controller;
            ControllerLib::ControllerEnums& type;
            uint8_t leftVelocity;
            uint8_t rightVelocity;

            //welcome to my amazing code
            int32_t leftJoystickY;
            int32_t leftJoystickX;
            int32_t rightJoystickY;
            int32_t rightJoystickX;
        public:
            float MAX_TURN_SPEED = 0.6f;
            float MAX_FORWARD_SPEED = 0.8f;
            float DEADZONE = 0.1f;
        public:
            ControlScheme(ControllerLib::ControllerEnums typ, DriveUtils::Drivetrain& driveref, pros::Controller& controlleref)  : drive(driveref), controller(controlleref), type(typ) {}
            void update() {
                /*@brief main controller loop.*/
                leftJoystickY = controller.get_analog(ANALOG_LEFT_Y);
                rightJoystickY = controller.get_analog(ANALOG_RIGHT_Y);
                leftJoystickX = controller.get_analog(ANALOG_LEFT_X);
                rightJoystickX = controller.get_analog(ANALOG_RIGHT_X);
                switch (type)
                {
                case ControllerLib::ControllerEnums::DRIVE_MODE_ARCADE:
                    leftVelocity = (rightJoystickX * -MAX_TURN_SPEED) - (leftJoystickY * MAX_FORWARD_SPEED);
                    rightVelocity = (rightJoystickX * -MAX_TURN_SPEED) + (leftJoystickY * MAX_FORWARD_SPEED);
                    break;
                
                case ControllerLib::ControllerEnums::DRIVE_MODE_TANK:
                    leftVelocity = leftJoystickY * MAX_FORWARD_SPEED;
                    rightVelocity = rightJoystickY * MAX_FORWARD_SPEED;
                default:
                    break;
                }
                if (abs(leftJoystickY) > DEADZONE || abs(rightJoystickY) > DEADZONE) {
                    drive.setLeftVelocity((leftVelocity/127.0f)*100.0f);
                    drive.setRightVelocity((rightVelocity/127.0f)*100.0f);
                    drive.drive(DrivetrainEnums::Direction::FORWARD);
                }else{
                    drive.stop();
                }
            }

    };
} // namespace ControllerLib
