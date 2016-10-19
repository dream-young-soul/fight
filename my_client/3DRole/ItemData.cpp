
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ItemData.cpp: implementation of the CItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "ItemData.h"

// class static
MAP_ITEMINFO CItemData::s_mapItemType;
//---------------------------------------------------------------------
CItemData::CItemData()
{
   
    FILE* fp = fopen("ini/itemnamelist.ini", "r");
    if (fp)
    {
        char szLine[1024] = "";
        while(true)
        {
            if (1 != fscanf(fp, "%s\n", szLine))
            {
                break;
            }
            if (strlen(szLine) == 0)
            {
                continue;
            }
            std::string str	= szLine;
            m_setItemName.push_back(str);
        }
        fclose(fp);
    }
    this->CreateAdditionalInfo();
	m_dwRecordAmount = 0;
}
//---------------------------------------------------------------------
BOOL CItemData::CheckItem(const char* pszItemName)
{
    if (!pszItemName)
    {
        return false;
    }
    for(int i = 0; i < (int)m_setItemName.size(); i ++)
    {
        char szName[_MAX_NAMESIZE] = "";
        strcpy(szName, m_setItemName[i].c_str());
        if (strcmp(szName, pszItemName) == 0)
        {
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------
CItemData::~CItemData()
{
  
    m_setItemName.clear();
    m_mapItemAdditional.clear();
  ;
}
void CItemData::LoadAllItemInfo()
{
	FILE*	fp = NULL;
	char szFilename[]="ini/itemtype.dat";
	fp = fopen(szFilename, "rb");
	MYASSERT(fp);
	fread(&m_dwRecordAmount, sizeof(DWORD), 1, fp);

	OBJID *p_index = new OBJID[m_dwRecordAmount];
	 // ¶ÁÈ¡IDË÷Òý
    fread(p_index, sizeof(OBJID), m_dwRecordAmount, fp);
	delete []p_index;
	ItemTypeInfo infoItemType;
	for(DWORD i = 0; i < m_dwRecordAmount; i++)
	{
		// ¶Á¼ÇÂ¼
		fread(&infoItemType, sizeof(ItemTypeInfo), 1, fp);
		// save type info
		s_mapItemType.insert(MAP_ITEMINFO::value_type(infoItemType.uID, infoItemType));
	}
	fclose(fp);
	

	this->ExportItemData();
}

void CItemData::ExportItemData()
{
	char szFileName[] = "ini/itemtype.txt";
	FILE* f = fopen(szFileName,"wt+");
	
	MAP_ITEMINFO::iterator itr = s_mapItemType.begin();
	for(;itr!= s_mapItemType.end();itr++)
	{
		ItemTypeInfo info = itr->second;
		char szStr[1024] = {0};
		sprintf(szStr,"%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",info.uID,info.szName,info.ucRequiredProfession,info.ucRequiredWeaponSkill,info.ucRequiredLevel,info.ucRequiredSex,
			info.usRequiredForce,info.usRequiredSpeed,info.usRequiredHealth,info.usRequiredSoul,info.ucMonopoly,info.usWeight,info.uPrice,info.uLuck,info.usMaxAttack,info.usMinAttack,info.usDefense,info.usDexterity,info.usDodge,info.sLife,info.sMana,
			info.usAmount,info.usAmountLimit,info.ucStatus,info.ucGem1,info.ucGem2,info.ucMagic1,info.ucMagic2,info.ucMagic3,info.usMagicAttack,info.usMagicDefence,info.usRange,info.usAttackSpeed,info.nHitarte,info.usTarget,info.szDesc);


		fputs(szStr,f);

	}
	fclose(f);

}
//---------------------------------------------------------------------
BOOL CItemData::GetItemTypeInfo(OBJID idType, ItemTypeInfo& infoItemType)
{
	MAP_ITEMINFO::iterator itr = s_mapItemType.find(idType);
	if (itr != s_mapItemType.end())
	{
		memcpy(&infoItemType, &itr->second, sizeof(ItemTypeInfo));
		return true;
	}
	return false;
}
//---------------------------------------------------------------------
BOOL	CItemData::GetItemAdditionInfo(OBJID idType, int nLevel, ItemAdditionInfo& infoAdditional)
{
    __int64 i64Data = nLevel;
    idType = (idType / 10) * 10;
    i64Data = (i64Data << 32) + idType;
    map<__int64, ItemAdditionInfo>::iterator iter = m_mapItemAdditional.find(i64Data);
    if (iter != m_mapItemAdditional.end())
    {
        infoAdditional = iter->second;
        return true;
    }
    return false;
}
//---------------------------------------------------------------------
void	CItemData::CreateAdditionalInfo()
{
    const char szIniFile[] = "ini/ItemAddition.ini";
    FILE* fp = fopen(szIniFile, "r");
    if (!fp)
    {
        return;
    }
    char szLine[1024];
    while(fgets(szLine, 1023, fp))
    {
        if (strlen(szIniFile) <= 0)
        {
            continue;
        }
        if (szIniFile[0] == ';')
        {
            continue;
        }
        DWORD dwID, dwLevel;
        ItemAdditionInfo info;
        if (11 == sscanf(szLine, "%u %u %d %d %d %d %d %d %d %d %d\n", &dwID, &dwLevel,
                         &info.nLife, &info.nAttackMax, &info.nAttackMin,
                         &info.nDefense, &info.nMAttackMax, &info.nMAttackMin,
                         &info.nMDef, &info.nDexterity, &info.nDodge))
        {
            __int64 i64Data = dwLevel;
            i64Data = (i64Data << 32) + dwID;
            //			m_mapItemAdditional[1] = info;
            m_mapItemAdditional.insert(MAP_ITEMADDITIONINFO::value_type(i64Data, info));
        }
    }
    fclose(fp);
}
