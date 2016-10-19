
//**********************************************************
// 代码编辑器
//**********************************************************

// SynAttr.cpp: implementation of the CSynAttr class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "common.h"
#include "SynAttr.h"
#include "MapGroup.h"
#include "AutoPtr.h"

//////////////////////////////////////////////////////////////////////
const int	MINI_SUB_SYN			= 20;				// 20人以下的子帮派，算迷你子帮派，堂主为“准堂主”


MYHEAP_IMPLEMENTATION(CSynAttr, s_heap)
char	szSynAttrTable[]	= _TBL_SYNATTR;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSynAttr::CSynAttr()
{
    m_pData	= NULL;
    m_nProfferInc = 0;
}

//////////////////////////////////////////////////////////////////////
CSynAttr::~CSynAttr()
{
    SaveInfo();
    if (m_pData)
    {
        m_pData->Release();
    }
}

//////////////////////////////////////////////////////////////////////
CSynAttr* CSynAttr::CreateNew(PROCESS_ID idProcess, CUser* pUser)
{
    CSynAttr* pSynAttr = new CSynAttr;
    CHECKF(pSynAttr);
    pSynAttr->m_idProcess	= idProcess;
    pSynAttr->m_pUser		= pUser;
    return pSynAttr;
}

//////////////////////////////////////////////////////////////////////
// login
//////////////////////////////////////////////////////////////////////
bool CSynAttr::Create()
{
    CHECKF(CMapGroup::IsValidMapGroupProcessID(m_idProcess));
    CHECKF(!m_pData);
    CHECKF(m_pUser);
    m_pData	= CSynAttrData::CreateNew();
    CHECKF(m_pData);
    // 是否加入了帮派
    if (!m_pData->Create(m_pUser->GetID(), Database()))
    {
        SAFE_RELEASE (m_pData);			// 未加入帮派时，m_pData为NULL
        return true;
    }
    // 是否帮派删除了
    if (SynManager()->QuerySyndicate(GetSynID()) == NULL)
    {
        SAFE_RELEASE (m_pData);			// 未加入帮派时，m_pData为NULL
        return true;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::SendInfoToClient()
{
    SynAttrInfoStruct	info;
    GetInfo(&info);
    CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());	// 可能为空
    if (pSyn)
    {
        pSyn = pSyn->GetMasterSyn();
    }
    CMsgSynAttrInfo msg;
    IF_OK(msg.Create(&info, pSyn))		// pSyn may be null
    m_pUser->SendMsg(&msg);
    return true;
}

//////////////////////////////////////////////////////////////////////
// MapGroup
//////////////////////////////////////////////////////////////////////
bool CSynAttr::SaveInfo()
{
    if (m_pData)
    {
        return m_pData->Update();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::GetInfo(SynAttrInfoStruct* pInfo)
{
    CHECKF(pInfo);
    memset(pInfo, 0, sizeof(SynAttrInfoStruct));
    pInfo->id			= m_pUser->GetID();
    if (m_pData)
    {
        pInfo->idSyn		= m_pData->GetInt(SYNATTRDATA_SYNID);
        pInfo->ucRank		= m_pData->GetInt(SYNATTRDATA_RANK);
        pInfo->ucRankShow	= GetSynRankShow();
        pInfo->nProffer		= m_pData->GetInt(SYNATTRDATA_PROFFER);
        pInfo->nProfferInc	= m_pData->GetInt(SYNATTRDATA_PROFFERINC);
        pInfo->usMantle		= 0;
        pInfo->ucLevel		= m_pData->GetInt(SYNATTRDATA_MEMBERLEVEL);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::AppendSynAttr(SynAttrInfoStruct* pInfo)
{
    CHECKF(!m_pData);
    if (pInfo->idSyn != ID_NONE)
    {
        m_pData = CSynAttrData::CreateNew();
        CHECKF(m_pData);
        IF_NOT(m_pData->Create(GameDataDefault()->GetSynAttrData(), pInfo->id))
        return false;
        m_pData->SetInt(SYNATTRDATA_SYNID, pInfo->idSyn);
        m_pData->SetInt(SYNATTRDATA_RANK, pInfo->ucRank);
        m_pData->SetInt(SYNATTRDATA_PROFFER, pInfo->nProffer);
        m_pData->SetInt(SYNATTRDATA_PROFFERINC, pInfo->nProfferInc);
        m_pData->SetInt(SYNATTRDATA_MEMBERLEVEL, pInfo->ucLevel);
        m_pData->ClearUpdateFlags();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::CreateSyndicate(LPCTSTR szName, int nLevel, int nMoney, int nMoneyLeave)
{
    CHECKF(!m_pData);
    if (m_pUser->GetLev() < nLevel)
    {
        m_pUser->SendSysMsg(STR_NOT_ENOUGH_LEV, nLevel);
        return false;
    }
    if (m_pUser->GetMoney() < nMoney)
    {
        m_pUser->SendSysMsg(STR_NOT_ENOUGH_MONEY, nMoney);
        return false;
    }
    CreateSyndicateInfo	info;
    info.idSyn	= ID_NONE;
    SafeCopy(info.szName, szName, _MAX_NAMESIZE);
    SafeCopy(info.szTenet, DEFAULT_TENET, MAX_TENETSIZE);
    SafeCopy(info.szLeaderTitle, DEFAULT_LEADERTITLE, _MAX_NAMESIZE);
    SafeCopy(info.szMemberTitle, DEFAULT_MEMBERTITLE, _MAX_NAMESIZE);
    info.idLeader	= m_pUser->GetID();
    SafeCopy(info.szLeader, m_pUser->GetName(), _MAX_NAMESIZE);
    info.nMoney		= nMoneyLeave;
    info.idFealty	= ID_NONE;
    info.nMantle	= 0;
    info.nSynFlag	= 0;
    OBJID idSyn = SynManager()->QueryModify()->CreateSyndicate(&info);
    if (idSyn == ID_NONE)
    {
        m_pUser->SendSysMsg(STR_FORBIDDEN_GUILD_NAME);
        return false;
    }
    if (!JoinSyn(idSyn, RANK_LEADER, nMoneyLeave))
    {
        ::GmLogSave("错误：玩家[%s]创帮成功，但无法创建帮主记录!", m_pUser->GetName());
        LOGERROR("错误：玩家[%s]创帮成功，但无法创建帮主记录!", m_pUser->GetName());
    }
    CHECKF(m_pData);
    m_pUser->SpendMoney(nMoney, SYNCHRO_TRUE);
    //	m_pData->SetInt(SYNATTRDATA_RANK, RANK_LEADER);
    //	m_pData->Update();
    return true;
}

//////////////////////////////////////////////////////////////////////
//bool CSynAttr::CreateSubSyn(LPCTSTR szName, int nLevel, int nProffer, int nMoney, int nMoneyLeave)
//{
//	CHECKF(m_pData);
//
//	if (m_pUser->GetLev() < nLevel)
//	{
//		m_pUser->SendSysMsg(STR_NOT_ENOUGH_LEV, nLevel);
//		return false;
//	}
//	if (m_pUser->QuerySynProffer()<nProffer)
//	{
//		m_pUser->SendSysMsg(STR_SYN_SETSUBLEADER_NOPROFFER,nProffer);
//		return false;
//	}
//
//	CSyndicate* pFealty = SynManager()->QuerySyndicate(GetSynID());
//	CHECKF(pFealty);
//	if (pFealty->GetInt(SYNDATA_MONEY) < nMoney)
//	{
//		m_pUser->SendSysMsg(STR_GUILD_FUND_NOT_ENOUGH, nMoney);
//		return false;
//	}
//
//	CreateSyndicateInfo	info;
//	info.idSyn	= ID_NONE;
//	SafeCopy(info.szName, szName, _MAX_NAMESIZE);
//	SafeCopy(info.szTenet, DEFAULT_TENET, MAX_TENETSIZE);
//	SafeCopy(info.szLeaderTitle, DEFAULT_LEADERTITLE, _MAX_NAMESIZE);
//	SafeCopy(info.szMemberTitle, DEFAULT_MEMBERTITLE, _MAX_NAMESIZE);
//	info.idLeader	= m_pUser->GetID();
//	SafeCopy(info.szLeader, m_pUser->GetName(), _MAX_NAMESIZE);
//	info.nMoney		= nMoneyLeave;
//	info.idFealty	= GetSynID();
//
//	OBJID idNewSyn = SynManager()->QueryModify()->CreateSyndicate(&info);
//	if (idNewSyn == ID_NONE)
//	{
//		m_pUser->SendSysMsg(STR_FORBIDDEN_GUILD_NAME);
//		return false;
//	}
//
//	// update new leader
//	DelMemberList(GetSynID(), m_pUser->GetName());
//	AddMemberList(idNewSyn, m_pUser->GetName(), m_pUser->GetLev());
//	m_pData->SetInt(SYNATTRDATA_SYNID, idNewSyn);
//	m_pData->SetInt(SYNATTRDATA_RANK, RANK_LEADER);
////	m_pData->Update();
//// update later	SynchroInfo();
///*
//	// update syndicate
//	pFealty->QueryModify()->AddData(SYNDATA_MONEY, -1*nMoney);
//	pFealty->QueryModify()->AddData(SYNDATA_AMOUNT, -1);
//*/
//	// synchro relation
//	CMsgSyndicate	msg;
//	IF_OK(msg.Create(SET_SUBSYN, idNewSyn))
//		pFealty->BroadcastSynMsg(&msg);
//
//	return true;
//}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::ChangeLeader(LPCTSTR szSyn, int nLevel)
{
    CHECKF(m_pData);
    if (m_pUser->GetLev() < nLevel)
    {
        m_pUser->SendSysMsg(STR_NOT_ENOUGH_LEV, nLevel);
        return false;
    }
    CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());
    CHECKF(pSyn);
    CSyndicate* pTargetSyn = SynManager()->QuerySynByName(szSyn);
    if (!pTargetSyn)
    {
        m_pUser->SendSysMsg(STR_GUILD_INEXIST);
        return false;
    }
    // leave old leader
    OBJID idOldLeader = pTargetSyn->GetInt(SYNDATA_LEADERID);
    /*	CUser* pOldLeader = UserManager()->GetUser(idOldLeader);
    	if (pOldLeader)
    	{
    		pOldLeader->QuerySynAttr()->SetRank(RANK_NEWBIE, true);
    	}
    	else
    	{
    		SQLBUF	szSQL;
    		sprintf(szSQL, "UPDATE %s SET syn_id=%u,rank=%d WHERE id=%u LIMIT 1",
    				_TBL_SYNATTR,
    				pSyn->GetID(), RANK_NEWBIE, idOldLeader);
    		Database()->ExecuteSQL(szSQL);

    		char	buf[1024];
    		REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
    		pInfo->idUser		= idOldLeader;
    		pInfo->ucFuncType	= REMOTE_CALL_DROP_LEADER;
    		pInfo->IntParam[0]	= pSyn->GetID();
    		pInfo->IntParam[1]	= RANK_NEWBIE;
    		pInfo->nSize		= sizeof(REMOTE_CALL0) + sizeof(int)*2;
    		MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
    	}
    */
    // update new leader
    //	m_pData->SetInt(SYNATTRDATA_SYNID, pTargetSyn->GetID());
    //	m_pData->SetInt(SYNATTRDATA_RANK, RANK_LEADER);
    //	m_pData->Update();
    //	SynchroInfo();
    // update syndicate
    //	pTargetSyn->QueryModify()->Demise(idOldLeader, m_pUser->GetID(), m_pUser->GetName(), pSyn->GetID(), true);
    CUser* pUser = UserManager()->GetUser(idOldLeader);
    pSyn->QueryModify()->Demise(idOldLeader, pUser->QuerySynAttr()->GetSynMemberLevel(), m_pUser->GetID(), m_pUser->GetName(), m_pUser->QuerySynAttr()->GetSynMemberLevel(), GetSynID(), true);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::DestroySyndicate()
{
    CHECKF(m_pData);
    CHECKF(GetInt(SYNATTRDATA_RANK) == RANK_LEADER);
    SynManager()->QueryModify()->DestroySyndicate(GetSynID());
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::JoinSyn(OBJID idSyn, int nRank, int nProffer)
{
    CHECKF(!m_pData);
    m_pData	= CSynAttrData::CreateNew();
    CHECKF(m_pData);
    IF_NOT(m_pData->Create(GameDataDefault()->GetSynAttrData(), m_pUser->GetID()))
    {
        m_pData->Release();
        m_pData = NULL;
        return false;
    }
    // 注意：必须复写所有属性，以复盖旧记录。
    m_pData->SetInt(SYNATTRDATA_ID, m_pUser->GetID());
    m_pData->SetInt(SYNATTRDATA_SYNID, idSyn);
    m_pData->SetInt(SYNATTRDATA_RANK, nRank);
    m_pData->SetInt(SYNATTRDATA_PROFFER, nProffer);
    m_pData->SetInt(SYNATTRDATA_PROFFERINC, nProffer);
    //---jinggy---2004-12-7---刚加入帮派时，在线时间默认为 TIMEONLINE_FIRSTJOIN ---begin
    //刚加入时在线时间累加和为0，以后随着在线时间进行增加
    m_pData->SetInt(SYNATTRDATA_TIMEONLINE, 0);
    //加入帮派的时间
    time_t tm = time(NULL);
    m_pData->SetInt(SYNATTRDATA_TIMEDETECTED, (int)tm);
    //---jinggy---2004-12-7---刚加入帮派时，在线时间默认为 TIMEONLINE_FIRSTJOIN ---end
    if (m_pData->InsertRecord() != m_pUser->GetID())		//? 已有记录，改为UPDATE
    {
        m_pData->SetInt(SYNATTRDATA_SYNID, idSyn);
        m_pData->SetInt(SYNATTRDATA_RANK, nRank);
        m_pData->SetInt(SYNATTRDATA_PROFFER, nProffer);
        m_pData->SetInt(SYNATTRDATA_PROFFERINC, nProffer);
        m_pData->Update();
    }
    CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());
    if (pSyn)
    {
        pSyn->QueryModify()->AddData(SYNDATA_AMOUNT, 1);
        if (nRank != RANK_LEADER || nRank != RANK_RESERVED) //帮主创建帮派除外;预备团员也除外
        {
            pSyn->QueryModify()->AddData(SYNDATA_REPUTE, REPUTE_PER_MEMBER);	// 招一个人增加1000声望
            pSyn->QueryModify()->AddData(SYNDATA_MONEY, MONEY_PER_MEMBER);		// 招一个人增加10000基金
        }
    }
    AddMemberList(idSyn, m_pUser->GetName(), m_pUser->GetLev());
    // synchro
    SynchroInfo(true);
    if (pSyn)
    {
        pSyn->SendInfoToClient(m_pUser);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::AddMemberList(OBJID idSyn, LPCTSTR szName, int nLevel)
{
    MESSAGESTR	buf;
    SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
    pInfo->idSyn		= idSyn;
    pInfo->nRank		= this->GetSynRank();
    pInfo->nSynMemberLevel = this->GetSynMemberLevel();
    pInfo->ucFuncType	= SYNFUNC_ADD_MEMBER;
    SafeCopy(pInfo->StrParam, szName, _MAX_NAMESIZE);
    pInfo->ucSaveFlag		= true;
    pInfo->nSize		= sizeof(SynFuncInfo0) + _MAX_NAMESIZE;
    MapGroup(PID)->QuerySynMessage()->ChangeSyndicate(pInfo);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CSynAttr::UpLevel(int nLevel)
{
    CHECK(GetSynID());
    MESSAGESTR	buf;
    SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
    pInfo->idSyn		= GetSynID();
    pInfo->ucFuncType	= SYNFUNC_UP_LEVEL;
    pInfo->IntParam[0]	= m_pUser->GetLev();
    SafeCopy((char*)&pInfo->IntParam[1], m_pUser->GetName(), _MAX_NAMESIZE);
    pInfo->ucSaveFlag		= true;
    pInfo->nSize		= sizeof(SynFuncInfo0) + sizeof(int) + _MAX_NAMESIZE;
    MapGroup(PID)->QuerySynMessage()->ChangeSyndicate(pInfo);
}

//////////////////////////////////////////////////////////////////////
void CSynAttr::SynchroInfo(BOOL bAnnounce)		// 修改后，广播自己的属性
{
    SynAttrInfoStruct	info;
    GetInfo(&info);
    CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());
    CMsgSynAttrInfo msg;
    IF_OK(msg.Create(&info, pSyn))		// pSyn may be null
    m_pUser->SendMsg(&msg);
    CMsgPlayer msgPlayer;
    IF_OK(msgPlayer.Create(m_pUser->QueryRole()))
    m_pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
    if (bAnnounce)
    {
        LPCTSTR pAnnounce = "";
        if (pSyn)
        {
            CSyndicate* pMaster = pSyn->GetMasterSyn();
            ASSERT(pMaster);
            pAnnounce = pMaster->GetStr(SYNDATA_ANNOUNCE);
        }
        m_pUser->SendSysMsg(_TXTATR_SYNANNOUNCE, pAnnounce);
    }
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::LeaveSyn(bool bKickout/*=false*/, bool bDeleteRecord/*=true*/, bool bSynchro/*=true*/)		// false: 用于帮派解散
{
    CHECKF(m_pData);
    OBJID idOldSyn = GetSynID();
    if (bDeleteRecord)
    {
        m_pData->DeleteRecord();
    }
    m_pData->Release();
    m_pData	= NULL;
    if (bSynchro)
    {
        CSyndicate* pSyn = SynManager()->QuerySyndicate(idOldSyn);
        IF_OK(pSyn && pSyn->GetInt(SYNDATA_AMOUNT) > 0)
        {
            pSyn->QueryModify()->AddData(SYNDATA_AMOUNT, -1);
            if (bKickout)
            {
                pSyn->QueryModify()->AddData(SYNDATA_REPUTE, -1 * REPUTE_KICKOUT);		// 踢出帮派扣声望
                pSyn->QueryModify()->AddData(SYNDATA_MONEY, -1 * MONEY_KICKOUT);			// 踢出帮派扣的基金
            }
            else
            {
                pSyn->QueryModify()->AddData(SYNDATA_REPUTE, -1 * REPUTE_PER_MEMBER_OUT);		// 主动脱离帮派扣帮派的声望
                pSyn->QueryModify()->AddData(SYNDATA_MONEY, -1 * MONEY_PER_MEMBER_OUT);		// 主动脱离帮派减少的基金
            }
        }
        DelMemberList(idOldSyn, m_pUser->GetName());
        SynchroInfo();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::DelMemberList(OBJID idOldSyn, LPCTSTR szName)
{
    MESSAGESTR	buf;
    SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
    pInfo->idSyn		= idOldSyn;
    pInfo->nRank		= this->GetSynRank();
    pInfo->nSynMemberLevel = this->GetSynMemberLevel();
    pInfo->ucFuncType	= SYNFUNC_DEL_MEMBER;
    SafeCopy(pInfo->StrParam, szName, _MAX_NAMESIZE);
    pInfo->ucSaveFlag		= true;
    pInfo->nSize		= sizeof(SynFuncInfo0) + _MAX_NAMESIZE;
    MapGroup(PID)->QuerySynMessage()->ChangeSyndicate(pInfo);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::DonateMoney(int nMoney)
{
    if (!m_pData)
    {
        return false;
    }
    if (nMoney <= 0)
    {
        return false;
    }
    // 检查帮派
    CSynPtr pSyn = SynManager()->QuerySyndicate(GetSynID());
    // 检查同步
    if (!pSyn)
    {
        return false;
    }
    pSyn = pSyn->GetMasterSyn();
    CHECKF(pSyn);
    if (m_pUser->GetMoney() < nMoney)
    {
        m_pUser->SendSysMsg(STR_NOT_SO_MUCH_MONEY);
        return false;
    }
    if (pSyn->QueryModify()->AddData(SYNDATA_MONEY, nMoney))
    {
        m_pUser->SpendMoney(nMoney, SYNCHRO_TRUE);
        AddProffer(nMoney);
        MSGBUF szMsg;
        sprintf(szMsg, STR_DONATE_ssd, m_pUser->GetName(), nMoney);
        CMsgTalk	msg;
        IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, 0xFFFFFF, _TXTATR_SYNDICATE))
        pSyn->BroadcastSynMsg(&msg);
        if (nMoney >= CHANGELEADER_MONEY)
        {
            ChangeLeader();
        }
        SynchroInfo();
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::AddProffer(int nData)
{
    CHECKF(m_pData && nData > 0);
    int	nProffer = ::AddToTop(m_pData->GetInt(SYNATTRDATA_PROFFER), nData, SYNPROFFER_LIMIT);
    m_pData->SetInt(SYNATTRDATA_PROFFER, nProffer);
    //---jinggy---添加贡献值的累加值---begin
    nProffer = ::AddToTop(m_pData->GetInt(SYNATTRDATA_PROFFERINC), nData, SYNPROFFER_LIMIT);
    m_pData->SetInt(SYNATTRDATA_PROFFERINC, nProffer);
    m_nProfferInc += nData;
    if (m_nProfferInc >= 200)
    {
        AddProfferToLeader(m_nProfferInc);
        m_nProfferInc = 0;
    }
    //---jinggy---添加贡献值的累加值---end
    // synchro
    CMsgUserAttrib	msg;
    IF_OK(msg.Create(m_pUser->GetID(), _USERATTRIB_PROFFER, nProffer))
    m_pUser->SendMsg(&msg);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::DecProffer(int nData)
{
    CHECKF(m_pData && nData > 0);
    int	nProffer = __max(0, (int)m_pData->GetInt(SYNATTRDATA_PROFFER) - nData);
    m_pData->SetInt(SYNATTRDATA_PROFFER, nProffer);
    // synchro
    CMsgUserAttrib	msg;
    IF_OK(msg.Create(m_pUser->GetID(), _USERATTRIB_PROFFER, nProffer))
    m_pUser->SendMsg(&msg);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CSynAttr::SetProffer(int nData)
{
    CHECK (m_pData && nData > 0);
    nData = __min(nData, SYNPROFFER_LIMIT);
    m_pData->SetInt(SYNATTRDATA_PROFFER, nData);
    // synchro
    CMsgUserAttrib	msg;
    IF_OK(msg.Create(m_pUser->GetID(), _USERATTRIB_PROFFER, nData))
    m_pUser->SendMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
// 帮主不在时，代替帮主
bool CSynAttr::ChangeLeader()
{
    CHECKF(m_pData);
    CSynPtr pSyn = SynManager()->QuerySyndicate(GetSynID());
    CHECKF(pSyn);
    OBJID idOldLeader = pSyn->GetInt(SYNDATA_LEADERID);
    if (UserManager()->GetUser(idOldLeader))
    {
        return false;
    }
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT * FROM %s WHERE rank=%d && syn_id=%u", _TBL_SYNATTR, RANK_LEADER, pSyn->GetID());
    IRecordset* pRes = Database()->CreateNewRecordset(szSQL);		// VVVVVVVVVVVVVVVVVV
    if (pRes)
    {
        IF_NOT_(pRes->RecordCount() <= 1 && pRes->LoadDWord("id") == idOldLeader)
        {
            pRes->Release();		// AAAAAAAAAAAAAAAAAAAAAAAAAA
            return false;
        }
        sprintf(szSQL, "SELECT * FROM %s WHERE id=%u", _TBL_USER, idOldLeader);
        IRecordset* pUserRes = Database()->CreateNewRecordset(szSQL);
        if (pUserRes)
        {
            int nLastLogin = pUserRes->LoadDWord("last_login");
            pUserRes->Release();
            if (::DateStamp() <= ::DateStampPass(nLastLogin, CHANGELEADER_DAYS))
            {
                pRes->Release();		// AAAAAAAAAAAAAAAAAAAAAAAAAA
                return false;
            }
            pRes->SetInt(SYNATTRDATA_RANK, RANK_NEWBIE);
            pRes->UpdateRecord();
        }
        pRes->Release();		// AAAAAAAAAAAAAAAAAAAAAAAAAA
    }
    MSGBUF	szMsg;
    sprintf(szMsg, STR_SYN_LEADER_LOSE_uss, pSyn->GetID(), pSyn->GetStr(SYNDATA_NAME), m_pUser->GetName());
    ::GmLogSave(szMsg);
    LOGMSG(szMsg);
    sprintf(szSQL, "SELECT * FROM %s WHERE rank=%d && syn_id=%u", _TBL_SYNATTR, RANK_LEADER, pSyn->GetID());
    IRecordset* pTempRes = Database()->CreateNewRecordset(szSQL);		//??? 有共享冲突。此时检查，下面修改，可能会出多个帮主
    if (pTempRes)
    {
        pTempRes->Release();
        return false;
    }
    CUser* pUser = UserManager()->GetUser(idOldLeader);
    pSyn->QueryModify()->Demise(idOldLeader, pUser->QuerySynAttr()->GetSynMemberLevel(), m_pUser->GetID(), m_pUser->GetName(), m_pUser->QuerySynAttr()->GetSynMemberLevel(), GetSynID(), true);
    //	m_pData->SetInt(SYNATTRDATA_RANK, RANK_LEADER);
    //	m_pData->Update();		//??? 有共享冲突。此时修改，上面检查
    //	SynchroInfo();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::SetRank(int nRank, bool bLeader /*= false*/)
{
    CHECKF(nRank != RANK_LEADER && m_pData->GetInt(SYNATTRDATA_RANK) != RANK_LEADER || bLeader);
    m_pData->SetInt(SYNATTRDATA_RANK, nRank);
    if (bLeader)
    {
        m_pData->Update();
    }
    SynchroInfo();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::SetIdRank(OBJID idSyn, int nRank, bool bSynchro)
{
    m_pData->SetInt(SYNATTRDATA_SYNID, idSyn);
    m_pData->SetInt(SYNATTRDATA_RANK, nRank);
    m_pData->Update();
    if (bSynchro)
    {
        SynchroInfo();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
int CSynAttr::GetAssistantCount()
{
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT COUNT(*) FROM %s as s LEFT JOIN %s AS u ON u.id=s.id WHERE s.syn_id=%u && s.rank=%d && u.id IS NOT NULL", _TBL_SYNATTR, _TBL_USER, GetSynID(), RANK_SUBLEADER);
    CAutoPtr<IRecordset> pStatRes = Database()->CreateNewRecordset(szSQL);
    IF_OK(pStatRes)
    return pStatRes->GetInt(0);
    ASSERT(!"GetAssistantCount");
    return MAX_ASSISTANTSIZE;			// prevent add assistant
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::KickoutMember(LPCTSTR szTarget)
{
    CHECKF(szTarget);
    OBJID idSyn = GetSynID();
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT id FROM %s WHERE name='%s' LIMIT 1", _TBL_USER, szTarget);
    IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
    if (!pRes)
    {
        return false;    // not this name
    }
    OBJID idUser = pRes->GetInt(0);			// 0: id
    pRes->Release();
    sprintf(szSQL, "DELETE FROM %s WHERE id=%u && syn_id=%u && rank!=%u LIMIT 1", _TBL_SYNATTR, idUser, idSyn, RANK_LEADER);
    Database()->ExecuteSQL(szSQL);
    // remote call
    char	buf[1024];
    REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
    pInfo->idUser		= idUser;
    pInfo->ucFuncType	= REMOTE_CALL_LEAVE_SYN;
    pInfo->IntParam[0]	= GetSynID();
    SafeCopy((char*)&pInfo->IntParam[1], m_pUser->GetName(), _MAX_NAMESIZE);
    pInfo->nSize		= sizeof(REMOTE_CALL0) + sizeof(int) + _MAX_NAMESIZE;
    MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
    OBJID	idOldSyn = GetSynID();
    {
        CSyndicate* pSyn = SynManager()->QuerySyndicate(idOldSyn);
        IF_OK(pSyn && pSyn->GetInt(SYNDATA_AMOUNT) > 0)
        {
            pSyn->QueryModify()->AddData(SYNDATA_AMOUNT, -1);
            pSyn->QueryModify()->AddData(SYNDATA_REPUTE, -1 * REPUTE_KICKOUT);	// 踢人出帮派扣声望
            pSyn->QueryModify()->AddData(SYNDATA_MONEY, -1 * MONEY_KICKOUT);			// 踢出帮派扣的基金
        }
    }
    DelMemberList(idOldSyn, szTarget);
    return true;
}

//////////////////////////////////////////////////////////////////////
DWORD CSynAttr::GetSynRankShow		()
{
    if (GetSynID() != ID_NONE)
    {
        int nRankShow = m_pData->GetInt(SYNATTRDATA_RANK);
        if (nRankShow > RANK_NEWBIE)
        {
            CSynPtr pSyn = SynManager()->QuerySyndicate(GetSynID());
            while (pSyn && pSyn->GetInt(SYNDATA_FEALTY) != ID_NONE)
            {
                nRankShow	-= RANK_SUB;
                pSyn = SynManager()->QuerySyndicate(pSyn->GetInt(SYNDATA_FEALTY));
                //				if (pSyn->GetInt(SYNDATA_AMOUNT) < MINI_SUB_SYN)
                //					nRankShow		-= RANK_PRE;
            }
        }
        return nRankShow;
    }
    return RANK_NONE;
}

/*
//////////////////////////////////////////////////////////////////////
int CSynAttr::SumSynMember()
{
	int nSum = 0;
	CSynSet::Iterator pSyn = SynManager()->QuerySynSet()->NewEnum();
	while(pSyn.Next())
	{
		if (pSyn && pSyn->GetMasterSynID() == GetMasterSynID())
		{
			nSum += pSyn->GetInt(SYNDATA_AMOUNT);
		}
	}
	return nSum;
}
*/

//////////////////////////////////////////////////////////////////////
LPCTSTR CSynAttr::GetRankTitle()
{
    switch(GetSynRankShow())
    {
    case	100:
        return STR_SYNDICATE_LEADER;
        break;
    case	90:
        return STR_SYNDICATE_LEADER2;
        break;
    case	80:
        return STR_SYNDICATE_LEADER3;
        break;
    case	70:
        return STR_SYNDICATE_LEADER4;
        break;
    case	60:
        return STR_SYNDICATE_LEADER5;
        break;
    case	50:
        return STR_SYNDICATE_MEMBER;
        break;
    }
    return STR_ERROR;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::ChangeSyn(CSyndicate* pSyn, CSyndicate* pTarget)
{
    CHECKF(pSyn && pTarget);
    CHECKF(m_pData->GetInt(SYNATTRDATA_RANK) != RANK_LEADER);
    if (pSyn->GetMasterSynID() != pTarget->GetMasterSynID())
    {
        return false;
    }
    pSyn->QueryModify()->AddData(SYNDATA_AMOUNT, -1);
    DelMemberList(pSyn->GetID(), m_pUser->GetName());
    pTarget->QueryModify()->AddData(SYNDATA_AMOUNT, 1);
    AddMemberList(pTarget->GetID(), m_pUser->GetName(), m_pUser->GetLev());
    this->SetIdRank(pTarget->GetID(), RANK_NEWBIE);
    //---jinggy---2004-12-7---刚加入帮派时，在线时间默认为 TIMEONLINE_FIRSTJOIN ---begin
    //	m_pData->SetInt(SYNATTRDATA_TIMEONLINE, TIMEONLINE_FIRSTJOIN);
    m_pData->SetInt(SYNATTRDATA_TIMEONLINE, 0);
    //加入帮派的时间
    time_t tm = time(NULL);
    m_pData->SetInt(SYNATTRDATA_TIMEDETECTED, (int)tm);
    //---jinggy---2004-12-7---刚加入帮派时，在线时间默认为 TIMEONLINE_FIRSTJOIN ---end
    return true;
}

//////////////////////////////////////////////////////////////////////
CSyndicate* CSynAttr::QuerySyndicate()
{
    if (GetSynID() == ID_NONE)
    {
        return NULL;
    }
    if (m_linkSyn && m_linkSyn->GetID() == GetSynID())
    {
        return m_linkSyn;
    }
    CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());
    if (pSyn)
    {
        m_linkSyn	= pSyn->QueryLink();
    }
    return m_linkSyn;
}

//---jinggy---2004-12-7---获得帮派成员的连续在线时间
unsigned int CSynAttr::GetTimeOnline()
{
    DEBUG_TRY
    if (m_pData)
    {
        return m_pData->GetInt(SYNATTRDATA_TIMEONLINE);
    }
    DEBUG_CATCH(" CSynAttr::GetTimeOnline()")
    return ID_NONE;
}
//增加帮派成员的连续在线时间
void CSynAttr::IncTimeOnline(unsigned short usTimeInc)
{
    DEBUG_TRY
    unsigned int uiTimeOnline = GetTimeOnline(); //在线时间到60*10后就 不在增加
    if ( uiTimeOnline >= TIMEONLINE_FIRSTJOIN ) //10*60分钟
    {
        return ;
    }
    unsigned int uiTimeSum = uiTimeOnline + usTimeInc;
    m_pData->SetInt(SYNATTRDATA_TIMEONLINE, uiTimeSum);
    m_pData->Update();
    //检测是否背叛帮派
    DetectIsBetrySyn();
    DEBUG_CATCH("void CSynAttr::IncTimeOnline(UINT8 uiTimeInc)")
}
/*
派的正式成员必须每两个月必须上线10小时，如果没有达到要求，则视为背叛帮派处理。
*/
bool CSynAttr::DetectIsBetrySyn()
{
    DEBUG_TRY
    //上次的记录时间,将现在的时间和记录的时间进行比较，看是否过了2个月，若过了2个月在线时间没有达到 10个小时，则
    //背叛帮派
    int nTimeDetected = m_pData->GetInt(SYNATTRDATA_TIMEDETECTED);
    time_t tDetected = (time_t) nTimeDetected;
    tm*	pTmDetected	= localtime(&tDetected);
    //将时间格式转换为2个月后的值
    tm	tmE;
    memset(&tmE, 0, sizeof(tm));
    tmE = *pTmDetected;
    tmE.tm_mon += 2;
    time_t	tE	= mktime(&tmE);
    //与当前时间进行比较
    time_t tCurTime = time(NULL);
    if (tCurTime >= tE) //若大于或等于2个月
    {
        unsigned int uiTimeOnline = m_pData->GetInt(SYNATTRDATA_TIMEONLINE);
        if (m_pUser->GetSynRank() == RANK_RESERVED)
        {
            if (uiTimeOnline < TIMEONLINE_FIRSTJOIN) //连续2个月连续在线时间没有
            {
                //预备成员被开除
                this->Betray();
                return true;
            }
            else
            {
                //重新设置在线时间为0
                m_pData->SetInt(SYNATTRDATA_TIMEONLINE, 0);
                m_pData->SetInt(SYNATTRDATA_TIMEDETECTED, tCurTime);
                m_pData->Update();
                return false;
            }
        }
        else
        {
            if (uiTimeOnline < TIMEONLINE_FIRSTJOIN / 2) //连续2个月连续在线时间没有
            {
                this->Betray();
                return true;
            }
            else
            {
                //重新设置在线时间为0
                m_pData->SetInt(SYNATTRDATA_TIMEONLINE, 0);
                m_pData->SetInt(SYNATTRDATA_TIMEDETECTED, tCurTime);
                m_pData->Update();
                return false;
            }
        }
    }
    DEBUG_CATCH("bool CSynAttr::IsFullTimeOnline(int iMonth, int iDay)")
    return true;//未到检测时间 返回TRUE
}
//背叛帮派
void CSynAttr::Betray()
{
    DEBUG_TRY
    //
    if (m_pUser->GetSynRank() == RANK_RESERVED)
    {
        OBJID idOldSyn = GetSynID();
        //---jinggy---从帮派中删除此成员---begin
        if (m_pData)
        {
            m_pData->DeleteRecord();
            m_pData->Release();
            m_pData	= NULL;
        }
        else
        {
            return;
        }
        DelMemberList(idOldSyn, m_pUser->GetName());
        //---jinggy---从帮派中删除此成员---end
    }
    //若没有
    int  nDecProffer = 0 ;
    switch(m_pUser->GetSynRank())
    {
    case RANK_NEWBIE:
        {
            nDecProffer = 200;
            this->DecProffer(nDecProffer);
            this->SetRank(RANK_NEWBIE);
        }
        break;
    case RANK_SUBLEADER:
        {
            nDecProffer = 200;
            this->DecProffer(nDecProffer);
            this->SetRank(RANK_NEWBIE);
        }
        break;
    case RANK_TEAMLEADER:
        {
            nDecProffer = 500;
            this->DecProffer(nDecProffer);
            this->SetRank(RANK_NEWBIE);
        }
        break;
    default:
        break;
    }
    SynchroInfo();
    /*
    	CSyndicate* pSyn = SynManager()->QuerySyndicate(idOldSyn);

    	IF_OK(pSyn && pSyn->GetInt(SYNDATA_AMOUNT) > 0)
    	{
    		pSyn->QueryModify()->AddData(SYNDATA_AMOUNT, -1);
    		pSyn->QueryModify()->AddData(SYNDATA_REPUTE, -1*REPUTE_PER_MEMBER_OUT);		// 主动脱离帮派扣帮派的声望
    		pSyn->QueryModify()->AddData(SYNDATA_MONEY, -1*MONEY_PER_MEMBER_OUT);		// 主动脱离帮派减少的基金

    		//---jinggy---减少帮主和副帮主的贡献值---begin
    		CSynAttrData* pData = CSynAttrData::CreateNew();

    		IF_NOT(pData)
    		{
    			return ;
    		}

    		//帮主 的贡献值减2000
    		char szSQL[1024];
    		sprintf(szSQL, "SELECT * FROM %s WHERE syn_id=%d AND rank=%d LIMIT 1",\
    			           _TBL_SYNATTR,idOldSyn,RANK_LEADER);

    		IRecordset* pRes = Database()->CreateNewRecordset(szSQL);

    		IF_NOT(pRes)
    		{
    			pData->Release();
    			return;
    		}

    		IF_NOT(pData->Create(pRes))
    		{
    			pData->Release();
    			pRes->Release();
    			return;
    		}
    		pRes->Release();

    		int nProffer = pData->GetInt(SYNATTRDATA_PROFFER) - 2000;
    		pData->SetInt(SYNATTRDATA_PROFFER,nProffer);
    		pData->Update();
    		pData->Release();

    		//副帮主 的贡献值减1000
    		sprintf(szSQL, "SELECT * FROM %s WHERE syn_id=%d AND rank=%d",\
    			_TBL_SYNATTR,idOldSyn,RANK_SUBLEADER);

    		pRes = Database()->CreateNewRecordset(szSQL);
    		IF_NOT(pRes)
    		{
    			return;
    		}

    		for(int i = 0; i < pRes->RecordCount(); i++)
    		{
    			pData = CSynAttrData::CreateNew();

    			IF_NOT(pData)
    			{
    				pRes->Release();
    				return ;
    			}

    			IF_NOT(pData->Create(pRes))
    			{
    				pData->Release();
    				pRes->Release();
    				return;
    			}

    			nProffer = pData->GetInt(SYNATTRDATA_PROFFER) - 1000;
    			pData->SetInt(SYNATTRDATA_PROFFER,nProffer);
    			pData->Update();
    			pData->Release();

    			pRes->MoveNext();
    		}

    		pRes->Release();
    	}
    	//---jinggy---减少帮主和副帮主的贡献值---end
    */
    DEBUG_CATCH("void CSynAttr::Betray()")
}

//当帮主分配了贡献值后，需要重新设置 成员的 贡献值的增量
void CSynAttr::ResetSynProfferInc()
{
    m_pData->SetInt(SYNATTRDATA_PROFFERINC, 0);
}
/*
//添加帮派声望增量
void CSynAttr::AddReputeInc(int nReputeInc)
{
	//每帮军团增加10点军团声望，玩家军团贡献增加1点
	if (nReputeInc>=10)
	{
		int nProffer = nReputeInc/10;
		this->AddProffer(nProffer);
		return;
	}
}
*/
/*
//添加帮派基金增量
void CSynAttr::AddMoneyInc(int nMoneyInc)
{
	//每帮军团赚取100军团基金（不包括捐献军团基金），军团贡献增加1点。（这里包括招收新玩家）。
	DWORD dwMoneySum =  nMoneyInc;
	if (dwMoneySum/=100)
	{
		this->AddProffer(dwMoneySum);return;
	}
}
*/
/*
CSyndicate* CSynAttr::GetMasterSyn()
{
	if (GetSynID() != ID_NONE)
	{
		CSynPtr pSyn = SynManager()->QuerySyndicate(GetSynID());
		IF_OK(pSyn)
		{
			if (pSyn->GetInt(SYNDATA_FEALTY) != ID_NONE)
			{
				OBJID idMaster = pSyn->GetInt(SYNDATA_FEALTY);
				CSynPtr pSyn = SynManager()->QuerySyndicate(idMaster);
				if (pSyn)
					return pSyn;
			}
			else
				return pSyn;
		}
	}

	return NULL;
}*/


static int	PROFFER_TIME_DIV[]	= {10, 20, 40};
// 给领导分贡献度
void CSynAttr::AddProfferToLeader(int nProffer)
{
    int nTime = 0;
    CSyndicate* pSyn = QuerySyndicate();
    if (pSyn)
    {
        CUser* pUser = NULL;
        if (pSyn->GetInt(SYNDATA_LEADERID) != ID_NONE
                && pSyn->GetInt(SYNDATA_LEADERID) != m_pUser->GetID())
        {
            // 如果本人不是本帮派帮主，且存在帮主，直接找帮主
            CUser* pUser = UserManager()->GetUser(pSyn->GetInt(SYNDATA_LEADERID));
            if (pUser && pUser->QuerySynAttr())
            {
                CHECK(nTime < 3);
                int nAddProffer = nProffer / PROFFER_TIME_DIV[nTime];
                pUser->QuerySynAttr()->AddProffer(nAddProffer);
            }
            ++nTime;	// 即使帮主不在线，也当作分享了贡献度
        }
        // 向上寻找帮主
        while (!pSyn->IsMasterSyn())
        {
            pSyn = SynManager()->QuerySyndicate(pSyn->GetInt(SYNDATA_FEALTY));
            if (pSyn && pSyn->GetInt(SYNDATA_LEADERID) != ID_NONE)
            {
                // 如果存在帮主，寻找帮主并分享贡献度
                pUser = UserManager()->GetUser(pSyn->GetInt(SYNDATA_LEADERID));
                if (pUser)
                {
                    CHECK(nTime < 3);
                    int nAddProffer = nProffer / PROFFER_TIME_DIV[nTime];
                    pUser->QuerySynAttr()->AddProffer(nAddProffer);
                }
                ++nTime;	// 即使帮主不在线，也当作分享了贡献度
            }
        }
    }
}

//改变帮派的成员的等级
bool CSynAttr::SetSynMemberLevel(int nLevel)
{
    if (m_pData)
    {
        m_pData->SetInt(SYNATTRDATA_MEMBERLEVEL, nLevel);
        return true;
    }
    return false;
}

int CSynAttr::GetSynMemberLevel()
{
    if (m_pData)
    {
        return m_pData->GetInt(SYNATTRDATA_MEMBERLEVEL);
    }
    return -1;
}

bool CSynAttr::CreateSubSubSyn(LPCTSTR szName, int nLevel, int nProffer, int nMoney, int nMoneyLeave)
{
    /*	CHECKF(m_pData);

    	if (m_pUser->GetLev() < nLevel)
    	{
    		m_pUser->SendSysMsg(STR_NOT_ENOUGH_LEV, nLevel);
    		return false;
    	}
    	if (m_pUser->QuerySynProffer()<nProffer)
    	{
    		m_pUser->SendSysMsg(STR_SYN_SETSUBLEADER_NOPROFFER,nProffer);
    		return false;
    	}

    	CSyndicate* pFealty = SynManager()->QuerySyndicate(GetSynID());
    	CHECKF(pFealty);
    	if (pFealty->GetInt(SYNDATA_MONEY) < nMoney)
    	{
    		m_pUser->SendSysMsg(STR_GUILD_FUND_NOT_ENOUGH, nMoney);
    		return false;
    	}

    	CreateSyndicateInfo	info;
    	info.idSyn	= ID_NONE;
    	SafeCopy(info.szName, szName, _MAX_NAMESIZE);
    	SafeCopy(info.szTenet, DEFAULT_TENET, MAX_TENETSIZE);
    	SafeCopy(info.szLeaderTitle, DEFAULT_LEADERTITLE, _MAX_NAMESIZE);
    	SafeCopy(info.szMemberTitle, DEFAULT_MEMBERTITLE, _MAX_NAMESIZE);
    	info.idLeader	= m_pUser->GetID();
    	SafeCopy(info.szLeader, m_pUser->GetName(), _MAX_NAMESIZE);
    	info.nMoney		= nMoneyLeave;
    	info.idFealty	= GetSynID();

    	OBJID idNewSyn = SynManager()->QueryModify()->CreateSyndicate(&info);
    	if (idNewSyn == ID_NONE)
    	{
    		m_pUser->SendSysMsg(STR_FORBIDDEN_GUILD_NAME);
    		return false;
    	}

    	// update new leader
    	DelMemberList(GetSynID(), m_pUser->GetName());
    	AddMemberList(idNewSyn, m_pUser->GetName(), m_pUser->GetLev());
    //	m_pData->SetInt(SYNATTRDATA_SYNID, idNewSyn);
    //	m_pData->SetInt(SYNATTRDATA_RANK, RANK_LEADER);
    //	m_pData->Update();
    // update later	SynchroInfo();

    	// update syndicate
    	pFealty->QueryModify()->AddData(SYNDATA_MONEY, -1*nMoney);
    	pFealty->QueryModify()->AddData(SYNDATA_AMOUNT, -1);

    	// synchro relation
    	CMsgSyndicate	msg;
    	IF_OK(msg.Create(SET_SUBSUBSYN,pSyn->GetID(),pSynTeam->GetID()))
    		pFealty->BroadcastSynMsg(&msg);
    */
    return true;
}


OBJID CSynAttr::CreateSubSyn(OBJID idFealty, const char* pszName)
{
    if (ID_NONE == idFealty)
    {
        return ID_NONE;
    }
    CSyndicate* pFealty = SynManager()->QuerySyndicate(idFealty);
    CHECKF(pFealty);
    CreateSyndicateInfo	info;
    info.idSyn	= ID_NONE;
    SafeCopy(info.szName, pszName, _MAX_NAMESIZE);
    SafeCopy(info.szTenet, DEFAULT_TENET, MAX_TENETSIZE);
    SafeCopy(info.szLeaderTitle, DEFAULT_LEADERTITLE, _MAX_NAMESIZE);
    SafeCopy(info.szMemberTitle, DEFAULT_MEMBERTITLE, _MAX_NAMESIZE);
    info.idLeader	= ID_NONE;
    SafeCopy(info.szLeader, "", _MAX_NAMESIZE);
    info.nMoney		= 0;
    info.idFealty	= idFealty;
    info.nSynFlag	= 1;
    OBJID idNewSyn = SynManager()->QueryModify()->CreateSyndicate(&info);
    if (idNewSyn == ID_NONE)
    {
        m_pUser->SendSysMsg(STR_FORBIDDEN_GUILD_NAME);
        return ID_NONE;
    }
    // synchro relation
    CMsgSyndicate	msg;
    IF_OK(msg.Create(SET_SYN, idNewSyn, idFealty))
    pFealty->BroadcastSynMsg(&msg);
    return idNewSyn;
}

OBJID CSynAttr::CreateSubSubSyn(OBJID idFealty, const char szName[])
{
    IF_NOT(idFealty)
    return ID_NONE;
    CreateSyndicateInfo	info;
    info.idSyn	= ID_NONE;
    SafeCopy(info.szName, szName, _MAX_NAMESIZE);
    SafeCopy(info.szTenet, DEFAULT_TENET, MAX_TENETSIZE);
    SafeCopy(info.szLeaderTitle, DEFAULT_LEADERTITLE, _MAX_NAMESIZE);
    SafeCopy(info.szMemberTitle, DEFAULT_MEMBERTITLE, _MAX_NAMESIZE);
    info.idLeader	= NULL;
    SafeCopy(info.szLeader, "", _MAX_NAMESIZE);
    info.nMoney		= 0;
    info.idFealty	= idFealty;
    info.nSynFlag	= 2;
    OBJID idNewSyn = SynManager()->QueryModify()->CreateSyndicate(&info);
    if (idNewSyn == ID_NONE)
    {
        m_pUser->SendSysMsg(STR_FORBIDDEN_GUILD_NAME);
        return ID_NONE;
    }
    CSyndicate* pFealty = SynManager()->QuerySyndicate(idFealty);
    CHECKF(pFealty);
    // synchro relation
    CMsgSyndicate	msg;
    IF_OK(msg.Create(SET_SYN, idNewSyn, idFealty))
    pFealty->BroadcastSynMsg(&msg);
    return idNewSyn;
}

int CSynAttr::GetSynRank()
{
    if (GetSynID() != ID_NONE)
    {
        return m_pData->GetInt(SYNATTRDATA_RANK);
    }
    else
    {
        return RANK_NONE;
    }
}
