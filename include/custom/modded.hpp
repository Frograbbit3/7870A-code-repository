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

    MotorGroup(const std::vector<int8_t> &ports_) : ports(ports_)
    {
        for (int8_t port : ports)
        {
            pros::Motor *tmp = new pros::Motor(port);
            group.emplace_back(tmp);
        }
    }

    /*
        Sets the velocity.
        vel -> -127 to 127
    */
    void setVelocity(int vel)
    {
        velocity = minmax(vel, -127, 127);
        for (DrivetrainEnums::CustomMotor& mtr: group) {
            mtr.setVelocity(velocity, VELOCITY_RAW);
        }
    }

    void move(DrivetrainEnums::Direction &dir)
    {
        for (DrivetrainEnums::CustomMotor &mtr : group)
        {
            switch (dir)
            {
            case DRIVE_FORWARD:
                mtr.move(DRIVE_FORWARD);
                break;
            case DRIVE_REVERSE:
                mtr.move(DRIVE_REVERSE);
                break;
            case DRIVE_STOP:
                brake();
                break;
            }
        }
    }
    void brake()
    {
        for (DrivetrainEnums::CustomMotor &mtr : group)
        {
            mtr.brake();
            // stopped=time;
        }
    }
    void calibrate() {
        for (DrivetrainEnums::CustomMotor &mtr : group)
        {
            mtr.calibrate();
        }
    }
    double getRotation()
    {
        double v = 0.0f;
        for (DrivetrainEnums::CustomMotor &mtr : group)
        {
            v += mtr.getPosition();
        }
        return (v / group.size() / 2);
    }

    void moveRelative(double angle)
    {
        for (DrivetrainEnums::CustomMotor &mtr : group)
        {
            mtr.moveRelative(DRIVE_FORWARD, angle);
        }
    }

    void update()
    {
        /*
        if (stopped > 0) {
            if (time - stopped >1) {
                for (DrivetrainEnums::CustomMotor& mtr: group) {
                    mtr.setBrakeMode(MOTOR_BRAKE_COAST);
                }
                stopped = 0;
            }else{
                for (DrivetrainEnums::CustomMotor& mtr: group) {

                   // mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                }
            }
        }
        */
        if (stopped > 0)
        {
            for (DrivetrainEnums::CustomMotor &mtr : group)
            {
                mtr.setBrakeMode(pros::MotorBrake::coast);
            }
            stopped = 0;
        }
    }
};