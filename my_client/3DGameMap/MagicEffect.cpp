
//**********************************************************
// 代码编辑器
//**********************************************************

// MagicEffect.cpp: implementation of the CMagicEffect class.
//
//////////////////////////////////////////////////////////////////////

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
#include "TerrainEffectManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMagicEffect::CMagicEffect()
{
    m_nType = MAGICEFFECT_NONE;
    m_pData = NULL;
    m_bBegin = false;
    m_bDelayOk = false;
    m_dwBegin = ::TimeGet();
    m_dwBegin = 0;
    m_dwDelay = 0;
}

CMagicEffect::~CMagicEffect()
{
    this->Destroy();
}
//--------------------------------------------------------------------
void CMagicEffect::Destroy()
{
    int nAmount = m_setRoleInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        MagicEffectRoleInfo* pInfo = m_setRoleInfo[i];
        SAFE_DELETE(pInfo);
    }
    m_setRoleInfo.clear();
}
//--------------------------------------------------------------------
CMagicEffect* CMagicEffect::CreateNew(OBJID idSender, OBJID idTarget, DWORD dwMagicType, DWORD dwLevel, int nType, int nIndex)
{
    CMagicEffect* pEffect = new CMagicEffect;
    MYASSERT(pEffect);
    if (!pEffect->Create(idSender, idTarget, dwMagicType, dwLevel, nType, nIndex))
    {
        SAFE_DELETE(pEffect);
        return NULL;
    }
    return pEffect;
}
//--------------------------------------------------------------------
BOOL CMagicEffect::Create(OBJID idSender, OBJID idTarget, DWORD dwMagicType, DWORD dwLevel, int nType, int nIndex)
{
    m_idSender = idSender;
    m_idTarget = idTarget;
    m_dwMagicType = dwMagicType;
    m_dwLevel = dwLevel;
    m_nType = nType;
    m_dwBegin = 0;
    if (nType == MAGICEFFECT_SOUL)
    {
        m_dwDelay = ::RandGet(5) * 300 + 1000;
        m_fSpeedA = (float)(::RandGet(300) + 50);
        m_nIndex = nIndex;
    }
    return true;
}
//--------------------------------------------------------------------
void CMagicEffect::AddRoleInfo(OBJID idRole, DWORD dwData)
{
    MagicEffectRoleInfo* pInfo = new MagicEffectRoleInfo;
    pInfo->dwData = dwData;
    pInfo->idRole = idRole;
    m_setRoleInfo.push_back(pInfo);
}
//--------------------------------------------------------------------
BOOL CMagicEffect::Process()
{
    // return true if this effect is end
    if (m_nType == MAGICEFFECT_NONE)
    {
        MagicTypeInfo infoMagicType;
        if (!CMagic::s_objMagicData.GetMagicTypeInfo(m_dwMagicType, m_dwLevel, infoMagicType))
        {
            return true;
        }
        // get type ...
        m_nType = infoMagicType.dwClientRepresent;
    }
    switch(m_nType)
    {
    case MAGICEFFECT_BODY_BODY:
        return this->ProcessBodyBody();
        break;
    case MAGICEFFECT_SOUL:
        return this->ProcessSoul();
        break;
    case MAGICEFFECT_ARROW:
        return this->ProcessArrow();
        break;
    case MAGICEFFECT_BODY_TERRAIN_BODY:
        return this->ProcessBodyTerrainBody();
        break;
    case MAGICEFFECT_BODY_TRACE_BODY:
        return this->ProcessBodyTraceBody();
        break;
    case MAGICEFFECT_BODY_TRACE_POS:
        return this->ProcessBodyTracePos();
        break;
    default:
        return true;
    }
}


