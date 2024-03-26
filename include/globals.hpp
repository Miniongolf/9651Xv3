#pragma once

#include "pros/distance.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/adi.hpp"
#include "pros/imu.hpp"

#include "lemlib/api.hpp"

#include "nicklib/gamepad.hpp"

#include "subHeads/pneumatics.hpp"

// Gamepads
extern nicklib::Gamepad gamepad1;
extern nicklib::Gamepad gamepad2;

// SENSORS
extern pros::IMU imu; // Inertial Measurement Unit
// extern pros::Distance cataDistance; // Catapult distance sensor
// extern pros::Distance leftDistance; // Left distance sensor
// extern pros::Distance rightDistance; // Right distance sensor

// MOTOR GROUPS
extern pros::MotorGroup normalLeftMotors; // Left side drivetrain motor group (normal)
extern pros::MotorGroup normalRightMotors; // Right side drivetrain motor group (normal)

// INTAKE MOTOR
extern pros::MotorGroup intakeMotors;

// CATA MOTORS
extern pros::MotorGroup cataMotors; // Catapult motor group

// SUBSYSTEMS
extern pros::ADIDigitalOut leftWing; // Front wings piston group
extern pros::ADIDigitalOut rightWing; // Front wings piston group
extern PistonGroup frontWings; // Front wings piston group
extern PistonGroup rearWing; // Rear wing piston
extern pros::ADIDigitalOut hangArm; // Hang arm pistons
extern pros::ADIDigitalOut hangRatchet; // Ratchet pistons

// CHASSIS PID
extern lemlib::ControllerSettings lateralController; // LemLib Lateral PID
extern lemlib::ControllerSettings angularController; // LemLib Turning PID

// LEMLIB CHASSIS
extern lemlib::Drivetrain drivetrain; // LemLib Drivetrain
extern lemlib::OdomSensors odomSensors; // LemLib Odometry sensors
extern lemlib::Chassis chassis; // LemLib Chassis

// MISC
extern bool isCompMatch;