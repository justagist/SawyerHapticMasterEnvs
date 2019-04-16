#include <math.h>
#include <stdio.h>

#include "FcsGLCamera.h"


CFcsGLCamera::CFcsGLCamera()
{
   m_bRotating = false;
   m_bZooming = false;
   m_bEnabled = false;

   m_dAngleXY = 0.0;
   m_dAngleYZ = 0.0;
   m_dRadius = CAMERA_DEFAULT_RADIUS;

   m_dSpeedXY = DEFAULT_MOVE_SPEED;
   m_dSpeedYZ = DEFAULT_MOVE_SPEED;
   m_dSpeedZoom = CAMERA_DEFAULT_ZOOM_SPEED;

   m_iLastX = 0;
   m_iLastY = 0;

	m_gldCameraX = CAMERA_DEFAULT_RADIUS;
	m_gldCameraY = 0.0;
	m_gldCameraZ = 0.0;
}

CFcsGLCamera::~CFcsGLCamera()
{
}


void CFcsGLCamera::Enable()
{
   m_bEnabled = true;
}

void CFcsGLCamera::Disable()
{
   m_bEnabled = false;
}


bool CFcsGLCamera::IsEnabled()
{
   return m_bEnabled;
}

void CFcsGLCamera::Move(int iX, int iY)
{
   if (m_bEnabled)
	{
		if (m_bRotating)
		{
			m_dAngleXY += m_dSpeedXY*(iX - m_iLastX);
			m_dAngleYZ += m_dSpeedYZ*(iY - m_iLastY);
		}
		else if (m_bZooming)
		{
			m_dRadius += m_dSpeedZoom*(iY- m_iLastY);
		}

		m_iLastX = iX;
		m_iLastY = iY;

		m_gldCameraX = m_dRadius*cos(m_dAngleXY)*cos(m_dAngleYZ);
		m_gldCameraY = m_dRadius*sin(m_dAngleXY)*cos(m_dAngleYZ);
		m_gldCameraZ = m_dRadius*sin(m_dAngleYZ);

        

	}
}



void CFcsGLCamera::StartRotating(int iX, int iY)
{
   if (m_bEnabled)
	{
		m_iLastX = iX;
		m_iLastY = iY;
		m_bRotating = true;
        
	}
}

void CFcsGLCamera::EndRotating()
{
   if (m_bEnabled)
		m_bRotating = false;

   
}

bool CFcsGLCamera::IsRotating()
{
   return m_bRotating;
}


void CFcsGLCamera::StartZooming(int iX, int iY)
{
   if (m_bEnabled)
	{
		m_iLastX = iX;
		m_iLastY = iY;
		m_bZooming = true;
        
	}
}

void CFcsGLCamera::EndZooming()
{
   if (m_bEnabled)
		m_bZooming = false;

   
}

bool CFcsGLCamera::IsZooming()
{
   return m_bZooming;
}


GLdouble CFcsGLCamera::CameraX()
{
   return m_gldCameraX;
}

GLdouble CFcsGLCamera::CameraY()
{
   return m_gldCameraY;
}

GLdouble CFcsGLCamera::CameraZ()
{
   return m_gldCameraZ;
}


// OpenGl Polar Camera

CFcsGLPolarCamera::CFcsGLPolarCamera()
{
	m_bEnabled = true;

	m_bTwistingAndMoving = false;
	m_bLiftingAndTurning = false;

	m_dElevation = 0.0;
	m_dAzimuth = 0.0;
	m_dTwist = 0.0;
	m_dDistance = 1.0;
}

CFcsGLPolarCamera::~CFcsGLPolarCamera()
{
}


void CFcsGLPolarCamera::Enable(void)
{
	m_bEnabled = true;
}

void CFcsGLPolarCamera::Disable(void)
{
	m_bEnabled = false;
}

bool CFcsGLPolarCamera::IsEnabled()
{
	return m_bEnabled;
}

void CFcsGLPolarCamera::StartLiftAndTurn(int X, int Y)
{
	m_bLiftingAndTurning = true;

	m_iLastX = X;
	m_iLastY = Y;
}

void CFcsGLPolarCamera::StopLiftAndTurn(void)
{
	m_bLiftingAndTurning = false;
    PrintState();
}

void CFcsGLPolarCamera::StartTwistAndMove(int X, int Y)
{
	m_bTwistingAndMoving = true;

	m_iLastX = X;
	m_iLastY = Y;
}

