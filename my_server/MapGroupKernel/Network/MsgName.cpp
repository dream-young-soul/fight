
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgName.cpp: implementation of the CMsgName class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "mapgroup.h"
#include "WantedList.h"
#include "PoliceWanted.h"
#include "BetOutside.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgName::CMsgName()
{
    Init();
    m_pInfo	= (MSG_Info* )m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgName::~CMsgName()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgName::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_NAME != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgName::Create(int nType, const char* pszName, DWORD dwData /*= 0*/)
{
    // param check
    if (nType == NAMEACT_NONE || !pszName)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_pInfo->ucType	= nType;
    m_pInfo->dwData	= dwData;
    m_StrPacker.AddString(pszName);
    m_unMsgType	= _MSG_NAME;
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgName::Create(int nType, const char* pszName, USHORT usPosX, USHORT usPosY)
{
    // param check
    if (nType == NAMEACT_NONE || !pszName)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_pInfo->ucType	= nType;
    m_pInfo->usPosX	= usPosX;
    m_pInfo->usPosY	= usPosY;
    m_StrPacker.AddString(pszName);
    m_unMsgType	= _MSG_NAME;
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgName::	AppendStr(const char* pszName)
{
    CHECKF(pszName);
    m_StrPacker.AddString(pszName);
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgName::Process(void* pInfo)
{
    CUserPtr pUser = UserManager()->GetUser(this);
    if (!pUser)
    {
        return;
    }
    char szName[_MAX_WORDSSIZE];
    m_StrPacker.GetString(0, szName, sizeof(szName));
    switch(m_pInfo->ucType)
    {
    case	NAMEACT_QUERY_WANTED:
        {
            CWantedData* pWanted = CWantedList::s_WantedList.GetWanted(pUser->WantedInfo().idWanted);
            if (pWanted)
            {
                CUser* pTarget = UserManager()->GetUser(pWanted->GetStr(DATA_TARGET_NAME));
                if (pTarget && pUser->FindAroundUser(pTarget->GetID()))
                {
                    pUser->SendSysMsg(_TXTATR_NORMAL, STR_FOND_WANTED, pTarget->GetName());
                }
            }
        }
        break;
    case	NAMEACT_QUERY_POLICEWANTED:
        {
            OBJID idWanted = pUser->PoliceWantedID();
            PoliceWantedStruct* pInfo = PoliceWanted().GetWanted(idWanted);
            if (pUser->FindAroundUser(idWanted) && pInfo)
            {
                pUser->SendSysMsg(_TXTATR_NORMAL, STR_FOND_POLICEWANTED, pInfo->strName.c_str());
            }
        }
        break;
    case	NAMEACT_FIREWORKS:
        {
            pUser->BroadcastRoomMsg(this, EXCLUDE_SELF);
        }
        break;
    case	NAMEACT_CREATE_SYN: 				// 改到TALK中
        {
            ASSERT(!"NAMEACT_CREATE_SYN");
        }
        break;
    case	NAMEACT_SYNDICATE: 				// 无法实现修改帮派名，仅下传
        {
            ASSERT(!"NAMEACT_SYNDICATE");
        }
        break;
    case	NAMEACT_CHANGE_TITLE:
        {
            ASSERT(!"NAMEACT_CHANGE_TITLE");
        }
        break;
    case	NAMEACT_DELROLE:						// 无意义
        {
            ASSERT(!"NAMEACT_DELROLE");
        }
        break;
    case	NAMEACT_MATE:
        {
            ASSERT(!"NAMEACT_MATE");
        }
        break;
    case	NAMEACT_QUERY_NPC:
        {
            OBJID idNpc = m_pInfo->idTarget;
            if (idNpc == ID_NONE)
            {
                return ;
            }
            CNpc* pNpc;
            IRole* pRole = RoleManager()->QuerySet()->GetObj(idNpc);
            if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)))
            {
                Create(NAMEACT_QUERY_NPC, pNpc->GetName(), idNpc);
                pUser->SendMsg(this);
            }
        }
        break;
    case NAMEACT_MEMBERLIST_SPECIFYSYN:
        {
            CSyndicate* pSyn = SynManager()->QuerySynByName(szName);
            CHECK(pSyn);
            OBJID idSyn = pSyn->GetID();
            CHECK(idSyn != ID_NONE);
            SetTransData(idSyn);
            MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
        }
        break;
    case	NAMEACT_MEMBERLIST:
        {
            OBJID idSyn = pUser->GetSynID();
            CHECK(idSyn != ID_NONE);
            SetTransData(idSyn);
            MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
        }
        break;
    case	NAMEACT_QUERY_MATE:
        {
            CUser* pTarget = UserManager()->GetUser(m_pInfo->idTarget);
            if (pTarget)
            {
                IF_OK(Create(NAMEACT_QUERY_MATE, pTarget->GetMate(), m_pInfo->idTarget))
                SendMsg(this);
            }
        }
        break;
    case	NAMEACT_ADDDICE_PLAYER:
        {
            IRole* pRole = pUser->FindAroundRole(m_pInfo->idTarget);
            CNpc* pNpc;
            if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsBetNpc())
            {
                pNpc->QueryBet()->JoinBet(pUser);
            }
        }
        break;
    case	NAMEACT_DELDICE_PLAYER:
        {
            CMsgName	msg;
            IF_OK(msg.Create(NAMEACT_DELDICE_PLAYER, pUser->GetName(), m_pInfo->idTarget))
            pUser->SendMsg(&msg);
            IRole* pRole = RoleManager()->QueryRole(m_pInfo->idTarget);
            CNpc* pNpc;
            if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsBetNpc())
            {
                pNpc->QueryBet()->LeaveBet(pUser);
            }
        }
        break;
    case	NAMEACT_CHANGE_EUDEMON_NAME:
        {
            if (m_StrPacker.GetStrAmount() < 1)
            {
                return;
            }
            // 只有幻兽才可以改名
            CItem* pItem = pUser->GetItem(m_pInfo->idTarget);
            if (!pItem || !pItem->IsEudemon())
            {
                return ;
            }
            // 召回幻兽
            //pUser->CallBackEudemon(pItem->GetID());
            // 检查名字合法性
            char szName[_MAX_NAMESIZE];
            if (!m_StrPacker.GetString(0, szName, _MAX_NAMESIZE))
            {
                return;
            }
            if (!NameStrCheck(szName))
            {
                pUser->SendSysMsg(STR_ERROR_ILLEGAL_NAME2);
                return;
            }
            // 成功改名
            pItem->SetStr(ITEMDATA_NAME, szName, _MAX_NAMESIZE, true);
            CMsgItemInfo	msg;
            if (msg.Create(pItem, ITEMINFO_UPDATE))
            {
                pUser->SendMsg(&msg);
            }
            // 如果幻兽召唤出来了，则更新幻兽资料
            CMonster* pEudemon = pUser->QueryEudemon(pItem->GetID());
            if (pEudemon)
            {
                CMsgPlayer	msg;
                if (msg.Create(pEudemon->QueryRole()))
                {
                    pEudemon->BroadcastRoomMsg(&msg, true);
                }
            }
        }
        break;
    }
}
