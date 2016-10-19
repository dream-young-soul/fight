
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAnnounceInfo.cpp: implementation of the CMsgAnnounceInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgAnnounceInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAnnounceInfo::CMsgAnnounceInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgAnnounceInfo::~CMsgAnnounceInfo()
{
}

BOOL CMsgAnnounceInfo::Create(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_ANNOUNCE_INFO != this->GetType())
    {
        return false;
    }
    return true;
}

BOOL CMsgAnnounceInfo::UpAnnounce(const char* szTitle, const char* szDesc)
{
    this->Init();
    m_StrPacker.AddString(szTitle);
    m_StrPacker.AddString(szDesc);
    m_pInfo->usType = _ANNOUNCE_ISSUE;
    m_pInfo->unMsgSize = sizeof(MSG_Info) + m_StrPacker.GetSize();
    m_pInfo->unMsgType = _MSG_ANNOUNCE_INFO;
    this->Send();
    return TRUE;
}

BOOL CMsgAnnounceInfo::QueryMyAnnounceInfo()
{
    this->Init();
    m_pInfo->usType = _ANNOUNCE_SLEF_CHECK;
    m_pInfo->user_id = g_objHero.GetID();
    m_pInfo->unMsgSize = sizeof(MSG_Info);
    m_pInfo->unMsgType = _MSG_ANNOUNCE_INFO;
    this->Send();
    return TRUE;
}

BOOL CMsgAnnounceInfo::QueryAnnounceInfo(OBJID idAnnounce)
{
    this->Init();
    m_pInfo->usType = _ANNOUNCE_CHECK;
    m_pInfo->idAnnounce = idAnnounce;
    m_pInfo->unMsgSize = sizeof(MSG_Info);
    m_pInfo->unMsgType = _MSG_ANNOUNCE_INFO;
    this->Send();
    return TRUE;
}

BOOL CMsgAnnounceInfo::CancelAnnounce(OBJID idAnnounce)
{
    this->Init();
    m_pInfo->usType = _ANNOUNCE_CANCEL;
    m_pInfo->idAnnounce = idAnnounce;
    m_pInfo->unMsgSize = sizeof(MSG_Info);
    m_pInfo->unMsgType = _MSG_ANNOUNCE_INFO;
    this->Send();
    return TRUE;
}

void CMsgAnnounceInfo::Process(void* pInfo)
{
    switch(m_pInfo->usType)
    {
    case _ANNOUNCE_SLEF_CHECK:
        break;
    case _ANNOUNCE_CANCEL:
        break;
    case _ANNOUNCE_CHECK:
        break;
    case _ANNOUNCE_USER_INFO:
        {
            char szName[16];
            char szTitle[32];
            char szStr[128];
            m_StrPacker.GetString(0, szName, 16);
            g_objHero.m_infoAnnounce.szName = szName;
            m_StrPacker.GetString(1, szTitle, 32);
            g_objHero.m_infoAnnounce.szTitle = szTitle;
            m_StrPacker.GetString(2, szStr, 128);
            g_objHero.m_infoAnnounce.szDesc = szStr;
            g_objHero.m_infoAnnounce.usType = m_pInfo->usType;
            g_objHero.m_infoAnnounce.idAnnounce = m_pInfo->idAnnounce;
            g_objHero.m_infoAnnounce.user_id = m_pInfo->user_id;
            g_objHero.m_infoAnnounce.level = m_pInfo->level;
            g_objHero.m_infoAnnounce.teacher_level = m_pInfo->teacher_level;
            g_objHero.m_infoAnnounce.profession = m_pInfo->profession;
            ::PostCmd(CMD_FLASH_ANNOUNCEINFO);
        }
        break;
    default:
        break;
    }
}