void CFcsGLPolarCamera::StopTwistAndMove(void)
{
	m_bTwistingAndMoving = false;
    PrintState();
}


void CFcsGLPolarCamera::Move(int X, int Y)
{
	if(m_bEnabled)
	{
		if(m_bLiftingAndTurning)
		{
			m_dAzimuth += CAMERA_TURN_SPEED*(X - m_iLastX);
			m_dElevation += CAMERA_LIFT_SPEED*(Y - m_iLastY);
		}
		else if (m_bTwistingAndMoving)
		{
			m_dDistance += CAMERA_MOVE_SPEED*(Y- m_iLastY);
			m_dTwist += CAMERA_TWIST_SPEED*(X - m_iLastX);
		}
		
		m_iLastX = X;
		m_iLastY = Y;
	}
}


void CFcsGLPolarCamera::View(void)
{
	glRotated(-90.0, 0.0, 1.0, 0.0);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	glTranslated(-m_dDistance, 0.0, 0.0);
	glRotated(-m_dTwist, 1.0, 0.0, 0.0);
	glRotated(-m_dElevation, 0.0, 1.0, 0.0);
	glRotated(m_dAzimuth, 0.0, 0.0, -1.0);
}

void CFcsGLPolarCamera::PrintState(void)
{
    printf("Azimuth          := %f\n", m_dAzimuth);
    printf("Elevation        := %f\n", m_dElevation);
    printf("Twist            := %f\n", m_dTwist);
    printf("Distance         := %f\n", m_dDistance);

}


void CFcsGLPolarCamera::SetAzimuth(double dAz)
{
    m_dAzimuth = dAz;
    PrintState();

}

void CFcsGLPolarCamera::SetElevation(double dEl)
{
    m_dElevation = dEl;
    PrintState();
}

void CFcsGLPolarCamera::SetTwist(double dTw)
{
    m_dTwist = dTw;
    PrintState();
}

void CFcsGLPolarCamera::SetDistance(double dDi)
{
    m_dDistance = dDi;
    PrintState();
}



// OpenGL Polar Stereo Camera

CFcsGLPolarStereoCamera::CFcsGLPolarStereoCamera()
{
    int i;

	m_bEnabled = true;

	m_bTwistingAndMoving = false;
	m_bLiftingAndTurning = false;

	m_dElevation = 0.0;
	m_dAzimuth = 0.0;
	m_dTwist = 0.0;
	m_dDistance = DEFAULT_DISTANCE;

	m_dEyeSeparation = DEFAULT_EYE_SEP;
	m_dFocusSeparation = DEFAULT_FOCUS_SEP;

	m_dEl = m_dElevation * (3.14159265358979/180.0);
	m_dAz = m_dAzimuth * (3.14159265358979/180.0);
	m_dTw = m_dTwist * (3.14159265358979/180.0);

    for(i=0; i<3; i++)
    {
        m_dLeftEye[i] = 0.0;
	    m_dRightEye[i] = 0.0;
	    m_dLeftFocus[i] = 0.0;
	    m_dRightFocus[i] = 0.0;
    }

}

CFcsGLPolarStereoCamera::~CFcsGLPolarStereoCamera()
{
}


void CFcsGLPolarStereoCamera::Enable(void)
{
	m_bEnabled = true;
}

void CFcsGLPolarStereoCamera::Disable(void)
{
	m_bEnabled = false;
}

bool CFcsGLPolarStereoCamera::IsEnabled()
{
	return m_bEnabled;
}

void CFcsGLPolarStereoCamera::StartLiftAndTurn(int X, int Y)
{
	m_bLiftingAndTurning = true;

	m_iLastX = X;
	m_iLastY = Y;
}

void CFcsGLPolarStereoCamera::StopLiftAndTurn(void)
{
	m_bLiftingAndTurning = false;
    PrintState();
}

void CFcsGLPolarStereoCamera::StartTwistAndMove(int X, int Y)
{
	m_bTwistingAndMoving = true;

	m_iLastX = X;
	m_iLastY = Y;
}

void CFcsGLPolarStereoCamera::StopTwistAndMove(void)
{
	m_bTwistingAndMoving = false;
    PrintState();
}


