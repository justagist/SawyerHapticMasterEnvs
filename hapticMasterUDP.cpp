// hapticMasterUDP.cpp
// POSITION CONTROL
// original: Luka Peternel
// modified by Chie Takahashi
// modified by Laura Ferrante
//

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



double blockStiffnessEnv = 20000.0;
float y_1;
float y_11 = -0.197021;
float y_12 = -0.179420;
float y_13 = -0.158017;

float y_2; //-0.0846;
float y_21 = -0.081648; //-0.0846;
float y_22 = -0.047011; //-0.0846;
float y_23 = 0.002566; //-0.0846;

double kEnv;
double kLow = 1500.0;
double kHigh = 20000.0;

double dVisc;
double dLow = 0.0;
double dHigh = 40.0;

int pertDir1;
float pertLoc1;
int pertDir2;
float pertLoc2;

double blockStiffnessEnv1 ;
double viscous ;
int set_1 ;
int set_3 ;
/*
double blockStiffnessEnv1 = 1500.0;

double viscous = 40.0;
int set_1 = 0;
int set_3 = 1;
*/
/*
double blockStiffnessEnv1 = 20000.0;//20000.0;

double viscous = 40.0;
int set_1 = 0;
int set_3 = 0;
*/
/*
double blockStiffnessEnv1 = 1500.0;//20000.0;

double viscous = 0.0;
int set_1 = 0;
int set_3 = 0;
*/
/*
double blockStiffnessEnv1 = 20000.0;//20000.0;

double viscous = 0.0;
int set_1 = 0;
int set_3 = 1;
*/
/*
double blockStiffnessEnv1 = 20000.0;//20000.0;

double viscous = 0.0;
int set_1 = 1;
int set_3 = 0;
*/

/*
double blockStiffnessEnv1 = 1500.0;//20000.0;

double viscous = 40.0;
int set_1 = 1;
int set_3 = 1;
*/
/*
double blockStiffnessEnv1 = 1500.0;//20000.0;

double viscous = 0.0;
int set_1 = 1;
int set_3 = 0;
*/




//////////////////////////////////////
bool bias_xy = false;
/////////////////////////////////////

//#include "Timer.h"

//#import "HapticAPI.dll" //no_namespace // import DLL
int flag = 0, measure = 0;
double virtualPos[3] = {0.0, 0.0, 0.0};
double springPos1[3] =  {0.0, 0.0, 0.0};
double springPos2[3] =  {0.0, 0.0, 0.0};
int stop_bias = 0;
#include "HapticGraphicsVisualiser.h"


/*
//1
int set_1 = 1;
int set_2 = 1;
int set_3 = 0;
int set_4 = 0;
int set_5 = 0;
//2
int set_1 = 0;
int set_2 = 1;
int set_3 = 1;
int set_4 = 1;
int set_5 = 0;
//3
int set_1 = 0;
int set_2 = 0;
int set_3 = 0;
int set_4 = 1;
int set_5 = 0;
//4
int set_1 = 1;
int set_2 = 1;
int set_3 = 0;
int set_4 = 1;
int set_5 = 1;
*/

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

float calstiffness = 0.0;
float caldamping = 0.0;
float springlength1 = 0.0;
float springlength2 = 0.0;
int setTargetPos = 0;
int springType = 0;

int start;
float end_time,end_time_temp;

float y_3 = -0.0114;
float y_4 = 0.0967;
float y_5 = 0.1961;
float eps = 0.0500;

bool block1;
bool block2;
bool block3;
bool block4;
bool block5;

int primavolta;
// spring 
double damperCoef[3] = {0.0,0.0,0.0};
double springStiffness = 2;
// double springPos[3] =  {0.0, 0.0, 0.0};
double springDampFactor = 0.0;
double springMaxForce = 100.0;

/////
char subjNumber[10];
char trialNumber[10];
char envNumber[10];
char datafile[20];
int envType;

char gBuffer[80]; /// string buffer

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
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutMainLoop();

	_endthread();
}

