
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TxtMsgSet.cpp: implementation of the CTxtMsgSet class.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "windows.h"
#include "TxtMsgSet.h"

// const
const int _MAX_TXTMSGSIZE	= 256;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTxtMsgSet::CTxtMsgSet()
{
    m_dwMaxMsg	= 32;	// default max msg is 32
    m_dwMsgTake	= 0;
    this->ClearMsg();
}

CTxtMsgSet::~CTxtMsgSet(void)
{
    this->ClearMsg();
}

//--------------------------------------------------------------
BOOL CTxtMsgSet::AddMsg(const char* pszMsg)
{
    if (!pszMsg || _MAX_TXTMSGSIZE <= strlen(pszMsg))
    {
        return false;
    }
    char* pszNewMsg	= new char[_MAX_TXTMSGSIZE];
    if (!pszNewMsg)
    {
        return false;
    }
    strcpy(pszNewMsg, pszMsg);
    if (m_dwMaxMsg <= m_dequeMsg.size())
    {
        delete[] m_dequeMsg[0];
        m_dequeMsg.pop_front();
        if (m_dwMsgTake > 0)
        {
            m_dwMsgTake--;
        }
    }
    m_dequeMsg.push_back(pszNewMsg);
    return true;
}

//--------------------------------------------------------------
void CTxtMsgSet::ClearMsg(void)
{
    int nMsgAmount	= m_dequeMsg.size();
    for(int i = 0; i < nMsgAmount; i++)
    {
        delete [] m_dequeMsg[i];
    }
    m_dequeMsg.clear();
    m_dwMsgTake	= 0;
}

//--------------------------------------------------------------
const char* CTxtMsgSet::GetMsg(DWORD dwIndex) const
{
    if (dwIndex >= m_dequeMsg.size())
    {
        return NULL;
    }
    return m_dequeMsg[dwIndex];
}

//--------------------------------------------------------------
const char* CTxtMsgSet::GetLastMsg(void)
{
    if (m_dwMsgTake >= m_dequeMsg.size())
    {
        return NULL;
    }
    char* pLastMsg	= m_dequeMsg[m_dwMsgTake++];
    return pLastMsg;
}

//--------------------------------------------------------------
void CTxtMsgSet::SetMaxMsg(DWORD dwMaxMsg)
{
    DWORD dwCurMsgCount	= m_dequeMsg.size();
    if (dwMaxMsg < dwCurMsgCount)
    {
        for(UINT i = dwMaxMsg; i < dwCurMsgCount; i++)
        {
            delete[] m_dequeMsg[0];
            m_dequeMsg.pop_front();
            if (m_dwMsgTake > 0)
            {
                m_dwMsgTake--;
            }
        }
    }
    m_dwMaxMsg	= dwMaxMsg;
}