void CFcsGLPolarStereoCamera::Move(int X, int Y)
{
	if(m_bEnabled)
	{
		if(m_bLiftingAndTurning)
		{
			m_dElevation += CAMERA_TURN_SPEED*(X - m_iLastX);
			m_dAzimuth += CAMERA_LIFT_SPEED*(Y - m_iLastY);
		}
		else if (m_bTwistingAndMoving)
		{
			m_dDistance += CAMERA_MOVE_SPEED*(Y- m_iLastY);
			m_dTwist += CAMERA_TWIST_SPEED*(X - m_iLastX);
		}
		
		m_iLastX = X;
		m_iLastY = Y;

		m_dEl = m_dElevation * (3.14159265358979/180.0);
		m_dAz = m_dAzimuth * (3.14159265358979/180.0);
		m_dTw = m_dTwist * (3.14159265358979/180.0);

	}
}


void CFcsGLPolarStereoCamera::View(int Eye)
{
	
	if(Eye == LEFT)
	{
		
		m_dLeftEye[0] = m_dDistance*cos(m_dEl);
		m_dLeftEye[1] = m_dDistance*sin(m_dEl)*cos(m_dAz);
		m_dLeftEye[2] = m_dDistance*sin(m_dEl)*sin(m_dAz);

		gluLookAt (m_dLeftEye[0], m_dLeftEye[1]-m_dEyeSeparation, m_dLeftEye[2], m_dLeftFocus[0], m_dLeftFocus[1]-m_dFocusSeparation, m_dLeftFocus[2], 0.0, 0.0, 1.0);

		
	}
	else if (Eye == RIGHT)
	{

		m_dRightEye[0] = m_dDistance*cos(m_dEl);
		m_dRightEye[1] = m_dDistance*sin(m_dEl)*cos(m_dAz);
		m_dRightEye[2] = m_dDistance*sin(m_dEl)*sin(m_dAz);

		gluLookAt (m_dRightEye[0], m_dRightEye[1]+m_dEyeSeparation, m_dRightEye[2], m_dRightFocus[0], m_dRightFocus[1]+m_dFocusSeparation, m_dRightFocus[2], 0.0, 0.0, 1.0);
	}
}


void CFcsGLPolarStereoCamera::BiggerEyeSep(void)
{
	m_dEyeSeparation += EYE_SEP_STEP;
    PrintState();
}

void CFcsGLPolarStereoCamera::SmallerEyeSep(void)
{
	m_dEyeSeparation -= EYE_SEP_STEP;
    PrintState();
}

void CFcsGLPolarStereoCamera::BiggerFocusSep(void)
{
	m_dFocusSeparation += FOCUS_SEP_STEP;
    PrintState();
}

void CFcsGLPolarStereoCamera::SmallerFocusSep(void)
{
	m_dFocusSeparation -= FOCUS_SEP_STEP;
    PrintState();
}

double CFcsGLPolarStereoCamera::GetEyeSep(void)
{
	return m_dEyeSeparation;
}

double CFcsGLPolarStereoCamera::GetFocusSep(void)
{
	return m_dFocusSeparation;
}

void CFcsGLPolarStereoCamera::PrintState(void)
{
    printf("Eye Separation   := %f\n", m_dEyeSeparation);
    printf("Focus Separation := %f\n", m_dFocusSeparation);
    printf("Azimuth          := %f\n", m_dAzimuth);
    printf("Elevation        := %f\n", m_dElevation);
    printf("Twist            := %f\n", m_dTwist);
    printf("Distance         := %f\n", m_dDistance);

}

void CFcsGLPolarStereoCamera::SetAzimuth(double dAz)
{
    m_dAzimuth = dAz;
	m_dAz = m_dAzimuth * (3.14159265358979/180.0);
    PrintState();

}

void CFcsGLPolarStereoCamera::SetElevation(double dEl)
{
    m_dElevation = dEl;
    m_dEl = m_dElevation * (3.14159265358979/180.0);
    PrintState();
}

void CFcsGLPolarStereoCamera::SetTwist(double dTw)
{
    m_dTwist = dTw;
	m_dTw = m_dTwist * (3.14159265358979/180.0);
    PrintState();
}

void CFcsGLPolarStereoCamera::SetDistance(double dDi)
{
    m_dDistance = dDi;
    PrintState();
}


void CFcsGLPolarStereoCamera::SetEyeSeparation(double dEyeSep)
{
   m_dEyeSeparation = dEyeSep; 
   PrintState();
}

void CFcsGLPolarStereoCamera::SetFocusSeparation(double dFocusSep)
{
    m_dFocusSeparation = dFocusSep;
    PrintState();

}