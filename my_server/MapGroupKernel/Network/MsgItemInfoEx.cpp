
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgItemInfo.cpp: interface for the CMsgItemInfoEx class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "Item.h"
#pragma	warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgItemInfoEx::CMsgItemInfoEx()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, MAX_PACKETSIZE - sizeof(MSG_Info) + 1);
}

CMsgItemInfoEx::~CMsgItemInfoEx()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgItemInfoEx::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_ITEMINFOEX != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgItemInfoEx::Create(CItem* pItem, OBJID idOwner, int nCost, int nAction /*= ITEMINFOEX_BOOTH*/)
{
    CHECKF(pItem);
    ItemInfoStruct info;
    CHECKF(pItem->GetInfo(&info));
    // init
    this->Init();
    // fill info now
    m_pInfo->ucAction			= nAction;
    m_pInfo->id					= info.id;
    m_pInfo->dwType				= info.idType;
    m_pInfo->idOwner			= idOwner;
    m_pInfo->dwPrice			= nCost;
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
        m_pInfo->dwWarGhostExp		= info.dwWarGhostExp;
        m_pInfo->dwGemAtkType		= info.dwGemAtkType;
        m_pInfo->dwAvailabeTime		= info.dwAvailabeTime;
        if (pItem->IsEudemon())
        {
            m_StrPacker.AddString(pItem->GetStr(ITEMDATA_NAME));
        }
    }
    m_unMsgType	= _MSG_ITEMINFOEX;
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgItemInfoEx::Process(void* pInfo)
{
    ASSERT(!"CMsgItemInfoEx::Process()");
    return;
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgItemInfoEx, id:%u", m_info.id);
#endif
}
