#pragma once
#include "main.h"

/**
 * @brief Processes the inputs from the gamepad sticks and converts them to movement powers.
 *
 * @return Tuple of forwards power and turning power. Use std::tie() to unpack.
 */
std::tuple<float, float> processSticks(nicklib::Gamepad* gamepad);

// Drive mode state machine enum
enum class DModes { normal, override, semiauton };

// Autoalign state machine enum
enum class AutoAlignStates { off, start, on, stop };

// Catapult state machine enum
enum class CataStates { fire, idle };

// Intake state machine enum
enum class IntakeStates { intake, outtake, stop };

// Wings state machine enum
enum class WingStates { none, front, back };

// Balance piston state machine enum
enum class BalanceState { none, up, down };

// Single wing state machine enum
// enum class WingStates { extended, retracted };

// Subsystems state machines struct
struct SysStates {
        SysStates(CataStates cataState = CataStates::idle,
                  IntakeStates intakeState = IntakeStates::stop,
                  WingStates blockerState = WingStates::none,
                  BalanceState balanceState = BalanceState::none);

        CataStates cataState;
        IntakeStates intakeState;
        WingStates wingState;
        BalanceState balanceState;
};