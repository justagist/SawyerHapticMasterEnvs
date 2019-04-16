#include <glut.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "ClassCollection.h"

 
const double dFact = 3.14159265 / 180.0;

CVertex::CVertex(double dX, double dY, double dZ)
{
	m_dCoords[0] = dX;
	m_dCoords[1] = dY;
	m_dCoords[2] = dZ;
}
  
CVertex::CVertex(const CVertex &v) 
{
	m_dCoords[0] = v.m_dCoords[0];
	m_dCoords[1] = v.m_dCoords[1]; 
	m_dCoords[2] = v.m_dCoords[2];
}
 
CVertex::CVertex(double *pCoords)
{
	m_dCoords[0] = pCoords[0];
	m_dCoords[1] = pCoords[1];
	m_dCoords[2] = pCoords[2];
}


CVertex::~CVertex()
{
}


CVertex CVertex::operator*(double d) const
{
	return CVertex(d*m_dCoords[0], d*m_dCoords[1], d*m_dCoords[2]);
}


void CVertex::operator*=(double d)
{
	m_dCoords[0] *= d;
	m_dCoords[1] *= d;
	m_dCoords[2] *= d;
}


CVertex CVertex::operator/(double d) const
{
	return CVertex(m_dCoords[0]/d, m_dCoords[1]/d, m_dCoords[2]/d);
}

void CVertex::operator/=(double d)
{
	m_dCoords[0] /= d;
	m_dCoords[1] /= d;
	m_dCoords[2] /= d;
}


double CVertex::Dot(const CVertex &v) 
{
	return (m_dCoords[0]*v.m_dCoords[0] + m_dCoords[1]*v.m_dCoords[1] + m_dCoords[2]*v.m_dCoords[2]);
}

CVertex CVertex::Cross(const CVertex &v)
{
	return CVertex(m_dCoords[1]*v.m_dCoords[2] - m_dCoords[2]*v.m_dCoords[1],
				   m_dCoords[2]*v.m_dCoords[0] - m_dCoords[0]*v.m_dCoords[2],
				   m_dCoords[0]*v.m_dCoords[1] - m_dCoords[1]*v.m_dCoords[0] );
}

CVertex CVertex::operator=(const CVertex &v)
{
	m_dCoords[0] = v.m_dCoords[0];
	m_dCoords[1] = v.m_dCoords[1];
	m_dCoords[2] = v.m_dCoords[2];
	
   return CVertex(m_dCoords[0], m_dCoords[1], m_dCoords[2]);
}


CVertex CVertex::operator+(CVertex &v) const
{
	return CVertex(m_dCoords[0]+v.m_dCoords[0], m_dCoords[1]+v.m_dCoords[1], m_dCoords[2]+v.m_dCoords[2]);
}

CVertex CVertex::operator-(CVertex &v)
{
	return CVertex(m_dCoords[0]-v.m_dCoords[0], m_dCoords[1]-v.m_dCoords[1], m_dCoords[2]-v.m_dCoords[2]);
}


void CVertex::operator+=(CVertex &v)
{
	m_dCoords[0] += v.m_dCoords[0];
	m_dCoords[1] += v.m_dCoords[1];
	m_dCoords[2] += v.m_dCoords[2];
}

void CVertex::operator-=(CVertex &v)
{
	m_dCoords[0] -= v.m_dCoords[0];
	m_dCoords[1] -= v.m_dCoords[1];
	m_dCoords[2] -= v.m_dCoords[2];
}


bool CVertex::operator!=(CVertex &v)
{
	return !(m_dCoords[0]==v.m_dCoords[0] && m_dCoords[1]==v.m_dCoords[1] && m_dCoords[2]==v.m_dCoords[2]);
}

bool CVertex::operator==(CVertex &v)
{
	return (m_dCoords[0]==v.m_dCoords[0] && m_dCoords[1]==v.m_dCoords[1] && m_dCoords[2]==v.m_dCoords[2]);
}


double CVertex::Length()
{
	return sqrt(m_dCoords[0]*m_dCoords[0] + m_dCoords[1]*m_dCoords[1] + m_dCoords[2]*m_dCoords[2]);
}

void CVertex::Normalize(void)
{
	
	double dLen = Length();
	
	if (dLen > 0.0) 
		operator/=(dLen);

}


double CVertex::Distance(CVertex &v)
{
	double dXc, dYc, dZc;

	dXc = v.m_dCoords[0] - m_dCoords[0];
	dYc = v.m_dCoords[1] - m_dCoords[1];
	dZc = v.m_dCoords[2] - m_dCoords[2];

	return sqrt(dXc*dXc + dYc*dYc + dZc*dZc);
}

double CVertex::Distance2(CVertex &v)
{
	double dXc, dYc, dZc;

	dXc = v.m_dCoords[0] - m_dCoords[0];
	dYc = v.m_dCoords[1] - m_dCoords[1];
	dZc = v.m_dCoords[2] - m_dCoords[2];

	return (dXc*dXc + dYc*dYc + dZc*dZc);
}

double CVertex::Angle(CVertex &v)
{
    double a;

    a = acos( Dot(v) / (Length()*v.Length()) );

    return a;
}

void CVertex::Write(FILE *pFile)
{
}

void CVertex::Read(FILE *pFile)
{
}


CEdge::CEdge()
{
	mp_tVertices[0] = NULL;
	mp_tVertices[1] = NULL;

	mp_tTriangles[0] = NULL;
	mp_tTriangles[1] = NULL;

	mp_tBisectPlaneNormal = new CVertex();
	mp_tTriangleNormalMean = new CVertex();
}

CEdge::CEdge(CVertex *pVertex0, CVertex *pVertex1)
{
	mp_tVertices[0] = pVertex0;
	mp_tVertices[1] = pVertex1;

	mp_tTriangles[0] = NULL;
	mp_tTriangles[1] = NULL;

	mp_tBisectPlaneNormal = new CVertex();
	mp_tTriangleNormalMean = new CVertex();
}

CEdge::~CEdge()
{
	/*
	
	if(mp_tVertices[0] != NULL)
		delete mp_tVertices[0];

	if(mp_tVertices[1] != NULL)
		delete mp_tVertices[1];

	if(mp_tTriangles[0] != NULL)
		delete mp_tTriangles[0];

	if(mp_tTriangles[1] != NULL)
		delete mp_tTriangles[1];
		*/

	if(mp_tBisectPlaneNormal != NULL)
		delete mp_tBisectPlaneNormal;

	if(mp_tTriangleNormalMean != NULL)
		delete mp_tTriangleNormalMean;
	
}

void CEdge::CalcBisectPlaneNormal(void)
{
	CTriangle *pTemp;
	CVertex *pV = NULL;
	int i;

	// First test if the edge has two triangles or just one, in which case the egde is an edge of the mesh
	if((mp_tTriangles[0] != NULL) && (mp_tTriangles[1] != NULL))
		mp_tTriangleNormalMean = &(*(mp_tTriangles[0]->mp_tNormal) + *(mp_tTriangles[1]->mp_tNormal));
	else if (mp_tTriangles[0] != NULL)
		mp_tTriangleNormalMean = mp_tTriangles[0]->mp_tNormal;
	else if (mp_tTriangles[1] != NULL)
		mp_tTriangleNormalMean = mp_tTriangles[1]->mp_tNormal;

	mp_tTriangleNormalMean->Normalize();

	// Determine the Normal vector of the bisect plane
	*mp_tBisectPlaneNormal = (*mp_tVertices[1] - *mp_tVertices[0]).Cross(*mp_tTriangleNormalMean);
	mp_tBisectPlaneNormal->Normalize();

	
	// Decide which triangle is on which side of the bisect plane
	// first find the vertex of the triangle NOT in the edge
	// if the dot product of this vertex with the bisect plane normal >= 0.0
	// it is on the 'positive' side of the plane, put it in the second triangle pointer
	for(i=0; i<3; i++)
		if ((mp_tTriangles[0]->mp_tVertices[i] != mp_tVertices[0]) && 
			 (mp_tTriangles[0]->mp_tVertices[i] != mp_tVertices[1]))
			 pV = mp_tTriangles[0]->mp_tVertices[i];

	if (pV != NULL)
		if(mp_tBisectPlaneNormal->Dot(*pV - *mp_tVertices[0]) > 0.0) // >= 0.0
		{
			//switch triangle ptrs;
			pTemp = mp_tTriangles[0];
			mp_tTriangles[0] = mp_tTriangles[1];
			mp_tTriangles[1] = pTemp;
		}

}


void CEdge::Write(FILE *pFile)
{
}

void CEdge::Read(FILE *pFile)
{
}


