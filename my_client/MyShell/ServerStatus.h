
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ServerStatus.h: interface for the CServerStatus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSTATUS_H__933A7B42_D161_4B31_A91B_214100F769F4__INCLUDED_)
#define AFX_SERVERSTATUS_H__933A7B42_D161_4B31_A91B_214100F769F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<vector>
using namespace std;

typedef struct
{
    char	szServerName[256];
    int		nServerStatus;
} ServerInfo;

enum {_SERVERSATUS_NONE = 0, _SERVERSATUS_RED = 1, _SERVERSATUS_GREEN = 2, _SERVERSATUS_YELLOW = 3};

class CServerStatus
{
public:
    CServerStatus();
    virtual ~CServerStatus();
private:
    vector<ServerInfo*> m_setServerInfo;
    void Destroy();
public:
    int  GetServerStatus(char* pszServerName);
    void FlashStatus();
};

#endif // !defined(AFX_SERVERSTATUS_H__933A7B42_D161_4B31_A91B_214100F769F4__INCLUDED_)
