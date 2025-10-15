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



