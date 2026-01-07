#include "config.h"
#include "custom/drivetrain.hpp"
#include "custom/enums.hpp"
#include "pros/rtos.hpp"

namespace MKV5 {
void Drivetrain::setLeftVelocity(int velocity) {
	leftMotors.setVelocity(minmax(static_cast<int>(velocity), -127, 127));
}
void Drivetrain::setRightVelocity(int velocity) {
	rightMotors.setVelocity(minmax(static_cast<int>(velocity), -127, 127));
}
void Drivetrain::moveDistance(MKV5::Units::Distance distance) {
	MKV5::Units::DistanceUnit dist = MKV5::Units::DistanceUnit::INCHES;
	MKV5::Units::DirectionUnit dir;

	if (distance.inches() < 0) {
		dir = MKV5::Units::DirectionUnit::REVERSE;
	} else if (distance.inches() > 0) {
		dir = MKV5::Units::DirectionUnit::FORWARD;
	} else {
		dir = MKV5::Units::DirectionUnit::STOP;
	}

#ifdef DO_DISTANCE_TRACKING
	double wheelRad = getWheelDiameter(leftMotors.group[0].getWheelType()) /
	                  2.0; // Convert diameter to radius
	double rotations = distance.inches() / (D_PI * wheelRad);
	double motorRotations = (rotations * gearRatio) * 360.0f;

	std::cout << "DISTANCE:" << rotations << std::endl;
	leftMotors.setVelocity(55);
	rightMotors.setVelocity(55);
	leftMotors.moveRelative(motorRotations);
	rightMotors.moveRelative(motorRotations);
#else
	// approximate a time value for it
	double rpm = 450.0f;
	double rps = rpm / 60.0f; // 7.5 rps
	double wheelRadius = 1.625f;

	// actual math
	double distancePerRotation = (2 * M_PI * wheelRadius); // in inches
	// inches and rps
	double inchesPerSecond = rps * distancePerRotation;

	double secondsToDrive = distance.inches() / inchesPerSecond;
	double maxSpeed = 0.25f;
	secondsToDrive *= 1.18; // approx friction
	secondsToDrive /= maxSpeed;

	setVelocity(127 * maxSpeed, 127 * maxSpeed);
	uint32_t start = pros::millis();
	startDrive(dir);
	std::cout << "STARTING AUTODRIVE WITH LEGNTH OF " << secondsToDrive
	          << std::endl;
	while ((pros::millis() - start) < (secondsToDrive * 1000)) {
		pros::delay(10);
	}
	std::cout << "ENDING AUTODRIVE" << std::endl;
	stopDrive();
// TODO: Seconds / per inch
#endif
}
void Drivetrain::setVelocity(int leftVelocity, int rightVelocity) {
	setLeftVelocity(leftVelocity);
	setRightVelocity(rightVelocity);
}
void Drivetrain::rotateTo(MKV5::Units::Angle heading) {
	double difference = getHeading() - heading.dg();
	Units::DirectionUnit dir;
	const int MAX_VELOCITY = 25;
	const int MIN_VELOCITY = 5;
	const int TIMEOUT = 50000;
	int elapsedTime = 0;
	dir = Units::DirectionUnit::FORWARD;
	leftMotors.startMove(Units::DirectionUnit::FORWARD);
	rightMotors.startMove(Units::DirectionUnit::REVERSE);
	while (fabs(difference) > ROTATION_OFFSET_LIMIT &&
	       elapsedTime < TIMEOUT) {

		difference = getHeading() - heading.dg();

		double percent = fabs(difference) / 360.0;
		double vel = percent * MAX_VELOCITY;

		vel = minmax<double>(vel, MIN_VELOCITY, MAX_VELOCITY);
		leftMotors.setVelocity(127);
		rightMotors.setVelocity(127);

		if (elapsedTime % 200 == 0) {
			std::cout << "heading:" << getHeading() << std::endl;
		}
		pros::delay(25);
		elapsedTime += 25;
	}
	stopDrive();
}
void Drivetrain::stopDrive() {
	leftMotors.stopMove();
	rightMotors.stopMove();
}
void Drivetrain::startDrive(Units::DirectionUnit direction) {
	leftMotors.startMove(direction);
	rightMotors.startMove(direction);
}
} // namespace MKV5