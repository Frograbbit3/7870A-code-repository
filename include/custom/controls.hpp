#pragma once
#include "custom/modded.hpp"
#include "custom/enums.hpp"
#include "custom/drivetrain.hpp"
#include <map>
#include <vector>
#include <algorithm>  // For std::sort

// random helper func
std::vector<pros::controller_digital_e_t> __BUTTON_LIST = {
    pros::E_CONTROLLER_DIGITAL_L1,
    pros::E_CONTROLLER_DIGITAL_L2,
    pros::E_CONTROLLER_DIGITAL_R1,
    pros::E_CONTROLLER_DIGITAL_R2,
    pros::E_CONTROLLER_DIGITAL_UP,
    pros::E_CONTROLLER_DIGITAL_DOWN,
    pros::E_CONTROLLER_DIGITAL_LEFT,
    pros::E_CONTROLLER_DIGITAL_RIGHT,
    pros::E_CONTROLLER_DIGITAL_X,
    pros::E_CONTROLLER_DIGITAL_B,
    pros::E_CONTROLLER_DIGITAL_Y,
    pros::E_CONTROLLER_DIGITAL_A,
    pros::E_CONTROLLER_DIGITAL_POWER};

std::vector<pros::controller_digital_e_t> getPressedButtons(pros::Controller &control)
{
    std::vector<pros::controller_digital_e_t> pressed = {};
    for (auto &m : __BUTTON_LIST)
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
    enum class ControllerEnums
    {
        DRIVE_MODE_TANK = 1,
        DRIVE_MODE_ARCADE = 2
    };

    struct ControllerMacro {
        std::vector<pros::controller_digital_e_t> keybinds;
        bool hold = false;
        void* callable;
    };
    class ControlScheme
    {
    private:
        DriveUtils::Drivetrain &drive;
        pros::Controller &controller;
        ControllerLib::ControllerEnums &type;
        uint8_t leftVelocity;
        uint8_t rightVelocity;
        std::vector<ControllerLib::ControllerMacro> macros;
        std::vector<pros::controller_digital_e_t> pressed;
        std::vector<pros::controller_digital_e_t> last_pressed;
        int lastPressedtime = pros::millis();
        bool is_held = true;
        // welcome to my amazing code
        int32_t leftJoystickY;
        int32_t leftJoystickX;
        int32_t rightJoystickY;
        int32_t rightJoystickX;

        bool keepGoing = false;

        void runMacro(const ControllerLib::ControllerMacro macro)
        {
            const std::vector<pros::controller_digital_e_t> &inputs = macro.keybinds;
            void (*macro_func)() = reinterpret_cast<void (*)()>(macro.callable);
            //std::cout << pressed.size() << std::endl;
                if (!is_held && isMacroPressed(macro))
                {
                    lastPressedtime = pros::millis();
                    if (!macro.hold) {is_held = true;}
                    macro_func();
                    return;
                }
        }

    public:
        float MAX_TURN_SPEED = 0.6f;
        float MAX_FORWARD_SPEED = 0.8f;
        float DEADZONE = 0.1f;
        ControlScheme(ControllerLib::ControllerEnums typ, DriveUtils::Drivetrain &driveref, pros::Controller &controlleref) : drive(driveref), controller(controlleref), type(typ) {}
        void createMacro(const std::vector<pros::controller_digital_e_t> &keys, void (*func)(), bool hold=false)
        {
            ControllerMacro mac;
            mac.callable = reinterpret_cast<void*>(func);
            mac.hold = hold;
            mac.keybinds = keys;
            macros.push_back(mac);
        }

        bool isMacroPressed(const ControllerLib::ControllerMacro &macro) {
            if (getPressedButtons(controller).size() != macro.keybinds.size()) {return false;}
            for (const pros::controller_digital_e_t &m : macro.keybinds)
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

            /// MACRO SYSTEM
            // std::cout << "." << std::endl;
            if (pros::millis() - lastPressedtime > 25)
            {
                pressed = getPressedButtons(controller);
                if (pressed.size() <= 0)
                {
                    //std::cout << "cleared" << std::endl;
                    is_held = false;
                    last_pressed.clear();
                }
                else
                {
                    std::vector<ControllerMacro*> sorted_macros;
                    for (auto& entry : macros) {
                        sorted_macros.push_back(&entry);
                    }
                    
                    std::sort(sorted_macros.begin(), sorted_macros.end(), 
                        [](const auto* a, const auto* b) {
                            return a->keybinds.size() > b->keybinds.size(); 
                        });
                    
                    for (const auto* entry_ptr : sorted_macros)
                    {
                        if (entry_ptr->keybinds.size() != last_pressed.size()) {
                            runMacro(*entry_ptr);
                        }
                    }
                }
            }

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
            if (abs(leftJoystickY) > DEADZONE || abs(rightJoystickY) > DEADZONE)
            {
                drive.setLeftVelocity((leftVelocity / 127.0f) * 100.0f);
                drive.setRightVelocity((rightVelocity / 127.0f) * 100.0f);
                drive.drive(DrivetrainEnums::Direction::FORWARD);
            }
            else
            {
                drive.stop();
            }
        }
    };
} // namespace ControllerLib
