
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************



#include "fireBug.h"
#include "3dgamemap.h"
#include "basefunc.h"
#include "gamedataset.h"
#include "../ndsound/ndsound.h"

const int FLYFLOWER_PER_LAYER	=  6;
const int FLYING_MAX_AMOUNT		= 150;
//--------------------------------------------------------------
//
// fly bug
//
//------------------------------------------------------------------
int DirectoryWorm[4][2] = {1, -1,  1, 1,  -1, 1,  -1, -1};
int Param[10] = {1 , 5 , 9 , 12 , 12 , 2 , 4 , 18 , 13 , 9 };
int ParamWorm[10][3] = {0, 80, 2,  1, 65, 3,  3, 80, 3,  3, 40, 2,  2, 30, 1,  3, 40, 2,  2, 70, 3,  3, 40, 2,  3, 45, 3,  0, 25, 3}; //directory ,totalframe, speed

int CFireBug::GetY(int nx)
{
    return Param[this->m_nCurState] * (nx / 2) * (nx / 2) / 100;
}

void CFireBug::Initial()
{
    int iOrigDir = 0;
    m_nDirChanged = 0;
    iOrigDir = ParamWorm[m_nCurState][0];
    m_nCurState ++;
    m_nCurFrame = 0;
    if (m_nCurState >= 10)
    {
        m_nCurState = 0;
    }
    this->m_nCurWorldPoint.x =  m_nShowWorldPoint.x;
    this->m_nCurWorldPoint.y =  m_nShowWorldPoint.y;
    this->m_nCurX = 0;
    this->m_nCurY = 0;
    m_nSpeed = ParamWorm[m_nCurState][2];
    int iNewDir;
    iNewDir = ParamWorm[m_nCurState][0];
    if (abs(iNewDir - iOrigDir) > 2)
    {
        m_nDirChanged = 1;
    }
    m_nLoopCount	= 0;
}

void CFireBug::InitialLayer()
{
    m_nDirChanged = 0;
    m_nCurState ++;
    m_nCurFrame = 0;
    if (m_nCurState >= 10)
    {
        m_nCurState = 0;
    }
    m_nLayer ++;
    if (m_nLayer >= 4)
    {
        m_nLayer = rand() % 2;
    }
    int nx, ny;
    if (DirectoryWorm[ParamWorm[m_nCurState][0]][0] == 1)
    {
        nx = -8 ;
    }
    else
    {
        nx = 745;
    }
    ny = rand() % (660  - 30 * m_nLayer)  ;
    int ntempx, ntempy;
    g_objGameMap.Screen2World(nx, ny, ntempx, ntempy);
    m_nCurWorldPoint.x = ntempx;
    m_nCurWorldPoint.y = ntempy;
    this->m_nCurX = 0;
    this->m_nCurY = 0;
    m_nSpeed = ParamWorm[m_nCurState][2] * 2;
    m_nLoopCount	= 0;
}

BOOL CFireBug::Create(int nCurFrame)
{
    m_nDirChanged	= 0;
    m_nCurFrame		= nCurFrame;
    m_nCurState		= 0;
    m_nLayer		= 0;
    int nx, ny;
    nx				= rand() % 700 - 60;
    ny				= rand() % 610 - 80;
    int ntempx, ntempy;
    g_objGameMap.Screen2World(nx, ny, ntempx, ntempy);
    m_nCurWorldPoint.x = ntempx;
    m_nCurWorldPoint.y = ntempy;
    m_nCurX = 0;
    m_nCurY = 0;
    m_nSpeed = ParamWorm[m_nCurState][2];
    m_nLoopCount	= 0;
    if (rand() % 2 == 1)
    {
        m_nLayer = 0;
    }
    else
    {
        m_nLayer = 1;
    }
    m_iStatus = _FIREBUGSTATUS_CREATE;
    return 1;
}



void CFireBug::Show( void* pInfo)
{
    if (m_iStatus == _FIREBUGSTATUS_NONE)
    {
        return;
    }
    if (m_nCurFrame < 0)
    {
        m_nCurFrame ++;
        return;
    }
    if (m_iStatus == _FIREBUGSTATUS_CREATE)
    {
        m_nCurFrame ++;
        int nCurFrame;
        nCurFrame = ParamWorm[this->m_nCurState][1];
        if (m_nCurFrame >= nCurFrame)
        {
            Initial();
        }
        int ndetalLayer = 1;
        if (m_nLayer == 0)
        {
            ndetalLayer =(int) 1.5;
        }
        int iYDetal = 0;
        if (m_nDirChanged)
        {
            iYDetal = 3;
        }
        m_nCurX += ParamWorm[m_nCurState][2] * DirectoryWorm[ParamWorm[m_nCurState][0]][0] * ndetalLayer ;
        m_nCurY = GetY(m_nCurX + iYDetal) * DirectoryWorm[ParamWorm[m_nCurState][0]][1] / 10 * ndetalLayer;
        if (m_nDirChanged)
        {
            m_nCurY += DirectoryWorm[ParamWorm[m_nCurState][0]][1] * 2;
            m_nCurY = m_nCurY * 10 / 8;
        }
        m_nShowWorldPoint.x = m_nCurWorldPoint.x + m_nCurX;
        m_nShowWorldPoint.y = m_nCurWorldPoint.y + m_nCurY;
        int nScreenX, nScreenY;
        g_objGameMap.World2Screen( m_nShowWorldPoint.x, m_nShowWorldPoint.y, nScreenX, nScreenY);
        int iLeftLimit = -50;
        int iRightLimit = _SCR_WIDTH + 100;
        int iTopLimit = -50;
        int iBottomLimit = _SCR_HEIGHT + 100;
        if (nScreenX < iLeftLimit || nScreenX > iRightLimit || nScreenY < iTopLimit || nScreenY > iBottomLimit)
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
        CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", "firebug");
        if (pAni)
        {
            pAni->Show(nFrame, nScreenX, nScreenY , 255);
        }
    }
    return;
}


