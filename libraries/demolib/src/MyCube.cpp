#include <glut.h>

double CubePoints[8][3] = { -0.50, -0.50,  0.50,   // links onder voor
                             0.50, -0.50,  0.50,   // rechts onder voor
							 0.50,  0.50,  0.50,   // rechts boven voor
							-0.50,  0.50,  0.50,   // links boven voor
							-0.50, -0.50, -0.50,   // links onder achter
                             0.50, -0.50, -0.50,   // rechts onder achter
							 0.50,  0.50, -0.50,   // rechts boven achter
							-0.50,  0.50, -0.50 }; // links boven achter

double CubeNormals[12][3] = { 0.0,  0.0,  1.0,   // Front
							  0.0,  0.0, -1.0,   // Back
							 -1.0,  0.0,  0.0,   // Left
							  1.0,  0.0,  0.0,   // Right
							  0.0,  1.0,  0.0,   // Top
                              0.0, -1.0,  0.0 }; // Bottom

/*
unsigned int TriangleIndices[12][3] = { 0, 1, 3,	// Front
                                        1, 2, 3, 
								        5, 7, 6,	// Back
								        5, 4, 7, 
								        4, 0, 3,	// Left
								        4, 3, 7, 
								        1, 5, 2,	// Right
								        5, 6, 2,
								        3, 2, 6,	// Top
								        3, 6, 7, 
								        5, 1, 0,	// Bottom
								        4, 5, 0 };

*/

unsigned int QuadIndices[6][4] = { 0, 1, 2, 3,	// Front
							       5, 4, 7, 6,	// Back
							       4, 0, 3, 7,	// Left
							       1, 5, 6, 2,	// Right
							       3, 2, 6, 7,	// Top
                                   4, 5, 1, 0 };  // Bottom


void MyCubeSide(double dSize=1.0, unsigned int uiSide=0)
{
    unsigned int j;

    glPushMatrix();
    
    glScaled(dSize, dSize, dSize);
	glBegin(GL_QUADS);
    glNormal3dv(CubeNormals[uiSide]);
    for(j=0; j<4; j++)
	{
	    glVertex3dv(CubePoints[QuadIndices[uiSide][j]]);	
  	}
	glEnd();

    glPopMatrix();
}


void MyCube(double dSize=1.0)
{
	unsigned int i, j;

    glPushMatrix();
    
    glScaled(dSize, dSize, dSize);
	glBegin(GL_QUADS);
		for(i=0; i<6; i++)
		{
            glNormal3dv(CubeNormals[i]);
			for(j=0; j<4; j++)
			{
                //glNormal3dv(CubePoints[i]);
				glVertex3dv(CubePoints[QuadIndices[i][j]]);	
    		}
		}
	glEnd();

    glPopMatrix();
}



void MyCube(double dSizeX=1.0, double dSizeY=1.0, double dSizeZ=1.0)
{
	unsigned int i, j;

    glPushMatrix();
    
    glScaled(dSizeX, dSizeY, dSizeZ);
	glBegin(GL_QUADS);
		for(i=0; i<6; i++)
		{
            glNormal3dv(CubeNormals[i]);
			for(j=0; j<4; j++)
			{
                //glNormal3dv(CubePoints[i]);
				glVertex3dv(CubePoints[QuadIndices[i][j]]);	
    		}
		}
	glEnd();

    glPopMatrix();
}

void MyCube(double dSizeX=1.0, double dSizeY=1.0, double dSizeZ=1.0, unsigned int uiSide=0)
{
	unsigned int j;

    glPushMatrix();
    
    glScaled(dSizeX, dSizeY, dSizeZ);
	glBegin(GL_QUADS);
	glNormal3dv(CubeNormals[uiSide]);
	for(j=0; j<4; j++)
	{
		glVertex3dv(CubePoints[QuadIndices[uiSide][j]]);	
    }
	glEnd();

    glPopMatrix();
}



double dot3(double v1[3], double v2[3])
{
	return (v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2] );
}


