
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef AIRBUBBLE_HEADER_KATTY
#define AIRBUBBLE_HEADER_KATTY


#include <windows.h>
#include <Windef.h>
#include "Ani.h"
#include "MapObj.h"
#include <deque>
#include "BaseFunc.h"
using namespace std;



enum {AIRBUBBLE_NONE, AIRBUBBLE_CREATE, AIRBUBBLE_SHOW, AIRBUBBLE_DESTROY};
const int BUBBLE_TYPE		= 7;
const int BUBBLE_FRAME	= 1;

class CAirBubble : public CMapObj
{
public:
    CAirBubble() {AIRBUBBLE_NONE;}
    ~CAirBubble() {Destroy();}
    BOOL Create(DWORD dwDelayTime, int iWorldX, int iWorldY);
    void Destroy();
    void Process();
    void Show(void* pInfo);
    void SetPicture(char* pszFilename) { strcpy(m_szFileName, pszFilename); }
    void GetPos		(int& iPosX, int& iPosY);
    void GetWorldPos(int& iWorldX, int& iWorldY) {iWorldX = m_nOrigWorldPoint.x; iWorldY = m_nOrigWorldPoint.y;}
    //	void GetSize	(CSize& sizeObj){sizeObj.iWidth =1;sizeObj.iHeight =1;sizeObj.iSize = 1;}
    void Process	(void* pInfo) {}
    int		m_iStatus;			//ÐÐÎª×´Ì¬

private:

    int		m_iWorldX;
    int		m_iWorldY;
    char 	m_szFileName[100];
    POINT	m_nShowWorldPoint;
    POINT	m_nOrigWorldPoint;
    int		m_nSpeed;
    int		m_nLoopCount;
    DWORD	m_dwCreateTime;
    DWORD	m_dwDelayTime;


};

typedef deque<CAirBubble*> DequeBubble;
class CAirBubbleSys: public CMapObj
{
public:
    CAirBubbleSys() {m_nStatus = AIRBUBBLE_NONE;}
    ~CAirBubbleSys() {Destroy();}
    void Destroy();
    void* pInfo() {}
    void Show();
    BOOL Create(int nNum, int nType);
    DequeBubble		m_dequeBubble;
    void AddBubble(int iWorldX, int iWorldY);

    void GetWorldPos(CMyPos& posWorld) {posWorld.x = 1000; posWorld.y = 1000;}
    void GetPos(CMyPos& posCell)      {posCell.x = 1000; posCell.y = 1000;}
    void GetBase(CMySize& infoSize)   {infoSize.iWidth = 1; infoSize.iHeight = 1;}

private:
    char		m_szFileName[BUBBLE_TYPE][100];
    int			m_nStatus;
    int			m_nNum;
    DWORD		m_dwCreateTime;
};


#endif
