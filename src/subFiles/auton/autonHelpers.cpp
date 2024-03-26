#include "subHeads/auton/autonHelpers.hpp"

// CLOSE POSITIONS
lemlib::Pose closeMatchLoadPose(-51, -49, -45);
lemlib::Pose closeTouchBarPose(-13, -56, 90);

lemlib::Pose closeLeftBall(-24, 0); // Barrier ball
lemlib::Pose closeRightBall(-3.4, 0); // Middle ball

// FAR POSITIONS
lemlib::Pose farTouchBarPose(5, -60, -90);
lemlib::Pose farMatchLoadPose(50, -50, -45);

lemlib::Pose farLeftBall(3.4, 0); // Far long barrier ball
lemlib::Pose farRightBall(24, 0); // Far middle ball
lemlib::Pose farCloseBall(3.4, -24); // Close long barrier ball
lemlib::Pose farBarBall(0, -60); // Elevation bar ball

void debug::printPose(bool wait) {
    if (wait) chassis.waitUntilDone();
    std::printf("x: %f | y: %f | θ: %f\n", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
}

/**
 * @brief Calculate the target position to move to based on the ball position, intake offset, and the target angle
 *
 * @param ballPoint The position of the ball as a lemlib::Pose
 * @param targetAngle The angle to end at (in degrees)
 */
lemlib::Pose calcTargetPose(lemlib::Pose ballPoint, float targetAngle) {
    float x = ballPoint.x + intakeOffset * sin(targetAngle);
    float y = ballPoint.y + intakeOffset * cos(targetAngle);
    return lemlib::Pose(x, y, targetAngle);
}

/**
 * @brief Move to the ball at the given position and angle
 *
 * @param ballPoint position of the ball as a lemlib::Pose
 * @param targetAngle target heading in degrees
 * @param timeout longest time the robot can spend moving
 * @param params struct to simulate named parameters
 */
void grabBall(lemlib::Pose ballPoint, float targetAngle, float timeout, lemlib::MoveToPoseParams params) {
    lemlib::Timer timer(0);
    timer.set(timeout);
    
    if (chassis.isInMotion()) chassis.waitUntilDone();

    lemlib::Pose targetPose = calcTargetPose(ballPoint, targetAngle);
    chassis.moveToPose(targetPose.x, targetPose.y, targetPose.theta, timeout, params);

    intakeMotors.move(127);
    while (intakeMotors[0].get_power() < 4.0 && chassis.isInMotion() && !timer.isDone()) {
        pros::delay(20);
    }
}