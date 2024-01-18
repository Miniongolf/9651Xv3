#pragma once

#include <array>
#include <memory>
#include <cmath>

#include "pros/misc.h"
#include "pros/misc.hpp"

#include "subHeads/button.hpp"
#include "subHeads/constants.hpp"

class Gamepad {
    private:
        // Pointer array for joysticks
        std::array<double*, 4> ptrSticksArr = {&leftX, &leftY, &rightX, &rightY};

        // Pointer array for buttons (using wrapper button class)
        std::array<Button*, 12> ptrButtonsArr = {&lb,       &lt,        &rb, &rt, &dpadUp, &dpadDown,
                                                 &dpadLeft, &dpadRight, &x,  &b,  &y,      &a};
    public:
        double leftX, leftY, rightX, rightY;
        Button lb, lt, rb, rt;
        Button dpadUp, dpadDown, dpadLeft, dpadRight;
        Button x, b, y, a;

        // Smart pointer to pros controller | use smart pointer to auto delete when ptr goes out of scope
        std::unique_ptr<pros::Controller> controller;

        bool disabled;

        /**
         * @brief Gamepad class constructor
         *
         * @param id `pros::controller_id_e_t` enum
         */
        Gamepad(pros::controller_id_e_t id);

        /**
         * @brief Delete default constructor to force initialization with an id
         */
        Gamepad() = delete;

        /**
         * @brief Gets the inputs from the gamepad
         *
         * @param None
         */
        void getInputs();

        /**
         * @brief Processes the inputs from the gamepad sticks and converts them to movement powers.
         *
         * @param deadzone Minimum input scale to consider (cross deadzone)
         * @param curve Curves the stick inputs (credits to Finlay 46846T for the formula)
         * @return Float array with lateral power and turning power
         */
        std::array<float, 2> processSticks(float deadzone, bool curve = true);

        /**
         * @brief Sets all gamepad inputs to 0 or false (except cancel button)
         *
         * @param None
         * @return `None`
         *
         * @note Use when running semi auton
         */
        void disable();

        /**
         * @brief Sets disabled to false
         *
         * @param None
         * @return `None`
         *
         * @note Use every time semiauton is canceled or ends
         */
        void resume();
};
