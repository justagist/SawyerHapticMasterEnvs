#ifndef __COpenGLCamera_h_
#define __COpenGLCamera_h_

#include <glut.h>

#define CAMERA_DEFAULT_ZOOM_SPEED 0.01
#define CAMERA_DEFAULT_RADIUS 1.0
#define DEFAULT_MOVE_SPEED 0.01
#define DEFAULT_DISTANCE 0.7


#define CAMERA_LIFT_SPEED 0.1
#define CAMERA_TURN_SPEED 0.1
#define CAMERA_TWIST_SPEED 0.085
#define CAMERA_MOVE_SPEED 0.05

#define LEFT 0
#define RIGHT 1
#define EYE_SEP_STEP 0.0005
#define FOCUS_SEP_STEP 0.0005
#define EYE_CENTER_DIST_STEP 0.005
#define DEFAULT_EYE_SEP 0.01
#define DEFAULT_FOCUS_SEP 0.0


class COpenGLCamera
{

public:
   
   COpenGLCamera();
   ~COpenGLCamera();

   void Enable();
   void Disable();
   bool IsEnabled();
   void Move(int iX, int iY);
   
   void StartRotating(int iX, int iY);
   void EndRotating();
   bool IsRotating();

   void StartZooming(int iX, int iY);
   void EndZooming();
   bool IsZooming();

	GLdouble CameraX();
   GLdouble CameraY();
   GLdouble CameraZ();

private:
   bool m_bRotating;
   bool m_bZooming;
	
   bool m_bEnabled;

   double m_dAngleXY;
   double m_dAngleYZ;
   double m_dRadius;

   double m_dSpeedXY;
   double m_dSpeedYZ;
   double m_dSpeedZoom;

	
	int m_iLastX; // previous mouse X position
   int m_iLastY; // previous mouse Y position
	double m_dDeltaX; // Difference between current and previous mouse X position
	double m_dDeltaY; // Difference between current and previous mouse Y position

	GLdouble m_gldCameraX;
	GLdouble m_gldCameraY;
	GLdouble m_gldCameraZ;


};


class COpenGLPolarCamera
{

public:
   
   COpenGLPolarCamera();
   ~COpenGLPolarCamera();

   void Enable(void);
   void Disable(void);
   bool IsEnabled(void);

	void StartLiftAndTurn(int X, int Y);
	void StopLiftAndTurn(void);

	void StartTwistAndMove(int X, int Y);
	void StopTwistAndMove(void);

	void Move (int X, int Y);

	void View(void);

    void PrintState(void);

    void SetAzimuth(double dAz);
    void SetElevation(double dEl);
    void SetTwist(double dTw);
    void SetDistance(double dDi);

public:
//private :
	bool m_bEnabled;

	bool m_bTwistingAndMoving;
	bool m_bLiftingAndTurning;

	GLdouble m_dElevation;
	GLdouble m_dAzimuth;
	GLdouble m_dTwist;
	GLdouble m_dDistance;

	int m_iLastX; // previous mouse X position
    int m_iLastY; // previous mouse Y position

};



class COpenGLPolarStereoCamera
{

public:
   
   COpenGLPolarStereoCamera();
   ~COpenGLPolarStereoCamera();

   void Enable(void);
   void Disable(void);
   bool IsEnabled(void);

	void StartLiftAndTurn(int X, int Y);
	void StopLiftAndTurn(void);

	void StartTwistAndMove(int X, int Y);
	void StopTwistAndMove(void);

	void Move (int X, int Y);
	void View(int Eye);

	void BiggerEyeSep(void);
	void SmallerEyeSep(void);
	void BiggerFocusSep(void);
	void SmallerFocusSep(void);

	double GetEyeSep(void);
	double GetFocusSep(void);

    void SetAzimuth(double dAz);
    void SetElevation(double dEl);
    void SetTwist(double dTw);
    void SetDistance(double dDi);
    void SetEyeSeparation(double dEyeSep);
    void SetFocusSeparation(double dFocusSep);

    void PrintState(void);



public:
//private :
	bool m_bEnabled;

	bool m_bTwistingAndMoving;
	bool m_bLiftingAndTurning;

	GLdouble m_dElevation; //in degrees
	GLdouble m_dAzimuth;   //in degrees
	GLdouble m_dTwist;     //in degrees
	GLdouble m_dEl; // in radians
	GLdouble m_dAz; // in radians
	GLdouble m_dTw; // in radians
	GLdouble m_dDistance;
	

	GLdouble m_dLeftEye[3];
	GLdouble m_dRightEye[3];
	GLdouble m_dLeftFocus[3];
	GLdouble m_dRightFocus[3];

	GLdouble m_dEyeSeparation;
	GLdouble m_dFocusSeparation;
	GLdouble m_dEyeFocusDistance;

	int m_iLastX; // previous mouse X position
    int m_iLastY; // previous mouse Y position

};

#endif // __CFcsOPenGlCamera_h_