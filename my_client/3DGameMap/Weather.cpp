
//**********************************************************
// 代码编辑器
//**********************************************************

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "Weather.h"
#include "3dgamemap.h"
#include "gamedataset.h"
#include "basefunc.h"
#include <string.h>
#include "../ndsound/ndsound.h"

#define HZK24c "hzk24k"
#define HZK24t "hzk24t"
#define const int _SCR_HEIGHT = 600;
int SIN[7] = { 0, 173, 342, 500, 642, 766, 766};
int COS[7] = {1000, 984, 939, 866, 766, 642, 642};
//------------------------------------------------------------
//
//树叶
//--------------------------------------------------------------
CLeaf::CLeaf(void)
{
    m_iStatus		= _LEAFSTATUS_NONE;
    return;
}


//--------------------------------------------------------------
BOOL CLeaf::Create(int iPosX, int iPosY, int iSceneDeepth, int fWind, int fGravity)
{
    if (m_iStatus != _LEAFSTATUS_NONE)
    {
        return FALSE;
    }
    m_iStatus			= _LEAFSTATUS_CREATE;
    int iWorldX, iWorldY;
    g_objGameMap.Screen2World(iPosX, iPosY, iWorldX, iWorldY);
    m_iStartX			= iWorldX;
    m_iStartY			= iWorldY;
    m_iSceneDeepth		= iSceneDeepth;
    m_fWind				= fWind;
    m_dwLifeBeginTime	= TimeGet();
    m_dwElapseTime		= 0;
    m_iSceneDeepth	   %= 36;
    m_iLayerFrame		= iSceneDeepth / 9 * 6;
    m_iSize				= 4 + 4 * (36 - m_iSceneDeepth) / 36 ;
    m_fSpeed			= 2 + 16 * m_iSceneDeepth / 36;
    m_fGravity			= fGravity * (36 - m_iSceneDeepth) / 36 + 2 ;
    m_fSinA				= SIN[(abs(m_fWind) + rand()%10)/10];
    if (m_fWind < 0)
    {
        m_fSinA = - m_fSinA;
    }
    m_fCosA				= COS[(abs(m_fWind) + rand()%10)/10];
    this->m_dwLifeTime		= 10000 * (36 - m_iSceneDeepth) / 36 + 500 + 9000 * abs(m_fSinA) / 1000;
    this->m_ibmpFrameIndex	= m_iLayerFrame;
    this->m_nLoopCount		= 0;
    this->m_dStayGroundTime	= 0;
    this->m_iSplashX		= 0;
    this->m_iSplashY		= 0;
    this->m_bLieOnGround	= 0;
    this->m_iLastFrame		= -1;
    this->m_iLastPosX		= 0;
    this->m_iLastPosY		= 0;
    return TRUE;
}

void CLeaf::Show()
{
    if (m_iStatus == _LEAFSTATUS_NONE)
    {
        return;
    }
    DWORD dwNow = TimeGet();
    m_dwElapseTime	= dwNow - this->m_dwLifeBeginTime;
    if (m_dwElapseTime >= m_dwLifeTime)
    {
        if (m_iStatus == _LEAFSTATUS_CREATE)
        {
            this->m_dwLifeBeginTime	= TimeGet();
            m_dwElapseTime		= 0;
            this->m_iStatus			= _LEAFSTATUS_SPLASH;
            if (!this->m_iSplashX || !this->m_iSplashY)
            {
                this->m_iStatus = _LEAFSTATUS_NONE;
            }
            int nCellx, nCelly;
            g_objGameMap.World2Cell(this->m_iSplashX, this->m_iSplashY, nCellx, nCelly);
            int nCellW, nCellH;
            CMySize s;
            g_objGameMap.GetMapSize(s);
            nCellW = s.iWidth;
            nCellH = s.iHeight;
            if (nCellx < 0 || nCellx > nCellW || nCelly < 0 || nCelly > nCellH)
            {
                this->m_iStatus = _LEAFSTATUS_NONE;
            }
#ifndef _NOTUSETRY
            try
            {
#endif
                DWORD uMask;
                CellInfo* c;
                c = g_objGameMap.GetCell(nCellx, nCelly);
                LayerInfo* pLayer = g_objGameMap.GetLastLayerInfo(c);
                uMask = pLayer->usTerrain;
                if (uMask == 1)//UNSTAY
                {
                    this->m_bLieOnGround = 0;
                }
                else
                {
                    this->m_bLieOnGround = 1;
                }
#ifndef _NOTUSETRY
            }
            catch(...)
            {
                this->m_iStatus = _LEAFSTATUS_NONE;
            }
#endif
        }
    }
    if (m_iStatus == _LEAFSTATUS_CREATE)
    {
        int dOffset	= (int)((m_fSpeed * m_dwElapseTime * 1000  *  10 / 1800 / 1000 ) + (m_fGravity * m_dwElapseTime * 1000 / 1800   * m_dwElapseTime * 1000 / 1800 * 10 / 1000 / 1000 / 2))  ; // *10
        if (dOffset <= 0 && m_dwElapseTime)
        {
            this->m_iStatus	= _LEAFSTATUS_NONE;
        }
        else
        {
            g_objGameMap.World2Screen(this->m_iStartX, this->m_iStartY, m_iScreenX, m_iScreenY);
            int iX0	= m_iScreenX + dOffset * m_fSinA * m_iSize / 1000 / 10;
            int iY0	= m_iScreenY + dOffset * m_fCosA * m_iSize / 1000 / 10;
            if (m_nLoopCount % 3 == 1)
            {
                m_ibmpFrameIndex++;
            }
            if (m_ibmpFrameIndex >= this->m_iLayerFrame +  _LEAF_FRAME / 4)
            {
                m_ibmpFrameIndex = this->m_iLayerFrame;
            }
            CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileName);
            if (pAni)
            {
                pAni->Show(m_ibmpFrameIndex, iX0, iY0, 255);
            }
            int nWorldx, nWorldy;
            int iX1	= iX0  + m_iSize * m_fSinA / 1000 ;
            int iY1	= iY0  + m_iSize * m_fCosA / 1000 ;
            g_objGameMap.Screen2World(iX1, iY1, nWorldx, nWorldy);
            this->m_iSplashX	= nWorldx;
            this->m_iSplashY	= nWorldy;
            this->m_nLoopCount ++;
        }
    }
    else if (m_iStatus == _LEAFSTATUS_SPLASH)
    {
        this->m_dStayGroundTime ++;
        int nScreenx, nScreeny ;
        g_objGameMap.World2Screen(this->m_iSplashX, this->m_iSplashY, nScreenx, nScreeny);
        if (nScreenx < -5 || nScreenx > _SCR_WIDTH || nScreeny < -5 || nScreeny > _SCR_HEIGHT)
        {
            m_iStatus	= _LEAFSTATUS_NONE;
            return;
        }
        CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileName);
        if (!m_bLieOnGround)
        {
            if (m_dStayGroundTime > 20)
            {
                m_iStatus = _LEAFSTATUS_NONE;
            }
            if (pAni)
            {
                pAni->Show(m_ibmpFrameIndex, nScreenx, nScreeny, 210 - m_dStayGroundTime * 10);
            }
        }
        else
        {
            if (m_dStayGroundTime > 480)
            {
                m_iStatus	= _LEAFSTATUS_NONE;
            }
            else if (pAni)
            {
                pAni->Show(m_ibmpFrameIndex, nScreenx, nScreeny, 250 - m_dStayGroundTime / 2);
            }
        }
    }
    return;
}

//--------------------------------------------------------------
void CLeaf::Destroy(void)
{
    this->m_iStatus = _LEAFSTATUS_NONE;
}

//--------------------------------------------------------------

void CLeaf::Process()
{
    Show();
    return;
}


//--------------------------------------------------------------


void CLeafSys::Pause()
{
    DXSetVolume("sound/leaf.wav", 0, 0, 0);
    m_iStatu =  _LEAF_PAUSE;
    m_dwPauseTime = TimeGet();
}
void CLeafSys::Continue()
{
    if (m_iStatu !=  _LEAF_PAUSE)
    {
        return;
    }
    DWORD dwElapseTime = TimeGet() -  m_dwPauseTime;
    DWORD i;
    for(i = 0 ; i < this->m_Leaf.size(); i++)
    {
        m_Leaf[i]->m_dwLifeBeginTime += dwElapseTime;
    }
    int nSelfVolume = m_iLeafAmount * 5 * 100 / LEAF_MAX_AMOUNT;
    if (nSelfVolume > 120)
    {
        nSelfVolume = 120;
    }
    else if (nSelfVolume < 50)
    {
        nSelfVolume = 50;
    }
    DXSetVolume("sound/leaf.wav", nSelfVolume, 0, 0);
    m_iStatu = _LEAF_BEGIN;
}


BOOL CLeafSys::Create(int nWindDir = 20 , int nLevel = 2, int nType = 1)
{
    Destroy();
    char szTitle[100];
    switch(nType)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        sprintf(szTitle, "%s%d", "Leaf", nType);
        break;
    case 6:
    case 7:
        sprintf(szTitle, "%s%d", "Leaf", 6);
        break;
    case 8:
        sprintf(szTitle, "%s%d", "flying", 1);
        break;
    case 9:
    case 10:
        return FALSE;
    default:
        return FALSE;
    }
    strcpy(m_szFileName, szTitle);
    this->m_iStatu	= _LEAF_BEGIN;
    m_iLeafAmount	= 1;
    m_dwPauseTime	= 0;
    this->m_iTargetAmount = nLevel ;
    this->m_nWindDirection = nWindDir;
    int i;
    for(i = 0; i < LEAF_MAX_AMOUNT; i++)
    {
        CLeaf* leaf = new CLeaf();
        m_Leaf.push_back(leaf);
    }
    int nSelfVolume = m_iLeafAmount * 500 / LEAF_MAX_AMOUNT;
    if (nSelfVolume > 120)
    {
        nSelfVolume = 120;
    }
    else if (nSelfVolume < 30)
    {
        nSelfVolume = 30;
    }
    DXPlaySound("sound/leaf.wav", 0, 0, 0, -1, nSelfVolume);
    return TRUE;
}


void CLeafSys::Process(int& nStatus)
{
    if ( m_iStatu == _LEAF_NONE )
    {
        return;
    }
    if ( m_iStatu == _LEAF_PAUSE)
    {
        return;
    }
    for (int i = 0; i < this->m_iLeafAmount ; i++)
    {
        if (this->m_iStatu == _LEAF_BEGIN && m_iLeafAmount < m_iTargetAmount )
        {
            int nInternal = m_iLeafAmount * 2;
            if (nInternal > 99)
            {
                nInternal = 99;
            }
            nInternal = 100 - nInternal;
            if (rand() % nInternal == 0)
            {
                m_iLeafAmount	+= 1;
                int nSelfVolume = m_iLeafAmount * 300 / LEAF_MAX_AMOUNT;
                if (nSelfVolume > 120)
                {
                    nSelfVolume = 120;
                }
                else if (nSelfVolume < 20)
                {
                    nSelfVolume = 20;
                }
                DXSetVolume("sound/leaf.wav", nSelfVolume, 0, 0);
            }
        }
        else
        {
            if (this->m_iStatu == _LEAF_STOP )
            {
                if (m_iLeafAmount > 2)
                {
                    int nInternal = m_iLeafAmount * 2 ;
                    if (nInternal > 99)
                    {
                        nInternal = 99;
                    }
                    nInternal = 100 - nInternal;
                    if (rand() % nInternal == 0)
                    {
                        m_iLeafAmount -= 1;
                        int nSelfVolume = m_iLeafAmount * 300 / LEAF_MAX_AMOUNT;
                        if (nSelfVolume > 120)
                        {
                            nSelfVolume = 120;
                        }
                        else if (nSelfVolume < 20)
                        {
                            nSelfVolume = 20;
                        }
                        DXSetVolume("sound/leaf.wav", nSelfVolume, 0, 0);
                    }
                }
                else
                {
                    Destroy();
                    nStatus = 0;
                    return;
                }
            }
            else
            {
                if (this->m_iStatu == _LEAF_RESET)
                    if (m_iLeafAmount < m_iTargetAmount)
                    {
                        int nInternal = m_iLeafAmount * 2 ;
                        if (nInternal > 99)
                        {
                            nInternal = 99;
                        }
                        nInternal = 100 - nInternal;
                        if (rand() % nInternal == 0)
                        {
                            m_iLeafAmount += 1;
                            int nSelfVolume = m_iLeafAmount * 300 / LEAF_MAX_AMOUNT;
                            if (nSelfVolume > 120)
                            {
                                nSelfVolume = 120;
                            }
                            else if (nSelfVolume < 20)
                            {
                                nSelfVolume = 20;
                            }
                            DXSetVolume("sound/leaf.wav", nSelfVolume, 0, 0);
                        }
                    }
                    else if (m_iLeafAmount > m_iTargetAmount && m_iLeafAmount > 1)
                    {
                        int nInternal = m_iLeafAmount * 2 ;
                        if (nInternal > 99)
                        {
                            nInternal = 99;
                        }
                        nInternal = 100 - nInternal;
                        if (rand() % nInternal == 0)
                        {
                            m_iLeafAmount -= 1;
                            int nSelfVolume = m_iLeafAmount * 300 / LEAF_MAX_AMOUNT;
                            if (nSelfVolume > 120)
                            {
                                nSelfVolume = 120;
                            }
                            else if (nSelfVolume < 20)
                            {
                                nSelfVolume = 20;
                            }
                            DXSetVolume("sound/leaf.wav", nSelfVolume, 0, 0);
                        }
                    }
            }
        }
        if (!m_Leaf[i]->IsCreated())
        {
            int iPosX;
            if (this->m_nWindDirection >= 0)
            {
                iPosX	= rand() % (_SCR_WIDTH +  10 * m_nWindDirection)  - 10 * m_nWindDirection;
            }
            else
            {
                iPosX	= rand() % (_SCR_WIDTH - 10 * m_nWindDirection) + 20;
            }
            int iPosY		= rand() % 250 - 120;
            int iSceneDeepth = rand() % 32 + 4;
            m_Leaf[i]->Create(iPosX, iPosY, iSceneDeepth, m_nWindDirection + rand() % 8, 2);
            m_Leaf[i]->SetPicture(m_szFileName);
        }
        m_Leaf[i]->Process();
    }
    return;
}


void CLeafSys::LeafStop()
{
    this->m_iStatu = _LEAF_STOP;
}

void CLeafSys::Destroy()
{
    DWORD i;
    for( i = 0 ; i < m_Leaf.size(); i++)
    {
        SAFE_DELETE(m_Leaf[i]);
    }
    m_Leaf.clear();
    this->m_iStatu = _LEAF_NONE;
    DXCloseSound("sound/leaf.wav", 0, 0);
}
void CLeafSys::SetLeaf(int nWindDir, int nLevel = 0)
{
    if (nLevel >= LEAF_MAX_AMOUNT)
    {
        nLevel = LEAF_MAX_AMOUNT - 1;
    }
    if (m_iLeafAmount != nLevel)
    {
        m_iStatu = _LEAF_RESET;
        m_iTargetAmount = nLevel;
    }
    for(DWORD i = 0; i < m_Leaf.size(); i++)
    {
        m_Leaf[i]->m_fSinA				= SIN[(abs(nWindDir) + rand()%10)/10];
        if (nWindDir < 0)
        {
            m_Leaf[i]->m_fSinA = - m_Leaf[i]->m_fSinA;
        }
        m_Leaf[i]->m_fCosA				= COS[(abs(nWindDir) + rand()%10)/10];
    }
    m_nWindDirection = nWindDir;
}






//--------------------------------------------------------------
//雪
//--------------------------------------------------------------


CSnow::CSnow(void)
{
    m_iStatus		= _SNOWSTATUS_NONE;
    return;
}


//--------------------------------------------------------------
void CSnow::Reset(void)
{
    m_iStatus		= _SNOWSTATUS_NONE;
}

