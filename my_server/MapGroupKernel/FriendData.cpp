
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// FriendData.cpp: implementation of the CFriendData class.
//
//////////////////////////////////////////////////////////////////////

#include "FriendData.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFriendData::CFriendData()
{
    memset(&m_Info, 0, sizeof(FriendInfoStruct));
    SetObjType(OBJ_FRIEND);
}

//////////////////////////////////////////////////////////////////////
CFriendData::~CFriendData()
{
}

//////////////////////////////////////////////////////////////////////
bool CFriendData::Create(OBJID id, IDatabase* pDb)
{
    CHECKF(id);
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT * FROM %s WHERE id=%u LIMIT 1", _TBL_FRIEND, id);
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
bool CFriendData::Create(IRecordset* pRes)
{
    CHECKF(pRes);
    IRecord* pRecord = pRes->CreateNewRecord();
    bool ret = LoadInfo(pRecord);
    pRecord->Release();
    return ret;
}

//////////////////////////////////////////////////////////////////////
bool CFriendData::LoadInfo(IRecord* pRes)
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
    m_Info.idUser			= pRes->LoadDWord("userid");
    m_Info.idFriend			= pRes->LoadDWord("friend");
    pRes->LoadString(m_Info.szFriendName,	"friendname", _MAX_NAMESIZE);
    return true;
}

//////////////////////////////////////////////////////////////////////
OBJID CFriendData::CreateRecord(OBJID idUser, OBJID idFriend, LPCTSTR szFriendName, IDatabase* pDb)
{
    m_Info.idUser		= idUser;
    m_Info.idFriend		= idFriend;
    SafeCopy(m_Info.szFriendName, szFriendName, _MAX_NAMESIZE);
    SQLBUF	szSQL;
    sprintf(szSQL, "INSERT %s SET userid='%u',friend='%u',friendname='%s'", _TBL_FRIEND, idUser, idFriend, szFriendName);
    pDb->ExecuteSQL(szSQL);
    m_Info.id = pDb->GetInsertId();
    return m_Info.id;
}

//////////////////////////////////////////////////////////////////////
bool CFriendData::DeleteRecord(IDatabase* pDb)
{
    SQLBUF	szSQL;
    sprintf(szSQL, "DELETE FROM %s WHERE id='%u'", _TBL_FRIEND, m_Info.id);
    pDb->ExecuteSQL(szSQL);
    return false;
}










