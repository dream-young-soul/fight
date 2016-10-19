
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ConnectUser.cpp: implementation of the CConnectUser class.
//
//////////////////////////////////////////////////////////////////////
#include "define.h"
#include "windows.h"
#include "ConnectUser.h"
//using namespace world_kernel;

MYHEAP_IMPLEMENTATION(CConnectUser, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CConnectUser::CConnectUser()
{
}

//////////////////////////////////////////////////////////////////////
CConnectUser::~CConnectUser()
{
}

//////////////////////////////////////////////////////////////////////
bool CConnectUser::Create(OBJID id, DWORD dw, LPCTSTR szInfo, SOCKET_ID idSocket /*= SOCKET_NONE*/)
{
    m_idAccount		= id;
    m_dwCheckData	= dw;
    SafeCopy(m_szInfo, szInfo, _MAX_NAMESIZE);
    m_idSocket		= idSocket;
    m_tDelete.SetInterval(DELETECONNECTUSER_SECS);
    m_tDelete.Update();
    return true;
}


