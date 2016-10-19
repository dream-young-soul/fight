
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgSyndicate.cpp: implementation of the CMsgSyndicate class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgSyndicate.h"
#include "GamePlayerSet.h"
#include "Hero.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSyndicate::CMsgSyndicate()
{
    Init();
    m_pInfo	= (MSG_Info* )m_bufMsg;
    m_sName.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgSyndicate::~CMsgSyndicate()
{
}

//-------------------------------------------------------------------
BOOL CMsgSyndicate::Create(char* pMsgBuf, DWORD dwSize)
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
//-------------------------------------------------------------------
BOOL CMsgSyndicate::Create(unsigned short usAction, OBJID idTarget)
{
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_SYNDICATE;
    m_pInfo->usAction = usAction;
    m_pInfo->idTarget = idTarget;
    return true;
}
//-------------------------------------------------------------------
BOOL CMsgSyndicate::Create(unsigned short usAction,  char* szName, OBJID idTarget)
{
    this->Init();
    m_sName.AddString(szName);
    m_pInfo->idTarget = idTarget;
    m_pInfo->usAction = usAction;
    m_pInfo->unMsgType = _MSG_SYNDICATE;
    m_pInfo->unMsgSize = sizeof(MSG_Info) - 1 + m_sName.GetSize();
    return true;
}
//-------------------------------------------------------------------
BOOL CMsgSyndicate::Create(unsigned short usAction, char* pMsgBuf)
{
    this->Init();
    m_sName.AddString(pMsgBuf);
    m_pInfo->usAction = usAction;
    m_pInfo->unMsgType = _MSG_SYNDICATE;
    m_pInfo->unMsgSize = sizeof(MSG_Info) - 1 + m_sName.GetSize();
    return true;
}
//-------------------------------------------------------------------
void CMsgSyndicate::Process(void* pInfo)
{
    switch(m_pInfo->usAction)
    {
    case APPLY_JOIN:
        {
            ::PostCmd(CMD_SYNDICATE_APPLY);
            g_objHero.SetLastSyndicateApplyID(m_pInfo->idTarget);
        }
        break;
    case INVITE_JOIN:
        {
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_pInfo->idTarget);
            if (pPlayer)
            {
                ::PostCmd(CMD_SYNDICATE_INVITE);
                g_objHero.SetLastSyndicateApplyID(m_pInfo->idTarget);
                g_objHero.SetLastSyndicateID(pPlayer->GetSydicateID());
            }
        }
        break;
    case ALLY_APPLY:	//服务器同步盟友帮派
        {
            g_objPlayerSet.GetSyndicateName(m_pInfo->idTarget);
            g_objPlayerSet.SetSyndicateStatus(m_pInfo->idTarget, SYNDICATE_ALLY);
        }
        break;
    case CLEAR_ALLY:
        {
            g_objPlayerSet.GetSyndicateName(m_pInfo->idTarget);
            g_objPlayerSet.SetSyndicateStatus(m_pInfo->idTarget, SYNDICATE_NORMAL);
        }
        break;
    case ANTAGONIZE:		//服务器同步敌对帮派
        {
            g_objPlayerSet.GetSyndicateName(m_pInfo->idTarget);
            g_objPlayerSet.SetSyndicateStatus(m_pInfo->idTarget, SYNDICATE_ANTAGONIZE);
        }
        break;
    case CLEAR_ANTAGONIZE:
        {
            g_objPlayerSet.GetSyndicateName(m_pInfo->idTarget);
            g_objPlayerSet.SetSyndicateStatus(m_pInfo->idTarget, SYNDICATE_NORMAL);
        }
        break;
        /*
        	case SET_MAINSYN:
        		{
        			char szName[_MAX_NAMESIZE]="";
        			char szLeaderName[_MAX_NAMESIZE]="";
        			if (m_sName.GetStrAmount())
        			{
        				m_sName.GetString(0,szName,_MAX_NAMESIZE);
        				m_sName.GetString(1,szLeaderName,_MAX_NAMESIZE);
        			}
        			g_objPlayerSet.AddSyndicate(m_pInfo->idFealty,m_pInfo->idFealty,szName,szLeaderName,SYNDICATE_SELF);
        			g_objPlayerSet.GetSyndicateName(m_pInfo->idTarget);
        			g_objPlayerSet.SetSyndicateStatus(m_pInfo->idTarget, SYNDICATE_SELF);
        		}
        */
        break;
    case SET_SYN:
        {
            char szName[_MAX_NAMESIZE] = "";
            char szLeaderName[_MAX_NAMESIZE] = "";
            if (m_sName.GetStrAmount())
            {
                m_sName.GetString(0, szName, _MAX_NAMESIZE);
                m_sName.GetString(1, szLeaderName, _MAX_NAMESIZE);
            }
            if (m_pInfo->idFealty)
            {
                g_objPlayerSet.AddSyndicate(m_pInfo->idTarget, m_pInfo->idFealty, szName, szLeaderName, SYNDICATE_SUB);
            }
            else
            {
                g_objPlayerSet.AddSyndicate(m_pInfo->idTarget, m_pInfo->idFealty, szName, szLeaderName, SYNDICATE_SELF);
            }
        }
        break;
        /*
        	case SET_SUBSUBSYN:
        		{
        			char szName[_MAX_NAMESIZE]="";
        			char szLeaderName[_MAX_NAMESIZE]="";
        			if (m_sName.GetStrAmount())
        			{
        				m_sName.GetString(0,szName,_MAX_NAMESIZE);
        				m_sName.GetString(1,szLeaderName,_MAX_NAMESIZE);
        			}
        			g_objPlayerSet.AddSyndicate(m_pInfo->idTeam,m_pInfo->idFealty,szName,szLeaderName,SYNDICATE_SUBSUB);
        		}
        		break;*/
        /*
        	case UNITE_SUBSYN:
        		{
        			g_objHero.UniteSubSyn(m_pInfo->dwData, m_pInfo->idTarget);
        		}
        		break;
        	case UNITE_SYN:
        		{
        			g_objHero.UniteSyn(m_pInfo->dwData, m_pInfo->idTarget);
        		}
        		break;
        */
    case SET_WHITE_SYN:
        g_objPlayerSet.SetWhiteSynId(m_pInfo->idTarget);
        break;
    case SET_BLACK_SYN:
        g_objPlayerSet.SetBlackSynId(m_pInfo->idTarget);
        break;
    case DESTROY_SYN:
        g_objPlayerSet.DestroySyndicate(m_pInfo->idTarget);
        break;
    case SET_MANTLE:
        g_objPlayerSet.SetMantle(m_pInfo->idTarget, m_pInfo->dwData);
        break;
    case APPLY_ALLY:
        {
            g_objHero.SetLastSynAlly(m_pInfo->dwData);		//发出请求的player id
            ::PostCmd(CMD_ACTION_ADDALLY);
        }
        break;
    case SYN_SET_PUBLISHTIME: //设置公告时间
        {
            g_objHero.SetSyndicateAnnounceTime(m_pInfo->idFealty);
            ::PostCmd(CMD_FLASH_SYNDICATE_WORD);
        }
        break;
    case KICKOUT_MEMBER_INFO_QUERY : // = 110, 帮主查询申请开除的成员
        {
            if (m_sName.GetStrAmount() > 0)
            {
                for(UINT i = 0; i < m_sName.GetStrAmount(); i++)
                {
                    char szName[_MAX_NAMESIZE] = "";
                    m_sName.GetString(i, szName, _MAX_NAMESIZE);
                    g_objHero.AddSynKickDoc(szName);
                }
            }
            ::PostCmd(CMD_FLASH_SYNDOC);
        }
        break;
    }
}
//-------------------------------------------------------------------
