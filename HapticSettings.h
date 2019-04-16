// HapticGraphics.h
// header for virtual environment

#include "HapticAPI.h"
#include "HapticMaster.h"

//---------------------------------------------------------------------
// O P E N G L   M A T E R I A L S
//---------------------------------------------------------------------
// EndEffector OpenGL Material Parameters.
GLfloat EndEffectorAmbient[] = {0.91, 0.44, 0.00, 1.00}; // {0.00, 0.00, 0.00, 1.00};
GLfloat EndEffectorDiffuse[] = {0.90, 0.38, 0.00, 1.00}; // {0.50, 0.50, 0.50, 1.00};

// Block OpenGL Material Parameters.
GLfloat BlockAmbient1[] = {0.00, 1.00, 1.00, 1.00};
GLfloat BlockAmbient2[] = {0.00, 1.00, 0.50, 1.00};
GLfloat BlockAmbient3[] = {0.50, 0.50, 1.00, 1.00};
GLfloat BlockAmbient4[] = {1.00, 0.50, 0.50, 1.00};
GLfloat BlockAmbient5[] = {0.00, 0.66, 0.60, 1.00};
GLfloat BlockAmbient6[] = {0.00, 0.66, 0.60, 1.00};

GLfloat BlockDiffuse[] = {0.50, 0.50, 0.50, 1.00};

// Floor Object OpenGL Material Parameters
GLfloat FloorAmbient[] = {0.00, 0.66, 0.60, 1.00}; 
GLfloat FloorDiffuse[] = {0.00, 0.66, 0.60, 0.25}; 

GLfloat FloorAmbient2[] = {1.00, 1.00, 1.00, 1.00};
GLfloat FloorDiffuse2[] = {1.00, 1.00, 1.00, 1.00};

// General OpenGL Material Parameters
GLfloat Specular[] = {1.00, 1.00, 1.00, 1.00};
GLfloat Emissive[] = {0.00, 0.00, 0.00, 1.00};
GLfloat Shininess = {128.00};

GLfloat SpecularOff[] = {0.00, 0.00, 0.00, 0.00};
GLfloat EmissiveOff[] = {0.50, 0.50, 0.50, 0.00};
GLfloat ShininessOff = {0.00};

//---------------------------------------------------------------------
//                 O B J E C T   P A R A M E T E R S
//---------------------------------------------------------------------
double blockPos1[3] = { 0.1,  0.0,  0.0};
double blockPos2[3] = {-0.1,  0.0,  0.0};
double blockPos3[3] = { 0.0,  0.1,  0.0};
double blockPos4[3] = { 0.0, -0.1,  0.0};
double blockPos5[3] = { 0.0,  0.0,  0.1};  // top
double blockPos6[3] = { 0.0,  0.0, -0.15}; // bottom

double blockSize1[3] = {0.199,0.199,0.5};
double blockSize2[3] = {0.199,0.199,0.5};
double blockSize3[3] = {0.199,0.199,0.5};
double blockSize4[3] = {0.199,0.199,0.5};
double blockSize5[3] = {0.4, 0.4, 0.02}; // top 
double blockSize6[3] = {0.4, 0.4, 0.02}; // bottom 


double blockStiffness = 20000;

// Location And Size Parameters For The Haptic Floor Plane Object
double FloorCenter[3] = {0.0, 0.0, -0.5};
double FloorSize[3] = {0.3, 0.3, 1.0}; 

double FloorCenterV[3] = {0.0, 0.0, -0.2}; 
double FloorSizeV[3] = {0.05, 0.05, 0.4}; 


//---------------------------------------------------------------------
//              E N D   E F F E C T O R   M A T E R I A L
//
// EndEffectorMaterial() Sets The Current OpenGl Material Paremeters. 
// Call This Function Prior To Drawing The EndEffector.
//---------------------------------------------------------------------
void EndEffectorMaterial()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, EndEffectorAmbient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, EndEffectorDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, Specular);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive);
   glMaterialf(GL_FRONT, GL_SHININESS, Shininess);
}

//---------------------------------------------------------------------
//                     B L O C K   M A T E R I A L
//
// BlockMaterial() Sets The Current OpenGl Material Paremeters. 
// Call This Function Prior To Drawing The Block.
//---------------------------------------------------------------------
void BlockMaterial1()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, BlockAmbient1);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, BlockDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularOff);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive);
   glMaterialf(GL_FRONT, GL_SHININESS, ShininessOff);
}
void BlockMaterial2()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, BlockAmbient2);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, BlockDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularOff);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive);
   glMaterialf(GL_FRONT, GL_SHININESS, ShininessOff);
}
void BlockMaterial3()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, BlockAmbient3);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, BlockDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularOff);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive);
   glMaterialf(GL_FRONT, GL_SHININESS, ShininessOff);
}
void BlockMaterial4()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, BlockAmbient4);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, BlockDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularOff);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive);
   glMaterialf(GL_FRONT, GL_SHININESS, ShininessOff);
}

void BlockMaterial5()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, BlockAmbient5);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, BlockDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularOff);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive);
   glMaterialf(GL_FRONT, GL_SHININESS, ShininessOff);
}

void BlockMaterial6()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, BlockAmbient6);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, BlockDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularOff);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive);
   glMaterialf(GL_FRONT, GL_SHININESS, ShininessOff);
}

