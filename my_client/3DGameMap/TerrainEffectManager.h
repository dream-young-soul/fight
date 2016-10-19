
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TerrainEffectManager.h: interface for the TerrainEffectManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TERRAINEFFECTMANAGER_H__F110DACB_E760_4EA0_B002_647FCC56F4E8__INCLUDED_)
#define AFX_TERRAINEFFECTMANAGER_H__F110DACB_E760_4EA0_B002_647FCC56F4E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"
#include "3DMapEffect.h"

typedef struct
{
    char	szIndex[128];
    DWORD	dwDelay;
    DWORD	dwRandomDelay;
    int		nOffsetX;
    int		nOffsetY;
} TerrainEffectUnitInfo;

#include <vector>
using namespace std;
typedef vector<TerrainEffectUnitInfo*> VECTOR_TERRAINEFFECTUNIT;

class CTerainEffectUnit
{
public:
    CTerainEffectUnit();
    ~CTerainEffectUnit();
private:
    VECTOR_TERRAINEFFECTUNIT m_setUnit;
    CMyPos m_posWorld;
    DWORD  m_dwTimeBegin;
    int    m_nDir;
    vector<C3DMapEffect*> m_setMapEffect;
public:
    BOOL Process();
    void Destroy();
public:
    void SetPos(CMyPos posWorld) {m_posWorld = posWorld;}
    void AddUnit(TerrainEffectUnitInfo* pInfo) {m_setUnit.push_back(pInfo);}
    BOOL Create(char* pszFileName);
    void Die();
    static CTerainEffectUnit* CreateNew(char* pszFileName);
    void SetDir(int nDir) {m_nDir = nDir;}
    void Rotate(CMyPos posSender, CMyPos posTarget);
};


typedef vector<CTerainEffectUnit*> VECTOR_TERRAINEFFECT;
class CTerrainEffectManager
{
public:
    CTerrainEffectManager();
    virtual ~CTerrainEffectManager();
private:
    VECTOR_TERRAINEFFECT m_setEffect;
public:
    void Process();
    void Destroy();
    void AddUnit(CTerainEffectUnit* pUnit) {m_setEffect.push_back(pUnit);}
};
#endif // !defined(AFX_TERRAINEFFECTMANAGER_H__F110DACB_E760_4EA0_B002_647FCC56F4E8__INCLUDED_)
