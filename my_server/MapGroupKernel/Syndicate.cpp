
//**********************************************************
// 代码编辑器
//**********************************************************

// Syndicate.cpp: implementation of the CSyndicate class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "Syndicate.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
char	szSynTable[] = _TBL_SYNDICATE;

MYHEAP_IMPLEMENTATION(CSyndicate, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSyndicate::CSyndicate()
{
    SetObjType(OBJ_SYN);
    m_pData	= NULL;
    m_obj.m_pOwner	= this;
    m_link.Init(this);
}

//////////////////////////////////////////////////////////////////////
CSyndicate::~CSyndicate()
{
    if (m_pData)
    {
        m_pData->Update();
        m_pData->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::Create(IRecordset* pRes)
{
    ASSERT(!m_pData);
    m_pData = CSynBase::CreateNew();
    CHECKF(m_pData);
    IF_NOT(m_pData->Create(pRes))
    return false;
    if (m_pData->GetInt(SYNDATA_DEL_FLAG) != 0)		// 已删除的帮派
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::ReloadData()
{
    ASSERT(m_pData);
    /*		//??? 单服务器，不需要RELOAD
    	CSynBase* pData = CSynBase::CreateNew();
    	CHECKF(pData);
    	IF_NOT(pData->Create(GetID(), Database()))
    	{
    		pData->Release();
    		return false;
    	}

    	if (pData->GetInt(SYNDATA_DEL_FLAG) != 0)		// 已删除的帮派
    	{
    		pData->Release();
    		return false;
    	}

    	m_pData->Release();
    	m_pData	= pData;
    */
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::Create(const CreateSyndicateInfo* pInfo, bool bSave)
{
    ASSERT(!m_pData);
    OBJID idSyn = bSave ? ID_NONE : pInfo->idSyn;
    m_pData = CSynBase::CreateNew();
    CHECKF(m_pData);
    IF_NOT(m_pData->Create(GameDataDefault()->GetSynData(), idSyn))
    return false;
    m_pData->SetInt(SYNDATA_ID, idSyn);
    m_pData->SetStr(SYNDATA_NAME, pInfo->szName, _MAX_NAMESIZE);
    m_pData->SetStr(SYNDATA_TENET, pInfo->szTenet, MAX_TENETSIZE);
    m_pData->SetStr(SYNDATA_ANNOUNCE, pInfo->szLeaderTitle, MAX_TENETSIZE);
    m_pData->SetStr(SYNDATA_MEMBERTITLE, pInfo->szMemberTitle, _MAX_NAMESIZE);
    m_pData->SetInt(SYNDATA_LEADERID, pInfo->idLeader);
    m_pData->SetStr(SYNDATA_LEADERNAME, pInfo->szLeader, _MAX_NAMESIZE);
    m_pData->SetInt(SYNDATA_MONEY, pInfo->nMoney);
    m_pData->SetInt(SYNDATA_FEALTY, pInfo->idFealty);
    m_pData->SetInt(SYNDATA_MANTLE_FALG, pInfo->nMantle);
    m_pData->SetInt(SYNDATA_RANK, 1);
    if (pInfo->nSynFlag == 0)
    {
        m_pData->SetInt(SYNDATA_AMOUNT, 1);
    }
    else
    {
        m_pData->SetInt(SYNDATA_AMOUNT, 0);
    }
    m_pData->SetInt(SYNDATA_REPUTE, 0);
    if (bSave)
    {
        return m_pData->InsertRecord() != ID_NONE;
    }
    else
    {
        m_pData->ClearUpdateFlags();
        return true;
    }
}

//////////////////////////////////////////////////////////////////////
// modify
//////////////////////////////////////////////////////////////////////
bool CSyndicate::Demise(OBJID idOldLeader, int nOldSynMemberLevel, OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave)
{
    if (!( idOldLeader == (OBJID)m_pData->GetInt(SYNDATA_LEADERID) || (OBJID)m_pData->GetInt(SYNDATA_LEADERID) == ID_NONE ))	//? 兼容于旧数据
    {
        return false;
    }
    NAMESTR	szNewLeader2;
    SafeCopy(szNewLeader2, szNewLeader, _MAX_NAMESIZE);
    m_pData->SetInt(SYNDATA_LEADERID, idNewLeader);
    m_pData->SetStr(SYNDATA_LEADERNAME, szNewLeader2, _MAX_NAMESIZE);
    if (bSave)
    {
        m_pData->Update();
    }
    else
    {
        m_pData->ClearUpdateFlags();
    }
    CUser* pOldUser = UserManager()->GetUser(idOldLeader);
    if (pOldUser)
    {
        pOldUser->QuerySynAttr()->SetIdRank(GetID(), RANK_NEWBIE);
        pOldUser->QuerySynAttr()->SaveInfo();
        pOldUser->QuerySynAttr()->SynchroInfo();
    }
    CUser* pNewUser = UserManager()->GetUser(idNewLeader);
    if (pNewUser)
    {
        pNewUser->QuerySynAttr()->SetIdRank(this->GetID(), RANK_LEADER);
        pNewUser->QuerySynAttr()->SaveInfo();
        pNewUser->QuerySynAttr()->SynchroInfo();
    }
    CMsgSynInfo	msg;
    if (msg.Create(this))
    {
        this->BroadcastSynMsg(&msg);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::SetTenet(LPCTSTR szTenet, bool bSave)
{
    CHECKF(strlen(szTenet) < MAX_TENETSIZE);
    m_pData->SetStr(SYNDATA_TENET, szTenet, MAX_TENETSIZE);
    if (bSave)
    {
        m_pData->Update();
    }
    else
    {
        m_pData->ClearUpdateFlags();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave)
{
    m_pData->SetStr(SYNDATA_ANNOUNCE, szLeaderTitle, MAX_TENETSIZE);
    if (bSave)
    {
        m_pData->Update();
    }
    else
    {
        m_pData->ClearUpdateFlags();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::SetMemberTitle(LPCTSTR szMemberTitle, bool bSave)
{
    m_pData->SetStr(SYNDATA_MEMBERTITLE, szMemberTitle, _MAX_NAMESIZE);
    if (bSave)
    {
        m_pData->Update();
    }
    else
    {
        m_pData->ClearUpdateFlags();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::AddData(SYNDATA idx, int nData, bool bSave)
{
    int nOldMoney	= m_pData->GetInt(SYNDATA_MONEY);
    int nAllData = ::AddToTop(m_pData->GetInt(idx), nData, SYNMONEY_LIMIT);		// use for money or amount
    m_pData->SetInt(idx, nAllData);
    if (bSave)
    {
        m_pData->Update();
    }
    else
    {
        m_pData->ClearUpdateFlags();
    }
    if (nOldMoney > 0 && m_pData->GetInt(SYNDATA_MONEY) <= 0)
    {
        CMsgTalk	msg;
        IF_OK(msg.Create(_TXTATR_GM, STR_DESTROY_SYNDICATE_SOON))
        BroadcastSynMsg(&msg);
    }
    {
        this->SynchroInfo();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::SetData(SYNDATA idx, int nData, bool bSave)
{
    int	nOldData = m_pData->GetInt(idx);
    m_pData->SetInt(idx, nData);
    if (bSave)
    {
        m_pData->Update();
    }
    else
    {
        m_pData->ClearUpdateFlags();
    }
    // synchro
    SynchroInfo();
    if (idx >= SYNDATA_ENEMY0 && idx < SYNDATA_ENEMY0 + MAX_SYNENEMYSIZE)
    {
        CMsgSyndicate msg;
        if (nData)
        {
            IF_OK(msg.Create(SET_ANTAGONIZE, nData))
            BroadcastSynMsg(&msg);
        }
        else
        {
            IF_OK(msg.Create(CLEAR_ANTAGONIZE, nOldData))
            BroadcastSynMsg(&msg);
        }
    }
    else if (idx >= SYNDATA_ALLY0 && idx < SYNDATA_ALLY0 + MAX_SYNALLYSIZE)
    {
        CMsgSyndicate msg;
        if (nData)
        {
            IF_OK(msg.Create(SET_ALLY, nData))
            BroadcastSynMsg(&msg);
        }
        else
        {
            IF_OK(msg.Create(CLEAR_ALLY, nOldData))
            BroadcastSynMsg(&msg);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::PresentMoney(OBJID idSyn, int nMoney, bool bSave)
{
    if (nMoney <= 0 || m_pData->GetInt(SYNDATA_MONEY) < nMoney)
    {
        return false;
    }
    CSyndicate* pTarget = SynManager()->QuerySyndicate(idSyn);
    if (!pTarget)
    {
        return false;
    }
    m_pData->SetInt(SYNDATA_MONEY, m_pData->GetInt(SYNDATA_MONEY) - nMoney);
    pTarget->m_pData->SetInt(SYNDATA_MONEY, pTarget->m_pData->GetInt(SYNDATA_MONEY) + nMoney);
    if (bSave)
    {
        m_pData->Update();
        pTarget->m_pData->Update();
    }
    else
    {
        m_pData->ClearUpdateFlags();
        pTarget->m_pData->ClearUpdateFlags();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::SetSynName(LPCTSTR szSynName, bool bSave)
{
    m_pData->SetStr(SYNDATA_NAME, szSynName, _MAX_NAMESIZE);
    if (bSave)
    {
        m_pData->Update();
    }
    else
    {
        m_pData->ClearUpdateFlags();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::ChangeSyndicate(const SynFuncInfo0* pFuncInfo)
{
    CHECKF(pFuncInfo->idSyn == GetID());
    switch(pFuncInfo->ucFuncType)
    {
    case	SYNFUNC_DEMISE:
        return Demise(pFuncInfo->IntParam[0], pFuncInfo->IntParam[3], pFuncInfo->IntParam[1], (LPCTSTR)&pFuncInfo->IntParam[5], pFuncInfo->IntParam[2], pFuncInfo->IntParam[4], pFuncInfo->ucSaveFlag != 0);
        break;
    case	SYNFUNC_SETTENET:
        {
            bool bRet = (SetTenet(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag != 0)
                         && SetData(SYNDATA_PUBLISHTIME, pFuncInfo->dwData[0], pFuncInfo->ucSaveFlag != 0));
            if (bRet)
            {
                CMsgSyndicate msg;
                if (msg.Create(SYN_SET_PUBLISHTIME, GetID(), GetInt(SYNDATA_PUBLISHTIME)))
                {
                    BroadcastSynMsg(&msg);
                }
            }
            return bRet;
        }
        break;
    case	SYNFUNC_SETLEADERTITLE:
        return SetLeaderTitle(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag != 0);
        break;
    case	SYNFUNC_SETMEMBERTITLE:
        return SetMemberTitle(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag != 0);
        break;
    case	SYNFUNC_ADDDATA:
        return AddData((SYNDATA)pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag != 0);
        break;
    case	SYNFUNC_SETDATA:
        return SetData((SYNDATA)pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag != 0);
        break;
    case	SYNFUNC_PRESENT:
        return PresentMoney(pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag != 0);
        break;
    case	SYNFUNC_SETSYNNAME:
        return SetSynName(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag != 0);
        break;
    default:
        ASSERT(!"switch CSyndicate::ChangeSyndicate()");
        break;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::XSynModify::Demise(OBJID idOldLeader, int nOldSynMemberLevel, OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave)
{
    MESSAGESTR	buf;
    SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
    pInfo->idSyn		= This()->GetID();
    pInfo->ucFuncType	= SYNFUNC_DEMISE;
    pInfo->IntParam[0]	= idOldLeader;
    pInfo->IntParam[1]	= idNewLeader;
    pInfo->IntParam[2]	= idBackSyn;
    pInfo->IntParam[3]	= nOldSynMemberLevel;
    pInfo->IntParam[4]	= nNewSynMemberLevel;
    SafeCopy((char*)(&(pInfo->IntParam[5])), szNewLeader, _MAX_NAMESIZE);
    pInfo->ucSaveFlag		= (unsigned char)bSave;
    pInfo->nSize		= sizeof(SynFuncInfo0) + sizeof(int) * 3 + _MAX_NAMESIZE;
    MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::XSynModify::SetTenet(LPCTSTR szTenet, bool bSave)
{
    MESSAGESTR	buf;
    SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
    pInfo->idSyn		= This()->GetID();
    pInfo->dwData[0]	= DateStamp();
    pInfo->ucFuncType	= SYNFUNC_SETTENET;
    SafeCopy(pInfo->StrParam, szTenet, MAX_TENETSIZE);
    pInfo->ucSaveFlag		= (unsigned char)bSave;
    pInfo->nSize		= sizeof(SynFuncInfo0) + MAX_TENETSIZE;
    MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::XSynModify::SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave)
{
    MESSAGESTR	buf;
    SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
    pInfo->idSyn		= This()->GetID();
    pInfo->ucFuncType	= SYNFUNC_SETLEADERTITLE;
    SafeCopy(pInfo->StrParam, szLeaderTitle, _MAX_NAMESIZE);
    pInfo->ucSaveFlag		= (unsigned char)bSave;
    pInfo->nSize		= sizeof(SynFuncInfo0) + _MAX_NAMESIZE;
    MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::XSynModify::SetMemberTitle(LPCTSTR szMemberTitle, bool bSave)
{
    MESSAGESTR	buf;
    SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
    pInfo->idSyn		= This()->GetID();
    pInfo->ucFuncType	= SYNFUNC_SETMEMBERTITLE;
    SafeCopy(pInfo->StrParam, szMemberTitle, _MAX_NAMESIZE);
    pInfo->ucSaveFlag		= (unsigned char)bSave;
    pInfo->nSize		= sizeof(SynFuncInfo0) + _MAX_NAMESIZE;
    MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::XSynModify::AddData(SYNDATA idx, int nData, bool bSave)
{
    MESSAGESTR	buf;
    SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
    pInfo->idSyn		= This()->GetID();
    pInfo->ucFuncType	= SYNFUNC_ADDDATA;
    pInfo->IntParam[0]	= idx;
    pInfo->IntParam[1]	= nData;
    pInfo->ucSaveFlag	= (unsigned char)bSave;
    pInfo->nSize		= sizeof(SynFuncInfo0) + sizeof(int) * 2;
    if ((idx == SYNDATA_MONEY || idx == SYNDATA_REPUTE) && This()->GetInt(SYNDATA_FEALTY) != ID_NONE)
    {
        CSyndicate* pSyn = This()->GetMasterSyn();
        IF_OK (pSyn)
        pInfo->idSyn = pSyn->GetID();					// 基金合并，声望也合并
    }
    MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::XSynModify::SetData(SYNDATA idx, int nData, bool bSave)
{
    MESSAGESTR	buf;
    SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
    pInfo->idSyn		= This()->GetID();
    pInfo->ucFuncType	= SYNFUNC_SETDATA;
    pInfo->IntParam[0]	= idx;
    pInfo->IntParam[1]	= nData;
    pInfo->ucSaveFlag	= (unsigned char)bSave;
    pInfo->nSize		= sizeof(SynFuncInfo0) + sizeof(int) * 2;
    ASSERT(idx != SYNDATA_MONEY);
    ASSERT(idx != SYNDATA_REPUTE);
    MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::XSynModify::PresentMoney(OBJID idSyn, int nMoney, bool bSave)
{
    MESSAGESTR	buf;
    SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
    pInfo->idSyn		= This()->GetID();
    pInfo->ucFuncType	= SYNFUNC_PRESENT;
    pInfo->IntParam[0]	= idSyn;
    pInfo->IntParam[1]	= nMoney;
    pInfo->ucSaveFlag	= (unsigned char)bSave;
    pInfo->nSize		= sizeof(SynFuncInfo0) + sizeof(int) * 2;
    MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::XSynModify::SetSynName(LPCTSTR szSynName, bool bSave)
{
    MESSAGESTR	buf;
    SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
    pInfo->idSyn		= This()->GetID();
    pInfo->ucFuncType	= SYNFUNC_SETSYNNAME;
    SafeCopy(pInfo->StrParam, szSynName, _MAX_NAMESIZE);
    pInfo->ucSaveFlag		= (unsigned char)bSave;
    pInfo->nSize		= sizeof(SynFuncInfo0) + _MAX_NAMESIZE;
    MapGroup(This()->m_idProcess)->QuerySynMessage()->ChangeSyndicate(pInfo);
    return true;
}

//////////////////////////////////////////////////////////////////////
// application
//////////////////////////////////////////////////////////////////////
int	CSyndicate::GetInt(SYNDATA idx)
{
    OBJID idFealty = m_pData->GetInt(SYNDATA_FEALTY);		// no loop
    if (idFealty != ID_NONE)
    {
        if (idx == SYNDATA_MONEY		// 基金合并
                || idx >= SYNDATA_ENEMY0 && idx < SYNDATA_ENEMY0 + MAX_SYNENEMYSIZE
                || idx >= SYNDATA_ALLY0 && idx < SYNDATA_ALLY0 + MAX_SYNALLYSIZE)
        {
            CSyndicate* pSyn = GetMasterSyn();
            IF_OK (pSyn)
            return pSyn->GetInt(idx);		// no loop
        }
    }
    return m_pData->GetInt(idx);		// no loop
}

//////////////////////////////////////////////////////////////////////
// 在本地线程广播子帮派消息
void CSyndicate::BroadcastLocalSynMsg(CNetMsg* pMsg, CUser* pExcludeSender)
{
    CHECK(pMsg);
    CUserManager::Iterator pUser = UserManager()->NewEnum();
    while(pUser.Next())
    {
        if (pUser && pUser->GetSynID() == GetID()
                && !(pExcludeSender && pExcludeSender->GetID() == pUser->GetID()) )
        {
            pUser->SendMsg(pMsg);
        }
    }
}

//////////////////////////////////////////////////////////////////////
// 在本线程中广播总帮派消息(含子帮派)
void CSyndicate::BroadcastSynMsg(CNetMsg* pMsg, CUser* pExcludeSender)
{
    CSyndicate* pMaster = this->GetMasterSyn();
    CHECK(pMaster);
    pMaster->BroadcastLocalSynMsg(pMsg, pExcludeSender);
    {
        OBJID idMaster = pMaster->GetID();
        CHECK(idMaster != ID_NONE);
        for(int i = 0; i < SynManager()->QuerySynSet()->GetAmount(); i++)
        {
            CSynPtr pSyn = SynManager()->QuerySynSet()->GetObjByIndex(i);
            if (pSyn && pSyn->GetInt(SYNDATA_FEALTY) == idMaster)
            {
                //分队-----begin
                for(int j = 0; j < SynManager()->QuerySynSet()->GetAmount(); j++)
                {
                    CSyndicate* pSynTeam = SynManager()->QuerySynSet()->GetObjByIndex(j);
                    if (pSynTeam && pSynTeam->GetInt(SYNDATA_FEALTY) == pSyn->GetID())
                    {
                        pSynTeam->BroadcastLocalSynMsg(pMsg, pExcludeSender);
                    }
                }
                //分队-----end
                pSyn->BroadcastLocalSynMsg(pMsg, pExcludeSender);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
// 在世界中广播总帮派消息(含子帮派)
void CSyndicate::BroadcastSubSynMsg(SOCKET_ID idSocket, OBJID idNpc, LPCTSTR szMsg, CUser* pExcludeSender, LPCTSTR pszEmotion, DWORD dwColor, int nAttribute)
{
    CHECK(szMsg);
    CMsgTalk	msg;
    IF_OK(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, pszEmotion, dwColor, nAttribute))
    BroadcastSynMsg(&msg, pExcludeSender);
    MapGroup(PID)->QueryIntraMsg()->TransmitMsg(&msg, idSocket, idNpc);
}

//////////////////////////////////////////////////////////////////////
LPCTSTR CSyndicate::GetFealtyName()
{
    OBJID idFealty = GetInt(SYNDATA_FEALTY);
    if (idFealty)
    {
        CSyndicate* pSyn = SynManager()->QuerySyndicate(idFealty);
        if (pSyn)
        {
            return pSyn->GetStr(SYNDATA_NAME);
        }
    }
    return SYNNAME_NONE;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::IsNearSyn(OBJID idTarget)
{
    if (idTarget == ID_NONE)
    {
        return false;
    }
    if (GetID() == idTarget)
    {
        return true;
    }
    CSyndicate* pTarget = SynManager()->QuerySyndicate(idTarget);
    if (!pTarget)		// 帮派柱子的帮派ID可能无效
    {
        return false;
    }
    if (pTarget->GetInt(SYNDATA_FEALTY) == ID_NONE)
    {
        if (GetInt(SYNDATA_FEALTY) == idTarget)
        {
            return true;
        }
    }
    else
    {
        if (pTarget->GetInt(SYNDATA_FEALTY) == GetID() || pTarget->GetInt(SYNDATA_FEALTY) == GetInt(SYNDATA_FEALTY))
        {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
CSyndicate* CSyndicate::GetMasterSyn()
{
    if (GetInt(SYNDATA_FEALTY) != ID_NONE)
    {
        CSyndicate* pFealtySyn = SynManager()->QuerySyndicate(GetInt(SYNDATA_FEALTY));
        IF_OK(pFealtySyn)
        {
            return pFealtySyn->GetMasterSyn();
        }
    }
    return this;
}

//////////////////////////////////////////////////////////////////////
void CSyndicate::SendInfoToClient(CUser* pUser)
{
    CHECK(pUser);
    for(int i = 0; i < MAX_SYNENEMYSIZE; i++)
    {
        if (this->GetInt((SYNDATA)(SYNDATA_ENEMY0 + i)) != ID_NONE)
        {
            CMsgSyndicate	msg;
            IF_OK(msg.Create(SET_ANTAGONIZE, this->GetInt((SYNDATA)(SYNDATA_ENEMY0 + i))))
            pUser->SendMsg(&msg);
        }
    }
    for(int i = 0; i < MAX_SYNALLYSIZE; i++)
    {
        if (this->GetInt((SYNDATA)(SYNDATA_ALLY0 + i)) != ID_NONE)
        {
            CMsgSyndicate	msg;
            IF_OK(msg.Create(SET_ALLY, this->GetInt((SYNDATA)(SYNDATA_ALLY0 + i))))
            pUser->SendMsg(&msg);
        }
    }
    /*	OBJID idMaster = GetID();
    	if (GetInt(SYNDATA_FEALTY) != 0)
    		idMaster = GetInt(SYNDATA_FEALTY);
    	CHECK(idMaster != ID_NONE);
    	{
    		CMsgSyndicate	msg;
    		IF_OK(msg.Create(SET_MAINSYN, idMaster))
    			pUser->SendMsg(&msg);
    	}
    	for(int i = 0;  i < SynManager()->QuerySynSet()->GetAmount(); i++)
    	{
    		CSynPtr pSyn = SynManager()->QuerySynSet()->GetObjByIndex(i);
    		if (pSyn && pSyn->GetInt(SYNDATA_FEALTY) == idMaster)
    		{
    			CMsgSyndicate	msg;
    			IF_OK(msg.Create(SET_SUBSYN, pSyn->GetID()))
    				pUser->SendMsg(&msg);
    		}
    	}*/
    OBJID idSyn = this->GetID();
    CMsgSyndicate	msg;
    IF_OK(msg.Create(SET_SYN, idSyn, this->GetInt(SYNDATA_FEALTY)))
    pUser->SendMsg(&msg);
    IF_OK(msg.Create(SYN_SET_PUBLISHTIME, idSyn, this->GetInt(SYNDATA_PUBLISHTIME)))
    pUser->SendMsg(&msg);
    // 子帮派
    for(int i = 0; i < SynManager()->QuerySynSet()->GetAmount(); i++)
    {
        CSyndicate* pSyn = SynManager()->QuerySynSet()->GetObjByIndex(i);
        if (pSyn && pSyn->GetInt(SYNDATA_FEALTY) == idSyn)
        {
            pSyn->SendInfoToClient(pUser);
        }
    }
    if (this->IsMasterSyn())
    {
        LPCTSTR pTenet = GetStr(SYNDATA_TENET);
        if (strlen(pTenet) > 0)
        {
            pUser->SendSysMsg(_TXTATR_SYNTENET, pTenet);
        }
        LPCTSTR pAnnounce = GetStr(SYNDATA_ANNOUNCE);
        if (strlen(pAnnounce) > 0)
        {
            pUser->SendSysMsg(_TXTATR_SYNANNOUNCE, pAnnounce);
        }
    }
}

//////////////////////////////////////////////////////////////////////
// syn war -------------------------------------
//////////////////////////////////////////////////////////////////////
void CSyndicate::AddSynWarScore(OBJID idNpc, int nScore)
{
    for(int i = 0; i < m_setSynWarScore.size(); i++)
    {
        if (m_setSynWarScore[i].idNpc == idNpc)
        {
            m_setSynWarScore[i].nScore	+= nScore;
            return;
        }
    }
    SynWarInfo	info;
    info.idNpc		= idNpc;
    info.nScore		= nScore;
    m_setSynWarScore.push_back(info);
}

//////////////////////////////////////////////////////////////////////
int CSyndicate::GetSynWarScore(OBJID idNpc)
{
    for(int i = 0; i < m_setSynWarScore.size(); i++)
    {
        if (m_setSynWarScore[i].idNpc == idNpc)
        {
            return m_setSynWarScore[i].nScore;
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////
void CSyndicate::ClearSynWarScore(OBJID idNpc)
{
    for(int i = 0; i < m_setSynWarScore.size(); i++)
    {
        if (m_setSynWarScore[i].idNpc == idNpc)
        {
            m_setSynWarScore.erase(m_setSynWarScore.begin() + i);
            return ;
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::IsFriendly(OBJID idTarget, BOOL bNest)
{
    if (idTarget == ID_NONE)
    {
        return false;
    }
    CSyndicate* pTarget = SynManager()->QuerySyndicate(idTarget);
    CHECKF(pTarget);
    idTarget = pTarget->GetMasterSynID();
    if (bNest && GetInt(SYNDATA_FEALTY) != ID_NONE)
    {
        CSyndicate* pSyn = SynManager()->QuerySyndicate(GetInt(SYNDATA_FEALTY));
        IF_OK(pSyn)
        return pSyn->IsFriendly(idTarget);
    }
    for(int i = SYNDATA_ALLY0; i < SYNDATA_ALLY0 + MAX_SYNALLYSIZE; i++)
    {
        if (GetInt((SYNDATA)i) == idTarget)
        {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::IsHostile(OBJID idTarget, BOOL bNest)
{
    CSyndicate* pTarget = SynManager()->QuerySyndicate(idTarget);
    idTarget = pTarget->GetMasterSynID();
    if (bNest && GetInt(SYNDATA_FEALTY) != ID_NONE)
    {
        CSyndicate* pSyn = SynManager()->QuerySyndicate(GetInt(SYNDATA_FEALTY));
        IF_OK(pSyn)
        return pSyn->IsHostile(idTarget);
    }
    for(int i = SYNDATA_ENEMY0; i < SYNDATA_ENEMY0 + MAX_SYNENEMYSIZE; i++)
    {
        if (GetInt((SYNDATA)i) == idTarget)
        {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::IsNeutral(OBJID idSyn)
{
    if (!IsFriendly(idSyn) && !IsHostile(idSyn))
    {
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
int CSyndicate::GetMemberAmount(bool bIncludeSub)
{
    int nMemberSum	= GetInt(SYNDATA_AMOUNT);
    if (bIncludeSub)
    {
        // 子帮派人数
        for(int i = 0; i < SynManager()->QuerySynSet()->GetAmount(); i++)
        {
            CSyndicate* pSyn = SynManager()->QuerySynSet()->GetObjByIndex(i);
            if (pSyn && pSyn->GetInt(SYNDATA_FEALTY) == this->GetID())
            {
                nMemberSum += pSyn->GetMemberAmount(bIncludeSub);
            }
        }
    }
    return nMemberSum;
}

//////////////////////////////////////////////////////////////////////
int CSyndicate::GetSynAmount()
{
    if (!IsMasterSyn())
    {
        CSyndicate* pMainSyn = this->GetMasterSyn();
        CHECKF(pMainSyn);
        return pMainSyn->GetMemberAmount(true);
    }
    return GetMemberAmount(true);
}

//////////////////////////////////////////////////////////////////////
void CSyndicate::SetNewSubName(LPCTSTR szName)		// 临时代码
{
    m_pData->SetStr(SYNDATA_NAME, szName, _MAX_NAMESIZE);
    m_pData->Update();
}

//////////////////////////////////////////////////////////////////////@
bool CSyndicate::IsWhiteSynMapOwner()
{
    return GetID() == SynManager()->GetMapSynID(SET_WHITE_SYN);
}

//////////////////////////////////////////////////////////////////////@
bool CSyndicate::IsBlackSynMapOwner()
{
    return GetID() == SynManager()->GetMapSynID(SET_BLACK_SYN);
}

//////////////////////////////////////////////////////////////////////@
bool CSyndicate::ClearAllySyn(CSyndicate* pTargetSyn)
{
    // 检查帮派
    CHECKF (pTargetSyn);
    for(int i = 0; i < MAX_SYNALLYSIZE; i++)
    {
        OBJID idTarget = GetInt((SYNDATA)(SYNDATA_ALLY0 + i));
        if (idTarget == pTargetSyn->GetID())
        {
            QueryModify()->SetData((SYNDATA)(SYNDATA_ALLY0 + i), ID_NONE, true);		// true: 必须save
            break;
        }
    }
    for(int i = 0; i < MAX_SYNALLYSIZE; i++)
    {
        OBJID idTarget = pTargetSyn->GetInt((SYNDATA)(SYNDATA_ALLY0 + i));
        if (idTarget == GetID())
        {
            pTargetSyn->QueryModify()->SetData((SYNDATA)(SYNDATA_ALLY0 + i), ID_NONE, true);		// true: 必须save
            break;
        }
    }
    CMsgSyndicate	msg;
    IF_OK(msg.Create(CLEAR_ALLY, pTargetSyn->GetID()))
    MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(GetID(), &msg);
    return true;
}

//////////////////////////////////////////////////////////////////////@
bool CSyndicate::ClearEnemySyn(CSyndicate* pTargetSyn)
{
    // 检查帮派
    CHECKF (pTargetSyn);
    for(int i = 0; i < MAX_SYNENEMYSIZE; i++)
    {
        OBJID idTarget = GetInt((SYNDATA)(SYNDATA_ENEMY0 + i));
        if (idTarget == pTargetSyn->GetID())
        {
            QueryModify()->SetData((SYNDATA)(SYNDATA_ENEMY0 + i), ID_NONE, true);		// true: 必须save
            break;
        }
    }
    CMsgSyndicate	msg;
    IF_OK(msg.Create(CLEAR_ANTAGONIZE, pTargetSyn->GetID()))
    MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(GetID(), &msg);
    return true;
}

//////////////////////////////////////////////////////////////////////@
// add huang 2004.1.7
bool CSyndicate::CheckDismissSyn()
{
    if (this->IsFallLevel())
    {
        return true;
    }
    else
    {
        return false;
    }
    /*	bool bCheck = false;
    	CMsgTalk msg;
    	if (GetInt(SYNDATA_MONEY) < DISMISSSYN_MONEY_LIMIT)
    	{
    		IF_OK(msg.Create(_TXTATR_SYNDICATE, STR_SYN_MONEY_LIMIT))
    			BroadcastSynMsg(&msg);
    		bCheck = true;
    	}
    	if (GetInt(SYNDATA_AMOUNT) < DISMISSSYN_AMOUNT_LIMIT)
    	{
    		IF_OK(msg.Create(_TXTATR_SYNDICATE, STR_SYN_AMOUNT_LIMIT))
    			BroadcastSynMsg(&msg);
    		bCheck = true;
    	}
    	if (!bCheck)
    		return false;
    	return true;
    	*/
}

bool CSyndicate::CheckDismissTime()
{
    time_t tTime = time(NULL);
    if (GetInt(SYNDATA_DISMISS_TIME) < tTime - DISMISSSYN_TIME_LIMIT)
    {
        return true;
    }
    return false;
}

bool CSyndicate::AutoDismissSyn()
{
    IF_OK(GetInt(SYNDATA_FEALTY) == 0)
    {
        if (CheckDismissSyn())
        {
            if (GetInt(SYNDATA_DISMISS_TIME))
            {
                if (CheckDismissTime())
                {
                    //对帮派降级
                    if (this->FallLevel())
                    {
                        return true;
                    }
                    /*	CMsgSyndicate	msg;
                    	IF_OK(msg.Create(DESTROY_SYN, this->GetID()))
                    		UserManager()->BroadcastMsg(&msg);
                    	IF_OK(SynManager()->QueryModify()->DestroySyndicate(this->GetID()))
                    		return true;
                    */
                }
            }
            else
            {
                int tTime = time(NULL);
                QueryModify()->SetData(SYNDATA_DISMISS_TIME, tTime, true);
            }
        }
        else
        {
            QueryModify()->SetData(SYNDATA_DISMISS_TIME, 0, true);
        }
    }
    return false;
}

int CSyndicate::GetMoney()
{
    return GetInt(SYNDATA_MONEY);
}

int CSyndicate::GetSubSynNum()
{
    int nNum = 0;
    for (int i = 0; i < SynManager()->QuerySynSet()->GetAmount(); i++)
    {
        CSyndicate* pSyn = SynManager()->QuerySynSet()->GetObjByIndex(i);
        if (pSyn && pSyn->GetInt(SYNDATA_FEALTY) == this->GetID())
        {
            ++nNum;
        }
    }
    return nNum;
    //	CSynSet::Iterator pSyn = SynManager()->QuerySynSet()->NewEnum();
    //	while(pSyn.Next())
    //	{
    //		if (pSyn && pSyn->GetInt(SYNDATA_FEALTY) == GetID())
    //			nNum ++;
    //	}
    //	return nNum;
}
//副帮主申请开除会员
bool CSyndicate::ApplyKickoutMember(CUserPtr pLeader, CUserPtr pTarget)
{
    DEBUG_TRY
    OBJID idSynLeader = pLeader->GetSynID(), idTarget = pLeader->GetSynID();
    if (idSynLeader != idTarget)
    {
        return false;
    }
    int nRank = pLeader->GetSynRank();
    if (nRank != RANK_SUBLEADER || nRank != RANK_TEAMLEADER)
    {
        return false;
    }
    CSyndicate* pSyn = pLeader->GetSyndicate();
    if (!pSyn)
    {
        return false;
    }
    char szWords[256];
    memset(szWords, 0, sizeof(szWords));
    //特定格式的留言（开除会员的申请）
    sprintf(szWords, "ApplyDismiss %d %s %s", pSyn->GetID(), pLeader->GetName(), pTarget->GetName() );
    //插入留言
    LeaveWord()->InsertWords(pLeader->GetName(), pSyn->GetStr(SYNDATA_LEADERNAME), szWords);
    DEBUG_CATCH("bool CSyndicate::ApplyKickoutMember(CUserPtr pLeader, CUserPtr pTarget)")
    return true;
}
//帮派每周会在统一的地方发放本周军团利润的一部分作为成员所得
/*
CUserPtr pUser 帮主
DWORD dwBonus 奖金
*/
bool CSyndicate::AllotBonusForMember(CUserPtr pUser, DWORD dwBonus)
{
    //得到帮派基金总和，值不能超过军团基金的50%。
    if (dwBonus >= (this->GetMoney() / 2))
    {
        return false;
    }
    OBJID idMap = pUser->GetMapID();
    OBJID idSyn = pUser->GetSynID();
    DWORD dwProfferSum = 0, dwProffer = 0, dwLevel = 0, dwMoney = 0;
    typedef std::vector<CUser*>		USER_SET;
    USER_SET	setUser;
    //得到在场的所有的帮派成员的增量贡献值
    CUserManager::Iterator pCurUser = UserManager()->NewEnum();
    while(pCurUser.Next())
    {
        if (pCurUser && pCurUser->GetMapID() == idMap && pCurUser->IsMemberOfSyn(idSyn))
        {
            setUser.push_back(pCurUser);
            dwProfferSum = ::AddToTop(pCurUser->QuerySynAttr()->GetProfferInc(), dwProfferSum, SYNPROFFER_LIMIT);
        }
    }
    if (!dwProfferSum)
    {
        return false;
    }
    // 先扣钱
    this->QueryModify()->AddData(SYNDATA_MONEY, -1 * dwBonus);
    //给每位在场的帮派成员根据他们的贡献值分配奖金
    for (int i = 0; i < setUser.size(); i++)
    {
        CUser* pUser = setUser[i];
        IF_NOT (pUser)
        continue;
        {
            dwProffer	= pUser->QuerySynAttr()->GetProfferInc();
            dwLevel		= pUser->QuerySynAttr()->GetSynMemberLevel();
            int nRank	= pUser->GetSynRankShow();
            if (nRank == RANK_TEAMLEADER)
            {
                nRank = 3;
            }
            else if (nRank == RANK_SUBLEADER || nRank == RANK_LEADER)
            {
                nRank = 5;
            }
            else
            {
                nRank = 0;
            }
            dwLevel = __max(dwLevel, nRank);
            const int	ADDITION_PERCENT_PER_LEVEL	= 5;
            dwMoney = ((__int64)dwProffer * (100 + dwLevel * ADDITION_PERCENT_PER_LEVEL) * (__int64)dwBonus) / 100 / dwProfferSum;
            //				dwMoney = ::CutRange(MulDiv(dwProffer, (1 + MulDiv(dwLevel,nPercent,100))* dwBonus, dwProfferSum), 0, SYNMONEY_LIMIT);
            pUser->AddMoney(dwMoney);
            pUser->QuerySynAttr()->ResetSynProfferInc();
        }
    }
    return true;
}

//是否可以创建分队
bool CSyndicate::IfCanCreateSubSubSyn()
{
    int nNum = this->GetSubSubSynNum();
    if (nNum < 0)
    {
        return false;
    }
    int nSynRank = GetInt(SYNDATA_RANK);
    switch(nSynRank)
    {
    case SYNRANK_ONE:
        {
            return false;
        }
        break;
    case SYNRANK_TWO:
        {
            if (nNum >= 2)
            {
                return false;
            }
        }
        break;
    case SYNRANK_THREE:
        {
            if (nNum >= 2)
            {
                return false;
            }
        }
        break;
    case SYNRANK_FOUR:
        {
            if (nNum >= 3)
            {
                return false;
            }
        }
        break;
    default :
        return false;
        break;
    }
    return true;
}
//得到分团的分队个数
int CSyndicate::GetSubSubSynNum()
{
    if (GetInt(SYNDATA_FEALTY) == ID_NONE) //必须是分团才可以调用这个函数
    {
        return -1;
    }
    int nNum = 0;
    CSynSet::Iterator pSyn = SynManager()->QuerySynSet()->NewEnum();
    while(pSyn.Next())
    {
        if (pSyn && pSyn->GetInt(SYNDATA_FEALTY) == GetID())
        {
            //是分团的分队
            nNum ++;
        }
    }
    return nNum;
}
//是否是 分团
bool CSyndicate::IsSubSyn()
{
    if (this->IsMasterSyn())
    {
        return false;
    }
    OBJID idFealtySyn = GetInt(SYNDATA_FEALTY);
    CSyndicate* pFealtySyn = SynManager()->QuerySyndicate(idFealtySyn);
    if (pFealtySyn->IsMasterSyn())
    {
        return true;
    }
    else
    {
        return false;
    }
}
//是否是分队
bool CSyndicate::IsSubSubSyn()
{
    if (this->IsMasterSyn())
    {
        return false;
    }
    OBJID idFealtySyn = GetInt(SYNDATA_FEALTY);
    CSyndicate* pFealtySyn = SynManager()->QuerySyndicate(idFealtySyn);
    if (pFealtySyn->IsMasterSyn())
    {
        return false;
    }
    else
    {
        return true;
    }
}
//是否对帮派进行降级
bool CSyndicate::IsFallLevel()
{
    if (!IsMasterSyn())
    {
        return false;
    }
    int nSynRank = GetInt(SYNDATA_RANK);
    int nMemSum = this->GetSynAmount();
    int nLimit  = 0;
    //一级军团不降级
    switch(nSynRank)
    {
    case SYNRANK_TWO:
        {
            nLimit = MulDiv(SYNMEMSUM_LOWERLIMIT_TWO, 80, 100);
            if (nMemSum <= nLimit)
            {
                return true;
            }
        }
        break;
    case SYNRANK_THREE:
        {
            nLimit = MulDiv(SYNMEMSUM_LOWERLIMIT_THREE, 80, 100);
            if (nMemSum <= nLimit)
            {
                return true;
            }
        }
        break;
    case SYNRANK_FOUR:
        {
            nLimit = MulDiv(SYNMEMSUM_LOWERLIMIT_FOUR, 80, 100);
            if (nMemSum <= nLimit)
            {
                return true;
            }
        }
        break;
    default :
        break;
    }
    return false;
}
//对帮派进行降级，调用这个函数前先检查是否允许降级
bool CSyndicate::FallLevel()
{
    if (!this->IsMasterSyn())
    {
        return false;
    }
    if (GetInt(SYNDATA_RANK) == SYNRANK_ONE)
    {
        SynManager()->QueryModify()->DestroySyndicate(GetID());
        return true;
    }
    //---将所有子帮派的成员合并到 主帮派中
    for(int i = 0; i < SynManager()->QuerySynSet()->GetAmount(); i++)
    {
        CSyndicate* pSyn = SynManager()->QuerySynSet()->GetObjByIndex(i);
        if (pSyn && pSyn->GetInt(SYNDATA_FEALTY) == this->GetID())
        {
            SynManager()->QueryModify()->CombineSyndicate(pSyn->GetID(), this->GetID());
        }
    }
    //降级
    this->QueryModify()->AddData(SYNDATA_RANK, -SYNRANK_ONE, true);
    return true;
}

OBJID CSyndicate::GetMasterSynID()
{
    if (GetInt(SYNDATA_FEALTY) != ID_NONE)
    {
        CSyndicate* pFealtySyn = SynManager()->QuerySyndicate(GetInt(SYNDATA_FEALTY));
        IF_OK(pFealtySyn)
        {
            return pFealtySyn->GetMasterSynID();
        }
    }
    return GetID();
}

void CSyndicate::SynchroInfo()
{
    CSyndicate* pMaster = this->GetMasterSyn();
    CHECK(pMaster);
    CUserManager::Iterator pUser = UserManager()->NewEnum();
    while(pUser.Next())
    {
        if (pUser && pUser->GetSynID() == pMaster->GetID())
        {
            pUser->QuerySynAttr()->SendInfoToClient();
        }
    }
    {
        OBJID idMaster = pMaster->GetID();
        CHECK(idMaster != ID_NONE);
        for(int i = 0; i < SynManager()->QuerySynSet()->GetAmount(); i++)
        {
            CSynPtr pSyn = SynManager()->QuerySynSet()->GetObjByIndex(i);
            if (pSyn && pSyn->GetInt(SYNDATA_FEALTY) == idMaster)
            {
                //分队-----begin
                for(int j = 0; j < SynManager()->QuerySynSet()->GetAmount(); j++)
                {
                    CSyndicate* pSynTeam = SynManager()->QuerySynSet()->GetObjByIndex(j);
                    if (pSynTeam && pSynTeam->GetInt(SYNDATA_FEALTY) == pSyn->GetID())
                    {
                        CUserManager::Iterator pUser = UserManager()->NewEnum();
                        while(pUser.Next())
                        {
                            if (pUser && pUser->GetSynID() == pSynTeam->GetID())
                            {
                                pUser->QuerySynAttr()->SendInfoToClient();
                            }
                        }
                    }
                }
                //分队-----end
                CUserManager::Iterator pUser = UserManager()->NewEnum();
                while(pUser.Next())
                {
                    if (pUser && pUser->GetSynID() == pSyn->GetID())
                    {
                        pUser->QuerySynAttr()->SendInfoToClient();
                    }
                }
            }
        }
    }
}
