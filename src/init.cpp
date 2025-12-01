#include "init.hpp"
#include "custom/controls.hpp"
#include "custom/emulated_controller.hpp"
#include "custom/enums.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"

//players
pros::Imu gyro(11);
MKV5::Drivetrain drivetrain({-3,-21, -20}, {5, 9,10}, 12.75f, &gyro);

MKV5::EmulatedController control(pros::E_CONTROLLER_MASTER);
MKV5::ControlScheme mainControl(ARCADE_DRIVE, drivetrain, control);
MKV5::CustomMotor flywheel(4);
MKV5::CustomMotor secondFlywheel(8);


void initialize() {
    MKV5::ControllerInputs::ControlBinding fly{
        .motor = &flywheel,
        .buttons = std::pair{
            &control.buttons.R1,
            &control.buttons.R2
        },
    };//Register the flywheel hooked up to the R1 and L1 buttons

    mainControl.registerMotor(fly);

}
