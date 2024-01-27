#include "opcontrolHelpers.hpp"

std::tuple<float, float> processSticks() {
    float forwardsVel = gamepad1.leftY, turnVel = gamepad1.rightX;

    // forwardsVel = nicklib::deadzone(forwardsVel, CONTROLLER_DEADZONE);
    // turnVel = nicklib::deadzone(turnVel, CONTROLLER_DEADZONE);

    // forwardsVel = nicklib::curveInput(forwardsVel, CONTROLLER_CURVE_GAIN);
    // turnVel = nicklib::curveInput(turnVel, CONTROLLER_CURVE_GAIN);

    return std::make_tuple(forwardsVel, turnVel * TURN_CONST);
}

// Subsystem states default constructor
SysStates::SysStates() {
    cataState = CataStates::disconnect;
    intakeState = IntakeStates::stop;
    blockerState = BlockerStates::down;
}

// Subsystem states constructor
SysStates::SysStates(CataStates cataState, IntakeStates intakeState, BlockerStates blockerState)
    : cataState(cataState),
      intakeState(intakeState),
      blockerState(blockerState) {}