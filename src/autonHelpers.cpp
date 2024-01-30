#include "autonHelpers.hpp"
#include "globals.hpp"
#include "lemlib/chassis/chassis.hpp"

// Red front auton
void frontQual_funct() {
    std::cout << "Front qual auto \n";

    chassis.setPose(-48, -51, -45); // Initial pose
    chassis.moveToPose(-58, -27, 0, 2000, {.forwards=false, .minSpeed=127, .earlyExitRange=1}); // score preload
    chassis.waitUntil(5);
    intakeMotor.move(-127);
    chassis.waitUntil(10);
    intakeMotor.move(0);
    chassis.waitUntilDone();

    chassis.setPose(-58, -33, 0); // Odom reset on net
    chassis.moveToPose(-53, -51, 135, 2000, {.forwards=false, .lead=0.6}); // Move beside matchload bar
    rearWings.set_value(true); // Extend wings
    chassis.turnTo(chassis.getPose().x+100, chassis.getPose().y, 1000, false); // Turn to push triball out of matchload zone
    rearWings.set_value(false); // Retract wings
    chassis.moveToPose(-10, -58, 90, 1000, {.forwards=false, .lead=0.6}); // Touch matchload bar
}

// Red front auton
void frontSafe_funct() {
    std::cout << "Front safe auto \n";
    chassis.setPose(0, 0, 0);
    normalLeftMotors.move(127);
    normalRightMotors.move(127);
    pros::delay(1400);
    normalLeftMotors.move(0);
    normalRightMotors.move(0);
    normalLeftMotors.move(-70);
    normalRightMotors.move(-70);
    pros::delay(500);
    chassis.turnTo(150, 0, 1000);
}

// Front elim auton
void frontElim_funct() { std::cout << "Front elim auto \n"; }

// Back qualification auton
void backQual_funct() { std::cout << "Back qual auto \n"; }

// Back safe auton
void backSafe_funct() { std::cout << "Back safe auto \n"; }

// Back elim auton
void backElim_funct() {
    std::cout << "Back elims auto \n";
    chassis.setPose(50, -58, -45); // Set start pose
    chassis.moveToPose(24, -18, -15, 2000, {.lead=0.6, .minSpeed=100, .earlyExitRange=5}); // Interpolation to 1st triball
    chassis.moveToPose(0, 15, -75, 5000, {.lead=0.5}); // Grab 1st triball
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