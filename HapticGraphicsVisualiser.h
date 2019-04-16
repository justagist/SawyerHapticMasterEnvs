// HapticGraphics.h
// header for virtual environment

#include "HapticAPI.h"
#include "HapticMaster.h"
#include "HapticMasterOpenGl.h"
#include "glut.h"

#define PosX 0
#define PosY 1
#define PosZ 2


//---------------------------------------------------------------------
// O P E N G L   M A T E R I A L S
//---------------------------------------------------------------------
// EndEffector OpenGL Material Parameters.
GLfloat EndEffectorAmbient[] = {0.91, 0.44, 0.00, 1.00}; // {0.00, 0.00, 0.00, 1.00};
GLfloat EndEffectorDiffuse[] = {0.90, 0.38, 0.00, 1.00}; // {0.50, 0.50, 0.50, 1.00};

// Arrow OpenGL Material Parameters.
GLfloat ArrowAmbient[] = {1.00, 0.00, 1.00, 1.00};
GLfloat ArrowDiffuse[] = {0.97, 0.0, 0.97, 1.00};
//-------------------------------------------------------------------------------------------//
// Block OpenGL Material Parameters.
GLfloat BlockAmbient1[] = {0.00, 0.66, 0.60, 1.00};
GLfloat BlockAmbient2[] = {0.00, 0.66, 0.60, 1.00};
GLfloat BlockAmbient3[] = {0.50, 0.50, 1.00, 1.00};
GLfloat BlockAmbient4[] = {1.00, 0.50, 0.50, 1.00};
GLfloat BlockAmbient5[] = {0.00, 0.66, 0.60, 1.00};
GLfloat BlockAmbient6[] = {0.00, 0.66, 0.60, 1.00};
GLfloat BlockAmbient7[] = {0.00, 0.66, 0.60, 1.00};

GLfloat BlockDiffuse[] = {0.50, 0.50, 0.50, 1.00};
GLfloat BlockDiffuse7[] = {0.00, 0.80, 0.67, 0.58};

// Floor Object OpenGL Material Parameters
GLfloat FloorAmbient[] = {0.00, 0.66, 0.60, 1.00}; 
GLfloat FloorDiffuse[] = {0.00, 0.66, 0.60, 0.25}; 

GLfloat FloorAmbient2[] = {1.00, 1.00, 1.00, 1.00};
GLfloat FloorDiffuse2[] = {1.00, 1.00, 1.00, 1.00};

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
GLfloat SpecularOff[] = {0.00, 0.00, 0.00, 0.00};
GLfloat EmissiveOff[] = {0.50, 0.50, 0.50, 0.00};
GLfloat ShininessOff = {0.00};

//---------------------------------------------------------------------
//                 O B J E C T   P A R A M E T E R S
//---------------------------------------------------------------------

struct Block {
	double blockPos[3];
	double blockSize[3];
};

vector<Block> allBlocksList;

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

//---------------------------LAURA-------------------------------------//
//---------------------------------------------------------------------
//                     B L O C K   M A T E R I A L
//
// BlockMaterial() Sets The Current OpenGl Material Paremeters. 
// Call This Function Prior To Drawing The Block.
//---------------------------------------------------------------------
void BlockMaterial()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, BlockAmbient7);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, BlockDiffuse7);
   glMaterialfv(GL_FRONT, GL_SPECULAR, Specular7);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive7);
   glMaterialf(GL_FRONT, GL_SHININESS, Shininess7);
}

//----------------------------------------------------------------------------DESIGN

//---------------------------------------------------------------------
// DRAW FLOOR
//
// This Function Is Called To Draw The Graphic Equivalent Of 
// The Floor In OpenGl.
//---------------------------------------------------------------------
// Location And Size Parameters For The Haptic Floor Plane Object
double FloorCenter[3] = {0.0, 0.0, -0.5};
double FloorSize[3] = {0.3, 0.3, 1.0}; 

double FloorCenterV[3] = {0.0, 0.0, -0.2}; 
double FloorSizeV[3] = {0.05, 0.05, 0.4}; 
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
   glutSolidSphere(0.005, 20, 20); // (0.005, 20, 20);
   glPopMatrix();
}


//---------------------------------------------------------------------
//                        D R A W   A R R O W
//
// This Function Is Called To Draw one arrow in the magnetic field
//---------------------------------------------------------------------
void ArrowMaterial()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, ArrowAmbient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, ArrowDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, Specular);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive);
   glMaterialf(GL_FRONT, GL_SHININESS, Shininess);
}
void DrawArrow(double arrowXPos, double arrowYPos)
{
   ArrowMaterial();

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
void DrawMagneticField(void)
{
   for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
         double tempX = i * 0.15;
         double tempZ = j * 0.15;
         DrawArrow(tempX, tempZ);
      }
   }
}

