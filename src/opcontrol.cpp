#include "globals.hpp"
#include "main.h"
#include "opcontrolHelpers.hpp"

// TeleOp
void opcontrol() {
    // Drive mode enums
    DModes driveMode = DModes::normal;

    // System states
    SysStates sysStates;


    // Robot position pose
    lemlib::Pose robotPos = chassis.getPose();

    float throttleVel, turnVel;
    float targetTheta;

    // Task for controller driver assistance
    pros::Task controllerAssistTask{[=] {
        int opcontrolStartTime = pros::millis();
        while (true) {
            // Controller driver assistance
            if (pros::millis() - opcontrolStartTime > 90000) {
                gamepad1.controller->rumble("...");
                break;
            }
            pros::delay(10);
        }
    }};

    while (true) {
        /** REGION: UPDATE SYSTEM STATES */
        gamepad1.update();
        robotPos = chassis.getPose();

        /** NOTE: TESTS | DISABLE FOR COMP */
        if (!isCompMatch) {
            if (gamepad1.a.pressed) { 
                autonomous();
            }
        }

        /** REGION: INTAKE STATE MACHINE*/
        // hold rt to intake, hold rb to outtake
        switch (sysStates.intakeState) {
            case IntakeStates::intake:
                intakeMotor.move(127);

                if (gamepad1.rb.pressed) {
                    sysStates.intakeState = IntakeStates::outtake;
                } else if (gamepad1.rt.released) {
                    if (gamepad1.rb) {
                        sysStates.intakeState = IntakeStates::outtake;
                    } else {
                        sysStates.intakeState = IntakeStates::stop;
                    }
                }

                break;

            case IntakeStates::outtake:
                intakeMotor.move(-127);

                if (gamepad1.rt.pressed) {
                    sysStates.intakeState = IntakeStates::intake;
                } else if (gamepad1.rb.released) {
                    if (gamepad1.rt) {
                        sysStates.intakeState = IntakeStates::intake;
                    } else {
                        sysStates.intakeState = IntakeStates::stop;
                    }
                    break;
                }

                break;

            case IntakeStates::stop:
                intakeMotor.move(0);

                if (gamepad1.rt.pressed) { // press rt to intake
                    sysStates.intakeState = IntakeStates::intake;
                } else if (gamepad1.rb.pressed) { // press rb to outtake
                    sysStates.intakeState = IntakeStates::outtake;
                }

                break;
        }

        /** REGION: WINGS STATE MACHINES */
        switch (sysStates.wingState) {
            case WingStates::none:
                rearWings.set_value(false);
                frontWings.retract();
                if (gamepad1.lt.pressed) sysStates.wingState = WingStates::front;
                else if (gamepad1.lb.pressed) sysStates.wingState = WingStates::back;

                break;

            case WingStates::front:
                frontWings.extend();
                rearWings.set_value(false);
                if (gamepad1.lt.released) sysStates.wingState = WingStates::none;
                if (gamepad1.lb.pressed) sysStates.wingState = WingStates::back;

                break;

            case WingStates::back:
                rearWings.set_value(true);
                frontWings.retract();
                if (gamepad1.lb.released) sysStates.wingState = WingStates::none;
                if (gamepad1.lt.pressed) sysStates.wingState = WingStates::front;

                break;
        }

        /** REGION: DRIVETRAIN COMMANDS */
        // Map stick inputs to throttleVel and turnVel
        std::tie(throttleVel, turnVel) = processSticks();
        std::array<float, 2> vels = nicklib::normalizeVels(throttleVel, turnVel);
        throttleVel = vels[0], turnVel = vels[1];

        // Autoalign
        if ((std::fabs(gamepad1.rightY) > 0.8) && (driveMode != DModes::semiauton)) {
            targetTheta = (gamepad1.rightY > 0.75) ? 90 : -90;
            targetTheta = lemlib::degToRad(targetTheta);
            chassis.turnTo(100 * cos(targetTheta), 100 * sin(targetTheta), 750);
        }

        /** REGION: DRIVE MODE STATE MACHINE */
        switch (driveMode) {
            // Normal (drive forwards)
            case DModes::normal:
                // Switch to semiauton when in a LemLib motion
                if (chassis.isInMotion()) driveMode = DModes::semiauton;

                chassis.arcade(throttleVel * 127, turnVel * 127);

                break;

            // Semiauton (disable controller inputs), hold `b` to cancel
            case DModes::semiauton:
                if ((int)gamepad1.b > 500) { chassis.cancelAllMotions(); }
                if (!chassis.isInMotion()) { driveMode = DModes::normal; }

                throttleVel = 0;
                turnVel = 0;
                
                break;
        }

        pros::delay(10); // Delay to prevent from overdrawing cpu resources
    }
}