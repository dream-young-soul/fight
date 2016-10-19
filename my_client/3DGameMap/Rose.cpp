
//**********************************************************
// 代码编辑器
//**********************************************************

#include "3dgamemap.h"
#include "rose.h"
#include <math.h>
#include "skylayer.h"
#include "skyrose.h"
#include "gamedataset.h"

//#include "ndsound.h"


BOOL CRose::Create(int iWorldX, int iWorldY, int iTargetX, int iTargetY, int iStep)
{
    m_iStatus				= ROSE_CREATE;
    m_nOrigWorldPoint.x		= iTargetX;
    m_nOrigWorldPoint.y		= iTargetY;
    m_nShowWorldPoint.x		= iTargetX;
    m_nShowWorldPoint.y		= iTargetY;
    m_nTargetPoint.x		= iTargetX;
    m_nTargetPoint.y		= iTargetY;
    m_nRiseCount			= 0;
    m_nLoopFrame			= 0;
    m_nCurX					= rand() % 10;
    m_nCurY					= 0;
    m_nStayCount			= iStep * 50;
    m_iYMoved				= (16 - iStep) * 3;
    m_nPathLength			= iStep;
    m_bAniIndex				= 0;
    m_iAlpha				= 255;
    m_dwFrameTime			= 0;
    m_nLastFrame			= -1;
    return 1;
}

void CRose::Destroy()
{
    m_iStatus = ROSE_NONE;
}

void  CRose::Show()
{
    if (m_bAniIndex > 8)
    {
        return;
    }
    int nScreenX, nScreenY;
    g_objGameMap.World2Screen( m_nShowWorldPoint.x , m_nShowWorldPoint.y, nScreenX, nScreenY);
    int iAniIndex = m_bAniIndex;
    if (iAniIndex == 0)
        if (m_nCurX == 0)
        {
            CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileName[2]);
            if (pAni)
            {
                pAni->Show(m_nLoopFrame, nScreenX + 80, nScreenY + 20, m_iAlpha);
            }
        }
        else if (m_nCurX == 1)
        {
            CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileName[3]);
            if (pAni)
            {
                pAni->Show(m_nLoopFrame, nScreenX + 40, nScreenY + 50, m_iAlpha);
            }
        }
    CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileName[iAniIndex]);
    if (pAni)
        if (pAni->Show(m_nLoopFrame, nScreenX, nScreenY, m_iAlpha))
        {
            if (iAniIndex == 1  )
            {
                m_nLoopFrame = 0;
            }
        }
        else
        {
            DWORD  dwNow = TimeGet();
            DWORD nInterval = 120;
            if (iAniIndex == 0)
            {
                nInterval = 70;
            }
            if ((dwNow - m_dwFrameTime) > nInterval )
            {
                m_dwFrameTime = dwNow;
                m_nLoopFrame++;
            }
        }
}

void CRose::Process()
{
    if (m_iStatus == ROSE_NONE)
    {
        return;
    }
    if (m_iStatus == ROSE_CREATE)
    {
        m_nStayCount -= 10;
        if (m_nStayCount <= 0)
        {
            if (m_iYMoved -- < 0)
            {
                m_iStatus = ROSE_DISPERSE;
                m_nStayCount = (8 - m_iYMoved) * 12; //控制ＳＨＯＷ的时间
                m_nPathCount = 1;
            }
            m_iAlpha = 255  ;
            m_bAniIndex = 0 ;
            Show();
        }
        else
        {
            m_bAniIndex = 10 ;
        }
    }
    if (m_iStatus == ROSE_DISPERSE)
    {
        m_nStayCount --;
        if (m_nStayCount <= 0)
        {
            if (rand() % 2 == 1)
            {
                m_iStatus = ROSE_DESTROY;
            }
            else
            {
                m_iStatus = ROSE_RISE;
                m_nRiseCount = 0;
                m_nRiseStep =  rand() % 12 + 4;
                m_iYMoved = 0;
                m_iCircleDir = rand() % 12 - 6 ;
                CSkyRose* rose = new CSkyRose;
                rose->Create(this->m_szAniTitle, m_nShowWorldPoint.x , m_nShowWorldPoint.y, m_iCircleDir, m_nRiseStep);
                CSkyLayer* p = g_objGameMap.GetSkyLayer();
                p->AddObj(rose);
                m_iStatus = ROSE_DESTROY;
            }
        }
        m_iAlpha	=  255 ;
        m_bAniIndex =  0 ;
        Show();
    }
    if (m_iStatus == ROSE_RISE)
    {
        m_iYMoved += m_nRiseStep;
        m_nShowWorldPoint.y = m_nOrigWorldPoint.y - m_iYMoved;
        m_nCurX	+= m_iCircleDir;
        m_nShowWorldPoint.x = m_nOrigWorldPoint.x + m_nCurX;
        if (m_iYMoved > 600)
        {
            m_iStatus    = ROSE_FALL;
            m_nRiseCount = rand() % 250;
            m_nCurX	 	 = 0;
            m_nOrigWorldPoint.x = m_nShowWorldPoint.x ;
            m_nOrigWorldPoint.y = m_nShowWorldPoint.y ;
        }
        m_iAlpha = 255;
        m_bAniIndex = 1;
        //	Show();
    }
    if (m_iStatus == ROSE_FALL)
    {
        if (m_nCurX ++ > m_nRiseCount)
        {
            m_iStatus = ROSE_DESTROY;
            m_bAniIndex = 10;
        }
        else
        {
            m_nShowWorldPoint.y = m_nOrigWorldPoint.y + m_nCurX * 3;
            m_iAlpha = 255 - m_nCurX  ;
            m_bAniIndex = 1;
            //	Show();
        }
    }
}

