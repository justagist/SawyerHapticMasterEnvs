#ifndef __DemoLib_h_
#define  __DemoLib_h_

#include "utils.h"
#include "Control.h"
#include "ClassCollection.h"
#include "HmWorkspace.h"
#include "OpenGLCamera.h"
#include "FcsBitmap.h"


extern bool GetIniFileString(char *pFileName, char *pKeyName, char *pFieldName, char *pDest, unsigned int *pDestSize);

extern void MyCube(double dSize);
extern void MyCube(double dSizeX, double dSizeY, double dSizeZ);
extern void MyCube(double dSizeX, double dSizeY, double dSizeZ, unsigned int uiSide);

// The 'old' workspace, keep it just for now
extern void DrawWorkspace(void);

#define DEG_PER_RAD 57.296
#define RAD_PER_DEG 0.01745


#endif  // __DemoLib_h_