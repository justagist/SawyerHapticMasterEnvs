
#include <conio.h>
#include <stdio.h>
#include <iostream>
using namespace std;

enum envs { PORRIDGE , SPRING };

envs ENVIRONMENT = PORRIDGE;
bool visualiseEnv = false;


enum axis {X_axis, Y_axis, Z_axis, NONE};
/////////////////////////////////////////////////
// Environment Parameters --- Porridge
/////////////////////////////////////////////////
double minDampingFactor = 100.0;
double maxDampingFactor = 500.0;
double planePlay[2] = {0.01, 0.01}; // play in plane for remaining in plane (along both directions)
double minVelToChangeDamping = 0.01; // step count is incremented if velocity is above this value
double dampingIncrement = 0.05; // increment in dampingFactor per 10 steps
axis constrainPlaneNormal = NONE; // Normal of the plane to constrain motion (X_axis / Y_axis / Z_axis / NONE)
/////////////////////////////////////////////////


// ----- HapticMASTER handles
long device = 0;
char response[100];

#include "include/porridgeEnv.h"

int main()
{
	
	switch (ENVIRONMENT){
	case PORRIDGE: 
		{
			sawyerEnvs::porridgeEnv env = sawyerEnvs::porridgeEnv(device, constrainPlaneNormal, planePlay, minDampingFactor, maxDampingFactor, dampingIncrement, minVelToChangeDamping, visualiseEnv);
			env.mainloop();
			break;
		}
	default:
		break;
	}

}