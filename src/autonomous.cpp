#include "autonHelpers.hpp"
#include "main.h"

/**
 * LemLib path txt file assets
 */
ASSET(jerrycurve_txt);

// Auton selection enum
enum class AutoSelect {
    frontQual = 1,
    frontSafe = 2,
    frontElim = 3,
    backQual = -1,
    backSafe = -2,
    backElim = -3,
    doNothing = 0
};

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
    std::cout << "auton\n";
    chassis.setPose(0, 0, 0);

    chassis.turnTo(100, 0, 1000);
    chassis.waitUntilDone();
    std::cout << "turned | " << chassis.getPose().x << ' ' << chassis.getPose().y << ' ' << chassis.getPose().theta
              << '\n';
    pros::delay(1000);

    chassis.moveToPoint(24, 0, 2000);
    chassis.waitUntilDone();
    std::cout << "moved | " << chassis.getPose().x << ' ' << chassis.getPose().y << ' ' << chassis.getPose().theta
              << '\n';

    AutoSelect autonMode = static_cast<AutoSelect>(selector::auton);

    switch (autonMode) {
        case AutoSelect::frontQual: frontQual_funct(); break;
        case AutoSelect::frontSafe: frontSafe_funct(); break;
        case AutoSelect::frontElim: frontElim_funct(); break;
        case AutoSelect::backQual: backSafe_funct(); break;
        case AutoSelect::backSafe: backSafe_funct(); break;
        case AutoSelect::backElim: frontElim_funct(); break;
        case AutoSelect::doNothing: backElim_funct(); break;
    }
}