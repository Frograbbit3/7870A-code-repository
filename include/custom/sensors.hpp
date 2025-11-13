#include "pros/gps.hpp"
#include "pros/imu.hpp"
#include "drivetrain.hpp"
#include "config.h"
namespace SensorLib {
    class SensorReader {
        private:
            pros::GPS* gps=nullptr;
            pros::Imu* gyro=nullptr;
            DrivetrainLib::Drivetrain* drive=nullptr;
            double rotationBuffer[ROTATION_BUFFER];
            int curRBuffer=0;
            double rotRange = 0;
            double rotMin = 0;
            double rotLast = 999999;
            double rotMax = 0;

        public:
            SensorReader(pros::IMU *mu, pros::GPS *gp, DrivetrainLib::Drivetrain *dr): gps(gp), gyro(mu), drive(dr) {

            }
            int rotation = 0;
            int calibrated = false;
            void init() {
                if (gps) {
                    // i'm lazy lol we'll add this later
                    //gps->initialize_full() 
                }
                if (gyro) {
                    gyro->reset(false);
                    while (gyro->is_calibrating()) {
                        pros::delay(50);
                    }
                }
                calibrated=true;
            }
            void update() {
                rotationBuffer[curRBuffer] = gyro->get_heading();
                curRBuffer++;
                curRBuffer = curRBuffer % ROTATION_BUFFER;
                for (int i = 0; i < ROTATION_BUFFER; ++i) {
                    double slot = rotationBuffer[i];
                    if (rotLast < 360) { //we are not on the first one
                        if (fabs(slot - rotLast) > ROTATION_OFFSET_LIMIT) { // A weird reading
                            continue; //why is this so bad?
                        } 
                    }
                }
            }
    };
}