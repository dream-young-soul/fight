
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#include "fish.h"
#include "nd_bitmap.h"
#include "3dgamemap.h"
#include "gamedataset.h"
#include "interactivelayer.h"

extern  CGameMap		g_objGameMap;
enum {OPC_0 , OPC_1 , OPC_2, OPC_3, OPC_4 , OPC_5 , OPC_6, OPC_7, OPC_STOP, OPC_END };

int pattern_1[] = {OPC_4, 25, OPC_3, 8, OPC_2, 38, OPC_3, 3, OPC_2, 50, OPC_1, 8, OPC_0, 8, OPC_7, 8, OPC_6, 78, OPC_7, 5, OPC_6, 50, OPC_5, 8,
                   OPC_4, 10, OPC_3, 10, OPC_2, 70, OPC_1, 3, OPC_2, 3, OPC_3, 3, OPC_2, 50, OPC_1, 10,
                   OPC_0, 10, OPC_7, 10, OPC_6, 75, OPC_5, 10,
                   OPC_4, 10, OPC_5, 25, OPC_6, 10, OPC_7, 15, OPC_0, 5, OPC_1, 10, OPC_2, 10, OPC_3, 5, OPC_4, 5, OPC_5, 5, OPC_6, 28, OPC_7, 3, OPC_6, 60, OPC_7, 5,
                   OPC_0, 10, OPC_1, 10, OPC_2, 30, OPC_1, 20, OPC_0, 30, OPC_7, 10, OPC_6, 5, OPC_5, 5, OPC_6, 30, OPC_5, 10, OPC_4, 3, OPC_END, 0
                  };


//#define ATTEND_SCREEN_SORT

BOOL CFish::Create(int IsDolphin)
{
    m_iStatus			= FISH_CREATE;
    m_pCurPattern		= 0;
    m_iIp				= 0;
    m_iCount			= 0;
    m_iPatternIndex		= 0;
    m_info.curr_frame     = 0;
    m_info.num_frames     = FISH_FRAME;
    m_info.curr_animation = 4;
    m_info.anim_index     = 0;
    m_bIsDolphin		  = IsDolphin;
    int iScreenX, iScreenY;
    iScreenX = 300 + rand() % 100;
    iScreenY = _SCR_HEIGHT + rand() % 100;
    g_objGameMap.Screen2World(iScreenX, iScreenY, m_info.x, m_info.y);
    m_info.xv             = 0;
    m_info.yv             = 0;
    m_bOnTop			  = 0;
    int i;
    for(i = 0; i < MAX_ANIMATIONS; i++)
    {
        m_info.width[i]  = 1;
        m_info.height[i] = 1;
    }
    return 1;
}

void CFish::Reset()
{
    m_pCurPattern = pattern_1;
    m_iIp = 0;
    m_iCount = 0;
    int iScreenX, iScreenY;
    iScreenX = 200 + rand() % 200;
    iScreenY = _SCR_HEIGHT + rand() % 100;
    m_bOnTop				= rand() % 2;
    g_objGameMap.Screen2World(iScreenX, iScreenY, m_info.x, m_info.y);
}

void CFish::Destroy()
{
    m_iStatus = FISH_NONE;
    return;
}

void CFish::Show(void* pInfo)
{
    if (m_info.curr_frame++ >= FISH_FRAME)
    {
        m_info.curr_frame = 0;
    }
    int iXDelta, iYDelta;
    iXDelta = - m_info.width[m_info.curr_animation] / 4;
    iYDelta = - m_info.height[m_info.curr_animation] * 3 / 4;
    CAni* pAni =	g_objGameDataSet.GetDataAni("ani/weather.ani", m_szAniTitle[m_info.curr_animation], EXIGENCE_IMMEDIATE, 30 * 1000);
    pAni->Show(m_info.curr_frame, m_iScreenX + iXDelta , m_iScreenY + iYDelta, 255);
}



