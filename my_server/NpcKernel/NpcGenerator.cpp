
//**********************************************************
// 代码编辑器
//**********************************************************

// NpcGenerator.cpp: implementation of the CNpcGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "NpcGenerator.h"
#include "BaseFunc.h"
#include "NpcWorld.h"
#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
char	szGenerator[] = _TBL_GENERATOR;
const int	RANDOM_GENERATOR_SECS			= 600;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpcGenerator::CNpcGenerator()
{
    SetObjType(OBJ_NPCGEN);
    m_pData	= CNpcGenData::CreateNew();
    m_nAmount			= 0;
    m_nGenAmount		= 0;
    m_nCurrGen			= 0;
}

//////////////////////////////////////////////////////////////////////
CNpcGenerator::~CNpcGenerator()
{
    if (m_pData)
    {
        m_pData->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CNpcGenerator::Create(IRecordset* pRes)
{
    if (!m_pData->Create(pRes))
    {
        return false;
    }
    m_timeoutRest.SetInterval(GetInt(NPCGENDATA_RESTSECS) + ::RandGet(GetInt(NPCGENDATA_RESTSECS)));
    m_timeoutRest.Update();
    int nGrid = GetInt(NPCGENDATA_GRID);
    IF_NOT(nGrid >= 1)
    m_nMaxNpc = 1;
    else
    {
        m_nMaxNpc		= (GetInt(NPCGENDATA_BOUNDCX) / nGrid) * (GetInt(NPCGENDATA_BOUNDCY) / nGrid);
    }
    if (m_nMaxNpc < 1)
    {
        m_nMaxNpc = 1;
    }
    m_idxLastGen	= ::RandGet(m_nMaxNpc);
    m_pMap			= MapManager()->QueryMap(GetInt(NPCGENDATA_MAPID));
    if (!m_pMap)
    {
        LOGERROR("Error: npc generator [%u] got invalid map [%u]!", this->GetID(), this->GetInt(NPCGENDATA_MAPID));
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
POINT CNpcGenerator::FindGenPos()
{
    POINT pos;
    if (GetInt(NPCGENDATA_BORNX) || GetInt(NPCGENDATA_BORNY))
    {
        pos.x = GetInt(NPCGENDATA_BORNX);
        pos.y = GetInt(NPCGENDATA_BORNY);
        return pos;
    }
    pos.x = GetInt(NPCGENDATA_BOUNDX);
    pos.y = GetInt(NPCGENDATA_BOUNDY);
    if (m_nMaxNpc <= 1)
    {
        pos.x = GetInt(NPCGENDATA_BOUNDX) + ::RandGet(GetInt(NPCGENDATA_BOUNDCX));
        pos.y = GetInt(NPCGENDATA_BOUNDY) + ::RandGet(GetInt(NPCGENDATA_BOUNDCY));
        return pos;
    }
    m_idxLastGen++;
    if (m_idxLastGen >= m_nMaxNpc)
    {
        m_idxLastGen = 0;
    }
    IF_NOT(GetInt(NPCGENDATA_GRID) > 0)
    return pos;
    int nGridX = GetInt(NPCGENDATA_BOUNDCX) / GetInt(NPCGENDATA_GRID);
    if (nGridX < 0)
    {
        nGridX = 1;
    }
    pos.x = GetInt(NPCGENDATA_BOUNDX) + GetInt(NPCGENDATA_GRID) * (m_idxLastGen % nGridX) + ::RandGet(GetInt(NPCGENDATA_GRID));
    pos.y = GetInt(NPCGENDATA_BOUNDY) + GetInt(NPCGENDATA_GRID) * (m_idxLastGen / nGridX) + ::RandGet(GetInt(NPCGENDATA_GRID));
    return pos;
}

//////////////////////////////////////////////////////////////////////
int CNpcGenerator::Generator(int nNum)		// return : Generator num
{
    if (!m_pMap)
    {
        return 0;
    }
    if (GetInt(NPCGENDATA_BEGIN))
    {
        if (!::IsTimeRange(GetInt(NPCGENDATA_BEGIN), GetInt(NPCGENDATA_END)))
        {
            return 0;
        }
    }
    if (m_nGenAmount >= GetMaxNpc())
    {
        return 0;
    }
    if (m_timeoutRest.ToNextTime())
    {
        if (GetInt(NPCGENDATA_RESTSECS) >= RANDOM_GENERATOR_SECS)
        {
            m_timeoutRest.Startup(GetInt(NPCGENDATA_RESTSECS) + ::RandGet(GetInt(NPCGENDATA_RESTSECS)));
        }
        m_nCurrGen	= GetInt(NPCGENDATA_MAXPERGEN);
    }
    if (m_nCurrGen <= 0)
    {
        return 0;
    }
    int nGenNum = 0;
    for(int i = 0; i < nNum; i++)
    {
        // 找合法地图点
        POINT pos	= FindGenPos();
        if (!m_pMap->IsStandEnable(pos.x, pos.y))
        {
            pos.x	= GetInt(NPCGENDATA_BOUNDX) + ::RandGet(GetInt(NPCGENDATA_BOUNDCX));
            pos.y	= GetInt(NPCGENDATA_BOUNDY) + ::RandGet(GetInt(NPCGENDATA_BOUNDCY));
            if (!m_pMap->IsStandEnable(pos.x, pos.y))
            {
                continue;    // 下次再生成
            }
        }
        OBJID idNewNpc	= NpcManager()->SpawnNewNpcID();
        m_nCurrGen--;
        m_nGenAmount++;
        CMsgAiNpcInfo		msg;
        if (msg.Create(MSGAINPCINFO_CREATENEW, idNewNpc, GetID(), GetInt(NPCGENDATA_NPCTYPE), GetInt(NPCGENDATA_MAPID), pos.x, pos.y))
        {
            NpcWorld()->SendMsg(ID_NONE, &msg);
        }
        nGenNum++;
        if (m_nCurrGen <= 0)
        {
            break;
        }
    }
    return nGenNum;			// 增加了一个NPC
}

//////////////////////////////////////////////////////////////////////
void CNpcGenerator::GeneratorOK(OBJID idNpc)
{
    CHECK(idNpc != ID_NONE);
    m_nAmount++;
    if (m_nGenAmount - m_nAmount > MAXNPC_PER_ONTIMER)			// 强制同步
    {
        LOGERROR("NPC发生器生成NPC数量[%d]，游戏服务器创建NPC数量[%d]。差距太大，强制同步。", m_nGenAmount, m_nAmount);
        m_nGenAmount = m_nAmount + MAXNPC_PER_ONTIMER;
    }
}

//////////////////////////////////////////////////////////////////////
void CNpcGenerator::DeleteNpc(OBJID idNpc)
{
    CHECK(m_nAmount > 0);
    m_nAmount--;
    m_nGenAmount--;
    if (m_nGenAmount < m_nAmount)
    {
        LOGERROR("NPC发生器生成NPC数量[%d]，游戏服务器创建NPC数量[%d]。数量错误，强制同步。", m_nGenAmount, m_nAmount);
        m_nGenAmount = m_nAmount;
    }
}











