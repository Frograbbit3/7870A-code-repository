#pragma once
#include "enums.hpp"
#include "mathlib.h"
#include "pros/motors.hpp"
#include <cmath>

#define time pros::millis()
class MotorGroup {
    private:
        int stopped = 0;
    public:
        std::vector<DrivetrainEnums::CustomMotor> group;
        std::vector<int8_t> ports;
        int velocity = 0;

        MotorGroup(const std::vector<int8_t>& ports_) : ports(ports_) {
            for (int8_t port : ports) {
                group.emplace_back(pros::Motor(port));
            }
        }


        /*
            Sets the velocity.
            vel -> -127 to 127
        */
        void setVelocity(int vel) {
            velocity = minmax(vel, -127, 127);
        }

        void move(DrivetrainEnums::Direction& dir) {
            for (DrivetrainEnums::CustomMotor& mtr : group) {
                switch (dir) {
                    case DRIVE_FORWARD:
                        mtr.move(velocity);
                        break;
                    case DRIVE_REVERSE:
                        mtr.move(-velocity);
                        break;
                    case DRIVE_STOP:
                        brake();
                        break;
                }
            }
        }
        void brake() {
            for (DrivetrainEnums::CustomMotor& mtr : group) {
                mtr.brake();
                mtr.move(0);
                //stopped=time;
            }
        }
        double getRotation() {
            double v = 0.0f;
            for (DrivetrainEnums::CustomMotor& mtr : group) {
                v+=pros::c::motor_get_position(mtr.get_port());
            }
            return (v / group.size()/2);
        }

        double getDistance(DrivetrainEnums::Distance dst){
            switch (dst)
            {
            case DrivetrainEnums::Distance::INCHES:
                return M_PI*getRotation()*properties.WHEEL_SIZE;            
            case DrivetrainEnums::Distance::MM:
                return INCH_TO_MM(M_PI*getRotation()*properties.WHEEL_SIZE);
            case DrivetrainEnums::Distance::ROTATION:
                return getRotation();
            }
        }

        void moveRelative(int angle, int voltage) {
            for (DrivetrainEnums::CustomMotor& mtr : group) {
                mtr.move_relative(angle, voltage);
            }
        }

        /*
            Attempts to move the motor group a certain distance. This uses rough estimation, so it's not accurate.
            TODO: Use sensors.
        */
        void moveDistance(float distance, DrivetrainEnums::Distance dst) {
            int rot = 0;
            switch (dst)
            {
            case DrivetrainEnums::Distance::INCHES:
                moveRelative(distance/(M_PI*properties.WHEEL_SIZE), velocity);     
                break;     
            case DrivetrainEnums::Distance::MM:
                moveRelative(INCH_TO_MM(distance)/(M_PI*properties.WHEEL_SIZE), velocity);
                break;
            case DrivetrainEnums::Distance::ROTATION:
                moveRelative(distance, velocity);
                break;
            }
        }
        void update() {
            if (stopped > 0) {
                if (time - stopped > properties.STOP_COOLDOWN) {
                    for (DrivetrainEnums::CustomMotor& mtr: group) {
                        mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
                    }
                    stopped = 0;
                }else{
                    for (DrivetrainEnums::CustomMotor& mtr: group) {
                    
                       // mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                    }
                }
            }

        }
};