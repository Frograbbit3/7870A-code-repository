#pragma once
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include <vector>
#include <functional>

class EmulatedController {
private:
    pros::Controller* controller;

    struct Button;
    struct Joystick;
    std::vector<Button*> buttons;

    struct Joystick {
        float x = 0;
        float y = 0;
        float deadzone = 0.15f;
        bool process = true;
        std::function<void(float, float)> OnMoveCallback=nullptr;
        pros::controller_analog_e_t stickX;
        pros::controller_analog_e_t stickY;
        pros::Controller* control = nullptr;

        Joystick(pros::Controller* ctrl,
                 pros::controller_analog_e_t stkX,
                 pros::controller_analog_e_t stkY)
            : stickX(stkX), stickY(stkY), control(ctrl) {}

        void update() {
            if (!process || control == nullptr) {return;}
            if (static_cast<float>(control->get_analog(stickX)) != x || static_cast<float>(control->get_analog(stickY)) != y) {
                x = static_cast<float>(control->get_analog(stickX));
                y = static_cast<float>(control->get_analog(stickY));
                if (OnMoveCallback!=nullptr){OnMoveCallback(x, y);}
            }else{
                x = static_cast<float>(control->get_analog(stickX));
                y = static_cast<float>(control->get_analog(stickY));
            }
        }
        /*
            Will manually set the values of the stick. Note that this will only persist for one frame, to keep it going make sure to SetEnabled(false)
            @param nx A float from -1.0 to 1.0 representing left and right position.
            @param ny A float from -1.0 to 1.0. representing up and down position.
        */
        void SetStick(float nx, float ny) {
            x=nx;
            y=ny;
            if (OnMoveCallback!=nullptr){OnMoveCallback(x, y);}
        }

        /*Changes normal control of the joystick*/
        void SetEnabled(bool enabled) {process=enabled;}

        /*
            Register a function to trigger whenever the controller joystick changes.
            @param func An std::function which takes in two float params (stickX and stickY).
        */
        void OnMove(const std::function<void(float, float)>& func) {
            OnMoveCallback = func;
        }
    };

    struct Button {
        bool pressed = false;
        bool process = true;
        pros::controller_digital_e_t button;
        pros::Controller* control = nullptr;
        EmulatedController* parent = nullptr;

        // Auto-register with parent
        Button(EmulatedController* parent_,
               pros::Controller* ctrl,
               pros::controller_digital_e_t btn)
            : button(btn), control(ctrl), parent(parent_) {
            if (parent) parent->buttons.push_back(this);
        }

        void update() {
            if (!process|| control == nullptr) {return;}
            pressed = control->get_digital(button);
        }
    };

public:
    Joystick LeftJoystick;
    Joystick RightJoystick;

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

    bool enabled=true;

    EmulatedController(pros::Controller* ctrl)
        : controller(ctrl), //i know it's so amazing
          LeftJoystick(ctrl, pros::E_CONTROLLER_ANALOG_LEFT_X,  pros::E_CONTROLLER_ANALOG_LEFT_Y),
          RightJoystick(ctrl, pros::E_CONTROLLER_ANALOG_RIGHT_X, pros::E_CONTROLLER_ANALOG_RIGHT_Y),
          A(this, ctrl, pros::E_CONTROLLER_DIGITAL_A),
          B(this, ctrl, pros::E_CONTROLLER_DIGITAL_B),
          Y(this, ctrl, pros::E_CONTROLLER_DIGITAL_Y),
          X(this, ctrl, pros::E_CONTROLLER_DIGITAL_X),
          Left(this, ctrl, pros::E_CONTROLLER_DIGITAL_LEFT),
          Right(this, ctrl, pros::E_CONTROLLER_DIGITAL_RIGHT),
          Up(this, ctrl, pros::E_CONTROLLER_DIGITAL_UP),
          Down(this, ctrl, pros::E_CONTROLLER_DIGITAL_DOWN),
          L1(this, ctrl, pros::E_CONTROLLER_DIGITAL_L1),
          L2(this, ctrl, pros::E_CONTROLLER_DIGITAL_L2),
          R1(this, ctrl, pros::E_CONTROLLER_DIGITAL_R1),
          R2(this, ctrl, pros::E_CONTROLLER_DIGITAL_R2),
          Power(this, ctrl, pros::E_CONTROLLER_DIGITAL_POWER) {
    }

    void update() {
        LeftJoystick.update();
        RightJoystick.update();
        for (Button* b : buttons) {
            if (b){
                b->update();
            } 
        }
    }
};