CTriangle::CTriangle()
{
	mp_tVertices[0] = NULL;
	mp_tVertices[1] = NULL;
	mp_tVertices[2] = NULL;

	mp_tCenter = NULL;
	mp_tNormal = NULL;
	mp_tNormalEnd = NULL;
	
	mp_tEdgeNormals[0] = NULL;
	mp_tEdgeNormals[1] = NULL;
	mp_tEdgeNormals[2] = NULL;

	m_NewIndices[0] = 0;
	m_NewIndices[1] = 1;
	m_NewIndices[2] = 2;
}

CTriangle::CTriangle(CVertex *pVertex0, CVertex *pVertex1, CVertex *pVertex2)
{
	mp_tVertices[0] = pVertex0;
	mp_tVertices[1] = pVertex1;
	mp_tVertices[2] = pVertex2;

	mp_tCenter = new CVertex();
	mp_tNormal = new CVertex();
	mp_tNormalEnd = new CVertex();

	mp_tEdgeNormals[0] = new CVertex();
	mp_tEdgeNormals[1] = new CVertex();
	mp_tEdgeNormals[2] = new CVertex();

	CalcCenter();
	CalcNormal();
	CalcNormalEndPoint();
	CalcEdgeNormals();

	m_NewIndices[0] = 0;
	m_NewIndices[1] = 1;
	m_NewIndices[2] = 2;
}

CTriangle::~CTriangle()
{
	
	/*
	if(mp_tVertices[0] != NULL)
		delete mp_tVertices[0];

	if(mp_tVertices[1] != NULL)
		delete mp_tVertices[1];

	if(mp_tVertices[2] != NULL)
		delete mp_tVertices[2];
	*/

	if(mp_tCenter != NULL)
		delete mp_tCenter;

	if(mp_tNormal != NULL)
		delete mp_tNormal;

	if(mp_tNormalEnd != NULL)
		delete mp_tNormalEnd;

	if(mp_tEdgeNormals[0] != NULL)
		delete mp_tEdgeNormals[0];

	if(mp_tEdgeNormals[1] != NULL)
		delete mp_tEdgeNormals[1];

	if(mp_tEdgeNormals[2] != NULL)
		delete mp_tEdgeNormals[2];

	
}

void CTriangle::CalcCenter(void)
{
	if (mp_tCenter != NULL)
	{
		*mp_tCenter = (*mp_tVertices[0] + *mp_tVertices[1] + *mp_tVertices[2]) / 3.0;
	}
}

void CTriangle::CalcNormal(void)
{

	CVertex v, w;

	v = *mp_tVertices[1] - *mp_tVertices[0];
	w = *mp_tVertices[2] - *mp_tVertices[0];

	*mp_tNormal = v.Cross(w);
	mp_tNormal->Normalize();
}

void CTriangle::CalcNormalEndPoint(void)
{
	if(mp_tNormalEnd != NULL)
		*mp_tNormalEnd = *mp_tCenter + (*mp_tNormal)*(0.01);

}

void CTriangle::CalcEdgeNormals(void)
{
	
	if(mp_tEdgeNormals[0] != NULL)
	{
		*mp_tEdgeNormals[0] = mp_tNormal->Cross(*mp_tVertices[1] - *mp_tVertices[0]);
	}

	
	if(mp_tEdgeNormals[1] != NULL)
	{
		*mp_tEdgeNormals[1] = mp_tNormal->Cross(*mp_tVertices[2] - *mp_tVertices[1]);
	}

	
	if(mp_tEdgeNormals[2] != NULL)
	{
		*mp_tEdgeNormals[2] = mp_tNormal->Cross(*mp_tVertices[0] - *mp_tVertices[2]);
	}
}


void CTriangle::Write(FILE *pFile)
{
}

void CTriangle::Read(FILE *pFile)
{
}


bool CTriangle::WithinBisectPlanes(CVertex *pV)
{
	bool bResult=true;
	int i, iIndexNr;
	double dProjection;
	

	for(i=0; i<3; i++)
	{
		dProjection = mp_tEdges[i]->mp_tBisectPlaneNormal->Dot(*pV - *(mp_tEdges[i]->mp_tVertices[0]));
		if (dProjection <= 0.0)
		{
			iIndexNr = mp_tEdges[i]->mp_tTriangles[0]->m_uliIndexNr;
		}
		else
		{
			iIndexNr = mp_tEdges[i]->mp_tTriangles[0]->m_uliIndexNr;
		}


		if(iIndexNr != m_uliIndexNr)
		{
			bResult = false;
			break;
		}

	}
	

	return bResult;
}

CMesh::CMesh()
{
	mp_tClosestVertex = NULL;
	mp_tClosestTriangle = NULL;

	m_bBuild = false;
	m_bEmpty = true;
	m_iBuildPerc = 100;
	
	//ReadTriangleFile("data_face.txt");
	//ReadTriangleFile("data_torso.txt");
	//ReadTriangleFile("data_teapot.txt");
	//ReadTriangleFile("data_knot.txt");
	//ReadTriangleFile("data_kies.txt");
	//ReadTriangleFile("data_keverbody.txt");
	//ReadTriangleFile("data_skull.txt");
	//ReadTriangleFile("data_porsche.txt");
	//ReadTriangleFile("data_sculpt3.txt");
	//ReadTriangleFile("data_lamborghini.txt");
	//ReadTriangleFile("data_sculpt_3.txt");
	//ReadTriangleFile("data_sculpt5.txt");
	//ReadTriangleFile("data_van.txt");
	//ReadTriangleFile("data_face3.txt");
	//ReadTriangleFile("data_spaceshuttle.txt");
	//ReadTriangleFile("data_duck.txt");
    //ReadTriangleFile("data_moteur.txt");

	//InitMesh();

	//DumpDataStructure();

	//Write("moteur.tri");

	//Read("porsche.tri");
	//Read("torso.tri");
	//Read("beetle.tri");
	//Read("face.tri");
	//Read("kies.tri");
	//Read("cube.tri");
	//Read("spaceshuttle.tri");

}

CMesh::~CMesh()
{
	this->ClearDataStructure();
}

void CMesh::Display(bool bSmooth)
{
	unsigned long int i, iMax;
	
	if (!m_bEmpty) 
	{
        /*
		if( (m_bBuild) && (m_iBuildPerc < 100) && (m_iBuildPerc >= 0) )
		{
			iMax = (m_vTrianglePtrs.size() / 100) * m_iBuildPerc;
		}
		else
		{
			iMax = m_vTrianglePtrs.size();
		}
        */

        iMax = m_vTrianglePtrs.size();

		glBegin(GL_TRIANGLES);
		for (i=0; i<iMax; i++)
		{	
			if(bSmooth)
			{
				glNormal3dv(m_vVertexNormalPtrs[m_vTrianglePtrs[i]->mp_tVertices[0]->m_uliIndexNr]->m_dCoords );
				glVertex3dv(m_vTrianglePtrs[i]->mp_tVertices[0]->m_dCoords);
				
				glNormal3dv(m_vVertexNormalPtrs[m_vTrianglePtrs[i]->mp_tVertices[1]->m_uliIndexNr]->m_dCoords );
				glVertex3dv(m_vTrianglePtrs[i]->mp_tVertices[1]->m_dCoords);
				
				glNormal3dv(m_vVertexNormalPtrs[m_vTrianglePtrs[i]->mp_tVertices[2]->m_uliIndexNr]->m_dCoords );
				glVertex3dv(m_vTrianglePtrs[i]->mp_tVertices[2]->m_dCoords);

			}
			else
			{
				glNormal3dv(m_vTrianglePtrs[i]->mp_tNormal->m_dCoords);
				glVertex3dv(m_vTrianglePtrs[i]->mp_tVertices[0]->m_dCoords);
				glVertex3dv(m_vTrianglePtrs[i]->mp_tVertices[1]->m_dCoords);
				glVertex3dv(m_vTrianglePtrs[i]->mp_tVertices[2]->m_dCoords);
			}
		}
		glEnd();
	}
}


