#ifndef __HmWorkspace_h_
#define __HmWorkspace_h_


#define WS_BOTTOM 1
#define WS_TOP 2
#define WS_LEFT 4
#define WS_RIGHT 8
#define WS_BACK 16
#define WS_AXES 32
#define WS_GRID 64

class CHmWorkspace
{
public:
    CHmWorkspace(   float fRadiusMin=0.28, 
                    float fRadiusMax=0.64, 
                    float fCenterOffset=-0.45, 
                    float fPhiMin=-0.5,
                    float fPhiMax=0.5,
                    float fZMin=-0.2,
                    float fZMax=0.2,
                    unsigned int uiNrSegments=16,
                    unsigned int uiNrSubdivs=16,
                    bool bUseTriangles=false);
    ~CHmWorkspace();

    void Display(unsigned char ucMode);

private:

    void CreateVertices(void);
    
    void Display2(bool bGrid);
    void DisplayBottom(bool bGrid);
    void DisplayTop(bool bGrid);
    void DisplayLeft(bool bGrid);
    void DisplayRight(bool bGrid);
    void DisplayBack(bool bGrid);
    void DisplayAxes(void);

private:

    float m_fRadiusMin; 
    float m_fRadiusMax; 
    float m_fCenterOffset;
    float m_fZMin;
    float m_fZMax;
    float m_fPhiMin;
    float m_fPhiMax;
    unsigned int m_uiNrSegments;
    unsigned int m_uiNrSubdivs;
    
    float* m_pPlaneVertices;
    float* m_pSideVertices;
    float* m_pBackVertices;

    float* m_pPlaneNormals;
    float* m_pSideNormals;
    float* m_pBackNormals;

    unsigned int* m_pIndices;
    unsigned int m_uiNrPolygons;
    
    // indicates whether the workspace is created and drawn 
    // using triangles or quads, default is triangles
    bool m_bUseTriangles; 

};


#endif // __HmWorkspace_h_