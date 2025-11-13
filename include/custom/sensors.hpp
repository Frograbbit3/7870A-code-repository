#include "pros/gps.hpp"
#include "pros/imu.hpp"
#include "drivetrain.hpp"
namespace SensorLib {
    class SensorReader {
        private:
            pros::GPS* gps=nullptr;
            pros::Imu* gyro=nullptr;
            DrivetrainLib::Drivetrain* drive=nullptr;
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
                //currently gps isn't used so we are just gonna guestimate 
                
            }
    };
}