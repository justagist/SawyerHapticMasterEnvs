
#ifndef _PORRIDGE_ENV_
#define _PORRIDGE_ENV_
#include <conio.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <process.h>
#define IPADDRESS "10.0.11.12" // default IP
#define SECONDS_TO_LOG 20.0

#include "include/HapticGraphicsVisualiser.h"
#include "include/Vector3d.h"
#include <Windows.h>
#include <math.h>
#include <random>
#include <ws2tcpip.h>
#include <time.h>
#include <cstdlib>
#include <sys/timeb.h>

namespace sawyerEnvs{

double recvData[3];
double *precvData = recvData;
char response[100];
int device;

class porridgeEnv 
{

	private:

		double blockStiffnessEnv; // stiffness of blocks
		double dampingFactor; // starting damping factor 

		// ----- Global variables
		
		

		double distBetweenPoints;
		Vector3d prevPos;

		bool visualiseEnv;
				
		bool bContinue;

		FILE *fp;
		float** theMatrix;
		int matrixColumnCount;

		double PCFreq;
		__int64 CounterStart;

		void StartCounter();

		double GetCounter();

		int GetMilliCount();

		int GetMilliSpan(int nTimeStart);

		//static void KbMon(void *pParam);

		int constrainPlaneNormal;

		//void UDPrecv(void *pParam);

		double planePlay[2];

		//void Graphics(void *pParam);

		double maxDampingFactor, dampingIncrement, minVelToChangeDamping;
		/* 
		 Constrain robot motion to a plane (with some play)
		*/
		void setInitEnv();

	public:
		porridgeEnv(int device_id, int constrainNormal, double planeTolerance[2],  double minDampFactor, double maxDampingFctr, double dampInc, double minVelThreshold, bool visualise = false);
		//~porridgeEnv();

		int mainloop();

};

porridgeEnv::porridgeEnv(int device_id, int constrainNormal, double planeTolerance[2], double minDampFactor, double maxDampingFctr, double dampInc, double minVelThreshold, bool visualise) {
		
		maxDampingFactor = maxDampingFctr;
		dampingIncrement = dampInc;
		minVelToChangeDamping = minVelThreshold;

		planePlay[0] = planeTolerance[0]; planePlay[1] = planeTolerance[1];

		constrainPlaneNormal = constrainNormal;
		device = device_id;
		blockStiffnessEnv = 20000.0; // stiffness of blocks
		dampingFactor = minDampFactor; // starting damping factor 



		visualiseEnv = visualise;
		// ----- Global variables
		bContinue = true;
		
		
		matrixColumnCount = 0;
	}

	//porridgeEnv::~porridgeEnv(){}

	void porridgeEnv::StartCounter()
	{
		LARGE_INTEGER li;
		if(!QueryPerformanceCounter(&li))
			std::cout << "QUesryPerformanceCounter failed!\n";

		PCFreq = double(li.QuadPart)/1000.0;

		QueryPerformanceCounter(&li);
		CounterStart = li.QuadPart;

	}

