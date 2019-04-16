#include <glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "HmWorkspace.h"

const double PI = 3.1415926535897932384626433832795;

GLfloat HM_WS_BLACK[] = {0.0, 0.0, 0.0, 1.0};
GLfloat HM_WS_GREY[] = {0.4, 0.4, 0.4, 1.0};
GLfloat HM_WS_BKG[] = {0.4745, 0.6118, 0.6902, 0.30};


CHmWorkspace::CHmWorkspace( float fRadiusMin, 
                            float fRadiusMax, 
                            float fCenterOffset, 
                            float fPhiMin,
                            float fPhiMax,
                            float fZMin,
                            float fZMax,
                            unsigned int uiNrSegments,
                            unsigned int uiNrSubdivs,
                            bool bUseTriangles)
{
    m_pPlaneVertices= NULL;
    m_pSideVertices = NULL;
    m_pBackVertices = NULL;

    m_pPlaneNormals = NULL;
    m_pSideNormals = NULL;
    m_pBackNormals = NULL;

    m_pIndices = NULL;

    m_fRadiusMin = fRadiusMin;
    m_fRadiusMax = fRadiusMax;
    m_fCenterOffset = fCenterOffset;
    m_fPhiMin = fPhiMin;
    m_fPhiMax = fPhiMax;
    m_fZMin = fZMin;
    m_fZMax = fZMax;
    m_uiNrSegments = uiNrSegments;
    m_uiNrSubdivs = uiNrSubdivs;

    m_bUseTriangles = bUseTriangles;

    CreateVertices();
}

CHmWorkspace::~CHmWorkspace()
{
    if(m_pPlaneVertices != NULL)
        delete[] m_pPlaneVertices;

    if(m_pSideVertices != NULL)
        delete[] m_pSideVertices;

    if(m_pBackVertices != NULL)
        delete[] m_pBackVertices;

    if(m_pIndices != NULL)
        delete[] m_pIndices;
}

