#include "custom/drivetrain.hpp"
namespace DrivetrainLib
{
    Drivetrain::Drivetrain(const std::vector<int8_t> &leftSide, const std::vector<int8_t> &rightSide, double trc, std::optional<pros::Imu *> i) : leftMotors(leftSide), rightMotors(rightSide), trackWidth(trc), imu(i.value_or(nullptr))
    {
        doCalibration();
    }
    void Drivetrain::doCalibration()
    {
        if (imu != nullptr)
        {
            imu->reset(false);
        }
        leftMotors.doCalibration();
        rightMotors.doCalibration();
        while (imu->is_calibrating())
        {
            pros::delay(20);
        }
    }
}