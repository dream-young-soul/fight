
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NpcTable.cpp: implementation of the CNpcTable class.
//
//////////////////////////////////////////////////////////////////////

#include "NpcTable.h"
#include "MapGroup.h"
#include "Npc.h"

char	szNpcTableTable[]		= _TBL_TABLE;
MYHEAP_IMPLEMENTATION(CNpcTable, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNpcTable::CNpcTable()
{
}

//////////////////////////////////////////////////////////////////////
CNpcTable::~CNpcTable()
{
}

//////////////////////////////////////////////////////////////////////
bool CNpcTable::Create(PROCESS_ID idProcess, CNpc* pOwner, OBJID idNpc)
{
    m_pOwner	= pOwner;
    m_idProcess	= idProcess;
    m_setTable	= CTableSet::CreateNew();
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT * FROM %s WHERE npc_id=%u ORDER BY id LIMIT 100", _TBL_TABLE, idNpc);
    IF_NOT(m_setTable->Create(szSQL, false))
    return false;
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpcTable::AddRecord(int nType, OBJID idKey, int setData[4], LPCTSTR szData)
{
    if (GetRecord(nType, idKey))
    {
        return false;
    }
    CTableData* pData = CTableData::CreateNew();
    IF_NOT(pData && pData->Create(Database(), ID_NONE))		// ID_NONE: for new record
    {
        pData->Release();
        return false;
    }
    if (idKey)
    {
        CTableSet::Iterator pData = m_setTable->NewEnum();
        while(pData.Next())
        {
            if (pData->GetInt(TABLEDATA_KEY) == idKey)
            {
                m_setTable->DelObj(pData);
                break;
            }
        }
    }
    pData->SetInt(TABLEDATA_NPCID, m_pOwner->GetRecordID());
    pData->SetInt(TABLEDATA_TYPE, nType);
    pData->SetInt(TABLEDATA_KEY, idKey);
    //	pData->SetInt(TABLEDATA_DATE_STAMP, ::DateStamp());
    pData->SetStr(TABLEDATA_DATASTR, szData, MAX_FIELDSIZE);
    pData->SetInt(TABLEDATA_DATA0, setData[0]);
    pData->SetInt(TABLEDATA_DATA1, setData[1]);
    pData->SetInt(TABLEDATA_DATA2, setData[2]);
    pData->SetInt(TABLEDATA_DATA3, setData[3]);
    pData->InsertRecord();
    m_setTable->AddObj(pData);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpcTable::DelRecord(int nType, OBJID idKey, int setData[4], LPCTSTR szData)	// del it when match all no 0 param
{
    CHECKF(szData);
    int nDataLen = strlen(szData);
    CTableSet::Iterator pData = m_setTable->NewEnum();
    while(pData.Next())
    {
        if (nType && pData->GetInt(TABLEDATA_TYPE) != nType)
        {
            continue;
        }
        if (idKey && pData->GetInt(TABLEDATA_KEY) != idKey)
        {
            continue;
        }
        if (setData[0] && pData->GetInt(TABLEDATA_DATA0) != setData[0])
        {
            continue;
        }
        if (setData[1] && pData->GetInt(TABLEDATA_DATA1) != setData[1])
        {
            continue;
        }
        if (setData[2] && pData->GetInt(TABLEDATA_DATA2) != setData[2])
        {
            continue;
        }
        if (setData[3] && pData->GetInt(TABLEDATA_DATA3) != setData[3])
        {
            continue;
        }
        if (nDataLen && strcmp(pData->GetStr(TABLEDATA_DATASTR), szData) != 0)
        {
            continue;
        }
        m_setTable->DelObj(pData);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
CTableData* CNpcTable::GetRecord(int nType, OBJID idKey)
{
    CTableSet::Iterator pData = m_setTable->NewEnum();
    while(pData.Next())
    {
        if (pData->GetInt(TABLEDATA_TYPE) == nType && pData->GetInt(TABLEDATA_KEY) == idKey)
        {
            return pData;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CNpcTable::DelInvalid(int nType, int idxDateStamp)
{
    CHECKF(idxDateStamp >= 0 && idxDateStamp < TABLEDATA_SIZE);
    CTableSet::Iterator pData = m_setTable->NewEnum();
    while(pData.Next())
    {
        if (pData->GetInt(TABLEDATA_TYPE) == nType && pData->GetInt(TABLEDATA(TABLEDATA_DATA0 + idxDateStamp)) < ::DateStamp())
        {
            pData->DeleteRecord();
            m_setTable->DelObj(pData->GetID());
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpcTable::DelRecord(OBJID idRecord)
{
    CTableSet::Iterator pData = m_setTable->NewEnum();
    while(pData.Next())
    {
        if (pData->GetID() == idRecord)
        {
            m_setTable->DelObj(pData);
            return true;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpcTable::FindNext(int nType, int& nIter)			// order by ID, return false: no more syn
{
    OBJID	idLast	= nIter;
    OBJID	idNext	= ID_NONE;
    bool	nBreak		= false;
    CTableSet::Iterator pTarget = QuerySet()->NewEnum();
    while(pTarget.Next())
    {
        if (pTarget && pTarget->GetInt(TABLEDATA_TYPE) == nType)
        {
            if (idNext == ID_NONE)
            {
                idNext	= pTarget->GetID();
                if (nBreak)
                {
                    break;
                }
                else
                {
                    nBreak = true;
                }
            }
            if (pTarget->GetID() == idLast)
            {
                idNext	= ID_NONE;
                continue;
            }
        }
    }
    if (idNext)
    {
        nIter = idNext;
        return true;
    }
    return false;
}

