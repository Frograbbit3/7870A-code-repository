#pragma once
#include "enums.hpp"
#include "mathlib.h"
#include "pros/motors.hpp"
#include <cmath>

#define time pros::millis()
class MotorGroup
{
private:
    int stopped = 0;

public:
    std::vector<DrivetrainEnums::CustomMotor> group;
    std::vector<int8_t> ports;
    int velocity = 0;

    MotorGroup(const std::vector<int8_t> &ports_);

    /*
        Sets the velocity.
        vel -> -127 to 127
    */
    void setVelocity(int vel);
    void moveRelative(double angle);
    void startMove(DrivetrainEnums::Direction &dir);
    void stopMove();
    void doCalibration();
    double getRotation();
    bool isMoving();
    void update();
};