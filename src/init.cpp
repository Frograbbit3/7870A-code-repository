#include "init.hpp"
#include "custom/controls.hpp"
#include "custom/emulated_controller.hpp"
#include "custom/enums.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"

//players
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Imu gyro(11);
MKV5::Drivetrain drivetrain({-3,-21, -20}, {5, 9,10}, 12.75f, &gyro);

MKV5::EmulatedController control(&master);
MKV5::ControlScheme mainControl(ARCADE_DRIVE, drivetrain, control);
pros::Motor f(7);
pros::Motor s(8);
MKV5::CustomMotor flywheel(&f);
MKV5::CustomMotor secondFlywheel(&s);

void initialize() {
    MKV5::ControllerInputs::ControlBinding fly{
        .motor = &flywheel,
        .buttons = std::pair{
            control.buttons.R1,
            control.buttons.L1
        },
    };//Register the flywheel hooked up to the R1 and L1 buttons
     MKV5::ControllerInputs::ControlBinding secondB{
        .motor = &secondFlywheel,
        .buttons = std::pair{
            control.buttons.R2,
            control.buttons.L2
        },
    };//Register the flywheel hooked up to the R1 and L1 buttons

}
