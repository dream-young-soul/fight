
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// WantedList.cpp: implementation of the CWantedList class.
//
//////////////////////////////////////////////////////////////////////

#include <afxmt.h>
#define	LOCKOBJ		CSingleLock xLock(&s_xCtrl, true)

#include "WantedList.h"
#include "i_mydb.h"

CCriticalSection	CWantedList::s_xCtrl;
CWantedList CWantedList::s_WantedList;

char	szWantedTable[] = _TBL_WANTED;
MYHEAP_IMPLEMENTATION(CWantedList, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWantedList::CWantedList()
{
    m_pResDefault = NULL;
}

ULONG CWantedList::Release()
{
    if (m_pResDefault)
    {
        m_pResDefault->Release();
    }
    for(int i = 0; i < m_setWanted.size(); i++)
    {
        m_setWanted[i]->Release();
    }
    m_setWanted.clear();
    return 0;
}

//////////////////////////////////////////////////////////////////////
void CWantedList::InsertWanted	(CWantedData* pNewData)
{
    IF_NOT (pNewData)
    return;
    int nWantedAmount = m_setWanted.size();
    for (int i = 0; i < nWantedAmount; i++)
    {
        CWantedData* pData = m_setWanted[i];
        IF_OK (pData)
        {
            if (pNewData->GetInt(DATA_BOUNTY) >= pData->GetInt(DATA_BOUNTY))
            {
                m_setWanted.insert(m_setWanted.begin() + i, pNewData);
                return;
            }
        }
    }
    // go through all wanted already
    m_setWanted.push_back(pNewData);
}


//////////////////////////////////////////////////////////////////////
bool CWantedList::Create(IDatabase* pDb)
{
    IF_NOT (pDb)
    return false;
    char szSQL[1024];
    sprintf(szSQL, "SELECT * FROM %s", _TBL_WANTED);
    IRecordset*	pRes = pDb->CreateNewRecordset(szSQL);
    if (pRes)
    {
        for(int i = 0; i < pRes->RecordCount(); i++)
        {
            CWantedData* pData = CWantedData::CreateNew();
            if (!pData)
            {
                pRes->Release();
                return false;
            }
            if (!pData->Create(pRes))
            {
                pData->Release();
                pRes->Release();
                return false;
            }
            this->InsertWanted(pData);
            pRes->MoveNext();
        }
        pRes->Release();
    }
    sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_WANTED);
    m_pResDefault = pDb->CreateNewRecordset(szSQL, false);
    return (m_pResDefault != NULL);
}

//////////////////////////////////////////////////////////////////////
bool CWantedList::AddWanted (const char* pszTarget, const char* pszPayer, DWORD dwBonuty)
{
    LOCKOBJ;
    IF_NOT (pszTarget && pszPayer)
    return false;
    /*
    // repeat check
    for (int i=0; i<m_setWanted.size(); i++)
    {
    	CWantedData* pData = m_setWanted[i];
    	IF_NOT (pData)
    		continue;

    	if (0 == strcmp(pData->GetStr(DATA_TARGET_NAME), pszTarget)
    			|| 0 == strcmp(pData->GetStr(DATA_PAYER), pszPayer))
    	{
    		return false;
    	}
    }
    */
    // add new
    CWantedData* pNewData = CWantedData::CreateNew();
    IF_NOT (pNewData)
    return false;
    IF_NOT(pNewData->Create(m_pResDefault, ID_NONE))
    return false;
    // set data
    pNewData->SetStr(DATA_TARGET_NAME, pszTarget, _MAX_NAMESIZE);
    pNewData->SetStr(DATA_PAYER, pszPayer, _MAX_NAMESIZE);
    pNewData->SetInt(DATA_BOUNTY, dwBonuty);
    pNewData->SetInt(DATA_ORDER_TIME, ::SysTimeGetEx());
    if (0 == pNewData->InsertRecord())
    {
        pNewData->Release();
        return false;
    }
    // keep new record
    this->InsertWanted(pNewData);
    return true;
}

//////////////////////////////////////////////////////////////////////
CWantedData* CWantedList::GetWanted	(OBJID id)
{
    LOCKOBJ;
    if (ID_NONE == id)
    {
        return NULL;
    }
    int nSize = m_setWanted.size();
    for (int i = 0; i < nSize; i++)
    {
        CWantedData* pData = m_setWanted[i];
        if (pData && pData->GetInt(DATA_ID) == id)
        {
            return pData;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CWantedData* CWantedList::GetWantedByIndex	(int idx)
{
    LOCKOBJ;
    if (idx < 0 || idx >= m_setWanted.size())
    {
        return NULL;
    }
    return m_setWanted[idx];
}

//////////////////////////////////////////////////////////////////////
CWantedData* CWantedList::GetWantedByName	(const char* pszName)
{
    LOCKOBJ;
    if (!pszName)
    {
        return NULL;
    }
    int nSize = m_setWanted.size();
    for (int i = 0; i < nSize; i++)
    {
        CWantedData* pData = m_setWanted[i];
        IF_OK (pData)
        {
            if (0 == strcmp(pszName, pData->GetStr(DATA_TARGET_NAME)))
            {
                return pData;
            }
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CWantedList::DelWanted	(OBJID id)
{
    LOCKOBJ;
    if (ID_NONE == id)
    {
        return false;
    }
    int nSize = m_setWanted.size();
    for (int i = 0; i < nSize; i++)
    {
        CWantedData* pData = m_setWanted[i];
        if (pData && pData->GetInt(DATA_ID) == id)
        {
            pData->DeleteRecord();
            m_setWanted.erase(m_setWanted.begin() + i);
            return true;;
        }
    }
    return false;
}
