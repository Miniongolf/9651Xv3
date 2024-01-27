#pragma once
#include "main.h"

/**
 * @brief Processes the inputs from the gamepad sticks and converts them to movement powers.
 *
 * @return Tuple of forwards power and turning power. Use std::tie() to unpack.
 */
std::tuple<float, float> processSticks();

// Drive mode state machine enum
enum class DModes { normal, reverse, semiauton };

// Autoalign state machine enum
enum class AutoAlignStates { off, start, on, stop };

// Catapult state machine enum
enum class CataStates { fire, idle };

// Intake state machine enum
enum class IntakeStates { intake, outtake, stop };

// Blocker state machine enum
enum class BlockerStates { up, down, hang };

// Single wing state machine enum
// enum class WingStates { extended, retracted };

// Subsystems state machines struct
struct SysStates {
        SysStates(CataStates cataState = CataStates::idle,
                  IntakeStates intakeState = IntakeStates::stop,
                  BlockerStates blockerState = BlockerStates::down);

        CataStates cataState;
        IntakeStates intakeState;
        BlockerStates blockerState;
};