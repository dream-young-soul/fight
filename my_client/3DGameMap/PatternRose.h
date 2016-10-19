
//**********************************************************
// 代码编辑器
//**********************************************************

#ifndef PATTERNROSE_HEADER_KATTY
#define PATTERNROSE_HEADER_KATTY

#include <windows.h>
#include <Windef.h>
#include "Ani.h"
#include "MapObj.h"
#include <deque>
#include "rose.h"
using namespace std;


const int MAX_PatternRoseANI = 4;
enum { PATTERNROSE_NONE, PATTERNROSE_CREATE, PATTERNROSE_RISE, PATTERNROSE_FALL, PATTERNROSE_DESTROY};

class CPatternRose
{
public:
    CPatternRose() { m_iStatus = ROSE_NONE;}
    ~CPatternRose() {Destroy();}
    BOOL Create(int iWorldX, int iWorldY);
    void Destroy();
    void Process();
    void SetPicture(char szFileName[8][100], int rose, int rise, int grass, int paint, int nType)
    {
        strcpy(m_szFileName[0] , szFileName[rose]);
        strcpy(m_szFileName[1] , szFileName[rise]);
        strcpy(m_szFileName[2] , szFileName[grass]);
        strcpy(m_szFileName[3] , szFileName[paint]);
        sprintf(m_szAniTitle, "rose%d_ani%d", nType, 1);
    }
    int		m_iStatus;			//行为状态
private:
    void Show();
    char 	m_szFileName[4][100];
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
};

typedef deque<CPatternRose*> DequePatternRose;
class PatternRose
{
public:
    PatternRose() {m_nStatus = ROSE_NONE;}
    ~PatternRose() {Destroy();}
    void Destroy();
    void Process();
    BOOL Create(int iWorldX, int iWorldY, int nType, int nColor);
    BOOL Create(int iWorldX, int iWorldY, int nColor, char* szChar);
    int  test(DWORD a, int k) {	a <<= (k - 1);	if (a & 0x80)	{ return (1); }	else	{ return (0); }}
    BOOL  GetPoint(int* nArray, char* zw, int X, int Y);
    int* GetBmpPoint(int index, char* szpFileName);
    DequePatternRose		m_dequePatternRose;
    int				m_nStatus;

private:
    char		m_szFileName[MAX_ROSEANI+4][100];
    int			m_nPoint[35*20*2];
    int			m_nNum;
    int			m_iWorldX;
    int			m_iWorldY;

};


typedef deque<PatternRose*> DequePatternRoseSys;

class CPatternRoseSys : public CMapObj
{
public:
    CPatternRoseSys() {}
    ~CPatternRoseSys() {Destroy();}
    BOOL  Create(int iWorldX, int iWorldY, int nType, int nColor); //nType图案 1 --- 5+ 外部文件图案数   //ncolor 1:红色,2:黄色,3:玫瑰色 ,4:三色花 ,5三色花2
    BOOL Create(int iWorldX, int iWorldY, int nColor, char* szChar);
    int   GetRoseNum() { return m_DequePatternRose.size();}
    void  Destroy();
    void Process() {}
    void Show();
    void GetWorldPos(CMyPos& posWorld) {posWorld.x = 0; posWorld.y = 0;}
    void GetPos(CMyPos& posCell)      {posCell.x = 0; posCell.y = 0;}
    void GetBase(CMySize& infoSize)   {infoSize.iWidth = 1; infoSize.iHeight = 1;}
    DequePatternRoseSys  m_DequePatternRose;
};


#endif




