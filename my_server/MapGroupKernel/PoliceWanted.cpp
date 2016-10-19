
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// PoliceWanted.cpp: implementation of the CPoliceWanted class.
//
//////////////////////////////////////////////////////////////////////
#include <afxmt.h>
#define	LOCKOBJ		CSingleLock xLock(&s_xCtrl, true)

#include <WinSock2.h>
#include "AllMsg.h"
#include "User.h"
#include "MapGroup.h"
#include "PoliceWanted.h"

// static
CCriticalSection	CPoliceWanted::s_xCtrl;
CPoliceWanted		CPoliceWanted::s_objPoliceWanted;

CPoliceWanted& PoliceWanted(void) { return CPoliceWanted::s_objPoliceWanted; }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPoliceWanted::CPoliceWanted()
{
}

CPoliceWanted::~CPoliceWanted()
{
}

//////////////////////////////////////////////////////////////////////
bool	CPoliceWanted::AddWanted	(CUser* pUser)
{
    IF_NOT (pUser)
    return false;
    // repeat check
    if (this->GetWanted(pUser->GetID()))
    {
        return true;
    }
    // fill info
    PoliceWantedStruct info;
    info.idUser		= pUser->GetID();
    info.strName	= pUser->GetName();
    info.nPk		= pUser->GetPk();
    info.nLev		= pUser->GetLev();
    CSyndicate* pSyn = pUser->GetSyndicate();
    if (pSyn)
    {
        info.strSynName = pSyn->GetStr(SYNDATA_NAME);
    }
    else
    {
        info.strSynName = "NULL";
    }
    // add info with decrease order
    LOCKOBJ;
    int nSize = m_setWanted.size();
    int i = 0;
    for (; i < nSize; i++)
    {
        if (m_setWanted[i].nLev < info.nLev)
        {
            m_setWanted.insert(m_setWanted.begin() + i, info);
            break;
        }
    }
    if (i >= nSize)
    {
        m_setWanted.push_back(info);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool	CPoliceWanted::DelWanted	(OBJID idUser)
{
    LOCKOBJ;
    IF_NOT (ID_NONE != idUser)
    return false;
    int nSize = m_setWanted.size();
    for (int i = nSize - 1; i >= 0; i--)
    {
        if (m_setWanted[i].idUser == idUser)
        {
            m_setWanted.erase(m_setWanted.begin() + i);
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
PoliceWantedStruct*	CPoliceWanted::GetWantedByIndex	(int idx)
{
    LOCKOBJ;
    if (!(idx >= 0 && idx < m_setWanted.size()))
    {
        return NULL;
    }
    else
    {
        return &m_setWanted[idx];
    }
}

//////////////////////////////////////////////////////////////////////
PoliceWantedStruct*	CPoliceWanted::GetWanted(OBJID idUser)
{
    LOCKOBJ;
    if (ID_NONE == idUser)
    {
        return NULL;
    }
    int nSize = m_setWanted.size();
    for (int i = nSize - 1; i >= 0; i--)
    {
        if (m_setWanted[i].idUser == idUser)
        {
            return &m_setWanted[i];
        }
    }
    return NULL;
}
