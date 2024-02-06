#include <functional>
#include "globals.hpp"

class SemiAutonMovement {
    private:
        std::function<void()> movementFunct;
        std::function<bool()> exitCondition;
    
    public:
        bool isActive;

        /**
         * @brief Construct a new Semi Auton Movement object
         *
         * @param movementFunct The function to run the movement. All movements must be asynchronous.
         * @param exitCondition The function to check for exit conditions.
         */
        SemiAutonMovement(std::function<void()> movementFunct, std::function<bool()> exitCondition);
        
        /**
         * @brief Run the movement function until the exit condition is met.
         */
        void start();
};