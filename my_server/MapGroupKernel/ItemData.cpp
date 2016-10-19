
//**********************************************************
// 代码编辑器
//**********************************************************

// ItemData.cpp: implementation of the CItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "ItemData.h"
#include "I_mydb.h"
#include "MapGroup.h"
#include "UserTable.h"
#include "ItemAddition.h"

char	szItemTable[] = _TBL_ITEM;
char	szEudemonTable[]	= _TBL_EUDEMON;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CItemData::CItemData()
{
    m_pData		= NULL;
    m_pType		= NULL;
    //	m_pEudemonType	= NULL;
    m_pAddition	= NULL;
    m_pEudemonData = NULL;
}

//////////////////////////////////////////////////////////////////////
CItemData::~CItemData()
{
    if (m_pData)
    {
        m_pData->Release();
    }
    SAFE_RELEASE (m_pEudemonData);
}

//////////////////////////////////////////////////////////////////////
bool CItemData::Create(IRecord* pDefault, const ItemInfoStruct* pInfo, IRecord* pDefaultEudemonData, bool bInsert/*=true*/, OBJID idNew/*=ID_NONE*/)			// false: 不存数据库
{
    CHECKF(!m_pData);
    CHECKF(!m_pType);
    //	CHECKF(!m_pEudemonType);
    CHECKF(pInfo);
    CHECKF(pDefault);
    OBJID idItem = pInfo->id;
    if (idNew)
    {
        idItem = idNew;
    }
    // data
    m_pData = CGameItemData::CreateNew();
    CHECKF(m_pData);
    IF_NOT(m_pData->Create(pDefault, idItem))
    return false;
    if (idNew)
    {
        SetInt(ITEMDATA_ID_,		idNew);
    }
    m_pData->SetInt(ITEMDATA_TYPE,			pInfo->idType);
    m_pData->SetInt(ITEMDATA_OWNERID,		pInfo->idOwner);
    m_pData->SetInt(ITEMDATA_PLAYERID,		pInfo->idPlayer);
    m_pData->SetInt(ITEMDATA_AMOUNT,		pInfo->nAmount);
    m_pData->SetInt(ITEMDATA_AMOUNTLIMIT,	pInfo->nAmountLimit);
    m_pData->SetInt(ITEMDATA_IDENT,			pInfo->nIdent);
    m_pData->SetInt(ITEMDATA_POSITION,		pInfo->nPosition);
    m_pData->SetInt(ITEMDATA_DATA,			pInfo->nData);
    m_pData->SetInt(ITEMDATA_GEM1,			pInfo->nGem1);
    m_pData->SetInt(ITEMDATA_GEM2,			pInfo->nGem2);
    m_pData->SetInt(ITEMDATA_MAGIC1,		pInfo->nMagic1);
    m_pData->SetInt(ITEMDATA_MAGIC2,		pInfo->nMagic2);
    m_pData->SetInt(ITEMDATA_MAGIC3,		pInfo->nMagic3);
    m_pData->SetInt(ITEMDATA_DATA,			pInfo->nData);
    //---jinggy---2004-11-19---begin
    m_pData->SetInt(ITEMDATA_WARGHOSTEXP, pInfo->dwWarGhostExp);
    m_pData->SetInt(ITEMDATA_GEMTYPE, pInfo->dwGemAtkType);
    m_pData->SetInt(ITEMDATA_AVAILABLETIME, pInfo->dwAvailabeTime);
    //---jinggy---2004-11-19---end
    // db
    if (bInsert)
    {
        IF_NOT(m_pData->InsertRecord())
        return false;
    }
    else
    {
        m_pData->ClearUpdateFlags();
    }
    // item type
    m_pType	= ItemType()->QueryItemType(GetInt(ITEMDATA_TYPE));
    IF_NOT(m_pType)
    return false;
    m_pAddition = ItemAddition()->QueryItemAddition(pInfo->idType, pInfo->nMagic3);
    // eudemon data
    if (IsEudemon())
    {
        if (strlen(pInfo->szName) > 0)
        {
            if (!CreateEudemonData(pDefaultEudemonData, this->GetID(), pInfo->szName, bInsert))
            {
                return false;
            }
        }
        else
        {
            if (!CreateEudemonData(pDefaultEudemonData, this->GetID(), m_pType->GetStr(ITEMTYPEDATA_NAME), bInsert))
            {
                return false;
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CItemData::Create (IRecord* pDefault, OBJID idType, OBJID idUser, int nPosition, IRecord* pDefaultEudemonData/*=NULL*/)
{
    CHECKF(!m_pData);
    CHECKF(!m_pType);
    CHECKF(idType != ID_NONE);
    CHECKF(pDefault);
    // type
    m_pType	= ItemType()->QueryItemType(idType);
    IF_NOT(m_pType)
    return false;
    // data
    m_pData = CGameItemData::CreateNew();
    CHECKF(m_pData);
    IF_NOT(m_pData->Create(pDefault, ID_NONE))
    return false;
    m_pData->SetInt(ITEMDATA_TYPE,			idType);
    m_pData->SetInt(ITEMDATA_OWNERID,		idUser);
    m_pData->SetInt(ITEMDATA_PLAYERID,		idUser);
    m_pData->SetInt(ITEMDATA_AMOUNT,		GetInt(ITEMDATA_AMOUNT_ORIGINAL));
    m_pData->SetInt(ITEMDATA_AMOUNTLIMIT,	GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL));
    m_pData->SetInt(ITEMDATA_IDENT,			GetInt(ITEMDATA_IDENT_ORIGINAL));
    m_pData->SetInt(ITEMDATA_POSITION,		nPosition);
    m_pData->SetInt(ITEMDATA_GEM1,			GetInt(ITEMDATA_GEM1_ORIGINAL));
    m_pData->SetInt(ITEMDATA_GEM2,			GetInt(ITEMDATA_GEM2_ORIGINAL));
    m_pData->SetInt(ITEMDATA_MAGIC1,		GetInt(ITEMDATA_MAGIC1_ORIGINAL));
    m_pData->SetInt(ITEMDATA_MAGIC2,		GetInt(ITEMDATA_MAGIC2_ORIGINAL));
    m_pData->SetInt(ITEMDATA_MAGIC3,		GetInt(ITEMDATA_MAGIC3_ORIGINAL));
    //---jinggy---2004-11-19---begin
    m_pData->SetInt(ITEMDATA_WARGHOSTEXP,		GetInt(ITEMDATA_WARGHOSTEXP));
    m_pData->SetInt(ITEMDATA_GEMTYPE,		GetInt(ITEMDATA_GEMTYPE));
    m_pData->SetInt(ITEMDATA_AVAILABLETIME,		GetInt(ITEMDATA_AVAILABLETIME));
    //---jinggy---2004-11-19---end
    // db
    IF_NOT(m_pData->InsertRecord())
    return false;
    m_pAddition = ItemAddition()->QueryItemAddition(idType, GetInt(ITEMDATA_ADDITION));
    // eudemon data
    if (IsEudemon())
    {
        if (!CreateEudemonData(pDefaultEudemonData, this->GetID(), m_pType->GetStr(ITEMTYPEDATA_NAME)))
        {
            return false;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CItemData::Create(OBJID idItem, IDatabase* pDb)
{
    CHECKF(!m_pData);
    CHECKF(!m_pType);
    CHECKF(!m_pEudemonData);
    CHECKF(pDb);
    // data
    m_pData = CGameItemData::CreateNew();
    IF_NOT(m_pData)
    return false;
    IF_NOT(m_pData->Create(idItem, pDb))
    return false;
    // type
    m_pType	= ItemType()->QueryItemType(GetInt(ITEMDATA_TYPE));
    IF_NOT(m_pType)
    return false;
    if (!LoadEudemonData(idItem, pDb))
    {
        return false;
    }
    m_pAddition = ItemAddition()->QueryItemAddition(GetInt(ITEMDATA_TYPE), GetInt(ITEMDATA_ADDITION));
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CItemData::Create(IRecordset* pRes, IDatabase* pDb)
{
    CHECKF(!m_pData);
    CHECKF(!m_pType);
    CHECKF(!m_pEudemonData);
    CHECKF(pRes);
    CHECKF(pDb);
    // data
    m_pData = CGameItemData::CreateNew();
    IF_NOT(m_pData)
    return false;
    IF_NOT(m_pData->Create(pRes))
    return false;
    // type
    m_pType	= ItemType()->QueryItemType(GetInt(ITEMDATA_TYPE));
    if (!m_pType)
    {
        LOGERROR("数据库发现未知物品类型[%d]", GetInt(ITEMDATA_TYPE));
        return false;
    }
    if (!LoadEudemonData(this->GetID(), pDb))
    {
        return false;
    }
    m_pAddition = ItemAddition()->QueryItemAddition(GetInt(ITEMDATA_TYPE), GetInt(ITEMDATA_ADDITION));
    return true;
}

//////////////////////////////////////////////////////////////////////
// info
//////////////////////////////////////////////////////////////////////
bool CItemData::GetInfo	(ItemInfoStruct* pInfo)
{
    CHECKF(pInfo);
    CHECKF(GetID() != ID_NONE);
    pInfo->id			= GetID();
    pInfo->idType		= m_pData->GetInt(ITEMDATA_TYPE);
    pInfo->idOwner		= m_pData->GetInt(ITEMDATA_OWNERID);
    pInfo->idPlayer		= m_pData->GetInt(ITEMDATA_PLAYERID);
    pInfo->nAmount		= m_pData->GetInt(ITEMDATA_AMOUNT);
    pInfo->nAmountLimit	= m_pData->GetInt(ITEMDATA_AMOUNTLIMIT);
    pInfo->nIdent		= m_pData->GetInt(ITEMDATA_IDENT);
    pInfo->nPosition	= m_pData->GetInt(ITEMDATA_POSITION);
    pInfo->nData		= m_pData->GetInt(ITEMDATA_DATA);
    pInfo->nGem1		= m_pData->GetInt(ITEMDATA_GEM1);
    pInfo->nGem2		= m_pData->GetInt(ITEMDATA_GEM2);
    pInfo->nMagic1		= m_pData->GetInt(ITEMDATA_MAGIC1);
    pInfo->nMagic2		= m_pData->GetInt(ITEMDATA_MAGIC2);
    pInfo->nMagic3		= m_pData->GetInt(ITEMDATA_MAGIC3);
    pInfo->nData		= m_pData->GetInt(ITEMDATA_DATA);
    //---jinggy---2004-11-19---begin
    pInfo->dwWarGhostExp = m_pData->GetInt(ITEMDATA_WARGHOSTEXP);
    pInfo->dwGemAtkType = m_pData->GetInt(ITEMDATA_GEMTYPE);
    pInfo->dwAvailabeTime = m_pData->GetInt(ITEMDATA_AVAILABLETIME);
    //---jinggy---2004-11-19---end
    if (this->IsEudemon())
    {
        ::SafeCopy(pInfo->szName, m_pEudemonData->GetStr(EUDEMONDATA_NAME), _MAX_NAMESIZE);
    }
    else
    {
        pInfo->szName[0] = 0;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CItemData::SaveInfo(void)
{
    CHECKF(m_pData);
    CHECKF(m_pType);
    m_pData->Update();
    if (m_pEudemonData)
    {
        m_pEudemonData->Update();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CItemData::DeleteRecord()
{
    DEBUG_TRY
    if (this->IsEudemon())
    {
        IF_OK (m_pEudemonData)
        m_pEudemonData->DeleteRecord();
    }
    DEBUG_CATCH ("CItemData::DeleteRecord() delete eudemon data record")
    return m_pData->DeleteRecord();
}

//////////////////////////////////////////////////////////////////////
bool CItemData::DeleteItemRecord(OBJID id, IDatabase* pDb)
{
    if (id == ID_NONE || !pDb)
    {
        return false;
    }
    SQLBUF	szSQL;
    sprintf(szSQL, "DELETE FROM %s WHERE owner_id=%u LIMIT 1", _TBL_EUDEMON, id);
    pDb->ExecuteSQL(szSQL);
    sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_ITEM, id);
    return pDb->ExecuteSQL(szSQL);
}

//////////////////////////////////////////////////////////////////////
int CItemData::GetInt(ITEMDATA idx)
{
    // add by zlong 2003-02-03
    if (IsEudemon())
    {
        // TODO: 这里添加关于幻兽的字段转意(如果需要)
        switch (idx)
        {
        case ITEMDATA_EXP:
            idx = ITEMDATA_GEMTYPE;
            break;
        case ITEMDATA_GROWTH:
            idx = ITEMDATA_DATA;
            break;
        case ITEMDATA_EUDEMON_LEVEL:
            idx = ITEMDATA_AMOUNTLIMIT;
            break;
        case ITEMDATA_EUDEMON_LIFE:
            idx = ITEMDATA_WARGHOSTEXP;
            break;
        case ITEMDATA_FIDELITY:
            idx = ITEMDATA_AMOUNT;
            break;
        }
    }
    else if (IsSprite())	// add by zlong 2003-12-01
    {
        switch (idx)
        {
        case ITEMDATA_AMOUNT:
        case ITEMDATA_AMOUNTLIMIT:
            return 1;
            break;
        case ITEMDATA_DATA:
            return 0;
            break;
        case ITEMDATA_EXP:
            idx = ITEMDATA_AMOUNT;
            break;
        case ITEMDATA_ATTRIB:
            idx = ITEMDATA_AMOUNTLIMIT;
            break;
        case ITEMDATA_GROWTH:
            idx = ITEMDATA_DATA;
            break;
        case ITEMDATA_ATTACK_MAX_:
        case ITEMDATA_ATTACK_MIN_:
        case ITEMDATA_MAGICATK_MAX_:
        case ITEMDATA_MAGICATK_MIN_:
        case ITEMDATA_DEFENSE_:
        case ITEMDATA_MAGICDEF_:
        case ITEMDATA_MANA:
            {
                int nSpriteType = (m_pData->GetInt(ITEMDATA_TYPE) / 1000) % 10;
                if (((idx == ITEMDATA_ATTACK_MAX_ || idx == ITEMDATA_ATTACK_MIN_) && nSpriteType == _SPRITE_ADDITION_PATK)
                        || ((idx == ITEMDATA_MAGICATK_MAX_ || idx == ITEMDATA_MAGICATK_MIN_) && (nSpriteType == _SPRITE_ADDITION_MATK))
                        || (idx == ITEMDATA_DEFENSE_ && nSpriteType == _SPRITE_ADDITION_PDEF)
                        || (idx == ITEMDATA_MAGICDEF_ && nSpriteType == _SPRITE_ADDITION_MDEF)
                        || (idx == ITEMDATA_MANA && nSpriteType == _SPRITE_ADDITION_SOUL))
                {
                    idx = ITEMDATA_AMOUNTLIMIT;
                }
                else
                {
                    return 0;
                }
            }
            break;
        case ITEMDATA_REQ_LEVEL:
            {
                // 要求等级是精灵等级*2
                return 2 * m_pType->GetInt((ITEMTYPEDATA)(ITEMDATA_REQ_LEVEL - ITEMTYPEDATA_OFFSET));
            }
            break;
        case ITEMDATA_SPRITE_LEVEL:
            idx = ITEMDATA_REQ_LEVEL;
            break;
        case ITEMDATA_GROWTH_ORIGINAL:
            idx = ITEMDATA_ATTACK_MAX_;
            break;
        case ITEMDATA_LEVEXP:
            idx = ITEMDATA_ATTACK_MIN_;
            break;
        };
    }
    else if (IsMount())
    {
        if (idx == ITEMDATA_GEM1)
        {
            return GEM_NONE;
        }
        else if (idx == ITEMDATA_INTIMACY)
        {
            idx = ITEMDATA_GEM1;
        }
    }
    if (idx < ITEMTYPEDATA_OFFSET)
    {
        return m_pData->GetInt(idx);
    }
    return m_pType->GetInt((ITEMTYPEDATA)(idx - ITEMTYPEDATA_OFFSET));
}

//////////////////////////////////////////////////////////////////////
void CItemData::SetInt(ITEMDATA idx, int nData, bool bUpdate/*=false*/)
{
    // add by zlong 2003-12-01
    if (IsSprite())
    {
        switch (idx)
        {
        case ITEMDATA_AMOUNT:
        case ITEMDATA_AMOUNTLIMIT:
        case ITEMDATA_DATA:
            ASSERT(!"CItemData::SetInt");		// paled: 可以发现隐藏的代码错误：)
            return ;
            break;
        case ITEMDATA_EXP:
            idx = ITEMDATA_AMOUNT;
            break;
        case ITEMDATA_ATTRIB:
            idx = ITEMDATA_AMOUNTLIMIT;
            break;
        case ITEMDATA_GROWTH:
            idx = ITEMDATA_DATA;
            break;
        };
    }
    else if (IsMount())
    {
        CHECK(idx != ITEMDATA_GEM1);
        if (idx == ITEMDATA_INTIMACY)
        {
            idx = ITEMDATA_GEM1;
        }
    }
    else if (IsEudemon())
    {
        switch (idx)
        {
        case ITEMDATA_EXP:
            idx = ITEMDATA_GEMTYPE;
            break;
        case ITEMDATA_GROWTH:
            idx = ITEMDATA_DATA;
            break;
        case ITEMDATA_EUDEMON_LEVEL:
            idx = ITEMDATA_AMOUNTLIMIT;
            break;
        case ITEMDATA_EUDEMON_LIFE:
            idx = ITEMDATA_WARGHOSTEXP;
            break;
        case ITEMDATA_FIDELITY:
            idx = ITEMDATA_AMOUNT;
            break;
        }
    }
    CHECK(idx < ITEMTYPEDATA_OFFSET);
    m_pData->SetInt(idx, nData);
    if (bUpdate)
    {
        m_pData->Update();
    }
}

//////////////////////////////////////////////////////////////////////
LPCTSTR CItemData::GetStr(ITEMDATA idx)
{
    if (IsEudemon())
    {
        CHECKF (m_pEudemonData);
        switch (idx)
        {
        case ITEMDATA_NAME:
            return m_pEudemonData->GetStr(EUDEMONDATA_NAME);
            break;
        }
    }
    if (idx < ITEMTYPEDATA_OFFSET)
    {
        return m_pData->GetStr(idx);
    }
    return m_pType->GetStr((ITEMTYPEDATA)(idx - ITEMTYPEDATA_OFFSET));
}

//////////////////////////////////////////////////////////////////////
void CItemData::SetStr(ITEMDATA idx, LPCTSTR szData, int nSize, bool bUpdate /*= false*/)
{
    if (IsEudemon())
    {
        ASSERT (m_pEudemonData);
        switch (idx)
        {
        case ITEMDATA_NAME:
            m_pEudemonData->SetStr(EUDEMONDATA_NAME, szData, nSize);
            if (bUpdate)
            {
                m_pEudemonData->Update();
            }
            return;
            break;
        }
    }
    CHECK(idx < ITEMTYPEDATA_OFFSET);
    m_pData->SetStr(idx, szData, nSize);
    if (bUpdate)
    {
        m_pData->Update();
    }
}

//////////////////////////////////////////////////////////////////////
bool CItemData::LoadEudemonData(OBJID idOwner, IDatabase* pDb)
{
    CHECKF (!m_pEudemonData);
    CHECKF (pDb);
    CHECKF (idOwner != ID_NONE);
    if (IsEudemon())
    {
        SQLBUF	szSQL;
        sprintf(szSQL, "SELECT * FROM %s WHERE owner_id=%u LIMIT 1", _TBL_EUDEMON, idOwner);
        IRecordset* pRes = pDb->CreateNewRecordset(szSQL);	//VVVVVVVVV
        IF_NOT (pRes)
        return false;
        m_pEudemonData = CEudemonData::CreateNew();
        IF_NOT (m_pEudemonData && m_pEudemonData->Create(pRes))
        {
            SAFE_RELEASE (pRes);
            return false;
        }
        SAFE_RELEASE (pRes);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CItemData::CreateEudemonData(IRecord* pDefaultData, OBJID idOwner, const char* pszName, bool bInsert/*=true*/)
{
    CHECKF (pDefaultData);
    CHECKF (pszName);
    CHECKF (!m_pEudemonData);
    m_pEudemonData = CEudemonData::CreateNew();
    CHECKF (m_pEudemonData);
    IF_NOT (m_pEudemonData->Create(pDefaultData, ID_NONE))
    return false;
    m_pEudemonData->SetInt(EUDEMONDATA_OWNERID,	idOwner);
    m_pEudemonData->SetStr(EUDEMONDATA_NAME,	pszName, _MAX_NAMESIZE);
    if (bInsert)
    {
        IF_NOT (m_pEudemonData->InsertRecord())
        return false;
    }
    else
    {
        m_pEudemonData->ClearUpdateFlags();
    }
    return true;
}
