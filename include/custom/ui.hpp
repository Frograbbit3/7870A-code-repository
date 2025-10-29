#pragma once
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
            Screen(std::string title) {
                lv_win_add_title(win, title.c_str());
                lv_obj_t * close_btn = lv_win_add_button(win, LV_SYMBOL_CLOSE,64);           /*Add close button and use built-in close action*/
                lv_add_event(close_btn,__PROCESS_CLOSE_BUTTON, LV_EVENT_CLICKED, NULL);
            }
    };
    class Keyboard {
        private:
            lv_obj_t* kb;    
            lv_obj_t* ta2;
        
        public:
            Keyboard() {
                kb = lv_keyboard_create(activeScreen);
                ta2 = lv_textarea_create(activeScreen);
                lv_obj_set_size(ta2, 0, 0);
                lv_keyboard_set_textarea(kb, ta2);
            }
    };
    class Slider {
        public:
            int value = 0;
        private:
            lv_obj_t* slider = lv_slider_create(activeScreen);
            lv_obj_t* label = lv_label_create(activeScreen);
            void (*move_events)(int) = nullptr;
            static void __PROCESS_SLIDER_STATIC(lv_event_t * e) {
                Slider* instance = (Slider*)lv_event_get_user_data(e);
                instance->value = lv_slider_get_value(lv_event_get_current_target_obj(e));
                lv_label_set_text(instance->label,std::to_string(lv_slider_get_value(lv_event_get_current_target_obj(e))).c_str());
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

                lv_obj_set_x(label, x+100);
                lv_obj_set_y(label, y+15);
                lv_label_set_text(label, std::to_string(0).c_str());
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
    class Button {
    private:
        lv_obj_t* btn;
        void (*on_click)() = nullptr;

        static void __PROCESS_BUTTON_CLICK(lv_event_t* e) {
            Button* instance = (Button*)lv_event_get_user_data(e);
            if (instance->on_click) {
                instance->on_click();
            }
        }

    public:
        Button(int x, int y, std::string label = "") {
            btn = lv_obj_create(activeScreen); // Use lv_obj_create for generic objects
            lv_obj_set_size(btn, 100, 50);     // Set size for the button
            lv_obj_set_x(btn, x);
            lv_obj_set_y(btn, y);
            lv_center(btn);

            if (!label.empty()) {
                lv_obj_t* lbl = lv_label_create(btn);
                lv_label_set_text(lbl, label.c_str());
                lv_obj_center(lbl);
            }

            lv_add_event(btn, __PROCESS_BUTTON_CLICK, LV_EVENT_CLICKED, this);
        }

        void onClick(void (*callback)()) {
            on_click = callback;
        }

    };

    // Dropdown API
    class Dropdown {
    private:
        lv_obj_t* dd;
        void (*on_select)(int) = nullptr;

        static void __PROCESS_DROPDOWN_SELECT(lv_event_t* e) {
            Dropdown* instance = (Dropdown*)lv_event_get_user_data(e);
            if (instance->on_select) {
                int selected = lv_dropdown_get_selected(lv_event_get_current_target_obj(e));
                instance->on_select(selected);
            }
        }

    public:
        Dropdown(int x, int y, std::vector<std::string> options) {
            dd = lv_dropdown_create(activeScreen);
            lv_obj_set_x(dd, x);
            lv_obj_set_y(dd, y);

            std::string option_str = "";
            for (const auto& option : options) {
                option_str += option + "\n";
            }
            lv_dropdown_set_options(dd, option_str.c_str());

            lv_add_event(dd, __PROCESS_DROPDOWN_SELECT, LV_EVENT_VALUE_CHANGED, this);
        }

        void onSelect(void (*callback)(int)) {
            on_select = callback;
        }
    };

    // Checkbox API
    class Checkbox {
    private:
        lv_obj_t* cb;
        void (*on_toggle)(bool) = nullptr;

        static void __PROCESS_CHECKBOX_TOGGLE(lv_event_t* e) {
            Checkbox* instance = (Checkbox*)lv_event_get_user_data(e);
            instance->checked = lv_obj_has_state(instance->cb, LV_STATE_CHECKED);
            if (instance->on_toggle) {
                // Update the `checked` property directly from the `cb` object
                instance->on_toggle(instance->checked);
            }
        }

    public:
        bool checked;

        Checkbox(int x, int y, std::string label) {
            cb = lv_checkbox_create(activeScreen);
            lv_obj_set_x(cb, x);
            lv_obj_set_y(cb, y);
            lv_checkbox_set_text(cb, label.c_str());

            // Initialize the `checked` property
            checked = lv_obj_has_state(cb, LV_STATE_CHECKED);

            lv_add_event(cb, __PROCESS_CHECKBOX_TOGGLE, LV_EVENT_VALUE_CHANGED, this);
        }

        void onToggle(void (*callback)(bool)) {
            on_toggle = callback;
        }
    };

    // Roller API
    class Roller {
    private:
        lv_obj_t* roller;
        void (*on_select)(int) = nullptr;

        static void __PROCESS_ROLLER_SELECT(lv_event_t* e) {
            Roller* instance = (Roller*)lv_event_get_user_data(e);
            if (instance->on_select) {
                int selected = lv_roller_get_selected(lv_event_get_current_target_obj(e));
                instance->on_select(selected);
            }
        }

    public:
        Roller(int x, int y, std::vector<std::string> options, int visible_rows = 3) {
            roller = lv_roller_create(activeScreen);
            lv_obj_set_x(roller, x);
            lv_obj_set_y(roller, y);

            std::string option_str = "";
            for (const auto& option : options) {
                option_str += option + "\n";
            }
            lv_roller_set_options(roller, option_str.c_str(), LV_ROLLER_MODE_NORMAL);
            lv_roller_set_visible_row_count(roller, visible_rows);

            lv_add_event(roller, __PROCESS_ROLLER_SELECT, LV_EVENT_VALUE_CHANGED, this);
        }

        void onSelect(void (*callback)(int)) {
            on_select = callback;
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