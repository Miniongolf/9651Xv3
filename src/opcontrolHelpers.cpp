#include "opcontrolHelpers.hpp"

void normalizeVels(float& vel1, float& vel2) {
    float highVel = std::max(fabs(vel1), fabs(vel2));
    if (highVel > 1) {
        vel1 /= highVel;
        vel2 /= highVel;
    }
}

void setDrivetrainMotors(pros::MotorGroup *newLeftMotors, pros::MotorGroup *newRightMotors) {
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
SysStates::SysStates(CataStates cataState, IntakeStates intakeState,
                     std::array<WingStates, 2> wingStates)
    : cataState(cataState), intakeState(intakeState),
      leftWingState(wingStates[0]), rightWingState(wingStates[1]) {}