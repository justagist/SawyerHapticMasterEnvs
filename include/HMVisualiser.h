//*********************************************************************
//
// package : SawyerHapticMasterEnvs
// file    : HMVisualiser.h
// purpose : Functions, utilities and global variables for 
//           visualising environments
//
// author  : Saif Sidhik
// email   : sxs1412@bham.ac.uk
//
//*********************************************************************
#ifndef _HM_VISUALISER_
#define _HM_VISUALISER_

#include "HapticAPI.h"
#include "HapticMaster.h"
#include "HapticMasterOpenGl.h"
#include "glut.h"
#include <vector>
using namespace std;

#define PosX 0
#define PosY 1
#define PosZ 2

namespace HMVisualiser {
//---------------------------------------------------------------------
// O P E N G L   M A T E R I A L S
//---------------------------------------------------------------------
// EndEffector OpenGL Material Parameters.
GLfloat EndEffectorAmbient[] = {0.91, 0.44, 0.00, 1.00}; // {0.00, 0.00, 0.00, 1.00};
GLfloat EndEffectorDiffuse[] = {0.90, 0.38, 0.00, 1.00}; // {0.50, 0.50, 0.50, 1.00};

// Arrow OpenGL Material Parameters.
GLfloat ArrowAmbient[] = {1.00, 0.00, 1.00, 1.00};
GLfloat ArrowDiffuse[] = {0.97, 0.0, 0.97, 1.00};

GLfloat BlockAmbient[] = {0.00, 0.66, 0.60, 1.00};

GLfloat BlockDiffuse[] = {0.00, 0.80, 0.67, 0.58};

// Spring OpenGL Material Parameters.
GLfloat SpringAmbient[] = {1.00, 0.00, 1.00, 1.00};
GLfloat SpringDiffuse[] = {0.97, 0.0, 0.97, 1.00};

// General OpenGL Material Parameters
GLfloat Specular[] = {1.00, 1.00, 1.00, 1.00};
GLfloat Emissive[] = {0.00, 0.00, 0.00, 1.00};
GLfloat Shininess = {128.00};
GLfloat Specular7[] = {1.00, 1.00, 1.00, 1.00};
GLfloat Emissive7[] = {0.00, 0.00, 0.00, 0.2500};
GLfloat Shininess7 = {128.00};


//---------------------------------------------------------------------
//                 O B J E C T   P A R A M E T E R S
//---------------------------------------------------------------------
double virtualPos[3] = {0.0, 0.0, 0.0};

struct Block {
	double blockPos[3];
	double blockSize[3];
};
struct Spring {
	double SpringPos[3];
};

vector<Block> allBlocksList;
vector<Spring> allSpringsList;

void addBlock(double pos[3], double size[3])
{	
	Block newBlock = Block();
	newBlock.blockPos[0] = pos[0];
	newBlock.blockPos[1] = pos[1];
	newBlock.blockPos[2] = pos[2];

	newBlock.blockSize[0] = size[0];
	newBlock.blockSize[1] = size[1];
	newBlock.blockSize[2] = size[2];

	allBlocksList.push_back(newBlock);
}

void addSpring(double pos[3])
{
	Spring newSpring = Spring();
	newSpring.SpringPos[0] = pos[0];
	newSpring.SpringPos[1] = pos[1];
	newSpring.SpringPos[2] = pos[2];

	allSpringsList.push_back(newSpring);
}


//---------------------------------------------------------------------
//              E N D   E F F E C T O R   M A T E R I A L
//
// endEffectorMaterial() Sets The Current OpenGl Material Paremeters. 
// Call This Function Prior To Drawing The EndEffector.
//---------------------------------------------------------------------
void endEffectorMaterial()
{

   glMaterialfv(GL_FRONT, GL_AMBIENT, EndEffectorAmbient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, EndEffectorDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, Specular);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive);
   glMaterialf(GL_FRONT, GL_SHININESS, Shininess);
}

//---------------------------LAURA-------------------------------------//
//---------------------------------------------------------------------
//                     B L O C K   M A T E R I A L
//
// blockMaterial() Sets The Current OpenGl Material Paremeters. 
// Call This Function Prior To Drawing The Block.
//---------------------------------------------------------------------
void blockMaterial()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, BlockAmbient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, BlockDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, Specular7);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive7);
   glMaterialf(GL_FRONT, GL_SHININESS, Shininess7);
}



//---------------------------------------------------------------------
//                  D R A W   E N D   E F F E C T O R
//
// This Function Is Called To Draw The Graphic Equivalent Of 
// The EndEffector In OpenGl.
// The EndEffector Is Drawn At The Current Position
//---------------------------------------------------------------------
void drawEndEffector(void)
{
   endEffectorMaterial();
   glPushMatrix();
   glTranslatef(virtualPos[PosX], virtualPos[PosY], virtualPos[PosZ]);
   glutSolidSphere(0.005, 20, 20); // (0.005, 20, 20);
   glPopMatrix();
}


