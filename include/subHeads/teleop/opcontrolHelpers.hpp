#pragma once
#include "globals.hpp"
#include "main.h"

// Drive mode state machine enum
enum class DModes { normal, override, semiauton };

// Autoalign state machine enum
enum class AutoAlignStates { off, start, on, stop };

// Catapult state machine enum
enum class CataStates { fire, load, idle };

// Intake state machine enum
enum class IntakeStates { intake, outtake, stop };

// Wings state machine enum
enum class WingStates { none, left, right, front, back };

// Hang state machine enum
enum class HangStates { up, down };

// Subsystems state machines struct
struct SysStates {
        SysStates(CataStates cataState = CataStates::idle,
                  IntakeStates intakeState = IntakeStates::stop,
                  WingStates wingState = WingStates::none,
                  HangStates hangState = HangStates::down);

        CataStates cataState;
        IntakeStates intakeState;
        WingStates wingState;
        HangStates hangState;
};

/**
 * @brief Processes the inputs from the gamepad sticks and converts them to movement powers.
 *
 * @return Tuple of forwards power and turning power. Use std::tie() to unpack.
 */
std::tuple<float, float> processSticks(nicklib::Gamepad* gamepad);

bool isBHeld();
bool isSticksMoved();

void autoAlignUp_funct();
void autoAlignDown_funct();

void rightSideBowl_funct();