//============================================================================================
//
//   RoseSys
//
//=============================================================================================

BOOL Rose::Create(int iWorldX, int iWorldY, int nNum, int nType)
{
    Destroy();
    int i;
    if (nNum > 300)
    {
        nNum = 300;
    }
    m_nNum = nNum;
    if (nType <= 3)
    {
        for(i = 0; i < MAX_ROSEANI; i++)
        {
            char szTitle[100];
            if (i < 2)
            {
                sprintf(szTitle, "rose%d_ani%d" , nType, i);
            }
            if (i == 2)
            {
                strcpy(szTitle, "paint");
            }
            if (i == 3)
            {
                strcpy(szTitle, "grass");
            }
            strcpy(m_szFileName[i], szTitle);
        }
        strcpy(m_szFileName[MAX_ROSEANI] , "");
        strcpy(m_szFileName[MAX_ROSEANI + 1] , "");
        strcpy(m_szFileName[MAX_ROSEANI + 2] , "");
        strcpy(m_szFileName[MAX_ROSEANI + 3] , "");
    }
    else
    {
        int icolor = 1;
        for(i = 0; i < 3; i++)
        {
            sprintf(m_szFileName[i], "rose%d_ani%d" , icolor++, 0);
        }
        icolor = 1;
        for(i = 3; i < 6 ; i++) //载入飘的动画
        {
            sprintf(m_szFileName[i], "rose%d_ani%d" , icolor++, 1);
        }
        strcpy(m_szFileName[6], "paint");
        strcpy(m_szFileName[7], "grass");
    }
    m_nStatus = ROSE_CREATE;
    m_nNum = 16;
    for(i = 1; i <= m_nNum; i++)
    {
        int nRx = i * 30 ;
        int nRy = nRx * 3 / 4;
        for(int j = -nRx; j < nRx; j += 6)
        {
            int x = j;
            int nOrigWorldPointX = iWorldX ;
            int nOrigWorldPointY = iWorldY ;
            int nTargetPointX, nTargetPointY;
            CRose*   pObjRose	= new CRose;
            if (nType <= 3)
            {
                pObjRose->SetPicture(m_szFileName, 0, 1, 2, 3);
            }
            else
            {
                int nflower = rand() % 3;
                pObjRose->SetPicture(m_szFileName, nflower, nflower + 3, 6, 7);
            }
            int Y	= (int)(sqrt(float(nRy * nRy - x * x * 1000  / (nRx * nRx) * (nRy * nRy)  / 1000 )));
            nTargetPointX = nOrigWorldPointX + x + rand() % 25;
            if (rand() % 2 == 1)
            {
                nTargetPointY = nOrigWorldPointY + Y + rand() % 30  - 60 ;
            }
            else
            {
                nTargetPointY = nOrigWorldPointY - Y + rand() % 30 + 60;
            }
            pObjRose->Create (nOrigWorldPointX , nOrigWorldPointY, nTargetPointX, nTargetPointY, i);
            m_dequeRose.push_back(pObjRose);
        }
    }
    return 1;
}


void Rose::Destroy()
{
    if (m_nStatus == ROSE_NONE || m_nStatus == ROSE_DESTROY)
    {
        return;
    }
    int i;
    for(i = 0 ; i <(int) m_dequeRose.size(); i++)
    {
        SAFE_DELETE(m_dequeRose[i]);
    }
    m_dequeRose.clear();
    m_nStatus = ROSE_DESTROY;
}


void Rose::Process()
{
    int i;
    int iFlag = 1;
    for(i = 0; i <(int) m_dequeRose.size(); i++)
    {
        m_dequeRose[i]->Process();
        if (m_dequeRose[i]->m_iStatus != ROSE_DESTROY)
        {
            iFlag = 0;
        }
    }
    if (iFlag)
    {
        Destroy();
    }
}

//==========================================================================
//
//   ROSE INTERFACE
//==========================================================================

BOOL CRoseSys::Create(int iWorldX, int iWorldY, int nType)
{
    if (m_DequeRose.size() > 1)
    {
        return 0;
    }
    CMyPos ViewPos;
    g_objGameMap.GetViewPos(ViewPos);
    if ( abs(  ViewPos.x + 600 - iWorldX) < 2000 && abs( ViewPos.y + 300 - iWorldY) < 1200)
    {
        Rose* pObjRose = new Rose;
        if (pObjRose)
        {
            pObjRose->Create(iWorldX, iWorldY, 1, nType);
            m_DequeRose.push_back(pObjRose);
            //	DSoundPlaySound("sound\\marry.wav",DSUTIL_LOOP_PLAY|DSUTIL_MULTI_PLAY);
            return 1;
        }
    }
    return 0;
}

void CRoseSys::Destroy()
{
    int i;
    for(i = 0; i < (int)m_DequeRose.size(); i++)
    {
        SAFE_DELETE(m_DequeRose[i]);
        //	DSoundCloseSound("sound\\marry.wav");
    }
    m_DequeRose.clear();
}


void CRoseSys::Show()
{
    int i, iDestroyFlag = 1;
    for(i = 0; i < (int)m_DequeRose.size(); i++)
    {
        m_DequeRose[i]->Process();
        if (m_DequeRose[i]->m_nStatus != ROSE_DESTROY)
        {
            iDestroyFlag = 0;
        }
    }
    if (iDestroyFlag)
    {
        for(i = 0; i < (int)m_DequeRose.size(); i++)
        {
            SAFE_DELETE( m_DequeRose[i]);
        }
        m_DequeRose.clear();
    }
}


