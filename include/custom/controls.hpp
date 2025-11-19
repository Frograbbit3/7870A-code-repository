#pragma once
#include "custom/emulated_controller.hpp"
#include "custom/enums.hpp"
#include "custom/drivetrain.hpp"
#include "mathlib.h"
#include <variant>

namespace ControllerLib
{
    struct ControlBinding
    {
        std::variant<DrivetrainEnums::CustomMotor *, MotorGroup *> motor;
        std::variant<std::pair<ControllerLib::Button, ControllerLib::Button>, ControllerLib::Button> buttons;
        int speed = 127;
        bool toggle = false;
    };

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

        float linearCmd = 0.0f;
        bool turnInPlace = false;
        double prevThrottle;
        double prevTurn;
        int left;
        int right;
        double quickStopAccumlator = 0.0;
        double negInertiaAccumlator = 0.0;
        float LeftJoystickX;
        float LeftJoystickY;
        float RightJoystickX;
        float RightJoystickY;

        void _updateAccumulators();
        void tankDrive();
        void arcadeDrive();
        void gtaDrive();
        void cheeseDrive();

    public:
        ControlScheme(ControllerEnums::ControllerDriveTypes typ,
                      DrivetrainLib::Drivetrain &driveRef,
                      EmulatedController &controllerRef);

        void registerMotor(ControlBinding binding);

    private:
        void doControllerInputs();

    public:
        void update();
    };
}
