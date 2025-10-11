#include "main.h"

using pros::Motor;

namespace DriveUtils {
    enum class Direction {
        FORWARD = 1,
        REVERSE = -1,
        STOP = 0 
    };
    struct MotorProperties {
        int velocity = 0; //in mV
        bool is_driving = false;
    };

    class Drivetrain {
        private:
            std::vector<int8_t> leftSide;
            std::vector<int8_t> rightSide;
            DriveUtils::MotorProperties leftProperties;
            DriveUtils::MotorProperties rightProperties;
            
        public:
            pros::MotorGroup leftMotors;
            pros::MotorGroup rightMotors;

            Drivetrain(const std::vector<int8_t>& leftSide,const std::vector<int8_t>& rightSide): leftMotors(leftSide),rightMotors(rightSide){
                
            }
            void setLeftVelocity(int velocity) {
                ///Sets the left velocity to a value between 0 - 100. Does this by attempting to set max voltage.
                int32_t result = (velocity * 12) / 100;
                leftProperties.velocity = result * 1000;
            }
            void setRightVelocity(int velocity) {
                ///Sets the right velocity to a value between 0 - 100. Does this by attempting to set max voltage.
                int32_t result = (velocity * 12) / 100;
                rightProperties.velocity = result * 1000;
            }
            void drive(DriveUtils::Direction direction) {
                ///Moves the robot. Simple.
                switch (direction)
                {
                    case DriveUtils::Direction::FORWARD:
                        leftMotors.move(leftProperties.velocity);
                        rightMotors.move(rightProperties.velocity);
                        leftProperties.is_driving = true;
                        rightProperties.is_driving = true;
                        break;
                    case DriveUtils::Direction::REVERSE:
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