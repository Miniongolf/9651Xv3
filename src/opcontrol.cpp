#include "main.h"
#include "opcontrolHelpers.hpp"

// TeleOp
void opcontrol() {
    // System states
    SysStates sysStates;

    // Drive mode enums
    DModes driveMode = DModes::normal;
    AutoAlignStates autoAlignState = AutoAlignStates::off;

    // Robot position pose
    lemlib::Pose robotPos = chassis.getPose();

    float controllerDeadzone = DEFAULT_CONTROLLER_DEADZONE;

    float throttleVel, turnVel;
    float targetTheta;

    Gamepad gamepad2;

    while (true) {
        /** REGION: UPDATE SYSTEM STATES */
        gamepad1.getInputs();
        robotPos = chassis.getPose();

        /** REGION: CATA STATE MACHINE*/

        switch (sysStates.cataState) {
            case CataStates::connect:
                /** TODO: release PTO from drivetrain */

                if (gamepad1.x.released) sysStates.cataState = CataStates::fire;
                break;
            case CataStates::fire:
                cataMotors.move(127);

                if (gamepad1.x.pressed) { sysStates.cataState = CataStates::idle; }

                break;

            case CataStates::idle:
                cataMotors.move(0);

                if (gamepad1.x.released) {
                    sysStates.cataState = CataStates::disconnect;
                } else if (gamepad1.x.pressed) {
                    sysStates.cataState = CataStates::fire;
                }

                break;

            case CataStates::disconnect:
                /** TODO: PTO back to drivetrain */

                if (gamepad1.x.pressed) { sysStates.cataState = CataStates::fire; }

                break;
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
        // Hold left trigger to extend wings, release to retract
        switch (sysStates.leftWingState) {
            case WingStates::retracted:
                if (gamepad1.lt) { sysStates.leftWingState = WingStates::extended; }

                wings.retract(Wings::L);
                break;
            case WingStates::extended:
                if (!gamepad1.lt) { sysStates.leftWingState = WingStates::retracted; }

                wings.extend(Wings::L);
                break;
        }
        switch (sysStates.rightWingState) {
            case WingStates::retracted:
                if (gamepad1.lt) { sysStates.leftWingState = WingStates::extended; }

                wings.retract(Wings::R);
                break;
            case WingStates::extended:
                if (!gamepad1.lt) { sysStates.leftWingState = WingStates::retracted; }

                wings.extend(Wings::R);
                break;
        }

        /** NOTE: TESTS | DISABLE FOR COMP */
        if (!isCompMatch) {
            std::cout << gamepad1.leftY << ' ' << gamepad1.rightX << '\n';
            if (gamepad1.a.pressed) { autonomous(); }
        }

        /** REGION: DRIVETRAIN COMMANDS */
        // Map stick inputs to throttleVel and turnVel
        std::array<float, 2> vels = gamepad1.processSticks(controllerDeadzone, true);

        throttleVel = vels[0], turnVel = vels[1];

        if (fabs(gamepad1.rightY) > 0.75 && driveMode != DModes::semiauton) {
            targetTheta = (gamepad1.rightY > 0.75) ? 90 : -90;
            targetTheta = lemlib::degToRad(targetTheta);
            chassis.turnTo(100 * cos(targetTheta), 100 * sin(targetTheta), 750);
        }

        /** REGION: DRIVE MODE STATE MACHINE */
        normalizeVels(throttleVel, turnVel);

        // Precision drive (half speed)
        if (gamepad1.lt) {
            throttleVel /= 2;
            turnVel /= 2;
        }

        switch (driveMode) {
            // Normal (do nothing)
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

                if (gamepad1.lb) driveMode = DModes::normal;

                chassis.arcade(throttleVel * 127, turnVel * 127);
                break;

            // Semiauton (disable controller inputs)
            // Hold `b` to cancel
            case DModes::semiauton:
                if ((int)gamepad1.b > 500) { chassis.cancelAllMotions(); }
                if (!chassis.isInMotion()) { driveMode = DModes::normal; }

                throttleVel = 0;
                turnVel = 0;
                break;
        }

        pros::delay(20); // Delay to prevent from overdrawing cpu resources
    }
}