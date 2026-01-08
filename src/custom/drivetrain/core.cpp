#include "custom/drivetrain.hpp"
#include "custom/enums.hpp"
#include "pros/rtos.hpp"
namespace MKV5
{
    Drivetrain::Drivetrain(const std::vector<int8_t> &leftSide, const std::vector<int8_t> &rightSide, Units::Distance trc, double gearRatio, std::optional<pros::Imu *> i) : leftMotors(leftSide), rightMotors(rightSide), trackWidth(trc.inches()), imu(i.value_or(nullptr)), gearRatio(gearRatio)
    {
        pros::Task task = pros::Task(_drivetrain_tick, (void*)this);
        
        doCalibration();
    }
    void Drivetrain::doCalibration()
    {
        if (imu != nullptr)
        {
            //imu->reset(false);
        }
        leftMotors.doCalibration();
        rightMotors.doCalibration();
        if (imu != nullptr) {
        while (imu->is_calibrating())
        {
            pros::delay(20);
        }
        }
    }
    void Drivetrain::_drivetrain_tick(void* i) {
        Drivetrain* self = (Drivetrain*)i; 
        while (true) {
            if (self->isDriving > 0) {
                if (pros::millis() - self->isDriving > self->timeout) {
                    self->stopDrive();
                    self->isDriving = 0;
                }
            }
            pros::delay(20);
        }
    }
}