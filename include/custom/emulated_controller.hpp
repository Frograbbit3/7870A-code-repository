#pragma once
#include "pros/misc.hpp"
#include "config.h"
#include <cmath>
#include <cstdlib>
#include <vector>
#include <functional>

namespace ControllerLib
{
    class EmulatedController;
    struct Joystick{
        pros::controller_analog_e_t stickX;
        pros::controller_analog_e_t stickY;
        float X = 0;
        float Y = 0;
        float deadzone = 0.15f;
        bool process = true;
        bool moving = false;
        std::function<void(float, float)> OnMoveCallback = nullptr;
        pros::Controller *control = nullptr;

        Joystick() = default;
        Joystick(pros::Controller *ctrl,
                 pros::controller_analog_e_t stkX,
                 pros::controller_analog_e_t stkY)
            : stickX(stkX), stickY(stkY), control(ctrl) {}

        /*Called automatically by the EmulatedController loop, but this is used if macro mode is not enabled to link the joystick to the physical joystick.*/
        void update()
        {
            if (!process || control == nullptr)
            {
                return;
            }
            if (static_cast<float>(control->get_analog(stickX)) != X || static_cast<float>(control->get_analog(stickY)) != Y)
            {
                SetStick(static_cast<float>(control->get_analog(stickX)), static_cast<float>(control->get_analog(stickY)));
            }
            else
            {
                SetStick(static_cast<float>(control->get_analog(stickX)), static_cast<float>(control->get_analog(stickY)));
            }
        }

        /*Sets the stick X and stick Y. If you want to set these values, call SetEnabled(false) to disable the joystick overwriting it.*/
        void SetStick(float nx, float ny)
        {   
            moving = (fabs(nx) > deadzone || fabs(ny) > deadzone); // FIXED: moving when OUTSIDE deadzone
            if (OnMoveCallback != nullptr && moving) {OnMoveCallback(nx-X, ny-Y);}
            X = fabs(nx) > deadzone ? nx : 0.0f;
            Y = fabs(ny) > deadzone ? ny : 0.0f;
        }

        /*Enabled / disables Macro mode.*/
        void SetEnabled(bool enabled) { process = enabled; }

        /*
            Pass in a function and it will call it with the DIFFERENCE between the old joystick and the new joystick
            @param func This is an std::function with two args, both being float.The first one is the difference in the X and the second one is the difference in the Y.
        */
        void OnMove(const std::function<void(float, float)> &func)
        {
            OnMoveCallback = func;
        }
    };

    struct Button
    {
        pros::controller_digital_e_t button;
        pros::Controller *control = nullptr;

        bool pressed = false;
        bool process = true;
        bool triggered_press_callback = false;
        bool triggered_release_callback = false;
        EmulatedController *parent = nullptr;
        std::function<void()> OnPressCallback = nullptr;
        std::function<void()> OnReleaseCallback = nullptr;

        Button() = default;

        Button(EmulatedController *parent_,
               pros::Controller *ctrl,
               pros::controller_digital_e_t btn)
            : button(btn), control(ctrl), parent(parent_)
        {
        }

        /*Processes button events. Called by EmulatedController main loop, calling manually does next to nothing.*/
        void update()
        {
            if (!process || control == nullptr)
            {
                return;
            }
            SetButton(control->get_digital(button));
        }
        
        /*Changes the button's state. To manually call this run SetEnabled(false)*/
        void SetButton(bool state)
        {
            pressed = state;
            if (pressed)
            {
                if (OnPressCallback != nullptr && !triggered_press_callback)
                {
                    triggered_press_callback = true;
                    triggered_release_callback = false;
                    OnPressCallback();
                }
            }
            else
            {
                if (OnReleaseCallback != nullptr && !triggered_release_callback)
                {
                    triggered_release_callback = true;
                    triggered_press_callback = false;
                    OnReleaseCallback();
                }
            }
        }
        /*Enabled / disables the button; disabling this will allow the user to control it*/
        void SetEnabled(bool enabled) { process = enabled; }

