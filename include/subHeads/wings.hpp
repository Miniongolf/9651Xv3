#pragma once

#include "pros/adi.hpp"


class Wings {
    private:    
        pros::ADIDigitalOut* leftPiston;
        pros::ADIDigitalOut* rightPiston;

    public:
        // Wings side enum (left, right, both)
        enum WingSide {
            L = 0,
            R,
            B
        };
        /**
         * @brief Wings class constructor (no args)
         * 
         * @param None
         */
        Wings(char leftPort, char rightPort);

        /**
         * @brief Extends selected wings
         * 
         * @param side WingSide enum value (L, R, B)
         * @returns `None`
         */
        void extend(WingSide side);

        /**
         * @brief Retracts selected wings
         * 
         * @param side WingSide enum value (L, R, B)
         * @returns `None`.
         */
        void retract(WingSide side);
};