
//**********************************************************
// 代码编辑器
//**********************************************************

// TaskNpc.cpp: implementation of the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "windows.h"
#include "Gameobj.h"
#include "MapGroup.h"
#include "define.h"
#include "Npc.h"
#include "NpcTask.h"
#include "NpcScene.h"
#include "NpcLink.h"
#include "NpcStorage.h"
#include "BetOutside.h"

//////////////////////////////////////////////////////////////////////
const int	FIXSYNFLAG_SECS				= 10;
const int	MAX_FIXMONEY				= 1000;
const int	FIXLIFE_PER_MONEY			= 1;
const int	BROADCASTSCORE_SECS			= 30;				// 注意：太大可能错开战斗时间检查
const int	TRANSNPC_FEE				= 10000;			// 每天每个传送门收费

//////////////////////////////////////////////////////////////////////
// static
char	szNpcTable[] = _TBL_NPC;
char	szDynTable[] = _TBL_DYNANPC;
MYHEAP_IMPLEMENTATION(CNpc, s_heap)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNpc::CNpc()
{
    SetObjType(OBJ_NPC);
    m_id	= ID_NONE;
    m_pData	= NULL;
    m_bDynaNpc	= false;
    m_pShop		= NULL;
    m_pAuction  = NULL;
    m_pTask		= NULL;
    //	m_pStorage	= NULL;
    m_pTrunk	= NULL;
    m_pScene	= NULL;
    m_pLinkMap	= NULL;
    m_pBet		= NULL;
    m_pEudemonBrooder	= NULL;
    m_pEudemonStorage	= NULL;
    m_i64Effect	= 0;
}

//////////////////////////////////////////////////////////////////////
CNpc::~CNpc()
{
    if (m_pMap)
    {
        //SendLeaveFromBlock();
        ClrBroadcastSet();
        if (m_pMap)
        {
            m_pMap->LeaveRoom(QueryMapThing(), WITH_BLOCK);
            UpdateHoldFloor(false);
        }
    }
    SAFE_RELEASE (m_setStatus);
    if (m_pData)
    {
        if (IsDynaNpc())
        {
            if (IsAlive())
            {
                SaveInfo();
            }
            else
            {
                DeleteRecord();
            }
        }
        m_pData->Release();
    }
    if (m_pShop)
    {
        m_pShop->Release();
    }
    if (m_pAuction)
    {
        m_pAuction->Release();
    }
    if (m_pTask)
    {
        m_pTask->Release();
    }
    /*if (m_pStorage)
    {
    	m_pStorage->Release();
    }*/
    if (m_pLinkMap)
    {
        m_pLinkMap->Release();
    }
    if (m_pScene)
    {
        m_pScene->Release();
    }
    if (m_pTrunk)
    {
        m_pTrunk->Release();
    }
    if (m_pBet)
    {
        m_pBet->Release();
    }
    if (m_pEudemonBrooder)
    {
        SAFE_RELEASE (m_pEudemonBrooder);
    }
    if (m_pEudemonStorage)
    {
        SAFE_RELEASE (m_pEudemonStorage);
    }
}