//--------------------------------------------------------------
BOOL CSnow::Create(int iPosX, int iPosY, int iSceneDeepth, int fWind, int fGravity)
{
    if (m_iStatus != _SNOWSTATUS_NONE)
    {
        return FALSE;
    }
    m_iStatus			= _SNOWSTATUS_CREATE;
    m_iStartX			= iPosX;
    m_iStartY			= iPosY;
    m_iSceneDeepth		= iSceneDeepth % 16;
    m_fWind				= fWind;
    m_dwLifeBeginTime	= TimeGet();
    m_dwElapseTime		= 0;
    m_fSinA				= SIN[(abs(m_fWind) + rand()%10)/10];
    if (m_fWind < 0)
    {
        m_fSinA = - m_fSinA;
    }
    m_fCosA				= COS[(abs(m_fWind) + rand()%10)/10];
    m_fSpeed			= 2 + (16 - m_iSceneDeepth) / 16;
    m_fGravity			= 1;
    m_dwLifeTime		= 6000 * (24 - m_iSceneDeepth) / 24 + 600 + 2500 * abs(m_fSinA) / 1000;
    m_nStayTick			= 0;
    m_iLastPosX			= 0;
    m_iLastPosY			= 0;
    return TRUE;
}

//--------------------------------------------------------------

void CSnow::ShowImage ( int iPosX, int iPosY )
{
    int iWorldX, iWorldY;
    g_objGameMap.Screen2World(iPosX, iPosY, iWorldX, iWorldY);
    m_iSplashX	= iWorldX;
    m_iSplashY	= iWorldY;
    if ((rand() % 6) == 1)
    {
        iPosX	+= 3 - rand() % 6;
    }
    CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileName);
    if (pAni)
    {
        pAni->Show(0, iPosX, iPosY,  (36 - m_iSceneDeepth + 5) * 6);
    }
    return;
}


void CSnow::Show(void)
{
    if (m_iStatus == _SNOWSTATUS_NONE)
    {
        return;
    }
    m_dwElapseTime	= TimeGet() - m_dwLifeBeginTime;
    if (m_dwElapseTime >= m_dwLifeTime)
    {
        if (m_iStatus == _SNOWSTATUS_CREATE)
        {
            m_dwLifeBeginTime	= TimeGet();
            m_dwElapseTime		= 0;
            m_dwLifeTime		= 2000;
            m_iStatus			= _SNOWSTATUS_SPLASH;
        }
        else if (m_iStatus == _SNOWSTATUS_SPLASH)
        {
            m_iStatus	= _SNOWSTATUS_NONE;
        }
    }
    if (m_iStatus == _SNOWSTATUS_CREATE)
    {
        int dOffset		= m_fSpeed * m_dwElapseTime * 10 / 20;
        if (dOffset <= 0 && m_dwElapseTime)
        {
            m_iStatus	= _SNOWSTATUS_NONE;
        }
        else
        {
            int iPosX	= m_iStartX + dOffset * m_fSinA / 10 / 1000;
            int iPosY	= m_iStartY + dOffset * m_fCosA / 10 / 1000;
            this->ShowImage(iPosX, iPosY);
        }
    }
    else if (m_iStatus == _SNOWSTATUS_SPLASH)
    {
        int iScreenX = 0, iScreenY = 0;
        g_objGameMap.World2Screen(m_iSplashX, m_iSplashY, iScreenX, iScreenY);
        CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileName);
        if (pAni)
        {
            pAni->Show(0, iScreenX, iScreenY, 242 - m_nStayTick * 8);
        }
        if (++m_nStayTick >= 30)
        {
            m_iStatus	= _SNOWSTATUS_NONE;
            m_nStayTick = 0;
        }
    }
    return;
}

//--------------------------------------------------------------
void CSnow::Destroy(void)
{
    return;
}
//--------------------------------------------------------------

void CSnow::Process()
{
    Show();
    return;
}

//--------------------------------------------------------------

//--------------------------------------------------------------

void CSnowSys::Pause()
{
    m_iStatu = _SNOW_PAUSE;
    m_dwPauseTime = TimeGet();
    DXSetVolume("sound/sand.wav", 0, 0, 0);
}
void CSnowSys::Continue()
{
    if (m_iStatu != _SNOW_PAUSE)
    {
        return;
    }
    DWORD dwElapseTime = TimeGet() -  m_dwPauseTime;
    DWORD i;
    for(i = 0 ; i < this->m_Snow.size(); i++)
    {
        m_Snow[i]->m_dwLifeBeginTime += dwElapseTime;
    }
    m_iStatu = _SNOW_BEGIN;
    int nSelfVolume = m_iTargetAmount * 500 / LEAF_MAX_AMOUNT;
    if (nSelfVolume > 120)
    {
        nSelfVolume = 120;
    }
    else if (nSelfVolume < 20)
    {
        nSelfVolume = 20;
    }
    DXSetVolume("sound/sand.wav", nSelfVolume, 0, 0);
}
void CSnowSys::Init(void)
{
    m_iSnowAmount	= 0;
    m_iStatu = _SNOW_NONE;
}

//--------------------------------------------------------------
void CSnowSys::Destroy(void)
{
    for(DWORD i = 0 ; i < m_Snow.size(); i++)
    {
        SAFE_DELETE(m_Snow[i]);
    }
    m_Snow.clear();
    DXCloseSound("sound/sand.wav", 0, 0);
    Init();
}

//--------------------------------------------------------------


BOOL CSnowSys::Create(int iWindDir, int iLevel, DWORD dwColor = 0x00ffffff)
{
    strcpy(m_szFileName, "Snow");
    m_dwPauseTime	= 0;
    m_iSnowAmount	= 4;
    m_iTargetAmount = iLevel;
    m_iStatu		= _SNOW_BEGIN;
    m_iWindDir		= iWindDir;
    for(int i = 0; i < _MAX_AMOUNT ; i++)
    {
        CSnow* obj_snow = new CSnow();
        m_Snow.push_back(obj_snow);
    }
    int nSelfVolume = m_iSnowAmount * 200 / LEAF_MAX_AMOUNT;
    if (nSelfVolume > 120)
    {
        nSelfVolume = 120;
    }
    else if (nSelfVolume < 20)
    {
        nSelfVolume = 20;
    }
    DXPlaySound("sound/sand.wav", 0, 0, 0, -1, nSelfVolume);
    return true;
}

//--------------------------------------------------------------
void CSnowSys::SetSnow(int nWindDir, int nLevel)
{
    if (m_iTargetAmount != nLevel)
    {
        m_iStatu = _SNOW_RESET;
        m_iTargetAmount = nLevel;
    }
    for(DWORD i = 0; i < m_Snow.size(); i++)
    {
        m_Snow[i]->m_fSinA				= SIN[(abs(nWindDir) + rand()%10)/10];
        if (nWindDir < 0)
        {
            m_Snow[i]->m_fSinA = - m_Snow[i]->m_fSinA;
        }
        m_Snow[i]->m_fCosA				= COS[(abs(nWindDir) + rand()%10)/10];
    }
    this->m_iWindDir = nWindDir;
}
void CSnowSys::Reset(void)
{
    m_iSnowAmount	= 4;
    for(DWORD i = 0; i < m_Snow.size(); i++)
    {
        m_Snow[i]->Reset();
    }
    int nSelfVolume = m_iSnowAmount * 400 / LEAF_MAX_AMOUNT;
    if (nSelfVolume > 120)
    {
        nSelfVolume = 120;
    }
    else if (nSelfVolume < 20)
    {
        nSelfVolume = 20;
    }
    DXSetVolume("sound/sand.wav", nSelfVolume, 0, 0);
}
void CSnowSys::SnowStop()
{
    //	DXCloseSound("sound/sand.wav",0,0);
    m_iStatu = _SNOW_STOP;
}
//--------------------------------------------------------------


void CSnowSys::Process(int& nStatus)
{
    if (m_iStatu == _SNOW_NONE)
    {
        return;
    }
    if (m_iStatu == _SNOW_PAUSE)
    {
        return;
    }
    for (int i = 0; i < m_iSnowAmount; i++)
    {
        if (m_iStatu == _SNOW_BEGIN && m_iSnowAmount < m_iTargetAmount)
        {
            int nInternal = m_iSnowAmount ;
            if (nInternal > 199)
            {
                nInternal = 199;
            }
            nInternal = 200 - nInternal;
            if (rand() % nInternal == 0)
            {
                m_iSnowAmount	++;
                int nSelfVolume = m_iSnowAmount * 4 * 100 / LEAF_MAX_AMOUNT;
                if (nSelfVolume > 120)
                {
                    nSelfVolume = 120;
                }
                else if (nSelfVolume < 20)
                {
                    nSelfVolume = 20;
                }
                DXSetVolume("sound/sand.wav", nSelfVolume, 0, 0);
            }
        }
        else if (this->m_iStatu == _SNOW_STOP )
        {
            if (m_iSnowAmount > 1)
            {
                int nInternal = m_iSnowAmount ;
                if (nInternal > 199)
                {
                    nInternal = 199;
                }
                nInternal = 200 - nInternal;
                if (rand() % nInternal == 0)
                {
                    m_iSnowAmount -= 1;
                    int nSelfVolume = m_iSnowAmount * 400 / LEAF_MAX_AMOUNT;
                    if (nSelfVolume > 120)
                    {
                        nSelfVolume = 120;
                    }
                    else if (nSelfVolume < 20)
                    {
                        nSelfVolume = 20;
                    }
                    DXSetVolume("sound/sand.wav", nSelfVolume, 0, 0);
                }
            }
            else
            {
                this->Destroy();
                nStatus = 0;
                return;
            }
        }
        else if (this->m_iStatu == _SNOW_RESET)
            if (this->m_iTargetAmount > m_iSnowAmount)
            {
                int nInternal = m_iSnowAmount ;
                if (nInternal > 199)
                {
                    nInternal = 199;
                }
                nInternal = 200 - nInternal;
                if (rand() % nInternal == 0)
                {
                    m_iSnowAmount	++;
                    int nSelfVolume = m_iSnowAmount * 400 / LEAF_MAX_AMOUNT;
                    if (nSelfVolume > 120)
                    {
                        nSelfVolume = 120;
                    }
                    else if (nSelfVolume < 20)
                    {
                        nSelfVolume = 20;
                    }
                    DXSetVolume("sound/sand.wav", nSelfVolume, 0, 0);
                }
            }
            else if (m_iSnowAmount > m_iTargetAmount && m_iSnowAmount > 15)
            {
                int nInternal = m_iSnowAmount ;
                if (nInternal > 199)
                {
                    nInternal = 199;
                }
                nInternal = 200 - nInternal;
                if (rand() % nInternal == 0)
                {
                    m_iSnowAmount -= 1;
                    int nSelfVolume = m_iSnowAmount * 400 / LEAF_MAX_AMOUNT;
                    if (nSelfVolume > 120)
                    {
                        nSelfVolume = 120;
                    }
                    else if (nSelfVolume < 20)
                    {
                        nSelfVolume = 20;
                    }
                    DXSetVolume("sound/sand.wav", nSelfVolume, 0, 0);
                }
            }
        if (m_iSnowAmount > _MAX_AMOUNT)
        {
            m_iSnowAmount = _MAX_AMOUNT;
        }
        if (m_iSnowAmount < 0)
        {
            this->Destroy();
            return;
        }
        if (!m_Snow[i]->IsCreated())
        {
            int iPosX;
            if (m_iWindDir >= 5)
            {
                iPosX	= rand() % (_SCR_WIDTH +  500)  - 500;
            }
            else if (m_iWindDir < -2)
            {
                iPosX	= rand() % (_SCR_WIDTH + 400);
            }
            else
            {
                iPosX = rand() % (_SCR_WIDTH +  300)  - 150;
            }
            int iPosY		= -1 * (rand() % 50) - 20;
            int iSceneDeepth = rand() % 16 + 4;
            int fWind   = m_iWindDir - rand() % 5;
            m_Snow[i]->Create(iPosX, iPosY, iSceneDeepth, fWind, 1);
            m_Snow[i]->SetImageBmp(m_szFileName);
        }
        m_Snow[i]->Process();
    }
    return;
}


//--------------------------------------------------------------
//雨
//--------------------------------------------------------------

//--------------------------------------------------------------
CRain::CRain(void)
{
    m_iStatus		= _RAINSTATUS_NONE;
    return;
}

//--------------------------------------------------------------
void CRain::Reset(void)
{
    m_iStatus		= _RAINSTATUS_NONE;
}

//--------------------------------------------------------------
int	CRain::GetInt(double dValue)
{
    if ((int)(dValue + 0.5) > (int)dValue)
    {
        return int(dValue) + 1;
    }
    else
    {
        return int(dValue);
    }
}

//--------------------------------------------------------------
BOOL CRain::Create(int iPosX, int iPosY, int iSceneDeepth, int fWind, int fGravity, int nLevel, DWORD dwColor)
{
    if (m_iStatus != _RAINSTATUS_NONE)
    {
        return FALSE;
    }
    m_iStatus			= _RAINSTATUS_CREATE;
    m_iStartX			= iPosX;
    m_iStartY			= iPosY;
    m_iSceneDeepth		= iSceneDeepth;
    m_iSceneDeepth	   %= 36;
    m_fWind				= fWind ;
    m_dwLifeBeginTime	= TimeGet();
    m_dwElapseTime		= 0;
    m_dwColor			= dwColor;
    m_iSize				= 6 + 4 * (36 - m_iSceneDeepth) / 36 ;
    m_iSize += nLevel / 400;
    m_fSpeed			= 1 + 36 * m_iSceneDeepth / 36;
    m_fGravity			= fGravity * (36 - m_iSceneDeepth) / 9  ;
    m_nLevel			= nLevel;
    m_fSinA				= SIN[abs(m_fWind)/10];
    if (m_fWind < 0)
    {
        m_fSinA = - m_fSinA;
    }
    m_fCosA				= COS[abs(m_fWind)/10];
    int nBase = 3600;
    if (_SCR_WIDTH == 1024)
    {
        nBase = 4600;
    }
    m_dwLifeTime		= (nBase + abs(m_fSinA) / 10) * (36 - m_iSceneDeepth) / 36 ;
    m_iSplashX			= 0;
    m_iSplashY			= 0;
    m_iRippleFrameIndex	= 0;
    m_iLastPosX			= 0;
    m_iLastPosY			= 0;
    return TRUE;
}

//--------------------------------------------------------------


