#include "globals.hpp"

// SENSORS
pros::IMU imu(12);
pros::Distance cataDistance(1);
pros::Distance leftDistance(1);
pros::Distance rightDistance(1);

// MOTOR GROUPS
pros::MotorGroup normalLeftMotors({
    pros::Motor(-20, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(-16, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(-11, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(6, pros::E_MOTOR_GEAR_GREEN) // 5.5 watt motor
});

pros::MotorGroup normalRightMotors({
    pros::Motor(10, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(3, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(1, pros::E_MOTOR_GEAR_BLUE),
    pros::Motor(-15, pros::E_MOTOR_GEAR_GREEN) // 5.5 watt motor
    
});

// INTAKE MOTOR
pros::MotorGroup intakeMotors({pros::Motor(19, pros::E_MOTOR_GEAR_GREEN), pros::Motor(-6, pros::E_MOTOR_GEAR_GREEN)});

// SUBSYSTEMS
pros::ADIDigitalOut frontLeftWing('F');
pros::ADIDigitalOut frontRightWing('G');
PistonGroup frontWings({'F', 'G'});
PistonGroup rearWings('H');

// CHASSIS PID
lemlib::ControllerSettings lateralController(5, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             5, // derivative gain (kD)
                                             0, // anti windup
                                             1, // small error range, in inches
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in inches
                                             500, // large error range timeout, in milliseconds
                                             20 // maximum acceleration (slew)
);

lemlib::ControllerSettings angularController(3.5, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             5, // derivative gain (kD)
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
                              13, // track width
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