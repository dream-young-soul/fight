
//**********************************************************
// 代码编辑器
//**********************************************************

// UserWeaponSkill.cpp: implementation of the CUserWeaponSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "UserWeaponSkill.h"
#include "I_mydb.h"
#include "Mapgroup.h"
#include "string.h"

//////////////////////////////////////////////////////////////////////
char	szWpSkillTable[] = _TBL_WEAPONSKILL;


//////////////////////////////////////////////////////////////////////
const char SQL_CREATERECORDSET[512]	= "INSERT INTO %s VALUES (%d, %d, %d, %d, %u, %u)";

//-------------------------------------
// static
MYHEAP_IMPLEMENTATION(CUserWeaponSkill, m_sHeap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserWeaponSkill::CUserWeaponSkill()
{
}

CUserWeaponSkill::~CUserWeaponSkill()
{
    if (m_pRes)
    {
        m_pRes->UpdateRecord();
        m_pRes->Release();
    }
}

//////////////////////////////////////////////////////////////////////
CUserWeaponSkill* CUserWeaponSkill::CreateNew(IRecordset* pRes)
{
    CHECKF(pRes);		// CHECKF相当于ASSERT，只是失败的时候会返回0。paled
    CUserWeaponSkill* pSkill	= new CUserWeaponSkill();
    if (!pSkill)
    {
        return NULL;
    }
    if (!pSkill->Create(pRes))
    {
        pSkill->ReleaseByOwner();
        return NULL;
    }
    return pSkill;
}

//////////////////////////////////////////////////////////////////////
CUserWeaponSkill* CUserWeaponSkill::CreateNew(UserWeaponSkillInfoStruct* pInfo, IRecordset* pDefault)
{
    CHECKF(pInfo);
    CUserWeaponSkill* pSkill	= new CUserWeaponSkill();
    if (!pSkill)
    {
        return NULL;
    }
    memcpy(&pSkill->m_Info, pInfo, sizeof(UserWeaponSkillInfoStruct));
    pSkill->m_pRes = pDefault->CreateNewRecord(pInfo->id);
    pSkill->m_pRes->SetDWord("type",		pInfo->nType);
    pSkill->m_pRes->SetDWord("level",		pInfo->ucLevel);
    pSkill->m_pRes->SetDWord("exp",			pInfo->nExp);
    pSkill->m_pRes->SetDWord("old_level",	pInfo->ucOldLev);
    pSkill->m_pRes->SetDWord("owner_id",	pInfo->idowner);
    pSkill->m_pRes->ClearUpdateFlags();
    return pSkill;
}

//////////////////////////////////////////////////////////////////////
CUserWeaponSkill* CUserWeaponSkill::InsertNewRecord(UserWeaponSkillInfoStruct* pInfo, IRecordset* pDefault, OBJID idNew)
{
    CHECKF(pInfo);
    CUserWeaponSkill* pSkill	= new CUserWeaponSkill();
    if (!pSkill)
    {
        return NULL;
    }
    if (idNew)
    {
        pInfo->id	= idNew;
    }
    pSkill->m_pRes = pDefault->CreateNewRecord(pInfo->id);
    pSkill->m_pRes->SetDWord("type",		pInfo->nType);
    pSkill->m_pRes->SetDWord("level",		pInfo->ucLevel);
    pSkill->m_pRes->SetDWord("exp",			pInfo->nExp);
    pSkill->m_pRes->SetDWord("owner_id",	pInfo->idowner);
    pSkill->m_pRes->SetDWord("unlearn",		false);
    if (idNew == ID_NONE)
    {
        pSkill->m_pRes->SetDWord("old_level",	pInfo->ucOldLev);
        pInfo->id = pSkill->m_pRes->InsertRecord();
    }
    else
    {
        pSkill->m_pRes->UpdateRecord();
    }
    memcpy(&pSkill->m_Info, pInfo, sizeof(UserWeaponSkillInfoStruct));
    return pSkill;
}

//////////////////////////////////////////////////////////////////////
BOOL CUserWeaponSkill::Create(IRecordset* pRes)
{
    CHECKF(pRes);
    m_pRes = pRes->CreateNewRecord();
    return this->LoadInfo();
}

//////////////////////////////////////////////////////////////////////
BOOL CUserWeaponSkill::LoadInfo(void)
{
    if (!m_pRes)
    {
        return false;
    }
    // load again?
    if (m_Info.id != ID_NONE)
    {
        return true;
    }
    m_Info.id		= m_pRes->LoadDWord("id");
    m_Info.idowner	= m_pRes->LoadDWord("owner_id");
    m_Info.nExp		= m_pRes->LoadInt("exp");
    m_Info.ucLevel	= m_pRes->LoadInt("level");
    m_Info.ucOldLev	= m_pRes->LoadInt("old_level");
    m_Info.nType	= m_pRes->LoadInt("type");
    if (m_Info.nType == ARROW_SUBTYPE)
    {
        return false;
    }
    return true;
}

/*/////////////////////////////////////////////////////////////////////
BOOL CUserWeaponSkill::CreateNewRecordset(UserWeaponSkillInfoStruct &info, IDatabase* pDb)
{	//INSERT INTO cq_weapon_skill VALUES (...)
	if (!pDb)
		return false;

	info.id	= ID_NONE;

	char szSql[512];
	sprintf(szSql, SQL_CREATERECORDSET, _TBL_WEAPONSKILL, info.nType, info.ucLevel, info.nExp, info.ucOldLev, info.idowner, info.id);

	DEBUG_TRY
	pDb->ExecuteSQL(szSql);
	DEBUG_CATCH("Fatal Database error Database() operate crash in UserWeaponSkill create new recordset.")

	return true;
}*/

//////////////////////////////////////////////////////////////////////
int	CUserWeaponSkill::GetType()
{
    return m_Info.nType;
}

//////////////////////////////////////////////////////////////////////
void CUserWeaponSkill::SetType(int nType, BOOL fUpdate)
{
    m_Info.nType	= nType;
    if (m_pRes)
    {
        m_pRes->SetInt("type", nType);
        if (fUpdate)
        {
            m_pRes->UpdateRecord();
        }
    }
}

//////////////////////////////////////////////////////////////////////
int CUserWeaponSkill::GetLevel()
{
    return m_Info.ucLevel;
}

//////////////////////////////////////////////////////////////////////
void CUserWeaponSkill::SetLevel(int nLevel, BOOL fUpdate)
{
    m_Info.ucLevel	= nLevel;
    if (m_pRes)
    {
        m_pRes->SetInt("level", nLevel);
        if (fUpdate)
        {
            m_pRes->UpdateRecord();
        }
    }
}

//////////////////////////////////////////////////////////////////////
int CUserWeaponSkill::GetExp()
{
    return m_Info.nExp;
}

//////////////////////////////////////////////////////////////////////
void CUserWeaponSkill::SetExp(int nExp, BOOL fUpdate)
{
    m_Info.nExp	= nExp;
    if (m_pRes)
    {
        m_pRes->SetInt("exp", nExp);
        if (fUpdate)
        {
            m_pRes->UpdateRecord();
        }
    }
}

//////////////////////////////////////////////////////////////////////
int CUserWeaponSkill::GetOldLev()
{
    return m_Info.ucOldLev;
}

//////////////////////////////////////////////////////////////////////
void CUserWeaponSkill::SetUnlearn(int nData, BOOL fUpdate)
{
    // no memery var	m_Info.ucOldLev	= nOldLev;
    if (m_pRes)
    {
        m_pRes->SetInt("unlearn", nData);
        if (fUpdate)
        {
            m_pRes->UpdateRecord();
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CUserWeaponSkill::SetOldLev(int nOldLev, BOOL fUpdate)
{
    m_Info.ucOldLev	= nOldLev;
    if (m_pRes)
    {
        m_pRes->SetInt("old_level", nOldLev);
        if (fUpdate)
        {
            m_pRes->UpdateRecord();
        }
    }
}

//////////////////////////////////////////////////////////////////////
OBJID CUserWeaponSkill::GetID()
{
    return m_Info.id;
}

//////////////////////////////////////////////////////////////////////
void CUserWeaponSkill::SetID(OBJID id)
{
    m_Info.id	= id;
}

//////////////////////////////////////////////////////////////////////
OBJID CUserWeaponSkill::GetOwnerID()
{
    return m_Info.idowner;
}

//////////////////////////////////////////////////////////////////////
void CUserWeaponSkill::SetOwnerID(OBJID idOwner, BOOL fUpdate)
{
    m_Info.idowner	= idOwner;
    if (m_pRes)
    {
        m_pRes->SetInt("owner_id", idOwner);
        if (fUpdate)
        {
            m_pRes->UpdateRecord();
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CUserWeaponSkill::DeleteRecord()
{
    return m_pRes->DeleteRecord() != 0;
}
