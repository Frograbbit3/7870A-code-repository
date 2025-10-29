#pragma once
#include "custom/modded.hpp"
#include "custom/enums.hpp"
#include "custom/drivetrain.hpp"
#include <map>
#include <vector>
#include <algorithm>
inline std::vector<pros::controller_digital_e_t> getPressedButtons(pros::Controller &control)
{
    std::vector<pros::controller_digital_e_t> pressed = {};
    for (auto &m : ControllerEnums::__BUTTON_LIST)
    {
        if (control.get_digital(m))
        {
            pressed.emplace_back(m);
        }
    }
    return pressed;
}
namespace ControllerLib
{
    class Macro
    {
    public:
        std::vector<pros::controller_digital_e_t> MACRO_KEYS;
        void (*ON_PRESSED)();
        void (*ON_RELEASED)();
        bool REGISTER_HOLD = false;
        Macro(const std::vector<pros::controller_digital_e_t> &key_pressed, void (*press)(), void (*release)() = nullptr, bool hold = false) : MACRO_KEYS(key_pressed),
                                                                                                                                               ON_PRESSED(press),
                                                                                                                                               ON_RELEASED(release),
                                                                                                                                               REGISTER_HOLD(hold)
        {
        }
    };
    class ControlScheme
    {
    public:
        int32_t leftJoystickY;
        int32_t leftJoystickX;
        int32_t rightJoystickY;
        int32_t rightJoystickX;
        ControllerEnums::ControllerSettings configuration;

    private:
        int current_action;
        DrivetrainLib::Drivetrain &drive;
        pros::Controller &controller;
        std::vector<ControllerLib::Macro> macros;
        int16_t leftVelocity;
        int16_t rightVelocity;
        std::vector<pros::controller_digital_e_t> pressed;
        std::vector<pros::controller_digital_e_t> last_pressed;
        int lastPressedTime = pros::millis();
        bool is_held = true;
        bool keepGoing = false;
        void runMacro(const ControllerLib::Macro macro)
        {
            const std::vector<pros::controller_digital_e_t> &inputs = macro.MACRO_KEYS;
            void (*on_press)() = macro.ON_PRESSED;
            void (*on_release)() = macro.ON_RELEASED;
            if (!is_held && isMacroPressed(macro))
            {
                lastPressedTime = pros::millis();
                if (!macro.REGISTER_HOLD)
                {
                    is_held = true;
                }
                on_press();
                return;
            }
        }

