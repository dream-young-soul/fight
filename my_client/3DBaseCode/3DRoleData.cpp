
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DRoleData.cpp: implementation of the C3DRoleData class.
//
//////////////////////////////////////////////////////////////////////


#include "basefunc.h"
#include "3DRoleData.h"
#include "3DSimpleObj.h"
#include "GameDataSet.h"

// const...
const DWORD _ACTION_UNIVERSAL	= 999;
const DWORD _LOOKTYPE_UNIVERSAL = 999;
const DWORD _WEAPONTYPE_UNIVERSAL = 999;
const DWORD _PARTTYPE_UNIVERSAL = 999;
const DWORD _VARIABLE_UNIVERSAL = 999;

const char INI_EFFECT[]	= "ini/effect.ini";

// gobal...
C3DRoleData g_obj3DRoleData;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD RolePartInfoStr2ID( const char* str )
{
    int i;
    unsigned int v;
    static unsigned m[70];
    strncpy((char*)m, str, 256);
    for (i = 0; i < 256 / 4 && m[i]; i++) ;
    m[i++] = 0x9BE74448, m[i++] = 0x66F42C48;
    v = 0xF4FA8928;
    __asm
    {
        mov esi, 0x37A8470E		;//x0=0x37A8470E
        mov edi, 0x7758B42B		;//y0=0x7758B42B
        xor ecx, ecx
        _loop:
        mov ebx, 0x267B0B11		;//w=0x267B0B11
        rol v, 1
        lea eax, m
        xor ebx, v

        mov eax, [eax+ecx*4]
        mov edx, ebx
        xor esi, eax
        xor edi, eax

        add edx, edi
        or edx, 0x2040801		;//a=0x2040801
        and edx, 0xBFEF7FDF		;//c=0xBFEF7FDF

        mov eax, esi
        mul edx
        adc eax, edx
        mov edx, ebx
        adc eax, 0

        add edx, esi
        or edx, 0x804021		;//b=0x804021
        and edx, 0x7DFEFBFF		;//d=0x7DFEFBFF

        mov esi, eax
        mov eax, edi
        mul edx

        add edx, edx
        adc eax, edx
        jnc _skip
        add eax, 2
        _skip:
        inc ecx;
        mov edi, eax
        cmp ecx, i
        jnz _loop
        xor esi, edi
        mov v, esi
    }
    return v;
}
//////////////////////////////////////////////////////////////////////

C3DRoleData::C3DRoleData()
{
}

C3DRoleData::~C3DRoleData()
{
    this->DestroyEffectLookSetInfo();
    this->DestoryWeaponInfo();
    this->DestroyActionDelayInfo();
    this->DestroyActionInfo();
    this->Destroy3DEffectInfo();
    this->DestroyWeaponCombinInfo();
    this->DestroySoundInfo();
    this->DestroyMonsterInfo();
    this->DestroyActionMapEffectInfo();
    this->DestroyMy3DEffectInfo();
    this->DestroyRolePartInfo();
    this->DestroyTransFormInfo();
    this->DestroyNpcTypeInfo();
    this->DestroySimpleObjInfo();
    this->DestroyEffectInfo();
    this->DestroyMapMagicItemInfo();
    this->DestroyPkTitleInfo();
    this->DestroyFlyObjInfo();
    this->DestroyTerrainWeatherInfo();
    this->DestroyStatusEffectInfo();
    this->DestroyUserHelpInfo();
    this->DestroyTerrainNpcTypeInfo();
    this->DestroyTipsInfo();
    this->DestroyTextAction();
    this->DestroyPetInfo();
    this->DestroyRolePartInfo();
    this->DestroyRolePartLightInfo();
    this->Destroy3DItemInfo();
    this->DestroyMedalInfo();
    this->DestroyNobilityTitleInfo();
    this->DestroyHonorTitleInfo();
    this->DestroyRolePartLightInfo();
    this->Destroy3DItemInfo();
    this->DestroyMedalInfo();
    this->DestroyNobilityTitleInfo();
    this->DestroyHonorTitleInfo();
    this->DestroyLightInfo();
    this->DestroyTrackInfo();
    this->DestroyEffectIndexInfo();
    this->DestroyActionCtrlInfo();
    this->DestroyActionDelayInfo();
}

//---------------------------------------------------------------------
void C3DRoleData::Init()
{
    this->CreateActionInfo();
    this->Create3DEffectInfo();
    this->CreateWeaponCombinInfo();
    this->CreateRolePartInfo();
    this->CreateSoundInfo();
    this->CreateMonsterInfo();
    this->CreateMy3DEffectInfo();
    this->LoadCommonIni();
    //this->CreateTransFormInfo();
    this->Create3DSimpleObjInfo();
    this->CreateNpcTypeInfo();
    //this->CreatePkTitleInfo();
    this->CreateEffectInfo();
    this->CreateMapMagicItemInfo();
    //this->CreateFlyObjInfo();
    //this->CreateTerrainWeatherInfo();
    this->CreateStatusEffectInfo();
    this->CreateUserHelpInfo();
    //this->CreateTerrainNpcTypeInfo();
    //this->CreateTipsInfo();
    this->CreateTextAction();
    this->CreatePetInfo();
    this->CreateLightLInfo();
    this->CreateRolePartLightInfo();
    this->Create3DItemInfo();
    this->CreateMedalInfo();
    this->CreateNobilityTitleInfo();
    this->CreateHonorTitleInfo();
    this->CreateEffectIndexInfo();
    this->CreateActionMapEffectInfo();
    this->CreateTrackInfo();
    this->CreateActionCtrlInfo();
    this->CreateActionDelayInfo();
    this->CreateWeaponInfo();
    this->CreateEffectLookSetInfo();
}

//---------------------------------------------------------------------
void C3DRoleData::DestroySoundInfo()
{
    m_setSoundInfo.clear();
}
//---------------------------------------------------------------------
void C3DRoleData::DestroyMonsterInfo()
{
    int nAmount = m_setMonsterInfo.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        CMonsterInfo* pInfo = m_setMonsterInfo[i];
        SAFE_DELETE(pInfo);
    }
    m_setMonsterInfo.clear();
}
//---------------------------------------------------------------------
BOOL C3DRoleData::CreateActionInfo()
{
    m_setActionInfo.clear();
    char szDataFile[] = "ini/Action.dat";
    FILE* fp = fopen(szDataFile, "rb");
    if (!fp)
    {
        ::ErrorOut("Open file %s error", szDataFile);
        return false;
    }
    DWORD dwAmount;
    fread(&dwAmount, sizeof(DWORD), 1, fp);
    // ActionInfo
    typedef struct
    {
        DWORD	dwCheck1;
        DWORD	dwActionIndex;
        DWORD	dwCheck2;
        DWORD	dwTimeInterval;
        DWORD	dwCheck3;
    } ActionFileImage;
    for(DWORD i = 0; i < dwAmount; i++)
    {
        ActionFileImage imageInfo;
        fread(&imageInfo, sizeof(ActionFileImage), 1, fp);
        DWORD dwCheck1 = imageInfo.dwActionIndex * 2 + imageInfo.dwTimeInterval + i;
        DWORD dwCheck2 = i * dwCheck1 + imageInfo.dwActionIndex * imageInfo.dwTimeInterval - i;
        DWORD dwCheck3 = dwCheck1 * imageInfo.dwActionIndex + dwCheck2 * 7 + imageInfo.dwTimeInterval * i;
        BOOL bCheckOk = true;
        if (dwCheck1 != imageInfo.dwCheck1)
        {
            bCheckOk = false;
        }
        if (dwCheck2 != imageInfo.dwCheck2)
        {
            bCheckOk = false;
        }
        if (dwCheck3 != imageInfo.dwCheck3)
        {
            bCheckOk = false;
        }
        if (!bCheckOk)
        {
            MYASSERT(false);
        }
        m_setActionInfo[imageInfo.dwActionIndex] = imageInfo.dwTimeInterval;
    }
    fclose(fp);
    return true;
}
//---------------------------------------------------------------------
void C3DRoleData::DestroyActionInfo()
{
    m_setActionInfo.clear();
}
//---------------------------------------------------------------------
DWORD C3DRoleData::GetActionTimeInterval(DWORD dwActionIndex)
{
    map<OBJID, DWORD>::iterator iter = m_setActionInfo.find(dwActionIndex);
    if (iter != m_setActionInfo.end())
    {
        return iter->second;
    }
    else
    {
        // LOOK Í¨Åä
        DWORD dwMyIndex = dwActionIndex % 1000000 + 999 * 1000000;
        iter = m_setActionInfo.find(dwMyIndex);
        if (iter != m_setActionInfo.end())
        {
            return iter->second;
        }
        else
        {
            // weapon Í¨Åä
            DWORD dwMyIndex = dwActionIndex % 1000 + 999 * 1000 + dwActionIndex / 1000000 * 1000000;
            iter = m_setActionInfo.find(dwMyIndex);
            if (iter != m_setActionInfo.end())
            {
                return iter->second;
            }
            else
            {
                // È«Í¨Åä
                DWORD dwMyIndex = dwActionIndex % 1000 + 999 * 1000 + 999 * 1000000;
                iter = m_setActionInfo.find(dwMyIndex);
                if (iter != m_setActionInfo.end())
                {
                    return iter->second;
                }
            }
        }
    }
    //MYASSERT(false);
    return 33;
}
//---------------------------------------------------------------------
BOOL C3DRoleData::CreateWeaponCombinInfo()
{
    const char szIniFile[] = "ini/WeaponCombin.ini";
    FILE* fp = fopen(szIniFile, "r");
    if (!fp)
    {
        return false;
    }
    while (true)
    {
        DWORD idxRWeapon = 0, idxLWeapon = 0;
        int rval = fscanf(fp, "%u.%u\n", &idxRWeapon, &idxLWeapon);
        if (rval == EOF)
        {
            break;
        }
        WeaponCombinInfo* pInfo = new WeaponCombinInfo;
        MYASSERT (pInfo);
        pInfo->idxRWeapon = idxRWeapon;
        pInfo->idxLWeapon = idxLWeapon;
        m_setWeaponCombinInfo.push_back(pInfo);
    }
    fclose(fp);
    return true;
}
//---------------------------------------------------------------------
void C3DRoleData::DestroyWeaponCombinInfo()
{
    int nAmount = m_setWeaponCombinInfo.size();
    for(int i = 0; i < nAmount; i++)
    {
        WeaponCombinInfo* pInfo = m_setWeaponCombinInfo[i];
        SAFE_DELETE(pInfo);
    }
    m_setWeaponCombinInfo.clear();
}
//---------------------------------------------------------------------
BOOL C3DRoleData::CanWeaponCombin(DWORD idxRWeapon, DWORD idxLWeapon)
{
    int nAmount = m_setWeaponCombinInfo.size();
    for(int i = 0; i < nAmount; i++)
    {
        WeaponCombinInfo* pInfo = m_setWeaponCombinInfo[i];
        if (pInfo->idxRWeapon == idxRWeapon &&
                pInfo->idxLWeapon == idxLWeapon)
        {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------
BOOL C3DRoleData::Create3DEffectInfo()
{
    m_setGame3DEffectInfo.clear();
    const char szIniFile[] = "ini/Action3DEffect.ini";
    FILE* fp = fopen(szIniFile, "r");
    if (!fp)
    {
        return false;
    }
    while (true)
    {
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        DWORD idxAction = 0, idxPart = 0, idxVar = 0, nLook = 0;
        char szEffect[256] = "";
        if (5 != sscanf(szLine, "%u.%u.%u.%u=%s", &nLook, &idxAction, &idxPart, &idxVar, szEffect))
        {
            ::ErrorMsg("invalid line [%s] found in %s.", szLine, szIniFile);
            continue;
        }
        __int64 id = nLook * 1000000000 + idxAction * 1000000 + idxPart * 1000 + idxVar;
        m_setGame3DEffectInfo[id] = szEffect;
    }
    fclose(fp);
    return true;
}

//---------------------------------------------------------------------
void C3DRoleData::Destroy3DEffectInfo()
{
    m_setGame3DEffectInfo.clear();
}

//---------------------------------------------------------------------
const char* C3DRoleData::GetRolePart3DEffect(DWORD dwLook, DWORD idxAction, DWORD idxPart, DWORD idxVar)
{
    __int64 id = dwLook * 1000000000 + idxAction * 1000000 + idxPart * 1000 + idxVar;
    map<__int64, string>::iterator iter = m_setGame3DEffectInfo.find(id);
    if (iter != m_setGame3DEffectInfo.end())
    {
        return iter->second.c_str();
    }
    // wildcard_LOOKTYPE_UNIVERSAL
    id = dwLook * 1000000000 + _ACTION_UNIVERSAL * 1000000 + idxPart * 1000 + idxVar;
    iter = m_setGame3DEffectInfo.find(id);
    if (iter != m_setGame3DEffectInfo.end())
    {
        return iter->second.c_str();
    }
    id = dwLook * 1000000000 + idxAction * 1000000 + _WEAPONTYPE_UNIVERSAL * 1000 + idxVar;
    iter = m_setGame3DEffectInfo.find(id);
    if (iter != m_setGame3DEffectInfo.end())
    {
        return iter->second.c_str();
    }
    id = dwLook * 1000000000 + idxAction * 1000000 + idxPart * 1000 + _VARIABLE_UNIVERSAL;
    iter = m_setGame3DEffectInfo.find(id);
    if (iter != m_setGame3DEffectInfo.end())
    {
        return iter->second.c_str();
    }
    return NULL;
}


//---------------------------------------------------------------------
void C3DRoleData::CreateSoundInfo(void)
{
    const char szIniFile[] = "ini/ActionSound.ini";
    FILE* fp = fopen(szIniFile, "r");
    IF_NOT (fp)
    return;
    while (true)
    {
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            fclose(fp);
            return;
        }
        DWORD idxLook, idxAction, idxWeapon, dwTerrain;
        char szSound[256] = "";
        if (5 != sscanf(szLine, "%u.%u.%u.%u=%s", &dwTerrain, &idxLook, &idxWeapon, &idxAction, szSound))
        {
            fclose(fp);
            ::ErrorOut("error line:[%s] in %s!", szLine, szIniFile);
            return;
        }
        MYASSERT (strlen(szSound) < 128);
        // new sound info
        __int64 id = dwTerrain * 1000000000 + idxLook * 1000000 + idxWeapon * 1000 + idxAction;
        m_setSoundInfo[id] = szSound;
    }
}

//---------------------------------------------------------------------
const char* C3DRoleData::GetActionSound(DWORD dwTerrain, DWORD idxLook, DWORD idxWeapon, DWORD idxAction)
{
    if (idxAction == _ACTION_STANDBY)
    {
        return NULL;
    }
    __int64 id = dwTerrain * 1000000000 + idxLook * 1000000 + idxWeapon * 1000 + idxAction;
    map<__int64, string>::iterator iter = m_setSoundInfo.find(id);
    if (iter != m_setSoundInfo.end())
    {
        return iter->second.c_str();
    }
    id = dwTerrain * 1000000000 + idxLook * 1000000 + 999 * 1000 + idxAction;
    iter = m_setSoundInfo.find(id);
    if (iter != m_setSoundInfo.end())
    {
        return iter->second.c_str();
    }
    id = idxLook * 1000000 + idxWeapon * 1000 + idxAction;
    iter = m_setSoundInfo.find(id);
    if (iter != m_setSoundInfo.end())
    {
        return iter->second.c_str();
    }
    id = idxLook * 1000000 + 999 * 1000 + idxAction;
    iter = m_setSoundInfo.find(id);
    if (iter != m_setSoundInfo.end())
    {
        return iter->second.c_str();
    }
    return NULL;
}

//---------------------------------------------------------------------
BOOL C3DRoleData::GetTransformJumpAble(int nLook)
{
    int nAmount = m_setTransFormInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        if (m_setTransFormInfo[i].nLook == nLook)
        {
            return m_setTransFormInfo[i].bCanJump;
        }
    }
    return false;
}
//---------------------------------------------------------------------
int C3DRoleData::GetTransformAdditiveSize(int nLook)
{
    int nAmount = m_setTransFormInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        if (m_setTransFormInfo[i].nLook == nLook)
        {
            return m_setTransFormInfo[i].nAddSize;
        }
    }
    return 0;
}
//---------------------------------------------------------------------

