
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Game3DEffect.cpp: implementation of the CGame3DEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "Game3DEffect.h"
#include "3DRolePart.h"
#include "GameDataSet.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGame3DEffect::CGame3DEffect()
{
    m_idEffect		= ID_NONE;
    m_idTexture		= ID_NONE;
    m_pShapeInfoBuf = NULL;
    m_nShapes		= 0;
}

CGame3DEffect::~CGame3DEffect()
{
    C3DEffect::DestroyShapeInfo(m_pShapeInfoBuf, m_nShapes);
}

//-----------------------------------------------------------------
void CGame3DEffect::ResetShape(void)
{
    C3DEffect* pEffect = g_objGameDataSet.Get3DEffect(m_idEffect);
    if (pEffect)
    {
        pEffect->ResetShape();
    }
}

//-----------------------------------------------------------------
void CGame3DEffect::BackupShapeInfo(void)
{
    C3DEffect* pEffect = g_objGameDataSet.Get3DEffect(m_idEffect);
    if (pEffect)
    {
        m_pShapeInfoBuf = pEffect->CreateShapeInfo();
        m_nShapes		= pEffect->GetShapeAmount();
        pEffect->BackupShapeInfo(m_pShapeInfoBuf);
    }
}

//-----------------------------------------------------------------
void CGame3DEffect::Draw(C3DRolePart* pPart)
{
    if (m_idTexture == ID_NONE || m_idEffect == ID_NONE)
    {
        return;
    }
    if (!pPart)
    {
        return;
    }
    C3DObjPartInfo* pInfo = pPart->GetInfo(0);
    if (!pInfo)
    {
        return;
    }
    C3DObj* pObj = g_objGameDataSet.Get3DObj(pInfo->idMesh);
    if (!pObj)
    {
        return;
    }
    C3DEffect* pEffect = g_objGameDataSet.Get3DEffect(m_idEffect);
    if (!pEffect)
    {
        return;
    }
    C3DTexture* pTexture = g_objGameDataSet.Get3DTexture(m_idTexture);
    if (!pTexture)
    {
        return;
    }
    pEffect->SetTexture(pTexture);
    pEffect->RestoreShapeInfo(m_pShapeInfoBuf);
    pEffect->Bind(pObj);
    pEffect->Draw();
    pEffect->BackupShapeInfo(m_pShapeInfoBuf);
}
//-----------------------------------------------------------------
void CGame3DEffect::Rotate(float x, float y, float z)
{
    C3DEffect* pEffect = g_objGameDataSet.Get3DEffect(m_idEffect);
    if (pEffect)
    {
        pEffect->Rotate(x, y, z);
    }
}
//-----------------------------------------------------------------
void CGame3DEffect::Scale(float x, float y, float z)
{
    C3DEffect* pEffect = g_objGameDataSet.Get3DEffect(m_idEffect);
    if (pEffect)
    {
        pEffect->Scale(x, y, z);
    }
}
//-----------------------------------------------------------------
