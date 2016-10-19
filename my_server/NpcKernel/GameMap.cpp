
//**********************************************************
// 代码编辑器
//**********************************************************

// GameMap.cpp: implementation of the CGameMap class.
//
//////////////////////////////////////////////////////////////////////

#include "define.h"
#include "I_MapData.h"
#include "NpcWorld.h"
#include "GameMap.h"

char	szMapTable[] = _TBL_MAP;

//////////////////////////////////////////////////////////////////////
const int	MAPFLAG_PER_ROLE	= 2;		// 每个角色占多少个地图标记数
const int	MAX_MONSTERSIZEADD	= 4;		// 支持最大的size_add

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGameMap::CGameMap()
{
    SetObjType(OBJ_MAP);
    m_pMapData	= NULL;
    m_pData		= CGameMapData::CreateNew();
    m_nStatus	= STATUS_NONE;
}

//////////////////////////////////////////////////////////////////////
CGameMap::~CGameMap()
{
    if (m_pMapData)
    {
        m_pMapData->Release();
    }
    if (m_pData)
    {
        m_pData->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::Create(IRecordset* pRes)
{
    CHECKF(m_pData);
    if (!m_pData->Create(pRes))
    {
        return false;
    }
    m_pMapData = IMapData::CreateNew(m_pData->GetInt(GAMEMAPDATA_MAPDOC), MAPDATA_VERSION);
    if (!m_pMapData)
    {
        MSGBUF	szMsg;
        sprintf(szMsg, "ERROR：Can't load data file of map[%u]!", GetID());
        NpcWorld()->PrintText(szMsg);
        return false;
    }
    // 装载地图
    m_setBlock.resize(GetWidthOfBlock());
    for(int i = 0; i < m_setBlock.size(); i++)
    {
        m_setBlock[i].resize(GetHeightOfBlock());
        for(int j = 0; j < m_setBlock[i].size(); j++)
        {
            m_setBlock[i][j].Create();
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::IsStandEnable(int nPosX, int nPosY)
{
    if (!( nPosX >= 0 && nPosX < m_pMapData->GetMapWidth() && nPosY >= 0 && nPosY < m_pMapData->GetMapHeight() ))
    {
        return false;
    }
    return (m_pMapData->GetFloorMask(nPosX, nPosY) == 0);
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::IsMoveEnable(int x, int y, int nDir, int nSizeAdd, int nClimbCap /*= 0*/)
{
    if (nSizeAdd > MAX_MONSTERSIZEADD)
    {
        nSizeAdd = MAX_MONSTERSIZEADD;
    }
    int nNewX = x + _DELTA_X[nDir];
    int nNewY = y + _DELTA_Y[nDir];
    if (!IsValidPoint(nNewX, nNewY))
    {
        return false;
    }
    if (m_pMapData->GetFloorMask(nNewX, nNewY) != 0)
    {
        return false;
    }
    if (nSizeAdd <= 2 && m_pMapData->GetRoleAmount(nNewX, nNewY) > nSizeAdd)
    {
        return false;
    }
    if (nClimbCap && m_pMapData->GetFloorAlt(nNewX, nNewY) - m_pMapData->GetFloorAlt(x, y) > nClimbCap)
    {
        return false;
    }
    int nEnableVal	= nSizeAdd % 2;	// 扩展探测点需要剩余的值
    if (nSizeAdd > 0 && nSizeAdd <= 2)
    {
        int	nMoreDir	= (nDir % 2) ? 1 : 2;					// 需要向两个方向各多探测多少个DIR
        for(int i = -1 * nMoreDir; i <= nMoreDir; i++)
        {
            int nDir2	= (nDir + i + 8) % 8;				// 探测方向
            int nNewX2	= nNewX + _DELTA_X[nDir2];			// 探测点
            int nNewY2	= nNewY + _DELTA_Y[nDir2];
            if (IsValidPoint(nNewX2, nNewY2) && m_pMapData->GetRoleAmount(nNewX2, nNewY2) > nEnableVal)
            {
                return false;
            }
        }
    }
    else if (nSizeAdd > 2)
    {
        int	nRange		= (nSizeAdd + 1) / 2;
        for(int i = nNewX - nRange; i <= nNewX + nRange; i++)
        {
            for(int j = nNewY - nRange; j <= nNewY + nRange; j++)
            {
                if (Distance(i, j, x, y) > nRange)
                {
                    if (IsValidPoint(i, j) && m_pMapData->GetRoleAmount(i, j) > nEnableVal)
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::IsSuperposition(IRole* pRole)
{
    CGameBlock* pBlock = &BlockSet(pRole->GetPosX(), pRole->GetPosY());
    for(int i = 0; i < pBlock->QueryUserSet()->GetAmount(); i++)
    {
        CUserPtr pUser = pBlock->QueryUserSet()->GetObjByIndex(i);
        if (pUser && pUser->GetPosX() == pRole->GetPosX() && pUser->GetPosY() == pRole->GetPosY())
        {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
/*bool CGameMap::IsActiveCell(int nPosX, int nPosY)
{
	CHECKF(nPosX >= 0 && nPosX < m_pMapData->GetMapWidth());
	CHECKF(nPosY >= 0 && nPosY < m_pMapData->GetMapHeight());

	return BlockSet(nPosX, nPosY).IsDormancy();
}*/

//////////////////////////////////////////////////////////////////////
bool CGameMap::SetDormancy(int nPosX, int nPosY)		// return false : 附近有玩家
{
    CHECKF(nPosX >= 0 && nPosX < m_pMapData->GetMapWidth());
    CHECKF(nPosY >= 0 && nPosY < m_pMapData->GetMapHeight());
    if (BlockSet(nPosX, nPosY).IsDormancy())
    {
        return true;
    }
    int x, y;
    FOR_9_BLOCKS(this, nPosX, nPosY)
    {
        if (BlockByIndex(x, y).QueryUserSet()->GetAmount() > 0)
        {
            return false;
        }
    }
    BlockSet(nPosX, nPosY).SetDormancy(true);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CGameMap::EnterMap(CUser* pUser)
{
    CHECK(pUser->GetPosX() >= 0 && pUser->GetPosX() < m_pMapData->GetMapWidth());
    CHECK(pUser->GetPosY() >= 0 && pUser->GetPosY() < m_pMapData->GetMapHeight());
    if (pUser->IsBlockNpc())
    {
        IncRole(pUser->GetPosX(), pUser->GetPosY());
    }
    BlockSet(pUser->GetPosX(), pUser->GetPosY()).QueryUserSet()->AddObj(pUser);
    // 唤醒休眠NPC
    CallWakeMonster(pUser->GetPosX(), pUser->GetPosY());
}

//////////////////////////////////////////////////////////////////////
void CGameMap::EnterMap(INpc* pNpc)
{
    CHECK(pNpc->GetPosX() >= 0 && pNpc->GetPosX() < m_pMapData->GetMapWidth());
    CHECK(pNpc->GetPosY() >= 0 && pNpc->GetPosY() < m_pMapData->GetMapHeight());
    if (pNpc->IsBlockNpc())
    {
        IncRole(pNpc->GetPosX(), pNpc->GetPosY(), pNpc->GetSizeAdd());
    }
    BlockSet(pNpc->GetPosX(), pNpc->GetPosY()).QueryNpcSet()->AddObj(pNpc);
    // 唤醒休眠NPC
    if (Cast<CAgent>(pNpc))
    {
        CallWakeMonster(pNpc->GetPosX(), pNpc->GetPosY());
    }
}

//////////////////////////////////////////////////////////////////////
void CGameMap::MoveTo(CUser* pUser, int nNewPosX, int nNewPosY)
{
    int nOldPosX = pUser->GetPosX();
    int nOldPosY = pUser->GetPosY();
    CHECK(nOldPosX >= 0 && nOldPosX < m_pMapData->GetMapWidth());
    CHECK(nOldPosY >= 0 && nOldPosY < m_pMapData->GetMapHeight());
    CHECK(nNewPosX >= 0 && nNewPosX < m_pMapData->GetMapWidth());
    CHECK(nNewPosY >= 0 && nNewPosY < m_pMapData->GetMapHeight());
    if (pUser->IsBlockNpc())
    {
        DecRole(nOldPosX, nOldPosY);
        IncRole(nNewPosX, nNewPosY);
    }
    if (Block(nOldPosX) == Block(nNewPosX) && Block(nOldPosY) == Block(nNewPosY))
    {
        return;
    }
    BlockSet(nOldPosX, nOldPosY).QueryUserSet()->DelObj(pUser->GetID());
    BlockSet(nNewPosX, nNewPosY).QueryUserSet()->AddObj(pUser);
    // 唤醒休眠NPC
    CallWakeMonster(nNewPosX, nNewPosY);
}

//////////////////////////////////////////////////////////////////////
void CGameMap::MoveTo(INpc* pNpc, int nNewPosX, int nNewPosY)
{
    int nOldPosX = pNpc->GetPosX();
    int nOldPosY = pNpc->GetPosY();
    CHECK(nOldPosX >= 0 && nOldPosX < m_pMapData->GetMapWidth());
    CHECK(nOldPosY >= 0 && nOldPosY < m_pMapData->GetMapHeight());
    CHECK(nNewPosX >= 0 && nNewPosX < m_pMapData->GetMapWidth());
    CHECK(nNewPosY >= 0 && nNewPosY < m_pMapData->GetMapHeight());
    if (pNpc->IsBlockNpc())
    {
        DecRole(nOldPosX, nOldPosY, pNpc->GetSizeAdd());
        IncRole(nNewPosX, nNewPosY, pNpc->GetSizeAdd());
    }
    if (Block(nOldPosX) == Block(nNewPosX) && Block(nOldPosY) == Block(nNewPosY))
    {
        return;
    }
    BlockSet(nOldPosX, nOldPosY).QueryNpcSet()->DelObj(pNpc->GetID());
    BlockSet(nNewPosX, nNewPosY).QueryNpcSet()->AddObj(pNpc);
    // 唤醒休眠NPC
    if (Cast<CAgent>(pNpc))
    {
        CallWakeMonster(nNewPosX, nNewPosY);
    }
}

//////////////////////////////////////////////////////////////////////
void CGameMap::IncRole(int x, int y, int nSizeAdd /*= 0*/)
{
    CHECK(nSizeAdd >= 0);
    if (nSizeAdd > MAX_MONSTERSIZEADD)
    {
        nSizeAdd = MAX_MONSTERSIZEADD;
    }
    int		nRange		= (nSizeAdd + 1) / 2;
    bool	bDecBorder	= (nSizeAdd % 2) != 0;
    for(int i = x - nRange; i <= x + nRange; i++)
    {
        for(int j = y - nRange; j <= y + nRange; j++)
        {
            if (IsValidPoint(i, j))
            {
                if (bDecBorder && (i == x - nRange || i == x + nRange || j == y - nRange || j == y + nRange))
                {
                    m_pMapData->IncRole(i, j);
                }
                else
                {
                    m_pMapData->IncRole(i, j, MAPFLAG_PER_ROLE);
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CGameMap::DecRole(int x, int y, int nSizeAdd /*= 0*/)
{
    CHECK(nSizeAdd >= 0);
    if (nSizeAdd > MAX_MONSTERSIZEADD)
    {
        nSizeAdd = MAX_MONSTERSIZEADD;
    }
    int		nRange		= (nSizeAdd + 1) / 2;
    bool	bDecBorder	= (nSizeAdd % 2) != 0;
    for(int i = x - nRange; i <= x + nRange; i++)
    {
        for(int j = y - nRange; j <= y + nRange; j++)
        {
            if (IsValidPoint(i, j))
            {
                if (bDecBorder && (i == x - nRange || i == x + nRange || j == y - nRange || j == y + nRange))
                {
                    m_pMapData->DecRole(i, j);
                }
                else
                {
                    m_pMapData->DecRole(i, j, MAPFLAG_PER_ROLE);
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CGameMap::LeaveMap(CUser* pUser)
{
    CHECK(pUser->GetPosX() >= 0 && pUser->GetPosX() < m_pMapData->GetMapWidth());
    CHECK(pUser->GetPosY() >= 0 && pUser->GetPosY() < m_pMapData->GetMapHeight());
    if (pUser->IsBlockNpc())
    {
        DecRole(pUser->GetPosX(), pUser->GetPosY());
    }
    BlockSet(pUser->GetPosX(), pUser->GetPosY()).QueryUserSet()->DelObj(pUser->GetID());
}

//////////////////////////////////////////////////////////////////////
void CGameMap::LeaveMap(INpc* pNpc)
{
    CHECK(pNpc->GetPosX() >= 0 && pNpc->GetPosX() < m_pMapData->GetMapWidth());
    CHECK(pNpc->GetPosY() >= 0 && pNpc->GetPosY() < m_pMapData->GetMapHeight());
    if (pNpc->IsBlockNpc())
    {
        DecRole(pNpc->GetPosX(), pNpc->GetPosY(), pNpc->GetSizeAdd());
    }
    BlockSet(pNpc->GetPosX(), pNpc->GetPosY()).QueryNpcSet()->DelObj(pNpc->GetID());
}

//////////////////////////////////////////////////////////////////////
void CGameMap::CollectRole(vector<IRole*>* psetRole, const POINT pos, int nRange)
{
    //	CHECK(psetMapThing);
    int	nSize		= nRange * 2 + 1;
    int	nBufSize	= nSize * nSize;
    psetRole->resize(nBufSize);
    int x, y;
    FOR_9_BLOCKS(this, pos.x, pos.y)
    {
        for(int i = 0; i < BlockByIndex(x, y).QueryNpcSet()->GetAmount(); i++)
        {
            INpc* pNpc = BlockByIndex(x, y).QueryNpcSet()->GetObjByIndex(i);
            if (pNpc && Distance(pNpc->GetPosX(), pNpc->GetPosY(), pos.x, pos.y) <= nRange)
            {
                int idx = POS2INDEX(pNpc->GetPosX() - (pos.x - nRange), pNpc->GetPosY() - (pos.y - nRange), nSize, nSize);
                IF_OK(idx >= 0 && idx < nBufSize)
                (*psetRole)[idx]	= Cast<IRole>(pNpc);
            }
        }
        for(int i = 0; i < BlockByIndex(x, y).QueryUserSet()->GetAmount(); i++)
        {
            CUser* pUser = BlockByIndex(x, y).QueryUserSet()->GetObjByIndex(i);
            if (pUser && Distance(pUser->GetPosX(), pUser->GetPosY(), pos.x, pos.y) <= nRange)
            {
                int idx = POS2INDEX(pUser->GetPosX() - (pos.x - nRange), pUser->GetPosY() - (pos.y - nRange), nSize, nSize);
                IF_OK(idx >= 0 && idx < nBufSize)
                (*psetRole)[idx]	= Cast<IRole>(pUser);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CGameMap::CallWakeMonster(int nPosX, int nPosY)
{
    // 唤醒休眠NPC
    int x, y;
    FOR_9_BLOCKS(this, nPosX, nPosY)
    {
        if (BlockByIndex(x, y).IsDormancy())
        {
            BlockByIndex(x, y).SetDormancy(false);
            for(int i = 0; i < BlockByIndex(x, y).QueryNpcSet()->GetAmount(); i++)
            {
                CNpcPtr pNpc = BlockByIndex(x, y).QueryNpcSet()->GetObjByIndex(i);
                if (pNpc && Cast<CNpc>(pNpc))
                {
                    Cast<CNpc>(pNpc)->WakeUp();
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::ChangeMapDoc(OBJID idDoc)
{
    ASSERT(!"no use");
    return true;
    IMapData* pMapData	= IMapData::CreateNew(idDoc, MAPDATA_VERSION);	// VVVVVVVVVVVVVVVVVVVVVVVVVV
    if (!pMapData)
    {
        LOGERROR("地图文件[%d]加载失败!", idDoc);
        return false;
    }
    CHECKF(pMapData->GetMapWidth() == m_pMapData->GetMapWidth());
    CHECKF(pMapData->GetMapHeight() == m_pMapData->GetMapHeight());
    /*
    	// 装载地图
    	IF_OK(nWidth > GetWidthOfBlock())
    		m_setBlock.resize(GetWidthOfBlock());
    	for(int i = 0; i < m_setBlock.size(); i++)
    	{
    		IF_OK(nHeight > GetHeightOfBlock())
    			m_setBlock[i].resize(GetHeightOfBlock());
    		for(int j = 0; j < m_setBlock[i].size(); j++)
    			m_setBlock[i][j].Create();
    	}
    */
    // copy role count
    for(int i = 0; i < m_pMapData->GetMapWidth(); i++)
    {
        for(int j = 0; j < m_pMapData->GetMapHeight(); j++)
        {
            int nCount = m_pMapData->GetRoleAmount(i, j);
            if (nCount)
            {
                pMapData->IncRole(i, j, nCount);
            }
        }
    }
    //	m_pData->SetInt(GAMEMAPDATA_MAPDOC, idDoc);
    //	m_pData->Update();
    IF_OK(m_pMapData)
    m_pMapData->Release();
    m_pMapData	= pMapData;		// AAAAAAAAAAAAAAAAA
    return true;
}
