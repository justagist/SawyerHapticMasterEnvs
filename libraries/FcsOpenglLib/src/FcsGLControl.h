#ifndef __CFcsGLControl_h_
#define __CFcsGLControl_h_

#include <windows.h> 
#include <mmsystem.h>
#include <glut.h>
#include <stdio.h>
#include <vector>

using namespace std;

const int MAX_NAME_LENGHT = 128;
const int CONTROL_SPACE = 3;

const int PANEL_TITLE_HEIGHT = 20;

const int CHK_BOX_SPACE = 3;
const int CHK_BOX_SIZE = 12;

const int CHK_CHECK_SPACE = 2;
const int CHK_CHECK_SIZE = CHK_BOX_SIZE - 2*CHK_CHECK_SPACE;

const int SLD_TOTAL_HEIGHT = 27;
const int SLD_HEIGHT = 15;
const int SLD_SPACE = 2;

const int OPT_TITLE_HEIGHT = 15;
const int OPT_SPACE = 2;

const int GRAPH_TITLE_HEIGHT = 15;
const int GRAPH_SPACE = 2;
const int GRAPH_BOX_SIZE = 80;
const int GRAPH_GRID_X = 20;
const int GRAPH_GRID_Y = 8;
const int GRAPH_MAX_CHANNELS = 8;

const int MAX_SAMPLES = 250;

class CFcsGLControlPanel;
class CCheckBox;
class CSlider;
class COptionBox;
class CGraphChannel;
class CGraphBox;


class CControl
{
public:
	virtual void Display(void) = 0;
	
};

class CFcsGLControlPanelCollection
{
public:

    CFcsGLControlPanelCollection();
    ~CFcsGLControlPanelCollection();

    CFcsGLControlPanel* AddFcsGLControlPanel(char *pName="", int iPosH=0, int iPosV=0, int iSizeH=0, int iSizeV=0);

    bool Mouse(int Button, int State, int x, int y);
	bool Motion(int iX, int iY);
	bool PassiveMotion(int iX, int iY);

    CFcsGLControlPanel* GetCurrentPanel();

	void Display(void);

public:
    int m_iCurrentPanel;
    vector<CFcsGLControlPanel*> m_vFcsGLControlPanels;
    
};



class CFcsGLControlPanel
{
public:
	CFcsGLControlPanel(char *pName="", int iPosH=0, int iPosV=0, int iSizeH=0, int iSizeV=0);
	~CFcsGLControlPanel();

	CCheckBox* AddCheckBox(char *pName="", bool bVal=false, bool bEnabled=true, void (*pCallBack)(bool bNewVal)=NULL);
	CSlider* AddSlider(char *pName="", double dVal=0.0, double dMinVal=1.0, double dMaxVal=10.0, bool bEnabled=true, void (*pCallBack)(double bNewVal)=NULL);
	COptionBox* AddOptionBox(char *pName="", int iVal=0, bool bEnabled=true, void (*pCallBack)(int iNewVal)=NULL);
	CSlider* AddInfoSlider(char *pName="", double dVal=0.0, double dMinVal=1.0, double dMaxVal=10.0, bool bEnabled=true, void (*pCallBack)(double bNewVal)=NULL);
    CGraphBox* AddGraphBox(char *pName="", bool bEnabled=true, void (*pCallBack)(int iNewVal)=NULL);

	void SetVisible(bool bVisible);
	void Resize(int iPosH=0, int iPosV=0, int iSizeH=0, int iSizeV=0);

	bool Mouse(int Button, int State, int x, int y);
	bool Motion(int iX, int iY);
	bool PassiveMotion(int iX, int iY);

	void Display(void);

public:

	int m_iLeft;
	int m_iRight;
	int m_iTop;
	int m_iBottom;

	int m_iTitleLeft;
	int m_iTitleRight;
	int m_iTitleTop;
	int m_iTitleBottom;

	bool m_bVisible;
	bool m_bOver;  // Is the mouse over this control panels area
    bool m_bStick; // Overrule m_bOver, Control Panel is always visible
	bool m_bPlay;  // Play a sound when mouse over the Control Panel
	bool m_bEnablePlay;

	char m_sName[MAX_NAME_LENGHT];

	vector<CCheckBox*> m_vCheckBoxes;
	vector<CSlider*> m_vSliders;
	vector<COptionBox*> m_vOptionBoxes;
	vector<CSlider*> m_vInfoSliders;
    vector<CGraphBox* > m_vGraphBoxes;

};


class CCheckBox : public CControl
{
public:
	CCheckBox(char *pName="", int iPosH=0, int iPosV=0, bool bVal=false, bool bEnabled=true, void (*pCallBack)(bool bNewVal)=NULL);
	~CCheckBox();

