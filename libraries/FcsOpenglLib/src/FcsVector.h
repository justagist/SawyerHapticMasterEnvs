#ifndef ClassCollection_h
#define ClassCollection_h

#include <vector>
using namespace std;

#define PLANE_NEG_SIDE 0; // indicate if dot produkt of point with normal of plane < 0
#define PLANE_POS_SIDE 1; // indicate if dot produkt of point with normal of plane >= 0

class CVector
{
public:
	CVector(double dX=0.0, double dY=0.0, double dZ=0.0);
	CVector(const CVector &v);
	CVector(double *pCoords);
	~CVector();
	 
	CVector operator*(double d) const;
	void operator*=(double d);

	CVector operator/(double d) const;
	void operator/=(double d);
	
	CVector operator=(const CVector &v);
	
	CVector operator+(CVector &v) const;
	CVector operator-(CVector &v);
		
	void operator+=(CVector &v);
    void operator-=(CVector &v);

    bool operator!=(CVector &v);
    bool operator==(CVector &v);

	double Length();

	double Dot(const CVector &v);
	CVector Cross(const CVector &v);

	void Normalize(void);

	double Distance(CVector &v);
	double Distance2(CVector &v);

    double Angle(CVector &v);

	void Write(FILE *pFile);
	void Read(FILE *pFile);

public :
	double m_dCoords[3];
	unsigned long int m_uliIndexNr;
	
};

#endif