void CRain::Show()
{
    if (m_iStatus == _RAINSTATUS_NONE)
    {
        return;
    }
    m_dwElapseTime	= TimeGet() - m_dwLifeBeginTime;
    if (m_dwElapseTime >= m_dwLifeTime)
    {
        if (m_iStatus == _RAINSTATUS_CREATE)
        {
            m_dwLifeBeginTime	= TimeGet();
            m_dwElapseTime		= 0;
            m_dwLifeTime		= 1800 * (36 - m_iSceneDeepth) / 36 + 10 * m_fSinA + 80 ;
            m_iStatus			= _RAINSTATUS_SPLASH;
            WORD uMask;
            int nCellx, nCelly;
            g_objGameMap.World2Cell(this->m_iSplashX, this->m_iSplashY, nCellx, nCelly);
            CellInfo* c;
            c = g_objGameMap.GetCell(nCellx, nCelly);
            LayerInfo* pLayer = g_objGameMap.GetLastLayerInfo(c);
            this->m_bOnWater = 0;
            if (pLayer)
            {
                uMask = pLayer->usTerrain;
                if (uMask == 1)
                {
                    this->m_bOnWater = 1;
                }
            }
        }
        else if (m_iStatus == _RAINSTATUS_SPLASH)
        {
            m_iStatus	= _RAINSTATUS_NONE;
        }
    }
    if (m_iStatus == _RAINSTATUS_CREATE)
    {
        float dElapseTime	= (float)(m_dwElapseTime / 380.0f);
        float dOffsetA		= (float)(m_fSpeed * dElapseTime);
        float dOffsetB		= (float)(m_fGravity * dElapseTime * dElapseTime / 2.0f);
        float dOffset		= (float)(dOffsetA + dOffsetB);
        //	int 	dOffset		= (m_fSpeed * m_dwElapseTime * 1000  *  10 / 380/1000 ) + (m_fGravity* m_dwElapseTime * 1000/380   * m_dwElapseTime * 1000/380 * 10 /1000 / 1000 /2)  ;// *10
        if (dOffset <= 0 && m_dwElapseTime)
        {
            m_iStatus	= _RAINSTATUS_NONE;
        }
        else
        {
            int iX0	= (int)(m_iStartX + dOffset * m_fSinA * m_iSize / 1000);
            int iY0	= (int)(m_iStartY + dOffset * m_fCosA * m_iSize / 1000);
            int iX1	= (int)(iX0 + m_iSize * m_fSinA / 1000) ;
            int iY1	= (int)(iY0 + m_iSize * m_fCosA / 1000) ;
            DWORD dwColor;
            int nAlpha;
            if (this->m_nLevel / (_MAX_AMOUNT / 3) < 1)
            {
                nAlpha = 110  + (36 - m_iSceneDeepth);
            }
            else if (this->m_nLevel / (_MAX_AMOUNT / 3) < 2)
            {
                nAlpha = 130 + (36 - m_iSceneDeepth);
            }
            else
            {
                nAlpha = 140 + (36 - m_iSceneDeepth);
            }
            if (m_dwColor == 0x00ffffff)
            {
                dwColor = 0x00000000;
                dwColor |= ((36 - m_iSceneDeepth + 2) * 3 + 100) << 16;
                dwColor |= ((36 - m_iSceneDeepth + 2) * 3 + 100) << 8;
                dwColor |= ((36 - m_iSceneDeepth + 2) * 3 + 100);
                dwColor |= nAlpha << 24;
            }
            else
            {
                dwColor = m_dwColor;
                int nr = dwColor >> 16 & 0x00ff;
                nr = nr - m_iSceneDeepth ;
                if (nr < 0)
                {
                    nr = 0;
                }
                int ng = dwColor >> 8 & 0x0000ff;
                ng = ng - m_iSceneDeepth;
                if (ng < 0)
                {
                    ng = 0;
                }
                int nb = dwColor & 0x000000ff;
                nb = nb - m_iSceneDeepth;
                if (nb < 0)
                {
                    nb = 0;
                }
                dwColor = (nAlpha << 24) | (nr << 16) | (ng << 8) | nb;
            }
            CMyBitmap::ShowLine(iX0, iY0, iX1, iY1, dwColor);
            g_objGameMap.Screen2World(iX1, iY1, m_iSplashX, m_iSplashY);
            m_iRippleFrameIndex	= 0;
        }
    }
    else if (m_iStatus == _RAINSTATUS_SPLASH)
    {
        int iScreenX, iScreenY;
        g_objGameMap.World2Screen(m_iSplashX, m_iSplashY, iScreenX, iScreenY);
        CMyBitmap::PrepareSprite();
        if (m_bOnWater)
        {
            m_iRippleFrameIndex++;
            CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileName[1]);
            if (pAni)
            {
                if (m_dwColor != 0x00ffffff)
                {
                    CMyBitmap* pBmp = pAni->GetFrame(m_iRippleFrameIndex++);
                    if (m_iRippleFrameIndex >= pAni->GetFrameAmount())
                    {
                        m_iStatus	= _RAINSTATUS_NONE;
                    }
                    int r, g, b;
                    r = m_dwColor >> 16 & 0xff;
                    g = m_dwColor >> 8 & 0xff;
                    b = m_dwColor & 0xff;
                    pBmp->SetColor(200, r, g, b);
                    pBmp->Show(iScreenX, iScreenY);
                }
                else if (pAni->Show(m_iRippleFrameIndex, iScreenX, iScreenY))
                {
                    m_iStatus	= _RAINSTATUS_NONE;
                }
            }
        }
        else
        {
            CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileName[0]);
            if (pAni)
            {
                if (m_dwColor != 0x00ffffff)
                {
                    CMyBitmap* pBmp = pAni->GetFrame(m_iRippleFrameIndex++);
                    if (m_iRippleFrameIndex >= pAni->GetFrameAmount())
                    {
                        m_iStatus	= _RAINSTATUS_NONE;
                    }
                    int r, g, b;
                    r = m_dwColor >> 16 & 0xff;
                    g = m_dwColor >> 8 & 0xff;
                    b = m_dwColor & 0xff;
                    pBmp->SetColor(200, r, g, b);
                    pBmp->Show(iScreenX, iScreenY);
                }
                else if (pAni->Show(m_iRippleFrameIndex++, iScreenX, iScreenY))
                {
                    m_iStatus	= _RAINSTATUS_NONE;
                }
            }
        }
    }
    return;
}

//--------------------------------------------------------------
void CRain::Destroy(void)
{
    return;
}

void CRain::Process()
{
    Show();
    return;
}

//--------------------------------------------------------------

CRainSys::CRainSys()
{
    m_iStatu = _RAIN_NONE;
}

void CRainSys::Pause()
{
    DXSetVolume("sound/rain.wav", 0, 0, 0);
    m_iStatu =  _RAIN_PAUSE;
    m_dwPauseTime = TimeGet();
}
void CRainSys::Continue()
{
    int nSelfVolume = m_iTargetAmount * 4 * 100 / _MAX_AMOUNT;
    if (nSelfVolume > 140)
    {
        nSelfVolume = 140;
    }
    else if (nSelfVolume < 65)
    {
        nSelfVolume = 65;
    }
    DXSetVolume("sound/rain.wav", nSelfVolume, 0, 0);
    if (m_iStatu !=  _RAIN_PAUSE)
    {
        return;
    }
    DWORD dwElapseTime = TimeGet() -  m_dwPauseTime;
    DWORD i;
    for(i = 0 ; i < this->m_Rain.size(); i++)
    {
        m_Rain[i]->m_dwLifeBeginTime += dwElapseTime;
    }
    m_iStatu = _RAIN_BEGIN;
}


void CRainSys::SetColor(DWORD dwColor)
{
    for(DWORD i = 0; i < m_Rain.size(); i++)
    {
        m_Rain[i]->m_dwColor = dwColor;
    }
}

void CRainSys::SetRain(int nWindDir, int nLevel)
{
    if (m_iRainAmount != nLevel)
    {
        m_iStatu = _RAIN_RESET;
        m_iTargetAmount = nLevel;
    }
    for(DWORD i = 0; i < m_Rain.size(); i++)
    {
        m_Rain[i]->m_fSinA				= SIN[(abs(nWindDir) + rand()%10)/10];
        if (nWindDir < 0)
        {
            m_Rain[i]->m_fSinA = - m_Rain[i]->m_fSinA;
        }
        m_Rain[i]->m_fCosA				= COS[(abs(nWindDir) + rand()%10)/10];
    }
    m_nWindDirection = nWindDir;
}


int CRainSys::RendBMP(DWORD dwColor)
{
    return 1;
}
BOOL CRainSys::Create(int nWindDir = 30 , int nLevel = 200, DWORD dwColor = 0x00ffffff)
{
    if (nLevel > _MAX_AMOUNT )
    {
        nLevel = _MAX_AMOUNT;
    }
    m_iStatu		= _RAIN_BEGIN;
    m_iRainAmount	= 4;
    m_dwPauseTime	= 0;
    m_dwColor		= dwColor;
    m_iTargetAmount = nLevel;
    m_nWindDirection = nWindDir;
    for(int i = 0; i <  _MAX_AMOUNT ; i++)
    {
        CRain* rain = new CRain();
        m_Rain.push_back(rain);
    }
    strcpy(m_szFileName[0], "Ripple");
    strcpy(m_szFileName[1], "RippleWater");
    int nSelfVolume = m_iRainAmount * 4 * 100 / _MAX_AMOUNT;
    if (nSelfVolume > 140)
    {
        nSelfVolume = 140;
    }
    else if (nSelfVolume < 35)
    {
        nSelfVolume = 35;
    }
    DXPlaySound("sound/rain.wav", 0, 0, 0, -1, nSelfVolume);
    return TRUE;
}

//----------------------------------------------------------------
void CRainSys::Reset(void)
{
    m_iStatu = _RAIN_BEGIN;
    m_iRainAmount	= 4;
    for(DWORD i = 0; i < m_Rain.size(); i++)
    {
        m_Rain[i]->Reset();
    }
    int nSelfVolume = m_iRainAmount * 4 * 100 / _MAX_AMOUNT;
    if (nSelfVolume > 140)
    {
        nSelfVolume = 140;
    }
    else if (nSelfVolume < 35)
    {
        nSelfVolume = 35;
    }
    DXSetVolume("sound/rain.wav", nSelfVolume, 0, 0);
}

//--------------------------------------------------------------

void CRainSys::Process(int& nStatus)
{
    if (m_iStatu == _RAIN_NONE)
    {
        return;
    }
    if (m_iStatu == _RAIN_PAUSE)
    {
        return;
    }
    for (int i = 0; i < this->m_iRainAmount ; i++)
    {
        if (this->m_iStatu == _RAIN_BEGIN && m_iRainAmount < m_iTargetAmount)
        {
            int nInternal = m_iRainAmount ;
            if (nInternal > 199)
            {
                nInternal = 199;
            }
            nInternal = 200 - nInternal;
            if (nInternal < 100)
            {
                nInternal = 100;
            }
            if (rand() % nInternal == 0)
            {
                m_iRainAmount	+= 1;
                int nSelfVolume = m_iRainAmount * 3 * 100 / _MAX_AMOUNT;
                if (nSelfVolume > 140)
                {
                    nSelfVolume = 140;
                }
                else if (nSelfVolume < 55)
                {
                    nSelfVolume = 55;
                }
                DXSetVolume("sound/rain.wav", nSelfVolume, 0, 0);
            }
        }
        else if (this->m_iStatu == _RAIN_STOP )
        {
            if (m_iRainAmount > 1)
            {
                int nInternal = m_iRainAmount ;
                if (nInternal > 199)
                {
                    nInternal = 199;
                }
                nInternal = 200 - nInternal;
                if (nInternal < 100)
                {
                    nInternal = 100;
                }
                if (rand() % nInternal == 0)
                {
                    m_iRainAmount -= 1;
                    int nSelfVolume = m_iRainAmount * 3 * 100 / _MAX_AMOUNT;
                    if (nSelfVolume > 140)
                    {
                        nSelfVolume = 140;
                    }
                    else if (nSelfVolume < 55)
                    {
                        nSelfVolume = 55;
                    }
                    DXSetVolume("sound/rain.wav", nSelfVolume, 0, 0);
                }
            }
            else
            {
                Destroy();
                nStatus = 0;
                return ;
            }
        }
        else
        {
            if (this->m_iStatu == _RAIN_RESET)
                if (this->m_iTargetAmount > m_iRainAmount)
                {
                    int nInternal = m_iRainAmount ;
                    if (nInternal > 199)
                    {
                        nInternal = 199;
                    }
                    nInternal = 200 - nInternal;
                    if (nInternal < 100)
                    {
                        nInternal = 100;
                    }
                    if (rand() % nInternal == 0)
                    {
                        m_iRainAmount	+= 1;
                        int nSelfVolume = m_iRainAmount * 3 * 100 / _MAX_AMOUNT;
                        if (nSelfVolume > 140)
                        {
                            nSelfVolume = 140;
                        }
                        else if (nSelfVolume < 55)
                        {
                            nSelfVolume = 55;
                        }
                        DXSetVolume("sound/rain.wav", nSelfVolume, 0, 0);
                    }
                }
                else if (m_iRainAmount > 2 && this->m_iTargetAmount < m_iRainAmount)
                {
                    int nInternal = m_iRainAmount ;
                    if (nInternal > 199)
                    {
                        nInternal = 199;
                    }
                    nInternal = 200 - nInternal;
                    if (nInternal < 100)
                    {
                        nInternal = 100;
                    }
                    if (rand() % nInternal == 0)
                    {
                        m_iRainAmount -= 1;
                        int nSelfVolume = m_iRainAmount * 3 * 100 / _MAX_AMOUNT;
                        if (nSelfVolume > 140)
                        {
                            nSelfVolume = 140;
                        }
                        else if (nSelfVolume < 55)
                        {
                            nSelfVolume = 55;
                        }
                        DXSetVolume("sound/rain.wav", nSelfVolume, 0, 0);
                    }
                }
        }
        if (m_iRainAmount > _MAX_AMOUNT)
        {
            m_iRainAmount = _MAX_AMOUNT;
        }
        if (m_iRainAmount < 0 )
        {
            Destroy();
            nStatus = 0;
            return ;
        }
        if (!m_Rain[i]->IsCreated())
        {
            int iPosX;
            if (this->m_nWindDirection >= 0)
            {
                iPosX	= rand() % (_SCR_WIDTH +  12 * this->m_nWindDirection)  - 12 * this->m_nWindDirection;
            }
            else
            {
                iPosX	= rand() % (_SCR_WIDTH - 12 * this->m_nWindDirection) + 10;
            }
            int iSceneDeepth;
            if (rand() % 2 == 1)
            {
                iSceneDeepth = rand() % 16 + 20;
            }
            else
            {
                iSceneDeepth = rand() % 35 + 1 ;
            }
            int iPosY		= -10;
            m_Rain[i]->Create(iPosX, iPosY, iSceneDeepth, this->m_nWindDirection , 1, this->m_iTargetAmount, m_dwColor);
            m_Rain[i]->SetRipple(m_szFileName);
        }
        m_Rain[i]->Process( );
    }
    return;
}

void CRainSys::RainStop()
{
    //	DXCloseSound("sound/rain.wav",0,0);
    this->m_iStatu = _RAIN_STOP;
}

void CRainSys::Destroy()
{
    DXCloseSound("sound/rain.wav", 0, 0);
    for(DWORD i = 0 ; i < m_Rain.size(); i++)
    {
        SAFE_DELETE(m_Rain[i]);
    }
    m_Rain.clear();
    this->m_iStatu = _RAIN_NONE;
}

//--------------------------------------------------------------








//--------------------------------------------------------------
//沙
//--------------------------------------------------------------

//--------------------------------------------------------------
CSand::CSand(void)
{
    m_iStatus		= _SANDSTATUS_NONE;
    return;
}

//--------------------------------------------------------------
void CSand::Reset(void)
{
    m_iStatus		= _SANDSTATUS_NONE;
}

//--------------------------------------------------------------
int	CSand::GetInt(double dValue)
{
    if ((int)(dValue + 0.5) > (int)dValue)
    {
        return int(dValue) + 1;
    }
    else
    {
        return int(dValue);
    }
}

//--------------------------------------------------------------
BOOL CSand::Create(int iPosX, int iPosY, int iSceneDeepth, int fWind, int fGravity, int nLevel)
{
    if (m_iStatus != _RAINSTATUS_NONE)
    {
        return FALSE;
    }
    m_iStatus			= _SANDSTATUS_CREATE;
    m_iStartX			= iPosX;
    m_iStartY			= iPosY;
    m_iSceneDeepth		= iSceneDeepth;
    m_fWind				= fWind;
    m_dwLifeBeginTime	= TimeGet();
    m_dwElapseTime		= 0;
    m_iSceneDeepth	   %= 36;
    m_iSize				= 4 + 2 * (36 - m_iSceneDeepth) / 36 ;
    m_fSpeed			= 4 + 36 * m_iSceneDeepth / 36;
    m_fGravity			= 1;
    this->m_nLevel = nLevel;
    m_fSinA				= SIN[(abs(m_fWind) + rand()%10)/10];
    if (m_fWind < 0)
    {
        m_fSinA = - m_fSinA;
    }
    m_fCosA				= COS[(abs(m_fWind) + rand()%10)/10];
    m_dwLifeTime		= 2000 + 10 * abs(m_fSinA) / 1000 + rand() % 8 ;
    return TRUE;
}

//--------------------------------------------------------------



