#include "main.h"
#include "init.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"

void controllerTick() {
	while (pros::competition::is_field_control()) {
		mainControl.update();
		pros::delay(10);
	}
}
