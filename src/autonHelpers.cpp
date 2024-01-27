#include "autonHelpers.hpp"

// Red front auton
void frontQual_funct() {
    std::cout << "Front qual auto \n";
    chassis.setPose(-50, -58, 90);
    rearWings.set_value(true);
    chassis.turnTo(-65, -75, 2000, true);
    chassis.waitUntilDone();
    rearWings.set_value(false);
    chassis.moveToPoint(-58, -23, 1000);
    // outtake ball
    chassis.moveToPoint(-30, -59, 1000);
    chassis.moveToPoint(-4,-59,1000);
}

// Red front auton
void frontSafe_funct() {
    std::cout << "Front safe auto \n";
    // leftMotors.move(127);
    // rightMotors.move(127);
    // pros::delay(1400);
    // leftMotors.move(0);
    // rightMotors.move(0);
    // leftMotors.move(-70);
    // rightMotors.move(-70);
    // pros::delay(500);
    // chassis.turnTo(150, 0, 1000);
}

// Front elim auton
void frontElim_funct() { std::cout << "Front elim auto \n"; }

// Back qualification auton
void backQual_funct() { std::cout << "Back qual auto \n"; }

// Back safe auton
void backSafe_funct() { std::cout << "Back safe auto \n"; }

// Back elim auton
void backElim_funct() { std::cout << "Back elims auto \n"; }