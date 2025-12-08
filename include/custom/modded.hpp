#pragma once
#include "enums.hpp"

#define time pros::millis()

namespace MKV5
{
    class MotorGroup
    {
    private:
        
        int stopped = 0;
        
    public:
        std::vector<MKV5::CustomMotor> group;
        std::vector<int8_t> ports;
        int velocity = 0;

        MotorGroup(const std::vector<int8_t> &ports_);

        /*
            Sets the velocity.
            @param vel A value from -127 to 127.
        */
        void setVelocity(int vel);
        /*
            Moves the motor `angle` degrees from where it is.
            @param angle The amount of degrees to turn.
        */
        void moveRelative(double angle, int vl);
        /*
            Starts moving indefinately until stopped with `stopMove`.
            @param dir The direction to move.
        */
        void startMove(MKV5::Enums::Direction dir);
        /*
            Stops all the motors.
        */
        void stopMove();
        /*
            Resets the motors' encoders. Does not block.
        */
        void doCalibration();
        /*
            Gets the average rotation of the motors.
        */
        double getRotation();
        /*
            Checks if the motors are spinning.
            @return If the motors are spinning.
        */
        bool isMoving();
        /*
            Processes misc events within the motor.
            @warning Don't need to call this, handled by drivetrain.
        */
        void update();
    };
}