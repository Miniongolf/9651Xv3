#pragma once
#include "main.h"

void normalizeVels(float *vel1, float *vel2);

// Drive mode state machine enum
enum class DModes { normal, semiauton };

// Autoalign state machine enum
enum class AutoAlignStates { off, start, on, stop };

// Catapult state machine enum
enum class CataStates { load, fire, disconnect };

// Intake state machine enum
enum class IntakeStates { intake, outtake, stop };

// Single wing state machine enum
enum class WingStates { extended, retracted };

// Subsystems state machines struct
struct SysStates {
    SysStates();
    SysStates(CataStates cataState, IntakeStates intakeState,
              std::array<WingStates, 2> wingStates);

    CataStates cataState;
    IntakeStates intakeState;
    WingStates leftWingState;
    WingStates rightWingState;
};