void CMesh::InitMesh(void)
{
	int i, j;

	for(i=0; i<m_vEdgePtrs.size(); i++)
		m_vEdgePtrs[i]->CalcBisectPlaneNormal();

// Calculate Vertex Normals Unweighted
/*
	for(i=0; i<m_vTrianglePtrs.size(); i++)
	{
		for(j=0; j<3; j++)
		{
			*m_vVertexNormalPtrs[m_vTrianglePtrs[i]->mp_tVertices[j]->m_uliIndexNr] += *m_vTrianglePtrs[i]->mp_tNormal;		
		}
	}
*/
// Calculate Vertex Normals Weighted with angles of participating triangles

	CVertex PA, PB;
	int iA, iB;
	double dPhi;
	
	for(i=0; i<m_vTrianglePtrs.size(); i++)
	{
		for(j=0; j<3; j++)
		{
			iA = (j+1) % 3;
			iB = (j+2) % 3;

			PA = *m_vTrianglePtrs[i]->mp_tVertices[iA] - *m_vTrianglePtrs[i]->mp_tVertices[j];
			PB = *m_vTrianglePtrs[i]->mp_tVertices[iB] - *m_vTrianglePtrs[i]->mp_tVertices[j];

			dPhi = acos(PA.Dot(PB));

			*m_vVertexNormalPtrs[m_vTrianglePtrs[i]->mp_tVertices[j]->m_uliIndexNr] += *m_vTrianglePtrs[i]->mp_tNormal * dPhi;
		}
	}


	for(i=0; i<m_vVertexNormalPtrs.size(); i++)
		m_vVertexNormalPtrs[i]->Normalize();

}


CVertex* CMesh::AddVertex(double dX, double dY, double dZ)
{
	CVertex *pNewVertex = NULL;
	//CVertex *pNewDisplayVertex = NULL;
	CVertex *pNewNormal = NULL;
	
	pNewVertex = new CVertex(dX, dY, dZ);
	//pNewDisplayVertex = new CVertex(dX, dY, dZ);
	pNewNormal = new CVertex();

	pNewVertex->m_uliIndexNr = m_vVertexPtrs.size();
	//pNewDisplayVertex->m_uliIndexNr = m_vDisplayVertexPtrs.size();
	pNewNormal->m_uliIndexNr = m_vVertexNormalPtrs.size();

	if(pNewVertex != NULL)
	{
		m_vVertexPtrs.push_back(pNewVertex);
	}
	else
	{
		int i =100;
	}

	//if(pNewDisplayVertex != NULL)
	//	m_vDisplayVertexPtrs.push_back(pNewDisplayVertex);

	if(pNewNormal != NULL)
	{
		m_vVertexNormalPtrs.push_back(pNewNormal);
	}
	else
	{
		int j = 10;
	}


	return pNewVertex;
}

CEdge* CMesh::AddEdge(unsigned long int Vertex0, unsigned long int Vertex1, CTriangle *pTriangle)
{
	CEdge *pEdge = NULL;
	unsigned long int i;
	bool bFound = false;

	// First find out if the Edge already exists (because edges can be shared by triangles)
	// Do this by parsing through the Edges collection.
	// Then check for every edge if the two end vertices are the same as the given vertices Vertex0 & Vertex1
	// If an edge is found skip the break the search
	for(i=0; i<m_vEdgePtrs.size(); i++)
	{
		if( ((m_vEdgePtrs[i]->mp_tVertices[0] == m_vVertexPtrs[Vertex0]) && (m_vEdgePtrs[i]->mp_tVertices[1] == m_vVertexPtrs[Vertex1])) ||
			 ((m_vEdgePtrs[i]->mp_tVertices[0] == m_vVertexPtrs[Vertex1]) && (m_vEdgePtrs[i]->mp_tVertices[1] == m_vVertexPtrs[Vertex0])) )
		{
			bFound = true;
			break;
		}
	}

	if(bFound)
	{
		// If an edge is found add the triangle two the triangles collection of the edge
		// This is a very small collection consisting of 1 or two triangles
		// Add the triangle to the second position
		// The first should already be filled, because the edge exists
		m_vEdgePtrs[i]->mp_tTriangles[1] = pTriangle;
		pEdge = m_vEdgePtrs[i];
	}
	else
	{
		// If the no edge is found which consists of the two vertices Vertex0 & Vertex1
		// Create a new Edge object withe Vertex0 & Vertex1
		// 
		pEdge = new CEdge(m_vVertexPtrs[Vertex0], m_vVertexPtrs[Vertex1]);
		

		if(pEdge != NULL)
		{
			// If the Edge creation object went well, add the triangle to the first 
			// position of the edge's triangle collection
			// Later on in the CalcBisectPlaneNormal function, which is called
			// during mesh initialization, it is possibel that the two triangles 
			// belonging to this Edge are swapped depending on the BisectPLane normal vector

			pEdge->mp_tTriangles[0] = pTriangle;
			pEdge->m_uliIndexNr = m_vEdgePtrs.size();
			
			// Next push the newly created Edge object to the Edge collection of the Mesh object
			m_vEdgePtrs.push_back(pEdge);

			// The last thing to do is to add the Edge to the Edge collection of both vertices
			// so later on we can easily find which edges belong to the vertices
			m_vVertexPtrs[Vertex0]->m_vEdgePtrs.push_back(pEdge);
			m_vVertexPtrs[Vertex1]->m_vEdgePtrs.push_back(pEdge);
		}
	}

	return pEdge;
}

CTriangle* CMesh::AddTriangle(unsigned long int Vertex0, unsigned long int Vertex1, unsigned long int Vertex2)
{
	CTriangle *pNewTriangle = NULL;

	// Create a new traingle object with the given vertices Vertex0, Vertex1 & Vertex2
	pNewTriangle = new CTriangle(m_vVertexPtrs[Vertex0], m_vVertexPtrs[Vertex1], m_vVertexPtrs[Vertex2]);
	pNewTriangle->m_uliIndexNr = m_vTrianglePtrs.size();
	
	if(pNewTriangle != NULL)
	{
		// If Triangle Object creation went well,
		// push the new triangle on the Mesh triangle collection
		// and add three edges, one for each triangle edge
		m_vTrianglePtrs.push_back(pNewTriangle);
		pNewTriangle->mp_tEdges[0] = AddEdge(Vertex0, Vertex1, pNewTriangle);
		pNewTriangle->mp_tEdges[1] = AddEdge(Vertex1, Vertex2, pNewTriangle);
		pNewTriangle->mp_tEdges[2] = AddEdge(Vertex2, Vertex0, pNewTriangle);

	}

	return pNewTriangle;
}



CVertex* CMesh::GetClosestVertex(CVertex *pV)
{
	double dDist = 0.0;
	double dCurrentDist = 10.0;
	unsigned long int i, uliClosestVertex;
	CVertex *pReturn = NULL;

	if( (m_iBuildPerc == 100) && (m_vVertexPtrs.size() > 0) )
	{
		for(i=0; i<m_vVertexPtrs.size(); i++)
		{
			dDist = m_vVertexPtrs[i]->Distance2(*pV);
			if(dDist < dCurrentDist)
			{
				dCurrentDist = dDist;
				uliClosestVertex = i;
			}
		}

		pReturn = m_vVertexPtrs[uliClosestVertex];
	}

	return pReturn;
}

