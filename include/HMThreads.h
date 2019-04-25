
#ifndef _HM_THREADS_
#define _HM_THREADS_

#include <Windows.h>
#include <math.h>
#include <random>
#include <ws2tcpip.h>
#include <time.h>
#include <cstdlib>
#include <sys/timeb.h>

namespace HMThreads {

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

}

#endif