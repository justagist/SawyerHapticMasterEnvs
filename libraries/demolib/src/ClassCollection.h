#ifndef __ClassCollection_h_
#define __ClassCollection_h_

#include <vector>
using namespace std;

#define PLANE_NEG_SIDE 0; // indicate if dot produkt of point with normal of plane < 0
#define PLANE_POS_SIDE 1; // indicate if dot produkt of point with normal of plane >= 0

class CEdge;
class CTriangle;
class COctree;

class CVertex
{
public:
	CVertex(double dX=0.0, double dY=0.0, double dZ=0.0);
	CVertex(const CVertex &v);
	CVertex(double *pCoords);
	~CVertex();
	 
	CVertex operator*(double d) const;
	void operator*=(double d);

	CVertex operator/(double d) const;
	void operator/=(double d);
	
	CVertex operator=(const CVertex &v);
	
	CVertex operator+(CVertex &v) const;
	CVertex operator-(CVertex &v);
		
	void operator+=(CVertex &v);
    void operator-=(CVertex &v);

    bool operator!=(CVertex &v);
    bool operator==(CVertex &v);

	double Length();

	double Dot(const CVertex &v);
	CVertex Cross(const CVertex &v);

	void Normalize(void);

	double Distance(CVertex &v);
	double Distance2(CVertex &v);

    double Angle(CVertex &v);

	void Write(FILE *pFile);
	void Read(FILE *pFile);

public :
	double m_dCoords[3];
	vector<CEdge *> m_vEdgePtrs;
	unsigned long int m_uliIndexNr;
	
};


class CEdge
{
public:
	CEdge();
	CEdge(CVertex *pVertex0, CVertex *pVertex1);
	~CEdge();

	void CalcBisectPlaneNormal(void);

	void Write(FILE *pFile);
	void Read(FILE *pFile);

public:
	CVertex *mp_tVertices[2];
	CTriangle *mp_tTriangles[2];

	CVertex *mp_tBisectPlaneNormal;
	CVertex *mp_tTriangleNormalMean;

	unsigned long int m_uliIndexNr;
};


class CTriangle
{
public:
	CTriangle();
	CTriangle(CVertex *pVertex0, CVertex *pVertex1, CVertex *pVertex2);
	~CTriangle();

	void CalcNormal(void);
	void CalcCenter(void);
	void CalcNormalEndPoint(void);
	void CalcEdgeNormals(void);

	void Write(FILE *pFile);
	void Read(FILE *pFile);

	bool WithinBisectPlanes(CVertex *pV);

public:

	CVertex *mp_tVertices[3];
	CVertex *mp_tNormal;
	CVertex *mp_tCenter;
	CVertex *mp_tNormalEnd;
	
	CEdge *mp_tEdges[3];
	CVertex *mp_tEdgeNormals[3];

	int m_NewIndices[3];

	unsigned long int m_uliIndexNr;
};



class CMesh
{
public:
	CMesh();
	~CMesh();

	CVertex* AddVertex(double dX, double dY, double dZ);
	CEdge* AddEdge(unsigned long int Vertex0, unsigned long int Vertex1, CTriangle *pTriangle);
	CTriangle* AddTriangle(unsigned long int Vertex0, unsigned long int Vertex1, unsigned long int Vertex2);

	void InitCube(void);
	void InitMesh(void);

	CVertex* GetClosestVertex(CVertex *pV);
	CTriangle* GetClosestTriangle(CVertex *pV);
	CTriangle* GetClosestTriangle2(CVertex *pV);

	void DumpDataStructure(void);

	void ReadTriangleFile(char *pFileName);

	void Write(char *pFileName);
	void Read(char *pFileName);
	void ClearDataStructure(void);

	void Display(bool bSmooth=true);

public:
	vector<CVertex *> m_vVertexPtrs;
	vector<CEdge *> m_vEdgePtrs;
	vector<CTriangle *> m_vTrianglePtrs;
	vector<CVertex *> m_vVertexNormalPtrs;

	vector<CVertex *> m_vDisplayVertexPtrs;

	CVertex *mp_tClosestVertex;
	double dDistanceToClosestVertex;
	CTriangle *mp_tClosestTriangle;

	bool m_bBuild;
	bool m_bEmpty;
	int m_iBuildPerc;

};

#endif // __ClassCollection_h_