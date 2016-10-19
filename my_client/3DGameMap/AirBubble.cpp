
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#include "airbubble.h"
#include "3dGameMap.h"
#include "gamedataset.h"
#include <math.h>

extern CGameMap		g_objGameMap;

BOOL CAirBubble::Create(DWORD dwDelayTime, int iWorldX, int iWorldY)
{
    m_iWorldX				= iWorldX;
    m_iWorldY				= iWorldY;
    m_iStatus				= AIRBUBBLE_CREATE;
    m_nOrigWorldPoint.x		= m_iWorldX;
    m_nOrigWorldPoint.y		= m_iWorldY;
    m_nShowWorldPoint.x		= m_iWorldX;
    m_nShowWorldPoint.y		= m_iWorldY;
    m_nLoopCount			= 0;
    m_nSpeed				= rand() % 4 + 2;
    m_dwDelayTime			= dwDelayTime;
    m_dwCreateTime			= TimeGet();
    return 1;
}

void CAirBubble::Destroy()
{
    m_iStatus =  AIRBUBBLE_DESTROY;
    return;
}

void CAirBubble::GetPos(int& iPosX, int& iPosY)
{
    g_objGameMap.World2Cell(m_nOrigWorldPoint.x, m_nOrigWorldPoint.y, iPosX, iPosY);
}


void CAirBubble::Process()
{
    if (AIRBUBBLE_NONE == m_iStatus)
    {
        return ;
    }
    DWORD dwPassTime ;
    DWORD dwNow	= TimeGet();
    dwPassTime = dwNow - m_dwCreateTime;
    if (dwPassTime < m_dwDelayTime)
    {
        return;
    }
    else
    {
        m_iStatus	= AIRBUBBLE_SHOW;
    }
    m_nShowWorldPoint.y	-= m_nSpeed;
    Show(NULL);
    return;
}

void CAirBubble::Show(void* pInfo)
{
    int nScreenX, nScreenY;
    g_objGameMap.World2Screen( m_nShowWorldPoint.x - rand() % 3 + 1, m_nShowWorldPoint.y, nScreenX, nScreenY);
    if (rand() % 5 == 1)
    {
        m_nLoopCount ++;
    }
    if (m_nLoopCount >= BUBBLE_FRAME)
    {
        m_nLoopCount = 0;
    }
    CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", m_szFileName );
    pAni->Show(m_nLoopCount, nScreenX, nScreenY, 255, _SHOWWAY_ADDITIVE);
    if (nScreenX < - 50 || nScreenX > _SCR_WIDTH + 100 || nScreenY  < -100 || nScreenY > _SCR_HEIGHT + 100)
    {
        m_iStatus = AIRBUBBLE_DESTROY;
    }
}



//================================================================================

void CAirBubbleSys::AddBubble(int iWorldX, int iWorldY)
{
    if (m_nStatus == AIRBUBBLE_NONE)
    {
        return ;
    }
    int iNum = rand() % 5 + 1;
    int i;
    for(i = 0; i < iNum; i++)
    {
        CAirBubble*   pObjBubble	= new CAirBubble;
        int nAni					= rand() % BUBBLE_TYPE;
        pObjBubble->SetPicture(m_szFileName[nAni]);
        DWORD dwDelayTime			= rand() % 1000;
        pObjBubble->Create(dwDelayTime, iWorldX - rand() % 4 + 2, iWorldY   );
        m_dequeBubble.push_back(pObjBubble);
    }
}

BOOL CAirBubbleSys::Create(int nNum, int nType)
{
    Destroy();
    int i;
    m_nNum = nNum;
    for(i = 0; i < BUBBLE_TYPE; i++)
    {
        sprintf(m_szFileName[i], "airbubble%d", i + 1);
    }
    m_nStatus = AIRBUBBLE_CREATE;
    int iX, iY;
    iX							= rand() % 700 + 20;
    iY							= rand() % 400 + 100;
    int iWorldX, iWorldY;
    g_objGameMap.Screen2World(iX, iY, iWorldX, iWorldY);
    for(i = 0; i < nNum; i++)
    {
        CAirBubble*   pObjBubble	= new CAirBubble;
        pObjBubble->SetPicture(m_szFileName[rand()%BUBBLE_TYPE]);
        DWORD dwDelayTime			= rand() % 1000 * 10;
        pObjBubble->Create(dwDelayTime, iWorldX, iWorldY   );
        m_dequeBubble.push_back(pObjBubble);
    }
    this->m_dwCreateTime = TimeGet();
    return 1;
}

void CAirBubbleSys::Show( )
{
    if (m_nStatus == AIRBUBBLE_NONE)
    {
        return ;
    }
    int i;
    DWORD dwNow		= TimeGet();
    DWORD dwEscape	= dwNow - m_dwCreateTime;
    DWORD dwInterval = m_nNum * 100;
    if (dwEscape >= 1500 - dwInterval || m_dequeBubble.size() < 40 )
    {
        m_dwCreateTime = dwNow;
        int iNum = rand() % 15 + 2;
        int iX, iY;
        iX		= rand() % 730 + 5;
        iY		= rand() % 350 + 100;
        int iWorldX, iWorldY;
        g_objGameMap.Screen2World(iX, iY, iWorldX, iWorldY);
        for(i = 0; i < iNum; i++)
        {
            CAirBubble*   pObjBubble	= new CAirBubble;
            pObjBubble->SetPicture(m_szFileName[rand()%BUBBLE_TYPE]);
            DWORD dwDelayTime			= rand() % 1000 * 3;
            pObjBubble->Create(dwDelayTime, iWorldX - rand() % 20 + 10, iWorldY   );
            m_dequeBubble.push_back(pObjBubble);
        }
    }
    for(i = 0; i < (int)m_dequeBubble.size(); i++)
        if (m_dequeBubble[i]->m_iStatus == AIRBUBBLE_DESTROY)
        {
            if (m_dequeBubble[i])
            {
                delete m_dequeBubble[i];
            }
            m_dequeBubble[i] = NULL;
            m_dequeBubble.erase(m_dequeBubble.begin() + i);
        }
        else
        {
            m_dequeBubble[i]->Process(  );
        }
}





void CAirBubbleSys::Destroy()
{
    if (m_nStatus == AIRBUBBLE_NONE)
    {
        return;
    }
	
	for(int i = 0 ; i < (int)m_dequeBubble.size(); i++)
    {
        if (m_dequeBubble[i])
        {
            delete m_dequeBubble[i];
        }
        m_dequeBubble[i] = NULL;
    }
    m_dequeBubble.clear();
    m_nStatus = AIRBUBBLE_NONE;
}

