#include "Control.h"


GLdouble CP_BKG_COLOR[] = {0.4745, 0.6118, 0.6902, 0.85};
GLdouble CP_TXT_COLOR[] = {0.999, 0.999, 0.999, 1.00};
GLdouble CP_LIN_COLOR[] = {0.0078, 0.4588, 0.7176, 0.85};

/*
GLdouble CP_WHITE[] = {0.999, 0.999, 0.999, 1.00};
GLfloat CP_RED[] = {1.0, 0.0, 0.0, 1.0};
GLfloat CP_GREEN[] = {0.0, 1.0, 0.0, 1.0};
GLfloat CP_BLUE[] = {0.0, 0.0, 1.0, 1.0};
GLfloat CP_BLACK[] = {0.0, 0.0, 0.0, 1.0};
*/

GLdouble CTRL_OUT_COLOR[] = {0.0078, 0.4588, 0.7176, 0.85};
GLdouble CTRL_SEL_COLOR[] = {0.999, 0.999, 0.999, 1.00};
GLdouble CTRL_VAL_COLOR[] = {0.0078, 0.4588, 0.7176, 0.85};
GLdouble CTRL_TXT_COLOR[] = {0.999, 0.999, 0.999, 1.00};
GLdouble CTRL_TXT_COLOR_DISABLED[] = {0.699, 0.699, 0.699, 1.00};


GLfloat CHANNEL_COLORS_BRIGHT[8][4] = { {1.00, 0.00, 0.00, 1.00}, // Red
                                        {0.00, 1.00, 0.00, 1.00}, // Green 
                                        {0.00, 0.00, 1.00, 1.00}, // Blue
                                        {1.00, 1.00, 0.00, 1.00}, // Yellow
                                        {1.00, 0.00, 1.00, 1.00}, // Magenta
                                        {0.00, 1.00, 1.00, 1.00}, // Cyan
                                        {1.00, 1.00, 1.00, 1.00}, // White
                                        {0.00, 0.00, 0.00, 1.00} }; // Black

GLfloat CHANNEL_COLORS_DIM[8][4] = { {1.00, 0.00, 0.00, 0.20}, // Red
                                     {0.00, 1.00, 0.00, 0.10}, // Green 
                                     {0.00, 0.00, 1.00, 0.20}, // Blue
                                     {1.00, 1.00, 0.00, 0.20}, // Yellow
                                     {1.00, 0.00, 1.00, 0.20}, // Magenta
                                     {0.00, 1.00, 1.00, 0.20}, // Cyan
                                     {1.00, 1.00, 1.00, 0.20}, // White
                                     {0.00, 0.00, 0.00, 0.20} }; // Black

bool bBrightChannelColors = false;


enum CHAN_COLORS { RED=0, GREEN, BLUE, YELLOW, MAGENTA, CYAN, BLACK, WHITE};


CControlPanelCollection::CControlPanelCollection()
{
    m_iCurrentPanel = -1;
}

CControlPanelCollection::~CControlPanelCollection()
{
    unsigned int i;

    for(i=0; i<m_vControlPanels.size(); i++)
        if(m_vControlPanels[i] != NULL)
            delete m_vControlPanels[i];
}

CControlPanel* CControlPanelCollection::AddControlPanel(char *pName, int iPosH, int iPosV, int iSizeH, int iSizeV)
{
    CControlPanel* pCp = NULL;

    pCp = new CControlPanel(pName, iPosH, iPosV, iSizeH, iSizeV);

    if(pCp != NULL)
        m_vControlPanels.push_back(pCp);

    return pCp;
}

bool CControlPanelCollection::Mouse(int Button, int State, int x, int y)
{
    bool bRet;
    unsigned int i;


    for(i=0; i<m_vControlPanels.size(); i++)
    {
        bRet = m_vControlPanels[i]->Mouse(Button, State, x, y);
        if(bRet)
        {
            m_iCurrentPanel = i;
            break;
        }
        else
            m_iCurrentPanel = -1;
    }

    return bRet;
}

bool CControlPanelCollection::Motion(int iX, int iY)
{
    bool bRet;
    unsigned int i;

    for(i=0; i<m_vControlPanels.size(); i++)
    {
        bRet = m_vControlPanels[i]->Motion(iX, iY);
        if(bRet)
        {
            m_iCurrentPanel = i;
            break;
        }
        else
            m_iCurrentPanel = -1;
    }

    return bRet;
}

bool CControlPanelCollection::PassiveMotion(int iX, int iY)
{
    bool bRet;
    unsigned int i;

    for(i=0; i<m_vControlPanels.size(); i++)
    {
        bRet = m_vControlPanels[i]->PassiveMotion(iX, iY);
        if(bRet)
        {
            m_iCurrentPanel = i;
            break;
        }
        else
            m_iCurrentPanel = -1;
    }

    return bRet;
}

void CControlPanelCollection::Display(void)
{
    unsigned int i;

    for(i=0; i<m_vControlPanels.size(); i++)
        if(m_vControlPanels[i] != NULL)
            m_vControlPanels[i]->Display();
}

CControlPanel* CControlPanelCollection::GetCurrentPanel()
{
    if( (m_iCurrentPanel < 0) || (m_iCurrentPanel > (m_vControlPanels.size() - 1)) )
        return NULL;
    else
        return m_vControlPanels[m_iCurrentPanel];
}


CControlPanel::CControlPanel(char *pName, int iPosH, int iPosV, int iSizeH, int iSizeV)
{
	m_iLeft = iPosH;
	m_iRight = iPosH+iSizeH;
	m_iTop = iPosV;
	m_iBottom = iPosV-iSizeV;

	m_iTitleLeft = m_iLeft + CONTROL_SPACE;
	m_iTitleRight = m_iRight - CONTROL_SPACE;
	m_iTitleTop = m_iTop - CONTROL_SPACE;
	m_iTitleBottom = m_iTitleTop - PANEL_TITLE_HEIGHT;

	strcpy(m_sName, pName);

	m_bVisible = true;
	m_bOver = false;
    m_bStick = false;
    m_bPlay = false;
	m_bEnablePlay = true;

	
	
}

CControlPanel::~CControlPanel()
{
	int i;

	for(i=0; i<m_vCheckBoxes.size(); i++)
	{
		if(m_vCheckBoxes[i] != NULL)
			delete m_vCheckBoxes[i];
	}

	for(i=0; i<m_vSliders.size(); i++)
	{
		if(m_vSliders[i] != NULL)
			delete m_vSliders[i];
	}

	for(i=0; i<m_vInfoSliders.size(); i++)
	{
		if(m_vInfoSliders[i] != NULL)
			delete m_vInfoSliders[i];
	}

	for(i=0; i<m_vOptionBoxes.size(); i++)
	{
		if(m_vOptionBoxes[i] != NULL)
			delete m_vOptionBoxes[i];
	}
}

