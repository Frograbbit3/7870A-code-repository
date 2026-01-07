#include "custom/enums.hpp"
#include "init.hpp"
#include "pros/rtos.hpp"

using pros::delay;

void autonomousTick() {
    
    //start outtake
    flywheel->spin(SPIN_FORWARD);
    //move to the right goal
    drivetrain->moveDistance(34_in);
    //face goal
    drivetrain->rotateTo(90_dg);
    drivetrain->moveDistance(30_in);
    //prevents hitting so aggressively
    drivetrain->moveDistance(-0.5_in);
    secondFlywheel->spin(SPIN_FORWARD);

    //start going for match loader
    matchLoader->enable();
    drivetrain->moveDistance(50_in);
    secondFlywheel->spin(SPIN_FORWARD);
    delay(1.5_s);
    secondFlywheel->spin(SPIN_REVERSE);
    //go dispense
    drivetrain->moveDistance(50_in);
    secondFlywheel->spin(SPIN_FORWARD);

   // drivetrain->setVelocity(100, 100);
    //drivetrain->startDrive(DRIVE_FORWARD);
    
    /*
    
        drivetrain.moveDistance(12.0f, DIST_INCH);
        drivetrain.rotateTo(heading);

        drivetrain.leftMotors.move(DRIVE_FORWARD);
        drivetrain.rightMotors.move(DRIVE_FOWARD);

        wait(seconds);
        drivetrain.antiDrift();

        drivetrain.setVelocity(left, right); (-100 -> 100)
        drivetrain.setVelocity(left)


        flywheel.setVelocity(100, VELOCITY_PERCENT);
        flywheel.move(DRIVE_FOWARD);

        secondFlywheel.setVelocity()
        secondFlywheel.move()

        piston(true);
    */





    
}