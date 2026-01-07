#include "init.hpp"
#include "custom/controls.hpp"
#include "custom/drivetrain.hpp"
#include "custom/emulated_controller.hpp"
#include "custom/enums.hpp"
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include <cstddef>
#include <utility>

//players

pros::Imu *gyro = nullptr;
MKV5::Drivetrain* drivetrain = nullptr;
MKV5::EmulatedController *control = nullptr;
MKV5::ControlScheme *mainControl = nullptr;
MKV5::CustomMotor* flywheel;
MKV5::CustomMotor* secondFlywheel;
MKV5::Piston* matchLoader;
void initialize() {
    gyro  = new pros::Imu(4);
    drivetrain  = new MKV5::Drivetrain({-9,-7, -20}, {5, 21,10}, 12.75f, 3/5, gyro);

    control= new MKV5::EmulatedController(pros::E_CONTROLLER_MASTER);
    mainControl = new MKV5::ControlScheme (CHEESE_DRIVE, *drivetrain, *control);
    flywheel = new MKV5::CustomMotor(3);
    secondFlywheel = new MKV5::CustomMotor(2);
    matchLoader = new MKV5::Piston(1, true);

}
