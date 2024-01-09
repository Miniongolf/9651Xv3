#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    // LVGL auton selector init
    selector::init();
    
    // Initialize chassis
    leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
    rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
    
    chassis.calibrate();

    // Tuck in wings
    wings.retract(Wings::B);

    // Default start position (will change depending on selected auton)
    lemlib::Pose origin(0, 0, 0);
    chassis.setPose(origin);
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	isCompMatch = true;
}