//////////////////////////////////////////////////////////////////////
bool CNpc::Create(PROCESS_ID idProcess, NPCINFO_ST* pInfo, OBJID idItemType, LPCTSTR szName)
{
    m_idProcess = idProcess;
    m_bDynaNpc	= true;
    /*
    	// 使用VALUES单独INSERT，防止SQL溢出
    	SQLBUF	szSQL;
    	ASSERT(_MAX_NPCTASK == 8);
    	ASSERT(MAX_NPCDATA == 4);
    	sprintf(szSQL, "INSERT %s VALUES(%u,%u,%d,'%s',%d,%d,%d,%u,%d,%d"",%d,%d,%d,%d,%d,%d,%d,%d"",%d,%d,%d,%d,'%s'"",%u,%d,%d,%d,%u",
    					_TBL_DYNANPC,
    					ID_NONE, pInfo->idOwner, pInfo->nOwnerType, "无", pInfo->nType, pInfo->nLookFace, -1, pInfo->idMap, pInfo->nPosX, pInfo->nPosY,
    					pInfo->setTask[0], pInfo->setTask[1], pInfo->setTask[2], pInfo->setTask[3],
    					pInfo->setTask[4], pInfo->setTask[5], pInfo->setTask[6], pInfo->setTask[7],
    					pInfo->setData[0], pInfo->setData[1], pInfo->setData[2], pInfo->setData[3], pInfo->szData,
    					ID_NONE, pInfo->nLife, pInfo->nLife, pInfo->nDir, pInfo->idRegion);
    	CHECKFDatabase()->ExecuteSQL(szSQL));
    	OBJID idRecord = Database()->GetInsertId();
    	CHECKF(idRecord != ID_NONE);
    */
    ASSERT(!m_pData);
    CDynaNpcData* pData = CDynaNpcData::CreateNew();
    if (pData)
    {
        m_pData	= pData->QueryInterface();
    }
    IF_NOT(m_pData && m_pData->Create(GameDataDefault()->GetDynaNpc(), ID_NONE))			// idRecord))
    return false;
    m_pData->SetInt(NPCDATA_OWNERID,	pInfo->idOwner);
    m_pData->SetInt(NPCDATA_OWNERTYPE,	pInfo->nOwnerType);
    m_pData->SetInt(NPCDATA_TYPE,		pInfo->nType);
    m_pData->SetStr(NPCDATA_NAME,		szName, _MAX_NAMESIZE);
    m_pData->SetInt(NPCDATA_LOOKFACE,	pInfo->nLookFace);
    m_pData->SetInt(NPCDATA_LENGTH,		pInfo->cLength);
    m_pData->SetInt(NPCDATA_FAT,		pInfo->cFat);
    m_pData->SetInt(NPCDATA_IDXSERVER,	-1);
    m_pData->SetInt(NPCDATA_MAPID,		pInfo->idMap);
    m_pData->SetInt(NPCDATA_POSX_,		pInfo->nPosX);
    m_pData->SetInt(NPCDATA_POSY_,		pInfo->nPosY);
    m_pData->SetInt(NPCDATA_TASK0,		pInfo->setTask[0]);
    m_pData->SetInt(NPCDATA_TASK1,		pInfo->setTask[1]);
    m_pData->SetInt(NPCDATA_TASK2,		pInfo->setTask[2]);
    m_pData->SetInt(NPCDATA_TASK3,		pInfo->setTask[3]);
    m_pData->SetInt(NPCDATA_TASK4,		pInfo->setTask[4]);
    m_pData->SetInt(NPCDATA_TASK5,		pInfo->setTask[5]);
    m_pData->SetInt(NPCDATA_TASK6,		pInfo->setTask[6]);
    m_pData->SetInt(NPCDATA_TASK7,		pInfo->setTask[7]);
    m_pData->SetInt(NPCDATA_DATA0,		pInfo->setData[0]);
    m_pData->SetInt(NPCDATA_DATA1,		pInfo->setData[1]);
    m_pData->SetInt(NPCDATA_DATA2,		pInfo->setData[2]);
    m_pData->SetInt(NPCDATA_DATA3,		pInfo->setData[3]);
    m_pData->SetStr(NPCDATA_DATASTR,	pInfo->szData, _MAX_NAMESIZE);
    m_pData->SetInt(NPCDATA_LINKID,		pInfo->idLink);
    m_pData->SetInt(NPCDATA_LIFE,		pInfo->nLife);
    m_pData->SetInt(NPCDATA_MAXLIFE,	pInfo->nLife);
    m_pData->SetInt(NPCDATA_BASE,		pInfo->idBase);
    m_pData->SetInt(NPCDATA_SORT,		pInfo->nSort);
    if (pInfo->nSort & NPCSORT_RECYCLE)
    {
        m_pData->SetInt(NPCDATA_ITEMTYPE,		idItemType);
    }
    m_pData->InsertRecord();
    m_id	= MapManager()->SpawnNewNpcID();
    m_nPosX = m_pData->GetInt(NPCDATA_POSX_);
    m_nPosY = m_pData->GetInt(NPCDATA_POSY_);
    if (this->IsTrunkNpc()
            || this->IsShopNpc()
            || this->IsSceneNpc()
            || this->IsTaskNpc()
            || this->IsBetNpc())
    {
        IF_NOT(CreateFuncObj(idProcess))
        return false;
    }
    IF_NOT(CheckSortMutex())
    return false;
    m_setStatus = CStatusSet::CreateNew(true);
    CHECKF(m_setStatus);
    m_pMap = MapManager()->QueryMap(m_pData->GetInt(NPCDATA_MAPID));
    IF_NOT(m_pMap)
    return false;			//?? 非本进程组地图
    // trans npc exclusive
    MapGroup(PID)->QueryIntraMsg()->DelTransNpc(GetInt(NPCDATA_LINKID));	// del this mapgroup first
    m_pMap->EnterRoom(QueryMapThing(), WITH_BLOCK);
    UpdateHoldFloor(true);
    //SendSelfToBlock();
    UpdateBroadcastSet();
    // set map syn id
    if (IsSynFlag())
    {
        GetMap()->SetSynID(GetSynID(), true);		// with all map npc
        //		CheckFightTime();
        // temporary code
        OBJID idMoreMap = GetData(SYNFLAGNPC_MOREMAPID);
        if (idMoreMap != ID_NONE)
        {
            CGameMap* pMoreMap = MapManager()->QueryMap(idMoreMap);
            if (pMoreMap)
            {
                pMoreMap->SetSynID(GetSynID(), true);    // with all map npc
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::Create(PROCESS_ID idProcess, IRecordset* pRecordset, bool bDynaNpc)
{
    m_idProcess = idProcess;
    m_bDynaNpc	= bDynaNpc;
    ASSERT(!m_pData);
    if (bDynaNpc)
    {
        CDynaNpcData* pData = CDynaNpcData::CreateNew();
        if (pData)
        {
            m_pData	= pData->QueryInterface();
        }
    }
    else
    {
        CNpcData* pData = CNpcData::CreateNew();
        if (pData)
        {
            m_pData	= pData->QueryInterface();
        }
    }
    IF_NOT(m_pData && m_pData->Create(pRecordset))
    return false;
    CGameMap* pMap = MapManager()->QueryMap(m_pData->GetInt(NPCDATA_MAPID), false);		// false: not load dynamic map
    if (!pMap)
    {
        return false;    //?? 非本进程组地图，或者动态地图的NPC
    }
    if (bDynaNpc)
    {
        m_id	= MapManager()->SpawnNewNpcID();
    }
    else
    {
        m_id	= m_pData->GetID();    // fixed to database id
    }
    m_nPosX = m_pData->GetInt(NPCDATA_POSX_);
    m_nPosY = m_pData->GetInt(NPCDATA_POSY_);
    if (this->IsTrunkNpc()
            || this->IsShopNpc()
            || this->IsSceneNpc()
            || this->IsTaskNpc()
            || this->IsBetNpc()
            || this->IsAuctionNpc() )
    {
        IF_NOT(CreateFuncObj(idProcess))
        return false;
    }
    IF_NOT(CheckSortMutex())
    return false;
    m_setStatus = CStatusSet::CreateNew(true);
    CHECKF(m_setStatus);
    m_pMap = pMap;
    // 冗错非法NPC坐标
    if (!GetMap()->IsValidPoint(GetPosX(), GetPosY()))
    {
        LOGERROR("NPC数据错[%u]，坐标非法[%d,%d]", GetRecordID(), GetPosX(), GetPosY());
        m_pMap	= NULL;
        return false;
    }
    m_pMap->EnterRoom(QueryMapThing(), WITH_BLOCK);
    UpdateHoldFloor(true);
    UpdateBroadcastSet();
    // set map syn id
    if (IsSynFlag())
    {
        GetMap()->SetSynID(GetSynID(), false);		// false: no set npc in map
        CheckFightTime();
        // temporary code
        OBJID idMoreMap = GetData(SYNFLAGNPC_MOREMAPID);
        if (idMoreMap != ID_NONE)
        {
            CGameMap* pMoreMap = MapManager()->QueryMap(idMoreMap);
            if (pMoreMap)
            {
                pMoreMap->SetSynID(GetSynID(), false);    // false: no set npc in map
            }
        }
    }
    if (!IsDynaNpc() && GetMaxLife() > 0)
    {
        LOGERROR("静态NPC不能设置最大血为非0：[%d]", GetID());
    }
    return true;
}

/////////////////////////////////////////////////////////////////////
BOOL CNpc::	ChangePos (OBJID idMap, int nPosX, int nPosY)
{
    if (!MapManager()->QueryMap(idMap))
    {
        return false;
    }
    this->SendLeaveFromBlock();
    m_pMap->LeaveRoom(QueryMapThing(), WITH_BLOCK);
    UpdateHoldFloor(false);
    m_pMap = MapManager()->QueryMap(idMap);
    m_nPosX = nPosX;
    m_nPosY = nPosY;
    m_pMap->EnterRoom(QueryMapThing(), WITH_BLOCK);
    UpdateHoldFloor(true);
    UpdateBroadcastSet();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::CreateFuncTrunk(PROCESS_ID idProcess)
{
    /*if (IsStorageNpc())
    {
    	ASSERT(!m_pStorage);
    	CNpcStorage*	m_pStorage = CNpcStorage::CreateNew();
    	IF_NOT(m_pStorage && m_pStorage->Create(idProcess, GetID())
    		return false;
    }*/
    if (IsTrunkNpc())
    {
        ASSERT(!m_pTrunk);
        m_pTrunk = CNpcTrunk::CreateNew();
        IF_NOT(m_pTrunk && m_pTrunk->Create(idProcess, GetRecordID(), GetData(TRUNKNPC_SIZE), ITEMPOSITION_TRUNK))
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::CreateFuncEudemonBrooder(PROCESS_ID idProcess)
{
    if (IsEudemonBrooderNpc())
    {
        ASSERT(!m_pEudemonBrooder);
        m_pEudemonBrooder = CNpcTrunk::CreateNew();
        IF_NOT(m_pEudemonBrooder && m_pEudemonBrooder->Create(idProcess, GetRecordID(), GetData(EUDEMON_BROODER_NPC_SIZE), ITEMPOSITION_EUDEMON_BROODER))
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::CreateFuncEudemonStorage(PROCESS_ID idProcess)
{
    if (IsEudemonStorageNpc())
    {
        ASSERT(!m_pEudemonStorage);
        m_pEudemonStorage = CNpcTrunk::CreateNew();
        IF_NOT(m_pEudemonStorage && m_pEudemonStorage->Create(idProcess, GetRecordID(), GetData(EUDEMON_STORAGE_NPC_SIZE), ITEMPOSITION_EUDEMON_STORAGE))
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::CreateFuncObj(PROCESS_ID idProcess)
{
    // shop
    if (IsShopNpc())
    {
        ASSERT(!m_pShop);
        m_pShop = CNpcShop::CreateNew();
        IF_NOT(m_pShop && m_pShop->Create(idProcess, this, GetRecordID()))
        return false;
    }
    if (IsAuctionNpc())
    {
        ASSERT(!m_pAuction);
        /*		CAuction* temp = CAuction::CreateNew(idProcess);
        		CHECKF(temp);
        		m_pAuction = temp->QueryInterface();*/
        m_pAuction = CAuction::CreateNew(idProcess);
        IF_NOT(QueryAuction()->Create(idProcess, GetRecordID()))
        return false;
        //	delete temp;
    }
    if (IsTaskNpc())
    {
        ASSERT(!m_pTask);
        m_pTask = CNpcTask::CreateNew();
        IF_NOT(m_pTask && m_pTask->Create(idProcess, this, m_pData))
        return false;
    }
    if (IsSceneNpc())
    {
        CGameMap* pMap = MapManager()->QueryMap(m_pData->GetInt(NPCDATA_MAPID), false);
        if (!pMap)
        {
            return false;
        }
        ASSERT(!m_pScene);
        m_pScene = CNpcScene::CreateNew();
        IF_NOT(m_pScene && m_pScene->Create(this, pMap))
        return false;
    }
    if (IsLinkNpc())
    {
        ASSERT(!m_pLinkMap);
        m_pLinkMap = CNpcLink::CreateNew(m_idProcess, this);
        IF_NOT(m_pLinkMap)
        return false;
    }
    if (IsBetNpc())
    {
        ASSERT(!m_pBet);
        m_pBet = CNpcBet::CreateNew(m_idProcess, this);
        IF_NOT(m_pBet)
        return false;
    }
    if ((m_pData->GetInt(NPCDATA_SORT) & NPCSORT_TABLE) != 0)
    {
        ASSERT(!m_pTable);
        m_pTable = CNpcTable::CreateNew();
        IF_NOT(m_pTable && m_pTable->Create(idProcess, this, GetRecordID()))
        return false;
    }
    return true;		//@
}

int CNpc::GetMountTypeID()
{ return IsShelfNpc() ? QueryTrunk()->QueryPackage()->GetItemType(0) : m_pData->GetInt(STATUARYDATA_HELMET); }
int CNpc::GetArmorTypeID()
{ return IsShelfNpc() ? QueryTrunk()->QueryPackage()->GetItemType(1) : m_pData->GetInt(STATUARYDATA_ARMOR); }
int CNpc::GetWeaponRTypeID()
{ return IsShelfNpc() ? QueryTrunk()->QueryPackage()->GetItemType(2) : m_pData->GetInt(STATUARYDATA_WEAPONR); }
int CNpc::GetWeaponLTypeID()
{ return IsShelfNpc() ? QueryTrunk()->QueryPackage()->GetItemType(3) : m_pData->GetInt(STATUARYDATA_WEAPONL); }
//////////////////////////////////////////////////////////////////////
void CNpc::SendShow(IRole* pRole)
{
    if (IsStatuary() || IsShelfNpc())
    {
        CMsgPlayer msg;
        IF_OK(msg.Create(QueryRole()))
        pRole->SendMsg(&msg);
    }
    else if (IsDynaNpc() && GetMaxLife() > 0)
    {
        CMsgNpcInfoEx	msg;
        IF_OK(msg.Create(this))
        pRole->SendMsg(&msg);
    }
    else
    {
        CMsgNpcInfo	msg;
        IF_OK(msg.Create(this))
        pRole->SendMsg(&msg);
    }
}

//////////////////////////////////////////////////////////////////////
// application
//////////////////////////////////////////////////////////////////////
bool CNpc::SendSelfToBlock()
{
    CHECKF(GetMap());
    if (IsStatuary() || IsShelfNpc())
    {
        CMsgPlayer msg;
        IF_OK(msg.Create(QueryRole()))
        GetMap()->BroadcastBlockMsg(QueryMapThing(), &msg);
    }
    else if (IsDynaNpc() && GetMaxLife() > 0)
    {
        CMsgNpcInfoEx	msg;
        IF_OK(msg.Create(this))
        GetMap()->BroadcastBlockMsg(QueryMapThing(), &msg);
    }
    else
    {
        CMsgNpcInfo	msg;
        IF_OK(msg.Create(this))
        GetMap()->BroadcastBlockMsg(QueryMapThing(), &msg);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::SendLeaveFromBlock()
{
    CHECKF(GetMap());
    CMsgNpc	msg;
    CHECKF(msg.Create(EVENT_LEAVEMAP, GetID()));
    GetMap()->BroadcastBlockMsg(QueryMapThing(), &msg);
    return true;
}

bool CNpc::ActivateNpc(IRole* pRole, int nAction)
{
    CUser* pUser = NULL;
    if (IsTaskNpc() && IsAlive() && pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        return QueryTask()->ActivateTask(pUser);
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
OBJID CNpc::GetMapID()
{
    CHECKF(m_pMap);
    return m_pMap->GetID();
}

//////////////////////////////////////////////////////////////////////
void CNpc::OnTimer(time_t tCurr)
{
    if (IsAuctionNpc())
    {
        QueryAuction()->OnTimer(tCurr);
    }
    // auto recruit life
    //---jinggy---进行帮派每占领一周，军团声望增加50万的处理---begin
    if (this->IsSynWarLetter())  //判断是否是接收战书的NPC
    {
        if (!m_tSynWarLetter.IsActive())
        {
            m_tSynWarLetter.Startup(SYNWARLETTERNPC_CHECKINTERVAL);
        }
        if (m_tSynWarLetter.ToNextTime())
        {
            int nOccupyTime = this->GetData(SYNWARLETTER_OCCUPYTIME);
            time_t tTmp = nOccupyTime;
            tm* pTm = localtime(&tTmp);
            if (!pTm)
            {
                return;
            }
            tm	tmOccupy;
            memset(&tmOccupy, 0, sizeof(tm));
            tmOccupy = *pTm;
            tmOccupy.tm_mday	+= 7;
            time_t	tAfterOccupy7 = mktime(&tmOccupy);
            time_t	tCur;
            tCur = time(NULL);
            if (tCur >= tAfterOccupy7)
            {
                CSyndicate* pSyn = SynManager()->QuerySyndicate(this->GetSynID());
                if (pSyn)
                {
                    const int nRepute = 500000;
                    pSyn->QueryModify()->AddData(SYNDATA_REPUTE, nRepute, true);	//
                }
                SetData(SYNWARLETTER_OCCUPYTIME, (int)tCur, true);
            }
        }
    }
    //---jinggy---进行帮派每占领一周，军团声望增加50万的处理---end
    if (IsSynFlag())
    {
        if (m_tSynEvent.ToNextTime(FIXSYNFLAG_SECS))
        {
            // recruit life
            RecruitLife();
        }
        if (m_tBroadcastScore.ToNextTime(BROADCASTSCORE_SECS))
        {
            if (GetMap()->IsWarTime())
            {
                BroadcastScore();
            }
            // set war flag
            CheckFightTime();
        }
    }
    if (IsDynaNpc() && GetType() == _SYNTRANS_NPC)
    {
        time_t	tCurr	= time(NULL);
        tm*		pTm		= localtime(&tCurr);
        if (m_bFeeOK)
        {
            if (pTm->tm_hour != 00)
            {
                m_bFeeOK	= false;
            }
        }
        else
        {
            if (pTm->tm_hour == 00)
            {
                m_bFeeOK	= true;
                OBJID idSyn = GetSynID();
                if (idSyn != ID_NONE)
                {
                    CSyndicate*	pSyn = SynManager()->QuerySyndicate(idSyn);
                    IF_OK(pSyn)
                    pSyn->QueryModify()->AddData(SYNDATA_MONEY, -1 * TRANSNPC_FEE, true);		// mast true
                }
            }
        }
    }
    if (IsTaskNpc())
    {
        QueryTask()->OnTimer(tCurr);
    }
    else if (IsBetNpc())
    {
        QueryBet()->OnTimer();
    }
}

//////////////////////////////////////////////////////////////////////
void CNpc::BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf)
{
    CRole::BroadcastRoomMsg(pMsg, bSendSelf);
    CHECK(GetMap());
    GetMap()->BroadcastBlockMsg(QueryMapThing(), pMsg, bSendSelf);
}

//////////////////////////////////////////////////////////////////////
void CNpc::BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf)
{
    CMsgTalk	msg;
    if (msg.Create(GetName(), ALLUSERS_NAME, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
    {
        this->BroadcastRoomMsg(&msg, bSendSelf);
    }
}

//////////////////////////////////////////////////////////////////////
void CNpc::BeKill(IRole* pTarget /*= NULL*/)
{
    CHECK(IsDynaNpc());
    if (IsDeleted())
    {
        return ;
    }
    // die action
    if (IsDieAction())
    {
        OBJID idAction = m_pData->GetInt(NPCDATA_LINKID);
        CHECK(idAction);
        CUser* pUser = NULL;
        if (pTarget)
        {
            pTarget->QueryObj(OBJ_USER, IPP_OF(pUser));
        }
        GameAction()->ProcessAction(idAction, pUser, QueryRole());
    }
    else
    {
        this->SetStatus(STATUS_DIE);
        ASSERT(DelNpc());
    }
}

//////////////////////////////////////////////////////////////////////
bool CNpc::DelNpc()			// call this mast !IsDeleted()
{
    CHECKF(IsDynaNpc() && !IsDeleted());
    if (IsLinkNpc())
    {
        if (!QueryLinkMap()->EraseMap())
        {
            return false;
        }
    }
    if (IsSceneNpc())
    {
        QueryScene()->DelTerrainObj();
    }
    m_pData->SetInt(NPCDATA_LIFE, 0);		// set flag to die
    // die status
    m_tDie.Startup(DYNANPCDIEDELAY_SECS);
    // set map syn id
    if (IsSynFlag())
    {
        GetMap()->SetStatus(CGameMap::STATUS_WAR, false);
        GetMap()->SetSynID(ID_NONE, true);		// set all npc
        // temporary code
        OBJID idMoreMap = GetData(SYNFLAGNPC_MOREMAPID);
        if (idMoreMap != ID_NONE)
        {
            CGameMap* pMoreMap = MapManager()->QueryMap(idMoreMap);
            if (pMoreMap)
            {
                pMoreMap->SetSynID(ID_NONE, true);    // true: set npc in map
            }
        }
    }
    SendLeaveFromBlock();		//??? show leave map now!
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::BeAttack(bool bMagic, IRole* pTarget, int nPower, bool bReflectEnable/*=true*/)
{
    if (IsDynaNpc())
    {
        if (IsSynNpc())
        {
            CUser* pUser = NULL;
            CHECKF(pTarget && pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)));
            CGameMap* pSynMap = this->GetMap();
            if (pSynMap->IsSynMap())
            {
                if ( pSynMap->IsWarTime() && pSynMap->GetSynID() != pUser->GetSynID())
                {
                    int nID = GetSynID();
                    //在攻城战中，玩家每伤害对方建筑100点血，得到1点军团贡献度
                    pUser->QuerySynAttr()->AddProffer(__max(0, nPower / 100));
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        return true;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::AddAttrib(int idxAttr, __int64 i64Data, int nSynchro)
{
    CHECKF(IsDynaNpc());
    CMsgUserAttrib	msg;
    if (!msg.Create(GetID(), _USERATTRIB_NONE, _USERATTRIB_NONE))			// _USERATTRIB_NONE : 不添加
    {
        return false;
    }
    if (i64Data)
    {
        switch(idxAttr)
        {
        case	_USERATTRIB_LIFE:
            {
                IF_NOT(!( IsAwardScore() && !GetMap()->IsWarTime() && i64Data < 0 ))
                return false;
                if ((int)this->GetLife() + i64Data <= 0)
                {
                    //					this->SetStatus(STATUS_DIE);
                    m_pData->SetInt(NPCDATA_LIFE, 0);
                }
                else
                {
                    m_pData->SetInt(NPCDATA_LIFE, this->GetLife() + i64Data);
                }
                if (!msg.Append(_USERATTRIB_LIFE, GetLife()))
                {
                    return false;
                }
            }
            break;
        case	_USERATTRIB_MANA:
            //			if (!msg.Append(_USERATTRIB_MANA, GetMana()))
            return false;
            break;
        case	_USERATTRIB_EXP:
            //			if (!msg.Append(_USERATTRIB_EXP, GetExp()))
            return false;
            break;
        case	_USERATTRIB_MONEY:
            //			if (!msg.Append(_USERATTRIB_MONEY, GetMoney()))
            return false;
            break;
        default:
            return false;
        }
    }
    if (nSynchro != SYNCHRO_FALSE)
    {
        switch(nSynchro)
        {
        case SYNCHRO_TRUE:
            this->SendMsg(&msg);
            break;
        case SYNCHRO_BROADCAST:
            this->BroadcastRoomMsg(&msg, INCLUDE_SELF);
            break;
        default:
            break;
        }
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
void CNpc::SaveInfo()
{
    if (m_pData && IsDynaNpc())
    {
        m_pData->Update();
    }
}

/////////////////////////////////////////////////////////////////////////////
void CNpc::TransPos(int nPosX, int nPosY)
{
    /*
    	CHECK(IsDynaNpc());

    	this->SendLeaveFromBlock();
    	GetMap()->LeaveRoom(QueryMapThing(), WITH_BLOCK);

    	m_nPosX	= nPosX;
    	m_nPosY	= nPosY;
    	m_pData->SetInt(NPCDATA_POSX, nPosX_);
    	m_pData->SetInt(NPCDATA_POSY, nPosY_);
    	if (IsDynaNpc())
    		SaveInfo();

    	GetMap()->EnterRoom(QueryMapThing(), WITH_BLOCK);
    	SendSelfToBlock();
    */
}

/////////////////////////////////////////////////////////////////////////////
void CNpc::SetStatus(int nStatus, bool bSynchro)
{
    I64	i64OldEffect = GetEffect();
    switch (nStatus)
    {
    case	STATUS_NORMAL:
        {
            //			m_data.m_Info.dwStatus	= nStatus;
            m_i64Effect	= KEEPEFFECT_NORMAL;
        }
        break;
    case	STATUS_DIE:
        {
            //			m_data.m_Info.dwStatus	= nStatus;
            m_i64Effect	&= KEEPEFFECT_NOT_CLEAR;
            m_i64Effect	|= KEEPEFFECT_DIE;
            //			ClearAllStatus();
        }
        break;
    case	STATUS_TEAMLEADER:
        {
            m_i64Effect	|= KEEPEFFECT_TEAMLEADER;
        }
        break;
    case	STATUS_CRIME:
        {
            m_i64Effect	|= KEEPEFFECT_CRIME;
        }
        break;
    case	STATUS_SYNCRIME:
        {
            m_i64Effect	|= KEEPEFFECT_SYNCRIME;
        }
        break;
    case	STATUS_POISON:
        {
            m_i64Effect	|= KEEPEFFECT_POISON;
        }
        break;
    case	STATUS_PKVALUE:
        {
        }
        break;
    case	STATUS_DEFENCE1:
        {
            m_i64Effect	&= ~(KEEPEFFECT_DEFENCE2 | KEEPEFFECT_DEFENCE3);
            m_i64Effect	|= KEEPEFFECT_DEFENCE1;
        }
        break;
    case	STATUS_DEFENCE2:
        {
            m_i64Effect	&= ~(KEEPEFFECT_DEFENCE1 | KEEPEFFECT_DEFENCE3);
            m_i64Effect	|= KEEPEFFECT_DEFENCE2;
        }
        break;
    case	STATUS_DEFENCE3:
        {
            m_i64Effect	&= ~(KEEPEFFECT_DEFENCE1 | KEEPEFFECT_DEFENCE2);
            m_i64Effect	|= KEEPEFFECT_DEFENCE3;
        }
        break;
    case	STATUS_FREEZE:
        {
            m_i64Effect |= KEEPEFFECT_FREEZE;
        }
        break;
    case	STATUS_SMOKE:
        {
            m_i64Effect |= KEEPEFFECT_SMOKE;
        }
        break;
    case	STATUS_DARKNESS:
        {
            m_i64Effect |= KEEPEFFECT_DARKNESS;
        }
        break;
    case	STATUS_PALSY:
        {
            m_i64Effect |= KEEPEFFECT_PALSY;
        }
        break;
    case	STATUS_ATTACK:
        {
            m_i64Effect	|= KEEPEFFECT_ATTACK;
        }
        break;
    case	STATUS_MAGICDEFENCE:
        {
            m_i64Effect	|= KEEPEFFECT_MAGICDEFENCE;
        }
        break;
    case	STATUS_REFLECT:
        {
            m_i64Effect	|= KEEPEFFECT_REFLECT;
        }
        break;
    case	STATUS_REFLECTMAGIC:
        {
            m_i64Effect	|= KEEPEFFECT_REFLECTMAGIC;
        }
        break;
    case	STATUS_MAGICDAMAGE:
        {
            m_i64Effect	|= KEEPEFFECT_MAGICDAMAGE;
        }
        break;
    case	STATUS_ATKSPEED:
        {
            m_i64Effect	|= KEEPEFFECT_ATKSPEED;
        }
        break;
    case	STATUS_LURKER:
        {
            m_i64Effect	|= KEEPEFFECT_LURKER;
        }
        break;
    case	STATUS_SLOWDOWN1:
        {
            m_i64Effect |= KEEPEFFECT_SLOWDOWN1;
        }
        break;
    case	STATUS_SLOWDOWN2:
        {
            m_i64Effect |= KEEPEFFECT_SLOWDOWN2;
        }
        break;
    case	STATUS_MAXLIFE:
        {
            m_i64Effect |= KEEPEFFECT_MAXLIFE;
        }
        break;
    case	STATUS_HIDDEN:
        {
            m_i64Effect |= KEEPEFFECT_HIDDEN;
        }
        break;
    case	STATUS_ADD_EXP:
        {
            m_i64Effect	|= KEEPEFFECT_ADD_EXP;
        }
        break;
        // 以下是结界系统相关的状态 ================
    case	STATUS_TEAMHEALTH:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAM_ALL;
            m_i64Effect	|= KEEPEFFECT_TEAM_HEALTH;
        }
        break;
    case	STATUS_TEAMATTACK:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAM_ALL;
            m_i64Effect	|= KEEPEFFECT_TEAM_ATTACK;
        }
        break;
    case	STATUS_TEAMDEFENCE:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAM_ALL;
            m_i64Effect	|= KEEPEFFECT_TEAM_DEFENCE;
        }
        break;
    case	STATUS_TEAMSPEED:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAM_ALL;
            m_i64Effect	|= KEEPEFFECT_TEAM_SPEED;
        }
        break;
    case	STATUS_TEAMEXP:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAM_ALL;
            m_i64Effect	|= KEEPEFFECT_TEAM_EXP;
        }
        break;
    case	STATUS_TEAMSPIRIT:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAM_ALL;
            m_i64Effect	|= KEEPEFFECT_TEAM_SPIRIT;
        }
        break;
    case	STATUS_TEAMCLEAN:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAM_ALL;
            m_i64Effect	|= KEEPEFFECT_TEAM_CLEAN;
        }
        break;
        // 以上是结界系统相关状态 ===============
    case	STATUS_HEAL:
        {
            m_i64Effect |= KEEPEFFECT_HEAL;
        }
        break;
    case	STATUS_FAINT:
        {
            m_i64Effect |= KEEPEFFECT_FAINT;
        }
        break;
    case	STATUS_TRUCULENCE:
        {
            m_i64Effect |= KEEPEFFECT_TRUCULENCE;
        }
        break;
    case	STATUS_DAMAGE:
        {
            m_i64Effect |= KEEPEFFECT_DAMAGE;
        }
        break;
    case	STATUS_ATKER_DAMAGE:
        {
            m_i64Effect |= KEEPEFFECT_ATKER_DAMAGE;
        }
        break;
    case	STATUS_CONFUSION:
        {
            m_i64Effect |= KEEPEFFECT_CONFUSION;
        }
        break;
    default:
        ASSERT(!"switch status");
    }
    if (bSynchro && i64OldEffect != GetEffect())
    {
        CMsgUserAttrib msg;
        if (msg.Create(GetID(), _USERATTRIB_KEEPEFFECT, GetEffect()))
        {
            if (STATUS_MAXLIFE == nStatus)
            {
                msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
                msg.Append(_USERATTRIB_LIFE, this->GetLife());
            }
            BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
    }
}


/////////////////////////////////////////////////////////////////////////////
void CNpc::ClsStatus(int nStatus, bool bSynchro)
{
    I64	i64OldEffect = GetEffect();
    switch (nStatus)
    {
    case	STATUS_NORMAL:
        {
            ASSERT(!"STATUS_NORMAL");
        }
        break;
    case	STATUS_DIE:
        {
            m_i64Effect	&= ~KEEPEFFECT_DIE;
        }
        break;
    case	STATUS_TEAMLEADER:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAMLEADER;
        }
        break;
    case	STATUS_CRIME:
        {
            m_i64Effect	&= ~KEEPEFFECT_CRIME;
        }
        break;
    case	STATUS_SYNCRIME:
        {
            m_i64Effect	&= ~KEEPEFFECT_SYNCRIME;
        }
        break;
    case	STATUS_POISON:
        {
            m_i64Effect	&= ~KEEPEFFECT_POISON;
        }
        break;
    case	STATUS_PKVALUE:
        {
        }
        break;
    case	STATUS_DEFENCE1:
        {
            m_i64Effect	&= ~KEEPEFFECT_DEFENCE1;
        }
        break;
    case	STATUS_DEFENCE2:
        {
            m_i64Effect	&= ~KEEPEFFECT_DEFENCE2;
        }
        break;
    case	STATUS_DEFENCE3:
        {
            m_i64Effect	&= ~KEEPEFFECT_DEFENCE3;
        }
        break;
    case	STATUS_FREEZE:
        {
            m_i64Effect &= ~KEEPEFFECT_FREEZE;
        }
        break;
    case	STATUS_SMOKE:
        {
            m_i64Effect &= ~KEEPEFFECT_SMOKE;
        }
        break;
    case	STATUS_DARKNESS:
        {
            m_i64Effect &= ~KEEPEFFECT_DARKNESS;
        }
        break;
    case	STATUS_PALSY:
        {
            m_i64Effect &= ~KEEPEFFECT_PALSY;
        }
        break;
    case	STATUS_ATTACK:
        {
            m_i64Effect	&= ~KEEPEFFECT_ATTACK;
        }
        break;
    case	STATUS_MAGICDEFENCE:
        {
            m_i64Effect	&= ~KEEPEFFECT_MAGICDEFENCE;
        }
        break;
    case	STATUS_REFLECT:
        {
            m_i64Effect	&= ~KEEPEFFECT_REFLECT;
        }
        break;
    case	STATUS_REFLECTMAGIC:
        {
            m_i64Effect	&= ~KEEPEFFECT_REFLECTMAGIC;
        }
        break;
    case	STATUS_MAGICDAMAGE:
        {
            m_i64Effect	&= ~KEEPEFFECT_MAGICDAMAGE;
        }
        break;
    case	STATUS_ATKSPEED:
        {
            m_i64Effect	&= ~KEEPEFFECT_ATKSPEED;
        }
        break;
    case	STATUS_LURKER:
        {
            m_i64Effect	&= ~KEEPEFFECT_LURKER;
        }
        break;
    case	STATUS_SLOWDOWN1:
        {
            m_i64Effect &= ~KEEPEFFECT_SLOWDOWN1;
        }
        break;
    case	STATUS_SLOWDOWN2:
        {
            m_i64Effect &= ~KEEPEFFECT_SLOWDOWN2;
        }
        break;
    case	STATUS_MAXLIFE:
        {
            m_i64Effect &= ~KEEPEFFECT_MAXLIFE;
        }
        break;
    case	STATUS_HIDDEN:
        {
            m_i64Effect &= ~KEEPEFFECT_HIDDEN;
        }
        break;
    case	STATUS_ADD_EXP:
        {
            m_i64Effect	&= ~KEEPEFFECT_ADD_EXP;
        }
        break;
        // 以下是结界系统相关的状态 ================
    case	STATUS_TEAMHEALTH:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAM_HEALTH;
        }
        break;
    case	STATUS_TEAMATTACK:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAM_ATTACK;
        }
        break;
    case	STATUS_TEAMDEFENCE:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAM_DEFENCE;
        }
        break;
    case	STATUS_TEAMSPEED:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAM_SPEED;
        }
        break;
    case	STATUS_TEAMEXP:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAM_EXP;
        }
        break;
    case	STATUS_TEAMSPIRIT:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAM_SPIRIT;
        }
        break;
    case	STATUS_TEAMCLEAN:
        {
            m_i64Effect	&= ~KEEPEFFECT_TEAM_CLEAN;
        }
        break;
        // 以上是结界系统相关状态 ===============
    case	STATUS_HEAL:
        {
            m_i64Effect &= ~KEEPEFFECT_HEAL;
        }
        break;
    case	STATUS_FAINT:
        {
            m_i64Effect &= ~KEEPEFFECT_FAINT;
        }
        break;
    case	STATUS_TRUCULENCE:
        {
            m_i64Effect &= ~KEEPEFFECT_TRUCULENCE;
        }
        break;
    case	STATUS_DAMAGE:
        {
            m_i64Effect &= ~KEEPEFFECT_DAMAGE;
        }
        break;
    case	STATUS_ATKER_DAMAGE:
        {
            m_i64Effect &= ~KEEPEFFECT_ATKER_DAMAGE;
        }
        break;
    case	STATUS_CONFUSION:
        {
            m_i64Effect &= ~KEEPEFFECT_CONFUSION;
        }
        break;
    default:
        ASSERT(!"switch status");
    }
    if (bSynchro && i64OldEffect != GetEffect())
    {
        CMsgUserAttrib msg;
        if (msg.Create(GetID(), _USERATTRIB_KEEPEFFECT, GetEffect()))
        {
            if (STATUS_MAXLIFE == nStatus)
            {
                msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
                msg.Append(_USERATTRIB_LIFE, this->GetLife());
            }
            BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
CItem* CNpc::Recycle(OBJID idUser)
{
    CHECKF(IsRecycleEnable());
    OBJID idType = GetInt(NPCDATA_ITEMTYPE);
    if (idType != 0)
    {
        CItem* pItem = CItem::CreateNew();
        IF_OK(pItem)
        {
            pItem->Create(GameDataDefault()->GetGameItemData(), idType, idUser, ITEMPOSITION_BACKPACK, GameDataDefault()->GetEudemonData());
            if (IsDynaNpc() && GetMaxLife() > 0 && GetLife() < GetMaxLife())
            {
                pItem->SetInt(ITEMDATA_DATA, GetLife());
            }
            return pItem;
        }
        else
        {
            pItem->ReleaseByOwner();
        }
    }
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////
bool CNpc::IsOwnerOf(CUser* pUser, bool bMateEnable/*=false*/)
{
    CHECKF(pUser);
    if (IsSynNpc() && GetInt(NPCDATA_OWNERID) == pUser->GetSynID() && pUser->GetSynRankShow() == RANK_LEADER)
    {
        return true;
    }
    if (IsUserNpc() && GetInt(NPCDATA_OWNERID) == pUser->GetID())
    {
        return true;
    }
    if (bMateEnable && IsUserNpc() && GetInt(NPCDATA_OWNERID) == pUser->GetMateID())
    {
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
bool CNpc::IsSynMoneyEmpty()
{
    CHECKF(IsSynFlag());
    CSyndicate* pSyn = SynManager()->QuerySyndicate(GetInt(NPCDATA_OWNERID));
    if (pSyn)
    {
        if (pSyn->GetInt(SYNDATA_MONEY) > 0)
        {
            return false;
        }
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CNpc::CheckSortMutex()
{
    DWORD	dwSort = GetSort() & NPCSORT_USELINKID;
    if (GetType() == _SYNTRANS_NPC && dwSort != 0)
    {
        return false;
    }
    for(int i = 0; i < 32; i++)
    {
        if (dwSort == 0)
        {
            return true;
        }
        if ((dwSort & 1) != 0 && (dwSort >>= 1) != 0)
        {
            ASSERT(!"_TBL_NPC(_TBL_DYNANPC) link id mutex!!!");
            return false;
        }
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
void CNpc::CheckFightTime()
{
    time_t	tCurr	= time(NULL);
    tm*	pTm			= localtime(&tCurr);
    CHECK(pTm);
    int nCurrTime = tCurr;
    /*	int nWeek		= pTm->tm_wday;
    	int	nHour		= pTm->tm_hour;
    	int nMinute		= pTm->tm_min;
    	if (nWeek == 0)
    		nWeek = 7;

    	int	nCurrTime	= nWeek*1000000 + nHour*10000 + nMinute*100;
    	int nStartTime	= (GetData(SYNFLAGNPC_FIGHTBEGIN)/100)*100;
    	int nEndTime	= (GetData(SYNFLAGNPC_FIGHTEND)/100)*100;
    	*/
    int nStartTime	= GetData(SYNFLAGNPC_FIGHTBEGIN);
    int nEndTime	= GetData(SYNFLAGNPC_FIGHTEND);
    if (nCurrTime >= nStartTime && nCurrTime < nEndTime)
    {
        if (!GetMap()->IsWarTime())
        {
            GetMap()->SetStatus(CGameMap::STATUS_WAR, true);
            CMsgTalk	msg;
            IF_OK_(msg.Create(STR_SYN_WAR_START))
            UserManager()->BroadcastMapMsg(GetMap()->GetID(), &msg);
        }
    }
    else // if (!( 	nCurrTime >= nStartTime && nCurrTime < nEndTime ))
    {
        if (GetMap()->IsWarTime())
        {
            //攻城战结束后的处理
            OnFightEnd();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
void CNpc::RecruitLife()
{
    if (IsAlive() && GetLife() < GetMaxLife() && GetData(SYNFLAGNPC_FIXFUND) > 0)
    {
        int nFixMoney = ::CutOverflow(GetData(SYNFLAGNPC_FIXFUND), MAX_FIXMONEY);
        int nLoseLife = GetMaxLife() - GetLife();
        nFixMoney = ::CutOverflow(nFixMoney, (nLoseLife - 1) / FIXLIFE_PER_MONEY + 1);
        SetData(SYNFLAGNPC_FIXFUND, ::CutTrail(GetData(SYNFLAGNPC_FIXFUND) - nFixMoney, 0), UPDATE_FALSE);
        if (GetSynID() == ID_NONE)
        {
            return ;
        }
        CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());
        if (!pSyn)
        {
            return ;
        }
        if (pSyn->GetInt(SYNDATA_MONEY) < nFixMoney)
        {
            nFixMoney = pSyn->GetInt(SYNDATA_MONEY);
        }
        if (nFixMoney <= 0)
        {
            SetData(SYNFLAGNPC_FIXFUND, 0, UPDATE_FALSE);
            return ;
        }
        int nFixLife = ::CutOverflow(nLoseLife, nFixMoney * FIXLIFE_PER_MONEY);
        pSyn->QueryModify()->AddData(SYNDATA_MONEY, -1 * nFixMoney);
        AddAttrib(_USERATTRIB_LIFE, nFixLife, SYNCHRO_BROADCAST);
        // effect
        CMsgMagicEffect	msg;
        IF_OK(msg.Create(GetID(), MAGICSORT_RECRUIT, 0, GetID(), nFixLife, GetDir()))
        BroadcastRoomMsg(&msg);
    }
}

/////////////////////////////////////////////////////////////////////////////
#include <map>
using namespace std;
void CNpc::BroadcastScore()
{
    typedef	multimap<int, OBJID>	TOP_SET;
    TOP_SET	setTop;
    OBJID idNpc = GetID();
    int i = 0;
    for(; i < SynManager()->QuerySynSet()->GetAmount(); i++)
    {
        CSyndicate* pSyn = SynManager()->QuerySynSet()->GetObjByIndex(i);
        if (pSyn)
        {
            setTop.insert(TOP_SET::value_type(pSyn->GetSynWarScore(idNpc), pSyn->GetID()));
        }
    }
    int	nChannel = _TXTATR_SYNWAR_FIRST;
    i = 1;
    for(TOP_SET::reverse_iterator iter = setTop.rbegin(); iter != setTop.rend() && i <= 5; iter++, i++)
    {
        OBJID	idSyn	= iter->second;
        int		nScore	= iter->first;
        if (nScore == 0)
        {
            break;
        }
        CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
        if (!pSyn)
        {
            continue;
        }
        MSGBUF	szMsg;
        sprintf(szMsg, STR_TOP_LIST_dsd, i, pSyn->GetStr(SYNDATA_NAME), nScore);
        CMsgTalk	msg;
        IF_OK(msg.Create(nChannel, szMsg))
        UserManager()->BroadcastMapMsg(GetMap()->GetID(), &msg);
        nChannel = _TXTATR_SYNWAR_NEXT;
    }
}

/////////////////////////////////////////////////////////////////////////////
LPCTSTR	CNpc::GetName()
{
    if (IsSynFlag() && GetInt(NPCDATA_OWNERID) != ID_NONE)
    {
        CSyndicate* pSyn = SynManager()->QuerySyndicate(GetInt(NPCDATA_OWNERID));
        if (pSyn)
        {
            return pSyn->GetStr(SYNDATA_NAME);
        }
    }
    return m_pData->GetStr(NPCDATA_NAME);
}

/////////////////////////////////////////////////////////////////////////////
bool CNpc::IsBeAtkable()
{
    if (IsSynFlag() && !GetMap()->IsWarTime())
    {
        return false;
    }
    return IsDynaNpc() && GetMaxLife() > 0;
}

/////////////////////////////////////////////////////////////////////////////
bool CNpc::SetSynOwnerID(OBJID idSyn, BOOL bWithLinkMap)		// true: with link map
{
    if (GetInt(NPCDATA_OWNERTYPE) != OWNER_SYN)
    {
        return false;
    }
    SetInt(NPCDATA_OWNERID, idSyn, true);
    if (bWithLinkMap && IsLinkNpc())
    {
        CGameMap* pMap = QueryLinkMap()->GetLinkMap();
        IF_OK(pMap)
        pMap->SetSynID(idSyn, true);		// true: with all npc in map
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CNpc::SetUserOwnerID(OBJID idUser, BOOL bWithLinkMap/*=false*/)		// true: with link map
{
    if (GetInt(NPCDATA_OWNERTYPE) != OWNER_USER)
    {
        return false;
    }
    SetInt(NPCDATA_OWNERID, idUser, true);
    if (bWithLinkMap && IsLinkNpc())
    {
        CGameMap* pMap = QueryLinkMap()->GetLinkMap();
        IF_OK(pMap)
        pMap->SetUserID(idUser, true);		// true: with all npc in map
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CNpc::IsDelEnable()
{
    if ( (GetSort() & NPCSORT_DELENABLE) == 0 )
    {
        return false;
    }
    if (IsTrunkNpc() && !QueryTrunk()->IsEmpty())
    {
        return false;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
void CNpc::UpdateHoldFloor(bool bEnter)
{
    CHECK(GetMap());
    int nDirFront	= GetDir();
    int nDirBack	= (GetDir() + 4) % 8;
    int nDirLeft	= (GetDir() + 8 - 2) % 8;
    int nDirRight	= (GetDir() + 2) % 8;
    POINT posFront, posBack, posLeft, posRight;
    posFront.x = GetPosX() + _DELTA_X[nDirFront] * this->GetInt(NPCDATA_SIZE_FRONT);
    posFront.y = GetPosY() + _DELTA_Y[nDirFront] * this->GetInt(NPCDATA_SIZE_FRONT);
    posBack.x = GetPosX() + _DELTA_X[nDirBack] * this->GetInt(NPCDATA_SIZE_BACK);
    posBack.y = GetPosY() + _DELTA_Y[nDirBack] * this->GetInt(NPCDATA_SIZE_BACK);
    posLeft.x = GetPosX() + _DELTA_X[nDirLeft] * this->GetInt(NPCDATA_SIZE_LEFT);
    posLeft.y = GetPosY() + _DELTA_Y[nDirLeft] * this->GetInt(NPCDATA_SIZE_LEFT);
    posRight.x = GetPosX() + _DELTA_X[nDirRight] * this->GetInt(NPCDATA_SIZE_RIGHT);
    posRight.y = GetPosY() + _DELTA_Y[nDirRight] * this->GetInt(NPCDATA_SIZE_RIGHT);
    int nMinX = __min(__min(posFront.x, posBack.x), __min(posLeft.x, posRight.x));
    int nMinY = __min(__min(posFront.y, posBack.y), __min(posLeft.y, posRight.y));
    int nMaxX = __max(__max(posFront.x, posBack.x), __max(posLeft.x, posRight.x));
    int nMaxY = __max(__max(posFront.y, posBack.y), __max(posLeft.y, posRight.y));
    for (int i = nMinX; i <= nMaxX; i++)
    {
        for (int j = nMinY; j <= nMaxY; j++)
        {
            if (GetMap()->IsValidPoint(i, j)
                    && !(GetPosX() == i && GetPosY() == j))
            {
                if (bEnter)
                {
                    GetMap()->IncRole(i, j);
                }
                else
                {
                    GetMap()->DecRole(i, j);
                }
            }
        }
    }
}



/////////////////////////////////////////////////////////////////////////////
//---jinggy---
OBJID CNpc::GetApplyAttackerSynID()
{
    return this->GetInt(NPCDATA_DATA1);
}
//---jinggy---
//对战争结束后的处理
void CNpc::OnFightEnd()
{
    GetMap()->SetStatus(CGameMap::STATUS_WAR, false);
    CMsgTalk	msg;
    IF_OK_(msg.Create(STR_SYN_WAR_END))
    UserManager()->BroadcastMapMsg(GetMap()->GetID(), &msg);
    // stop all auto battal
    CRoleSet::Iterator pRole = RoleManager()->QuerySet()->NewEnum();
    while(pRole.Next())
    {
        if (pRole  && pRole->GetMap()->GetID() == this->GetMapID())
        {
            pRole->ClrAttackTarget();
        }
    }
    //---jinggy---进行　战争后的条约处理---begin
    OBJID idSynApply = GetMap()->GetNpcSynWarLetter()->GetApplyAttackerSynID();//申请攻击的帮派ＩＤ
    if ( idSynApply == this->GetSynID() || idSynApply < 0)
    {
        return;
    }
    OBJID idOldSyn = GetSynID();
    int nScale = 0, nWarValue = 0;
    //得到 所有帮派NPC的血的丢失和 比例
    this->GetMap()->GetAllNpcLostLifeScaleAndValue(nScale, nWarValue);
    CNpc* pWarLetterNpc = GetMap()->GetNpcSynWarLetter();
    int nMoney = 0;
    IF_OK(pWarLetterNpc)
    {
        nMoney = pWarLetterNpc->GetData(SYNWARLETTER_DEPOSIT);//押金
    }
    if (nScale >= 50) //攻击方胜利，但没有占领SYNDATA_MONEY
    {
        CSyndicate* pSyn = SynManager()->QuerySyndicate(idSynApply);
        nMoney = ::AddToTop(MulDiv(nWarValue, 3, 10), nMoney, SYNMONEY_LIMIT);
        pSyn->QueryModify()->AddData(SYNDATA_MONEY, nMoney, true);
    }
    else//守护方胜利：攻击方赔偿70%，系统补偿50%。
    {
        int nMoneyApplySyn = nMoney;
        CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());
        nMoney = ::AddToTop(MulDiv(nMoney, 70, 100), MulDiv(nWarValue, 50, 100), SYNMONEY_LIMIT);
        pSyn->QueryModify()->AddData(SYNDATA_MONEY, __max(0, nMoney), true);
        pSyn = SynManager()->QuerySyndicate(idSynApply);
        pSyn->QueryModify()->AddData(SYNDATA_MONEY, ::CutRange(MulDiv(nMoneyApplySyn, 30, 100), 0, SYNMONEY_LIMIT), true);
    }
    //---jinggy---进行　战争后的条约处理---end
}
//---jinggy---
//得到修理 最大生命值 所需要的钱
int CNpc::GetMaxFixMoney()
{
    return  ::CutRange(MulDiv(GetMaxLife() - 1, 1, FIXLIFE_PER_MONEY) + 1, 0, SYNMONEY_LIMIT);
}
//---jinggy---
//将丢失的生命值计算成钱
int CNpc::GetLostFixMoney()
{
    int nLostLifeTmp = GetMaxLife() - GetLife() ;
    return  ::CutRange(MulDiv(nLostLifeTmp - 1, 1, FIXLIFE_PER_MONEY) + 1, 0, SYNMONEY_LIMIT);
}
//---jinggy---
//将占领的帮派地图的时间存放到 战书NPC中
void CNpc::SetOccupyTime()
{
    if (!this->IsSynWarLetter())
    {
        return;
    }
    time_t	tCur;
    tCur = time(NULL);
    SetData(SYNWARLETTER_OCCUPYTIME, (int)tCur, true);
}