//--------------------------------------------------------------------
BOOL CMagicEffect::ProcessSoul()
{
    if (!m_bBegin)
    {
        // 创建 3dmapeffect
        if (m_dwBegin == 0)
        {
            m_dwBegin = ::TimeGet();
        }
        if (::TimeGet() - m_dwBegin < m_dwDelay)
        {
            return false;
        }
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idSender, true);
        if (!pPlayer)
        {
            return true;
        }
        CMyPos posWorld, posCell;
        pPlayer->GetPos(posCell);
        g_objGameMap.Cell2World(posCell.x, posCell.y, posWorld.x, posWorld.y);
        m_nIndex = max(::RandGet(12) * 6, 40);
        m_posBegin.x = posWorld.x + ::RandGet(10) * 10 - 85;
        m_posBegin.y = posWorld.y;
        g_objGameMap.World2Cell(m_posBegin.x, m_posBegin.y, posCell.x, posCell.y);
        CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
        if (!pLayer)
        {
            return true;
        }
        m_pData = new MagicEffectSoulInfo;
        if (!m_pData)
        {
            return true;
        }
        MagicEffectSoulInfo* pInfo = (MagicEffectSoulInfo*)m_pData;
        for(int i = 0; i < 1; i ++)
        {
            C3DMapEffect*  pEffect = C3DMapEffect::CreateNew(m_posBegin, "pet53", false);
            pLayer->AddMapObj(pEffect);
            pEffect->SetPos(posCell);
            pInfo->pEffect[i] = pEffect;
        }
        m_bBegin = true;
        m_dwMoment = 0;
        m_dwBegin = ::TimeGet();
        return false;
    }
    if (m_dwMoment == 0)
    {
        if (!m_pData)
        {
            return true;
        }
        MagicEffectSoulInfo* pInfo = (MagicEffectSoulInfo*)m_pData;
        // 开始缓缓上升 1 秒上升到高度 150 // 匀速 // 只改变世界坐标
        if (::TimeGet() - m_dwBegin >= m_dwDelay)
        {
            m_dwMoment = 1;
            m_dwData = ::TimeGet();
            CMyPos posBg, posWorld;
            pInfo->pEffect[0]->GetWorldPos(posBg); //BG POS
            g_objGameMap.Bg2World(posBg.x, posBg.y, posWorld.x, posWorld.y);
            m_posBegin = posWorld;
        }
        else
        {
            int nOffsetY = m_nIndex * (::TimeGet() - m_dwBegin) / m_dwDelay;
            CMyPos posCurrent = m_posBegin;
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idSender, true);
            if (!pPlayer)
                for(int i = 0; i < 1; i ++)
                {
                    pInfo->pEffect[i]->Die();
                    return true;
                }
            {
                pInfo->pEffect[0]->SetWorldPos(posCurrent);
                pInfo->pEffect[0]->SetOffsetY(-nOffsetY);
                CMyPos posCell;
                g_objGameMap.World2Cell(posCurrent.x, posCurrent.y, posCell.x, posCell.y);
                pInfo->pEffect[0]->SetPos(posCell);
                return false;
            }
        }
    }
    if (m_dwMoment == 1)
    {
        if (::TimeGet() - m_dwData >= 800)
        {
            m_dwMoment = 2;
            m_dwData = ::TimeGet();
        }
        else
        {
            return false;
        }
    }
    if (m_dwMoment == 2)
    {
        if (!m_pData)
        {
            return true;
        }
        MagicEffectSoulInfo* pInfo = (MagicEffectSoulInfo*)m_pData;
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idTarget, true);
        if (!pPlayer)
        {
            for(int i = 0; i < 1; i ++)
            {
                pInfo->pEffect[i]->Die();
            }
            return true;
        }
        // 加速画线到人身上 // 初速度 0 // 加速度 300 象素/秒
        CMyPos posEnd, posCurrent, posCell;
        int nV = 400;
        float fA = (float)m_fSpeedA;
        DWORD dwTime = ::TimeGet() - m_dwBegin - m_dwDelay - 800;
        float fTime = (float)(dwTime * 1.0 / 1000);
        int nCurrentV =(int)( nV + fTime * fA);
        float fCurrentTime = (float)((::TimeGet() - m_dwData) * 1.0f / 1000);;
        pPlayer->GetPos(posCell);
        g_objGameMap.Cell2World(posCell.x, posCell.y, posEnd.x, posEnd.y);
        posEnd.y += 50;
        BOOL bOk = CPhysics::S2DBMGetPos(nCurrentV, 0, fCurrentTime, m_posBegin, posEnd, posCurrent);
        m_posBegin = posCurrent;
        m_dwData = ::TimeGet();
        {
            pInfo->pEffect[0]->SetWorldPos(posCurrent);
        }
        g_objGameMap.World2Cell(posCurrent.x, posCurrent.y, posCell.x, posCell.y);
        for(int i = 0; i < 1; i ++)
        {
            pInfo->pEffect[i]->SetPos(posCell);
        }
        if (bOk)
        {
            for(int i = 0; i < 1; i ++)
            {
                pInfo->pEffect[i]->Die();
            }
            SAFE_DELETE(m_pData);
            CCommand cmd;
            cmd.iType		= _COMMAND_EMOTION;
            cmd.iStatus		= _CMDSTATUS_BEGIN;
            cmd.nData		= _ACTION_RELIVE;
            cmd.nDir		= pPlayer->GetDir();
            pPlayer->SetCommand(&cmd);
            pPlayer->m_objEffect.Add("pet54");
        }
        return bOk;
    }
    return true;
}
//--------------------------------------------------------------------
BOOL CMagicEffect::ProcessSoulEx()
{
    if (!m_bBegin)
    {
        if (m_dwBegin == 0)
        {
            m_dwBegin = ::TimeGet();
        }
        if (m_dwDelay > 0 && ::TimeGet() - m_dwBegin < m_dwDelay)
        {
            return false;
        }
        m_bBegin = true;
        m_dwMoment = 0;
        // 创建 3dmapeffect
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idSender, true);
        if (!pPlayer)
        {
            return true;
        }
        CPlayer* pTarget = g_objPlayerSet.GetPlayer(m_idTarget, true);
        if (pTarget )
        {
            m_nDir = pPlayer->GetDir(pTarget);
        }
        else
        {
            m_nDir = 0;
        }
        CMyPos posWorld, posCell;
        pPlayer->GetPos(posCell);
        g_objGameMap.Cell2World(posCell.x, posCell.y, posWorld.x, posWorld.y);
        m_posBegin = posWorld;
        CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
        if (!pLayer)
        {
            return true;
        }
        m_pData = new MagicEffectSoulInfo;
        if (!m_pData)
        {
            return true;
        }
        MagicEffectSoulInfo* pInfo = (MagicEffectSoulInfo*)m_pData;
        for(int i = 0; i < 1; i ++)
        {
            C3DMapEffect*  pEffect = C3DMapEffect::CreateNew(posWorld, "soul", false);
            pLayer->AddMapObj(pEffect);
            pEffect->SetPos(posCell);
            pInfo->pEffect[i] = pEffect;
        }
        m_dwBegin = ::TimeGet();
        return false;
    }
    if (m_dwMoment == 0)
    {
        if (!m_pData)
        {
            return true;
        }
        MagicEffectSoulInfo* pInfo = (MagicEffectSoulInfo*)m_pData;
        // 从sender身上冲出500 秒
        if (::TimeGet() - m_dwBegin >= 200)
        {
            m_dwMoment = 1;	//进入悬停状态
            m_dwData = ::TimeGet();
            CMyPos posBg, posWorld;
            pInfo->pEffect[0]->GetWorldPos(posBg); //BG POS
            g_objGameMap.Bg2World(posBg.x, posBg.y, posWorld.x, posWorld.y);
            m_posBegin = posWorld;
        }
        else
        {
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idSender, true);
            if (!pPlayer)
                for(int i = 0; i < 1; i ++)
                {
                    pInfo->pEffect[i]->Die();
                    return true;
                }
            CMyPos posCurrent ;
            int nX, nY;
            this->GetSoulDirectPos(m_nIndex, m_nDir, nX, nY);
            int nCurrentTime = ::TimeGet() - m_dwBegin;
            posCurrent.x = m_posBegin.x - nX * nCurrentTime / 200;
            posCurrent.y = m_posBegin.y - nY * nCurrentTime / 200;
            pInfo->pEffect[0]->SetWorldPos(posCurrent);
            CMyPos posCell;
            g_objGameMap.World2Cell(posCurrent.x, posCurrent.y, posCell.x, posCell.y);
            pInfo->pEffect[0]->SetPos(posCell);
            return false;
        }
    }
    if (m_dwMoment == 1)
    {
        if (::TimeGet() - m_dwData >= 1000)
        {
            m_dwMoment = 2;
            m_dwData = ::TimeGet();
        }
        else
        {
            return false;
        }
    }
    if (m_dwMoment == 2)
    {
        if (!m_pData)
        {
            return true;
        }
        MagicEffectSoulInfo* pInfo = (MagicEffectSoulInfo*)m_pData;
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idTarget, true);
        if (!pPlayer)
        {
            for(int i = 0; i < 1; i ++)
            {
                pInfo->pEffect[i]->Die();
            }
            return true;
        }
        // 加速画线到人身上 // 初速度 0 // 加速度 300 象素/秒
        CMyPos posEnd, posCurrent, posCell;
        int nV = 100;
        float fA = m_fSpeedA;
        DWORD dwTime = ::TimeGet() - m_dwBegin - 1200;
        float fTime = (float)(dwTime * 1.0f / 1000);
        int nCurrentV = (int)(nV + fTime * fA);
        float fCurrentTime =(float)( (::TimeGet() - m_dwData) * 1.0f / 1000);;
        pPlayer->GetPos(posCell);
        g_objGameMap.Cell2World(posCell.x, posCell.y, posEnd.x, posEnd.y);
        posEnd.y += 50;
        BOOL bOk = CPhysics::S2DBMGetPos(nCurrentV, 0, fCurrentTime, m_posBegin, posEnd, posCurrent);
        m_posBegin = posCurrent;
        m_dwData = ::TimeGet();
        {
            pInfo->pEffect[0]->SetWorldPos(posCurrent);
        }
        g_objGameMap.World2Cell(posCurrent.x, posCurrent.y, posCell.x, posCell.y);
        for(int i = 0; i < 1; i ++)
        {
            pInfo->pEffect[i]->SetPos(posCell);
        }
        if (bOk)
        {
            for(int i = 0; i < 1; i ++)
            {
                pInfo->pEffect[i]->Die();
            }
            SAFE_DELETE(m_pData);
            if (pPlayer->GetID() == g_objHero.GetID())
            {
                g_objHero.m_objEffect.Add("GreenGlow");
            }
        }
        return bOk;
    }
    return true;
}

