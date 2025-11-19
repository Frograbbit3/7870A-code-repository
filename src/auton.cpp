#include "custom/enums.hpp"
#include "init.hpp"

void autonomousTick() {
    forward(50);
    left();
    forward(15);
}