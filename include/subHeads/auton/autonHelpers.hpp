#pragma once
#include "main.h"

// Auton selection enum
enum class AutoSelect {
    skills = 0,
    closeQual = 1,
    closeSafe = 2,
    closeElim = 3,
    farQual = -1,
    farSafe = -2,
    farElim = -3
};

constexpr float intakeOffset = 9.0;

namespace debug {
void printPose(bool wait = true);
}

extern lemlib::Pose closeMatchLoadPose;
extern lemlib::Pose closeTouchBarPose;

extern lemlib::Pose closeLeftBall;
extern lemlib::Pose closeRightBall;

extern lemlib::Pose farTouchBarPose;
extern lemlib::Pose farMatchLoadPose;

extern lemlib::Pose farLeftBall;
extern lemlib::Pose farRightBall;
extern lemlib::Pose farCloseBall;
extern lemlib::Pose farBarBall;

/**
 * @brief Calculate the target position to move to based on the ball position and the target angle
 *
 * @param ballPoint The position of the ball as a lemlib::Pose
 * @param targetAngle The angle to end at
 */
lemlib::Pose calcTargetPose(lemlib::Pose ballPoint, float targetAngle);

/**
 * @brief Move to the ball at the given position and angle
 *
 * @param ballPoint position of the ball as a lemlib::Pose
 * @param targetAngle target heading in degrees
 * @param timeout longest time the robot can spend moving
 * @param grabDistance movement distance to wait before grabbing. -1 by default, meaning no grab.
 * @param params struct to simulate named parameters
 */
void moveToBall(lemlib::Pose ballPoint, float targetAngle, float timeout, int grabDistance = -1,
                lemlib::MoveToPoseParams params = {});