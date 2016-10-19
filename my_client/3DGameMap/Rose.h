
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef ROSE_HEADER_KATTY
#define ROSE_HEADER_KATTY

#include <windows.h>
#include <Windef.h>
#include "Ani.h"
#include "MapObj.h"
#include <deque>
using namespace std;

const int MAX_ROSEANI = 4;
enum { ROSE_NONE, ROSE_CREATE, ROSE_DISPERSE, ROSE_RISE, ROSE_FALL, ROSE_DESTROY};

class CRose
{
public:
    CRose() { m_iStatus = ROSE_NONE;}
    ~CRose() {Destroy();}
    BOOL Create(int iWorldX, int iWorldY, int iTargetX, int iTargetY, int iStep);
    void Destroy();
    void Process();
    void SetPicture(char  pszChar[8][100], int rose, int rise, int grass, int paint)
    {
        strcpy(m_szFileName[0] , pszChar[rose]);
        strcpy(m_szFileName[1] , pszChar[rise]);
        strcpy(m_szFileName[2] , pszChar[grass]);
        strcpy(m_szFileName[3] , pszChar[paint]);
        sprintf(m_szAniTitle, "rose%d_ani%d", rose + 1, 1);
    }

    int		m_iStatus;			//ÐÐÎª×´Ì¬
private:
    void Show();
    POINT	m_nShowWorldPoint;
    POINT	m_nOrigWorldPoint;
    POINT	m_nTargetPoint;
    int		m_nLastFrame;
    int		m_nStayCount;
    int		m_nLoopFrame;
    int		m_nRiseCount;
    int		m_iCircleDir;
    int		m_nCurX;
    int		m_nCurY;
    int		m_iYMoved;
    int		m_nRiseStep;
    int		m_nPathLength;
    int		m_nPathCount;
    BYTE	m_bAniIndex;
    int		m_iAlpha;
    DWORD	m_dwFrameTime;
    char	m_szFileName[4][100];
    char	m_szAniTitle[100];

};

typedef deque<CRose*> DequeRose;
class Rose
{
public:
    Rose() {m_nStatus = ROSE_NONE;}
    ~Rose() {Destroy();}
    void Destroy();
    void Process();
    BOOL Create(int iWorldX, int iWorldY, int nNum, int nType);
    DequeRose		m_dequeRose;
    int				m_nStatus;
private:
    char		m_szFileName[MAX_ROSEANI+4][100];
    int			m_nNum;
    int			m_iWorldX;
    int			m_iWorldY;

};


typedef deque<Rose*> DequeRoseSys;

class CRoseSys: public CMapObj
{
public:
    CRoseSys() {}
    ~CRoseSys() {Destroy();}
    BOOL  Create(int iWorldX, int iWorldY, int nType);
    int   GetRoseNum() { return m_DequeRose.size();}
    void  Destroy();
    void Process() {}
    void Show();

    void GetWorldPos(CMyPos& posWorld) {posWorld.x = 0; posWorld.y = 0;}
    void GetPos(CMyPos& posCell)      {posCell.x = 0; posCell.y = 0;}
    void GetBase(CMySize& infoSize)   {infoSize.iWidth = 1; infoSize.iHeight = 1;}
    DequeRoseSys  m_DequeRose;
};


#endif




