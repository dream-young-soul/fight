
//**********************************************************
// 代码编辑器
//**********************************************************

// MagicType.cpp: implementation of the CMagicType class.
//
//////////////////////////////////////////////////////////////////////

#include "MagicType.h"
#include "MapGroup.h"
#include "UserTable.h"

MYHEAP_IMPLEMENTATION(CMagicTypeData, s_heap)
//MYHEAP_IMPLEMENTATION(CMagicType, s_heap)


char	szMagicTypeTable[] = _TBL_MAGICTYPE;
//////////////////////////////////////////////////////////////////////
// MagicTypeData
bool CMagicTypeData::Create(IRecordset* pRes)
{
    IF_NOT (pRes)
    return false;
    m_pData = CGameMagicTypeData::CreateNew();
    IF_NOT (m_pData)
    return false;
    if (!m_pData->Create(pRes))
    {
        m_pData->Release();
        return false;
    }
    // 载入移动轨迹
    m_setTrackS.clear();
    m_setTrackT.clear();
    int nCount = 0;
    OBJID idTrack = m_pData->GetInt(MAGICTYPEDATA_TRACK_ID);
    while (idTrack != ID_NONE && (nCount++) < 50)		// 强制限制轨迹不得超过50步
    {
        CMagicTrackData* pTrack = MagicTrackSet()->GetObj(idTrack);
        if (!pTrack)
        {
            ::LogSave("Get track failed! [id=%u]", idTrack);
            break;
        }
        ST_TRACK track;
        track.ucDir = pTrack->GetInt(MAGICTRACKDATA_DIR);
        if (track.ucDir < 0 || track.ucDir > 7)
        {
            ::LogSave("Load an invalid track ! [id=%u, dir=%u]", idTrack, track.ucDir);
            break;
        }
        track.ucStep	= pTrack->GetInt(MAGICTRACKDATA_STEP);
        track.ucAlt		= pTrack->GetInt(MAGICTRACKDATA_ALT);
        track.idAction	= pTrack->GetInt(MAGICTRACKDATA_ACTION);
        track.nPower	= pTrack->GetInt(MAGICTRACKDATA_POWER);
        track.nApplyMS	= pTrack->GetInt(MAGICTRACKDATA_APPLY_MS);
        m_setTrackS.push_back(track);
        idTrack = pTrack->GetInt(MAGICTRACKDATA_ID_NEXT);
    }
    nCount = 0;
    idTrack = m_pData->GetInt(MAGICTYPEDATA_TRACK_ID2);
    while (idTrack != ID_NONE && (nCount++) < 50)		// 强制限制轨迹不得超过50步
    {
        CMagicTrackData* pTrack = MagicTrackSet()->GetObj(idTrack);
        if (!pTrack)
        {
            ::LogSave("Get track failed! [id=%u]", idTrack);
            break;
        }
        ST_TRACK track;
        track.ucDir = pTrack->GetInt(MAGICTRACKDATA_DIR);
        if (track.ucDir < 0 || track.ucDir > 7)
        {
            ::LogSave("Load an invalid track ! [id=%u, dir=%u]", idTrack, track.ucDir);
            break;
        }
        track.ucStep	= pTrack->GetInt(MAGICTRACKDATA_STEP);
        track.ucAlt		= pTrack->GetInt(MAGICTRACKDATA_ALT);
        track.idAction	= pTrack->GetInt(MAGICTRACKDATA_ACTION);
        track.nPower	= pTrack->GetInt(MAGICTRACKDATA_POWER);
        track.nApplyMS	= pTrack->GetInt(MAGICTRACKDATA_APPLY_MS);
        m_setTrackT.push_back(track);
        idTrack = pTrack->GetInt(MAGICTRACKDATA_ID_NEXT);
    }
    return true;
}


//////////////////////////////////////////////////////////////////////
// MagicType
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
CMagicType::CMagicType()
{
	m_setType	= NULL;
}

CMagicType::~CMagicType()
{
	if (m_setType)
		SAFE_RELEASE (m_setType);
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
bool CMagicType::Create(IDatabase* pDb)
{
	m_setType	= CMagicTypeSet::CreateNew(true);
	CHECKF(m_setType);

	char szSQL[1024];
	sprintf(szSQL, "SELECT * FROM %s", _TBL_MAGICTYPE);
	IF_NOT_(m_setType->Create(szSQL, pDb))
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
CMagicTypeData* CMagicType::QueryMagicType(OBJID idType)
{
	return m_setType->GetObj(idType);
}
*/
