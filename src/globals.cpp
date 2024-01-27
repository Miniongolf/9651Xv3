#include "globals.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/adi.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"

// GAMEPADS
nicklib::Gamepad gamepad1(pros::E_CONTROLLER_MASTER); // Primary controller

// SENSORS
pros::IMU imu(12);

// MOTOR GROUPS
pros::MotorGroup normalLeftMotors({
    pros::Motor(-12, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(-19, pros::E_MOTOR_GEAR_BLUE)
});

pros::MotorGroup normalRightMotors({
    pros::Motor(2, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(6, pros::E_MOTOR_GEAR_BLUE)
});

pros::MotorGroup cataMotors({
    pros::Motor(18, pros::E_MOTOR_GEAR_GREEN)
});

// CHASSIS PID
lemlib::ControllerSettings lateralController(8, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             0, // derivative gain (kD)
                                             0, // anti windup
                                             1, // small error range, in inches
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in inches
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

lemlib::ControllerSettings angularController(8, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             0, // derivative gain (kD)
                                             0, // anti windup
                                             3, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             5, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// LEMLIB CHASSIS
lemlib::Drivetrain drivetrain(&normalLeftMotors, // left drivetrain motors
                              &normalRightMotors, // right drivetrain motors
                              10.7, // track width
                              lemlib::Omniwheel::OLD_4, // wheel diameter
                              600, // wheel rpm
                              8 // chase power
);

lemlib::OdomSensors odomSensors(nullptr, // vertical tracking wheel 1
                                nullptr, // vertical tracking wheel 2
                                nullptr, // horizontal tracking wheel 1
                                nullptr, // we don't have a second tracking wheel, so we set it to nullptr
                                &imu // inertial sensor
);

lemlib::Chassis chassis(drivetrain, lateralController, angularController, odomSensors);

// MISC
bool isCompMatch = false;