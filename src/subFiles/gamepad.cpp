#include "subHeads/gamepad.hpp"
#include "globals.hpp"
#include "subHeads/constants.hpp"

Gamepad::Gamepad(pros::controller_id_e_t id) {
    controller = std::make_unique<pros::Controller>(id);
}

void Gamepad::getInputs() {
    // Guard clause against a disabled controller (during auton and semi-autons)
    // Use the B button to cancel semi autons
    if (disabled) {
        b.setStatus(controller->get_digital(pros::E_CONTROLLER_DIGITAL_B));
        return;
    }
    
    // Get the input value for the stick axes
    for (int i = 0; i < 4; i++) {
        pros::controller_analog_e_t stick = static_cast<pros::controller_analog_e_t>(i);
        double stickInput = controller->get_analog(stick)/127.0;
        
        *ptrSticksArr[i]= stickInput;
    }

    // Get the status for the buttons
    for (int i = 0; i < 12; i++) {
        /** NOTE: add 6 to i to match the pros::controller_digital_e_t enum in misc.h */
        pros::controller_digital_e_t button = static_cast<pros::controller_digital_e_t>(i+6);
        (*ptrButtonsArr[i]).setStatus(controller->get_digital(button));
    }
}

std::array<float, 2> Gamepad::processSticks(float deadzone, bool curve) {
    // Stick curving lambda
    // Credit to Finlay 46846T for the curve formula
    auto curveInput = [] (float x) -> float {
        float a = STICK_CURVE_GAIN;
        float y;
        if (x <= 0) {
            y = -1 * (1 - powf(M_E, -1 * a * x) / (1 - powf(M_E, a)));
        } else {
            y = (1 - powf(M_E, a * x) / (1 - powf(M_E, a)));
        }
        return y;
    };

    float forwardsVel, turnVel;

    forwardsVel = this->leftY;
    turnVel = this->rightX*TURN_CONST;

    // Cross deadzone
    if (fabs(forwardsVel) < deadzone) forwardsVel = 0;
    if (fabs(forwardsVel) < deadzone) turnVel = 0;

    // Curve inputs
    if (curve) {
        forwardsVel = curveInput(forwardsVel);
        turnVel = curveInput(turnVel);
    }

    return {forwardsVel, turnVel};
}

void Gamepad::disable() {
    disabled = true;

    for (int i = 0; i < 4; i++) {
        pros::controller_analog_e_t stick = static_cast<pros::controller_analog_e_t>(i);
        *ptrSticksArr[i] = 0;
    }

    for (int i = 0; i < 12; i++) {
        /** NOTE: add 6 to i to match the pros::controller_digital_e_t enum in misc.h */
        pros::controller_digital_e_t button = static_cast<pros::controller_digital_e_t>(i+6);
        (*ptrButtonsArr[i]).disable();
    }
}

void Gamepad::resume() {
    disabled = false;
}