
//**********************************************************
// 代码编辑器
//**********************************************************

#include "nd_bitmap.h"
#include "HellFire.h"
#include "3dGameMap.h"
#include "basefunc.h"
#include <math.h>
#include "gamedataset.h"
#include "interactivelayer.h"
extern CGameMap		g_objGameMap;

int Directory[4][2] = {1, -1,  1, 1,  -1, 1,  -1, -1};
int ParamD1[10][3] = {0, 20, 3,  1, 30, 5,  3, 20, 4,  2, 30, 5,  1, 30, 2,  0, 20, 10,  3, 10, 5,  2, 30, 6,  0, 45, 4,  1, 25, 6}; //directory ,totalframe, speed
float ParamA1[10] = {0.31f , 0.35f , 0.116f , 0.115f , 0.112f , 0.02f , 0.04f , 0.18f , 0.13f , 0.49f };



BOOL  CHellFire::Create( int iWorldX, int iWorldY)
{
    m_iStatus				= HELLFIRE_FREE;
    m_dwTimeCount			= 0;
    m_iAttackRange			= 600;
    m_iAttachMan			= 0;
    m_iWorldX				= iWorldX;
    m_iWorldY				= iWorldY;
    m_nCurFrame				= 0;
    m_nCurState				= 0;
    m_nCurWorldPoint.x		= m_iWorldX;
    m_nCurWorldPoint.y		= m_iWorldY;
    m_nOrigWorldPoint.x		= m_iWorldX;
    m_nOrigWorldPoint.y		= m_iWorldY;
    m_nCurX					= 0;
    m_nCurY					= 0;
    m_nLoopCount			= 0;
    m_nSpeed				= ParamD1[m_nCurState][2];
    m_iRadius				= HELLFIRE_CIRCLE_RADIUS;
    m_iBmpWidth				= 20;
    m_iBmpHeight			= 20;
    CMySize nsize;// nW,nH;
    g_objGameMap.GetBgSize(nsize);
    UINT uX, uY;
    g_objGameMap.GetBgWorldPos(uX, uY);
    m_nBoundTop.x			= uX;
    m_nBoundTop.y			= uY;
    m_nBoundBottom.x		= m_nBoundTop.x + nsize.iWidth;
    m_nBoundBottom.y		= m_nBoundTop.y + nsize.iHeight;
    return 1;
}


void CHellFire::Destroy()
{
    m_iStatus =  HELLFIRE_NONE;
    return;
}

void CHellFire::Initial()
{
    m_nCurState ++;
    m_nCurFrame			= 0;
    if (m_nCurState		>= 10)
    {
        m_nCurState		= 0;
    }
    m_nCurWorldPoint.x	=  m_nShowWorldPoint.x;
    m_nCurWorldPoint.y	=  m_nShowWorldPoint.y;
    m_nCurX				= 0;
    m_nCurY				= 0;
    m_nSpeed			= ParamD1[m_nCurState][2];
}

void CHellFire::Show( void* pInfo)
{
    int nScreenX, nScreenY;
    g_objGameMap.World2Screen( m_nShowWorldPoint.x, m_nShowWorldPoint.y, nScreenX, nScreenY);
    m_nLoopCount ++;
    if (m_nLoopCount >= HELLFIRE_FRAME)
    {
        m_nLoopCount = 0;
    }
    CAni* pAni =	g_objGameDataSet.GetDataAni("ani/weather.ani", m_szAniTitle , EXIGENCE_IMMEDIATE, 30 * 1000);
    pAni->Show(m_nLoopCount, nScreenX - m_iBmpWidth / 2  , nScreenY - m_iBmpHeight - 80 , 255);
}

int CHellFire::GetY(int nx)
{
    return  (int)(ParamA1[this->m_nCurState] * (nx / 2) * (nx / 2)); //			//由抛物线函数消减X的值得到Y点
}

void CHellFire::GetPos(CMyPos& posCell)
{
    if (m_iStatus == HELLFIRE_CIRCLE && m_iCircleDir > 0)
    {
        g_objGameMap.World2Cell(m_nShowWorldPoint.x, m_nShowWorldPoint.y - 150, posCell.x, posCell.y);
    }
    else
    {
        g_objGameMap.World2Cell(m_nShowWorldPoint.x, m_nShowWorldPoint.y, posCell.x, posCell.y);
    }
}

