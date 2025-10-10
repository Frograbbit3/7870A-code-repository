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
    };
    class Drivetrain {
        private:
            std::vector<int8_t> leftSide;
            std::vector<int8_t> rightSide;
            pros::MotorGroup leftMotors;
            pros::MotorGroup rightMotors;
            DriveUtils::MotorProperties leftProperties;
            DriveUtils::MotorProperties rightProperties;
            
        public:
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
                switch (direction)
                {
                    case DriveUtils::Direction::FORWARD:
                        leftMotors.move(leftProperties.velocity);
                        break;
                    case DriveUtils::Direction::REVERSE:
                        rightMotors.move(rightProperties.velocity);
                    
                    default:
                        leftMotors.move(0);
                        rightMotors.move(0);
                        break;
                }
            }

    };
}