CCheckBox* CControlPanel::AddCheckBox(char *pName, bool bVal, bool bEnabled, void (*pCallBack)(bool bNewVal))
{
	int iPosH, iPosV;
	CCheckBox *pC; 
		
	iPosH = m_iLeft+CONTROL_SPACE;

    if(m_vCheckBoxes.size() > 0)
		iPosV = m_vCheckBoxes[m_vCheckBoxes.size()-1]->m_iBottom - CONTROL_SPACE;

	iPosV = m_iTop - CONTROL_SPACE - m_vCheckBoxes.size()*(CHK_BOX_SIZE+CONTROL_SPACE);
		
	pC = new CCheckBox(pName, iPosH, iPosV, bVal, bEnabled, pCallBack);

	if(pC != NULL)
		m_vCheckBoxes.push_back(pC);

	return pC;

}

CSlider* CControlPanel::AddSlider(char *pName, double dVal, double dMinVal, double dMaxVal, bool bEnabled, void (*pCallBack)(double bNewVal))
{
	int iWidth, iPosH, iPosV;
		
	iPosH = m_iLeft+CONTROL_SPACE;
	iPosV = m_vCheckBoxes[m_vCheckBoxes.size()-1]->m_iBottom - CONTROL_SPACE - (m_vSliders.size()+0.5)*(SLD_TOTAL_HEIGHT + CONTROL_SPACE);
	iWidth = m_iRight - m_iLeft - 2*CONTROL_SPACE - 1;

	CSlider *pS = new CSlider(pName, iPosH, iPosV, iWidth, dVal, dMinVal, dMaxVal, bEnabled, pCallBack);

	if(pS != NULL)
		m_vSliders.push_back(pS);

	return pS;
}


COptionBox* CControlPanel::AddOptionBox(char *pName, int iVal, bool bEnabled, void (*pCallBack)(int bNewVal))
{
	int iPosH, iPosV, iWidth;

	iPosH = m_iLeft + CONTROL_SPACE;
	
	if(m_vOptionBoxes.size() > 0)
		iPosV = m_vOptionBoxes[m_vOptionBoxes.size()-1]->m_iBottom - CONTROL_SPACE;
	else
        if(m_vSliders.size() > 0)
		    iPosV = m_vSliders[m_vSliders.size()-1]->m_iBottom - CONTROL_SPACE;
        else
            iPosV = - CONTROL_SPACE;

	iWidth = m_iRight - m_iLeft - 2*CONTROL_SPACE - 1;

	COptionBox *pO = new COptionBox(pName, iPosH, iPosV, iWidth, iVal, bEnabled, pCallBack);

	if(pO != NULL)
		m_vOptionBoxes.push_back(pO);

	return pO;

}


CSlider* CControlPanel::AddInfoSlider(char *pName, double dVal, double dMinVal, double dMaxVal, bool bEnabled, void (*pCallBack)(double bNewVal))
{
	int iPosH, iPosV, iWidth;
		
	iPosH = m_iLeft+CONTROL_SPACE;
	iPosV = m_vOptionBoxes[m_vOptionBoxes.size()-1]->m_iBottom - CONTROL_SPACE - (m_vInfoSliders.size()+0.5)*(SLD_TOTAL_HEIGHT + CONTROL_SPACE);
	iWidth = m_iRight - m_iLeft - 2*CONTROL_SPACE - 1;

	CSlider *pS = new CSlider(pName, iPosH, iPosV, iWidth, dVal, dMinVal, dMaxVal, false, pCallBack);

	if(pS != NULL)
		m_vInfoSliders.push_back(pS);

	return pS;
}


CGraphBox* CControlPanel::AddGraphBox(char *pName, bool bEnabled, void (*pCallBack)(int iNewVal))
{
	int iPosH, iPosV, iWidth;

    iPosH = m_iLeft + CONTROL_SPACE;
    
    if(m_vGraphBoxes.size() > 0)
		iPosV = m_vGraphBoxes[m_vGraphBoxes.size()-1]->m_iBottom - CONTROL_SPACE;
    else
        iPosV = m_iTitleBottom - 2 * CONTROL_SPACE ;

	iWidth = m_iRight - m_iLeft - 2*CONTROL_SPACE - 1;

	CGraphBox *pG = new CGraphBox(pName, iPosH, iPosV, iWidth, bEnabled, pCallBack);

	if(pG != NULL)
		m_vGraphBoxes.push_back(pG);

	return pG;

}


void CControlPanel::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

void CControlPanel::Resize(int iPosH, int iPosV, int iSizeH, int iSizeV)
{
	int i, iH, iV, iW;

	m_iLeft = iPosH;
	m_iRight = iPosH+iSizeH;
	m_iTop = iPosV;
	m_iBottom = iPosV-iSizeV;

	m_iTitleLeft = m_iLeft + CONTROL_SPACE;
	m_iTitleRight = m_iRight - CONTROL_SPACE;
	m_iTitleTop = m_iTop - CONTROL_SPACE;
	m_iTitleBottom = m_iTitleTop - PANEL_TITLE_HEIGHT;

	for(i=0; i<m_vCheckBoxes.size(); i++)
	{
		iH = m_iLeft+CONTROL_SPACE;
		iV = m_iTitleBottom - CONTROL_SPACE - i*(CHK_BOX_SIZE+CONTROL_SPACE);

		if(m_vCheckBoxes[i] != NULL)
			m_vCheckBoxes[i]->Repos(iH, iV);
	}

	for(i=0; i<m_vSliders.size(); i++)
	{
		iH = m_iLeft+CONTROL_SPACE;
		iV = m_vCheckBoxes[m_vCheckBoxes.size()-1]->m_iBottom - CONTROL_SPACE - (i+0.5)*(SLD_TOTAL_HEIGHT + CONTROL_SPACE);
		iW = m_iRight - m_iLeft - 2*CONTROL_SPACE - 1;

		if(m_vSliders[i] != NULL)
			m_vSliders[i]->Repos(iH, iV, iW);
	}

	for(i=0; i<m_vOptionBoxes.size(); i++)
	{
		iH = m_iLeft+CONTROL_SPACE;

		if(i > 0)
		{
			if(m_vOptionBoxes[i-1] != NULL)
				iV = m_vOptionBoxes[i-1]->m_iBottom - CONTROL_SPACE;
		}
		else
		{
			if(m_vSliders[m_vSliders.size()-1] != NULL)
				iV = m_vSliders[m_vSliders.size()-1]->m_iBottom - CONTROL_SPACE;

			iV -= (i*(CHK_BOX_SIZE + CHK_BOX_SPACE) + CONTROL_SPACE);
		}

		iW = m_iRight - m_iLeft - 2*CONTROL_SPACE - 1;

		if(m_vOptionBoxes[i] != NULL)
			m_vOptionBoxes[i]->Repos(iH, iV, iW);
	}

	for(i=0; i<m_vInfoSliders.size(); i++)
	{
		iH = m_iLeft+CONTROL_SPACE;
		
		if(m_vOptionBoxes[m_vOptionBoxes.size()-1] != NULL)
			iV = m_vOptionBoxes[m_vOptionBoxes.size()-1]->m_iBottom - CONTROL_SPACE - (i+0.5)*(SLD_TOTAL_HEIGHT + CONTROL_SPACE);
		
		iW = m_iRight - m_iLeft - 2*CONTROL_SPACE - 1;

		if(m_vInfoSliders[i] != NULL)
			m_vInfoSliders[i]->Repos(iH, iV, iW);
	}

    for(i=0; i<m_vGraphBoxes.size(); i++)
    {
        iH = m_iLeft+CONTROL_SPACE;

        iW = m_iRight - m_iLeft - 2*CONTROL_SPACE - 1;

		iV = m_iTitleBottom - CONTROL_SPACE - i*(GRAPH_BOX_SIZE + GRAPH_TITLE_HEIGHT + 3*CONTROL_SPACE);
        
        if(m_vGraphBoxes[i] != NULL)
            m_vGraphBoxes[i]->Repos(iH, iV, iW);
    }


}

