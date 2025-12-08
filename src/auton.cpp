#include "custom/enums.hpp"
#include "init.hpp"
#include "pros/rtos.hpp"


#define piston(v) pros::c::adi_digital_write(2, v)
#define wait(time) pros::delay(time*1000)
void autonomousTick() {
    pros::c::adi_port_set_config(2, pros::E_ADI_DIGITAL_OUT);
    pros::c::adi_digital_write(2, false);
    drivetrain->moveDistance(40, MKV5::Enums::Distance::INCHES);
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