        /*
            Callback for when the button is pressed.
            @param func An std::function with no args.
        */
        void OnButtonPress(const std::function<void()> &func)
        {
            OnPressCallback = func;
        }
        /*
            Callback for when the button is released.
            @param func An std::function with no args.
        */
        void OnButtonRelease(const std::function<void()> &func)
        {
            OnReleaseCallback = func;
        }
    };

    class EmulatedController
    {
    private:
        pros::Controller *controller;
        std::vector<Button*> btns;

    public:

        struct
        {
            Button A;
            Button B;
            Button Y;
            Button X;
            Button Left;
            Button Right;
            Button Up;
            Button Down;
            Button L1;
            Button L2;
            Button R1;
            Button R2;
            Button Power;
        } buttons;

        struct
        {
            Joystick Left;
            Joystick Right;
        } joysticks;

        double battery = 100;
        bool enabled = true;
        bool connected = false;

        EmulatedController(pros::Controller *ctrl)
            : controller(ctrl)
        {
            joysticks.Left = Joystick(ctrl, pros::E_CONTROLLER_ANALOG_LEFT_X, pros::E_CONTROLLER_ANALOG_LEFT_Y);
            joysticks.Right = Joystick(ctrl, pros::E_CONTROLLER_ANALOG_RIGHT_X, pros::E_CONTROLLER_ANALOG_RIGHT_Y);
            buttons.A = Button(this, ctrl, pros::E_CONTROLLER_DIGITAL_A);
            buttons.B = Button(this, ctrl, pros::E_CONTROLLER_DIGITAL_B);
            buttons.Y = Button(this, ctrl, pros::E_CONTROLLER_DIGITAL_Y);
            buttons.X = Button(this, ctrl, pros::E_CONTROLLER_DIGITAL_X);
            buttons.Left = Button(this, ctrl, pros::E_CONTROLLER_DIGITAL_LEFT);
            buttons.Right = Button(this, ctrl, pros::E_CONTROLLER_DIGITAL_RIGHT);
            buttons.Up = Button(this, ctrl, pros::E_CONTROLLER_DIGITAL_UP);
            buttons.Down = Button(this, ctrl, pros::E_CONTROLLER_DIGITAL_DOWN);
            buttons.L1 = Button(this, ctrl, pros::E_CONTROLLER_DIGITAL_L1);
            buttons.L2 = Button(this, ctrl, pros::E_CONTROLLER_DIGITAL_L2);
            buttons.R1 = Button(this, ctrl, pros::E_CONTROLLER_DIGITAL_R1);
            buttons.R2 = Button(this, ctrl, pros::E_CONTROLLER_DIGITAL_R2);
            buttons.Power = Button(this, ctrl, pros::E_CONTROLLER_DIGITAL_POWER);
            
            btns.push_back(&buttons.A);
            btns.push_back(&buttons.B);
            btns.push_back(&buttons.Y);
            btns.push_back(&buttons.X);
            btns.push_back(&buttons.Left);
            btns.push_back(&buttons.Right);
            btns.push_back(&buttons.Up);
            btns.push_back(&buttons.Down);
            btns.push_back(&buttons.L1);
            btns.push_back(&buttons.L2);
            btns.push_back(&buttons.R1);
            btns.push_back(&buttons.R2);
            btns.push_back(&buttons.Power);
        }

        void vibrate(std::string pattern)
        {
            if (pattern.size() <= 8)
            {
                controller->rumble(pattern.c_str());
            }
            else
            {
                int loops = (int)ceil((float)pattern.size() / 8.0f);
                for (int i = 0; i < loops; i++)
                {
                    size_t start = i * 8;
                    size_t length = std::min<size_t>(8, pattern.size() - start);
                    std::string part = pattern.substr(start, length);
                    controller->rumble(part.c_str());
                }
            }
        }

        void update()
        {
            joysticks.Left.update();
            joysticks.Right.update();
            for (Button *b : btns)
            {
                if (b)
                {
                    b->update();
                }
            }
            
            #ifndef SKIP_OPTIONAL_CONTROLLER_UPDATES
                battery = static_cast<float>(controller->get_battery_level());
                connected = controller->is_connected();
            #endif
        }
    };
}