void CFish::Fish_AI(void)
{
    if (m_pCurPattern == NULL)
    {
        m_pCurPattern = pattern_1;
        m_iIp = 0;
        m_iCount = 0;
    }
    if (--m_iCount <= 0)
    {
        if (rand() % 10 == 1)
        {
            m_nOPcode  =  OPC_STOP;
            m_nOPrand =  rand() % 30 + 15;
        }
        else
        {
            m_nOPcode  = m_pCurPattern[m_iIp++];
            m_nOPrand = m_pCurPattern[m_iIp++] + rand() % 30;
        }
        int MINSPEED;
        if (m_bIsDolphin)
        {
            MINSPEED = 5;
        }
        else
        {
            MINSPEED = 2;
        }
        switch(m_nOPcode)
        {
        case OPC_0:
            {
                m_info.xv = 0;
                m_info.yv = MINSPEED + rand() % 2;
                Set_Animation(m_nOPcode);
                m_iCount = m_nOPrand;
            }
            break;
        case OPC_1:
            {
                m_info.xv = -MINSPEED - rand() % 2;
                m_info.yv = MINSPEED + rand() % 2;
                Set_Animation(m_nOPcode);
                m_iCount = m_nOPrand;
            }
            break;
        case OPC_2:
            {
                m_info.xv = -MINSPEED - rand() % 2;
                m_info.yv = 0;
                Set_Animation(m_nOPcode);
                m_iCount = m_nOPrand;
            }
            break;
        case OPC_3:
            {
                m_info.xv = -MINSPEED - rand() % 2;
                m_info.yv = -MINSPEED - rand() % 2;
                Set_Animation(m_nOPcode);
                m_iCount = m_nOPrand;
            }
            break;
        case OPC_4:
            {
                m_info.xv = 0;
                m_info.yv = -MINSPEED - rand() % 2;
                Set_Animation(m_nOPcode);
                m_iCount = m_nOPrand;
            }
            break;
        case OPC_5:
            {
                m_info.xv = MINSPEED + rand() % 2;
                m_info.yv = -MINSPEED - rand() % 2;
                Set_Animation(m_nOPcode);
                m_iCount = m_nOPrand;
            }
            break;
        case OPC_6:
            {
                m_info.xv = MINSPEED + rand() % 2;
                m_info.yv = 0;
                Set_Animation(m_nOPcode);
                m_iCount = m_nOPrand;
            }
            break;
        case OPC_7:
            {
                m_info.xv = MINSPEED + rand() % 2;
                m_info.yv = MINSPEED + rand() % 2;
                Set_Animation(m_nOPcode);
                m_iCount = m_nOPrand;
            }
            break;
        case OPC_STOP:
            {
                m_info.xv = 0;
                m_info.yv = 0;
                m_iCount = m_nOPrand;
            }
            break;
        case OPC_END:
            {
                m_info.xv = 0;
                m_info.yv = 0;
                m_pCurPattern = pattern_1;
                m_iIp = 0;
                m_iCount = 0;
            }
            break;
        default:
            break;
        } // end switch
    }
}


void CFish::Process(void* pInfo )
{
    Fish_AI();
    Move_Fish();
    g_objGameMap.World2Screen(m_info.x, m_info.y, m_iScreenX, m_iScreenY);
    if (m_iScreenX < - 150 || m_iScreenX > _SCR_WIDTH + 100 || m_iScreenY  < -150 || m_iScreenY > _SCR_HEIGHT + 100)
    {
        Reset();
    }
    CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
    if (pLayer)
    {
        pLayer->ApplyShow(this);
    }
}


////////////////////////////////////////////////
void CFish::GetPos( CMyPos& posCell)
{
    g_objGameMap.World2Cell(m_info.x, m_info.y, posCell.x, posCell.y);
    if (m_bOnTop)
    {
        posCell.x += 200;
        posCell.y += 200;
    }
}

void CFish::Move_Fish()
{
    m_info.x += m_info.xv;
    m_info.y += m_info.yv;
}

void CFish::Set_Animation(int iopcode)
{
    m_info.curr_animation = iopcode;
    m_info.anim_index = 0;
}

BOOL CFish::SetPicture(int nType)
{
    for(int j = 0; j < MAX_ANIMATIONS; j++)
    {
        sprintf(m_szAniTitle[j], "fish%d_walk%d", nType, j);
        g_objGameDataSet.GetDataAni("ani/weather.ani", m_szAniTitle[j], EXIGENCE_IMMEDIATE, 30 * 1000);
    }
    return 1;
}


void CFish::AddFish(int nNum)
{
    for(int i = 0; i < nNum ; i++)
    {
        CFish*   pObjFish	= new CFish;
        int iAniIndex = rand() % FISH_NUM + 1;
        if (nNum > 5)
            if (iAniIndex == 3)
            {
                iAniIndex = 1;
            }
        pObjFish->SetPicture(iAniIndex);
        if (iAniIndex == 4)
        {
            pObjFish->Create(TRUE);
        }
        else
        {
            pObjFish->Create(FALSE);
        }
        CInteractiveLayer* p = g_objGameMap.GetInteractiveLayer();
        p->AddMapObj(pObjFish);
    }
}

