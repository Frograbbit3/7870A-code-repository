#pragma once
#include "pros/apix.h"
#include "liblvgl/lvgl.h"

#define lv_center lv_obj_center
#define lv_add_event lv_obj_add_event_cb
#define lv_get_obj lv_event_get_current_target_obj
namespace UILib {
    auto activeScreen = lv_screen_active();
    enum class MessageBoxType {
        OK_BOX=1,
        YES_NO_BOX=2,
        APPLY_CANCEL=3
    };
    class Screen {
        private:
            static void __PROCESS_CLOSE_BUTTON(lv_event_t * e) {
                Screen* instance = (Screen*)lv_event_get_user_data(e);
                lv_obj_delete(lv_event_get_current_target_obj(e));
            }
        public:
            lv_obj_t * win = lv_win_create(activeScreen);
            Screen(std::string title) {
                lv_win_add_title(win, title.c_str());
                lv_obj_t * close_btn = lv_win_add_button(win, LV_SYMBOL_CLOSE,64);           /*Add close button and use built-in close action*/
                lv_add_event(close_btn,__PROCESS_CLOSE_BUTTON, LV_EVENT_CLICKED, this);
            }
    };
    class Keyboard {
        private:
            lv_obj_t* kb;    
            lv_obj_t* ta2;
            void (*on_key_press)() = nullptr;
            static void __PROCESS_KEYBOARD_INPUTS(lv_event_t * e) {
                Keyboard* instance = (Keyboard*)lv_event_get_user_data(e);
                if (instance->on_key_press) {
                    instance->on_key_press();
                }
            }
        public:
            Keyboard() {
                kb = lv_keyboard_create(activeScreen);
                ta2 = lv_textarea_create(activeScreen);
                lv_obj_set_size(ta2, 0, 0);
                lv_keyboard_set_textarea(kb, ta2);
                lv_add_event(kb, __PROCESS_KEYBOARD_INPUTS, LV_EVENT_VALUE_CHANGED, this);
            }

            std::string get_typed() {
                std::string st = lv_textarea_get_text(ta2);
                return st;
            }

            void onKeyPressed(void (*callback)()) {
                on_key_press = callback;
            }

            void hide() {
                lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
            }
            void show() {
                lv_obj_remove_flag(kb, LV_OBJ_FLAG_HIDDEN);
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
                lv_center(slider);
                lv_obj_set_x(slider, x); // Fixed: using lv_obj_set_x instead of lv_obj_set_style_x
                lv_obj_set_y(slider, y);
                lv_add_event(slider, __PROCESS_SLIDER_STATIC, LV_EVENT_VALUE_CHANGED, this);
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
    class MessageBox {
        private:
            MessageBoxType _type;
        public:
            lv_obj_t* mbox = lv_msgbox_create(NULL);
            MessageBox(std::string title, std::string message) {
                lv_msgbox_add_title(mbox, title.c_str());
                lv_msgbox_add_text(mbox, message.c_str());
                lv_msgbox_add_close_button(mbox);
            }
            

    };
    class Bar {
        private:
            lv_obj_t* bar = lv_bar_create(activeScreen);
            int value = 0;
            
        public:
            Bar(int x, int y, int value = 0) {
                lv_center(bar);
                lv_obj_set_x(bar, x); // Fixed: using lv_obj_set_x instead of lv_obj_set_style_x
                lv_obj_set_y(bar, y);
                change_value(value);
            }
            
            void change_value(int new_value) {
                value = new_value;
                lv_bar_set_value(bar,value, LV_ANIM_ON);
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