void CSand::Show()
{
    if (m_iStatus == _SANDSTATUS_NONE)
    {
        return;
    }
    m_dwElapseTime	= TimeGet() - m_dwLifeBeginTime;
    if (m_dwElapseTime >= m_dwLifeTime)
    {
        m_iStatus			= _SANDSTATUS_NONE;
    }
    if (m_iStatus == _SANDSTATUS_CREATE)
    {
        int dOffset		= (m_fSpeed * m_dwElapseTime * 1000  *  10 / 60 / 1000 ) + (m_fGravity * m_dwElapseTime * 1000 / 60   * m_dwElapseTime * 1000 / 60 * 10 / 1000 / 1000 / 2)  ; // *10
        if (dOffset <= 0 && m_dwElapseTime)
        {
            m_iStatus	= _SANDSTATUS_NONE;
        }
        else
        {
            int iX0	= m_iStartX + dOffset * m_fCosA * m_iSize / 2 * m_nDir / 10 / 1000 + rand() % 50;
            int iY0	= m_iStartY + dOffset * m_fSinA * m_iSize / 20 / 10 / 1000   - rand() % 50;
            int iX1 = iX0 + m_fCosA * m_iSize / 2 / 1000 * m_nDir;
            int iY1 = iY0 - m_fSinA * m_iSize / 2 / 1000 ;
            if (this->m_nLevel / (_MAX_AMOUNT / 3) < 1)
            {
                CMyBitmap::ShowLine(iX0, iY0, iX1, iY1, 0xffffff00);
                DWORD dwColor = ((150 + m_iSceneDeepth * 2) << 24) | (225 << 16) | (225 << 8) | 0;
                CMyBitmap::ShowLine(iX0 + 15 * m_nDir, iY0 + 12, iX1 + 15 * m_nDir, iY1 + 12, dwColor);
                dwColor = ((120 + m_iSceneDeepth * 2) << 24) | (205 << 16) | (205 << 8) | 0;
                CMyBitmap::ShowLine(iX0 + 10 * m_nDir, iY0 + 5, iX1 + 10 * m_nDir, iY1 + 5, dwColor);
                dwColor = ((100 + m_iSceneDeepth * 2) << 24) | (205 << 16) | (205 << 8) | 0;
                CMyBitmap::ShowLine(iX0 + 60 * m_nDir, iY0 + 10, iX0 + 60 * m_nDir, iY1 + 11, dwColor);
            }
            else if (this->m_nLevel / (_MAX_AMOUNT / 3) < 2)
            {
                CMyBitmap::PrepareLine();
                DWORD dwColor = (120 + m_iSceneDeepth * 2) << 24;
                dwColor |= 0x00ffad00;
                CMyBitmap::ShowRect(iX0, iY0, iX0 + 1, iY0 + 1, dwColor );
                int nOff = rand() % 10 * m_nDir;
                dwColor = (150 + m_iSceneDeepth * 2) << 24 | 0x00ffad00;
                CMyBitmap::ShowRect(iX0 + nOff,  iY0 + nOff, iX0 + 1 + nOff, iY0 + 1 + nOff, dwColor );
                CMyBitmap::ShowLine(iX0, iY0, iX1, iY1, 0xffffff00);
                dwColor = ((150 + m_iSceneDeepth * 2) << 24) | (225 << 16) | (225 << 8) | 0;
                CMyBitmap::ShowLine(iX0 + 15 * m_nDir, iY0 + 12, iX1 + 15 * m_nDir, iY1 + 12, dwColor);
                dwColor = ((120 + m_iSceneDeepth * 2) << 24) | (205 << 16) | (205 << 8) | 0;
                CMyBitmap::ShowLine(iX0 + 10 * m_nDir, iY0 + 5, iX1 + 10 * m_nDir, iY1 + 5, dwColor);
                dwColor = ((100 + m_iSceneDeepth * 2) << 24) | (205 << 16) | (205 << 8) | 0;
                CMyBitmap::ShowLine(iX0 + 60 * m_nDir, iY0 + 10, iX0 + 60 * m_nDir, iY1 + 11, dwColor);
            }
            m_iSplashX	= iX0;
            m_iSplashY	= iY0;
        }
    }
    return;
}
//--------------------------------------------------------------
void CSand::Destroy(void)
{
    return;
}

//--------------------------------------------------------------

void CSand::Process()
{
    Show();
    return;
}


//--------------------------------------------------------------



void CSandSys::Pause()
{
    DXSetVolume("sound/sand.wav", 0, 0, 0);
    m_iStatu =  _SAND_PAUSE;
    m_dwPauseTime = TimeGet();
}
void CSandSys::Continue()
{
    if (m_iStatu !=  _SAND_PAUSE)
    {
        return;
    }
    DWORD dwElapseTime = TimeGet() -  m_dwPauseTime;
    DWORD i;
    for(i = 0 ; i < this->m_Sand.size(); i++)
    {
        m_Sand[i]->m_dwLifeBeginTime += dwElapseTime;
    }
    int nSelfVolume = m_nLevel * 4 * 100 / _MAX_AMOUNT;
    if (nSelfVolume > 140)
    {
        nSelfVolume = 140;
    }
    else if (nSelfVolume < 20)
    {
        nSelfVolume = 20;
    }
    DXSetVolume("sound/sand.wav", nSelfVolume, 0, 0);
    m_iStatu = _SAND_BEGIN;
}

void CSandSys::SetSand(int nWindDir, int nLevel)
{
    if (m_nLevel != nLevel)
    {
        m_iStatu = _SAND_RESET;
        m_nLevel = nLevel;
    }
    for(DWORD i = 0; i < m_Sand.size(); i++)
    {
        m_Sand[i]->m_fSinA				= SIN[(abs(nWindDir) + rand()%10)/10];
        if (nWindDir < 0)
        {
            m_Sand[i]->m_fSinA = - m_Sand[i]->m_fSinA;
        }
        m_Sand[i]->m_fCosA				= COS[(abs(nWindDir) + rand()%10)/10];
    }
    m_nWindDirection = nWindDir;
}

BOOL CSandSys::Create(int nWindDir = -30 , int nLevel = 200)
{
    m_dwPauseTime = 0;
    m_iStatu = _SAND_BEGIN;
    m_iSandAmount	= 10;
    if (nLevel > _MAX_AMOUNT)
    {
        nLevel = _MAX_AMOUNT;
    }
    m_nLevel = nLevel;
    m_iTargetAmount = m_nLevel;
    m_nWindDirection = nWindDir;
    for(int i = 0; i <  _MAX_AMOUNT ; i++)
    {
        CSand* sand = new CSand();
        m_Sand.push_back(sand);
    }
    int nSelfVolume = m_iSandAmount  * 100 / _MAX_AMOUNT;
    if (nSelfVolume > 140)
    {
        nSelfVolume = 140;
    }
    else if (nSelfVolume < 20)
    {
        nSelfVolume = 20;
    }
    DXPlaySound("sound/sand.wav", 0, 0, 0, -1, nSelfVolume);
    return TRUE;
}

//--------------------------------------------------------------
void CSandSys::Reset(void)
{
    m_iStatu = _SAND_BEGIN;
    m_iSandAmount	= 10;
    m_iTargetAmount = 10;
    int nSelfVolume = m_iSandAmount * 4 * 100 / _MAX_AMOUNT;
    if (nSelfVolume > 140)
    {
        nSelfVolume = 140;
    }
    else if (nSelfVolume < 20)
    {
        nSelfVolume = 20;
    }
    DXSetVolume("sound/sand.wav", nSelfVolume, 0, 0);
    for(DWORD i = 0; i < m_Sand.size(); i++)
    {
        m_Sand[i]->Reset();
    }
}

//--------------------------------------------------------------


void CSandSys::Process(int& nStatus)
{
    if (m_iStatu == _SAND_NONE)
    {
        return;
    }
    if (m_iStatu == _SAND_PAUSE)
    {
        return;
    }
    for (int i = 0; i < m_iSandAmount ; i++)
    {
        if (m_iStatu == _SAND_BEGIN && m_iSandAmount < m_iTargetAmount)
        {
            int nInternal = m_iSandAmount ;
            if (nInternal > 199)
            {
                nInternal = 199;
            }
            nInternal = 200 - nInternal;
            if (rand() % nInternal == 0)
            {
                m_iSandAmount	+= 1;
                int nSelfVolume = m_iSandAmount * 4 * 100 / _MAX_AMOUNT;
                if (nSelfVolume > 140)
                {
                    nSelfVolume = 140;
                }
                else if (nSelfVolume < 25)
                {
                    nSelfVolume = 25;
                }
                DXSetVolume("sound/rain.wav", nSelfVolume, 0, 0);
            }
        }
        else if (m_iStatu == _SAND_STOP )
        {
            if (m_iSandAmount > 5)
            {
                int nInternal = m_iSandAmount ;
                if (nInternal > 199)
                {
                    nInternal = 199;
                }
                nInternal = 200 - nInternal;
                if (rand() % nInternal == 0)
                {
                    m_iSandAmount	--;
                    int nSelfVolume = m_iSandAmount * 4 * 100 / _MAX_AMOUNT;
                    if (nSelfVolume > 140)
                    {
                        nSelfVolume = 140;
                    }
                    else if (nSelfVolume < 25)
                    {
                        nSelfVolume = 25;
                    }
                    DXSetVolume("sound/rain.wav", nSelfVolume, 0, 0);
                }
            }
            else
            {
                Destroy();
                nStatus = 0;
                return;
            }
        }
        else if (m_iStatu == _SAND_RESET)
            if (m_nLevel > m_iSandAmount)
            {
                int nInternal = m_iSandAmount ;
                if (nInternal > 199)
                {
                    nInternal = 199;
                }
                nInternal = 200 - nInternal;
                if (rand() % nInternal == 0)
                {
                    m_iSandAmount	+= 1;
                    int nSelfVolume = m_iSandAmount * 4 * 100 / _MAX_AMOUNT;
                    if (nSelfVolume > 140)
                    {
                        nSelfVolume = 140;
                    }
                    else if (nSelfVolume < 25)
                    {
                        nSelfVolume = 25;
                    }
                    DXSetVolume("sound/rain.wav", nSelfVolume, 0, 0);
                }
            }
            else if (m_iSandAmount > m_nLevel && m_iSandAmount > 10 )
            {
                int nInternal = m_iSandAmount ;
                if (nInternal > 199)
                {
                    nInternal = 199;
                }
                nInternal = 200 - nInternal;
                if (rand() % nInternal == 0)
                {
                    m_iSandAmount	--;
                    int nSelfVolume = m_iSandAmount * 4 * 100 / _MAX_AMOUNT;
                    if (nSelfVolume > 140)
                    {
                        nSelfVolume = 140;
                    }
                    else if (nSelfVolume < 25)
                    {
                        nSelfVolume = 25;
                    }
                    DXSetVolume("sound/rain.wav", nSelfVolume, 0, 0);
                }
            }
        if (m_iSandAmount > _MAX_AMOUNT)
        {
            m_iSandAmount = _MAX_AMOUNT;
        }
        if (m_iSandAmount < 0)
        {
            m_iSandAmount = 0;
        }
        if (!m_Sand[i]->IsCreated())
        {
            int iPosY, iPosX;
            if (m_nWindDirection >= 0)
            {
                iPosY	= rand() % (_SCR_HEIGHT +  8 * m_nWindDirection)  - 8 * m_nWindDirection;
                iPosX		= -rand() % 50 - 10;
                m_Sand[i]->m_nDir = 1;
            }
            else
            {
                iPosY	= rand() % (_SCR_HEIGHT - 8 * m_nWindDirection) + 10;
                iPosX		= _SCR_WIDTH + rand() % 30;
                m_Sand[i]->m_nDir = -1;
            }
            int iSceneDeepth = rand() % 32 + 4;
            int nDir = 1;
            if (rand() % 2 == 1)
            {
                nDir = -1;
            }
            m_Sand[i]->Create(iPosX, iPosY, iSceneDeepth, m_nWindDirection + rand() % 30 * nDir  , 1, m_nLevel);
        }
        m_Sand[i]->Process( );
    }
    return;
}

void CSandSys::SandStop()
{
    this->m_iStatu = _SAND_STOP;
}

void CSandSys::Destroy()
{
    DXCloseSound("sound/sand.wav", 0, 0);
    for(DWORD i = 0 ; i < m_Sand.size(); i++)
    {
        SAFE_DELETE(m_Sand[i]);
    }
    m_Sand.clear();
    m_iStatu = _SAND_NONE;
}





//-----------------------------------------------------------------------------------
//
//烟花
//-----------------------------------------------------------------------------


//--------------------------------------------------------------
void CYinFa::Reset(void)
{
    m_iStatus		= _YINFASTATUS_NONE;
}

BOOL CYinFa::Create(int iOrigPosX, int iOrigPosY, int nMarginX, int nMarginY, int nRadius  )
{
    if (m_iStatus != _YINFASTATUS_NONE)
    {
        return FALSE;
    }
    m_iStatus				=  _YINFASTATUS_START;
    m_nCenterWorldPoint.x	= iOrigPosX - 80 + rand() % 170;
    m_nCenterWorldPoint.y	= iOrigPosY ;
    m_nMarginWorldPoint.x	= nMarginX;
    m_nMarginWorldPoint.y	= nMarginY;
    m_fRadius				= (float)nRadius;
    m_nPathCount			= rand() % (nRadius / 20 + 1) + 2;
    m_nPathLength			= nRadius / 28 + 2 ;
    m_nStartWoldPoint.x		= iOrigPosX;
    m_nStartWoldPoint.y		= iOrigPosY + 200;
    return TRUE;
}


void CYinFa::Show()
{
    if (m_iStatus == _YINFASTATUS_NONE)
    {
        return;
    }
    if (	m_iStatus == _YINFASTATUS_START)
    {
        if ( m_nStartWoldPoint.y <= this->m_nCenterWorldPoint.y + m_nPathCount * 5)
        {
            m_iStatus = _YINFASTATUS_CREATE;
            m_dwLifeBeginTime = TimeGet();
            m_nPathCount = 0;
        }
        else
        {
            m_nPathCount ++;
            m_nStartWoldPoint.y -= m_nPathCount * 5 - rand() % 5;
            int iY0	=  	m_nStartWoldPoint.y;
            int iX0	=  m_nStartWoldPoint.x + (m_nStartWoldPoint.x - this->m_nCenterWorldPoint.x) * (m_nPathCount * m_nPathCount)  / 600;
            int nScreenX, nScreenY;
            g_objGameMap.World2Screen(iX0, iY0, nScreenX, nScreenY);
            CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileTitle);
            if (pAni)
            {
                pAni->Show(0, nScreenX, nScreenY , 255, _SHOWWAY_ADDITIVE);
            }
        }
    }
    else if (m_iStatus == _YINFASTATUS_CREATE)
    {
        DWORD m_dwElapseTime	= TimeGet() - m_dwLifeBeginTime;
        double dElapseTime	= (double)m_dwElapseTime / 80.0;
        double dOffsetB		= (double)1.0 * dElapseTime * dElapseTime / 2.0;
        int iY0	=(int)( m_nPathCount * (m_nMarginWorldPoint.y - m_nCenterWorldPoint.y ) / m_nPathLength + m_nCenterWorldPoint.y + dOffsetB );
        int iX0	= (int)(m_nPathCount * (m_nMarginWorldPoint.x - m_nCenterWorldPoint.x ) / m_nPathLength + m_nCenterWorldPoint.x) ;
        int nScreenX, nScreenY;
        g_objGameMap.World2Screen(iX0, iY0, nScreenX, nScreenY);
        int nStepPerColor = m_nPathLength / _YINFA_FRAME + 1;
        int nFrame = (int)((m_nPathCount  ) / nStepPerColor * 1.5f);
        if (nFrame >= _YINFA_FRAME)
        {
            nFrame = _YINFA_FRAME - 1;
        }
        if (nFrame < 0)
        {
            nFrame = 0;
        }
        CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileTitle);
        if (pAni)
        {
            pAni->Show(nFrame, nScreenX, nScreenY + (m_nPathLength - m_nPathCount), 255, _SHOWWAY_ADDITIVE);
            pAni->Show(nFrame, nScreenX + 10, nScreenY + 10 + (m_nPathLength - m_nPathCount), 255, _SHOWWAY_ADDITIVE);
        }
        m_nPathCount ++;
        if (m_nPathCount >= m_nPathLength)
        {
            m_iStatus	= _YINFASTATUS_FALL;
            int nRadius;
            nRadius =(int)((m_fRadius  + 1) / 5) ;
            if (nRadius < 15)
            {
                nRadius = 15;
            }
            m_nFallLenght = rand() % nRadius + 10 ;
            m_nFallStep = 0;
            m_nFallWorldPoint.x =  m_nMarginWorldPoint.x + rand() % 10;
            m_nFallWorldPoint.y  =	(LONG)(m_nPathCount * (m_nMarginWorldPoint.y - m_nCenterWorldPoint.y ) / m_nPathLength + m_nCenterWorldPoint.y + dOffsetB);
        }
    }
    else if (m_iStatus == _YINFASTATUS_FALL)
    {
        if ( m_nFallStep >= m_nFallLenght )
        {
            m_iStatus = _YINFASTATUS_END;
            return;
        }
        m_nFallStep++;
        int iX0, iY0, nScreenx, nScreeny;
        iX0 = m_nFallWorldPoint.x ;
        iY0 = m_nFallWorldPoint.y + m_nFallStep * 4 + m_nFallStep * m_nFallStep / 50 ;
        g_objGameMap.World2Screen(iX0, iY0, nScreenx, nScreeny);
        CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileTitle);
        if (pAni)
        {
            int nFrame = pAni->GetFrameAmount() - 1;
            pAni->Show(nFrame, nScreenx, nScreeny, 255 - 255 * m_nFallStep / m_nFallLenght); //,_SHOWWAY_ADDITIVE);
            pAni->Show(nFrame, nScreenx + 10 , nScreeny + 10, 255 - 255 * m_nFallStep / m_nFallLenght); //,_SHOWWAY_ADDITIVE);
        }
    }
    return;
}



