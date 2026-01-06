#include "custom/enums.hpp"
#include "init.hpp"
#include "pros/rtos.hpp"


#define piston(v) pros::c::adi_digital_write(2, v)
#define wait(time) pros::delay(time)
#define inttake(val) secondFlywheel->move(DRIVE_FORWARD, -127 * val);
void autonomousTick() {
    pros::c::adi_port_set_config(2, pros::E_ADI_DIGITAL_OUT);
    pros::c::adi_digital_write(2, false);
    
    //start outtake
    flywheel->move(DRIVE_FORWARD, 127);
    //move to the right goal
    drivetrain->moveDistance(34_in);
    //face goal
    drivetrain->rotateTo(90_dg);
    drivetrain->moveDistance(30_in);
    //prevents hitting so aggressively
    drivetrain->moveDistance(-0.5_in);
    inttake(true);

    //start going for match loader
    piston(true);
    drivetrain->moveDistance(50_in);
    secondFlywheel->move(DRIVE_FORWARD, -127);
    wait(1.5_ms);
    inttake(false);
    //go dispense
    drivetrain->moveDistance(50_in);
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