void CHellFire::Process(void* pInfo)
{
    CMyPos* pos = (CMyPos*)pInfo;
    m_iWorldX = pos->x;
    m_iWorldY = pos->y;
    if (HELLFIRE_NONE == m_iStatus)
    {
        return ;
    }
    if (m_iStatus == HELLFIRE_FREE)
    {
        m_nCurFrame ++;
        int nCurFrame;
        nCurFrame = ParamD1[this->m_nCurState][1];
        if (m_nCurFrame >= nCurFrame)
        {
            Initial();
        }
        m_nCurX += ParamD1[m_nCurState][2] * Directory[ParamD1[m_nCurState][0]][0]  ;
        m_nCurY = GetY(m_nCurX) * Directory[ParamD1[m_nCurState][0]][1] / 40 ;
        m_nShowWorldPoint.x = m_nCurWorldPoint.x + m_nCurX;
        m_nShowWorldPoint.y = m_nCurWorldPoint.y + m_nCurY;
        if (abs ( m_iWorldX - m_nShowWorldPoint.x ) < m_iAttackRange / 4 && abs( m_iWorldY - m_nShowWorldPoint.y ) < m_iAttackRange / 4)
        {
            m_iStatus = HELLFIRE_FOLLOW;
        }
        if (m_nShowWorldPoint.x - m_nBoundTop.x < -50 || m_nShowWorldPoint.x - m_nBoundBottom.x > 50
                || m_nShowWorldPoint.y - m_nBoundTop.y < -50 || m_nShowWorldPoint.y - m_nBoundBottom.y > 50)
        {
            m_iStatus = HELLFIRE_TURNTOORIG;
        }
    }
    else if (HELLFIRE_FOLLOW == m_iStatus)
    {
        int iDetalX, iDetalY;
        iDetalX =  m_iWorldX   - m_nShowWorldPoint.x ;
        iDetalY =  m_iWorldY   - m_nShowWorldPoint.y ;
        if (abs (iDetalX) < HELLFIRE_CIRCLE_RADIUS && abs(iDetalY) < HELLFIRE_CIRCLE_RADIUS )
        {
            m_iStatus		= HELLFIRE_CORRECTE;
            m_nCurX			= -HELLFIRE_CIRCLE_RADIUS;
            m_iCircleDir	= 1;
            m_iAttachMan	= 1;
            return;
        }
        if (iDetalX > 0)
        {
            m_nShowWorldPoint.x += iDetalX > 0 ? 3 : -3;
        }
        if (iDetalY > 0)
        {
            m_nShowWorldPoint.y += iDetalY > 0 ? 3 : -3;
        }
        m_nShowWorldPoint.x += iDetalX / HELLFIRE_CIRCLE_RADIUS;
        m_nShowWorldPoint.y += iDetalY / HELLFIRE_CIRCLE_RADIUS;
    }
    else if (HELLFIRE_CORRECTE == m_iStatus)
    {
        int nDetalX, nDetalY;
        nDetalX =  m_iWorldX - HELLFIRE_CIRCLE_RADIUS - m_nShowWorldPoint.x ;
        nDetalY =  m_iWorldY - m_iBmpHeight - m_nShowWorldPoint.y ;
        if (  abs(nDetalX) > 10  || abs(nDetalY) > 10 )
        {
            if (nDetalX > 0)
            {
                m_nShowWorldPoint.x += nDetalX > 0 ? 2 : -2;
            }
            if (nDetalY > 0)
            {
                m_nShowWorldPoint.y += nDetalY > 0 ? 2 : -2;
            }
            m_nShowWorldPoint.x += nDetalX / 5;
            m_nShowWorldPoint.y += nDetalY / 5;
        }
        else
        {
            m_iStatus = HELLFIRE_CIRCLE;
            this->m_iYMoved		= 0;
            this->m_iYMoveDir	= 1;
        }
    }
    else if (HELLFIRE_CIRCLE == m_iStatus)
    {
        m_nCurX	+= m_iCircleDir;
        m_iYMoved += m_iYMoveDir;
        if (m_nCurX >= HELLFIRE_CIRCLE_RADIUS)
        {
            m_iCircleDir = -1;
        }
        else if (m_nCurX <= - HELLFIRE_CIRCLE_RADIUS)
        {
            m_iCircleDir = 1;
        }
        int nRx	= HELLFIRE_CIRCLE_RADIUS  ;
        int nRy	= HELLFIRE_CIRCLE_RADIUS / 3 ;
        m_nCurY	=(int)( sqrt( float(nRy * nRy - m_nCurX * m_nCurX * 1000 / (nRx * nRx)  * (nRy * nRy) / 1000 ))); // 由圆公式计算Y点
        if (m_iCircleDir > 0)
        {
            m_nCurY = - m_nCurY;
        }
        m_nCurY += m_iYMoved;
        if (m_iYMoved > 100)
        {
            m_iYMoveDir = -1 * m_iYMoveDir;    //围绕人上下运动
        }
        else if (m_iYMoved <= 0)
        {
            m_iYMoveDir = -1 * m_iYMoveDir;
        }
        m_nShowWorldPoint.x = m_iWorldX + m_nCurX;
        m_nShowWorldPoint.y = m_iWorldY + m_nCurY;
        if (m_nShowWorldPoint.x >= this->m_iAttackRange + m_nOrigWorldPoint.x || m_nShowWorldPoint.y >= m_iAttackRange + m_nOrigWorldPoint.y)
        {
            m_iStatus = HELLFIRE_TURNTOORIG;
        }
    }
    else if (HELLFIRE_TURNTOORIG == m_iStatus)
    {
        int iDetalX, iDetalY;
        iDetalX =  m_nOrigWorldPoint.x   - m_nShowWorldPoint.x ;
        iDetalY =  m_nOrigWorldPoint.y   - m_nShowWorldPoint.y ;
        if (abs (iDetalX) < 100 && abs(iDetalY) < 100 )
        {
            m_iStatus		= HELLFIRE_FREE;
            return;
        }
        m_nShowWorldPoint.x += iDetalX / 100;
        m_nShowWorldPoint.y += iDetalY / 100;
        //	if (abs (m_nShowWorldPoint.x - m_iWorldX) < m_iAttackRange/5 && abs(m_nShowWorldPoint.y - m_iWorldY) < m_iAttackRange/5)
        //	m_iStatus = HELLFIRE_FOLLOW;
    }
    //	Show(bmpDes);
    CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
    if (pLayer)
    {
        pLayer->ApplyShow(this);
    }
    return;
}

