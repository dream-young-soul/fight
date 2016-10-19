
//**********************************************************
// 代码编辑器
//**********************************************************

// UserManager.cpp: implementation of the CUserManager class.
//
//////////////////////////////////////////////////////////////////////

#include "MessagePort.h"
#pragma warning(disable:4786)
#include "AllMsg.h"
#include "MapGroup.h"
#pragma warning(disable:4786)
#include <algorithm>
#include "netmsg.h"
#include "protocol.h"
#include "PoliceWanted.h"
#include "Agent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
MYHEAP_IMPLEMENTATION(CUserManager, s_heap)


//////////////////////////////////////////////////////////////////////
bool CUserManager::Create()
{
    m_pTeamSet		= CTeamSet::CreateNew(true);
    m_idLastTeam	= m_idProcess;					//? 保证每个进程组的TEAM_ID不冲突。
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUserManager::Release()
{
    LogoutAllUser();
    if (m_pTeamSet)
    {
        m_pTeamSet->Release();
    }
    delete this;
}

//////////////////////////////////////////////////////////////////////
void CUserManager::OnTimer(time_t tCurr)
{
}

//////////////////////////////////////////////////////////////////////
CUser* CUserManager::CreateUser(SOCKET_ID idSocket, OBJID idUser)		// login第一步
{
    if (idSocket == SOCKET_NONE || idUser == ID_NONE)
    {
        ASSERT(!"CUserManager::LoginUser");
        return NULL;
    }
    if (idSocket < m_setUser.size() && m_setUser[idSocket])
    {
        ASSERT(!"CUserManager::LoginUser 重复SOCKET_ID");
        DEBUG_TRY
        LogoutUser(idSocket);				//@ 同一SOCKET重复登录，前一个强行退出。
        DEBUG_CATCH("LogoutUser(idSocket)")	//@
        m_setUser[idSocket]	= NULL;
    }
    CUserPtr pOldUser = GetUser(idUser);
    if (pOldUser)
    {
        SOCKET_ID idOldSocket = pOldUser->GetSocketID();			// 前一玩家的SOCKET_ID
        DEBUG_TRY
        LOGMSG("DEBUG：踢出重复登录的USER_ID[%u], SOCKET_ID[%d]", idUser, idOldSocket);
        KickOutSocket(idOldSocket, "重复登录");				//@ 同一ID重复登录，前一个强行退出。
        DEBUG_CATCH("LogoutUser(id)")	//@
        return NULL;		// 不能继续
    }
    CUser* pUser = CUser::CreateNew();
    if (!pUser)
    {
        return NULL;
    }
    if (!pUser->Create(m_idProcess, idSocket, idUser))
    {
        pUser->ReleaseByOwner();
        return NULL;
    }
    return pUser;
}

//////////////////////////////////////////////////////////////////////
CAgent* CUserManager::CreateAgent(OBJID idAgent)
{
    if (idAgent == ID_NONE)
    {
        ASSERT(!"CUserManager::LoginAgent");
        return NULL;
    }
    CAgent* pAgent = CAgent::CreateNew();
    if (!pAgent)
    {
        return NULL;
    }
    if (!pAgent->Create(m_idProcess, idAgent))
    {
        pAgent->ReleaseByOwner();
        return NULL;
    }
    return pAgent;
}

//////////////////////////////////////////////////////////////////////
bool CUserManager::LoginUser(SOCKET_ID idSocket, CUser* pUser)		// login第二步
{
    CHECKF(pUser);
    if (idSocket >= m_setUser.size())
    {
        m_setUser.resize(idSocket + 1, NULL);    // 保证idSocket索引有效
    }
    m_setUser[idSocket]	= pUser;
    RoleManager()->QuerySet()->AddObj(pUser->QueryRole());
    CMsgUserInfo	msg;
    if (msg.Create(pUser))
    {
        MapGroup(PID)->QueryIntraMsg()->SendClientMsg(idSocket, &msg);
    }
    // 同步EFFECT
    {
        CMsgUserAttrib	msg;
        if (msg.Create(pUser->GetID(), _USERATTRIB_KEEPEFFECT, pUser->GetEffect()))
        {
            MapGroup(PID)->QueryIntraMsg()->SendClientMsg(idSocket, &msg);
        }
    }
    // police wanted
    if (pUser->GetPk() >= PKVALUE_BADGUY)
    {
        if (MAP_PRISON != pUser->GetRecordMapID())
        {
            PoliceWanted().AddWanted(pUser);
        }
    }
    // mate inform
    if (pUser->IsMarried())
    {
        CUser* pMate = UserManager()->GetUser(pUser->GetMate());
        if (pMate)
        {
            const char* pszMsg = MALE_MATE_LOGIN;
            if (pMate->GetSex() == SEX_MAN)
            {
                pszMsg = FEMALE_MATE_LOGIN;
            }
            pMate->SendSysMsg(_TXTATR_NORMAL, pszMsg);
        }
    }
    // tutor&student notify
    pUser->NotifyTutorStatus(MEMBERSTATUS_ONLINE);
    // max user record
    DWORD dwUserAmount = this->GetUserAmount();
    if (dwUserAmount >= m_dwMaxUser)
    {
        m_dwMaxUser = dwUserAmount;
    }
    const int ACTION_LOGINPROMPT = 1000000;
    GameAction()->ProcessAction(ACTION_LOGINPROMPT, pUser);
    // gm ?
    if (pUser->IsGM())
    {
        m_setGm.push_back(pUser->GetID());
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUserManager::LoginAgent(CAgent* pAgent)
{
    CHECKF(pAgent);
    RoleManager()->QuerySet()->AddObj(pAgent->QueryRole());
    CMsgUserInfo	msg;
    if (msg.Create(pAgent))
    {
        pAgent->SendMsg(&msg);
    }
    // 同步EFFECT
    {
        CMsgUserAttrib	msg;
        if (msg.Create(pAgent->GetID(), _USERATTRIB_KEEPEFFECT, pAgent->GetEffect()))
        {
            pAgent->SendMsg(&msg);
        }
    }
    // police wanted
    if (pAgent->GetPk() >= PKVALUE_BADGUY)
    {
        if (MAP_PRISON != pAgent->GetRecordMapID())
        {
            PoliceWanted().AddWanted(pAgent);
        }
    }
    // mate inform
    if (pAgent->IsMarried())
    {
        CUser* pMate = UserManager()->GetUser(pAgent->GetMate());
        if (pMate)
        {
            const char* pszMsg = MALE_MATE_LOGIN;
            if (pMate->GetSex() == SEX_MAN)
            {
                pszMsg = FEMALE_MATE_LOGIN;
            }
            pMate->SendSysMsg(_TXTATR_NORMAL, pszMsg);
        }
    }
    //	const ACTION_LOGINPROMPT = 1000000;
    //	GameAction()->ProcessAction(ACTION_LOGINPROMPT, pAgent);
    return true;
}

//////////////////////////////////////////////////////////////////////     change huang 2004.1.5
bool CUserManager::LogoutUser(SOCKET_ID idSocket, BOOL bOffline/*=true*/)
{
    if (idSocket >= m_setUser.size() || !m_setUser[idSocket])
    {
        return false;
    }
    CUserPtr pUser = GetUserBySocketID(idSocket);
    if (pUser)
    {
        DEBUG_TRY	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
        // save info
        pUser->SaveInfo();		// SaveAllInfo
        bool bCheck = false;
        if (bCheck)
        {
            if (bOffline)            //add huang 2004.1.7
            {
                CSyndicate* pSyn = pUser->GetSyndicate();
                if (pSyn)
                {
                    pSyn = pSyn->GetMasterSyn();
                    if (pSyn->AutoDismissSyn())
                    {
                    }
                }
            }
        }
        if (bOffline)
        {
            // save record point
            if (pUser->IsAlive())
            {
                if (pUser->GetMap()->IsStandEnable(pUser->GetPosX(), pUser->GetPosY()) && !pUser->GetMap()->IsRecordDisable())
                {
                    pUser->SetRecordPos(pUser->GetMapID(), pUser->GetPosX(), pUser->GetPosY());
                }
            }
            // 通过WORLD核心查其它在线玩家
            for(int i = 0; i < pUser->GetFriendAmount(); i++)
            {
                CFriend* pFriend = pUser->GetFriendByIndex(i);
                if (pFriend)
                {
                    OBJID	idFriend	= pFriend->GetFriendID();
                    CMsgFriend	msg;
                    IF_OK(msg.Create(_FRIEND_OFFLINE, pUser->GetID(), pUser->GetName()))
                    MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, idFriend);
                }
            }
            CEnemy* pEnemy = pUser->QueryEnemy();
            if (pEnemy) { pEnemy->BeforeLogout(); }
            //pUser->QueryEnemy()->OffLine();
            // tutor&student notify
            pUser->NotifyTutorStatus(MEMBERSTATUS_OFFLINE);
            //			pUser->NotifyChatRoomOffline();//通知聊天室，用户离线
            // police wanted
            PoliceWanted().DelWanted(pUser->GetID());
            // gm?
            if (pUser->IsGM())
            {
                vector<OBJID>::iterator iter;
                for (iter = m_setGm.begin(); iter != m_setGm.end(); iter++)
                {
                    if (pUser->GetID() == *iter)
                    {
                        m_setGm.erase(iter);
                        break;
                    }
                }
            }
        }
        DEBUG_CATCH2("logout user [%d]", idSocket)
        // and so on...
        m_setUser[idSocket] = NULL;
        RoleManager()->QuerySet()->DelObj(pUser->GetID());
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUserManager::LogoutAgent(CUser* pUser, BOOL bOffline/*=true*/)
{
    ASSERT(pUser && pUser->IsAgent());
    {
        DEBUG_TRY	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
        // save info
        pUser->SaveInfo();		// SaveAllInfo
        if (bOffline)
        {
            // 通过WORLD核心查其它在线玩家
            for(int i = 0; i < pUser->GetFriendAmount(); i++)
            {
                CFriend* pFriend = pUser->GetFriendByIndex(i);
                if (pFriend)
                {
                    OBJID	idFriend	= pFriend->GetFriendID();
                    CMsgFriend	msg;
                    IF_OK(msg.Create(_FRIEND_OFFLINE, pUser->GetID(), pUser->GetName()))
                    MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, idFriend);
                }
            }
            //			pUser->QueryEnemy()->OffLine();
            // police wanted
            PoliceWanted().DelWanted(pUser->GetID());
        }
        DEBUG_CATCH("logout agent")
        // and so on...
        RoleManager()->QuerySet()->DelObj(pUser->GetID());
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUserManager::KickOutSocket(SOCKET_ID idSocket, LPCTSTR szReason/*=NULL*/)
{
    CHECKF(idSocket >= 0 && idSocket < m_setUser.size());
    CUserPtr pUser = GetUserBySocketID(idSocket);
    if (pUser)
    {
        pUser->SaveInfo();		// SaveAllInfo
        if (szReason)
        {
            LOGWARNING("KickOutSocket()踢出SOCKET_ID[%d], 玩家[%s]。理由：%s", idSocket, pUser->GetName(), szReason);
        }
    }
    else
    {
        if (szReason)
        {
            LOGWARNING("KickOutSocket()踢出SOCKET_ID[%d]。理由：%s", idSocket, szReason);
        }
    }
    MapGroup(PID)->QueryIntraMsg()->CloseSocket(idSocket);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUserManager::LogoutAllUser()
{
    CUserManager::Iterator pUser = UserManager()->NewEnum();
    while(pUser.Next())
    {
        if (!pUser->IsAgent())
        {
            LogoutUser(pUser->GetSocketID());
        }
        else
        {
            LogoutAgent(pUser);
        }
    }
}

//////////////////////////////////////////////////////////////////////
CUser* CUserManager::GetUser(OBJID idUser)
{
    if (idUser == ID_NONE)
    {
        return NULL;
    }
    IRole* pRole = RoleManager()->QueryRole(idUser);
    CUser* pUser;
    if (pRole && pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        return pUser;
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CUser* CUserManager::GetUser(LPCTSTR szName)
{
    ASSERT(szName);
    CUserManager::Iterator pUser = this->NewEnum();
    while(pUser.Next())
    {
        if (strcmp(pUser->GetName(), szName) == 0)
        {
            return pUser;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CUser* CUserManager::GetUser(CNetMsg* pMsg)			// for agent
{
    ASSERT(pMsg);
    if (pMsg->IsNpcMsg())
    {
        IRole* pRole = RoleManager()->QuerySet()->GetObj(pMsg->GetNpcID());
        if (!pRole)
        {
            return NULL;
        }
        CUser* pUser = NULL;
        pRole->QueryObj(OBJ_USER, IPP_OF(pUser));
        return pUser;
    }
    else
    {
        return GetUserBySocketID(pMsg->GetSocketID());
    }
}

//////////////////////////////////////////////////////////////////////
bool CUserManager::FindNextSynMember(OBJID idSyn, int& nIter)
{
    CHECKF(idSyn != ID_NONE);
    OBJID	idLastUser	= nIter;
    CUser*	pTarget		= NULL;
    bool	bFindNext	= true;
    CUserManager::Iterator pUser = UserManager()->NewEnum();
    while(pUser.Next())
    {
        if (pUser && pUser->GetSynID() == idSyn)
        {
            if (bFindNext)
            {
                pTarget		= pUser;
                bFindNext	= false;
            }
            if (pUser->GetID() == idLastUser)
            {
                pTarget		= NULL;
                bFindNext	= true;
                continue;
            }
        }
    }
    if (pTarget)
    {
        nIter = pTarget->GetID();
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CUserManager::BroadcastMsg(CNetMsg* pMsg, CUser* pExcludeSender/*=NULL*/)
{
    CHECK(pMsg);
    BroadcastMapGroupMsg(pMsg, pExcludeSender);
    MapGroup(PID)->QueryIntraMsg()->SendBroadcastMsg(pMsg);
}

//////////////////////////////////////////////////////////////////////
void CUserManager::BroadcastMsg(LPCTSTR szMsg, CUser* pExcludeSender/*=NULL*/, LPCTSTR pszEmotion, DWORD dwColor, int nAttribute)
{
    CHECK(szMsg);
    CMsgTalk	msg;
    if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, pszEmotion, dwColor, nAttribute))
    {
        BroadcastMsg(&msg, pExcludeSender);
    }
}

//////////////////////////////////////////////////////////////////////
void CUserManager::BroadcastMapMsg(OBJID idMap, CNetMsg* pMsg, CUser* pExcludeSender)
{
    ASSERT(pMsg);
    bool	bSendAiServer = true;
    CUserManager::Iterator pUser = UserManager()->NewEnum();
    while(pUser.Next())
    {
        if (!pUser->IsAgent())
        {
            if (pUser->GetMap()->GetID() == idMap && (!pExcludeSender || pExcludeSender->GetID() != pUser->GetID()))
            {
                MapGroup(PID)->QueryIntraMsg()->SendClientMsg(pUser->GetSocketID(), pMsg);
            }
        }
        else if (bSendAiServer)
        {
            if (pUser->GetMap()->GetID() == idMap && (!pExcludeSender || pExcludeSender->GetID() != pUser->GetID()))
            {
                MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(pUser->GetID(), pMsg);
                bSendAiServer = false;	// only one npc msg
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
// for mapgroup
//////////////////////////////////////////////////////////////////////
void CUserManager::BroadcastMapGroupMsg(CNetMsg* pMsg, CUser* pExcludeSender)
{
    ASSERT(pMsg);
    bool	bSendAiServer = true;
    CUserManager::Iterator pUser = UserManager()->NewEnum();
    while(pUser.Next())
    {
        if (!pUser->IsAgent())
        {
            if (!pExcludeSender || pExcludeSender->GetID() != pUser->GetID())
            {
                MapGroup(PID)->QueryIntraMsg()->SendClientMsg(pUser->GetSocketID(), pMsg);
            }
        }
        else if (bSendAiServer)
        {
            if (!pExcludeSender || pExcludeSender->GetSocketID() != pUser->GetID())
            {
                MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(pUser->GetID(), pMsg);
                bSendAiServer = false;	// only one npc msg
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CUserManager::JoinMapGroup(SOCKET_ID idSocket, UserInfoStruct* pInfo)
{
    CHECK(idSocket != SOCKET_NONE && pInfo);
    if (idSocket >= m_setUser.size())
    {
        m_setUser.resize(idSocket + 1, NULL);    // 保证idSocket索引有效
    }
    CHECK(m_setUser[idSocket] == NULL);
    CUser* pUser = CUser::CreateNewUser(m_idProcess, idSocket, pInfo);
    if (!pUser)
    {
        return;
    }
    ASSERT(!m_setUser[idSocket]);
    m_setUser[idSocket]	= pUser;
    RoleManager()->QuerySet()->AddObj(pUser->QueryRole());
}

//////////////////////////////////////////////////////////////////////
DWORD CUserManager::GetUserAmount()
{
    DWORD dwUsers = 0;
    for(int i = 0; i < m_setUser.size(); i++)
    {
        if (m_setUser[i])
        {
            dwUsers++;
        }
    }
    return dwUsers;
}

//////////////////////////////////////////////////////////////////////
DWORD CUserManager::GetMapUserAmount(OBJID idMap)
{
    DWORD dwUsers = 0;
    for(int i = 0; i < m_setUser.size(); i++)
    {
        CUser* pUser = m_setUser[i];
        if (pUser && pUser->GetMapID() == idMap)
        {
            dwUsers++;
        }
    }
    return dwUsers;
}

//////////////////////////////////////////////////////////////////////
int CUserManager::GetTeamAmount()
{
    CHECKF(m_pTeamSet);
    return m_pTeamSet->GetAmount();
}

//////////////////////////////////////////////////////////////////////
CTeam* CUserManager::GetTeamByIndex(int nIndex)
{
    CHECKF(m_pTeamSet);
    CTeam* pTeam	= m_pTeamSet->GetObjByIndex(nIndex);
    return pTeam;
}

//////////////////////////////////////////////////////////////////////
CTeam* CUserManager::GetTeamByLeaderId(OBJID idLeader)
{
    for (int i = 0; i < this->GetTeamAmount(); i++)
    {
        CTeam* pTeam	= this->GetTeamByIndex(i);
        if (pTeam && pTeam->GetLeader() == idLeader)
        {
            return pTeam;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
BOOL CUserManager::IsValidTeam(CTeam* pTeam)
{
    if (!pTeam)
    {
        return false;
    }
    for (int i = 0; i < this->GetTeamAmount(); i++)
    {
        CTeam* pNowTeam	= this->GetTeamByIndex(i);
        if (pNowTeam && pNowTeam->GetLeader() == pTeam->GetLeader() &&
                pNowTeam->GetMemberAmount() == pTeam->GetMemberAmount())
        {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
BOOL CUserManager::AddTeam(CTeam* pTeam)
{
    if (!pTeam)
    {
        return false;
    }
    m_pTeamSet->AddObj(pTeam);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUserManager::DelTeam(CTeam* pTeam)
{
    if (!pTeam)
    {
        return;
    }
    for (int i = 0; i < this->GetTeamAmount(); i++)
    {
        CTeam* pNowTeam	= this->GetTeamByIndex(i);
        if (pNowTeam && pNowTeam->GetID() == pTeam->GetID())
        {
            m_pTeamSet->DelObj(pTeam->GetID());
            return;
        }
    }
}

//////////////////////////////////////////////////////////////////////
OBJID CUserManager::SpawnNewTeamID()
{
    m_idLastTeam += MapGroup(PID)->GetMapGroupAmount();					//? 保证每个进程组的TEAM_ID不冲突。
    ASSERT(m_idLastTeam < 4012345678);
    return m_idLastTeam;
}

CUserManager::Iterator CUserManager::NewEnum()						{ return Iterator(RoleManager()->QuerySet()); }

//////////////////////////////////////////////////////////////////////
bool CUserManager::AddCheat(OBJID idUser, int* pnAmount, int* pnTimes)
{
    CHECKF(pnAmount && pnTimes);
    int	nAmount = m_setCheatIDTimes.Size();
    int	nTimes = 1;
    if (m_setCheatIDTimes.IsIn(idUser))
    {
        nTimes = m_setCheatIDTimes.At(idUser) + 1;
    }
    else
    {
        nAmount++;
    }
    m_setCheatIDTimes.Add(idUser, nTimes);
    *pnAmount	= nAmount;
    *pnTimes	= nTimes;
    return true;
}
