#include "custom/enums.hpp"

namespace MKV5
{
    double getWheelDiameter(Enums::WheelType type)
    {
        switch (type)
        {
        case Enums::WheelType::WHEEL_275:
            return 2.75f;
        case Enums::WheelType::WHEEL_325:
            return 3.25f;
        case Enums::WheelType::WHEEL_400:
            return 4.00f;
        default:
            return 3.25f;
        }
    }
}