BOOL C3DRoleData::CreateTransFormInfo()
{
    this->DestroyTransFormInfo();
    m_setTransFormInfo.clear();
    CONST char INI_FILE[] = "ini/AdditiveSize.ini";
    char szTitle[64];
    char szSubtitle[64];
    // get header info
    strcpy(szTitle, "Header");
    strcpy(szSubtitle, "Amount");
    int nAmount = 0;
    ::IniDataGet(INI_FILE, szTitle, szSubtitle, nAmount, true);
    for(int i = 0; i < nAmount; i ++)
    {
        // get Index
        int nIndex = 0;
        strcpy(szTitle, "Header");
        sprintf(szSubtitle, "Index%d", i);
        ::IniDataGet(INI_FILE, szTitle, szSubtitle, nIndex, true);
        // get look ...
        sprintf(szTitle, "Transform%d", nIndex);
        strcpy(szSubtitle, "Look");
        int nLook;
        ::IniDataGet(INI_FILE, szTitle, szSubtitle, nLook, true);
        // get size ...
        int nSize;
        strcpy(szSubtitle, "AdditiveSize");
        ::IniDataGet(INI_FILE, szTitle, szSubtitle, nSize, true);
        CTransformInfo info;
        info.nLook = nLook;
        info.nAddSize = nSize;
        strcpy(szSubtitle, "Jump");
        char szJump[256];
        ::IniStrGet(INI_FILE, szTitle, szSubtitle, szJump, true);
        if (strcmp(szJump, "ON") == 0)
        {
            info.bCanJump = true;
        }
        else
        {
            info.bCanJump = false;
        }
        int nScale;
        strcpy(szSubtitle, "Scale");
        ::IniDataGet(INI_FILE, szTitle, szSubtitle, nScale, true);
        info.nScale = nScale;
        info.nIndex = nIndex;
        m_setTransFormInfo.push_back(info);
    }
    return true;
}
//---------------------------------------------------------------------
void C3DRoleData::DestroyTransFormInfo()
{
    m_setTransFormInfo.clear();
}
//---------------------------------------------------------------------
CTransformInfo* C3DRoleData::GetTransformInfo(int nIndex)
{
    int nAmount = m_setTransFormInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        if (m_setTransFormInfo[i].nIndex == nIndex)
        {
            return &m_setTransFormInfo[i];
        }
    }
    return NULL;
}
//---------------------------------------------------------------------

BOOL C3DRoleData::CreateMonsterInfo(void)
{
    CONST CHAR INI_FILE[] = "ini/Monster.ini";
    FILE* fp = fopen(INI_FILE, "r");
    if (!fp)
    {
        ::ErrorOut("error on open %s.", INI_FILE);
        return false;
    }
    while(true)
    {
        // scan title
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        DWORD dwType = 0;
        if (1 != sscanf(szLine, "[%d]", &dwType))
        {
            continue;
        }
        if (dwType == 0 )
        {
            continue;
        }
        // get info ...
        CMonsterInfo* pInfo = new CMonsterInfo;
        if (!pInfo)
        {
            break;
        }
        //memset(pInfo, 0L, sizeof(CMonsterInfo));
        BOOL bSucRead = true;
        char szTitle[_MAX_STRING] = "";
        pInfo->idType = dwType;
        if (!fscanf(fp, "Name=%s\n", pInfo->szMonster))
        {
            strcpy(pInfo->szMonster, "NULL");
        }
        bSucRead &= fscanf(fp, "SizeAdd=%d\n", &pInfo->nAddSize);
        bSucRead &= fscanf(fp, "ZoomPercent=%d\n", &pInfo->nZoomPercent);
        bSucRead &= fscanf(fp, "MaxLife=%d\n", &pInfo->nMaxLife);
        bSucRead &= fscanf(fp, "Level=%d\n", &pInfo->nLevel);
        bSucRead &= fscanf(fp, "WhiteBlack=%d\n", &pInfo->nWhiteBlack);
        int nData;
        bSucRead &= fscanf(fp, "Defy=%d\n", &nData);
        bSucRead &= fscanf(fp, "Defence=%d\n", &nData);
        bSucRead &= fscanf(fp, "MDefence=%d\n", &nData);
        if (!fscanf(fp, "DisappearEffect=%s\n", pInfo->szDisappearEffect))
        {
            strcpy(pInfo->szDisappearEffect, "NULL");
            strcpy(pInfo->szDisappearSound, "NULL");
        }
        else if (!fscanf(fp, "DisappearSound=%s\n", pInfo->szDisappearSound))
        {
            strcpy(pInfo->szDisappearSound, "NULL");
        }
        if (!bSucRead)
        {
            ::ErrorOut("error data: %s of %s", szTitle, INI_FILE);
            break;
        }
        m_setMonsterInfo.push_back(pInfo);
    }
    fclose(fp);
    return true;
}

//---------------------------------------------------------------------
const CMonsterInfo* C3DRoleData::GetMonsterInfo(const char* pszMonster)
{
    if (!pszMonster && strlen(pszMonster) > 16)
    {
        return NULL;
    }
    int nInfoAmount = m_setMonsterInfo.size();
    for (int i = nInfoAmount - 1; i >= 0; i--)
    {
        CMonsterInfo* pInfo = m_setMonsterInfo[i];
        if (pInfo && 0 == strcmp(pszMonster, pInfo->szMonster))
        {
            return pInfo;
        }
    }
    return NULL;
}
//---------------------------------------------------------------------
const CMonsterInfo* C3DRoleData::GetMonsterInfo(DWORD idType)
{
    if (idType == 0)
    {
        return NULL;
    }
    int nInfoAmount = m_setMonsterInfo.size();
    for (int i = nInfoAmount - 1; i >= 0; i--)
    {
        CMonsterInfo* pInfo = m_setMonsterInfo[i];
        if (pInfo && idType == pInfo->idType)
        {
            return pInfo;
        }
    }
    return NULL;
}

//---------------------------------------------------------------------
BOOL C3DRoleData::CreateActionMapEffectInfo()
{
    const char szIniFile[] = "ini/ActionMap3DEffect.ini";
    FILE* fp = fopen(szIniFile, "r");
    if (!fp)
    {
        ::ErrorOut("error on open %s.", szIniFile);
        return false;
    }
    while(true)
    {
        // scan title
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        // get info ...
        char szTitle[_MAX_STRING] = "";
        if (1 != sscanf(szLine, "[%s]", szTitle))
        {
            continue;
        }
        ActionMapEffectInfo* pInfo = new ActionMapEffectInfo;
        MYASSERT (pInfo);
        BOOL bSucRead = true;
        bSucRead &= fscanf(fp, "Index=%d\n", &pInfo->idx);
        bSucRead &= fscanf(fp, "Effect=%s\n", &pInfo->szEffect);
        bSucRead &= fscanf(fp, "ShowTime=%d\n", &pInfo->nShowTime);
        bSucRead &= fscanf(fp, "Rotate=%d\n", &pInfo->bRotate);
        if (!fscanf(fp, "Type=%d\n", &pInfo->nType))
        {
            pInfo->nType = 0;
        }
        if (!bSucRead)
        {
            ::ErrorOut("error data: %s of %s", szTitle, szIniFile);
            break;
        }
        // keep it
        m_setMapEffectInfo.push_back(pInfo);
    }
    fclose(fp);
    return true;
}

//---------------------------------------------------------------------
void C3DRoleData::DestroyActionMapEffectInfo()
{
    int nInfos = m_setMapEffectInfo.size();
    for (int i = nInfos - 1; i >= 0; i--)
    {
        ActionMapEffectInfo* pInfo = m_setMapEffectInfo[i];
        SAFE_DELETE(pInfo);
    }
    m_setMapEffectInfo.clear();
}

//---------------------------------------------------------------------
const ActionMapEffectInfo* C3DRoleData::GetActionMapEffect (int nLook, int nAction, int nTerrain) const
{
    int nInfos = m_setMapEffectInfo.size();
    for (int i = nInfos - 1; i >= 0; i--)
    {
        ActionMapEffectInfo* pInfo = m_setMapEffectInfo[i];
        MYASSERT (pInfo);
        DWORD idx = nLook * 1000000 + nAction * 1000 + nTerrain;
        if (pInfo->idx == idx)
        {
            return pInfo;
        }
        // wildcard
        const int nWildCard = 999;
        idx = nWildCard * 1000000 + nAction * 1000 + nTerrain;
        if (pInfo->idx == idx)
        {
            return pInfo;
        }
        idx = nLook * 1000000 + nWildCard * 1000 + nTerrain;
        if (pInfo->idx == idx)
        {
            return pInfo;
        }
        idx = nLook * 1000000 + nAction * 1000 + nWildCard;
        if (pInfo->idx == idx)
        {
            return pInfo;
        }
    }
    return NULL;
}

