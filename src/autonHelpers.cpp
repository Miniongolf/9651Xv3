#include "autonHelpers.hpp"

// Red front auton
void redFront_funct() {
    std::cout << "Red front auto \n";
    // leftMotors.move(127);
    // rightMotors.move(127);
    // pros::delay(1400);
    // leftMotors.move(0);
    // rightMotors.move(0);
    // leftMotors.move(-70);
    // rightMotors.move(-70);
    // pros::delay(500);
    // chassis.turnTo(-120, 0, 1000);
}

// Red front auton
void redBack_funct() {
    std::cout << "Red back auto \n";
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

// Literally just do nothing
void doNothing_funct() { std::cout << "Do nothing auto \n"; }

// Blue front auton
void blueFront_funct() {
    std::cout << "Blue front auto \n";
    redFront_funct();
}

// Blue back auton
void blueBack_funct() {
    std::cout << "Blue back auto \n";
    redBack_funct();
}

// Skills auton
void skillsAuto_funct() { std::cout << "Skills auto \n"; }