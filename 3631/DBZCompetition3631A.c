#pragma config(Sensor, in1,    pot,            sensorPotentiometer)
#pragma config(Sensor, dgtl1,  ,               sensorQuadEncoder)
#pragma config(Motor,  port1,           rightDrivea,   tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           rightDriveb,   tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           clawR,         tmotorVex393_MC29, PIDControl, encoderPort, dgtl1)
#pragma config(Motor,  port4,           ArmRb,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           ArmRt,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           ArmLt,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           ArmLa,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           clawL,         tmotorVex393_MC29, PIDControl, encoderPort, dgtl1)
#pragma config(Motor,  port9,           leftDrivea,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          leftDriveb,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
	// Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{
/*	motor [leftDrivea] = 127;
	motor [leftDriveb] = 127;
	motor [rightDrivea] = 127;
	motor [rightDriveb] = 127;
	sleep(1000);*/

	/*motor [leftDrivea] = -127;
	motor [leftDriveb] = -127;//backs up to align with cube
	motor [rightDrivea] = -127;
	motor [rightDriveb] = -127;
	sleep(400);

	motor [leftDrivea] = 90;//turns to alin wcube
	motor [leftDriveb] = 90;
	motor [rightDrivea] = -90;
	motor [rightDriveb] = -90;
	sleep(480);

	motor [leftDrivea] = -100;
	motor [leftDriveb] = -100;
	motor [rightDrivea] = -100;
	motor [rightDriveb] = -100;
	sleep(350);

	motor [leftDrivea] = 0;
	motor [leftDriveb] = 0;
	motor [rightDrivea] = 0;
	motor [rightDriveb] = 0;
	sleep(350);

	motor [leftDrivea] = 90;
	motor [leftDriveb] = 90;  // drives forward to go under cube
	motor [rightDrivea] = 90;
	motor [rightDriveb] = 90;
	sleep(1800);

	motor [leftDrivea] = 0;
	motor [leftDriveb] = 0;
	motor [rightDrivea] = 0;
	motor [rightDriveb] = 0;
	sleep(100);

	motor[ArmLa] = 60;
	motor[ArmLt] = -60;
	motor[ArmRt] = 60;
	motor[ArmRb] = -60;
	sleep(960);

	motor [leftDrivea] = 127;
	motor [leftDriveb] = 127;
	motor [rightDrivea] = -127;
	motor [rightDriveb] = -127;
	sleep(350);

	motor [leftDrivea] = -127;
	motor [leftDriveb] = -127;
	motor [rightDrivea] = -127;
	motor [rightDriveb] = -127;
	sleep(100);

	motor[ArmLa] = 100;
	motor[ArmLt] = -100;
	motor[ArmRt] = 100;
	motor[ArmRb] = -100;
	sleep(750);

	motor[ArmLa] = 0;
	motor[ArmLt] = 0;
	motor[ArmRt] = 0;
	motor[ArmRb] = 0;
	sleep(70);

	motor[ArmLa] = -100;
	motor[ArmLt] = 100;
	motor[ArmRt] = -100;
	motor[ArmRb] = 100;
	sleep(1500);

	motor[ArmLa] = 0;
	motor[ArmLt] = 0;
	motor[ArmRt] = 0;
	motor[ArmRb] = 0;
	sleep(100);*/
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                             //
//                                 User Control Task                                           //
//                                                                                             //
// This task is used to control your robot during the user control phase of a VEX Competition. //
// You must modify the code to add your own robot specific commands here.                      //
//                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////

