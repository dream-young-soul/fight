
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#include "cloud.h"
#include "3dgamemap.h"
#include "gamedataset.h"

BOOL CCloud::Create(int nCloud0, int nCloud1, int nCloud2, int nCloud3)
{
    m_nCloudResource[0]	= nCloud0;
    m_nCloudResource[1]	= nCloud1;
    m_nCloudResource[2]	= nCloud2;
    m_nCloudResource[3]	= nCloud3;
    m_bCount			= 0;
    g_objGameMap.Screen2World(-200 - rand() % 600, rand() % _SCR_HEIGHT, m_infoCloud[0].nWorldX, m_infoCloud[0].nWorldY);
    if (m_nCloudResource[0])
    {
        m_infoCloud[0].nCurSport = 0;
        m_infoCloud[0].nSportDirX = 1;
        m_infoCloud[0].nSportDirY = 0;
        m_infoCloud[0].nSportRadian = 160 + _SCR_WIDTH;
        m_infoCloud[0].nCurAlpha = 1;
        m_infoCloud[0].AlphaChange = 1;
        m_infoCloud[0].nCurFrame = 0;
    }
    if (m_nCloudResource[1])
    {
        m_infoCloud[1].nCurSport = 0;
        if (rand() % 2)
        {
            m_infoCloud[1].nSportDirX = 1;
            m_infoCloud[1].nSportDirY = 0;
            m_infoCloud[1].nSportRadian = rand() % 10 + 20;
        }
        else
        {
            m_infoCloud[1].nSportDirY = 1;
            m_infoCloud[1].nSportDirX = 0;
            m_infoCloud[1].nSportRadian = rand() % 10 + 5;
        }
        m_infoCloud[1].nCurAlpha = 1;
        m_infoCloud[1].AlphaChange = 1;
        m_infoCloud[1].nCurFrame = 0;
        m_infoCloud[1].nWorldX = m_infoCloud[0].nWorldX + 28;
        m_infoCloud[1].nWorldY = m_infoCloud[0].nWorldY + 10;
    }
    if (m_nCloudResource[2])
    {
        m_infoCloud[2].nCurSport = 0;
        if (rand() % 2)
        {
            m_infoCloud[2].nSportDirX = 1;
            m_infoCloud[2].nSportDirY = 0;
            m_infoCloud[2].nSportRadian = rand() % 10 + 30;
        }
        else
        {
            m_infoCloud[2].nSportDirY = 1;
            m_infoCloud[2].nSportDirX = 0;
            m_infoCloud[2].nSportRadian = rand() % 10 + 10;
        }
        m_infoCloud[2].nCurAlpha = 1;
        m_infoCloud[2].AlphaChange = 1;
        m_infoCloud[2].nCurFrame = 0;
        m_infoCloud[2].nWorldX = m_infoCloud[0].nWorldX - 25;
        m_infoCloud[2].nWorldY = m_infoCloud[0].nWorldY + 5;
    }
    if (m_nCloudResource[3])
    {
        m_infoCloud[3].nCurSport = 0;
        if (rand() % 2)
        {
            m_infoCloud[3].nSportDirX = 1;
            m_infoCloud[3].nSportDirY = 0;
            m_infoCloud[3].nSportRadian = rand() % 20 + 20;
        }
        else
        {
            m_infoCloud[3].nSportDirY = 1;
            m_infoCloud[3].nSportDirX = 0;
            m_infoCloud[3].nSportRadian = rand() % 10 + 10;
        }
        m_infoCloud[3].nCurAlpha = 1;
        m_infoCloud[3].AlphaChange = 1;
        m_infoCloud[3].nCurFrame = 0;
        m_infoCloud[3].nWorldX = m_infoCloud[0].nWorldX - 10;
        m_infoCloud[3].nWorldY = m_infoCloud[0].nWorldY - 8;
    }
    return 1;
}


