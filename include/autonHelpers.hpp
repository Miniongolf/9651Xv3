#pragma once
#include "main.h"

// Auton selection enum
enum class AutoSelect {
    testing = 0,
    closeQual = 1,
    closeSafe = 2,
    closeElim = 3,
    farQual = -1,
    farSafe = -2,
    farElim = -3
};

void closeQual_funct();
void closeSafe_funct();
void closeElim_funct();
void farQual_funct();
void farSafe_funct();
void farElim_funct();