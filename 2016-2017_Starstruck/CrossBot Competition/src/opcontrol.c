/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
void operatorControl() {
	int drivePos, driveSpeed;

	while (1) {
		lcdLoop();
		driveSpeed = joystickGetAnalog(1,3);
		// printf("setting drive pos\n\r");
		driveForward(driveSpeed);
		drivePos = ( (-joystickGetAnalog(1,1) + 128) * 3000 / 256 )  + 550; // Convert joystick input to wheel position

		if ( joystickGetDigital(1,6,JOY_UP) ) {
			pointTurn(joystickGetDigital(1,6,JOY_UP) ? joystickGetAnalog(1,1) : 0);
			//servoSet(drive,drivePos);
		} else {
			driveSetPos(drivePos);
		}

		if(joystickGetDigital(2,6,JOY_UP)){
			motorSet(PL, -joystickGetAnalog(2,4));
			motorSet(PR, joystickGetAnalog(2,1));
		} else {
			motorStop(PL);
			motorStop(PR);
			pince(joystickGetAnalog(2,1));
			lift(joystickGetAnalog(2,3));
		}

		if((joystickGetDigital(1,7,JOY_UP))&&(joystickGetDigital(1,7,JOY_DOWN))&&(joystickGetDigital(1,7,JOY_LEFT))&&(joystickGetDigital(1,7,JOY_RIGHT))){
			autonomous();
		}

		printf("4+3: %d\t 1: %d\n",(joystickGetAnalog(1,4)+joystickGetAnalog(1,3)),joystickGetAnalog(1,1));

		delay(200);
	}
}
