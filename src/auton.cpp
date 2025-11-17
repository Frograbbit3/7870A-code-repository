#include "init.hpp"

void autonomousTick() {
    drivetrain.moveDistance(32.0f, DIST_INCHES);
    drivetrain.rotateTo(90.0f);
}