
//**********************************************************
// 代码编辑器
//**********************************************************

#pragma warning(disable:4786)
#include "basefunc.H"
#include "m_map.H"
#include "ani.H"
#include "3dgamemap.H"
#include "role.H"
#include "3droledata.H"
#include "Item.h"

//--------------------------------------------------------------
int CRole::GetDir(CMyPos posTarget) const
{
    int nDir	= CGameMap::GetDirection(m_Info.posMap.x, m_Info.posMap.y, posTarget.x, posTarget.y);
    return (nDir + 1) % 8;
}

//--------------------------------------------------------------
int CRole::GetDir(CRole* pTarget) const
{
    if (!pTarget)
    {
        return this->GetDir();
    }
    CMyPos posTarget;
    pTarget->GetPos(posTarget);
    return this->GetDir(posTarget);
}

//--------------------------------------------------------------
DWORD CRole::GetDistance(CRole* pTarget)
{
    CMyPos posTarget;
    pTarget->GetPos(posTarget);
    return __max(abs(m_Info.posMap.x - posTarget.x), abs(m_Info.posMap.y - posTarget.y));
}

//--------------------------------------------------------------
BOOL CRole::IsMapAttainable(int iPosX, int iPosY)
{
    CellInfo* pCellInfo = g_objGameMap.GetCell(iPosX, iPosY);
    if (!pCellInfo)
    {
        return false;
    }
    if (g_objGameMap.GetGroundMask(pCellInfo) == 1)
    {
        return false;
    }
    return true;
}

