#include "custom/modded.hpp"
MotorGroup::MotorGroup(const std::vector<int8_t> &ports_) : ports(ports_)
{
    for (int8_t port : ports)
    {
        pros::Motor *tmp = new pros::Motor(port);
        group.emplace_back(tmp);
    }
}

void MotorGroup::setVelocity(int vel)
{
    velocity = minmax(vel, -127, 127);
    for (DrivetrainEnums::CustomMotor &mtr : group)
    {
        mtr.setVelocity(velocity, VELOCITY_RAW);
    }
}

void MotorGroup::move(DrivetrainEnums::Direction &dir)
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
void MotorGroup::brake()
{
    for (DrivetrainEnums::CustomMotor &mtr : group)
    {
        mtr.brake();
        // stopped=time;
    }
}

void MotorGroup::calibrate()
{
    for (DrivetrainEnums::CustomMotor &mtr : group)
    {
        mtr.calibrate();
    }
}

double MotorGroup::getRotation()
{
    double v = 0.0f;
    for (DrivetrainEnums::CustomMotor &mtr : group)
    {
        v += mtr.getPosition();
    }
    return (v / group.size() / 2);
}

bool MotorGroup::isMoving()
{
    for (DrivetrainEnums::CustomMotor &mtr : group)
    {
        if (mtr.getMovement())
        {
            return true;
        }
    }
    return false;
}

void MotorGroup::moveRelative(double angle)
{
    for (DrivetrainEnums::CustomMotor &mtr : group)
    {
        mtr.moveRelative(DRIVE_FORWARD, angle);
    }
}

void MotorGroup::update()
{
    if (stopped > 0)
    {
        for (DrivetrainEnums::CustomMotor &mtr : group)
        {
            mtr.setBrakeMode(pros::MotorBrake::coast);
        }
        stopped = 0;
    }
}