//---------------------------------------------------------------------
void C3DRoleData::UpdateMy3DEffectInfo()
{
    CONST CHAR INI_FILE[] = "ini/3DEffect.ini";
    FILE* fp = fopen(INI_FILE, "w");
    if (!fp)
    {
        ::ErrorOut("error on open %s.", INI_FILE);
        return;
    }
    for(int i = 0; i < (int)m_setMy3DEffectInfo.size(); i ++)
    {
        CMy3DEffectInfo* pInfo = m_setMy3DEffectInfo[i];
        fprintf(fp, "[%s]\n", pInfo->szIndex);
        fprintf(fp, "Amount=%u\n", pInfo->nAmount);
        for (int j = 0; j < pInfo->nAmount; j++)
        {
            fprintf(fp, "EffectId%d=%u\n", j, pInfo->idEffect[j]);
            fprintf(fp, "TextureId%d=%u\n", j, pInfo->idTexture[j]);
            fprintf(fp, "ASB%d=%d\n", j, pInfo->nASB[j]);
            fprintf(fp, "ADB%d=%d\n", j, pInfo->nADB[j]);
        }
        fprintf(fp, "Delay=%d\n", pInfo->nDelay);
        fprintf(fp, "LoopTime=%d\n", pInfo->nLoopTime);
        fprintf(fp, "FrameInterval=%d\n", pInfo->nFrameInterval);
        fprintf(fp, "LoopInterval=%d\n", pInfo->nLoopInterval);
        fprintf(fp, "OffsetX=%d\n", pInfo->nOffsetX);
        fprintf(fp, "OffsetY=%d\n", pInfo->nOffsetY);
        fprintf(fp, "OffsetZ=%d\n", pInfo->nOffsetZ);
        fprintf(fp, "\n");
        fprintf(fp, "\n");
        fprintf(fp, "\n");
    }
    fclose(fp);
}
//---------------------------------------------------------------------
void C3DRoleData::CreateMy3DEffectInfo()
{
    CONST CHAR INI_FILE[] = "ini/3DEffect.ini";
    FILE* fp = fopen(INI_FILE, "r");
    if (!fp)
    {
        ::ErrorOut("error on open %s.", INI_FILE);
        return;
    }
    while(true)
    {
        // scan title
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        char szTitle[_MAX_STRING] = "";
        if (1 != sscanf(szLine, "[%s]", szTitle))
        {
            continue;
        }
        szTitle[strlen(szTitle)-1] = '\0';
        // get info ...
        CMy3DEffectInfo* pInfo = new CMy3DEffectInfo();
        MYASSERT (pInfo);
        //memset(pInfo, 0L, sizeof(CMy3DEffectInfo));
        strcpy(pInfo->szIndex, szTitle);
        BOOL bSuc = true;
        bSuc &= fscanf(fp, "Amount=%d\n", &pInfo->nAmount);
        if (!bSuc)
        {
            ::ErrorOut("error part amount data: %s of %s", szTitle, INI_FILE);
            break;
        }
        for (int i = 0; i < pInfo->nAmount; i++)
        {
            char szIndex[256] = "";
            sprintf(szIndex, "EffectId%d=%s\n", i, "%d");
            bSuc &= fscanf(fp, szIndex, &pInfo->idEffect[i]);
            sprintf(szIndex, "TextureId%d=%s\n", i, "%d");
            bSuc &= fscanf(fp, szIndex, &pInfo->idTexture[i]);
            sprintf(szIndex, "ASB%d=%s\n", i, "%d");
            if (!fscanf(fp, szIndex, &pInfo->nASB[i]))
            {
                pInfo->nASB[i] = 5;
            }
            sprintf(szIndex, "ADB%d=%s\n", i, "%d");
            if (!fscanf(fp, szIndex, &pInfo->nADB[i]))
            {
                pInfo->nADB[i] = 6;
            }
        }
        bSuc &= fscanf(fp, "Delay=%d\n", &pInfo->nDelay);
        bSuc &= fscanf(fp, "LoopTime=%d\n", &pInfo->nLoopTime);
        bSuc &= fscanf(fp, "FrameInterval=%d\n", &pInfo->nFrameInterval);
        bSuc &= fscanf(fp, "LoopInterval=%d\n", &pInfo->nLoopInterval);
        bSuc &= fscanf(fp, "OffsetX=%d\n", &pInfo->nOffsetX);
        bSuc &= fscanf(fp, "OffsetY=%d\n", &pInfo->nOffsetY);
        if (!fscanf(fp, "OffsetZ=%d\n", &pInfo->nOffsetZ))
        {
            pInfo->nOffsetZ = 0;
        }
        if (!fscanf(fp, "ShapeAir=%d\n", &pInfo->nShapeAir) )
        {
            pInfo->nShapeAir = 0;
        }
        if (!bSuc)
        {
            ::ErrorOut("error data: %s of %s", szTitle, INI_FILE);
            break;
        }
        m_setMy3DEffectInfo.push_back(pInfo);
    }
    fclose(fp);
}

//---------------------------------------------------------------------
const CMy3DEffectInfo* C3DRoleData::GetMy3DEffectInfo(const char* pszIndex)
{
    if (!pszIndex)
    {
        return false;
    }
    int nAmount = m_setMy3DEffectInfo.size();
    for (int i = nAmount - 1; i >= 0; i--)
    {
        CMy3DEffectInfo* pInfo = m_setMy3DEffectInfo[i];
        if (pInfo && 0 == strcmp(pInfo->szIndex, pszIndex))
        {
            return pInfo;
        }
    }
    return NULL;
}

//---------------------------------------------------------------------
void C3DRoleData::DestroyMy3DEffectInfo()
{
    int nAmount = m_setMy3DEffectInfo.size();
    for (int i = nAmount - 1; i >= 0; i--)
    {
        CMy3DEffectInfo* pInfo = m_setMy3DEffectInfo[i];
        SAFE_DELETE (pInfo);
    }
    m_setMy3DEffectInfo.clear();
}
//---------------------------------------------------------------------
void C3DRoleData::CreateEffectLookSetInfo()
{
    const char szIniFile[] = "ini/EffectLookSet.ini";
    FILE* fp = fopen(szIniFile, "r");
    if (fp)
    {
        char szLine[1024];
        while(NULL != fgets(szLine, 1023, fp))
        {
            if (strlen(szLine) <= 0)
            {
                continue;
            }
            if (szLine[0] == ';')
            {
                continue;
            }
            char szTitle[_MAX_STRING] = "";
            if (1 != sscanf(szLine, "[%s]", szTitle))
            {
                continue;
            }
            BOOL bSuc = true;
            EffectLookSetInfo* pInfo = new EffectLookSetInfo;
            bSuc &= fscanf(fp, "Look=%d\n", &pInfo->dwLook);
            bSuc &= fscanf(fp, "DefaultX=%d\n", &pInfo->nDefaultX);
            bSuc &= fscanf(fp, "DefaultY=%d\n", &pInfo->nDefaultY);
            bSuc &= fscanf(fp, "DefaultZ=%d\n", &pInfo->nDefaultZ);
            bSuc &= fscanf(fp, "Amount=%d\n", &pInfo->nAmount);
            if (!bSuc)
            {
                ::ErrorOut("error data: %s of %s", szTitle, szIniFile);
                break;
            }
            for(int i = 0; i < pInfo->nAmount; i++)
            {
                char szIndex[256] = "";
                char strEffect[64] = "";
                int nX, nY, nZ;
                nX = 0;
                nY = 0;
                nZ = 0;
                sprintf(szIndex, "Effect%d=%s\n", i, "%s");
                bSuc &= fscanf(fp, szIndex, strEffect);
                bSuc &= fscanf(fp, "X=%d\n", &nX);
                if (!bSuc)
                {
                    ::ErrorOut("error X: Effect%d of %s %s", i, szIniFile, szTitle);
                    return;
                }
                bSuc &= fscanf(fp, "Y=%d\n", &nY);
                if (!bSuc)
                {
                    ::ErrorOut("error Y: Effect%d of %s %s", i, szIniFile, szTitle);
                    return;
                }
                bSuc &= fscanf(fp, "Z=%d\n", &nZ);
                if (!bSuc)
                {
                    ::ErrorOut("error Z: Effect%d of %s %s", i, szIniFile, szTitle);
                    return;
                }
                else
                {
                    EffectLookSet pLookSet;
                    pLookSet.nX = nX;
                    pLookSet.nY = nY;
                    pLookSet.nZ = nZ;
                    strcpy(pLookSet.szEffect, strEffect);
                    pInfo->setEffect.push_back(pLookSet);
                }
            }
            m_setEffectLookSetInfo[pInfo->dwLook] = pInfo;
        }
        fclose(fp);
    }
    else
    {
        ::ErrorOut("error on open %s.", szIniFile);
    }
}
//---------------------------------------------------------------------
BOOL C3DRoleData::GetEffectLookSetInfo(DWORD dwLook , char* szEffect , int& x, int& y, int& z)
{
    map<OBJID, EffectLookSetInfo*>::iterator iter = m_setEffectLookSetInfo.find(dwLook);
    if (iter != m_setEffectLookSetInfo.end())
    {
        EffectLookSetInfo* pInfo = iter->second;
        if (pInfo)
        {
            int nAmount = pInfo->nAmount;
            if (nAmount == 0)
            {
                x = pInfo->nDefaultX;
                y = pInfo->nDefaultY;
                z = pInfo->nDefaultZ;
                return TRUE;
            }
            for(int i = 0; i < nAmount; i++)
            {
                if (strcmp(szEffect, pInfo->setEffect[i].szEffect) == 0)
                {
                    x = pInfo->setEffect[i].nX;
                    y = pInfo->setEffect[i].nY;
                    z = pInfo->setEffect[i].nZ;
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}
//---------------------------------------------------------------------
void C3DRoleData::DestroyEffectLookSetInfo()
{
    map<OBJID, EffectLookSetInfo*>::iterator iter = m_setEffectLookSetInfo.begin();
    while(iter != m_setEffectLookSetInfo.end())
    {
        EffectLookSetInfo* pInfo = iter->second;
        pInfo->setEffect.clear();
        delete iter->second;
        iter ++;
    }
    m_setEffectLookSetInfo.clear();
}
//---------------------------------------------------------------------
void C3DRoleData::DestroyRolePartInfo()
{
    this->DestroyRolePartInfo(m_setArmor);
    this->DestroyRolePartInfo(m_setArmet);
    this->DestroyRolePartInfo(m_setWeapon);
    this->DestroyRolePartInfo(m_setMantle);
    this->DestroyRolePartInfo(m_setMount);
    this->DestroyRolePartInfo(m_setHead);
}
//---------------------------------------------------------------------
void C3DRoleData::CreateRolePartInfo()
{
    this->DestroyRolePartInfo();
    this->CreateRolePartInfo(m_setHead, "ini/Head.ini");
    this->CreateRolePartInfo(m_setArmor, "ini/armor.ini");
    this->CreateRolePartInfo(m_setArmet, "ini/armet.ini");
    this->CreateRolePartInfo(m_setWeapon, "ini/weapon.ini");
    this->CreateRolePartInfo(m_setMantle, "ini/Mantle.ini");
    this->CreateRolePartInfo(m_setMount, "ini/mount.ini");
}
//---------------------------------------------------------------------
void C3DRoleData::CreateRolePartInfo(MAP_ROLEPARTINFO& m_setInfo, const char* pszFilename)
{
    if (!pszFilename)
    {
        return;
    }
    this->DestroyRolePartInfo(m_setInfo);
    //---------------------------------------
    FILE* fp = fopen((char*)pszFilename, "r");
    if (!fp)
    {
        return;
    }
    char szLine[1024];
    char szTitle[_MAX_STRING];
    while(true)
    {
        // scan title
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            fclose(fp);
            return;
        }
        if (1 != sscanf(szLine, "[%s]", szTitle))
        {
            continue;
        }
        szTitle[strlen(szTitle)-1] = '\0';
        // get info ...
        OBJID idMesh = 0, idTexture = 0, idTexture2 = 0;
        float fMoveRateX = 0.0f, fMoveRateY = 0.0f;
        fscanf(fp, "Mesh=%u\n", &idMesh);
        fscanf(fp, "Texture=%u\n", &idTexture);
        if (1 == fscanf(fp, "Texture2=%u\n", &idTexture2))
        {
            fscanf(fp, "MoveRateX=%f\n", &fMoveRateX);
            fscanf(fp, "MoveRateY=%f\n", &fMoveRateY);
        }
        static RolePartInfo infoRolePart;
        infoRolePart.idMesh = idMesh;
        infoRolePart.idTexture = idTexture;
        infoRolePart.fMoveRateX = fMoveRateX;
        infoRolePart.fMoveRateY = fMoveRateY;
        OBJID id = RolePartInfoStr2ID(szTitle);
        m_setInfo[id] = infoRolePart;
    }
    return;
}

//---------------------------------------------------------------------
void C3DRoleData::DestroyRolePartInfo(MAP_ROLEPARTINFO& m_setInfo)
{
    m_setInfo.clear();
}
//---------------------------------------------------------------------
RolePartInfo* C3DRoleData::GetRoleRartInfo(MAP_ROLEPARTINFO& m_setInfo, const char* pszTitle)
{
    if (!pszTitle)
    {
        return NULL;
    }
    OBJID id = RolePartInfoStr2ID(pszTitle);
    map<OBJID, RolePartInfo>::iterator iter = m_setInfo.find(id);
    if (iter != m_setInfo.end())
    {
        return &iter->second;
    }
    return NULL;
}

//---------------------------------------------------------------------
RolePartInfo* C3DRoleData::GetRoleRartInfo(int nRolePartIndex, const char* pszTitle)
{
    switch(nRolePartIndex)
    {
    case ROLEPARTINFO_WEAPON:
            return this->GetRoleRartInfo(m_setWeapon, pszTitle);
    case ROLEPARTINFO_ARMET:
            return this->GetRoleRartInfo(m_setArmet, pszTitle);
    case ROLEPARTINFO_AEMOR:
            return this->GetRoleRartInfo(m_setArmor, pszTitle);
    case ROLEPARTINFO_MANTLE:
            return this->GetRoleRartInfo(m_setMantle, pszTitle);
    case ROLEPARTINFO_MOUNT:
            return  this->GetRoleRartInfo(m_setMount, pszTitle);
    case ROLEPARTINFO_HEAD:
            return  this->GetRoleRartInfo(m_setHead, pszTitle);
    default:
            return NULL;
    }
}
//---------------------------------------------------------------------
void C3DRoleData::LoadCommonIni()
{
    const char szIniFile[] = "ini/Common.ini";
    char szTitle[64];
    char szSubtitle[64];
    // face amount
    strcpy(szTitle, "Playerface");
    strcpy(szSubtitle, "MaleFaceAmount");
    MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, m_nMaleFaceAmount));
    strcpy(szSubtitle, "FemaleFaceAmount");
    MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, m_nFemaleFaceAmount));
    strcpy(szTitle, "EmblemAmount");
    strcpy(szSubtitle, "EmblemAmount");
    MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, m_nEmblemAmount));
    strcpy(szTitle, "Statuary");
    strcpy(szSubtitle, "LeaderScale");
    MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, m_nLeaderScale));
    strcpy(szSubtitle, "LeaderMaxLife");
    MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, m_nLeaderMaxLife));
    strcpy(szSubtitle, "AssistantScale");
    MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, m_nAssistantScale));
    strcpy(szSubtitle, "AssistantMaxLife");
    MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, m_nAssistantMaxLife));
    strcpy(szSubtitle, "SubLeaderScale");
    MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, m_nSubLeaderScale));
    strcpy(szSubtitle, "SubLeaderMaxLife");
    MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, m_nSubLeaderMaxLife));
}

