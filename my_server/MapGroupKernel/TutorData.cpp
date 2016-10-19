
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TutorData.cpp: implementation of the CTutorData class.
//
//////////////////////////////////////////////////////////////////////

#include "TutorData.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTutorData::CTutorData()
{
    memset(&m_Info, 0L, sizeof(TutorInfoStruct));
    SetObjType(OBJ_TUTOR);
}

CTutorData::~CTutorData()
{
}

//////////////////////////////////////////////////////////////////////
bool CTutorData::Create(OBJID id, IDatabase* pDb)
{
    CHECKF(id);
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT * FROM %s WHERE id=%u LIMIT 1", _TBL_TUTOR, id);
    IRecordset* pRes = pDb->CreateNewRecordset(szSQL);
    if (pRes)
    {
        bool ret = LoadInfo(pRes);
        pRes->Release();
        return ret;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CTutorData::Create(IRecordset* pRes)
{
    CHECKF(pRes);
    IRecord* pRecord = pRes->CreateNewRecord();
    bool ret = LoadInfo(pRecord);
    pRecord->Release();
    return ret;
}

//////////////////////////////////////////////////////////////////////
bool CTutorData::LoadInfo(IRecord* pRes)
{
    if (!pRes)
    {
        return false;
    }
    // load again?
    if (m_Info.id != ID_NONE)
    {
        return true;
    }
    m_Info.id				= pRes->LoadDWord("id");
    m_Info.idUser			= pRes->LoadDWord("user_id");
    m_Info.idTutor			= pRes->LoadDWord("tutor_id");
    pRes->LoadString(m_Info.szUserName,	"user_name", _MAX_NAMESIZE);
    pRes->LoadString(m_Info.szTutorName, "tutor_name", _MAX_NAMESIZE);
    return true;
}

//////////////////////////////////////////////////////////////////////
OBJID CTutorData::CreateRecord(OBJID idUser,  OBJID idTutor, LPCTSTR szUserName, LPCTSTR szTutorName, IDatabase* pDb)
{
    CHECKF(szUserName && szTutorName && pDb);
    m_Info.idUser		= idUser;
    m_Info.idTutor		= idTutor;
    SafeCopy(m_Info.szUserName, szUserName, _MAX_NAMESIZE);
    SafeCopy(m_Info.szTutorName, szTutorName, _MAX_NAMESIZE);
    SQLBUF	szSQL;
    sprintf(szSQL, "INSERT %s SET user_id='%u', tutor_id='%u', user_name='%s', tutor_name='%s'",
            _TBL_TUTOR, idUser, idTutor, szUserName, szTutorName);
    pDb->ExecuteSQL(szSQL);
    m_Info.id	= pDb->GetInsertId();
    return m_Info.id;
}

//////////////////////////////////////////////////////////////////////
bool CTutorData::DeleteRecord(IDatabase* pDb)
{
    SQLBUF	szSQL;
    sprintf(szSQL, "DELETE FROM %s WHERE id='%u'", _TBL_TUTOR, m_Info.id);
    pDb->ExecuteSQL(szSQL);
    return false;
}