bool CControlPanel::Mouse(int iButton, int iState, int iX, int iY)
{
	int i;
	bool bHit=false;

	if( (iX > m_iLeft) && (iX < m_iRight) && (iY < m_iTop) && (iY > m_iBottom) )
	{
		// Check if the title bar was clicked
        if( (iX > m_iTitleLeft) && (iX < m_iTitleRight) && (iY < m_iTitleTop) && (iY > m_iTitleBottom) )
        {
            if( (iButton == GLUT_LEFT_BUTTON) && (iState == GLUT_UP) )
            {
                m_bStick = !m_bStick;
                bHit = true;
            }
        }

		if(!bHit)
        {
            for(i=0; i<m_vCheckBoxes.size(); i++)
		    {
			    if(m_vCheckBoxes[i] != NULL)
				    bHit = m_vCheckBoxes[i]->Mouse(iButton, iState, iX, iY);

			    if(bHit)
				    break;
		    }
        }

		if(!bHit)
		{
			for(i=0; i<m_vSliders.size(); i++)
			{
				if(m_vSliders[i] != NULL)
					bHit = m_vSliders[i]->Mouse(iButton, iState, iX, iY);

				if(bHit)
					break;
			}
		}

		if(!bHit)
		{
			for(i=0; i<m_vOptionBoxes.size(); i++)
			{
				if(m_vOptionBoxes[i] != NULL)
					bHit = m_vOptionBoxes[i]->Mouse(iButton, iState, iX, iY);

				if(bHit)
					break;
			}
		}


		bHit = true;
		glutPostRedisplay();
	}

	return bHit;
}

bool CControlPanel::Motion(int iX, int iY)
{
	int i;
	bool bHit=false;

	if( (iX > m_iLeft) && (iX < m_iRight) && (iY < m_iTop) && (iY > m_iBottom) )
	{
		
		for(i=0; i<m_vCheckBoxes.size(); i++)
		{
			if(m_vCheckBoxes[i] != NULL)
				bHit = m_vCheckBoxes[i]->Motion(iX, iY);

			if(bHit)
				break;
		}

		if(!bHit)
		{
			for(i=0; i<m_vSliders.size(); i++)
			{
				if(m_vSliders[i] != NULL)
					bHit = m_vSliders[i]->Motion(iX, iY);

				if(bHit)
					break;
			}
		}

		if(!bHit)
		{
			for(i=0; i<m_vOptionBoxes.size(); i++)
			{
				if(m_vOptionBoxes[i] != NULL)
					bHit = m_vOptionBoxes[i]->Motion(iX, iY);

				if(bHit)
					break;
			}
		}

		bHit = true;
		glutPostRedisplay();
	}

	return bHit;
}

bool CControlPanel::PassiveMotion(int iX, int iY)
{
	bool bHit = false;
	int i;

	if( (iX > m_iLeft) && (iX < m_iRight) && (iY < m_iTop) && (iY > m_iBottom) )
	{
        bHit = true;
		m_bOver = true;
		m_bPlay = m_bEnablePlay;
	}
	else
	{
        bHit = false;
		m_bOver = false;
		m_bEnablePlay = true;
	}


    for(i=0; i<m_vCheckBoxes.size(); i++)
	{
		if(m_vCheckBoxes[i] != NULL)
			m_vCheckBoxes[i]->PassiveMotion(iX, iY);
	}

	for(i=0; i<m_vSliders.size(); i++)
	{
		if(m_vSliders[i] != NULL)
			m_vSliders[i]->PassiveMotion(iX, iY);
	}

	for(i=0; i<m_vOptionBoxes.size(); i++)
	{
		if(m_vOptionBoxes[i] != NULL)
			m_vOptionBoxes[i]->PassiveMotion(iX, iY);
	}

	for(i=0; i<m_vInfoSliders.size(); i++)
	{
		if(m_vInfoSliders[i] != NULL)
			m_vInfoSliders[i]->PassiveMotion(iX, iY);
	}

	return bHit;
}


