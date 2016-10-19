
//**********************************************************
// 代码编辑器
//**********************************************************

// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include "User.h"
#include "NpcWorld.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUser::CUser()
{
    memset(&m_Info, 0, sizeof(ST_USERINFO));
    SetObjType(OBJ_USER);
    m_pMap = NULL;
    m_lnkThis.Init(Cast<IRole>(this));
}

//////////////////////////////////////////////////////////////////////
CUser::~CUser()
{
    if (m_pMap)
    {
        m_pMap->LeaveMap(this);
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::Create(ST_USERINFO* pInfo, OBJID idMap)
{
    memcpy(&m_Info, pInfo, sizeof(ST_USERINFO));
    m_pMap	= MapManager()->QueryMap(idMap);
    if (!m_pMap)
    {
        return false;
    }
    m_pMap->EnterMap(this);
    m_tActive.SetInterval(PLAYERACTIVE_SECS);
    m_tActive.Update();
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::OnTimer()
{
    if (m_tActive.ToNextTime())
    {
        int	x, y;
        FOR_9_BLOCKS(GetMap(), GetPosX(), GetPosY())
        {
            INpcSet* pNpcSet	= GetMap()->BlockByIndex(x, y).QueryNpcSet();
            for(int i = 0; i < pNpcSet->GetAmount(); i++)
            {
                CNpcPtr pNpc = pNpcSet->GetObjByIndex(i);
                if (pNpc && pNpc->IsAlive())
                {
                    int nDist =  this->Distance(pNpc->GetPosX(), pNpc->GetPosY());
                    if (nDist <= CELLS_PER_VIEW)
                    {
                        return ;	// 找到周围有NPC，不删除自己
                    }
                }
            }
        } // FOR_9_BLOCKS
        // 未找到周围有NPC，删除自己
        UserManager()->QuerySet()->DelObj(GetID());
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::FlyMap(OBJID idMap, int nPosX, int nPosY)
{
    CMapPtr pMap = GetMap();
    CMapPtr pTarget = MapManager()->QueryMap(idMap);
    if (!pMap || !pTarget)
    {
        return ;
    }
    IF_NOT(pTarget->IsValidPoint(nPosX, nPosY))
    return ;
    pMap->LeaveMap(this);
    m_pMap = pTarget;
    SetPos(nPosX, nPosY);
    pTarget->EnterMap(this);
}

//////////////////////////////////////////////////////////////////////
bool CUser::MoveForward(int nDir)
{
    m_Info.ucDir	= nDir % 8;
    int	nDirX	= _DELTA_X[GetDir()];
    int	nDirY	= _DELTA_Y[GetDir()];
    int nNewPosX = GetPosX() + nDirX;
    int nNewPosY = GetPosY() + nDirY;
    IF_NOT(GetMap()->IsValidPoint(nNewPosX, nNewPosY))
    return false;
    SetPos(nNewPosX, nNewPosY);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::JumpPos(int nPosX, int nPosY, int nDir)
{
    IF_NOT(GetMap()->IsValidPoint(nPosX, nPosY))
    return ;
    SetPos(nPosX, nPosY);
    SetDir(nDir);
}

//////////////////////////////////////////////////////////////////////
void CUser::SetSizeAdd(int nSizeAdd)
{
    if (nSizeAdd != m_Info.ucSizeAdd)
    {
        // block npc size change
        GetMap()->LeaveMap(this);
        m_Info.ucSizeAdd	= nSizeAdd;
        GetMap()->EnterMap(this);
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::SetEffect(I64 i64Effect)
{
    if ((i64Effect & KEEPEFFECT_NOT_BLOCKNPC) != (m_Info.i64Effect & KEEPEFFECT_NOT_BLOCKNPC))
    {
        // block npc change
        GetMap()->LeaveMap(this);
        m_Info.i64Effect = i64Effect;
        GetMap()->EnterMap(this);
    }
    else
    {
        m_Info.i64Effect = i64Effect;
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::SetPos(int nPosX, int nPosY)
{
    CMapPtr pMap = GetMap();
    IF_OK(pMap)
    pMap->MoveTo(this, nPosX, nPosY);
    m_Info.usPosX = nPosX;
    m_Info.usPosY = nPosY;
}

//////////////////////////////////////////////////////////////////////
OBJID CUser::GetMapID()									{ return m_pMap->GetID(); }

//////////////////////////////////////////////////////////////////////
int	 CUser::GetAttackRange(int nSizeAdd)
{
    int nItemType = GetWeaponRType();
    if (nItemType)
    {
        CItemTypeData* pType = NpcWorld()->GetItemType()->QueryItemType(nItemType);
        if (pType)
        {
            return pType->GetInt(ITEMTYPEDATA_ATKRANGE) + (GetSizeAdd() + nSizeAdd + 1) / 2;
        }
    }
    return 1 + (GetSizeAdd() + nSizeAdd + 1) / 2;
}



