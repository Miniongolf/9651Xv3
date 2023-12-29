#pragma once
#include "main.h"

// Drive mode enum class
enum class DMode {
	normal = 0,
	precision
};

// Autoalign state enum
enum class AutoAlignState {
	off = 0,
	on,
	start,
};

/* #region AutoAlign helper functions */
float getAutoAlignTarget(float robotTheta);
/* #endregion AutoAlign helper functions */