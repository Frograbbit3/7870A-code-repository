#include "custom/drivetrain.hpp"
#include "main.h"
#pragma once


extern pros::Controller master;
extern MKV5::Drivetrain drivetrain;
extern MKV5::EmulatedController control;
extern MKV5::ControlScheme mainControl;
extern MKV5::CustomMotor flywheel;
extern pros::Imu gyro;


#ifdef USE_EASY_MOVEMENT
    #define forward(movement) (drivetrain.moveDistance((movement), DIST_INCHES))
    #define reverse(movement) (drivetrain.moveDistance(-(movement), DIST_INCHES))
    #define turn(degrees) (drivetrain.rotateTo(drivetrain.getHeading() + (degrees)))
    #define set_heading(degrees) (drivetrain.rotateTo(degrees))
    #define left() (drivetrain.rotateTo(270))
    #define right() (drivetrain.rotateTo(90))
    #define up() (drivetrain.rotateTo(0))
    #define down() (drivetrain.rotateTo(180))
#endif