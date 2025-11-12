#include "init.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);
DrivetrainLib::Drivetrain drivetrain({-1, -5, -20}, {11, 12, 13});
ControllerLib::EmulatedController control(&master);
ControllerLib::ControlScheme mainControl(ARCADE_DRIVE, drivetrain, control);

void initialize() {
    // initialization logic
}