CTriangle* CMesh::GetClosestTriangle(CVertex *pV)
{
	unsigned long int i, j, uliNrEdges;
	vector<CTriangle *> vTrianglePtrs;
	CTriangle *pTri, *pTmp;
	CVertex tAE, tAZ;
	
	bool bFound = false;
	bool bE[3];
	bool bBreak = false;
	
	double dProjection;
	double dDist = -1000.0;
	double dTmp;
	
	int iIterationCount = 0;

	mp_tClosestTriangle = NULL;

	if((m_bEmpty == false ) && (m_iBuildPerc == 100) )
		mp_tClosestVertex = GetClosestVertex(pV);

	if(mp_tClosestVertex != NULL)
	{
		do
		{
			vTrianglePtrs.clear();
			bBreak = false;

			uliNrEdges = mp_tClosestVertex->m_vEdgePtrs.size();

			for(i=0; i<uliNrEdges; i++)
			{
				dProjection = mp_tClosestVertex->m_vEdgePtrs[i]->mp_tBisectPlaneNormal->Dot(*pV - *(mp_tClosestVertex->m_vEdgePtrs[i]->mp_tVertices[0]));

				if (dProjection <= 0.0)
				{
					pTmp = mp_tClosestVertex->m_vEdgePtrs[i]->mp_tTriangles[0];
					vTrianglePtrs.push_back(mp_tClosestVertex->m_vEdgePtrs[i]->mp_tTriangles[0]);
				}
				else
				{
					pTmp = mp_tClosestVertex->m_vEdgePtrs[i]->mp_tTriangles[0];
					vTrianglePtrs.push_back(mp_tClosestVertex->m_vEdgePtrs[i]->mp_tTriangles[1]);
				}
			}

			
			dDist = -1000.0;
			for(i=0; i<uliNrEdges; i++)
			{
				for(j=i+1; j<uliNrEdges; j++)
				{
					pTmp = vTrianglePtrs[i];
					if((vTrianglePtrs[i] == vTrianglePtrs[j]) && (vTrianglePtrs[i] != NULL))
					{
						tAE = *pV - *mp_tClosestVertex;
						tAE.Normalize();

						tAZ = *vTrianglePtrs[i]->mp_tCenter - *mp_tClosestVertex;
						tAZ.Normalize();

						dTmp = tAE.Dot(tAZ);
						if( _isnan(dTmp) )
							printf("ÏSNAN\n");

						if(  dTmp > dDist )
						{
						  mp_tClosestTriangle = vTrianglePtrs[i];
						  dDist = dTmp;

						}
					}
				}
			}
			
			/*
			for(i=0; i<uliNrEdges; i++)
			{
				for(j=i+1; j<uliNrEdges; j++)
				{
					if((vTrianglePtrs[i] == vTrianglePtrs[j]) && (vTrianglePtrs[i] != NULL))
					{
						mp_tClosestTriangle = vTrianglePtrs[i];
						bBreak = true; //Stop het zoeken en vergelijken als we twee dezelfde pointers vinden
						break;
					}
				}
				
				if(bBreak)
					break;
			}
			*/


			if( mp_tClosestTriangle != NULL )
			{
				// Now test the 3rd edge of the triangle we think is the right one
				// We do this by first finding this 3rd edge and checking if the EE
				// lies on the triangle side of the bisectplane of this 3rd edge
				
				bE[0] = false; // bool to indicate if edge 0 is already tested
				bE[1] = false; // bool to indicate if edge 1 is already tested
				bE[2] = false; // bool to indicate if edge 2 is already tested

				// Check the edge ptrs of the triangle against the edge ptrs of the closest vertex
				// This way we should find that two of the triangle edge ptrs  have a match
				// These edges are marked true by setting the bE[?] var
				// One of the bE[?] vars is left false, this is the 3rd edge
				for(i=0; i<3; i++)
				{
					for(j=0; j<mp_tClosestVertex->m_vEdgePtrs.size(); j++)
					{
						if(mp_tClosestTriangle->mp_tEdges[i] == mp_tClosestVertex->m_vEdgePtrs[j])
						{
							bE[i] = true;
							break;
						}
					}
				}

				// At this point the edges are marked if already tested
				// walk through them, if one is marked false
				// find out if the EE is on the same side of the bisectplane as the current closest triangle is
				for(i=0; i<3; i++)
				{
					if(bE[i] == false)
					{
						dProjection = mp_tClosestTriangle->mp_tEdges[i]->mp_tBisectPlaneNormal->Dot(*pV - *(mp_tClosestTriangle->mp_tEdges[i]->mp_tVertices[0]));
						j=i;
						break;
					}
				}

				// Now we know de projection of the EE on the Bisect plane normal
				// Pick one of either triangles depending on the sign of the projection
				if(dProjection <= 0.0)
					pTri = mp_tClosestTriangle->mp_tEdges[j]->mp_tTriangles[0];
				else
					pTri = mp_tClosestTriangle->mp_tEdges[j]->mp_tTriangles[1];

				// If the found triangle is NULL (we were on an boundary of the model)
				// or the found triangle matches the current closest triangle
				// were done here, we found the closest triangle
				if( (pTri == NULL) || (pTri == mp_tClosestTriangle) )
				{
					mp_tClosestTriangle = pTri;
					bFound = true;
				}
				else
				{
					// If the EE was on the other side of the bisectplane than we make
					// a switch assuming that the closest triangle is the one on the 
					// other side of the bisect plane
					// Now we take this tiangle and say this is the current closest triangle
					// Execpt now we have to make sure and test the EE against the other
					// two bisect planes as well
					// We do this swicth over trick atmost 6 times
					if ( mp_tClosestTriangle->mp_tEdges[j]->mp_tVertices[0]->Distance2(*pV) < 
											mp_tClosestTriangle->mp_tEdges[j]->mp_tVertices[1]->Distance2(*pV) )
						mp_tClosestVertex = mp_tClosestTriangle->mp_tEdges[j]->mp_tVertices[0];
					else
						mp_tClosestVertex = mp_tClosestTriangle->mp_tEdges[j]->mp_tVertices[1];

					iIterationCount++;
				}
			}
			else
				bFound = true;
		}
		while ((!bFound) && (iIterationCount < 6)) ;
	}
	

	return mp_tClosestTriangle;
}



CTriangle* CMesh::GetClosestTriangle2(CVertex *pV)
{
	unsigned long int i, j, uliNrEdges;
	vector<CTriangle *> vTrianglePtrs;
	CTriangle *pTri, *pTmp;
	CVertex tAE, tAZ;
	
	bool bFound = false;
	bool bE[3];
	bool bBreak = false;
	
	double dProjection;
	double dDist = -1000.0;
	double dTmp;
	
	int iIterationCount = 0;

	mp_tClosestTriangle = NULL;
	mp_tClosestVertex = GetClosestVertex(pV);

	if(mp_tClosestVertex != NULL)
	{
		do
		{
			vTrianglePtrs.clear();
			bBreak = false;

			uliNrEdges = mp_tClosestVertex->m_vEdgePtrs.size();

			for(i=0; i<uliNrEdges; i++)
			{
				dProjection = mp_tClosestVertex->m_vEdgePtrs[i]->mp_tBisectPlaneNormal->Dot(*pV - *(mp_tClosestVertex->m_vEdgePtrs[i]->mp_tVertices[0]));

				if (dProjection <= 0.0)
				{
					pTmp = mp_tClosestVertex->m_vEdgePtrs[i]->mp_tTriangles[0];
					vTrianglePtrs.push_back(mp_tClosestVertex->m_vEdgePtrs[i]->mp_tTriangles[0]);
				}
				else
				{
					pTmp = mp_tClosestVertex->m_vEdgePtrs[i]->mp_tTriangles[0];
					vTrianglePtrs.push_back(mp_tClosestVertex->m_vEdgePtrs[i]->mp_tTriangles[1]);
				}
			}

			
			dDist = -1000.0;
			for(i=0; i<uliNrEdges; i++)
			{
				for(j=i+1; j<uliNrEdges; j++)
				{
					pTmp = vTrianglePtrs[i];
					if((vTrianglePtrs[i] == vTrianglePtrs[j]) && (vTrianglePtrs[i] != NULL))
					{
						tAE = *pV - *mp_tClosestVertex;
						tAE.Normalize();

						tAZ = *vTrianglePtrs[i]->mp_tCenter - *mp_tClosestVertex;
						tAZ.Normalize();

						dTmp = tAE.Dot(tAZ);
						if( _isnan(dTmp) )
							printf("ÏSNAN\n");

						if(  dTmp > dDist )
						{
						  mp_tClosestTriangle = vTrianglePtrs[i];
						  dDist = dTmp;

						}
					}
				}
			}
			
			/*
			for(i=0; i<uliNrEdges; i++)
			{
				for(j=i+1; j<uliNrEdges; j++)
				{
					if((vTrianglePtrs[i] == vTrianglePtrs[j]) && (vTrianglePtrs[i] != NULL))
					{
						mp_tClosestTriangle = vTrianglePtrs[i];
						bBreak = true; //Stop het zoeken en vergelijken als we twee dezelfde pointers vinden
						break;
					}
				}
				
				if(bBreak)
					break;
			}
			*/


			// Check the third bisect plane
			if( mp_tClosestTriangle != NULL )
			{
				bE[0] = false;
				bE[1] = false;
				bE[2] = false;


				// First mark the bisect planes already tested
				for(i=0; i<3; i++)
				{
					for(j=0; j<mp_tClosestVertex->m_vEdgePtrs.size(); j++)
					{
						if(mp_tClosestTriangle->mp_tEdges[i] == mp_tClosestVertex->m_vEdgePtrs[j])
						{
							bE[i] = true;
							break;
						}
					}
				}

				// Find the third edge and thus the third bisect plane we have to check
				for(i=0; i<3; i++)
				{
					if(bE[i] == false)
					{
						// Calc the projection, this determines the side of the edge the trinagle lies on
						dProjection = mp_tClosestTriangle->mp_tEdges[i]->mp_tBisectPlaneNormal->Dot(*pV - *(mp_tClosestTriangle->mp_tEdges[i]->mp_tVertices[0]));
						j=i;
						break;
					}
				}

				
				// This is the first attempt to iterate through some neighbouring triangles
				// To make sure we've got the right one
				// If the triangle is not found a switch is made to another vertex of the edge
				// and another iteration is made
				
				// j = the number of the edge/bisectplane we checking now
				if(dProjection <= 0.0)
					pTri = mp_tClosestTriangle->mp_tEdges[j]->mp_tTriangles[0];
				else
					pTri = mp_tClosestTriangle->mp_tEdges[j]->mp_tTriangles[1];

				if( (pTri == NULL) || (pTri == mp_tClosestTriangle) )
				{
					mp_tClosestTriangle = pTri;
					bFound = true;
				}
				else
				{
					if ( mp_tClosestTriangle->mp_tEdges[j]->mp_tVertices[0]->Distance2(*pV) < 
											mp_tClosestTriangle->mp_tEdges[j]->mp_tVertices[1]->Distance2(*pV) )
						mp_tClosestVertex = mp_tClosestTriangle->mp_tEdges[j]->mp_tVertices[0];
					else
						mp_tClosestVertex = mp_tClosestTriangle->mp_tEdges[j]->mp_tVertices[1];

					iIterationCount++;
				}
			}
			else
				bFound = true;
		}
		while ((!bFound) && (iIterationCount < 6)) ;
	}
	

	return mp_tClosestTriangle;
}



