#pragma once

#include <array>
#include <memory>
#include <cmath>

#include "pros/misc.hpp"

#include "nicklib/button.hpp"

namespace nicklib {
class Gamepad {
    private:
        // Pointer array for joysticks
        std::array<float*, 4> ptrSticksArr = {&leftX, &leftY, &rightX, &rightY};

        // Pointer array for buttons (using wrapper button class)
        std::array<Button*, 12> ptrButtonsArr = {&lb,       &lt,        &rb, &rt, &dpadUp, &dpadDown,
                                                 &dpadLeft, &dpadRight, &x,  &b,  &y,      &a};
    public:
        float leftX, leftY, rightX, rightY;
        Button lb, lt, rb, rt;
        Button dpadUp, dpadDown, dpadLeft, dpadRight;
        Button x, b, y, a;

        // Smart pointer to pros controller | use smart pointer to auto delete when ptr goes out of scope
        std::unique_ptr<pros::Controller> controller;

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
        void update();
};
} // namespace nicklib