#include "subHeads/teleop/opcontrolHelpers.hpp"

// Subsystem states constructor
SysStates::SysStates(CataStates cataState, IntakeStates intakeState, WingStates wingState)
    : cataState(cataState),
      intakeState(intakeState),
      wingState(wingState) {}

std::tuple<float, float> processSticks(nicklib::Gamepad* gamepad) {
    float forwardsVel = (*gamepad).leftY, turnVel = gamepad->rightX;

    forwardsVel = nicklib::deadzone(forwardsVel, CONTROLLER_DEADZONE);
    turnVel = nicklib::deadzone(turnVel, CONTROLLER_DEADZONE);

    forwardsVel = nicklib::curveInput(forwardsVel, CONTROLLER_CURVE_GAIN);
    turnVel = nicklib::curveInput(turnVel, CONTROLLER_CURVE_GAIN);

    return std::make_tuple(forwardsVel, turnVel * TURN_CONST);
}

bool isBHeld() {
    return (int)gamepad1.b > 500;
}

bool isSticksMoved() {
    return (gamepad1.leftX != 0 || gamepad1.leftY != 0 || gamepad1.rightX != 0 || gamepad1.rightY != 0);
}

void autoAlignUp_funct() {
    chassis.turnToHeading(90, 1000);
}
void autoAlignDown_funct() {
    chassis.turnToHeading(-90, 1000);
}

void rightSideBowl_funct() {
    chassis.moveToPose(40, -60, 90, 2000);
}