#include "Utils.h"

const double Pi =  3.14159265358979;
const double Pi2 = 6.28318530717958;

const double DegPerRad = 180/Pi;
const double RadPerDeg = Pi/180;


double Deg2Rad(double Deg)
{
	return Deg*RadPerDeg;
}

double Rad2Deg(double Rad)
{
	return Rad*DegPerRad;
}