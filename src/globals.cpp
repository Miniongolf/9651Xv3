#include "globals.hpp"

#define UNUSED_PORT 4

nicklib::Gamepad gamepad1(pros::E_CONTROLLER_MASTER);
nicklib::Gamepad gamepad2(pros::E_CONTROLLER_PARTNER);

// SENSORS
pros::IMU imu(12);
// pros::Distance cataDistance(UNUSED_PORT);
// pros::Distance leftDistance(UNUSED_PORT);
// pros::Distance rightDistance(UNUSED_PORT);

// MOTOR GROUPS
pros::MotorGroup normalLeftMotors({
    pros::Motor(-20, pros::E_MOTOR_GEAR_BLUE), // m1
    pros::Motor(-16, pros::E_MOTOR_GEAR_BLUE), // m2
    pros::Motor(-11, pros::E_MOTOR_GEAR_BLUE), // m3
    // pros::Motor(-15, pros::E_MOTOR_GEAR_GREEN) // 5.5 watt motor
});

pros::MotorGroup normalRightMotors({
    pros::Motor(10, pros::E_MOTOR_GEAR_BLUE), // m1
    pros::Motor(3, pros::E_MOTOR_GEAR_BLUE), // m2
    pros::Motor(1, pros::E_MOTOR_GEAR_BLUE), // m3
    // pros::Motor(4, pros::E_MOTOR_GEAR_GREEN) // 5.5 watt motor

});

// CATA MOTOR
pros::MotorGroup cataMotors({pros::Motor(5, pros::E_MOTOR_GEAR_RED)});

// INTAKE MOTOR
pros::MotorGroup intakeMotors({pros::Motor(19, pros::E_MOTOR_GEAR_GREEN), pros::Motor(-6, pros::E_MOTOR_GEAR_GREEN)});

// SUBSYSTEMS
PistonGroup frontWings('G');
PistonGroup rearWings('H');
PistonGroup balancePiston('E');

// CHASSIS PID
lemlib::ControllerSettings lateralController(20, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             30, // derivative gain (kD)
                                             3, // anti windup
                                             1, // small error range, in inches
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in inches
                                             500, // large error range timeout, in milliseconds
                                             20 // maximum acceleration (slew)
);

lemlib::ControllerSettings angularController(7, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             50, // derivative gain (kD)
                                             3, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// LEMLIB CHASSIS
lemlib::Drivetrain drivetrain(&normalLeftMotors, // left drivetrain motors
                              &normalRightMotors, // right drivetrain motors
                              12.5, // track width
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