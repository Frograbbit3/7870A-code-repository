#include "init.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Imu gyro(11);
MKV5::Drivetrain drivetrain({-3,-21, -20}, {5, 9,10}, 12.75f, &gyro);

MKV5::EmulatedController control(&master);
MKV5::ControlScheme mainControl(CHEESE_DRIVE, drivetrain, control);
pros::Motor f(7);
MKV5::CustomMotor flywheel(&f);

void initialize() {
    mainControl.registerMotor(MKV5::ControllerInputs::ControlBinding{
        .motor = &flywheel,
        .buttons = std::pair{
            control.buttons.R1,
            control.buttons.L1
        },
    }); //Register the flywheel hooked up to the R1 and L1 buttons

}