//--------------------------------------------------------------
void CYinFa::Destroy(void)
{
    return;
}

//--------------------------------------------------------------

void CYinFa::Process()
{
    Show();
    return;
}



BOOL CYinFaSys::Create(int nScreenX, int nScreenY, int nNum, int nShowNum, int nRadius, int nType, DWORD dwDelayTime)
{
    if ( nRadius > 400)
    {
        nRadius = 400;
    }
    if ( nRadius < 5 )
    {
        nRadius = 5;
    }
    if (nShowNum > 20)
    {
        nShowNum = 20;
    }
    if (nShowNum < 1 )
    {
        nShowNum = 1;
    }
    if (nNum < 1)
    {
        nNum = 1;
    }
    if (nType <= 0)
    {
        nType = 1;
    }
    this->m_nYinFaShowNum   = nShowNum;
    this->m_nYinFaTotal		= nNum;
    this->m_nYinFaType		= nType;
    m_nYinFaUsed			= 0;
    m_fRadius				= nRadius;
    sprintf(m_szFileTitle, "%s%d", "YinFa", nType);
    this->m_iStatu			= _YINFA_BEGIN;
    m_dwCreateTime			= TimeGet() + dwDelayTime;
    for(int i = 0; i < _MAX_AMOUNT * 2; i++)
    {
        CYinFa* YinFa = new CYinFa();
        m_YinFa.push_back(YinFa);
    }
    this->m_nInterval		= 0;
    int nCenterX			= nScreenX;
    int nCenterY			= nScreenY;
    g_objGameMap.Screen2World( nCenterX  , nCenterY   , m_nOrigWorldPointX, m_nOrigWorldPointY );
    m_nCreated = 1;
    m_dwDelayTime			= dwDelayTime;
    m_dwBeginTime			= TimeGet();
    return TRUE;
}

//--------------------------------------------------------------
// FIREWORK
//--------------------------------------------------------------



void CYinFaSys::Process()
{
    if (m_nCreated != 1)
    {
        return;
    }
    else
    {
        if ((TimeGet() - m_dwBeginTime) < m_dwDelayTime)
        {
            return;
        }
    }
    if (m_iStatu == _YINFA_NONE)
    {
        return;
    }
    int nParticleStep = 4;
    if (m_fRadius > 500)
    {
        nParticleStep = 8;
    }
    else if (m_fRadius > 300)
    {
        nParticleStep = 6;
    }

	float x;
	int i;
    if ( m_iStatu !=  _YINFA_STOP)
    {
        if ( !m_YinFa[(unsigned int)(m_fRadius/3 + m_fRadius /2 +m_fRadius*2 /3+1)]->IsCreated())
        {
            
            this->m_nInterval++;
            if (this->m_nInterval > 3)
            {
                float nRx = (float)m_fRadius  ;
                float nRy = (float)nRx * 2 / 3;
                for ( x = -nRx, i = (int)(m_fRadius / 3.0f + m_fRadius / 2 + m_fRadius * 2 / 3) ; x < nRx ; x += (float)nParticleStep)
                {
                    if (i >= _MAX_AMOUNT * 2)
                    {
                        break;
                    }
                    int TargetY = (int)(sqrt( nRy * nRy - x * x / (nRx * nRx)  * (nRy * nRy) ) + m_nOrigWorldPointY);
                    int TargetX = (int)(x + m_nOrigWorldPointX);
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx );//* 1.8
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                    int nOtherSideY = TargetY - m_nOrigWorldPointY;
                    TargetY = m_nOrigWorldPointY - nOtherSideY;
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx );// * 1.8
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                }
                m_nYinFaUsed += this->m_nYinFaShowNum;
                if (m_nYinFaUsed >= this->m_nYinFaTotal)
                {
                    this->YinFaStop();
                }
            }
            else	if (this->m_nInterval > 2)
            {
                float nRx = (float)m_fRadius * 2 / 3;
                float nRy = (float)nRx * 2 / 3;
                float nRx1 = nRx;
                float nRy1 = nRy;
                for (  x = -nRx, i = (int)(m_fRadius / 3 + m_fRadius / 2 ) ; x < nRx ; x += (float)nParticleStep)
                {
                    float x1 = (float)x;
                    int TargetY = (int)(sqrt( nRy1 * nRy1 - x1 * x1 / (nRx1 * nRx1)  * (nRy1 * nRy1) ) + m_nOrigWorldPointY);
                    int TargetX = (int)(x + m_nOrigWorldPointX);
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx1 );//* 1.5
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                    int nOtherSideY = TargetY - m_nOrigWorldPointY;
                    TargetY = m_nOrigWorldPointY - nOtherSideY;
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx1  );//* 1.5
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                }
            }
            else if (this->m_nInterval > 1)
            {
                float nRx = (float)m_fRadius / 2.0f;
                float nRy = (float)nRx * 2 / 3.0f;
                float nRx1 = nRx;
                float nRy1 = nRy;
                for (x = -nRx, i = (int)(m_fRadius / 3) ; x < nRx ; x += (float)nParticleStep)
                {
                    float x1 = x;
                    int TargetY = (int)(sqrt( nRy1 * nRy1 - x1 * x1 / (nRx1 * nRx1)  * (nRy1 * nRy1) ) + m_nOrigWorldPointY);
                    int TargetX = (int)(x1 + m_nOrigWorldPointX);
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx1 );//*1.2
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                    int nOtherSideY = TargetY - m_nOrigWorldPointY;
                    TargetY = m_nOrigWorldPointY - nOtherSideY;
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx1  );//	*1.2
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                }
            }
            else
            {
                DXPlaySound("sound/firework1.wav", 0, 0 );
                float nRx = (float)m_fRadius * 3 / 12.0f;
                float nRy = (float)nRx * 2 / 3.0f;
                float nRx1 = nRx;
                float nRy1 = nRy;
                for ( x = -nRx, i = (int)(m_fRadius / 12) ; x < nRx ; x += (float)nParticleStep)
                {
                    float x1 = x;// * 2;
                    int TargetY =(int)( sqrt( nRy1 * nRy1 - x1 * x1 / (nRx1 * nRx1)  * (nRy1 * nRy1) ) + m_nOrigWorldPointY);
                    int TargetX = (int)(x + m_nOrigWorldPointX);
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY,(int)nRx );
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                    int nOtherSideY = TargetY - m_nOrigWorldPointY;
                    TargetY = m_nOrigWorldPointY - nOtherSideY;
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx );
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                }
                nRx = (float)m_fRadius / 12.0f;
                nRy = (float)nRx * 2 / 3.0f;
                for (x = -nRx, i = 0 ; x < nRx ; x += (float)nParticleStep)
                {
                    int TargetY = (int)(sqrt( nRy * nRy - x * x / (nRx * nRx)  * (nRy * nRy) ) + m_nOrigWorldPointY);
                    int TargetX =(int)( x + m_nOrigWorldPointX);
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx );
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                    int nOtherSideY = TargetY - m_nOrigWorldPointY;
                    TargetY = m_nOrigWorldPointY - nOtherSideY;
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx );
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                }
            }
        }
    }
    else
    {
        if (m_iStatu != _YINFA_NONE)
            if (_YINFASTATUS_END  == m_YinFa[(unsigned int)(m_fRadius  + m_fRadius /2) ]->IsCreated()   && m_nInterval++ > 255 )
            {
                this->Destroy();
            }
    }
    for(DWORD j = 0; j < m_YinFa.size(); j++)
    {
        m_YinFa[j]->Process( );
    }
}



void CYinFaSys::YinFaStop()
{
    this->m_iStatu = _YINFA_STOP;
}

void CYinFaSys::Destroy()
{
    if (m_iStatu == _YINFA_NONE)
    {
        return;
    }
    DWORD i;
    for( i = 0 ; i < m_YinFa.size(); i++)
    {
        SAFE_DELETE(m_YinFa[i]);
    }
    m_YinFa.clear();
    this->m_iStatu		= _YINFA_NONE;
    m_nCreated			= 2;
}


////////////////////////////////////////////////////////////////////
//
//firework 2
//
////////////////////////////////////////////////////////////////////



//--------------------------------------------------------------


//--------------------------------------------------------------
BOOL CYinFa2::Create(int iOrigPosX, int iOrigPosY, int nMarginX, int nMarginY, int nRadius  )
{
    if (m_iStatus != _YINFASTATUS_NONE)
    {
        return FALSE;
    }
    m_iStatus				=  _YINFASTATUS_CREATE;
    m_nCenterWorldPoint.x	= iOrigPosX  ;
    m_nCenterWorldPoint.y	= iOrigPosY ;
    m_nMarginWorldPoint.x	= nMarginX;
    m_nMarginWorldPoint.y	= nMarginY;
    m_fRadius				= (float)(nRadius);
    m_nPathCount			= rand() % 8 + 2;
    m_nPathLength			=  ((nRadius) / 35 + 1) + 2;
    m_dwLifeBeginTime		= TimeGet();
    return TRUE;
}

//--------------------------------------------------------------------------------------


void CYinFa2::Show()
{
    if (m_iStatus == _YINFASTATUS_NONE)
    {
        return;
    }
    if (m_iStatus == _YINFASTATUS_CREATE)
    {
        DWORD m_dwElapseTime	= TimeGet() - m_dwLifeBeginTime;
        double dElapseTime	= (double)m_dwElapseTime / 100.0;
        double dOffsetB		= (double)1.0 * dElapseTime * dElapseTime / 2.0;
        int iY0	=(int)( m_nPathCount * (m_nMarginWorldPoint.y - m_nCenterWorldPoint.y ) / m_nPathLength + m_nCenterWorldPoint.y + dOffsetB) ;
        int iX0	= (int)( m_nPathCount * (m_nMarginWorldPoint.x - m_nCenterWorldPoint.x ) / m_nPathLength + m_nCenterWorldPoint.x) ;
        int nStepPerColor = m_nPathLength / _YINFA_FRAME + 1;
        int nFrame = (m_nPathCount  ) / nStepPerColor ;
        if (nFrame >= _YINFA_FRAME)
        {
            nFrame = _YINFA_FRAME - 1;
        }
        if (nFrame < 0)
        {
            nFrame = 0;
        }
        int nScreenX, nScreenY;
        g_objGameMap.World2Screen(iX0, iY0, nScreenX, nScreenY);
        CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileTitle);
        if (pAni)
        {
            pAni->Show(nFrame, nScreenX, nScreenY + (m_nPathLength - m_nPathCount), 255, _SHOWWAY_ADDITIVE);
            pAni->Show(nFrame, nScreenX + 10, nScreenY + 10 + (m_nPathLength - m_nPathCount), 255, _SHOWWAY_ADDITIVE);
        }
        m_nPathCount ++;
        if (m_nPathCount >= m_nPathLength)
        {
            m_iStatus	= _YINFASTATUS_FALL;
            int nRadius;
            nRadius =(int)( (m_fRadius + 1) / 5);
            if (nRadius < 15)
            {
                nRadius = 15;
            }
            m_nFallLenght = rand() % nRadius + 10;
            m_nFallStep = 0;
            m_nFallWorldPoint.x =  m_nMarginWorldPoint.x + rand() % 10;
            if ( m_nMarginWorldPoint.y > m_nCenterWorldPoint.y)
            {
                m_nFallWorldPoint.y =  m_nMarginWorldPoint.y - rand() % (nRadius / 2);
            }
            else
            {
                m_nFallWorldPoint.y =  m_nMarginWorldPoint.y + rand() % (nRadius / 2);
            }
        }
    }
    else if (m_iStatus == _YINFASTATUS_FALL)
    {
        if (  m_nFallStep >= m_nFallLenght)
        {
            m_iStatus = _YINFASTATUS_END;
            return;
        }
        m_nFallStep++;
        int iX0, iY0, nScreenx, nScreeny;
        iX0 = m_nFallWorldPoint.x ;
        iY0 = m_nFallWorldPoint.y + m_nFallStep * 5 + m_nFallStep * m_nFallStep / 50 ;
        g_objGameMap.World2Screen(iX0, iY0, nScreenx, nScreeny);
        CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileTitle);
        if (pAni)
        {
            int nFrame = pAni->GetFrameAmount() - 1;
            pAni->Show(nFrame, nScreenx, nScreeny, 255 - 255 * m_nFallStep / m_nFallLenght); //,_SHOWWAY_ADDITIVE);
            pAni->Show(nFrame, nScreenx + 10 , nScreeny + 10,  255 - 255 * m_nFallStep / m_nFallLenght); //,_SHOWWAY_ADDITIVE);
        }
    }
    return;
}
//--------------------------------------------------------------
void CYinFa2::Destroy(void)
{
    return;
}

//--------------------------------------------------------------


void CYinFa2::Process()
{
    Show();
    return;
}



BOOL CYinFaSys2::Create(int nScreenX, int nScreenY, int nNum, int nShowNum, int nRadius, int nType, DWORD dwDelayTime)
{
    if ( nRadius > 400)
    {
        nRadius = 400;
    }
    if ( nRadius < 5 )
    {
        nRadius = 5;
    }
    if (nShowNum > 20)
    {
        nShowNum = 20;
    }
    if (nShowNum < 1 )
    {
        nShowNum = 1;
    }
    if (nNum < 1)
    {
        nNum = 1;
    }
    if (nType <= 0)
    {
        nType = 1;
    }
    this->m_nYinFaShowNum   = nShowNum;
    this->m_nYinFaTotal		= nNum;
    this->m_nYinFaType		= nType;
    m_nYinFaUsed			= 0;
    m_fRadius				= nRadius;
    sprintf(m_szFileTitle, "%s%d", "YinFa", nType);
    this->m_iStatu = _YINFA_BEGIN;
    m_dwCreateTime = TimeGet() + dwDelayTime;
    for(int i = 0; i < _MAX_AMOUNT * 2; i++)
    {
        CYinFa2* YinFa = new CYinFa2();
        m_YinFa.push_back(YinFa);
    }
    this->m_nInterval	= 0;
    int nCenterX		= nScreenX;
    int nCenterY		= nScreenY;
    g_objGameMap.Screen2World( nCenterX  , nCenterY   , m_nOrigWorldPointX, m_nOrigWorldPointY );
    m_nCreated			= 1;
    m_dwDelayTime		= dwDelayTime;
    m_dwBeginTime		= TimeGet();
    return TRUE;
}

//--------------------------------------------------------------


