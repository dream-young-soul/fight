
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Announce.cpp: implementation of the CAnnounce class.
//
//////////////////////////////////////////////////////////////////////
//#include "mapgroup.h"
#include "Announce.h"
#include "I_Role.h"
#include "AllMsg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAnnounce::CAnnounce()
{
}
CAnnounce::~CAnnounce()
{
    ANNOUNCEDATA_SET::iterator it = m_setData.begin();
    for (; it != m_setData.end(); it++)
    {
        CAnnounceData* pData = *it;
        SAFE_RELEASE (pData);
    }
    m_setData.clear();
}
bool CAnnounce::Create(IDatabase* pDb, OBJID id)
{
    if (QueryAnnounceDataByOwner(id))
    {
        return true;
    }
    CHECKF (pDb);
    SQLBUF	szSQL;
    DEBUG_TRY
    sprintf(szSQL, "SELECT * FROM %s WHERE user_id = '%ld' LIMIT 1", _TBL_ANNOUNCE, id);
    IRecordset* pRes = pDb->CreateNewRecordset(szSQL);
    if (pRes)
    {
        CAnnounceData* pData = CAnnounceData::CreateNew();
        if (pData && pData->Create(pRes, pDb))
        {
            m_setData.push_back(pData);
        }
        else
        {
            SAFE_RELEASE (pData);
            SAFE_RELEASE (pRes);
            return false;
        }
        SAFE_RELEASE (pRes);
    }
    DEBUG_CATCH("CAnnounce::Create(IDatabase* pDb,OBJID id)");
    return true;
}
bool CAnnounce::Create(IDatabase* pDb)
{
    CHECKF (pDb);
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT * FROM %s", _TBL_ANNOUNCE);
    IRecordset* pRes = pDb->CreateNewRecordset(szSQL);
    if (pRes)
    {
        for (int i = 0; i < pRes->RecordCount(); i++)
        {
            CAnnounceData* pData = CAnnounceData::CreateNew();
            if (pData && pData->Create(pRes, pDb))
            {
                m_setData.push_back(pData);
            }
            else
            {
                SAFE_RELEASE (pData);
                SAFE_RELEASE (pRes);
                return false;
            }
            pRes->MoveNext();
        }
        SAFE_RELEASE (pRes);
    }
    return true;
}
bool	CAnnounce::CreateNewAnnounce(const ST_ANNOUNCE_DATA* pInfo, IDatabase* pDb)
{
    CAnnounceData* pData = CAnnounceData::CreateNew();
    if (pData)
    {
        if (pData->CreateRecord(pInfo, pDb))
        {
            m_setData.push_back(pData);
            return true;
        }
        SAFE_RELEASE (pData);
    }
    return false;
}
bool	CAnnounce::DeleteAnnounceByObjID(OBJID idAnnounce)
{
    ANNOUNCEDATA_SET::iterator it = m_setData.begin();
    for (; it != m_setData.end(); it++)
    {
        CAnnounceData* pData = *it;
        if (pData->GetID() == idAnnounce)
        {
            if (!pData->DeleteRecord())
            {
                return false;
            }
            m_setData.erase(it);
            SAFE_RELEASE (pData);
            return true;
        }
    }
    return false;
}
bool    CAnnounce::DeleteAnnounceByUserID(OBJID id, bool isDelete)
{
    ANNOUNCEDATA_SET::iterator it = m_setData.begin();
    for (; it != m_setData.end(); it++)
    {
        CAnnounceData* pData = *it;
        if (pData && pData->GetInt(ANNOUNCEDATA_USER_ID) == id)
        {
            if (isDelete)
                if (!pData->DeleteRecord())
                {
                    return false;
                }
            m_setData.erase(it);
            SAFE_RELEASE (pData);
            return true;
        }
    }
    return false;
}
CAnnounceData*	CAnnounce::QueryAnnounceDataByIndex(int nIdx)
{
    if (nIdx >= m_setData.size())
    {
        return NULL;
    }
    return m_setData[nIdx];
}
CAnnounceData*	CAnnounce::QueryAnnounceDataByID(OBJID idAnnounce)
{
    ANNOUNCEDATA_SET::iterator it = m_setData.begin();
    for (; it != m_setData.end(); it++)
    {
        CAnnounceData* pData = *it;
        if (pData && pData->GetID() == idAnnounce)
        {
            return pData;
        }
    }
    return NULL;
}
CAnnounceData*	CAnnounce::QueryAnnounceDataByOwner(OBJID idUser)
{
    ANNOUNCEDATA_SET::iterator it = m_setData.begin();
    for (; it != m_setData.end(); it++)
    {
        CAnnounceData* pData = *it;
        if (pData && pData->GetInt(ANNOUNCEDATA_USER_ID) == idUser)
        {
            return pData;
        }
    }
    return NULL;
}
bool	CAnnounce::SendAnnounceList(IRole* pRole, UCHAR usType, int nIndexBegin)
{
    CHECKF (pRole);
    if (GetAnnounceAmount() < nIndexBegin)	// Òç³ö¼ì²é
    {
        return false;
    }
    CMsgAnnounceList	msg;
    if (!msg.Create(usType))
    {
        return false;
    }
    if (usType == CHECK_BY_INDEX)
    {
        int i = 0;
        for (; i < _MAX_ANNOUNCE_PER_PAGE; i++)
        {
            CAnnounceData* pData = this->QueryAnnounceDataByIndex(nIndexBegin + i);
            if (!pData)
            {
                break;
            }
            if (!msg.Append(pData->GetID(), pData->GetStr(ANNOUNCEDATA_USER_NAME)))
            {
                return false;
            }
        }
        if (i == 0)
        {
            pRole->SendSysMsg(STR_NOANY_ANNOUNCE);
        }
    }
    else if (usType == CHECK_BY_ID)
    {
        CAnnounceData* pData = this->QueryAnnounceDataByID(nIndexBegin);
        if (!pData || !msg.Append(pData->GetID(), pData->GetStr(ANNOUNCEDATA_USER_NAME)))
        {
            return false;
        }
    }
    else if (usType == CHECK_BY_OWNER)
    {
        for(int i = 0; i < nIndexBegin ; i++)
        {
            CAnnounceData* pData = this->QueryAnnounceDataByIndex(nIndexBegin + i);
            if (!pData)
            {
                break;
            }
            if (!msg.Append(pData->GetID(), pData->GetStr(ANNOUNCEDATA_USER_NAME)))
            {
                return false;
            }
            break;
        }
    }
    return pRole->SendMsg(&msg);
}
bool    CAnnounce::SendAnnounceSelf(IRole* pRole/*, OBJID idAnnounce*/)
{
    CHECKF (pRole);
    CMsgAnnounceInfo	msg;
    CAnnounceData* pData = this->QueryAnnounceDataByOwner(pRole->GetID());
    if (!pData)
    {
        return false;
    }
    if (!msg.Create(pData, _ANNOUNCE_USER_INFO))
    {
        return false;
    }
    return pRole->SendMsg(&msg);
}
bool	CAnnounce::SendAnnounceInfo(IRole* pRole, OBJID idAnnounce)
{
    CHECKF (pRole);
    CMsgAnnounceInfo	msg;
    CAnnounceData* pData = this->QueryAnnounceDataByID(idAnnounce);
    if (!pData)
    {
        return false;
    }
    if (!msg.Create(pData, _ANNOUNCE_USER_INFO))
    {
        return false;
    }
    return pRole->SendMsg(&msg);
}
