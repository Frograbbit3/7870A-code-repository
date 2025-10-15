#pragma once
#include "pros/apix.h"
#include "liblvgl/lvgl.h"

namespace UILib {
    void update() {
        lv_tick_inc(20);
        lv_timer_handler();
    }
}