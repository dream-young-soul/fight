
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#include "skyrose.h"
#include "patternrose.h"
#include "3dgamemap.h"
#include "gamedataset.h"


void CSkyRose::Create(char* pszAniTitle, int iWorldX, int iWorldY,	int iCircleDir , int nRiseStep )
{
    m_iStatus				= PATTERNROSE_RISE;
    m_nShowWorldPoint.x		= iWorldX;
    m_nShowWorldPoint.y		= iWorldY;
    m_nOrigWorldPoint.x		= iWorldX;
    m_nOrigWorldPoint.y		= iWorldY;
    m_nRiseCount			= 0;
    m_nLoopFrame			= 0;
    m_nCurX					= rand() % 10;
    m_iYMoved				= 0;
    m_iAlpha				= 255;
    m_dwFrameTime			= 0;
    m_nRiseStep				= nRiseStep;
    m_iCircleDir			= iCircleDir;
    strcpy(this->m_szAniTitle, pszAniTitle);
}


void CSkyRose::Destroy()
{
    m_iStatus = PATTERNROSE_NONE;
}

void  CSkyRose::Show(void* pInfo)
{
    if (m_iStatus == PATTERNROSE_NONE || m_iStatus == PATTERNROSE_DESTROY)
    {
        return;
    }
    int nScreenX, nScreenY;
    g_objGameMap.World2Screen( m_nShowWorldPoint.x , m_nShowWorldPoint.y, nScreenX, nScreenY);
    CAni* pAni =	g_objGameDataSet.GetDataAni("ani/weather.ani", m_szAniTitle , EXIGENCE_IMMEDIATE, 30 * 1000);
    if (pAni)
        if (pAni->Show(m_nLoopFrame, nScreenX, nScreenY, m_iAlpha))
        {
            m_nLoopFrame = 0;
        }
        else
        {
            DWORD  dwNow = TimeGet();
            DWORD nInterval = 150;
            if ((dwNow - m_dwFrameTime) > nInterval )
            {
                m_dwFrameTime = dwNow;
                m_nLoopFrame++;
            }
        }
}


void CSkyRose::Process(void* pInfo)
{
    if (m_iStatus == PATTERNROSE_NONE)
    {
        return;
    }
    if (m_iStatus == PATTERNROSE_RISE)
    {
        m_iYMoved += m_nRiseStep;
        m_nShowWorldPoint.y = m_nOrigWorldPoint.y - m_iYMoved;
        m_nCurX	+= m_iCircleDir;
        m_nShowWorldPoint.x = m_nOrigWorldPoint.x + m_nCurX;
        if (m_iYMoved > 600)
        {
            m_iStatus    = PATTERNROSE_FALL;
            m_nRiseCount = rand() % 250;
            m_nCurX	 	 = 0;
            m_nOrigWorldPoint.x = m_nShowWorldPoint.x ;
            m_nOrigWorldPoint.y = m_nShowWorldPoint.y ;
        }
        m_iAlpha = 255;
        m_bAniIndex = 1;
        //	Show();
    }
    if (m_iStatus ==  PATTERNROSE_FALL)
    {
        if (m_nCurX ++ > m_nRiseCount)
        {
            m_iStatus = PATTERNROSE_DESTROY;
        }
        else
        {
            m_nShowWorldPoint.y = m_nOrigWorldPoint.y + m_nCurX * 2;
            m_iAlpha = 255 - m_nCurX  ;
            //	Show( );
        }
    }
}
