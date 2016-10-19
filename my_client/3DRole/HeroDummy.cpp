
//**********************************************************
// 代码编辑器
//**********************************************************

//---------------------------------------------------------------------------------------
// HeroDummy.cpp
//---------------------------------------------------------------------------------------
#include "Hero.h"
#include "3DGameMap.h"
#include "3DRoleData.h"
#include "GameMsg.h"
#include "AllMsg.h"
#include "GamePlayerSet.h"

//////////////////////////////////////////////////////////////////////
void CHero::CreateStatuty()
{
    // 比较地图ID
    const OBJID idMapDoc0 = 1037;
    const OBJID idMapDoc1 = 1038;
    OBJID idDoc = g_objGameMap.GetIDDoc();
    int Statuary[7][3];
    if (idDoc == idMapDoc0)
    {
        memcpy(&Statuary, &Statuary1037, sizeof(int) * 3 * 7);
    }
    else if (idDoc == idMapDoc1)
    {
        memcpy(&Statuary, &Statuary1038, sizeof(int) * 3 * 7);
    }
    else
    {
        OBJID idStr = 100128;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));
        return;
    }
    // 检查职务级别
    int nRank = this->GetSyndicateRank();
    if ((nRank != RANK_LEADER) && (nRank != RANK_ASSISTANT) && (nRank != RANK_SUBLEADER))
    {
        OBJID idStr = 100129;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));
        return;
    }
    // 点检查
    CMyPos posStatuary, posHero;
    this->GetPos(posHero);
    switch(nRank)
    {
    case RANK_LEADER:
        {
            posStatuary.x = Statuary[0][0];
            posStatuary.y = Statuary[0][1];
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(posStatuary.x, posStatuary.y, _ROLE_STATUARY_NPC);
            if (pPlayer)
            {
                return;
            }
        }
        break;
    case RANK_ASSISTANT:
        {
            int nDistance = 999999999;
            for(int i = 1; i < 3; i ++)
            {
                int nMyDistance = (Statuary[i][0] - posHero.x) * (Statuary[i][0] - posHero.x) +
                                  (Statuary[i][1] - posHero.y) * (Statuary[i][1] - posHero.y);
                CPlayer* pPlayer = g_objPlayerSet.GetPlayer(Statuary[i][0], Statuary[i][1], _ROLE_STATUARY_NPC);
                if (pPlayer)
                {
                    continue;
                }
                if (nMyDistance < nDistance)
                {
                    nDistance = nMyDistance;
                    posStatuary.x = Statuary[i][0];
                    posStatuary.y = Statuary[i][1];
                }
            }
        }
        break;
    case RANK_SUBLEADER:
        {
            int nDistance = 999999999;
            for(int i = 3; i < 7; i ++)
            {
                int nMyDistance = (Statuary[i][0] - posHero.x) * (Statuary[i][0] - posHero.x) +
                                  (Statuary[i][1] - posHero.y) * (Statuary[i][1] - posHero.y);
                CPlayer* pPlayer = g_objPlayerSet.GetPlayer(Statuary[i][0], Statuary[i][1], _ROLE_STATUARY_NPC);
                if (pPlayer)
                {
                    continue;
                }
                if (nMyDistance < nDistance)
                {
                    nDistance = nMyDistance;
                    posStatuary.x = Statuary[i][0];
                    posStatuary.y = Statuary[i][1];
                }
            }
        }
        break;
    }
    const int nMaxDistance = 4;
    if ((posStatuary.x - posHero.x > nMaxDistance) && (posStatuary.y - posHero.y > nMaxDistance))
    {
        OBJID idStr = 100128;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));
        return;
    }
    // 开始发送放置消息
    CMsgNpcInfo msg;
    if (msg.Create(posStatuary, m_objDummy.GetDir(), m_objDummy.GetFrameIndex(), m_objDummy.GetAction()))
    {
        msg.Send();
    }
}
//-----------------------------------------------------------------------------------------------
