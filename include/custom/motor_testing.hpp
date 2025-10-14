#pragma once
#include "main.h"

bool testMotor(pros::Motor &mtr) {
    for (int i = 0; i < 20; ++i) {
        mtr.move( (i < 10) ? (i * 12) : ((20 - i) * 12));
        pros::delay(200);
        if (mtr.is_over_temp() || mtr.is_over_current()) {return false;}
        if (mtr.get_efficiency() < 65) {return false;}
        mtr.brake();
        pros::delay(50);
    } 
    return true;
}