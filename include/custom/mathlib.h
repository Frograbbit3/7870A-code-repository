#pragma once
#include <algorithm>
#define CD_TURN_NONLINEARITY 0.65
#include "math.h"
constexpr double D_PI = M_PI * 2; //idk why this is here

template<typename T>
/*A helper function that literally just sets a number if its too big or too low.*/
T minmax(T num, T min, T max)  {
    if (num > max) {
        return max;
    }
    if (num < min) {
        return min;
    }
    return num;
}




extern "C" {
    /*Will take a float and pow it by an int. powf can do the same but is slower then this.*/
    inline float powerf(float num1, int num2) {
        float new_num = 1.0f;
        for (int i = 0; i < num2; i++) {
            new_num *= num1;
        }
        return new_num;
    }
    /*
        The equation to preprocess the joystick. 
        Pass in a value from -127 to 127 and it will perform the math to feel slightly better

        Source: https://github.com/purduesigbots/forkner-public/blob/62e1328b7902715035357622d81e4a35cb15ff2f/src/libforkner/drive.cpp#L363
    */
        inline float JoystickCurve(float iturn) {
            const double nl  = CD_TURN_NONLINEARITY;
            const double k   = M_PI * 0.5 * nl;
            const double den = std::sin(k);

            double x = std::clamp(iturn / 127.0, -1.0, 1.0);

            double y = std::sin(k * x) / den;
            double z = std::sin(k * y) / den;

            return static_cast<float>(std::clamp(z, -1.0, 1.0) * 127.0);
        }

}

