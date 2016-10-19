
//**********************************************************
// 代码编辑器
//**********************************************************

//-------------------------------------------------------------------------
// MagicEffectArrow.cpp
//-------------------------------------------------------------------------

#include "MagicEffect.h"
#include "Magic.h"
#include "GamePlayerSet.h"
#include "Player.h"
#include "Hero.h"
#include "3DMapEffect.h"
#include "3DGameMap.h"
#include "../NdSound/Ndsound.h"
#include "Physics.h"
#include "3DMapSimpleObj.h"
#include "3DRoleData.h"

//--------------------------------------------------------------------
CMagicEffect* CMagicEffect::CreateNewArrow(OBJID idSender, OBJID idTarget, int nType, DWORD dwDelay)
{
    CMagicEffect* pEffect = new CMagicEffect;
    if (pEffect->CreateArrow(idSender, idTarget, nType, dwDelay))
    {
        return pEffect;
    }
    SAFE_DELETE(pEffect);
    return NULL;
}
//--------------------------------------------------------------------
BOOL CMagicEffect::CreateArrow(OBJID idSender, OBJID idTarget, int nArrowType, DWORD dwDelay)
{
    // check sender
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idSender);
    if (!pPlayer && g_objHero.GetID() == idSender)
    {
        pPlayer = &g_objHero;
    }
    if (!pPlayer)
    {
        return false;
    }
    CPlayer* pTarget = g_objPlayerSet.GetPlayer(idTarget);
    if (!pTarget && g_objHero.GetID() == idTarget)
    {
        pTarget = &g_objHero;
    }
    if (!pTarget)
    {
        return false;
    }
    m_idSender = idSender;
    m_idTarget = idTarget;
    m_nType = MAGICEFFECT_ARROW;
    CMyPos posSender, posTarget, posCell;
    const int _OFFSET_Y = -60;
    pPlayer->GetPos(posCell);
    g_objGameMap.Cell2World(posCell.x, posCell.y, posSender.x, posSender.y);
    m_posBegin = posSender;
    pTarget->GetPos(posCell);
    g_objGameMap.Cell2World(posCell.x, posCell.y, posTarget.x, posTarget.y);
    m_posEnd = posTarget;
    m_posEnd.y += _OFFSET_Y;
    m_dwData = nArrowType;
    m_dwDelay = dwDelay;
    // 第一次修正
    int nDesc = (int)sqrt(float((posSender.x - posTarget.x) * (posSender.x - posTarget.x)
                           + (posSender.y - posTarget.y) * (posSender.y - posTarget.y)));
    if (nDesc <= 0)
    {
        nDesc = 1;
    }
    CMyPos posOffset;
    posOffset.x = (posSender.x - m_posEnd.x) * 25 / nDesc;
    posOffset.y = (posSender.y - m_posEnd.y) * 25 / nDesc;
    m_posBegin.x -= posOffset.x;
    m_posBegin.y -= posOffset.y;
    // 第二次修正
    m_posBegin.y += _OFFSET_Y - pPlayer->GetFlyOffset();
    return true;
}
//--------------------------------------------------------------------
BOOL CMagicEffect::ProcessArrow()
{
    typedef struct
    {
        C3DMapSimpleObj* pObj;
        float fHorizontal;
        float fVertical;
        char  szFlyingSound[_MAX_PATH];
        char  szHitSound[_MAX_PATH];
        char  szEffectIndex[64];
        char  szTargetEffect[64];
    } ArrowInfo;
    if (!m_bDelayOk)
    {
        const DWORD dwArrowDelay = 800;
        if (::TimeGet() - m_dwBegin >= dwArrowDelay + m_dwDelay)
        {
            m_bDelayOk = true;
        }
        return false;
    }
    if (!m_bBegin)
    {
        if (!m_pData)
        {
            m_pData = new ArrowInfo;
        }
        if (!m_pData)
        {
            return true;
        }
        ArrowInfo* pInfo = (ArrowInfo*)m_pData;
        // get flying obj ini ...
        OBJID idSimpleObj;
        //----------------------------------------------------
        {
            FlyingObjInfo* pMyInfo = g_obj3DRoleData.GetFlyingObjInfo(m_dwData);
            MYASSERT(pInfo);
            idSimpleObj = pMyInfo->idSimpleObj;
            strcpy(pInfo->szEffectIndex, pMyInfo->szFlyEffect);
            strcpy(pInfo->szTargetEffect, pMyInfo->szTargetEffect);
        }
        //----------------------------------------------------
        m_bBegin = true;
        m_dwMoment = 0;
        m_dwBegin = ::TimeGet();
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idSender, true);
        if (!pPlayer)
        {
            return true;
        }
        C3DMapSimpleObj* pObj = C3DMapSimpleObj::CreateNew(idSimpleObj, m_posBegin, true);
        CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
        if (pLayer && pObj)
        {
            if (strcmp(pInfo->szEffectIndex, "NULL") != 0)
            {
                pObj->AddEffect(pInfo->szEffectIndex);
            }
            pLayer->AddMapObj(pObj);
        }
        else
        {
            SAFE_DELETE(pObj);
            SAFE_DELETE(pInfo);
            return true;
        }
        pInfo->pObj = pObj;
        CMy3DPos posBegin, posEnd;
        CMy3DDir dir3D;
        posBegin.x = m_posBegin.x;
        posBegin.y = m_posBegin.y;
        posBegin.z = 0;
        posEnd.x = m_posEnd.x;
        posEnd.y = m_posEnd.y;
        posEnd.z = 0;
        CPhysics::S3Get3DDir(posBegin, posEnd, dir3D);
        pInfo->fHorizontal = dir3D.fHorizontal;
        pInfo-> fVertical = dir3D.fVertical;
        const double fPei = 3.14159;
        pObj->SetRotate(dir3D.fHorizontal, dir3D.fVertical);
        m_dwBegin = ::TimeGet();
        return false;
    }
    ArrowInfo* pInfo = (ArrowInfo*)m_pData;
    // 开始飞行
    if (!pInfo)
    {
        return true;
    }
    int nV = 1600;
    float fA = 0;
    DWORD dwTime = ::TimeGet() - m_dwBegin;
    float fTime = (float)(dwTime * 1.0f / 1000);
    CMyPos posCurrent;
    if (CPhysics::S2DBMGetPos(nV, fA, fTime, m_posBegin, m_posEnd, posCurrent))
    {
        m_pData = NULL;
        for(int i = 0; i < (int)m_setRoleInfo.size(); i++)
        {
            MagicEffectRoleInfo* pRloeInfo = m_setRoleInfo[i];
            CPlayer* pTarget = NULL;
            CPlayer* pSender = NULL;
            if (pRloeInfo)
            {
                pTarget = g_objPlayerSet.GetPlayer(pRloeInfo->idRole, true);
            }
            pSender = g_objPlayerSet.GetPlayer(m_idSender, true);
            if (pTarget && pSender)
            {
                pTarget->m_objEffect.Add(pInfo->szTargetEffect, false, 0, pTarget->GetLook());
                if (pRloeInfo->dwData > 0)
                {
                    if (!pTarget->IsAttacking() && !pTarget->TestStatus(USERSTATUS_DIE))
                    {
                        pTarget->SetIntoneCmd(_ACTION_BRUISE0, pTarget->GetDir(pSender));
                    }
                    DWORD dwColor = _COLOR_GREEN;
                    if (g_objHero.GetID() == pTarget->GetID())
                    {
                        dwColor = _COLOR_RED;
                    }
                    pTarget->AddZoomNum(pRloeInfo->dwData, dwColor, 300, 30);
                    if (pTarget->IsNpc())
                    {
                        int nLife = (int)pTarget->GetData(CPlayer::_PLAYER_LIFE) - (int)pRloeInfo->dwData;
                        pTarget->SetData(CPlayer::_PLAYER_LIFE, __max(0, nLife));
                        //pTarget->SetLife(__max(0, nLife));
                    }
                }
                else
                {
                    if (!pTarget->IsAttacking() && !pTarget->TestStatus(USERSTATUS_DIE) && pTarget->IsPlayer())
                    {
                        pTarget->SetIntoneCmd(_ACTION_DODGE0, pTarget->GetDir(pSender));
                    }
                }
            }
        }
        if (pInfo->pObj)
        {
            pInfo->pObj->Die();
        }
        SAFE_DELETE(pInfo);
        return true;
    }
    else
    {
        if (pInfo->pObj)
        {
            pInfo->pObj->SetWorldPos(posCurrent);
        }
        else
        {
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------
