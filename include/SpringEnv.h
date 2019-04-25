
#ifndef _SPRING_ENV_
#define _SPRING_ENV_
#include <conio.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <process.h>
#define IPADDRESS "10.0.11.12" // default IP
#define SECONDS_TO_LOG 20.0

#include "include/HapticGraphicsVisualiser.h"
#include "include/HMThreads.h"
#include "include/Vector3d.h"

namespace sawyerEnvs{

class SpringEnv 
{

	private:

		double springStiffness; // stiffness of blocks
		double springPos[3]; 

		bool makeSpringHere; // if set to true, initial value of springPos is ignored
		
		double springMaxForce;

		double springDampFactor;

		Vector3d prevPos;

		bool visualiseEnv;
				
		bool bContinue;

		/* 
		 Create spring at specified location
		*/
		void setInitEnv();

	public:
		
		SpringEnv(double springStiffnessVal, double springDampingFactor, double maxSpringForce, double springPosition[3],  bool SpringAtCurrPos, bool visualise);
		//~SpringEnv();

		int mainloop();

};

	SpringEnv::SpringEnv(double springStiffnessVal, double springDampingFactor, double maxSpringForce, double springPosition[3],bool SpringAtCurrPos, bool visualise) 
	{
		springStiffness = springStiffnessVal;
		springDampFactor = springDampingFactor;
		springMaxForce = maxSpringForce;
		makeSpringHere = SpringAtCurrPos;
		springPos[0] = springPosition[0]; springPos[1] = springPosition[1]; springPos[2] = springPosition[2];
		makeSpringHere = SpringAtCurrPos;		
		visualiseEnv = visualise;
		// ----- Global variables
		bContinue = true;
	}

	void SpringEnv::setInitEnv()
	{

		Vector3d prevPos;
		haSendCommand(device, "get measpos", response);
		ParseFloatVec(response, prevPos.x, prevPos.y, prevPos.z);
		
		printf("spring Stiffness: %g\n",springDampFactor);
		if (makeSpringHere)
		{
			springPos[PosX] = prevPos.x; springPos[PosY] = prevPos.y; springPos[PosZ] = prevPos.z;
		}

		      // Create the Haptic spring Effect and supply it with parameters
		if ( haSendCommand(device, "create spring mySpring", response) ) {
			printf("--- ERROR: Could not send command create spring mySpring\n");
		}

		printf("create spring mySpring ==> %s\n", response);

		if ( strstr ( response, "--- ERROR:" ) ) {
			getchar();
			exit(-1);
		}
		else {
			haSendCommand(device, "set mySpring stiffness", springStiffness, response);
			printf("set mySpring stiffness %g ==> %s\n", springStiffness, response);
         
			haSendCommand(device, "set mySpring pos", springPos[PosX], springPos[PosY], springPos[PosZ], response);
			printf("set mySpring pos [%g,%g,%g] ==> %s\n", springPos[PosX], springPos[PosY], springPos[PosZ], response);
         
			haSendCommand(device, "set mySpring dampfactor", springDampFactor, response);
			printf("set mySpring dampfactor %g ==> %s\n", springDampFactor, response);
         
			haSendCommand(device, "set mySpring maxforce", springMaxForce, response);
			printf("set mySpring maxforce %g ==> %s\n", springMaxForce, response);
         
			haSendCommand(device, "set mySpring enable", response);
			printf("set mySpring enable ==> %s\n", response);
		}

		if (visualiseEnv)
		{
			HMVisualiser::addSpring(springPos);
		}
	
		
	}

