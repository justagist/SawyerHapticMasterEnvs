#include <conio.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <process.h>

#include "include/HapticAPI.h"
#include "include/HapticMaster.h"
#include "include/HapticMasterOpenGl.h"
#include "glut.h"
#include "include/Vector3d.h"
#include <Windows.h>
#include <math.h>
#include <random>
#define IPADDRESS "10.0.11.12" // default IP
#define SECONDS_TO_LOG 20.0
#include <ws2tcpip.h>
#include <time.h>
#include <cstdlib>
#include <sys/timeb.h>

/////////////////////////////////////////////////
// Environment Parameters -- Linear Spring
/////////////////////////////////////////////////
double springStiffness = 00;
double springPos[] = {-0.233, -0.100, -0.111}; // if spring is to be at current end-effector position, set makeSpringHere = true
bool makeSpringHere = false; // if set to true, initial value of springPos is ignored
double springDampFactor = 0.7;
double springMaxForce = 7.0;

bool visualiseEnv = false;
/////////////////////////////////////////////////

double virtualPos[3] = {0.0, 0.0, 0.0};

#include "include/HapticGraphicsVisualiser.h"

// ----- Global variables
bool bContinue = true;
int gUpdateRateCounter;
double recvData[3];
double *precvData = recvData;

// ----- HapticMASTER handles
long device = 0;
char response[100];

FILE *fp;
float** theMatrix;
int matrixColumnCount = 0;

double PCFreq;
__int64 CounterStart;


void StartCounter()
{
	LARGE_INTEGER li;
	if(!QueryPerformanceCounter(&li))
		std::cout << "QUesryPerformanceCounter failed!\n";

	PCFreq = double(li.QuadPart)/1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;

}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart)/PCFreq;
}

int GetMilliCount()
{
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff)*1000;
	return nCount;
}

int GetMilliSpan(int nTimeStart)
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

		//printf( "Loops: %i\n", gUpdateRateCounter );
		gUpdateRateCounter = 0;

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


/* 
 Constrain robot motion to a plane (with some play)
*/
void setInitEnv()
{
		Vector3d prevPos;
		haSendCommand(device, "get measpos", response);
		ParseFloatVec(response, prevPos.x, prevPos.y, prevPos.z);

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

/* 
	Required only for visualisation
*/
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

/////////////////////////////////////////////////
// MAIN
/////////////////////////////////////////////////
int main(void)
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
		printf("Press Enter to stop the application\n");
		printf("_______________________________________________________\n\n");

		int stepCounter = 0;
		

		while ( bContinue )
		{
			gUpdateRateCounter++;

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

			virtualPos[0] = measPos.x;   virtualPos[1] = measPos.y;   virtualPos[2] = measPos.z;

			if (stepCounter % 1000 == 0)
			{
				printf("\rPosition: %f,%f,%f	; Force:  %f,%f,%f", measPos.x, measPos.y, measPos.z, measForce.x, measForce.y, measForce.z);
				fflush(stdout);
				//printf("Step %d: Current damping factor %f\n",stepCounter,distBetweenPoints);
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