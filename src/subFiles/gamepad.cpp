#include "subHeads/gamepad.hpp"

Gamepad::Gamepad(pros::controller_id_e_t id)
    : controller(std::make_unique<pros::Controller>(id)) {}

void Gamepad::update() {
    // Guard clause against a disabled controller (during auton and semi-autons)
    // Use the B button to cancel semi autons
    if (disabled) {
        b.setStatus(controller->get_digital(pros::E_CONTROLLER_DIGITAL_B));
        return;
    }

    // Get the input value for the stick axes
    for (int i = 0; i < 4; i++) {
        pros::controller_analog_e_t stick = static_cast<pros::controller_analog_e_t>(i);
        double stickInput = controller->get_analog(stick) / 127.0;

        *ptrSticksArr[i] = stickInput;
    }

    // Get the status for the buttons
    for (int i = 0; i < 12; i++) {
        /** NOTE: add 6 to i to match the pros::controller_digital_e_t enum in misc.h */
        pros::controller_digital_e_t button = static_cast<pros::controller_digital_e_t>(i + 6);
        ptrButtonsArr[i]->setStatus(controller->get_digital(button));
    }
}