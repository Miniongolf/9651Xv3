#include "opcontrolHelpers.hpp"

std::tuple<float, float> processSticks() {
    float forwardsVel = gamepad1.leftY, turnVel = gamepad1.rightX;

    forwardsVel = nicklib::deadzone(forwardsVel, CONTROLLER_DEADZONE);
    turnVel = nicklib::deadzone(turnVel, CONTROLLER_DEADZONE);

    forwardsVel = nicklib::curveInput(forwardsVel, CONTROLLER_CURVE_GAIN);
    turnVel = nicklib::curveInput(turnVel, CONTROLLER_CURVE_GAIN);

    return std::make_tuple(forwardsVel, turnVel * TURN_CONST);
}



void normalizeVels(float& vel1, float& vel2) {
    float highVel = std::max(fabs(vel1), fabs(vel2));
    if (highVel > 1) {
        vel1 /= highVel;
        vel2 /= highVel;
    }
}

void setDrivetrainMotors(pros::MotorGroup* newLeftMotors, pros::MotorGroup* newRightMotors) {
    chassis.drivetrain.leftMotors = newLeftMotors;
    chassis.drivetrain.rightMotors = newRightMotors;
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