void CControlPanel::Display(void)
{
    
	if(m_bVisible)
	{
		int i;

        glDisable(GL_LIGHTING);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if( m_bOver || m_bStick)
		{
			CP_BKG_COLOR[3] = 0.85;
			CP_TXT_COLOR[3] = 1.00;
			CP_LIN_COLOR[3] = 0.85;

			CTRL_OUT_COLOR[3] = 0.85;
			CTRL_VAL_COLOR[3] = 0.85;
            CTRL_TXT_COLOR[3] = 1.00;
            CTRL_TXT_COLOR_DISABLED[3] = 1.00;

            bBrightChannelColors = true;
		}
		else
		{
			CP_BKG_COLOR[3] = 0.10;
			CP_TXT_COLOR[3] = 0.15;
			CP_LIN_COLOR[3] = 0.10;

			CTRL_OUT_COLOR[3] = 0.10;
			CTRL_VAL_COLOR[3] = 0.10;
            CTRL_TXT_COLOR[3] = 0.15;
            CTRL_TXT_COLOR_DISABLED[3] = 0.15;

            bBrightChannelColors = false;
		}

        // Draw ControlPanel Frame
		glLineWidth(2);
		glColor4dv(CP_LIN_COLOR);
		glBegin(GL_LINE_STRIP);
			glVertex2d(m_iLeft, m_iTop);
			glVertex2d(m_iRight, m_iTop);
			glVertex2d(m_iRight, m_iBottom);
			glVertex2d(m_iLeft, m_iBottom);
			glVertex2d(m_iLeft, m_iTop);
		glEnd();
		glLineWidth(1.0);
        

    	// Draw Control Panel Name
		glColor4dv(CP_TXT_COLOR);
		glPushMatrix();
		glRasterPos2i(m_iTitleLeft + CHK_BOX_SPACE+32, m_iTitleBottom+5);
		i=0;
		while( (i<strlen(m_sName)) && (m_sName[i] != '\0'))
		{
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, m_sName[i]);
			i++;
		}
		glPopMatrix();

		//Draw ControlPanel Title Bar

		glColor4dv(CP_LIN_COLOR);
		glBegin(GL_POLYGON);
			glVertex2d(m_iTitleLeft, m_iTitleTop);
			glVertex2d(m_iTitleRight, m_iTitleTop);
			glVertex2d(m_iTitleRight, m_iTitleBottom);
			glVertex2d(m_iTitleLeft, m_iTitleBottom);
		glEnd();
		

		//Draw Controls

        for(i=0; i<m_vGraphBoxes.size(); i++)
        {
            if(m_vGraphBoxes[i] != NULL)
                m_vGraphBoxes[i]->Display();
        }

		for(i=0; i<m_vCheckBoxes.size(); i++)
		{
			if(m_vCheckBoxes[i] != NULL)
				m_vCheckBoxes[i]->Display();
		}

		for(i=0; i<m_vSliders.size(); i++)
		{
			if(m_vSliders[i] != NULL)
				m_vSliders[i]->Display();
		}

		for(i=0; i<m_vOptionBoxes.size(); i++)
		{
			if(m_vOptionBoxes[i] != NULL)
				m_vOptionBoxes[i]->Display();
		}

		for(i=0; i<m_vInfoSliders.size(); i++)
		{
			if(m_vInfoSliders[i] != NULL)
				m_vInfoSliders[i]->Display();
		}
        //Draw ControlPanel Background
		
        glLineWidth(1.0);
		glColor4dv(CP_BKG_COLOR);
		glBegin(GL_POLYGON);
			glVertex2d(m_iLeft, m_iTop);
			glVertex2d(m_iRight, m_iTop);
			glVertex2d(m_iRight, m_iBottom);
			glVertex2d(m_iLeft, m_iBottom);
		glEnd();
        
        
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
        glEnable(GL_CULL_FACE);
		
	}

    
}


CCheckBox::CCheckBox(char *pName, int iPosH, int iPosV, bool bVal, bool bEnabled, void (*pCallBack)(bool bNewVal))
{
	//iPosH & iPosV Specify Upper Left Corner
	strcpy(m_sName, pName);
	m_bValue = bVal;

	m_iLeft = iPosH;
	m_iRight = m_iLeft + CHK_BOX_SIZE;
	m_iTop = iPosV;
	m_iBottom = m_iTop - CHK_BOX_SIZE;

	m_iCheckLeft = m_iLeft + CHK_CHECK_SPACE;
	m_iCheckRight = m_iRight - CHK_CHECK_SPACE;
	m_iCheckTop = m_iTop - CHK_CHECK_SPACE;
	m_iCheckBottom = m_iBottom + CHK_CHECK_SPACE;

	m_bEnabled = bEnabled;
	m_bOver = false;

	mpf_CallBack = pCallBack;


}

CCheckBox::~CCheckBox()
{
}

void CCheckBox::Repos(int iPosH, int iPosV)
{
	m_iLeft = iPosH;
	m_iRight = m_iLeft + CHK_BOX_SIZE;
	m_iTop = iPosV;
	m_iBottom = m_iTop - CHK_BOX_SIZE;

	m_iCheckLeft = m_iLeft + CHK_CHECK_SPACE;
	m_iCheckRight = m_iRight - CHK_CHECK_SPACE;
	m_iCheckTop = m_iTop - CHK_CHECK_SPACE;
	m_iCheckBottom = m_iBottom + CHK_CHECK_SPACE;
}

void CCheckBox::Enable(bool bEnabled)
{
	m_bEnabled = bEnabled;
}

bool CCheckBox::Mouse(int iButton, int iState, int iX, int iY)
{
	bool bHandled = false;

	if( (iX > m_iLeft) && (iX < m_iRight) && (iY < m_iTop) && (iY > m_iBottom) )
	{
		if(m_bEnabled)
		{
			switch(iButton)
			{
				case GLUT_LEFT_BUTTON:
					switch(iState)
					{
						case GLUT_DOWN:
							break;
						case GLUT_UP :
								m_bValue = !m_bValue;
								if(mpf_CallBack != NULL)
								{
									mpf_CallBack(m_bValue);
								}
								
								
								break;
						default : break;
					}
					break;
				case GLUT_RIGHT_BUTTON:
					switch(iState)
					{
						case GLUT_DOWN:
							break;
						case GLUT_UP :
							break;
						default : break;
					}
					break;
				case GLUT_MIDDLE_BUTTON:
					switch(iState)
					{
						case GLUT_DOWN:
							break;
						case GLUT_UP :
							break;
						default : break;
					}
					break;
				default: break;
			} // switch
		} // if enabled

		bHandled = true;

	}
	

	return bHandled;
}

bool CCheckBox::Motion(int iX, int iY)
{
	bool bHandled = false;

	if( (iX > m_iLeft) && (iX < m_iRight) && (iY < m_iTop) && (iY > m_iBottom) )
	{
		if(m_bEnabled)
		{
		}

		bHandled = true;

	}


	return bHandled;
}

bool CCheckBox::PassiveMotion(int iX, int iY)
{
	bool bHandled = true;

	if( (iX > m_iLeft) && (iX < m_iRight) && (iY < m_iTop) && (iY > m_iBottom) )
	{
		m_bOver = true;
	}
	else
	{
		m_bOver = false;
	}

	return bHandled;
}

void CCheckBox::Display(void)
{
	int i;

	if( (m_bOver) && (m_bEnabled) )
		glColor4dv(CTRL_SEL_COLOR);
	else
		glColor4dv(CTRL_OUT_COLOR);

	// Draw Control
	glBegin(GL_LINE_STRIP);
		glVertex2d(m_iLeft, m_iTop);
		glVertex2d(m_iRight, m_iTop);
		glVertex2d(m_iRight, m_iBottom);
		glVertex2d(m_iLeft, m_iBottom);
		glVertex2d(m_iLeft, m_iTop);
	glEnd();

	// Draw Check Mark
	glColor4dv(CTRL_VAL_COLOR);
	if(m_bValue == true)
	{
		glBegin(GL_POLYGON);
			glVertex2d(m_iCheckLeft+1, m_iCheckTop);
			glVertex2d(m_iCheckRight, m_iCheckTop);
			glVertex2d(m_iCheckRight, m_iCheckBottom+1);
			glVertex2d(m_iCheckLeft+1, m_iCheckBottom+1);
		glEnd();
	}
	

	// Draw Checkbox Name
    if(m_bEnabled)
	    glColor4dv(CTRL_TXT_COLOR);
    else
        glColor4dv(CTRL_TXT_COLOR_DISABLED);

	glPushMatrix();
	glRasterPos2i(m_iRight + CHK_BOX_SPACE, m_iBottom+3);
	i=0;
	while( (i<strlen(m_sName)) && (m_sName[i] != '\0'))
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m_sName[i]);
		i++;
	}
	glPopMatrix();

}


