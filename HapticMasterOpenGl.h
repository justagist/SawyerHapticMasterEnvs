//---------------------------------------------------------------------
//                      H A P T I C   M A S T E R
//
// Included functions.
// 
//---------------------------------------------------------------------
#ifndef _HAPTIC_MASTER_OPENGL_H_
#define _HAPTIC_MASTER_OPENGL_H_

#include "HapticMaster.h"
#include "glut.h"


GLfloat XAxisAmbient[] = {1.00, 0.00, 0.00, 1.00};
GLfloat XAxisDiffuse[] = {1.00, 0.00, 0.00, 1.00};

GLfloat YAxisAmbient[] = {0.00, 1.00, 0.00, 1.00};
GLfloat YAxisDiffuse[] = {0.00, 1.00, 0.00, 1.00};

GLfloat ZAxisAmbient[] = {0.00, 0.00, 1.00, 1.00};
GLfloat ZAxisDiffuse[] = {0.00, 0.00, 1.00, 1.00};

GLfloat WorkSpaceAmbient[] = {1.00, 1.00, 0.00, 1.00};
GLfloat WorkSpaceDiffuse[] = {1.00, 1.00, 0.00, 1.00};

GLfloat LibSpecular[] = {1.00, 1.00, 1.00, 1.00};
GLfloat LibEmissive[] = {0.00, 0.00, 0.00, 1.00};
GLfloat LibShininess = {128.00};

GLfloat FrontArc[iNrSegments+1][3];
GLfloat BackArc[iNrSegments+1][3];

//---------------------------------------------------------------------
//                 O P E N   G L   M A T E R I A L S
//---------------------------------------------------------------------
GLfloat MagentaPlasticAmbient[] = {0.529411, 0.129411, 0.180392, 1.000000};
GLfloat MagentaPlasticDiffuse[] = {0.337500, 0.082500, 0.115000, 1.000000};
GLfloat MagentaPlasticSpecular[] = {1.000000, 1.000000, 1.000000, 1.000000};
GLfloat MagentaPlasticEmissive[] = {0.000000, 0.000000, 0.000000, 1.000000};
GLfloat MagentaPlasticShininess = {128.000000};

GLfloat GreenPlasticAmbient[] = {0.000000, 1.000000, 0.000000, 1.000000};
GLfloat GreenPlasticDiffuse[] = {1.000000, 1.000000, 0.000000, 1.000000};
GLfloat GreenPlasticSpecular[] = {1.000000, 1.000000, 1.000000, 1.000000};
GLfloat GreenPlasticEmissive[] = {0.000000, 0.000000, 0.000000, 1.000000};
GLfloat GreenPlasticShininess = {128.000000};


//---------------------------------------------------------------------
//                    X   A X I S   M A T E R I A L
//
// Create material for the X axis
//---------------------------------------------------------------------
void XAxisMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, XAxisAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, XAxisDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, LibSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, LibEmissive);
	glMaterialf(GL_FRONT, GL_SHININESS, LibShininess);
}

//---------------------------------------------------------------------
//                    Y   A X I S   M A T E R I A L
//
// Create material for the Y axis
//---------------------------------------------------------------------
void YAxisMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, YAxisAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, YAxisDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, LibSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, LibEmissive);
	glMaterialf(GL_FRONT, GL_SHININESS, LibShininess);
}

//---------------------------------------------------------------------
//                    Z   A X I S   M A T E R I A L
//
// Create material for the Z axis
//---------------------------------------------------------------------
void ZAxisMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ZAxisAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ZAxisDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, LibSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, LibEmissive);
	glMaterialf(GL_FRONT, GL_SHININESS, LibShininess);
}

//---------------------------------------------------------------------
//                   W O R K S P A C E   M A T E R I A L 
//
// Create material for the workspace visualization
//---------------------------------------------------------------------
void WorkSpaceMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, WorkSpaceAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, WorkSpaceDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, LibSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, LibEmissive);
	glMaterialf(GL_FRONT, GL_SHININESS, LibShininess);
}

//---------------------------------------------------------------------
//         W O R K S P A C E   M A T E R I A L   M A G E N T A
//
// Create material for the workspace visualization - in Magenta
//---------------------------------------------------------------------
void WorkSpaceMaterialMagenta()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, MagentaPlasticAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, MagentaPlasticDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, MagentaPlasticSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, MagentaPlasticEmissive);
	glMaterialf(GL_FRONT, GL_SHININESS, MagentaPlasticShininess);
}

//---------------------------------------------------------------------
//         W O R K S P A C E   M A T E R I A L   G R E E N 
//
// Create material for the workspace visualization - in Green
//---------------------------------------------------------------------
void WorkSpaceMaterialGreen()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, GreenPlasticAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, GreenPlasticDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, GreenPlasticSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, GreenPlasticEmissive);
	glMaterialf(GL_FRONT, GL_SHININESS, GreenPlasticShininess);
}

