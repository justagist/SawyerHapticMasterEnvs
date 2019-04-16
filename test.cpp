#include <conio.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <process.h>

#include "HapticAPI.h"
#include "HapticMaster.h"
#include "HapticMasterOpenGl.h"
#include "glut.h"
#include "Vector3d.h"
#include <Windows.h>
#include <math.h>
#include <random>
#define IPADDRESS "10.0.11.12" // default IP
#define SECONDS_TO_LOG 20.0
#include <ws2tcpip.h>
#include <time.h>
#include <cstdlib>
#include <sys/timeb.h>
//#define PRINTER(name) getVarName(#name, (name))
//
//string getVarName(char *name){
//	return 
//}



bool visualiseEnv = false;

int flag = 0, measure = 0;
double virtualPos[3] = {0.0, 0.0, 0.0};
double springPos1[3] =  {0.0, 0.0, 0.0};
double springPos2[3] =  {0.0, 0.0, 0.0};
int stop_bias = 0;
#include "HapticGraphics.h"


double blockStiffnessEnv = 20000.0;

// Global variables
bool bContinue = true;
int gUpdateRateCounter;
double recvData[3];
double *precvData = recvData;

// HapticMASTER handles
long device = 0;
char response[100];
char gCurrentState[30] = "unknown";
double CurrentPosition[3];
double CurrentVelocity[3];
double dampingFactor = 0.0;
double distBetweenPoints;
Vector3d prevPos;

FILE *fp;
float** theMatrix;
int matrixColumnCount = 0;

double PCFreq;
__int64 CounterStart;

struct Block {
	double position[3];
	double size[3];
};

//
//vector<Block> blocks { 
//	{{1.0,2.0,3.0},{1.0,2.2,3.4}},{{1.0,2.0,3.0},{1.0,2.2,3.4}
//	};

//struct block block1;
//block1.num = 1;

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
				// Escape will end the application
				*((bool*)pParam) = false;
				break;
			default :
				// any other key will also end the application
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

		//printf("RECV: %d %x %x %x\n", status,recvData_tmp[0],recvData_tmp[1],recvData_tmp[2]);
		//printf("RECV: %d %f %f %f\n", status,recvData[0],recvData[1],recvData[2]);
	}

	closesocket(serverSock);
	WSACleanup();

	_endthread();
}


double springStiffness = 00;
double springDampFactor = 0;
double springMaxForce = 100.0;

void setInitEnv()
{
		// Creating block1 object: constraint block1
		
		if ( haSendCommand(device, "create block myBlock1", response) ) 
		{
			printf("failed sending command: create block myBlock1\n");
			getchar(); exit(-1);
		}
		//Vector3d eePos;
		haSendCommand(device, "get measpos", response);
			ParseFloatVec(response, prevPos.x, prevPos.y, prevPos.z);

		blockPos1[PosX] = prevPos.x + 0.02;
		blockPos2[PosX] = prevPos.x - 0.02;

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
	
	  // Create a damper effect for position control
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

		

		//springPos[0] = eePos.x;   springPos[1] = eePos.y;   springPos[2] = eePos.z;
		//// Create the Haptic spring Effect and supply it with parameters
		//if ( haSendCommand(device, "create spring mySpring", response) ) {
		//printf("--- ERROR: Could not send command create spring mySpring\n");
		//}
		//printf("create spring mySpring ==> %s\n", response);
		//if ( strstr ( response, "--- ERROR:" ) ) {
		//getchar();
		//exit(-1);
		//}
		//else {
		//haSendCommand(device, "set mySpring stiffness", springStiffness, response);
		//printf("set mySpring stiffness %g ==> %s\n", springStiffness, response);
		//haSendCommand(device, "set mySpring pos", springPos[PosX], springPos[PosY],
		//springPos[PosZ], response);
		//printf("set mySpring pos [%g,%g,%g] ==> %s\n", springPos[PosX], springPos[PosY],
		//springPos[PosZ], response);
		//haSendCommand(device, "set mySpring dampfactor", springDampFactor, response);
		//printf("set mySpring dampfactor %g ==> %s\n", springDampFactor, response);
		//haSendCommand(device, "set mySpring maxforce", springMaxForce, response);
		//printf("set mySpring maxforce %g ==> %s\n", springMaxForce, response);
		//haSendCommand(device, "set mySpring enable", response);
		//printf("set mySpring enable ==> %s\n", response);
		//}

		//// Create the Haptic BiasForce Effect and supply it with parameters
		//if ( haSendCommand(device, "create biasforce myBiasForce", response) ) {
		//printf ("--- ERROR: Could not send command create biasforce myBiasForce\n");
		//}
		//printf("create biasforce myBiasForce ==> %s\n", response);
		//if ( strstr (response, "--- ERROR:" ) ) {
		//getchar();
		//exit(-1);
		//}
		//else {
		//printf("create biasforce myBiasForce ==> %s\n", response);
		//haSendCommand(device, "set myBiasForce force [0.0,-1.0,0.0]", response);
		//printf("set myBiasForce force [0.0,-1.0,0.0] ==> %s\n", response);
		//
		//}

}

