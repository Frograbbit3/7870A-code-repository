#include "custom/enums.hpp"
#include "custom/modded.hpp"
namespace MKV5 {
MotorGroup::MotorGroup(const std::vector<int8_t> &ports_) : ports(ports_) {
	for (int8_t port : ports) {
		MKV5::CustomMotor tmp = MKV5::CustomMotor (port);
		group.emplace_back(tmp);
	}
}

void MotorGroup::setVelocity(int vel) {
	velocity = minmax(vel, -127, 127);
	for (CustomMotor &mtr : group) {
		mtr.setVelocity(velocity, VELOCITY_RAW);
	}
}

void MotorGroup::startMove(Enums::Direction dir) {
	for (CustomMotor &mtr : group) {
		switch (dir) {
		case DRIVE_FORWARD:
			mtr.move(DRIVE_FORWARD);
			break;
		case DRIVE_REVERSE:
			mtr.move(DRIVE_REVERSE);
			break;
		case DRIVE_STOP:
			stopMove();
			break;
		}
	}
}
void MotorGroup::stopMove() {
	for (CustomMotor &mtr : group) {
		mtr.brake();
		// stopped=time;
	}
}

void MotorGroup::doCalibration() {
	for (CustomMotor &mtr : group) {
		mtr.calibrate();
	}
}

double MotorGroup::getRotation() {
	double v = 0.0f;
	for (CustomMotor &mtr : group) {
		v += mtr.getPosition();
	}
	return (v / group.size() / 2);
}

bool MotorGroup::isMoving() {
	for (CustomMotor &mtr : group) {
		if (mtr.getMovement()) {
			return true;
		}
	}
	return false;
}

void MotorGroup::moveRelative(double angle, int vl) {
	for (CustomMotor &mtr : group) {
		mtr.moveRelative(DRIVE_FORWARD, angle, vl);
	}
}

void MotorGroup::update() {
	if (stopped > 0) {
		for (CustomMotor &mtr : group) {
			mtr.setBrakeMode(pros::MotorBrake::coast);
		}
		stopped = 0;
	}
}
} // namespace MKV5