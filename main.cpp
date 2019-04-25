//*********************************************************************
//
// package : SawyerHapticMasterEnvs
// file    : main.cpp
// purpose : main file to simulate sawyer experiment environments
//
// author  : Saif Sidhik
// email   : sxs1412@bham.ac.uk
//
//*********************************************************************
#include <conio.h>
#include <stdio.h>
#include <iostream>
#define IPADDRESS "10.0.11.12" // default IP
#define SECONDS_TO_LOG 20.0


using namespace std;

enum envs { PORRIDGE , SPRING };

envs ENVIRONMENT = PORRIDGE;
bool visualiseEnv = true;


enum axis {X_axis, Y_axis, Z_axis, NONE};
/////////////////////////////////////////////////
// Environment Parameters --- Porridge
/////////////////////////////////////////////////
double minDampingFactor = 50.0;
double maxDampingFactor = 500.0;
double planePlay[2] = {0.01, 0.01}; // play in plane for remaining in plane (along both directions)
double minVelToChangeDamping = 0.01; // step count is incremented if velocity is above this value
double dampingIncrement = 0.05; // increment in dampingFactor per 10 steps
axis constrainPlaneNormal = NONE; // Normal of the plane to constrain motion (X_axis / Y_axis / Z_axis / NONE)

/////////////////////////////////////////////////
// Environment Parameters -- Linear Spring
/////////////////////////////////////////////////
double springStiffness = 2;
double springPos[] = {-0.233, -0.100, -0.111}; // if spring is to be at current end-effector position, set makeSpringHere = true
bool makeSpringHere = false; // if set to true, initial value of springPos is ignored
double springDampFactor = 0.7;
double springMaxForce = 7.0;
/////////////////////////////////////////////////

// ----- HapticMASTER handles
long device = 0;
double recvData[3];
double *precvData = recvData;
char response[100];

#include "include/PorridgeEnv.h"
#include "include/SpringEnv.h"

int main()
{
	
	switch (ENVIRONMENT){
	case PORRIDGE: 
		{
			sawyerEnvs::PorridgeEnv env = sawyerEnvs::PorridgeEnv(constrainPlaneNormal, planePlay, minDampingFactor, maxDampingFactor, dampingIncrement, minVelToChangeDamping, visualiseEnv);
			env.mainloop();
			break;
		}
	 case SPRING:
	 	{
	 		sawyerEnvs::SpringEnv env = sawyerEnvs::SpringEnv(springStiffness, springDampFactor, springMaxForce, springPos, makeSpringHere, visualiseEnv);
	 		env.mainloop();
	 		break;
	 	}
	default:
		break;
	}

}