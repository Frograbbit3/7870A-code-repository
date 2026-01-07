#include "config.h"
#include "custom/drivetrain.hpp"
#include "custom/enums.hpp"
#include "pros/rtos.hpp"
#include <optional>

namespace MKV5 {
void Drivetrain::setLeftVelocity(int velocity) {
	leftMotors.setVelocity(minmax(static_cast<int>(velocity), -127, 127));
}
void Drivetrain::setRightVelocity(int velocity) {
	rightMotors.setVelocity(minmax(static_cast<int>(velocity), -127, 127));
}
void Drivetrain::moveDistance(
    MKV5::Units::Distance distance,
    std::optional<MKV5::Units::DirectionUnit> direction) {
	MKV5::Units::DistanceUnit dist = MKV5::Units::DistanceUnit::INCHES;
	MKV5::Units::DirectionUnit dir;

	if (!direction.has_value()) {
		if (distance.inches() < 0) {
			dir = MKV5::Units::DirectionUnit::REVERSE;
		} else if (distance.inches() > 0) {
			dir = MKV5::Units::DirectionUnit::FORWARD;
		} else {
			dir = MKV5::Units::DirectionUnit::STOP;
		}
	}
	else{
		dir = direction.value();
	}

#ifdef DO_DISTANCE_TRACKING
	double wheelRad = getWheelDiameter(leftMotors.group[0].getWheelType()) /
	                  2.0; // Convert diameter to radius
	double rotations = distance.inches() / (D_PI * wheelRad);
	double motorRotations = (rotations * gearRatio) * 360.0f;

	leftMotors.setVelocity(leftMotors.velocity);
	rightMotors.setVelocity(rightMotors.velocity);
	double aimL = leftMotors.getRotation() + motorRotations;
	double aimR = rightMotors.getRotation() + motorRotations;

	while (true) {
		double errL = aimL - leftMotors.getRotation();
		double errR = aimR - rightMotors.getRotation();

		if (std::abs(errL) <= 5 && std::abs(errR) <= 5)
			break;

		if (std::abs(errL) > 5)
			leftMotors.startMove(direction.value());
		else
			leftMotors.stopMove();

		if (std::abs(errR) > 5)
			rightMotors.startMove(direction.value());
		else
			rightMotors.stopMove();

		antiDrift();
		pros::delay(10);
	}

	leftMotors.stopMove();
	rightMotors.stopMove();

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
void Drivetrain::turn(MKV5::Units::Angle heading, MKV5::Units::RotationUnit direction) {
	double h = std::fmod(getHeading() + heading.dg(), 360.0);
	if (h < 0) h += 360.0;

	rotateTo(MKV5::Units::Angle{h}, direction);
}
void Drivetrain::rotateTo(MKV5::Units::Angle heading, MKV5::Units::RotationUnit direction) {
	double difference = getHeading() - heading.dg();
	double rot = std::fmod(heading.dg() - getHeading() + 540.0, 360.0) - 180.0;
	Units::DirectionUnit dir;
	const int MAX_VELOCITY = 25;
	const int MIN_VELOCITY = 5;
	const int TIMEOUT = 50000;
	int elapsedTime = 0;


	switch (direction) {
		case MKV5::Units::RotationUnit::CLOCKWISE:
			leftMotors.startMove(Units::DirectionUnit::FORWARD);
			rightMotors.startMove(Units::DirectionUnit::REVERSE);
			break;
		case MKV5::Units::RotationUnit::COUNTER_CLOCK:
			leftMotors.startMove(Units::DirectionUnit::REVERSE);
			rightMotors.startMove(Units::DirectionUnit::FORWARD);
			break;
		default:
			if (rot > 0) {
				leftMotors.startMove(Units::DirectionUnit::FORWARD);
				rightMotors.startMove(Units::DirectionUnit::REVERSE);
			}else{
				leftMotors.startMove(Units::DirectionUnit::REVERSE);
				rightMotors.startMove(Units::DirectionUnit::FORWARD);
			}
			break;
	}

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