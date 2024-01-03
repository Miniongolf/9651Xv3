#include "opcontrolHelpers.hpp"

void normalizeVels(float *vel1, float *vel2) {
    float highVel = std::max(fabs(*vel1), fabs(*vel2));
    if (highVel > 1) {
        *vel1 /= highVel;
        *vel2 /= highVel;
    }
}

// Subsystem states default constructor
SysStates::SysStates() {
    cataState = CataStates::standby;
    intakeState = IntakeStates::stop;
    leftWingState = WingStates::retracted;
    rightWingState = WingStates::retracted;
}

void cataStateMachine(CataStates cataState) {
    switch (cataState) {
        case CataStates::active:
            cata.fire();
        case CataStates::standby:
            cata.standby();
        case CataStates::hardstop:
            cata.stop();
    }
}

void intakeStateMachine(IntakeStates intakeState) {

}

void wingStateMachine(WingStates leftWingState, WingStates rightWingState) {

}

void moveStateMachines(SysStates sysStates) {

}