
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAnnounceList.cpp: implementation of the CMsgAnnounceList class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "MsgAnnounceList.h"
#include "mapgroup.h"
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

//////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////
BOOL CMsgAnnounceList::Create(USHORT usType, USHORT usAmount, ST_ANNOUNCE_TITLE* setAnnounce)
{
    if (usAmount > 0)
    {
        CHECKF (setAnnounce);
    }
    this->Init();
    m_unMsgType	= _MSG_ANNOUNCE_LIST;
    m_unMsgSize	= sizeof(MSG_Info) - sizeof(ST_ANNOUNCE_TITLE) + usAmount * sizeof(ST_ANNOUNCE_TITLE);
    ASSERT (m_unMsgSize <= MAX_PACKETSIZE);
    m_pInfo->usType			= usType;
    //	m_pInfo->user_id
    m_pInfo->usAmount		= usAmount;
    //if (usType == ANNOUNCELIST_TITLE)
    {
        for (int i = 0; i < usAmount; i++)
        {
            m_pInfo->setAnnounce[i].idAnnounce		= setAnnounce[i].idAnnounce;
            ::SafeCopy(m_pInfo->setAnnounce[i].szName, setAnnounce[i].szName, 32);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAnnounceList::Append(OBJID idAnnounce, const char* pszTitle, int type)
{
    CHECKF (pszTitle);
    m_pInfo->usAmount++;
    m_unMsgSize	= sizeof(MSG_Info) - sizeof(ST_ANNOUNCE_TITLE) + m_pInfo->usAmount * sizeof(ST_ANNOUNCE_TITLE);
    ASSERT (m_unMsgSize <= MAX_PACKETSIZE);
    m_pInfo->setAnnounce[m_pInfo->usAmount-1].idAnnounce	= idAnnounce;
    //m_pInfo->setAnnounce[m_pInfo->usAmount-1].ucReqLev	= ucReqLev;
    ::SafeCopy(m_pInfo->setAnnounce[m_pInfo->usAmount-1].szName, pszTitle, 32);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgAnnounceList::Process(void* pInfo)
{
#ifdef _MYDEBUG
    ::LogSave("Process CMsgAnnounceList: type=%u, amount=%u, index=%u",
              m_pInfo->usType, m_pInfo->usAmount, m_pInfo->dwIndexFirst);
#endif
    OBJID idUser = UserManager()->GetUser(this)->GetID();
    switch(m_pInfo->usType)
    {
    case CHECK_BY_INDEX:
        Announce()->SendAnnounceList(RoleManager()->QueryRole(idUser), CHECK_BY_INDEX, m_pInfo->nIndex);
        break;
    case CHECK_BY_ID:
        Announce()->SendAnnounceList(RoleManager()->QueryRole(idUser), CHECK_BY_ID, m_pInfo->idAnnounce);
        break;
    case CHECK_BY_OWNER:
        Announce()->SendAnnounceList(RoleManager()->QueryRole(idUser), CHECK_BY_OWNER, m_pInfo->usAmount);
        break;
    }
    //	ASSERT(!"CMsgAnnounceList::Process");
}