void CCheckBox::SetValue(bool bNewVal)
{
	if(m_bEnabled)
    {
        m_bValue = bNewVal;
	    if(mpf_CallBack != NULL)
	    {
		    mpf_CallBack(m_bValue);
	    }
    }
}


CSlider::CSlider(char *pName, int iPosH, int iPosV, int iWidth, double dVal, double dMinVal, double dMaxVal, bool bEnabled, void (*pCallBack)(double bNewVal))
{
	//iPosH & iPosV Specify Upper Left Corner
	strcpy(m_sName, pName);
	m_dMinVal = dMinVal;
	m_dMaxVal = dMaxVal;

	
	mpf_CallBack = pCallBack;

	m_iLeft = iPosH;
	m_iRight = m_iLeft + iWidth;
	m_iTop = iPosV;
	m_iBottom = m_iTop - SLD_HEIGHT;

	m_iValRightMax = m_iRight - SLD_SPACE;
	m_iValLeft = m_iLeft + SLD_SPACE;
	m_iValTop = m_iTop - SLD_SPACE;
	m_iValBottom = m_iBottom + SLD_SPACE;
	
//	  m_dPercPerPix = 1000.0 / (m_iValRightMax - m_iValLeft);
//	  m_dValPerPerc = (m_dMaxVal - m_dMinVal) / 1000.0;
   m_dValPerPix = (m_dMaxVal - m_dMinVal) / double((m_iValRightMax - m_iValLeft));

	m_dValue = dVal;
//	  m_iPerc = m_dValue / m_dValPerPerc;

	m_bEnabled = bEnabled;

}

CSlider::~CSlider()
{
}

void CSlider::Repos(int iPosH, int iPosV, int iWidth)
{
	m_iLeft = iPosH;
	m_iRight = m_iLeft + iWidth;
	m_iTop = iPosV;
	m_iBottom = m_iTop - SLD_HEIGHT;

	m_iValRightMax = m_iRight - SLD_SPACE;
	m_iValLeft = m_iLeft + SLD_SPACE;
	m_iValTop = m_iTop - SLD_SPACE;
	m_iValBottom = m_iBottom + SLD_SPACE;
	
   m_dValPerPix = (m_iValRightMax - m_iValLeft) / (m_dMaxVal - m_dMinVal);

//	m_dPercPerPix = 200.0 / (m_iValRightMax - m_iValLeft);
//	m_dValPerPerc = (m_dMaxVal - m_dMinVal) / 200.0;

//	m_iPerc = m_dValue / m_dValPerPerc;

}

void CSlider::Enable(bool bEnabled)
{
	m_bEnabled = bEnabled;
}


bool CSlider::Mouse(int iButton, int iState, int iX, int iY)
{
	bool bHandled = false;

	if( (iX > m_iLeft) && (iX < m_iRight) && (iY < m_iTop) && (iY > m_iBottom) )
	{
		if(m_bEnabled)
		{
			switch(iButton)
			{
				case GLUT_LEFT_BUTTON:
					switch(iState)
					{
						case GLUT_DOWN:
							m_bSliding = true;
							m_dValue = (iX - m_iLeft) / this->m_dValPerPix + this->m_dMinVal;
							if(mpf_CallBack != NULL)
							{
								mpf_CallBack(m_dValue);
							}
							break;
						case GLUT_UP :
							m_bSliding = false;
							break;
						default : break;
					}
					break;
				case GLUT_RIGHT_BUTTON:
					switch(iState)
					{
						case GLUT_DOWN:
							break;
						case GLUT_UP :
							break;
						default : break;
					}
					break;
				case GLUT_MIDDLE_BUTTON:
					switch(iState)
					{
						case GLUT_DOWN:
							break;
						case GLUT_UP :
							break;
						default : break;
					}
					break;
				default: break;
			} // switch
		} // if enabled
	}

	return bHandled;
}

bool CSlider::Motion(int iX, int iY)
{
	bool bHandled = false;

	if( (iX > m_iLeft) && (iX < m_iRight) && (iY < m_iTop) && (iY > m_iBottom) )
	{
		if(m_bEnabled)
		{
			if(m_bSliding)
			{
				m_dValue = (iX - m_iLeft) / this->m_dValPerPix + this->m_dMinVal;
//				m_iPerc = (iX - m_iLeft) * m_dPercPerPix;
//				m_dValue = m_iPerc * m_dValPerPerc;

				if (mpf_CallBack != NULL)
				{
					mpf_CallBack(m_dValue);
				}
			}

		} // if enabled

		bHandled = true;
	}
	

	return bHandled;
}


bool CSlider::PassiveMotion(int iX, int iY)
{
	bool bHandled = true;

	if( (iX > m_iLeft) && (iX < m_iRight) && (iY < m_iTop) && (iY > m_iBottom) )
	{
		m_bOver = true;
	}
	else
	{
		m_bOver = false;
	}


	return bHandled;
}



void CSlider::Display(void)
{
	int i;
	char sVal[20];


	// Draw Value Text
	if(m_bEnabled)
        glColor4dv(CTRL_TXT_COLOR);
    else
        glColor4dv(CTRL_TXT_COLOR_DISABLED);
	sprintf(sVal, "%4.2f\0", m_dValue);
	glPushMatrix();
	glRasterPos2i(m_iValLeft + SLD_SPACE, m_iValBottom+SLD_SPACE);
	i=0;
	while( (i<strlen(sVal)) && (sVal[i] != '\0'))
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, sVal[i]);
		i++;
	}
	glPopMatrix();


	if(m_bOver)
		glColor4dv(CTRL_SEL_COLOR);
	else
		glColor4dv(CTRL_OUT_COLOR);

	// Draw Slider Control
	glBegin(GL_LINE_STRIP);
		glVertex2d(m_iLeft, m_iTop);
		glVertex2d(m_iRight, m_iTop);
		glVertex2d(m_iRight, m_iBottom);
		glVertex2d(m_iLeft, m_iBottom);
		glVertex2d(m_iLeft, m_iTop);
	glEnd();

	// Draw Slider Value Bar
	glColor4dv(CTRL_VAL_COLOR);
   m_iValRight = m_iValLeft + int( this->m_dValPerPix * (this->m_dValue - this->m_dMinVal) );
	glBegin(GL_POLYGON);
		glVertex2d(m_iValLeft+1, m_iValTop);
		glVertex2d(m_iValRight, m_iValTop);
		glVertex2d(m_iValRight, m_iValBottom+1);
		glVertex2d(m_iValLeft+1, m_iValBottom+1);
	glEnd();

	// Draw Slider Name & Values
	if(m_bEnabled)
        glColor4dv(CTRL_TXT_COLOR);
    else
        glColor4dv(CTRL_TXT_COLOR_DISABLED);
	glPushMatrix();
	glRasterPos2i(m_iLeft + SLD_SPACE, m_iTop+3);
	i=0;
	while( (i<strlen(m_sName)) && (m_sName[i] != '\0'))
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m_sName[i]);
		i++;
	}
	glPopMatrix();

}

