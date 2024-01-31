#include "autonHelpers.hpp"
#include "globals.hpp"
#include "lemlib/chassis/chassis.hpp"

void closeQual_funct() {
    std::cout << "Close qual auto \n";

    chassis.setPose(-48, -55, -45); // Initial pose
    chassis.moveToPose(-75, -0, 0, 1250); // score preload
    chassis.waitUntil(24);
    intakeMotor.move(-127);
    chassis.waitUntilDone();
    intakeMotor.move(0);
    chassis.tank(-50, -50);
    pros::delay(500);
    chassis.tank(127, 127);
    pros::delay(500);
    chassis.tank(0, 0);
    pros::delay(500);
    chassis.setPose(-58, -33, 0); // Odom reset on net

    chassis.moveToPose(-53, -65, -45, 5000, {.forwards = false, .lead = 0.6}); // Move beside matchload bar
    // rearWings.set_value(true); // Extend wings
    // chassis.turnTo(chassis.getPose().x + 100, chassis.getPose().y, 1000,
    //                false); // Turn to push triball out of matchload zone
    // rearWings.set_value(false); // Retract wings
    // chassis.moveToPose(-10, -58, 90, 1000, {.forwards = false, .lead = 0.6}); // Touch matchload bar
}

void closeSafe_funct() {
    std::cout << "Close safe auto \n";
    chassis.setPose(-51, -49, -45);
    rearWings.extend();
    pros::delay(500);
    chassis.turnTo(0, 0, 1000, false);
    chassis.waitUntilDone();
    rearWings.retract();
    chassis.moveToPose(-8, -59, 90, 2000, {.forwards = false, .lead = 0.6});
    
}

void closeElim_funct() { std::cout << "Close elim auto \n"; }

void farQual_funct() {
    std::cout << "Far 3-ball \n";
    chassis.setPose(10, -58, -90); // Set start pose
    intakeMotor.move(127); // Run intake
    chassis.moveToPose(50, -49, 45, 2000, {.lead = 0.6, .earlyExitRange = 2}); // Interpolation to 1st triball
}

void farSafe_funct() {
    std::cout << "Far safe \n";
    chassis.setPose(0, 0, 0);
    chassis.tank(-127, -127);
    pros::delay(750);
    chassis.tank(70, 70);
    pros::delay(500);
    chassis.turnTo(0, 0, 1000);
}

void farElim_funct() {
    std::cout << "Far 6-ball rush \n";
    chassis.setPose(50, -58, -45); // Set start pose
    chassis.moveToPose(24, -18, -15, 2000,
                       {.lead = 0.6, .minSpeed = 100, .earlyExitRange = 5}); // Interpolation to 1st triball
    chassis.moveToPose(0, 15, -75, 5000, {.lead = 0.5}); // Grab 1st triball
    intakeMotor.move(127); // Run intake
    chassis.waitUntilDone();
    intakeMotor.move(0);
    chassis.turnTo(1000, chassis.getPose().y, 1000); // Turn to face net
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