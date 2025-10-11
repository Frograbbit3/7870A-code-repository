#include "main.h"



class MotorGroup {
    public:
        std::vector<pros::Motor> group;
        std::vector<int8_t> ports;
        MotorGroup(const std::vector<int8_t>& ports) {
            for (int8_t port : ports) {
                group.push_back(pros::Motor(port));
            }
        }
        void move(int32_t voltage) {
            for (pros::Motor mtr : group) {
                mtr.move(voltage);
            }
        }
        void brake() {
            for (pros::Motor mtr : group) {
                mtr.brake();
            }
        }
};