#pragma once

#include "pros/rtos.hpp"
#include <memory>

struct AsyncReturn {
    pros::Task* task;
    void block() {
        task->join();
    };
};