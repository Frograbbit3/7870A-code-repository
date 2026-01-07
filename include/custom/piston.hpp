#include "pros/adi.hpp"
namespace MKV5 {
    class Piston {
        private:
            int port;
            bool state;
        public:

            /*
                @brief The constructor of the Piston object. Takes in a number from port 1-8.
            */
            Piston(int p, bool start);

            /*
                @brief Gets if the piston is extended or compressed.
                @return A bool value depending on state.
            */
            bool getState();

            /*
                @brief Sets the state of the piston.
            */
            void setState(bool state);

            /*
                @brief Toggles the piston.
                @return The new state of the piston.
            */
            bool toggleState();
            

            /*
                Enables the piston.
            */
           void enable();

           /*
                Disables the piston.
           */
          void disable();
    };
}