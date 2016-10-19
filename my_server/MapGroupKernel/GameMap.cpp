
//**********************************************************
// 代码编辑器
//**********************************************************

// GameMap.cpp: implementation of the CGameMap class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning(disable: 4786)

#include "GameObj.h"
#include "GameMap.h"
#include <algorithm>
#include "AllMsg.h"
#include "GameData.h"
#include "I_MapData.h"
#include "MapGroup.h"
#include "BaseFunc.h"
#include "WeatherRegion.h"
#include "RoleManager.h"
#include "Array.h"

//////////////////////////////////////////////////////////////////////
#define		MAPDATA_VERSION		((LPCTSTR)true)					// DDVersion
const DWORD	DEFAULT_LIGHTRGB		= 0xFFFFFFFF;

//////////////////////////////////////////////////////////////////////
MYHEAP_IMPLEMENTATION(CGameMap, s_heap)
char	szMapTable[]		= _TBL_MAP;
char	szRegionTable[]		= _TBL_REGION;
char	szPortalTable[]		= _TBL_PORTAL;
char	szPasswayTable[]	= _TBL_PASSWAY;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGameMap::CGameMap()
{
    SetObjType(OBJ_MAP);
    m_idProcess = PROCESS_NONE;
    m_pData		= NULL;
    m_pMapData	= NULL;
    m_setRegion	= NULL;
    m_setWeather	= NULL;
    m_dwLightRGB	= DEFAULT_LIGHTRGB;
    m_nStatus	= STATUS_NONE;
}

