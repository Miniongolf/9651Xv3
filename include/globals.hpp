#pragma once

#include "lemlib/api.hpp"
#include "pros/motors.hpp"
#include "pros/adi.hpp"
#include "pros/imu.hpp"

#include "subHeads/gamepad.hpp"
#include "subHeads/cata.hpp"
#include "subHeads/wings.hpp"

// GAMEPADS
extern Gamepad gamepad1; // Primary controller
extern Gamepad gamepad2; // Secondary controller

// SENSORS
extern pros::IMU imu; // Inertial Measurement Unit

// DRIVE MOTORS
extern pros::Motor lf;
extern pros::Motor lm;
extern pros::Motor lb;
extern pros::Motor ls;
extern pros::Motor rf;
extern pros::Motor rm;
extern pros::Motor rb;
extern pros::Motor rs;

// SUBSYS MOTORS
extern pros::Motor cataMotor;
extern pros::Motor intakeMotor;

// MOTOR GROUPS
extern pros::MotorGroup leftMotors; // Left side drivetrain motor group
extern pros::MotorGroup rightMotors; // Right side drivetrain motor group
extern pros::MotorGroup cataMotors; // Catapult motor group

// SUBSYSTEMS
extern Cata cata;   // Catapult wrapper class
extern Wings wings; // Wings wrapper class

// CHASSIS PID
extern lemlib::ControllerSettings lateralController; // LemLib Lateral PID
extern lemlib::ControllerSettings angularController; // LemLib Turning PID

// LEMLIB CHASSIS
extern lemlib::Drivetrain drivetrain;                // LemLib Drivetrain
extern lemlib::OdomSensors odomSensors;              // LemLib Odometry sensors
extern lemlib::Chassis chassis;                      // LemLib Chassis

// MISC
extern bool isCompMatch;