//-------------------------------------------------------------------//

void DrawCubeGeneric(double size, double position[3], int rotation[3])
{
	BlockMaterial();
	glPushMatrix();

	if (rotation[0] != 0)
		glRotatef(rotation[0], 1.0, 0.0, 0.0);
	if (rotation[1] != 0)
		glRotatef(rotation[1], 0.0, 1.0, 0.0);
	if (rotation[2] != 0)
		glRotatef(rotation[2], 0.0, 0.0, 1.0);

	glTranslatef(position[PosX], position[PosY], position[PosZ]);
	//glScalef(size,size,size);
	BlockMaterial();
	glutSolidCube(size);
	
	glPopMatrix();
}

void createBlock(double position[3], double size[3])
{
	BlockMaterial();
	glPushMatrix();
	glTranslatef(position[PosX], position[PosY], position[PosZ]);
	glScalef(size[0], size[1], size[2]);
	BlockMaterial();
	glutSolidCube(1.0);
	glPopMatrix();
}

void SpringMaterial()
{
   glMaterialfv(GL_FRONT, GL_AMBIENT, SpringAmbient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, SpringDiffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, Specular);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emissive);
   glMaterialf(GL_FRONT, GL_SHININESS, Shininess);
}

void createSpring(double position[3])
{
	SpringMaterial();
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
   gluLookAt (1.0, 0.0, 0.0, 0.0, 0.0, -0.03, 0.0, 0.0, 1.0);
   // gluLookAt (0.3, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
   //gluLookAt (1.5, 0.015, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
   glutPostRedisplay();
   
   //DrawMagneticField();
   for (vector<Block>::iterator it = allBlocksList.begin(); it != allBlocksList.end(); ++it)
   {
	   createBlock(it->blockPos, it->blockSize);
   }
 /*  createBlock(blockPos1, blockSize1);
   createBlock(blockPos2, blockSize2);*/

   //createSpring(springPos);
   //DrawBlock1();
   //DrawBlock2();
   //DrawBlock3();
   //DrawBlock4();
   //DrawBlock5(); //blocco sopra
   //DrawBlock6();//blocco sotto
   
   //DrawBlock7();
   
   
   /*int rotCu_11[3] = { 0, 0, 0};
   DrawCubeGeneric(sizeCu_11, posCu_11, rotCu_11);

   
   int rotCu_12[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_12, posCu_12, rotCu_12);

   
   int rotCu_21[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_21, posCu_21, rotCu_21);

   
   int rotCu_22[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_22, posCu_22, rotCu_22);

   
   int rotCu_31[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_31, posCu_31, rotCu_31);

   
   int rotCu_32[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_32, posCu_32, rotCu_32);

   
   int rotCu_41[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_41, posCu_41, rotCu_41);

   
   int rotCu_42[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_42, posCu_42, rotCu_42);

   

   int rotCu_412[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_412, posCu_412, rotCu_412);

   
   int rotCu_422[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_422, posCu_422, rotCu_422);

   
   int rotCu_51[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_51, posCu_51, rotCu_51);*/
  
   /*
   int rotCu_52[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_52, posCu_52, rotCu_52);

   
   int rotCu_52small[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_52small, posCu_52small, rotCu_52small);

   
   int rotCu_61[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_61, posCu_61, rotCu_61);

   
   int rotCu_61small[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_61small, posCu_61small, rotCu_61small);

   
   int rotCu_62[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_62, posCu_62, rotCu_62);

   
   int rotCu_71[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_71, posCu_71, rotCu_71);

   int rotCu_72[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_72, posCu_72, rotCu_72);

   
   //int rotCu_72small[3] = { 0, 0, 0 };
   //DrawCubeGeneric(sizeCu_72small, posCu_72small, rotCu_72small);
   int rotCu_81[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_81, posCu_81, rotCu_81);

   int rotCu_82[3] = { 0, 0, 0 };
   DrawCubeGeneric(sizeCu_82, posCu_82, rotCu_82);
   
   */
   //DrawSpring();
   //DrawSpring1();
   DrawAxes();
  
   DrawEndEffector();
   
   
   //-------------------------------------------//

   //-------------------------------------------//
   //DrawBlock2();
   //DrawFloor2(); // wireframe
   //DrawFloor();
   
   
   //DrawMagneticField();
   
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
   //cout << "width " << iWidth << endl; 
   glViewport (-0, 0, (GLsizei)iWidth, (GLsizei)iHeight);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

   float fAspect = (float)iWidth/iHeight;
   gluPerspective (30.0, fAspect, 0.05, 20.0); 

   //gluLookAt (0.0, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
 
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();
}