task usercontrol()
{

	//UNTESTED
	/*TARGET = <a number>
	Kp = <another number>
	maxOut = 0

	while(true){
		error = TARGET - pot;
		output = Kp * error;

		if(abs(error) < maxOut){
			motor[clawL] = output;
			motor[clawR] = output;
		}
		else{
			motor[arm] = output/abs(output) * maxOut */
		//UNTESTED

	// User control code here, inside the loop
	while(true){

		int speedLimit = 127;
		if(abs(vexRT[Ch3]) <= speedLimit) {
			motor[leftDrivea] = vexRT[Ch3];
			motor[leftDriveb] = vexRT[Ch3];
		}
		else{
			motor[leftDrivea] = sgn(vexRT[Ch3]) * speedLimit;
			motor[leftDriveb] = sgn(vexRT[Ch3]) * speedLimit;
		}

		if(abs(vexRT[Ch2]) <= speedLimit) {
			motor[rightDrivea] = vexRT[Ch2];
			motor[rightDriveb] = vexRT[Ch2];
		}
		else{
			motor[rightDrivea] = sgn(vexRT[Ch2]) * speedLimit;
			motor[rightDriveb] = sgn(vexRT[Ch2]) * speedLimit;
		}

		if(vexRT(Btn6U)==1){  //arm lifts
			motor[ArmRb] = -127;
			motor[ArmRt] = 127;
			motor[ArmLt] = -127;
			motor[ArmLa] = 127;
		}
		else if(vexRT(Btn6D)==1){ //arm decends
			motor[ArmRb] = 127;
			motor[ArmRt] = -127;
			motor[ArmLt] = 127;
			motor[ArmLa] = -127;
		}
		else{
			motor[ArmRb]= 0;
			motor[ArmRt]= 0;
			motor[ArmLt]= 0;
			motor[ArmLa]= 0;
		}
		if(vexRT(Btn5U)==1){  //arm lifts
			motor[clawL] = -127;
			motor[clawR] = 127;
		}
		else if(vexRT(Btn5D)==1){ //arm decends
			motor[clawL] = 127;
			motor[clawR] = -127;
		}
		else{
			motor[clawL] = 0;
			motor[clawR] = 0;
		}
		if(vexRT(Btn8L)==1)
		{
			motor [leftDrivea] = 127;
			motor [leftDriveb] = 127;
			motor [rightDrivea] = 127;
			motor [rightDriveb] = 127;
			sleep(1100);

			motor [leftDrivea] = -127;
			motor [leftDriveb] = -127;//backs up to align with cube
			motor [rightDrivea] = -127;
			motor [rightDriveb] = -127;
			sleep(400);

			motor [leftDrivea] = 90;//turns to alin wcube
			motor [leftDriveb] = 90;
			motor [rightDrivea] = -90;
			motor [rightDriveb] = -90;
			sleep(480);

			motor [leftDrivea] = -100;
			motor [leftDriveb] = -100;
			motor [rightDrivea] = -100;
			motor [rightDriveb] = -100;
			sleep(350);

			motor [leftDrivea] = 0;
			motor [leftDriveb] = 0;
			motor [rightDrivea] = 0;
			motor [rightDriveb] = 0;
			sleep(350);

			motor [leftDrivea] = 90;
			motor [leftDriveb] = 90;  // drives forward to go under cube
			motor [rightDrivea] = 90;
			motor [rightDriveb] = 90;
			sleep(1800);

			motor [leftDrivea] = 0;
			motor [leftDriveb] = 0;
			motor [rightDrivea] = 0;
			motor [rightDriveb] = 0;
			sleep(100);

			motor[ArmLa] = 60;
			motor[ArmLt] = -60;
			motor[ArmRt] = 60;
			motor[ArmRb] = -60;
			sleep(960);

			motor [leftDrivea] = 127;
			motor [leftDriveb] = 127;
			motor [rightDrivea] = -127;
			motor [rightDriveb] = -127;
			sleep(350);

			motor [leftDrivea] = -127;
			motor [leftDriveb] = -127;
			motor [rightDrivea] = -127;
			motor [rightDriveb] = -127;
			sleep(100);

			motor[ArmLa] = 100;
			motor[ArmLt] = -100;
			motor[ArmRt] = 100;
			motor[ArmRb] = -100;
			sleep(750);

			motor[ArmLa] = 0;
			motor[ArmLt] = 0;
			motor[ArmRt] = 0;
			motor[ArmRb] = 0;
			sleep(70);

			motor[ArmLa] = -100;
			motor[ArmLt] = 100;
			motor[ArmRt] = -100;
			motor[ArmRb] = 100;
			sleep(1500);

			motor[ArmLa] = 0;
			motor[ArmLt] = 0;
			motor[ArmRt] = 0;
			motor[ArmRb] = 0;
			sleep(100);
		}
	}
}

// This is the main execution loop for the user control program. Each time through the loop
// your program should update motor + servo values based on feedback from the joysticks.

// .....................................................................................
// Insert user code here. This is where you use the joystick values to update your motors, etc.
// .....................................................................................

// Remove this function call once you have "real" code.
