
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgItemInfo.cpp: interface for the CMsgItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "Item.h"
#include "NpcWorld.h"
#include "Agent.h"
#pragma	warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgItemInfo::CMsgItemInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgItemInfo::~CMsgItemInfo()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgItemInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_ITEMINFO != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgItemInfo::Create(CItem* pItem, int nAction /*= ITEMINFO_ADDITEM*/, OBJID idUser/*=ID_NONE*/)
{
    CHECKF(pItem);
    // init
    this->Init();
    // fill info now
    m_unMsgType	= _MSG_ITEMINFO;
    m_unMsgSize	= sizeof(MSG_Info) - 1;
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgItemInfo::Process(void* pInfo)
{
    CAgent* pAgent = NpcManager()->QueryAgent(GetNpcID());
    if (!pAgent)
    {
        return ;
    }
    switch(m_pInfo->ucAction)
    {
    case	ITEMINFO_ADDITEM:
        {
            ItemInfoStruct info;
            memset(&info, 0, sizeof(ItemInfoStruct));
            info[ITEMDATA_ID_]			= m_pInfo->id;
            info[ITEMDATA_TYPE]			= m_pInfo->dwType;
            info[ITEMDATA_AMOUNT]		= m_pInfo->usAmount;
            info[ITEMDATA_AMOUNTLIMIT]	= m_pInfo->usAmountLimit;
            info[ITEMDATA_IDENT]		= m_pInfo->ucIdent;
            info[ITEMDATA_POSITION]		= m_pInfo->ucPosition;
            info[ITEMDATA_GEM1]			= m_pInfo->ucGem1;
            info[ITEMDATA_GEM2]			= m_pInfo->ucGem2;
            info[ITEMDATA_MAGIC1]		= m_pInfo->ucMagic1;
            info[ITEMDATA_MAGIC2]		= m_pInfo->ucMagic2;
            info[ITEMDATA_MAGIC3]		= m_pInfo->ucMagic3;
            info[ITEMDATA_WARGHOSTEXP]	= m_pInfo->dwWarGhostExp;
            info[ITEMDATA_GEMTYPE]		= m_pInfo->dwGemAtkType;
            info[ITEMDATA_AVAILABLETIME] = m_pInfo->dwAvailabeTime;
            CItem* pItem = CItem::CreateNew();
            IF_OK(pItem)
            {
                IF_OK(pItem->Create(&info))
                {
                    if (m_pInfo->ucPosition == ITEMPOSITION_BACKPACK)
                    {
                        ASSERT(Cast<CItemPack>(pAgent)->AddItem(pItem));
                    }
                    else
                    {
                        ASSERT(Cast<CItemPack>(pAgent)->AddEquip(pItem));
                    }
                }
                else
                {
                    pItem->ReleaseByOwner();
                }
            }
        }
        break;
    case	ITEMINFO_UPDATE:
        {
            ItemInfoStruct info;
            memset(&info, 0, sizeof(ItemInfoStruct));
            info[ITEMDATA_ID_]			= m_pInfo->id;
            info[ITEMDATA_TYPE]			= m_pInfo->dwType;
            info[ITEMDATA_AMOUNT]		= m_pInfo->usAmount;
            info[ITEMDATA_AMOUNTLIMIT]	= m_pInfo->usAmountLimit;
            info[ITEMDATA_IDENT]		= m_pInfo->ucIdent;
            info[ITEMDATA_POSITION]		= m_pInfo->ucPosition;
            info[ITEMDATA_GEM1]			= m_pInfo->ucGem1;
            info[ITEMDATA_GEM2]			= m_pInfo->ucGem2;
            info[ITEMDATA_MAGIC1]		= m_pInfo->ucMagic1;
            info[ITEMDATA_MAGIC2]		= m_pInfo->ucMagic2;
            info[ITEMDATA_MAGIC3]		= m_pInfo->ucMagic3;
            info[ITEMDATA_WARGHOSTEXP]	= m_pInfo->dwWarGhostExp;
            info[ITEMDATA_GEMTYPE]		= m_pInfo->dwGemAtkType;
            info[ITEMDATA_AVAILABLETIME] = m_pInfo->dwAvailabeTime;
            OBJID idItem = m_pInfo->id;
            CItem* pItem = Cast<CItemPack>(pAgent)->FindItem(idItem, idItem);
            if (pItem)
            {
                ASSERT(pItem->UpdateInfo(&info));
            }
        }
        break;
    }
}
