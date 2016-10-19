
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgItemInfo.cpp: interface for the CMsgItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "MapGroup.h"
#include "ItemType.h"
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
BOOL CMsgItemInfo::Create(CGameAuctionSystemData* pData, int nAction)
{
    // init
    CHECKF(pData);
    CItemTypeData* pItemInfo = NULL;
    pItemInfo = ItemType()->QueryItemType(pData->GetInt(AUCTION_SYSTEMITEM_TYPE));
    CHECKF(pItemInfo);
    this->Init();
    // fill info now
    m_pInfo->ucAction			= nAction;
    m_pInfo->id					= pData->GetID();
    m_pInfo->dwType				= pData->GetInt(AUCTION_SYSTEMITEM_TYPE);
    m_pInfo->ucIdent			= _ITEM_STATUS_NOT_IDENT;
    m_pInfo->usAmount			= pData->GetInt(AUCTION_SYSTEMITEM_AMOUNT);
    m_pInfo->usAmountLimit		= pData->GetInt(AUCTION_SYSTEMITEM_AMOUNTLIMIT);
    m_pInfo->ucIdent			= pData->GetInt(AUCTION_SYSTEMITEM_IDENT);
    m_pInfo->ucGem1				= pData->GetInt(AUCTION_SYSTEMITEM_GEM1);
    m_pInfo->ucGem2				= pData->GetInt(AUCTION_SYSTEMITEM_GEM2);
    m_pInfo->ucMagic1			= pData->GetInt(AUCTION_SYSTEMITEM_MAGIC1);
    m_pInfo->ucMagic2			= pData->GetInt(AUCTION_SYSTEMITEM_MAGIC2);
    m_pInfo->ucMagic3			= pData->GetInt(AUCTION_SYSTEMITEM_MAGIC3);
    m_pInfo->nData				= pData->GetInt(AUCTION_SYSTEMITEM_DATA);
    m_pInfo->dwWarGhostExp      = pData->GetInt(AUCTION_SYSTEMITEM_WARGHOSTEXP);
    m_pInfo->dwGemAtkType       = pData->GetInt(AUCTION_SYSTEMITEM_GEMTYPE);
    m_pInfo->dwAvailabeTime     = pData->GetInt(AUCTION_SYSTEMITEM_AVAILABLETIME);
    m_StrPacker.AddString(pItemInfo->GetStr(ITEMTYPEDATA_NAME));
    m_unMsgType	= _MSG_ITEMINFO;
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgItemInfo::Create(CItem* pItem, int nAction /*= ITEMINFO_ADDITEM*/, OBJID idUser/*=ID_NONE*/)
{
    CHECKF(pItem);
    ItemInfoStruct info;
    CHECKF(pItem->GetInfo(&info));
    // init
    this->Init();
    // fill info now
    m_pInfo->ucAction			= nAction;
    m_pInfo->id					= info.id;
    if (idUser != ID_NONE)
    {
        m_pInfo->id					= idUser;
    }
    m_pInfo->dwType				= info.idType;
    m_pInfo->ucPosition			= info.nPosition;
    m_pInfo->ucIdent			= _ITEM_STATUS_NOT_IDENT;
    if (!pItem->IsNeedIdent())
    {
        m_pInfo->usAmount			= info.nAmount;
        m_pInfo->usAmountLimit		= info.nAmountLimit;
        m_pInfo->ucIdent			= info.nIdent;
        m_pInfo->ucGem1				= info.nGem1;
        m_pInfo->ucGem2				= info.nGem2;
        m_pInfo->ucMagic1			= info.nMagic1;
        m_pInfo->ucMagic2			= info.nMagic2;
        m_pInfo->ucMagic3			= info.nMagic3;
        m_pInfo->nData				= info.nData;
        //---jinggy---2004-11-19---begin
        m_pInfo->dwWarGhostExp = info.dwWarGhostExp;
        m_pInfo->dwGemAtkType = info.dwGemAtkType;
        m_pInfo->dwAvailabeTime = info.dwAvailabeTime;
        //---jinggy---2004-11-19---end
    }
    else
    {
        m_pInfo->dwType					= CItem::HideTypeUnident(m_pInfo->dwType);
    }
    // eudemon
    if (pItem->IsEudemon())
    {
        m_StrPacker.AddString(pItem->GetStr(ITEMDATA_NAME));
    }
    m_unMsgType	= _MSG_ITEMINFO;
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgItemInfo::Process(void* pInfo)
{
    ASSERT(!"CMsgItemInfo::Process()");
    return;
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgItemInfo, id:%u", m_info.id);
#endif
}
