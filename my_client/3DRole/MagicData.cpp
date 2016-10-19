
//**********************************************************
// 代码编辑器
//**********************************************************

// MagicData.cpp: implementation of the CMagicData class.
//
//////////////////////////////////////////////////////////////////////

#include "MagicData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMagicData::CMagicData()
{
    this->Create();
    //this->CreateAction();
}

CMagicData::~CMagicData()
{
    this->Destroy();
}

//------------------------------------------------------------------
void CMagicData::CreateAction()
{
    const char szIniFile[] = "ini/MagicTypeAction.ini";
    FILE* fp = fopen(szIniFile, "r");
    if (!fp)
    {
        return;
    }
    char szLine[1024];
    while(true)
    {
        if (NULL == fgets(szLine, 1023, fp))
        {
            fclose(fp);
            return;
        }
        if (szLine[0] == '[')
        {
            DWORD dwType, dwLevel;
            DWORD dwAmount;
            sscanf(szLine, "[%u_%d]", &dwType, &dwLevel);
            if (1 == fscanf(fp, "Amount=%d/n", &dwAmount))
            {
                for(int j = 0; j < (int)m_setMagicType.size(); j ++)
                {
                    if (m_setMagicType[j]->idMagicType == dwType &&
                            m_setMagicType[j]->dwLevel == dwLevel)
                    {
                        for(DWORD i = 0; i < dwAmount; i ++)
                        {
                            int nAction;
                            if (1 == fscanf(fp, "%d/n", &nAction))
                            {
                                m_setMagicType[j]->setAction.push_back(nAction);
                            }
                        }
                    }
                }
            }
        }
    }
}
//------------------------------------------------------------------
void CMagicData::Create()
{
    // 需求不定 只能先从INI传创建，待需求稳定，改向Data。
    const char szIniFile[] = "ini/MagicType.ini";
    DWORD dwData[64];
    char szFileName[_MAX_NAMESIZE];
    char szDisc[64];
    char szLine[1024];
    FILE* fp = fopen(szIniFile, "r");
    if (!fp)
    {
        return;
    }
    while(true)
    {
        // 扩展
        char	szIntoneEffect[64];			// 吟唱特效
        char	szIntoneSound[_MAX_PATH];	// 吟唱声效
        char	szSenderEffect[64];			// 攻击特效
        char	szSenderSound[_MAX_PATH];	// 攻击音效
        DWORD	dwTargetDelay;				// 受击延时
        char	szTargetEffect[64];			// 受击特效
        char	szTargetSound[_MAX_PATH];	// 受击音效
        char	szGroundEffect[64];			// 地效索引
        char	szTraceEffect[64];			// 轨迹特效
        int nResult = fscanf(fp, "%u %u %s %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %s %s %s %s %s %u %s %s %s %s %u %u %u %u %u/n",
                             &dwData[0], &dwData[1], szFileName,
                             &dwData[2], &dwData[3], &dwData[4], &dwData[5], &dwData[6],
                             &dwData[7], &dwData[8], &dwData[9], &dwData[10], &dwData[11],
                             &dwData[12], &dwData[13], &dwData[14], &dwData[15], &dwData[16],
                             &dwData[17], &dwData[18], &dwData[19],
                             &dwData[20], &dwData[21], &dwData[22], &dwData[23], &dwData[24], &dwData[25], &dwData[26],
                             &dwData[27], &dwData[28], &dwData[29], &dwData[30],
                             &dwData[31], &dwData[32], &dwData[33],
                             &dwData[34], szDisc, szIntoneEffect, szIntoneSound, szSenderEffect,
                             szSenderSound, &dwTargetDelay, szTargetEffect, szTargetSound,
                             szGroundEffect, szTraceEffect, &dwData[35], &dwData[36], &dwData[37], &dwData[38],
                             &dwData[39]);
        if (nResult == EOF)
        {
            fclose(fp);
            return;
        }
        if (nResult != 51)
        {
            fscanf(fp, "%s/n", szLine);
            continue;
        }
        MagicTypeInfo* pInfo = new 	MagicTypeInfo;
        MYASSERT(pInfo);
        pInfo->idMagicType = dwData[0];
        pInfo->dwActionSort = dwData[1];
        strcpy(pInfo->szName, szFileName);
        pInfo->dwCrime = dwData[2];
        pInfo->dwGround = dwData[3];
        pInfo->dwMulti = dwData[4];
        pInfo->dwTarget = dwData[5];
        pInfo->dwLevel = dwData[6];
        pInfo->dwMpCost = dwData[7];
        pInfo->dwPotential = dwData[8];
        pInfo->dwPower = dwData[9];
        pInfo->dwIntoneDuration = dwData[10];
        pInfo->dwHitPoint = dwData[11];
        pInfo->dwDuration = dwData[12];
        pInfo->dwRange = dwData[13];
        pInfo->dwDistance = dwData[14];
        pInfo->dwStatus = dwData[15];
        pInfo->dwProfessionalRequired = dwData[16];
        pInfo->dwExpRequired = dwData[17];
        pInfo->dwMonsterLevelRequired	= dwData[18];
        pInfo->dwXp			 = dwData[19];
        pInfo->dwWeaponSubType = dwData[20];
        pInfo->dwActiveTime	= dwData[21];
        pInfo->dwAutoActive = dwData[22];
        pInfo->dwFloorAttribute = dwData[23];
        pInfo->dwAutoLearn = dwData[24];
        pInfo->dwLearnLevel = dwData[25];
        pInfo->dwDropWeapon = dwData[26];
        pInfo->dwUsePP = dwData[27];
        pInfo->dwWeaponHit = dwData[28];
        pInfo->idUseItem = dwData[29];
        pInfo->idNextMagic = dwData[30];
        pInfo->dwMagicBreak = dwData[31];
        pInfo->dwClientRepresent	= dwData[32];
        pInfo->dwSenderAction		= dwData[33];
        pInfo->dwNeedLevel          = dwData[34];
        pInfo->dwScreenRepresent	= dwData[35];
        pInfo->dwCanBeusedInMarket	= dwData[36];
        pInfo->dwTargetWoundDelay	= dwData[37];
        pInfo->dwDelay				= dwData[38];
        pInfo->bTrack				= dwData[39];
        strcpy(pInfo->szIntoneEffect, szIntoneEffect);
        strcpy(pInfo->szIntoneSound, szIntoneSound);
        strcpy(pInfo->szSenderEffect, szSenderEffect);
        strcpy(pInfo->szSenderSound, szSenderSound);
        pInfo->dwTargetDelay =  dwTargetDelay;
        strcpy(pInfo->szTargetEffect, szTargetEffect);
        strcpy(pInfo->szTargetSound, szTargetSound);
        strcpy(pInfo->szGroundEffect, szGroundEffect);
        strcpy(pInfo->szTraceEffect, szTraceEffect);
        strcpy(pInfo->szDisc, szDisc);
        m_setMagicType.push_back(pInfo);
    }
}
//------------------------------------------------------------------
void CMagicData::Destroy()
{
    int nAmount = m_setMagicType.size();
    for(int i = 0; i < nAmount; i ++)
    {
        MagicTypeInfo* pInfo = m_setMagicType[i];
        SAFE_DELETE(pInfo);
    }
    m_setMagicType.clear();
}
//------------------------------------------------------------------
BOOL CMagicData::GetMagicTypeInfo(OBJID dwIDType, DWORD dwLevel, MagicTypeInfo& infoMagic)
{
    int nAmount = m_setMagicType.size();
    for(int i = 0; i < nAmount; i ++)
    {
        MagicTypeInfo* pInfo = m_setMagicType[i];
        if	(pInfo &&
                (pInfo->idMagicType == dwIDType) &&
                (pInfo->dwLevel == dwLevel))
        {
			infoMagic.idMagicType = pInfo->idMagicType;
			infoMagic.dwActionSort = pInfo->dwActionSort;	
			strcpy(infoMagic.szName,pInfo->szName);
			infoMagic.dwCrime = pInfo->dwCrime;			
			infoMagic.dwGround = pInfo->dwGround;	
			infoMagic.dwMulti = pInfo->dwMulti;		
			infoMagic.dwTarget = pInfo->dwTarget;		
			infoMagic.dwLevel = pInfo->dwLevel;		
			infoMagic.dwMpCost = pInfo->dwMpCost;				
			infoMagic.dwPotential = pInfo->dwPotential;			
			infoMagic.dwPower = pInfo->dwPower;			
			infoMagic.dwIntoneDuration = pInfo->dwIntoneDuration;			
			infoMagic.dwHitPoint = pInfo->dwHitPoint;		
			infoMagic.dwDuration = pInfo->dwDuration;	;				
			infoMagic.dwRange = pInfo->dwRange;	
			infoMagic.dwDistance = pInfo->dwDistance;		
			infoMagic.dwStatus = pInfo->dwStatus;
			infoMagic.dwProfessionalRequired = pInfo->dwProfessionalRequired;;
			infoMagic.dwExpRequired = pInfo->dwExpRequired;			
			infoMagic.dwMonsterLevelRequired = pInfo->dwMonsterLevelRequired;
			infoMagic.dwXp = pInfo->dwXp;				
			infoMagic.dwWeaponSubType = pInfo->dwWeaponSubType;	
			infoMagic.dwActiveTime = pInfo->dwActiveTime;	
			infoMagic.dwAutoActive = pInfo->dwAutoActive;			
			infoMagic.dwFloorAttribute = pInfo->dwFloorAttribute;		
			infoMagic.dwAutoLearn = pInfo->dwAutoLearn;		
			infoMagic.dwLearnLevel = pInfo->dwLearnLevel;		
			infoMagic.dwDropWeapon = pInfo->dwDropWeapon;				
			infoMagic.dwUsePP = pInfo->dwUsePP;				
			infoMagic.dwWeaponHit = pInfo->dwWeaponHit;		
			infoMagic.idUseItem = pInfo->idUseItem;	
			infoMagic.idNextMagic = pInfo->idNextMagic;	
			infoMagic.dwMagicBreak = pInfo->dwMagicBreak;
			infoMagic.dwClientRepresent = pInfo->dwClientRepresent;	
			infoMagic.dwSenderAction = pInfo->dwSenderAction;		
			infoMagic.dwNeedLevel = pInfo->dwNeedLevel;		
			strcpy(infoMagic.szDisc,pInfo->szDisc);
			strcpy(infoMagic.szIntoneEffect,pInfo->szIntoneEffect);			
			strcpy(infoMagic.szIntoneSound,pInfo->szIntoneSound);		
			strcpy(infoMagic.szSenderEffect,pInfo->szSenderEffect);	
			strcpy(infoMagic.szSenderSound,pInfo->szSenderSound);	
			infoMagic.dwTargetDelay = pInfo->dwTargetDelay;
			strcpy(infoMagic.szTargetEffect,pInfo->szTargetEffect);	
			strcpy(infoMagic.szTargetSound,pInfo->szTargetSound);
			strcpy(infoMagic.szGroundEffect,pInfo->szGroundEffect);
			strcpy(infoMagic.szTraceEffect,pInfo->szTraceEffect);
			infoMagic.dwScreenRepresent = pInfo->dwScreenRepresent;
			infoMagic.dwCanBeusedInMarket = pInfo->dwCanBeusedInMarket;
			infoMagic.dwTargetWoundDelay = pInfo->dwTargetWoundDelay;
			infoMagic.dwDelay = pInfo->dwDelay;
			infoMagic.bTrack = pInfo->bTrack;
			if(pInfo->setAction.size() > 0)
			{
				infoMagic.setAction.resize(pInfo->setAction.size());
				for(int i = 0;i < (int)pInfo->setAction.size();i++)
				{
					infoMagic.setAction[i] = pInfo->setAction[i];
				}

			}
          //  memcpy(&infoMagic, pInfo, sizeof(MagicTypeInfo));
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------

