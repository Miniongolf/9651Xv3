#include "globals.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/exitcondition.hpp"
#include "main.h"
#include "opcontrolHelpers.hpp"
#include "subHeads/constants.hpp"

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

      if (gamepad1.x.pressed)
        sysStates.cataState = CataStates::load;
      break;
    case CataStates::load:
      if (gamepad1.x.pressed || gamepad1.y)
        sysStates.cataState = CataStates::fire;
      cata.standby();
      break;
    case CataStates::hardstop:
      if (gamepad1.x.pressed || gamepad1.y)
        sysStates.cataState = CataStates::fire;
      cata.stop();
      break;
    }

    /** REGION: INTAKE STATE MACHINE*/
    // hold rt to intake, hold rb to outtake
    switch (sysStates.intakeState) {
    case IntakeStates::intake:
      intakeMotor.move(127);

      if (gamepad1.rb.pressed) // press rb to outtake
        sysStates.intakeState = IntakeStates::outtake;
      else if (gamepad1.rt.released) // release rt to stop
        sysStates.intakeState = IntakeStates::stop;
    case IntakeStates::outtake:
      intakeMotor.move(-127);

      if (gamepad1.rt.pressed) // press rt to intake
        sysStates.intakeState = IntakeStates::intake;
      else if (gamepad1.rb.released) // release rb to stop
        sysStates.intakeState = IntakeStates::stop;
    case IntakeStates::stop:
      intakeMotor.move(0);

      if (gamepad1.rt.pressed) // press rt to intake
        sysStates.intakeState = IntakeStates::intake;
      else if (gamepad1.rb.pressed) // press rb to outtake
        sysStates.intakeState = IntakeStates::outtake;
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

    switch (autoAlignState) {
    case AutoAlignStates::off:
      if (fabs(gamepad1.rightY) > 0.75) {
        targetTheta = (gamepad1.rightY > 0.75) ? 90 : -90;
        autoAlignState = AutoAlignStates::start;
      }
      break;
    case AutoAlignStates::start:
      autoAlignPID.reset();
      autoAlignState = AutoAlignStates::on;
      break;
    case AutoAlignStates::on:
      chassis.autoAlign(targetTheta, &turnVel, 1000);
      break;
    case AutoAlignStates::stop:
      autoAlignState = AutoAlignStates::off;
      break;
    }

    /** REGION: DRIVE MODE STATE MACHINE */
    // lt = precision, else normal
    driveMode = (gamepad1.lt) ? DModes::precision : DModes::normal;

    switch (driveMode) {
    // Normal (do nothing)
    case DModes::normal:
      normalizeVels(&throttleVel, &turnVel);
      chassis.arcade(throttleVel * 127, turnVel * 127);
      break;
    // Precision drive (half speed)
    case DModes::precision:
      normalizeVels(&throttleVel, &turnVel);
      chassis.arcade(throttleVel * 127, turnVel * 127);
      throttleVel /= 2;
      turnVel /= 2;
      break;
    case DModes::semiauton:
      throttleVel = 0;
      turnVel = 0;
    }

    pros::delay(10); // Delay to prevent from overdrawing cpu resources
  }
}