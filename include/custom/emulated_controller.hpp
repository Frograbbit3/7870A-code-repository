#pragma once
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include <cmath>
#include <cstdlib>
#include <vector>
#include <functional>

namespace ControllerLib
{
    class EmulatedController;
    struct Joystick
    {
        float X = 0;
        float Y = 0;
        float deadzone = 0.15f;
        bool process = true;
        bool moving = false;
        std::function<void(float, float)> OnMoveCallback = nullptr;
        pros::controller_analog_e_t stickX;
        pros::controller_analog_e_t stickY;
        pros::Controller *control = nullptr;

        Joystick() = default;
        Joystick(pros::Controller *ctrl,
                 pros::controller_analog_e_t stkX,
                 pros::controller_analog_e_t stkY)
            : stickX(stkX), stickY(stkY), control(ctrl) {}

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

        void SetStick(float nx, float ny)
        {   
            X = fabs(nx) > deadzone ? nx : 0.0f;
            Y = fabs(ny) > deadzone ? ny : 0.0f;
            moving = (fabs(X) > deadzone || fabs(Y) > deadzone); // FIXED: moving when OUTSIDE deadzone
            if (OnMoveCallback != nullptr) {OnMoveCallback(X, Y);}
        }

        void SetEnabled(bool enabled) { process = enabled; }

        void OnMove(const std::function<void(float, float)> &func)
        {
            OnMoveCallback = func;
        }
    };

    struct Button
    {
        bool pressed = false;
        bool process = true;
        bool triggered_press_callback = false;
        bool triggered_release_callback = false;

        pros::controller_digital_e_t button;
        pros::Controller *control = nullptr;
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

        void update()
        {
            if (!process || control == nullptr)
            {
                return;
            }
            SetButton(control->get_digital(button));
        }

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

        void SetEnabled(bool enabled) { process = enabled; }

        void OnButtonPress(const std::function<void()> &func)
        {
            OnPressCallback = func;
        }

        void OnButtonRelease(const std::function<void()> &func)
        {
            OnReleaseCallback = func;
        }
    };

    class EmulatedController
    {
    private:
        pros::Controller *controller;

    public:
        std::vector<Button*> btns;

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
            battery = static_cast<float>(controller->get_battery_level());
        }
    };
}