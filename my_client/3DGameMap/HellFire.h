
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef HELLFIRE_HEADER_KATTY
#define HELLFIRE_HEADER_KATTY
#include <windows.h>
#include <windows.h>
#include <Windef.h>
#include "Ani.h"
#include "MapObj.h"
#include <deque>
using namespace std;

enum {HELLFIRE_FREE = 10, HELLFIRE_FOLLOW, HELLFIRE_CORRECTE, HELLFIRE_CIRCLE, HELLFIRE_TURNTOORIG};
enum {HELLFIRE_NONE, HELLFIRE_CREATE, HELLFIRE_STOP};
const  int HELLFIRE_FRAME			= 9;
const  int HELLFIRE_CIRCLE_RADIUS	= 30;
typedef deque<int> deque_int;


class CHellFire : public CMapObj
{
public:
    CHellFire() {}
    ~CHellFire() {Destroy();}
    BOOL Create( int iWorldX, int iWorldY);
    void Destroy();
    //	void Process(int nWorldX,int nWorldY );
    void Show(void* pInfo);
    void SetPicture(char* szTitle); //{m_pPicture = pAni;}
    void Initial();
    int	 GetObjType   () {return MAP_COVER;}
    void GetPos		( CMyPos& posCell);
    void GetWorldPos(int& iWorldX, int& iWorldY) {iWorldX = m_nShowWorldPoint.x; iWorldY = m_nShowWorldPoint.y;}
    void GetBase(CMySize& infoSize)   {infoSize.iWidth = 1; infoSize.iHeight = 1;}
    void Process	(void* pInfo);
    static void AddHellFire(int nDensity);


public:
    int		m_iAttachMan;
private:

    int		GetY(int nx);
    int		m_iYMoved;
    int		m_iYMoveDir;
    int		m_iRadius;
    int		m_iCircleDir;
    int		m_iStatus;
    int		m_iAttackRange;
    int		m_iWorldX;
    int		m_iWorldY;
    DWORD	m_dwTimeCount;
    POINT	m_nCurWorldPoint;
    POINT	m_nOrigWorldPoint;
    POINT	m_nShowWorldPoint;
    POINT	m_nBoundTop;
    POINT	m_nBoundBottom;
    int		m_nLayer;
    int		m_nCurState;
    int		m_nCurFrame;
    int		m_nCurX;
    int		m_nCurY;
    int		m_nSpeed;
    int		m_nLoopCount;
    int		m_iBmpWidth;
    int		m_iBmpHeight;
    char	m_szAniTitle[100];

};




#endif
