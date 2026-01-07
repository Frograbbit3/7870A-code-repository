#include "custom/enums.hpp"

namespace MKV5
{
    double getWheelDiameter(Units::WheelUnit type)
    {
        switch (type)
        {
        case Units::WheelUnit::WHEEL_275:
            return 2.75f;
        case Units::WheelUnit::WHEEL_325:
            return 3.25f;
        case Units::WheelUnit::WHEEL_400:
            return 4.00f;
        default:
            return 3.25f;
        }
    }
}