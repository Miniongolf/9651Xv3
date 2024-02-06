#include "globals.hpp"
#include "main.h"
#include "pros/motors.h"
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    // LVGL auton selector init
    selector::init();

    // Set chassis brake mode
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    // Retract wings
    rearWings.set_value(false);
    frontWings.set_value(false);
    
    // Initialize chassis
    chassis.calibrate();
    chassis.setPose(0, 0, 0);

    // Initialize subsystems
    cataMotors.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    cataMotors.tare_position();
    cataMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
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
void competition_initialize() { isCompMatch = true; }
