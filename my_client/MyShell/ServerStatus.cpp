
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ServerStatus.cpp: implementation of the CServerStatus class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <afxinet.h>
#include "ServerStatus.h"
#include "BaseFunc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerStatus::CServerStatus()
{
}

CServerStatus::~CServerStatus()
{
    this->Destroy();
}

//-------------------------------------------------------------------
void CServerStatus::Destroy()
{
    int nAmount = m_setServerInfo.size();
    for(int i = 0; i < nAmount; i++)
    {
        ServerInfo* pInfo = m_setServerInfo[i];
        SAFE_DELETE(pInfo);
    }
    m_setServerInfo.clear();
}

//-------------------------------------------------------------------
int CServerStatus::GetServerStatus(char* pszServerName)
{
    if (!pszServerName)
    {
        return _SERVERSATUS_NONE;
    }
    int nAmount = m_setServerInfo.size();
    for(int i = 0; i < nAmount; i++)
    {
        ServerInfo* pInfo = m_setServerInfo[i];
        if (pInfo && strcmp(pszServerName, pInfo->szServerName) == 0)
        {
            return pInfo->nServerStatus;
        }
    }
    return _SERVERSATUS_NONE;
}

//-------------------------------------------------------------------
void CServerStatus::FlashStatus()
{
    /*	this->Destroy();
    	CInternetSession	session;
    	CInternetFile* file = NULL;

    	// get url ...
    	char szIniFile[] = "ini/common.ini";
    	char szTitle[] = "ServerStatus";
    	char szSubtitle[]= "URL";
    	char szUrl[1024];
    	if (!::IniStrGet(szIniFile, szTitle, szSubtitle, szUrl))
    		return;
    	try{file = (CInternetFile*)session.OpenURL(szUrl);}
    		catch(CInternetException* pEx){file = NULL; pEx->Delete();}
    	if (file)
    	{
    		while(true)
    		{
    			CString string;
    			if (!file->ReadString(string))
    				break;

    			char	szServerName[256] = "";
    			int		nServerStatus;
    			if (2 == sscanf(string, "%s %d", szServerName, &nServerStatus))
    			{
    				ServerInfo* pInfo = new ServerInfo;
    				if (pInfo)
    				{
    					pInfo->nServerStatus = nServerStatus;
    					strcpy(pInfo->szServerName, szServerName);
    					m_setServerInfo.push_back(pInfo);
    				}
    			}

    			string.Empty();
    		}
    		file->Close();
    		delete	file;
    	}*/
}
