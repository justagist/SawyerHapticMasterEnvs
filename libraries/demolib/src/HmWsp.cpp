#include <glut.h>
#include "math.h"

const int iNrSegments = 20;
const int iNrSubDivs = 20;
const double AxisLength = 0.25; //meter

const double dLeftSideArc = -0.5; //radians
const double dRightSideArc = 0.5; //radians

const double dMinRadius = 0.28; //meter
const double dMaxRadius = 0.64; //meter

const double dCenterOffset = -0.45; //meter

const double dTotalHeight = 0.4; //meter
const double dPosHeight = 0.2; //meter
const double dNegHeight = -0.2; //meter


double FrontArc[iNrSegments+1][3];
double BackArc[iNrSegments+1][3];

double Arcs[iNrSubDivs+1][iNrSegments+1][3];
double dHeightStep = dTotalHeight / iNrSubDivs;

typedef enum Axes{X=0, Y, Z};

const double Pi = 3.1415926535897932384626433832795;

GLdouble HM_WS_BLACK[] = {0.0, 0.0, 0.0, 1.0};
GLdouble HM_WS_GREY[] = {0.4, 0.4, 0.4, 1.0};
GLdouble HM_WS_BKG[] = {0.4745, 0.6118, 0.6902, 0.30};

void CreatePoints(void)
{
	double dAngle;
	double dAngleStart = dLeftSideArc;
	double dAngleEnd = dRightSideArc;
	double dAngleStep = (dAngleEnd-dAngleStart)/iNrSegments;
	double dRadius;
	double dRadiusStep = (dMaxRadius - dMinRadius) / iNrSubDivs;
	int i, j;

	
	for(i=0; i<=iNrSubDivs; i++)
	{
		for(j=0; j<=iNrSegments; j++)
		{
			dRadius = dMinRadius + i * dRadiusStep;
			dAngle = dAngleStart + j * dAngleStep;

			Arcs[i][j][X] = dRadius*cos(dAngle);
			Arcs[i][j][Y] = dRadius*sin(dAngle);
			Arcs[i][j][Z] = 0.0;
		}
	}
}


