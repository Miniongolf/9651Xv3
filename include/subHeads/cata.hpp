#pragma once

#include <memory>
#include <cmath>
#include <iostream>

#include "pros/motors.hpp"
#include "subHeads/constants.hpp"

class Cata {
    public:
        // Catapult motor group
        std::unique_ptr<pros::MotorGroup> motors;

        // Catapult angle
        int angle;

        Cata(pros::MotorGroup* motorGroup);
        
        void resetPos();

        /**
         * @brief Getter for catapult angle
         * 
         * @param None
         * @return Catapult angle
         */
        int getAngle();

        /**
         * @brief Updates catapult angle using IME
         * 
         * @param None
         * @return `None`
         */
        void update();

        /**
         * @brief Runs the catapult at max power
         * 
         * @param power
         *        Motor voltage [0, 127]
         * @return `None`
         */
        void fire(int power = 127);

        /**
         * @brief Stops the catapult instantly
         * 
         * @param None
         * @return `None`
         */
        void stop();

        /**
         * @brief Stops the catapult if in ready position
         * 
         * @param None
         * @return `None`
         */
        void standby();
};