//////////////////////////////////////////////////////////////////////
CGameMap::~CGameMap()
{
    if (m_pData)
    {
        if (IsDynaMap())
        {
            m_pData->Update();
        }
        m_pData->Release();
    }
    if (m_pMapData)
    {
        m_pMapData->Release();
    }
    if (m_setRegion)
    {
        m_setRegion->Release();
    }
    if (m_setWeather)
    {
        m_setWeather->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::Create(PROCESS_ID idProcess, IRecordset* pRes)
{
    m_idProcess = idProcess;
    m_pData	= CGameMapData::CreateNew();
    if (!m_pData || !m_pData->Create(pRes))
    {
        return false;
    }
    //*
    LOGMSG("加载地图文件[%d]...", m_pData->GetInt(GAMEMAPDATA_MAPDOC));
    m_pMapData	= IMapData::CreateNew(m_pData->GetInt(GAMEMAPDATA_MAPDOC), MAPDATA_VERSION);
    if (!m_pMapData)
    {
        LOGERROR("地图文件[%d]加载失败！", m_pData->GetInt(GAMEMAPDATA_MAPDOC));
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
    //*/
    m_setRegion = CRegionSet::CreateNew(true);
    CHECKF(m_setRegion);
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT * FROM %s WHERE mapid=%u", szRegionTable, m_pData->GetID());
    if (!m_setRegion->Create(szSQL, Database()))
    {
        return false;
    }
    m_setWeather	= CWeatherSet::CreateNew(true);
    CHECKF(m_setWeather);
    for(int i = 0; i < m_setRegion->GetAmount(); i++)
    {
        CRegionData*	pData = m_setRegion->GetObjByIndex(i);
        if (pData && pData->GetInt(REGIONDATA_TYPE) == REGION_WEATHER)
        {
            CWeatherRegion* pWeather = CWeatherRegion::CreateNew();
            CHECKF(pWeather);
            IF_OK(pWeather->Create(pData, idProcess))
            m_setWeather->AddObj(pWeather);
            else
            {
                pWeather->ReleaseByOwner();
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CGameMap::OnTimer(DWORD tCurr)
{
    for(int i = 0; i < m_setWeather->GetAmount(); i++)
    {
        CWeatherRegion* pWeather = m_setWeather->GetObjByIndex(i);
        if (pWeather)
        {
            pWeather->QueryWeather()->OnTimer();
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::QueryObjInPos(int nPosX, int nPosY, OBJID idObjType, void** ppObj)
{
    *ppObj = NULL;
    CHECKF(idObjType != ID_NONE);
    IThingSet* pSet = QueryBlock(nPosX, nPosY).QuerySet();
    for(int i = 0; i < pSet->GetAmount(); i++)
    {
        IMapThing* pTarget = pSet->GetObjByIndex(i);
        void* pTemp;
        if (pTarget && pTarget->QueryObj(idObjType, &pTemp) && pTarget->GetPosX() == nPosX && pTarget->GetPosY() == nPosY)
        {
            *ppObj = pTemp;
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::QueryObj(int nCenterX, int nCenterY, OBJID idObjType, OBJID idObj, void** ppObj)
{
    *ppObj = NULL;
    CHECKF(idObjType != ID_NONE);
    void* pTemp = NULL;
    int x, y, z;
    FOR_9_BLOCKTHINGS(this, nCenterX, nCenterY)
    {
        IMapThing* pTarget = QueryThingByIndex(x, y, z);
        if (pTarget && pTarget->QueryObj(idObjType, &pTemp))
        {
            if (idObj != ID_NONE)
            {
                if (pTarget->GetID() == idObj)
                {
                    *ppObj = pTemp;
                    return true;
                }
            }
            else
            {
                *ppObj = pTemp;
                return true;
            }
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
IRole* CGameMap::QueryRole(int nCenterX, int nCenterY, OBJID idObj)
{
    //CHECKF(idObj != ID_NONE);
    IRole*	pRole	= NULL;
    int x, y, z;
    FOR_9_BLOCKTHINGS(this, nCenterX, nCenterY)
    {
        IMapThing* pTarget = QueryThingByIndex(x, y, z);
        if (pTarget && pTarget->QueryRole(IPP_OF(pRole)))
        {
            if (idObj == ID_NONE
                    && pTarget->GetPosX() == nCenterX
                    && pTarget->GetPosY() == nCenterY)
            {
                return pRole;
            }
            if (pTarget->GetID() == idObj)
            {
                return pRole;
            }
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
IRole* CGameMap::QueryRoleByPos(int nPosX, int nPosY)
{
    if (!IsValidPoint(nPosX, nPosY))
    {
        return NULL;
    }
    IRole*	pRole	= NULL;
    IThingSet* pSet = QueryBlock(nPosX, nPosY).QuerySet();
    for(int i = 0; i < pSet->GetAmount(); i++)
    {
        IMapThing* pTarget = pSet->GetObjByIndex(i);
        if (pTarget && pTarget->QueryRole(IPP_OF(pRole))
                && pTarget->GetPosX() == nPosX && pTarget->GetPosY() == nPosY)
        {
            return pRole;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
void CGameMap::SendBlockInfo(IRole* pRole)
{
    CHECK(pRole);
    // 优化
    void* pTemp = NULL;
    if (pRole->QueryObj(OBJ_USER, IPP_OF(pTemp)))
    {
        int x, y;
        FOR_9_BLOCKS(this, pRole->GetPosX(), pRole->GetPosY())
        {
            IThingSet* pSet = BlockByIndex(x, y).QuerySet();
            for(int i = 0; i < pSet->GetAmount(); i++)
            {
                IMapThing* pMapThing = pSet->GetObjByIndex(i);
                if (pMapThing && !(pMapThing->GetObjType() == OBJ_USER && pMapThing->GetID() == pRole->GetID()) )
                {
                    if (!( IsNewbieMap() && pMapThing->GetObjType() == OBJ_USER ))
                    {
                        pMapThing->SendShow(pRole);
                    }
                }
            }
        }
    }
    else // if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pTemp)))
    {
        CUser* pUser = NULL;
        int x, y;
        FOR_9_BLOCKS(this, pRole->GetPosX(), pRole->GetPosY())
        {
            IThingSet* pSet = BlockByIndex(x, y).QuerySet();
            for(int i = 0; i < pSet->GetAmount(); i++)
            {
                IMapThing* pMapThing = pSet->GetObjByIndex(i);
                if (pMapThing && pMapThing->GetObjType() == OBJ_USER)		//@ 只发USER的信息给NPC服务器
                {
                    pMapThing->SendShow(pRole);
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CGameMap::BroadcastBlockMsg(IMapThing* pThing, CNetMsg* pMsg, bool bSendSelf /*= false*/)
{
    ASSERT(pMsg);
    //? 这个函数的逻辑有点乱：(
    void*	pObj	= NULL;
    IRole*	pRole	= NULL;
    CMonster* pMonster = NULL;
    //@不发非玩家消息到NPC服务器	bool	bSendToNpc = (!pThing->QueryObj(OBJ_MONSTER, IPP_OF(pObj)) || bSendSelf);
    bool	bSendToNpc = (pThing->QueryObj(OBJ_USER, IPP_OF(pObj))
                          || (pThing->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && (pMonster->IsCallPet() || pMonster->IsEudemon()))
                          || bSendSelf);
    // newbie map
    if (IsNewbieMap() && bSendSelf)
    {
        if (pThing->QueryRole(IPP_OF(pRole)))
        {
            pRole->SendMsg(pMsg);
        }
        return ;
    }
    int x, y, z;
    FOR_9_BLOCKTHINGS(this, pThing->GetPosX(), pThing->GetPosY())
    {
        DEBUG_TRY	// VVVVVVVVVVVVVVVVVVVV
        IMapThing* pTarget = QueryThingByIndex(x, y, z);
        if (pTarget && pTarget->QueryRole(IPP_OF(pRole)) && pRole->GetDistance(pThing) <= CELLS_PER_VIEW)
        {
            if (bSendSelf || !(pTarget->GetObjType() == pThing->GetObjType() && pTarget->GetID() == pThing->GetID()))
            {
                // monster
                if (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && bSendToNpc)
                {
                    if (pMonster->IsCallPet() || pMonster->IsEudemon())
                    {
                    }
                    else
                    {
                        pRole->SendMsg(pMsg);
                        bSendToNpc = false;
                    }
                }
                // user
                CUser* pUser;
                if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
                {
                    if (!pUser->IsAgent())
                    {
                        // player
                        pRole->SendMsg(pMsg);
                    }
                    else if (bSendToNpc)
                    {
                        // agent
                        pRole->SendMsg(pMsg);
                        bSendToNpc = false;
                    }
                }
            }
        }
        DEBUG_CATCH("BroadcastBlockMsg")		// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    }
}

//////////////////////////////////////////////////////////////////////
void CGameMap::BroadcastBlockMsg(int nPosX, int nPosY, CNetMsg* pMsg)
{
    ASSERT(pMsg);
    bool bSendNpcAlready = false;
    void*	pObj	= NULL;
    IRole*	pRole	= NULL;
    int x, y, z;
    FOR_9_BLOCKTHINGS(this, nPosX, nPosY)
    {
        DEBUG_TRY	// VVVVVVVVVVVVVVVVVVVV
        IMapThing* pTarget = QueryThingByIndex(x, y, z);
        if (pTarget && pTarget->QueryRole(IPP_OF(pRole)) && pRole->GetDistance(nPosX, nPosY) <= CELLS_PER_VIEW)
        {
            // monster
            CMonster* pMonster = NULL;
            if (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && !bSendNpcAlready)
            {
                if (pMonster->IsCallPet() || pMonster->IsEudemon())
                {
                }
                else
                {
                    pRole->SendMsg(pMsg);
                    bSendNpcAlready = true;
                }
            }
            // user
            CUser* pUser;
            if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
            {
                if (!pUser->IsAgent())
                {
                    // player
                    pRole->SendMsg(pMsg);
                }
                else if (!bSendNpcAlready)
                {
                    // agent
                    pRole->SendMsg(pMsg);
                    bSendNpcAlready = true;
                }
            }
        }
        DEBUG_CATCH("BroadcastBlockMsg")		// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    }
}

//////////////////////////////////////////////////////////////////////
// role
//////////////////////////////////////////////////////////////////////
void CGameMap::EnterRoom(IMapThing* pThing, BOOL bWithBlock/*=false*/)
{
    CHECK(pThing);
    QueryBlock(pThing->GetPosX(), pThing->GetPosY()).QuerySet()->AddObj(pThing);
    if (bWithBlock)
    {
        m_pMapData->IncRole(pThing->GetPosX(), pThing->GetPosY());
    }
}

//////////////////////////////////////////////////////////////////////
void CGameMap::LeaveRoom(IMapThing* pThing, BOOL bWithBlock/*=false*/)
{
    CHECK(pThing);
    QueryBlock(pThing->GetPosX(), pThing->GetPosY()).QuerySet()->DelObj(pThing->GetID(), pThing->GetObjType());
    if (bWithBlock)
    {
        m_pMapData->DecRole(pThing->GetPosX(), pThing->GetPosY());
    }
}

//////////////////////////////////////////////////////////////////////
void CGameMap::MoveTo(IRole* pRole, int nNewPosX, int nNewPosY, BOOL bLeaveBlock/*=false*/, BOOL bEnterBlock/*=false*/)		// 移动到相邻的BLOCK
{
    CHECK(pRole);
    if (bLeaveBlock)
    {
        m_pMapData->DecRole(pRole->GetPosX(), pRole->GetPosY());
    }
    if (bEnterBlock)
    {
        m_pMapData->IncRole(nNewPosX, nNewPosY);
    }
    if (Block(pRole->GetPosX()) == Block(nNewPosX) && Block(pRole->GetPosY()) == Block(nNewPosY))
    {
        return;
    }
    IMapThing* pThing = pRole->QueryMapThing();
    CHECK(pThing);
    QueryBlock(pThing->GetPosX(), pThing->GetPosY()).QuerySet()->DelObj(pThing->GetID(), pThing->GetObjType());
    QueryBlock(nNewPosX, nNewPosY).QuerySet()->AddObj(pThing);
}

//////////////////////////////////////////////////////////////////////
// region info
//////////////////////////////////////////////////////////////////////
void CGameMap::SendRegionInfo(CUser* pUser)
{
    for(int i = 0; i < m_setWeather->GetAmount(); i++)
    {
        CWeatherRegion* pRegion = m_setWeather->GetObjByIndex(i);
        if (pRegion && IsInRegion(pRegion->QueryRegion(), pUser->GetMapID(), pUser->GetPosX(), pUser->GetPosY()))
        {
            pRegion->QueryWeather()->SendWeather(pUser);
            pUser->SetWeatherID(pRegion->GetID());
            return;
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CGameMap::ClearRegionInfo(CUser* pUser)
{
    if (pUser->GetWeatherID() != ID_NONE)
    {
        CWeather::SendNoWeather(pUser);
        pUser->SetWeatherID(ID_NONE);
    }
}

//////////////////////////////////////////////////////////////////////
void CGameMap::ChangeRegion(CUser* pUser, int nNewPosX, int nNewPosY)
{
    for(int i = 0; i < m_setWeather->GetAmount(); i++)
    {
        CWeatherRegion* pRegion = m_setWeather->GetObjByIndex(i);
        if (pRegion && IsInRegion(pRegion->QueryRegion(), pUser->GetMapID(), nNewPosX, nNewPosY))
        {
            // found weather region
            if (pUser->GetWeatherID() != pRegion->GetID())
            {
                pRegion->QueryWeather()->SendWeather(pUser);
                pUser->SetWeatherID(pRegion->GetID());
            }
            return;
        }
    }
    // not in any weather region
    if (pUser->GetWeatherID() != ID_NONE)
    {
        CWeather::SendNoWeather(pUser);
        pUser->SetWeatherID(ID_NONE);
    }
}

//////////////////////////////////////////////////////////////////////
// const
//////////////////////////////////////////////////////////////////////
bool CGameMap::IsStandEnable(int nPosX, int nPosY)
{
    if (!IsValidPoint(nPosX, nPosY))
    {
        return false;
    }
    return (m_pMapData->GetFloorMask(nPosX, nPosY) == 0);
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::IsMoveEnable(int x, int y)
{
    if (!IsValidPoint(x, y))
    {
        return false;
    }
    if (m_pMapData->GetFloorMask(x, y) != 0)
    {
        return false;
    }
    if (m_pMapData->GetRoleAmount(x, y) > 0)
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::IsMoveEnable(int xSour, int ySour, int x, int y, int nClimbCap)
{
    if (!IsValidPoint(x, y))
    {
        return false;
    }
    if (m_pMapData->GetFloorMask(x, y) != 0)
    {
        return false;
    }
    if (m_pMapData->GetRoleAmount(x, y) > 0)
    {
        return false;
    }
    if (m_pMapData->GetFloorAlt(x, y) - m_pMapData->GetFloorAlt(xSour, ySour) > nClimbCap)
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::IsAltEnable(const POINT& posSour, const POINT& pos, int nAltDiff)
{
    if (!IsValidPoint(pos))
    {
        return false;
    }
    if (abs(m_pMapData->GetFloorAlt(pos.x, pos.y) - m_pMapData->GetFloorAlt(posSour.x, posSour.y)) >= nAltDiff)
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::IsAltOver(const POINT& pos, int nAlt)
{
    if (!IsValidPoint(pos))
    {
        return false;
    }
    if (m_pMapData->GetFloorAlt(pos.x, pos.y) > nAlt)
    {
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
CRegionData* CGameMap::QueryRegion(int nRegionType, int x, int y)
{
    for(int i = 0; i < QueryRegionSet()->GetAmount(); i++)
    {
        CRegionData* pData = QueryRegionSet()->GetObjByIndex(i);
        if (pData && pData->GetInt(REGIONDATA_TYPE) == nRegionType && IsInRegion(pData, GetID(), x, y))
        {
            return pData;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::IsInRegionType(int nRegionType, int x, int y)
{
    for(int i = 0; i < QueryRegionSet()->GetAmount(); i++)
    {
        CRegionData* pData = QueryRegionSet()->GetObjByIndex(i);
        if (pData && pData->GetInt(REGIONDATA_TYPE) == nRegionType && IsInRegion(pData, GetID(), x, y))
        {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::IsInRegion(CRegionData* pData, OBJID idMap, int x, int y)
{
    if (idMap == pData->GetInt(REGIONDATA_MAPID)
            && x >= pData->GetInt(REGIONDATA_BOUND_X) && x < pData->GetInt(REGIONDATA_BOUND_X) + pData->GetInt(REGIONDATA_BOUND_CX)
            && y >= pData->GetInt(REGIONDATA_BOUND_Y) && y < pData->GetInt(REGIONDATA_BOUND_Y) + pData->GetInt(REGIONDATA_BOUND_CY))
    {
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::BroadcastDance(CUser* pUser, OBJID idEmotion)
{
    CRegionData* pData = QueryRegion(REGION_DANCE, pUser->GetPosX(), pUser->GetPosY());
    if (pData && pData->GetInt(REGIONDATA_DATA0) == pData->GetID() && idEmotion > 1)	// leader
    {
        CMsgAction	msg;
        IF_OK(msg.Create(pUser->GetID(), pUser->GetPosX(), pUser->GetPosY(), pUser->GetDir(), actionEmotion, idEmotion))
        pUser->BroadcastRoomMsg(&msg, INCLUDE_SELF);
        CRegionData* pFollow = QueryRegionSet()->GetObj(pData->GetInt(REGIONDATA_DATA1));
        CUserManager::Iterator pTarget = UserManager()->NewEnum();
        while(pTarget.Next())
        {
            if (IsInRegion(pFollow, pTarget->GetMapID(), pTarget->GetPosX(), pTarget->GetPosY()) && pTarget->GetSex() == pUser->GetSex())
            {
                pTarget->SetPose(idEmotion);
                CMsgAction	msg;
                IF_OK(msg.Create(pTarget->GetID(), pTarget->GetPosX(), pTarget->GetPosY(), pTarget->GetDir(), actionEmotion, idEmotion))
                pTarget->BroadcastRoomMsg(&msg, INCLUDE_SELF);
#ifdef _DEBUG
                pTarget->SendSysMsg("群舞：%d", idEmotion);
#endif
            }
        }
        /*
        		CMsgAction	msg;
        		IF_OK(msg.Create(pUser->GetID(), pUser->GetPosX(), pUser->GetPosY(), pUser->GetDir(), actionMusic, pData->GetInt(REGIONDATA_DATA2)))
        			pUser->BroadcastRoomMsg(&msg, INCLUDE_SELF);
        */
        return true;
    }
    if (pData && pData->GetInt(REGIONDATA_DATA0) != pData->GetID())	// follow
    {
        CRegionData* pLeaderData = QueryRegionSet()->GetObj(pData->GetInt(REGIONDATA_DATA0));
        IF_OK(pLeaderData)
        {
            CUser* pLeader;
            if (QueryObjInPos(pLeaderData->GetInt(REGIONDATA_BOUND_X), pLeaderData->GetInt(REGIONDATA_BOUND_Y), OBJ_USER, IPP_OF(pLeader))
                    && pLeader->GetSex() == pUser->GetSex() && pLeader->GetPose() > 1 && pLeader->GetPose() < 10)
            {
                idEmotion = pLeader->GetPose();
#ifdef _DEBUG
                pUser->SendSysMsg("跟舞：%d", idEmotion);
#endif
            }
        }
    }
    pUser->SetPose(idEmotion);
    CMsgAction	msg;
    IF_OK(msg.Create(pUser->GetID(), pUser->GetPosX(), pUser->GetPosY(), pUser->GetDir(), actionEmotion, idEmotion))
    pUser->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    return true;
}

//////////////////////////////////////////////////////////////////////
// application
//////////////////////////////////////////////////////////////////////
void CGameMap::CollectMapThing(MAPTHING_SET& setMapThing, const POINT pos, int nRange, OBJID idObjTypeUnion)
{
    //	CHECK(psetMapThing);
    int	nSize		= nRange * 2 + 1;
    int	nBufSize	= nSize * nSize;
    setMapThing.resize(nBufSize);
    int x, y, z;
    FOR_9_BLOCKTHINGS(this, pos.x, pos.y)
    {
        IMapThing* pThing = QueryThingByIndex(x, y, z);
        if (pThing && ::IsObjType(pThing->GetObjType(), idObjTypeUnion)
                && Distance(pThing->GetPosX(), pThing->GetPosY(), pos.x, pos.y) <= nRange)
        {
            int idx = POS2INDEX(pThing->GetPosX() - (pos.x - nRange), pThing->GetPosY() - (pos.y - nRange), nSize, nSize);
            IF_OK(idx >= 0 && idx < nBufSize)
            setMapThing[idx]	= pThing;
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::FindDropItemCell(int nRange, POINT* pPos)		// pos: in/out
{
    int	nSize		= nRange * 2 + 1;
    int	nBufSize	= nSize * nSize;
    MAPTHING_SET	setItem;
    CollectMapThing(setItem, *pPos, nRange, OBJ_MAPITEM);
    // 先随机
    int		nIndex = ::RandGet(nBufSize);
    if (!setItem[nIndex])
    {
        POINT	posTest;
        int		nLeft	= pPos->x - nRange;
        int		nTop	= pPos->y - nRange;
        posTest.x	= nLeft + INDEX2X(nIndex, nSize, nSize);
        posTest.y	= nTop + INDEX2Y(nIndex, nSize, nSize);
        if (IsLayItemEnable(posTest.x, posTest.y))
        {
            *pPos = posTest;
            return true;
        }
    }
    // 顺序找
    int		nMinRange = nRange + 1;
    int		ret	= false;
    POINT	posFree;
    for(int i = __max(pPos->x - nRange, 0); i <= pPos->x + nRange && i < GetWidth(); i++)
    {
        for(int j = __max(pPos->y - nRange, 0); j <= pPos->y + nRange && j < GetHeight(); j++)
        {
            int idx = POS2INDEX(i - (pPos->x - nRange), j - (pPos->y - nRange), nSize, nSize);
            IF_OK(idx >= 0 && idx < nBufSize)
            if (setItem[idx])
            {
                continue;
            }
            if (IsLayItemEnable(i, j))
            {
                int	nDistance = Distance(i, j, pPos->x, pPos->y);
                if (nDistance < nMinRange)
                {
                    // 找到了!
                    nMinRange = nDistance;
                    posFree.x	= i;
                    posFree.y	= j;
                    ret = true;
                }
            }
        }
    }
    if (ret)
    {
        *pPos = posFree;
        return true;
    }
    else
    {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::GetPassageMap(OBJID* pidMap, POINT* pposTarget, const POINT& pos)
{
    CHECKF(pos.x >= 0 && pos.x < GetWidth() && pos.y >= 0 && pos.y < GetHeight());
    int	idxPassage = m_pMapData->GetPassage(pos.x, pos.y);
    if (idxPassage == PASSAGE_NONE)
    {
        return false;
    }
    if (IsDynaMap())
    {
        *pidMap				= m_pData->GetInt(GAMEMAPDATA_LINKMAP);
        pposTarget->x		= m_pData->GetInt(GAMEMAPDATA_LINK_X);
        pposTarget->y		= m_pData->GetInt(GAMEMAPDATA_LINK_Y);
        return true;
    }
    OBJID	idTargetMap	= ID_NONE;
    int		idxPortal	= 0;
    if (!MapManager()->FindPassway(&idTargetMap, &idxPortal, GetID(), idxPassage))
    {
        LOGWARNING("没有找到[%d]地图的[%d]号出口点! %s 数据错误。", GetID(), idxPassage, _TBL_PASSWAY);
        return false;
    }
    POINT	posNew;
    if (!MapManager()->FindPortal(&posNew, idTargetMap, idxPortal))
    {
        LOGWARNING("没有找到地图入口点[%u][%d], 使用缺省入口点! %s 数据错误。", idTargetMap, idxPortal, _TBL_PORTAL);
        CGameMapData* pTargetMap = MapManager()->QuerySystemMapSet()->GetObj(idTargetMap);
        IF_NOT(pTargetMap)
        {
            LOGERROR("没有找到切屏地图[%u]! %s 数据错误。", idTargetMap, _TBL_PORTAL);
            return false;
        }
        posNew.x	= pTargetMap->GetInt(GAMEMAPDATA(GAMEMAPDATA_PORTAL0_X));
        posNew.y	= pTargetMap->GetInt(GAMEMAPDATA(GAMEMAPDATA_PORTAL0_Y));
    }
    *pidMap	= idTargetMap;
    *pposTarget	= posNew;
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::SetStatus(int nStatus, bool flag)
{
    int	nOldStatus = m_nStatus;
    if (flag)
    {
        m_nStatus	|= nStatus;
    }
    else
    {
        m_nStatus	&= ~nStatus;
    }
    if (nOldStatus != m_nStatus)
    {
        // synchro npc server
        CMsgAction	msg;
        IF_OK(msg.Create(GetID(), 1, 1, 1, actionMapStatus, m_nStatus))
        MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msg);
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::GetRebornMap(OBJID* pidMap, POINT* pposTarget)
{
    OBJID	idTargetMap	= m_pData->GetInt(GAMEMAPDATA_REBORN_MAPID);
    int		idxPortal	= m_pData->GetInt(GAMEMAPDATA_REBORN_PORTAL);
    CHECKF(idTargetMap != ID_NONE);
    /*
    if (!MapManager()->FindPortal(&posNew, idTargetMap, idxPortal))
    {
    	LOGWARNING("没有找到地图入口点[%u][%d], 使用缺省入口点.", idTargetMap, idxPortal);
    }
    */
    CGameMapData* pTargetMap = MapManager()->QuerySystemMapSet()->GetObj(idTargetMap);
    IF_NOT(pTargetMap)
    {
        LOGERROR("没有找到切屏地图[%u] !", idTargetMap);
        return false;
    }
    POINT	posNew;
    posNew.x	= pTargetMap->GetInt(GAMEMAPDATA(GAMEMAPDATA_PORTAL0_X));
    posNew.y	= pTargetMap->GetInt(GAMEMAPDATA(GAMEMAPDATA_PORTAL0_Y));
    IF_NOT (posNew.x != 0 || posNew.y != 0)
    return false;
    *pidMap	= idTargetMap;
    *pposTarget	= posNew;
    return true;
}

//////////////////////////////////////////////////////////////////////
void CGameMap::SendMapInfo(CUser* pUser)
{
    IF_NOT (pUser)
    return;
    CMsgMapInfo msg;
    IF_OK (msg.Create(this))
    pUser->SendMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
void CGameMap::SetSynID(OBJID idSyn, bool bWithAllNpc)
{
    if (!IsDynaMap())
    {
        m_pData->SetInt(GAMEMAPDATA_OWNERTYPE, OWNER_SYN);
    }
    if (m_pData->GetInt(GAMEMAPDATA_OWNERTYPE) == OWNER_SYN)
    {
        m_pData->SetInt(GAMEMAPDATA_OWNERID_, idSyn);
        if (IsDynaMap())
        {
            m_pData->Update();
        }
    }
    if (bWithAllNpc)
    {
        for(IRoleSet::Iter_t i = RoleManager()->QuerySet()->Begin(); i != RoleManager()->QuerySet()->End(); i++)
        {
            IRole* pRole = RoleManager()->QuerySet()->GetObjByIter(i);
            CNpc* pNpc;
            if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetInt(NPCDATA_OWNERTYPE) == OWNER_SYN)
            {
                pNpc->SetSynOwnerID(idSyn, true);    // true: with link map
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CGameMap::SetUserID(OBJID idUser, bool bWithAllNpc)
{
    if (m_pData->GetInt(GAMEMAPDATA_OWNERTYPE) == OWNER_USER)
    {
        m_pData->SetInt(GAMEMAPDATA_OWNERID_, idUser);
        if (IsDynaMap())
        {
            m_pData->Update();
        }
    }
    if (bWithAllNpc)
    {
        for(IRoleSet::Iter_t i = RoleManager()->QuerySet()->Begin(); i != RoleManager()->QuerySet()->End(); i++)
        {
            IRole* pRole = RoleManager()->QuerySet()->GetObjByIter(i);
            CNpc* pNpc;
            if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetInt(NPCDATA_OWNERTYPE) == OWNER_SYN)
            {
                pNpc->SetUserOwnerID(idUser, true);    // true: with link map
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
OBJID CGameMap::CreateDynaMap(PROCESS_ID idProcess, const NewMapInfo* pInfo)
{
    m_idProcess		= idProcess;
    CHECKF(pInfo);
    m_pData	= CGameMapData::CreateNew();
    if (!m_pData || !m_pData->Create(GameDataDefault()->GetDynaMap(), ID_NONE))
    {
        return false;
    }
    m_pData->SetStr(GAMEMAPDATA_NAME, pInfo->szName, _MAX_NAMESIZE);
    m_pData->SetInt(GAMEMAPDATA_OWNERTYPE,		pInfo->nOwnerType);
    m_pData->SetInt(GAMEMAPDATA_OWNERID_,		pInfo->idOwner);
    m_pData->SetInt(GAMEMAPDATA_MAPDOC,			pInfo->nMapDoc);
    m_pData->SetInt(GAMEMAPDATA_TYPE,			pInfo->nType);
    m_pData->SetInt(GAMEMAPDATA_MAPGROUP,		pInfo->nMapGroup);
    m_pData->SetInt(GAMEMAPDATA_LINKMAP,		pInfo->idLikeMap);
    m_pData->SetInt(GAMEMAPDATA_LINK_X,			pInfo->nLinkX);
    m_pData->SetInt(GAMEMAPDATA_LINK_Y,			pInfo->nLinkY);
    m_pData->SetInt(GAMEMAPDATA_PORTAL0_X,		pInfo->nPortalX);
    m_pData->SetInt(GAMEMAPDATA_PORTAL0_Y,		pInfo->nPortalY);
    m_pData->SetInt(GAMEMAPDATA_REBORN_MAPID,	pInfo->idRebornMap);
    m_pData->SetInt(GAMEMAPDATA_REBORN_PORTAL,	pInfo->nRebornPortal);
    m_pData->SetInt(GAMEMAPDATA_RESOURCE_LEV,	pInfo->nResLev);
    OBJID idMap = m_pData->InsertRecord();
    CHECKF(idMap != ID_NONE);
    //*
    LOGMSG("加载动态地图文件[%d]...", m_pData->GetInt(GAMEMAPDATA_MAPDOC));
    m_pMapData	= IMapData::CreateNew(m_pData->GetInt(GAMEMAPDATA_MAPDOC), MAPDATA_VERSION);
    if (!m_pMapData)
    {
        LOGERROR("动态地图文件[%d]加载失败！", m_pData->GetInt(GAMEMAPDATA_MAPDOC));
        return ID_NONE;
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
    //*/
    m_setRegion = CRegionSet::CreateNew(true);
    CHECKF(m_setRegion);
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT * FROM %s WHERE mapid=%u", szRegionTable, m_pData->GetID());
    IF_NOT(m_setRegion->Create(szSQL, Database()))
    return ID_NONE;
    m_setWeather	= CWeatherSet::CreateNew(true);
    CHECKF(m_setWeather);
    for(int i = 0; i < m_setRegion->GetAmount(); i++)
    {
        CRegionData*	pData = m_setRegion->GetObjByIndex(i);
        if (pData && pData->GetInt(REGIONDATA_TYPE) == REGION_WEATHER)
        {
            CWeatherRegion* pWeather = CWeatherRegion::CreateNew();
            CHECKF(pWeather);
            IF_OK(pWeather->Create(pData, idProcess))
            m_setWeather->AddObj(pWeather);
            else
            {
                pWeather->ReleaseByOwner();
            }
        }
    }
    return idMap;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::EraseMap()
{
    CHECKF(IsDynaMap());
    CRoleSet::Iterator	pRole = RoleManager()->QuerySet()->NewEnum();
    while(pRole.Next())
    {
        if (pRole && pRole->GetMap()->GetID() == GetID())
        {
            CUser* pUser;
            CNpc* pNpc;
            if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
            {
                return false;
            }
            else if (pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsLinkNpc())
            {
                return false;
            }
        }
    }
    pRole.Reset();
    while(pRole.Next())
    {
        DEBUG_TRY	// VVVVVVVVVVVVVV
        if (pRole && pRole->GetMap()->GetID() == GetID())
        {
            RoleManager()->QuerySet()->DelObj(pRole->GetID());
        }
        DEBUG_CATCH("EraseMap()")	// AAAAAAAAAAAAAAA
    }
    //??? 未处理Booth，估计不会在动态地图中摆摊
    CMapItemSet::Iterator pMapItem = MapManager()->QueryMapItemSet()->NewEnum();
    while(pMapItem.Next())
    {
        if (pMapItem && pMapItem->GetMap()->GetID() == GetID())
        {
            MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());
        }
    }
    SQLBUF	szSQL;
    sprintf(szSQL, "UPDATE %s SET del_flag=1 WHERE id=%u LIMIT 1", _TBL_DYNAMAP, GetID());
    Database()->ExecuteSQL(szSQL);
    MapManager()->QuerySet()->DelObj(GetID());
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::ChangeMapDoc(OBJID idDoc)						// false: user in map, can't change
{
    /*	CUserManager::Iterator pUser = UserManager()->NewEnum();
    	while(pUser.Next())
    	{
    		if (pUser->GetMapID() == GetID())
    			return false;
    	}

    	IMapData* pMapData	= IMapData::CreateNew(idDoc, MAPDATA_VERSION);	// VVVVVVVVVVVVVVVVVVVVVVVVVV
    	if (!pMapData)
    	{
    		LOGERROR("地图文件[%d]加载失败!", idDoc);
    		return false;
    	}
    	CHECKF(pMapData->GetMapWidth() == m_pMapData->GetMapWidth());
    	CHECKF(pMapData->GetMapHeight() == m_pMapData->GetMapHeight());
    */
    int nOldDoc = m_pData->GetInt(GAMEMAPDATA_MAPDOC);
    m_pData->SetInt(GAMEMAPDATA_MAPDOC, idDoc);
    m_pData->Update();
    m_pData->SetInt(GAMEMAPDATA_MAPDOC, nOldDoc);
    m_pData->ClearUpdateFlags();
    /*
    	IF_OK(m_pMapData)
    		m_pMapData->Release();
    	m_pMapData	= pMapData;		// AAAAAAAAAAAAAAAAA

    	// synchro ai server
    	CMsgAction	msg;
    	IF_OK(msg.Create(GetID(), 0, 0, 0, actionChangeMapDoc, idDoc))
    		MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msg);
    */
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::AddTerrainObj(OBJID idOwner, int x, int y, OBJID idTerrainObj)
{
    if (m_pMapData->AddTerrainItem(idOwner, x, y, idTerrainObj))
    {
        CMsgAction	msg;
        IF_OK(msg.Create(GetID(), x, y, 0, actionAddTerrainObj, idOwner))
        MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(idTerrainObj, &msg);
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::DelTerrainObj(OBJID idOwner)
{
    if (m_pMapData->DelTerrainItem(idOwner))
    {
        CMsgAction	msg;
        IF_OK(msg.Create(GetID(), 0, 0, 0, actionDelTerrainObj, idOwner))
        MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msg);
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CGameMap::DelNpcByType(int nType)
{
    for(IRoleSet::Iter_t i = RoleManager()->QuerySet()->Begin(); i != RoleManager()->QuerySet()->End(); )
    {
        IRole* pRole = RoleManager()->QuerySet()->GetObjByIter(i);
        i++;		// 要删除，提前移动
        CNpc* pNpc;
        if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetMapID() == GetID() && pNpc->GetType() == nType)
        {
            if (!pNpc->IsDeleted())
            {
                ASSERT(pNpc->DelNpc());
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CGameMap::ProcessRpc(CEventPack& pack)
{
    switch(pack.GetAction())
    {
    case	RPC_MAP_DELNPC:
        {
            int nType = 0;
            pack >> nType;
            this->DelNpcByType(nType);
        }
    default:
        ASSERT(!"CGameMap::ProcessRpc");
    }
}

//check castle class          add huang 2003.12.30
//bool CGameMap::IsNormalCastle(OBJID idMap)
//{
//	if (idMap == SYNDICATE_CASTLE_1_ID)
//		return true;
//	else if (idMap == SYNDICATE_CASTLE_2_ID)
//		return true;
//	else if (idMap == SYNDICATE_CASTLE_3_ID)
//		return true;
//	else
//		return false;
//}
//
//bool CGameMap::IsCentreCastle(OBJID idMap)
//{
//	if (idMap == SYNDICATE_CASTLE_CENTR_ID)
//		return true;
//	else
//		return false;
//得到帮派地图中的帮派标志NPC
CNpc* CGameMap::GetNpcSynFlag()
{
    if (!IsSynMap() || !IsDynaMap())
    {
        return NULL;
    }
    CNpc* pNpc;
    CRoleSet::Iterator pRole = RoleManager()->QuerySet()->NewEnum();
    while(pRole.Next())
    {
        if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetMapID() == this->GetID() &&
                pNpc->IsDynaNpc() && pNpc->IsAlive() && pNpc->GetMaxLife() && pNpc->IsSynFlag() )
        {
            return pNpc;
        }
    }
    return NULL;
}
//得到　帮派地图中的　战书ＮＰＣ
CNpc* CGameMap::GetNpcSynWarLetter()
{
    if (!IsSynMap() || !IsDynaMap())
    {
        return NULL;
    }
    CNpc* pNpc;
    CRoleSet::Iterator pRole = RoleManager()->QuerySet()->NewEnum();
    while(pRole.Next())
    {
        if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc))
                && pNpc->GetMapID() == this->GetID() && pNpc->IsSynWarLetter() )
        {
            return pNpc;
        }
    }
    return NULL;
}
//根据ＮＰＣ计算城市的价值,地图中DYNNPC的生命
int CGameMap::GetSynMapValue()
{
    if (!IsSynMap())
    {
        return -1;    //不是帮派地图
    }
    int nMoneySum = 0;
    CNpc* pNpc;
    CRoleSet::Iterator pRole = RoleManager()->QuerySet()->NewEnum();
    while(pRole.Next())
    {
        if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetMapID() == this->GetID()
                && pNpc->IsDynaNpc() && pNpc->IsAlive() && pNpc->GetMaxLife() && pNpc->IsSynNpc())
        {
            nMoneySum = ::AddToTop(pNpc->GetMaxFixMoney() - pNpc->GetLostFixMoney(), nMoneySum, SYNMONEY_LIMIT);
        }
    }
    return nMoneySum;
}
//得到战争后，ＮＰＣ丢失的生命与原来的生命的比例，和价值
void CGameMap::GetAllNpcLostLifeScaleAndValue(int& nScale, int& nValue)
{
    if (!IsSynMap() || !IsDynaMap())
    {
        return ;    //不是帮派地图
    }
    int nMoneySum = 0;
    int nLostLife = 0, nAllLife = 0;
    CNpc* pNpc;
    CRoleSet::Iterator pRole = RoleManager()->QuerySet()->NewEnum();
    while(pRole.Next())
    {
        if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetMapID() == this->GetID()
                && pNpc->IsDynaNpc() && pNpc->IsAlive() && pNpc->GetMaxLife() && pNpc->IsSynNpc())
        {
            nMoneySum = AddToTop(pNpc->GetLostFixMoney(), nMoneySum, SYNMONEY_LIMIT);
            int nLostLifeTmp = pNpc->GetMaxLife() - pNpc->GetLife() ;
            nLostLife = ::CutTrail(nLostLife + nLostLifeTmp, nLostLifeTmp);
            nAllLife = ::CutTrail(nAllLife + pNpc->GetMaxLife(), pNpc->GetMaxLife());
        }
    }
    nScale =  CutRange(MulDiv(nLostLife, 100, nAllLife), 0, 100);
    nValue = nMoneySum;
}

void CGameMap::SetSynOccupyTime()
{
    //将占领的帮派城市的时间存到 帮派地图中的 战书NPC中
    {
        CNpc* pNpc = this->GetNpcSynWarLetter();
        if (pNpc)
        {
            pNpc->SetOccupyTime();
        }
    }
}
//得到地图的随机位置
bool CGameMap::GetRandomPos(int& nX, int& nY)
{
    int nWidth = this->GetWidth();
    int nHeight = this->GetHeight();
    int nNum = 0;
    while(nNum < 20)
    {
        nX = ::RandGet(nWidth);
        nY = ::RandGet(nHeight);
        if (this->IsStandEnable(nX, nY))
        {
            return true;
        }
        else
        {
            nNum++;
            continue;
        }
    }
    return false;
}
