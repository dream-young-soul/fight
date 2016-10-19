
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// AniEffect.cpp: implementation of the CAniEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseFunc.h"
#include "nd_bitmap.h"
#include "Ani.h"
#include "AniEffect.h"
#include "gamedataset.h"
#include "3DGameMap.h"
#include "3DRoleData.h"


// constant ...
const char ANI_EFFECT[]	= "ani/effect.ani";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAniEffect::CAniEffect()
{
    this->Init();
}

CAniEffect::~CAniEffect()
{
}

//////////////////////////////////////////////////////////////////////
void CAniEffect::Init()
{
    m_nLoopTime	= 0;
    m_nFrame	= 0;
    m_nShowWay	= _SHOWWAY_NORMAL;
    m_dwFrameInterval	= 0;
    m_dwLastFrameTime	= 0;
    m_dwLoopInterval	= 0;
    m_dwLastLoopTime	= 0;
    m_dwBeginTime		= 0;
    m_nExigence			= 0;
    m_dwDelay			= 0;
    m_pAni				= NULL;
}

//////////////////////////////////////////////////////////////////////
BOOL CAniEffect::CreateEx	(const char* pszIndex, int nLoopTime, DWORD dwFrameInterval, int nShowWay, DWORD dwLoopInterval,
                             int nOffsetX, int nOffsetY, int nExigence, DWORD dwDelay)
{
    if (!pszIndex || nLoopTime <= 0)
    {
        return false;
    }
    m_nLoopTime	= nLoopTime;
    m_nShowWay	= nShowWay;
    m_dwFrameInterval	= dwFrameInterval;
    m_dwLoopInterval	= dwLoopInterval;
    m_dwBeginTime		=::TimeGet();
    m_dwDelay			= dwDelay;
    m_posOffset.x		= nOffsetX;
    m_posOffset.y		= nOffsetY;
    m_nExigence			= nExigence;
    strcpy(m_szIndex, pszIndex);
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CAniEffect::Create(const char* pszIndex)
{
    if (!pszIndex)
    {
        return false;
    }
    EffectInfo* pInfo = g_obj3DRoleData.GetEffectInfo((char*)pszIndex);
    if (!pInfo)
    {
        return false;
    }
    this->CreateEx(pInfo->szAniTitle, pInfo->dwLoopTime, pInfo->dwFrameInterval, pInfo->dwShowWay,
                   pInfo->dwLoopInterval, pInfo->nOffsetX, pInfo->nOffsetY, pInfo->nExigence, pInfo->dwDelay);
    return true;
}

//////////////////////////////////////////////////////////////////////
// return false if show over
void CAniEffect::Show(int nPosX, int nPosY) const
{
    if (!m_pAni)	// ani not load yet
    {
        return;
    }
    if (m_dwBeginTime == 0)	// not begin yet
    {
        return;
    }
    if (m_bInterval)
    {
        return;
    }
    CMyBitmap* pFrame	= m_pAni->GetFrame(m_nFrame);
    if (!pFrame)
    {
        return;
    }
    // get draw pos
    int nShowX	= nPosX - pFrame->GetWidth() / 2 + m_posOffset.x;
    int nShowY	= nPosY - pFrame->GetHeight() * 3 / 4 + m_posOffset.y;
    g_objGameMap.MapScaleShowPos(nShowX, nShowY);
    CMySize sizeBmp;
    pFrame->GetSize(sizeBmp.iWidth, sizeBmp.iHeight);
    sizeBmp.iWidth = sizeBmp.iWidth * g_objGameMap.GetScale() / _NORMAL_SCALE;
    sizeBmp.iHeight = sizeBmp.iHeight * g_objGameMap.GetScale() / _NORMAL_SCALE;
    pFrame->ShowEx(nShowX, nShowY, NULL, sizeBmp.iWidth, sizeBmp.iHeight, 0, m_nShowWay);
}

//////////////////////////////////////////////////////////////////////
// return false means this ani show has already finished
BOOL CAniEffect::Process()
{
    if (m_nLoopTime == 0)	// valid?
    {
        return false;
    }
    m_pAni	= g_objGameDataSet.GetDataAni((char*)ANI_EFFECT, m_szIndex, m_nExigence);
    if (!m_pAni)
    {
        return true;
    }
    DWORD dwTimeNow		=::TimeGet();
    if ((dwTimeNow - m_dwBeginTime) < m_dwDelay)
    {
        return true;
    }
    if (m_dwBeginTime == 0)	// not begin yet
    {
        m_dwBeginTime = dwTimeNow;
    }
    DWORD dwFrames		= m_pAni->GetFrameAmount();
    DWORD dwTimePerLoop	= dwFrames * m_dwFrameInterval + m_dwLoopInterval;
    DWORD dwTimeGoesBy	= dwTimeNow - m_dwBeginTime;
    if (dwTimeGoesBy >= dwTimePerLoop * m_nLoopTime)
    {
        return false;    // over already
    }
    // in loop interval?
    if ((dwTimeGoesBy % dwTimePerLoop) >= dwFrames * m_dwFrameInterval)
    {
        m_bInterval = true;
        return true;	// in loop interval
    }
    else
    {
        m_bInterval = false;
    }
    // show frame
    m_nFrame	= (dwTimeGoesBy % dwTimePerLoop) / m_dwFrameInterval;
    return true;
}


