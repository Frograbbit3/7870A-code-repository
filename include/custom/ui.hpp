#pragma once
#include "pros/apix.h"
#include "liblvgl/lvgl.h"

#define lv_center lv_obj_center
#define lv_add_event lv_obj_add_event_cb
#define lv_get_obj lv_event_get_current_target_obj
namespace UILib {
    auto activeScreen = lv_screen_active();
    class Screen {
        private:
            static void __PROCESS_CLOSE_BUTTON(lv_event_t * e) {
                Screen* instance = (Screen*)lv_event_get_user_data(e);
                lv_obj_delete(lv_event_get_current_target_obj(e));
            }
        public:
            lv_obj_t * win = lv_win_create(activeScreen);
            Screen(std::string title, int width, int height) {
                lv_win_add_title(win, title.c_str());
                lv_obj_t * close_btn = lv_win_add_button(win, LV_SYMBOL_CLOSE,64);           /*Add close button and use built-in close action*/
                lv_add_event(close_btn,__PROCESS_CLOSE_BUTTON, LV_EVENT_CLICKED, NULL);
            }
    };
    class Slider {
        public:
            int value = 0;
        private:
            lv_obj_t* slider = lv_slider_create(activeScreen);
            void (*move_events)(int) = nullptr;
            static void __PROCESS_SLIDER_STATIC(lv_event_t * e) {
                Slider* instance = (Slider*)lv_event_get_user_data(e);
                instance->value = lv_slider_get_value(lv_event_get_current_target_obj(e));
                if (instance && instance->move_events) {
                    instance->move_events(instance->value);
                }
            }
            
        public:
            Slider(int x, int y, int start, int end) {
                lv_slider_set_mode(slider, LV_SLIDER_MODE_NORMAL);
                lv_slider_set_range(slider, start, end);
                lv_obj_set_x(slider, x); // Fixed: using lv_obj_set_x instead of lv_obj_set_style_x
                lv_obj_set_y(slider, y);
                lv_add_event(slider, __PROCESS_SLIDER_STATIC, LV_EVENT_VALUE_CHANGED, this);
                lv_center(slider);
            }
            
            void on_move(void (*callback)(int)) {
                move_events = callback;
            }
    }; 
    class Label {
        private:
            lv_obj_t* label = lv_label_create(activeScreen);
            std::string value = "";
            
        public:
            Label(int x, int y, std::string val) {
                lv_center(label);
                lv_obj_set_x(label, x); // Fixed: using lv_obj_set_x instead of lv_obj_set_style_x
                lv_obj_set_y(label, y);
                change_text(val);
            }
            
            void change_text(std::string msg) {
                value = msg;
                lv_label_set_text(label, value.c_str());
            }

    }; 

    void update() {
        lv_tick_inc(20);
        lv_timer_handler();
    }

    void setActive(UILib::Screen& scrn) {
        activeScreen = lv_win_get_content(scrn.win);
    }
}