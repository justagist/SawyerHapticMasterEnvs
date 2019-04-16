#include <glut.h>

#include "FcsOpenglLib.h"

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


void XAxisMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, XAxisAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, XAxisDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, LibSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, LibEmissive);
	glMaterialf(GL_FRONT, GL_SHININESS, LibShininess);
}


void YAxisMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, YAxisAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, YAxisDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, LibSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, LibEmissive);
	glMaterialf(GL_FRONT, GL_SHININESS, LibShininess);
}


void ZAxisMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ZAxisAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ZAxisDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, LibSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, LibEmissive);
	glMaterialf(GL_FRONT, GL_SHININESS, LibShininess);
}

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