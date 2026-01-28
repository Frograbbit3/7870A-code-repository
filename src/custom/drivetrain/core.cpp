#include "custom/drivetrain.hpp"
#include "custom/enums.hpp"
#include "pros/rtos.hpp"
namespace MKV5
{
    Drivetrain::Drivetrain(const std::vector<int8_t> &leftSide, const std::vector<int8_t> &rightSide, Units::Distance trc, double gearRatio, std::optional<pros::Imu *> i) : leftMotors(leftSide), rightMotors(rightSide), trackWidth(trc.inches()), imu(i.value_or(nullptr)), gearRatio(gearRatio)
    {
        
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
    
}