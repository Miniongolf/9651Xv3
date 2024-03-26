#include "subHeads/auton/autonFuncts.hpp"
#include "globals.hpp"
#include "subHeads/auton/autonHelpers.hpp"

void chassisMovePowers(int leftPower, int rightPower, int time) {
    // Wait until chassis is done async LemLib motions
    if (chassis.isInMotion()) chassis.waitUntilDone();

    chassis.tank(leftPower, rightPower);
    if (time >= 0) { pros::delay(time); }
    chassis.tank(0, 0);
}

void chassisRam() {
    chassisMovePowers(50, 50, 400);
    chassisMovePowers(-127, -127, 400);
}

void closeQual_funct() {
    chassis.setPose(-43.5, -58, -45);

    // Descore matchload zone
    chassisMovePowers(40, 40, 450);
    intakeMotors.move(-127);

    // Descore matchload bar
    rearWing.extend();
    pros::delay(500);
    intakeMotors.move(0);
    chassis.turnToHeading(-90, 1000);
    chassis.waitUntilDone();
    rearWing.retract();
    pros::delay(50);

    // Ram preload
    // chassis.moveToPose(-60, -28, 180, 1000, {.forwards = false, .minSpeed = 100});
    // chassisRam();

    // Touch elevation bar
    chassis.turnToHeading(90, 1250);
    intakeMotors.move(-127);
    chassis.moveToPose(-34, -57, 90, 2000, {.lead = 0, .maxSpeed = 100});
    chassis.moveToPose(closeTouchBarPose.x, closeTouchBarPose.y, closeTouchBarPose.theta, 2000,
                       {.lead = 0, .maxSpeed = 100});

    while (true) { pros::delay(50); }
}

void closeSafe_funct() {
    // Score preload
    chassis.setPose(-45, -57, 135);
    chassis.moveToPose(-60, -28, 180, 1000, {.forwards = false});
    chassis.waitUntilDone();
    chassisRam();
    chassis.setPose(-60, -32, 180); // odom reset

    // Move to descore
    chassis.moveToPose(-49, -47, 135, 2000);
    chassis.waitUntilDone();

    // Descore matchload bar
    rearWing.extend();
    pros::delay(500);
    intakeMotors.move(0);
    chassis.turnToHeading(45, 1000, {.forwards = false});
    chassis.waitUntilDone();
    rearWing.retract();

    // Touch elevation bar
    chassis.turnToHeading(135, 2000, {.forwards = false});
    intakeMotors.move(-127);
    chassis.moveToPose(-34, -60, 90, 2000, {.lead = 0, .maxSpeed = 100});
    chassis.moveToPose(closeTouchBarPose.x, closeTouchBarPose.y, closeTouchBarPose.theta, 2000,
                       {.lead = 0, .maxSpeed = 100});
    while (true) { pros::delay(50); }
}

void closeElim_funct() {
    int autonStartTime = pros::millis();

    // Set start pose
    chassis.setPose(-41.5, -57, 0);

    // Wing push preload near net
    leftWing.set_value(true);

    // Grab middle ball
    chassis.moveToPose(-30, -24, 30, 1500, {.minSpeed = 100});
    pros::delay(500);
    leftWing.set_value(false);
    chassis.moveToPose(-24, -4, 0, 1500);
    intakeMotors.move(127);

    // Come back and score preload
    chassis.moveToPose(-32, -32, 45, 1500, {.forwards = false, .minSpeed = 100});

    chassis.waitUntil(24);
    intakeMotors.move(0);

    chassis.moveToPose(-60, -36, 90, 1000, {.forwards = false, .minSpeed = 100});
    chassis.moveToPose(-60, -24, 180, 1000, {.forwards = false, .lead = 0});
    chassisRam();
    chassis.setPose(-60, -32, 180);

    chassisMovePowers(50, 50, 400);
}

void farQual_funct() {
    chassis.setPose(24, -58, -90); // Set start pose
    grabBall(farBarBall, -90, 2000); // Grab elevation bar ball
    chassis.moveToPose(50, -60, 90, 2000, {.forwards = false}); // Interpolation to matchload zone
    chassis.moveToPose(farMatchLoadPose.x, farMatchLoadPose.y, farMatchLoadPose.theta, 2000,
                       {.forwards = false, .lead = 0.6}); // Go to matchload zone
    chassis.waitUntilDone();
    rearWing.set_value(true);
    chassis.turnToHeading(180, 1000);
    chassis.turnToHeading(45, 1000, {.forwards = false});
    chassis.moveToPose(58, -24, 180, 1200, {.forwards = false});
    chassisRam();
}

void farSafe_funct() {
    chassis.setPose(0, 0, 0);
    chassis.tank(-127, -127);
    pros::delay(750);
    chassis.tank(70, 70);
    pros::delay(500);
    chassis.turnToHeading(45, 1000);
}

void farElim_funct() {
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

void skillsAuton_funct() {
    // Score preloads
    chassis.setPose(-45, -57, 135);
    chassis.moveToPose(-60, -28, 180, 1000, {.forwards = false, .minSpeed = 100});
    chassisRam();
    chassis.setPose(-60, -32, 180); // odom reset

    // Move to shoot
    chassis.moveToPose(-56, -47, -115, 2000, {.maxSpeed = 70}); // Move to matchload bar

    chassisMovePowers(-50, -50, 400);
    chassis.turnToHeading(-115, 1000, {.forwards = false});
    chassisMovePowers(-50, -50, 400);

    // Shoot
    chassis.waitUntilDone();

    cataMotors.tare_position();
    float startCataPose = cataMotors[0].get_position();
    int startCataTime = pros::millis();

    frontWings.set_value(true); // Deploy wings to maintain contact with bar
    cataMotors.move(127); // Start shooting

    // Wait to finish matchloading
    while (cataMotors[0].get_position() - startCataPose < 360 * 50) { pros::delay(20); }

    cataMotors.move(0); // Stop shooting
    frontWings.set_value(false); // Retract wings

    // Cross to other side
    chassis.moveToPoint(-29, -58, 2000, {.forwards = false}); // Interpolate to bowling position
    chassis.moveToPose(-32, -58, -90, 2000, {.forwards = false}); // Move to bowling position
    debug::printPose();
    chassis.moveToPose(45, -58, -90, 2000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 24});

    // Right push
    chassis.moveToPose(48, -48, -135, 1000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 10});
    chassis.moveToPose(60, -28, 180, 2000, {.forwards = false, .minSpeed = 127});
    chassisRam();

    // Middle push 1
    chassisMovePowers(50, 50, 750);
    chassis.turnToHeading(-45, 1000);
    chassis.moveToPoint(29, -33, 2000);
    chassis.moveToPose(18, -10, 0, 2000);
    chassis.waitUntilDone();
    frontWings.set_value(true);
    chassis.moveToPose(38, -7, 90, 2000);
    chassisMovePowers(-50, -50, 400);
    chassis.arcade(0, 0);
}