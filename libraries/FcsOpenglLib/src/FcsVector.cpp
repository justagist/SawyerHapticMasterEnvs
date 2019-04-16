#include <glut.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "FcsVector.h"

const double dFact = 3.14159265 / 180.0;
 
CVector::CVector(double dX, double dY, double dZ)
{ 
	m_dCoords[0] = dX;
	m_dCoords[1] = dY;
	m_dCoords[2] = dZ;
}

CVector::CVector(const CVector &v)
{
	m_dCoords[0] = v.m_dCoords[0];
	m_dCoords[1] = v.m_dCoords[1];
	m_dCoords[2] = v.m_dCoords[2];
}

CVector::CVector(double *pCoords)
{
	m_dCoords[0] = pCoords[0];
	m_dCoords[1] = pCoords[1];
	m_dCoords[2] = pCoords[2];
}


CVector::~CVector()
{
}


CVector CVector::operator*(double d) const
{
	return CVector(d*m_dCoords[0], d*m_dCoords[1], d*m_dCoords[2]);
}


void CVector::operator*=(double d)
{
	m_dCoords[0] *= d;
	m_dCoords[1] *= d;
	m_dCoords[2] *= d;
}


CVector CVector::operator/(double d) const
{
	return CVector(m_dCoords[0]/d, m_dCoords[1]/d, m_dCoords[2]/d);
}

void CVector::operator/=(double d)
{
	m_dCoords[0] /= d;
	m_dCoords[1] /= d;
	m_dCoords[2] /= d;
}


double CVector::Dot(const CVector &v) 
{
	return (m_dCoords[0]*v.m_dCoords[0] + m_dCoords[1]*v.m_dCoords[1] + m_dCoords[2]*v.m_dCoords[2]);
}

CVector CVector::Cross(const CVector &v)
{
	return CVector(m_dCoords[1]*v.m_dCoords[2] - m_dCoords[2]*v.m_dCoords[1],
				   m_dCoords[2]*v.m_dCoords[0] - m_dCoords[0]*v.m_dCoords[2],
				   m_dCoords[0]*v.m_dCoords[1] - m_dCoords[1]*v.m_dCoords[0] );
}

CVector CVector::operator=(const CVector &v)
{
	m_dCoords[0] = v.m_dCoords[0];
	m_dCoords[1] = v.m_dCoords[1];
	m_dCoords[2] = v.m_dCoords[2];
	
   return CVector(m_dCoords[0], m_dCoords[1], m_dCoords[2]);
}


CVector CVector::operator+(CVector &v) const
{
	return CVector(m_dCoords[0]+v.m_dCoords[0], m_dCoords[1]+v.m_dCoords[1], m_dCoords[2]+v.m_dCoords[2]);
}

CVector CVector::operator-(CVector &v)
{
	return CVector(m_dCoords[0]-v.m_dCoords[0], m_dCoords[1]-v.m_dCoords[1], m_dCoords[2]-v.m_dCoords[2]);
}


void CVector::operator+=(CVector &v)
{
	m_dCoords[0] += v.m_dCoords[0];
	m_dCoords[1] += v.m_dCoords[1];
	m_dCoords[2] += v.m_dCoords[2];
}

void CVector::operator-=(CVector &v)
{
	m_dCoords[0] -= v.m_dCoords[0];
	m_dCoords[1] -= v.m_dCoords[1];
	m_dCoords[2] -= v.m_dCoords[2];
}


bool CVector::operator!=(CVector &v)
{
	return !(m_dCoords[0]==v.m_dCoords[0] && m_dCoords[1]==v.m_dCoords[1] && m_dCoords[2]==v.m_dCoords[2]);
}

bool CVector::operator==(CVector &v)
{
	return (m_dCoords[0]==v.m_dCoords[0] && m_dCoords[1]==v.m_dCoords[1] && m_dCoords[2]==v.m_dCoords[2]);
}


double CVector::Length()
{
	return sqrt(m_dCoords[0]*m_dCoords[0] + m_dCoords[1]*m_dCoords[1] + m_dCoords[2]*m_dCoords[2]);
}

void CVector::Normalize(void)
{
	
	double dLen = Length();
	
	if (dLen > 0.0) 
		operator/=(dLen);

}


double CVector::Distance(CVector &v)
{
	double dXc, dYc, dZc;

	dXc = v.m_dCoords[0] - m_dCoords[0];
	dYc = v.m_dCoords[1] - m_dCoords[1];
	dZc = v.m_dCoords[2] - m_dCoords[2];

	return sqrt(dXc*dXc + dYc*dYc + dZc*dZc);
}

double CVector::Distance2(CVector &v)
{
	double dXc, dYc, dZc;

	dXc = v.m_dCoords[0] - m_dCoords[0];
	dYc = v.m_dCoords[1] - m_dCoords[1];
	dZc = v.m_dCoords[2] - m_dCoords[2];

	return (dXc*dXc + dYc*dYc + dZc*dZc);
}

double CVector::Angle(CVector &v)
{
    double a;

    a = acos( Dot(v) / (Length()*v.Length()) );

    return a;
}

void CVector::Write(FILE *pFile)
{
}

void CVector::Read(FILE *pFile)
{
}


