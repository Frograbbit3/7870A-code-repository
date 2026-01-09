#include "custom/drivetrain.hpp"
#include "custom/enums.hpp"
#include "init.hpp"
#include "pros/imu.h"
#include "pros/rtos.hpp"
#include <cstdint>
#include <cstdlib>
#include <vector>
namespace MKV5 {
void Drivetrain::antiDrift() {
	double lastLeft = leftMotors.getRotation();
	double lastRight = rightMotors.getRotation();

	const double kP = 0.4;
	const double deadband = 0.01;

	while (leftMotors.isMoving() && rightMotors.isMoving()) {
		double leftNow = leftMotors.getRotation();
		double rightNow = rightMotors.getRotation();

		double dLeft = leftNow - lastLeft;
		double dRight = rightNow - lastRight;

		lastLeft = leftNow;
		lastRight = rightNow;

		double drift = dLeft - dRight;

		if (std::abs(drift) < deadband) {
			pros::delay(10);
			continue;
		}

		double correction = drift * kP;

		double leftVel = 100 - correction;
		double rightVel = 100 + correction;

		leftVel = minmax(leftVel, -127.0, 127.0);
		rightVel = minmax(rightVel, -127.0, 127.0);

		if (abs(leftVel) > 10 && abs(rightVel) > 10) {
			leftMotors.setVelocity(leftVel);
			rightMotors.setVelocity(rightVel);
		}
		pros::delay(10);
	}
}

double Drivetrain::getHeading() {

	if (imu != nullptr) // use the imu if it's there
	{
		double gyroAngle = imu->get_heading();

		return (gyroAngle);
	}

	double leftRotations = leftMotors.getRotation();
	double rightRotations = rightMotors.getRotation();
	double wheelRad =
	    getWheelDiameter(leftMotors.group[0].getWheelType()) / 2.0;

	double leftDistance = leftRotations * D_PI * wheelRad;
	double rightDistance = rightRotations * D_PI * wheelRad;

	double deltaDistance = rightDistance - leftDistance;
	double headingRadians = deltaDistance / trackWidth;
	double headingDegrees = headingRadians * (180.0 / pi);
	return headingDegrees;
}
bool Drivetrain::isStationary() {
	constexpr double MAX_OFFSET = 0.25;
	constexpr double MAX_TIME = 50; // in ms

	if (gyro == nullptr)
		return false;

	pros::imu_gyro_s_t rate = gyro->get_gyro_rate();
	double timeEllapsed = 0;

	if ((abs(rate.x) > MAX_OFFSET || abs(rate.y) > MAX_OFFSET ||
	     abs(rate.z) > MAX_OFFSET)) {
		stationaryRate = pros::millis();
		return false;
	}

	if (pros::millis() - stationaryRate > MAX_TIME)
		return true;

	return false;
}
bool Drivetrain::waitForStationary(int timeout) {
	uint32_t start = pros::millis();
	while (!isStationary() && (pros::millis() - start) < timeout) {
		pros::delay(10);
	}
	if (pros::millis() - start >= timeout)
		return false;
	return true;
}
void Drivetrain::resetHeading(Units::Angle heading) {
	gyro->set_heading(heading.dg());
};

void Drivetrain::setTimeout(int tim) {
	if (tim <= 0) {
		timeout = M_MAX;
		return;
	}
	timeout = static_cast<long long>(tim);
}

std::vector<uint8_t> Drivetrain::getOverheatingMotors() {
	std::vector<uint8_t> mtrs;
	for (MKV5::CustomMotor &mtr : leftMotors.group) {
		if (mtr.isOverheating()) {
			mtrs.push_back(mtr.getPort());
		}
	}
	for (MKV5::CustomMotor &mtr : rightMotors.group) {
		if (mtr.isOverheating()) {
			mtrs.push_back(mtr.getPort());
		}
	}
	return mtrs;
}

bool Drivetrain::isOverheating() {
	return (getOverheatingMotors().size() > 0);
};
} // namespace MKV5
