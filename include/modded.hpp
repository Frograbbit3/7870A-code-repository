#include "main.h"
#include "enums.hpp"
#define time pros::millis()
class MotorGroup {
    public:
        std::vector<pros::Motor> group;
        std::vector<int8_t> ports;
        double ratio=3.5; //Assuming inch wheels fn, change later.
        int stopped = 0;
        int STOP_COOLDOWN = 1000; //in ms, the time to stop. A higher value uses more battery but can feel better to drive.
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
            ///@brief DO NOT USE!
            switch (dst)
            {
            case DrivetrainEnums::Distance::INCHES:
                return M_PI*getWheelRotation()*3.5;
                break;
            case DrivetrainEnums::Distance::MM:
                return INCH_TO_MM(M_PI*getWheelRotation()*3.5);
            default:
                return 0.0f;
                break;
            }
        }
        void update() {
            if (stopped > 0) {
                if (time - stopped > STOP_COOLDOWN) {
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