//---------------------------------------------------------------------
void C3DRoleData::Create3DSimpleObjInfo()
{
    CONST CHAR INI_FILE[] = "ini/3DSimpleObj.ini";
    FILE* fp = fopen(INI_FILE, "r");
    if (!fp)
    {
        ::ErrorOut("error on open %s.", INI_FILE);
        return;
    }
    while(true)
    {
        // scan title
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        char szTitle[_MAX_STRING] = "";
        if (1 != sscanf(szLine, "[%s]", szTitle))
        {
            continue;
        }
        szTitle[strlen(szTitle)-1] = '\0';
        // get info ...
        C3DSimpleObjInfo* pInfo = new C3DSimpleObjInfo;
        MYASSERT(pInfo);
        memset(pInfo, 0L, sizeof(C3DSimpleObjInfo));
        pInfo->idType = atoi(szTitle + strlen("ObjIDType"));
        BOOL bSuc = true;
        bSuc &= fscanf(fp, "PartAmount=%d\n", &(pInfo->nParts));
        if (!bSuc || pInfo->nParts > _MAXPART_SIMPLEOBJ)
        {
            ::ErrorOut("error part amount data: %s of %s, must <= %d", szTitle, INI_FILE, _MAXPART_SIMPLEOBJ);
            break;
        }
        for (int i = 0; i < pInfo->nParts; i++)
        {
            char szIndex[256] = "";
            sprintf(szIndex, "Part%d=%s\n", i, "%d");
            bSuc &= fscanf(fp, szIndex, &(pInfo->idMesh[i]));
            sprintf(szIndex, "Texture%d=%s\n", i, "%d");
            bSuc &= fscanf(fp, szIndex, &(pInfo->idTexture[i]));
            if (!bSuc)
            {
                ::ErrorOut("error data: %s of %s, must <= %d", szTitle, INI_FILE);
                break;
            }
        }
        m_vct3DSimpleObjInfo.push_back(pInfo);
    }
    fclose(fp);
}

//---------------------------------------------------------------------
C3DSimpleObjInfo* C3DRoleData::Get3DSimpleObjInfo(OBJID idType)
{
    int nInfos = m_vct3DSimpleObjInfo.size();
    for (int i = 0; i < nInfos; i++)
    {
        C3DSimpleObjInfo* pInfo = m_vct3DSimpleObjInfo[i];
        if (pInfo && idType == pInfo->idType)
        {
            return pInfo;
        }
    }
    ::ErrorMsg("3DSimpleObj [%u] not found!", idType);
    return m_vct3DSimpleObjInfo[0];
}

//---------------------------------------------------------------------
void C3DRoleData::DestroySimpleObjInfo()
{
    int nInfos = m_vct3DSimpleObjInfo.size();
    for (int i = 0; i < nInfos; i++)
    {
        SAFE_DELETE(m_vct3DSimpleObjInfo[i]);
    }
    m_vct3DSimpleObjInfo.clear();
}
//---------------------------------------------------------------------
BOOL	C3DRoleData::Create2DNpcTypeInfo()
{
    return true;
}
//---------------------------------------------------------------------
void	C3DRoleData::Destroy2DNpcTypeInfo()
{
    /*	int nAmount = m_set2DNpcType.size();
    	for(int i = 0; i < nAmount; i ++)
    	{
    		Npc2DTypeInfo infoNpc = m_set2DNpcType[i];
    		for(int i = 0; i < nAmount; i ++)
    		{

    		}
    	}*/
}
//---------------------------------------------------------------------
BOOL	C3DRoleData::CreateTerrainNpcTypeInfo()
{
    CONST CHAR INI_FILE[] = "ini/TerrainNpc.ini";
    FILE* fp = fopen(INI_FILE, "r");
    if (!fp)
    {
        ::ErrorOut("error on open %s.", INI_FILE);
        return false;
    }
    while(true)
    {
        // scan title
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        char szTitle[_MAX_STRING] = "";
        if (1 != sscanf(szLine, "[%s]", szTitle))
        {
            continue;
        }
        szTitle[strlen(szTitle)-1] = '\0';
        // get info ...
        TerrainNpcTypeInfo* pInfo = new TerrainNpcTypeInfo;
        MYASSERT(pInfo);
        pInfo->nType = atoi(szTitle + strlen("NpcType"));
        BOOL bSuc = true;
        bSuc &= fscanf(fp, "Name=%s\n", pInfo->szNpcName);
        for(int i = 0; i < 8; i ++)
        {
            char szFile[_MAX_PATH];
            int nDir;
            fscanf(fp, "Dir%d=%s\n", &nDir, szFile);
            string str = szFile;
            pInfo->setStr.push_back(str);
        }
        if (!bSuc)
        {
            ::ErrorOut("error data: %s of %s", szTitle, INI_FILE);
            break;
        }
        m_setTerrainNpcTypeInfo.push_back(pInfo);
    }
    fclose(fp);
    return true;
}
//---------------------------------------------------------------------
void	C3DRoleData::DestroyTerrainNpcTypeInfo()
{
    int nAmount = m_setTerrainNpcTypeInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        TerrainNpcTypeInfo* pInfo = m_setTerrainNpcTypeInfo[i];
        if (pInfo)
        {
            pInfo->setStr.clear();
            SAFE_DELETE(pInfo);
        }
    }
    m_setTerrainNpcTypeInfo.clear();
}
//---------------------------------------------------------------------
BOOL C3DRoleData::CreateNpcTypeInfo()
{
    CONST CHAR INI_FILE[] = "ini/Npc.ini";
    FILE* fp = fopen(INI_FILE, "r");
    if (!fp)
    {
        ::ErrorOut("error on open %s.", INI_FILE);
        return false;
    }
    while(true)
    {
        // scan title
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        char szTitle[_MAX_STRING] = "";
        if (1 != sscanf(szLine, "[%s]", szTitle))
        {
            continue;
        }
        szTitle[strlen(szTitle)-1] = '\0';
        // get info ...
        NpcTypeInfo* pInfo = new NpcTypeInfo;
        MYASSERT(pInfo);
        pInfo->nType = atoi(szTitle + strlen("NpcType"));
        BOOL bSuc = true;
        bSuc &= fscanf(fp, "Name=%s\n", pInfo->szNpcName);
        bSuc &= fscanf(fp, "SimpleObjID=%u\n", &pInfo->idSimpleObj);
        bSuc &= fscanf(fp, "StandByMotion=%u\n", &pInfo->idMotionStandBy);
        bSuc &= fscanf(fp, "BlazeMotion=%u\n", &pInfo->idMotionBlaze);
        bSuc &= fscanf(fp, "BlazeMotion1=%u\n", &pInfo->idMotionBlaze1);
        bSuc &= fscanf(fp, "BlazeMotion2=%u\n", &pInfo->idMotionBlaze2);
        bSuc &= fscanf(fp, "RestMotion=%u\n", &pInfo->idMotionRest);
        bSuc &= fscanf(fp, "Effect=%s\n", pInfo->szEffectIndex);
        pInfo->nZoomPercent = 100;
        strcpy(pInfo->szNpcTitle, "");
        fscanf(fp, "ZoomPercent=%d\n", &pInfo->nZoomPercent);
        fscanf(fp, "Note=%s\n", &pInfo->szNpcTitle);
        if (strcmp(pInfo->szNpcTitle, "NULL") == 0)
        {
            strcpy(pInfo->szNpcTitle, "");
        }
        pInfo->bMouseSign = 1;
        fscanf(fp, "MouseSign=%d\n", &pInfo->bMouseSign);
        pInfo->bChangeDir = true;
        fscanf(fp, "ChangeDir=%d\n", &pInfo->bChangeDir);
        pInfo->nFront = pInfo->nBack = pInfo->nLeft = pInfo->nRight = 0;
        fscanf(fp, "FrontBlock=%d\n", &pInfo->nFront);
        fscanf(fp, "BackBlock=%d\n", &pInfo->nBack);
        fscanf(fp, "LeftBlock=%d\n", &pInfo->nLeft);
        fscanf(fp, "RightBlock=%d\n", &pInfo->nRight);
        if (!bSuc)
        {
            ::ErrorOut("error data: %s of %s", szTitle, INI_FILE);
            break;
        }
        m_setNpcType.push_back(pInfo);
    }
    fclose(fp);
    return true;
}
//---------------------------------------------------------------------
void C3DRoleData::DestroyNpcTypeInfo()
{
    int nAmount = m_setNpcType.size();
    for(int i = 0; i < nAmount; i ++)
    {
        SAFE_DELETE(m_setNpcType[i]);
    }
    m_setNpcType.clear();
}
//---------------------------------------------------------------------
NpcTypeInfo* C3DRoleData::GetNpcTypeInfo(OBJID idType)
{
    int nAmount = m_setNpcType.size();
    for(int i = 0; i < nAmount; i ++)
    {
        NpcTypeInfo* pInfo = m_setNpcType[i];
        if (pInfo && pInfo->nType == idType)
        {
            return pInfo;
        }
    }
    return m_setNpcType[0];
}
//---------------------------------------------------------------------
BOOL	C3DRoleData::CreatePkTitleInfo()
{
    const char szIniFile[] = "ini/Common.ini";
    char szTitle[] = "PkTitle";
    char szSubtitle[64];
    // get amount
    int nAmount = 0;
    strcpy(szSubtitle, "Amount");
    MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, nAmount));
    for(int i = 0; i < nAmount; i ++)
    {
        PkTitleInfo* pInfoPkTitle = new PkTitleInfo;
        MYASSERT(pInfoPkTitle);
        sprintf(szSubtitle, "PkValueMax%d", i);
        MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, pInfoPkTitle->nMaxPkValue));
        sprintf(szSubtitle, "PkValueMin%d", i);
        MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, pInfoPkTitle->nMinPkValue));
        sprintf(szSubtitle, "PkTitle%d", i);
        MYASSERT(::IniStrGet(szIniFile, szTitle, szSubtitle, pInfoPkTitle->szTitle));
        m_setPkTitle.push_back(pInfoPkTitle);
    }
    return true;
}
//---------------------------------------------------------------------
void	C3DRoleData::DestroyPkTitleInfo()
{
    int nAmount = m_setPkTitle.size();
    for(int i = 0; i < nAmount; i ++)
    {
        SAFE_DELETE(m_setPkTitle[i]);
    }
    m_setPkTitle.clear();
}
//---------------------------------------------------------------------
char* C3DRoleData::GetPkTitle(int nPkValue)
{
    for(int i = 0; i < (int)m_setPkTitle.size(); i ++)
    {
        PkTitleInfo* pInfo = m_setPkTitle[i];
        if (pInfo && nPkValue <= pInfo->nMaxPkValue && nPkValue >= pInfo->nMinPkValue)
        {
            return pInfo->szTitle;
        }
    }
    return NULL;
}
//---------------------------------------------------------------------
void C3DRoleData::DestroyEffectInfo()
{
    int nAmount = m_setEffectInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        SAFE_DELETE(m_setEffectInfo[i]);
    }
    m_setEffectInfo.clear();
}
//---------------------------------------------------------------------
EffectInfo* C3DRoleData::GetEffectInfo(char* pszIndex)
{
    if (!pszIndex)
    {
        return NULL;
    }
    int nAmount = m_setEffectInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        EffectInfo* pInfo = m_setEffectInfo[i];
        if (pInfo && strcmp(pInfo->szTitle, pszIndex) == 0)
        {
            return pInfo;
        }
    }
    return NULL;
}
//---------------------------------------------------------------------
void C3DRoleData::CreateEffectInfo()
{
    CONST CHAR INI_FILE[] = "ini/Effect.ini";
    FILE* fp = fopen(INI_FILE, "r");
    if (!fp)
    {
        ::ErrorOut("error on open %s.", INI_FILE);
        return;
    }
    while(true)
    {
        // scan title
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        char szTitle[_MAX_STRING] = "";
        if (1 != sscanf(szLine, "[%s]", szTitle))
        {
            continue;
        }
        szTitle[strlen(szTitle)-1] = '\0';
        // get info ...
        EffectInfo* pInfo = new EffectInfo;
        MYASSERT (pInfo);
        //memset(pInfo, 0L, sizeof(EffectInfo));
        strcpy(pInfo->szTitle, szTitle);
        BOOL bSuc = true;
        bSuc &= fscanf(fp, "AniTitle=%s\n", pInfo->szAniTitle);
        bSuc &= fscanf(fp, "LoopTime=%d\n", &pInfo->dwLoopTime);
        bSuc &= fscanf(fp, "FrameInterval=%d\n", &pInfo->dwFrameInterval);
        bSuc &= fscanf(fp, "ShowWay=%d\n", &pInfo->dwShowWay);
        bSuc &= fscanf(fp, "LoopInterval=%d\n", &pInfo->dwLoopInterval);
        bSuc &= fscanf(fp, "OffsetX=%d\n", &pInfo->nOffsetX);
        bSuc &= fscanf(fp, "OffsetY=%d\n", &pInfo->nOffsetY);
        bSuc &= fscanf(fp, "Exigence=%d\n", &pInfo->nExigence);
        bSuc &= fscanf(fp, "Delay=%d\n", &pInfo->dwDelay);
        if (!bSuc)
        {
            ::ErrorOut("error data: %s of %s", szTitle, INI_FILE);
            break;
        }
        m_setEffectInfo.push_back(pInfo);
    }
    fclose(fp);
}
//---------------------------------------------------------------------

