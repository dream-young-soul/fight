
//**********************************************************
// 代码编辑器
//**********************************************************

// SynWorldShell.cpp: implementation of the CSyndicateWorld class.
//
//////////////////////////////////////////////////////////////////////

#define	_WINSOCKAPI_		// 阻止加载winsock.h
#include "AllMsg.h"
#include "I_MessagePort.h"
#include "define.h"
#include "MessageBoard.h"
#include "UserList.h"
#include "SynWorldShell.h"
#include "WorldKernel.h"

//////////////////////////////////////////////////////////////////////
const int	SYNMONEY_LIMIT		= 2000000000;		// 基金的限制额
const int	MEMBERLIST_SIZE		= 18;				// 帮众列表的每次数量
const int	MEMBERLIST_LIMIT	= 1234567890;		// 帮众列表的限制额

MYHEAP_IMPLEMENTATION(CSyndicateWorld, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSyndicateWorld::CSyndicateWorld()
{
    m_pData	= NULL;
    m_ptrMsgBd	= CMessageBoard::CreateNew(_TXTATR_MSG_SYN);
}

//////////////////////////////////////////////////////////////////////
CSyndicateWorld::~CSyndicateWorld()
{
    if (m_pData)
    {
        m_pData->Update();
        m_pData->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::Create(IRecordset* pRes)
{
    ASSERT(!m_pData);
    m_pData = CSynWorldBase::CreateNew();
    CHECKF(m_pData);
    IF_NOT(m_pData->Create(pRes))
    return false;
    if (m_pData->GetInt(SYNDATA_DEL_FLAG) != 0)		// 已删除的帮派
    {
        return false;
    }
    // 统计帮派人数
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT COUNT(*) FROM %s as s LEFT JOIN %s AS u ON u.id=s.id WHERE s.syn_id=%u && u.id IS NOT NULL", _TBL_SYNATTR, _TBL_USER, m_pData->GetID());
    IRecordset* pStatRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);
    if (pStatRes)
    {
        m_pData->SetInt(SYNDATA_AMOUNT, pStatRes->GetInt(0));	// 0: count
        m_pData->Update();
        pStatRes->Release();
    }
    // 载入帮众列表
    sprintf(szSQL, "SELECT u.name, s.rank FROM %s as s LEFT JOIN %s AS u ON u.id=s.id WHERE s.syn_id=%u && u.id IS NOT NULL ORDER BY s.rank DESC, u.name LIMIT %d",
            _TBL_SYNATTR, _TBL_USER, m_pData->GetID(), MEMBERLIST_LIMIT);
    pStatRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);
    if (pStatRes)
    {
        for(int i = 0; i < pStatRes->RecordCount(); i++, pStatRes->MoveNext())
        {
            m_setMember.push_back(MemberInfo(pStatRes->GetStr(0), pStatRes->GetInt(1)));	// 0: name, 1: rank
        }
        pStatRes->Release();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::Create(IRecordset* pDefault, const CreateSyndicateInfo* pInfo, bool bSave)
{
    ASSERT(!m_pData);
    OBJID idSyn = bSave ? ID_NONE : pInfo->idSyn;
    m_pData = CSynWorldBase::CreateNew();
    CHECKF(m_pData);
    IF_NOT(m_pData->Create(pDefault, idSyn))
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
bool CSyndicateWorld::Demise(OBJID idOldLeader, int nOldSynMemberLevel, OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave)
{
    if (!( idOldLeader == (OBJID)m_pData->GetInt(SYNDATA_LEADERID) || (OBJID)m_pData->GetInt(SYNDATA_LEADERID) == ID_NONE ))	//? 兼容于旧数据
    {
        return false;
    }
    NAMESTR	szOldLeader;
    SafeCopy(szOldLeader, m_pData->GetStr(SYNDATA_LEADERNAME), _MAX_NAMESIZE);
    m_pData->SetInt(SYNDATA_LEADERID, idNewLeader);
    m_pData->SetStr(SYNDATA_LEADERNAME, szNewLeader, _MAX_NAMESIZE);
    if (bSave)
    {
        m_pData->Update();
    }
    else
    {
        m_pData->ClearUpdateFlags();
    }
    int	RANK_NEWBIE		= 20;
    int RANK_LEADER		= 100;
    SQLBUF	szSQL;
    sprintf(szSQL, "UPDATE %s SET syn_id=%u,rank=%d WHERE id=%u LIMIT 1",
            _TBL_SYNATTR, idBackSyn, RANK_NEWBIE, idOldLeader);
    GameWorld()->GetDatabase()->ExecuteSQL(szSQL);
    // synchro member list
    if (GetID() != idBackSyn)
    {
        CSyndicateWorld* pBackSyn = SynWorldManager()->QuerySyndicate(idBackSyn);
        IF_OK(pBackSyn)
        {
            DelMember(szOldLeader);
            pBackSyn->AddMember(szOldLeader, RANK_NEWBIE);
            pBackSyn->DelMember(szNewLeader);
            AddMember(szNewLeader, RANK_LEADER);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::SetTenet(LPCTSTR szTenet, bool bSave)
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
bool CSyndicateWorld::SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave)
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
bool CSyndicateWorld::SetMemberTitle(LPCTSTR szMemberTitle, bool bSave)
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
bool CSyndicateWorld::AddData(SYNDATA idx, int nData, bool bSave)
{
    int nAllData = ::AddToTop(m_pData->GetInt(idx), nData, SYNMONEY_LIMIT);		// 用于金钱和帮众数量
    m_pData->SetInt(idx, nAllData);
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
bool CSyndicateWorld::SetData(SYNDATA idx, int nData, bool bSave)
{
    m_pData->SetInt(idx, nData);
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
bool CSyndicateWorld::PresentMoney(OBJID idSyn, int nMoney, bool bSave)
{
    if (nMoney <= 0 || m_pData->GetInt(SYNDATA_MONEY) < nMoney)
    {
        return false;
    }
    CSyndicateWorld* pTarget = GameWorld()->QuerySynManager()->QuerySyndicate(idSyn);
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
    ::GmLogSave("EVENT: 帮派[%s]向帮派[%s]划拨基金[%d]两银子。", GetStr(SYNDATA_NAME), pTarget->GetStr(SYNDATA_NAME), nMoney);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::SetSynName(LPCTSTR szSynName, bool bSave)
{
    m_pData->SetStr(SYNDATA_NAME, szSynName, MAX_TENETSIZE);
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
bool CSyndicateWorld::ChangeSyndicate(const SynFuncInfo0* pFuncInfo)
{
    bool ret = false;
    switch(pFuncInfo->ucFuncType)
    {
    case	SYNFUNC_DEMISE:
        ret = Demise(pFuncInfo->IntParam[0], pFuncInfo->IntParam[3], pFuncInfo->IntParam[1], (LPCTSTR)&pFuncInfo->IntParam[5], pFuncInfo->IntParam[2], pFuncInfo->IntParam[4], pFuncInfo->ucSaveFlag != 0);
        break;
    case	SYNFUNC_SETTENET:
        ret = (SetTenet(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag != 0)
               && SetData(SYNDATA_PUBLISHTIME, pFuncInfo->dwData[0], pFuncInfo->ucSaveFlag != 0));
        break;
    case	SYNFUNC_SETLEADERTITLE:
        ret = SetLeaderTitle(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag != 0);
        break;
    case	SYNFUNC_SETMEMBERTITLE:
        ret = SetMemberTitle(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag != 0);
        break;
    case	SYNFUNC_ADDDATA:
        ret = AddData((SYNDATA)pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag != 0);
        break;
    case	SYNFUNC_SETDATA:
        ret = SetData((SYNDATA)pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag != 0);
        break;
    case	SYNFUNC_PRESENT:
        ret = PresentMoney(pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag != 0);
        break;
    case	SYNFUNC_SETSYNNAME:
        ret = SetSynName(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag != 0);
        break;
    case	SYNFUNC_ADD_MEMBER:
        ret = AddMember(pFuncInfo->StrParam, pFuncInfo->nRank);
        return ret;		// not broadcast
        break;
    case	SYNFUNC_DEL_MEMBER:
        ret = DelMember(pFuncInfo->StrParam);
        return ret;		// not broadcast
        break;
    default:
        ASSERT(!"switch CSynShell::ChangeSyndicate()");
        return false;
        break;
    }
    if (ret)
    {
        // broadcast
        MESSAGESTR	buf;
        SynFuncInfo0*	pInfo = (SynFuncInfo0*)buf;
        memcpy(buf, pFuncInfo, MAX_MESSAGESIZE);
        pInfo->ucSaveFlag	= (unsigned char)false;
        GameWorld()->QuerySynMessage()->ChangeSyndicate(pInfo);
    }
    return ret;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::AddMember(LPCTSTR szName, int nUserRank)
{
    CHECKF(szName);
    ASSERT(!DelMember(szName));
    m_setMember.push_back(MemberInfo(szName, nUserRank));
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::DelMember(LPCTSTR szName)
{
    CHECKF(szName);
    for(int i = m_setMember.size() - 1; i >= 0; i--)		// delete, reverse traverse
    {
        if (strcmp(m_setMember[i].szName.c_str(), szName) == 0)
        {
            m_setMember.erase(m_setMember.begin() + i);
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
enum
{
    RANK_LEADER			= 100,			//军团团长
    RANK_SUBLEADER		= 80,			//分团长
    RANK_TEAMLEADER		= 60,			//队长
    RANK_NEWBIE			= 20,			//普通团员
    RANK_RESERVED		= 10,			//预备团员
    RANK_NONE			= 0,

    RANK_SUB		= RANK_LEADER - RANK_SUBLEADER,
};

int CSyndicateWorld::GetSynRankShow(int nRank)
{
    int nRankShow = nRank;
    if (nRankShow > RANK_NEWBIE)
    {
        OBJID idFealty	= GetInt(SYNDATA_FEALTY);
        while (idFealty != ID_NONE)
        {
            CSyndicateWorld* pSyn = GameWorld()->QuerySynManager()->QuerySyndicate(idFealty);
            if (!pSyn)
            {
                break;
            }
            nRankShow -= RANK_SUB;
            idFealty = pSyn->GetInt(SYNDATA_FEALTY);
        }
    }
    return nRankShow;
}

//////////////////////////////////////////////////////////////////////
/*
int CSyndicateWorld::AppendOnlineMember(class CMsgName& msg, int idx)
{
	int	nAmount = m_setMember.size();
	int nCount	= 0;
	for(int i = idx; i < nAmount && nCount < MEMBERLIST_SIZE; i++)
	{
		bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);
		if (bOnline)
		{
			char	szMsg[256];
			sprintf(szMsg, "%s %d %d", m_setMember[i].szName.c_str(), GetSynRankShow(m_setMember[i].nRank), bOnline);

			// msg append
			CHECKF(msg.AppendString(szMsg, i));
			nCount++;
		}
	}
	if (nCount < MEMBERLIST_SIZE)
	{
	}

	return nCount;
}
//////////////////////////////////////////////////////////////////////
int CSyndicateWorld::AppendOnlineMemberForAllMember(class CMsgName& msg, int idx)
{
	int	nAmount = m_setMemberTmp.size();
	int nCount	= 0;
	for(int i = idx; i < nAmount && nCount < MEMBERLIST_SIZE; i++)
	{
		bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);
		if (bOnline)
		{
			char	szMsg[256];
			sprintf(szMsg, "%s %d %d", m_setMemberTmp[i].szName.c_str(), GetSynRankShow(m_setMemberTmp[i].nRank), bOnline);

			// msg append
			CHECKF(msg.AppendString(szMsg, i));
			nCount++;
		}
	}

	return nCount;
}
//////////////////////////////////////////////////////////////////////
int CSyndicateWorld::AppendOfflineMember(class CMsgName& msg, int idx, int nCount)
{
	int	nAmount = m_setMember.size();
	for(int i = idx; i < nAmount && nCount < MEMBERLIST_SIZE; i++)
	{
		bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);
		if (!bOnline)
		{
			char	szMsg[256];
			sprintf(szMsg, "%s %d %d", m_setMember[i].szName.c_str(), GetSynRankShow(m_setMember[i].nRank), bOnline);
			// msg append
			CHECKF(msg.AppendString(szMsg, i));
			nCount++;
		}
	}
	return nCount;
}
//////////////////////////////////////////////////////////////////////
int CSyndicateWorld::AppendOfflineMemberForAllMember(class CMsgName& msg, int idx, int nCount)
{
	int	nAmount = m_setMemberTmp.size();
	for(int i = idx; i < nAmount && nCount < MEMBERLIST_SIZE; i++)
	{
		bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);
		if (!bOnline)
		{
			char	szMsg[256];
			sprintf(szMsg, "%s %d %d", m_setMemberTmp[i].szName.c_str(), GetSynRankShow(m_setMemberTmp[i].nRank), bOnline);
			// msg append
			CHECKF(msg.AppendString(szMsg, i));
			nCount++;
		}
	}
	return nCount;
}
*/

//////////////////////////////////////////////////////////////////////
void CSyndicateWorld::SendMemberList(CPlayer* pUser, int idxLast)		// idxLast is last member or -1 for first time query
{
    if (idxLast < 0 )
    {
        return ;
    }
    CMsgName	msg;
    CHECK(msg.CreateHead(NAMEACT_MEMBERLIST, idxLast));
    int nCount = 0;
    AppendMember(msg, 0, idxLast, nCount);	// 先从本帮派中添加
    // 如果不够一页则从子帮派中添加
    if (nCount < MEMBERLIST_SIZE)
    {
        int idx  = GetMemberAmount();
        idxLast -= GetMemberAmount();
        // 分团
        for(int i = 0; i < GameWorld()->QuerySynManager()->GetSynAmount(); i++)
        {
            CSyndicateWorld* pSyn = GameWorld()->QuerySynManager()->GetSynByIndex(i);
            if (pSyn && pSyn->GetInt(SYNDATA_FEALTY) == this->GetID())
            {
                pSyn->AppendMember(msg, idx, idxLast, nCount);
                if (nCount >= MEMBERLIST_SIZE)
                {
                    break;
                }
                idx		+= pSyn->GetMemberAmount();
                idxLast -= pSyn->GetMemberAmount();
                // 分队
                for(int i = 0; i < GameWorld()->QuerySynManager()->GetSynAmount(); i++)
                {
                    CSyndicateWorld* pSynTeam = GameWorld()->QuerySynManager()->GetSynByIndex(i);
                    if (pSynTeam && pSynTeam->GetInt(SYNDATA_FEALTY) == pSyn->GetID())
                    {
                        pSynTeam->AppendMember(msg, idx, idxLast, nCount);
                        if (nCount >= MEMBERLIST_SIZE)
                        {
                            break;
                        }
                        idx		+= pSynTeam->GetMemberAmount();
                        idxLast	-= pSynTeam->GetMemberAmount();
                    }
                }
                if (nCount >= MEMBERLIST_SIZE)
                {
                    break;
                }
            }
        }
    }
    pUser->SendMsg(&msg);
    /*
    	m_setMemberTmp.clear();
    	m_setMemberTmp = m_setMember;
    	MEMBER_SET setMember;

    	// 分团---begin
    	for(int i = 0; i < GameWorld()->QuerySynManager()->GetSynAmount(); i++)
    	{
    		CSyndicateWorld* pSyn = GameWorld()->QuerySynManager()->GetSynByIndex(i);
    		if (pSyn && pSyn->GetInt(SYNDATA_FEALTY) == this->GetID())
    		{
    			setMember = pSyn->QueryMemberList();
    			if (setMember.size())
    				m_setMemberTmp.insert(m_setMemberTmp.end(), setMember.begin(), setMember.end());
    			//分队-----begin
    			for(int j= 0; j < GameWorld()->QuerySynManager()->GetSynAmount(); j++)
    			{
    				CSyndicateWorld* pSynTeam =  GameWorld()->QuerySynManager()->GetSynByIndex(i);
    				if (pSynTeam && pSynTeam->GetInt(SYNDATA_FEALTY) == pSyn->GetID())
    				{
    					setMember = pSynTeam->QueryMemberList();
    					if (setMember.size())
    						m_setMemberTmp.insert(m_setMemberTmp.end(), setMember.begin(), setMember.end());
    				}
    			}
    			//分队-----end
    		}
    	}
    	// 分团---end
    	// msg head
    	CMsgName	msg;
    	CHECK(msg.CreateHead(NAMEACT_MEMBERLIST, idxLast));

    	bool	bOnline = true;		//? -1 for first query
    	int nCount = 0;

    	if (idxLast != -1)
    		bOnline = (UserList()->GetPlayer(m_setMember[idxLast].szName.c_str()) != NULL);

    	int idx	= idxLast + 1;
    	if (bOnline)
    	{
    		nCount	= AppendOnlineMemberForAllMember(msg, idx);
    		if (nCount < MEMBERLIST_SIZE)
    			nCount = AppendOfflineMemberForAllMember(msg, 0, nCount);
    	}
    	else
    	{
    		nCount = AppendOfflineMemberForAllMember(msg, idx);
    	}

    	pUser->SendMsg(&msg);
    */
}


//////////////////////////////////////////////////////////////////////
void CSyndicateWorld::CombineMemberList(const MEMBER_SET& setMember)
{
    if (setMember.size())
    {
        m_setMember.insert(m_setMember.end(), setMember.begin(), setMember.end());
    }
}

//////////////////////////////////////////////////////////////////////
CSyndicateWorld* CSyndicateWorld::GetMasterSyn()
{
    if (GetInt(SYNDATA_FEALTY) == ID_NONE)
    {
        return this;
    }
    CSyndicateWorld* pSyn = GameWorld()->QuerySynManager()->QuerySyndicate(GetInt(SYNDATA_FEALTY));
    IF_NOT (pSyn)
    return this;
    return pSyn->GetMasterSyn();
}

//////////////////////////////////////////////////////////////////////
/*
void CSyndicateWorld::SendMemberListSpecifySyn(CPlayer *pUser, int idxLast)
{
	if (idxLast < -1 || idxLast >= static_cast<int>(m_setMember.size()))
		return ;

	// msg head
	CMsgName	msg;
	CHECK(msg.CreateHead(NAMEACT_MEMBERLIST, idxLast));

	bool	bOnline = true;		//? -1 for first query
	int nCount = 0;

	if (idxLast != -1)
		bOnline = (UserList()->GetPlayer(m_setMember[idxLast].szName.c_str()) != NULL);

	int idx	= idxLast + 1;
	if (bOnline)
	{
		nCount	= AppendOnlineMember(msg, idx);
		if (nCount < MEMBERLIST_SIZE)
			nCount = AppendOfflineMember(msg, 0, nCount);
	}
	else
	{
		nCount = AppendOfflineMember(msg, idx);
	}

	pUser->SendMsg(&msg);
}
*/

int CSyndicateWorld::AppendMember(class CMsgName& msg, int idx, int idxFirst, int& nCount)
{
    int nAmount = GetMemberAmount();
    for (int i = idxFirst; i < nAmount && nCount < MEMBERLIST_SIZE; i++)
    {
        bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);
        char	szMsg[256];
        sprintf(szMsg, "%s %d %d", m_setMember[i].szName.c_str(), GetSynRankShow(m_setMember[i].nRank), bOnline);
        CHECKF(msg.AppendString(szMsg, idx + i));
        nCount++;
    }
    return nCount;
}
