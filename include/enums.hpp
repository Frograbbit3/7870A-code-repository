#pragma once
static inline double INCH_TO_MM(double inch) {return 25.4 * inch;}
static inline double MM_TO_INCH(double mm) {return mm / 25.4;}
namespace DrivetrainEnums
{
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
