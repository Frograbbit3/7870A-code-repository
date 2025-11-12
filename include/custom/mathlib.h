#pragma once
#define CD_TURN_NONLINEARITY 0.65
#include "math.h"

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
    inline float JoystickCurve(double iturn) {
        double denominator = sin(M_PI / 2 * CD_TURN_NONLINEARITY);
        double firstRemapIteration =
            sin(M_PI / 2 * CD_TURN_NONLINEARITY * (iturn/127.0f)) / denominator;
        return static_cast<float>(sin(M_PI / 2 * CD_TURN_NONLINEARITY * firstRemapIteration) / denominator)*127.0f;
    }
}