void CMesh::DumpDataStructure(void)
{
	FILE *fp;
	int i, j;

	fp = fopen("MeshData.txt", "w");

	if(fp != NULL)
	{
		fprintf(fp, "Vertices :\n");
		for(i=0; i<m_vVertexPtrs.size(); i++)
		{
			fprintf(fp, "%2d : x:=%6.3f; y:=%6.3f; z:=%6.3f;  ", m_vVertexPtrs[i]->m_uliIndexNr, m_vVertexPtrs[i]->m_dCoords[0], m_vVertexPtrs[i]->m_dCoords[1], m_vVertexPtrs[i]->m_dCoords[2]);
			for(j=0; j<m_vVertexPtrs[i]->m_vEdgePtrs.size(); j++)
				fprintf(fp, "E%2d:=%2d  ", j, m_vVertexPtrs[i]->m_vEdgePtrs[j]->m_uliIndexNr);
			fprintf(fp, "\n");
		}


		fprintf(fp, "\n");
		fprintf(fp, "Edges :\n");
		for(i=0; i<m_vEdgePtrs.size(); i++)
		{
			fprintf(fp, "%2d : V0:=%2d; V1:=%2d; ",  m_vEdgePtrs[i]->m_uliIndexNr, m_vEdgePtrs[i]->mp_tVertices[0]->m_uliIndexNr, m_vEdgePtrs[i]->mp_tVertices[1]->m_uliIndexNr);
			
			if(m_vEdgePtrs[i]->mp_tTriangles[0] != NULL)
				fprintf(fp, "T0;=%2d; ", m_vEdgePtrs[i]->mp_tTriangles[0]->m_uliIndexNr);
			else
				fprintf(fp, "T0:= X; ");

				
			if(m_vEdgePtrs[i]->mp_tTriangles[1] != NULL)
				fprintf(fp, "T1;=%2d; ", m_vEdgePtrs[i]->mp_tTriangles[1]->m_uliIndexNr);
			else
				fprintf(fp, "T1:= X; ");

			fprintf(fp, "BisectNorm:=(%6.3f, %6.3f, %6.3f)\n", m_vEdgePtrs[i]->mp_tBisectPlaneNormal->m_dCoords[0], m_vEdgePtrs[i]->mp_tBisectPlaneNormal->m_dCoords[1], m_vEdgePtrs[i]->mp_tBisectPlaneNormal->m_dCoords[2]);
		}


		fprintf(fp, "\n");
		fprintf(fp, "Triangles :\n");
		for(i=0; i<m_vTrianglePtrs.size(); i++)
		{
			fprintf(fp, "%2d : V0:=%2d; V1:=%2d; V2:=%2d; ", m_vTrianglePtrs[i]->m_uliIndexNr, m_vTrianglePtrs[i]->mp_tVertices[0]->m_uliIndexNr, m_vTrianglePtrs[i]->mp_tVertices[1]->m_uliIndexNr, m_vTrianglePtrs[i]->mp_tVertices[2]->m_uliIndexNr);
			fprintf(fp, "E0:=%2d; E1:=%2d; E2:=%2d\n", m_vTrianglePtrs[i]->mp_tEdges[0]->m_uliIndexNr, m_vTrianglePtrs[i]->mp_tEdges[1]->m_uliIndexNr, m_vTrianglePtrs[i]->mp_tEdges[2]->m_uliIndexNr);
		}
	}

	fclose(fp);
}



