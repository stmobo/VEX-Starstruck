#pragma config(StandardModel, "RVW CLAWBOT")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

#define DEBUG

#include "../Enterprise.c"
#include "./Shimakaze.c"

replay_t replay;

//void pre_auton() {}

task autonomous() {
	if(bIfiAutonomousMode) {
	    control_t state;

	    initReplayData(&replay);

	    loadReplayFromFile("replay", &replay);

	    while(1) {
	        replayToControl(&state, &replay);
	        controlToMotors(state);

	        sleep(deltaT);
	    }
	  }
}

task usercontrol() {
    control_t state;

    initReplayData(&replay);

    while(1) {
        joystickToControl(&state);
        controlToMotors(state);
        controlToReplay(state, &replay);

        if(vexRT[Btn7R]) {
            break;
        }

        sleep(deltaT);
    }

    stopAllMotorsCustom();

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

    saveReplayToFile("replay", &replay);
}
