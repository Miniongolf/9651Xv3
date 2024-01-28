#include "autonHelpers.hpp"
#include "globals.hpp"
#include "lemlib/chassis/chassis.hpp"

// Red front auton
void frontQual_funct() {
    std::cout << "Front qual auto \n";

    chassis.setPose(-48, -51, -45);
    chassis.moveToPose(-58, -27, 0, 2000, {.forwards=false, .minSpeed=127});
    // chassis.moveToPose(-46, -58, -15, 2000, {.forwards=false});
    // rearWings.set_value(false);
    // chassis.turnTo(chassis.getPose().x + 100, chassis.getPose().y, 2000);
}

// Red front auton
void frontSafe_funct() {
    std::cout << "Front safe auto \n";
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
void backElim_funct() { std::cout << "Back elims auto \n"; }