void CMesh::ReadTriangleFile(char *pFileName)
{
	FILE *pF;
	char cWord[15]; 
	double dCoords[3], dCoordsI[3];
	int uiCorners[4];
	unsigned int c, i=0, j=0, uiNrPoints=0, uiNrTriangles=0;


	pF = fopen(pFileName, "r");
	if (pF != NULL)
	{
		// move to beginning of vertex data
		do
		{
			c = fgetc(pF); 
		}
		while(c != '[');

		printf("Reading vertices ");
		// 
		do
		{
			c = fgetc(pF); 
		}
		while( ((c < '0') || (c > '9')) && (c != '+') && (c != '-') && (c != '.') );

				
		do
		{
			i = 0;
			do
			{
				j = 0;
				do
				{
					cWord[j] = c;
					c = fgetc(pF);
					j++;
				}
				while( ((c >= '0') && (c <= '9')) || (c == '.') );
				
				cWord[j] = '\0';

				// scale down; skull=1000, face=150, torso=100; knot =750; teapot = 75; kies = 150; kever = 1000.0; Porsche =1000; Lamborghini=1500; spaceshuttle=3000;
				dCoords[i] = atof(cWord) / 500.0; 
				i++;

				while ( ((c < '0') || (c > '9')) && (c != '+') && (c != '-') && (c != '.') && (c != ']') )
				{
					c = fgetc(pF); 
				}
				
			}
			while(i<3);

			//Transform teapot
			/*			
			//rotate .. deg about x-axis 
			dCoordsI[0] = dCoords[0];
			dCoordsI[1] = dCoords[1]*cos(90.0*dFact) - dCoords[2]*sin(90.0*dFact);
			dCoordsI[2] = dCoords[1]*sin(90.0*dFact) + dCoords[2]*cos(90.0*dFact);
			
			//rotate .. deg about z-axis
			dCoords[0] = dCoordsI[0]*cos(-45.0*dFact) - dCoordsI[1]*sin(-45.0*dFact);
			dCoords[1] = dCoordsI[0]*sin(-45.0*dFact) + dCoordsI[1]*cos(-45.0*dFact);
			dCoords[2] = dCoordsI[2];

			//rotate .. deg about y-axis //face=60; torso=90
			//dCoords[0] = dCoordsI[0]*cos(90*dFact) + dCoordsI[2]*sin(90*dFact);
			//dCoords[1] = dCoordsI[1];
			//dCoords[2] = dCoordsI[0]*-sin(90*dFact) + dCoordsI[2]*cos(90*dFact);
			
			//dCoords[2] -= 0.1;
			*/


			//Transform Van
			/*
			//rotate .. deg about x-axis 
			dCoordsI[0] = dCoords[0];
			dCoordsI[1] = dCoords[1]*cos(90.0*dFact) - dCoords[2]*sin(90.0*dFact);
			dCoordsI[2] = dCoords[1]*sin(90.0*dFact) + dCoords[2]*cos(90.0*dFact);
			
			//rotate .. deg about z-axis
			dCoords[0] = dCoordsI[0]*cos(125.0*dFact) - dCoordsI[1]*sin(125.0*dFact);
			dCoords[1] = dCoordsI[0]*sin(125.0*dFact) + dCoordsI[1]*cos(125.0*dFact);
			dCoords[2] = dCoordsI[2];

			//rotate .. deg about y-axis //face=60; torso=90
			//dCoords[0] = dCoordsI[0]*cos(90*dFact) + dCoordsI[2]*sin(90*dFact);
			//dCoords[1] = dCoordsI[1];
			//dCoords[2] = dCoordsI[0]*-sin(90*dFact) + dCoordsI[2]*cos(90*dFact);
			
			dCoords[0] += 0.13;
			dCoords[2] -= 0.05;
			*/


			//Transform skull

			/*
			//rotate .. deg about x-axis 
			dCoordsI[0] = dCoords[0];
			dCoordsI[1] = dCoords[1]*cos(90.0*dFact) - dCoords[2]*sin(90.0*dFact);
			dCoordsI[2] = dCoords[1]*sin(90.0*dFact) + dCoords[2]*cos(90.0*dFact);
			
			//rotate .. deg about z-axis
			dCoords[0] = dCoordsI[0]*cos(60.0*dFact) - dCoordsI[1]*sin(60.0*dFact);
			dCoords[1] = dCoordsI[0]*sin(60.0*dFact) + dCoordsI[1]*cos(60.0*dFact);
			dCoords[2] = dCoordsI[2];

			//rotate .. deg about y-axis //face=60; torso=90
			//dCoords[0] = dCoordsI[0]*cos(90*dFact) + dCoordsI[2]*sin(90*dFact);
			//dCoords[1] = dCoordsI[1];
			//dCoords[2] = dCoordsI[0]*-sin(90*dFact) + dCoordsI[2]*cos(90*dFact);
			
			//dCoords[2] -= 0.1;
			*/

			//Transform Kies
			/*
			//translate kies
			dCoords[0] -= 0.1952;
			dCoords[1] += 0.423;
			dCoords[2] -= 0.2141;

			//rotate .. deg about x-axis 
			dCoordsI[0] = dCoords[0];
			dCoordsI[1] = dCoords[1]*cos(15.0*dFact) - dCoords[2]*sin(15.0*dFact);
			dCoordsI[2] = dCoords[1]*sin(15.0*dFact) + dCoords[2]*cos(15.0*dFact);

			//rotate .. deg about y-axis //face=60; torso=90; kies=30;
			dCoords[0] = dCoordsI[0]*cos(30.0*dFact) + dCoordsI[2]*sin(30.0*dFact);
			dCoords[1] = dCoordsI[1];
			dCoords[2] = dCoordsI[0]*-sin(30.0*dFact) + dCoordsI[2]*cos(30.0*dFact);
			
			dCoords[0] += 0.045;
			dCoords[1] -= 0.15;
			dCoords[2] -= 0.07;
			*/



			//Transform Torso
			
			/*
			//rotate .. deg about z-axis
			dCoordsI[0] = dCoords[0]*cos(90.0*dFact) - dCoords[1]*sin(90.0*dFact);
			dCoordsI[1] = dCoords[0]*sin(90.0*dFact) + dCoords[1]*cos(90.0*dFact);
			dCoordsI[2] = dCoords[2];

			//rotate .. deg about y-axis //torso=90
			dCoords[0] = dCoordsI[0]*cos(90.0*dFact) + dCoordsI[2]*sin(90.0*dFact);
			dCoords[1] = dCoordsI[1];
			dCoords[2] = dCoordsI[0]*-sin(90.0*dFact) + dCoordsI[2]*cos(90.0*dFact);
			*/
			
			// Translate Face
			/*
			//rotate .. deg about z-axis
			dCoordsI[0] = dCoords[0]*cos(90*dFact) - dCoords[1]*sin(90*dFact);
			dCoordsI[1] = dCoords[0]*sin(90*dFact) + dCoords[1]*cos(90*dFact);
			dCoordsI[2] = dCoords[2];

			//rotate .. deg about y-axis //face=60; torso=90
			dCoords[0] = dCoordsI[0]*cos(60*dFact) + dCoordsI[2]*sin(60*dFact);
			dCoords[1] = dCoordsI[1];
			dCoords[2] = dCoordsI[0]*-sin(60*dFact) + dCoordsI[2]*cos(60*dFact);

			// translate face
			dCoords[0] += 0.2;
			dCoords[1] += 0.05;
			dCoords[2] -= 1.0;
			*/



			//Transform Kever
			/*
			//rotate .. deg about x-axis 
			
			dCoordsI[0] = dCoords[0];
			dCoordsI[1] = dCoords[1]*cos(90.0*dFact) - dCoords[2]*sin(90.0*dFact);
			dCoordsI[2] = dCoords[1]*sin(90.0*dFact) + dCoords[2]*cos(90.0*dFact);
			
			//rotate .. deg about z-axis
			dCoords[0] = dCoordsI[0]*cos(-45.0*dFact) - dCoordsI[1]*sin(-45.0*dFact);
			dCoords[1] = dCoordsI[0]*sin(-45.0*dFact) + dCoordsI[1]*cos(-45.0*dFact);
			dCoords[2] = dCoordsI[2];

			//rotate .. deg about y-axis 
			//dCoords[0] = dCoordsI[0]*cos(0.0*dFact) + dCoordsI[2]*sin(0.0*dFact);
			//dCoords[1] = dCoordsI[1];
			//dCoords[2] = dCoordsI[0]*-sin(0.0*dFact) + dCoordsI[2]*cos(0.0*dFact);
			dCoords[2] -= 0.03;
			*/
			

			//Transform Porsche
			/*
			//rotate .. deg about x-axis 
			
			dCoordsI[0] = dCoords[0];
			dCoordsI[1] = dCoords[1]*cos(90.0*dFact) - dCoords[2]*sin(90.0*dFact);
			dCoordsI[2] = dCoords[1]*sin(90.0*dFact) + dCoords[2]*cos(90.0*dFact);
			
			//rotate .. deg about z-axis
			dCoords[0] = dCoordsI[0]*cos(-45.0*dFact) - dCoordsI[1]*sin(-45.0*dFact);
			dCoords[1] = dCoordsI[0]*sin(-45.0*dFact) + dCoordsI[1]*cos(-45.0*dFact);
			dCoords[2] = dCoordsI[2];

			//rotate .. deg about y-axis 
			//dCoords[0] = dCoordsI[0]*cos(0.0*dFact) + dCoordsI[2]*sin(0.0*dFact);
			//dCoords[1] = dCoordsI[1];
			//dCoords[2] = dCoordsI[0]*-sin(0.0*dFact) + dCoordsI[2]*cos(0.0*dFact);
			
			dCoords[2] -= 0.03;
			*/
			

			//Transform Lamborghini
			/*
			//rotate .. deg about x-axis 
			
			dCoordsI[0] = dCoords[0];
			dCoordsI[1] = dCoords[1]*cos(90.0*dFact) - dCoords[2]*sin(90.0*dFact);
			dCoordsI[2] = dCoords[1]*sin(90.0*dFact) + dCoords[2]*cos(90.0*dFact);
			
			//rotate .. deg about z-axis
			dCoords[0] = dCoordsI[0]*cos(135.0*dFact) - dCoordsI[1]*sin(135.0*dFact);
			dCoords[1] = dCoordsI[0]*sin(135.0*dFact) + dCoordsI[1]*cos(135.0*dFact);
			dCoords[2] = dCoordsI[2];

			//rotate .. deg about y-axis 
			//dCoords[0] = dCoordsI[0]*cos(0.0*dFact) + dCoordsI[2]*sin(0.0*dFact);
			//dCoords[1] = dCoordsI[1];
			//dCoords[2] = dCoordsI[0]*-sin(0.0*dFact) + dCoordsI[2]*cos(0.0*dFact);
			
			dCoords[2] -= 0.1;
			*/

			/*

			//Transform SpaceShuttle
			
			//translate SpaceShuttle
			//dCoords[0] -= 0.1952;
			//dCoords[1] += 0.423;
			//dCoords[2] -= 0.2141;

			
			//rotate .. deg about x-axis 
			dCoordsI[0] = dCoords[0];
			dCoordsI[1] = dCoords[1]*cos(90.0*dFact) - dCoords[2]*sin(90.0*dFact);
			dCoordsI[2] = dCoords[1]*sin(90.0*dFact) + dCoords[2]*cos(90.0*dFact);

			//rotate .. deg about z-axis
			dCoords[0] = dCoordsI[0]*cos(110.0*dFact) - dCoordsI[1]*sin(110.0*dFact);
			dCoords[1] = dCoordsI[0]*sin(110.0*dFact) + dCoordsI[1]*cos(110.0*dFact);
			dCoords[2] = dCoordsI[2];
									
			dCoords[0] += 0.045;
			dCoords[1] += 0.20;
			dCoords[2] -= 0.07;

			*/

			//Transform Ducky
			
			//translate SpaceShuttle
			//dCoords[0] -= 0.1952;
			//dCoords[1] += 0.423;
			//dCoords[2] -= 0.2141;

            //translate Moteur
			dCoords[0] -= 3.4; //x
			dCoords[1] += 1.5; //z
			dCoords[2] -= 1.1; //y
			
			//rotate .. deg about x-axis 
			dCoordsI[0] = dCoords[0];
			dCoordsI[1] = dCoords[1]*cos(100.0*dFact) - dCoords[2]*sin(100.0*dFact);
			dCoordsI[2] = dCoords[1]*sin(100.0*dFact) + dCoords[2]*cos(100.0*dFact);

			//rotate .. deg about z-axis
			//dCoordsI[0] = dCoords[0]*cos(180.0*dFact) - dCoords[1]*sin(180.0*dFact);
			//dCoordsI[1] = dCoords[0]*sin(180.0*dFact) + dCoords[1]*cos(180.0*dFact);
			//dCoordsI[2] = dCoords[2];
									
		   //dCoords[0] += 0.045;
			//dCoords[1] += 0.20;
			//dCoords[2] -= 0.07;
			

            


			
			AddVertex(dCoordsI[0], dCoordsI[1], dCoordsI[2]);
			uiNrPoints++;

			if((uiNrPoints % 250) == 0)
				printf(".");

		}
		while(c != ']');

		printf("%d\n", uiNrPoints);
		printf("Reading triangles ");

	
						
		// move to beginning of triangle data
		do
		{
			c = fgetc(pF);
		}
		while(c != '[');

		do
		{
			c = fgetc(pF); 
		}
		while( ((c < '0') || (c > '9')) );

		do
		{
			i = 0;
			do
			{
				j = 0;
				do
				{
					cWord[j] = c;
					c = fgetc(pF);
					j++;
				}
				while( ((c >= '0') && (c <= '9')));
				
				cWord[j] = '\0';
				uiCorners[i] = atoi(cWord);
				i++;

				while ( ((c < '0') || (c > '9')) && (c != '+') && (c != '-') && (c != '.') && (c != ']') )
				{
					c = fgetc(pF); 
				}
			}
			while(i<4);

			AddTriangle(uiCorners[0], uiCorners[1], uiCorners[2]);
			uiNrTriangles++;

			if((uiNrTriangles % 250) ==0)
				printf(".");
		}		
		while (c != ']');

		printf("%d\n", uiNrTriangles);

		m_bEmpty = false;
		fclose(pF);
	}
}


