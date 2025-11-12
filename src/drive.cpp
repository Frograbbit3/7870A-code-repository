#include "main.h"
#include "init.hpp"
#include "custom/controls.hpp"
#include "custom/enums.hpp"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include <functional>
#include <numeric> 





///Called every 20ms while currently in the driver period.
void controllerTick() {
	while (true) {
		mainControl.update();
		pros::delay(20);
	}
}