BOOL C3DRoleData::CreateMapMagicItemInfo()
{
    CONST CHAR INI_FILE[] = "ini/MapMagicItem.ini";
    FILE* fp = fopen(INI_FILE, "r");
    if (!fp)
    {
        ::ErrorOut("error on open %s.", INI_FILE);
        return false;
    }
    while(true)
    {
        // scan title
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        char szTitle[_MAX_STRING] = "";
        int nType = 0;
        if (1 != sscanf(szLine, "[Type%d]", &nType))
        {
            continue;
        }
        // get info ...
        MapMagicItemInfo* pInfo = new MapMagicItemInfo;
        if (!pInfo)
        {
            break;
        }
        //memset(pInfo, 0L, sizeof(MapMagicItemInfo));
        pInfo->idType = nType;
        BOOL bSucRead = true;
        bSucRead &= fscanf(fp, "Start=%s\n", pInfo->szStart);
        bSucRead &= fscanf(fp, "Last=%s\n", pInfo->szLast);
        bSucRead &= fscanf(fp, "End=%s\n", pInfo->szEnd);
        if (!bSucRead)
        {
            ::ErrorOut("error data: %s of %s", szTitle, INI_FILE);
            break;
        }
        m_setMapMagicItem.push_back(pInfo);
    }
    fclose(fp);
    return true;
}

//-------------------------------------------------------------------

void C3DRoleData::DestroyMapMagicItemInfo()
{
    int nAmount = m_setMapMagicItem.size();
    for(int i = 0; i < nAmount; i ++)
    {
        MapMagicItemInfo* pInfo = m_setMapMagicItem[i];
        SAFE_DELETE(pInfo);
    }
    m_setMapMagicItem.clear();
}

//-------------------------------------------------------------------

MapMagicItemInfo*	C3DRoleData::GetMapMagicItemInfo(int nType)
{
    int nAmount = m_setMapMagicItem.size();
    for(int i = 0; i < nAmount; i ++)
    {
        MapMagicItemInfo* pInfo = m_setMapMagicItem[i];
        if (pInfo && pInfo->idType == nType)
        {
            return pInfo;
        }
    }
    return NULL;
}

//-------------------------------------------------------------------
FlyingObjInfo*	C3DRoleData::GetFlyingObjInfo(OBJID idItem)
{
    int nAmount = m_setFlyObjInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        FlyingObjInfo* pInfo = m_setFlyObjInfo[i];
        if (pInfo && pInfo->idType == idItem)
        {
            return pInfo;
        }
    }
    // ÈÝ´í...
    if (nAmount > 0)
    {
        return m_setFlyObjInfo[0];
    }
    return NULL;
}
//-------------------------------------------------------------------
BOOL	C3DRoleData::CreateFlyObjInfo()
{
    char szIniFile[] = "ini/3DFlyingObj.ini";
    char szTitle[64];
    char szSubTitle[64];
    //---------------------------------------------------------------
    // Get Header info ...
    int nData, nAmount;
    strcpy(szTitle, "Header");
    strcpy(szSubTitle, "Amount");
    ::IniDataGet(szIniFile, szTitle, szSubTitle, nData, true);
    nAmount = nData;
    //---------------------------------------------------------------
    for(int i = 0; i < nAmount; i ++)
    {
        // Get ID ...
        strcpy(szTitle, "Header");
        sprintf(szSubTitle, "ID%d", i);
        OBJID id;
        ::IniDataGet(szIniFile, szTitle, szSubTitle, nData, true);
        id = nData;
        FlyingObjInfo* pInfo = new  FlyingObjInfo;
        MYASSERT(pInfo);
        pInfo->idType = id;
        // Get Info ...
        sprintf(szTitle, "%u", id);
        strcpy(szSubTitle, "SimpleObjID");
        ::IniDataGet(szIniFile, szTitle, szSubTitle, nData, true);
        pInfo->idSimpleObj = nData;
        strcpy(szSubTitle, "EffectIndex");
        ::IniStrGet(szIniFile, szTitle, szSubTitle, pInfo->szFlyEffect, true);
        strcpy(szSubTitle, "FlyingSound");
        ::IniStrGet(szIniFile, szTitle, szSubTitle, pInfo->szFlySound, true);
        strcpy(szSubTitle, "HitSound");
        ::IniStrGet(szIniFile, szTitle, szSubTitle, pInfo->szHitSound, true);
        strcpy(szSubTitle, "TargetEffect");
        ::IniStrGet(szIniFile, szTitle, szSubTitle, pInfo->szTargetEffect, true);
        m_setFlyObjInfo.push_back(pInfo);
    }
    return true;
}
//-------------------------------------------------------------------
void	C3DRoleData::DestroyFlyObjInfo()
{
    int nAmount = m_setFlyObjInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        SAFE_DELETE(m_setFlyObjInfo[i]);
    }
    m_setFlyObjInfo.clear();
}
//-------------------------------------------------------------------
BOOL	C3DRoleData::CreateTerrainWeatherInfo()
{
    TerrainWeatherInfo infoWeather;
    char szIni[] = "ini/weather.ini";
    FILE* fp = fopen(szIni, "r");
    MYASSERT(fp);
    while(true)
    {
        int nReturn = fscanf(fp, "%d %d %d %d %u\n", &infoWeather.nTerrain, &infoWeather.nType, &infoWeather.nWindDir, &infoWeather.nLevel, &infoWeather.dwColor);
        if (EOF == nReturn)
        {
            break;
        }
        if (5 == nReturn)
        {
            TerrainWeatherInfo* pInfo = new TerrainWeatherInfo;
            if (pInfo)
            {
                memcpy(pInfo, &infoWeather, sizeof(TerrainWeatherInfo));
                m_setTerrainWeatherInfo.push_back(pInfo);
            }
        }
    }
    return true;
}
//-------------------------------------------------------------------
void	C3DRoleData::DestroyTerrainWeatherInfo()
{
    int nAmount = m_setTerrainWeatherInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        if (m_setTerrainWeatherInfo[i])
        {
            SAFE_DELETE(m_setTerrainWeatherInfo[i]);
        }
        m_setTerrainWeatherInfo.clear();
    }
}
//-------------------------------------------------------------------
TerrainWeatherInfo* C3DRoleData::GetWeatherInfo(int nTerrain)
{
    int nAmount = m_setTerrainWeatherInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        if (m_setTerrainWeatherInfo[i] && m_setTerrainWeatherInfo[i]->nTerrain == nTerrain)
        {
            return m_setTerrainWeatherInfo[i];
        }
    }
    return NULL;
}
//-------------------------------------------------------------------
StatusEffectInfo*	C3DRoleData::GetStatusEffectInfoByIndex(int nIndex)
{
    int nAmount = m_setStatusEffectInfo.size();
    if (nIndex > 0 && nIndex < nAmount)
    {
        return m_setStatusEffectInfo[nIndex];
    }
    return NULL;
}
//-------------------------------------------------------------------
DWORD C3DRoleData::GetStatusEffectInfoAmount()
{
    return m_setStatusEffectInfo.size();
}
//-------------------------------------------------------------------
BOOL C3DRoleData::CreateStatusEffectInfo()
{
    char szIni[] = "ini/statuseffect.ini";
    StatusEffectInfo info;
    FILE* fp = fopen(szIni, "r");
    MYASSERT(fp);
    while(true)
    {
        int nReturn = fscanf(fp, "%I64x %s %s\n", &info.dwStatus, info.sz3DEffect, info.sz2DEffect);
        if (EOF == nReturn)
        {
            break;
        }
        if (3 == nReturn)
        {
            StatusEffectInfo* pInfo = new StatusEffectInfo;
            if (pInfo)
            {
                memcpy(pInfo, &info, sizeof(StatusEffectInfo));
                m_setStatusEffectInfo.push_back(pInfo);
            }
        }
    }
    return true;
}
//-------------------------------------------------------------------
void C3DRoleData::DestroyStatusEffectInfo()
{
    int nAmount = m_setStatusEffectInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        if (m_setStatusEffectInfo[i])
        {
            SAFE_DELETE(m_setStatusEffectInfo[i]);
        }
    }
    m_setStatusEffectInfo.clear();
}
//-------------------------------------------------------------------
const char*	C3DRoleData::GetUserHelpInfoLineByIndex(int nProfessional, int nLevel, int nSex, int nMapId, int nStatus, int nStatusLevel, int nItemType, int nMagicId, int nIndex)
{
    int nAmount = m_setUserHelpInfo.size();
    for(int i = 1; i < nAmount; i ++)
    {
        UserHelpInfo* pInfo = m_setUserHelpInfo[i];
        if (pInfo
                && pInfo->nProfession == nProfessional
                && nLevel <= pInfo->nMaxLevel
                && nLevel >= pInfo->nMinLevel
                && nStatus == pInfo->nStatus
                && nStatusLevel == pInfo->nStatusLevel
                && nSex == pInfo->nSex
                && nMapId == pInfo->nMapId
                && nItemType == pInfo->nItemType
                && nMagicId == pInfo->nMagicId)
        {
            int nStrAmount = pInfo->m_setStr.size();
            if (nIndex >= nStrAmount || nIndex < 0)
            {
                return NULL;
            }
            return pInfo->m_setStr[nIndex].c_str();
        }
    }
    if (nAmount == 0)
    {
        return NULL;
    }
    // default ...
    UserHelpInfo* pInfo = m_setUserHelpInfo[0];
    if (!pInfo)
    {
        return NULL;
    }
    int nStrAmount = pInfo->m_setStr.size();
    if (nIndex >= nStrAmount || nIndex < 0)
    {
        return NULL;
    }
    return pInfo->m_setStr[nIndex].c_str();
}
//--------------------------------------------------------------------------------------
int		C3DRoleData::GetUserHelpInfoLineAmount(int nProfessional, int nLevel,  int nSex, int nMapId, int nStatus, int nStatusLevel, int nItemType, int nMagicId)
{
    int nAmount = m_setUserHelpInfo.size();
    for(int i = 1; i < nAmount; i ++)
    {
        UserHelpInfo* pInfo = m_setUserHelpInfo[i];
        if (pInfo &&
                pInfo->nProfession == nProfessional
                && nLevel <= pInfo->nMaxLevel
                && nLevel >= pInfo->nMinLevel
                && nStatus == pInfo->nStatus
                && nStatusLevel == pInfo->nStatusLevel
                && nSex == pInfo->nSex
                && nMapId == pInfo->nMapId
                && nItemType == pInfo->nItemType
                && pInfo->nMagicId == nMagicId  )
        {
            return pInfo->m_setStr.size();
        }
    }
    if (nAmount == 0)
    {
        return 0;
    }
    // default ...
    UserHelpInfo* pInfo = m_setUserHelpInfo[0];
    if (pInfo)
    {
        return pInfo->m_setStr.size();
    }
    return NULL;
}
//--------------------------------------------------------------------------------------
BOOL	C3DRoleData::CreateTipsInfo()
{
    const char szDataFile[] = "ini/tips.dat";
    // open file ...
    FILE* fp = fopen(szDataFile, "rb");
    MYASSERT(fp);
    // header ...
    char szHeader[32];
    fread(szHeader, sizeof(char), 32, fp);
    // version
    DWORD dwVersion;
    fread(&dwVersion, sizeof(DWORD), 1, fp);
    // amount
    DWORD dwRecordAmount;
    fread(&dwRecordAmount, sizeof(DWORD), 1, fp);
    for(DWORD dwCounter = 0; dwCounter < dwRecordAmount; dwCounter++)
    {
        TipsInfo* pInfo = new TipsInfo;
        // read str ...
        DWORD dwStrAmount;
        fread(&dwStrAmount, sizeof(DWORD), 1, fp);
        for(DWORD dwStrCounter = 0; dwStrCounter < dwStrAmount; dwStrCounter++)
        {
            // get length ...
            DWORD dwLength;
            fread(&dwLength, sizeof(DWORD), 1, fp);
            // get str ...
            char szStr[_MAX_STRING];
            fread(szStr, sizeof(char), dwLength, fp);
            string str = szStr;
            pInfo->m_setStr.push_back(str);
        }
        m_setTipsInfo.push_back(pInfo);
    }
    return true;
}
//--------------------------------------------------------------------------------------
void	C3DRoleData::DestroyTipsInfo()
{
    int nAmount = m_setTipsInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        TipsInfo* pInfo = m_setTipsInfo[i];
        if (pInfo)
        {
            pInfo->m_setStr.clear();
            SAFE_DELETE(pInfo);
        }
    }
    m_setTipsInfo.clear();
}
//--------------------------------------------------------------------------------------

