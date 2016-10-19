
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgBetOutside.cpp: implementation of the CMsgAtk class.
// 仙剑修，2002.1.8
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "basefunc.h"
#include "MapGroup.h"
#include "SynManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSyndicate::CMsgSyndicate()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgSyndicate::~CMsgSyndicate()
{
}

//////////////////////////////////////////////////////////////////////
BOOL	CMsgSyndicate::Create(unsigned short usAction, OBJID idTarget, OBJID idFealty)
{
    // check param
    if (usAction == ACTION_NONE) //  || idTarget == ID_NONE
    {
        ASSERT(!"CMsgSyndicate::Create()");
        return false;
    }
    // init
    this->Init();
    m_pInfo->usAction	= usAction;
    m_pInfo->idTarget	= idTarget;
    m_pInfo->idFealty	= idFealty;
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_SYNDICATE;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL	CMsgSyndicate::Create		(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_SYNDICATE != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void	CMsgSyndicate::Process		(CGameSocket* pSocket)
{
    // 检查消息格式
    // 参数检查
    ASSERT(pSocket);
    if (!pSocket)
    {
        return;
    }
    // 状态检查
    CUserPtr pUser = UserManager()->GetUser(this);
    IF_NOT(pUser)
    return;
    OBJID idUser = pUser->GetID();
    IF_NOT(idUser != ID_NONE)
    return;
    if (!pUser->IsAlive())
    {
        pUser->SendSysMsg(STR_DIE);
        return ;
    }
    char szName[_MAX_NAMESIZE] = "";
    if (m_StrPacker.GetStrAmount() > 0)
    {
        m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
    }
    switch(m_pInfo->usAction)
    {
        //	case	ACTION_NONE = 0,			// 保留值
        // the operation of all users
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case SYN_DEMISE:
        {
            // 检查帮派
            OBJID idSyn = pUser->GetSynID();
            if (idSyn == ID_NONE)
            {
                return ;
            }
            CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
            if (!pSyn)
            {
                return ;
            }
            // 检查权限
            if (pUser->GetSynRank() != RANK_LEADER)
            {
                return ;
            }
            // target syn
            CUser* pTarget = UserManager()->GetUser(szName);
            if (!pTarget)
            {
                pUser->SendSysMsg(STR_NOT_HERE);
                return ;
            }
            if ((pTarget->GetID() == pUser->GetID()) || (pTarget->GetSynID() != pUser->GetSynID()))
            {
                return ;
            }
            int nLevel = 50;
            int nProffer = 10000;
            if (pTarget->GetLev() < nLevel || pTarget->QuerySynProffer() < nProffer)
            {
                char szBuff[128];
                sprintf(szBuff, STR_LEVEL_NOT_ENOUGH, pTarget->GetName());
                pUser->SendSysMsg(szBuff);
                return ;
            }
            OBJID	idTargetSyn = pTarget->GetSynID();
            if (idTargetSyn == ID_NONE || idTargetSyn != idSyn)
            {
                return ;
            }
            pSyn->QueryModify()->Demise(pUser->GetID(), pUser->QuerySynAttr()->GetSynMemberLevel(), pTarget->GetID(), pTarget->GetName(), pTarget->QuerySynAttr()->GetSynMemberLevel(), pSyn->GetID());
            char szBuff[128];
            sprintf(szBuff, STR_SYN_DEMISE_SUCC, szName);
            pUser->SendSysMsg(szBuff);
            sprintf(szBuff, STR_SYN_DEMISE_CON, pSyn->GetStr(SYNDATA_NAME));
            pTarget->SendSysMsg(szBuff);
            sprintf(szBuff, STR_SYN_DEMISE_PROMPT, pTarget->GetName(), pSyn->GetStr(SYNDATA_NAME));
            pTarget->SendSysMsg(_TXTATR_SYNDICATE, szBuff);
        }
        break;
    case SYN_SET_ASSISTANT://设置副团
        {
            // 检查帮派
            OBJID idSyn = pUser->GetSynID();
            if (idSyn == ID_NONE)
            {
                return ;
            }
            CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
            if (!pSyn)
            {
                return ;
            }
            // 检查权限
            int nRank = pUser->GetSynRank();
            if (nRank != RANK_LEADER)
            {
                return ;
            }
            // check amount, temporary code.
            if (pUser->QuerySynAttr()->GetAssistantCount() >= MAX_ASSISTANTSIZE)
            {
                pUser->SendSysMsg(STR_SYN_PLACE_FULL);
                return ;
            }
            // target syn
            CUser* pTarget = UserManager()->GetUser(szName);
            if (!pTarget || (!pTarget->GetSyndicate()->IsSubSyn()))
            {
                pUser->SendSysMsg(STR_NOT_HERE);
                return ;
            }
            int nLevel = 40;
            int nProffer = 3000;
            if (pTarget->GetLev() < nLevel && pTarget->QuerySynProffer() < nProffer)
            {
                char szBuff[128];
                sprintf(szBuff, STR_LEVEL_NOT_ENOUGH, pTarget->GetName());
                pUser->SendSysMsg(szBuff);
                pTarget->SendSysMsg(STR_SYN_SETSUBLEADER_ERR);
                return ;
            }
            OBJID	idTargetSyn = pTarget->GetSynID();
            if (idTargetSyn == ID_NONE || idTargetSyn != idSyn)
            {
                return ;
            }
            CSynPtr pSubSyn = SynManager()->QuerySyndicate(idTargetSyn);
            CHECK(pSubSyn);
            pSubSyn->QueryModify()->Demise(pSubSyn->GetInt(SYNDATA_LEADERID), 0, pTarget->GetID(), pTarget->GetName(), pTarget->QuerySynAttr()->GetSynMemberLevel(), pSubSyn->GetID());
            char szBuff[128];
            sprintf(szBuff, STR_SYN_SETSUBLEADER_SUCC, szName, pTarget->GetSyndicate()->GetStr(SYNDATA_NAME));
            pUser->SendSysMsg(szBuff);
            sprintf(szBuff, STR_SYN_SETSUBLEADER_CON, pSyn->GetStr(SYNDATA_NAME));
            pTarget->SendSysMsg(szBuff);
            sprintf(szBuff, STR_SYN_SETSUBLEADER_PROMPT, pTarget->GetName(), pTarget->GetSyndicate()->GetStr(SYNDATA_NAME));
            pTarget->SendSysMsg(_TXTATR_SYNDICATE, szBuff);
        }
        break;
    case SYN_SET_TEAMLEADER:
        {
            OBJID idSyn = pUser->GetSynID();
            if (idSyn == ID_NONE)
            {
                return ;
            }
            CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
            if (!pSyn)
            {
                return ;
            }
            // 检查权限
            int nRank = pUser->GetSynRank();
            if (nRank != RANK_LEADER || nRank != RANK_SUBLEADER)
            {
                return ;
            }
            // check amount, temporary code.
            if (pUser->QuerySynAttr()->GetAssistantCount() >= MAX_ASSISTANTSIZE)
            {
                pUser->SendSysMsg(STR_SYN_PLACE_FULL);
                return ;
            }
            // target syn
            CUser* pTarget = UserManager()->GetUser(szName);
            if (!pTarget || (!pTarget->GetSyndicate()->IsSubSubSyn()))
            {
                pUser->SendSysMsg(STR_NOT_HERE);
                return ;
            }
            int nLevel = 30;
            int nProffer = 1000;
            if (pTarget->GetLev() < nLevel && pTarget->QuerySynProffer() < nProffer )
            {
                char szBuff[128];
                sprintf(szBuff, STR_LEVEL_NOT_ENOUGH, pTarget->GetName());
                pTarget->SendSysMsg(szBuff);
                return ;
            }
            OBJID	idTargetSyn = pTarget->GetSynID();
            if (idTargetSyn == ID_NONE || idTargetSyn == idSyn)
            {
                return ;
            }
            CSynPtr pSubSubSyn = SynManager()->QuerySyndicate(idTargetSyn);
            CHECK(pSubSubSyn);
            if (nRank == RANK_SUBLEADER)
            {
                if (idSyn != pSubSubSyn->GetInt(SYNDATA_FEALTY))
                {
                    return;
                }
            }
            pSubSubSyn->QueryModify()->Demise(pSubSubSyn->GetInt(SYNDATA_LEADERID), 0, pTarget->GetID(), pTarget->GetName(), pTarget->QuerySynAttr()->GetSynMemberLevel(), pSubSubSyn->GetID());
            char szBuff[128];
            sprintf(szBuff, STR_SYN_SETSUBSUBLEADER_SUCC, szName, pTarget->GetSyndicate()->GetStr(SYNDATA_NAME));
            pUser->SendSysMsg(szBuff);
            sprintf(szBuff, STR_SYN_SETSUBSUBLEADER_CON, pSyn->GetStr(SYNDATA_NAME));
            pTarget->SendSysMsg(szBuff);
            sprintf(szBuff, STR_SYN_SETSUBSUBLEADER_PROMPT, pTarget->GetName(), pTarget->GetSyndicate()->GetStr(SYNDATA_NAME));
            pTarget->SendSysMsg(_TXTATR_SYNDICATE, szBuff);
        }
        break;
        /*
        	case SYN_CHANGE_SUBSUBNAME://改队名
        		{
        			OBJID idSyn = pUser->GetSynID();
        			int	  nRank = pUser->GetSynRankShow();
        			if (idSyn == ID_NONE || nRank != RANK_TEAMLEADER)
        				return ;
        			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);

        			if (!pSyn || strlen(pSyn->GetStr(SYNDATA_NAME)) <= 6 || !pSyn->IsSubSubSyn())
        				return ;

        			pSyn->SetNewSubName(szName);

        			CMsgSyndicate	msg;
        			if (msg.Create(SYN_CHANGE_SUBSUBNAME, pSyn->GetID(), szName))
        				pSyn->BroadcastSynMsg(&msg);

        		}
        		break;
        	case SYN_CHANGE_SUBNAME://改分团名
        		{
        			OBJID idSyn = pUser->GetSynID();
        			int	  nRank = pUser->GetSynRankShow();
        			if (idSyn == ID_NONE || nRank != RANK_SUBLEADER)
        				return ;
        			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);

        			if (!pSyn || strlen(pSyn->GetStr(SYNDATA_NAME)) <= 6 || !pSyn->IsSubSyn())
        				return ;

        			pSyn->SetNewSubName(szName);

        			CMsgSyndicate	msg;
        			if (msg.Create(SYN_CHANGE_SUBNAME,  pSyn->GetID(), szName))
        				pSyn->BroadcastSynMsg(&msg);
        		}
        		break;
        */
    case SYNMEMBER_ASSIGN:			//帮派成员编制---
        {
            int nRank = pUser->GetSynRank();
            if (nRank != RANK_LEADER || nRank != RANK_SUBLEADER)
            {
                return;
            }
            CSyndicate* pSyn = pUser->GetSyndicate();
            CHECK(pSyn);
            OBJID idSynAssigned = m_pInfo->idTarget;
            CSyndicate* pSynAssigned = SynManager()->QuerySyndicate(idSynAssigned);
            CHECK(pSynAssigned);
            OBJID idUser = m_pInfo->idUser;
            CUser* pAssignUser = UserManager()->GetUser(idUser);
            CHECK(pAssignUser);
            CSyndicate* pCurSyn = pAssignUser->GetSyndicate();
            if (pCurSyn->GetID() == pSynAssigned->GetID()) //不能为同一个帮派
            {
                return;
            }
            //---开始分配---begin
            if (pAssignUser->QuerySynAttr()->ChangeSyn(pCurSyn, pSynAssigned))
            {
                //成功后，回应消息
                pCurSyn->BroadcastSynMsg(this);
                pAssignUser->QuerySynAttr()->SynchroInfo(true);
            }
            //---开始分配---end
        }
        break;
    case	APPLY_JOIN:					// 申请加入黑社会
        {
            // 查参数
            if (m_pInfo->idTarget == ID_NONE || m_pInfo->idTarget == idUser)
            {
                return;
            }
            CUserPtr pTarget = g_UserManager.GetUser(m_pInfo->idTarget);
            if (!pTarget)
            {
                return ;
            }
            // 检查帮派
            OBJID	idSyn	= pUser->GetSynID();
            int		nRank	= pUser->GetSynRank();
            OBJID	idTargetSyn = pTarget->GetSynID();
            int		nTargetRank	= pTarget->GetSynRank();
            //检查 级别
            if (idSyn != ID_NONE || idTargetSyn == ID_NONE || nTargetRank < RANK_SUBLEADER)
            {
                pUser->SendSysMsg(STR_SYN_APPLY_JOIN_FAILED);
                return;
            }
            // no money ?
            CSyndicate* pTargetSyn = SynManager()->QuerySyndicate(idTargetSyn);
            if (!pTargetSyn)
            {
                return;
            }
            if (pTargetSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
            {
                pUser->SendSysMsg(STR_TOO_LOWER_SYN_MONEY);
                return;
            }
            // is full?
            /*
            if (pTarget->QuerySynAttr()->SumSynMember() > SYN_MEMBER_LIMIT)
            {
            	pUser->SendSysMsg(STR_SYN_FULL);
            	return;
            }
            */
            if (!pTarget->FetchApply(CUser::APPLY_INVITE_JOIN_SYN, idUser))
            {
                // 保存申请项
                pUser->SetApply(CUser::APPLY_JOIN_SYN, m_pInfo->idTarget);
                IF_OK_(Create(APPLY_JOIN, idUser))
                pTarget->SendMsg(this);
                return;
            }
            nRank = RANK_NEWBIE;
            if ( pUser->GetLev() < SYN_CONDITION_NEWBIE)
            {
                nRank = RANK_RESERVED;
            }
            pUser->QuerySynAttr()->JoinSyn(idTargetSyn, nRank);
            pUser->SendSysMsg(STR_JOIN_SYN_s, pTargetSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case	INVITE_JOIN:				// 邀请加入黑社会
        {
            // 查参数
            if (m_pInfo->idTarget == ID_NONE || m_pInfo->idTarget == idUser)
            {
                return;
            }
            CUserPtr pTarget = g_UserManager.GetUser(m_pInfo->idTarget);
            if (!pTarget)
            {
                return ;
            }
            // 检查帮派
            OBJID	idSyn	= pUser->GetSynID();
            int		nRank	= pUser->GetSynRank();
            OBJID	idTargetSyn = pTarget->GetSynID();
            int		nTargetRank	= pTarget->GetSynRank();
            if (idSyn == ID_NONE || nRank < RANK_SUBLEADER || idTargetSyn != ID_NONE)
            {
                pUser->SendSysMsg(STR_JOIN_SYN_FAILED);
                return;
            }
            // no money ?
            CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
            if (!pSyn)
            {
                return;
            }
            if (pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
            {
                pUser->SendSysMsg(STR_TOO_LOWER_SYN_MONEY);
                return;
            }
            /*
            // is full?
            if (pUser->QuerySynAttr()->SumSynMember() > SYN_MEMBER_LIMIT)
            {
            	pUser->SendSysMsg(STR_SYN_FULL);
            	return;
            }
            */
            if (!pTarget->FetchApply(CUser::APPLY_JOIN_SYN, idUser))
            {
                // 保存申请项
                pUser->SetApply(CUser::APPLY_INVITE_JOIN_SYN, m_pInfo->idTarget);
                IF_OK_(Create(INVITE_JOIN, idUser))
                pTarget->SendMsg(this);
                return;
            }
            //级别低于20级 是预备团员
            nRank = RANK_NEWBIE;
            if ( pTarget->GetLev() < SYN_CONDITION_NEWBIE)
            {
                nRank = RANK_RESERVED;
            }
            pTarget->QuerySynAttr()->JoinSyn(idSyn, nRank);
            pTarget->SendSysMsg(STR_JOIN_SYN_s, pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));
        }
        break;
        // the operation of members
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case	LEAVE_SYN:				// 脱离黑社会
        {
            // 检查帮派
            OBJID idSyn = pUser->GetSynID();
            if (idSyn == ID_NONE)
            {
                return;
            }
            // 检查帮主
            int	nRank = pUser->GetSynRank();
            if (nRank == RANK_LEADER)
            {
                pUser->SendSysMsg(STR_NO_DISBAND);
                return;
            }
            else
            {
                CONST int MINPROFFER_QUITSYN = 200; // 预备团员以上退出军团必须军团贡献度200以上
                if (nRank > RANK_RESERVED && pUser->QuerySynProffer() < MINPROFFER_QUITSYN)
                {
                    pUser->SendSysMsg(STR_SYN_NOTENOUGH_PROFFER_LEAVE, pUser->QuerySynProffer());
                    return;
                }
            }
            pUser->QuerySynAttr()->LeaveSyn();
            CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
            if (pSyn)
            {
                char szMsg[256];
                sprintf(szMsg, STR_SYN_LEAVE_s, pUser->GetName());
                CMsgTalk	msg;
                IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, 0xFFFFFF, _TXTATR_SYNDICATE))
                pSyn->BroadcastSynMsg(&msg);
            }
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case KICKOUT_MEMBER_NOAGREE:
        {
            //////////////////////////////////////////////////////////////////////////
            char	szSQL[1024];
            sprintf(szSQL, "SELECT * FROM %s WHERE words LIKE 'ApplyDismiss%'", _TBL_LEAVEWORD);
            IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
            if (pRes)
            {
                for(int i = 0; i < pRes->RecordCount(); i++)
                {
                    char	szRecvUser[_MAX_NAMESIZE];
                    SafeCopy(szRecvUser, pRes->GetStr(LEAVEWORDDATA_USER), _MAX_NAMESIZE);
                    CUserPtr pUser = g_UserManager.GetUser(szRecvUser);
                    if (pUser && !pUser->IsMoreLeaveWord())
                    {
                        char	szTime[_MAX_NAMESIZE];
                        char	szWords[_MAX_WORDSSIZE];
                        SafeCopy(szTime, pRes->GetStr(LEAVEWORDDATA_TIME), _MAX_NAMESIZE);
                        SafeCopy(szWords, pRes->GetStr(LEAVEWORDDATA_WORDS), _MAX_WORDSSIZE);
                        //---jinggy---2004-12-20---判断是否是 申请开除会员的留言---begin
                        char szBuffer[255] = ""; //开除成员的标志
                        OBJID idSyn; //要开除的成员的帮派ID
                        char  szNameDismissed[16]; //开除的成员ID
                        char  szLeaderName[16];
                        if (4 != sscanf(szWords, "%s %d %s %s", szBuffer, &idSyn, szLeaderName, szNameDismissed))
                        {
                            continue;
                        }
                        if (strcmp(szBuffer, "ApplyDismiss") == 0 && idSyn > 0 ) //是申请开除留言
                        {
                            CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
                            CHECK(pSyn);
                            DWORD dwMainSynID = pSyn->GetMasterSynID();
                            char szCurLeaderName[16];
                            sprintf(szCurLeaderName, "%s", pSyn->GetStr(SYNDATA_LEADERNAME));
                            if (strcmp(szNameDismissed, szName) == 0) //有效期内此人还是领导则发送请求
                            {
                                //删除记录
                                OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
                                sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
                                g_db.ExecuteSQL(szSQL);
                                break;
                            }
                        }
                    }
                }
                pRes->MoveNext();
            }
            pRes->Release();
        }
        break;
    case KICKOUT_MEMBER_INFO_QUERY: //帮主查列表
        {
            if (pUser->GetSynRank() != RANK_LEADER)
            {
                return;
            }
            char	szSQL[1024];
            sprintf(szSQL, "SELECT * FROM %s WHERE words LIKE 'ApplyDismiss%'", _TBL_LEAVEWORD);
            IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
            if (pRes)
            {
                for(int i = 0; i < pRes->RecordCount(); i++)
                {
                    char	szRecvUser[_MAX_NAMESIZE];
                    SafeCopy(szRecvUser, pRes->GetStr(LEAVEWORDDATA_USER), _MAX_NAMESIZE);
                    CUserPtr pUser = g_UserManager.GetUser(szRecvUser);
                    if (pUser && !pUser->IsMoreLeaveWord())
                    {
                        //	char	szSender[_MAX_NAMESIZE];
                        char	szTime[_MAX_NAMESIZE];
                        char	szWords[_MAX_WORDSSIZE];
                        //	SafeCopy(szSender, pRes->GetStr(LEAVEWORDDATA_SENDER), _MAX_NAMESIZE);
                        SafeCopy(szTime, pRes->GetStr(LEAVEWORDDATA_TIME), _MAX_NAMESIZE);
                        SafeCopy(szWords, pRes->GetStr(LEAVEWORDDATA_WORDS), _MAX_WORDSSIZE);
                        //---jinggy---2004-12-20---判断是否是 申请开除会员的留言---begin
                        char szBuffer[255] = ""; //开除成员的标志
                        OBJID idSyn; //要开除的成员的帮派ID
                        char  szNameDismissed[16]; //开除的成员ID
                        char  szLeaderName[16];
                        if (4 != sscanf(szWords, "%s %d %s %s", szBuffer, &idSyn, szLeaderName, szNameDismissed))
                        {
                            continue;
                        }
                        if (strcmp(szBuffer, "ApplyDismiss") == 0 && idSyn > 0 ) //是申请开除留言
                        {
                            time_t	tDelta = time(NULL);
                            tDelta -= 7 * 86400;  //超过7天，帮主没有相应就表示同意开除
                            char	szDelta[_MAX_NAMESIZE];
                            tm*	pTm		= localtime(&tDelta);
                            if (pTm)
                            {
                                sprintf(szDelta, "%04d%02d%02d%02d%02d%02d",
                                        pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
                            }
                            if (strcmp(szTime, szDelta) < 0)
                            {
                                //响应过期
                                CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
                                if (pSyn && pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
                                {
                                    pUser->SendSysMsg(STR_TOO_LOWER_SYN_MONEY);
                                    return;
                                }
                                pUser->QuerySynAttr()->KickoutMember(szNameDismissed);
                                MSGBUF	szMsg;
                                sprintf(szMsg, STR_KICKOUT_MEMBER_s, szNameDismissed);
                                if (pSyn)
                                {
                                    pSyn->BroadcastSubSynMsg(pUser->GetSocketID(), 0, szMsg);
                                }
                                //删除记录
                                OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
                                sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
                                g_db.ExecuteSQL(szSQL);
                            }
                            else
                            {
                                CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
                                DWORD dwMainSynID = pSyn->GetMasterSynID();
                                char szCurLeaderName[16];
                                sprintf(szCurLeaderName, "%s", pSyn->GetStr(SYNDATA_LEADERNAME));
                                if (strcmp(szCurLeaderName, szLeaderName) == 0) //有效期内此人还是领导则发送请求
                                {
                                    //向帮主发送申请开除的成员的信息
                                    //									if (Create(KICKOUT_MEMBER_INFO_QUERY, szNameDismissed))
                                    //										pUser->SendMsg(this);
                                }
                                else //在有效期内若是换了帮主，则取消开除
                                {
                                    //删除记录
                                    OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
                                    sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
                                    g_db.ExecuteSQL(szSQL);
                                }
                            }
                        }
                    }
                    pRes->MoveNext();
                }
                pRes->Release();
            }
        }
        break;
    case KICKOUT_MEMBER_AGREE:  //帮主同意申请的开除会员
        {
            // 检查帮派
            OBJID idSyn = pUser->GetSynID();
            int nRank = pUser->GetSynRank();
            if (idSyn == ID_NONE || nRank != RANK_LEADER )
            {
                pUser->SendSysMsg(STR_NOT_AUTHORIZED);
                return;
            }
            CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
            // no money ?
            if (pSyn && pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
            {
                pUser->SendSysMsg(STR_TOO_LOWER_SYN_MONEY);
                return;
            }
            //pTarget->QuerySynAttr()->LeaveSyn();		// 改成立刻踢出
            pUser->QuerySynAttr()->KickoutMember(szName);
            MSGBUF	szMsg;
            sprintf(szMsg, STR_KICKOUT_MEMBER_s, LPCTSTR(szName));
            IF_OK(pSyn)
            pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
            //////////////////////////////////////////////////////////////////////////
            char	szSQL[1024];
            sprintf(szSQL, "SELECT * FROM %s WHERE words LIKE 'ApplyDismiss%'", _TBL_LEAVEWORD);
            IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
            if (pRes)
            {
                for(int i = 0; i < pRes->RecordCount(); i++)
                {
                    char	szRecvUser[_MAX_NAMESIZE];
                    SafeCopy(szRecvUser, pRes->GetStr(LEAVEWORDDATA_USER), _MAX_NAMESIZE);
                    CUserPtr pUser = g_UserManager.GetUser(szRecvUser);
                    if (pUser && !pUser->IsMoreLeaveWord())
                    {
                        char	szTime[_MAX_NAMESIZE];
                        char	szWords[_MAX_WORDSSIZE];
                        SafeCopy(szTime, pRes->GetStr(LEAVEWORDDATA_TIME), _MAX_NAMESIZE);
                        SafeCopy(szWords, pRes->GetStr(LEAVEWORDDATA_WORDS), _MAX_WORDSSIZE);
                        //---jinggy---2004-12-20---判断是否是 申请开除会员的留言---begin
                        char szBuffer[255] = ""; //开除成员的标志
                        OBJID idSyn; //要开除的成员的帮派ID
                        char  szNameDismissed[16]; //开除的成员ID
                        char  szLeaderName[16];
                        if (4 != sscanf(szWords, "%s %d %s %s", szBuffer, &idSyn, szLeaderName, szNameDismissed))
                        {
                            continue;
                        }
                        if (strcmp(szBuffer, "ApplyDismiss") == 0 && idSyn > 0 ) //是申请开除留言
                        {
                            CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
                            CHECK(pSyn);
                            DWORD dwMainSynID = pSyn->GetMasterSynID();
                            char szCurLeaderName[16];
                            sprintf(szCurLeaderName, "%s", pSyn->GetStr(SYNDATA_LEADERNAME));
                            if (strcmp(szNameDismissed, szName) == 0) //有效期内此人还是领导则发送请求
                            {
                                //删除记录
                                OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
                                sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
                                g_db.ExecuteSQL(szSQL);
                                break;
                            }
                        }
                    }
                }
                pRes->MoveNext();
            }
            pRes->Release();
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case	KICKOUT_MEMBER:				// 开除黑社会成员
        {
            // 检查帮派
            OBJID idSyn = pUser->GetSynID();
            int nRank = pUser->GetSynRank();
            if (idSyn == ID_NONE || nRank != RANK_LEADER)
            {
                pUser->SendSysMsg(STR_NOT_AUTHORIZED);
                return;
            }
            CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
            CHECK(pSyn);
            // no money ?
            if (pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
            {
                pUser->SendSysMsg(STR_TOO_LOWER_SYN_MONEY);
                return;
            }
            // 开除
            CUser* pTargetUser = UserManager()->GetUser(szName);
            if (pTargetUser)
            {
                pTargetUser->SendSysMsg(STR_KICKOUT_MEMBER_s, pTargetUser->GetName());
                if (pTargetUser->GetSynID() == idSyn)
                {
                    pTargetUser->QuerySynAttr()->LeaveSyn();
                }
            }
            else
            {
                pUser->QuerySynAttr()->KickoutMember(szName);
            }
            MSGBUF	szMsg;
            //			// 通知对方
            //			sprintf(szMsg, STR_KICKOUT_ME_s, pUser->GetName());
            //			CMsgTalk msg;
            //			if (msg.Create(SYSTEM_NAME, szName, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
            //				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, szName);
            sprintf(szMsg, STR_KICKOUT_MEMBER_s, szName, pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));
            IF_OK(pSyn)
            pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case	QUERY_SYN_NAME:
        {
            OBJID idSyn	= m_pInfo->idTarget;
            CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
            if (!pSyn)
            {
                //				LOGWARNING("玩家[%s]未找到帮派名[%d]!", pUser->GetName(), idSyn);
                return;
            }
            CSynPtr pMasterSyn = pSyn->GetMasterSyn();
            CHECK(pMasterSyn);
            CMsgName	msg;
            IF_OK(msg.Create(NAMEACT_SYNDICATE, pMasterSyn->GetStr(SYNDATA_NAME), idSyn))
            {
                if (pSyn->GetID() != pMasterSyn->GetID())
                {
                    ASSERT(msg.AppendStr(pSyn->GetStr(SYNDATA_NAME)));
                }
                pUser->SendMsg(&msg);
            }
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case	DONATE_MONEY:
        {
            if (pUser->GetSynID() == ID_NONE)
            {
                return ;
            }
            int nMoney = m_pInfo->idTarget;
            pUser->QuerySynAttr()->DonateMoney(nMoney);
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case	QUERY_SYNATTR:
        {
            pUser->QuerySynAttr()->SendInfoToClient();
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case	_APPLY_ALLY:	// 请求添加结盟帮派
        {
            // 查参数
            if (m_pInfo->idTarget == ID_NONE || m_pInfo->idTarget == idUser)
            {
                return;
            }
            CUserPtr pTargetUser = UserManager()->GetUser(m_pInfo->idTarget);
            if (!pTargetUser)
            {
                return;
            }
            // 检查帮派
            OBJID	idSyn	= pUser->GetSynID();
            int		nRank	= pUser->GetSynRankShow();
            OBJID	idTargetSyn = pTargetUser->GetSynID();
            int		nTargetRank	= pTargetUser->GetSynRankShow();
            if (idSyn == ID_NONE || nRank != RANK_LEADER
                    || idTargetSyn == ID_NONE || nTargetRank != RANK_LEADER)
            {
                pUser->SendSysMsg(STR_APPLY_ALLY_FAILED);
                return;
            }
            if (!pTargetUser->FetchApply(CUser::APPLY_ALLY_SYN, idUser))
            {
                // 保存申请项
                pUser->SetApply(CUser::APPLY_ALLY_SYN, m_pInfo->idTarget);
                IF_OK_(Create(_APPLY_ALLY, idUser))
                pTargetUser->SendMsg(this);
                return;
            }
            // set ally
            CSyndicate* pSyn = pUser->GetSyndicate();
            CHECK(pSyn);
            CSyndicate* pTargetSyn = pTargetUser->GetSyndicate();
            CHECK(pTargetSyn);
            // 先解除敌对关系
            pSyn->ClearEnemySyn(pTargetSyn);
            pTargetSyn->ClearEnemySyn(pSyn);
            int nIdx = -1;
            for(int i = 0; i < MAX_SYNALLYSIZE; i++)
            {
                OBJID idTarget = pSyn->GetInt((SYNDATA)(SYNDATA_ALLY0 + i));
                if (idTarget == ID_NONE || SynManager()->QuerySyndicate(idTarget) == NULL || idTarget == pTargetSyn->GetID())
                {
                    nIdx = i;
                }
            }
            if (nIdx == -1)
            {
                pUser->SendSysMsg(STR_ALLY_FULL);
                return ;
            }
            int nIdx2 = -1;
            for(int i = 0; i < MAX_SYNALLYSIZE; i++)
            {
                OBJID idTarget = pTargetSyn->GetInt((SYNDATA)(SYNDATA_ALLY0 + i));
                if (idTarget == ID_NONE || SynManager()->QuerySyndicate(idTarget) == NULL || idTarget == pSyn->GetID())
                {
                    nIdx2 = i;
                }
            }
            if (nIdx2 == -1)
            {
                pUser->SendSysMsg(STR_HIS_ALLY_FULL);
                return ;
            }
            pSyn->QueryModify()->SetData((SYNDATA)(SYNDATA_ALLY0 + nIdx), pTargetSyn->GetID(), true);		// true: 必须save
            pTargetSyn->QueryModify()->SetData((SYNDATA)(SYNDATA_ALLY0 + nIdx2), pSyn->GetID(), true);	// true: 必须save
            MSGBUF szMsg;
            sprintf(szMsg, STR_SYN_ALLY_s, pSyn->GetStr(SYNDATA_NAME), pTargetSyn->GetStr(SYNDATA_NAME));
            pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
            pTargetSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
            {
                // synchro npc server
                CMsgSyndicate	msg;
                IF_OK(msg.Create(SET_ALLY, pTargetSyn->GetID()))
                MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(pSyn->GetID(), &msg);
            }
        }
        break;
    case _CLEAR_ALLY: //客户端向服务器 解除结盟
        {
            // 检查帮派
            OBJID idSyn = pUser->QuerySynAttr()->GetSynID();
            int	nRankShow	= pUser->QuerySynAttr()->GetSynRankShow();
            if (idSyn == ID_NONE || nRankShow != RANK_LEADER)
            {
                return ;
            }
            CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
            CHECK(pSyn);
            CSyndicate* pTargetSyn = SynManager()->QuerySynByName(szName);
            if (!pTargetSyn)
            {
                return ;
            }
            pSyn->ClearAllySyn(pTargetSyn);
            MSGBUF szMsg;
            sprintf(szMsg, STR_SYN_CLEAR_ALLY1_s, szName);
            pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
            sprintf(szMsg, STR_SYN_CLEAR_ALLY2_s, pSyn->GetStr(SYNDATA_NAME));
            pTargetSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
        }
        break;
    case _SET_ANTAGONIZE:	// 添加敌对军团
        {
            // 检查帮派
            OBJID idSyn = pUser->QuerySynAttr()->GetSynID();
            int nRankShow = pUser->QuerySynAttr()->GetSynRankShow();
            if (idSyn == ID_NONE || nRankShow != RANK_LEADER)
            {
                return ;
            }
            CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
            CHECK(pSyn);
            // 检查帮派
            CSyndicate* pTargetSyn = SynManager()->QuerySynByName(szName);
            if (!pTargetSyn)
            {
                return;
            }
            // 先解除同盟关系
            pSyn->ClearAllySyn(pTargetSyn);
            if ((!pTargetSyn->IsMasterSyn()) || pTargetSyn->GetID() == idSyn)
            {
                return ;
            }
            for(int i = 0; i < MAX_SYNENEMYSIZE; i++)
            {
                OBJID idTarget = pSyn->GetInt((SYNDATA)(SYNDATA_ENEMY0 + i));
                if (idTarget != ID_NONE && idTarget == pTargetSyn->GetID())
                {
                    return ;
                }
            }
            for(int i = 0; i < MAX_SYNENEMYSIZE; i++)
            {
                OBJID idTarget = pSyn->GetInt((SYNDATA)(SYNDATA_ENEMY0 + i));
                if (idTarget == ID_NONE || SynManager()->QuerySyndicate(idTarget) == NULL)
                {
                    pSyn->QueryModify()->SetData((SYNDATA)(SYNDATA_ENEMY0 + i), pTargetSyn->GetID(), true);		// true: 必须save
                    MSGBUF szMsg;
                    sprintf(szMsg, STR_SYN_ENEMY_s, pSyn->GetStr(SYNDATA_NAME));
                    pTargetSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
                    break;
                }
            }
            CMsgSyndicate	msg;
            IF_OK(msg.Create(SET_ANTAGONIZE, pTargetSyn->GetID()))
            MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(pSyn->GetID(), &msg);
        }
        break;
    case _CLEAR_ANTAGONIZE:	// 删除敌对军团
        {
            // 检查帮派
            OBJID idSyn = pUser->QuerySynAttr()->GetSynID();
            int nRankShow = pUser->QuerySynAttr()->GetSynRankShow();
            if (idSyn == ID_NONE || nRankShow != RANK_LEADER)
            {
                return ;
            }
            CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
            CHECK(pSyn);
            CSyndicate* pTargetSyn = SynManager()->QuerySynByName(szName);
            if (!pTargetSyn)
            {
                return;
            }
            pSyn->ClearEnemySyn(pTargetSyn);
            MSGBUF szMsg;
            sprintf(szMsg, STR_SYN_CLEAR_ENEMY_s, pSyn->GetStr(SYNDATA_NAME));
            pTargetSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
        }
        break;
    default:
        ASSERT(!"switch MsgSyndicate");
        return;
    }
}
