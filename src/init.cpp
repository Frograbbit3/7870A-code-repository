#include "init.hpp"
#include "custom/controls.hpp"
#include "custom/emulated_controller.hpp"
#include "custom/enums.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include <utility>

//players
pros::Imu gyro(11);
MKV5::Drivetrain drivetrain({-4,-7, -20}, {5, 21,10}, 12.75f, &gyro);

MKV5::EmulatedController control(pros::E_CONTROLLER_MASTER);
MKV5::ControlScheme mainControl(CHEESE_DRIVE, drivetrain, control);
MKV5::CustomMotor flywheel(3);
MKV5::CustomMotor secondFlywheel(2);
MKV5::Piston matchLoader(1, true);


void initialize() {
    MKV5::ControllerInputs::ControlBinding fly{
        .motor = &flywheel,
        .buttons = std::pair{
            &control.buttons.L1,
            &control.buttons.L2
        },
    };//Register the flywheel hooked up to the R1 and L1 buttons
    MKV5::ControllerInputs::ControlBinding outtake {
        .motor = &secondFlywheel,
        .buttons = std::pair{
            &control.buttons.R1,
            &control.buttons.R2
        }
    };
    
    

}