void CCloud::Process()
{
    char szTitle[256];
    int i;
    for(i = 0; i < 4; i++)
    {
        if (m_nCloudResource[i])
        {
            sprintf(szTitle, "Cloud%d", m_nCloudResource[i]);
            CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", szTitle);
            if (pAni)
            {
                int nX, nY;
                g_objGameMap.World2Screen(m_infoCloud[i].nWorldX, m_infoCloud[i].nWorldY, nX, nY);
                if ( nX < -800 || nY < - 300 || nX > _SCR_WIDTH + 200 || nY > _SCR_HEIGHT + 100)
                {
                    g_objGameMap.Screen2World( -200 - rand() % 600 , rand() % _SCR_HEIGHT - 100, m_infoCloud[i].nWorldX, m_infoCloud[i].nWorldY);
                    m_infoCloud[0].nCurAlpha = 1;
                    m_infoCloud[0].AlphaChange = 1;
                    m_infoCloud[0].nCurSport = 0;
                    m_infoCloud[0].nSportRadian = rand() % 200 + _SCR_WIDTH;
                    m_infoCloud[0].nSportDirX = 1;
                    m_infoCloud[0].nSportDirY = 0;
                    for(int n = 1; n < 4; n++)
                    {
                        m_infoCloud[n].nCurSport = 0;
                        m_infoCloud[n].nSportDirX *= -1;
                        m_infoCloud[n].nSportDirY *= -1;
                        m_infoCloud[n].nCurAlpha = 1;
                        m_infoCloud[n].AlphaChange = 1;
                        m_infoCloud[n].nCurFrame = 0;
                        m_infoCloud[n].nWorldX = m_infoCloud[0].nWorldX + rand() % 50;
                        m_infoCloud[n].nWorldY = m_infoCloud[0].nWorldY + rand() % 20;
                        if (m_infoCloud[n].nSportDirY )
                        {
                            m_infoCloud[n].nSportRadian = rand() % 10 + 8;
                        }
                        else
                        {
                            m_infoCloud[n].nSportRadian = rand() % 20 + 20;
                        }
                    }
                }
                pAni->Show(	m_infoCloud[i].nCurFrame, nX, nY, m_infoCloud[i].nCurAlpha, _SHOWWAY_NORMAL);
            }
        }
    }
    m_bCount ++;
    if (m_bCount  > 2)
    {
        m_bCount = 0;
        for(i = 0; i < 4; i++)
        {
            if (m_nCloudResource[i])
            {
                sprintf(szTitle, "Cloud%d", m_nCloudResource[i]);
                CAni* pAni = g_objGameDataSet.GetDataAni("ani/weather.ani", szTitle);
                if (pAni)
                {
                    if (++m_infoCloud[i].nCurFrame >= pAni->GetFrameAmount())
                    {
                        m_infoCloud[i].nCurFrame = 0;
                    }
                }
                if (rand() % 8 == 0)
                {
                    m_infoCloud[i].nCurAlpha += m_infoCloud[i].AlphaChange;
                }
                if (m_infoCloud[i].nCurAlpha >= 255)
                {
                    m_infoCloud[i].nCurAlpha = 255;
                    m_infoCloud[i].AlphaChange = -1 ;
                }
                if (m_infoCloud[i].nCurAlpha < 100)
                {
                    m_infoCloud[i].nCurAlpha = 100;
                    m_infoCloud[i].AlphaChange = 1;
                }
                m_infoCloud[i].nCurSport++;
                if (m_infoCloud[0].nSportDirX)
                {
                    m_infoCloud[i].nWorldX += m_infoCloud[0].nSportDirX;
                }
                if (m_infoCloud[0].nSportDirY)
                {
                    m_infoCloud[i].nWorldY +=  m_infoCloud[0].nSportDirY;
                }
                if (i > 0)
                {
                    if (m_infoCloud[i].nCurSport > m_infoCloud[i].nSportRadian)
                    {
                        m_infoCloud[i].nCurSport = 0;
                        m_infoCloud[i].nSportDirX *= -1;
                        m_infoCloud[i].nSportDirY *= -1;
                        if (m_infoCloud[i].nSportDirY )
                        {
                            m_infoCloud[i].nSportRadian = rand() % 10 + 8;
                        }
                        else
                        {
                            m_infoCloud[i].nSportRadian = rand() % 20 + 20;
                        }
                    }
                    if (m_infoCloud[i].nSportDirX)
                    {
                        m_infoCloud[i].nWorldX +=  m_infoCloud[i].nSportDirX;
                    }
                    if (m_infoCloud[i].nSportDirY)
                        if (rand() % 3 == 0)
                        {
                            m_infoCloud[i].nWorldY +=  m_infoCloud[i].nSportDirY;
                        }
                }
            }
        }//endof for
    }
}







//================================================================================
//       cloudsystem
//
//==============================================================================



BOOL CCloudSys::Create(int nCloudNum)
{
    Destroy();
    if (nCloudNum > MAXCLOUD)
    {
        nCloudNum = MAXCLOUD;
    }
    int i;
    for(i = 0; i < MAXCLOUD; i++)
    {
        CCloud* pCloud = new CCloud;
        pCloud->Create(rand() % 9 + 1, rand() % 9 + 1, rand() % 9 + 1, rand() % 9 + 1);
        this->m_dequeCloud.push_back(pCloud);
    }
    m_nCurAmound = 1;
    m_nTargetAmound = nCloudNum;
    m_bStatus = CLOUD_CREATE;
    return 1;
}


void CCloudSys::Process(int& nStatus)
{
    if (	m_bStatus == CLOUD_NONE || m_bStatus == CLOUD_PAUSE)
    {
        return;
    }
    int i;
    if (m_bStatus == CLOUD_CREATE )
    {
        if (m_nCurAmound < m_nTargetAmound)
        {
            int nAlternation = m_nCurAmound * 10;
            nAlternation = 199 - nAlternation;
            if (rand() % nAlternation == 0)
            {
                m_nCurAmound ++;
            }
        }
    }
    else
    {
        if (m_bStatus == CLOUD_STOP)
        {
            if (m_nCurAmound > 1)
            {
                int nAlternation = m_nCurAmound * 10;
                nAlternation = 199 - nAlternation;
                if (rand() % nAlternation == 0)
                {
                    m_nCurAmound --;
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
            //reset
            int nAlternation = m_nCurAmound * 10;
            nAlternation = 199 - nAlternation;
            if (rand() % nAlternation == 0)
            {
                if (m_nCurAmound > m_nTargetAmound)
                {
                    m_nCurAmound--;
                }
                else if (m_nCurAmound < m_nTargetAmound)
                {
                    m_nCurAmound++;
                }
            }
        }
    }
    for(i = 0; i < m_nCurAmound ; i++)
    {
        m_dequeCloud[i]->Process();
    }
}

void CCloudSys::Destroy()
{
    m_bStatus = CLOUD_NONE;
    int i;
    for(i = 0; i < (int)m_dequeCloud.size(); i++)
    {
        if (m_dequeCloud[i])
        {
            delete m_dequeCloud[i];
        }
    }
    m_dequeCloud.clear();
}

void CCloudSys::Stop()
{
    m_bStatus = CLOUD_STOP;
}

void CCloudSys::ReSet(int nnum)
{
    if (nnum > MAXCLOUD)
    {
        nnum = MAXCLOUD;
    }
    m_bStatus = CLOUD_RESET;
    m_nTargetAmound = nnum;
}

void CCloudSys::Pause()
{
    m_bStatus = CLOUD_PAUSE;
}

void CCloudSys::Continue()
{
    m_bStatus = CLOUD_CREATE;
}
