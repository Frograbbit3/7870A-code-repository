#include "custom/enums.hpp"
#include "main.h"
#include "init.hpp"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include <cmath>
#include <cstring>

bool tank_mode = false;
bool state = false;
void controllerTick() {
    int f= 0;
    int debounce = 0;
	while (true) {
		mainControl->update();
        mainControl->processBindings();
        /*
        if (control->buttons.L1.pressed || control->buttons.L2.pressed) {
            flywheel->setVelocity((control->buttons.L2.pressed-control->buttons.L1.pressed) * 127);
            flywheel->move(FORWARD);
        }else{
            flywheel->brake();
        }
        if (control->buttons.R1.pressed || control->buttons.R2.pressed) {
            secondFlywheel->setVelocity((control->buttons.R2.pressed-control->buttons.R1.pressed) * 127);
            secondFlywheel->move(FORWARD);
        }else{
            secondFlywheel->brake();
        }
        */

        if (control->buttons.A.pressed && debounce < 1) {
            control->vibrate(".");
            debounce = 90;
            matchLoader->toggleState();
        }
        debounce--;

        if (control->buttons.B.pressed) {
            tank_mode = !tank_mode;
            control->vibrate("-");
            while (control->buttons.B.pressed) {
                mainControl->update();
                pros::delay(20);
            }
        }
        
        mainControl->configuration.CONTROL_SCHEME = CHEESE_DRIVE;
        if (tank_mode)
            mainControl->configuration.CONTROL_SCHEME = TANK_DRIVE;
    
		pros::delay(10);
        f++;
	}
}
