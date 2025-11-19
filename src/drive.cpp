#include "main.h"
#include "init.hpp"

void controllerTick() {
	while (true) {
		mainControl.update();
		pros::delay(10);
	}
}
