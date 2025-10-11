#include "main.h"

#define time pros::millis()


class MotorGroup {
    public:
        std::vector<pros::Motor> group;
        std::vector<int8_t> ports;
        int stopped = 0;
        int STOP_COOLDOWN = 1000; //in ms, the time to stop.
        MotorGroup(const std::vector<int8_t>& ports) {
            for (int8_t port : ports) {
                group.push_back(pros::Motor(port));
            }
        }
        void move(int32_t voltage) {
            for (pros::Motor mtr : group) {
                mtr.move(voltage);
                stopped=0;
            }
        }
        void brake() {
            for (pros::Motor mtr : group) {
                mtr.brake();
                stopped=time;
            }
        }
        void update() {
            if (stopped > 0) {
                if (time - stopped > STOP_COOLDOWN) {
                    for (pros::Motor mtr: group) {
                        mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
                    }
                    stopped = 0;
                }else{
                    for (pros::Motor mtr: group) {
                        mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                    }
                }
            }

        }
};