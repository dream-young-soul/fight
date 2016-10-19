
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TerrainEffectManager.cpp: implementation of the TerrainEffectManager class.
//
//////////////////////////////////////////////////////////////////////

#include "TerrainEffectManager.h"
#include "3DGameMap.h"
#include "Physics.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTerrainEffectManager::CTerrainEffectManager()
{
}

CTerrainEffectManager::~CTerrainEffectManager()
{
    this->Destroy();
}

//-----------------------------------------------------------------
void CTerrainEffectManager::Destroy()
{
    int nAmount = m_setEffect.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CTerainEffectUnit* pUnit = m_setEffect[i];
        SAFE_DELETE(pUnit);
    }
    m_setEffect.clear();
}
//-----------------------------------------------------------------
void CTerrainEffectManager::Process()
{
    int nAmount = m_setEffect.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        CTerainEffectUnit* pUnit = m_setEffect[i];
        if (pUnit)
        {
            if (pUnit->Process())
            {
                SAFE_DELETE(pUnit);
                m_setEffect.erase(m_setEffect.begin() + i);
            }
        }
    }
}
//-----------------------------------------------------------------

/*###################################################################*/

//-----------------------------------------------------------------
CTerainEffectUnit::CTerainEffectUnit()
{
    m_dwTimeBegin = ::TimeGet();
    m_nDir = -1;
    m_setMapEffect.clear();
}

CTerainEffectUnit::~CTerainEffectUnit()
{
    this->Die();
}

//------------------------------------------------------------------

void CTerainEffectUnit::Destroy()
{
    int nAmount = m_setUnit.size();
    for(int i = 0; i < nAmount; i ++)
    {
        TerrainEffectUnitInfo* pInfo = m_setUnit[i];
        SAFE_DELETE(pInfo);
    }
    m_setUnit.clear();
    m_setMapEffect.clear();
}

//------------------------------------------------------------------

void CTerainEffectUnit::Die()
{
    int nAmount = m_setMapEffect.size();
    for(int i = 0; i < nAmount; i ++)
    {
        m_setMapEffect[i]->Die();
    }
    m_setMapEffect.clear();
    this->Destroy();
}

//-------------------------------------------------------------------

BOOL CTerainEffectUnit::Process()
{
    int nEffectAmount = m_setMapEffect.size();
    for(int j = nEffectAmount - 1; j >= 0; j --)
    {
        if (m_setMapEffect[j]->IsOver())
        {
            m_setMapEffect[j]->Die();
            m_setMapEffect.erase(m_setMapEffect.begin() + j);
        }
    }
    int nAmount = m_setUnit.size();
    if (nAmount == 0 && m_setMapEffect.size() == 0)
    {
        return true;
    }
    for(int i = nAmount - 1; i >= 0; i --)
    {
        TerrainEffectUnitInfo* pInfo = m_setUnit[i];
        if (pInfo)
        {
            if (::TimeGet() -  m_dwTimeBegin >= pInfo->dwDelay +::RandGet(pInfo->dwRandomDelay))
            {
                CMyPos posEffect;
                posEffect.x = m_posWorld.x + pInfo->nOffsetX;
                posEffect.y = m_posWorld.y + pInfo->nOffsetY;
                C3DMapEffect* pEffect = g_objGameMap.Add3DMapEffect(posEffect, pInfo->szIndex, false);
                m_setMapEffect.push_back(pEffect);
                SAFE_DELETE(pInfo);
                m_setUnit.erase(m_setUnit.begin() + i);
                if (pEffect && m_nDir != -1)
                {
                    pEffect->SetRotate((float)((-45 * m_nDir - 135) * 3.14 / 180), 0.0);
                }
            }
        }
    }
    return false;
}

//------------------------------------------------------------------

BOOL CTerainEffectUnit::Create(char* pszFileName)
{
    if (!pszFileName)
    {
        return false;
    }
    FILE* fp = fopen(pszFileName, "rb");
    if (!fp)
    {
        return false;
    }
    int nAmount = 0;
    fread(&nAmount, sizeof(DWORD), 1, fp);
    for(int i = 0; i < nAmount; i ++)
    {
        TerrainEffectUnitInfo* pInfo = new TerrainEffectUnitInfo;
        MYASSERT(pInfo);
        fread(pInfo, sizeof(TerrainEffectUnitInfo), 1, fp);
        pInfo->nOffsetX = -pInfo->nOffsetX;
        m_setUnit.push_back(pInfo);
    }
    fclose(fp);
    return true;
}
//------------------------------------------------------------------
CTerainEffectUnit* CTerainEffectUnit::CreateNew(char* pszFileName)
{
    CTerainEffectUnit* pUnit = new CTerainEffectUnit;
    MYASSERT(pUnit);
    if (pUnit->Create(pszFileName))
    {
        return pUnit;
    }
    SAFE_DELETE(pUnit);
    return NULL;
}
//------------------------------------------------------------------
void CTerainEffectUnit::Rotate(CMyPos posSender, CMyPos posTarget)
{
    CMyPos posMyTarget;
    posMyTarget.x = posTarget.x - posSender.x;
    posMyTarget.y = posTarget.y - posSender.y;
    int nAmount = m_setUnit.size();
    for(int i = 0; i  < nAmount; i ++)
    {
        TerrainEffectUnitInfo* pInfo = m_setUnit[i];
        MYASSERT(pInfo);
        CMyPos posPoint;
        posPoint.x = pInfo->nOffsetX;
        posPoint.y = pInfo->nOffsetY;
        CPhysics::S2DR45VP(posMyTarget, posPoint);
        pInfo->nOffsetX = posPoint.x;
        pInfo->nOffsetY = posPoint.y;
    }
}
