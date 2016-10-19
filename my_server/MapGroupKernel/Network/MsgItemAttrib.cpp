
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgItemAttrib.cpp: implementation of the CMsgItemAttrib class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgItemAttrib::CMsgItemAttrib()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgItemAttrib::~CMsgItemAttrib()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgItemAttrib::Create(OBJID idItem, int nType, DWORD dwData)
{
    CHECKF(nType == _ITEMATTRIB_NONE || nType >= 0);
    this->Init();
    m_unMsgSize		= sizeof(MSG_Info) - sizeof(ST_ITEMATTRIB);
    m_unMsgType		= _MSG_ITEMATTRIB;
    m_pInfo->idItem	= idItem;
    m_pInfo->dwAttributeNum	= 0;
    if (nType >= 0)
    {
        m_unMsgSize	+= sizeof(ST_ITEMATTRIB);
        m_pInfo->setItemAttrib[m_pInfo->dwAttributeNum].ucAttributeType	= nType;
        m_pInfo->setItemAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= dwData;
        ++m_pInfo->dwAttributeNum;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgItemAttrib::Append(int nType, DWORD dwData)
{
    CHECKF (nType >= 0);
    CHECKF (m_unMsgSize + sizeof(ST_ITEMATTRIB) < MAX_PACKETSIZE);
    m_unMsgSize	+= sizeof(ST_ITEMATTRIB);
    m_pInfo->setItemAttrib[m_pInfo->dwAttributeNum].ucAttributeType	= nType;
    m_pInfo->setItemAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= dwData;
    ++m_pInfo->dwAttributeNum;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgItemAttrib::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_ITEMATTRIB != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgItemAttrib::Process(CGameSocket* pSocket)
{
    ::LogSave("invalid msg CMsgUserAttrib.");
}
