#pragma once
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include <vector>
#include <functional>

class EmulatedController
{
private:
    pros::Controller *controller;

    struct Button;
    struct Joystick;
    std::vector<Button *> btns;

    struct Joystick
    {
        float x = 0;
        float y = 0;
        float deadzone = 0.15f;
        bool process = true;
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
            if (static_cast<float>(control->get_analog(stickX)) != x || static_cast<float>(control->get_analog(stickY)) != y)
            {
                x = static_cast<float>(control->get_analog(stickX));
                y = static_cast<float>(control->get_analog(stickY));
                if (OnMoveCallback != nullptr)
                {
                    OnMoveCallback(x, y);
                }
            }
            else
            {
                x = static_cast<float>(control->get_analog(stickX));
                y = static_cast<float>(control->get_analog(stickY));
            }
        }
        /*
            Will manually set the values of the stick. Note that this will only persist for one frame, to keep it going make sure to SetEnabled(false)
            @param nx A float from -1.0 to 1.0 representing left and right position.
            @param ny A float from -1.0 to 1.0. representing up and down position.
        */
        void SetStick(float nx, float ny)
        {
            x = nx;
            y = ny;
            if (OnMoveCallback != nullptr)
            {
                OnMoveCallback(x, y);
            }
        }

        /*Changes normal control of the joystick*/
        void SetEnabled(bool enabled) { process = enabled; }

        /*
            Register a function to trigger whenever the controller joystick changes.
            @param func An std::function which takes in two float params (stickX and stickY).
        */
        void OnMove(const std::function<void(float, float)> &func)
        {
            OnMoveCallback = func;
        }
    };

    struct Button
    {
        bool pressed = false;
        bool process = true;

        pros::controller_digital_e_t button;
        pros::Controller *control = nullptr;
        EmulatedController *parent = nullptr;
        std::function<void()> OnPressCallback = nullptr;
        std::function<void()> OnReleaseCallback = nullptr;

        Button() = default; // Add this line

        Button(EmulatedController *parent_,
               pros::Controller *ctrl,
               pros::controller_digital_e_t btn)
            : button(btn), control(ctrl), parent(parent_)
        {
            if (parent)
                parent->btns.push_back(this);
        }

        void update()
        {
            if (!process || control == nullptr)
            {
                return;
            }
            SetButton(control->get_digital(button));
        }

        /*Will set the button's manual press. Make sure to run SetEnabled(false) or else this will only apply for one frame.*/
        void SetButton(bool state)
        {
            pressed = state;
            if (pressed)
            {
                if (OnPressCallback != nullptr)
                {
                    OnPressCallback();
                }
            }
            else
            {
                if (OnReleaseCallback != nullptr)
                {
                    OnReleaseCallback();
                }
            }
        }

        /*Enables or disables the button.*/
        void SetEnabled(bool enabled) { process = enabled; }

        /*
            Register a function to trigger whenever the button is pressed.
        */
        void OnButtonPress(const std::function<void()> &func)
        {
            OnPressCallback = func;
        }
        /*
            Register a function to trigger whenever the button is released.
        */
        void OnButtonRelease(const std::function<void()> &func)
        {
            OnReleaseCallback = func;
        }
    };

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

    struct {
        Joystick Left;
        Joystick Right;
    } joysticks;
    double battery=100;
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