#pragma once

#include "pros/rtos.hpp"

namespace nicklib {
class Button {
    private:
        bool prevState = false, currState = false;
        int startTime, currTime;
    public:
        // Button statuses
        bool pressed, released, held;
        int heldTime;

        /**
         * @brief Button class constructor
         *
         * @param None
         * @note Should only be used by gamepad class and not in loose code
         */
        Button();

        /**
         * @brief Sets the button state of pressed, held, and released
         *
         * @param isPressed from `gamepad.get_digital()`
         * @return `None`
         * @note Should only be used in gamepad class, not for general code.
         */
        void setStatus(bool isPressed);

        /**
         * @brief Casts button to boolean.
         *
         * @return True if the button is held, false otherwise.
         * @note Equivalent to `{gamepad}.{button}.held`
         */
        explicit operator bool() const;

        /**
         * @brief Casts button to int.
         *
         * @return how long the button has been held down (in ms).
         * @note Equivalent to `{gamepad}.{button}.heldTime`
         */
        explicit operator int() const;
};
} // namespace nicklib