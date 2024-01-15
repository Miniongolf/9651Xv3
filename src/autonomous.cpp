#include "autonHelpers.hpp"
#include "main.h"

/**
 * LemLib path txt file assets
 */
ASSET(jerrycurve_txt);

// Auton selection enum
enum autonSelection {
  redFront = 1,
  redBack = 2,
  redNone = 3,
  blueFront = -1,
  blueBack = -2,
  blueNone = -3,
  skills = 0
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
  std::cout << "turned | " << chassis.getPose().x << ' ' << chassis.getPose().y << ' '
            << chassis.getPose().theta << '\n';
  pros::delay(1000);
  
  chassis.moveToPoint(24, 0, 2000);
  chassis.waitUntilDone();
  std::cout << "moved | " << chassis.getPose().x << ' ' << chassis.getPose().y << ' '
            << chassis.getPose().theta << '\n';

  autonSelection autonMode = static_cast<autonSelection>(selector::auton);
  
  std::cout << "selected auton | " << autonMode << '\n';

  // switch (autonMode) {
  //     case redFront:
  //         redFront_funct();
  //         break;
  //     case redBack:
  //         redBack_funct();
  //         break;
  //     case redNone:
  //         doNothing_funct();
  //         break;
  //     case blueFront:
  //         blueFront_funct();
  //         break;
  //     case blueBack:
  //         blueBack_funct();
  //         break;
  //     case blueNone:
  //         doNothing_funct();
  //         break;
  //     case skills:
  //         skillsAuto_funct();
  //         break;
  //     default:
  //         doNothing_funct();
  // }
}