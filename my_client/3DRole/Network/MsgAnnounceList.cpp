
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAnnounceList.cpp: implementation of the CMsgAnnounceList class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgAnnounceList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAnnounceList::CMsgAnnounceList()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgAnnounceList::~CMsgAnnounceList()
{
}

BOOL CMsgAnnounceList::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_ANNOUNCE_LIST != this->GetType())
    {
        return false;
    }
    return true;
}

BOOL CMsgAnnounceList::Create(USHORT usType, int nIndex, OBJID idAnnounce)
{
    this->Init();
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_ANNOUNCE_LIST;
    if (usType == CHECK_BY_ID)
    {
        m_pInfo->usType = usType;
    }
    else if (usType == CHECK_BY_INDEX)
    {
        m_pInfo->idAnnounce = nIndex;
    }
    return TRUE;
}

void CMsgAnnounceList::Process(void* pInfo)
{
    int nAmount = m_pInfo->usAmount;
    if ( 0 == nAmount)
    {
        return;
    }
    g_objHero.ClearAnnounceList();
    for(int i = 0; i < nAmount; i++)
    {
        g_objHero.AddAnnounceList(m_pInfo->setAnnounce[i].idAnnounce,
                                  m_pInfo->setAnnounce[i].szTitle);
    }
    ::PostCmd(CMD_FLASH_ANNOUNCELIST);
}
