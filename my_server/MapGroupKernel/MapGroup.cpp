
//**********************************************************
// 代码编辑器
//**********************************************************

// UserManager.cpp: implementation of the CMapGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "MessagePort.h"
#pragma warning(disable:4786)
#include "AllMsg.h"
#include "MapGroup.h"
#pragma warning(disable:4786)
#include "protocol.h"
#include "itemtype.h"
#include "MapManager.h"
#include "ActionDefine.h"
#include "WantedList.h"
#include "DeadLoop.h"

//#include "DropRuleMap.h"	//add by zlong 2003-11-15

//MYHEAP_IMPLEMENTATION(CMapGroup,s_heap)
//////////////////////////////////////////////////////////////////////
// 全局对象
CMapGroup::MAPGROUP_SET		CMapGroup::m_setMapGroup;
CItemType*		g_pItemType		= NULL;

CDropRuleMap*	g_pDropRuleMap	= NULL;		//add by zlong 2003-11-15

IStaticMapSet*	g_setStaticMap	= NULL;
ITaskSet*		g_setTask		= NULL;
IActionSet*		g_setAction		= NULL;
ILevupexpSet*	g_setLevupexp	= NULL;
IMagicTypeSet*	g_setMagicType	= NULL;
IMagicTrackSet*	g_setMagicTrack	= NULL;
IMagicTypeSet*	g_setAutoMagicType	= NULL;
IAddPointSet*	g_setAddPoint	= NULL;
ITrapTypeSet*	g_setTrapType	= NULL;
IMonsterTypeSet*	g_setMonsterType	= NULL;
IRebirthSet*	g_setRebirth	= NULL;
CItemAddition*	g_pItemAddition	= NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMapGroup::CMapGroup()
{
    m_idProcess		= NULL;
    m_pSocket		= NULL;
    m_pDb			= NULL;
    m_pMsgPort		= NULL;
    m_pRoleManager	= NULL;
    m_pUserManager	= NULL;
    m_pMapManager	= NULL;
    m_pNpcManager	= NULL;
    m_pSynManager	= NULL;
    m_pLeaveWord	= NULL;
    m_pGameAction	= NULL;
    m_pMercenaryTask = NULL;
    m_pAnnounce     = NULL;
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::Create(PROCESS_ID idProcess, ISocket* pSocket, IDatabase* pDb, IMessagePort* pMsgPort)
{
    m_idProcess	= idProcess;
    m_pSocket	= pSocket;
    m_pDb		= pDb;
    m_pMsgPort	= pMsgPort;
    // 初始化所有DATA类的表结构。(注：所有要写盘的DATA类都需要在此初始化，并同时在Destroy()中清除。)
    char szSQL[11][1024];
    sprintf(szSQL[0], "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_ITEM);
    sprintf(szSQL[1], "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_USER);
    sprintf(szSQL[2], "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_MAP);
    sprintf(szSQL[3], "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_WEAPONSKILL);
    sprintf(szSQL[4], "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_MAGIC);
    sprintf(szSQL[5], "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_SYNDICATE);
    sprintf(szSQL[6], "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_SYNATTR);
    sprintf(szSQL[7], "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_PET);
    sprintf(szSQL[8], "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_DYNANPC);
    sprintf(szSQL[9], "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_DYNAMAP);
    sprintf(szSQL[10], "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_EUDEMON);
    if (!((m_pGameItemDataDefault			= Database()->CreateNewRecordset(szSQL[0], false))
			&& (m_pUserDataDefault			= Database()->CreateNewRecordset(szSQL[1], false))
            && (m_pGameMapDataDefault		= Database()->CreateNewRecordset(szSQL[2], false))
            && (m_pUserWeaponSkillDefault	= Database()->CreateNewRecordset(szSQL[3], false))
            && (m_pMagicDataDefault			= Database()->CreateNewRecordset(szSQL[4], false))
            && (m_pSynDataDefault			= Database()->CreateNewRecordset(szSQL[5], false))
            && (m_pSynAttrDataDefault		= Database()->CreateNewRecordset(szSQL[6], false))
            && (m_pPetDataDefault			= Database()->CreateNewRecordset(szSQL[7], false))
            && (m_pDynaNpcDataDefault		= Database()->CreateNewRecordset(szSQL[8], false))
            && (m_pDynaMapDataDefault		= Database()->CreateNewRecordset(szSQL[9], false))
            && (m_pEudemonDataDefault		= Database()->CreateNewRecordset(szSQL[10], false))
        ))
    {
        return false;
    }
    // 创建全局常量对象
    if (m_idProcess == MSGPORT_MAPGROUP_FIRST)
    {
        g_pItemType = CItemType::CreateNew();
        IF_NOT(g_pItemType && g_pItemType->Create(pDb))
        return false;
        //add by zlong 2003-11-15
        g_pDropRuleMap = CDropRuleMap::CreateNew();
        IF_NOT(g_pDropRuleMap && g_pDropRuleMap->Create(pDb))
        return false;
        char	szSQL[1024];
        sprintf(szSQL, "SELECT * FROM %s", _TBL_MAP);
        g_setStaticMap = CStaticMapSet::CreateNew(true);
        IF_NOT_(g_setStaticMap && g_setStaticMap->Create(szSQL, pDb))
        return false;
        sprintf(szSQL, "SELECT * FROM %s", _TBL_TASK);
        g_setTask	= CTaskSet::CreateNew(true);
        IF_NOT_(g_setTask && g_setTask->Create(szSQL, Database()))
        return false;
        sprintf(szSQL, "SELECT * FROM %s", _TBL_ACTION);
        g_setAction	= CActionSet::CreateNew(true);
        IF_NOT_(g_setAction && g_setAction->Create(szSQL, Database()))
        return false;
        sprintf(szSQL, "SELECT * FROM %s", _TBL_LEVEXP);
        g_setLevupexp	= CLevupexpSet::CreateNew(true);
        IF_NOT_(g_setLevupexp && g_setLevupexp->Create(szSQL, Database()))
        return false;
        sprintf(szSQL, "SELECT * FROM %s", _TBL_POINTALLOT);
        g_setAddPoint	= CAddPointSet::CreateNew(true);
        IF_NOT_(g_setAddPoint && g_setAddPoint->Create(szSQL, Database()))
        return false;
        sprintf(szSQL, "SELECT * FROM %s", _TBL_TRACK);
        g_setMagicTrack	= CMagicTrackSet::CreateNew(true);
        IF_NOT (g_setMagicTrack && g_setMagicTrack->Create(szSQL, Database()))
        return false;
        sprintf(szSQL, "SELECT * FROM %s", _TBL_MAGICTYPE);
        g_setMagicType	= CMagicTypeSet::CreateNew(true);
        IF_NOT_(g_setMagicType && g_setMagicType->Create(szSQL, Database()))
        return false;
        sprintf(szSQL, "SELECT * FROM %s WHERE auto_learn!=0", _TBL_MAGICTYPE);
        g_setAutoMagicType	= CMagicTypeSet::CreateNew(true);
        IF_NOT_(g_setAutoMagicType && g_setAutoMagicType->Create(szSQL, Database()))
        return false;
        IF_NOT(CWantedList::s_WantedList.Create(Database()))
        return false;
        sprintf(szSQL, "SELECT * FROM %s", _TBL_TRAPTYPE);
        g_setTrapType	= CTrapTypeSet::CreateNew(true);
        IF_NOT_(g_setTrapType && g_setTrapType->Create(szSQL, Database()))
        return false;
        sprintf(szSQL, "SELECT * FROM %s", _TBL_MONSTERTYPE);
        g_setMonsterType	= CMonsterTypeSet::CreateNew(true);
        IF_NOT_(g_setMonsterType && g_setMonsterType->Create(szSQL, Database()))
        return false;
        sprintf(szSQL, "SELECT * FROM %s", _TBL_REBIRTH);
        g_setRebirth	= CRebirthSet::CreateNew(true);
        IF_NOT_(g_setRebirth && g_setRebirth->Create(szSQL, Database()))
        return false;
        g_pItemAddition	= CItemAddition::CreateNew();
        IF_NOT(g_pItemAddition && g_pItemAddition->Create(pDb))
        return false;
        /*		sprintf(szSQL, "DELETE FROM %s WHERE state = 2", _TBL_AUCTION);//LW每次起动服务器时删除已拍买过的物品
        		Database()->ExecuteSQL(szSQL);*/
    }
    CMapManager* pMapManager = new CMapManager(m_idProcess);
    if (!pMapManager || !pMapManager->Create())
    {
        ASSERT(!"pMapManager->Create");
        return false;
    }
    m_pMapManager = pMapManager->GetInterface();
    CRoleManager* pRoleManager = new CRoleManager(m_idProcess);
    if (!pRoleManager || !pRoleManager->Create())
    {
        ASSERT(!"pRoleManager->Create");
        return false;
    }
    m_pRoleManager = pRoleManager->GetInterface();
    CUserManager* pUserManager = new CUserManager(m_idProcess);
    if (!pUserManager || !pUserManager->Create())
    {
        ASSERT(!"pUserManager->Create");
        return false;
    }
    m_pUserManager = pUserManager->GetInterface();
    m_pSynManager = new CSynManager(m_idProcess);
    if (!m_pSynManager || !m_pSynManager->Create())
    {
        ASSERT(!"m_pSynManager->Create");
        return false;
    }
    CNpcManager* pNpcManager = new CNpcManager(m_idProcess);
    if (!pNpcManager || !pNpcManager->Create())
    {
        ASSERT(!"pNpcManager->Create");
        return false;
    }
    m_pNpcManager = pNpcManager->GetInterface();
    m_pLeaveWord = new CLeaveWord();
    if (!m_pLeaveWord || !m_pLeaveWord->Create(m_idProcess))
    {
        ASSERT(!"m_pLeaveWord->Create");
        return false;
    }
    m_pGameAction = CGameAction::CreateNew(m_idProcess);
    if (!m_pGameAction)
    {
        ASSERT(!"m_pGameAction->Create");
        return false;
    }
    m_pMercenaryTask = CMercenaryTask::CreateNew();
    IF_NOT(m_pMercenaryTask && m_pMercenaryTask->Create(m_idProcess))
    return false;
    m_pAnnounce = CAnnounce::CreateNew();
    if (!m_pAnnounce)
    {
        ASSERT(!"CAnnounce::CreateNew()");
        return false;
    }
    CONST int INTERVAL_EVENT = 60;	// seconds
    m_tmEvent.Startup(INTERVAL_EVENT);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::Destroy()
{
    SAFE_RELEASE (m_pMercenaryTask);
    SAFE_RELEASE (m_pGameAction);
    SAFE_RELEASE (m_pLeaveWord);
    SAFE_RELEASE (m_pNpcManager);
    SAFE_RELEASE (m_pUserManager);
    SAFE_RELEASE (m_pRoleManager);
    SAFE_RELEASE (m_pSynManager);
    SAFE_RELEASE (m_pMapManager);
    SAFE_RELEASE (m_pAnnounce);
    // 清除全局常量对象
    if (m_idProcess == MSGPORT_MAPGROUP_FIRST)		//@ 可能有问题
    {
        CWantedList::s_WantedList.Release();
        SAFE_RELEASE (g_setRebirth);
        SAFE_RELEASE (g_setMonsterType);
        SAFE_RELEASE (g_setTrapType);
        SAFE_RELEASE (g_setMagicTrack);
        SAFE_RELEASE (g_setAutoMagicType);
        SAFE_RELEASE (g_setMagicType);
        SAFE_RELEASE (g_setAddPoint);
        SAFE_RELEASE (g_setLevupexp);
        SAFE_RELEASE(g_setAction);
        SAFE_RELEASE(g_setTask);
        SAFE_RELEASE (g_setStaticMap);
        SAFE_RELEASE (g_pItemType);
        // add by zlong 2003-11-15
        SAFE_RELEASE (g_pDropRuleMap);
        SAFE_RELEASE (g_pItemAddition);
    }
    SAFE_RELEASE (m_pEudemonDataDefault);
    SAFE_RELEASE (m_pDynaMapDataDefault);
    SAFE_RELEASE (m_pDynaNpcDataDefault);
    SAFE_RELEASE (m_pPetDataDefault);
    SAFE_RELEASE (m_pSynAttrDataDefault);
    SAFE_RELEASE (m_pSynDataDefault);
    SAFE_RELEASE (m_pMagicDataDefault);
    SAFE_RELEASE (m_pUserWeaponSkillDefault);
    SAFE_RELEASE (m_pGameItemDataDefault);
    SAFE_RELEASE (m_pUserDataDefault);
    SAFE_RELEASE (m_pGameMapDataDefault);
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::OnTimer(time_t tCurr)
{
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "RoleManager()->OnTimer")
    RoleManager()->OnTimer(tCurr);
    DEBUG_CATCH("RoleManager()->OnTimer(tCurr);")
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "MapManager()->OnTimer")
    MapManager()->OnTimer(tCurr);
    DEBUG_CATCH("MapManager()->OnTimer(tCurr);")
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "UserManager()->OnTimer")
    UserManager()->OnTimer(tCurr);
    DEBUG_CATCH("UserManager()->OnTimer(tCurr);")
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "LeaveWord()->OnTimer")
    LeaveWord()->OnTimer(tCurr);
    DEBUG_CATCH("LeaveWord()->OnTimer(tCurr);")
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "ProcessEvent")
    clock_t	tStart = clock();
    if (m_tmEvent.ToNextTime())
    {
        this->ProcessEvent();
    }
    clock_t tUsed = clock() - tStart;
    if (tUsed > 1000)
    {
        ASSERT(!"EVENT定时任务超时!");
    }
    DEBUG_CATCH("ProcessEvent();")
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "MercenaryTask()->OnTimer")
    MercenaryTask()->OnTimer(tCurr);
    DEBUG_CATCH("MercenaryTask()->Ontimer(tCurr);")
}

//////////////////////////////////////////////////////////////////////
BOOL CMapGroup::CheckTime(const DWORD dwType, const char* pszParam)
{
    if (!pszParam)
    {
        return false;
    }
    time_t ltime;
    time( &ltime );
    struct tm* pTime;
    pTime = localtime( &ltime ); /* Convert to local time. */
    int nCurYear, nCurMonth, nCurDay, nCurWeekDay, nCurYearDay, nCurHour, nCurMinute;
    nCurYear	= pTime->tm_year + 1900;
    nCurMonth	= pTime->tm_mon + 1;
    nCurDay		= pTime->tm_mday;
    nCurWeekDay	= pTime->tm_wday;
    nCurYearDay	= pTime->tm_yday;
    nCurHour	= pTime->tm_hour;
    nCurMinute	= pTime->tm_min;
    int nYear0 = 0, nMonth0 = 0, nDay0 = 0, nHour0 = 0, nMinute0 = 0;
    int nYear1 = 0, nMonth1 = 0, nDay1 = 0, nHour1 = 0, nMinute1 = 0;
    if (dwType == 0)			//精确时间
    {
        if (10 == sscanf(pszParam, "%d-%d-%d %d:%d %d-%d-%d %d:%d",
                         &nYear0, &nMonth0, &nDay0, &nHour0, &nMinute0,
                         &nYear1, &nMonth1, &nDay1, &nHour1, &nMinute1))
        {
            struct tm time0, time1;
            memset(&time0, 0L, sizeof(tm));
            memset(&time1, 0L, sizeof(tm));
            time0.tm_year	= nYear0 - 1900;
            time0.tm_mon	= nMonth0 - 1;
            time0.tm_mday	= nDay0;
            time0.tm_hour	= nHour0;
            time0.tm_min	= nMinute0;
            time0.tm_isdst	= pTime->tm_isdst;
            time1.tm_year	= nYear1 - 1900;
            time1.tm_mon	= nMonth1 - 1;
            time1.tm_mday	= nDay1;
            time1.tm_hour	= nHour1;
            time1.tm_min	= nMinute1;
            time1.tm_isdst	= pTime->tm_isdst;
            time_t t0	= mktime(&time0);
            time_t t1	= mktime(&time1);
            if (ltime >= t0 && ltime <= t1)
            {
                return true;
            }
        }
    }
    else if (dwType == 1)	//年时间
    {
        if (8 == sscanf(pszParam, "%d-%d %d:%d %d-%d %d:%d",
                        &nMonth0, &nDay0, &nHour0, &nMinute0,
                        &nMonth1, &nDay1, &nHour1, &nMinute1))
        {
            struct tm time0, time1;
            memset(&time0, 0L, sizeof(tm));
            memset(&time1, 0L, sizeof(tm));
            time0.tm_year	= pTime->tm_year;
            time0.tm_mon	= nMonth0 - 1;
            time0.tm_mday	= nDay0;
            time0.tm_hour	= nHour0;
            time0.tm_min	= nMinute0;
            time0.tm_isdst	= pTime->tm_isdst;
            time1.tm_year	= pTime->tm_year;
            time1.tm_mon	= nMonth1 - 1;
            time1.tm_mday	= nDay1;
            time1.tm_hour	= nHour1;
            time1.tm_min	= nMinute1;
            time1.tm_isdst	= pTime->tm_isdst;
            time_t t0	= mktime(&time0);
            time_t t1	= mktime(&time1);
            if (ltime >= t0 && ltime <= t1)
            {
                return true;
            }
        }
    }
    else if (dwType == 2)	//月时间
    {
        if (6 == sscanf(pszParam, "%d %d:%d %d %d:%d",
                        &nDay0, &nHour0, &nMinute0,
                        &nDay1, &nHour1, &nMinute1))
        {
            struct tm time0, time1;
            memset(&time0, 0L, sizeof(tm));
            memset(&time1, 0L, sizeof(tm));
            time0.tm_year	= pTime->tm_year;
            time0.tm_mon	= pTime->tm_mon;
            time0.tm_mday	= nDay0;
            time0.tm_hour	= nHour0;
            time0.tm_min	= nMinute0;
            time0.tm_isdst	= pTime->tm_isdst;
            time1.tm_year	= pTime->tm_year;
            time1.tm_mon	= pTime->tm_mon;
            time1.tm_mday	= nDay1;
            time1.tm_hour	= nHour1;
            time1.tm_min	= nMinute1;
            time1.tm_isdst	= pTime->tm_isdst;
            time_t t0	= mktime(&time0);
            time_t t1	= mktime(&time1);
            if (ltime >= t0 && ltime <= t1)
            {
                return true;
            }
        }
    }
    else if (dwType == 3)	//周时间
    {
        if (6 == sscanf(pszParam, "%d %d:%d %d %d:%d",
                        &nDay0, &nHour0, &nMinute0,
                        &nDay1, &nHour1, &nMinute1))
        {
            DWORD timeNow	= nCurWeekDay * 24 * 60 + nCurHour * 60 + nCurMinute;
            if (timeNow >= nDay0 * 24 * 60 + nHour0 * 60 + nMinute0 &&
                    timeNow <= nDay1 * 24 * 60 + nHour1 * 60 + nMinute1)
            {
                return true;
            }
        }
    }
    else if (dwType == 4)	//日时间
    {
        if (4 == sscanf(pszParam, "%d:%d %d:%d",
                        &nHour0, &nMinute0,
                        &nHour1, &nMinute1))
        {
            DWORD timeNow	= nCurHour * 60 + nCurMinute;
            if (timeNow >= nHour0 * 60 + nMinute0 &&
                    timeNow <= nHour1 * 60 + nMinute1)
            {
                return true;
            }
        }
    }
    else if (dwType == 5)	//小时时间
    {
        if (2 == sscanf(pszParam, "%d %d",
                        &nMinute0,
                        &nMinute1))
        {
            if (nCurMinute >= nMinute0 && nCurMinute <= nMinute1)
            {
                return true;
            }
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::ProcessEvent(void)
{
    CONST OBJID ID_EVENT_BEGIN	= 2000000;
    CONST DWORD MAX_EVENT		= 100;
    OBJID idBegin = ID_EVENT_BEGIN + m_idProcess * 10000;
    for (OBJID id = idBegin; id < idBegin + MAX_EVENT; id++)
    {
        //this->ProcessAction(id);
        if (ActionSet()->GetObj(id))
        {
            GameAction()->ProcessAction(id);
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::SendClientMsg(SOCKET_ID idSocket, CNetMsg* pMsg)
{
    pMsg->AppendInfo(m_idProcess, idSocket, ID_NONE);
    return m_pSocket->SendMsg(pMsg);
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::SendNpcMsg(OBJID idNpc, CNetMsg* pMsg)
{
    pMsg->AppendInfo(m_idProcess, SOCKET_NONE, idNpc);
    return m_pSocket->SendMsg(pMsg);
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::SendBroadcastMsg(CNetMsg* pMsg)
{
    for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
    {
        if (i != m_idProcess)		// 不回送
        {
            MESSAGESTR	buf;
            CLIENTMSG_PACKET0* pPacket = (CLIENTMSG_PACKET0*)buf;
            pPacket->idSocket	= pMsg->GetSocketID();
            pPacket->idPacket	= pMsg->GetType();
            memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
            pPacket->nSize		= pMsg->GetSize();
            m_pMsgPort->Send(i, MAPGROUP_BROADCASTMSG, STRUCT_TYPE(buf), pPacket);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
// 广播给其它地图组核心处理
bool CMapGroup::TransmitMsg(CNetMsg* pMsg, SOCKET_ID idSocket, OBJID idNpc)
{
    ASSERT(idSocket != SOCKET_NONE || idNpc != ID_NONE);
    MESSAGESTR	buf;
    TRANSMSG_PACKET0* pPacket = (TRANSMSG_PACKET0*)buf;
    pPacket->idSocket	= idSocket;
    pPacket->idNpc		= idNpc;
    pPacket->idPacket	= pMsg->GetType();
    memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
    pPacket->nSize		= pMsg->GetSize();
    pPacket->nTrans		= pMsg->GetTransData();
    for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
    {
        if (i != m_idProcess)		// 不回送
        {
            m_pMsgPort->Send(i, MAPGROUP_TRANSMITMSG, STRUCT_TYPE(buf), pPacket);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
// 广播给其它地图组核心处理
void CMapGroup::RemoteCall(REMOTE_CALL0* pInfo, bool bSendToLocal)
{
    for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
    {
        if (i != m_idProcess || bSendToLocal)
        {
            m_pMsgPort->Send(i, MAPGROUP_REMOTECALL, BUFFER_TYPE(pInfo->nSize), pInfo);
        }
    }
}

//////////////////////////////////////////////////////////////////////
// 广播给世界核心处理
bool CMapGroup::TransmitWorldMsg(CNetMsg* pMsg)
{
    MESSAGESTR	buf;
    TRANSMSG_PACKET0* pPacket = (TRANSMSG_PACKET0*)buf;
    pPacket->idSocket	= pMsg->GetSocketID();
    pPacket->idNpc		= pMsg->GetNpcID();
    pPacket->idPacket	= pMsg->GetType();
    memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
    pPacket->nSize		= pMsg->GetSize();
    pPacket->nTrans		= pMsg->GetTransData();
    m_pMsgPort->Send(MSGPORT_WORLD, WORLD_TRANSMITMSG, STRUCT_TYPE(buf), pPacket);
    return true;
}

//////////////////////////////////////////////////////////////////////
// 通过世界核心转发给玩家
bool CMapGroup::TransmitWorldMsg(CNetMsg* pMsg, LPCTSTR szName)
{
    MESSAGESTR	buf;
    USERNAMEMSG_PACKET0* pPacket = (USERNAMEMSG_PACKET0*)buf;
    pPacket->idSocket	= pMsg->GetSocketID();
    pPacket->idPacket	= pMsg->GetType();
    memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
    pPacket->nSize		= pMsg->GetSize();
    SafeCopy(pPacket->szName, szName, _MAX_NAMESIZE);
    m_pMsgPort->Send(MSGPORT_WORLD, WORLD_USERNAMEMSG, STRUCT_TYPE(buf), pPacket);
    return true;
}

//////////////////////////////////////////////////////////////////////
// 通过世界核心转发给玩家
bool CMapGroup::TransmitWorldMsg(CNetMsg* pMsg, OBJID idUser)
{
    MESSAGESTR	buf;
    USERIDMSG_PACKET0* pPacket = (USERIDMSG_PACKET0*)buf;
    pPacket->idSocket	= pMsg->GetSocketID();
    pPacket->idPacket	= pMsg->GetType();
    memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
    pPacket->nSize		= pMsg->GetSize();
    pPacket->idUser		= idUser;
    m_pMsgPort->Send(MSGPORT_WORLD, WORLD_USERIDMSG, STRUCT_TYPE(buf), pPacket);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::SendSocketUserInfo(PROCESS_ID idProcess, SOCKET_ID idSocket, UserInfoStruct* pInfo)
{
    // 通知另一地图组
    MESSAGESTR	buf;
    ST_USERCHANGEMAPGORUP* pPacket = (ST_USERCHANGEMAPGORUP*)buf;
    pPacket->idSocket	= idSocket;
    memcpy(&pPacket->info, pInfo, sizeof(UserInfoStruct));
    return m_pMsgPort->Send(idProcess, MAPGROUP_SOCKETUSERINFO, STRUCT_TYPE(ST_USERCHANGEMAPGORUP), &buf);
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::SendObjInfo(PROCESS_ID idProcess, OBJID idUser, INFO_ID nInfoID, void* pInfo, int nSize)
{
    CHECKF(nInfoID > INFO_NONE && nInfoID < 20); // 20 最多20种INFO
    MESSAGESTR	buf;
    CHANGEMAPGORUP_INFO0* pPacket		= (CHANGEMAPGORUP_INFO0*)buf;
    pPacket->idUser		= idUser;
    pPacket->nInfoType	= nInfoID;
    memcpy(&pPacket->info, pInfo, nSize);
    return m_pMsgPort->Send(idProcess, MAPGROUP_SENDOBJINFO, STRUCT_TYPE(buf), pPacket);
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::SetNpcProcessID(OBJID idNpc, bool bAddNew)
{
    CHANGE_NPCPROCESSID	buf;
    buf.idNpc		= idNpc;
    if (bAddNew)
    {
        buf.idProcess	= m_idProcess;
    }
    else
    {
        buf.idProcess	= PROCESS_NONE;
    }
    return m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SETNPCPROCESSID, STRUCT_TYPE(CHANGE_NPCPROCESSID), &buf);
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::ChangeMapGroup(PROCESS_ID idProcess, OBJID idUser, OBJID idMap, int nPosX, int nPosY)
{
    ST_CHANGEMAPGROUP	st;
    st.idUser	= idUser;
    st.idMap	= idMap;
    st.nPosX	= nPosX;
    st.nPosY	= nPosY;
    return m_pMsgPort->Send(idProcess, MAPGROUP_CHANGEMAPGROUP, STRUCT_TYPE(ST_CHANGEMAPGROUP), &st);
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::PrintText			(LPCTSTR szText)
{
    return m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::CloseMapGroup(SOCKET_ID idGmSocket)		// 关闭所有socket(除GM)，禁止登录
{
    // TODO: 请在此添加关闭客户端SOCKET的代码
    return m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_BREAKALLCONNECT, VARTYPE_INT, &idGmSocket);
    RoleManager()->SaveAll();
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::ChangeTeam(int nAction, OBJID idTeam, OBJID idUser, int nData)			// 组队相关
{
    ST_CHANGETEAM	st;
    st.nAction	= nAction;
    st.idTeam	= idTeam;
    st.idUser	= idUser;
    st.nData	= nData;
    for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
    {
        if (i != m_idProcess)		// 不回送
        {
            m_pMsgPort->Send(i, MAPGROUP_CHANGETEAM, STRUCT_TYPE(st), &st);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::ModifyNpc(OBJID idNpc, LPCTSTR szField, LPCTSTR szData)
{
    char	buf[1024];
    char*	ptr = buf;
    *(int*)ptr	= idNpc;
    ptr	+= sizeof(int);
    strcpy(ptr, szField);
    ptr	= ptr + strlen(szField) + 1;
    strcpy(ptr, szData);
    ptr = ptr + strlen(szData) + 1;
    for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
    {
        if (i != m_idProcess)		// 不回送
        {
            m_pMsgPort->Send(i, MAPGROUP_CHANGENPC, BUFFER_TYPE(ptr - buf), buf);
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::DelTransNpc(OBJID idMasterNpc, bool bBroadcast)
{
    for(IRoleSet::Iter_t i = RoleManager()->QuerySet()->Begin(); i != RoleManager()->QuerySet()->End(); i++)
    {
        IRole* pRole = RoleManager()->QuerySet()->GetObjByIter(i);
        if (pRole)
        {
            CNpc* pNpc;
            if (pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)))
            {
                if (pNpc->GetType() == _SYNTRANS_NPC && pNpc->GetInt(NPCDATA_LINKID) == idMasterNpc)
                {
                    ASSERT(pNpc->DelNpc());
                }
            }
        }
    }
    if (bBroadcast)
    {
        for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
        {
            if (i != m_idProcess)		// 不回送
            {
                m_pMsgPort->Send(i, MAPGROUP_DELTRANSNPC, VARTYPE_INT, &idMasterNpc);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::ChangeCode (SOCKET_ID idSocket, DWORD dwData)
{
    CHANGE_USERDATA	st;
    st.idSocket		= idSocket;
    st.nData		= dwData;
    m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_CHANGECODE, STRUCT_TYPE(CHANGE_USERDATA), &st);
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::SendRpc(const CEventPack& pack)
{
    for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
    {
        if (i != m_idProcess)		// 不回送
        {
            m_pMsgPort->Send(i, MAPGROUP_RPC, BUFFER_TYPE(pack.GetSize()), pack.GetBuf());
        }
    }
}

//////////////////////////////////////////////////////////////////////
// syndicate
//////////////////////////////////////////////////////////////////////
bool CMapGroup::CreateSyndicate		(const CreateSyndicateInfo* pInfo)
{
    return m_pMsgPort->Send(MSGPORT_WORLD, KERNEL_CREATESYNDICATE, STRUCT_TYPE(CreateSyndicateInfo), pInfo);
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::DestroySyndicate(OBJID idSyn, OBJID idTarget)
{
    OBJID	setID[2];
    setID[0]	= idSyn;
    setID[1]	= idTarget;
    return m_pMsgPort->Send(MSGPORT_WORLD, KERNEL_DESTROYSYNDICATE,	STRUCT_TYPE(setID), &setID);
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::CombineSyndicate(OBJID idSyn, OBJID idTarget)
{
    OBJID	setID[2];
    setID[0]	= idSyn;
    setID[1]	= idTarget;
    return m_pMsgPort->Send(MSGPORT_WORLD, KERNEL_COMBINESYNDICATE, STRUCT_TYPE(setID), &setID);
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::ChangeSyndicate		(const SynFuncInfo0* pFuncInfo)
{
    return m_pMsgPort->Send(MSGPORT_WORLD, KERNEL_CHANGESYNDICATE, BUFFER_TYPE(pFuncInfo->nSize), pFuncInfo);
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::SetMapSynID(OBJID idMap, OBJID idSyn)
{
    OBJID	setID[2];
    setID[0]	= idMap;
    setID[1]	= idSyn;
    for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
    {
        if (i != m_idProcess)		// 不回送
        {
            m_pMsgPort->Send(i, MAPGROUP_SETMAPSYNID, STRUCT_TYPE(setID), &setID);
        }
    }
}

//////////////////////////////////////////////////////////////////////
int CMapGroup::GetMapGroupAmount()
{
    CHECKF(m_pMsgPort);
    return m_pMsgPort->GetSize() - MSGPORT_MAPGROUP_FIRST + 1;
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::LevelUp		(OBJID idUser, int nLevel)
{
    m_pMsgPort->Send(MSGPORT_WORLD, WORLD_LEVELUP, VARTYPE_INT, &idUser);
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::QueryFee		(OBJID idAccount)
{
    m_pMsgPort->Send(MSGPORT_WORLD, WORLD_QUERYFEE, VARTYPE_INT, &idAccount);
}



bool CMapGroup::TransmitWorldMsg(SOCKET_ID idSocket, CNetMsg* pMsg)
{
    MESSAGESTR	buf;
    TRANSMSG_PACKET0* pPacket = (TRANSMSG_PACKET0*)buf;
    pPacket->idSocket	= idSocket;
    pPacket->idNpc		= pMsg->GetNpcID();
    pPacket->idPacket	= pMsg->GetType();
    memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
    pPacket->nSize		= pMsg->GetSize();
    pPacket->nTrans		= pMsg->GetTransData();
    m_pMsgPort->Send(MSGPORT_WORLD, WORLD_TRANSMITMSG, STRUCT_TYPE(buf), pPacket);
    return true;
}