//--------------------------------------------------------------------

BOOL CMagicEffect::ProcessBodyBody()
{
    MagicTypeInfo infoMagicType;
    if (!CMagic::s_objMagicData.GetMagicTypeInfo(m_dwMagicType, m_dwLevel, infoMagicType))
    {
        return true;
    }
    const int nRange = 3000;
    if (!m_bBegin)
    {
        m_dwBegin = ::TimeGet();
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idSender, true);
        if (pPlayer)
        {
            if (strcmp(infoMagicType.szSenderEffect, "NULL") != 0)
            {
                pPlayer->m_objEffect.Add(infoMagicType.szSenderEffect, false, 0, pPlayer->GetLook());
            }
            if (infoMagicType.dwSenderAction != 0)
                if (!pPlayer->IsDead())
                {
                    pPlayer->ResetStepDir();
                    if (infoMagicType.dwSenderAction == _ACTION_INTONE_DURATION)
                    {
                        infoMagicType.dwSenderAction = _ACTION_INTONE_LAUNCH;
                    }
                    CPlayer* pTarget = g_objPlayerSet.GetPlayer(m_idTarget);
                    if (infoMagicType.dwSenderAction != 0)
                    {
                        if (infoMagicType.idNextMagic != ID_NONE)
                        {
                            pPlayer->SetIntoneCmd(infoMagicType.dwSenderAction, pTarget ? pPlayer->GetDir(pTarget) : pPlayer->GetDir(), 0, true);
                        }
                        else
                        {
                            pPlayer->SetIntoneCmd(infoMagicType.dwSenderAction, pTarget ? pPlayer->GetDir(pTarget) : pPlayer->GetDir(), 0, false);
                        }
                    }
                }
            if (strcmp(infoMagicType.szSenderSound, "NULL") != 0)
            {
                CMyPos posWorld;
                pPlayer->GetWorldPos(posWorld);
                ::DXPlaySound(infoMagicType.szSenderSound, posWorld.x, posWorld.y, nRange);
            }
        }
        this->BeginScreenEffect(infoMagicType.dwScreenRepresent);
        m_bBegin = true;
        m_dwMoment = 0;
    }
    if (::TimeGet() - m_dwBegin >= infoMagicType.dwTargetDelay && m_dwMoment == 0)
    {
        for(int i = 0; i < (int)m_setRoleInfo.size(); i++)
        {
            MagicEffectRoleInfo* pRoleInfo = m_setRoleInfo[i];
            if (!pRoleInfo)
            {
                continue;
            }
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(pRoleInfo->idRole, true);
            if (pPlayer)
            {
                if (strcmp(infoMagicType.szTargetEffect, "NULL") != 0)
                {
                    pPlayer->m_objEffect.Add(infoMagicType.szTargetEffect, false, 0, pPlayer->GetLook());
                }
            }
        }
        m_dwMoment = 1;
    }
    if (::TimeGet() - m_dwBegin >= infoMagicType.dwTargetDelay + infoMagicType.dwTargetWoundDelay)
    {
        for(int i = 0; i < (int)m_setRoleInfo.size(); i++)
        {
            MagicEffectRoleInfo* pRoleInfo = m_setRoleInfo[i];
            if (!pRoleInfo)
            {
                continue;
            }
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(pRoleInfo->idRole, true);
            if (pPlayer)
            {
                if ((infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SINGLE_HP) ||
                        (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_CROSS_HP) ||
                        (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SECTOR_HP) ||
                        (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_ROUND_HP))
                    if (!pPlayer->TestStatus(USERSTATUS_DIE) && !pPlayer->IsMoving() && infoMagicType.dwSenderAction != 0)
                    {
                        pPlayer->ResetStepDir();
                        pPlayer->SetIntoneCmd(_ACTION_BRUISE0, pPlayer->GetDir());
                    }
                DWORD dwColor = _COLOR_YELLOW;
                if (pPlayer->GetID() == g_objHero.GetID())
                {
                    dwColor = _COLOR_RED;
                }
                if (infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_HP)
                {
                    dwColor = 0xff33ff33;
                }
                // 万箭穿心
                if (infoMagicType.idMagicType / 10 == 1006
                        ||	infoMagicType.idMagicType / 10 == 1005
                        ||	infoMagicType.idMagicType / 10 == 1007)
                {
                    int nAmount = infoMagicType.idMagicType % 10 + 2;
                    for(int j = 0; j < nAmount; j ++)
                    {
                        if (j != nAmount - 1)
                        {
                            if (pPlayer->IsGuard())
                            {
                                dwColor = _COLOR_BLUE;
                            }
                            pPlayer->AddZoomNum(pRoleInfo->dwData / nAmount, dwColor, 300 * j);
                        }
                        else
                        {
                            if (pPlayer->IsGuard())
                            {
                                dwColor = _COLOR_BLUE;
                            }
                            pPlayer->AddZoomNum(pRoleInfo->dwData -
                                                (nAmount - 1)*(pRoleInfo->dwData / nAmount),
                                                dwColor, 100 * i);
                        }
                    }
                }
                else
                {
                    if (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SINGLE_STATUS ||
                            infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_STATUS ||
                            infoMagicType.dwActionSort == MAGICSORT_ATTRACK_MONSTER)
                    {
                    }
                    else
                    {
                        if (pPlayer->IsGuard())
                        {
                            dwColor = _COLOR_BLUE;
                        }
                        pPlayer->AddZoomNum(pRoleInfo->dwData, dwColor, 100);
                    }
                }
                BOOL bAddLife = false;
                if (infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_HP)
                {
                    bAddLife = true;
                }
                if (pPlayer->GetID() != g_objHero.GetID())
                {
                    if (!bAddLife)
                    {
                        pPlayer->SetData(CPlayer::_PLAYER_LIFE, __max(0, (int)pPlayer->GetData(CPlayer::_PLAYER_LIFE) - (int)pRoleInfo->dwData));
                    }
                    else
                    {
                        pPlayer->SetData(CPlayer::_PLAYER_LIFE, __min((int)pPlayer->GetMaxLife(), (int)pPlayer->GetData(CPlayer::_PLAYER_LIFE) + (int)pRoleInfo->dwData));
                    }
                }
                if (strcmp(infoMagicType.szTargetSound, "NULL") != 0)
                {
                    CMyPos posWorld;
                    pPlayer->GetWorldPos(posWorld);
                    ::DXPlaySound(infoMagicType.szTargetSound, posWorld.x, posWorld.y, nRange);
                }
            }
        }
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------

BOOL  CMagicEffect::CreateTerrain(OBJID idSender, CMyPos posTarget, DWORD dwMagicType, DWORD dwLevel)
{
    m_idSender = idSender;
    m_posTarget = posTarget;
    m_dwMagicType = dwMagicType;
    m_dwLevel = dwLevel;
    m_nType = MAGICEFFECT_BODY_TERRAIN_BODY;
    return true;
}
//----------------------------------------------------------------------------------------
CMagicEffect* CMagicEffect::CreateNewTerrain(OBJID idSender, CMyPos posTarget, DWORD dwMagicType, DWORD dwLevel)
{
    CMagicEffect* pEffect = new CMagicEffect;
    MYASSERT(pEffect);
    if (!pEffect->CreateTerrain(idSender, posTarget, dwMagicType, dwLevel))
    {
        SAFE_DELETE(pEffect);
        return NULL;
    }
    return pEffect;
}
//----------------------------------------------------------------------------------------
BOOL CMagicEffect::ProcessBodyTerrainBody()
{
    MagicTypeInfo infoMagicType;
    if (!CMagic::s_objMagicData.GetMagicTypeInfo(m_dwMagicType, m_dwLevel, infoMagicType))
    {
        return true;
    }
    const int nRange = 3000;
    if (!m_bBegin)
    {
        m_dwBegin = ::TimeGet();
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idSender, true);
        if (pPlayer)
        {
            if (strcmp(infoMagicType.szSenderEffect, "NULL") != 0)
            {
                pPlayer->m_objEffect.Add(infoMagicType.szSenderEffect, false, 0, pPlayer->GetLook());
            }
            if (infoMagicType.dwSenderAction != 0)
                if (!pPlayer->IsDead() )
                {
                    if (infoMagicType.dwSenderAction == _ACTION_INTONE_DURATION)
                    {
                        infoMagicType.dwSenderAction = _ACTION_INTONE_LAUNCH;
                    }
                    pPlayer->ResetStepDir();
                    CPlayer* pTarget = g_objPlayerSet.GetPlayer(m_idTarget);
                    if (infoMagicType.idNextMagic != ID_NONE)
                    {
                        pPlayer->SetIntoneCmd(infoMagicType.dwSenderAction, pPlayer->GetDir(), 0, true);
                    }
                    else
                    {
                        pPlayer->SetIntoneCmd(infoMagicType.dwSenderAction, pPlayer->GetDir(), 0, false);
                    }
                }
            if (strcmp(infoMagicType.szSenderSound, "NULL") != 0)
            {
                CMyPos posWorld;
                pPlayer->GetWorldPos(posWorld);
                ::DXPlaySound(infoMagicType.szSenderSound, posWorld.x, posWorld.y, nRange);
            }
            // transport
            CONST int MAGICTYPE_TRASNPORT = 1080;
            if (MAGICTYPE_TRASNPORT == infoMagicType.idMagicType)
            {
                pPlayer->SetPos(m_posTarget);
                if (pPlayer->GetID() == g_objHero.GetID())
                {
                    g_objHero.SetAlignPos(m_posTarget.x, m_posTarget.y);
                }
            }
        }
        // 直线攻击不走地效魔法
        if (infoMagicType.dwActionSort == MAGICSORT_LINE)
        {
            CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
            if (!pLayer)
            {
                return true;
            }
            CMyPos posBegin;
            pPlayer->GetWorldPos(posBegin);
            CMyPos posWorld;
            g_objGameMap.Cell2World(m_posTarget.x, m_posTarget.y, posWorld.x, posWorld.y);
            C3DMapEffect* pObj = C3DMapEffect::CreateNew(posBegin, infoMagicType.szGroundEffect);
            if (pObj)
            {
                pLayer->AddMapObj(pObj);
                CMy3DPos posMyBegin, posMyEnd;
                CMy3DDir dir3D;
                posMyBegin.x = posBegin.x;
                posMyBegin.y = posBegin.y;
                posMyBegin.z = 0;
                posMyEnd.x = posWorld.x;
                posMyEnd.y = posWorld.y;
                posMyEnd.z = 0;
                CPhysics::S3Get3DDir(posMyBegin, posMyEnd, dir3D);
                pObj->SetRotate(dir3D.fHorizontal + 1.57f, dir3D.fVertical);
            }
        }
        else
        {
            // 添加地效魔法对象
            char szDir[] = "ini/tme/";
            char szFile[_MAX_PATH];
            sprintf(szFile, "%s%s", szDir, infoMagicType.szGroundEffect);
            CTerainEffectUnit* pUnit = CTerainEffectUnit::CreateNew(szFile);
            if (pUnit)
            {
                CMyPos posWorld;
                g_objGameMap.Cell2World(m_posTarget.x, m_posTarget.y, posWorld.x, posWorld.y);
                pUnit->SetPos(posWorld);
                // 判断是不是方向魔法
                if (infoMagicType.dwActionSort == MAGICSORT_LINE ||
                        MAGIC_SORT_ATTACK_CROSS_HP ==  infoMagicType.dwActionSort ||
                        MAGIC_SORT_ATTACK_SECTOR_HP	== infoMagicType.dwActionSort)
                {
                    CMyPos posSender;
                    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idSender);
                    if (!pPlayer && (m_idSender == g_objHero.GetID()))
                    {
                        pPlayer = &g_objHero;
                    }
                    if (pPlayer)
                    {
                        pPlayer->GetWorldPos(posSender);
                    }
                    pUnit->SetPos(posSender);
                    pUnit->Rotate(posSender, posWorld);
                    if (pPlayer)
                    {
                        CMyPos posPlayer;
                        pPlayer->GetPos(posPlayer);
                        //int nDir = g_objGameMap.GetDirection(posPlayer.x, posPlayer.y,	m_posTarget.x, m_posTarget.y);
                        pUnit->SetDir(m_nDir);
                    }
                }
                g_objGameMap.AddTerrainMagicUnit(pUnit);
            }
        }
        this->BeginScreenEffect(infoMagicType.dwScreenRepresent);
        m_bBegin = true;
    }
    if (::TimeGet() - m_dwBegin >= infoMagicType.dwTargetDelay)
    {
        for(int i = 0; i < (int)m_setRoleInfo.size(); i++)
        {
            MagicEffectRoleInfo* pRoleInfo = m_setRoleInfo[i];
            if (!pRoleInfo)
            {
                continue;
            }
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(pRoleInfo->idRole);
            if (!pPlayer && (pRoleInfo->idRole == g_objHero.GetID()))
            {
                pPlayer = &g_objHero;
            }
            if (pPlayer)
            {
                if ((infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SINGLE_HP) ||
                        (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_CROSS_HP) ||
                        (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SECTOR_HP) ||
                        (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_ROUND_HP))
                    if (!pPlayer->TestStatus(USERSTATUS_DIE) && !pPlayer->IsMoving() && infoMagicType.dwSenderAction != 0)
                    {
                        pPlayer->ResetStepDir();
                        pPlayer->SetIntoneCmd(_ACTION_BRUISE0, pPlayer->GetDir());
                    }
                if (strcmp(infoMagicType.szTargetEffect, "NULL") != 0)
                {
                    pPlayer->m_objEffect.Add(infoMagicType.szTargetEffect, false, 0, pPlayer->GetLook());
                }
                DWORD dwColor = _COLOR_YELLOW;
                if (pPlayer->GetID() == g_objHero.GetID())
                {
                    dwColor = _COLOR_RED;
                }
                if (infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_HP)
                {
                    dwColor = 0xff33ff33;
                }
                if (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SINGLE_STATUS ||
                        infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_STATUS)
                {
                }
                else
                {
                    if (pPlayer->IsGuard())
                    {
                        dwColor = _COLOR_BLUE;
                    }
                    pPlayer->AddZoomNum(pRoleInfo->dwData, dwColor, 100);
                }
                BOOL bAddLife = false;
                if (infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_HP)
                {
                    bAddLife = true;
                }
                if (pPlayer->GetID() != g_objHero.GetID())
                {
                    if (!bAddLife)
                    {
                        pPlayer->SetData(CPlayer::_PLAYER_LIFE, __max(0, (int)pPlayer->GetData(CPlayer::_PLAYER_LIFE) - (int)pRoleInfo->dwData));
                    }
                    else
                    {
                        pPlayer->SetData(CPlayer::_PLAYER_LIFE, __min((int)pPlayer->GetMaxLife(), (int)pPlayer->GetData(CPlayer::_PLAYER_LIFE) + (int)pRoleInfo->dwData));
                    }
                }
                if (strcmp(infoMagicType.szTargetSound, "NULL") != 0)
                {
                    CMyPos posWorld;
                    pPlayer->GetWorldPos(posWorld);
                    ::DXPlaySound(infoMagicType.szTargetSound, posWorld.x, posWorld.y, nRange);
                }
            }
        }
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------------------

BOOL CMagicEffect::ProcessBodyTraceBody()
{
    MagicTypeInfo infoMagicType;
    if (!CMagic::s_objMagicData.GetMagicTypeInfo(m_dwMagicType, m_dwLevel, infoMagicType))
    {
        return true;
    }
    const int nRange = 3000;
    if (!m_bBegin)
    {
        m_dwBegin = ::TimeGet();
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idSender);
        if (!pPlayer && (m_idSender == g_objHero.GetID()))
        {
            pPlayer = &g_objHero;
        }
        if (pPlayer)
        {
            if (strcmp(infoMagicType.szSenderEffect, "NULL") != 0)
            {
                pPlayer->m_objEffect.Add(infoMagicType.szSenderEffect, false, 0, pPlayer->GetLook());
            }
            if (infoMagicType.dwSenderAction != 0)
                if (!pPlayer->IsDead())
                {
                    pPlayer->ResetStepDir();
                    CPlayer* pTarget = g_objPlayerSet.GetPlayer(m_idTarget);
                    if (infoMagicType.dwSenderAction == _ACTION_INTONE_DURATION)
                    {
                        infoMagicType.dwSenderAction = _ACTION_INTONE_LAUNCH;
                    }
                    if (infoMagicType.idNextMagic != ID_NONE)
                    {
                        pPlayer->SetIntoneCmd(infoMagicType.dwSenderAction, pTarget ? pPlayer->GetDir(pTarget) : pPlayer->GetDir(), 0, true);
                    }
                    else
                    {
                        pPlayer->SetIntoneCmd(infoMagicType.dwSenderAction, pTarget ? pPlayer->GetDir(pTarget) : pPlayer->GetDir(), 0, false);
                    }
                }
            if (strcmp(infoMagicType.szSenderSound, "NULL") != 0)
            {
                CMyPos posWorld;
                pPlayer->GetWorldPos(posWorld);
                ::DXPlaySound(infoMagicType.szSenderSound, posWorld.x, posWorld.y, nRange);
            }
        }
        // 添加3DMapEffect
        CMyPos posWorld, posCell;
        pPlayer->GetPos(posCell);
        g_objGameMap.Cell2World(posCell.x, posCell.y, posWorld.x, posWorld.y);
        m_posBegin = posWorld;
        CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
        if (!pLayer)
        {
            return true;
        }
        C3DMapEffect* pObj = C3DMapEffect::CreateNew(m_posBegin, infoMagicType.szTraceEffect, false);
        if (pObj)
        {
            pLayer->AddMapObj(pObj);
        }
        m_pData = pObj;
        m_bBegin = true;
        this->BeginScreenEffect(infoMagicType.dwScreenRepresent);
        pPlayer = g_objPlayerSet.GetPlayer(m_idTarget, true);
        if (!pPlayer)
        {
            pObj->Die();
            pObj = NULL;
            return true;
        }
        CMyPos posEnd;
        CMy3DPos posMyBegin, posMyEnd;
        CMy3DDir dir3D;
        posMyBegin.x = m_posBegin.x;
        posMyBegin.y = m_posBegin.y;
        posMyBegin.z = 0;
        pPlayer->GetPos(posCell);
        g_objGameMap.Cell2World(posCell.x, posCell.y, posEnd.x, posEnd.y);
        posEnd.y += 50;
        posMyEnd.x = posEnd.x;
        posMyEnd.y = posEnd.y;
        posMyEnd.z = 0;
        CPhysics::S3Get3DDir(posMyBegin, posMyEnd, dir3D);
        pObj->SetRotate(dir3D.fHorizontal + 1.57f, dir3D.fVertical);
        return false;
    }
    C3DMapEffect* pObj = (C3DMapEffect*)m_pData;
    if (!pObj)
    {
        return false;
    }
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idTarget, true);
    if (!pPlayer)
    {
        pObj->Die();
        pObj = NULL;
        return true;
    }
    // 加速画线到人身上 // 初速度 0 // 加速度 300 象素/秒
    CMyPos posEnd, posCurrent, posCell;
    int nV = 100;
    float fA = (float)300;
    DWORD dwTime = ::TimeGet() - m_dwBegin;
    float fTime = (float)(dwTime * 1.0f / 1000);
    int nCurrentV =(int)( nV + fTime * fA);
    float fCurrentTime = (float)((::TimeGet() - m_dwBegin) * 1.0 / 1000);;
    pPlayer->GetPos(posCell);
    g_objGameMap.Cell2World(posCell.x, posCell.y, posEnd.x, posEnd.y);
    posEnd.y += 50;
    CMy3DPos posMyBegin, posMyEnd;
    CMy3DDir dir3D;
    posMyBegin.x = m_posBegin.x;
    posMyBegin.y = m_posBegin.y;
    posMyBegin.z = 0;
    posMyEnd.x = posEnd.x;
    posMyEnd.y = posEnd.y;
    posMyEnd.z = 0;
    CPhysics::S3Get3DDir(posMyBegin, posMyEnd, dir3D);
    pObj->SetRotate(dir3D.fHorizontal + 1.57f, dir3D.fVertical);
    BOOL bOk = CPhysics::S2DBMGetPos(nCurrentV, 0, fCurrentTime, m_posBegin, posEnd, posCurrent);
    m_posBegin = posCurrent;
    posCurrent.y -= 50;
    pObj->SetWorldPos(posCurrent);
    //	g_objGameMap.World2Cell(posCurrent.x, posCurrent.y, posCell.x, posCell.y);
    //	pEffect->SetPos(posCell);
    if (bOk)
    {
        for(int i = 0; i < (int)m_setRoleInfo.size(); i++)
        {
            MagicEffectRoleInfo* pRoleInfo = m_setRoleInfo[i];
            if (!pRoleInfo)
            {
                continue;
            }
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(pRoleInfo->idRole);
            if (!pPlayer && (pRoleInfo->idRole == g_objHero.GetID()))
            {
                pPlayer = &g_objHero;
            }
            if (pPlayer)
            {
                if ((infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SINGLE_HP) ||
                        (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_CROSS_HP) ||
                        (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SECTOR_HP) ||
                        (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_ROUND_HP))
                    if (!pPlayer->TestStatus(USERSTATUS_DIE) && !pPlayer->IsMoving() && infoMagicType.dwSenderAction != 0)
                    {
                        pPlayer->ResetStepDir();
                        pPlayer->SetIntoneCmd(_ACTION_BRUISE0, pPlayer->GetDir());
                    }
                if (strcmp(infoMagicType.szTargetEffect, "NULL") != 0)
                {
                    pPlayer->m_objEffect.Add(infoMagicType.szTargetEffect, false, 0, pPlayer->GetLook());
                }
                DWORD dwColor = _COLOR_YELLOW;
                if (pPlayer->GetID() == g_objHero.GetID())
                {
                    dwColor = _COLOR_RED;
                }
                if (infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_HP)
                {
                    dwColor = 0xff33ff33;
                }
                if (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SINGLE_STATUS ||
                        infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_STATUS)
                {
                }
                else
                {
                    if (pPlayer->IsGuard())
                    {
                        dwColor = _COLOR_BLUE;
                    }
                    pPlayer->AddZoomNum(pRoleInfo->dwData, dwColor, 100);
                }
                BOOL bAddLife = false;
                if (infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_HP)
                {
                    bAddLife = true;
                }
                if (pPlayer->GetID() != g_objHero.GetID())
                {
                    if (!bAddLife)
                    {
                        pPlayer->SetData(CPlayer::_PLAYER_LIFE, __max(0, (int)pPlayer->GetData(CPlayer::_PLAYER_LIFE) - (int)pRoleInfo->dwData));
                    }
                    else
                    {
                        pPlayer->SetData(CPlayer::_PLAYER_LIFE, __min((int)pPlayer->GetMaxLife(), (int)pPlayer->GetData(CPlayer::_PLAYER_LIFE) + (int)pRoleInfo->dwData));
                    }
                }
                //pPlayer->SetLife(__max(0, (int)pPlayer->GetLife()-(int)pRoleInfo->dwData));
                if (strcmp(infoMagicType.szTargetSound, "NULL") != 0)
                {
                    CMyPos posWorld;
                    pPlayer->GetWorldPos(posWorld);
                    ::DXPlaySound(infoMagicType.szTargetSound, posWorld.x, posWorld.y, nRange);
                }
            }
        }
        pObj->Die();
        m_pData = NULL;
    }
    return bOk;
}
//----------------------------------------------------------------------------------------
void CMagicEffect::BeginScreenEffect(DWORD dwScreenRepresent)
{
    if (dwScreenRepresent & MAGIC_SHAKENORMAL)
    {
        g_objGameMap.BeginShakeEffect();
    }
    if (dwScreenRepresent & MAGIC_COLORNORMAL)
    {
        g_objGameMap.BeginColorEffect();
    }
    if (dwScreenRepresent & MAGIC_SCALENORMAL)
    {
        g_objGameMap.BeginScaleEffect();
    }
}
//----------------------------------------------------------------------------------------
BOOL CMagicEffect::ProcessBodyTracePos()
{
    MagicTypeInfo infoMagicType;
    if (!CMagic::s_objMagicData.GetMagicTypeInfo(m_dwMagicType, m_dwLevel, infoMagicType))
    {
        return true;
    }
    const int nRange = 3000;
    if (!m_bBegin)
    {
        m_dwBegin = ::TimeGet();
        m_bBegin = true;
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idSender);
        if (!pPlayer && (m_idSender == g_objHero.GetID()))
        {
            pPlayer = &g_objHero;
        }
        if (infoMagicType.dwSenderAction != 0)
            if (!pPlayer->IsDead())
            {
                if (infoMagicType.dwSenderAction != 0)
                {
                    pPlayer->ResetStepDir();
                    CPlayer* pTarget = g_objPlayerSet.GetPlayer(m_idTarget);
                    if (infoMagicType.idNextMagic != ID_NONE)
                    {
                        pPlayer->SetIntoneCmd(infoMagicType.dwSenderAction, pTarget ? pPlayer->GetDir(pTarget) : pPlayer->GetDir(), 0, true);
                    }
                    else
                    {
                        pPlayer->SetIntoneCmd(infoMagicType.dwSenderAction, pTarget ? pPlayer->GetDir(pTarget) : pPlayer->GetDir(), 0, false);
                    }
                }
            }
        return false;
    }
    if (!m_bDelayOk)
    {
        DWORD dwDelay = 200;
        if (infoMagicType.dwSenderAction != 0)
        {
            dwDelay = 600;
        }
        if (::TimeGet() - m_dwBegin < dwDelay)
        {
            return false;
        }
        else
        {
            m_bDelayOk = true;
            m_dwBegin = ::TimeGet();
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idSender);
            if (!pPlayer && (m_idSender == g_objHero.GetID()))
            {
                pPlayer = &g_objHero;
            }
            if (pPlayer)
            {
                if (strcmp(infoMagicType.szSenderEffect, "NULL") != 0)
                {
                    pPlayer->m_objEffect.Add(infoMagicType.szSenderEffect, false, 0, pPlayer->GetLook());
                }
                /*
                if (!pPlayer->IsDead())
                {
                	if (infoMagicType.dwSenderAction != 0)
                	{
                		pPlayer->ResetStepDir();

                		CPlayer* pTarget = g_objPlayerSet.GetPlayer(m_idTarget);

                		if (infoMagicType.idNextMagic != ID_NONE)
                			pPlayer->SetIntoneCmd(infoMagicType.dwSenderAction, pTarget? pPlayer->GetDir(pTarget) : pPlayer->GetDir(), 0, true);
                		else
                			pPlayer->SetIntoneCmd(infoMagicType.dwSenderAction, pTarget? pPlayer->GetDir(pTarget) : pPlayer->GetDir(), 0, false);
                	}
                }*/
                if (strcmp(infoMagicType.szSenderSound, "NULL") != 0)
                {
                    CMyPos posWorld;
                    pPlayer->GetWorldPos(posWorld);
                    ::DXPlaySound(infoMagicType.szSenderSound, posWorld.x, posWorld.y, nRange);
                }
            }
            // 添加3DMapEffect
            CMyPos posWorld, posCell;
            pPlayer->GetPos(posCell);
            g_objGameMap.Cell2World(posCell.x, posCell.y, posWorld.x, posWorld.y);
            m_posBegin = posWorld;
            CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
            if (!pLayer)
            {
                return true;
            }
            C3DMapEffect* pObj = C3DMapEffect::CreateNew(m_posBegin, infoMagicType.szTraceEffect, false);
            if (pObj)
            {
                pLayer->AddMapObj(pObj);
            }
            m_pData = pObj;
            m_bBegin = true;
            this->BeginScreenEffect(infoMagicType.dwScreenRepresent);
            pPlayer = g_objPlayerSet.GetPlayer(m_idTarget, true);
            if (!pPlayer)
            {
                pObj->Die();
                pObj = NULL;
                return true;
            }
            CMyPos posEnd;
            CMy3DPos posMyBegin, posMyEnd;
            posMyBegin.x = m_posBegin.x;
            posMyBegin.y = m_posBegin.y;
            posMyBegin.z = 0;
            pPlayer->GetPos(posCell);
            g_objGameMap.Cell2World(posCell.x, posCell.y, posEnd.x, posEnd.y);
            posEnd.y += 10;
            posMyEnd.x = posEnd.x;
            posMyEnd.y = posEnd.y;
            posMyEnd.z = 0;
            m_posEnd.x = posEnd.x;
            m_posEnd.y = posEnd.y;
            CMyPos posCurrent;
            BOOL bOk = CPhysics::S2DBMGetPos(0, 0, 0, m_posBegin, posEnd, posCurrent);
            CPhysics::S3Get3DDir(posMyBegin, posMyEnd, m_dir3D);
            pObj->SetRotate(m_dir3D.fHorizontal + 1.57f, m_dir3D.fVertical);
            posCurrent.y -= 50;
            pObj->SetWorldPos(posCurrent);
            return false;
        }
    }
    C3DMapEffect* pObj = (C3DMapEffect*)m_pData;
    if (!pObj)
    {
        return false;
    }
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idTarget, true);
    if (!pPlayer)
    {
        pObj->Die();
        pObj = NULL;
        return true;
    }
    // 加速画线到人身上 // 初速度 0 // 加速度 300 象素/秒
    CMyPos posEnd, posCurrent;
    int nV = 1000;
    //	int nV = 10;
    float fA = (float)0.0;
    DWORD dwTime = ::TimeGet() - m_dwBegin;
    float fTime = (float)(dwTime * 1.0f / 1000);
    int nCurrentV =(int)( nV + fTime * fA);
    float fCurrentTime = (float)((::TimeGet() - m_dwBegin) * 1.0f / 1000);
    CMy3DPos posMyBegin, posMyEnd;
    //	CMy3DDir dir3D;
    posEnd.x = m_posEnd.x;
    posEnd.y = m_posEnd.y;
    posMyBegin.x = m_posBegin.x;
    posMyBegin.y = m_posBegin.y;
    posMyBegin.z = 0;
    posMyEnd.x = posEnd.x;
    posMyEnd.y = posEnd.y;
    posMyEnd.z = 0;
    //CPhysics::S3Get3DDir(posMyBegin, posMyEnd, m_dir3D);
    pObj->SetRotate(m_dir3D.fHorizontal + 1.57f, m_dir3D.fVertical);
    BOOL bOk = CPhysics::S2DBMGetPos(nCurrentV, 0, fCurrentTime, m_posBegin, posEnd, posCurrent);
    m_posEnd.x = posEnd.x;
    m_posEnd.y = posEnd.y;
    //m_posBegin = posCurrent;
    posCurrent.y -= 50;
    pObj->SetWorldPos(posCurrent);
    //	g_objGameMap.World2Cell(posCurrent.x, posCurrent.y, posCell.x, posCell.y);
    //	pEffect->SetPos(posCell);
    if (bOk)
    {
        for(int i = 0; i < (int)m_setRoleInfo.size(); i++)
        {
            MagicEffectRoleInfo* pRoleInfo = m_setRoleInfo[i];
            if (!pRoleInfo)
            {
                continue;
            }
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(pRoleInfo->idRole);
            if (!pPlayer && (pRoleInfo->idRole == g_objHero.GetID()))
            {
                pPlayer = &g_objHero;
            }
            if (pPlayer)
            {
                if ((infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SINGLE_HP) ||
                        (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_CROSS_HP) ||
                        (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SECTOR_HP) ||
                        (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_ROUND_HP))
                    if (!pPlayer->TestStatus(USERSTATUS_DIE) && !pPlayer->IsMoving() && infoMagicType.dwSenderAction != 0)
                    {
                        pPlayer->ResetStepDir();
                        pPlayer->SetIntoneCmd(_ACTION_BRUISE0, pPlayer->GetDir());
                    }
                if (strcmp(infoMagicType.szTargetEffect, "NULL") != 0)
                {
                    pPlayer->m_objEffect.Add(infoMagicType.szTargetEffect, false, 0, pPlayer->GetLook());
                }
                DWORD dwColor = _COLOR_YELLOW;
                if (pPlayer->GetID() == g_objHero.GetID())
                {
                    dwColor = _COLOR_RED;
                }
                if (infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_HP)
                {
                    dwColor = 0xff33ff33;
                }
                if (infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SINGLE_STATUS ||
                        infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_STATUS)
                {
                }
                else
                {
                    if (pPlayer->IsGuard())
                    {
                        dwColor = _COLOR_BLUE;
                    }
                    pPlayer->AddZoomNum(pRoleInfo->dwData, dwColor, 100);
                }
                BOOL bAddLife = false;
                if (infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_HP)
                {
                    bAddLife = true;
                }
                if (pPlayer->GetID() != g_objHero.GetID())
                {
                    if (!bAddLife)
                    {
                        pPlayer->SetData(CPlayer::_PLAYER_LIFE, __max(0, (int)pPlayer->GetData(CPlayer::_PLAYER_LIFE) - (int)pRoleInfo->dwData));
                    }
                    else
                    {
                        pPlayer->SetData(CPlayer::_PLAYER_LIFE, __min((int)pPlayer->GetMaxLife(), (int)pPlayer->GetData(CPlayer::_PLAYER_LIFE) + (int)pRoleInfo->dwData));
                    }
                }
                if (strcmp(infoMagicType.szTargetSound, "NULL") != 0)
                {
                    CMyPos posWorld;
                    pPlayer->GetWorldPos(posWorld);
                    ::DXPlaySound(infoMagicType.szTargetSound, posWorld.x, posWorld.y, nRange);
                }
            }
        }
        pObj->Die();
        m_pData = NULL;
    }
    return bOk;
}
//----------------------------------------------------------------------------------------
void CMagicEffect::GetSoulDirectPos(int nIndex, int nDir, int& x, int& y)
{
    int nOptX, nOptY;
    nOptY = 1;
    nOptX = 1;
    switch(nDir)
    {
    case 0:
    case 1:
    case 2:
    case 3:
        nOptX = 1;
        nOptY = 1;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        nOptX = -1;
        nOptY = -1;
        break;
    default:
        break;
    }
    if (nDir == 0 || nDir == 4 )
    {
        switch(nIndex)
        {
        case 0:
            x = -80;
            y = 80;
            break;
        case 1:
            x = -60;
            y = 80;
            break;
        case 2:
            x = -72;
            y = 60;
            break;
        case 3:
            x = -32;
            y = 72;
            break;
        case 4:
            x = -72;
            y = 32;
            break;
        default:
            x = 0;
            y = 80;
            break;
        }
    }
    else if (nDir == 2 || nDir == 6)
    {
        switch(nIndex)
        {
        case 0:
            x = -80;
            y = -80;
            break;
        case 1:
            x = -60;
            y = -80;
            break;
        case 2:
            x = -72;
            y = -60;
            break;
        case 3:
            x = -32;
            y = -72;
            break;
        case 4:
            x = -72;
            y = -32;
            break;
        default:
            x = 0;
            y = -80;
            break;
        }
    }
    else if (nDir == 1 ||  nDir == 5 )
    {
        switch(nIndex)
        {
        case 0:
            x = -80;
            y = 0;
            break;
        case 1:
            x = -60;
            y = 40;
            break;
        case 2:
            x = -60;
            y = -40;
            break;
        case 3:
            x = -75;
            y = 24;
            break;
        case 4:
            x = -75;
            y = -24;
            break;
        default:
            x = -80;
            y = 0;
            break;
        }
    }
    else if (nDir == 3 || nDir == 7)
    {
        switch(nIndex)
        {
        case 0:
            x = 0;
            y = -80;
            break;
        case 1:
            x = 40;
            y = -60;
            break;
        case 2:
            x = -40;
            y = -60;
            break;
        case 3:
            x = 24;
            y = -75;
            break;
        case 4:
            x = -24;
            y = -75;
            break;
        default:
            x = 0;
            y = -80;
            break;
        }
    }
    x = nOptX * x;
    y = nOptY * y;
}
//--------------------------------------------------------------------
