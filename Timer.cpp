#include "include/Timer.h"


Timer::Timer()
{
	PCFreq = 0.0;
	CounterStart = 0;
}


void Timer::StartCounter()
{
	LARGE_INTEGER li;
	if(!QueryPerformanceCounter(&li))
		std::cout << "QUesryPerformanceCounter failed!\n";

	PCFreq = double(li.QuadPart)/1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;

}


double Timer::GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart)/PCFreq;
}


