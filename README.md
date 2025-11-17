# Vex Robotics Team 7870A Drive / Auton Code
### Current Version: 2.0

Welcome to my team's robotics code! This is a generalized system mainly designed for my (moakdoge) projects, not for public use. I will attempt to document it, but it will suck. Do not use this, especially not in the current state.

## Features (As of November 17, 2025):
  ### Drivetrain
  ```cpp
    //Left side, right side, track width (inches). Optional gyro as the 4th argument.
    DrivetrainLib drivetrain({1,2,3}, {4,5,6}, 320.0f);
    int main() {
      //DIST_INCHES & DIST_MM;
      drivetrain.driveDistance(32.0f, DIST_INCHES);
      drivetrain.driveDistance(1000.0f, DIST_MM);
      //Rotates to a heading.
      drivetrain.rotateTo(90.0f);
    }
  ```
  - A custom drivetrain class with a VERY simple API. Maybe even too simple.
  - Internally handles any anti drift, wheel sizes (defaults to 3.25in), and distance calculations.
  

  ### Controller
  ```cpp
  pros::Controller master(pros::E_CONTROLLER_MASTER);
  pros::Imu gyro(11);
  DrivetrainLib::Drivetrain drivetrain({-3,-21, -20}, {5, 9,10}, 32.0f, &gyro);
  ControllerLib::EmulatedController control(&master);
  ControllerLib::ControlScheme mainControl(ARCADE_DRIVE, drivetrain, control);
  ```
  - A custom created abstracted controller with basic macroing support (```.SetButton, .SetStick```)
  - A ControlScheme which is premade controller inputs and supports hot swapping (with code)

> This project is **NOT** designed to be used by others! I'm publishing this purely to release my code to my team easier. Maybe soon I'll properly document this project.

## Changelog
    1.0 - October 23, 2025; Release
    2.0 - November 17, 2025; Complete code rewrite. 