BOOL	C3DRoleData::CreateUserHelpInfo()
{
    const char szDataFile[] = "ini/UserHelpInfo.dat";
    // open file ...
    FILE* fp = fopen(szDataFile, "rb");
    MYASSERT(fp);
    // header ...
    char szHeader[32];
    fread(szHeader, sizeof(char), 32, fp);
    // version
    DWORD dwVersion;
    fread(&dwVersion, sizeof(DWORD), 1, fp);
    // amount
    DWORD dwRecordAmount;
    fread(&dwRecordAmount, sizeof(DWORD), 1, fp);
    for(DWORD dwCounter = 0; dwCounter < dwRecordAmount; dwCounter++)
    {
        UserHelpInfo* pInfo = new UserHelpInfo;
        MYASSERT(pInfo);
        fread(&pInfo->nMaxLevel, sizeof(int), 1, fp);
        fread(&pInfo->nMinLevel, sizeof(int), 1, fp);
        fread(&pInfo->nProfession, sizeof(int), 1, fp);
        fread(&pInfo->nSex, sizeof(int), 1, fp);
        fread(&pInfo->nMapId, sizeof(int), 1, fp);
        fread(&pInfo->nStatus, sizeof(int), 1, fp);
        fread(&pInfo->nStatusLevel, sizeof(int), 1, fp);
        fread(&pInfo->nItemType, sizeof(int), 1, fp);
        fread(&pInfo->nMagicId, sizeof(int), 1, fp);
        fread(&pInfo->nMagicLevel, sizeof(int), 1, fp);
        //set default to 0
        if (pInfo->nMaxLevel < 0) { pInfo->nMaxLevel = 0; }
        if (pInfo->nMinLevel < 0) { pInfo->nMinLevel = 0; }
        if (pInfo->nProfession < 0) { pInfo->nProfession = 0; }
        if (pInfo->nSex < 0) { pInfo->nSex = 0; }
        if (pInfo->nMapId < 0) { pInfo->nMapId = 0; }
        if (pInfo->nStatus < 0) { pInfo->nStatus = 0; }
        if (pInfo->nStatusLevel < 0) { pInfo->nStatusLevel = 0; }
        if (pInfo->nItemType < 0) { pInfo->nItemType = 0; }
        if (pInfo->nMagicId < 0) { pInfo->nMagicId = 0; }
        if (pInfo->nMagicLevel < 0) { pInfo->nMagicLevel = 0; }
        // read str ...
        DWORD dwStrAmount;
        fread(&dwStrAmount, sizeof(DWORD), 1, fp);
        for(DWORD dwStrCounter = 0; dwStrCounter < dwStrAmount; dwStrCounter++)
        {
            // get length ...
            DWORD dwLength;
            fread(&dwLength, sizeof(DWORD), 1, fp);
            // get str ...
            char szStr[_MAX_STRING];
            fread(szStr, sizeof(char), dwLength, fp);
            string str = szStr;
            pInfo->m_setStr.push_back(str);
        }
        m_setUserHelpInfo.push_back(pInfo);
    }
    return true;
}
//--------------------------------------------------------------------------------------
/*
BOOL	C3DRoleData::CreateUserHelpInfo()
{
	const char szIniFile[] = "ini/UserHelpInfo.ini";
	char	szTitle[64];
	char	szSubTitle[64];
	char	szStr[512];
	int		nData;
	int		i;
	string	str;
	//Part1: header ...
	DWORD dwInfoAmount = 0;
	strcpy(szTitle, "Header");
	strcpy(szSubTitle, "HelpInfoAmount");
	::IniDataGet(szIniFile, szTitle, szSubTitle, nData, true);
	dwInfoAmount = nData;

	//Part2: default info ...
	UserHelpInfo* pInfo = new UserHelpInfo;
	MYASSERT(pInfo);
	DWORD dwStrAmount = 0;
	strcpy(szTitle, "DefaultInfo");
	strcpy(szSubTitle, "HelpLineAmount");
	::IniDataGet(szIniFile, szTitle, szSubTitle, nData, true);
	dwStrAmount = nData;
	for(i = 0; i < dwStrAmount; i ++)
	{
		sprintf(szSubTitle, "Line%d", i);
		::IniStrGet(szIniFile, szTitle, szSubTitle, szStr, true);
		str = szStr;
		pInfo->m_setStr.push_back(str);
	}
	m_setUserHelpInfo.push_back(pInfo);

	//Part3: text ...
	for(i = 0; i < dwInfoAmount; i ++)
	{
		UserHelpInfo* pInfo = new UserHelpInfo;
		MYASSERT(pInfo);

		sprintf(szTitle, "HelpInfo%d", i);

		strcpy(szSubTitle, "Profession");
		::IniDataGet(szIniFile, szTitle, szSubTitle, nData, true);
		pInfo->nProfession = nData;

		strcpy(szSubTitle, "MinLevel");
		::IniDataGet(szIniFile, szTitle, szSubTitle, nData, true);
		pInfo->nMinLevel = nData;

		strcpy(szSubTitle, "MaxLevel");
		::IniDataGet(szIniFile, szTitle, szSubTitle, nData, true);
		pInfo->nMaxLevel = nData;

		strcpy(szSubTitle, "Syndicate");
		::IniDataGet(szIniFile, szTitle, szSubTitle, nData, true);
		pInfo->nSyndicate = nData;

		strcpy(szSubTitle, "Marriage");
		::IniDataGet(szIniFile, szTitle, szSubTitle, nData, true);
		pInfo->nMarriage = nData;

		strcpy(szSubTitle, "HelpLineAmount");
		::IniDataGet(szIniFile, szTitle, szSubTitle, nData, true);

		dwStrAmount = nData;

		for(int j = 0; j < dwStrAmount; j ++)
		{
			sprintf(szSubTitle, "Line%d", j);
			::IniStrGet(szIniFile, szTitle, szSubTitle, szStr, true);
			str = szStr;
			pInfo->m_setStr.push_back(str);
		}
		m_setUserHelpInfo.push_back(pInfo);
	}
	return true;
}*/