void CFireBug::Destroy()
{
    return;
}

//===================================================================================
BOOL CSysFireBug::Create( int nNum, DWORD dwDelayTime)
{
    Destroy();
    m_iStatu		= _FIREBUGSTATUS_BEGIN;
    m_dwCreateTime	= TimeGet();
    m_dwDelayTime	= dwDelayTime;
    if (nNum > FLYING_MAX_AMOUNT)
    {
        nNum = FLYING_MAX_AMOUNT;
    }
    m_iCurNum		= 4;//nNum;
    m_iTargetNum	= nNum;
    int i;
    for(i = 0; i < FLYING_MAX_AMOUNT; i++)
    {
        CFireBug* pobjflower = new CFireBug();
        pobjflower->Create(-rand() % 10);
        m_dequeFlyBug.push_back(pobjflower);
    }
    m_iStatu = _FIREBUGSTATUS_CREATE;
    m_nCreated = 1;
    int nSelfVolume = m_iCurNum * 2 * 100 / FLYING_MAX_AMOUNT;
    if (nSelfVolume > 100)
    {
        nSelfVolume = 100;
    }
    else if (nSelfVolume < 30)
    {
        nSelfVolume = 30;
    }
    DXPlaySound("sound/worm.wav", 0, 0, 0, -1, nSelfVolume);
    return TRUE;
}
void CSysFireBug::Destroy()
{
    if (	m_iStatu == _FIREBUGSTATUS_NONE)
    {
        return;
    }
    DXCloseSound("sound/worm.wav");
    int i;
    for(i = 0; i < (int)m_dequeFlyBug.size(); i++)
    {
        SAFE_DELETE( m_dequeFlyBug[i]);
    }
    m_dequeFlyBug.clear();
    m_iStatu = _FIREBUGSTATUS_NONE;
    m_nCreated = 0;
}


void CSysFireBug::Process( int& nStatus)
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
    if (m_iStatu == _FIREBUGSTATUS_NONE)
    {
        return;
    }
    if (m_iStatu == _FIREBUGSTATUS_BEGIN && m_iCurNum	< m_iTargetNum)
    {
        int nInternal = m_iCurNum ;
        if (nInternal > 99)
        {
            nInternal = 99;
        }
        nInternal = 100 - nInternal;
        if (rand() % nInternal == 1)
        {
            m_iCurNum ++;
            int nSelfVolume = m_iCurNum * 2 * 100 / FLYING_MAX_AMOUNT;
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
    if (m_iStatu == FIREBUGSTATUS_RESET)
    {
        if (m_iCurNum < m_iTargetNum)
        {
            m_iCurNum ++;
        }
        else if (m_iCurNum > m_iTargetNum)
        {
            m_iCurNum --;
        }
    }
    if (m_iStatu == _FIREBUGSTATUS_STOP)
    {
        if (m_iCurNum >= 1)
        {
            int nInternal = m_iCurNum ;
            if (nInternal > 99)
            {
                nInternal = 99;
            }
            nInternal = 100 - nInternal;
            if (rand() % nInternal == 1)
            {
                m_iCurNum --;
                int nSelfVolume = m_iCurNum * 2 * 100 / FLYING_MAX_AMOUNT;
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
            Destroy();
        }
    }
    int j;
    for( j = 0; j < m_iCurNum; j++)
    {
        m_dequeFlyBug[j]->Show(NULL);
    }
}


void CSysFireBug::Stop()
{
    //	DXCloseSound("sound/worm.wav");
    m_iStatu = _FIREBUGSTATUS_STOP;
}

void CSysFireBug::Pause()
{
    m_iStatu = FIREBUGSTATUS_PAUSE;
    DXSetVolume("sound/worm.wav", 0, 0, 0);
}
void CSysFireBug::Continue()
{
    if (m_iStatu !=  FIREBUGSTATUS_PAUSE)
    {
        return;
    }
    m_iStatu	 = _FIREBUGSTATUS_CREATE;
    int nSelfVolume = m_iTargetNum * 2 * 100 / FLYING_MAX_AMOUNT;
    if (nSelfVolume > 100)
    {
        nSelfVolume = 100;
    }
    else if (nSelfVolume < 30)
    {
        nSelfVolume = 30;
    }
    DXSetVolume("sound/worm.wav", nSelfVolume, 0, 0);
}
void CSysFireBug::ReSet(int inum)
{
    m_iStatu		= FIREBUGSTATUS_RESET;
    m_iTargetNum	= inum;
    int nSelfVolume = m_iTargetNum * 2 * 100 / FLYING_MAX_AMOUNT;
    if (nSelfVolume > 100)
    {
        nSelfVolume = 100;
    }
    else if (nSelfVolume < 30)
    {
        nSelfVolume = 30;
    }
    DXSetVolume("sound/worm.wav", nSelfVolume, 0, 0);
}
