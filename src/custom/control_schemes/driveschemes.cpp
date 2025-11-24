#include "custom/controls.hpp"
namespace MKV5 {

    void ControlScheme::tankDrive()
    {
        leftVelocity = static_cast<int>(LeftJoystickY * (configuration.maxForwardSpeed));
        rightVelocity = static_cast<int>(RightJoystickY * -(configuration.maxForwardSpeed));
    };
    void ControlScheme::arcadeDrive()
    {
        leftVelocity = static_cast<int>(RightJoystickX * -(configuration.maxTurnSpeed)) - static_cast<int>(-LeftJoystickY * (configuration.maxForwardSpeed));
        rightVelocity = static_cast<int>(RightJoystickX * -(configuration.maxTurnSpeed)) + static_cast<int>(-LeftJoystickY * (configuration.maxForwardSpeed));
    };
    void ControlScheme::gtaDrive()
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
    };
    void ControlScheme::cheeseDrive()
    {
        double ithrottle = (LeftJoystick->Y * configuration.maxForwardSpeed) / 127.0;
        double iturn = (RightJoystick->X * configuration.maxTurnSpeed) / 127.0;
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
    };
}