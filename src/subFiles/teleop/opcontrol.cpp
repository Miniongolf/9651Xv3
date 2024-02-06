#include "globals.hpp"
#include "lemlib/util.hpp"
#include "main.h"
#include "subHeads/teleop/opcontrolHelpers.hpp"
#include "subHeads/movements.hpp"
#include <cmath>

// TeleOp
void opcontrol() {
    // Drive mode enums
    DModes driveMode = DModes::normal;

    // System states
    SysStates sysStates;

    // Robot position pose
    lemlib::Pose robotPos = chassis.getPose();

    std::array<float, 2> vels;
    float throttleVel, turnVel;
    float targetTheta;

    // Task for controller driver assistance
    pros::Task controllerAssistTask {[] {
        int opcontrolStartTime = pros::millis();
        pros::delay(90000);
        gamepad1.controller->rumble("...");
        pros::delay(15000);
        gamepad1.controller->rumble("-..");
    }};

    // Semi auton movements
    SemiAutonMovement autoAlignUp([]() { chassis.turnToHeading(90, 1000); }, isSticksMoved);
    SemiAutonMovement autoAlignDown([]() { chassis.turnToHeading(-90, 1000); }, isSticksMoved);

    SemiAutonMovement rightSideBowl(
        []() {
            if (chassis.getPose().y > -34) chassis.moveToPoint(-30, -60, 2000);
            chassis.moveToPose(40, -60, 90, 5000, {.minSpeed = 70, .earlyExitRange = 10});
            chassis.moveToPose(48, -48, 45, 2000, {.lead = 0.8, .minSpeed = 70});
        },
        isSticksMoved);

    SemiAutonMovement leftSideBowl(
        []() {
            if (chassis.getPose().y < 34) chassis.moveToPoint(-30, 60, 2000, {.minSpeed = 70, .earlyExitRange = 10});
            chassis.moveToPose(40, 60, 90, 5000, {.minSpeed = 70, .earlyExitRange = 10});
            chassis.moveToPose(48, 48, 45, 2000, {.lead = 0.8, .minSpeed = 70});
        },
        isSticksMoved);

    while (true) {
        /** REGION: UPDATE SYSTEM STATES */
        gamepad1.update();
        gamepad2.update();
        robotPos = chassis.getPose();

        gamepad1.controller->print(0, 0, "%d %d | %d %d", normalLeftMotors[0].get_efficiency(),
                                       normalRightMotors[0].get_efficiency(), floor(normalLeftMotors[0].get_temperature() * 10), floor(normalRightMotors[0].get_temperature() * 10));

        if (isCompMatch) {
            
        } else {
            if (gamepad1.a.pressed) { autonomous(); }
        }

        /** REGION: INTAKE STATE MACHINE*/
        // Always controlled by gamepad 1
        switch (sysStates.intakeState) {
            case IntakeStates::intake:
                intakeMotors.move(127);

                if (gamepad1.rb.pressed) { // press rb to outtake
                    sysStates.intakeState = IntakeStates::outtake;
                } else if (!gamepad1.rt) {
                    if (gamepad1.rb) { // outtakes if rb is still pressed
                        sysStates.intakeState = IntakeStates::outtake;
                    } else { // stops if both are released
                        sysStates.intakeState = IntakeStates::stop;
                    }
                }

                break;

            case IntakeStates::outtake:
                intakeMotors.move(-127);

                if (gamepad1.rt.pressed) { // press rt to intake
                    sysStates.intakeState = IntakeStates::intake;
                } else if (!gamepad1.rb) {
                    if (gamepad1.rt) { // intakes if rt is still pressed
                        sysStates.intakeState = IntakeStates::intake;
                    } else { // stops if both are released
                        sysStates.intakeState = IntakeStates::stop;
                    }
                    break;
                }

                break;

            case IntakeStates::stop:
                intakeMotors.move(0);

                if (gamepad1.rt.pressed) { // press rt to intake
                    sysStates.intakeState = IntakeStates::intake;
                } else if (gamepad1.rb.pressed) { // press rb to outtake
                    sysStates.intakeState = IntakeStates::outtake;
                }

                break;
        }

        /** REGION: CATA STATE MACHINES */
        switch (sysStates.cataState) {
            case CataStates::fire:
                cataMotors.move(127);
                if (gamepad1.x.pressed) { sysStates.cataState = CataStates::load; }
                break;

            case CataStates::load:
                // if ((int)lemlib::avg(cataMotors.get_positions()) % 120 > 5) {
                //     cataMotors.move(127);
                // } else sysStates.cataState = CataStates::idle;
                sysStates.cataState = CataStates::idle;
                break;

            case CataStates::idle:
                cataMotors.move(0);
                if (gamepad1.x.pressed) { sysStates.cataState = CataStates::fire; }
                break;
        }

        /** REGION: WINGS STATE MACHINES */
        switch (sysStates.wingState) {
            case WingStates::none:
                frontWings.set_value(false);
                rearWings.set_value(false);
                if (driveMode == DModes::normal) {
                    if (gamepad1.lt) sysStates.wingState = WingStates::front;
                    else if (gamepad1.lb) sysStates.wingState = WingStates::back;
                } else if (driveMode == DModes::override) {
                    if (gamepad2.lt) sysStates.wingState = WingStates::front;
                    else if (gamepad2.lb) sysStates.wingState = WingStates::back;
                }

                break;

            case WingStates::front:
                frontWings.set_value(true);
                rearWings.set_value(false);

                if (driveMode == DModes::normal) {
                    if (!gamepad1.lt) sysStates.wingState = WingStates::none;
                    else if (gamepad1.lb) sysStates.wingState = WingStates::back;
                } else if (driveMode == DModes::override) {
                    if (!gamepad2.lt) sysStates.wingState = WingStates::none;
                    else if (gamepad2.lb) sysStates.wingState = WingStates::back;
                }

                break;

            case WingStates::back:
                frontWings.set_value(false);
                rearWings.set_value(true);
                if (gamepad1.lb.released) sysStates.wingState = WingStates::none;
                if (gamepad1.lt.pressed) sysStates.wingState = WingStates::front;

                break;
        }

        /** REGION: DRIVE MODE STATE MACHINE */
        switch (driveMode) {
            // Normal (drive forwards)
            case DModes::normal:
                // Map stick inputs to throttleVel and turnVel
                std::tie(throttleVel, turnVel) = processSticks(&gamepad1);
                vels = nicklib::normalizeVels(throttleVel, turnVel);
                throttleVel = vels[0], turnVel = vels[1];
                chassis.arcade(throttleVel * 127, turnVel * 127);

                /** REGION: Semi autons */
                // Autoalign
                if ((std::fabs(gamepad1.rightY) > 0.8) && (driveMode != DModes::semiauton)) {
                    if (gamepad1.rightY > 0) autoAlignUp.start();
                    else autoAlignDown.start();
                }

                // Auto bowl
                if (gamepad1.dpadUp.pressed) {
                    if (chassis.getPose().y < -24) rightSideBowl.start();
                    else if (chassis.getPose().x > 24) leftSideBowl.start();
                }


                if (gamepad2.lt && gamepad2.rt) driveMode = DModes::override;
                else if (chassis.isInMotion()) driveMode = DModes::semiauton;

                break;
            // Override (gamepad 2 has control over chassis and wings, but not intake and cata)
            case DModes::override:
                gamepad1.controller->rumble(".  ");
                std::tie(throttleVel, turnVel) = processSticks(&gamepad2);
                vels = nicklib::normalizeVels(throttleVel, turnVel);
                throttleVel = vels[0], turnVel = vels[1];
                chassis.arcade(throttleVel * 127, turnVel * 127);

                if (!(gamepad2.rb && gamepad2.rt)) driveMode = DModes::normal;

                break;

            // Semiauton (disable controller driving), hold `b` to cancel or meet the movement's exit conditions
            case DModes::semiauton:
                throttleVel = 0;
                turnVel = 0;

                if ((int)gamepad1.b > 500 || (gamepad2.rb && gamepad2.rt && gamepad2.b)) { chassis.cancelAllMotions(); }
                if (!chassis.isInMotion()) { driveMode = DModes::normal; }

                break;
        }

        /** REGION: Odom resets */
        if (gamepad1.dpadRight.pressed) { chassis.setPose(-50, -50, 45); }
        if (gamepad1.dpadLeft.pressed) { chassis.setPose(-50, 50, 135); }

        pros::delay(10); // Delay to prevent from overdrawing cpu resources
    }
}