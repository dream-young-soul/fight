
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ZoomNum.h: interface for the CZoomNum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZOOMNUM_H__43C6571A_40D0_4AC0_8DC8_378B7B5672C5__INCLUDED_)
#define AFX_ZOOMNUM_H__43C6571A_40D0_4AC0_8DC8_378B7B5672C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"
typedef struct
{
    int nNum;
    DWORD dwTimeBegin;
    DWORD dwColor;
    DWORD dwDelay;
    int	  nOffsetX;
} ZoomNumInfo;

#include <deque>
using namespace std;
typedef deque<ZoomNumInfo*>DEQUE_ZOOMNUMINFO;

class CZoomNum
{
public:
    CZoomNum();
    virtual ~CZoomNum();

private:
    DEQUE_ZOOMNUMINFO m_setInfo;

private:
    void Destroy();

public:
    void Add(int nNum, DWORD dwColor, DWORD dwDelay, int nRandOffset = 0);
    void Show(CMyPos posShow);
};

#endif // !defined(AFX_ZOOMNUM_H__43C6571A_40D0_4AC0_8DC8_378B7B5672C5__INCLUDED_)
