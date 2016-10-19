
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// AnnounceData.cpp: implementation of the CAnnounceData class.
//
//////////////////////////////////////////////////////////////////////

#include "AnnounceData.h"
#include "MapGroup.h"
char	szAnnounceTable[]	= _TBL_ANNOUNCE;

MYHEAP_IMPLEMENTATION(CAnnounceData, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnnounceData::CAnnounceData(): m_pData(NULL)
{
}

CAnnounceData::~CAnnounceData()
{
    SAFE_RELEASE (m_pData);
}

//////////////////////////////////////////////////////////////////////
bool CAnnounceData::Create(OBJID idAnnounce, IDatabase* pDb)
{
    CHECKF (idAnnounce != ID_NONE);
    CHECKF (!m_pData);
    CHECKF (pDb);
    // load task data
    m_pData	= CGameMAnnounceData::CreateNew();
    IF_NOT (m_pData)
    return false;
    IF_NOT (m_pData->Create(idAnnounce, pDb))
    return false;
    // load prize items
    return true;//LoadItems(pDb);
}

//////////////////////////////////////////////////////////////////////
bool CAnnounceData::Create(IRecordset* pRes, IDatabase* pDb)
{
    CHECKF (!m_pData);
    CHECKF (pRes);
    // load task data
    m_pData	= CGameMAnnounceData::CreateNew();
    IF_NOT (m_pData)
    return false;
    IF_NOT (m_pData->Create(pRes))
    return false;
    return true;//LoadItems(pDb);
}

//////////////////////////////////////////////////////////////////////
bool CAnnounceData::CreateRecord(const ST_ANNOUNCE_DATA* pInfo, IDatabase* pDb)
{
    CHECKF (!m_pData);
    CHECKF (pInfo);
    CHECKF (pDb);
    m_pData = CGameMAnnounceData::CreateNew();
    IF_NOT (m_pData)
    return false;
    IF_NOT (m_pData->Create(pDb, ID_NONE))		// create default
    return false;
    // fill data
    m_pData->SetInt(ANNOUNCEDATA_USER_ID, pInfo->user_id);
    m_pData->SetStr(ANNOUNCEDATA_USER_NAME, pInfo->name, _MAX_NAMESIZE);
    m_pData->SetInt(ANNOUNCEDATA_LEVEL, pInfo->level);
    m_pData->SetInt(ANNOUNCEDATA_TEACHER_LEVEL, pInfo->teacherlevel);
    m_pData->SetInt(ANNOUNCEDATA_PROFESSION, pInfo->profession);
    m_pData->SetStr(ANNOUNCEDATA_TITLE, pInfo->title, 32);
    m_pData->SetStr(ANNOUNCEDATA_CONTENT, pInfo->content, 128);
    if (ID_NONE == m_pData->InsertRecord())	// insert record
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CAnnounceData::DeleteRecord()
{
    return m_pData->DeleteRecord();
}

