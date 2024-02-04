#include "autonFuncts.hpp"
#include "autonHelpers.hpp"

void closeQual_funct() {
    std::cout << "Close qual auto \n";

    chassis.setPose(-48, -55, -45); // Initial pose
    chassis.moveToPose(-75, -0, 0, 1250); // score preload
    chassis.waitUntil(24);
    intakeMotors.move(-127);
    chassis.waitUntilDone();
    intakeMotors.move(0);
    chassis.tank(-50, -50);
    pros::delay(300);
    chassis.tank(127, 127);
    pros::delay(500);
    chassis.tank(0, 0);
    pros::delay(500);
    chassis.setPose(-58, -33, 0); // Odom reset on net

    chassis.moveToPose(-51, -49, -45, 2000, {.forwards = false});
    rearWings.extend();
    pros::delay(500);
    intakeMotors.move(0);
    chassis.turnToHeading(45, 1000, {.forwards = false});
    chassis.waitUntilDone();
    rearWings.retract();

    // Touch matchload bar
    chassis.turnToHeading(135, 2000, {.forwards = false});
    intakeMotors.move(-127);
    chassis.moveToPose(-34, -60, 90, 2000, {.lead = 0, .maxSpeed = 100});
    chassis.moveToPose(-8, -60, 90, 2000, {.lead = 0, .maxSpeed = 100});
    while (true) { pros::delay(50); }
}

void closeSafe_funct() {
    std::cout << "Close safe auto \n";
    // Matchload zone descore
    chassis.arcade(50, 0);
    pros::delay(450);
    chassis.arcade(0, 0);
    intakeMotors.move(-127);
    chassis.setPose(-51, -49, -45);
    rearWings.extend();
    pros::delay(500);
    intakeMotors.move(0);
    chassis.turnToHeading(45, 1000, {.forwards = false});
    chassis.waitUntilDone();
    rearWings.retract();

    // Touch matchload bar
    chassis.turnToHeading(135, 2000, {.forwards = false});
    intakeMotors.move(-127);
    chassis.moveToPose(-34, -60, 90, 2000, {.lead = 0, .maxSpeed = 100});
    chassis.moveToPose(-5, -60, 90, 2000, {.lead = 0, .maxSpeed = 100});
    while (true) { pros::delay(50); }
}

void closeElim_funct() { std::cout << "Close elim auto \n"; }

void farQual_funct() {
    std::cout << "Far 3-ball \n";
    chassis.setPose(24, -58, -90); // Set start pose
    moveToBall(farBarBall, -90, 2000, 0); // Grab elevation bar ball
    chassis.moveToPose(50, -60, 90, 2000, {.forwards = false}); // Interpolation to matchload zone
    chassis.moveToPose(farMatchLoadPose.x, farMatchLoadPose.y, farMatchLoadPose.theta, 2000, {.lead = 0.6}); // Interpolation to matchload
}

void farSafe_funct() {
    std::cout << "Far safe \n";
    chassis.setPose(0, 0, 0);
    chassis.tank(-127, -127);
    pros::delay(750);
    chassis.tank(70, 70);
    pros::delay(500);
    chassis.turnToHeading(45, 1000);
}

void farElim_funct() {
    std::cout << "Far 6-ball rush \n";
    chassis.setPose(50, -58, -45); // Set start pose
    chassis.moveToPose(24, -18, -15, 2000,
                       {.lead = 0.6, .minSpeed = 100, .earlyExitRange = 5}); // Interpolation to 1st triball
    chassis.moveToPose(0, 15, -75, 5000, {.lead = 0.5}); // Grab 1st triball
    intakeMotors.move(127); // Run intake
    chassis.waitUntilDone();
    intakeMotors.move(0);
    chassis.turnToHeading(90, 1000); // Turn to face net
    chassis.waitUntilDone();
    frontWings.extend();

    // chassis.waitUntilDone();
    // frontWings.extend(); // Extend wings to hit 2nd triball
    // chassis.moveToPose(46, -6, 90, 2000, {.lead=0.6, .minSpeed=127, .earlyExitRange=2}); // Score both
    // chassis.waitUntilDone();
    // frontWings.retract(); // Retract wings
    // chassis.moveToPoint(34.27, -15.49, 1000, false); // Back away from net
    // chassis.moveToPoint(12, -21, 2000, {}); // Grab 3rd triball
    // chassis.moveToPose(48, -13, 90, 2000, {.minSpeed=127, .earlyExitRange=2}); // Score 3rd triball
    // chassis.moveToPoint(37, -42, 1000); // Move away from the net backwards
    // chassis.moveToPose(6, -58, -90, 2000, {.lead=0.6, .earlyExitRange=2}); // Grab 4th triball
    // chassis.moveToPose(61, -49, 45, 2000, {.lead=0.6});

    // chassis.setPose(46, -9, 44);
}