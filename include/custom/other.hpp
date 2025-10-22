#pragma once
#include "main.h"
#include "cstdlib"

namespace FlappyBird {
    bool isStarted = false;
    int camX = 0;
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    void start() {
        isStarted = true;
    }
    class Bird{
        private:
            int birdY = 0;
            int vY = 0;
        public:
            void update() {
                pros::screen::erase_circle(120, birdY, 8);
                vY -= 1;
                if (vY < -8) {
                    vY = -8;
                }
                birdY -= vY;
                if (master.get_digital_new_press(DIGITAL_A)) {
                    vY =12;
                }
                pros::screen::set_pen(pros::Color::yellow);
                pros::screen::draw_circle(120, birdY, 8);
            }
    };
    class Pipe {
        private:
            int pipeX = 0;
            int pipeWidth = 50;
        public:
            void update() {
                pros::screen::erase_rect(camX+pipeX-pipeWidth, 0, camX+pipeX+pipeWidth, 60);
                pros::screen::erase_rect(camX+pipeX-pipeWidth, 90, camX+pipeX+pipeWidth, 320);
                pros::screen::set_pen(pros::Color::green);
                pros::screen::draw_rect(camX+pipeX-pipeWidth, 90, camX+pipeX+pipeWidth, 320);
            }
    };
    Pipe mainPipe;
    Bird mainBird;
    void update() {
        mainBird.update();
        mainPipe.update();
        camX += 2;
    }
}