void DrawCubeShadowVolume(int iLocalLight, double pLightPos[], double dExtendDist, bool bPass)
{
	double ExtendDir[3], ExtendedVertex[3], VertexLightVec[3];
	unsigned int i, j;

	glPushMatrix();
		
	if(iLocalLight)
	{
		for(i=0; i<6; i++)
		{
			VertexLightVec[0] = pLightPos[0] - CubePoints[QuadIndices[i][0]][0];
			VertexLightVec[1] = pLightPos[1] - CubePoints[QuadIndices[i][0]][1];
			VertexLightVec[2] = pLightPos[2] - CubePoints[QuadIndices[i][0]][2];

			if ( !bPass )
			{
				if( dot3(VertexLightVec, CubeNormals[i]) > 0.0)
				{
					glCullFace(GL_BACK);
					glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
				}
				else
				{
					glCullFace(GL_FRONT);
					glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
				}
			}
			else
			{
				if( dot3(VertexLightVec, CubeNormals[i]) >  0.0)
				{
					glCullFace(GL_FRONT);
					glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
				}
				else
				{
					glCullFace(GL_BACK);
					glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
				}
			}
			
			glBegin(GL_QUAD_STRIP);
			for(j=0; j<4; j++)
			{	
				
				glVertex3dv(CubePoints[QuadIndices[i][j]]);	

				ExtendDir[0] = CubePoints[QuadIndices[i][j]][0] - pLightPos[0];
				ExtendDir[1] = CubePoints[QuadIndices[i][j]][1] - pLightPos[1];
				ExtendDir[2] = CubePoints[QuadIndices[i][j]][2] - pLightPos[2];

				ExtendedVertex[0] = CubePoints[QuadIndices[i][j]][0] + ExtendDir[0] * dExtendDist;
				ExtendedVertex[1] = CubePoints[QuadIndices[i][j]][1] + ExtendDir[1] * dExtendDist;
				ExtendedVertex[2] = CubePoints[QuadIndices[i][j]][2] + ExtendDir[2] * dExtendDist;

				glVertex3dv(ExtendedVertex);	

			}

			glVertex3dv(CubePoints[QuadIndices[i][0]]);	

			ExtendDir[0] = CubePoints[QuadIndices[i][0]][0] - pLightPos[0];
			ExtendDir[1] = CubePoints[QuadIndices[i][0]][1] - pLightPos[1];
			ExtendDir[2] = CubePoints[QuadIndices[i][0]][2] - pLightPos[2];

			ExtendedVertex[0] = CubePoints[QuadIndices[i][0]][0] + ExtendDir[0] * dExtendDist;
			ExtendedVertex[1] = CubePoints[QuadIndices[i][0]][1] + ExtendDir[1] * dExtendDist;
			ExtendedVertex[2] = CubePoints[QuadIndices[i][0]][2] + ExtendDir[2] * dExtendDist;

			glVertex3dv(ExtendedVertex);
			glEnd();
		}
	}
	else
	{
		ExtendDir[0] = -pLightPos[0];
		ExtendDir[1] = -pLightPos[1];
		ExtendDir[2] = -pLightPos[2];
		
		for(i=0; i<6; i++)
		{

			VertexLightVec[0] = pLightPos[0] - CubePoints[QuadIndices[i][0]][0];
			VertexLightVec[1] = pLightPos[1] - CubePoints[QuadIndices[i][0]][1];
			VertexLightVec[2] = pLightPos[2] - CubePoints[QuadIndices[i][0]][2];


			if ( !bPass )
			{
				if( dot3(VertexLightVec, CubeNormals[i]) > 0.0)
				{
					glCullFace(GL_BACK);
					glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
				}
				else
				{
					glCullFace(GL_FRONT);
					glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
				}
			}
			else
			{
				if( dot3(VertexLightVec, CubeNormals[i]) >  0.0)
				{
					glCullFace(GL_FRONT);
					glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
				}
				else
				{
					glCullFace(GL_BACK);
					glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
				}
			}

			glBegin(GL_QUAD_STRIP);
			for(j=0; j<4; j++)
			{
				glVertex3dv(CubePoints[QuadIndices[i][j]]);	

				ExtendedVertex[0] = CubePoints[QuadIndices[i][j]][0] + ExtendDir[0] * dExtendDist;
				ExtendedVertex[1] = CubePoints[QuadIndices[i][j]][1] + ExtendDir[1] * dExtendDist;
				ExtendedVertex[2] = CubePoints[QuadIndices[i][j]][2] + ExtendDir[2] * dExtendDist;

				glVertex3dv(ExtendedVertex);	
			}

			glVertex3dv(CubePoints[QuadIndices[i][0]]);	

			ExtendedVertex[0] = CubePoints[QuadIndices[i][0]][0] + ExtendDir[0] * dExtendDist;
			ExtendedVertex[1] = CubePoints[QuadIndices[i][0]][1] + ExtendDir[1] * dExtendDist;
			ExtendedVertex[2] = CubePoints[QuadIndices[i][0]][2] + ExtendDir[2] * dExtendDist;

			glVertex3dv(ExtendedVertex);
			glEnd();

		}
	}
	glPopMatrix();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}