//--------------------------------------------------------------
BOOL CRole::IsMapReached(void)
{
    if (m_Info.posMap.x == m_Info.cmdProc.posTarget.x &&
            m_Info.posMap.y == m_Info.cmdProc.posTarget.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//--------------------------------------------------------------
BOOL CRole::SetPos(int iPosX, int iPosY)
{
    //	BOOL bSucSet	=g_objGameMap.SetObj(iPosX,iPosY,m_Info.id);
    //	if (bSucSet)
    {
        m_Info.posMap.x	= iPosX;
        m_Info.posMap.y	= iPosY;
        g_objGameMap.Cell2World(m_Info.posMap.x, m_Info.posMap.y, m_Info.posWorld.x, m_Info.posWorld.y);
        g_objGameMap.World2Screen(m_Info.posWorld.x, m_Info.posWorld.y, m_Info.posScr.x, m_Info.posScr.y);
        m_Info.posActionBegin = m_Info.posActionEnd = m_Info.posWorld;
    }
    return true;
}

//--------------------------------------------------------------
void CRole::ShowName(void)
{
    CMySize sizeFont;
    sizeFont.iHeight	= CMyBitmap::GetFontSize();
    sizeFont.iWidth		= sizeFont.iHeight / 2;
    const char* pszString	= this->GetName();
    int iInfoLen	= strlen(pszString);
    if (iInfoLen <= 0)
    {
        return;
    }
    int nShowX	= m_Info.posScr.x - sizeFont.iWidth * iInfoLen / 2;
    int nShowY	= m_Info.posScr.y - OFFSET_NAMESHOW - sizeFont.iHeight;
    {
        CMyBitmap::ShowString(nShowX + 1, nShowY + 1 - this->GetOffsetY(), 0x0, pszString);
        CMyBitmap::ShowString(nShowX, nShowY - this->GetOffsetY(), _COLOR_WHITE, pszString);
    }
}

//--------------------------------------------------------------
void CRole::Say(const char* pszWords)
{
    if (!pszWords)
    {
        return;
    }
    m_objSayHello.SetWords(pszWords);
}

//--------------------------------------------------------------
BOOL CRole::IsMouseFocus(int evtMouse)
{
    if (this->TestStatus(USERSTATUS_INVISIBLE))
    {
        return false;
    }
    if (!m_pIRoleView)
    {
        return false;
    }
    int nMouseX, nMouseY;
    int evtNow	=::MouseCheck(nMouseX, nMouseY);
    if (_MOUSE_NONE != evtMouse && evtMouse != evtNow)
    {
        return false;
    }
    int nBgViewportX, nBgViewportY;
    g_objGameMap.GetBgViewport(nBgViewportX, nBgViewportY);
    g_objGameMap.MapScaleMousePos(nMouseX, nMouseY);
    if (this->GetRoleType() == _ROLE_BOOTH_NPC)
    {
        CMyPos posCell;
        this->GetPos(posCell);
        nBgViewportX = posCell.x;
        nBgViewportY = posCell.y;
        CMyPos posMyCell;
        g_objGameMap.Screen2Cell(nMouseX, nMouseY, posMyCell.x, posMyCell.y);
        nMouseX	= posMyCell.x;
        nMouseY = posMyCell.y;
    }
    if (!m_pIRoleView->HitTest(nMouseX, nMouseY + m_Info.nFlyOffset, nBgViewportX, nBgViewportY))
    {
        return false;
    }
    return true;
}

//-------------------------------------------------------------
BOOL CRole::IsVisible(void)
{
    CMyPos posCell;
    this->GetPos(posCell);
    return g_objGameMap.IsPosVisible(posCell, 2);
}

//-------------------------------------------------------------
BOOL CRole::IsWalking(void)
{
    BOOL bReal = false;
    switch (this->GetActionType())
    {
    case _ACTION_WALKL:
    case _ACTION_WALKR:
    case _ACTION_WALKL_I:
    case _ACTION_WALKR_I:
        bReal = true;
        break;
    default:
        break;
    }
    return bReal;
}

//-------------------------------------------------------------
BOOL CRole::IsRunning(void)
{
    BOOL bReal = false;
    switch (this->GetActionType())
    {
    case _ACTION_RUNL:
    case _ACTION_RUNR:
    case _ACTION_RUNL_I:
    case _ACTION_RUNR_I:
        bReal = true;
        break;
    default:
        break;
    }
    return bReal;
}

//-------------------------------------------------------------
BOOL CRole::IsMoving(void)
{
    BOOL bReal = false;
    if (m_Info.posWorld.x != m_Info.posActionEnd.x || m_Info.posWorld.y != m_Info.posActionEnd.y)
    {
        bReal = true;
    }
    switch (this->GetActionType())
    {
    case _ACTION_WALKL:
    case _ACTION_WALKR:
    case _ACTION_WALKL_I:
    case _ACTION_WALKR_I:
    case _ACTION_RUNL:
    case _ACTION_RUNL_I:
    case _ACTION_RUNR:
    case _ACTION_RUNR_I:
    case _ACTION_JUMP:
    case _ACTION_JUMP_BACK:
    case _ACTION_JUMP_RUN:
    case _ACTION_JUMP_ATK:
    case _ACTION_BUMP:
    case _ACTION_POPOFF:
        bReal = true;
        break;
    default:
        break;
    }
    return bReal;
}

//--------------------------------------------------------------
BOOL CRole::IsStandBy(void)
{
    BOOL bStandBy = false;
    // test cmd
    if (_COMMAND_EMOTION == m_Info.cmdProc.iType
            || _COMMAND_STANDBY == m_Info.cmdProc.iType)
    {
        bStandBy = true;
    }
    // test action
    switch (this->GetActionType())
    {
    case _ACTION_STANDBY:
    case _ACTION_REST1:
    case _ACTION_REST2:
    case _ACTION_REST3:
    case _ACTION_STANDBY_I:
    case _ACTION_ALERT:
    case _ACTION_ALERT_I:
        bStandBy = true;
        break;
    default:
        break;
    }
    return bStandBy;
}

//--------------------------------------------------------------
BOOL CRole::IsBumpBackEnable(void)
{
    BOOL bBackEnable = false;
    switch(this->GetRoleType())
    {
    case _ROLE_PLAYER:
    case _ROLE_HERO:
    case _ROLE_MONSTER:
        bBackEnable = true;
        break;
    }
    return bBackEnable;
}

//--------------------------------------------------------------
void CRole::GetPrePos(int& nPosX, int& nPosY)
{
    nPosX	= m_Info.posMap.x - _DELTA_X[m_Info.iDirection];
    nPosY	= m_Info.posMap.y - _DELTA_Y[m_Info.iDirection];
}

//--------------------------------------------------------------
void CRole::ClrStatus(__int64 dwStatus)
{
    m_dwStatus &=  ~dwStatus;
}

//--------------------------------------------------------------
void CRole::SetStatus(__int64 dwStatus)
{
    m_dwStatus |= dwStatus;
}

//--------------------------------------------------------------
BOOL CRole::TestStatus(__int64 dwStatus)
{
    return ((m_dwStatus & dwStatus) == dwStatus);
}

//--------------------------------------------------------------
void CRole::SetName	(const char* pszName)
{
    if (!pszName || strlen(pszName) >= _MAX_NAMESIZE)
    {
        return;
    }
    strcpy(m_Info.szName, pszName);
}


//--------------------------------------------------------------
int CRole::TestAniEffect(const char* pszEffect)
{
    int nAmount = m_dequeAniEffect.size();
    if (nAmount == 0)
    {
        return 0;
    }
    for(int i = 0; i < nAmount; i ++)
    {
        CAniEffect* pAniEffect = m_dequeAniEffect[i];
        if (pAniEffect && (0 == strcmp(pszEffect, pAniEffect->GetIndex())))
        {
            return pAniEffect->GetLoopTime();
        }
    }
    return 0;
}
//--------------------------------------------------------------
BOOL CRole::AddAniEffectEx  (const char* pszEffectTitle,
                             int nTimes,
                             DWORD dwInterval,
                             int nShowWay,
                             DWORD dwLoopInterval,
                             int nOffsetX,
                             int nOffsetY,
                             int nExigence,
                             DWORD dwDelay)
{
    if (!pszEffectTitle || nTimes == 0)
    {
        return false;
    }
    CAniEffect* pAniEffect	= new CAniEffect;
    if (!pAniEffect)
    {
        return false;
    }
    if (!pAniEffect->CreateEx(pszEffectTitle, nTimes, dwInterval, nShowWay, dwLoopInterval, nOffsetX, nOffsetY, nExigence, dwDelay))
    {
        SAFE_DELETE(pAniEffect);
        return false;
    }
    m_dequeAniEffect.push_back(pAniEffect);
    return true;
}

//--------------------------------------------------------------
BOOL CRole::AddAniEffect(const char* pszEffect)
{
    CAniEffect* pAniEffect	= new CAniEffect;
    if (!pAniEffect)
    {
        return false;
    }
    if (!pAniEffect->Create(pszEffect))
    {
        SAFE_DELETE(pAniEffect);
        return false;
    }
    m_dequeAniEffect.push_back(pAniEffect);
    return true;
}

//--------------------------------------------------------------
void CRole::ProcessAniEffect()
{
    // 2d effect
    int nEffectCount	= m_dequeAniEffect.size();
    for (int i = nEffectCount - 1; i >= 0 ; i--)
    {
        CAniEffect* pAniEffect	= m_dequeAniEffect[i];
        if (pAniEffect)
        {
            if (!pAniEffect->Process()) // ani showing is over
            {
                m_dequeAniEffect.erase(m_dequeAniEffect.begin() + i);
                SAFE_DELETE(pAniEffect);
            }
        }
    }
    // 3d effect
    m_objEffect.Process();
}

//--------------------------------------------------------------
void CRole::ShowAniEffect(int nPosX, int nPosY)
{
    int nEffectCount	= m_dequeAniEffect.size();
    for (int i = nEffectCount - 1; i >= 0 ; i--)
    {
        CAniEffect* pAniEffect	= m_dequeAniEffect[i];
        if (pAniEffect)
        {
            pAniEffect->Show(nPosX, nPosY);
        }
    }
}

//--------------------------------------------------------------
void CRole::ClrAniEffect(const char* pszEffect)
{
    if (!pszEffect)
    {
        int nEffectCount	= m_dequeAniEffect.size();
        for (int i = 0; i < nEffectCount; i++)
        {
            CAniEffect* pAniEffect	= m_dequeAniEffect[i];
            if (pAniEffect)
            {
                SAFE_DELETE(pAniEffect);
            }
        }
        m_dequeAniEffect.clear();
    }
    else
    {
        int nEffectCount	= m_dequeAniEffect.size();
        for (int i = 0; i < nEffectCount; i++)
        {
            CAniEffect* pAniEffect	= m_dequeAniEffect[i];
            if (pAniEffect)
            {
                if (0 == strcmp(pszEffect, pAniEffect->GetIndex()))
                {
                    pAniEffect->Init();
                }
            }
        }
    }
}

//--------------------------------------------------------------
void CRole::SetRGBA(float alpha, float red, float green, float blue)
{
    if (m_pIRoleView)
    {
        m_pIRoleView->SetRGBA(alpha, red, green, blue);
    }
}

//--------------------------------------------------------------
void CRole::SetArmor	(OBJID idType)
{
    m_Info.idArmorType = idType;
    if (m_pIRoleView)
    {
        m_pIRoleView->SetVariable(_VAR_ARMOR, idType);
    }
}

//--------------------------------------------------------------
void CRole::SetMantle	(OBJID idType)
{
    m_Info.idMantleType = idType;
    if (m_pIRoleView)
    {
        m_pIRoleView->SetVariable(_VAR_MANTLE, idType);
    }
}
//--------------------------------------------------------------
void CRole::SetArmet	(OBJID idType)
{
    m_Info.idArmetType = idType;
    if (m_pIRoleView)
    {
        m_pIRoleView->SetVariable(_VAR_ARMET, idType);
    }
}

//--------------------------------------------------------------
void CRole::SetRWeapon	(OBJID idType)
{
    m_Info.idRWeaponType = idType;
    if (m_pIRoleView)
    {
        m_pIRoleView->SetVariable(_VAR_RWEAPON, idType);
    }
}

//--------------------------------------------------------------
void CRole::SetLWeapon	(OBJID idType)
{
    m_Info.idLWeaponType = idType;
    if (idType == _ARROW || idType == _DART)
    {
        return;
    }
    if (m_pIRoleView)
    {
        m_pIRoleView->SetVariable(_VAR_LWEAPON, idType);
    }
}
//--------------------------------------------------------------
void CRole::SetMount	(OBJID idType)
{
    m_Info.idMountType = idType;
    if (m_pIRoleView)
    {
        m_pIRoleView->SetVariable(_VAR_MOUNT, idType);
    }
}

//--------------------------------------------------------------
BOOL CRole::IsAttacking(void)
{
    BOOL bTrue = false;
    switch (this->GetActionType())
    {
    case _ACTION_ATTACK0:
    case _ACTION_ATTACK1:
    case _ACTION_ATTACK2:
        bTrue = true;
        break;
    default:
        break;
    }
    return bTrue;
}
//--------------------------------------------------------------
BOOL CRole::IsIdle (void)
{
    BOOL bReal = FALSE;
    switch(this->GetCommandType())
    {
    case _COMMAND_POSE:
    case _COMMAND_STANDBY:
    case _COMMAND_EMOTION:
    case _COMMAND_ACTION:
    case _COMMAND_MINE:
    case _COMMAND_WOUND:
    case _COMMAND_DEFEND:
        //case _COMMAND_LOCKBOOTH:
        bReal = TRUE;
        break;
    default:
        break;
    }
    return bReal;
}

//--------------------------------------------------------------
int CRole::GetIDType(OBJID id)
{
    if ((id >= MONSTERID_FIRST) && (id <= MONSTERID_LAST))
    {
        return _PLAYER_ID_MONSTER;
    }
    if ((id >= PETID_FIRST) && (id <= PETID_LAST))
    {
        return _PLAYER_ID_PET;
    }
    if ((id >= SYSNPCID_FIRST) && (id <= SYSNPCID_LAST))
    {
        return _PLAYER_ID_SYSNPC;
    }
    if ((id >= DYNANPCID_FIRST) && (id <= DYNANPCID_LAST))
    {
        return _PLAYER_ID_DYNNPC;
    }
    if ((id >= CALLPETID_FIRST) && (id <= CALLPETID_LAST))
    {
        return _PLAYER_ID_CALLPET;
    }
    if (id >= PLAYER_ID_FIRST)
    {
        return _PLAYER_ID_PLAYER;
    }
    return 0;
}
//--------------------------------------------------------------
DWORD CRole::GetActionDelayInfoKey	(CRole* pTarget)
{
    if (!pTarget)
    {
        return 0;
    }
    // 武器部分的数据
    DWORD dwWeaponData = 0;
    if (m_pIRoleView)
    {
        dwWeaponData = m_pIRoleView->GetVariable(_VAR_WEAPON_ACTION_DATA);
    }
    // 动作数据
    DWORD dwActionData = this->GetActionType() % 1000;
    // 外形数据
    DWORD dwLookData = this->GetLook() % 10;
    // 距离数据
    //	DWORD dwDistance = this->GetDistance(pTarget) % 10;
    // 相对方向数据
    //	DWORD dwRelativeDir = (this->GetDir(pTarget) + 8 - this->GetDir()) % 8;
    // MAKE KEY
    DWORD dwKey = dwLookData	* 1000000	+
                  dwWeaponData	* 1000		+
                  dwActionData;
    return dwKey;
}
//--------------------------------------------------------------
DWORD CRole::GetWeaponInfoKey (CRole* pTarget)
{
    DWORD dwKey = 0;
    if (m_pIRoleView)
    {
        dwKey = m_pIRoleView->GetVariable(_VAR_WEAPON_ACTION_DATA);
    }
    if (pTarget && pTarget->IsMonster())
    {
        CONST CMonsterInfo* pInfo = g_obj3DRoleData.GetMonsterInfo(pTarget->GetMonsterTypeId());
        if (pInfo)
        {
            dwKey += pInfo->idType * 1000;
        }
    }
    return dwKey;
}