    public:
        ControlScheme(ControllerEnums::ControllerDriveTypes typ, DrivetrainLib::Drivetrain &driveRef, pros::Controller &controllerRef) : drive(driveRef), controller(controllerRef)
        {
            configuration.CONTROL_SCHEME = typ;
        }
        inline void createMacro(ControllerLib::Macro mac)
        {
            macros.push_back(mac);
        }
        void vibrateController(std::string pattern)
        {
            if (pattern.size() <= 8)
            {
                controller.rumble(pattern.c_str());
            }
            else
            {
                int loops = (int)ceil((float)pattern.size() / 8.0f);
                for (int i = 0; i < loops; i++)
                {
                    size_t start = i * 8;
                    size_t length = std::min<size_t>(8, pattern.size() - start);
                    std::string part = pattern.substr(start, length);
                    controller.rumble(part.c_str());
                }
            }
        }
        bool isMacroPressed(const ControllerLib::Macro &macro)
        {
            if (!configuration.MACROS_ENABLED)
            {
                return false;
            }
            if (getPressedButtons(controller).size() != macro.MACRO_KEYS.size())
            {
                return false;
            }
            for (const pros::controller_digital_e_t &m : macro.MACRO_KEYS)
            {
                if (!controller.get_digital(m))
                {
                    return false;
                }
            }
            return true;
        }
        void update()
        {
            leftJoystickY = controller.get_analog(ANALOG_LEFT_Y);
            rightJoystickY = controller.get_analog(ANALOG_RIGHT_Y);
            leftJoystickX = controller.get_analog(ANALOG_LEFT_X);
            rightJoystickX = controller.get_analog(ANALOG_RIGHT_X);
            drive.configuration.AUTO_DRIVE_ENABLED = configuration.DRIVE_AUTO_CORRECTION;
            if (!configuration.ENABLED)
            {
                return;
            }
            if (pros::millis() - lastPressedTime > 25 && configuration.MACROS_ENABLED)
            {
                pressed = getPressedButtons(controller);
                if (pressed.empty()) {
                    is_held = false;
                    for (ControllerLib::Macro &mac : macros) {
                        keepGoing = false;

                        // Check if the last pressed keys match the macro's keys exactly
                        if (last_pressed.size() == mac.MACRO_KEYS.size()) {
                            keepGoing = true;
                            for (size_t i = 0; i < mac.MACRO_KEYS.size(); i++) {
                                if (last_pressed[i] != mac.MACRO_KEYS[i]) {
                                    keepGoing = false;
                                    break; // Exit early if there's a mismatch
                                }
                            }
                        }

                        if (keepGoing) {
                            mac.ON_RELEASED(); // Trigger the release action
                            last_pressed.clear(); // Clear only after release is handled
                        }
                    }
                } else {
                    std::vector<ControllerLib::Macro *> sorted_macros;

                    // Sort macros by the size of their MACRO_KEYS (longer macros first)
                    for (auto &entry : macros) {
                        sorted_macros.push_back(&entry);
                    }
                    std::sort(sorted_macros.begin(), sorted_macros.end(),
                            [](const auto *a, const auto *b) {
                                return a->MACRO_KEYS.size() > b->MACRO_KEYS.size();
                            });
                    for (const auto *entry_ptr : sorted_macros) {
                        // Check if the currently pressed keys match the macro's keys exactly
                        if (pressed.size() == entry_ptr->MACRO_KEYS.size()) {
                            bool match = true;

                            // Check if all keys in MACRO_KEYS are in pressed
                            for (const auto &key : entry_ptr->MACRO_KEYS) {
                                if (std::find(pressed.begin(), pressed.end(), key) == pressed.end()) {
                                    match = false;
                                    break; // Exit early if a key is not found
                                }
                            }

                            if (match) {
                                last_pressed = entry_ptr->MACRO_KEYS; // Update last_pressed
                                runMacro(*entry_ptr); // Trigger the macro
                                break; // Exit after the first matching macro
                            }
                        }
                    }
                }
            }
            if (configuration.CONTROL_SCHEME == ARCADE_DRIVE)
            {
                leftVelocity = static_cast<int16_t>(rightJoystickX * -configuration.MAX_TURN_SPEED) - static_cast<int16_t>(-leftJoystickY * configuration.MAX_FORWARD_SPEED);
                rightVelocity = static_cast<int16_t>(rightJoystickX * -configuration.MAX_TURN_SPEED) + static_cast<int16_t>(-leftJoystickY * configuration.MAX_FORWARD_SPEED);
                leftVelocity = std::min(127, std::max(-127, static_cast<int>(leftVelocity)));
                rightVelocity = std::min(127, std::max(-127, static_cast<int>(rightVelocity)));
            }
            else if (configuration.CONTROL_SCHEME == TANK_DRIVE)
            {
                leftVelocity = static_cast<int16_t>(leftJoystickY * configuration.MAX_FORWARD_SPEED);
                rightVelocity = static_cast<int16_t>(rightJoystickY * -configuration.MAX_FORWARD_SPEED);
                leftVelocity = std::min(127, std::max(-127, static_cast<int>(leftVelocity)));
                rightVelocity = std::min(127, std::max(-127, static_cast<int>(rightVelocity)));
            }
            if (abs(leftJoystickY) > configuration.DEADZONE || abs(rightJoystickY) > configuration.DEADZONE || abs(leftJoystickX) > configuration.DEADZONE || abs(rightJoystickX) > configuration.DEADZONE)
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