void CMesh::Write(char *pFileName)
{
	FILE *pF;	
	unsigned long int i, j, iTmp;

	pF = fopen(pFileName, "wb");
	if (pF != NULL)
	{
		// Write the number of vertices to the file
		iTmp = m_vVertexPtrs.size();
		fwrite(&iTmp, sizeof(iTmp), 1, pF);

		// Write the number of edges to the file
		iTmp = m_vEdgePtrs.size();
		fwrite(&iTmp, sizeof(iTmp), 1, pF);

		// Write the number of Triangles to the file
		iTmp = m_vTrianglePtrs.size();
		fwrite(&iTmp, sizeof(int), 1, pF);
			
		// Write the vertex coords 
		for(i=0; i<m_vVertexPtrs.size(); i++)
		{
			if(m_vVertexPtrs[i] != NULL)
				fwrite(m_vVertexPtrs[i]->m_dCoords, sizeof(m_vVertexPtrs[i]->m_dCoords[0]), 3, pF);
		}

		// Write the BisectPlaneNormal vectors and 
		// the TriangleNormalMean vectors
		for(i=0; i<m_vEdgePtrs.size(); i++)
		{
			if(m_vEdgePtrs[i] != NULL)
			{
				fwrite(m_vEdgePtrs[i]->mp_tBisectPlaneNormal->m_dCoords, sizeof(m_vEdgePtrs[i]->mp_tBisectPlaneNormal->m_dCoords[0]), 3, pF);
				fwrite(m_vEdgePtrs[i]->mp_tTriangleNormalMean->m_dCoords, sizeof(m_vEdgePtrs[i]->mp_tTriangleNormalMean->m_dCoords[0]), 3, pF);
			}
		}
					

		// Write the triangle Normals, NormalEnd, Centers & EdgeNormals for every triangle
		for(i=0; i<m_vTrianglePtrs.size(); i++)
		{
			if(m_vTrianglePtrs[i] != NULL)
			{
				fwrite(m_vTrianglePtrs[i]->mp_tNormal->m_dCoords, sizeof(m_vTrianglePtrs[i]->mp_tNormal->m_dCoords[0]), 3, pF);
				fwrite(m_vTrianglePtrs[i]->mp_tNormalEnd->m_dCoords, sizeof(m_vTrianglePtrs[i]->mp_tNormalEnd->m_dCoords[0]), 3, pF);
				fwrite(m_vTrianglePtrs[i]->mp_tCenter->m_dCoords, sizeof(m_vTrianglePtrs[i]->mp_tCenter->m_dCoords[0]), 3, pF);
				fwrite(m_vTrianglePtrs[i]->mp_tEdgeNormals[0]->m_dCoords, sizeof(m_vTrianglePtrs[i]->mp_tEdgeNormals[0]->m_dCoords[0]), 3, pF);
				fwrite(m_vTrianglePtrs[i]->mp_tEdgeNormals[1]->m_dCoords, sizeof(m_vTrianglePtrs[i]->mp_tEdgeNormals[1]->m_dCoords[0]), 3, pF);
				fwrite(m_vTrianglePtrs[i]->mp_tEdgeNormals[2]->m_dCoords, sizeof(m_vTrianglePtrs[i]->mp_tEdgeNormals[2]->m_dCoords[0]), 3, pF);
			}
		}
					
		// Write the VertexNormal vectors
		for(i=0; i<m_vVertexNormalPtrs.size(); i++)
		{
			if(m_vVertexNormalPtrs[i] != NULL)
				fwrite(m_vVertexNormalPtrs[i]->m_dCoords, sizeof(m_vVertexNormalPtrs[i]->m_dCoords[0]), 3, pF);
		}

		
		// Write for every vertex the indices of the edges that share this vertex
		// Start the write with the number of edges that share this vertex
		for(i=0; i<m_vVertexPtrs.size(); i++)
		{
			if(m_vVertexPtrs[i] != NULL)
			{
				iTmp = m_vVertexPtrs[i]->m_vEdgePtrs.size();
				fwrite(&iTmp, sizeof(iTmp), 1, pF);
				for(j=0; j<m_vVertexPtrs[i]->m_vEdgePtrs.size(); j++)
				{
					if(m_vVertexPtrs[i]->m_vEdgePtrs[j] != NULL)
						fwrite(&m_vVertexPtrs[i]->m_vEdgePtrs[j]->m_uliIndexNr, sizeof(m_vVertexPtrs[i]->m_vEdgePtrs[j]->m_uliIndexNr), 1, pF);
				}
			}
		}


		// Write for every edge the indices of the vertices that this edge consists off
		// Next write the two triangle indices of this edge,
		// if one of the triangles == NULL, a -1 is written
		for(i=0; i<m_vEdgePtrs.size(); i++)
		{
			if(m_vEdgePtrs[i] != NULL)
			{
				fwrite(&m_vEdgePtrs[i]->mp_tVertices[0]->m_uliIndexNr, sizeof(m_vEdgePtrs[i]->mp_tVertices[0]->m_uliIndexNr), 1, pF);
				fwrite(&m_vEdgePtrs[i]->mp_tVertices[1]->m_uliIndexNr, sizeof(m_vEdgePtrs[i]->mp_tVertices[1]->m_uliIndexNr), 1, pF);
				
				
				if(m_vEdgePtrs[i]->mp_tTriangles[0] != NULL)
					fwrite(&m_vEdgePtrs[i]->mp_tTriangles[0]->m_uliIndexNr, sizeof(m_vEdgePtrs[i]->mp_tTriangles[0]->m_uliIndexNr), 1, pF);
				else
				{
					iTmp = -1;
					fwrite(&iTmp, sizeof(iTmp), 1, pF);
				}
				
				if(m_vEdgePtrs[i]->mp_tTriangles[1] != NULL)
					fwrite(&m_vEdgePtrs[i]->mp_tTriangles[1]->m_uliIndexNr, sizeof(m_vEdgePtrs[i]->mp_tTriangles[1]->m_uliIndexNr), 1, pF);
				else
				{
					iTmp = -1;
					fwrite(&iTmp, sizeof(iTmp), 1, pF);
				}
			}
		}


		// Write for every triangle the indices of the vertices that this triangle consists of
		// Next write the indices of the triangle edges
		for(i=0;i<m_vTrianglePtrs.size(); i++)
		{
			if(m_vTrianglePtrs[i] != NULL)
			{
				fwrite(&m_vTrianglePtrs[i]->mp_tVertices[0]->m_uliIndexNr, sizeof(m_vTrianglePtrs[i]->mp_tVertices[0]->m_uliIndexNr), 1, pF);
				fwrite(&m_vTrianglePtrs[i]->mp_tVertices[1]->m_uliIndexNr, sizeof(m_vTrianglePtrs[i]->mp_tVertices[1]->m_uliIndexNr), 1, pF);
				fwrite(&m_vTrianglePtrs[i]->mp_tVertices[2]->m_uliIndexNr, sizeof(m_vTrianglePtrs[i]->mp_tVertices[2]->m_uliIndexNr), 1, pF);
				fwrite(&m_vTrianglePtrs[i]->mp_tEdges[0]->m_uliIndexNr, sizeof(m_vTrianglePtrs[i]->mp_tEdges[0]->m_uliIndexNr), 1, pF);
				fwrite(&m_vTrianglePtrs[i]->mp_tEdges[1]->m_uliIndexNr, sizeof(m_vTrianglePtrs[i]->mp_tEdges[1]->m_uliIndexNr), 1, pF);
				fwrite(&m_vTrianglePtrs[i]->mp_tEdges[2]->m_uliIndexNr, sizeof(m_vTrianglePtrs[i]->mp_tEdges[2]->m_uliIndexNr), 1, pF);
			}
		}

		fclose(pF);
	}


}

