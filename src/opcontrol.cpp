#include "globals.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/exitcondition.hpp"
#include "main.h"
#include "opcontrolHelpers.hpp"
#include "subHeads/constants.hpp"

void updateStateMachines(SysStates sysStates) {
	cataStateMachine(sysStates.cataState);
	intakeStateMachine(sysStates.intakeState);
	wingsStateMachine(sysStates.leftWingState, sysStates.rightWingState);
}

// TeleOp
void opcontrol() {
	// System states
	SysStates sysStates;

	// Drive mode enums
	DMode driveMode = DMode::normal;
	AutoAlignState autoAlignState = AutoAlignState::off;

	// Robot position pose
	lemlib::Pose robotPos = chassis.getPose();

	// Autoalign PID and target
	lemlib::PID autoAlignPID(angularController.kP, angularController.kI, angularController.kD, angularController.windupRange, true);
	lemlib::ExitCondition autoAlignExit(angularController.largeError, angularController.largeErrorTimeout);

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
			case CataStates::active:
				if (gamepad1.x.pressed) sysStates.cataState = CataStates::standby;
				cata.fire();
				break;
			case CataStates::standby:
				if (gamepad1.x.pressed || gamepad1.y) sysStates.cataState = CataStates::active;
				cata.standby();
				break;
			case CataStates::hardstop:
				cata.stop();
				break;
    	}


		/** REGION: WINGS STATE MACHINES */
		// Hold left trigger to extend wings, release to retract
		switch (sysStates.leftWingState) {
			case WingStates::retracted:
				if (gamepad1.lt) sysStates.leftWingState = WingStates::extended;
				
				wings.retract(Wings::L);
				break;
			case WingStates::extended:
				if (!gamepad1.lt) sysStates.leftWingState = WingStates::retracted;

				wings.extend(Wings::L);
				break;
		}
		switch (sysStates.rightWingState) {
			case WingStates::retracted:
				if (gamepad1.lt) sysStates.leftWingState = WingStates::extended;

				wings.retract(Wings::R);
				break;
			case WingStates::extended:
				if (!gamepad1.lt) sysStates.leftWingState = WingStates::retracted;

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
		std::array<float, 2> vels = gamepad1.processSticks(controllerDeadzone, true);
		float throttleVel = vels[0], turnVel = vels[1];

		float targetTheta;

		switch (autoAlignState) {
			case AutoAlignState::off:
				if (fabs(gamepad1.rightY) > 0.75) {
					targetTheta = (gamepad1.rightY > 0.75) ? 90 : -90;
					autoAlignState = AutoAlignState::start;
				}
				break;
			case AutoAlignState::start:
				autoAlignPID.reset();
				autoAlignState = AutoAlignState::on;
				break;
			case AutoAlignState::on:
				chassis.autoAlign(targetTheta, &turnVel, 1000);
				break;
			case AutoAlignState::stop:
				autoAlignState = AutoAlignState::off;
				break;
		}

		/** REGION: DRIVE MODE STATE MACHINE */
		// lt = precision, else normal
		driveMode = (gamepad1.lt) ? DMode::precision : DMode::normal;

		switch (driveMode) {
			// Normal (do nothing)
			case DMode::normal:
				normalizeVels(&throttleVel, &turnVel);
				chassis.arcade(throttleVel*127, turnVel*127);
				break;
			// Precision drive (half speed)
			case DMode::precision:
				normalizeVels(&throttleVel, &turnVel);
				chassis.arcade(throttleVel*127, turnVel*127);
				throttleVel /= 2;
				turnVel /= 2;
				break;
			case DMode::semiauton:
				throttleVel = 0;
				turnVel = 0;
		}

		pros::delay(10); // Delay to prevent from overdrawing cpu resources
	}
}