void CHmWorkspace::CreateVertices(void)
{
    unsigned int i, j, k, t, i1, i2, i3, i4;
    unsigned int uiNrVertices = (m_uiNrSegments+1) * (m_uiNrSubdivs+1);
    float fPhi, fCosPhi, fSinPhi, fPhiStep;
    float x, z, fXStep, fZStep;
    float fX, fY, fZ;
    float l;

    m_pPlaneVertices = new float[uiNrVertices * 3];
    m_pSideVertices = new float[uiNrVertices * 3];
    m_pBackVertices = new float[uiNrVertices * 3];

    m_pPlaneNormals = new float[uiNrVertices * 3];
    m_pSideNormals = new float[uiNrVertices * 3];
    m_pBackNormals = new float[uiNrVertices * 3];

    if(m_bUseTriangles)
    {
        m_uiNrPolygons = m_uiNrSegments * m_uiNrSubdivs * 2;
        m_pIndices = new unsigned int[m_uiNrPolygons * 3];
    }
    else
    {
        m_uiNrPolygons = m_uiNrSegments * m_uiNrSubdivs;
        m_pIndices = m_pIndices = new unsigned int[m_uiNrPolygons * 4];
    }
    
    fPhiStep = (m_fPhiMax - m_fPhiMin) / m_uiNrSegments;
    fXStep = (m_fRadiusMax - m_fRadiusMin) / m_uiNrSubdivs;
    fZStep = (m_fZMax - m_fZMin) / m_uiNrSegments;

    // create workspace vertices, 
    z = 0;
    for(i=0; i<=m_uiNrSegments; i++)
	{
		fPhi = m_fPhiMin + (i*fPhiStep);
		fCosPhi = cos(fPhi);
		fSinPhi = sin(fPhi);

        x = 0;
        for(j=0; j<=m_uiNrSubdivs; j++)
		{
			k = 3*(i*(m_uiNrSubdivs+1) + j);

			fX = m_fCenterOffset + (float)((m_fRadiusMin + x) * fCosPhi);
			fY = (float)((m_fRadiusMin + x) * fSinPhi);
            fZ  = m_fZMin + z;

			m_pPlaneVertices[k  ] = fX;  // x
			m_pPlaneVertices[k+1] = fY;  // y
			m_pPlaneVertices[k+2] = 0.0; // z

            m_pPlaneNormals[k  ] =  0.0;
            m_pPlaneNormals[k+1] =  0.0;
            m_pPlaneNormals[k+2] =  1.0;

            m_pSideVertices[k  ] = m_fRadiusMin + x; 
			m_pSideVertices[k+1] = 0.0;
			m_pSideVertices[k+2] = fZ;

            m_pSideNormals[k  ] =  0.0;
            m_pSideNormals[k+1] =  1.0;
            m_pSideNormals[k+2] =  0.0;

            m_pBackVertices[k  ] = m_fCenterOffset +  (float)(m_fRadiusMin * fCosPhi);
            m_pBackVertices[k+1] = (float)(m_fRadiusMin * fSinPhi);
            m_pBackVertices[k+2] = m_fZMin + j*fZStep;

            l = sqrt(m_pBackVertices[k  ]*m_pBackVertices[k  ] + m_pBackVertices[k+1]*m_pBackVertices[k+1]);

            m_pBackNormals[k  ] = m_pBackVertices[k  ] / l;
            m_pBackNormals[k+1] = m_pBackVertices[k+1] / l;
            m_pBackNormals[k+2] = 0.0;

            x += fXStep;
            
		}
        z += fZStep;
	}

    if(m_bUseTriangles)
    {
        // create triangle indices
        for(i=0; i<m_uiNrSegments; i++)
        {
            for(j=0; j<m_uiNrSubdivs; j++)
            {
                for(t=0; t<2; t++)
                {
                    k = 3*(i*m_uiNrSubdivs*2 + j*2 + t);
                
                    i1 =    i  * (m_uiNrSubdivs+1) + j + t;
                    i2 = (i+t) * (m_uiNrSubdivs+1) + j + 1;
                    i3 = (i+1) * (m_uiNrSubdivs+1) + j;

                    m_pIndices[k  ] = i1;
                    m_pIndices[k+1] = i2;
                    m_pIndices[k+2] = i3;
                }
            }
        }
    }
    else
    {
        // create quad indices
        for(i=0; i<m_uiNrSegments; i++)
        {
            for(j=0; j<m_uiNrSubdivs; j++)
            {
                k = i*m_uiNrSubdivs + j;
            
                i1 = k + i;
                i2 = i1 + 1;
                i3 = i2 + (m_uiNrSubdivs+1);
                i4 = i3 - 1;

                m_pIndices[k*4  ] = i1;
                m_pIndices[k*4+1] = i2;
                m_pIndices[k*4+2] = i3;
                m_pIndices[k*4+3] = i4;
            }
        }
    }
 
    
}
/*
void CHmWorkspace::Display(bool bGrid)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glMaterialfv(GL_FRONT, GL_AMBIENT, HM_WS_BKG);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, HM_WS_BKG);

    Display2();

    glDisable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDisable(GL_LIGHTING);
    glColor4fv(HM_WS_GREY);
    
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1.1, 1.0);

    Display2();

    glDisable(GL_POLYGON_OFFSET_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_LIGHTING);
}

*/

/*
void CHmWorkspace::Display2(void)
{
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // Display workspace bottom and top plane
    glVertexPointer(3, GL_FLOAT, 0, m_pPlaneVertices);
    glNormalPointer(GL_FLOAT, 0, m_pPlaneNormals);
    
    glPushMatrix();
    glTranslatef(0.0, 0.0, m_fZMin);
    if(m_bUseTriangles)
        glDrawElements(GL_TRIANGLES, 3*m_uiNrPolygons, GL_UNSIGNED_INT, m_pIndices);
    else
        glDrawElements(GL_QUADS, 4*m_uiNrPolygons, GL_UNSIGNED_INT, m_pIndices);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, m_fZMax);
    if(m_bUseTriangles)
        glDrawElements(GL_TRIANGLES, 3*m_uiNrPolygons, GL_UNSIGNED_INT, m_pIndices);
    else
        glDrawElements(GL_QUADS, 4*m_uiNrPolygons, GL_UNSIGNED_INT, m_pIndices);
    glPopMatrix();

   
    // Display workspace left and right side
    glVertexPointer(3, GL_FLOAT, 0, m_pSideVertices);
    glNormalPointer(GL_FLOAT, 0, m_pSideNormals);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glPushMatrix();
    glTranslatef(m_fCenterOffset, 0.0, 0.0);
    glRotatef(m_fPhiMin*180.0f/PI, 0.0, 0.0, 1.0);
    if(m_bUseTriangles)
        glDrawElements(GL_TRIANGLES, 3*m_uiNrPolygons, GL_UNSIGNED_INT, m_pIndices);
    else
        glDrawElements(GL_QUADS, 4*m_uiNrPolygons, GL_UNSIGNED_INT, m_pIndices);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(m_fCenterOffset, 0.0, 0.0);
    glRotatef(m_fPhiMax*180.0f/PI, 0.0, 0.0, 1.0);
    if(m_bUseTriangles)
        glDrawElements(GL_TRIANGLES, 3*m_uiNrPolygons, GL_UNSIGNED_INT, m_pIndices);
    else
        glDrawElements(GL_QUADS, 4*m_uiNrPolygons, GL_UNSIGNED_INT, m_pIndices);
    glPopMatrix();


    // Display workspace back
    glVertexPointer(3, GL_FLOAT, 0, m_pBackVertices);
    glNormalPointer(GL_FLOAT, 0, m_pBackNormals);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glPushMatrix();
    if(m_bUseTriangles)
        glDrawElements(GL_TRIANGLES, 3*m_uiNrPolygons, GL_UNSIGNED_INT, m_pIndices);
    else
        glDrawElements(GL_QUADS, 4*m_uiNrPolygons, GL_UNSIGNED_INT, m_pIndices);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

}
*/

