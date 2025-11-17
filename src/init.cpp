#include "init.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Imu gyro(11);
DrivetrainLib::Drivetrain drivetrain({-3,-21, -20}, {5, 9,10}, 32.0f, &gyro);
ControllerLib::EmulatedController control(&master);
ControllerLib::ControlScheme mainControl(CHEESE_DRIVE, drivetrain, control);
pros::Motor flywheel(7);

void OnR2Press() {
    flywheel.move(127);
}

void OnRelease() {
    flywheel.move(0);
}

void OnR1Press() {
    flywheel.move(-127);
}
void initialize() {
    // initialization logic
    control.buttons.R2.OnButtonPress(OnR2Press);
    control.buttons.R2.OnButtonRelease(OnRelease);
    control.buttons.R1.OnButtonPress(OnR1Press);
    control.buttons.R1.OnButtonRelease(OnRelease);
}