//--------------------------------------------------------------------------------------
void	C3DRoleData::DestroyUserHelpInfo()
{
    int nAmount = m_setUserHelpInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        UserHelpInfo* pInfo = m_setUserHelpInfo[i];
        if (pInfo)
        {
            pInfo->m_setStr.clear();
            SAFE_DELETE(pInfo);
        }
    }
    m_setUserHelpInfo.clear();
}
//--------------------------------------------------------------------------------------
TerrainNpcTypeInfo*			C3DRoleData::GetTerrainNpcInfo(OBJID idType)
{
    int nAmount = m_setTerrainNpcTypeInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        TerrainNpcTypeInfo* pInfo = m_setTerrainNpcTypeInfo[i];
        if (pInfo && pInfo->nType == idType)
        {
            return pInfo;
        }
    }
    if (nAmount > 0)
    {
        return m_setTerrainNpcTypeInfo[0];
    }
    return NULL;
}
//--------------------------------------------------------------------------------------
int							C3DRoleData::GetTipsAmount()
{
    return m_setTipsInfo.size();
}
//--------------------------------------------------------------------------------------
int							C3DRoleData::GetTipLineAmount(int nIndex)
{
    int nAmount = m_setTipsInfo.size();
    if ((nIndex < 0) && (nIndex >= nAmount))
    {
        return 0;
    }
    if (m_setTipsInfo[nIndex])
    {
        return m_setTipsInfo[nIndex]->m_setStr.size();
    }
    else
    {
        return 0;
    }
}
//--------------------------------------------------------------------------------------
char*						C3DRoleData::GetTipLine(int nTipIndex, int nLineIndex)
{
    int nAmount = m_setTipsInfo.size();
    if ((nTipIndex < 0) && (nTipIndex >= nAmount))
    {
        return NULL;
    }
    TipsInfo* pInfo = m_setTipsInfo[nTipIndex];
    if (!pInfo)
    {
        return NULL;
    }
    nAmount = pInfo->m_setStr.size();
    if ((nLineIndex < 0) && (nLineIndex >= nAmount))
    {
        return NULL;
    }
    return (char*)pInfo->m_setStr[nLineIndex].c_str();
}
//--------------------------------------------------------------------------------------
BOOL	C3DRoleData::CreateTextAction()
{
    const char szIniFile[] = "ini/TxtAction.ini";
    FILE* fp = fopen(szIniFile, "r");
    if (!fp)
    {
        return false;
    }
    char szAction[_MAX_STRING];
    char szText[_MAX_STRING];
    while (true)
    {
        int rval = fscanf(fp, "%s  %s\n", szAction, szText);
        if (rval == EOF)
        {
            break;
        }
        TextActionInfo* pInfo = new TextActionInfo;
        MYASSERT (pInfo);
        pInfo->strAction = szAction;
        pInfo->strText = szText;
        m_setTextAction.push_back(pInfo);
    }
    fclose(fp);
    return true;
}
//--------------------------------------------------------------------------------------
void	C3DRoleData::DestroyTextAction()
{
    int nAmount = m_setTextAction.size();
    for(int i = 0; i < nAmount; i ++)
    {
        SAFE_DELETE(m_setTextAction[i]);
    }
    m_setTextAction.clear();
}
//--------------------------------------------------------------------------------------
int	C3DRoleData::GetTextActionAmount()
{
    return m_setTextAction.size();
}
//--------------------------------------------------------------------------------------
char* C3DRoleData::GetTextActionTitle(int nIndex)
{
    int nAmount = m_setTextAction.size();
    if ((nIndex < 0) || (nIndex >= nAmount))
    {
        return NULL;
    }
    return (char*)m_setTextAction[nIndex]->strAction.c_str();
}
//--------------------------------------------------------------------------------------
char* C3DRoleData::GetTextActionText(int nIndex)
{
    int nAmount = m_setTextAction.size();
    if ((nIndex < 0) || (nIndex >= nAmount))
    {
        return NULL;
    }
    return (char*)m_setTextAction[nIndex]->strText.c_str();
}
//--------------------------------------------------------------------------------------
BOOL	C3DRoleData::CreatePetInfo()
{
    const char szPetData[_MAX_PATH]	=	"ini/pet.dat";
    FILE* fp = fopen(szPetData, "rb");
    if (!fp)
    {
        return false;
    }
    //read ...
    int nRecordAmount;
    fread(&nRecordAmount, sizeof(int), 1, fp);
    for(int i = 0; i < nRecordAmount; i ++)
    {
        PetInfo* pInfo = new PetInfo;
        MYASSERT(pInfo);
        m_setPetInfo.push_back(pInfo);
        // read Method Amount ...
        DWORD dwMethodAmount;
        fread(&dwMethodAmount, sizeof(DWORD), 1, fp);
        DWORD dwMyCheck = 0;
        for(DWORD j = 0; j < dwMethodAmount; j++)
        {
            int nMethod;
            fread(&nMethod, sizeof(int), 1, fp);
            pInfo->setLocateTargetMethod.push_back(nMethod);
            dwMyCheck += nMethod;
        }
        dwMyCheck = dwMethodAmount * dwMyCheck;
        fread(&pInfo->nEnvMethod, sizeof(int), 1, fp);
        fread(&pInfo->nAddSize, sizeof(int), 1, fp);
        fread(&pInfo->nScale, sizeof(int), 1, fp);
        fread(&pInfo->nAttackRange, sizeof(int), 1, fp);
        fread(&pInfo->dwAttackInterval, sizeof(DWORD), 1, fp);
        DWORD dwCheck;
        fread(&dwCheck, sizeof(DWORD), 1, fp);
        dwMyCheck += pInfo->nEnvMethod + pInfo->setLocateTargetMethod.size() * 2
                     + pInfo->nAddSize + pInfo->nAttackRange * pInfo->dwAttackInterval;
        if (dwCheck != dwMyCheck)
        {
            exit(-1);
        }
    }
    fclose(fp);
    return true;
}
//--------------------------------------------------------------------------------------
void	C3DRoleData::DestroyPetInfo()
{
    int nAmount = m_setPetInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        PetInfo* pInfo = m_setPetInfo[i];
        if (pInfo)
        {
            pInfo->setLocateTargetMethod.clear();
        }
        SAFE_DELETE(pInfo);
    }
    m_setPetInfo.clear();
}
//--------------------------------------------------------------------------------------
PetInfo*	C3DRoleData::GetPetInfo(int nIndex)
{
    int nAmount = m_setPetInfo.size();
    if (nIndex < 0 || nIndex >= nAmount)
    {
        return NULL;
    }
    return m_setPetInfo[nIndex];
}
//--------------------------------------------------------------------------------------
LightInfo*	C3DRoleData::GetLightInfoByIndex(int nIndex)
{
    if (nIndex == 0)
    {
        return NULL;
    }
    nIndex --;
    int nAmount = m_setLightInfo.size();
    if (nIndex < 0 && nIndex >= nAmount)
    {
        return NULL;
    }
    return m_setLightInfo[nIndex];
}
//--------------------------------------------------------------------------------------
DWORD		C3DRoleData::GetRolePartLightInfo(OBJID idType)
{
    map<OBJID, DWORD>::iterator iter = m_setRolePartLightInfo.find(idType);
    if (iter != m_setRolePartLightInfo.end())
    {
        return iter->second;
    }
    return 0;
}
//--------------------------------------------------------------------------------------
BOOL	C3DRoleData::CreateLightLInfo()
{
    this->DestroyLightInfo();
    const char szFile[] = "ini/Light.ini";
    FILE* fp = fopen(szFile, "r");
    if (!fp)
    {
        return false;
    }
    while(true)
    {
        // scan title
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            fclose(fp);
            return true;
        }
        char szTitle[MAX_PATH] = "";
        if (1 != sscanf(szLine, "[%s", szTitle))
        {
            continue;
        }
        char* p = strchr(szTitle, ']');
        if (p)
        {
            *p = '\0';
        }
        else
        {
            fclose(fp);
            ::ErrorOut("Error Title line %s in %s", szLine, szFile);
            return false;
        }
        LightInfo* pInfo = new LightInfo;
        MYASSERT(pInfo);
        pInfo->m_setLightPartInfo.clear();
        int nLightAmount = 0;
        MYASSERT(1 == fscanf(fp, "LightAmount=%d\n", &nLightAmount));
        for(int i = 0; i < nLightAmount; i ++)
        {
            LightInfoPart* pPart = new LightInfoPart;
            MYASSERT(pPart);
            int nIndex;
            int nData[4];
            MYASSERT(6 == fscanf(fp, "Light%d=%u %d %d %d %d\n", &nIndex, &pPart->dwTimeCost,
                                 &nData[0], &nData[1],
                                 &nData[2], &nData[3]));
            MYASSERT(nIndex == i);
            pPart->ucAlphaRate = nData[0];
            pPart->ucRedRate = nData[1];
            pPart->ucGreenRate = nData[2];
            pPart->ucBlueRate =  nData[3];
            pInfo->m_setLightPartInfo.push_back(pPart);
        }
        m_setLightInfo.push_back(pInfo);
    }
    fclose(fp);
    return true;
}
//--------------------------------------------------------------------------------------
void	C3DRoleData::DestroyLightInfo()
{
    int nAmount =  m_setLightInfo.size();
    for(int i  = nAmount - 1; i >= 0; i --)
    {
        LightInfo* pInfo = m_setLightInfo[i];
        if (pInfo)
        {
            int nPartAmount = pInfo->m_setLightPartInfo.size();
            for(int j = 0; j < nPartAmount; j ++)
            {
                SAFE_DELETE(pInfo->m_setLightPartInfo[j]);
            }
            pInfo->m_setLightPartInfo.clear();
        }
        SAFE_DELETE(pInfo);
    }
    m_setLightInfo.clear();
}
//--------------------------------------------------------------------------------------
BOOL	C3DRoleData::CreateRolePartLightInfo()
{
    m_setRolePartLightInfo.clear();
    const char szFile[] = "ini/RolePartLight.ini";
    FILE* fp = fopen(szFile, "r");
    if (!fp)
    {
        return false;
    }
    OBJID idItemType;
    DWORD dwLight;
    while(true)
    {
        int rval = 	fscanf(fp, "%u=%u\n", &idItemType, &dwLight);
        if (rval == EOF)
        {
            return false;
        }
        if (rval == 2)
        {
            m_setRolePartLightInfo[idItemType] = dwLight;
        }
    }
    fclose(fp);
    return true;
}
//--------------------------------------------------------------------------------------
void	C3DRoleData::DestroyRolePartLightInfo()
{
    m_setRolePartLightInfo.clear();
}
//--------------------------------------------------------------------------------------
BOOL	C3DRoleData::Create3DItemInfo()
{
    this->Destroy3DItemInfo();
    const char szFile[] = "ini/3DItem.ini";
    FILE* fp = fopen(szFile, "r");
    if (!fp)
    {
        return false;
    }
    OBJID idItemType, idMesh, idTexture;
    unsigned short usPercentBox;
    unsigned short usPercentMap;
    while(true)
    {
        DWORD dwPercentBox, dwPercentMap;
        int rval = 	fscanf(fp, "%u %u %u %u %u\n", &idItemType, &idMesh, &idTexture, &dwPercentBox, &dwPercentMap);
        usPercentBox = (unsigned short)dwPercentBox;
        usPercentMap = (unsigned short)dwPercentMap;
        if (rval == EOF)
        {
            break;
        }
        if (rval == 5)
        {
            m_set3DItemInfo[idItemType].idMesh = idMesh;
            m_set3DItemInfo[idItemType].idTexture = idTexture;
            m_set3DItemInfo[idItemType].usPercentItemBox = usPercentBox;
            m_set3DItemInfo[idItemType].usPercentItemMap = usPercentMap;
        }
    }
    fclose(fp);
    return true;
}
//--------------------------------------------------------------------------------------
void	C3DRoleData::Destroy3DItemInfo()
{
    m_set3DItemInfo.clear();
}
//--------------------------------------------------------------------------------------
BOOL	C3DRoleData::Get3DItem(OBJID idItemType, OBJID& idMesh, OBJID& idTexture,
                               unsigned short& usBoxSize, unsigned short& usMapSize)
{
    map<OBJID, C3DItemInfo>::iterator iter = m_set3DItemInfo.find(idItemType);
    if (iter != m_set3DItemInfo.end())
    {
        idMesh = iter->second.idMesh;
        idTexture = iter->second.idTexture;
        usBoxSize = iter->second.usPercentItemBox;
        usMapSize = iter->second.usPercentItemMap;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------------
BOOL	C3DRoleData::CreateHonorTitleInfo()
{
    this->DestroyHonorTitleInfo();
    const char szIniFile[] = "ini/Common.ini";
    char szTitle[64];
    char szSubtitle[64];
    strcpy(szTitle, "HonorTitle");
    strcpy(szSubtitle, "Amount");
    int nAmount = 0;
    ::IniDataGet(szIniFile, szTitle, szSubtitle, nAmount, true);
    for(int i = 0; i < nAmount; i ++)
    {
        char szStr[64];
        sprintf(szSubtitle, "Title%d", i);
        ::IniStrGet(szIniFile, szTitle, szSubtitle, szStr, true);
        m_setHonorTitle.push_back(szStr);
    }
    return true;
}
//--------------------------------------------------------------------------------------
void	C3DRoleData::DestroyHonorTitleInfo()
{
    m_setHonorTitle.clear();
}
//--------------------------------------------------------------------------------------
BOOL	C3DRoleData::CreateNobilityTitleInfo()
{
    this->DestroyNobilityTitleInfo();
    const char szIniFile[] = "ini/Common.ini";
    char szTitle[64];
    char szSubtitle[64];
    strcpy(szTitle, "NobilityTitle");
    strcpy(szSubtitle, "Amount");
    int nAmount = 0;
    ::IniDataGet(szIniFile, szTitle, szSubtitle, nAmount, true);
    for(int i = 0; i < nAmount; i ++)
    {
        char szStr[64];
        sprintf(szSubtitle, "Title%d", i);
        ::IniStrGet(szIniFile, szTitle, szSubtitle, szStr, true);
        m_setNobilityTitle.push_back(szStr);
    }
    return true;
}
//--------------------------------------------------------------------------------------
void	C3DRoleData::DestroyNobilityTitleInfo()
{
    m_setNobilityTitle.clear();
}
//--------------------------------------------------------------------------------------
const char*	 C3DRoleData::GetHonorTitle(int nIndex)
{
    int nAmount = m_setHonorTitle.size();
    if (nIndex < 0 || nIndex >= nAmount)
    {
        return NULL;
    }
    return m_setHonorTitle[nIndex].c_str();
}
//--------------------------------------------------------------------------------------
const char*	 C3DRoleData::GetNobilityTitle(int nIndex)
{
    int nAmount = m_setNobilityTitle.size();
    if (nIndex < 0 || nIndex >= nAmount)
    {
        return NULL;
    }
    return m_setNobilityTitle[nIndex].c_str();
}
//--------------------------------------------------------------------------------------
BOOL	C3DRoleData::CreateMedalInfo()
{
    this->DestroyMedalInfo();
    const char szIniFile[] = "ini/Common.ini";
    char szTitle[64];
    char szSubtitle[64];
    strcpy(szTitle, "MedalTitle");
    strcpy(szSubtitle, "Amount");
    int nAmount = 0;
    ::IniDataGet(szIniFile, szTitle, szSubtitle, nAmount, true);
    for(int i = 0; i < nAmount; i ++)
    {
        char szStr[64];
        sprintf(szSubtitle, "Title%d", i);
        ::IniStrGet(szIniFile, szTitle, szSubtitle, szStr, true);
        m_setMedal.push_back(szStr);
    }
    return true;
}
//--------------------------------------------------------------------------------------
void	C3DRoleData::DestroyMedalInfo()
{
    m_setMedal.clear();
}
//--------------------------------------------------------------------------------------
BOOL	C3DRoleData::CreateTrackInfo()
{
    const char szIniFile[] = "ini/track.ini";
    FILE* fp = fopen(szIniFile, "r");
    if (fp)
    {
        char szLine[1024];
        while(NULL != fgets(szLine, 1023, fp))
        {
            if (strlen(szLine) <= 0)
            {
                continue;
            }
            if (szLine[0] == ';')
            {
                continue;
            }
            DWORD			dwID;
            int				nAction;
            int				nStartFrame;
            int				nEndFrame;
            int				nFrameInterval;
            int				nHeight;
            char			strEffect[256];
            char			strWave[256];
            if (8 == sscanf(szLine, "%u %d %d %d %d %d %s %s\n", &dwID,
                            &nAction, &nStartFrame, &nEndFrame, &nFrameInterval,
                            &nHeight, strEffect, strWave))
            {
                TrackInfo* pInfo = new TrackInfo;
                if (pInfo)
                {
                    pInfo->dwID = dwID;
                    pInfo->nAction = nAction;
                    pInfo->nStartFrame = nStartFrame;
                    pInfo->nEndFrame = nEndFrame;
                    pInfo->nFrameInterval = nFrameInterval;
                    pInfo->nHeight = nHeight;
                    pInfo->strEffect = strEffect;
                    pInfo->strWave = strWave;
                    m_setTrackInfo[pInfo->dwID] = pInfo;
                }
            }
        }
        fclose(fp);
    }
    return true;
}
//--------------------------------------------------------------------------------------
void	C3DRoleData::DestroyTrackInfo()
{
    map<OBJID, TrackInfo*>::iterator iter = m_setTrackInfo.begin();
    while(iter != m_setTrackInfo.end())
    {
        delete iter->second;
        iter ++;
    }
    m_setTrackInfo.clear();
}
//--------------------------------------------------------------------------------------
TrackInfo*	C3DRoleData::GetTrackInfo(OBJID id)
{
    map<OBJID, TrackInfo*>::iterator iter = m_setTrackInfo.find(id);
    if (iter != m_setTrackInfo.end())
    {
        return iter->second;
    }
    return NULL;
}
//--------------------------------------------------------------------------------------
const char*	C3DRoleData::GetMedal(int nIndex)
{
    int nAmount = m_setMedal.size();
    if (nIndex < 0 || nIndex >= nAmount)
    {
        return NULL;
    }
    return m_setMedal[nIndex].c_str();
}
//--------------------------------------------------------------------------------------
UserHelpInfo* C3DRoleData::GetUserHelpInfoByIndex(int nIndex)
{
    int nAmount = m_setUserHelpInfo.size();
    if (nIndex < 0 || nIndex >= nAmount)
    {
        return NULL;
    }
    return m_setUserHelpInfo[nIndex];
}
//--------------------------------------------------------------------------------------
int C3DRoleData::GetUserHelpInfoAmount()
{
    return m_setUserHelpInfo.size();
}
//--------------------------------------------------------------------------------------
EffectIndex* C3DRoleData::GetEffectIndexInfo(OBJID id)
{
    map<OBJID, EffectIndex>::iterator iter;
    iter = m_setEffectIndexInfo.find(id);
    if (iter != m_setEffectIndexInfo.end())
    {
        return &iter->second;
    }
    return NULL;
}
//--------------------------------------------------------------------------------------
BOOL	C3DRoleData::CreateEffectIndexInfo()
{
    const char szIniFile[] = "ini/Effectindex.ini";
    FILE* fp = fopen(szIniFile, "r");
    if (fp)
    {
        char szLine[1024];
        while(NULL != fgets(szLine, 1023, fp))
        {
            if (strlen(szLine) <= 0)
            {
                continue;
            }
            if (szLine[0] == ';')
            {
                continue;
            }
            DWORD			dwID;
            char			strEffect[256];
            char			strWave[256];
            DWORD			dwData;
            if (4 == sscanf(szLine, "%u %s %s %u\n", &dwID, strEffect, strWave, &dwData))
            {
                EffectIndex info;
                info.strEffect = strEffect;
                info.strWave = strWave;
                info.dwData = dwData;
                m_setEffectIndexInfo[dwID] = info;
            }
        }
        fclose(fp);
    }
    return true;
}
//--------------------------------------------------------------------------------------
void	C3DRoleData::DestroyEffectIndexInfo()
{
    m_setEffectIndexInfo.clear();
}
//--------------------------------------------------------------------------------------
CONST CActionCtrlInfo* C3DRoleData::GetActionCtrlInfo (DWORD dwKey) CONST
{
    std::map< DWORD, CActionCtrlInfo >::const_iterator iter = m_setActionCtrlInfo.find(dwKey);
    if (iter == m_setActionCtrlInfo.end())
    {
        return NULL;
    }

    return &((*iter).second);
}
//--------------------------------------------------------------------------------------
BOOL C3DRoleData::CreateActionCtrlInfo()
{
    CONST CHAR INI_FILE[] = "ini/ActionCtrl.ini";
    FILE* fp = fopen(INI_FILE, "r");
    if (!fp)
    {
        ::ErrorOut("error on open %s.", INI_FILE);
        return FALSE;
    }
    while(true)
    {
        // scan title
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        char szTitle[_MAX_STRING] = "";
        if (1 != sscanf(szLine, "[%s]", szTitle))
        {
            continue;
        }
        szTitle[strlen(szTitle)-1] = '\0';
        DWORD dwKey = (DWORD) atoi(szTitle);
        // get info ...
        CActionCtrlInfo info;
        BOOL bSuc = TRUE;
        bSuc &= fscanf(fp, "Section=%d\n", &info.nSection);
        info.nSection %= CActionCtrlInfo::MAX_ACTION_SECTION;
        for (int i = 0; i < info.nSection; i++)
        {
            char szFormat[256] = "";
            sprintf(szFormat, "TimePercent%d=%s\n", i, "%d");
            bSuc &= fscanf(fp, szFormat, &info.nTimePercent[i]);
            sprintf(szFormat, "MovePercent%d=%s\n", i, "%d");
            bSuc &= fscanf(fp, szFormat, &info.nMovePercent[i]);
        }
        if (!bSuc)
        {
            ::ErrorOut("error data: %s of %s", szTitle, INI_FILE);
            return FALSE;
        }
        // keep info
        m_setActionCtrlInfo[dwKey] = info;
    }
    fclose(fp);
    return TRUE;
}
//--------------------------------------------------------------------------------------
void C3DRoleData::DestroyActionCtrlInfo()
{
    m_setActionCtrlInfo.clear();
}
//--------------------------------------------------------------------------------------
BOOL C3DRoleData::CreateActionDelayInfo()
{
    CONST CHAR INI_FILE[] = "ini/ActionDelay.ini";
    FILE* fp = fopen(INI_FILE, "r");
    if (!fp)
    {
        ::ErrorOut("error on open %s.", INI_FILE);
        return FALSE;
    }
    while(true)
    {
        // scan title
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        char szTitle[_MAX_STRING] = "";
        if (1 != sscanf(szLine, "[%s]", szTitle))
        {
            continue;
        }
        szTitle[strlen(szTitle)-1] = '\0';
        DWORD dwKey = (DWORD) atoi(szTitle);
        // get info ...
        CActionDelayInfo info;
        BOOL bSuc = TRUE;
        bSuc &= fscanf(fp, "WoundDelay=%u\n", &info.tmWoundDelay);
        bSuc &= fscanf(fp, "BlockDelay=%u\n", &info.tmBlockDelay);
        bSuc &= fscanf(fp, "DieDelay=%u\n",	&info.tmDieDelay);
        if (!bSuc)
        {
            ::ErrorOut("error data: %s of %s", szTitle, INI_FILE);
            return FALSE;
        }
        // keep info
        m_setActionDelayInfo[dwKey] = info;
    }
    fclose(fp);
    return TRUE;
}
//--------------------------------------------------------------------------------------
void C3DRoleData::DestroyActionDelayInfo()
{
    m_setActionDelayInfo.clear();
}
//--------------------------------------------------------------------------------------
CONST CActionDelayInfo*		C3DRoleData::GetActionDelayInfo	(DWORD dwKey) CONST
{
    std::map< DWORD, CActionDelayInfo >::const_iterator iter = m_setActionDelayInfo.find(dwKey);
    if (iter == m_setActionDelayInfo.end())
    {
        return NULL;
    }

    return &((*iter).second);
}
//--------------------------------------------------------------------------------------
void	C3DRoleData::DestoryWeaponInfo()
{
    m_setWeaponInfo.clear();
}
//////////////////////////////////////////////////////////////////////
BOOL C3DRoleData::CreateWeaponInfo()
{
    CONST CHAR INI_FILE[] = "ini/WeaponEffect.ini";
    FILE* fp = fopen(INI_FILE, "r");
    if (!fp)
    {
        ::ErrorOut("error on open %s.", INI_FILE);
        return FALSE;
    }
    while(true)
    {
        // scan title
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        char szTitle[_MAX_STRING] = "";
        if (1 != sscanf(szLine, "[%s]", szTitle))
        {
            continue;
        }
        szTitle[strlen(szTitle)-1] = '\0';
        DWORD dwKey = (DWORD) atoi(szTitle);
        // get info ...
        CWeaponInfo info;
        BOOL bSuc = TRUE;
        char szEffect[256] = "";
        bSuc &= fscanf(fp, "HitEffect=%s\n", szEffect);
        info.strHitEffect = szEffect;
        if (info.strHitEffect == "none")
        {
            info.strHitEffect = "";
        }
        char szSound[256] = "";
        bSuc &= fscanf(fp, "HitSound=%s\n", szSound);
        info.strHitSound = szSound;
        if (info.strHitSound == "none")
        {
            info.strHitSound = "";
        }
        bSuc &= fscanf(fp, "BlkEffect=%s\n", szEffect);
        info.strBlkEffect = szEffect;
        if (info.strBlkEffect == "none")
        {
            info.strBlkEffect = "";
        }
        bSuc &= fscanf(fp, "BlkSound=%s\n", szSound);
        info.strBlkSound = szSound;
        if (info.strBlkSound == "none")
        {
            info.strBlkSound = "";
        }
        if (!bSuc)
        {
            ::ErrorOut("error data: %s of %s", szTitle, INI_FILE);
            return FALSE;
        }
        // keep info
        m_setWeaponInfo[dwKey] = info;
    }
    fclose(fp);
    return TRUE;
}

//////////////////////////////////////////////////////////////////////
CONST CWeaponInfo* C3DRoleData::GetWeaponInfo (DWORD dwKey) CONST
{
    std::map< DWORD, CWeaponInfo >::const_iterator iter = m_setWeaponInfo.find(dwKey);
    if (iter == m_setWeaponInfo.end())
    {
        return NULL;
    }

    return &((*iter).second);
}
