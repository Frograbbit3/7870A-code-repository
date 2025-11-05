#pragma once
#include "custom/emulated_controller.hpp"
#include "custom/modded.hpp"
#include "custom/enums.hpp"
#include "custom/drivetrain.hpp"
#include <map>
#include <vector>
#include <algorithm>

namespace ControllerLib
{
    class Macro
    {
    public:
        std::vector<Button*> MACRO_KEYS;
        void (*ON_PRESSED)();
        void (*ON_RELEASED)();
        bool REGISTER_HOLD = false;
        bool LAST_PRESSED = false;
        Macro(const std::vector<Button*> &key_pressed, void (*press)(), void (*release)() = nullptr, bool hold = false) : MACRO_KEYS(key_pressed),
                                                                                                                                               ON_PRESSED(press),
                                                                                                                                               ON_RELEASED(release),
                                                                                                                                               REGISTER_HOLD(hold)
        {
        }
    };
    class ControlScheme
    {
    public:
        ControllerEnums::ControllerSettings configuration;

    private:
        int current_action;
        DrivetrainLib::Drivetrain &drive;
        ControllerLib::EmulatedController& controller;
        std::vector<ControllerLib::Macro> macros;
        int16_t leftVelocity;
        int16_t rightVelocity;
        int lastPressedTime = pros::millis();
        bool is_held = true;
        bool keepGoing = false;
        Joystick* RightJoystick;
        Joystick* LeftJoystick;
        void runMacro(const ControllerLib::Macro* macro)
        {
            const std::vector<Button*> &inputs = macro->MACRO_KEYS;
            void (*on_press)() = macro->ON_PRESSED;
            void (*on_release)() = macro->ON_RELEASED;
            if (!is_held && isMacroPressed(*macro))
            {
                lastPressedTime = pros::millis();
                if (!macro->REGISTER_HOLD)
                {
                    is_held = true;
                }
                on_press();
                return;
            }
        }

    public:
        ControlScheme(ControllerEnums::ControllerDriveTypes typ, DrivetrainLib::Drivetrain &driveRef, EmulatedController &controllerRef) : drive(driveRef), controller(controllerRef)
        {
            configuration.CONTROL_SCHEME = typ;
            LeftJoystick = &controller.joysticks.Left;
            RightJoystick = &controller.joysticks.Right;
        }
        inline void createMacro(ControllerLib::Macro mac)
        {
            macros.push_back(mac);
        }
        bool isMacroPressed(const ControllerLib::Macro &macro)
        {
            if (!configuration.MACROS_ENABLED)
            {
                return false;
            }
            for (const Button* m : macro.MACRO_KEYS)
            {
                if (!m->pressed)
                {
                    return false;
                }
            }
            return true;
        }
        void update(){
            drive.configuration.AUTO_DRIVE_ENABLED = configuration.DRIVE_AUTO_CORRECTION;
            controller.update();
            if (!configuration.ENABLED)
                return;

            if (pros::millis() - lastPressedTime > 25 && configuration.MACROS_ENABLED)
            {
                for (Macro &macro : macros) {
                    int buttonCount = 0;
                    for (const Button* button : macro.MACRO_KEYS) {
                        if (button->pressed) {
                            buttonCount++;
                        }
                    }
                    if (buttonCount == macro.MACRO_KEYS.size() && (!macro.LAST_PRESSED || macro.REGISTER_HOLD)) {
                        macro.LAST_PRESSED=true;
                        runMacro(&macro);
                    }else{
                        macro.LAST_PRESSED = false;
                        if (macro.ON_RELEASED) {
                            macro.ON_RELEASED();
                        }
                    }
                }
            }
            if (configuration.CONTROL_SCHEME == ARCADE_DRIVE)
            {
                leftVelocity = static_cast<int16_t>(RightJoystick->X * -configuration.MAX_TURN_SPEED) - static_cast<int16_t>(-LeftJoystick->Y* configuration.MAX_FORWARD_SPEED);
                rightVelocity = static_cast<int16_t>(RightJoystick->X * -configuration.MAX_TURN_SPEED) + static_cast<int16_t>(-LeftJoystick->Y * configuration.MAX_FORWARD_SPEED);
                leftVelocity = std::min(127, std::max(-127, static_cast<int>(leftVelocity)));
                rightVelocity = std::min(127, std::max(-127, static_cast<int>(rightVelocity)));
            }
            else if (configuration.CONTROL_SCHEME == TANK_DRIVE)
            {
                leftVelocity = static_cast<int16_t>(LeftJoystick->Y * configuration.MAX_FORWARD_SPEED);
                rightVelocity = static_cast<int16_t>(RightJoystick->Y * -configuration.MAX_FORWARD_SPEED);
                leftVelocity = std::min(127, std::max(-127, static_cast<int>(leftVelocity)));
                rightVelocity = std::min(127, std::max(-127, static_cast<int>(rightVelocity)));
            }
            if (LeftJoystick->moving || RightJoystick->moving)
            {
                drive.setLeftVelocity(leftVelocity);
                drive.setRightVelocity(rightVelocity);
                drive.drive(DRIVE_FORWARD);
            }
            else
            {
                drive.stop();
            }
        }
    };
}
