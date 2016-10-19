
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgGemEmbed.cpp: implementation of the CMsgGemEmbed class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "User.h"
#include "MapGroup.h"

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
    switch(m_pInfo->usAction)
    {
    case GEM_EMBED:
        pUser->EmbedGem(m_pInfo->idItem, m_pInfo->idGem, m_pInfo->usPos);
        break;
    case GEM_TAKEOUT:
        pUser->TakeOutGem(m_pInfo->idItem, m_pInfo->usPos);
        break;
    default:
        break;
    }
}
