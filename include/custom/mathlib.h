#pragma once
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
    float powerf(float num1, int num2) {
        float new_num = 1.0f;
        for (int i = 0; i < num2; i++) {
            new_num *= num1;
        }
        return new_num;
    }
    /*
        The equation to preprocess the joystick. 
        Pass in a value from -127 to 127 and it will perform the math to feel slightly better
    */
    float JoystickCurve(double x) {
        float num = static_cast<float>(x)/127.0f;
        int new_num = powerf(num, 3) * 127.0f;
        return minmax(new_num, -127, 1270);
    };
}
