#pragma once
#include <iostream>
inline double INCH_TO_MM(double inch) { return 25.4 * inch; }
inline double MM_TO_INCH(double mm) { return mm / 25.4; }
template <typename T>
inline void print(T m) { std::cout << m << std::endl; }

#define __VERSION 1.0b
namespace DrivetrainEnums
{
    struct WheelProperties
    {
        double GEAR_RATIO = 1.0;
        double WHEEL_SIZE = 3.5;
        double STOP_COOLDOWN = 1.0;
    };

    struct DrivetrainSettings
    {
        bool AUTO_DRIVE_ENABLED = true;
    };

    struct MotorProperties
    {
        int SET_VELOCITY = 0; // in mV
        bool IS_DRIVING = false;
        float ACTUAL_VELOCITY = 0; // updated by internal_telementry_collector
    };

    enum class Distance
    {
        INCHES = 1,
        MM = 0,
        ROTATION = 2,

    };
    enum class Direction
    {
        FORWARD = 1,
        REVERSE = -1,
        STOP = 0
    };

} // namespace DrivetrainEnums

// controller stuff

namespace ControllerEnums
{
    enum class ControllerDriveTypes
    {
        DRIVE_MODE_TANK = 1,
        DRIVE_MODE_ARCADE = 2
    };

    struct ControllerSettings
    {
        ControllerDriveTypes CONTROL_SCHEME = ControllerEnums::ControllerDriveTypes::DRIVE_MODE_ARCADE;
        float MAX_TURN_SPEED = -0.6f;
        float MAX_FORWARD_SPEED = 0.8f;
        float DEADZONE = 10.0f;
        bool DRIVE_AUTO_CORRECTION = false;
        bool ENABLED = true;
        double timeSinceJoystickStop = 0.0f;
    };
}

#define TANK_DRIVE ControllerEnums::ControllerDriveTypes::DRIVE_MODE_TANK
#define ARCADE_DRIVE ControllerEnums::ControllerDriveTypes::DRIVE_MODE_ARCADE
#define DRIVE_FORWARD DrivetrainEnums::Direction::FORWARD
#define DRIVE_REVERSE DrivetrainEnums::Direction::REVERSE
#define DRIVE_STOP DrivetrainEnums::Direction::STOP
