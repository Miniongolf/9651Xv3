#include "opcontrolHelpers.hpp"

std::array<float, 2> processSticks(float deadzone, bool curve) {
    // Stick curving lambda
    // Credit to Finlay 46846T for the curve formula
    auto curveInput = [](float& x) -> void {
        float a = STICK_CURVE_GAIN;
        if (x <= 0) {
            x = -1 * (1 - powf(M_E, -1 * a * x)) / (1 - powf(M_E, a));
        } else {
            x = (1 - powf(M_E, a * x)) / (1 - powf(M_E, a));
        }
    };

    float forwardsVel = gamepad1.leftY, turnVel = gamepad1.rightX;

    // Cross deadzone
    if (fabs(forwardsVel) < deadzone) forwardsVel = 0;
    if (fabs(forwardsVel) < deadzone) turnVel = 0;

    // Curve inputs
    if (curve) {
        curveInput(forwardsVel);
        curveInput(turnVel);
    }

    return {forwardsVel, turnVel * TURN_CONST};
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
    leftWingState = WingStates::retracted;
    rightWingState = WingStates::retracted;
}

// Subsystem states constructor
SysStates::SysStates(CataStates cataState, IntakeStates intakeState, std::array<WingStates, 2> wingStates)
    : cataState(cataState),
      intakeState(intakeState),
      leftWingState(wingStates[0]),
      rightWingState(wingStates[1]) {}