void CYinFaSys2::Process()
{
    if (m_nCreated != 1)
    {
        return;
    }
    else
    {
        if ( (TimeGet() - m_dwBeginTime) < m_dwDelayTime)
        {
            return;
        }
    }
    if (m_iStatu == _YINFA_NONE)
    {
        return;
    }
    int nParticleStep = 4;
    if (m_fRadius > 500)
    {
        nParticleStep = 8;
    }
    else if (m_fRadius > 300)
    {
        nParticleStep = 6;
    }
	float x;
	int i;
    if ( m_iStatu !=  _YINFA_STOP)
    {
        if ( !m_YinFa[(unsigned int)(m_fRadius + m_fRadius*2 /3)]->IsCreated())//m_fRadius/3 + m_fRadius /2 +m_fRadius*2 /3
        {
            //int x, i;
            this->m_nInterval++;
            if (this->m_nInterval > 3)
            {
                float nRx = (float)m_fRadius  ;
                float nRy = (float)nRx * 2 / 3;
                for ( x = -nRx, i = (int)(m_fRadius / 3 + m_fRadius / 2 + m_fRadius * 2 / 3) ; x < nRx ; x += (float)nParticleStep)
                {
                    if (i >= _MAX_AMOUNT * 2)
                    {
                        break;
                    }
                    int TargetY = (int)(sqrt( abs(nRy * nRy - x * x / (nRx * nRx)  * (nRy * nRy)) ) + m_nOrigWorldPointY);
                    int TargetX = (int)(x + m_nOrigWorldPointX);
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx    );
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                    int nOtherSideY = TargetY - m_nOrigWorldPointY;
                    TargetY = m_nOrigWorldPointY - nOtherSideY;
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx  );
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                }
                m_nYinFaUsed += this->m_nYinFaShowNum;
                if (m_nYinFaUsed >= this->m_nYinFaTotal)
                {
                    this->YinFaStop();
                }
            }
            else	if (this->m_nInterval > 2)
            {
                float nRx = (float)m_fRadius * 2 / 3;
                float nRx1 = (float)m_fRadius;
                float nRy1 = (float)m_fRadius * 2 / 3.0f;
                for ( x = -nRx, i =(int)( m_fRadius / 3 + m_fRadius / 2) ; x < nRx ; x += (float)nParticleStep)
                {
                    float x1 = x * 3 / 2;
                    int TargetY = (int)(sqrt(  abs(nRy1 * nRy1 - x1 * x1 / (nRx1 * nRx1)  * (nRy1 * nRy1)) ) + m_nOrigWorldPointY);
                    int TargetX =(int)( x + m_nOrigWorldPointX);
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx1  );
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                    int nOtherSideY = TargetY - m_nOrigWorldPointY;
                    TargetY = m_nOrigWorldPointY - nOtherSideY;
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx1   );
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                }
            }
            else if (this->m_nInterval > 1)
            {
                float nRx = (float)m_fRadius / 2.0f;
                float nRx1 = (float)m_fRadius;
                float nRy1 = (float)m_fRadius * 2 / 3.0f;
                for ( x = -nRx, i = (int)(m_fRadius / 3) ; x < nRx ; x += nParticleStep)
                {
                    float x1 = x * 2;
                    int TargetY =(int)( sqrt( abs( nRy1 * nRy1 - x1 * x1 / (nRx1 * nRx1)  * (nRy1 * nRy1) )) + m_nOrigWorldPointY);
                    int TargetX = (int)(x1 + m_nOrigWorldPointX);
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx1 );
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                    int nOtherSideY = TargetY - m_nOrigWorldPointY;
                    TargetY = m_nOrigWorldPointY - nOtherSideY;
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx1 );
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                }
            }
            else
            {
                DXPlaySound("sound/firework1.wav", 0, 0 );
                float nRx = (float)m_fRadius * 3 / 12.0f;
                float nRy = (float)nRx * 2 / 3.0f;
                float nRx1 = (float)m_fRadius / 2;
                float nRy1 = (float)m_fRadius * 2 / 3.0f;
                for ( x = -nRx, i = (int)(m_fRadius / 12) ; x < nRx ; x += (float)nParticleStep)
                {
                    int x1 =(int)( x * 4);
                    int TargetY = (int)(sqrt(  abs(nRy1 * nRy1 - x1 * x1 / (nRx1 * nRx1)  * (nRy1 * nRy1)) ) + m_nOrigWorldPointY);
                    int TargetX = (int)(x + m_nOrigWorldPointX);
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx );
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                    int nOtherSideY = TargetY - m_nOrigWorldPointY;
                    TargetY = m_nOrigWorldPointY - nOtherSideY;
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx );
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                }
                nRx = (float)m_fRadius / 12.0f;
                nRy = (float)nRx * 2 / 3.0f;
                for ( x = -nRx, i = 0 ; x < nRx ; x += (float)nParticleStep)
                {
                    int TargetY = (int)(sqrt(  abs(nRy * nRy - x * x / (nRx * nRx)  * (nRy * nRy) )) + m_nOrigWorldPointY);
                    int TargetX = (int)(x + m_nOrigWorldPointX);
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx );
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                    int nOtherSideY = TargetY - m_nOrigWorldPointY;
                    TargetY = m_nOrigWorldPointY - nOtherSideY;
                    m_YinFa[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, TargetX, TargetY, (int)nRx );
                    m_YinFa[i++]->SetPicture(m_szFileTitle);
                }
            }
        }//end if of is created
    }
    else if (m_iStatu == _YINFA_STOP)
        if ( _YINFASTATUS_END == m_YinFa[(unsigned int)(m_fRadius  + m_fRadius /2 )]->IsCreated()  && m_nInterval++ > 255 )
        {
            this->Destroy();
        }
    for(DWORD j = 0; j < m_YinFa.size(); j++)
    {
        m_YinFa[j]->Process();
    }
}

void CYinFaSys2::YinFaStop()
{
    this->m_iStatu = _YINFA_STOP;
}

void CYinFaSys2::Destroy()
{
    if (m_iStatu == _YINFA_NONE)
    {
        return;
    }
    DWORD i;
    for( i = 0 ; i < m_YinFa.size(); i++)
    {
        SAFE_DELETE(m_YinFa[i]);
    }
    m_YinFa.clear();
    this->m_iStatu = _YINFA_NONE;
    m_nCreated = 2;
}



void CFireWork::Create(int nScreenX, int nScreenY, int nNum,  int nRadius, int nType, DWORD dwDelayTime)
{
    CYinFaSys* obj_firework = new CYinFaSys();
    obj_firework->Create(  nScreenX,  nScreenY, nNum,  1,   nRadius,  nType, dwDelayTime);
    this->dequeFireWork.push_back(obj_firework);
}


void CFireWork::Process()
{
    DWORD i;
    for (i = 0; i < this->dequeFireWork.size(); i++)
    {
        if (this->dequeFireWork[i]->m_nCreated == 2 )
        {
            SAFE_DELETE( this->dequeFireWork[i]);
            this->dequeFireWork.erase(this->dequeFireWork.begin() + i);
        }
        else
        {
            this->dequeFireWork[i]->Process();
        }
    }
}

void CFireWork::Destroy()
{
    DWORD i;
    for(i = 0 ; i < this->dequeFireWork.size() ; i++ )
    {
        SAFE_DELETE( this->dequeFireWork[i]) ;
    }
    this->dequeFireWork.clear();
}




void CFireWork2::Create(int nScreenX, int nScreenY, int nNum,  int nRadius, int nType, DWORD dwDelayTime)
{
    if (nType > 5)
    {
        return;
    }
    CYinFaSys2* obj_firework = new CYinFaSys2();
    obj_firework->Create(  nScreenX,  nScreenY, nNum,  1,   nRadius,  nType, dwDelayTime);
    this->dequeFireWork2.push_back(obj_firework);
}


void CFireWork2::Process()
{
    DWORD i;
    for (i = 0; i < dequeFireWork2.size(); i++)
    {
        if (dequeFireWork2[i]->m_nCreated == 2 )
        {
            SAFE_DELETE( dequeFireWork2[i] );
            dequeFireWork2.erase(dequeFireWork2.begin() + i);
        }
        else
        {
            dequeFireWork2[i]->Process();
        }
    }
}

void CFireWork2::Destroy()
{
    DWORD i;
    for(i = 0 ; i < dequeFireWork2.size() ; i++ )
    {
        SAFE_DELETE( dequeFireWork2[i] );
    }
    dequeFireWork2.clear();
}



void Firework::Create(int nScreenX, int nScreenY, int nNum,  int nRadius, int nType  , int color, DWORD dwDelayTime)
{
    m_nType = nType;
    switch(m_nType)
    {
    case 0:
        firework1.Create(nScreenX, nScreenY, nNum, nRadius, color, dwDelayTime);
        return;
    case 1:
        firework2.Create(nScreenX, nScreenY, nNum, nRadius, color, dwDelayTime);
        return;
    }
}

void Firework::Destroy()
{
    switch(m_nType)
    {
    case 0:
        firework1.Destroy();
        break;
    case 1:
        firework2.Destroy();
        break;
    }
}

void Firework::Process()
{
    switch(m_nType)
    {
    case 0:
        firework1.Process();
        break;
    case 1:
        firework2.Process();
        break;
    }
}




//------------------------------------------------------------
//
//气候
//
//-------------------------------------------------------------

BOOL CWeather::CreateEnv(int nType, int nWindDir, int nLevel, DWORD dwColor )
{
    if (m_nType != 0)
    {
        if (m_nType != nType)
        {
            Destroy();
        }
        else
        {
            SetEnv(nWindDir, nLevel);
            return 1;
        }
    }
    m_nType = nType;
    switch(nType)
    {
    case __RAIN :
        if (nWindDir > 40)
        {
            nWindDir = 40;
        }
        if (nWindDir < -40)
        {
            nWindDir = -40;
        }
        if (nLevel > _MAX_AMOUNT)
        {
            nLevel =  _MAX_AMOUNT;
        }
        if (nLevel < 2)
        {
            nLevel = 20;
        }
        m_objRain.Create(nWindDir, nLevel, dwColor);
        return 1;
    case  __SNOW :
        if (nWindDir > 30)
        {
            nWindDir = 30;
        }
        if (nWindDir < -30)
        {
            nWindDir = -30;
        }
        if (nLevel > _MAX_AMOUNT)
        {
            nLevel =  _MAX_AMOUNT;
        }
        if (nLevel < 1)
        {
            nLevel = 1;
        }
        m_objSnow.Create(nWindDir, nLevel, dwColor);
        return 1;
    case __LEAF_G :
    case __LEAF_R :
    case __LEAF_Y :
    case __LEAF_Y_R :
    case __LEAF_5 :
    case __LEAF_6 :
    case __LEAF_7 :
    case __FLOWER_1 :
    case __FLOWER_2 :
    case __FLOWER_3 :
        if (nWindDir > 45)
        {
            nWindDir = 45;
        }
        if (nWindDir < -45)
        {
            nWindDir = -45;
        }
        if (nLevel > 200)
        {
            nLevel = 200;
        }
        if (nLevel < 5)
        {
            nLevel = 5;
        }
        m_objLeaf.Create(nWindDir, nLevel, nType - 2);
        return 1;
    case __SAND:
        if (nWindDir > 60)
        {
            nWindDir = 60;
        }
        if (nWindDir < -60)
        {
            nWindDir = -60;
        }
        if (nLevel > _MAX_AMOUNT )
        {
            nLevel =  _MAX_AMOUNT ;
        }
        if (nLevel < 100)
        {
            nLevel = 100;
        }
        m_objSand.Create( nWindDir, nLevel);
        return 1;
    case __FLYING_1:
        if (nLevel >= FLYING_MAX_AMOUNT )
        {
            nLevel =  FLYING_MAX_AMOUNT - 1 ;
        }
        if (nLevel < 1)
        {
            return 0;
        }
        m_objFlying.Create(nLevel, FLYINGTYPE, 0);
        return 1;
    case __FLYING_2:
        if (nLevel >= FLYING_MAX_AMOUNT )
        {
            nLevel =  FLYING_MAX_AMOUNT - 1 ;
        }
        if (nLevel < 1)
        {
            return 0;
        }
        m_objFlying.Create(nLevel, FLYINGTYPE + 1, 0);
        return 1;
    case __FLYING_3:
        if (nLevel >= FLYING_MAX_AMOUNT )
        {
            nLevel =  FLYING_MAX_AMOUNT - 1 ;
        }
        if (nLevel < 1)
        {
            return 0;
        }
        m_objFlying.Create(nLevel, FLYINGTYPE + 2, 0);
        return 1;
    case __FLYING_4:
        if (nLevel >= FLYING_MAX_AMOUNT )
        {
            nLevel =  FLYING_MAX_AMOUNT - 1 ;
        }
        if (nLevel < 1)
        {
            return 0;
        }
        m_objFlying.Create(nLevel, FLYINGTYPE + 3, 0);
        return 1;
    case __DANDELION:
        if (nLevel >= FLYING_MAX_AMOUNT )
        {
            nLevel =  FLYING_MAX_AMOUNT - 1 ;
        }
        if (nLevel < 1)
        {
            return 0;
        }
        m_objFlying.Create(nLevel, DANDELIONTYPE, 0);
        return 1;
    case __WORM:
        if (nLevel >= FLYING_MAX_AMOUNT )
        {
            nLevel =  FLYING_MAX_AMOUNT - 1 ;
        }
        if (nLevel < 1)
        {
            return 0;
        }
        m_objFlyBug.Create(nLevel, 0);
        return 1;
    case __CLOUD:
        if (nLevel >= FLYING_MAX_AMOUNT )
        {
            nLevel =  FLYING_MAX_AMOUNT - 1 ;
        }
        if (nLevel < 1)
        {
            return 0;
        }
        m_objCloud.Create(nLevel);
        return 1;
    }
    return 0;
}

void CWeather::SetColor(DWORD dwColor)
{
    switch(m_nType)
    {
    case __RAIN:
        m_objRain.SetColor(dwColor);
        break;
    default:
        break;
    }
}

void CWeather::SetEnv(int nWindDir, int nLevel)
{
    if (nLevel <= 0)
    {
        return;
    }
    switch(m_nType)
    {
    case __RAIN :
        if (nWindDir > 40)
        {
            nWindDir = 40;
        }
        if (nWindDir < -40)
        {
            nWindDir = -40;
        }
        if (nLevel > _MAX_AMOUNT)
        {
            nLevel =  _MAX_AMOUNT;
        }
        if (nLevel < 2)
        {
            nLevel = 2;
        }
        m_objRain.SetRain(nWindDir, nLevel);
        break  ;
    case  __SNOW :
        if (nWindDir > 30)
        {
            nWindDir = 30;
        }
        if (nWindDir < -30)
        {
            nWindDir = -30;
        }
        if (nLevel > _MAX_AMOUNT)
        {
            nLevel =  _MAX_AMOUNT;
        }
        if (nLevel < 2)
        {
            nLevel = 2;
        }
        m_objSnow.SetSnow(nWindDir, nLevel);
        break  ;
    case __LEAF_G :
    case __LEAF_R :
    case __LEAF_Y :
    case __LEAF_Y_R :
    case __LEAF_5 :
    case __LEAF_6 :
    case __LEAF_7 :
    case __FLOWER_1 :
    case __FLOWER_2 :
    case __FLOWER_3 :
        if (nWindDir > 45)
        {
            nWindDir = 45;
        }
        if (nWindDir < -45)
        {
            nWindDir = -45;
        }
        if (nLevel > 200)
        {
            nLevel = 200;
        }
        m_objLeaf.SetLeaf(nWindDir, nLevel);
        break;
    case __SAND:
        if (nWindDir > 60)
        {
            nWindDir = 60;
        }
        if (nWindDir < -60)
        {
            nWindDir = -60;
        }
        if (nLevel > _MAX_AMOUNT)
        {
            nLevel =  _MAX_AMOUNT;
        }
        if (nLevel < 100)
        {
            nLevel = 100;
        }
        m_objSand.SetSand( nWindDir, nLevel);
        break;
    case __FLYING_1:
    case __FLYING_2:
    case __FLYING_3:
    case __FLYING_4:
    case __DANDELION:
        if (nLevel >= FLYING_MAX_AMOUNT)
        {
            nLevel = FLYING_MAX_AMOUNT - 1;
        }
        else if (nLevel < 1)
        {
            return;
        }
        m_objFlying.ReSet(nLevel);
        break;
    case __WORM:
        if (nLevel >= FLYING_MAX_AMOUNT)
        {
            nLevel = FLYING_MAX_AMOUNT - 1;
        }
        else if (nLevel < 1)
        {
            return;
        }
        m_objFlyBug.ReSet(nLevel);
        break;
    case __CLOUD:
        if (nLevel >= FLYING_MAX_AMOUNT)
        {
            nLevel = FLYING_MAX_AMOUNT - 1;
        }
        else if (nLevel < 1)
        {
            return;
        }
        m_objCloud.ReSet(nLevel);
        break;
    }
}

