#include "main.h"
#include "enums.hpp"
#define time pros::millis()
class MotorGroup {
    private:
        int stopped = 0;
    public:
        std::vector<pros::Motor> group;
        std::vector<int8_t> ports;
        DrivetrainEnums::MotorProperties properties;
        MotorGroup(const std::vector<int8_t>& ports) {
            for (int8_t port : ports) {
                group.push_back(pros::Motor(port));
            }
        }
        void move(int32_t voltage) {
            for (pros::Motor& mtr : group) {
                mtr.move(voltage);
                stopped=0;
            }
        }
        void brake() {
            for (pros::Motor& mtr : group) {
                mtr.brake();
                stopped=time;
            }
        }
        double getWheelRotation() {
            double v = 0.0f;
            for (pros::Motor& mtr : group) {
                v+=mtr.get_position();
            }
            return (v / group.size()/2);
        }

        double getDistance(DrivetrainEnums::Distance dst){
            switch (dst)
            {
            case DrivetrainEnums::Distance::INCHES:
                return M_PI*getWheelRotation()*properties.wheelSize;            
            case DrivetrainEnums::Distance::MM:
                return INCH_TO_MM(M_PI*getWheelRotation()*properties.wheelSize);
            case DrivetrainEnums::Distance::ROTATION:
                return getWheelRotation();
            }
        }
        void update() {
            if (stopped > 0) {
                if (time - stopped > properties.stopCooldown) {
                    for (pros::Motor& mtr: group) {
                        mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
                    }
                    stopped = 0;
                }else{
                    for (pros::Motor& mtr: group) {
                        mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                    }
                }
            }

        }
};