	bool Mouse(int Button, int State, int x, int y);
	bool Motion(int iX, int iY);
	bool PassiveMotion(int iX, int iY);
	void Repos(int iPosH=0, int iPosV=0);
	void Enable(bool bEnable);
	void Display(void);
	void SetValue(bool bNewVal);
	

public:
	int m_iLeft;
	int m_iRight;
	int m_iTop;
	int m_iBottom;

	int m_iCheckLeft;
	int m_iCheckRight;
	int m_iCheckTop;
	int m_iCheckBottom;

	bool m_bValue;
	char m_sName[MAX_NAME_LENGHT];

	bool m_bEnabled;
	bool m_bOver;

	void (*mpf_CallBack)(bool bNewVal);


};


class CSlider : public CControl
{
public :
	CSlider(char *pName="", int iPosH=0, int iPosV=0, int iWidth=0, double dVal=0.0, double dMinVal=1.0, double dMaxVal=1.0, bool bEnabled=true, void (*pCallBack)(double bNewVal)=NULL);
	~CSlider();

	bool Mouse(int iButton, int iState, int iX, int iY);
	bool Motion(int iX, int iY);
	bool PassiveMotion(int iX, int iY);
	void Repos(int iPosH=0, int iPosV=0, int iWidth=0);
	void Enable(bool bEnable);
	void Display(void);
	void SetValue(double dNewVal);
	


public:

	int m_iLeft;
	int m_iRight;
	int m_iTop;
	int m_iBottom;

	int m_iValLeft;
	int m_iValRight;
	int m_iValTop;
	int m_iValBottom;

	int m_iValRightMax;

	double m_dPercPerPix;
	double m_dValPerPerc;
	int m_iPerc;
	
	double m_dMinVal;
	double m_dMaxVal;

	double m_dValue;
	char m_sName[MAX_NAME_LENGHT];

	bool m_bEnabled;
	bool m_bSliding;
	bool m_bOver;

	void (*mpf_CallBack)(double dNewVal);

};


class COptionBox : public CControl
{
public:

	COptionBox(char *pName="", int iPosH=0, int iPosV=0, int iWidth=0, int iVal=0, bool bEnabled=true, void (*pCallBack)(int bNewVal)=NULL);
	~COptionBox();
	bool Mouse(int iButton, int iState, int iX, int iY);
	bool Motion(int iX, int iY);
	bool PassiveMotion(int iX, int iY);
	void Repos(int iPosH=0, int iPosV=0, int iWidth=0);
	void Enable(bool bEnable);
	void Display(void);
	CCheckBox* AddOption(char *pName);
	void SetValue(int NewVal);
	void SetValue(int NewVal, bool bCallBack);

public:

	int m_iLeft;
	int m_iRight;
	int m_iTop;
	int m_iBottom;

	int m_iTitleLeft;
	int m_iTitleRight;
	int m_iTitleTop;
	int m_iTitleBottom;

	vector<CCheckBox*> m_vOptions;
	void (*mpf_CallBack)(int iNewVal);
	char m_sName[MAX_NAME_LENGHT];
	int m_iValue;
	bool m_bEnabled;
	bool m_bOver;

};


class CGraphChannel
{
public:
    CGraphChannel(char* pName="No Name");
    ~CGraphChannel();
    void AddSample(float fNewSample);
    void SetScale(float fScale);

public:
    char m_sName[MAX_NAME_LENGHT];
    
    unsigned int m_iFillNr;
    unsigned int m_uiSamples[MAX_SAMPLES];
    float m_fScale;
};


class CGraphBox : public CControl
{
public :

    CGraphBox(char *pName="", int iPosH=0, int iPosV=0, int iWidth=0, bool bEnabled=true, void (*pCallBack)(int bNewVal)=NULL);
	~CGraphBox();

    bool Mouse(int iButton, int iState, int iX, int iY);
	bool Motion(int iX, int iY);
	bool PassiveMotion(int iX, int iY);
	void Repos(int iPosH=0, int iPosV=0, int iWidth=0);
	void Enable(bool bEnable);
	void Display(void);

    void AddChannel(char* pChannelName="No Name");
    void AddSample(char* pChannelName, float fNewSample);

    void SetScale(char* pChannelName, float fChannelMin, float fChannelMax);

public:

	int m_iLeft;
	int m_iRight;
	int m_iTop;
	int m_iBottom;

	int m_iTitleLeft;
	int m_iTitleRight;
	int m_iTitleTop;
	int m_iTitleBottom;

    int m_iGraphLeft;
    int m_iGraphRight;
    int m_iGraphTop;
    int m_iGraphBottom;

    int m_iGraphWidth;
    int m_iGraphHeight;
    int m_iGraphCenterH;
    int m_iGraphCenterV;
    float m_fGraphStepH;

	void (*mpf_CallBack)(int iNewVal);
	char m_sName[MAX_NAME_LENGHT];
	bool m_bEnabled;
	bool m_bOver;

private:

    double m_dMinX, m_dMaxX;
    vector<CGraphChannel *> m_vChannels;
};


class CSeparator
{

};

#endif // __CControl_h_