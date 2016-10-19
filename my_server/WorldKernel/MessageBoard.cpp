
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MessageBoard.cpp: implementation of the CMessageBoard class.
//
//////////////////////////////////////////////////////////////////////

#include "MessageBoard.h"

MYHEAP_IMPLEMENTATION(CMessageBoard, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMessageBoard::CMessageBoard(int nChannel, int nSize) : m_nChannel(nChannel), m_nSize(nSize)
{
}

//////////////////////////////////////////////////////////////////////
CMessageBoard::~CMessageBoard()
{
}

//////////////////////////////////////////////////////////////////////
bool CMessageBoard::AddMsg(OBJID id, LPCTSTR szName, LPCTSTR szMsg)
{
    if (m_setMsg.size() >= m_nSize)
    {
        m_setMsg.pop_back();
    }
    if (id != ID_NONE)
    {
        DelMsg(id);
    }
    MessageInfo	info;
    info.id		= id;
    SafeCopy(info.name, szName, _MAX_NAMESIZE);
    SafeCopy(info.words, szMsg, _MAX_WORDSSIZE);
    info.time	= time(NULL);
    m_setMsg.push_front(info);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMessageBoard::DelMsg(OBJID id)
{
    for(int i = 0; i < m_setMsg.size(); i++)
    {
        if (m_setMsg[i].id == id)
        {
            m_setMsg.erase(m_setMsg.begin() + i);
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
CMessageBoard::MessageInfo* CMessageBoard::GetMsgInfo(LPCTSTR szName)
{
    CHECKF(szName && strlen(szName));
    for(int i = 0; i < m_setMsg.size(); i++)
    {
        if (strcmp(m_setMsg[i].name, szName) == 0)
        {
            return &m_setMsg[i];
        }
    }
    return NULL;
}







