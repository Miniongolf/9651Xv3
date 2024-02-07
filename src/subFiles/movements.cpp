#include "subHeads/movements.hpp"
#include "pros/rtos.hpp"
#include <iostream>

/**
 * @brief Construct a new Semi Auton Movement object
 *
 * @param movementFunct The function to run the movement. All movements must be asynchronous.
 * @param exitCondition The function to check for exit conditions.
 */
SemiAutonMovement::SemiAutonMovement(std::function<void()> movementFunct, std::function<bool()> exitCondition) 
    : movementFunct(movementFunct), exitCondition(exitCondition) {}

/**
 * @brief Run the movement function until the exit condition is met.
 */
void SemiAutonMovement::start() {
    this->isActive = true;
    movementFunct();

    pros::Task checkExit([this]() {
        while (this->isActive) {
            std::cout << fmt::format("%d");
            if (exitCondition()) {
                std::cout << "Exiting semi auton movement\n";
                chassis.cancelAllMotions();
                this->isActive = false;
                break;
            }
        }
    });
}