#include "custom/enums.hpp"
#include "init.hpp"


void autonomousTick() {
    drivetrain.moveDistance(50, DIST_INCHES);
    drivetrain.rotateTo(90);
}