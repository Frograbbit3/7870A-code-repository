#include "main.h"
#include "init.cpp"
#include "custom/controls.hpp"
#include "custom/enums.hpp"
#include "pros/screen.hpp"
#include <functional>
#include <numeric> 




void opcontrol() {
	//new comment
	DrivetrainLib::Drivetrain drivetrain ({-1,-5,-20}, {-9,-10,-21}); //put motor ports here
	ControllerLib::EmulatedController control(&master);
	ControllerLib::ControlScheme mainControl(
		ARCADE_DRIVE,
		drivetrain,
		control
	);
	int frame = 0;
	while (true) {
		mainControl.update();
		frame++;
		pros::delay(20);
	}
}
