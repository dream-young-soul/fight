
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgMapItem.cpp: interface for the CMsgMapItem class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "MsgMapItem.h"
#include "MapGroup.h"
#include "MapItem.h"
#include "MapTrap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgMapItem::CMsgMapItem()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, MAX_PACKETSIZE - sizeof(MSG_Info) + 1);
}

CMsgMapItem::~CMsgMapItem()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMapItem::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_MAPITEM != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMapItem::Create(int nAction, CMapItem* pMapItem)
{
    CHECKF(pMapItem && nAction);
    // init
    this->Init();
    // fill info now
    m_pInfo->nAction	= nAction;
    m_pInfo->id			= pMapItem->GetID();
    m_pInfo->idType		= pMapItem->GetType();
    m_pInfo->nPosX		= pMapItem->GetPosX();
    m_pInfo->nPosY		= pMapItem->GetPosY();
    ItemInfoStruct* pInfo	= pMapItem->GetInfo();
    if (strlen(pInfo->szName) > 0)
    {
        m_StrPacker.AddString(pInfo->szName);
    }
    // hide id
    m_pInfo->idType		= CItem::HideTypeQuality(m_pInfo->idType);
    m_unMsgType	= _MSG_MAPITEM;
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMapItem::Create(int nAction, CMapTrap* pTrap)
{
    CHECKF(pTrap && nAction);
    // init
    this->Init();
    // fill info now
    m_pInfo->nAction	= nAction;
    m_pInfo->id			= pTrap->GetID();
    m_pInfo->usLook		= pTrap->GetLookFace();
    m_pInfo->nPosX		= pTrap->GetPosX();
    m_pInfo->nPosY		= pTrap->GetPosY();
    m_unMsgType	= _MSG_MAPITEM;
    m_unMsgSize	= sizeof(MSG_Info);
    return true;
}

/*/////////////////////////////////////////////////////////////////////
BOOL CMsgMapItem::Create(int nAction, OBJID idUser)
{
	// init
	this->Init();

	// fill info now
	m_pInfo->nAction	= nAction;
	m_pInfo->id			= idUser;

	m_unMsgType	=_MSG_MAPITEM;
	m_unMsgSize	=sizeof(MSG_Info);

	return true;
}*/

//////////////////////////////////////////////////////////////////////
BOOL CMsgMapItem::Create(OBJID id, int nPosX, int nPosY, int nAction)
{
    if (id == ID_NONE)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_pInfo->nAction	= nAction;
    m_pInfo->id			= id;
    m_pInfo->nPosX		= nPosX;
    m_pInfo->nPosY		= nPosY;
    m_unMsgType	= _MSG_MAPITEM;
    m_unMsgSize	= sizeof(MSG_Info);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgMapItem::Process(void* pInfo)
{
    CUserPtr pUser = UserManager()->GetUser(this);
    if (!pUser)
    {
        return;
    }
    if (!pUser->IsAlive())
    {
        pUser->SendSysMsg(STR_DIE);
        return ;
    }
    switch(m_pInfo->nAction)
    {
    case MSGMAPITEM_PICK:
        {
            if (pUser->SynPosition(m_pInfo->nPosX, m_pInfo->nPosY, 0))
            {
                pUser->PickMapItem(m_pInfo->id);
            }
            else
            {
                UserManager()->KickOutSocket(m_idSocket, "ITEM_PICK SynPosition");
            }
        }
        break;
    default:
        {
            ASSERT(!"switch");
        }
    }
    return;
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgMapItem, id:%u", m_pInfo->id);
#endif
}
