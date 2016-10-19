
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef HEADER_SKYROSE_KATTY
#define HEADER_SKYROSE_KATTY

#include <windows.h>
#include "mapobj.h"


class CSkyRose : public CMapObj
{
public:
    CSkyRose() {}
    ~CSkyRose() {Destroy();}
    void Create(char* pszAniTitle, int iWorldX, int iWorldY, int iCircleDir, int nRiseStep ) ;
    void Process(void* pInfo);
    void Show(void* pInfo);
    void Destroy();
    int  GetObjType() {return MAP_SCENE;}
    int GetStatus() {return m_iStatus;}
private:
    POINT	m_nShowWorldPoint;
    POINT	m_nOrigWorldPoint;
    int		m_iCircleDir;
    int		m_nCurY;
    int		m_nLastFrame;
    int		m_nLoopFrame;
    int		m_nRiseCount;
    int		m_nCurX;
    int		m_iYMoved;
    int		m_nRiseStep;
    BYTE	m_bAniIndex;
    int		m_iAlpha;
    DWORD	m_dwFrameTime;
    char	m_szAniTitle[100];
    int		m_iStatus;
};





#endif
