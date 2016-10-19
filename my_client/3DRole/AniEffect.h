
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// AniEffect.h: interface for the CAniEffect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIEFFECT_H__5A33BDF3_F599_4C93_B7BF_2BDD147EF35F__INCLUDED_)
#define AFX_ANIEFFECT_H__5A33BDF3_F599_4C93_B7BF_2BDD147EF35F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "basefunc.h"

const int _LOOPTIME_MAX	= 99999999;

class CAni;
class CAniEffect
{
public:
    CAniEffect();
    virtual ~CAniEffect();

    void	Init	(void);

    BOOL	CreateEx(	const char* pszIndex,
                        int nLoopTime = _LOOPTIME_MAX,
                        DWORD dwFrameInterval = 40,
                        int nShowWay = _SHOWWAY_NORMAL,
                        DWORD dwLoopInterval = 0,
                        int nOffsetX = 0,
                        int nOffsetY = 0,
                        int nExigence = 0,
                        DWORD dwDelay = 0);

    BOOL	Create	(const char* pszIndex);

    void	Show	(int nPosX, int nPosY) const;
    BOOL	Process	();
    int		GetLoopTime() {return m_nLoopTime;}
    const char*	GetIndex(void) const {return m_szIndex;}

private:
    char	m_szIndex[32];
    int		m_nLoopTime;
    int		m_nShowWay;
    int		m_nType;
    int		m_nExigence;
    DWORD   m_dwDelay;
    DWORD	m_dwBeginTime;
    DWORD	m_dwFrameInterval, m_dwLastFrameTime;
    DWORD	m_dwLoopInterval, m_dwLastLoopTime;
    int		m_nFrame, m_nFrameAmount;
    CMyPos	m_posOffset;
    CAni*	m_pAni;
    BOOL	m_bInterval;
};

#include <deque>
using namespace std;
typedef deque<CAniEffect* > DEQUE_EFFECT;

#endif // !defined(AFX_ANIEFFECT_H__5A33BDF3_F599_4C93_B7BF_2BDD147EF35F__INCLUDED_)
