#include "custom/emulated_controller.hpp"


using namespace MKV5::ControllerInputs;
namespace MKV5
{

    EmulatedController::EmulatedController(pros::Controller *ctrl)
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
    void EmulatedController::vibrate(std::string pattern)
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
    void EmulatedController::update()
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
}