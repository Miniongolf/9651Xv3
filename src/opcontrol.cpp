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
                autonomous();
            }
            float kP = chassis.angularSettings.kP, kD = chassis.angularSettings.kD;
            chassis.angularSettings.kP = (gamepad1.dpadUp.pressed) ? kP + 1 : (gamepad1.dpadDown.pressed) ? kP - 1 : kP;
            chassis.angularSettings.kD = (gamepad1.dpadRight.pressed)  ? kD + 1
                                         : (gamepad1.dpadLeft.pressed) ? kD - 1
                                                                       : kD;
            // gamepad1.controller->print(0, 0, "%d | %d", (int)chassis.angularSettings.kP,
            // (int)chassis.angularSettings.kD);
        }

        /** REGION: CATA STATE MACHINE*/
        switch (sysStates.cataState) {
            case CataStates::fire:
                cataMotors.move(110);

                if (gamepad1.x.pressed) { sysStates.cataState = CataStates::idle; }

                break;

            case CataStates::idle:
                cataMotors.move(0);

                if (gamepad1.x.pressed) { sysStates.cataState = CataStates::fire; }

                break;
        }

        /** REGION: DRIVETRAIN COMMANDS */
        // Map stick inputs to throttleVel and turnVel
        std::tie(throttleVel, turnVel) = processSticks();
        std::array<float, 2> vels = nicklib::normalizeVels(throttleVel, turnVel);
        throttleVel = vels[0], turnVel = vels[1];
        gamepad1.controller->print(0, 0, "%d | %d", (int)(throttleVel * 100), (int)(turnVel * 100));

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

                chassis.arcade(throttleVel * 100, turnVel * 100);

                break;

            // Reverse (drive backwards)
            case DModes::reverse:
                throttleVel *= -1; // Reverse driving

                /** TODO: Retract front wings, use back wings */

                if (chassis.isInMotion()) // Switch to semiauton when in a LemLib motion
                    driveMode = DModes::semiauton;

                if (!gamepad1.lb) driveMode = DModes::normal;

                chassis.arcade(throttleVel * 100, turnVel * 100);

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