	int SpringEnv::mainloop()
	{
		// ----- Set the variables to their default values
		double effInertia = 4.0;
		char response[100];
	
		Vector3d measPos;
		Vector3d measVel;
		Vector3d measForce;


		// ----- UDP variables
		static double hapticData[10];
		char ip[30];
		unsigned short port = 10000; // remote port

		printf("-------------------------------------------------------\n");
		printf("            Porridge Environment Simulation            \n");
		printf("-------------------------------------------------------\n\n");


		strcpy(ip, "10.0.11.100");
		printf("IP address:     10.0.11.100\n\n");
	
		size_t pkt_length = sizeof(double)*10;
		sockaddr_in destination;
		// ----- sockaddr_in local;
		WSAData data;
		WSAStartup( MAKEWORD( 2, 2 ), &data );

		destination.sin_family = AF_INET;
		destination.sin_addr.s_addr = inet_addr( ip );
		destination.sin_port = htons( port );

		SOCKET s = socket( AF_INET, SOCK_DGRAM, 0 );
		if(( s ==INVALID_SOCKET ))
		{
			fprintf(stderr, "Could not create send socket.\n");
			WSACleanup();
			exit(1);
		}

		// ----- HAPTICMASTER INIT
		device = haDeviceOpen( IPADDRESS );
		fprintf(stderr, "Created.\n");

		// ----- Problems with connecting to HapticMASTER
		if( device == HARET_ERROR )
		{
			printf( "--- ERROR: Unable to connect to 1st device: %s\n", IPADDRESS );
			getchar();
			return HARET_ERROR;
		}
		// ----- Connection to HapticMASTER succeeded
		else
		{
			// ----- Initialize HapticMASTER
			InitializeDevice( device );
			// ----- Set force state
			haSendCommand(device, "set state force", response);
			if ( strstr ( response, "--- ERROR:" ) )
			{
				printf("device: set force ==> %s\n", response);
				getchar(); exit(-1);
			}
			// ----- Set inertia
			haSendCommand(device, "set inertia", effInertia, response);
			if ( strstr ( response, "--- ERROR:" ) )
			{
				printf("device: set inertia ==> %s\n", response);
				getchar(); exit(-1);
			}
			_beginthread(HMThreads::KbMon, 0, &bContinue);
			_beginthread(HMThreads::UDPrecv, 0, &bContinue);
			if (visualiseEnv)
			{
				_beginthread(HMVisualiser::Graphics, 0, &bContinue);
			}

			setInitEnv();
			printf("\n_______________________________________________________\n");
			printf("OPTIONS:\n");
			printf("Spring Environment Initialised\n");
			printf("_________________________________________________________\n\n");

			int stepCounter = 0;

			while ( bContinue )
			{

				// ----- Get positions in m
				haSendCommand(device, "get measpos", response);
				ParseFloatVec(response, measPos.x, measPos.y, measPos.z);

				// ----- Get velocities in m/s
				haSendCommand(device, "get measvel", response);
				ParseFloatVec(response, measVel.x, measVel.y, measVel.z);

				// ----- Get forces in N
				haSendCommand(device, "get measforce", response);
				ParseFloatVec(response, measForce.x, measForce.y, measForce.z);

				// ----- Send to destination IP
				hapticData[0] = measPos.x;   hapticData[1] = measPos.y;   hapticData[2] = measPos.z;
				hapticData[3] = measVel.x;   hapticData[4] = measVel.y;   hapticData[5] = measVel.z;
				hapticData[6] = measForce.x; hapticData[7] = measForce.y; hapticData[8] = measForce.z;

				HMVisualiser::virtualPos[0] = measPos.x;   HMVisualiser::virtualPos[1] = measPos.y;   HMVisualiser::virtualPos[2] = measPos.z;

				if (stepCounter % 1000 == 0)
				{
					printf("\rPosition: %f,%f,%f	; Force:  %f,%f,%f", measPos.x, measPos.y, measPos.z, measForce.x, measForce.y, measForce.z);
					fflush(stdout);
				}
				int ret = sendto( s, (char *)hapticData, pkt_length, 0, (sockaddr *)&destination, sizeof(destination) ); // send packet
				if(ret==SOCKET_ERROR){
					std::cout<<"ERROR\n";
					break;
				}

				stepCounter ++;
			}
		}


		// ----- UDP cleanup
		closesocket(s);
		WSACleanup();

		// ----- HapticMaster cleanup
		if (device)
		{
			haSendCommand(device, "remove all", response);
			haSendCommand(device, "set state stop", response);
			haDeviceClose(device);
		}
			printf("Shutting down application...\n");

		return 1;
	}

}

#endif