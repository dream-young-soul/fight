
//**********************************************************
// 代码编辑器
//**********************************************************

//------------------------------------------------------------------
// HeroSyndicate.cpp
//------------------------------------------------------------------

#include "Hero.h"
#include "AllMsg.h"
#include "GameMsg.h"
#include "GamePlayerSet.h"
#include "GameDataSet.h"

//------------------------------------------------------------------
void CHero::SetLastSyndicateApplyID(OBJID idApply)
{
    m_idLastSyndicateApply = idApply;
}
//------------------------------------------------------------------
void CHero::SyndicateApplyJoin(OBJID idTarget)
{
    if (this->GetSydicateID() != ID_NONE)
    {
        OBJID idLeaveSyndicate = 100065;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idLeaveSyndicate));
        return;
    }
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idTarget);
    {
        if (!pPlayer)
        {
            OBJID idErrorTarget = 100066;
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idErrorTarget));
            return;
        }
    }
    if (pPlayer->GetSydicateID() == ID_NONE)
    {
        char szMsg[128];
        const OBJID idCanNotJion = 100067;
        sprintf(szMsg, g_objGameDataSet.GetStr(idCanNotJion), pPlayer->GetName());
        g_objGameMsg.AddMsg(szMsg);
        return;
    }
    /*	if (pPlayer->GetSyndicateRank() != RANK_LEADER)
    	{
    		char szMsg[128];
    		const OBJID idCanNotRecruit = 100068;
    		sprintf(szMsg, g_objGameDataSet.GetStr(idCanNotRecruit), pPlayer->GetName());
    		g_objGameMsg.AddMsg(szMsg);
    		return;
    	}*/
    CMsgSyndicate msg;
    if (msg.Create(APPLY_JOIN, idTarget))
    {
        msg.Send();
        OBJID idApplicationSend = 100069;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idApplicationSend));
    }
}
//------------------------------------------------------------------
void CHero::SynDicateAcceptJoin()
{
    CMsgSyndicate msg;
    if (msg.Create(INVITE_JOIN, m_idLastSyndicateApply))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
void CHero::SyndicateApplyInvite(OBJID idTarget)
{
    if (this->GetSydicateID() == ID_NONE)
    {
        OBJID idPleaseJoinSyndicateFirst = 100070;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idPleaseJoinSyndicateFirst));
        return;
    }
    /*	if (this->GetSyndicateRank() != RANK_LEADER)
    	{
    		OBJID idCanNotRecurit = 100071;
    		g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idCanNotRecurit));
    		return;
    	}*/
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idTarget);
    {
        if (!pPlayer)
        {
            OBJID idErrorTarget = 100072;
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idErrorTarget));
            return;
        }
    }
    if (pPlayer->GetSydicateID() != ID_NONE)
    {
        OBJID idTargetHaveSyndicate = 100073;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTargetHaveSyndicate));
        return;
    }
    CMsgSyndicate msg;
    if (msg.Create(INVITE_JOIN, idTarget))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