void CHellFire::SetPicture(char* szTitle)
{
    strcpy(m_szAniTitle, szTitle);
    g_objGameDataSet.GetDataAni("ani/weather.ani", m_szAniTitle, EXIGENCE_IMMEDIATE, 30 * 1000);
}




void CHellFire::AddHellFire(int nDensity)
{
    if (nDensity > 10)
    {
        nDensity = 10;
    }
    else if (nDensity <= 0)
    {
        nDensity = 1;
    }
    unsigned int nBgW, nBgH;
    CMySize s;
    g_objGameMap.GetBgSize(s);
    nBgW = s.iWidth;
    nBgH = s.iHeight;
    UINT posBkWorldX, posBkWorldY;
    g_objGameMap.GetBgWorldPos(posBkWorldX, posBkWorldY);
    srand(2000000);
    int iFireNum = (nBgW / 500  ) * nDensity + (nBgH / 500 ) * nDensity  + 2;
    for(int i = 0; i < iFireNum; i++)
    {
        CHellFire* pObjFire = new CHellFire;
        assert(pObjFire);
        char szTitle[100];
        sprintf(szTitle, "hellfire%d", rand() % 3 + 1);
        pObjFire->SetPicture(szTitle);
        pObjFire->Create( posBkWorldX + rand() % nBgW, posBkWorldY + rand() % nBgH);
        CInteractiveLayer* p = g_objGameMap.GetInteractiveLayer();
        p->AddMapObj(pObjFire);
    }
}
