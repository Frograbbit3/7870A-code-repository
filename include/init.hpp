#include "custom/controls.hpp"
#include "custom/drivetrain.hpp"
#include "custom/emulated_controller.hpp"
#include "custom/enums.hpp"
#include "main.h"
#pragma once


extern MKV5::Drivetrain* drivetrain;
extern MKV5::EmulatedController* control;
extern MKV5::ControlScheme* mainControl;
extern MKV5::CustomMotor* flywheel;
extern MKV5::CustomMotor* secondFlywheel;
extern pros::Imu* gyro;
extern MKV5::Piston* matchLoader;


#ifdef USE_EASY_MOVEMENT
    //Drivetrain
    #define drive drivetrain.moveDistance(
    #define turn drivetrain.rotateTo(drivetrain.getHeading()+
    #define degrees );
    #define inches ,DIST_INCHES)
    #define mm ,DIST_MM)
    #define forward , DIST_INCHES)
    #define reverse *-1, DIST_INCHES)
    #define face drivetrain.rotateTo(
    #define left 270)
    #define right 90)
    #define up 0)
    #define down 180)

    //variables
    #define heading drivetrain.getHeading()

    //controller
    #define controller mainControl.configuration.
    #define power enabled=
    #define powered enabled=
    #define on true
    #define off false

    //dummy
    #define set auto
    #define to =
#endif

#ifdef luamode
    #define var auto
    #define repeat(code) do {code}
    #define until(statement) while (statement);
    #define function auto

    
#endif