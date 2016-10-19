
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgGemEmbed.cpp: implementation of the CMsgGemEmbed class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgGemEmbed::CMsgGemEmbed()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgGemEmbed::~CMsgGemEmbed()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgGemEmbed::Create(OBJID idUser, OBJID idItem, OBJID idGem, int nPos, int nAction)
{
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_GEMEMBED;
    m_pInfo->idUser		= idUser;
    m_pInfo->idItem		= idItem;
    m_pInfo->idGem		= idGem;
    m_pInfo->usPos		= nPos;
    m_pInfo->usAction	= nAction;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgGemEmbed::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_GEMEMBED != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgGemEmbed::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogSave("Process MsgItem, idUser:%u",
              m_pInfo->idUser);
#endif
}