// set up environment
void SetEnv()
{
   	
	switch (envType)
	   {
			case 0: // default (no perturbation)
				kEnv = kHigh;
				dVisc = dLow;
				pertLoc1 = y_11;
				pertDir1 = 0;
				pertLoc2 = y_21;
				pertDir2 = 0;
				break;

			case 1: 
				kEnv = kLow;
				dVisc = dLow;
				pertDir1 = 0;
				pertLoc1 = y_11;
				pertLoc2 = y_21;
				pertDir2 = 1;
				break;

			case 2: 
				kEnv = kLow;
				dVisc = dLow;
				pertDir1 = 1;
				pertLoc1 = y_12;
				pertLoc2 = y_21;
				pertDir2 = 0;
				break;

			case 3:
				kEnv = kLow;
				dVisc = dHigh;
				pertDir1 = 0;
				pertLoc1 = y_13;
				pertLoc2 = y_22;
				pertDir2 = 0;
				break;

			case 4:
				kEnv = kLow;
				dVisc = dHigh;
				pertDir1 = 1;
				pertLoc1 = y_11;
				pertLoc2 = y_22;
				pertDir2 = 1;
				break;
			case 5:
				kEnv = kHigh;
				dVisc = dLow;
				pertDir1 = 0;
				pertLoc1 = y_12;
				pertLoc2 = y_23;
				pertDir2 = 1;
				break;
			case 6:
				kEnv = kHigh;
				dVisc = dLow;
				pertDir1 = 1;
				pertLoc1 = y_13;
				pertLoc2 = y_23;
				pertDir2 = 0;
				break;
			case 7:
				kEnv = kHigh;
				dVisc = dHigh;
				pertDir1 = 0;
				pertLoc1 = y_11;
				pertLoc2 = y_21;
				pertDir2 = 1;
				break;
			case 8:
				kEnv = kLow;
				dVisc = dHigh;
				pertDir1 = 1;
				pertLoc1 = y_12;
				pertLoc2 = y_21;
				pertDir2 = 0;
				break;
		
	   }
		blockStiffnessEnv1 = kEnv;
		viscous = dVisc;
		y_1 = pertLoc1;
		y_3 = pertLoc2;
		set_1 = pertLoc1;
		set_3 = pertLoc2;
   }
