
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NetMsg.cpp: implementation of the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "basefunc.h"
#include "common.h"
#include "MapGroup.h"

//@ÔÝÆÁ±Î	MYHEAP_IMPLEMENTATION(CNetMsg,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNetMsg::CNetMsg()
{
    Init();
}

CNetMsg::~CNetMsg()
{
}

//////////////////////////////////////////////////////////////////////
void CNetMsg::Init()
{
    memset(m_bufMsg, 0L, sizeof(m_bufMsg));
    m_unMsgSize	= 0;
    m_unMsgType	= _MSG_NONE;
}

//////////////////////////////////////////////////////////////////////
BOOL CNetMsg::IsValid(void)
{
    if (_MSG_NONE == this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CNetMsg::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CNetMsg::Process(void* pInfo)
{
    //--------------------
    char szMsg[1024];
    sprintf(szMsg, "Process Msg:%d, Size:%d", m_unMsgType, m_unMsgSize);
    ::LogSave(szMsg);
}

//////////////////////////////////////////////////////////////////////
bool CNetMsg::SendMsg(CNetMsg* pMsg)
{
#ifdef _MYDEBUG
    ::LogMsg("SendMsg:%d, size:%d", m_unMsgType, m_unMsgSize);
#endif
    if (!IsNpcMsg())
    {
        return MapGroup(PID)->QueryIntraMsg()->SendClientMsg(m_idSocket, pMsg);
    }
    else
    {
        return MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(m_idNpc, pMsg);
    }
}

//////////////////////////////////////////////////////////////////////
// static functions;
//////////////////////////////////////////////////////////////////////
CNetMsg* CNetMsg::CreateClientMsg(PROCESS_ID idProcess, SOCKET_ID idSocket, OBJID idMsg,
                                  const char* pbufMsg, DWORD dwMsgSize, int nTrans)
{
    // check it...
    if (idMsg == _MSG_NONE || !pbufMsg || (int)dwMsgSize > GetMaxSize())
    {
        return NULL;
    }
    CNetMsg* pMsg = CreateMsg(idMsg, pbufMsg, dwMsgSize);
    if (!pMsg)
    {
        return NULL;
    }
    pMsg->m_idProcess	= idProcess;
    pMsg->m_idSocket	= idSocket;
    pMsg->m_idNpc		= ID_NONE;
    pMsg->m_unMsgType	= idMsg;
    pMsg->m_unMsgSize	= dwMsgSize;
    pMsg->m_nTransData	= nTrans;
    if (!pMsg->CNetMsg::Create((char*)pbufMsg, dwMsgSize))
    {
        SAFE_DELETE(pMsg);
        return NULL;
    }
    else
    {
#ifdef _MYDEBUG
        ::LogMsg("\n--Receive Msg:%u, Size:%u", pMsg->GetType(), pMsg->GetSize());
#endif
        return pMsg;
    }
}

//////////////////////////////////////////////////////////////////////
CNetMsg* CNetMsg::CreateNpcMsg(PROCESS_ID idProcess, OBJID idNpc, OBJID idMsg,
                               const char* pbufMsg, DWORD dwMsgSize, int nTrans)
{
    // check it...
    if (idMsg == _MSG_NONE || !pbufMsg || (int)dwMsgSize > GetMaxSize())
    {
        return NULL;
    }
    CNetMsg* pMsg = CreateMsg(idMsg, pbufMsg, dwMsgSize);
    if (!pMsg)
    {
        return NULL;
    }
    pMsg->m_idProcess	= idProcess;
    pMsg->m_idSocket	= SOCKET_NONE;
    pMsg->m_idNpc		= idNpc;
    pMsg->m_unMsgType	= idMsg;
    pMsg->m_unMsgSize	= dwMsgSize;
    pMsg->m_nTransData	= nTrans;
    if (!pMsg->CNetMsg::Create((char*)pbufMsg, dwMsgSize))
    {
        SAFE_DELETE(pMsg);
        return NULL;
    }
    else
    {
#ifdef _MYDEBUG
        ::LogMsg("\n--Receive Msg:%u, Size:%u", pMsg->GetType(), pMsg->GetSize());
#endif
        return pMsg;
    }
}

CNetMsg* CNetMsg::CreateMsg(OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize)
{
    // make it...
    CNetMsg* pMsg	= NULL;
    switch(idMsg)
    {
    case _MSG_TALK:
        pMsg	= new CMsgTalk;
        break;
    case _MSG_MESSAGEBOARD:
        pMsg	= new CMsgMessageBoard;
        break;
    case _MSG_ACTION:
        pMsg	= new CMsgAction;
        break;
    case _MSG_USERINFO:
        pMsg	= new CMsgUserInfo;
        break;
    case _MSG_PLAYER:
        pMsg	= new CMsgPlayer;
        break;
    case _MSG_TICK:
        pMsg	= new CMsgTick;
        break;
    case _MSG_ITEMINFO:
        pMsg	= new CMsgItemInfo;
        break;
    case _MSG_ITEM:
        pMsg	= new CMsgItem;
        break;
    case _MSG_FRIEND:
        pMsg	= new CMsgFriend;
        break;
    case _MSG_INTERACT:
        pMsg	= new CMsgInteract;
        break;
    case _MSG_USERATTRIB:
        pMsg	= new CMsgUserAttrib;
        break;
    case _MSG_NAME:
        pMsg	= new CMsgName;
        break;
    case _MSG_AINPCINFO:
        pMsg	= new CMsgMonsterInfo;
        break;
    case _MSG_WALK:
        pMsg	= new CMsgWalk;
        break;
    case _MSG_WALKEX:
        pMsg	= new CMsgWalkEx;
        break;
    case _MSG_MAPITEM:
        pMsg	= new CMsgMapItem;
        break;
    case _MSG_TRADE:
        pMsg	= new CMsgTrade;
        break;
        /*case _MSG_BATTLESYSTEM:
        	pMsg	= new CMsgBattleSystem;
        	break;*/
    case _MSG_NPC:
        pMsg	= new CMsgNpc;
        break;
    case _MSG_NPCINFO:
        pMsg	= new CMsgNpcInfo;
        break;
    case _MSG_PACKAGE:
        pMsg	= new CMsgPackage;
        break;
    case _MSG_DIALOG:
        pMsg	= new CMsgDialog;
        break;
    case _MSG_ALLOT:
        pMsg	= new CMsgAllot;
        break;
    case _MSG_TEAM:
        pMsg	= new CMsgTeam;
        break;
    case _MSG_TEAMMEMBER:
        pMsg	= new CMsgTeamMember;
        break;
    case _MSG_MAGICINFO:
        pMsg	= new CMsgMagicInfo;
        break;
    case _MSG_FLUSHEXP:
        pMsg	= new CMsgFlushExp;
        break;
    case _MSG_WEAPONSKILL:
        pMsg	= new CMsgWeaponSkill;
        break;
    case _MSG_MAGICEFFECT:
        pMsg	= new CMsgMagicEffect;
        break;
    case _MSG_SYNATTRINFO:
        pMsg	= new CMsgSynAttrInfo;
        break;
    case _MSG_SYNDICATE:
        pMsg	= new CMsgSyndicate;
        break;
    case _MSG_FRIENDINFO:
        pMsg	= new CMsgFriendInfo;
        break;
    case _MSG_WEATHER:
        pMsg	= new CMsgWeather;
        break;
    case _MSG_GEMEMBED:
        pMsg	= new CMsgGemEmbed;
        break;
    case _MSG_ITEMINFOEX:
        pMsg	= new CMsgItemInfoEx;
        break;
    case _MSG_NPCINFOEX:
        pMsg	= new CMsgNpcInfoEx;
        break;
    case _MSG_MAPINFO:
        pMsg	= new CMsgMapInfo;
        break;
    case _MSG_SYNMEMBERINFO:
        pMsg	= new CMsgSynMemberInfo;
        break;
    case _MSG_DICE:
        pMsg	= new CMsgBODice;
        break;
    case _MSG_PETINFO:
        pMsg	= new CMsgCallPetInfo;
        break;
    case _MSG_DATAARRAY:
        pMsg	= new CMsgDataArray;
        break;
    case _MSG_SYNINFO:
        pMsg	= new CMsgSynInfo;
        break;
    case _MSG_EUDEMONATTRIB:
        pMsg	= new CMsgEudemonAttrib;
        break;
    case _MSG_SCHOOLMEMBER:
        pMsg	= new CMsgSchoolMember;
        break;
    case _MSG_PLAYERTASK:
        pMsg	= new CMsgPlayerTask;
        break;
    case _MSG_ANNOUNCE_INFO:
        pMsg    = new CMsgAnnounceInfo;
        break;
    case _MSG_ANNOUNCE_LIST:
        pMsg    = new CMsgAnnounceList;
        break;
    case _MSG_AUCTION:
        pMsg    = new CMsgAuction;
        break;
        //	case _MSG_CHATROOM:
        //		pMsg	= new CMsgChatRoom;
        //		break;
    case _MSG_ITEMATTRIB:
        pMsg	= new CMsgItemAttrib;
        break;
        //#endif
        /////////
    default:
        break;
    }
    return pMsg;
}
