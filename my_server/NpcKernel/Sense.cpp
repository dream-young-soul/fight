
//**********************************************************
// 代码编辑器
//**********************************************************

// Sense.cpp: implementation of the CSense class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "define.h"
#include "Array.h"
#include "VarType.h"
#include "I_Aicenter.h"
#include "I_role.h"
#include "Sense.h"
#include "AGent.h"
#include "AGentData.h"
#include "NpcWorld.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSense::CSense()
{
}

CSense::~CSense()
{
}

bool CSense::Create(CAgent* pOwner)
{
    m_pOwner		= pOwner;
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSense::CheckCondition(int idxFactFunction, VarTypeSetRef setParam, ARGUMENT_SET* psetArgument)
{
    CHECKF(idxFactFunction >= 0);
    switch(idxFactFunction)
    {
    case	FACTFUNC_IS_ALIVE:					// "IsAlive":					// (id)，检测角色是否活着
        {
            OBJID	idRole	= (int)IAiCenter::GetValue(setParam[0], psetArgument);
            IRole* pRole	= UserManager()->QueryRole(idRole);
            if (pRole && pRole->IsAlive())
            {
                return true;
            }
        }
        break;
    case	FACTFUNC_IS_WING:					// "IsWing":					// (id)，检查角色是否在飞行中
        {
            OBJID	idRole	= (int)IAiCenter::GetValue(setParam[0], psetArgument);
            IRole* pRole	= UserManager()->QueryRole(idRole);
            if (pRole && pRole->IsWing())
            {
                return true;
            }
        }
        break;
    case	FACTFUNC_IS_USER:					// "IsUser",					// (id)
        {
            OBJID	idRole	= (int)IAiCenter::GetValue(setParam[0], psetArgument);
            IRole* pRole	= UserManager()->QueryRole(idRole);
            if (pRole && Cast<CUser>(pRole))
            {
                return true;
            }
        }
        break;
    case	FACTFUNC_IS_MONSTER:					// "IsMonster",				// (id)
        {
            OBJID	idRole	= (int)IAiCenter::GetValue(setParam[0], psetArgument);
            IRole* pRole	= UserManager()->QueryRole(idRole);
            CNpc* pNpc;
            if (pRole && Cast(&pNpc, pRole) && pNpc->IsMonster())
            {
                return true;
            }
        }
        break;
    case	FACTFUNC_EVIL_VALUE:				// "EvilValue":				// (id, int)，取角色的邪恶值，怪物和蓝名、黑名为100，红名为50，卫兵和普通玩家为0
        {
            OBJID	idRole	= (int)IAiCenter::GetValue(setParam[0], psetArgument);
            IRole* pRole	= UserManager()->QueryRole(idRole);
            if (pRole)
            {
                VarTypeSet	setFactParam;
                setFactParam.Push(idRole);
                if (pRole->IsEvil())
                {
                    setFactParam.Push(100);
                }
                else if (pRole->IsRighteous())
                {
                    setFactParam.Push(0);
                }
                else
                {
                    setFactParam.Push(50);
                }
                return IAiCenter::MatchParam(setFactParam, setParam, psetArgument);
            }
            return false;
        }
        break;
    case	FACTFUNC_COOL_VALUE:				// "CoolValue":				// (id, int)，100 ~ 0，有右手极品武器为50
        {
            OBJID	idRole	= (int)IAiCenter::GetValue(setParam[0], psetArgument);
            IRole* pRole	= UserManager()->QueryRole(idRole);
            if (pRole)
            {
                CUser* pUser = Cast<CUser>(pRole);
                if (pUser)
                {
                    VarTypeSet	setFactParam;
                    setFactParam.Push(idRole);
                    int nCool = 0;
                    if (CItem::IsWeapon2(pUser->GetWeaponRType()))
                    {
                        if (CItem::IsNonsuch(pUser->GetWeaponRType()))
                        {
                            nCool += 70;
                        }
                        else if (CItem::IsCool8(pUser->GetWeaponRType()))
                        {
                            nCool += 35;
                        }
                    }
                    else
                    {
                        if (CItem::IsNonsuch(pUser->GetWeaponRType()))
                        {
                            nCool += 50;
                        }
                        else if (CItem::IsCool8(pUser->GetWeaponRType()))
                        {
                            nCool += 25;
                        }
                    }
                    if (CItem::IsNonsuch(pUser->GetWeaponLType()))
                    {
                        nCool += 20;
                    }
                    else if (CItem::IsCool8(pUser->GetWeaponLType()))
                    {
                        nCool += 10;
                    }
                    if (CItem::IsNonsuch(pUser->GetArmorType()))
                    {
                        nCool += 20;
                    }
                    else if (CItem::IsCool8(pUser->GetArmorType()))
                    {
                        nCool += 10;
                    }
                    if (CItem::IsNonsuch(pUser->GetHelmetType()))
                    {
                        nCool += 10;
                    }
                    else if (CItem::IsCool8(pUser->GetHelmetType()))
                    {
                        nCool += 5;
                    }
                    setFactParam.Push(nCool);
                    return IAiCenter::MatchParam(setFactParam, setParam, psetArgument);
                }
            }
            return false;
        }
        break;
    case	FACTFUNC_LEVEL:						// "Level":					// (id, int)，取角色等级(身上装备的最高等级)
        {
            OBJID	idRole	= (int)IAiCenter::GetValue(setParam[0], psetArgument);
            IRole* pRole	= UserManager()->QueryRole(idRole);
            if (pRole)
            {
                CUser* pUser = Cast<CUser>(pRole);
                if (pUser)
                {
                    VarTypeSet	setFactParam;
                    setFactParam.Push(idRole);
                    int	nLevel = 0;
                    CItemTypeData* pType = NpcWorld()->GetItemType()->QueryItemType(pUser->GetWeaponRType());
                    int nLev = pType->GetInt(ITEMTYPEDATA_REQ_LEVEL);
                    if (nLevel < nLev)
                    {
                        nLevel = nLev;
                    }
                    pType = NpcWorld()->GetItemType()->QueryItemType(pUser->GetWeaponLType());
                    nLev = pType->GetInt(ITEMTYPEDATA_REQ_LEVEL);
                    if (nLevel < nLev)
                    {
                        nLevel = nLev;
                    }
                    pType = NpcWorld()->GetItemType()->QueryItemType(pUser->GetArmorType());
                    nLev = pType->GetInt(ITEMTYPEDATA_REQ_LEVEL);
                    if (nLevel < nLev)
                    {
                        nLevel = nLev;
                    }
                    pType = NpcWorld()->GetItemType()->QueryItemType(pUser->GetHelmetType());
                    nLev = pType->GetInt(ITEMTYPEDATA_REQ_LEVEL);
                    if (nLevel < nLev)
                    {
                        nLevel = nLev;
                    }
                    setFactParam.Push(nLevel);
                    return IAiCenter::MatchParam(setFactParam, setParam, psetArgument);
                }
            }
            return false;
        }
        break;
    case	FACTFUNC_MY_LIFE:					// "MyLife":					// (int)
        {
            int nLife = m_pOwner->GetLife();
            VarTypeSet	setFactParam;
            setFactParam.Push(nLife);
            return IAiCenter::MatchParam(setFactParam, setParam, psetArgument);
        }
        break;
    case	FACTFUNC_ROLE_POS:					// "RolePos",					// (id,x,y)，取角色的坐标
        {
            OBJID	idRole	= (int)IAiCenter::GetValue(setParam[0], psetArgument);
            IRole* pRole	= UserManager()->QueryRole(idRole);
            if (pRole && pRole->GetMapID() == m_pOwner->GetMapID())
            {
                VarTypeSet	setFactParam;
                setFactParam.Push(idRole);
                setFactParam.Push(pRole->GetPosX());
                setFactParam.Push(pRole->GetPosY());
                return IAiCenter::MatchParam(setFactParam, setParam, psetArgument);
            }
        }
        break;
    case	FACTFUNC_MYDISTANCE:				// "MyDistance",				// (x,y,int)，离目标点的距离
        {
            int 	nPosX	= (int)IAiCenter::GetValue(setParam[0], psetArgument);
            int 	nPosY	= (int)IAiCenter::GetValue(setParam[1], psetArgument);
            //if (m_pOwner->GetMapID() == SHOP_MAP_ID)
            {
                VarTypeSet	setFactParam;
                setFactParam.Push(nPosX);
                setFactParam.Push(nPosY);
                setFactParam.Push(m_pOwner->GetDistance(nPosX, nPosY));
                return IAiCenter::MatchParam(setFactParam, setParam, psetArgument);
            }
        }
        break;
    case	FACTFUNC_ATTACK_DISTNACE:			// "AttackDistance",			// (id,int)，能攻击对方的距离
        {
            OBJID 	idRole	= (int)IAiCenter::GetValue(setParam[0], psetArgument);
            IRole* pRole	= UserManager()->QueryRole(idRole);
            if (pRole && pRole->GetMapID() == m_pOwner->GetMapID())
            {
                VarTypeSet	setFactParam;
                setFactParam.Push(idRole);
                setFactParam.Push(m_pOwner->GetAttackRange(pRole->GetSizeAdd()));
                return IAiCenter::MatchParam(setFactParam, setParam, psetArgument);
            }
        }
        break;
    case	FACTFUNC_DANGER_VALUE:				// "DangerValue":				// (int)，+∞ ~ 0，通常80算危险，单个敌人为50，无敌人为0
        {
            int	nDanger = 0;
            CNpcBigSet::Iterator pNpc = NpcManager()->QuerySet()->NewEnum();
            while(pNpc.Next())
            {
                if (pNpc && pNpc->IsEvil())
                {
                    if (m_pOwner->IsCloseTarget(Cast<IRole, INpc>(pNpc)))
                    {
                        nDanger = ::CutTrail(50, nDanger + 20);
                    }
                    else if (m_pOwner->IsLookTarget(Cast<IRole, INpc>(pNpc)))
                    {
                        nDanger += 5;
                    }
                }
            }
            CUserSet::Iterator pUser = UserManager()->QuerySet()->NewEnum();
            while(pUser.Next())
            {
                if (pUser && pUser->IsEvil())
                {
                    if (m_pOwner->IsCloseTarget(Cast<IRole, CUser>(pUser)))
                    {
                        nDanger = ::CutTrail(50, nDanger + 20);
                    }
                    else if (m_pOwner->IsLookTarget(Cast<IRole, CUser>(pUser)))
                    {
                        nDanger += 5;
                    }
                }
            }
            VarTypeSet	setFactParam;
            setFactParam.Push(nDanger);
            return IAiCenter::MatchParam(setFactParam, setParam, psetArgument);
        }
        break;
    case	FACTFUNC_ITEM_COUNT:				// "ItemCount"					// (int)，40 ~ 0，背包中物品数量(药品数量)
        {
            CItemPack* pPack = Cast<CItemPack>(m_pOwner);
            IF_OK(pPack)
            {
                VarTypeSet	setFactParam;
                setFactParam.Push(pPack->QueryItemSet()->GetAmount());
                return IAiCenter::MatchParam(setFactParam, setParam, psetArgument);
            }
        }
        break;
    case	FACTFUNC_MY_LEADER:					// "MyLeader":					// (id)，队长ID
        {
            CTeam* pTeam = Cast<CTeam>(m_pOwner);
            if (pTeam && pTeam->IsValid())
            {
                VarTypeSet	setFactParam;
                setFactParam.Push(pTeam->GetLeaderID());
                return IAiCenter::MatchParam(setFactParam, setParam, psetArgument);
            }
        }
        break;
    case	FACTFUNC_IS_LEADER:					// "IsLeader":					// (id)，检查是否队长
        {
            OBJID	idRole	= (int)IAiCenter::GetValue(setParam[0], psetArgument);
            IRole* pRole	= UserManager()->QueryRole(idRole);
            CUser* pUser;
            if (pRole && Cast(&pUser, pRole) && pUser->IsTeamLeader())
            {
                return true;
            }
        }
        break;
    default:
        {
            ASSERT(!"switch(idxFactFunction)");
        }
    }
    return false;
}

void CSense::Die()
{
}

void CSense::BeAttack(IRole* pRole)
{
    //	Cast<CRoleFight>(m_pOwner)->BeAttack(pRole);
}

void CSense::BeKill(IRole* pRole)
{
    //	Cast<CRoleFight>(m_pOwner)->BeKill(pRole);
    /*	int		nPriority	= 0;
    	String	strFact = FACT_MODE;		// "Mode"					// (e,e)			// 0
    	strFact	+= "(1,1)";

    	IAiCenter* pAi = Cast<IAiCenter>(m_pOwner);
    */
}

void CSense::Recruit(int nLife)
{
}

void CSense::Reborn()
{
    /*	int		nPriority	= 0;
    	String	strFact = FACT_MODE;		// "Mode"					// (e,e)			// 0
    	if (Cast<CTeam>(m_pOwner)->IsValid())
    		strFact	+= "(2,2)";
    	else
    		strFact	+= "(2,1)";

    	IAiCenter* pAi = Cast<IAiCenter>(m_pOwner);
    */
}

void CSense::TeamApply(OBJID idUser)
{
    char	szNum[20];
    _ltoa(idUser, szNum, 10);
    int		nPriority	= 50;
    String	strFact = FACT_TEAM_INVITE;		// "TeamInvite"			// (id)				// 50
    strFact += '(';
    strFact	+= szNum;
    strFact += ')';
    IAiCenter* pAi = Cast<IAiCenter>(m_pOwner);
    pAi->AddFact(strFact, nPriority);
}

void CSense::OnTimer()
{
    int		nPriority	= 80;
    if (m_tLook.ToNextTime(::Priority2Durable(80)))
    {
        vector<IRole*>	setRole;
        POINT	pos;
        pos.x	= m_pOwner->GetPosX();
        pos.y	= m_pOwner->GetPosY();
        m_pOwner->GetMap()->CollectRole(&setRole, pos, m_pOwner->GetLookRange());
        for(int i = 0; i < setRole.size(); i++)
        {
            IRole* pRole = setRole[i];
            if (pRole && pRole->GetID() != m_pOwner->GetID())
            {
                char	szNum[20];
                _ltoa(pRole->GetID(), szNum, 10);
                String	strFact = FACT_LOOK_TARGET;		// "LookTarget"			// <id>				// 80
                strFact	+= '<';
                strFact	+= szNum;
                strFact	+= '>';
                IAiCenter* pAi = Cast<IAiCenter>(m_pOwner);
                pAi->AddFact(strFact, nPriority);
            }
        }
    }
}
