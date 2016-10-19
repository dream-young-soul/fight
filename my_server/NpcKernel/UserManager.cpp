
//**********************************************************
// 代码编辑器
//**********************************************************

// UserManager.cpp: implementation of the CUserManager class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "UserManager.h"
#include "User.h"
#include "GameMap.h"
#include "NpcWorld.h"
#include "iniFIle.h"

CUserManager::SIZEADD_SET		CUserManager::m_setSizeAdd;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUserManager::CUserManager()
{
    m_pUserSet	= CUserSet::CreateNew(true);
}

//////////////////////////////////////////////////////////////////////
CUserManager::~CUserManager()
{
    if (m_pUserSet)
    {
        m_pUserSet->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CUserManager::Create()
{
    CHECKF(m_pUserSet);
    CIniFile	ini("ini\\AdditiveSize.ini", "Header");
    int	nAmount	= ini.GetInt("Amount");
    for(int i = 0; i < nAmount; i++)
    {
        char	key[256];
        sprintf(key, "Index%d", i);
        ini.SetSection("Header");
        int	nLook = ini.GetInt(key);
        if (nLook)
        {
            char	look[256];
            sprintf(look, "Transform%d", nLook);
            ini.SetSection(look);
            int nSizeAdd = ini.GetInt("AdditiveSize");
            m_setSizeAdd[nLook] = nSizeAdd;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUserManager::OnTimer(DWORD nCurr)
{
    for(int i = QuerySet()->GetAmount() - 1; i >= 0; i--)		// 倒排，可能删除对象
    {
        CUserPtr pUser = QuerySet()->GetObjByIndex(i);
        if (pUser)
        {
            pUser->OnTimer();    // 可能删除玩家
        }
    }
}

//////////////////////////////////////////////////////////////////////
IRole* CUserManager::QueryRole(OBJID idRole)
{
    if (IsSceneID(idRole))
    {
        return NULL;
    }
    else if (IsNpcID(idRole))
    {
        CNpcPtr pNpc = NpcManager()->QueryNpc(idRole);
        if (pNpc)
        {
            return pNpc->QueryRole();
        }
    }
    else
    {
        CUserPtr pUser = QueryUser(idRole); 	//@ 自动转换，不好
        if (pUser)
        {
            return pUser->QueryRole();
        }
        CNpcPtr pNpc = NpcManager()->QueryNpc(idRole);
        if (pNpc)
        {
            return pNpc->QueryRole();
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
int CUserManager::GetSizeAdd(int nLook)
{
    SIZEADD_SET::iterator	iter = m_setSizeAdd.find(nLook);
    if (iter != m_setSizeAdd.end())
    {
        return iter->second;
    }
    return 0;		// default size_add
}

//////////////////////////////////////////////////////////////////////
CUser* CUserManager::QueryUser(LPCTSTR szUser)
{
    CUserSet::Iterator	pUser = m_pUserSet->NewEnum();
    while(pUser.Next())
    {
        if (pUser && strcmp(pUser->GetName(), szUser) == 0)
        {
            return pUser;
        }
    }
    return NULL;
}