//---------------------------------------------------------------------
//                        D R A W   A R R O W
//
// This Function Is Called To Draw one arrow in the magnetic field
//---------------------------------------------------------------------
void arrowMaterial()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, ArrowAmbient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, ArrowDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, Specular);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive);
   glMaterialf(GL_FRONT, GL_SHININESS, Shininess);
}
void arawArrow(double arrowXPos, double arrowYPos)
{
   arrowMaterial();

   glBegin(GL_LINES);
      //glVertex3f(arrowXPos, 0.07, arrowZPos);
      //glVertex3f(arrowXPos, 0.07 + 0.1, arrowZPos);

	  glVertex3f(-arrowXPos, -arrowYPos, -0.07);
      glVertex3f(-arrowXPos, -arrowYPos, -0.07 - 0.1);
   glEnd();

   
   glBegin(GL_LINES);
      glVertex3f(-arrowXPos, -arrowYPos - 0.01, -0.07 - 0.02);
      glVertex3f(-arrowXPos, -arrowYPos - 0.01, -0.07 - 0.02);
   glEnd();
    
   glBegin(GL_LINES);
      glVertex3f(-arrowXPos, -arrowYPos - 0.01, -0.07 - 0.02);
      glVertex3f(-arrowXPos, -arrowYPos, -0.07);
   glEnd();

   glBegin(GL_LINES);
      glVertex3f(-arrowXPos, -arrowYPos + 0.01, -0.07 - 0.02);
      glVertex3f(-arrowXPos, -arrowYPos, -0.07);
   glEnd();
   
   
}

//---------------------------------------------------------------------
//              D R A W   M A G N E T I C   F I E L D
//
// This Function Is Called To Draw The magnetic field
//---------------------------------------------------------------------
void drawMagneticField(void)
{
   for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
         double tempX = i * 0.15;
         double tempZ = j * 0.15;
         arawArrow(tempX, tempZ);
      }
   }
}


void drawBlock(double position[3], double size[3])
{
	blockMaterial();
	glPushMatrix();
	glTranslatef(position[PosX], position[PosY], position[PosZ]);
	glScalef(size[0], size[1], size[2]);
	blockMaterial();
	glutSolidCube(1.0);
	glPopMatrix();
}

void springMaterial()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, SpringAmbient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, SpringDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, Specular);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive);
   glMaterialf(GL_FRONT, GL_SHININESS, Shininess);
}

void drawSpring(double position[3])
{
	springMaterial();
	glLineWidth(10);
	glBegin(GL_LINES);
		glVertex3f(virtualPos[PosX], virtualPos[PosY], virtualPos[PosZ]);
		glVertex3f(position[PosX], position[PosY], position[PosZ]);
	glEnd();
	glLineWidth(1);
}

//---------------------------------------------------------------------
//                         I N I T   O P E N   G L
//
// This Function Initializes the OpenGl Graphics Engine
//---------------------------------------------------------------------
void initOpenGl (void)
{

   glShadeModel(GL_SMOOTH);

   glLoadIdentity();
   
   GLfloat GrayLight[] = {0.75, 0.75, 0.75, 1.0};
   GLfloat LightPosition[] = {1.0, 2.0, 1.0, 0.0};
   GLfloat LightDirection[] = {0.0, 0.0, -1.0, 0.0};

   glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
   glLightfv(GL_LIGHT0, GL_AMBIENT, GrayLight);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, GrayLight);
   glLightfv(GL_LIGHT0, GL_SPECULAR, GrayLight);
   
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   
   glEnable (GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
   glClearColor(0.0, 0.0, 0.3, 0.0);
}

//---------------------------------------------------------------------
//                           D I S P L A Y
//
// This Function Is Called By OpenGL To Redraw The Scene
// Here's Where You Put The EndEffector And Block Drawing FuntionCalls
//---------------------------------------------------------------------
void display (void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix ();
       
   // define eyepoint in such a way that
   // drawing can be done as in lab-frame rather than sgi-frame
   // (so X towards user, Z is up)
   gluLookAt (1.0, 0.0, 0.0, 0.0, 0.0, -0.03, 0.0, 0.0, 1.0);
   glutPostRedisplay();
   
   for (vector<Block>::iterator it = allBlocksList.begin(); it != allBlocksList.end(); ++it)
   {
	   drawBlock(it->blockPos, it->blockSize);
   }

   for (vector<Spring>::iterator it = allSpringsList.begin(); it != allSpringsList.end(); ++it)
   {
	   drawSpring(it->SpringPos);
   }
 
   DrawAxes();
  
   drawEndEffector();
   
   glPopMatrix ();
   glutSwapBuffers();
}

//---------------------------------------------------------------------
//                            R E S H A P E 
//
// The Function Is Called By OpenGL Whenever The Window Is Resized
//---------------------------------------------------------------------
void reshape(int iWidth, int iHeight)
{
   //cout << "width " << iWidth << endl; 
   glViewport (-0, 0, (GLsizei)iWidth, (GLsizei)iHeight);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

   float fAspect = (float)iWidth/iHeight;
   gluPerspective (30.0, fAspect, 0.05, 20.0); 
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();
}

void graphicsThread(void *pParam)
{
   // ----- OpenGL Initialization Calls
   char *myargv [1];
   int myargc=1;
   myargv [0]=strdup ("SawyerVarImpedance");
   glutInit(&myargc, myargv);
   glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (800, 600);

   // ----- Create The OpenGlWindow

   glutCreateWindow ("Sawyer Virtual Environment");
   haSendCommand( device, "get modelpos", response );
   HMVisualiser::initOpenGl();

   glutReshapeFunc(HMVisualiser::reshape);
   glutDisplayFunc(HMVisualiser::display);
   glutMainLoop();

   _endthread();
}

}

#endif