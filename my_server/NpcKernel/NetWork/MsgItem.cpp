
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgItem.cpp: interface for the CMsgItem class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "MsgItem.h"
#include "NpcWorld.h"
#include "ItemType.h"
#include "Agent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgItem::CMsgItem()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgItem::~CMsgItem()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgItem::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_ITEM != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgItem::Create(OBJID id, int nAction, int nPosition /*= ITEMPOSITION_NONE*/)
{
    CHECKF(nAction);		// REPAIRALL 不需要 id &&
    // init
    this->Init();
    // fill info now
    m_pInfo->usAction	= nAction;
    m_pInfo->id			= id;
    m_pInfo->dwData		= nPosition;
    m_unMsgType	= _MSG_ITEM;
    m_unMsgSize	= sizeof(MSG_Info);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgItem::Process(void* pInfo)
{
    CAgent* pAgent = NpcManager()->QueryAgent(GetNpcID());
    if (!pAgent)
    {
        return;
    }
    CItemPack* pPack = Cast<CItemPack>(pAgent);
    CHECK(pPack);
    try
    {
        switch(m_pInfo->usAction)
        {
        case ITEMACT_EQUIP:
            {
                pPack->EquipItem(m_pInfo->id, m_pInfo->dwData);
            }
            break;
        case ITEMACT_UNEQUIP:
            {
                pPack->UnequipItem(m_pInfo->id, m_pInfo->dwData);
            }
            break;
        case ITEMACT_DROP:
            {
                pPack->DropItem(m_pInfo->id);
            }
            break;
        case ITEMACT_DROPEQUIPMENT:
            {
                pPack->DropEquipItem(m_pInfo->id, m_pInfo->dwData);
            }
            break;
        case ITEMACT_SYNCHRO_AMOUNT:
            {
                pPack->SetItemAmount(m_pInfo->id, m_pInfo->dwData);
            }
            break;
        case ITEMACT_QUERYMONEYSAVED:
            {
                //@ 仓库
            }
            break;
        case ITEMACT_REPAIRALL:
            {
                //@
            }
            break;
        case ITEMACT_DURABILITY:
            {
                //@
            }
            break;
        case ITEMACT_BOOTH_ADD:
            {
                //@
            }
            break;
        case ITEMACT_BOOTH_DEL:
            {
                //@
            }
            break;
        }
    }
    catch(...)
    {
        ::LogSave("switch(MSGITEM) Action [%d], id [%u], data [%u] ", m_pInfo->usAction, m_pInfo->id, m_pInfo->dwData);
    }
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgItem, id:%u", m_pInfo->id);
#endif
}


