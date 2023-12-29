#include "main.h"
#include "opcontrolHelpers.hpp"
#include <cmath>

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	// Catapult toggle
	bool cataToggle = false;

	// Chassis velocities
	float throttleVel, turnVel, highVel;

	// Drive mode enums
	DMode driveMode;
	AutoAlignState autoAlignState;

	// Robot position pose
	lemlib::Pose robotPos = chassis.getPose();

	// Autoalign PID and target
	lemlib::PID autoAlignPID(0.1, 0, 0, 3, true);
	float targetTheta, deltaTheta;

	// 
	float curveGain = 0;
	float controllerDeadzone = DEFAULT_CONTROLLER_DEADZONE;

	while (true) {
		/** UPDATE SYSTEM STATES */
		gamepad1.getInputs(controllerDeadzone);
		gamepad2.getInputs(controllerDeadzone);
		cata.update();
		robotPos = chassis.getPose();

		/** DRIVETRAIN COMMANDS */
		// Map stick inputs to throttleVel and turnVel
		std::array<float, 2>vels = gamepad1.processSticks();
		throttleVel = vels[0], turnVel = vels[1];

		// AUTOALIGN
		// Get AutoAlign target
		targetTheta = (gamepad1.rightY > 0.75) ? 90 : (gamepad1.rightY < -0.75) ? -90 : 0;

		// Update AutoAlign status
		if (targetTheta == 0) {autoAlignState = AutoAlignState::off;}
		else if (autoAlignState == AutoAlignState::off) {autoAlignState = AutoAlignState::start;}
		else {autoAlignState = AutoAlignState::on;}

		// AutoAlign calculations
		if (autoAlignState == AutoAlignState::start) {
			autoAlignPID.reset();
			autoAlignState = AutoAlignState::on;
		}
		if (autoAlignState == AutoAlignState::on){
			turnVel += autoAlignPID.update(lemlib::angleError(targetTheta, robotPos.theta));
		}

		// DRIVE MODE
		// lt = precision, else normal
		driveMode = (gamepad1.lt) ? DMode::precision : DMode::normal;

		// Process drive mode calculations
		switch (driveMode) {
			// Speed (do nothing)
			case DMode::normal:
				break;
			// Precision drive (half speed)
			case DMode::precision:
				throttleVel /= 2;
				turnVel /= 2;
				break;
			// Should not reach this point but just in case
			default:
				break;
		}

		// Normalize throttle and turning to [-1, 1]
		highVel = std::max(fabs(throttleVel), fabs(turnVel));
		if (highVel > 1) {
			throttleVel /= highVel;
			turnVel /= highVel;
		}

		// LemLib arcade drive
		chassis.arcade(throttleVel*127, turnVel*127);

		/** CATA */
		// X button toggles catapult, Y button to manually run cata
		if (gamepad1.x.pressed) {cataToggle = !cataToggle;}
		
		/** TODO: test logic on standby */
		(gamepad1.y || cataToggle) ? cata.start() : cata.standby();

		/** WINGS */
		// Hold bumpers to extend their respective wings
		(gamepad1.lb) ? wings.extend(Wings::L) : wings.retract(Wings::L);
		(gamepad1.rb) ? wings.extend(Wings::R) : wings.retract(Wings::R);
		
		/** NOTE: #region testing code stuffs, disable for comp */
		if (!isCompMatch) {
			gamepad1.controller->print(0, 0, "%d", cata.angle);

			if (gamepad1.a.pressed) {
				autonomous();
			}
		}
		/* #endregion testing code stuffs, disable for comp */

		pros::delay(20); // Delay to prevent from overdrawing cpu resources
	}
}