void DrawWorkspace(void)
{
	int i, j;


    glDisable(GL_LIGHTING);
    //glDisable(GL_CULL_FACE);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_LINE_SMOOTH);

    CreatePoints();
	
	
	// Draw the top arcs
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dPosHeight);
	for(j=0; j<=iNrSubDivs; j++)
	{
		if(	(j==0) || (j==iNrSubDivs) || (j==iNrSubDivs/2))
			glColor4dv(HM_WS_BLACK);
		else
			glColor4dv(HM_WS_GREY);

		glBegin(GL_LINE_STRIP);
		for (i=0; i<=iNrSegments; i++)
		{
			glVertex3f(Arcs[j][i][X], Arcs[j][i][Y], Arcs[j][i][Z]);
		}
		glEnd();
	}
	glPopMatrix();

	// Draw the bottom arcs
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dNegHeight);
	for(j=0; j<=iNrSubDivs; j++)
	{
		if(	(j==0) || (j==iNrSubDivs) || (j==iNrSubDivs/2))
			glColor4dv(HM_WS_BLACK);
		else
			glColor4dv(HM_WS_GREY);

		glBegin(GL_LINE_STRIP);
		for (i=0; i<=iNrSegments; i++)
		{
			glVertex3f(Arcs[j][i][X], Arcs[j][i][Y], Arcs[j][i][Z]);
		}
		glEnd();
	}
	glPopMatrix();


	// Draw the back arcs
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dNegHeight);
	for(j=0; j<=iNrSubDivs; j++)
	{
		if(	(j==0) || (j==iNrSubDivs) || (j==iNrSubDivs/2))
			glColor4dv(HM_WS_BLACK);
		else
			glColor4dv(HM_WS_GREY);

		glBegin(GL_LINE_STRIP);
		for (i=0; i<=iNrSegments; i++)
		{
			glVertex3f(Arcs[0][i][X], Arcs[0][i][Y], Arcs[0][i][Z]+j*dHeightStep);
		}
		glEnd();
	}
	glPopMatrix();

	// Draw back vertical subdivs
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dPosHeight);
	for(i=0; i<=iNrSubDivs; i++)
	{
		if(	(i==0) || (i==iNrSubDivs) || (i==iNrSubDivs/2))
			glColor4dv(HM_WS_BLACK);
		else
			glColor4dv(HM_WS_GREY);

		glBegin(GL_LINES);
			glVertex3f(Arcs[0][i][X], Arcs[0][i][Y], Arcs[0][i][Z]);
			glVertex3f(Arcs[0][i][X], Arcs[0][i][Y], Arcs[0][0][Z]-dTotalHeight);
		glEnd();
	}
	glPopMatrix();


	// Draw the Left Side Vertical subdivs
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dPosHeight);
	for(i=0; i<=iNrSubDivs; i++)
	{
		if(	(i==0) || (i==iNrSubDivs) || (i==iNrSubDivs/2))
			glColor4dv(HM_WS_BLACK);
		else
			glColor4dv(HM_WS_GREY);

		glBegin(GL_LINES);
			glVertex3f(Arcs[i][0][X], Arcs[i][0][Y], Arcs[i][0][Z]);
			glVertex3f(Arcs[i][0][X], Arcs[i][0][Y], Arcs[i][0][Z]-dTotalHeight);
		glEnd();
	}
	glPopMatrix();

	// Draw the Right Side Vertical subdivs
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dPosHeight);
	for(i=0; i<=iNrSubDivs; i++)
	{
		if(	(i==0) || (i==iNrSubDivs) || (i==iNrSubDivs/2))
			glColor4dv(HM_WS_BLACK);
		else
			glColor4dv(HM_WS_GREY);

		glBegin(GL_LINES);
			glVertex3f(Arcs[i][iNrSegments][X], Arcs[i][iNrSegments][Y], Arcs[i][iNrSegments][Z]);
			glVertex3f(Arcs[i][iNrSegments][X], Arcs[i][iNrSegments][Y], Arcs[i][iNrSegments][Z]-dTotalHeight);
		glEnd();
	}
	glPopMatrix();

	// Draw the Top subdivs
	
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dPosHeight);
	for(i=0; i<=iNrSubDivs; i++)
	{
		if(	(i==0) || (i==iNrSubDivs) || (i==iNrSubDivs/2))
			glColor4dv(HM_WS_BLACK);
		else
			glColor4dv(HM_WS_GREY);

		glBegin(GL_LINES);
			glVertex3f(Arcs[0][i][X], Arcs[0][i][Y], Arcs[0][i][Z]);
			glVertex3f(Arcs[iNrSubDivs][i][X], Arcs[iNrSubDivs][i][Y], Arcs[iNrSubDivs][i][Z]);
		glEnd();
	}
	glPopMatrix();

	// Draw the bottom subdivs
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dNegHeight);
	for(i=0; i<=iNrSubDivs; i++)
	{
		if(	(i==0) || (i==iNrSubDivs) || (i==iNrSubDivs/2))
			glColor4dv(HM_WS_BLACK);
		else
			glColor4dv(HM_WS_GREY);

		glBegin(GL_LINES);
			glVertex3f(Arcs[0][i][X], Arcs[0][i][Y], Arcs[0][i][Z]);
			glVertex3f(Arcs[iNrSubDivs][i][X], Arcs[iNrSubDivs][i][Y], Arcs[iNrSubDivs][i][Z]);
		glEnd();
	}
	glPopMatrix();
	
	
	// Draw the Left Side Horizontal subdivs
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dNegHeight);
	for(i=0; i<=iNrSubDivs; i++)
	{
		if(	(i==0) || (i==iNrSubDivs) || (i==iNrSubDivs/2))
			glColor4dv(HM_WS_BLACK);
		else
			glColor4dv(HM_WS_GREY);

		glBegin(GL_LINES);
			glVertex3f(Arcs[0][0][X], Arcs[0][0][Y], Arcs[0][0][Z] + i*dHeightStep);
			glVertex3f(Arcs[iNrSubDivs][0][X], Arcs[iNrSubDivs][0][Y], Arcs[iNrSubDivs][0][Z] + i*dHeightStep);
		glEnd();
	}
	glPopMatrix();

	// Draw the Right Side Horizontal subdivs
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dNegHeight);
	for(i=0; i<=iNrSubDivs; i++)
	{
		if(	(i==0) || (i==iNrSubDivs) || (i==iNrSubDivs/2))
			glColor4dv(HM_WS_BLACK);
		else
			glColor4dv(HM_WS_GREY);

		glBegin(GL_LINES);
			glVertex3f(Arcs[0][iNrSegments][X], Arcs[0][iNrSegments][Y], Arcs[0][iNrSegments][Z] + i*dHeightStep);
			glVertex3f(Arcs[iNrSubDivs][iNrSegments][X], Arcs[iNrSubDivs][iNrSegments][Y], Arcs[iNrSubDivs][iNrSegments][Z] + i*dHeightStep);
		glEnd();
	}
	glPopMatrix();

    
    /*
	// Draw Left Side Transparent Fill
	glColor4dv(HM_WS_BKG);
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dNegHeight);
	glBegin(GL_POLYGON);
		glVertex3f(Arcs[0][0][X], Arcs[0][0][Y], Arcs[0][0][Z]);
		glVertex3f(Arcs[0][0][X], Arcs[0][0][Y], Arcs[0][0][Z]+dTotalHeight);
		glVertex3f(Arcs[iNrSubDivs][0][X], Arcs[iNrSubDivs][0][Y], Arcs[iNrSubDivs][0][Z] + dTotalHeight);
		glVertex3f(Arcs[iNrSubDivs][0][X], Arcs[iNrSubDivs][0][Y], Arcs[iNrSubDivs][0][Z]);
	glEnd();
	glPopMatrix();

	// Draw Right Side Transparent Fill
	glColor4dv(HM_WS_BKG);
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dNegHeight);
	glBegin(GL_POLYGON);
		glVertex3f(Arcs[0][iNrSegments][X], Arcs[0][iNrSegments][Y], Arcs[0][iNrSegments][Z]);
		glVertex3f(Arcs[0][iNrSegments][X], Arcs[0][iNrSegments][Y], Arcs[0][iNrSegments][Z]+dTotalHeight);
		glVertex3f(Arcs[iNrSubDivs][iNrSegments][X], Arcs[iNrSubDivs][iNrSegments][Y], Arcs[iNrSubDivs][iNrSegments][Z] + dTotalHeight);
		glVertex3f(Arcs[iNrSubDivs][iNrSegments][X], Arcs[iNrSubDivs][iNrSegments][Y], Arcs[iNrSubDivs][iNrSegments][Z]);
	glEnd();
	glPopMatrix();

	// Draw Bottom Transparent Fill
	glColor4dv(HM_WS_BKG);
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dNegHeight);
	for(i=0; i<iNrSegments; i++)
	{
		glBegin(GL_POLYGON);
			glVertex3f(Arcs[0][i][X], Arcs[0][i][Y], Arcs[0][i][Z]);
			glVertex3f(Arcs[0][i+1][X], Arcs[0][i+1][Y], Arcs[0][i+1][Z]);
			glVertex3f(Arcs[iNrSubDivs][i+1][X], Arcs[iNrSubDivs][i+1][Y], Arcs[iNrSubDivs][i+1][Z]);
			glVertex3f(Arcs[iNrSubDivs][i][X], Arcs[iNrSubDivs][i][Y], Arcs[iNrSubDivs][i][Z]);
		glEnd();
	}
	glPopMatrix();

	// Draw Top Transparent Fill
	glColor4dv(HM_WS_BKG);
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dPosHeight);
	for(i=0; i<iNrSegments; i++)
	{
		glBegin(GL_POLYGON);
			glVertex3f(Arcs[0][i][X], Arcs[0][i][Y], Arcs[0][i][Z]);
			glVertex3f(Arcs[0][i+1][X], Arcs[0][i+1][Y], Arcs[0][i+1][Z]);
			glVertex3f(Arcs[iNrSubDivs][i+1][X], Arcs[iNrSubDivs][i+1][Y], Arcs[iNrSubDivs][i+1][Z]);
			glVertex3f(Arcs[iNrSubDivs][i][X], Arcs[iNrSubDivs][i][Y], Arcs[iNrSubDivs][i][Z]);
		glEnd();
	}
	glPopMatrix();

	// Draw Back Transparent Fill
	glColor4dv(HM_WS_BKG);
	glPushMatrix();
	glTranslatef(dCenterOffset, 0.0, dNegHeight);
	for(i=0; i<iNrSegments; i++)
	{
		glBegin(GL_POLYGON);
			glVertex3f(Arcs[0][i][X], Arcs[0][i][Y], Arcs[0][i][Z]);
			glVertex3f(Arcs[0][i][X], Arcs[0][i][Y], Arcs[0][i][Z]+dTotalHeight);
			glVertex3f(Arcs[0][i+1][X], Arcs[0][i+1][Y], Arcs[0][i+1][Z]+dTotalHeight);
			glVertex3f(Arcs[0][i+1][X], Arcs[0][i+1][Y], Arcs[0][i+1][Z]);
		glEnd();
	}
	glPopMatrix();
    */
    

	glDisable (GL_LINE_SMOOTH);
	glEnable(GL_LIGHTING);
    //glEnable(GL_CULL_FACE);
    //glDisable(GL_BLEND);
}