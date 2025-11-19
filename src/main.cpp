#include "main.h"
#include "init.hpp"


void autonomousTick();
void controllerTick();

void disabled() {
    mainControl.configuration.enabled = false;
}
void competition_initialize() {}

void autonomous() {
    mainControl.configuration.enabled = false;
    drivetrain.calibrate();
    autonomousTick();
}

void opcontrol() {
    mainControl.configuration.enabled = true;
    controllerTick();
}
