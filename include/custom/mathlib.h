#include "math.h"

extern "C" {
    float powerf(float num1, int num2) {
        float new_num = 1.0f;
        for (int i = 0; i < num2; i++) {
            new_num *= num1;
        }
        return new_num;
    }
}

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