// reak time data logger
void collect_data(void *pParam)
{

	
	// Open output file
	if( ( fp = fopen(datafile, "w") ) == NULL ) {
		printf("Cannot open file.\n");
		exit(1);
	}
	int returnValue;

   
	long logger = haDataLoggerOpen ( IPADDRESS );
	haDataLoggerAddParameter ( logger, "samplenr", &matrixColumnCount );
	haDataLoggerAddParameter ( logger, "sampletime", &matrixColumnCount );
	haDataLoggerAddParameter ( logger, "#HapticMASTER.Model.Cartesian.pos Y", &matrixColumnCount );
    haDataLoggerAddParameter ( logger, "#HapticMASTER.Model.Cartesian.pos Z", &matrixColumnCount );
	haDataLoggerAddParameter ( logger, "#HapticMASTER.Model.Cartesian.vel Y", &matrixColumnCount );
    haDataLoggerAddParameter ( logger, "#HapticMASTER.Model.Cartesian.vel Z", &matrixColumnCount );
	haDataLoggerAddParameter ( logger, "#HapticMASTER.Model.Cartesian.acc Y", &matrixColumnCount );
	haDataLoggerAddParameter ( logger, "#HapticMASTER.Model.Cartesian.acc Z", &matrixColumnCount );
	haDataLoggerAddParameter ( logger, "#HapticMASTER.Measured force.meas force Y", &matrixColumnCount );
	haDataLoggerAddParameter ( logger, "#HapticMASTER.Measured force.meas force Z", &matrixColumnCount );
	haDataLoggerAddParameter ( logger, "#HapticMASTER.Measured force.Force Sensor Z.output", &matrixColumnCount );
    haDataLoggerAllocMatrix ( 512, matrixColumnCount, theMatrix );

	printf( "running a data logger session...\n" );
	haDataLoggerStart(logger); 

    fprintf ( fp, "%% Column 1: samplenr\n" );
    fprintf ( fp, "%% Column 2: sampletime\n" );
	fprintf ( fp, "%% Column 8: #HapticMASTER.Model.Cartesian.pos Y\n" );
    fprintf ( fp, "%% Column 5: #HapticMASTER.Model.Cartesian.pos Z\n" );
	fprintf ( fp, "%% Column 9: #HapticMASTER.Model.Cartesian.vel Y\n" );
    fprintf ( fp, "%% Column 6: #HapticMASTER.Model.Cartesian.vel Z\n" );
	fprintf ( fp, "%% Column 7: #HapticMASTER.Model.Cartesian.acc Y\n" );
    fprintf ( fp, "%% Column 7: #HapticMASTER.Model.Cartesian.acc Z\n" );
	fprintf ( fp, "%% Column 3: #HapticMASTER.Measured force.meas force Y\n" );
    fprintf ( fp, "%% Column 4: #HapticMASTER.Measured force.meas force Z\n" );
	fprintf ( fp, "%% Column 4: #HapticMASTER.Measured force.meas force Z\n" );
    unsigned int rowsFlushed = 0;
    unsigned int totalRowsFlushed = 0;
	while (  *((bool*)pParam)  )
    {	
		
	// Flush the matrix and query number of rows flushed
		rowsFlushed = haDataLoggerFlushMatrix(logger, theMatrix);
		totalRowsFlushed += rowsFlushed;
		if (rowsFlushed != -1) {
		// Iterate through all flushed rows
			
			for (unsigned int i = 0; i < rowsFlushed; i++) {
				for (int j = 0; j < matrixColumnCount; j++) {
					// Write flushed matrix to the output file
					fprintf ( fp, "%g ", theMatrix[j][i]);
				}
				if ( theMatrix[1][i] >= SECONDS_TO_LOG ) {
					bContinue = false;
				}
				fprintf ( fp, "\n" );
			}
		}
		else {
			printf ("Error flushing the RT DataLogger. Please press any key to continue\n");
			getchar();
			bContinue = false;
		}
	}


   
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
	springType = 1;
	//}
	
	// data input
	cout << "\n";
	cout << "Please enter the subject number: ";
	cin >> subjNumber;
	cout << "Please enter the trial number: ";
	cin >> trialNumber;
	cout << "Please enter the environment number: ";
	cin >> envNumber;
	envType = int(envNumber);
	SetEnv();
	// UDP INIT	
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
		
		// Creating block1 object: constraint block1
		
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
		/////
		if ( haSendCommand(device, "create block myCube11", response) ) 
		{
			printf("failed sending command: create block myCube11\n");
			getchar(); exit(-1);
		}
		
		if ( strstr(response, "--- ERROR:" ) ) // Error response returned from command execution
		{
			getchar(); exit(-1);
		}
	  
		else // No error response from command execution, go further with settings
		{
			haSendCommand(device, "set myCube11 pos", posCu_11[PosX], posCu_11[PosY], posCu_11[PosZ], response);
			haSendCommand(device, "set myCube11 size", sizeCu_11a, sizeCu_11a, sizeCu_11a, response);
			haSendCommand(device, "set myCube11 stiffness", blockStiffnessEnv1, response);
			haSendCommand(device, "set myCube11 enable", response);
		}
		/////
		if ( haSendCommand(device, "create block myCube12", response) ) 
		{
			printf("failed sending command: create block myCube12\n");
			getchar(); exit(-1);
		}
		
		if ( strstr(response, "--- ERROR:" ) ) // Error response returned from command execution
		{
			getchar(); exit(-1);
		}
	  
		else // No error response from command execution, go further with settings
		{
			haSendCommand(device, "set myCube12 pos", posCu_12[PosX], posCu_12[PosY], posCu_12[PosZ], response);
			haSendCommand(device, "set myCube12 size", sizeCu_12a, sizeCu_12a, sizeCu_12a, response);
			haSendCommand(device, "set myCube12 stiffness", blockStiffnessEnv1, response);
			haSendCommand(device, "set myCube12 enable", response);
		}
		/////
		if ( haSendCommand(device, "create block myCube21", response) ) 
		{
			printf("failed sending command: create block myCube21\n");
			getchar(); exit(-1);
		}
		
		if ( strstr(response, "--- ERROR:" ) ) // Error response returned from command execution
		{
			getchar(); exit(-1);
		}
	  
		else // No error response from command execution, go further with settings
		{
			haSendCommand(device, "set myCube21 pos", posCu_21[PosX], posCu_21[PosY], posCu_21[PosZ], response);
			haSendCommand(device, "set myCube21 size", sizeCu_21a, sizeCu_21a, sizeCu_21a, response);
			haSendCommand(device, "set myCube21 stiffness", blockStiffnessEnv1, response);
			haSendCommand(device, "set myCube21 enable", response);
		}
		/////
		if ( haSendCommand(device, "create block myCube22", response) ) 
		{
			printf("failed sending command: create block myCube22\n");
			getchar(); exit(-1);
		}
		
		if ( strstr(response, "--- ERROR:" ) ) // Error response returned from command execution
		{
			getchar(); exit(-1);
		}
	  
		else // No error response from command execution, go further with settings
		{
			haSendCommand(device, "set myCube22 pos", posCu_22[PosX], posCu_22[PosY], posCu_22[PosZ], response);
			haSendCommand(device, "set myCube22 size", sizeCu_22a, sizeCu_22a, sizeCu_22a, response);
			haSendCommand(device, "set myCube22 stiffness", blockStiffnessEnv1, response);
			haSendCommand(device, "set myCube22 enable", response);
		}
		/////
		if ( haSendCommand(device, "create block myCube31", response) ) 
		{
			printf("failed sending command: create block myCube31\n");
			getchar(); exit(-1);
		}
		
		if ( strstr(response, "--- ERROR:" ) ) // Error response returned from command execution
		{
			getchar(); exit(-1);
		}
	  
		else // No error response from command execution, go further with settings
		{
			haSendCommand(device, "set myCube31 pos", posCu_31[PosX], posCu_31[PosY], posCu_31[PosZ], response);
			haSendCommand(device, "set myCube31 size", sizeCu_31a, sizeCu_31a, sizeCu_31a, response);
			haSendCommand(device, "set myCube31 stiffness", blockStiffnessEnv1, response);
			haSendCommand(device, "set myCube31 enable", response);
		}
		/////
		if ( haSendCommand(device, "create block myCube32", response) ) 
		{
			printf("failed sending command: create block myCube32\n");
			getchar(); exit(-1);
		}
		
		if ( strstr(response, "--- ERROR:" ) ) // Error response returned from command execution
		{
			getchar(); exit(-1);
		}
	  
		else // No error response from command execution, go further with settings
		{
			haSendCommand(device, "set myCube32 pos", posCu_32[PosX], posCu_32[PosY], posCu_32[PosZ], response);
			haSendCommand(device, "set myCube32 size", sizeCu_32a, sizeCu_32a, sizeCu_32a, response);
			haSendCommand(device, "set myCube32 stiffness", blockStiffnessEnv1, response);
			haSendCommand(device, "set myCube32 enable", response);
		}
		/////
		if ( haSendCommand(device, "create block myCube41", response) ) 
		{
			printf("failed sending command: create block myCube41\n");
			getchar(); exit(-1);
		}
		
		if ( strstr(response, "--- ERROR:" ) ) // Error response returned from command execution
		{
			getchar(); exit(-1);
		}
	  
		else // No error response from command execution, go further with settings
		{
			haSendCommand(device, "set myCube41 pos", posCu_41[PosX], posCu_41[PosY], posCu_41[PosZ], response);
			haSendCommand(device, "set myCube41 size", sizeCu_41a, sizeCu_41a, sizeCu_41a, response);
			haSendCommand(device, "set myCube41 stiffness", blockStiffnessEnv1, response);
			haSendCommand(device, "set myCube41 enable", response);
		}
		/////
		if ( haSendCommand(device, "create block myCube42", response) ) 
		{
			printf("failed sending command: create block myCube42\n");
			getchar(); exit(-1);
		}
		
		if ( strstr(response, "--- ERROR:" ) ) // Error response returned from command execution
		{
			getchar(); exit(-1);
		}
	  
		else // No error response from command execution, go further with settings
		{
			haSendCommand(device, "set myCube42 pos", posCu_42[PosX], posCu_42[PosY], posCu_42[PosZ], response);
			haSendCommand(device, "set myCube42 size", sizeCu_42a, sizeCu_42a, sizeCu_42a, response);
			haSendCommand(device, "set myCube42 stiffness", blockStiffnessEnv1, response);
			haSendCommand(device, "set myCube42 enable", response);
		}
		
		if ( haSendCommand(device, "create block myCube412", response) ) 
		{
			printf("failed sending command: create block myCube412\n");
			getchar(); exit(-1);
		}
		
		if ( strstr(response, "--- ERROR:" ) ) // Error response returned from command execution
		{
			getchar(); exit(-1);
		}
	  
		else // No error response from command execution, go further with settings
		{
			haSendCommand(device, "set myCube412 pos", posCu_412[PosX], posCu_412[PosY], posCu_412[PosZ], response);
			haSendCommand(device, "set myCube412 size", sizeCu_412a, sizeCu_412a, sizeCu_412a, response);
			haSendCommand(device, "set myCube412 stiffness", blockStiffnessEnv1, response);
			haSendCommand(device, "set myCube412 enable", response);
		}
		/////
		if ( haSendCommand(device, "create block myCube422", response) ) 
		{
			printf("failed sending command: create block myCube422\n");
			getchar(); exit(-1);
		}
		
		if ( strstr(response, "--- ERROR:" ) ) // Error response returned from command execution
		{
			getchar(); exit(-1);
		}
	  
		else // No error response from command execution, go further with settings
		{
			haSendCommand(device, "set myCube422 pos", posCu_422[PosX], posCu_422[PosY], posCu_422[PosZ], response);
			haSendCommand(device, "set myCube422 size", sizeCu_422a, sizeCu_422a, sizeCu_422a, response);
			haSendCommand(device, "set myCube422 stiffness", blockStiffnessEnv1, response);
			haSendCommand(device, "set myCube422 enable", response);
		}
		
		if ( haSendCommand(device, "create block myCube51", response) ) 
		{
			printf("failed sending command: create block myCube51\n");
			getchar(); exit(-1);
		}
		
		if ( strstr(response, "--- ERROR:" ) ) // Error response returned from command execution
		{
			getchar(); exit(-1);
		}
	  
		else // No error response from command execution, go further with settings
		{
			haSendCommand(device, "set myCube51 pos", posCu_51[PosX], posCu_51[PosY], posCu_51[PosZ], response);
			haSendCommand(device, "set myCube51 size", sizeCu_51a, sizeCu_51a, sizeCu_51a, response);
			haSendCommand(device, "set myCube51 stiffness", blockStiffnessEnv1, response);
			haSendCommand(device, "set myCube51 enable", response);
		}
		
		
		if ( haSendCommand(device, "create biasforce myBiasForceY", response) ) 
		{
			printf ("--- ERROR: Could not send command create biasforce myBiasForceY\n");
		}
		printf("create biasforce myBiasForceY ==> %s\n", response);
		if ( strstr (response, "--- ERROR:" ) ) 
		{
			getchar();
			exit(-1);
		 }
		 else{
			printf("create biasforce myBiasForceY ==> %s\n", response);
			haSendCommand(device, "set myBiasForceY force [0.0,0.0,0.0]", response); //-6
			printf("set myBiasForceY disable ==> %s\n", response);
		}

			
		 if ( haSendCommand(device, "create biasforce myBiasForceZ", response) ) 
		 {
			 printf ("--- ERROR: Could not send command create biasforce myBiasForceZ\n");
		 }
		 printf("create biasforce myBiasForceZ ==> %s\n", response);
		 if ( strstr (response, "--- ERROR:" ) ) 
		 {
			 getchar();
			 exit(-1);
		  }
		  else{
			 printf("create biasforce myBiasForceZ ==> %s\n", response);
			
			 haSendCommand(device, "set myBiasForceZ force [0.0,0.0,0.0]", response); //10
			 printf("set myBiasForceZ disable ==> %s\n", response);
			
          }
		
	  //----------------------------------------------------
	  
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
		else
		{
			haSendCommand(device, "set myDamper dampcoef", viscous,viscous,viscous, response);
			haSendCommand(device, "set myDamper enable", response);
		}

	
		
		
		// data file initialisation
		strcpy (datafile, "./data/output-");
		strcat (datafile, subjNumber);
		strcat (datafile, "-");
		strcat (datafile, trialNumber);
		strcat (datafile, "-");
		strcat (datafile, envNumber);
		strcat (datafile, ".dat");
		
		// Start threads
		_beginthread(KbMon, 0, &bContinue);
		_beginthread(UDPrecv, 0, &bContinue);
		_beginthread(Graphics, 0, &bContinue);
		//_beginthread(collect_data, 0, &bContinue);
		
		printf("\n_______________________________________________________\n");
		printf("OPTIONS:\n");
		printf("Press any key to stop the application\n");
		printf("_______________________________________________________\n\n");

		primavolta = 0;
		StartCounter();
		start = GetMilliCount();
		block1 = true;
		block2 = true;
		block3 = true;
		block4 = true;
		block5 = true;
		
		//MAIN LOOP
		// While ESC not pressed, go on with the applications
		while ( bContinue )
		{
			gUpdateRateCounter++;

			// Get positions in m
			haSendCommand(device, "get measpos", response);
			ParseFloatVec(response, measPos.x, measPos.y, measPos.z);

			printf("%f\n", measPos.y);
			// Get velocities in m/s
			haSendCommand(device, "get measvel", response);
			ParseFloatVec(response, measVel.x, measVel.y, measVel.z);

			// Get forces in N
			haSendCommand(device, "get measforce", response);
			ParseFloatVec(response, measForce.x, measForce.y, measForce.z);

			// Send to destination IP
			hapticData[0] = measPos.x;   hapticData[1] = measPos.y;   hapticData[2] = measPos.z;
			hapticData[3] = measVel.x;   hapticData[4] = measVel.y;   hapticData[5] = measVel.z;
			hapticData[6] = measForce.x; hapticData[7] = measForce.y; hapticData[8] = measForce.z;

			virtualPos[0] = measPos.x;   virtualPos[1] = measPos.y;   virtualPos[2] = measPos.z;
			
			int ret = sendto( s, (char *)hapticData, pkt_length, 0, (sockaddr *)&destination, sizeof(destination) ); // send packet
			if(ret==SOCKET_ERROR){
				std::cout<<"ERROR\n";
				break;
			}
			
			// VISCOUS MATERIAL
			if ((measPos.y > y_11-eps)) {
				haSendCommand(device, "set myDamper enable", response);
			}else{
				haSendCommand(device, "set myDamper disable", response);
			}
			

			


			///////////////////////////////////////////////////// 1
			if (measPos.y > y_1 && block1 == true) {
				if (primavolta == 0){
					start = GetMilliCount();
					if (set_1 == 1){
						haSendCommand(device, "set myBiasForceZ enable", response);
						primavolta = 1;
						measure = 1;
					}else{
						haSendCommand(device, "set myBiasForceY enable", response);
						primavolta = 1;
						measure = 1;
					}
				}
				else{
					if (primavolta == 1 && GetMilliSpan(start) < 200){
						if (set_1 == 1){
							haSendCommand(device, "set myBiasForceZ enable", response);
							primavolta = 1;
							measure = 1;
						}else{
							haSendCommand(device, "set myBiasForceY enable", response);
							primavolta = 1;
							measure = 1;
						}
					}else{
						haSendCommand(device, "set myBiasForceY disable", response);
						haSendCommand(device, "set myBiasForceZ disable", response);
						measure = 0;
						primavolta = 0;
						block1 = false;
					}
				}
			}

///////////////////////////////////////////////////////////////// 2
			/*
			if (measPos.y > y_2 && block2 == true) {
				if (primavolta == 0){
					start = GetMilliCount();
					
					if (set_1 == 1){
						haSendCommand(device, "set myBiasForceZ enable", response);
						primavolta = 1;
						measure = 1;
					}
				}
				else{
					if (primavolta == 1 && GetMilliSpan(start) < 200){
						if (set_2 == 1){
							haSendCommand(device, "set myBiasForceZ enable", response);
							primavolta = 1;
							measure = 1;
						}
						
					}else{
						haSendCommand(device, "set myBiasForceY disable", response);
						haSendCommand(device, "set myBiasForceZ disable", response);
						measure = 0;
						primavolta = 0;
						block2 = false;
					}
				}
				
			}
			*/
			//////////////////////////////////////////////////// 3
			if (measPos.y > y_3 && block3 == true) {
				if (primavolta == 0){
					start = GetMilliCount();
					if (set_3 == 1){
						haSendCommand(device, "set myBiasForceZ enable", response);
						primavolta = 1;
						measure = 1;
					}else{
						haSendCommand(device, "set myBiasForceY enable", response);
						primavolta = 1;
						measure = 1;
					}
				}
				else{
					if (primavolta == 1 && GetMilliSpan(start) < 200){
						if (set_3 == 1){
							haSendCommand(device, "set myBiasForceZ enable", response);
							primavolta = 1;
							measure = 1;
						}else{
							haSendCommand(device, "set myBiasForceY enable", response);
							primavolta = 1;
							measure = 1;
						}
					}else{
						haSendCommand(device, "set myBiasForceY disable", response);
						haSendCommand(device, "set myBiasForceZ disable", response);
						measure = 0;
						primavolta = 0;
						block3 = false;
					}
				}
				
			}

		}
	}

	// When the application finishes, do some cleaning up
	printf("Shutting down application...\n");

	// UDP cleanup
	closesocket(s);
	WSACleanup();

	// close data file
	fclose(fp); 

	// HapticMaster cleanup
	if (device)
	{
		haSendCommand(device, "remove all", response);
		haSendCommand(device, "set state stop", response);
		haDeviceClose(device);
	}
	return 1;
}