//---------------------------------------------------------------------
//                             D R A W   A X E S
//
// Draw the axes
//---------------------------------------------------------------------
void DrawAxes(void)
{
   
   glPushMatrix();

   //x-axis
   XAxisMaterial();
   glBegin(GL_LINES);
   glVertex3f(AxisLength, 0.0, 0.0);
   glVertex3f(-AxisLength, 0.0, 0.0);
   glEnd();

   //y-axis
   YAxisMaterial();
   glBegin(GL_LINES);
   glVertex3f(0.0, AxisLength, 0.0);
   glVertex3f(0.0, -AxisLength, 0.0);
   glEnd();


   //z-axis
   ZAxisMaterial();
   glBegin(GL_LINES);
   glVertex3f(0.0, 0.0, AxisLength);
   glVertex3f(0.0, 0.0, -AxisLength);
   glEnd();
  
   glPopMatrix();

}


//---------------------------------------------------------------------
//                     C R E A T E   P O I N T S
//
// Calculates the points for the arcs of the workspace
//---------------------------------------------------------------------
void CreatePoints(long dev)
{
	float fAngle;

	char outputString[200] = "";
   double Dummy = 0.0;

	// get the R workspace
   if ( haDeviceSendString( dev, "get workspace_r", outputString) ) {
      printf("--- ERROR: get workspace_r\n", outputString);
   }
   ParseFloatVec(outputString, MinRadius, CenterOffset, MaxRadius);
   MinRadius = CenterOffset + MinRadius;
   MaxRadius = CenterOffset + MaxRadius;
   CenterOffset *= -1;
   
	// get the Phi workspace
   if ( haDeviceSendString( dev, "get workspace_phi", outputString) ) {
      printf("--- ERROR: get workspace_phi\n", outputString);
   }
   ParseFloatVec(outputString, LeftSideArc, Dummy, RightSideArc);

	// get the Z workspace
   if ( haDeviceSendString( dev, "get workspace_z", outputString) ) {
      printf("--- ERROR: get workspace_z\n", outputString);
   }
   ParseFloatVec(outputString, PosHeight, Dummy, NegHeight);

   TotalHeight = PosHeight - NegHeight;

	float fAngleStart = LeftSideArc;
	float fAngleEnd = RightSideArc;
	float fAngleStep = (fAngleEnd-fAngleStart)/iNrSegments;
	int i;
   
	for(i=0; i<=iNrSegments; i++)
	{
		fAngle = fAngleStart + i * fAngleStep;
		FrontArc[i][X] = MinRadius*cos(fAngle);
		FrontArc[i][Y] = MinRadius*sin(fAngle);
		FrontArc[i][Z] = 0.0;

		BackArc[i][X] = MaxRadius*cos(fAngle);
		BackArc[i][Y] = MaxRadius*sin(fAngle);
		BackArc[i][Z] = 0.0;
	}	
}

//---------------------------------------------------------------------
//                        D R A W   W O R K S P A C E
//
// Draws the workspace
//---------------------------------------------------------------------
void DrawWorkspace(long dev, int color)
{
	int i;


	CreatePoints(dev);

   if ( color == 1 )
	   WorkSpaceMaterialMagenta();
   else if ( color == 2 ) 
	   WorkSpaceMaterialGreen();
   else
	   WorkSpaceMaterial();

	glPushMatrix();
	glTranslatef(CenterOffset, 0.0, PosHeight);
	glBegin(GL_LINE_STRIP);
	for (i=0; i<=iNrSegments; i++)
	{
		glVertex3f(FrontArc[i][X], FrontArc[i][Y], FrontArc[i][Z]);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(CenterOffset, 0.0, NegHeight);
	glBegin(GL_LINE_STRIP);
	for (i=0; i<=iNrSegments; i++)
	{
		glVertex3f(FrontArc[i][X], FrontArc[i][Y], FrontArc[i][Z]);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(CenterOffset, 0.0, PosHeight);
	glBegin(GL_LINE_STRIP);
	for (i=0; i<=iNrSegments; i++)
	{
		glVertex3f(BackArc[i][X], BackArc[i][Y], BackArc[i][Z]);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(CenterOffset, 0.0, NegHeight);
	glBegin(GL_LINE_STRIP);
	for (i=0; i<=iNrSegments; i++)
	{
		glVertex3f(BackArc[i][X], BackArc[i][Y], BackArc[i][Z]);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(CenterOffset, 0.0, 0.0);
	glRotatef(LeftSideArc*180/Pi, 0.0, 0.0, 1.0); //To the left side 0.5 rad
	glBegin(GL_LINE_LOOP);
		glVertex3f(MaxRadius, 0.0, NegHeight);
		glVertex3f(MinRadius, 0.0, NegHeight);
		glVertex3f(MinRadius, 0.0, PosHeight);
		glVertex3f(MaxRadius, 0.0, PosHeight);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(CenterOffset, 0.0, 0.0);
	glRotatef(RightSideArc*180/Pi, 0.0, 0.0, 1.0); //To the right only 0.4 rad
	glBegin(GL_LINE_LOOP);
		glVertex3f(MaxRadius, 0.0, NegHeight);
		glVertex3f(MinRadius, 0.0, NegHeight);
		glVertex3f(MinRadius, 0.0, PosHeight);
		glVertex3f(MaxRadius, 0.0, PosHeight);
	glEnd();
	glPopMatrix();
}



#endif














