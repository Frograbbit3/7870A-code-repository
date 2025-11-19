#include "custom/controls.hpp"

namespace ControllerLib
{
    ControlScheme::ControlScheme(ControllerEnums::ControllerDriveTypes typ, DrivetrainLib::Drivetrain &driveRef, EmulatedController &controllerRef): drive(driveRef), controller(controllerRef) {
        configuration.CONTROL_SCHEME = typ;
        LeftJoystick = &controller.joysticks.Left;
        RightJoystick = &controller.joysticks.Right;
    }
    void ControlScheme::_updateAccumulators()
    {
        if (negInertiaAccumlator > 1)
        {
            negInertiaAccumlator -= 1;
        }
        else if (negInertiaAccumlator < -1)
        {
            negInertiaAccumlator += 1;
        }
        else
        {
            negInertiaAccumlator = 0;
        }

        if (quickStopAccumlator > 1)
        {
            quickStopAccumlator -= 1;
        }
        else if (quickStopAccumlator < -1)
        {
            quickStopAccumlator += 1;
        }
        else
        {
            quickStopAccumlator = 0.0;
        }
    }
    void ControlScheme::registerMotor(ControlBinding binding)
    {
        if (std::holds_alternative<DrivetrainEnums::CustomMotor *>(binding.motor))
        {
            auto m = std::get<DrivetrainEnums::CustomMotor *>(binding.motor);
            if (std::holds_alternative<ControllerLib::Button>(binding.buttons))
            {
                auto &btns = std::get<ControllerLib::Button>(binding.buttons);
                // we have a single button
                if (binding.toggle)
                {
                    auto onpress = [m, &binding]()
                    {
                        if (m->getMovement())
                        {
                            m->move(DRIVE_STOP);
                        }
                        else
                        {
                            m->move(DRIVE_FORWARD, binding.speed);
                        }
                    };
                    btns.OnButtonPress(onpress);
                }
                else
                {
                    auto onpress = [&m, &binding]()
                    {
                        m->move(DRIVE_FORWARD, binding.speed);
                    };
                    auto onrelease = [&m, &binding]()
                    {
                        m->move(DRIVE_STOP, binding.speed);
                    };
                    btns.OnButtonPress(onpress);
                    btns.OnButtonRelease(onrelease);
                }
            }
            else
            {
                if (std::holds_alternative<std::pair<ControllerLib::Button, ControllerLib::Button>>(binding.buttons))
                {
                    auto &btns = std::get<std::pair<ControllerLib::Button, ControllerLib::Button>>(binding.buttons);
                    // we have two buttons
                    // toggle doesnt work on this so
                    auto onforwardpress = [&m, &binding]()
                    {
                        m->move(DRIVE_FORWARD, binding.speed);
                    };
                    auto onbackwardspress = [&m, &binding]()
                    {
                        m->move(DRIVE_REVERSE, binding.speed);
                    };
                    auto onrelease = [&m, &binding]()
                    {
                        m->move(DRIVE_STOP, binding.speed);
                    };
                    btns.first.OnButtonPress(onforwardpress);
                    btns.second.OnButtonPress(onbackwardspress);
                    btns.first.OnButtonRelease(onrelease);
                    btns.second.OnButtonRelease(onrelease);
                }
            }
        }
        else
        {
            auto &mg = std::get<MotorGroup *>(binding.motor);
            // use mg
        }
    }
    std::pair<int, int> ControlScheme::doControllerInputs(float LeftJoystickX, float LeftJoystickY, float RightJoystickX, float RightJoystickY)
    {
        switch (configuration.CONTROL_SCHEME)
        {
        case ARCADE_DRIVE:
            leftVelocity = static_cast<int>(RightJoystickX * -(configuration.maxTurnSpeed)) - static_cast<int>(-LeftJoystickY * (configuration.maxForwardSpeed));
            rightVelocity = static_cast<int>(RightJoystickX * -(configuration.maxTurnSpeed)) + static_cast<int>(-LeftJoystickY * (configuration.maxForwardSpeed));
            break;
        case TANK_DRIVE:
            leftVelocity = static_cast<int>(LeftJoystickY * (configuration.maxForwardSpeed));
            rightVelocity = static_cast<int>(RightJoystickY * -(configuration.maxForwardSpeed));
            break;
        case GTA_DRIVE:
        {
            int forward = (controller.buttons.R2.pressed ? 1 : 0) -
                          (controller.buttons.L2.pressed ? 1 : 0);
            int turn = static_cast<int>(LeftJoystickX);

            leftVelocity = static_cast<int>(
                (forward * (configuration.maxForwardSpeed)) -
                (turn * (configuration.maxTurnSpeed)));

            rightVelocity = static_cast<int>(
                (forward * (configuration.maxForwardSpeed)) +
                (turn * (configuration.maxTurnSpeed)));
            break;
        }

        /*Credit where credit is due: made by Purdue ACM SIGBots. They also wrote the joystick curve I used.*/
        case CHEESE_DRIVE:
        {
            double ithrottle = (LeftJoystickY * configuration.maxForwardSpeed) / 127.0;
            double iturn = (RightJoystickX * configuration.maxTurnSpeed) / 127.0;
            double linearCmd = ithrottle;
            bool turnInPlace = false;

            if (fabs(ithrottle) < 0.1 && fabs(iturn) > 0.1)
            {
                linearCmd = 0.0;
                turnInPlace = true;
            }
            else if (ithrottle - prevThrottle > DRIVE_SLEW)
            {
                linearCmd = prevThrottle + DRIVE_SLEW;
            }
            else if (ithrottle - prevThrottle < -(DRIVE_SLEW * 2))
            {
                linearCmd = prevThrottle - (DRIVE_SLEW * 2);
            }

            double remappedTurn = iturn;
            double left, right;

            if (turnInPlace)
            {
                left = remappedTurn * fabs(remappedTurn);
                right = -remappedTurn * fabs(remappedTurn);
            }
            else
            {
                double negInertiaPower = (iturn - prevTurn) * CD_NEG_INERTIA_SCALAR;
                negInertiaAccumlator += negInertiaPower;

                double angularCmd =
                    fabs(linearCmd) *
                        (remappedTurn + negInertiaAccumlator) *
                        CD_SENSITIVITY -
                    quickStopAccumlator;

                right = left = linearCmd;
                left += angularCmd;
                right -= angularCmd;

                _updateAccumulators();
            }

            // scale to [-127, 127]
            leftVelocity = static_cast<int>(std::clamp(left, -1.0, 1.0) * 127);
            rightVelocity = static_cast<int>(std::clamp(right, -1.0, 1.0) * 127);

            prevTurn = iturn;
            prevThrottle = ithrottle;
            break;
        }
        }
        return std::make_pair(leftVelocity, rightVelocity);
    }
    void ControlScheme::update()
    {
        controller.update(); // For the EmulatedController
        if (!configuration.enabled)
        {
            return;
        }

        // Read joystick raw values through the pointer members and apply curve
        float LeftJoystickX = JoystickCurve(LeftJoystick->X);
        float LeftJoystickY = JoystickCurve(LeftJoystick->Y);
        float RightJoystickX = JoystickCurve(RightJoystick->X);
        float RightJoystickY = JoystickCurve(RightJoystick->Y);

        std::pair<int, int> velocities = doControllerInputs(LeftJoystickX, LeftJoystickY, RightJoystickX, RightJoystickY);
        int leftVelocity = velocities.first;
        int rightVelocity = velocities.second;

        if (fabs(leftVelocity) > 0 || fabs(rightVelocity) > 0)
        {
            leftVelocity = minmax(static_cast<int>(leftVelocity), -127, 127);
            rightVelocity = minmax(static_cast<int>(rightVelocity), -127, 127);
            drive.setVelocity(leftVelocity, rightVelocity);
            drive.drive(DRIVE_FORWARD);
        }
        else
        {
            configuration.timeSinceJoystickStop = pros::millis();
            drive.stop();
        }
        fr++;
    }
}