void CHmWorkspace::Display(unsigned char ucMode)
{
    bool bGrid = (ucMode & WS_GRID) > 0 ? true : false;

    if(ucMode & WS_BOTTOM)
        DisplayBottom(bGrid);
    
    if(ucMode & WS_TOP)
        DisplayTop(bGrid);
    
    if(ucMode & WS_LEFT)
        DisplayLeft(bGrid);

    if(ucMode & WS_RIGHT)
        DisplayRight(bGrid);
    
    if(ucMode & WS_BACK)
        DisplayBack(bGrid);
}

void CHmWorkspace::DisplayBottom(bool bGrid)
{
    glVertexPointer(3, GL_FLOAT, 0, m_pPlaneVertices);
    glNormalPointer(GL_FLOAT, 0, m_pPlaneNormals);

    glPushMatrix();
    glTranslatef(0.0, 0.0, m_fZMin);
    Display2(bGrid);
    glPopMatrix();
}

void CHmWorkspace::DisplayTop(bool bGrid)
{
    glVertexPointer(3, GL_FLOAT, 0, m_pPlaneVertices);
    glNormalPointer(GL_FLOAT, 0, m_pPlaneNormals);

    glPushMatrix();
    glTranslatef(0.0, 0.0, m_fZMax);
    Display2(bGrid);
    glPopMatrix();
}

void CHmWorkspace::DisplayLeft(bool bGrid)
{
    glVertexPointer(3, GL_FLOAT, 0, m_pSideVertices);
    glNormalPointer(GL_FLOAT, 0, m_pSideNormals);

    glPushMatrix();
    glTranslatef(m_fCenterOffset, 0.0, 0.0);
    glRotatef(m_fPhiMin*180.0f/PI, 0.0, 0.0, 1.0);
    Display2(bGrid);
    glPopMatrix();
}

void CHmWorkspace::DisplayRight(bool bGrid)
{
    glVertexPointer(3, GL_FLOAT, 0, m_pSideVertices);
    glNormalPointer(GL_FLOAT, 0, m_pSideNormals);

    glPushMatrix();
    glTranslatef(m_fCenterOffset, 0.0, 0.0);
    glRotatef(m_fPhiMax*180.0f/PI, 0.0, 0.0, 1.0);
    Display2(bGrid);
    glPopMatrix();
}

void CHmWorkspace::DisplayBack(bool bGrid)
{
    glVertexPointer(3, GL_FLOAT, 0, m_pBackVertices);
    glNormalPointer(GL_FLOAT, 0, m_pBackNormals);

    glPushMatrix();
    Display2(bGrid);
    glPopMatrix();
}

void CHmWorkspace::Display2(bool bGrid)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glMaterialfv(GL_FRONT, GL_AMBIENT, HM_WS_BKG);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, HM_WS_BKG);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
   
    if(m_bUseTriangles)
        glDrawElements(GL_TRIANGLES, 3*m_uiNrPolygons, GL_UNSIGNED_INT, m_pIndices);
    else
        glDrawElements(GL_QUADS, 4*m_uiNrPolygons, GL_UNSIGNED_INT, m_pIndices);

    if(bGrid)
    {
        glDisable(GL_BLEND);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glDisable(GL_LIGHTING);
        glColor4fv(HM_WS_GREY);
    
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.1, 1.0);

        if(m_bUseTriangles)
            glDrawElements(GL_TRIANGLES, 3*m_uiNrPolygons, GL_UNSIGNED_INT, m_pIndices);
        else
            glDrawElements(GL_QUADS, 4*m_uiNrPolygons, GL_UNSIGNED_INT, m_pIndices);

        glDisable(GL_POLYGON_OFFSET_LINE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void CHmWorkspace::DisplayAxes()
{
    
}