//---------------------------------------------------------------------
// FLOOR MATERIAL
//
// FloorMaterial() Sets The Current OpenGl Material Parameters. 
// Call This Function Prior To Drawing The Floor Object.
//---------------------------------------------------------------------
void FloorMaterial()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, FloorAmbient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, FloorDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularOff); 
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive);
   glMaterialf(GL_FRONT, GL_SHININESS, ShininessOff);
}

void FloorMaterial2()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, FloorAmbient2);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, FloorDiffuse2);
   glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularOff); 
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive);
   glMaterialf(GL_FRONT, GL_SHININESS, ShininessOff);

}

//---------------------------------------------------------------------
//                        D R A W   B L O C K 
//
// This Function Is Called To Draw The Graphic Equivalent Of 
// The Haptic Block Object In OpenGl
//---------------------------------------------------------------------
void DrawBlock(void)
{
   // Draw Block 1
   glPushMatrix();
   glTranslatef(-blockPos1[1], blockPos1[0], blockPos1[PosZ]);
   glScalef(blockSize1[1], blockSize1[0], blockSize1[2]);
   BlockMaterial1();
   glutSolidCube(1.0);   
   glPopMatrix();

   // Draw Block 2
   glPushMatrix();
   glTranslatef(-blockPos2[1], blockPos2[0], blockPos2[PosZ]);
   glScalef(blockSize2[1], blockSize2[0], blockSize2[2]);
   BlockMaterial2();
   glutSolidCube(1.0);   
   glPopMatrix();

   // Draw Block 3
   glPushMatrix();
   glTranslatef(-blockPos3[1], blockPos3[0], blockPos3[PosZ]);
   glScalef(blockSize3[1], blockSize3[0], blockSize3[2]);
   BlockMaterial3();
   glutSolidCube(1.0);   
   glPopMatrix();

   // Draw Block 4
   glPushMatrix();
   glTranslatef(-blockPos4[1], blockPos4[0], blockPos4[PosZ]);
   glScalef(blockSize4[1], -blockSize4[0], blockSize4[2]);
   BlockMaterial4();
   glutSolidCube(1.0);   
   glPopMatrix();

   // Draw Block 5 // top
   glPushMatrix();
   glTranslatef(blockPos5[PosX], blockPos5[PosY], blockPos5[PosZ]);
   glScalef(blockSize5[0], blockSize5[1], blockSize5[2]);
   BlockMaterial5();
   glutSolidCube(1.0);   
   glPopMatrix();

   // Draw Block 6 // bottom
   glPushMatrix();
   glTranslatef(blockPos6[PosX], blockPos6[PosY], blockPos6[PosZ]);
   glScalef(blockSize6[0], blockSize6[1], blockSize6[2]);
   BlockMaterial6();
   glutSolidCube(1.0);
   glPopMatrix();

}

//---------------------------------------------------------------------
// DRAW FLOOR
//
// This Function Is Called To Draw The Graphic Equivalent Of 
// The Floor In OpenGl.
//---------------------------------------------------------------------

void DrawFloor(void)
{
   FloorMaterial();
   glTranslatef(FloorCenterV[0], FloorCenterV[1], FloorCenterV[2]);
   glScalef(FloorSizeV[0], FloorSizeV[1], FloorSizeV[2]);
   glutSolidCube(1.0);
}

void DrawFloor2(void) // wireframe
{
   FloorMaterial2();
   glPushMatrix();
   glTranslatef(FloorCenterV[0], FloorCenterV[1], FloorCenterV[2]);
   glScalef(FloorSizeV[0], FloorSizeV[1], FloorSizeV[2]);
   glutWireCube(1.0);
   glPopMatrix();
}

//---------------------------------------------------------------------
//                  D R A W   E N D   E F F E C T O R
//
// This Function Is Called To Draw The Graphic Equivalent Of 
// The EndEffector In OpenGl.
// The EndEffector Is Drawn At The Current Position
//---------------------------------------------------------------------
void DrawEndEffector(void)
{
   EndEffectorMaterial();
   glPushMatrix();
   glTranslatef(virtualPos[PosX], virtualPos[PosY], virtualPos[PosZ]);
   glutSolidSphere(0.015, 20, 20); // (0.005, 20, 20);
   glPopMatrix();
}

//---------------------------------------------------------------------
//                         I N I T   O P E N   G L
//
// This Function Initializes the OpenGl Graphics Engine
//---------------------------------------------------------------------
void InitOpenGl (void)
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
void Display (void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix ();
       
   // define eyepoint in such a way that
   // drawing can be done as in lab-frame rather than sgi-frame
   // (so X towards user, Z is up)
   // gluLookAt (1.0, 0.5, 0.5, 0.0, 0.0, -0.03, 0.0, 0.0, 1.0);
   // gluLookAt (0.3, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
   gluLookAt (1.5, 0.015, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
   glutPostRedisplay();

   DrawAxes();
   //DrawWorkspace(dev, 3);
   DrawEndEffector();
   // DrawBlock();
   DrawFloor2(); // wireframe
   DrawFloor();

   glPopMatrix ();
   glutSwapBuffers();
}

//---------------------------------------------------------------------
//                            R E S H A P E 
//
// The Function Is Called By OpenGL Whenever The Window Is Resized
//---------------------------------------------------------------------
void Reshape(int iWidth, int iHeight)
{
   glViewport (0, 0, (GLsizei)iWidth, (GLsizei)iHeight);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

   float fAspect = (float)iWidth/iHeight;
   gluPerspective (30.0, fAspect, 0.05, 20.0);            
 
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();
}


