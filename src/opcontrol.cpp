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

    while (true) {
        /** REGION: UPDATE SYSTEM STATES */
        gamepad1.update();
        robotPos = chassis.getPose();

        /** NOTE: TESTS | DISABLE FOR COMP */
        if (!isCompMatch) {
            if (gamepad1.a.pressed) { 
                std::cout << "A PRESSED \n";
                chassis.setPose(0,0,0);
                chassis.turnTo(100,0,1000);
                chassis.waitUntilDone();
                normalLeftMotors.move_velocity(0);
                normalRightMotors.move_velocity(0);
                std::cout << "turned | " << chassis.getPose().theta
                          << '\n';
                // autonomous();
            }
            float kP = chassis.angularSettings.kP, kD = chassis.angularSettings.kD;
            chassis.angularSettings.kP = (gamepad1.dpadUp.pressed)     ? kP + 1
                                         : (gamepad1.dpadDown.pressed) ? kP - 1
                                                                       : kP;
            chassis.angularSettings.kD = (gamepad1.dpadRight.pressed)  ? kD + 1
                                         : (gamepad1.dpadLeft.pressed) ? kD - 1
                                                                       : kD;
            // gamepad1.controller->print(0, 0, "%d | %d", (int)chassis.angularSettings.kP, (int)chassis.angularSettings.kD);
        }

        /** REGION: CATA STATE MACHINE*/
        // switch (sysStates.cataState) {
        //     case CataStates::connect:
        //         ptoPiston.set_value(true);
        //         setDrivetrainMotors(&ptoLeftMotors, &ptoRightMotors);
        //         if (gamepad1.x.released) sysStates.cataState = CataStates::fire;

        //         break;

        //     case CataStates::fire:
        //         cataMotors.move(127);

        //         if (gamepad1.x.pressed) { sysStates.cataState = CataStates::idle; }

        //         break;

        //     case CataStates::idle:
        //         cataMotors.move(0);

        //         if (gamepad1.x.released) {
        //             sysStates.cataState = CataStates::disconnect;
        //         } else if (gamepad1.x.pressed) {
        //             sysStates.cataState = CataStates::connect;
        //         }

        //         break;

        //     case CataStates::disconnect:
        //         /** TODO: PTO back to drivetrain */
        //         ptoPiston.set_value(false);
        //         setDrivetrainMotors(&normalLeftMotors, &normalRightMotors);

        //         if (gamepad1.x.pressed) { sysStates.cataState = CataStates::connect; }

        //         break;
        // }

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

        /** REGION: DRIVETRAIN COMMANDS */
        // Map stick inputs to throttleVel and turnVel
        std::tie(throttleVel, turnVel) = processSticks();
        std::array<float, 2> vels = nicklib::normalizeVels(throttleVel, turnVel);
        throttleVel = vels[0], turnVel = vels[1];
        gamepad1.controller->print(0, 0, "%d | %d", (int)(throttleVel*100), (int)(turnVel*100));

        // Autoalign
        if (fabs(gamepad1.rightY) > 0.8 && driveMode != DModes::semiauton) {
            targetTheta = (gamepad1.rightY > 0.75) ? 90 : -90;
            targetTheta = lemlib::degToRad(targetTheta);
            chassis.turnTo(100 * cos(targetTheta), 100 * sin(targetTheta), 750);
        }

        /** REGION: DRIVE MODE STATE MACHINE */
        switch (driveMode) {
            // Normal (drive forwards)
            case DModes::normal:
                /** TODO: Retract back wings, use front wings */

                if (chassis.isInMotion()) // Switch to semiauton when in a LemLib motion
                    driveMode = DModes::semiauton;

                if (gamepad1.lb) // Reverse if lb is pressed
                    driveMode = DModes::reverse;

                chassis.arcade(throttleVel * 127, turnVel * 127);

                break;

            // Reverse (drive backwards)
            case DModes::reverse:
                throttleVel *= -1; // Reverse driving

                /** TODO: Retract front wings, use back wings */

                if (chassis.isInMotion()) // Switch to semiauton when in a LemLib motion
                    driveMode = DModes::semiauton;

                if (!gamepad1.lb) driveMode = DModes::normal;

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