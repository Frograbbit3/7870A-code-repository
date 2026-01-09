#include "custom/enums.hpp"
#include "init.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"

using pros::delay;
void autonomousTick() {

    drivetrain->waitForStationary(75_ms);


    std::cout << "I... AM RUNNING!" << std::endl;
    drivetrain->setVelocity(50, 50);
    //start outtake
    std::cout << "spin" << std::endl;
    flywheel->spin();
    //move to the right goal
    std::cout << "moving distance" << std::endl;
    drivetrain->moveDistance(34_in, FORWARD);
    //face goal
    drivetrain->rotateTo(90_dg);
    drivetrain->moveDistance(30_in, REVERSE);
    //prevents hitting so aggressively
    drivetrain->moveDistance(0.5_in, FORWARD);
    secondFlywheel->spin(SPIN_FORWARD);
    delay(2_s); //give it time to unload
    secondFlywheel->brake();

    //start going for match loader
    matchLoader->enable();
    drivetrain->moveDistance(50_in, FORWARD);
    flywheel->spin(SPIN_FORWARD);
    //go dispense
    drivetrain->moveDistance(50_in, REVERSE);
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