#include "globals.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "subHeads/auton/autonFuncts.hpp"

namespace debug {
void printPose(bool wait = true) {
    if (wait) chassis.waitUntilDone();
    std::printf("x: %f | y: %f | θ: %f\n", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
}
} // namespace debug

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
    AutoSelect autonMode = static_cast<AutoSelect>(selector::auton);

    switch (autonMode) {
        case AutoSelect::closeQual: closeQual_funct(); break;
        case AutoSelect::closeSafe: closeSafe_funct(); break;
        case AutoSelect::closeElim: closeElim_funct(); break;
        case AutoSelect::farQual: farQual_funct(); break;
        case AutoSelect::farSafe: farSafe_funct(); break;
        case AutoSelect::farElim: farElim_funct(); break;
        case AutoSelect::testing:
            // if (isCompMatch) { // Plugged into comp controller
                std::printf("comp");
                // Score preloads
                chassis.setPose(-45, -57, 135);
                chassis.moveToPose(-60, -28, 180, 1000, {.forwards = false, .minSpeed = 100});
                chassis.waitUntilDone();
                chassis.arcade(50, 0);
                pros::delay(400);
                chassis.arcade(-127, 0);
                pros::delay(400);
                chassis.setPose(-60, -32, 180); // odom reset

                // Move to shoot
                chassis.moveToPose(-56, -47, -115, 2000, {.maxSpeed = 70}); // Move to matchload bar

                // Shoot
                chassis.waitUntilDone();

                cataMotors.tare_position();
                float startCataPose = cataMotors[0].get_position();
                int startCataTime = pros::millis();

                frontWings.set_value(true); // Deploy wings to maintain contact with bar
                cataMotors.move(127); // Start shooting

                // Wait to finish matchloading
                while (cataMotors[0].get_position() - startCataPose < 360 * 50) { pros::delay(10); }

                cataMotors.move(0); // Stop shooting
                frontWings.set_value(false); // Retract wings

                chassis.setPose(-56, -48, -115);

                // Cross to other side
                chassis.moveToPoint(-29, -58, 2000, {.forwards = false}); // Interpolate to bowling position
                chassis.moveToPose(-32, -58, -90, 2000, {.forwards = false}); // Move to bowling position
                debug::printPose();
                chassis.moveToPose(45, -58, -90, 2000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 24});

                // Right push
                chassis.moveToPose(48, -48, -135, 1000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 10});
                chassis.moveToPose(60, -28, 180, 2000, {.forwards = false, .minSpeed = 127});
                chassis.waitUntilDone();
                chassis.arcade(50, 0);
                pros::delay(400);
                chassis.arcade(-127, 0);
                pros::delay(400);

                // Middle push 1
                chassis.waitUntilDone();
                chassis.arcade(50, 0);
                pros::delay(750);
                chassis.turnToHeading(-45, 1000);
                chassis.moveToPoint(29, -33, 2000);
                chassis.moveToPose(18, -10, 0, 2000);
                chassis.waitUntilDone();
                frontWings.set_value(true);
                chassis.moveToPose(38, -7, 90, 2000);
                chassis.waitUntilDone();
                chassis.arcade(-50, 0);
                pros::delay(400);
                chassis.arcade(0, 0);

                // Middle push 2

                // Left push

            // } else { // Testing
            //     std::printf("testing");
            //     chassis.setPose(-45, -57, 135);
            //     chassis.moveToPose(-60, -28, 180, 2000, {.forwards = false});
            //     debug::printPose();
            //     chassis.waitUntilDone();
            //     chassis.arcade(50, 0);
            //     pros::delay(400);
            //     chassis.arcade(-127, 0);
            //     pros::delay(400);
            //     chassis.setPose(-60, -32, 180); // odom reset

            //     chassis.moveToPose(-56, -47, -100, 2000, {.maxSpeed = 70});
            //     debug::printPose();

            //     // chassis.setPose(0, 0, 0);
            //     // chassis.turnToHeading(90, 2000);
            //     // debug::printPose();
            //     // std::printf("error: %f\n", chassis.getPose().theta - 90);
            //     // pros::delay(1000);
            //     // chassis.moveToPose(48, 0, 90, 2000);
            //     // debug::printPose();
            //     // std::printf("error: %f\n", chassis.getPose().x - 48);
            // }

            // chassis.setPose(0, 0, 0);
            // lemlib::Pose testBall(35, 48);
            // moveToBall(testBall, 90, 1000, 50);
            // chassis.setPose(0, 0, 0);
            // chassis.moveToPose(0, 24, 0, 5000);
            // chassis.waitUntilDone();
            // std::cout << fmt::format("x: {} | y: {} | θ: {}\n", chassis.getPose().x, chassis.getPose().y,
            //                          chassis.getPose().theta);
            // chassis.turnToHeading(90, 5000);
            // chassis.waitUntilDone();
            // std::cout << fmt::format("x: {} | y: {} | θ: {}\n", chassis.getPose().x, chassis.getPose().y,
            //                          chassis.getPose().theta);
    }
}