void Graphics(void *pParam)
{
	// OpenGL Initialization Calls
	char *myargv [1];
	int myargc=1;
	myargv [0]=strdup ("Myappname");
	glutInit(&myargc, myargv);
	glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (800, 600);

	// Create The OpenGlWindow
	
	glutCreateWindow ("HapticAPI Programming Manual : Example07: More Haptic effects");
	haSendCommand( device, "get modelpos", response );
	InitOpenGl();

	// More OpenGL Initialization Calls
	
	
	createBlock(blockPos1, blockSize1);
	createBlock(blockPos2, blockSize2);
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutMainLoop();

	_endthread();
}
/////////////////////////////////////////////////
// MAIN
/////////////////////////////////////////////////
int main(void)
{
	// Set the variables to their default values
	double effInertia = 4.0;
	char response[100];
	
	// double DamperCoeff[3] = {10.0, 10.0, 10.0};
	// double SpringStiffness = 500.0;
	double comdPosition1[3] = {0.0, 0.0, 0.0};
	double comdPosition1bias[3] = {0.0, 0.0, 0.0};
	double comdPosition2[3] = {0.0, 0.0, 0.0};
	double comdPosition3[3] = {0.0, 0.0, 0.0};
	double comdPosition3bias[3] = {0.0, 0.0, 0.0};
	// 3D vectors for measured values, calculation and commands
	Vector3d measPos;
	Vector3d measVel;
	Vector3d measForce;
	Vector3d measPosJoint;


	// measPos.x = 0.0; measPos.y = 0.0; measPos.z = 0.0;

	// UDP variables
	static double hapticData[10];
	char ip[30];
	unsigned short port = 10000; // remote port

	printf("-------------------------------------------------------\n");
	printf("            HapticMaster UDP Interface                 \n");
	printf("                 position control                      \n");
	printf("                       IJS                             \n");
	printf("-------------------------------------------------------\n\n");

	printf("Go to web browser to 178.172.42.83 for more options.\n\n");

	printf("Recv data:	desired (3 double values)\n");
	printf("		position	[x y z]\n");
	printf("Send data:	measured (9 double values)\n");
	printf("		position	[x y z]\n");
	printf("		velocity	[x y z]\n");
	printf("		force		[x y z]\n");
	printf("	act	position	[z]\n\n");
	printf("Recv port:	45156 \n");
	printf("Send port:	%d\n", port);
	// printf("Enter destination IP:  ");
	// gets(ip);
	// if(strlen(ip) < 8)
	// {
	//  strcpy(ip, "10.0.11.100");
	//  printf("IP:	10.0.11.100\n\n");
	// }
	
	strcpy(ip, "10.0.11.100");
	printf("IP address:     10.0.11.100\n\n");

	//// Set virtual spring stiffness
	// printf("Enter virtual spring stiffness (between 100-1000 N/m):  ");
	// cin >> SpringStiffness;
	// if(SpringStiffness >= 0 && SpringStiffness <= 1000) ;
	// else SpringStiffness = 500.0;
	// printf("Stiffness set to %.4f N/m\n\n", SpringStiffness);
	
	// Set spring type
	//printf("[1] linear [2] non-linear (quad) [3] non-linear (concave) [4] linear (half)");
	//cin >> springType;
	//if(springType == 1 | springType == 2 | springType == 3 | springType == 4){
		//printf("Spring Type %d \n\n", springType);
	//}else{
	//}
	
	size_t pkt_length = sizeof(double)*10;
	sockaddr_in destination;
	//sockaddr_in local;
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

	// HAPTICMASTER INIT
	device = haDeviceOpen( IPADDRESS );
	fprintf(stderr, "Created.\n");
	// Problems with connecting to HapticMASTER
	if( device == HARET_ERROR )
	{
		printf( "--- ERROR: Unable to connect to 1st device: %s\n", IPADDRESS );
		getchar();
		return HARET_ERROR;
	}
	// Connection to HapticMASTER succeeded
	else
	{
		// Initialize HapticMASTER
		InitializeDevice( device );
		// Set force state
		haSendCommand(device, "set state force", response);
		if ( strstr ( response, "--- ERROR:" ) )
		{
			printf("device: set force ==> %s\n", response);
			getchar(); exit(-1);
		}
		// Set inertia
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
		printf("Press any key to stop the application\n");
		printf("_______________________________________________________\n\n");

		int stepCounter = 0;
		

		while ( bContinue )
		{
			gUpdateRateCounter++;

			// Get positions in m
			haSendCommand(device, "get measpos", response);
			ParseFloatVec(response, measPos.x, measPos.y, measPos.z);

			//printf("%f\n", measPos.y);
			// Get velocities in m/s
			haSendCommand(device, "get measvel", response);
			ParseFloatVec(response, measVel.x, measVel.y, measVel.z);

			// Get forces in N
			haSendCommand(device, "get measforce", response);
			ParseFloatVec(response, measForce.x, measForce.y, measForce.z);

			//if (measPos.y > 0)
			//{
			//	haSendCommand(device, "set myBiasForce enable", response);
			//	//printf("set myBiasForce enable ==> %s\n", response);
			//}
			//else
			//{
			//	haSendCommand(device, "set myBiasForce disable", response);
			//	//printf("set myBiasForce enable ==> %s\n", response);
			//}

			// Send to destination IP
			hapticData[0] = measPos.x;   hapticData[1] = measPos.y;   hapticData[2] = measPos.z;
			hapticData[3] = measVel.x;   hapticData[4] = measVel.y;   hapticData[5] = measVel.z;
			hapticData[6] = measForce.x; hapticData[7] = measForce.y; hapticData[8] = measForce.z;

			virtualPos[0] = measPos.x;   virtualPos[1] = measPos.y;   virtualPos[2] = measPos.z;

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
			if (dampingFactor <= 70.0)
			{
				if (stepCounter % 10 == 1){
					dampingFactor += 0.02;
					haSendCommand(device, "set myDamper dampcoef", dampingFactor, dampingFactor, dampingFactor, response);
					haSendCommand(device, "set myDamper enable", response);
					if (stepCounter % 100 == 1){
						printf("Step %d: Current damping factor %f\n\n",stepCounter,dampingFactor);
						printf("Current damping factor %f\n\n",measForce.length());
					}
				
				}
			
				// Increase damping with distance
				if (distBetweenPoints >= 0.01)
				{
					stepCounter ++;
				}
			}
		}
	}


	// When the application finishes, do some cleaning up

	// UDP cleanup
	closesocket(s);
	WSACleanup();

	// HapticMaster cleanup
	if (device)
	{
		haSendCommand(device, "remove all", response);
		haSendCommand(device, "set state stop", response);
		haDeviceClose(device);
	}
		printf("Shutting down application...\n");

	return 1;
}