void CHero::SyndicateAcceptInvite()
{
    CMsgSyndicate msg;
    if (msg.Create(APPLY_JOIN, m_idLastSyndicateApply))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
void CHero::SetLastSyndicateID(OBJID idSynDicate)
{
    m_idLastSyndicate = idSynDicate;
}
//------------------------------------------------------------------
OBJID CHero::GetLastSyndicateID()
{
    return m_idLastSyndicate;
}
//------------------------------------------------------------------
void CHero::LeaveSyndicate()
{
    if (this->GetSydicateID() == ID_NONE)
    {
        return;
    }
    CONST int MINPROFFER_QUITSYN = 200; //贡献度大于200才能退出
    if (this->GetSyndicateProffer() < MINPROFFER_QUITSYN && 	this->GetSyndicateRank() > 10)  //检查贡献度 -- 预备团员不用检查
    {
        return;
    }
    CMsgSyndicate msg;
    if (msg.Create(LEAVE_SYN, g_objHero.GetID()))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
OBJID CHero::IsSubSynLeader()
{
    if (this->GetSydicateID() != ID_NONE)
    {
        int nAmount = g_objPlayerSet.GetSubSyndicateAmount(this->GetSydicateID());
        if (nAmount == 0)
        {
            return 0;
        }
        else
        {
            for(int i = 0; i < nAmount; i++)
            {
                OBJID idSubSyn = g_objPlayerSet.GetSubSyndicateId(this->GetSydicateID(), i);
                if (strcmp(g_objPlayerSet.GetSyndicateLeaderName(idSubSyn), this->GetName()) == 0 )
                {
                    return idSubSyn;
                }
            }
        }
    }
    return 0;
}
//------------------------------------------------------------------
OBJID CHero::IsSubSubSynLeader()
{
    if (this->GetSydicateID() != ID_NONE)
    {
        int nAmount = g_objPlayerSet.GetSubSyndicateAmount(this->GetSydicateID());
        if (nAmount == 0)
        {
            return 0;
        }
        else
        {
            for(int i = 0; i < nAmount; i++)
            {
                OBJID idSubSyn = g_objPlayerSet.GetSubSyndicateId(this->GetSydicateID(), i);
                int nSubAmount = g_objPlayerSet.GetSubSyndicateAmount(idSubSyn);
                for(int j = 0; j < nSubAmount; j++)
                {
                    OBJID idSubSubSyn = g_objPlayerSet.GetSubSyndicateId(idSubSyn, j);
                    if (strcmp(g_objPlayerSet.GetSyndicateLeaderName(idSubSubSyn), this->GetName()) == 0 )
                    {
                        return idSubSubSyn;
                    }
                }
            }
        }
    }
    return 0;
}
//------------------------------------------------------------------
void CHero::SynContribute(DWORD dwMoney)
{
    if (this->GetSydicateID() == ID_NONE)
    {
        OBJID idHaveNoSynDicate = 100074;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idHaveNoSynDicate));
        return;
    }
    if (this->GetMoney() < dwMoney)
    {
        OBJID idMoneyNotEnough = 100075;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMoneyNotEnough));
        return;
    }
    CMsgSyndicate msg;
    if (msg.Create(DONATE_MONEY, dwMoney))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