void CSlider::SetValue(double dNewVal)
{
	if(m_bEnabled)
    {
        if( (dNewVal >= m_dMinVal) && (dNewVal <= m_dMaxVal) )
	    {
		    m_dValue =  dNewVal;
//		    m_iPerc = m_dValue /  m_dValPerPerc;
	    
		    if(mpf_CallBack != NULL)
		    {
			    mpf_CallBack(m_dValue);
		    }
	    }
    }
}

COptionBox::COptionBox(char *pName, int iPosH, int iPosV, int iWidth, int iVal, bool bEnabled, void (*pCallBack)(int bNewVal))
{
	strcpy(m_sName, pName);
	m_iValue = iVal;
	m_bOver = false;
	m_bEnabled = bEnabled;

	mpf_CallBack = pCallBack;

	m_iLeft = iPosH;
	m_iRight = m_iLeft + iWidth;
	m_iTop = iPosV;
	m_iBottom = m_iTop - OPT_TITLE_HEIGHT - 2*CONTROL_SPACE - m_vOptions.size()*(CHK_BOX_SIZE+CONTROL_SPACE);

	m_iTitleLeft = m_iLeft + CONTROL_SPACE;
	m_iTitleRight = m_iRight - CONTROL_SPACE;
	m_iTitleTop = m_iTop - CONTROL_SPACE;
	m_iTitleBottom = m_iTitleTop - OPT_TITLE_HEIGHT;
}

COptionBox::~COptionBox()
{
	int i;

	for(i=0; i<m_vOptions.size(); i++)
	{
		if(m_vOptions[i] != NULL)
			delete m_vOptions[i];
	}
}

bool COptionBox::Mouse(int iButton, int iState, int iX, int iY)
{
	bool bHandled = false;
	bool bHit = false;
	int i, j;
	int iPrevVal;

	if( (iX > m_iLeft) && (iX < m_iRight) && (iY < m_iTop) && (iY > m_iBottom) )
	{
		if(m_bEnabled)
		{
			switch(iButton)
			{
				case GLUT_LEFT_BUTTON:
					switch(iState)
					{
						case GLUT_DOWN:
							break;
						case GLUT_UP :
							iPrevVal = m_iValue;
							for(i=0; i<m_vOptions.size(); i++)
							{
								if((m_vOptions[i] != NULL) && (m_vOptions[i]->m_bEnabled) )
								{
									bHit = m_vOptions[i]->Mouse(iButton, iState, iX, iY);
									if(bHit)
									{
										m_iValue = i;
										if(m_vOptions[i]->m_bValue == false)
											m_vOptions[i]->m_bValue = true;

										for(j=0; j<m_vOptions.size(); j++)
										{
											if(j != i)
												m_vOptions[j]->m_bValue = false;
										}

										break;
									}
								}
							}

							if((mpf_CallBack != NULL) && (iPrevVal != m_iValue) )
							{
								mpf_CallBack(m_iValue);
							}
							break;
						default : break;
					}
					break;
				case GLUT_RIGHT_BUTTON:
					switch(iState)
					{
						case GLUT_DOWN:
							break;
						case GLUT_UP :
							break;
						default : break;
					}
					break;
				case GLUT_MIDDLE_BUTTON:
					switch(iState)
					{
						case GLUT_DOWN:
							break;
						case GLUT_UP :
							break;
						default : break;
					}
					break;
				default: break;
			} // switch
		} // if enabled

		bHandled = true;
	}
	
	return bHandled;
}

bool COptionBox::Motion(int iX, int iY)
{
	bool bHandled = false;
	
	if( (iX > m_iLeft) && (iX < m_iRight) && (iY < m_iTop) && (iY > m_iBottom) )
	{
		if(m_bEnabled)
		{
		}

		bHandled = true;

	}

	return bHandled;
}

bool COptionBox::PassiveMotion(int iX, int iY)
{
	bool bHandled = false;
	int i;

	if( (iX > m_iLeft) && (iX < m_iRight) && (iY < m_iTop) && (iY > m_iBottom) )
	{
		m_bOver = true;
	}
	else
	{
		m_bOver = false;
	}

	for(i=0; i<m_vOptions.size(); i++)
	{
		if(m_vOptions[i] != NULL)
			m_vOptions[i]->PassiveMotion(iX, iY);
	}


	return bHandled;
}

void COptionBox::Repos(int iPosH, int iPosV, int iWidth)
{
	int i, iH, iV;

	m_iLeft = iPosH;
	m_iRight = m_iLeft + iWidth;
	m_iTop = iPosV;
	m_iBottom = m_iTop - OPT_TITLE_HEIGHT - 2*CONTROL_SPACE - m_vOptions.size()*(CHK_BOX_SIZE+CONTROL_SPACE) - CONTROL_SPACE;

	m_iTitleLeft = m_iLeft + CONTROL_SPACE;
	m_iTitleRight = m_iRight - CONTROL_SPACE;
	m_iTitleTop = m_iTop - CONTROL_SPACE;
	m_iTitleBottom = m_iTitleTop - OPT_TITLE_HEIGHT;
	

	for(i=0; i<m_vOptions.size(); i++)
	{
		iH = m_iLeft+CONTROL_SPACE;
		iV = m_iTitleBottom - CONTROL_SPACE - i*(CHK_BOX_SIZE+CONTROL_SPACE);

		if(m_vOptions[i] != NULL)
			m_vOptions[i]->Repos(iH, iV);
	}
}

void COptionBox::Enable(bool bEnable)
{
    int i;

    m_bEnabled = bEnable;
    
    for(i=0; i<m_vOptions.size(); i++)
    {
        if(m_vOptions[i] != NULL)
			m_vOptions[i]->Enable(m_bEnabled);
    }
}

