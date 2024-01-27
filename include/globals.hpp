#pragma once

#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/adi.hpp"
#include "pros/imu.hpp"

#include "lemlib/api.hpp"

#include "nicklib/gamepad.hpp"

#include "subHeads/pneumatics.hpp"

// GAMEPADS
extern nicklib::Gamepad gamepad1; // Primary controller

// SENSORS
extern pros::IMU imu; // Inertial Measurement Unit

// SUBSYS MOTORS
extern std::array<pros::Motor, 2> ptoMotors;

// MOTOR GROUPS
extern pros::MotorGroup normalLeftMotors; // Left side drivetrain motor group (normal)
extern pros::MotorGroup normalRightMotors; // Right side drivetrain motor group (normal)

// CATA MOTORS
extern pros::MotorGroup cataMotors; // Catapult motor group

// CHASSIS PID
extern lemlib::ControllerSettings lateralController; // LemLib Lateral PID
extern lemlib::ControllerSettings angularController; // LemLib Turning PID

// LEMLIB CHASSIS
extern lemlib::Drivetrain drivetrain; // LemLib Drivetrain
extern lemlib::OdomSensors odomSensors; // LemLib Odometry sensors
extern lemlib::Chassis chassis; // LemLib Chassis

// MISC
extern bool isCompMatch;