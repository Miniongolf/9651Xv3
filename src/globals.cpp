#include "globals.hpp"
#include "pros/motors.hpp"

// GAMEPADS
Gamepad gamepad1(pros::E_CONTROLLER_MASTER);  // Primary controller
Gamepad gamepad2(pros::E_CONTROLLER_PARTNER); // Secondary controller

// SENSORS
pros::IMU imu(10);

// DRIVE MOTORS
/*
pros::Motor lf(-1, pros::E_MOTOR_GEAR_BLUE);
pros::Motor lm(-2, pros::E_MOTOR_GEAR_BLUE);
pros::Motor lb(-3, pros::E_MOTOR_GEAR_BLUE);
pros::Motor ls(-4, pros::E_MOTOR_GEAR_GREEN);

pros::Motor rf(5, pros::E_MOTOR_GEAR_BLUE);
pros::Motor rm(6, pros::E_MOTOR_GEAR_BLUE);
pros::Motor rb(7, pros::E_MOTOR_GEAR_BLUE);
pros::Motor rs(8, pros::E_MOTOR_GEAR_GREEN);
*/

std::array<pros::Motor, 2> ptoMotors = {
    pros::Motor(4, pros::E_MOTOR_GEAR_GREEN),
    pros::Motor(-8, pros::E_MOTOR_GEAR_GREEN)};

// CATA MOTORS
pros::Motor cataMotor(9, pros::E_MOTOR_GEAR_GREEN);
pros::Motor intakeMotor(10, pros::E_MOTOR_GEAR_BLUE);

// MOTOR GROUPS
pros::MotorGroup leftMotors({pros::Motor(1, pros::E_MOTOR_GEAR_BLUE),
                             pros::Motor(2, pros::E_MOTOR_GEAR_BLUE),
                             pros::Motor(3, pros::E_MOTOR_GEAR_BLUE),
                             pros::Motor(4, pros::E_MOTOR_GEAR_GREEN)});

pros::MotorGroup rightMotors({pros::Motor(-5, pros::E_MOTOR_GEAR_BLUE),
                              pros::Motor(-6, pros::E_MOTOR_GEAR_BLUE),
                              pros::Motor(-7, pros::E_MOTOR_GEAR_BLUE),
                              pros::Motor(-8, pros::E_MOTOR_GEAR_GREEN)});

pros::MotorGroup cataMotors({cataMotor});

// SUBSYSTEMS
Wings wings('A', 'H');

// CHASSIS PID
lemlib::ControllerSettings
    lateralController(8,   // proportional gain (kP)
                      0,   // integral gain (kI)
                      3,   // derivative gain (kD)
                      0,   // anti windup
                      1,   // small error range, in inches
                      100, // small error range timeout, in milliseconds
                      3,   // large error range, in inches
                      500, // large error range timeout, in milliseconds
                      127  // maximum acceleration (slew)
    );

lemlib::ControllerSettings
    angularController(8,   // proportional gain (kP)
                      0,   // integral gain (kI)
                      2,   // derivative gain (kD)
                      0,   // anti windup
                      3,   // small error range, in degrees
                      100, // small error range timeout, in milliseconds
                      5,   // large error range, in degrees
                      500, // large error range timeout, in milliseconds
                      127  // maximum acceleration (slew)
    );

// LEMLIB CHASSIS
lemlib::Drivetrain drivetrain(&leftMotors,  // left drivetrain motors
                              &rightMotors, // right drivetrain motors
                              15,           // track width
                              lemlib::Omniwheel::NEW_325, // wheel diameter
                              450,                        // wheel rpm
                              8                           // chase power
);

lemlib::OdomSensors odomSensors(
    nullptr, // vertical tracking wheel 1
    nullptr, // vertical tracking wheel 2
    nullptr, // horizontal tracking wheel 1
    nullptr, // we don't have a second tracking wheel, so we set it to nullptr
    &imu     // inertial sensor
);

lemlib::Chassis chassis(drivetrain, lateralController, angularController,
                        odomSensors);

// MISC
bool isCompMatch = false;