void CMesh::ClearDataStructure(void)
{
	int i;

	if(m_bEmpty == false)
	{
		for(i=0; i<m_vTrianglePtrs.size(); i++)
		{
			if(m_vTrianglePtrs[i] != NULL)
				delete m_vTrianglePtrs[i];
		}


		for(i=0; i<m_vEdgePtrs.size(); i++)
		{
			if(m_vEdgePtrs[i] != NULL)
				delete m_vEdgePtrs[i];
		}

		for(i=0; i<m_vVertexPtrs.size(); i++)
		{
			if(m_vVertexPtrs[i] != NULL)
				delete m_vVertexPtrs[i];
		}

		for(i=0; i<m_vVertexNormalPtrs.size(); i++)
		{
			if(m_vVertexNormalPtrs[i] != NULL)
				delete m_vVertexNormalPtrs[i];
		}


		m_vVertexPtrs.clear();
		m_vVertexNormalPtrs.clear();
		m_vEdgePtrs.clear();
		m_vTrianglePtrs.clear();

		mp_tClosestVertex = NULL;
		mp_tClosestTriangle = NULL;
		dDistanceToClosestVertex = 0.0;

		m_bEmpty = true;
		m_iBuildPerc = 100;
	}

}

void CMesh::Read(char *pFileName)
{
	FILE *pF;

	unsigned long int i=0, j=0, iNrV=0, iNrE=0, iNrT=0, iIndex=0, iTmp=0;

	CVertex *pV;
	//CVertex *pV2;
	CEdge *pE;
	CTriangle *pT;

	
	pF = fopen(pFileName, "rb");
	if (pF != NULL)
	{

		// Read the nr of vertices, edges and triangles
		fread( (void*)&iNrV, sizeof(iNrV), 1, pF);
		fread( (void*)&iNrE, sizeof(iNrE), 1, pF);
		fread( (void*)&iNrT, sizeof(iNrT), 1, pF);

		// Read the vertex coordinates
		for(i=0; i<iNrV; i++)
		{
			pV = new CVertex();
			pV->m_uliIndexNr = i;

			fread((void*)pV->m_dCoords, sizeof(pV->m_dCoords[0]), 3, pF);

			m_vVertexPtrs.push_back(pV);

			//pV2 = new CVertex();
			//*pV2 = *pV;
			//m_vDisplayVertexPtrs.push_back(pV2);
		}


		// Read the BisectPlaneNormal vectors and 
		// the TriangleNormalMean vectors for the Edges
		for(i=0; i<iNrE; i++)
		{
			pE = new CEdge();
			pE->m_uliIndexNr = i;

			fread((void*)pE->mp_tBisectPlaneNormal->m_dCoords, sizeof(pE->mp_tBisectPlaneNormal->m_dCoords[0]), 3, pF);
			fread((void*)pE->mp_tTriangleNormalMean->m_dCoords, sizeof(pE->mp_tTriangleNormalMean->m_dCoords[0]), 3, pF);

			m_vEdgePtrs.push_back(pE);

		}

		// Read the triangle Normal, NormalEnd, Centers  EdgeNormals for every triangle
		for(i=0; i<iNrT; i++) 
		{
			pT = new CTriangle();
			pT->m_uliIndexNr = i;

			pT->mp_tCenter = new CVertex();
			pT->mp_tNormal = new CVertex();
			pT->mp_tNormalEnd = new CVertex();

			pT->mp_tEdgeNormals[0] = new CVertex();
			pT->mp_tEdgeNormals[1] = new CVertex();
			pT->mp_tEdgeNormals[2] = new CVertex();

			fread((void*)pT->mp_tNormal->m_dCoords, sizeof(pT->mp_tNormal->m_dCoords[0]), 3, pF);
			fread((void*)pT->mp_tNormalEnd->m_dCoords, sizeof(pT->mp_tNormalEnd->m_dCoords[0]), 3, pF);
			fread((void*)pT->mp_tCenter->m_dCoords, sizeof(pT->mp_tCenter->m_dCoords[0]), 3, pF);
			fread((void*)pT->mp_tEdgeNormals[0]->m_dCoords, sizeof(pT->mp_tEdgeNormals[0]->m_dCoords[0]), 3, pF);
			fread((void*)pT->mp_tEdgeNormals[1]->m_dCoords, sizeof(pT->mp_tEdgeNormals[1]->m_dCoords[0]), 3, pF);
			fread((void*)pT->mp_tEdgeNormals[2]->m_dCoords, sizeof(pT->mp_tEdgeNormals[2]->m_dCoords[0]), 3, pF);

			m_vTrianglePtrs.push_back(pT);

		}

		// Read the VertexNormal vectors
		for(i=0; i<iNrV; i++)
		{
			pV = new CVertex();
			fread((void*)&pV->m_dCoords, sizeof(pV->m_dCoords[0]), 3, pF);
			m_vVertexNormalPtrs.push_back(pV);
		}

	
		// Read for every vertex the indices of the edges that share this vertex
		// Start the reading with the number of edges that share this vertex
		for(i=0; i<iNrV; i++)
		{
			fread((void*)&iTmp, sizeof(iTmp), 1, pF);

			for(j=0; j<iTmp; j++)
			{
				fread((void*)&iIndex, sizeof(iIndex), 1, pF);
				m_vVertexPtrs[i]->m_vEdgePtrs.push_back(m_vEdgePtrs[iIndex]);
			}
		}

		// Read for every edge the indices of the vertices that this edge consists off
		// Next read the two triangle indices of this edge,
		// if one of the triangles reads as -1, this triangle pointer becomes NULL
		for(i=0; i<iNrE; i++)
		{
			
			fread((void*)&iIndex, sizeof(iIndex), 1, pF); // Read the first vertex index
			m_vEdgePtrs[i]->mp_tVertices[0] = m_vVertexPtrs[iIndex];

			fread((void*)&iIndex, sizeof(iIndex), 1, pF); // Read the second vertex index
			m_vEdgePtrs[i]->mp_tVertices[1] = m_vVertexPtrs[iIndex];


			fread((void*)&iIndex, sizeof(iIndex), 1, pF); // Read the first triangle index
			if(iIndex != -1)
				m_vEdgePtrs[i]->mp_tTriangles[0] = m_vTrianglePtrs[iIndex];
			else
				m_vEdgePtrs[i]->mp_tTriangles[0] = NULL;


			fread((void*)&iIndex, sizeof(iIndex), 1, pF); // Read the first triangle index
			if(iIndex != -1)
				m_vEdgePtrs[i]->mp_tTriangles[1] = m_vTrianglePtrs[iIndex];
			else
				m_vEdgePtrs[i]->mp_tTriangles[1] = NULL;
		}

		// Read for every triangle the indices of the vertices that this triangle consists of
		// Next read the indices of the triangle edges
		for(i=0;i<iNrT; i++)
		{
			fread((void*)&iIndex, sizeof(iIndex), 1, pF); // Read the first vertex index
			m_vTrianglePtrs[i]->mp_tVertices[0] = m_vVertexPtrs[iIndex];

			fread((void*)&iIndex, sizeof(iIndex), 1, pF); // Read the second vertex index
			m_vTrianglePtrs[i]->mp_tVertices[1] = m_vVertexPtrs[iIndex];

			fread((void*)&iIndex, sizeof(iIndex), 1, pF); // Read the third vertex index
			m_vTrianglePtrs[i]->mp_tVertices[2] = m_vVertexPtrs[iIndex];
			
			fread((void*)&iIndex, sizeof(iIndex), 1, pF); // Read the first edge index
			m_vTrianglePtrs[i]->mp_tEdges[0] = m_vEdgePtrs[iIndex];

			fread((void*)&iIndex, sizeof(iIndex), 1, pF); // Read the first edge index
			m_vTrianglePtrs[i]->mp_tEdges[1] = m_vEdgePtrs[iIndex];

			fread((void*)&iIndex, sizeof(iIndex), 1, pF); // Read the first edge index
			m_vTrianglePtrs[i]->mp_tEdges[2] = m_vEdgePtrs[iIndex];
		}
		
		m_bEmpty = false;
		fclose(pF);
	} // if(pF != NULL)



}