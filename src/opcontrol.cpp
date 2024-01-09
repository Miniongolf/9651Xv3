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

  // Autoalign PID and target
  lemlib::PID autoAlignPID(angularController.kP, angularController.kI,
                           angularController.kD, angularController.windupRange,
                           true);
  lemlib::ExitCondition autoAlignExit(angularController.largeError,
                                      angularController.largeErrorTimeout);

  float controllerDeadzone = DEFAULT_CONTROLLER_DEADZONE;

  while (true) {
    /** REGION: UPDATE SYSTEM STATES */
    gamepad1.getInputs();
    gamepad2.getInputs();
    cata.update();
    robotPos = chassis.getPose();

    /** REGION: CATA STATE MACHINE*/
    // X button toggle, Y button hold
    switch (sysStates.cataState) {
    case CataStates::fire:
      cataMotors.move(127);

      if (gamepad1.x.pressed) {
        sysStates.cataState = CataStates::load;
      }
      break;

    case CataStates::load:
      cataMotors.move(127);

      if (gamepad1.x.pressed || gamepad1.y) {
        sysStates.cataState = CataStates::fire;
      }

      /** TODO: Update cata angle getter depending on what sensors are available
       */
      if (abs((int)floor((cataMotors)[0].get_position() * CATA_RATIO) % 120) <=
          3) {
        sysStates.cataState = CataStates::disconnect;
      }
      break;

    case CataStates::disconnect:
      cataMotors.move(0);
      /** TODO: PTO back to drivetrain */

      if (gamepad1.x.pressed || gamepad1.y) {
        sysStates.cataState = CataStates::fire;
      }
      break;
    }

    /** REGION: INTAKE STATE MACHINE*/
    // hold rt to intake, hold rb to outtake
    switch (sysStates.intakeState) {
    case IntakeStates::intake:
      intakeMotor.move(127);

      sysStates.intakeState =
          (gamepad1.rb.pressed) ? IntakeStates::outtake
          : (gamepad1.rt.released)
              ? (gamepad1.rb) ? IntakeStates::outtake : IntakeStates::stop
              : IntakeStates::intake;

    case IntakeStates::outtake:
      intakeMotor.move(-127);

      sysStates.intakeState =
          (gamepad1.rt.pressed) ? IntakeStates::intake
          : (gamepad1.rb.released)
              ? ((gamepad1.rt) ? IntakeStates::intake : IntakeStates::stop)
              : IntakeStates::outtake;

    case IntakeStates::stop:
      intakeMotor.move(0);

      sysStates.intakeState = (gamepad1.rt.pressed)   ? IntakeStates::intake
                              : (gamepad1.rb.pressed) ? IntakeStates::outtake
                                                      : IntakeStates::stop;

      if (gamepad1.rt.pressed) { // press rt to intake
        sysStates.intakeState = IntakeStates::intake;
      } else if (gamepad1.rb.pressed) { // press rb to outtake
        sysStates.intakeState = IntakeStates::outtake;
      }
    }

    /** REGION: WINGS STATE MACHINES */
    // Hold left trigger to extend wings, release to retract
    switch (sysStates.leftWingState) {
    case WingStates::retracted:
      if (gamepad1.lt)
        sysStates.leftWingState = WingStates::extended;

      wings.retract(Wings::L);
      break;
    case WingStates::extended:
      if (!gamepad1.lt)
        sysStates.leftWingState = WingStates::retracted;

      wings.extend(Wings::L);
      break;
    }
    switch (sysStates.rightWingState) {
    case WingStates::retracted:
      if (gamepad1.lt)
        sysStates.leftWingState = WingStates::extended;

      wings.retract(Wings::R);
      break;
    case WingStates::extended:
      if (!gamepad1.lt)
        sysStates.leftWingState = WingStates::retracted;

      wings.extend(Wings::R);
      break;
    }

    /** NOTE: TESTS | DISABLE FOR COMP */
    if (!isCompMatch) {
      gamepad1.controller->print(0, 0, "%d", cata.angle);

      if (gamepad1.a.pressed) {
        autonomous();
      }
    }

    /** REGION: DRIVETRAIN COMMANDS */
    // Map stick inputs to throttleVel and turnVel
    std::array<float, 2> vels =
        gamepad1.processSticks(controllerDeadzone, true);
    float throttleVel = vels[0], turnVel = vels[1];

    float targetTheta;

    if (fabs(gamepad1.rightY) > 0.75 && driveMode != DModes::semiauton) {
      targetTheta = (gamepad1.rightY > 0.75) ? 90 : -90;
      chassis.turnToHeading(targetTheta, 750);
    }

    // switch (autoAlignState) {
    // case AutoAlignStates::off:
    //   if (fabs(gamepad1.rightY) > 0.75) {
    //     targetTheta = (gamepad1.rightY > 0.75) ? 90 : -90;
    //     autoAlignState = AutoAlignStates::start;
    //   }
    //   break;
    // case AutoAlignStates::start:
    //   autoAlignPID.reset();
    //   autoAlignState = AutoAlignStates::on;
    //   break;
    // case AutoAlignStates::on:
    //   chassis.autoAlign(targetTheta, &turnVel, 1000);
    //   break;
    // case AutoAlignStates::stop:
    //   autoAlignState = AutoAlignStates::off;
    //   break;
    // }

    /** REGION: DRIVE MODE STATE MACHINE */
    switch (driveMode) {
    // Normal (do nothing)
    case DModes::normal:
      normalizeVels(&throttleVel, &turnVel);

      // Precision drive (half speed)
      if (gamepad1.lt) {
        throttleVel /= 2;
        turnVel /= 2;
      }

      if (chassis.isInMotion())
        driveMode = DModes::semiauton;

      chassis.arcade(throttleVel * 127, turnVel * 127);
      break;

    // Semiauton (disable controller inputs)
    case DModes::semiauton:
      if ((int)gamepad1.b > 500) {
        chassis.cancelAllMotions();
      }
      if (!chassis.isInMotion()) {
        driveMode = DModes::normal;
      }
      throttleVel = 0;
      turnVel = 0;
    }

    pros::delay(10); // Delay to prevent from overdrawing cpu resources
  }
}