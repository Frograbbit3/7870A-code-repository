#pragma once
inline double INCH_TO_MM(double inch) {return 25.4 * inch;}
inline double MM_TO_INCH(double mm) {return mm / 25.4;}
namespace DrivetrainEnums
{
    struct MotorProperties {
        double gearRatio = 1.0;
        double wheelSize = 3.5;
        double stopCooldown = 1.0;
    };
    enum class Distance {
        INCHES = 1,
        MM = 0,
        ROTATION = 2,
        
    };
    enum class Direction {
        FORWARD = 1,
        REVERSE = -1,
        STOP = 0
    };    

} // namespace DrivetrainEnums


std::vector<pros::controller_digital_e_t> __BUTTON_LIST = {
    pros::E_CONTROLLER_DIGITAL_L1,
    pros::E_CONTROLLER_DIGITAL_L2,
    pros::E_CONTROLLER_DIGITAL_R1,
    pros::E_CONTROLLER_DIGITAL_R2,
    pros::E_CONTROLLER_DIGITAL_UP,
    pros::E_CONTROLLER_DIGITAL_DOWN,
    pros::E_CONTROLLER_DIGITAL_LEFT,
    pros::E_CONTROLLER_DIGITAL_RIGHT,
    pros::E_CONTROLLER_DIGITAL_X,
    pros::E_CONTROLLER_DIGITAL_B,
    pros::E_CONTROLLER_DIGITAL_Y,
    pros::E_CONTROLLER_DIGITAL_A,
    pros::E_CONTROLLER_DIGITAL_POWER};

#define TANK_DRIVE ControllerLib::ControllerEnums::DRIVE_MODE_TANK 
#define ARCADE_DRIVE ControllerLib::ControllerEnums::DRIVE_MODE_ARCADE 