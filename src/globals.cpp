#include "globals.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"

// GAMEPADS
pros::Controller controller1(pros::E_CONTROLLER_MASTER);
Gamepad gamepad1(&controller1); // Primary controller

// SENSORS
pros::IMU imu(10);

// DRIVE MOTORS
std::array<pros::Motor, 2> ptoMotors = {
    pros::Motor(2, pros::E_MOTOR_GEAR_GREEN), // left side
    pros::Motor(-4, pros::E_MOTOR_GEAR_GREEN) // right side
};

// MOTOR GROUPS
pros::MotorGroup normalLeftMotors({
    pros::Motor(-1, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(-11, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(-20, pros::E_MOTOR_GEAR_BLUE)
});

pros::MotorGroup normalRightMotors({
    pros::Motor(19, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(18, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(10, pros::E_MOTOR_GEAR_BLUE)
});

pros::MotorGroup ptoLeftMotors({
    pros::Motor(-1, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(-11, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(-20, pros::E_MOTOR_GEAR_BLUE),
    ptoMotors[0] // 5.5 watt motor
});

pros::MotorGroup ptoRightMotors({
    pros::Motor(19, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(18, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(10, pros::E_MOTOR_GEAR_BLUE),
    ptoMotors[1] // 5.5 watt motor
});

pros::MotorGroup* leftMotorsPtr = &normalLeftMotors;
pros::MotorGroup* rightMotorsPtr = &normalRightMotors;

// INTAKE MOTOR
pros::Motor intakeMotor(10, pros::E_MOTOR_GEAR_BLUE);

// CATA MOTORS
/** NOTE: these motors are the same as the drivetrain ones. */
pros::MotorGroup cataMotors({
    ptoMotors[0], // left side
    ptoMotors[1] // right side
});

// SUBSYSTEMS
Wings wings('A', 'H');

// CHASSIS PID
lemlib::ControllerSettings lateralController(8, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             0, // derivative gain (kD)
                                             0, // anti windup
                                             1, // small error range, in inches
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in inches
                                             500, // large error range timeout, in milliseconds
                                             127 // maximum acceleration (slew)
);

lemlib::ControllerSettings angularController(8, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             0, // derivative gain (kD)
                                             0, // anti windup
                                             3, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             5, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             127 // maximum acceleration (slew)
);

// LEMLIB CHASSIS
lemlib::Drivetrain drivetrain(leftMotorsPtr, // left drivetrain motors
                              rightMotorsPtr, // right drivetrain motors
                              12.7, // track width
                              lemlib::Omniwheel::NEW_325, // wheel diameter
                              450, // wheel rpm
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