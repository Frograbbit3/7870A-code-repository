#include "custom/enums.hpp"

namespace DrivetrainEnums
{
    double getWheelDiameter(WheelType type)
    {
        switch (type)
        {
        case WheelType::WHEEL_275:
            return 2.75f;
        case WheelType::WHEEL_325:
            return 3.25f;
        case WheelType::WHEEL_400:
            return 4.00f;
        default:
            return 3.25f;
        }
    }
}