#include "main.h"
#include "modded.hpp"
#include "enums.hpp"
#include <math.h>

#define pi M_PI
using pros::Motor;

namespace DriveUtils {
    struct MotorProperties {
        int velocity = 0; //in mV
        bool is_driving = false;
        float actual_velocity = 0; // updated by internal_telementry_collector
    };

    class Drivetrain {
        public:
            MotorGroup leftMotors;
            MotorGroup rightMotors;
        private:
            std::vector<int8_t> leftSide;
            std::vector<int8_t> rightSide;
            DriveUtils::MotorProperties leftProperties;
            DriveUtils::MotorProperties rightProperties;

            void internal_telementry_collector() {
                float average_vel;
                while (true) {
                    // left side
                    average_vel = 0.0f;
                    for (pros::Motor &mtr : leftMotors.group) {
                        average_vel += mtr.get_actual_velocity();
                    }
                    leftProperties.actual_velocity = average_vel / leftMotors.group.size();

                    //right side
                    average_vel = 0.0f;
                    for (pros::Motor &mtr : rightMotors.group) {
                        average_vel += mtr.get_actual_velocity();
                    }
                    rightProperties.actual_velocity = average_vel / rightMotors.group.size();
                    pros::delay(250);
                }
            }

            void drive_correction() { //NOTE TO SELF! Correct this soon!
                float correction = 0.02f;
                while (true) {
                    //only correct if both sides are driving
                    if (leftProperties.is_driving && rightProperties.is_driving) {
                        if (rightProperties.actual_velocity > leftProperties.actual_velocity) {
                            leftProperties.velocity = rightProperties.velocity + correction;
                            rightProperties.velocity -= correction;
                        }else{
                            rightProperties.velocity = leftProperties.velocity + correction;
                            leftProperties.velocity -= correction;
                        }
                    }else{ //Process breaking as well; makes life easier.
                        leftMotors.update();
                        rightMotors.update();
                    }
                    pros::delay(20);
                }
            }
            
            static void task_helper_telementry(void *ptr) {
                Drivetrain* self = static_cast<Drivetrain*>(ptr);
                self->internal_telementry_collector();
            }
            static void task_helper_drive_correction(void *ptr) {
                Drivetrain* self = static_cast<Drivetrain*>(ptr);
                self->drive_correction();
            }
        public:
            Drivetrain(const std::vector<int8_t>& leftSide,const std::vector<int8_t>& rightSide): leftMotors(leftSide),rightMotors(rightSide){
                pros::Task telementry(task_helper_telementry,(void*)this);
                pros::Task auto_drive(task_helper_drive_correction,(void*)this);
            }
            void setLeftVelocity(int velocity) {
                ///Sets the left velocity to a value between 0 - 100. Does this by attempting to set max voltage.
                int32_t result = (velocity * 127) / 100;
                leftProperties.velocity = result * 1000;
            }
            void setRightVelocity(int velocity) {
                ///Sets the right velocity to a value between 0 - 100. Does this by attempting to set max voltage.
                int32_t result = (velocity * 127) / 100;
                rightProperties.velocity = result * 100;
            }

            void stop() {
                ///Breaks the robot. This is the same as calling DriveUtils::Drivetrain::Drive(DrivetrainEnums::Direction::STOP)
                this->drive(DrivetrainEnums::Direction::STOP);
            }
            void drive(DrivetrainEnums::Direction direction) {
                ///Moves the robot. Simple.
                switch (direction)
                {
                    case DrivetrainEnums::Direction::FORWARD:
                        leftMotors.move(leftProperties.velocity);
                        rightMotors.move(rightProperties.velocity);
                        leftProperties.is_driving = true;
                        rightProperties.is_driving = true;
                        break;
                    case DrivetrainEnums::Direction::REVERSE:
                        leftMotors.move(leftProperties.velocity*-1);
                        rightMotors.move(rightProperties.velocity*-1);
                        leftProperties.is_driving = true;
                        rightProperties.is_driving = true;
                    default:
                        leftMotors.brake();
                        rightMotors.brake();
                        leftProperties.is_driving = false;
                        rightProperties.is_driving = false;
                        break;
                }
            }

    };
}