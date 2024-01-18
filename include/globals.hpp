#pragma once

#include "lemlib/api.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/adi.hpp"
#include "pros/imu.hpp"

#include "subHeads/gamepad.hpp"
#include "subHeads/wings.hpp"

// GAMEPADS
extern Gamepad gamepad1; // Primary controller

// SENSORS
extern pros::IMU imu; // Inertial Measurement Unit

// SUBSYS MOTORS
extern std::array<pros::Motor, 2> ptoMotors;

// MOTOR GROUPS
extern pros::MotorGroup* leftMotorsPtr;
extern pros::MotorGroup* rightMotorsPtr;

extern pros::MotorGroup normalLeftMotors; // Left side drivetrain motor group
extern pros::MotorGroup normalRightMotors; // Right side drivetrain motor group

extern pros::MotorGroup ptoLeftMotors; // Left side drivetrain motor group
extern pros::MotorGroup ptoRightMotors; // Right side drivetrain motor group

// INTAKE MOTOR
extern pros::Motor intakeMotor;

// CATA MOTORS
extern pros::MotorGroup cataMotors; // Catapult motor group

// SUBSYSTEMS
extern Wings wings; // Wings wrapper class

// CHASSIS PID
extern lemlib::ControllerSettings lateralController; // LemLib Lateral PID
extern lemlib::ControllerSettings angularController; // LemLib Turning PID

// LEMLIB CHASSIS
extern lemlib::Drivetrain drivetrain; // LemLib Drivetrain
extern lemlib::OdomSensors odomSensors; // LemLib Odometry sensors
extern lemlib::Chassis chassis; // LemLib Chassis

// MISC
extern bool isCompMatch;