CCheckBox* COptionBox::AddOption(char *pName)
{
	int iPosH, iPosV;
	CCheckBox *pC = NULL; 
		
	iPosH = m_iLeft+CONTROL_SPACE;
	iPosV = m_iTitleBottom - CONTROL_SPACE - m_vOptions.size()*(CHK_BOX_SIZE+CONTROL_SPACE);
		
	pC = new CCheckBox(pName, iPosH, iPosV, false, m_bEnabled, NULL);

	if(pC != NULL)
		m_vOptions.push_back(pC);

    return pC;
}

void COptionBox::Display(void)
{
	int i;

	if(m_bOver)
		glColor4dv(CTRL_SEL_COLOR);
	else
		glColor4dv(CTRL_OUT_COLOR);

	// Draw Option Control
	glBegin(GL_LINE_STRIP);
		glVertex2d(m_iLeft, m_iTop);
		glVertex2d(m_iRight, m_iTop);
		glVertex2d(m_iRight, m_iBottom);
		glVertex2d(m_iLeft, m_iBottom);
		glVertex2d(m_iLeft, m_iTop);
	glEnd();
	

	// Draw Option Box Name
	if(m_bEnabled)
        glColor4dv(CTRL_TXT_COLOR);
    else
        glColor4dv(CTRL_TXT_COLOR_DISABLED);
	glPushMatrix();
	glRasterPos2i(m_iTitleLeft + CONTROL_SPACE, m_iTitleBottom+3);
	i=0;
	while( (i<strlen(m_sName)) && (m_sName[i] != '\0'))
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m_sName[i]);
		i++;
	}
	glPopMatrix();


	//Draw Option Box Title Bar
	glColor4dv(CTRL_OUT_COLOR);
	glBegin(GL_POLYGON);
		glVertex2d(m_iTitleLeft, m_iTitleTop);
		glVertex2d(m_iTitleRight, m_iTitleTop);
		glVertex2d(m_iTitleRight, m_iTitleBottom);
		glVertex2d(m_iTitleLeft, m_iTitleBottom);
	glEnd();



	for(i=0; i<m_vOptions.size(); i++)
	{
		if(m_vOptions[i] != NULL)
			m_vOptions[i]->Display();
	}
}

void COptionBox::SetValue(int iNewVal)
{
    if(m_bEnabled)
    {
	    if( (iNewVal >= 0) && (iNewVal < m_vOptions.size()) )
	    {
		    int i;
		    m_iValue = iNewVal;
		    
		    for(i=0; i<m_vOptions.size(); i++)
		    {
			    if(m_vOptions[i] != NULL)
			    {
				    if(i == m_iValue)
					    m_vOptions[i]->SetValue(true);
				    else
					    m_vOptions[i]->SetValue(false);
			    }
		    }

		    if(mpf_CallBack != NULL)
		    {
			    mpf_CallBack(m_iValue);
		    }
	    }
    }
}

void COptionBox::SetValue(int iNewVal, bool bCallBack)
{
    if(m_bEnabled)
    {
	    if( (iNewVal >= 0) && (iNewVal < m_vOptions.size()) )
	    {
		    int i;
		    m_iValue = iNewVal;
		    
		    for(i=0; i<m_vOptions.size(); i++)
		    {
			    if(m_vOptions[i] != NULL)
			    {
				    if(i == m_iValue)
					    m_vOptions[i]->SetValue(true);
				    else
					    m_vOptions[i]->SetValue(false);
			    }
		    }

		    if( (bCallBack == true) && (mpf_CallBack != NULL))
		    {
			    mpf_CallBack(m_iValue);
		    }
	    }
    }
}


CGraphChannel::CGraphChannel(char* pName)
{
    unsigned int i, iNameLen;
    
    m_iFillNr = 0;

    iNameLen = strlen(pName);

    if(iNameLen > MAX_NAME_LENGHT)
        iNameLen = 128;
    
    strncpy(m_sName, pName, iNameLen);
    m_sName[ iNameLen] = '\0';

    for(i=0; i<MAX_SAMPLES; i++)
        m_uiSamples[i] = 0;

    m_fScale = 0.0;
}


CGraphChannel::~CGraphChannel()
{
}


void CGraphChannel::AddSample(float fNewSample)
{
    
    // Scale the incoming samples here and store them
    // (instead of scaling the samples each time we draw the graph)
    m_uiSamples[m_iFillNr] = (unsigned int)(m_fScale*fNewSample);
    
    m_iFillNr++;
    if(m_iFillNr >= MAX_SAMPLES)
        m_iFillNr = 0;
}


void CGraphChannel::SetScale(float fScale)
{
    m_fScale = fScale;
}

CGraphBox::CGraphBox(char *pName, int iPosH, int iPosV, int iWidth, bool bEnabled, void (*pCallBack)(int bNewVal))
{
    strcpy(m_sName, pName);
	m_bOver = false;
	m_bEnabled = bEnabled;

	mpf_CallBack = pCallBack;

	m_iLeft = iPosH;
	m_iRight = m_iLeft + iWidth;
	m_iTop = iPosV;
	m_iBottom = m_iTop - GRAPH_TITLE_HEIGHT - 2*CONTROL_SPACE - GRAPH_BOX_SIZE;

	m_iTitleLeft = m_iLeft + CONTROL_SPACE;
	m_iTitleRight = m_iRight - CONTROL_SPACE;
	m_iTitleTop = m_iTop - CONTROL_SPACE;
	m_iTitleBottom = m_iTitleTop - OPT_TITLE_HEIGHT;

    m_iGraphLeft = m_iTitleLeft;
    m_iGraphRight = m_iTitleRight;
    m_iGraphTop = m_iTitleBottom - CONTROL_SPACE;
    m_iGraphBottom = m_iBottom + CONTROL_SPACE;

    m_iGraphWidth = m_iGraphRight - m_iGraphLeft;
    m_iGraphHeight = m_iGraphTop - m_iGraphBottom;

    m_iGraphCenterV= m_iGraphTop - m_iGraphHeight/2.0f;
    m_iGraphCenterH = m_iGraphLeft + m_iGraphWidth/2.0f;

    m_fGraphStepH = (float)m_iGraphWidth / (float)MAX_SAMPLES;

}

CGraphBox::~CGraphBox()
{
    unsigned int i;

    for(i=0; i<m_vChannels.size(); i++)
        if(m_vChannels[i] != NULL)
            delete m_vChannels[i];
}

bool CGraphBox::Mouse(int iButton, int iState, int iX, int iY)
{
    return false;
}

bool CGraphBox::Motion(int iX, int iY)
{
    return false;
}

bool CGraphBox::PassiveMotion(int iX, int iY)
{
    return false;
}