BOOL CWeather::StopEnv()
{
    switch(m_nType)
    {
    case __RAIN :
        m_objRain.RainStop();
        break;
    case  __SNOW :
        m_objSnow.SnowStop();
        break;
    case __LEAF_G :
    case __LEAF_R :
    case __LEAF_Y :
    case __LEAF_Y_R :
    case __LEAF_5 :
    case __LEAF_6 :
    case __LEAF_7 :
    case __FLOWER_1 :
    case __FLOWER_2 :
    case __FLOWER_3 :
        m_objLeaf.LeafStop();
        break;
    case __SAND:
        m_objSand.SandStop();
        break;
    case __FLYING_1:
    case __FLYING_2:
    case __FLYING_3:
    case __FLYING_4:
    case __DANDELION:
        m_objFlying.Stop();
        break;
    case __WORM:
        m_objFlyBug.Stop();
        break;
    case __CLOUD:
        m_objCloud.Stop();
        break;
    }
    //	m_nType = 0;
    return 0;
}


void CWeather::EnvShow()
{
    if (!m_nType)
    {
        return;
    }
    switch(m_nType)
    {
    case __RAIN :
        m_objRain.Process(m_nType);
        break  ;
    case  __SNOW :
        m_objSnow.Process( m_nType);
        break  ;
    case __LEAF_G :
    case __LEAF_R :
    case __LEAF_Y :
    case __LEAF_Y_R :
    case __LEAF_5 :
    case __LEAF_6 :
    case __LEAF_7 :
    case __FLOWER_1 :
    case __FLOWER_2 :
    case __FLOWER_3 :
        m_objLeaf.Process( m_nType);
        break;
    case __SAND:
        m_objSand.Process(m_nType );
        break;
    case __FLYING_1:
    case __FLYING_2:
    case __FLYING_3:
    case __FLYING_4:
    case __DANDELION:
        m_objFlying.Process(m_nType);
        break;
    case __WORM:
        m_objFlyBug.Process(m_nType);
        break;
    case __CLOUD:
        m_objCloud.Process(m_nType);
        break;
    }
}

void CWeather::Pause()
{
    switch(m_nType)
    {
    case __RAIN :
        m_objRain.Pause();
        break  ;
    case  __SNOW :
        m_objSnow.Pause();
        break  ;
    case __LEAF_G :
    case __LEAF_R :
    case __LEAF_Y :
    case __LEAF_Y_R :
    case __LEAF_5 :
    case __LEAF_6 :
    case __LEAF_7 :
    case __FLOWER_1 :
    case __FLOWER_2 :
    case __FLOWER_3 :
        m_objLeaf.Pause();
        break;
    case __SAND:
        m_objSand.Pause();
        break;
    case __FLYING_1:
    case __FLYING_2:
    case __FLYING_3:
    case __FLYING_4:
    case __DANDELION:
        m_objFlying.Pause();
        break;
    case __WORM:
        m_objFlyBug.Pause();
        break;
    case __CLOUD:
        m_objCloud.Pause();
        break;
    default:
        break;
    }
}

void CWeather::Continue()
{
    switch(m_nType)
    {
    case __RAIN :
        m_objRain.Continue();
        break  ;
    case  __SNOW :
        m_objSnow.Continue();
        break  ;
    case __LEAF_G :
    case __LEAF_R :
    case __LEAF_Y :
    case __LEAF_Y_R :
    case __LEAF_5 :
    case __LEAF_6 :
    case __LEAF_7 :
    case __FLOWER_1 :
    case __FLOWER_2 :
    case __FLOWER_3 :
        m_objLeaf.Continue();
        break;
    case __SAND:
        m_objSand.Continue();
        break;
    case __FLYING_1:
    case __FLYING_2:
    case __FLYING_3:
    case __FLYING_4:
    case __DANDELION:
        m_objFlying.Continue();
        break;
    case __WORM:
        m_objFlyBug.Continue();
        break;
    case __CLOUD:
        m_objCloud.Continue();
        break;
    default:
        break;
    }
}


void CWeather::Destroy()
{
    StopEnv();
}

//--------------------------------------------------------------
BOOL CFireworkLetter::Create(int iOrigPosX, int iOrigPosY, int nMarginX, int nMarginY )
{
    if (m_iStatus != _YINFASTATUS_NONE)
    {
        return FALSE;
    }
    m_iStatus			= _FIREWORKLETTER_CREATE;
    m_nCenterWorldPoint.x	= iOrigPosX;
    m_nCenterWorldPoint.y	= iOrigPosY;
    m_nMarginWorldPoint.x	= nMarginX;
    m_nMarginWorldPoint.y	= nMarginY;
    m_nPathCount = rand() % 8;
    m_nPathLength  = 10;
    return TRUE;
}

//--------------------------------------------------------------
void CFireworkLetter::Show()
{
    if (m_iStatus == _FIREWORKLETTER_NONE)
    {
        return;
    }
    if (m_iStatus == _FIREWORKLETTER_CREATE)
    {
        int iY0	= m_nPathCount * (m_nMarginWorldPoint.y - m_nCenterWorldPoint.y ) / m_nPathLength + m_nCenterWorldPoint.y;
        int iX0	= m_nPathCount * (m_nMarginWorldPoint.x - m_nCenterWorldPoint.x ) / m_nPathLength + m_nCenterWorldPoint.x ;
        int nScreenX, nScreenY;
        g_objGameMap.World2Screen(iX0, iY0, nScreenX, nScreenY);
        CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileTitle);
        if (pAni)
        {
            pAni->Show(0, nScreenX, nScreenY  , 255, _SHOWWAY_ADDITIVE);
        }
        m_nPathCount ++;
        if (m_nPathCount >= m_nPathLength)
        {
            m_iStatus	= _FIREWORKLETTER_LETTER;
            m_nFallLenght = 16;
            m_nFallStep = 0;
            m_nPathCount = 0;
            m_nFallWorldPoint.x =  m_nMarginWorldPoint.x ;
            m_nFallWorldPoint.y =  m_nMarginWorldPoint.y ;
        }
    }
    else if (m_iStatus == _FIREWORKLETTER_LETTER)
    {
        if (m_nPathCount++ >= 6)
        {
            m_iStatus = _FIREWORKLETTER_FALL;
        }
        int nFrame = m_nPathCount % _FIREWORKLETTER_FRAME + 2;
        if (nFrame >= _FIREWORKLETTER_FRAME - 2)
        {
            nFrame = _FIREWORKLETTER_FRAME - 2;
        }
        int nScreenX, nScreenY;
        g_objGameMap.World2Screen(m_nFallWorldPoint.x , m_nFallWorldPoint.y , nScreenX, nScreenY);
        CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileTitle);
        if (pAni)
        {
            pAni->Show(nFrame, nScreenX, nScreenY , 255, _SHOWWAY_ADDITIVE);
        }
    }
    else if (m_iStatus == _FIREWORKLETTER_FALL)
    {
        if ( m_nFallStep >= m_nFallLenght)
        {
            m_iStatus = _FIREWORKLETTER_END;
            return;
        }
        m_nFallStep++;
        int iX0, iY0, nScreenx, nScreeny;
        iX0 = m_nFallWorldPoint.x ;
        iY0 = m_nFallWorldPoint.y + m_nFallStep * 10 + m_nFallStep * m_nFallStep / 50 ;
        g_objGameMap.World2Screen(iX0, iY0, nScreenx, nScreeny);
        CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileTitle);
        if (pAni)
        {
            pAni->Show(_FIREWORKLETTER_FRAME - 1, nScreenx, nScreeny, 255 - 250 * m_nFallStep / m_nFallLenght);    //,_SHOWWAY_ADDITIVE);
        }
    }
    return;
}

//--------------------------------------------------------------
void CFireworkLetter::Destroy(void)
{
    return;
}

void CFireworkLetter::Process()
{
    Show();
    return;
}

//--------------------------------------------------------------

BOOL CFireworkLetterSys::Create(int nScreenX, int nScreenY, char* szLetter , int nType, DWORD dwDelayTime)
{
    this->Destroy();
    if (nType < 1)
    {
        nType = 1;
    }
    this->m_nType	= nType;
    int nsl = strlen(szLetter);
    nsl = nsl * 24 * 4 + (nsl - 2) / 2 * 10;
    if (!GetPoint(szLetter, nScreenX   , nScreenY  ))
    {
        return FALSE;
    }
    char szTitle[100];
    sprintf(szTitle, "%s%d", "Letter", nType);
    strcpy(m_szFileTitle, szTitle);
    this->m_iStatu = _FIREWORKLETTER_BEGIN;
    for(DWORD i = 0; i < pPoint.size() ; i++)
    {
        CFireworkLetter* YinFa = new CFireworkLetter();
        m_firework.push_back(YinFa);
    }
    g_objGameMap.Screen2World( nScreenX + nsl / 4 , nScreenY + 48   , m_nOrigWorldPointX, m_nOrigWorldPointY );
    m_nCreated = 1;
    m_dwBeginTime = TimeGet();
    m_dwDelayTime = dwDelayTime;
    return TRUE;
}



void CFireworkLetterSys::Process()
{
    if (m_nCreated != 1)
    {
        return;
    }
    else
    {
        if ((TimeGet() - m_dwBeginTime) < m_dwDelayTime)
        {
            return;
        }
    }
    if (m_iStatu == _FIREWORKLETTER_NONE)
    {
        return;
    }
    if ( m_iStatu !=  _FIREWORKLETTER_STOP)
    {
        DWORD i ;
        for ( i = 0; i < pPoint.size(); i++)
        {
            m_firework[i]->Create( m_nOrigWorldPointX, m_nOrigWorldPointY, pPoint[i]->x, pPoint[i]->y);
            m_firework[i]->SetPicture( m_szFileTitle);
        }
        this-> Stop();
    }
    else if (m_iStatu != _FIREWORKLETTER_NONE)
        if (_FIREWORKLETTER_END == m_firework[pPoint.size()-1]->IsCreated())
        {
            this->Destroy();
        }
    for(DWORD j = 0; j < m_firework.size(); j++)
    {
        m_firework[j]->Process();
    }
}

void CFireworkLetterSys:: Stop()
{
    this->m_iStatu = _FIREWORKLETTER_STOP;
}

void CFireworkLetterSys::Destroy()
{
    if (m_nCreated == 0)
    {
        return;
    }
    if (m_iStatu == _FIREWORKLETTER_NONE)
    {
        return;
    }
    DWORD i;
    for( i = 0 ; i < m_firework.size(); i++)
    {
        SAFE_DELETE( m_firework[i]);
    }
    m_firework.clear();
    for(i = 0; i < pPoint.size(); i++)
    {
        SAFE_DELETE( pPoint[i]);
    }
    pPoint.clear();
    this->m_iStatu = _FIREWORKLETTER_NONE;
    m_nCreated = 2;
}


int CFireworkLetterSys::test(DWORD a, int k)
{
    a <<= (k - 1);
    if (a & 0x80)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}


BOOL  CFireworkLetterSys::GetPoint(char* zw, int X, int Y)
{
    DWORD n = 0;
    int nInterval = 4;
    int nX, nY;
    g_objGameMap.Screen2World(X, Y, nX, nY);
    X = nX;
    Y = nY;
    while(n != strlen(zw))
    {
        int i = 0, j = 0, l = 0;
        int xx = 0;
        int yy = 0;
        FILE* fp;
        int  num = 0;
        if ((zw[n] & 0x80) == 0)
        {
            char* wm;
            wm = (char*)malloc(72);
            if (zw[n] == ' ')
                {X = X + 24 * nInterval / 2 + 10 ; yy = 0; n++;}
            else
            {
                X += -15;
                fp = fopen(HZK24t, "rb++");
                if (fp == NULL)
                {
                    return FALSE;
                }
                num = 155 + zw[n];
                fseek(fp, (72)*num, 0);
                fread(wm, 72, 1, fp);
                fclose(fp);
                for(i = 0; i < 24; i++)
                {
                    for(j = 0; j < 3; j++)
                    {
                        for(l = 1; l <= 8; l++)
                            if (test(wm[i*3+j], l) == 1)
                            {
                                POINT* P = new POINT;
                                P->x = X + xx;
                                P->y = Y + yy;
                                pPoint.push_back(P);
                                yy += nInterval;
                            }
                            else
                            {
                                yy += nInterval;
                            }
                    }
                    X += nInterval ;
                    yy = 0;
                }
                n += 1;
                X += -15;
            }
        }
        else
        {
            BYTE* wm;
            wm = (BYTE*)malloc(72);
            fp = fopen(HZK24c, "rb++");
            if (fp == NULL)
            {
                return FALSE;
            }
            BYTE bChar = zw[n];
            bChar = bChar - 175;
            BYTE bChar1 = zw[n+1];
            bChar1 = bChar1 - 160;
            num = (bChar - 1) * 94  + bChar1 - 1;
            fseek(fp, (72)*num, 0);
            fread(wm, 72, 1, fp);
            fclose(fp);
            for(i = 0; i < 24; i++)
            {
                for(j = 0; j < 3; j++)
                {
                    for(l = 1; l <= 8; l++)
                        if (test(wm[i*3+j], l) == 1)
                        {
                            POINT* P = new POINT;
                            P->x = X + xx;
                            P->y = Y + yy;
                            pPoint.push_back(P);
                            yy += nInterval;
                        }
                        else
                        {
                            yy += nInterval;
                        }
                }
                X += nInterval;
                yy = 0;
            }
            n += 2;
            X += 10;
            yy = 0;
        }
    }
    return TRUE;
}


