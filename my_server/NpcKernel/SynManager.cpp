
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// SynManager.cpp: implementation of the CSynManager class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "SynManager.h"
#include "NpcWorld.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool CSyndicate::Create(IRecordset* pRes)
{
    m_id = pRes->GetInt(SYNDATA_ID);
    m_idFealty = pRes->GetInt(SYNDATA_FEALTY);
    for(int i = 0; i < MAX_SYNENEMYSIZE; i++)
    {
        OBJID id = pRes->GetInt((SYNDATA)(SYNDATA_ALLY0 + i));
        if (id != ID_NONE)
        {
            m_setAlly.push_back(id);
        }
    }
    for(int i = 0; i < MAX_SYNENEMYSIZE; i++)
    {
        OBJID id = pRes->GetInt((SYNDATA)(SYNDATA_ENEMY0 + i));
        if (id != ID_NONE)
        {
            m_setEnemy.push_back(id);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::IsFriendly(OBJID idSyn)
{
    if (idSyn == ID_NONE)
    {
        return false;
    }
    CSyndicate* pTarget = SynManager()->QuerySyndicate(idSyn);
    CHECKF(pTarget);
    OBJID idTarget = pTarget->GetMasterSynID();
    if (idTarget == GetMasterSynID())
    {
        return true;
    }
    if (find(m_setAlly.begin(), m_setAlly.end(), idTarget) != m_setAlly.end())
    {
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::IsHostile(OBJID idSyn)
{
    if (idSyn == ID_NONE)
    {
        return false;
    }
    CSyndicate* pTarget = SynManager()->QuerySyndicate(idSyn);
    CHECKF(pTarget);
    OBJID idTarget = pTarget->GetMasterSynID();
    if (find(m_setEnemy.begin(), m_setEnemy.end(), idTarget) != m_setEnemy.end())
    {
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicate::IsNeutral(OBJID idSyn)
{
    if (idSyn == ID_NONE)
    {
        return true;
    }
    if (!IsFriendly(idSyn) && !IsHostile(idSyn))
    {
        return true;
    }
    return false;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSynManager::CSynManager()
{
    m_setSyn = CSynSet::CreateNew(true);
}

//////////////////////////////////////////////////////////////////////
CSynManager::~CSynManager()
{
    if (m_setSyn)
    {
        m_setSyn->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CSynManager::Create()
{
    CHECKF(m_setSyn);
    char szSQL[1024];
    sprintf(szSQL, "SELECT * FROM %s WHERE del_flag=0", _TBL_SYNDICATE);
    IRecordset* pRes = NpcWorld()->GetDatabase()->CreateNewRecordset(szSQL);
    if (pRes)
    {
        for(int i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
        {
            CSyndicate* pSyn = CSyndicate::CreateNew();
            if (pSyn)
            {
                if (pSyn->Create(pRes))
                {
                    m_setSyn->AddObj(pSyn);
                }
                else
                {
                    pSyn->ReleaseByOwner();
                }
            }
        }
        pRes->Release();
    }
    return true;
}

