void CGraphBox::Repos(int iPosH, int iPosV, int iWidth)
{

    m_iLeft = iPosH;
	m_iRight = m_iLeft + iWidth;
	m_iTop = iPosV;
	m_iBottom = m_iTop - GRAPH_TITLE_HEIGHT - 2*CONTROL_SPACE - GRAPH_BOX_SIZE;

	m_iTitleLeft = m_iLeft + CONTROL_SPACE;
	m_iTitleRight = m_iRight - CONTROL_SPACE;
	m_iTitleTop = m_iTop - CONTROL_SPACE;
	m_iTitleBottom = m_iTitleTop - OPT_TITLE_HEIGHT;

    m_iGraphLeft = m_iTitleLeft;
    m_iGraphRight = m_iTitleRight;
    m_iGraphTop = m_iTitleBottom - CONTROL_SPACE;
    m_iGraphBottom = m_iBottom + CONTROL_SPACE;

    m_iGraphWidth = m_iGraphRight - m_iGraphLeft;
    m_iGraphHeight = m_iGraphTop - m_iGraphBottom;

    m_iGraphCenterV= m_iGraphTop - m_iGraphHeight/2.0f;
    m_iGraphCenterH = m_iGraphLeft + m_iGraphWidth/2.0f;

    m_fGraphStepH = (float)m_iGraphWidth / (float)MAX_SAMPLES;
    
}

void CGraphBox::Enable(bool bEnable)
{
    
}

void CGraphBox::Display(void)
{
    unsigned int i, j, s, x, y;

	if(m_bOver)
		glColor4dv(CTRL_SEL_COLOR);
	else
		glColor4dv(CTRL_OUT_COLOR);

	// Draw GraphBox Control
	glBegin(GL_LINE_STRIP);
		glVertex2i(m_iLeft, m_iTop);
		glVertex2i(m_iRight, m_iTop);
		glVertex2i(m_iRight, m_iBottom);
		glVertex2i(m_iLeft, m_iBottom);
		glVertex2i(m_iLeft, m_iTop);
	glEnd();
	

	// Draw GraphBox Name
	if(m_bEnabled)
        glColor4dv(CTRL_TXT_COLOR);
    else
        glColor4dv(CTRL_TXT_COLOR_DISABLED);
	glPushMatrix();
	glRasterPos2i(m_iTitleLeft + CONTROL_SPACE, m_iTitleBottom+3);
	i=0;
	while( (i<strlen(m_sName)) && (m_sName[i] != '\0'))
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, m_sName[i]);
		i++;
	}
	glPopMatrix();


	//Draw GraphBox Title Bar
	glColor4dv(CTRL_OUT_COLOR);
	glBegin(GL_POLYGON);
		glVertex2i(m_iTitleLeft, m_iTitleTop);
		glVertex2i(m_iTitleRight, m_iTitleTop);
		glVertex2i(m_iTitleRight, m_iTitleBottom);
		glVertex2i(m_iTitleLeft, m_iTitleBottom);
	glEnd();

    //Draw GraphBox Grid
    glColor4dv(CTRL_TXT_COLOR_DISABLED);
    glBegin(GL_LINE_STRIP);
        glVertex2i(m_iGraphLeft, m_iGraphTop);
        glVertex2i(m_iGraphRight, m_iGraphTop);
        glVertex2i(m_iGraphRight, m_iGraphBottom);
        glVertex2i(m_iGraphLeft, m_iGraphBottom);
        glVertex2i(m_iGraphLeft, m_iGraphTop);
    glEnd();

    glBegin(GL_LINES);
    for(i=1; i<GRAPH_GRID_X; i++)
    {
        glVertex2i(m_iGraphLeft+(i*m_iGraphWidth/GRAPH_GRID_X), m_iGraphTop);
        glVertex2i(m_iGraphLeft+(i*m_iGraphWidth/GRAPH_GRID_X), m_iGraphBottom);
    }
    glEnd();

    glBegin(GL_LINES);
    for(i=1; i<GRAPH_GRID_Y; i++)
    {
        glVertex2i(m_iGraphLeft, m_iGraphTop-(i*m_iGraphHeight/GRAPH_GRID_Y));
        glVertex2i(m_iGraphRight, m_iGraphTop-(i*m_iGraphHeight/GRAPH_GRID_Y));
    }
    glEnd();

    // Display channel samples
    for(i=0; i<m_vChannels.size(); i++)
    {
        if(bBrightChannelColors)
            glColor4fv(CHANNEL_COLORS_BRIGHT[i]);
        else
            glColor4fv(CHANNEL_COLORS_DIM[i]);

        if(m_vChannels[i] != NULL)
        {
            s = m_vChannels[i]->m_iFillNr;
            
            glBegin(GL_LINE_STRIP);
            for(j=0; j<=s; j++)
            {
                x = m_iGraphLeft + ((j+MAX_SAMPLES-s)*m_fGraphStepH);
                y = m_iGraphCenterV + m_vChannels[i]->m_uiSamples[j];
                glVertex2i(x, y);
            }
            glEnd();
            
            glBegin(GL_LINE_STRIP);
            for(j=s+1; j<MAX_SAMPLES; j++)
            {
                x = m_iGraphLeft + ((j-s+1)*m_fGraphStepH);
                y = m_iGraphCenterV + m_vChannels[i]->m_uiSamples[j];
                glVertex2i(x, y);
            }
            glEnd();
        }
    }
}

void CGraphBox::AddChannel(char* pChannelName)
{
    CGraphChannel* pC = NULL;
    CCheckBox* pCb = NULL;

    pC = new CGraphChannel(pChannelName);
    if((pC != NULL) && (m_vChannels.size() < GRAPH_MAX_CHANNELS))
    {
        m_vChannels.push_back(pC);
    }
}

void CGraphBox::AddSample(char* pChannelName, float fNewSample)
{
    unsigned int i;

    for(i=0; i<m_vChannels.size(); i++)
    {
        if(strcmp(m_vChannels[i]->m_sName, pChannelName) == 0)
            break;
    }

    if( (i >= 0) && (i<m_vChannels.size()) )
    {
        if(m_vChannels[i] != NULL)
        {
            m_vChannels[i]->AddSample(fNewSample);
        }
    }
}

void CGraphBox::SetScale(char* pChannelName, float fChannelMin, float fChannelMax)
{
    unsigned int i;
    float fScale;

    for(i=0; i<m_vChannels.size(); i++)
    {
        if(strcmp(m_vChannels[i]->m_sName, pChannelName) == 0)
            break;
    }

    if(fChannelMin < fChannelMax)
        fScale = (m_iGraphTop - m_iGraphBottom) / (fChannelMax - fChannelMin);
    else
        fScale = (m_iGraphTop - m_iGraphBottom) / 20.0;

    m_vChannels[i]->SetScale(fScale);

}