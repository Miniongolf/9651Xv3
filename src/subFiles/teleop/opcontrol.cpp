#include "globals.hpp"
#include "lemlib/util.hpp"
#include "main.h"
#include "subHeads/teleop/opcontrolHelpers.hpp"
#include "subHeads/movements.hpp"
#include <cmath>
#include <functional>

// Semiauton exit condition
std::function<bool()> semiAutonExitFunct;

// TeleOp
void opcontrol() {
    // Drive mode enums
    DModes driveMode = DModes::normal;

    // System states
    SysStates sysStates;

    // Robot position pose
    lemlib::Pose robotPos = chassis.getPose();

    // Start time
    int startTime = pros::millis();

    std::array<float, 2> vels;
    float throttleVel, turnVel;
    float targetTheta;

    while (true) {
        // Timer buzz
        if (pros::millis() == startTime + 90000) gamepad1.controller->rumble("...");
        else if (pros::millis() == startTime + 105000) gamepad1.controller->rumble(".._");

        /** REGION: UPDATE SYSTEM STATES */
        gamepad1.update();
        gamepad2.update();
        robotPos = chassis.getPose();

        gamepad1.controller->print(0, 0, "%d | %d", floor(normalLeftMotors[0].get_temperature() * 10),
                                   floor(normalRightMotors[0].get_temperature() * 10));

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
                if (gamepad1.x.pressed) { sysStates.cataState = CataStates::idle; }
                break;

            case CataStates::load:
                std::cout << (int)cataMotors[0].get_position() % 120 << '\n';
                if (((int)cataMotors[0].get_position() % 120) > 10) {
                    cataMotors.move(127);
                } else sysStates.cataState = CataStates::idle;
                break;

            case CataStates::idle:
                if (gamepad1.x.pressed) { sysStates.cataState = CataStates::fire; }

                cataMotors.move(0);
                break;
        }

        /** REGION: WINGS STATE MACHINES */
        switch (sysStates.wingState) {
            case WingStates::none:
                if (gamepad1.lt || (gamepad1.b && gamepad1.dpadDown)) sysStates.wingState = WingStates::front;
                else if (gamepad1.b) sysStates.wingState = WingStates::right;
                else if (gamepad1.dpadDown) sysStates.wingState = WingStates::left;
                else if (gamepad1.lb) sysStates.wingState = WingStates::back;

                frontWings.set_value(false);
                rearWing.set_value(false);

                break;

            case WingStates::left:
                if (gamepad1.dpadDown) sysStates.wingState = WingStates::left;
                else if (gamepad1.lt || (gamepad1.b && gamepad1.dpadDown)) sysStates.wingState = WingStates::front;
                else if (gamepad1.b) sysStates.wingState = WingStates::right;
                else if (gamepad1.lb) sysStates.wingState = WingStates::back;
                else sysStates.wingState = WingStates::none;

                leftWing.set_value(true);
                rightWing.set_value(false);
                rearWing.set_value(false);
                break;

            case WingStates::right:
                if (gamepad1.b) sysStates.wingState = WingStates::right;
                else if (gamepad1.lt || (gamepad1.b && gamepad1.dpadDown)) sysStates.wingState = WingStates::front;
                else if (gamepad1.dpadDown) sysStates.wingState = WingStates::left;
                else if (gamepad1.lb) sysStates.wingState = WingStates::back;
                else sysStates.wingState = WingStates::none;

                leftWing.set_value(false);
                rightWing.set_value(true);
                rearWing.set_value(false);
                break;

            case WingStates::front:
                if (gamepad1.lt) sysStates.wingState = WingStates::front;
                else if (gamepad1.b) sysStates.wingState = WingStates::right;
                else if (gamepad1.dpadDown) sysStates.wingState = WingStates::left;
                else if (gamepad1.lb) sysStates.wingState = WingStates::back;
                else sysStates.wingState = WingStates::none;

                frontWings.set_value(true);
                rearWing.set_value(false);

                break;

            case WingStates::back:
                if (gamepad1.lb) sysStates.wingState = WingStates::back;
                else if (gamepad1.lt || (gamepad1.b && gamepad1.dpadDown)) sysStates.wingState = WingStates::front;
                else if (gamepad1.b) sysStates.wingState = WingStates::right;
                else if (gamepad1.dpadDown) sysStates.wingState = WingStates::left;
                else sysStates.wingState = WingStates::none;

                frontWings.set_value(false);
                rearWing.set_value(true);

                break;
        }

        /** REGION: HANG STATE MACHINE */
        switch (sysStates.hangState) {
            case HangStates::down:
                if (gamepad1.y.pressed) sysStates.hangState = HangStates::up;
                hangArm.set_value(false);
                break;
            
            case HangStates::up:
                if (gamepad1.y.pressed) sysStates.hangState = HangStates::down;
                hangArm.set_value(true);
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

                if (gamepad2.rb && gamepad2.rt) driveMode = DModes::override;
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

                if (((int)gamepad1.rt > 500 && (int)gamepad1.rb > 500) || semiAutonExitFunct()) { chassis.cancelAllMotions(); }
                if (!chassis.isInMotion()) { driveMode = DModes::normal; }

                break;
        }

        /** REGION: Odom resets */
        if (gamepad1.dpadRight.pressed) { chassis.setPose(-45, -55, 45); }
        if (gamepad1.dpadLeft.pressed) { chassis.setPose(-45, 55, 135); }

        pros::delay(10); // Delay to prevent from overdrawing cpu resources
    }
}