#include "custom/enums.hpp"
#include "init.hpp"
#include "pros/rtos.hpp"


#define piston(v) pros::c::adi_digital_write(2, v)
#define wait(time) pros::delay(time*1000)
#define inttake(val) secondFlywheel->move(DRIVE_FORWARD, -127 * val);
void autonomousTick() {
    pros::c::adi_port_set_config(2, pros::E_ADI_DIGITAL_OUT);
    pros::c::adi_digital_write(2, false);
    
    //start outtake
    flywheel->move(DRIVE_FORWARD, 127);
    //move to the right goal
    drivetrain->moveDistance(34, MKV5::Enums::Distance::INCHES,DRIVE_FORWARD);
    //face goal
    drivetrain->rotateTo(90);
    drivetrain->moveDistance(30, DIST_INCHES,DRIVE_REVERSE);
    //prevents hitting so aggressively
    drivetrain->moveDistance(0.5,DIST_INCHES, DRIVE_FORWARD);
    inttake(true);

    //start going for match loader
    piston(true);
    drivetrain->moveDistance(50, DIST_INCHES, DRIVE_FORWARD);
    secondFlywheel->move(DRIVE_FORWARD, -127);
    wait(1.5);
    inttake(false);
    //go dispense
    drivetrain->moveDistance(50, DIST_INCHES, DRIVE_REVERSE);
    inttake(true);

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