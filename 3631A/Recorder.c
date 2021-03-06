#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    gyro,           sensorGyro)
#pragma config(Sensor, in2,    autoSelector,   sensorPotentiometer)
#pragma config(Sensor, in3,    posSelector,    sensorPotentiometer)
#pragma config(Sensor, dgtl1,  catapultLim,    sensorTouch)
#pragma config(Sensor, dgtl2,  upperLim,       sensorTouch)
#pragma config(Sensor, I2C_1,  leftEnc,        sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  rightEnc,       sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           RBack,         tmotorVex393HighSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           RFront,        tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port3,           rightLowerIntake, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           rightUpperIntake, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           hangMotor,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           leftUpperIntake, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           leftLowerIntake, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           LFront,        tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port10,          LBack,         tmotorVex393HighSpeed_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

#define DEBUG

#include "../Enterprise.c"
#include "./Akagi.c"
/* Recorder control stub. */

/* Max recording time in milliseconds.
 *
 *  For a regular match autonomous recording, this should be 15000 milliseconds (= 15 seconds).
 *  For an Auto Skills recording, this should be 60000 milliseconds (= 60 seconds = 1 minute).
 */
unsigned int timelimit = 61000;

replay_t loadedReplay;

unsigned int currentTime = 0;
unsigned int replayTime = 0;

bool recording = false;
bool auton_mode = false;

void saveAutonomous(replay_t* replay) {
	int pos = sensorValue[autoSelector];

	if(pos < 727) {		// Illuminati Skills
		writeDebugStreamLine("Saving: ilmskills");
		saveReplayToFile("ilmskills", replay);
	} else if(pos < 1920) {	// Illuminati routine
		writeDebugStreamLine("Saving: ilmroutine");
		saveReplayToFile("ilmroutine", replay);
	} else if(pos < 2678) {	// Off
		return;
	} else if(pos < 3200) {	// A1
		writeDebugStreamLine("Saving: slot1");
		saveReplayToFile("slot1", replay);
	} else if(pos < 3768) { // A2
		writeDebugStreamLine("Saving: slot2");
		saveReplayToFile("slot2", replay);
	} else if(pos > 4080) {	// A3
		writeDebugStreamLine("Saving: slot3");
		saveReplayToFile("slot3", replay);
	}

	clearLCDLine(0);
	displayLCDCenteredString(0, "Save done.");
	writeDebugStreamLine("Saving done.");
}

task lcdUpdate() {
    while(true) {
        clearLCDLine(1);

        if(currentTime > 0) {
            int sec = currentTime / 1000;
            int ms = currentTime % 1000;

            displayLCDString(1, 0, "Time: "); // length 5 (next char at 6)

            /* Displays: ss.mmm (ss = seconds, mmm = milliseconds) */
            displayLCDNumber(1, 6, sec, 2);
            displayLCDChar(1, 8, '.');
            displayLCDNumber(1, 9, ms, -3);
        }

        if(recording && timelimit > 0) {
            int sec = timelimit / 1000;
            int ms = timelimit % 1000;

            displayLCDString(1, 12, " / ");

            displayLCDNumber(1, 15, sec, 2);
            displayLCDChar(1, 17, '.');
            displayLCDNumber(1, 18, ms, -3);
        }

        if(auton_mode && replayTime > 0) {
            int sec = replayTime / 1000;
            int ms = replayTime % 1000;

            displayLCDString(1, 12, " / ");

            displayLCDNumber(1, 15, sec, 2);
            displayLCDChar(1, 17, '.');
            displayLCDNumber(1, 18, ms, -3);
        }

        sleep(deltaT);
    }
}

void pre_auton() {}

task autonomous() {
    control_t state;

	initReplayData(&loadedReplay);
    initState(&state);
	loadAutonomous(&loadedReplay);

    auton_mode = true;
    recording = false;
    replayTime = getReplayTime(&loadedReplay);
    currentTime = 0;

    startTask(lcdUpdate);

	while(loadedReplay.streamIndex < loadedReplay.streamSize) {
		replayToControlState(&state, &loadedReplay);
		controlLoopIteration(&state);

        currentTime += (int)deltaT;

		sleep((int)deltaT);
	}

    stopTask(lcdUpdate);
	stopAllMotorsCustom();
}

task usercontrol()
{
    control_t state;

	initState(&state);
	initReplayData(&loadedReplay);

	clearLCDLine(0);
	clearLCDLine(1);
	displayLCDCenteredString(0, "Ready to record.");
	displayLCDCenteredString(1, "Do stuff.");

	while (true)
	{
        controllerToControlState(&state);
		if(
            abs(state.yAxis) > deadband ||
            abs(state.zAxis) > deadband ||
            state.catUp || state.catDown || state.catReset || state.hangUp ||
            state.hangDown || state.turnLeft || state.turnRight ||
            state.slowDown
       ) {
			break;
		}

		sleep(5);
	}

	clearLCDLine(0);
	displayLCDCenteredString(0, "Recording...");

    recording = true;
    auton_mode = false;
    replayTime = 0;
    currentTime = 0;

    resetState(&state);
    startTask(lcdUpdate);

	while (true)
	{
		controllerToControlState(&state);
		controlLoopIteration(&state);

		if(timelimit > 0) {
			controlStateToReplay(&state, &loadedReplay);
		}

		currentTime += (int)deltaT;

		if(vexRT[Btn7R]) {
			break;
		}


		if((currentTime > timelimit) && (timelimit > 0)) {
			break;
		}

		sleep((int)deltaT);
	}

	loadedReplay.streamSize = loadedReplay.streamIndex+1;

	stopAllMotorsCustom();
    stopTask(lcdUpdate);

	bool doSave = false;
	while(true) {
		displayLCDCenteredString(0, "Save replay?");
		if(!doSave) {
			displayLCDCenteredString(1, "[No] Yes ");
		} else {
			displayLCDCenteredString(1, " No [Yes]");
		}

		if(nLCDButtons & 0x01) {
			doSave = false;
		} else if(nLCDButtons & 0x02) {
			break;
		} else if(nLCDButtons & 0x04) {
			doSave = true;
		}
	}

	if(doSave) {
		saveAutonomous(&loadedReplay);
	}

    RCFS_ReadVTOC();
}
