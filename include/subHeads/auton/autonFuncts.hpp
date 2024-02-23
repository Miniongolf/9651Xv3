#pragma once
#include "main.h"
#include "subHeads/auton/autonHelpers.hpp"

// Helper functions
void chassisMovePowers(int leftPower, int rightPower, int time);
void chassisRam();


// Auton selector functions
void closeQual_funct();
void closeSafe_funct();
void closeElim_funct();
void farQual_funct();
void farSafe_funct();
void farElim_funct();
void skillsAuton_funct();