	double porridgeEnv::GetCounter()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return double(li.QuadPart - CounterStart)/PCFreq;
	}

	int porridgeEnv::GetMilliCount()
	{
		timeb tb;
		ftime(&tb);
		int nCount = tb.millitm + (tb.time & 0xfffff)*1000;
		return nCount;
	}

	int porridgeEnv::GetMilliSpan(int nTimeStart)
	{
		int nSpan = GetMilliCount() - nTimeStart;
		if(nSpan < 0){
			nSpan += 0x100000*1000;

		}
		return nSpan;
	}

	/////
	void KbMon(void *pParam)
	{
		while ( *((bool*)pParam) )
		{  
      
			Sleep(1000);

			if ( _kbhit() )
			{
				switch(_getch())
				{
				case 27 :
					// ----- Escape will end the application
					*((bool*)pParam) = false;
					break;
				default :
					// ----- any other key will also end the application
					*((bool*)pParam) = false;
					break;
				}
			}
		}
		_endthread();
	}


	void UDPrecv(void *pParam)
	{
		struct sockaddr_in serverAddr;
		struct sockaddr_in clientAddr;
		WSADATA wsaData;
		SOCKET serverSock;
		int len=sizeof(struct sockaddr);
		unsigned short serverPort = 45156;

		if (WSAStartup(0x0202,&wsaData) != 0)
		{
			fprintf(stderr, "Could not open Windows connection.\n");
			exit(1);
		}

		memset((char*)&serverAddr,0,sizeof(serverAddr));
		serverAddr.sin_family=AF_INET;
		serverAddr.sin_addr.s_addr=INADDR_ANY;
		serverAddr.sin_port=htons(serverPort);

		serverSock = socket(AF_INET, SOCK_DGRAM, 0);
		if (serverSock < 0)
		{
			fprintf(stderr, "Could not create receive socket.\n");
			WSACleanup();
			exit(1);
		}

		if (bind(serverSock,(LPSOCKADDR)&serverAddr,len)<0)
		{
			fprintf(stderr,"Binding receive socket failed.\n");
			exit(1);
		}

		while ( *((bool*)pParam) )
		{
			int status = recvfrom(serverSock,(char *)precvData,24,0,(LPSOCKADDR)&clientAddr,&len);

		}

		closesocket(serverSock);
		WSACleanup();

		_endthread();
	}

	void Graphics(void *pParam)
	{
		// ----- OpenGL Initialization Calls
		char *myargv [1];
		int myargc=1;
		myargv [0]=strdup ("Myappname");
		glutInit(&myargc, myargv);
		glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize (800, 600);

		// ----- Create The OpenGlWindow
	
		glutCreateWindow ("HapticAPI Programming Manual : Example07: More Haptic effects");
		haSendCommand( device, "get modelpos", response );
		HMVisualiser::InitOpenGl();

		glutReshapeFunc(HMVisualiser::Reshape);
		glutDisplayFunc(HMVisualiser::Display);
		glutMainLoop();

		_endthread();
	}


	void porridgeEnv::setInitEnv()
	{

		haSendCommand(device, "get measpos", response);
		ParseFloatVec(response, prevPos.x, prevPos.y, prevPos.z);

		double blockPos1[3] = { 0.0,  0.0,  0.0};
		double blockPos2[3] = { 0.0,  0.0,  0.0};
		
		double blockSize1[3] = { 0.0,  0.0,  0.0};; // = {0.02,0.8, 0.8};
		double blockSize2[3] = { 0.0,  0.0,  0.0};; // = {0.02,0.8, 0.8}

		// ----- set plane constraints
		switch (constrainPlaneNormal){
		case 0: { blockPos1[PosX] = prevPos.x - planePlay[0] - 0.01; 
					blockPos1[PosY] = blockPos1[PosZ] = 0.0; 
					blockSize1[0] = 0.02; blockSize1[1] = blockSize1[2] = 0.8;

					blockPos2[PosX] = prevPos.x + planePlay[0] + 0.01; 
					blockPos2[PosY] = blockPos1[PosZ] = 0.0; 
					blockSize2[0] = 0.02; blockSize2[1] = blockSize2[2] = 0.8;
				}; break;
		case 1: { blockPos1[PosY] = prevPos.y - planePlay[0] - 0.01; 
					blockPos1[PosX] = blockPos1[PosZ] = 0.0; 
					blockSize1[1] = 0.02; blockSize1[0] = blockSize1[2] = 0.8;

					blockPos2[PosY] = prevPos.y + planePlay[0] + 0.01; 
					blockPos2[PosX] = blockPos1[PosZ] = 0.0; 
					blockSize2[1] = 0.02; blockSize2[0] = blockSize2[2] = 0.8;
				}; break;
		case 2: { blockPos1[PosZ] = prevPos.z - planePlay[0] - 0.01; 
					blockPos1[PosX] = blockPos1[PosY] = 0.0; 
					blockSize1[2] = 0.02; blockSize1[0] = blockSize1[1] = 0.8;

					blockPos2[PosZ] = prevPos.z + planePlay[0] + 0.01; 
					blockPos2[PosX] = blockPos1[PosY] = 0.0; 
					blockSize2[2] = 0.02; blockSize2[0] = blockSize2[1] = 0.8;
				}; break;
		default: break;

		}

		if ( haSendCommand(device, "create block myBlock1", response) ) 
		{
			printf("failed sending command: create block myBlock1\n");
			getchar(); exit(-1);
		}

		if ( strstr(response, "--- ERROR:" ) ) // Error response returned from command execution
		{
			getchar(); exit(-1);
		}
		
		else // No error response from command execution, go further with settings
		{
			haSendCommand(device, "set myBlock1 pos", blockPos1[PosX], blockPos1[PosY], blockPos1[PosZ], response);
			haSendCommand(device, "set myBlock1 size", blockSize1[0], blockSize1[1], blockSize1[2], response);
			haSendCommand(device, "set myBlock1 stiffness", blockStiffnessEnv, response);
			haSendCommand(device, "set myBlock1 enable", response);
		}
		/////
		if ( haSendCommand(device, "create block myBlock2", response) ) {
			printf("failed sending command: create block myBlock2\n");
			getchar(); exit(-1);
		}


		if ( strstr(response, "--- ERROR:" ) )  // Error response returned from command execution
		{      
			getchar(); exit(-1);
		}

		else // No error response from command execution, go further with settings
		{
			haSendCommand(device, "set myBlock2 pos", blockPos2[PosX], blockPos2[PosY], blockPos2[PosZ], response);
			haSendCommand(device, "set myBlock2 size", blockSize2[0], blockSize2[1], blockSize2[2], response);
			haSendCommand(device, "set myBlock2 stiffness", blockStiffnessEnv, response);
			haSendCommand(device, "set myBlock2 enable", response);
		}

		if (visualiseEnv)
		{
			HMVisualiser::addBlock(blockPos1,blockSize1);
			HMVisualiser::addBlock(blockPos2,blockSize2);
		}

		if ( haSendCommand(device, "create damper myDamper", response) )
		{
			printf ( "--- ERROR: Could not send command create damper myDamper" );   
		}

		printf("create damper myDamper ==> %s\n", response);

		if ( strstr ( response, "--- ERROR:" ) )
		{
			getchar();
			exit(-1);
		}
	
		
	}

	int porridgeEnv::mainloop()
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
			_beginthread(KbMon, 0, &bContinue);
			_beginthread(UDPrecv, 0, &bContinue);
			if (visualiseEnv)
			{
				_beginthread(Graphics, 0, &bContinue);
			}

			setInitEnv();
			printf("\n_______________________________________________________\n");
			printf("OPTIONS:\n");
			printf("Press Enter to start Posrridge Simulation\n");
			printf("_______________________________________________________\n\n");

			int stepCounter = 0;

			haSendCommand(device, "set myDamper dampcoef", dampingFactor, dampingFactor, dampingFactor, response);
			haSendCommand(device, "set myDamper enable", response);

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
					printf("\rStep %d: Position: %f, %f, %f; Damping factor %f",stepCounter,measPos.x, measPos.y, measPos.z,dampingFactor);
					fflush(stdout);
				}
				prevPos = Vector3d(measPos);
			
				int ret = sendto( s, (char *)hapticData, pkt_length, 0, (sockaddr *)&destination, sizeof(destination) ); // send packet
				if(ret==SOCKET_ERROR){
					std::cout<<"ERROR\n";
					break;
				}
			
				stepCounter ++;
			}
		
			printf("\n_______________________________________________________\n");
			printf("OPTIONS:\n");
			printf("Porridge Environment Intitialised. Press Enter to quit.\n");
			printf("_______________________________________________________\n\n");
		
			stepCounter = 0;
			bContinue = true;
			_beginthread(KbMon, 0, &bContinue);

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

				distBetweenPoints = measVel.length();

				//if (stepCounter % 1000 == 0)
					//printf("Step %d: Current damping factor %f\n",stepCounter,distBetweenPoints);
				//printf("currPoint: %f,%f,%f ; distance: %f\n", measPos.x, measPos.y, measPos.z, distBetweenPoints);
			
				prevPos = Vector3d(measPos);
			
				int ret = sendto( s, (char *)hapticData, pkt_length, 0, (sockaddr *)&destination, sizeof(destination) ); // send packet
				if(ret==SOCKET_ERROR){
					std::cout<<"ERROR\n";
					break;
				}
				if (dampingFactor <= maxDampingFactor)
				{
					if (stepCounter % 10 == 1){
						dampingFactor += dampingIncrement;
						haSendCommand(device, "set myDamper dampcoef", dampingFactor, dampingFactor, dampingFactor, response);
						haSendCommand(device, "set myDamper enable", response);
						if (stepCounter % 100 == 1){
							printf("\rPosition: %f, %f, %f; Damping factor %f",measPos.x, measPos.y, measPos.z,dampingFactor);
							//printf("Current damping factor %f\n\n",measForce.length());
							fflush(stdout);
						}
				
					}
			
			
					// ----- Increase damping with distance
					if (distBetweenPoints >= minVelToChangeDamping)
					{
						stepCounter ++;
					}
				}
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