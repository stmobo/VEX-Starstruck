#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    gyroSens,       sensorGyro)
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

#include "../Enterprise.c"
#include "./Akagi.c"
#include "../RobotCLibs/gyroLib/gyroLib2.c"
/* Competition control stub. */

bool enableLCD = true;

control_t state;
replay_t replay;

int currentTime = 0;
int replayTime = 0;

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
            displayLCDNumber(1, 9, sec, -3);
        }

        if(replayTime > 0) {
            int sec = replayTime / 1000;
            int ms = replayTime % 1000;

            displayLCDString(1, 12, " / ");

            displayLCDNumber(1, 15, sec, 2);
            displayLCDChar(1, 17, '.');
            displayLCDNumber(1, 18, sec, -3);
        }

        sleep(deltaT);
    }
}

task lcdUpdate2() {
	string str;
	while(true) {
		sprintf(str,"Gyro %5.1f   ", theGyro.abs_angle );
		clearLCDLine(0);
    displayLCDString(0, 0, str);
    sleep(50);
	}
}


void pre_auton() {
		bStopTasksBetweenModes = true;

    initReplayData(&replay);
    loadAutonomous(&replay);
		initState(&state);


    if(enableLCD) {
        startTask(lcdUpdate);
    }

    /*
    SensorValue[gyroSens] = 0;
    GyroInit(gyroSens);
    */
}

const float wheelDiameter = 4.0; //in
const float wheelCirc = wheelDiameter*PI; // in/rev
const float encConv = 627.5; // ticks/rev
const float ticksPerInch = (encConv / wheelCirc);

const int leftEncCoeff = -1;
const int rightEncCoeff = 1;
const int gyroCoeff = -1;

const int encDeadband = 20;
const float gyroThreshold = 2.50;

const short driveSpeed = 45;
const short turnSpeed = 65;

void driveStraightLine(float inches) {
	short ticks = (inches * ticksPerInch);

	SensorValue[leftEnc] = 0;
	SensorValue[rightEnc] = 0;

	while( (abs(SensorValue[leftEnc]-ticks) > encDeadband) &&
				 (abs(SensorValue[rightEnc]-ticks) > encDeadband)) {
		if(abs(SensorValue[leftEnc]-ticks) > encDeadband) {
			if(SensorValue[leftEnc] < ticks) {
				motor[LFront] = motor[LBack] = -driveSpeed;
			} else {
				motor[LFront] = motor[LBack] = driveSpeed;
			}
		} else {
			motor[LFront] = motor[LBack] = 0;
		}

		if(abs(SensorValue[rightEnc]-ticks) > encDeadband) {
			if(SensorValue[rightEnc] < ticks) {
				motor[RFront] = motor[RBack] = -driveSpeed;
			} else {
				motor[RFront] = motor[RBack] = driveSpeed;
			}
		} else {
			motor[RFront] = motor[RBack] = 0;
		}

		sleep(50);
	}

	motor[LFront] = motor[LBack] = 0;
	motor[RFront] = motor[RBack] = 0;
}

void turn90Right() {
	motor[LFront] = motor[LBack] = -turnSpeed;
 	motor[RFront] = motor[RBack] = turnSpeed;

 	sleep(500);

 	motor[LFront] = motor[LBack] = 0;
 	motor[RFront] = motor[RBack] = 0;
}

void turn90Left() {
	motor[LFront] = motor[LBack] = turnSpeed;
 	motor[RFront] = motor[RBack] = -turnSpeed;

 	sleep(500);

 	motor[LFront] = motor[LBack] = 0;
 	motor[RFront] = motor[RBack] = 0;
}

void primeCat() {
	while(SensorValue[catapultLim] == 0) {
			catapultDown();
			sleep(25);
	}
	catapultStop();
}

void fireCat() {
	while(SensorValue[catapultLim] > 0) {
		catapultDown();
		sleep(25);
	}
	catapultStop();
}

void slightRaiseCat() {
	catapultUp();
	sleep(150);
	catapultStop();
}

task autonomous() {
		if(doingReplayAuton) {
	    currentTime = 0;    // current elapsed milliseconds
	    replayTime = getReplayTime(&replay);

			while(replay.streamIndex < replay.streamSize) {
				replayToControlState(&state, &replay);
				controlLoopIteration(&state);

		    currentTime += (int)deltaT;

				sleep((int)deltaT);
			}
		} else {
			if(SensorValue[autoSelector] < 727) {
				/* Destick routine. */
				motor[LFront] = motor[LBack] = motor[RFront] = motor[RBack] = -127;
				sleep(900);
				motor[LFront] = motor[LBack] = motor[RFront] = motor[RBack] = 127;
				sleep(250);
				motor[LFront] = motor[LBack] = motor[RFront] = motor[RBack] = 0;

				sleep(250);

				/* Main auton routine. */
				//driveStraightLine(22.625);
				turn90Right();//driveTurn(90.0);
				sleep(250);

				primeCat();
				sleep(250);

				driveStraightLine(24.0);
				sleep(250);

				slightRaiseCat();
				sleep(250);

				turn90Right();//driveTurn(180.0);
				sleep(250);

				primeCat();
				fireCat();
				sleep(250);

				turn90Left();//driveTurn(90.0);
				sleep(250);

				primeCat();
				sleep(250);

				driveStraightLine(48.0);
				sleep(250);

				turn90Right();
				sleep(250);
				//driveTurn(180.0);
				driveStraightLine(6.0);
				sleep(250);

				slightRaiseCat();
				sleep(250);
				driveStraightLine(-6.0);
				sleep(250);
				primeCat();
				fireCat();
				sleep(250);
			}/* else if(SensorValue[autoSelector] < 1920) {
					primeCat();
					sleep(1500);
					fireCat();
			}*/
		}

		stopAllMotorsCustom();
}

task usercontrol()
{
	resetState(&state);

    currentTime = 0;
    replayTime = 0;

	while (true)
	{
		controllerToControlState(&state);
		controlLoopIteration(&state);

        currentTime += (int)deltaT;

		sleep((int)deltaT);
	}
}
