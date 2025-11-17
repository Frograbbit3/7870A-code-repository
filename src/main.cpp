#include "main.h"
#include "init.hpp"


void autonomousTick();
void controllerTick();

void disabled() {
    mainControl.configuration.ENABLED = false;
}
void competition_initialize() {}

void autonomous() {
    mainControl.configuration.ENABLED = false;
    autonomousTick();
}

void opcontrol() {
    mainControl.configuration.ENABLED = true;
    controllerTick();
}