int CFireworkLetterSys::GetBmpPoint(LPSTR lpfilename, int X, int Y)
{
    HANDLE f;
    f = CreateFile(lpfilename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    int nRemains = GetFileSize(f, 0);
    char* bmpdata = (char*) malloc(nRemains);
    DWORD nReadByte;
    ReadFile(f, bmpdata, nRemains, &nReadByte, NULL);
    if (nReadByte != nRemains)
    {
        MessageBox(GetActiveWindow(), "read bmp error", "error", MB_OK);
    }
    int width, height;
    BITMAPFILEHEADER* bf = (BITMAPFILEHEADER*)bmpdata;
    if (bf->bfType != 19778/*'MB'*/) { return 0; }
    BITMAPINFOHEADER* bi = (BITMAPINFOHEADER*)((DWORD)bmpdata + sizeof(BITMAPFILEHEADER));
    width = bi->biWidth;
    height = bi->biHeight;
    switch (bi->biBitCount)
    {
    case 8:
        {
            static DWORD pal[256];
            int p = (bi->biWidth + 3) & 0xfffffffc;
            BYTE* bits = (BYTE*)((DWORD)bf + bf->bfOffBits );
            int i, j, usrcolor = bi->biClrUsed;
            DWORD* color = (DWORD*)((DWORD)bi + bi->biSize);
            if (usrcolor == 0) { usrcolor = 256; }
            for (i = 0; i < usrcolor; i++)
            {
                pal[i] = color[i];
            }
            for (i = height; i > 0; i--, bits += p)
                for (j = 0; j < width; j++)
                {
                    if (pal[bits[j]] && 0xffffff00)
                    {
                        POINT* P = new POINT;
                        P->x = X + j * 5 ;
                        P->y = Y + i * 5;
                        pPoint.push_back(P);
                    }
                }
        }
        break;
    case 16:
        {
            int p = ((bi->biWidth * 2 + 3) & 0xfffffffc) / 2;
            short* bits = (short*)((DWORD)bf + bf->bfOffBits);
            int i, j;
            for (i = height - 1; i >= 0; i--, bits += p)
                for (j = 0; j < width; j++)
                {
                    if (bits[j] && 0xffffff00)
                    {
                        POINT* P = new POINT;
                        P->x = X + j * 5;
                        P->y = Y + i * 5;
                        pPoint.push_back(P);
                    }
                }
        }
        break;
    case 24:
        {
            int p = (bi->biWidth * 3 + 3) & 0xfffffffc;
            DWORD bits = (DWORD)bf + bf->bfOffBits ;
            int i, j;
            for (i = height - 1; i >= 0; i--, bits += p)
                for (j = 0; j < width; j++)
                {
                    if (*(DWORD*)(bits + j * 3) && 0xffffff00)
                    {
                        POINT* P = new POINT;
                        P->x = X + j * 2;
                        P->y = Y + i * 2;
                        pPoint.push_back(P);
                    }
                }
        }
        break;
    }
    CloseHandle(f);
    return 1;
}


void CFireWorkChar::Create(int nScreenX, int nScreenY, char* szLetter , int nType, DWORD dwDelayTime)
{
    int nsl = strlen(szLetter);
    if (nsl <= 0)
    {
        return ;
    }
    int nFlowerWidth = 300;
    switch(nsl)
    {
    case 1:
    case 2:
        nFlowerWidth = 260;
        break;
    case 3:
    case 4:
        nFlowerWidth = 400;
        break;
    case 5:
    case 6:
    default:
        nFlowerWidth = 400;
        break;
    }
    nsl = nsl * 24 * 4 + (nsl - 2) / 2 * 10;
    CYinFaSys2* pobj_firework = new CYinFaSys2();
    if (pobj_firework)
    {
        pobj_firework->Create(  nScreenX + nsl / 4 ,  nScreenY + 48 - 15 , 1,  1, nFlowerWidth, nType, dwDelayTime);
        this->m_dequeFireWork.push_back(pobj_firework);
    }
    CFireworkLetterSys* pobj_fireworkchar = new CFireworkLetterSys();
    if (pobj_fireworkchar)
    {
        pobj_fireworkchar->Create(nScreenX, nScreenY, szLetter, nType, dwDelayTime);
        this->m_dequeFireworkChar.push_back(pobj_fireworkchar);
    }
}

void CFireWorkChar::Process()
{
    DWORD i;
    for (i = 0; i < m_dequeFireWork.size(); i++)
    {
        if (m_dequeFireWork[i]->m_nCreated == 2 )
        {
            SAFE_DELETE( m_dequeFireWork[i] );
            m_dequeFireWork.erase(m_dequeFireWork.begin() + i);
        }
        else
        {
            m_dequeFireWork[i]->Process( );
        }
    }
    for (i = 0; i < m_dequeFireworkChar.size(); i++)
    {
        if (m_dequeFireworkChar[i]->m_nCreated == 2 )
        {
            SAFE_DELETE( m_dequeFireworkChar[i]) ;
            m_dequeFireworkChar.erase(m_dequeFireworkChar.begin() + i);
        }
        else
        {
            m_dequeFireworkChar[i]->Process( );
        }
    }
}

void CFireWorkChar::Destroy()
{
    DWORD i;
    for(i = 0 ; i < m_dequeFireworkChar.size() ; i++ )
    {
        SAFE_DELETE( m_dequeFireworkChar[i] );
    }
    m_dequeFireworkChar.clear();
    for(i = 0 ; i < m_dequeFireWork.size() ; i++ )
    {
        SAFE_DELETE( m_dequeFireWork[i]) ;
    }
    m_dequeFireWork.clear();
}



//--------------------------------------------------------------
//
// fly flower
//
//------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////////////////////////////////

int CFlyFlower::GetY(int nx)
{
    int nRet;
    switch(m_nCurState / 5)
    {
    case 0:
        nRet = (int) (4E-15 * nx * nx * nx * nx * nx * nx - 7E-12 * nx * nx * nx * nx * nx + 5E-09 * nx * nx * nx * nx - (3E-06 + m_nA / 100) * nx * nx * nx + (0.0014 + m_nB / 100) * nx * nx - 0.3874 * nx + 121.46);
        break;
    case 1:
        nx = nx / 3;
        nRet = (int) ( 4E-14 * nx * nx * nx * nx * nx * nx - 1E-10 * nx * nx * nx * nx * nx + 1E-07 * nx * nx * nx * nx - (7E-05 + m_nA / 100) * nx * nx * nx + (0.016 + m_nB / 100) * nx * nx - 1.5028 * nx + 112.04);
        break;
    case 2:
        nRet = (int) (-3E-15 * nx * nx * nx * nx * nx * nx + 9E-12 * nx * nx * nx * nx * nx - 9E-09 * nx * nx * nx * nx + (5E-06 + m_nA / 100) * nx * nx * nx - (0.0008 + m_nB / 100) * nx * nx - 0.1221 * nx + 124.15);
        break;
    case 3:
        nRet = (int) (-9E-15 * nx * nx * nx * nx * nx * nx + 1E-11 * nx * nx * nx * nx * nx + 1E-09 * nx * nx * nx * nx - (8E-06 + m_nA / 100) * nx * nx * nx + (0.0035 + m_nB / 100) * nx * nx - 0.52 * nx + 103.45);
        break;
    case 4:
        nRet = (int) (-2E-14 * nx * nx * nx * nx * nx * nx + 4E-11 * nx * nx * nx * nx * nx - 4E-08 * nx * nx * nx * nx + (2E-05 + m_nA / 100) * nx * nx * nx - (0.0022 + m_nB / 100) * nx * nx - 0.1433 * nx + 118.39);
        break;
    case 5:
        nRet = (int) (-1E-15 * nx * nx * nx * nx * nx * nx + 4E-12 * nx * nx * nx * nx * nx - 3E-09 * nx * nx * nx * nx + (1E-06 + m_nA / 100) * nx * nx * nx + (1E-04 + m_nB / 100) * nx * nx - 0.0116 * nx + 8.669);
        break;
    case 6:
        nRet = (int) (7E-15 * nx * nx * nx * nx * nx * nx - 2E-11 * nx * nx * nx * nx * nx + 2E-08 * nx * nx * nx * nx - (1E-05 + m_nA / 100) * nx * nx * nx + (0.0031 + m_nB / 100) * nx * nx - 0.479 * nx + 122.14);
        break;
    case 7:
        nx = nx / 2;
        nRet = (int) ( -5E-14 * nx * nx * nx * nx * nx * nx + 1E-10 * nx * nx * nx * nx * nx - 1E-07 * nx * nx * nx * nx + (4E-05 + m_nA / 100) * nx * nx * nx - (0.003 + m_nB / 100) * nx * nx - 0.6211 * nx + 131.81);
        break;
    case 8:
        nRet =(int) ( -9E-16 * nx * nx * nx * nx * nx * nx + 3E-12 * nx * nx * nx * nx * nx - 4E-09 * nx * nx * nx * nx + (2E-06 + m_nA / 100) * nx * nx * nx - (0.0001 + m_nB / 100) * nx * nx - 0.0838 * nx + 11.637);
        break;
    case 9:
        nRet =(int) (3E-15 * nx * nx * nx * nx * nx * nx - 8E-12 * nx * nx * nx * nx * nx + 6E-09 * nx * nx * nx * nx - (2E-06 + m_nA / 100) * nx * nx * nx + (0.0005 + m_nB / 100) * nx * nx - 0.2155 * nx + 125.61);
        break;
    }
    return -nRet;
}


void CFlyFlower::InitialLayer()
{
    m_nA = rand() % 100;
    m_nB = rand() % 100;
    m_nCurState ++;//做为要使用的函式的ID
    if (m_nCurState >= 50)
    {
        m_nCurState = 0;
    }
    m_nCurFrame = 0;
    m_nLayer ++;
    if (m_nLayer >= 4)
    {
        m_nLayer = 0;
        if (rand() % 2 == 1)
        {
            m_nLayer = 1;
        }
    }
    int nx = rand() % 100 - 50;
    int ny = rand() % 710;
    int ntempx, ntempy;
    g_objGameMap.Screen2World(nx, ny, ntempx, ntempy);
    m_nCurWorldPoint.x = ntempx;
    m_nCurWorldPoint.y = ntempy;
    this->m_nCurX = 0;
    this->m_nCurY = 0;
    m_nSpeed = rand() % 3 + 4;
    m_nLoopCount	= 0;
}

BOOL CFlyFlower::Create(int nCurFrame, int nType)
{
    m_nA = rand() % 100;
    m_nB = rand() % 100;
    m_nType			= nType;
    m_nCurFrame		= nCurFrame;
    m_nCurState		= rand() % 2;
    m_nLayer		= 0;
    int nx, ny;
    nx				= rand() % _SCR_WIDTH ;
    ny				= rand() % _SCR_HEIGHT - 20;
    int ntempx, ntempy;
    g_objGameMap.Screen2World(nx, ny, ntempx, ntempy);
    m_nCurWorldPoint.x = ntempx;
    m_nCurWorldPoint.y = ntempy;
    m_nCurX			= 0;
    m_nCurY			= 0;
    m_nSpeed		= rand() % 3 + 4;
    m_nLoopCount	= 0;
    m_nLayer		= rand() % 2;
    m_iStatus		= _FLYFLOWERSTATUS_CREATE;
    return 1;
}

void CFlyFlower::Show()
{
    if (m_iStatus == _FLYFLOWERSTATUS_NONE)
    {
        return;
    }
    if (m_nCurFrame < 0)
    {
        m_nCurFrame ++;
        return;
    }
    if (m_iStatus == _FLYFLOWERSTATUS_CREATE)
    {
        m_nCurFrame ++;
        int ndetalLayer = 1;
        if (m_nLayer == 0)
        {
            ndetalLayer = (int)1.8;
        }
        else if (m_nLayer == 1)
        {
            ndetalLayer =(int) 1.5;
        }
        m_nCurX += m_nSpeed * ndetalLayer ;
        m_nCurY = GetY(m_nCurX) * ndetalLayer;
        m_nShowWorldPoint.x = m_nCurWorldPoint.x + m_nCurX;
        m_nShowWorldPoint.y = m_nCurWorldPoint.y + m_nCurY;
        int nScreenX, nScreenY;
        g_objGameMap.World2Screen( m_nShowWorldPoint.x, m_nShowWorldPoint.y, nScreenX, nScreenY);
        if (nScreenX < -50 || nScreenX > _SCR_WIDTH + 100 || nScreenY < -50 || nScreenY > _SCR_HEIGHT + 100)
        {
            InitialLayer();
            return;
        }
        if (rand() % 10 == 1)
        {
            m_nLoopCount ++;
        }
        if (m_nLoopCount >= FLYFLOWER_PER_LAYER)
        {
            m_nLoopCount = 0;
        }
        int nFrame = this->m_nLayer * FLYFLOWER_PER_LAYER + this->m_nLoopCount;
        CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileTitle);
        if (pAni)
        {
            pAni->Show(nFrame, nScreenX, nScreenY , 255);
        }
    }
    return;
}

void CFlyFlower::Destroy()
{
    return;
}

BOOL CSysFlyFlower::Create( int nNum, int nType, DWORD dwDelayTime)
{
    Destroy();
    m_nType = nType;
    char szTitle[256];
    switch(nType)
    {
    case 1:
        strcpy(szTitle, "dandelion");
        break;
    case 2:
        sprintf(szTitle, "%s%d", "flying", nType - 1);
        break;
    case 3:
    case 4:
    case 5:
        return 0;
    default:
        return 0;
    }
    strcpy(m_szFileTitle, szTitle);
    m_iStatu		= _FLYFLOWERSTATUS_BEGIN;
    m_dwCreateTime	= TimeGet();
    m_dwDelayTime	= dwDelayTime;
    m_iCurNum		= 2 ;//nNum;
    m_iTargetNum	= m_iCurNum;
    int i;
    for(i = 0; i < FLYING_MAX_AMOUNT; i++)
    {
        CFlyFlower* pobjflower = new CFlyFlower();
        pobjflower->Create(-rand() % 10, m_nType);
        pobjflower->SetPicture( m_szFileTitle);
        m_dequeFlyFlower.push_back(pobjflower);
    }
    m_iStatu = _FLYFLOWERSTATUS_CREATE;
    int nSelfVolume = m_iCurNum * 2 ;
    if (nSelfVolume > 120)
    {
        nSelfVolume = 120;
    }
    else if (nSelfVolume < 50)
    {
        nSelfVolume = 50;
    }
    DXPlaySound("sound/sand.wav", 0, 0, 0, -1, nSelfVolume);
    m_nCreated = 1;
    return TRUE;
}
void CSysFlyFlower::Destroy()
{
    if (	m_iStatu == _FLYFLOWERSTATUS_NONE)
    {
        return;
    }
    int i;
    for(i = 0; i < (int)m_dequeFlyFlower.size(); i++)
    {
        SAFE_DELETE( m_dequeFlyFlower[i]);
    }
    m_dequeFlyFlower.clear();
    m_iStatu = _FLYFLOWERSTATUS_NONE;
    m_nCreated = 0;
    DXCloseSound("sound/sand.wav", 0, 0);
}


void CSysFlyFlower::Process(int& nStatus)
{
    if (m_nCreated != 1)
    {
        return;
    }
    else
    {
        if ((TimeGet() - m_dwBeginTime) < m_dwDelayTime)
        {
            return;
        }
    }
    if (m_iStatu == _FIREWORKLETTER_NONE)
    {
        return;
    }
    if (m_iStatu == _FLYFLOWERSTATUS_BEGIN && m_iCurNum	< m_iTargetNum)
    {
        int nInternal = m_iCurNum ;
        if (nInternal > 99)
        {
            nInternal = 99;
        }
        nInternal = 100 - nInternal;
        if (rand() % nInternal == 0)
        {
            m_iCurNum ++;
            int nSelfVolume = m_iCurNum * 2 ;
            if (nSelfVolume > 120)
            {
                nSelfVolume = 120;
            }
            else if (nSelfVolume < 30)
            {
                nSelfVolume = 30;
            }
            DXSetVolume("sound/sand.wav", nSelfVolume, 0, 0);
        }
    }
    if (m_iStatu == FLYFLOWERSTATUS_RESET)
        if (m_iCurNum < m_iTargetNum)
        {
            int nInternal = m_iCurNum ;
            if (nInternal > 99)
            {
                nInternal = 99;
            }
            nInternal = 100 - nInternal;
            if (rand() % nInternal == 0)
            {
                m_iCurNum ++;
                int nSelfVolume = m_iCurNum * 2 ;
                if (nSelfVolume > 120)
                {
                    nSelfVolume = 120;
                }
                else if (nSelfVolume < 30)
                {
                    nSelfVolume = 30;
                }
                DXSetVolume("sound/sand.wav", nSelfVolume, 0, 0);
            }
        }
        else if (m_iCurNum > m_iTargetNum)
        {
            int nInternal = m_iCurNum ;
            if (nInternal > 99)
            {
                nInternal = 99;
            }
            nInternal = 100 - nInternal;
            if (rand() % nInternal == 0)
            {
                m_iCurNum --;
                int nSelfVolume ;
                nSelfVolume = m_iCurNum * 2 ;
                if (nSelfVolume > 120)
                {
                    nSelfVolume = 120;
                }
                else if (nSelfVolume < 30)
                {
                    nSelfVolume = 30;
                }
                DXSetVolume("sound/sand.wav", nSelfVolume, 0, 0);
            }
        }
    if (m_iStatu == _FLYFLOWERSTATUS_STOP)
        if (m_iCurNum >= 1)
        {
            int nInternal = m_iCurNum ;
            if (nInternal > 99)
            {
                nInternal = 99;
            }
            nInternal = 100 - nInternal;
            if (rand() % nInternal == 0)
            {
                m_iCurNum --;
                int nSelfVolume ;
                nSelfVolume = m_iCurNum * 2 ;
                if (nSelfVolume > 120)
                {
                    nSelfVolume = 120;
                }
                else if (nSelfVolume < 30)
                {
                    nSelfVolume = 30;
                }
                DXSetVolume("sound/sand.wav", nSelfVolume, 0, 0);
            }
        }
        else
        {
            //destroy
            Destroy();
            nStatus = 0;
            return;
        }
    int j;
    for( j = 0; j < m_iCurNum; j++)
    {
        m_dequeFlyFlower[j]->Show( );
    }
}


void CSysFlyFlower::Stop()
{
    //	DXCloseSound("sound/sand.wav",0,0);
    m_iStatu = _FLYFLOWERSTATUS_STOP;
}

void CSysFlyFlower::Pause()
{
    m_iStatu = FLYFLOWERSTATUS_PAUSE;
    DXSetVolume("sound/sand.wav", 0, 0, 0);
}
void CSysFlyFlower::Continue()
{
    if (m_iStatu !=  FLYFLOWERSTATUS_PAUSE)
    {
        return;
    }
    m_iStatu	 = _FLYFLOWERSTATUS_CREATE;
    int nSelfVolume = m_iTargetNum * 2 ;
    if (nSelfVolume > 120)
    {
        nSelfVolume = 120;
    }
    else if (nSelfVolume < 30)
    {
        nSelfVolume = 30;
    }
    DXSetVolume("sound/sand.wav", nSelfVolume, 0, 0);
}
void CSysFlyFlower::ReSet(int inum)
{
    m_iStatu		= FLYFLOWERSTATUS_RESET;
    m_iTargetNum	= inum;
}


//======================================================================================



/////////////////////////////////////////////////////////////////////////////////////////////////////////


