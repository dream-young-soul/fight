
//**********************************************************
// 代码编辑器
//**********************************************************

// MagicData.cpp: implementation of the CTempMagicData class.
//
//////////////////////////////////////////////////////////////////////

#include "MagicData.h"
#include "BaseFunc.h"

MYHEAP_IMPLEMENTATION(CMagicTempData, s_heap)
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
int IMagicData::GetApplyMS()
{
    int nApplyMS = 0;
    if (this->GetSTrackAmount() > 0)
    {
        for (int i = 0; i < this->GetSTrackAmount(); i++)
        {
            const ST_TRACK&	track = this->GetSTrackByIndex(i);
            nApplyMS += track.nApplyMS;
        }
    }
    else
    {
        nApplyMS = GetInt(MAGICDATA_APPLY_MS);
    }
    return nApplyMS;
}

//////////////////////////////////////////////////////////////////////
int IMagicData::GetLockSecs()
{
    return (GetApplyMS() / 1000 + 1);
}

//////////////////////////////////////////////////////////////////////
void IMagicData::SetDelay()
{
    if (!m_pType)
    {
        FindType();
    }
    if (m_pType)
    {
        m_tDelay.SetInterval(m_pType->GetInt(MAGICTYPEDATA_DELAY));
    }
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMagicTempData::CMagicTempData()
{
    memset(&m_data, 0L, sizeof(ST_MAGIC_DATA));
}

//////////////////////////////////////////////////////////////////////
bool CMagicTempData::Create(IRecordset* pRecordset)
{
    ASSERT(!"CMagicTempData::Create(IRecordset* pRecordset)");
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CMagicTempData::Create(IRecordset* pDefaultRecord, OBJID idNewKey)
{
    ASSERT(!"CMagicTempData::Create(IRecordset* pDefaultRecord, OBJID idNewKey)");
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CMagicTempData::Create(ST_MAGIC_DATA* pMagicData)
{
    CHECKF (pMagicData);
    if (pMagicData->idMagic	= ID_NONE)
    {
        return false;
    }
    memcpy(&m_data, pMagicData, sizeof(ST_MAGIC_DATA));
    return true;
}

//////////////////////////////////////////////////////////////////////
int CMagicTempData::GetInt(MAGICDATA nDataName)
{
    if (nDataName < MAGICTYPEDATA_OFFSET)
    {
        switch (nDataName)
        {
        case MAGICDATA_ID_:
            return m_data.idMagic;
            break;
        case MAGICDATA_OWNERID:
            return m_data.idOwner;
            break;
        case MAGICDATA_TYPE:
            return m_data.usType;
            break;
        case MAGICDATA_LEVEL:
            return m_data.usLevel;
            break;
        case MAGICDATA_EXP:
            return m_data.dwExp;
            break;
        case MAGICDATA_UNLEARN:
            return m_data.ucUnlearn;
            break;
        case MAGICDATA_OLD_LEVEL:
            return m_data.usOldLevel;
            break;
        default:
            ::LogSave("Invalid MAGICDATA [%d]", nDataName);
            break;
        }
        return 0;
    }
    if (m_pType == NULL)
    {
        IF_NOT(FindType())
        {
            LOGERROR("魔法找不到类型。type[%d],level[%d]", GetInt(MAGICDATA_TYPE), GetInt(MAGICDATA_LEVEL));
            return 0;
        }
    }
    return m_pType->GetInt((MAGICTYPEDATA)(nDataName - MAGICTYPEDATA_OFFSET));
}

//////////////////////////////////////////////////////////////////////
void CMagicTempData::SetInt(MAGICDATA idx, int nData)
{
    ASSERT(idx < MAGICTYPEDATA_OFFSET);
    switch (idx)
    {
    case MAGICDATA_ID_:
        m_data.idMagic	= nData;
        break;
    case MAGICDATA_OWNERID:
        m_data.idOwner	= nData;
        break;
    case MAGICDATA_TYPE:
        m_data.usType	= nData;
        break;
    case MAGICDATA_LEVEL:
        m_data.usLevel	= nData;
        break;
    case MAGICDATA_EXP:
        m_data.dwExp	= nData;
        break;
    case MAGICDATA_UNLEARN:
        m_data.ucUnlearn	= nData;
        break;
    case MAGICDATA_OLD_LEVEL:
        m_data.usOldLevel	= nData;
        break;
    default:
        ::LogSave("Invalid MAGICDATA [%d]", idx);
        break;
    }
    if (idx == MAGICDATA_LEVEL)
    {
        CHECK(FindType());
    }
}

//////////////////////////////////////////////////////////////////
//bool CMagicTempData::FindType()
//{
//	return false;
//}

//////////////////////////////////////////////////////////////////////
