#include "autoSelect/selection.h"
#include "autonHelpers.hpp"
#include "main.h"

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
    // closeQual_funct();

    // normalLeftMotors.move(-127);
    // normalRightMotors.move(-127);
    // pros::delay(1400);
    // normalLeftMotors.move(70);
    // normalRightMotors.move(70);
    // pros::delay(500);
    // chassis.turnTo(150, 0, 1000);

    AutoSelect autonMode = static_cast<AutoSelect>(selector::auton);

    switch (autonMode) {
        case AutoSelect::closeQual: closeQual_funct(); break;
        case AutoSelect::closeSafe: closeSafe_funct(); break;
        case AutoSelect::closeElim: closeElim_funct(); break;
        case AutoSelect::farQual: farQual_funct(); break;
        case AutoSelect::farSafe: farSafe_funct(); break;
        case AutoSelect::farElim: farElim_funct(); break;
        case AutoSelect::testing:
            chassis.setPose(0, 0, 0);
            chassis.moveToPose(0, 24, 0, 1000);
    }
}