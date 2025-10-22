#pragma once
#include "custom/modded.hpp"
#include "custom/enums.hpp"
#include "custom/drivetrain.hpp"
#include <map>
#include <vector>
#include <algorithm> // For std::sort

// random helper func

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
    private:
        DriveUtils::Drivetrain &drive;
        pros::Controller &controller;
        std::vector<ControllerLib::Macro> macros;
        int16_t leftVelocity;
        int16_t rightVelocity;

        std::vector<pros::controller_digital_e_t> pressed;
        std::vector<pros::controller_digital_e_t> last_pressed;
        int lastPressedTime = pros::millis();
        bool is_held = true;
        // welcome to my amazing code
        int32_t leftJoystickY;
        int32_t leftJoystickX;
        int32_t rightJoystickY;
        int32_t rightJoystickX;

        bool keepGoing = false;

        void runMacro(const ControllerLib::Macro macro)
        {
            const std::vector<pros::controller_digital_e_t> &inputs = macro.MACRO_KEYS;
            // No need for reinterpret_cast since we changed the field types
            void (*on_press)() = macro.ON_PRESSED;
            void (*on_release)() = macro.ON_RELEASED;
            // std::cout << pressed.size() << std::endl;
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
        ControllerEnums::ControllerSettings configuration;
        ControlScheme(ControllerEnums::ControllerDriveTypes typ, DriveUtils::Drivetrain &driveRef, pros::Controller &controllerRef) : drive(driveRef), controller(controllerRef)
        {
            configuration.CONTROL_SCHEME = typ;
        }
        void createMacro(ControllerLib::Macro mac)
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
            ///@brief main controller loop
            leftJoystickY = controller.get_analog(ANALOG_LEFT_Y);
            rightJoystickY = controller.get_analog(ANALOG_RIGHT_Y);
            leftJoystickX = controller.get_analog(ANALOG_LEFT_X);
            rightJoystickX = controller.get_analog(ANALOG_RIGHT_X);

            /// Configuration processing
            drive.configuration.AUTO_DRIVE_ENABLED = configuration.DRIVE_AUTO_CORRECTION;
            if (!configuration.ENABLED)
            {
                return;
            }
            /// MACRO SYSTEM
            // std::cout << "." << std::endl;
            if (pros::millis() - lastPressedTime > 25)
            {
                pressed = getPressedButtons(controller);
                if (pressed.size() <= 0)
                {
                    // std::cout << "cleared" << std::endl;
                    is_held = false;
                    for (ControllerLib::Macro &mac : macros)
                    {
                        if (last_pressed == mac.MACRO_KEYS)
                        {
                            if (mac.ON_RELEASED != nullptr)
                            {
                                mac.ON_RELEASED();
                            }
                        }
                    }
                    last_pressed.clear();
                }
                else
                {
                    std::vector<ControllerLib::Macro *> sorted_macros;
                    for (auto &entry : macros)
                    {
                        sorted_macros.push_back(&entry);
                    }

                    std::sort(sorted_macros.begin(), sorted_macros.end(),
                              [](const auto *a, const auto *b)
                              {
                                  return a->MACRO_KEYS.size() > b->MACRO_KEYS.size();
                              });

                    for (const auto *entry_ptr : sorted_macros)
                    {
                        if (entry_ptr->MACRO_KEYS.size() != last_pressed.size())
                        {
                            runMacro(*entry_ptr);
                        }
                    }
                }
            }
            // print("going into controller scheme");

            if (configuration.CONTROL_SCHEME == ARCADE_DRIVE)
            {
                // Convert to int16_t to handle the multiplication properly
                leftVelocity = static_cast<int16_t>(rightJoystickX * -configuration.MAX_TURN_SPEED) - static_cast<int16_t>(-leftJoystickY * configuration.MAX_FORWARD_SPEED);
                rightVelocity = static_cast<int16_t>(rightJoystickX * -configuration.MAX_TURN_SPEED) + static_cast<int16_t>(-leftJoystickY * configuration.MAX_FORWARD_SPEED);
                // Clamp values to valid range for uint8_t
                leftVelocity = std::min(127, std::max(-127, static_cast<int>(leftVelocity)));
                rightVelocity = std::min(127, std::max(-127, static_cast<int>(rightVelocity)));
                // print("arcade");

            }else if (configuration.CONTROL_SCHEME == TANK_DRIVE)
                {
                    leftVelocity = static_cast<int16_t>(leftJoystickY * configuration.MAX_FORWARD_SPEED);
                    rightVelocity = static_cast<int16_t>(rightJoystickY * -configuration.MAX_FORWARD_SPEED);
                    // Clamp values to valid range for uint8_t
                    leftVelocity = std::min(127, std::max(-127, static_cast<int>(leftVelocity)));
                    rightVelocity = std::min(127, std::max(-127, static_cast<int>(rightVelocity)));
                }
                // print(leftVelocity / 127.0f);
                // std::cout << rightVelocity << std::endl;
                
                if (abs(leftJoystickY) > configuration.DEADZONE || abs(rightJoystickY) > configuration.DEADZONE ||abs(leftJoystickX) > configuration.DEADZONE || abs(rightJoystickX) > configuration.DEADZONE)
                {
                    // std::cout << "JOYSTICk!" << std::endl;
                    // drive.setLeftVelocity((leftVelocity / 127.0f)*100.0f);
                    // drive.setRightVelocity((rightVelocity / 127.0f)*100.0f);
                    // drive.drive(DrivetrainEnums::Direction::FORWARD);
                    drive.leftMotors.move(leftVelocity);
                    drive.rightMotors.move(rightVelocity);
                }
                else
                {
                    leftVelocity = 0.0f;
                    drive.leftMotors.move(0);
                    drive.rightMotors.move(0);
                }
            }
        };
    } // namespace ControllerLib
