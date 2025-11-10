#include "math.h"

template<typename T>
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
    float powerf(float num1, int num2) {
        float new_num = 1.0f;
        for (int i = 0; i < num2; i++) {
            new_num *= num1;
        }
        return new_num;
    }
    float JoystickCurve(double x) {
        float num = static_cast<float>(x)/127.0f;
        int new_num = powerf(num, 3) * 127.0f;
        return minmax(new_num, -127, 1270);
    };
}
