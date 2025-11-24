#include "main.h"
#include "init.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"

void controllerTick() {
	while (true) {
		mainControl.update();
		pros::delay(10);
	}
}