void CHero::SetSyndicateAnnounce(char* pszWord)
{
    if (pszWord)
    {
        strcpy(m_szSyndicateAnnounce, pszWord);
        ::PostCmd(CMD_FLASH_SYNDICATE_WORD);
    }
}
//------------------------------------------------------------------
char* CHero::GetSyndicateAnnounce()
{
    return m_szSyndicateAnnounce;
}
//------------------------------------------------------------------
void CHero::SetSyndicateAnnounceTime(DWORD dwTime)
{
    if (dwTime)
    {
        int nYear = dwTime / 10000;
        int nMonth = ( dwTime / 100 ) % 100;
        int nDay = dwTime % 100 ;
        sprintf( m_szSyndicateAnnounceTime, "%d-%d-%d", nYear, nMonth, nDay) ;
    }
}
//------------------------------------------------------------------
char* CHero::GetSyndicateAnnounceTime()
{
    return m_szSyndicateAnnounceTime;
}
//------------------------------------------------------------------
void CHero::QuerySynicateInfo()
{
    CMsgSyndicate msg;
    if (msg.Create(QUERY_SYNATTR, this->GetID()))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
void CHero::QueryOtherSynicateInfo(char* szSynName)
{
    if (strlen(szSynName) <= 0)
    {
        return;
    }
    else
    {
        OBJID idSyn  = g_objPlayerSet.GetSyndicateIdByName(szSynName);
        if (idSyn)
        {
            CMsgAction msg;
            if (msg.Create(this->GetID(), 0, 0, 0, actionQuerySynInfo, idSyn))
            {
                msg.Send();
            }
        }
    }
}
//------------------------------------------------------------------
int		CHero::GetSynWarReportAmount ()
{
    return m_dequeSynWarReport.size();
}
//------------------------------------------------------------------
char*	CHero::GetSynWarReportLine(int nIndex)
{
    int nAmount = m_dequeSynWarReport.size();
    if (nIndex >= nAmount && nIndex < 0)
    {
        return NULL;
    }
    return m_dequeSynWarReport[nIndex];
}
//------------------------------------------------------------------
void	CHero::ClearSynWarReport()
{
    int nAmount = m_dequeSynWarReport.size();
    for(int i = 0; i < nAmount; i ++)
    {
        char* pszChar = m_dequeSynWarReport[i];
        delete[] pszChar;
    }
    m_dequeSynWarReport.clear();
}
//------------------------------------------------------------------
void	CHero::AddSynWarReport(char* pszLine)
{
    if (!pszLine)
    {
        return;
    }
    if (strlen(pszLine) >= 512)
    {
        return;
    }
    char* pszTemp = new char[512];
    strcpy(pszTemp, pszLine);
    m_dequeSynWarReport.push_back(pszTemp);
    m_dwTimeSynWarReportLastFlash = ::TimeGet();
}
//------------------------------------------------------------------
void	CHero::ShowSynWarReport()
{
    const DWORD dwDelay = 60 * 1000;
    if (::TimeGet() - m_dwTimeSynWarReportLastFlash > dwDelay)
    {
        return;
    }
    CMyPos posShow = {_SCR_WIDTH - 300, 5};
    CMySize sizeFont;
    CMyBitmap::GetFontSize(sizeFont);
    int nAmount = this->GetSynWarReportAmount();
    for(int i = 0; i < nAmount; i ++)
    {
        char* pszLine = this->GetSynWarReportLine(i);
        if (pszLine)
        {
            posShow.y += sizeFont.iHeight + 5;
            CMyBitmap::ShowString(posShow.x, posShow.y, _COLOR_YELLOW, pszLine);
        }
    }
}
//------------------------------------------------------------------
void	CHero::SendSyndicateAnnounce(char* pszStr)
{
    if (!pszStr)
    {
        return;
    }
    CMsgTalk msg;
    if (msg.Create((char*)g_objHero.GetName(), "大家", pszStr, NULL, 0xffffff, _TXTATR_SYNANNOUNCEG, _TXT_NORMAL))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
void	CHero::QuerySynMemberList(int nIndex)
{
    if (nIndex < 0)
    {
        nIndex = 0;
    }
    CMsgName msg;
    if (msg.Create(NAMEACT_MEMBERLIST, NULL, nIndex))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
int CHero::GetSynMemberAmount()
{
    return m_setSyndicateMember.size();
}
//------------------------------------------------------------------
SynMemberInfo* CHero::GetSynMemberByIndex(int nIndex)
{
    int nAmount = m_setSyndicateMember.size();
    if (nIndex < 0 || nIndex >= nAmount)
    {
        return NULL;
    }
    return m_setSyndicateMember[nIndex];
}
//------------------------------------------------------------------
void CHero::ClearSynMember()
{
    int nAmount = m_setSyndicateMember.size();
    for(int i = 0; i < nAmount; i ++)
    {
        SynMemberInfo* pInfo = m_setSyndicateMember[i];
        SAFE_DELETE(pInfo);
    }
    m_setSyndicateMember.clear();
}
//------------------------------------------------------------------
void CHero::AddSynMember(char* pszMember)
{
    if (!pszMember)
    {
        return;
    }
    char szTemp[256];
    SynMemberInfo* pInfo = new SynMemberInfo;
    MYASSERT(pInfo);
    sscanf(pszMember, "%s %d %d", szTemp, &pInfo->ucRank , &pInfo->bOnLine);	//6 - 2 XL CODING
    if (strlen(szTemp) < _MAX_NAMESIZE)
    {
        strcpy(pInfo->szName, szTemp);
    }
    else
    {
        strcpy(pInfo->szName, "overflow");
    }
    int i;
    for(i = 0; i < RANKAMOUNT; i++)
    {
        if (SYNDICATERANKINFOSET[i].dwRank == pInfo->ucRank)
        {
            strcpy(pInfo->szRank, g_objGameDataSet.GetStr(SYNDICATERANKINFOSET[i].idTitle));
        }
    }
    strcpy(pInfo->szMate, "...");
    pInfo->ucLevel = 0;
    pInfo->nProffer = 0;
    pInfo->sPk = 0;
    pInfo->ucNobilityRank = 0;
    pInfo->ucProfession = 0;
    m_setSyndicateMember.push_back(pInfo);
}
//------------------------------------------------------------------
void CHero::KickoutSynMember(char* pszTargetName)
{
    if (!pszTargetName)
    {
        return;
    }
    CMsgSyndicate msg;
    if (msg.Create(KICKOUT_MEMBER, pszTargetName))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
void CHero::SetSynLeader(int nAction, char* pszTargetName)
{
    if (!pszTargetName)
    {
        return;
    }
    CMsgSyndicate msg;
    unsigned char ucAction;
    switch(nAction)
    {
    case 1:
        ucAction = SYN_DEMISE;
        break;
    case 2:
        ucAction = SYN_SET_ASSISTANT;
        break;
    case 3:
        ucAction = SYN_SET_TEAMLEADER;
        break;
    default:
        break;
    }
    if (msg.Create(ucAction, pszTargetName))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
void CHero::UniteSubSyn(OBJID idSource, OBJID idTarget)
{
    // 先处理Hero的数据...
    if (g_objHero.GetSydicateID() == idSource)
    {
        g_objHero.SetSyndicateID(idTarget);
        g_objHero.SetSyndicateRank(RANK_NEWBIE);
    }
    // 处理Player的数据...
    int nAmount = g_objPlayerSet.GetPlayerAmount();
    for(int i = 0; i < nAmount; i ++)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayerByIndex(i);
        if (pPlayer && pPlayer->GetSydicateID() == idSource)
        {
            pPlayer->SetSyndicateID(idTarget);
        }
    }
    // update syndicate name ...
    g_objPlayerSet.UpdateSyndicateName(idTarget);
}
//------------------------------------------------------------------
void CHero::UniteSyn(OBJID idSource, OBJID idTarget)
{
    // to do...
}
//------------------------------------------------------------------
void CHero::QuerySynMember(char* pszName)
{
    if (!pszName)
    {
        return;
    }
    strcpy(m_objSynMemberInfo.szName, pszName);
    strcpy(m_objSynMemberInfo.szRank, "...");
    strcpy(m_objSynMemberInfo.szMate, "...");
    m_objSynMemberInfo.ucLevel = 0;
    m_objSynMemberInfo.nProffer = 0;
    m_objSynMemberInfo.sPk = 0;
    m_objSynMemberInfo.ucNobilityRank = 0;
    m_objSynMemberInfo.ucProfession = 0;
    CMsgSynMemberInfo msg;
    if (msg.Create(pszName))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
void CHero::AddAllySyn(OBJID idTarget)
{
    if (idTarget == 0)
    {
        return;
    }
    CMsgSyndicate msg;
    if (msg.Create(APPLY_ALLY, idTarget))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
void CHero::ClearAllySyn(char* pszName)
{
    if (!pszName)
    {
        return;
    }
    OBJID idSyn = g_objPlayerSet.GetSyndicateIdByName(pszName);
    if (idSyn != ID_NONE)
    {
        CMsgSyndicate msg;
        if (msg.Create(_CLEAR_ALLY, pszName))
        {
            msg.Send();
        }
    }
}
//------------------------------------------------------------------
void CHero::AddAntagonizeSyn(char* pszName)
{
    if (!pszName)
    {
        return;
    }
    CMsgSyndicate msg;
    if (msg.Create(_SET_ANTAGONIZE, pszName))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
void CHero::ClearAntagonizeSyn(char* pszName)
{
    if (!pszName)
    {
        return;
    }
    OBJID idSyn = g_objPlayerSet.GetSyndicateIdByName(pszName);
    if (idSyn != ID_NONE)
    {
        CMsgSyndicate msg;
        if (msg.Create(_CLEAR_ANTAGONIZE, pszName))
        {
            msg.Send();
        }
    }
}
//------------------------------------------------------------------
void CHero::RenameSynName(char* szNowName)
{
}
//------------------------------------------------------------------
void CHero::QuerySpecifSynMember(OBJID idSyn, int nIndex)
{
    if (idSyn == ID_NONE)
    {
        return;
    }
    else
    {
        CMsgName msg;
        if (nIndex < 0)
        {
            nIndex = 0;
        }
        else { nIndex = m_nSynListIndex; }
        if (msg.Create(NAMEACT_MEMBERLIST_SPECIFYSYN, g_objPlayerSet.GetSyndicateName(idSyn) , nIndex))
        {
            msg.Send();
        }
    }
}
//------------------------------------------------------------------
int  CHero::GetSynKickDocAmount()
{
    int nAmount = m_setSynKickDoc.size();
    return nAmount;
}
//------------------------------------------------------------------
void CHero::AddSynKickDoc(char* pszName)
{
    if (pszName && strlen(pszName))
    {
        m_setSynKickDoc.push_back(pszName);
    }
}
//------------------------------------------------------------------
void CHero::TakeInSynKickDoc(char* pszName)
{
}
//------------------------------------------------------------------
void CHero::SetSynName(int nAction, char* szName)
{
    if (szName != NULL && strlen(szName) > 0)
    {
        unsigned short usAction;
        if (nAction == 1)
        {
            usAction = SYN_CHANGE_SUBNAME;
        }
        else if (nAction == 2)
        {
            usAction = SYN_CHANGE_SUBSUBNAME;
        }
        else
        {
            return;
        }
        CMsgSyndicate msg;
        if (msg.Create(usAction, szName))
        {
            msg.Send();
        }
    }
}
//------------------------------------------------------------------
void CHero::MoveSynMember(char* szName, OBJID idTarget)
{
    if (szName == NULL || strlen(szName) <= 0 || idTarget == ID_NONE)
    {
        return;
    }
    CMsgSyndicate msg;
    if (msg.Create(SYNMEMBER_ASSIGN, szName, idTarget))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
void CHero::AgreeSynKickDoc(int nIndex, BOOL bAgree)
{
    char szName[_MAX_NAMESIZE];
    int nAmount = m_setSynKickDoc.size();
    if (nIndex < nAmount)
    {
        strcpy(szName, m_setSynKickDoc[nIndex].c_str());
        if (strlen(szName) > 0)
        {
            unsigned short usAction;
            if (bAgree)
            {
                usAction = KICKOUT_MEMBER_AGREE;
            }
            else
            {
                usAction = KICKOUT_MEMBER_NOTAGREE;
            }
            CMsgSyndicate msg;
            if (msg.Create(usAction, szName))
            {
                msg.Send();
                m_setSynKickDoc.erase(m_setSynKickDoc.begin() + nIndex);
            }
        }
    }
}

//------------------------------------------------------------------
const char* CHero::GetSynKickDocByIndex(int nIndex)
{
    int nAmount = m_setSynKickDoc.size();
    if (nIndex >= nAmount)
    {
        return NULL;
    }
    else
    {
        return m_setSynKickDoc[nIndex].c_str();
    }
}

//------------------------------------------------------------------
void CHero::QuerySynKickDoc()
{
    m_setSynKickDoc.clear();
    if (this->IsSynLeader())
    {
        CMsgSyndicate msg;
        if (msg.Create(KICKOUT_MEMBER_INFO_QUERY, this->GetSydicateID()))
        {
            msg.Send();
        }
    }
}
