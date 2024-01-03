#pragma once
#include "main.h"
#include <array>


void normalizeVels(float *vel1, float *vel2);

// Drive mode state machine enum class
enum class DMode {
	normal = 0,
	precision,
    semiauton
};

// Autoalign state machine enum
enum class AutoAlignState {
	off = 0,
	start,
	on,
    stop
};

// Catapult state machine enum
enum class CataStates : int {
    standby = -1,
    hardstop = 0,
    active = 1
};

// Intake state machine enum
enum class IntakeStates {
    intake = 0,
    outtake,
    stop
};

// Single wing state machine enum
enum class WingStates {
    extended = 0,
    retracted
};

// Subsystems state machines struct
struct SysStates {
	SysStates();

    CataStates cataState;
	IntakeStates intakeState;
    WingStates leftWingState;
	WingStates rightWingState;
};

void cataStateMachine(CataStates cataState);
void intakeStateMachine(IntakeStates intakeState);
void wingsStateMachine(WingStates leftWingState, WingStates rightWingState);