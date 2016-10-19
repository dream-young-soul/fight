
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// Pet.cpp: implementation of the CPet class.
//
//////////////////////////////////////////////////////////////////////

#include "Pet.h"
#include "Hero.h"
#include "AllMsg.h"
#include "3DGameMap.h"
#include "GamePlayerSet.h"
#include "GameMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPet::CPet()
{
    m_nBehaviorType	=	_PET_BEHAVIOR_READY;
    m_idTarget		=	ID_NONE;
    m_idHeroTarget	=	ID_NONE;
    m_idItem		=   ID_NONE;
}

CPet::~CPet()
{
    DestroyMagicSet();
}

//--------------------------------------------------------------------------------------
CPet*	CPet::CreateNewPet	(OBJID idPet, int nType, OBJID idItem)
{
    CPet* pPet = new CPet;
    MYASSERT(pPet);
    pPet->SetID(idPet);
    pPet->SetAIType(nType);
    pPet->SetItemID (idItem);
    pPet->DestroyMagicSet();
    return pPet;
}
//--------------------------------------------------------------------------------------
int		CPet::GetAtkRange	(void)
{
    return m_AIInfo.nAttackRange;
}
//--------------------------------------------------------------------------------------

void	CPet::SetAIType(int nType)
{
    PetInfo* pInfo = g_obj3DRoleData.GetPetInfo(nType);
    MYASSERT(pInfo);
    m_AIInfo.setLocateTargetMethod.clear();
    m_AIInfo.nEnvMethod = pInfo->nEnvMethod;
    m_AIInfo.nScale = pInfo->nScale;
    m_AIInfo.nAttackRange = pInfo->nAttackRange;
    m_Info.iAddSize = pInfo->nAddSize;
    m_AIInfo.nAddSize = pInfo->nAddSize;
    m_AIInfo.dwAttackInterval = pInfo->dwAttackInterval;
    int nAmount = pInfo->setLocateTargetMethod.size();
    for(int i = 0; i < nAmount; i  ++)
    {
        int nMethod = pInfo->setLocateTargetMethod[i];
        m_AIInfo.setLocateTargetMethod.push_back(nMethod);
    }
}
//--------------------------------------------------------------------------------------
BOOL	CPet::CheckEnemy(CPlayer* pPlayer, int nRange, BOOL bHero)
{
    //const int _PET_SEARCH_ENEMY_RANGE = 12;
    if(!pPlayer)
    {
        return false;
    }
    if(pPlayer->IsMyPet())
    {
        return false;
    }
    if(pPlayer->IsDead())
    {
        return false;
    }
    CMyPos posHero, posTarget;
    if (bHero)
    {
        g_objHero.GetPos(posHero);
    }
    else
    {
        this->GetPos (posHero);
    }
    pPlayer->GetPos(posTarget);
    if(abs(posHero.x - posTarget.x) > nRange ||
            abs(posHero.y - posTarget.y) > nRange)
    {
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------------

void	CPet::SearchStatusAsistant()
{
    // 目标永远是Hero ...
    m_idTarget = g_objHero.GetID();
}
//--------------------------------------------------------------------------------------
void	CPet::SearchAttackAsistant()
{
    // 以Hero的目标为目标
    OBJID id = m_idHeroTarget;
    if(id == ID_NONE)
    {
        return;
    }
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(id);
    if(!pPlayer)
    {
        return;
    }
    if(this->CheckEnemy(pPlayer))
    {
        m_idTarget = id;
    }
}
//--------------------------------------------------------------------------------------
void	CPet::SearchEnemyRevenge()
{
    // 以攻击Hero者为目标
    OBJID id = g_objHero.GetIDWhoAttackMe();
    if(id == ID_NONE)
    {
        return;
    }
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(id);
    if(!pPlayer)
    {
        return;
    }
    if(pPlayer->IsDead())
    {
        return;
    }
    if(this->CheckEnemy(pPlayer))
    {
        m_idTarget = id;
        m_bRevengeID = true;
    }
}
//--------------------------------------------------------------------------------------
void	CPet::SearchEnemyGuard()
{
    // 搜索离Hero最近的Monster
    DWORD dwPlayerAmount = g_objPlayerSet.GetPlayerAmount();
    CPlayer* pTarget = NULL;
    DWORD dwDistance = 100;
    for(int i = 0; i < dwPlayerAmount; i ++)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayerByIndex(i);
        if(pPlayer && pPlayer->IsMonster() && !pPlayer->IsGuard() && !pPlayer->IsDead()
                && !pPlayer->IsMyPet())
        {
            DWORD dwMyDistance = g_objHero.GetDistance(pPlayer);
            if(dwMyDistance < dwDistance)
            {
                dwDistance = dwMyDistance;
                pTarget = pPlayer;
            }
        }
    }
    if(pTarget && this->CheckEnemy(pTarget))
    {
        m_idTarget = pTarget->GetID();
    }
}
//--------------------------------------------------------------------------------------
void	CPet::SearchEnemyWarlike()
{
    DWORD dwPlayerAmount = g_objPlayerSet.GetPlayerAmount();
    CPlayer* pTarget = NULL;
    DWORD dwDistance = 100;
    for(int i = 0; i < dwPlayerAmount; i ++)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayerByIndex(i);
        if(pPlayer && pPlayer->IsMonster() && !pPlayer->IsGuard() && !pPlayer->IsDead()
                && !pPlayer->IsMyPet())
        {
            DWORD dwMyDistance = this->GetDistance(pPlayer);
            if(dwMyDistance < dwDistance)
            {
                dwDistance = dwMyDistance;
                pTarget = pPlayer;
            }
        }
    }
    if(pTarget && this->CheckEnemy(pTarget))
    {
        m_idTarget = pTarget->GetID();
    }
}
//--------------------------------------------------------------------------------------
void	CPet::SearchEnemyJusticalA()
{
    // 找离自己最近的黑名
    DWORD dwPlayerAmount = g_objPlayerSet.GetPlayerAmount();
    CPlayer* pTarget = NULL;
    DWORD dwDistance = 100;
    for(int i = 0; i < dwPlayerAmount; i ++)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayerByIndex(i);
        if(pPlayer && pPlayer->TestStatus(USERSTATUS_DARKRED) && !pPlayer->IsGuard() && !pPlayer->IsDead()
                && !pPlayer->IsMyPet())
        {
            DWORD dwMyDistance = this->GetDistance(pPlayer);
            if(dwMyDistance < dwDistance)
            {
                dwDistance = dwMyDistance;
                pTarget = pPlayer;
            }
        }
    }
    if(pTarget && this->CheckEnemy(pTarget))
    {
        m_idTarget = pTarget->GetID();
    }
}
//--------------------------------------------------------------------------------------
void	CPet::SearchEnemyJusticalB()
{
    // 找离自己最近的红名
    DWORD dwPlayerAmount = g_objPlayerSet.GetPlayerAmount();
    CPlayer* pTarget = NULL;
    DWORD dwDistance = 100;
    for(int i = 0; i < dwPlayerAmount; i ++)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayerByIndex(i);
        if(pPlayer && pPlayer->TestStatus(USERSTATUS_RED) && !pPlayer->IsGuard() && !pPlayer->IsDead()
                && !pPlayer->IsMyPet())
        {
            DWORD dwMyDistance = this->GetDistance(pPlayer);
            if(dwMyDistance < dwDistance)
            {
                dwDistance = dwMyDistance;
                pTarget = pPlayer;
            }
        }
    }
    if(pTarget && this->CheckEnemy(pTarget))
    {
        m_idTarget = pTarget->GetID();
    }
}
//--------------------------------------------------------------------------------------
void	CPet::SearchEnemyJusticalC()
{
    // 找离自己最近的蓝名
    DWORD dwPlayerAmount = g_objPlayerSet.GetPlayerAmount();
    CPlayer* pTarget = NULL;
    DWORD dwDistance = 100;
    for(int i = 0; i < dwPlayerAmount; i ++)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayerByIndex(i);
        if(pPlayer && pPlayer->TestStatus(USERSTATUS_CRIME) && !pPlayer->IsGuard() && !pPlayer->IsDead()
                && !pPlayer->IsMyPet())
        {
            DWORD dwMyDistance = this->GetDistance(pPlayer);
            if(dwMyDistance < dwDistance)
            {
                dwDistance = dwMyDistance;
                pTarget = pPlayer;
            }
        }
    }
    if(pTarget && this->CheckEnemy(pTarget))
    {
        m_idTarget = pTarget->GetID();
    }
}
//--------------------------------------------------------------------------------------
BOOL	CPet::SearchEnemy()
{
    int nAmount = m_AIInfo.setLocateTargetMethod.size();
    if(nAmount > 0 && m_AIInfo.setLocateTargetMethod[0] != _PET_SE_STATUS_ASSISTANT)
    {
        OBJID idSpecial = g_objHero.GetSpecialTarget();
        if(idSpecial != ID_NONE)
        {
            CPlayer* pTarget = g_objPlayerSet.GetPlayer(idSpecial, true);
            if(pTarget && 	this->CheckEnemy(pTarget))
            {
                m_idTarget	= idSpecial;
                return true;
            }
        }
    }
    for(int i = 0; i < nAmount; i ++)
    {
        int nMethod = m_AIInfo.setLocateTargetMethod[i];
        this->SearchEnemy(nMethod);
        if(m_idTarget	!= ID_NONE)
        {
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------------
void	CPet::SearchEnemy(int nMethod)
{
    m_bRevengeID = false;
    switch(nMethod)
    {
    case 1:
        this->AttackAI_1 ();
        //		this->SearchStatusAsistant();
        break;
    case 2:
        this->AttackAI_2 ();
        //		this->SearchAttackAsistant();
        break;
    case 3:
        this->AttackAI_3 ();
        //		this->SearchEnemyRevenge();
        break;
    case 4:
        this->SearchEnemyGuard();
        break;
    case 5:
        this->SearchEnemyWarlike();
        break;
    case _PET_SE_JUSTICAL_A:
        this->SearchEnemyJusticalA();
        break;
    case _PET_SE_JUSTICAL_B:
        this->SearchEnemyJusticalB();
        break;
    case _PET_SE_JUSTICAL_C:
        this->SearchEnemyJusticalC();
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------------
void	CPet::CheckTarget()
{
    BOOL bProcess =  false;
    if(!bProcess)
    {
        bProcess = this->CheckTargetDie();
    }
    if(!bProcess)
    {
        bProcess = this->CheckTargetNotFound();
    }
    if(!bProcess)
    {
        bProcess = this->CheckTargetTooFar();
    }
    //	if(!bProcess)
    //		bProcess = this->CheckTargetSpecialCommand();
    //	if(!bProcess)
    //		bProcess = this->CheckTargetElude();
    if (bProcess)
    {
        bProcess = this->SearchEnemy ();
    }
    if (!bProcess)
    {
        m_nBehaviorType = _PET_BEHAVIOR_FIND_HOST;
    }
}
//--------------------------------------------------------------------------------------
BOOL	CPet::CheckTargetDie()
{
    CPlayer* pTarget = g_objPlayerSet.GetPlayer(m_idTarget, true);
    if(pTarget && pTarget->IsDead())
    {
        m_idTarget = ID_NONE;
        return true;
        //		this->SearchEnemy();
        //		if(m_idTarget != ID_NONE)
        //			return true;
    }
    return false;
}
//--------------------------------------------------------------------------------------
BOOL	CPet::CheckTargetNotFound()
{
    CPlayer* pTarget = g_objPlayerSet.GetPlayer(m_idTarget, true);
    if(!pTarget)
    {
        m_idTarget = ID_NONE;
        return true;
        //		this->SearchEnemy();
        //		if(m_idTarget != ID_NONE)
        //			return true;
    }
    return false;
}
//--------------------------------------------------------------------------------------
BOOL	CPet::CheckTargetTooFar()
{
    CMyPos posHero, posPet;
    g_objHero.GetPos(posHero);
    this->GetPos(posPet);
    if(abs(posPet.x - posHero.x) > 14 || abs(posPet.y - posHero.y) > 14)
    {
        m_idTarget = ID_NONE;
        return true;
        //		this->SearchEnemy();
        //		if(m_idTarget != ID_NONE)
        //			return true;
    }
    return false;
}
//--------------------------------------------------------------------------------------
BOOL	CPet::CheckTargetSpecialCommand()
{
    if(m_idTarget != g_objHero.GetSpecialTarget() && g_objHero.GetSpecialTarget() != ID_NONE)
    {
        OBJID idSpecial = g_objHero.GetSpecialTarget();
        CPlayer* pTarget = g_objPlayerSet.GetPlayer(idSpecial);
        if(pTarget && this->CheckEnemy(pTarget))
        {
            m_idTarget = g_objHero.GetSpecialTarget();
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------------
BOOL	CPet::CheckTargetElude()
{
    return false;
}
//--------------------------------------------------------------------------------------

void	CPet::Idle(DWORD dwDelay)
{
    m_dwIdleEnd = ::TimeGet() + dwDelay;
    m_nBehaviorType = _PET_IDLE;
}
//--------------------------------------------------------------------------------------
void	CPet::Process (void* pInfo)
{
    if(g_objHero.GetIDWhoAttackMe() != m_idTarget && m_bRevengeID)
    {
        m_idTarget = ID_NONE;
        this->StandBy();
    }
    if(this->IsMyPet())
    {
        this->ProcessAI();
    }
    CPlayer::Process(pInfo);
}
//--------------------------------------------------------------------------------------
void	CPet::ProcessAI	()
{
    if(this->IsDead())
    {
        return;
    }
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idTarget, true);
    if(this->IsAttacking() && pPlayer && !pPlayer->IsDead())
    {
        this->Idle(1000);
    }
    switch(m_nBehaviorType)
    {
    case _PET_IDLE:
        this->ProcessAI_Idle();
        break;
    case _PET_BEHAVIOR_READY:
        this->ProcessAI_Ready();
        break;
    case _PET_BEHAVIOR_FIND_HOST:
        this->ProcessAI_FindHost();
        break;
    case _PET_ATTACK:
        this->ProcessAI_Attack();
        break;
    default:
        break;
    }
    this->ProcessAI_GenerateNewBehavior();
}
//--------------------------------------------------------------------------------------
void	CPet::ProcessAI_Idle()
{
    if(::TimeGet() > m_dwIdleEnd)
    {
        m_nBehaviorType = _PET_BEHAVIOR_READY;
    }
}
//--------------------------------------------------------------------------------------
void	CPet::ProcessAI_Ready()
{
}
//--------------------------------------------------------------------------------------
void	CPet::ProcessAI_Attack()
{
    if(this->IsAttacking())
    {
        return;
    }
    else if(m_idTarget == ID_NONE)
    {
        return;
    }
    else
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idTarget, true);
        if(pPlayer)
        {
            this->LockAttack(pPlayer);
        }
    }
}
//--------------------------------------------------------------------------------------

void	CPet::ProcessAI_GenerateNewBehavior()
{
    CMyPos posHero, posPet;
    g_objHero.GetPos(posHero);
    this->GetPos(posPet);
    if(m_nBehaviorType == _PET_IDLE)
    {
        return;
    }
    this->CheckTarget();
    if(m_idTarget != ID_NONE && !this->IsDead())
    {
        m_nBehaviorType = _PET_ATTACK;
        return;
    }
}
//--------------------------------------------------------------------------------------
void	CPet::ProcessAI_FindHost()
{
    CMyPos posHero, posPet;
    g_objHero.GetPos(posHero);
    this->GetPos(posPet);
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(posPet.x, posPet.y);
    if(abs(posHero.x - posPet.x) < 6 && abs(posHero.y - posPet.y) < 6 && pPlayer == this)
    {
        m_nBehaviorType = _PET_BEHAVIOR_READY;
    }
    else if(!this->IsMoving())
    {
        CMyPos posPetTarget;
        int nDir = ::RandGet(8);
        posPetTarget.x = posHero.x + _DELTA_X[nDir] * 4 -::RandGet(2);
        posPetTarget.y = posHero.y + _DELTA_Y[nDir] * 4 -::RandGet(2);
        CellInfo* pCellInfo = g_objGameMap.GetCell(posPet.x, posPet.y);
        if(!pCellInfo ||
                g_objGameMap.GetGroundMask(pCellInfo) == 1 )
        {
            return;
        }
        //		if(abs(posHero.x - posPet.x) < 6 && abs(posHero.y - posPet.y) < 6)
        //			this->Walk(posPetTarget.x, posPetTarget.y);
        //		else
        if (abs(posHero.x - posPet.x) < 8 && abs(posHero.y - posPet.y) < 8)
        {
            this->Run(posPetTarget.x, posPetTarget.y);
        }
        else
        {
            this->Transfer(posPetTarget.x, posPetTarget.y);
        }
    }
}
//--------------------------------------------------------------------------------------
void CPet::LockAttack (CPlayer* pTarget)
{
    if (!pTarget)
    {
        return;
    }
    if (this->IsDead() || pTarget->IsDead())
    {
        return;
    }
    this->Idle(4000);
    // attack now
    {
        // set lock attck command
        CCommand cmd;
        cmd.iType		= _COMMAND_LOCKATK;
        cmd.iStatus		= _CMDSTATUS_BEGIN;
        cmd.idTarget	= pTarget->GetID();
        this->SetCommand(&cmd);
        m_bFirstAtk = true;
    }
}
//--------------------------------------------------------------------------------------

void	CPet::Transfer(int nPosX, int nPosY)
{
    CMyPos posPet;
    posPet.x = nPosX;
    posPet.y = nPosY;
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(nPosX, nPosY);
    CellInfo* pCellInfo = g_objGameMap.GetCell(nPosX, nPosY);
    if(!pCellInfo ||
            g_objGameMap.GetGroundMask(pCellInfo) == 1 )
    {
        return;
    }
    CMsgAction msg;
    if(msg.Create(this->GetID(), posPet.x, posPet.y, m_Info.cmdProc.nDir, actionJump, posPet.x, posPet.y, m_Info.cmdProc.dwData))
    {
        msg.Send();
    }
    this->SetPos(posPet);
}
//--------------------------------------------------------------------------------------
BOOL	CPet::CanJump(CMyPos posTarGet)
{
    // Can I Jump Over It?
    int nCanJumpHeight =  200;
    //	if(this->TestStatus(USERSTATUS_FLY))
    //		nCanJumpHeight = 100;
    CMyPos posStart, posEnd;
    posEnd = posTarGet;
    DEQUE_CELL setCell;
    this->GetPos(posStart);
    CMyPos posHighest;
    g_objGameMap.Sampling(setCell, posHighest, posStart, posEnd, 32);
    CellInfo* pTargetCell = g_objGameMap.GetCell(posHighest.x, posHighest.y);
    if(!pTargetCell)
    {
        return false;
    }
    CellInfo* pCellInfo = g_objGameMap.GetCell(posStart.x, posStart.y);
    if(!pCellInfo)
    {
        return false;
    }
    if(g_objGameMap.GetGroundAltitude(pTargetCell) - g_objGameMap.GetGroundAltitude(pCellInfo) > nCanJumpHeight)
    {
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------------
void	CPet::Walk(int nPosX, int nPosY)
{
    int nCommandType = this->GetCommandType();
    if(nCommandType == _COMMAND_ATTACK ||
            nCommandType == _COMMAND_ACTION ||
            nCommandType == _COMMAND_EMOTION ||
            nCommandType == _COMMAND_WOUND ||
            nCommandType == _COMMAND_DEFEND)
    {
        this->ResetActionData();
    }
    if(this->GetCommandType() == _COMMAND_ATTACK)
    {
        this->ResetActionData();
    }
    extern UINT	g_uStatus;
    if (_STATUS_NORMAL != g_uStatus)
    {
        return;
    }
    //	this->AbortIntone();
    if (this->IsDead() && !this->TestStatus(USERSTATUS_GHOST))
    {
        return;
    }
    // prevent repeat the same target point
    CCommand* pcmdNow	= this->GetCommand();
    if(_COMMAND_WALK == pcmdNow->iType)
    {
        if(nPosX ==  pcmdNow->posTarget.x &&
                nPosY ==  pcmdNow->posTarget .y)
        {
            return;
        }
    }
    // set command of role
    CCommand cmd;
    cmd.iType			= _COMMAND_WALK;
    cmd.iStatus			= _CMDSTATUS_BEGIN;
    cmd.posTarget.x		= nPosX;
    cmd.posTarget.y		= nPosY;
    this->SetCommand(&cmd);
}
//--------------------------------------------------------------------------------------
void	CPet::Run(int nPosX, int nPosY)
{
    //	if(this->IsMagicLock())
    //		return;
    if(this->TestStatus(USERSTATUS_CANNOTMOVE))
    {
        return;
    }
    //	this->StopChargeUp();
    int nCommandType = this->GetCommandType();
    if(nCommandType == _COMMAND_ATTACK ||
            nCommandType == _COMMAND_ACTION ||
            nCommandType == _COMMAND_EMOTION ||
            nCommandType == _COMMAND_WOUND ||
            nCommandType == _COMMAND_DEFEND)
    {
        this->ResetActionData();
    }
    //	if(this->TestStatus(USERSTATUS_FLY))
    //	{
    //		this->Jump(nPosX, nPosY);
    //		return;
    //	}
    if(this->GetCommandType() == _COMMAND_ATTACK)
    {
        this->ResetActionData();
    }
    //	if(m_objBoothManager.IsActive())
    //		return;
    extern UINT	g_uStatus;
    if (_STATUS_NORMAL != g_uStatus)
    {
        return;
    }
    //	this->AbortIntone();
    if (this->IsDead())
    {
        if (this->TestStatus(USERSTATUS_GHOST))
        {
            this->Walk(nPosX, nPosY);
        }
        return;
    }
    // prevent repeat the same target point
    CCommand* pcmdNow	= this->GetCommand();
    if(_COMMAND_RUN == pcmdNow->iType)
    {
        if(nPosX ==  pcmdNow->posTarget.x &&
                nPosY ==  pcmdNow->posTarget .y)
        {
            return;
        }
    }
    // here got player already
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(nPosX, nPosY);
    if (pPlayer && !pPlayer->IsDead())
    {
        return;
    }
    // set command of role
    CCommand cmd;
    cmd.iType			= _COMMAND_RUN;
    cmd.iStatus			= _CMDSTATUS_BEGIN;
    cmd.posTarget.x		= nPosX;
    cmd.posTarget.y		= nPosY;
    this->SetCommand(&cmd);
}
//--------------------------------------------------------------------------------------

DWORD	CPet::SetCommand(CCommand* pCommand)
{
    if (!pCommand)
    {
        return 0;
    }
    m_dwCommandIndex++;
    pCommand->dwIndex = m_dwCommandIndex;
    // lock attack 命令不可积累
    if(pCommand->iType == _COMMAND_LOCKATK ||
            pCommand->iType == _COMMAND_WALK ||
            pCommand->iType == _COMMAND_RUN)
    {
        for(int i = 0; i < m_infoPlayer.setCmd.size(); i ++)
        {
            if(m_infoPlayer.setCmd[i].iType ==  _COMMAND_LOCKATK ||
                    pCommand->iType == _COMMAND_WALK ||
                    pCommand->iType == _COMMAND_RUN)
            {
                m_infoPlayer.setCmd.erase(m_infoPlayer.setCmd.begin() + i);
                break;
            }
        }
    }
    // 受伤动作不累积
    // dodge动作不累积
    if(pCommand->iType == _COMMAND_ACTION)
        if(pCommand->nData == _ACTION_DODGE0 || pCommand->nData == _ACTION_BRUISE0)
        {
            if(m_infoPlayer.setCmd.size() != 0)
            {
                return false;
            }
        }
    m_infoPlayer.setCmd.push_back(*pCommand);
    if (_COMMAND_STANDBY != pCommand->iType)
    {
        // emotion, standby and action can be interrupted at any time.
        if (_COMMAND_STANDBY == m_Info.cmdProc.iType
                || _COMMAND_EMOTION == m_Info.cmdProc.iType
                || _COMMAND_INTONE == m_Info.cmdProc.iType)
        {
            this->ResetActionData();
        }
        // cmd accomplished can be interrupted at andy time
        if (_CMDSTATUS_ACCOMPLISH == m_Info.cmdProc.iStatus)
        {
            if (_ACTION_STANDBY == m_Info.iActType
                    || _ACTION_REST1 == m_Info.iActType
                    || _ACTION_REST2 == m_Info.iActType
                    || _ACTION_REST3 == m_Info.iActType)
            {
                this->ResetActionData();
            }
        }
        // die command can interrupt any command
        if (_COMMAND_DIE == pCommand->iType)
        {
            this->ResetActionData();
        }
    }
    // set command now
    memcpy(&(m_Info.cmdProc), pCommand, sizeof(CCommand));
    return m_dwCommandIndex;
}

//--------------------------------------------------------------------------------------
void	CPet::Jump(int nPosX, int nPosY)
{
    int nCommandType = this->GetCommandType();
    if(nCommandType == _COMMAND_ATTACK ||
            nCommandType == _COMMAND_ACTION ||
            nCommandType == _COMMAND_EMOTION ||
            nCommandType == _COMMAND_WOUND ||
            nCommandType == _COMMAND_DEFEND)
    {
        this->ResetActionData();
    }
    extern UINT	g_uStatus;
    if (_STATUS_NORMAL != g_uStatus)
    {
        return;
    }
    if (this->IsDead())
    {
        return;
    }
    CMyPos posTarget;
    posTarget.x = nPosX;
    posTarget.y = nPosY;
    const OBJID idCanNotJump = 100022;
    const OBJID idCanNotStay = 100023;
    CMyPos posPet;
    this->GetPos(posPet);
    CONST MAX_JUMPDISTANCE = 16;
    if (MAX_JUMPDISTANCE < g_objGameMap.GetDistance(posPet.x, posPet.y, nPosX, nPosY))
    {
        return;
    }
    if(!this->CanJump(posTarget))
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(idCanNotJump));
        return;
    }
    CellInfo* pCellInfo = g_objGameMap.GetCell(nPosX, nPosY);
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(nPosX, nPosY);
    if(!pCellInfo ||
            g_objGameMap.GetGroundMask(pCellInfo) == 1 ||
            (pPlayer && !pPlayer->IsDead()))
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(idCanNotStay));
        return;
    }
    if (posPet.x == nPosX && posPet.y == nPosY)
    {
        return;
    }
    // set command of role
    CCommand cmd;
    cmd.iType			= _COMMAND_JUMP;
    cmd.iStatus			= _CMDSTATUS_BEGIN;
    cmd.posTarget.x		= nPosX;
    cmd.posTarget.y		= nPosY;
    cmd.dwData			=::TimeGet();
    this->SetCommand(&cmd);
}
//--------------------------------------------------------------------------------------
BOOL	CPet::ProcessCommand()
{
    int iPosX, iPosY;
    this->GetPos(iPosX, iPosY);
    //--------------------------------------------------------
    if(_COMMAND_JUMP == m_Info.cmdProc.iType)
    {
        if(_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)		// is a new jump cmd
        {
            CMsgAction msg;
            if(msg.Create(this->GetID(), iPosX, iPosY, m_Info.cmdProc.nDir, actionJump, m_Info.cmdProc.posTarget.x, m_Info.cmdProc.posTarget.y, m_Info.cmdProc.dwData))
            {
                msg.Send();
            }
        }
    }
    //--------------------------------------------------------
    if(_COMMAND_EMOTION == m_Info.cmdProc.iType)
    {
        if(_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)		// is a new emotion cmd
        {
            CMsgAction	msg;
            if(msg.Create(this->GetID(), iPosX, iPosY, m_Info.cmdProc.nDir, actionEmotion, m_Info.cmdProc.nData))
            {
                msg.Send();
            }
        }
    }
    //--------------------------------------------------------
    if(_COMMAND_ATTACK == m_Info.cmdProc.iType)
    {
        if(_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)		// is a new atk cmd
        {
            CMsgInteract msg;
            if(msg.Create(INTERACT_ATTACK, this->GetID(), m_Info.cmdProc.idTarget, iPosX, iPosY))
            {
                msg.Send();
            }
        }
    }
    //--------------------------------------------------------
    if(_COMMAND_SHOOT == m_Info.cmdProc.iType)
    {
        if(_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)		// is a new atk cmd
        {
            // 转移到普通攻击
            m_Info.cmdProc.iType = _COMMAND_ATTACK;
            CMsgInteract msg;
            if(msg.Create(INTERACT_SHOOT, this->GetID(), m_Info.cmdProc.idTarget, m_Info.cmdProc.dwData))
            {
                msg.Send();
                CMagicEffect* pEffect = CMagicEffect::CreateNewArrow(this->GetID(), m_Info.cmdProc.idTarget, m_Info.cmdProc.nData);
                g_objGameMap.AddMagicEffect(pEffect);
            }
        }
    }
    //--------------------------------------------------------
    if(_COMMAND_RUSHATK == m_Info.cmdProc.iType)
    {
        if(_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
        {
            CMsgInteract msg;
            if(msg.Create(INTERACT_RUSHATK, this->GetID(), m_Info.cmdProc.idTarget, iPosX, iPosY, m_Info.cmdProc.posTarget.x, m_Info.cmdProc.posTarget.y))
            {
                msg.Send();
            }
        }
    }
    //--------------------------------------------------------
    // do command now
    //int iStatusBk	=m_Info.cmdProc.iStatus;
    BOOL bAcomplished	= CRole::ProcessCommand();
    //--------------------------------------------------------
    this->GetPos(iPosX, iPosY);
    switch(this->GetActionType())
    {
    case _ACTION_WALKL:
    case _ACTION_WALKR:
        {
            CMsgWalk msg;
            if(msg.Create(this->GetID(), this->GetDir() + 8 *::RandGet(31)))
            {
                msg.Send();
            }
        }
        break;
    case _ACTION_RUNL:
    case _ACTION_RUNR:
        {
            CMsgWalk msg;
            if(msg.Create(this->GetID(), this->GetDir() + 8 *::RandGet(31), m_Info.nNextStep))
            {
                msg.Send();
            }
        }
        break;
    }
    if (this->GetLastCommand() == _COMMAND_LOCKATK &&
            this->GetCommandStatus() == _CMDSTATUS_ACCOMPLISH)
    {
        if(!this->HaveBow())
        {
            CMsgInteract msg;
            if (msg.Create(INTERACT_ATTACK, this->GetID(), m_Info.cmdProc.idTarget, iPosX, iPosY))
            {
                msg.Send();
            }
        }
        else
        {
            CMsgInteract msg;
            if(msg.Create(INTERACT_SHOOT, this->GetID(), m_Info.cmdProc.idTarget, m_Info.cmdProc.dwData))
            {
                msg.Send();
            }
        }
    }
    //--------------------------------------------------------
    return bAcomplished;
}
//--------------------------------------------------------------------------------------
void CPet::AddMagic(OBJID idMagic, DWORD dwLevel, DWORD dwExp)
{
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMagic* pMagic = m_setMagic[i];
        if(pMagic && pMagic->m_infoMagicType.idMagicType == idMagic)
        {
            MagicTypeInfo infoMagicType;
            if(CMagic::s_objMagicData.GetMagicTypeInfo(idMagic, dwLevel, infoMagicType))
            {
                pMagic->m_infoMagicType = infoMagicType;
                pMagic->m_dwExp = dwExp;
                pMagic->m_idOwner = this->GetID();
            }
            return;
        }
    }
    CMagic* pMagic = CMagic::CreateNew(idMagic, dwLevel, dwExp);
    if(pMagic)
    {
        pMagic->m_idOwner = this->GetID();
        m_setMagic.push_back(pMagic);
        ::PostCmd(CMD_FLASH_PETSTATUS);
    }
}
//--------------------------------------------------------------------------------------
int CPet::GetMagicAmount()
{
    int nMagicAmount = 0;
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic)
        {
            nMagicAmount++;
        }
    }
    return nMagicAmount;
}
//--------------------------------------------------------------------------------------
CMagic* CPet::GetMagicByIndex(int nIndex)
{
    int nMagicIndex = 0;
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic && nMagicIndex == nIndex)
        {
            return pMagic;
        }
        nMagicIndex++;
    }
    return NULL;
}
//--------------------------------------------------------------------------------------
CMagic* CPet::GetMagic(OBJID idMagicType)
{
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = m_setMagic[i];
        if(pMagic && (pMagic->m_infoMagicType.idMagicType == idMagicType))
        {
            return pMagic;
        }
    }
    return NULL;
}
//--------------------------------------------------------------------------------------
void CPet::DestroyMagicSet()
{
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = m_setMagic[i];
        SAFE_DELETE(pMagic);
    }
    m_setMagic.clear();
}
//--------------------------------------------------------------------------------------
void CPet::AttackAI_1()
{
    OBJID id = g_objHero.GetIDWhoAttackMe ();	//攻击主人的对象
    if ( id != ID_NONE)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer (id);
        if (this->CheckEnemy (pPlayer, 3))		//是否在主人的3格之内
        {
            m_idTarget = id;
            return;
        }
    }
    id = this->GetIDWhoAttackMe ();		//攻击自己的对象
    if (id != ID_NONE)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer (id); //是否在主人3格之内
        if (this->CheckEnemy (pPlayer, 3))
        {
            m_idTarget = id;
            return;
        }
    }
    int nAmount = g_objPlayerSet.GetPlayerAmount ();
    DWORD dwDistance = 100;
    for(int i = 0; i < nAmount; i++)		//检测3格之内距离自己最近的对象
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayerByIndex (i);
        if (pPlayer && this->CheckEnemy(pPlayer, 3) && pPlayer->IsMonster () && !pPlayer->IsGuard ())
        {
            DWORD dwTmpDistance = this->GetDistance (pPlayer);
            if (dwDistance > dwTmpDistance)
            {
                dwDistance = dwTmpDistance;
                m_idTarget = pPlayer->GetID ();
            }
        }
    }
}

void CPet::AttackAI_2 ()
{
    OBJID id = this->GetIDWhoAttackMe (); //攻击自己的对象
    if (id != ID_NONE)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer (id);	//是否在自己5格之内
        if (this->CheckEnemy (pPlayer, 5, false))
        {
            m_idTarget = id;
            return;
        }
    }
    int nAmount = g_objPlayerSet.GetPlayerAmount ();
    DWORD dwDistance = 100;
    for(int i = 0; i < nAmount; i++)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayerByIndex (i);
        if (pPlayer && this->CheckEnemy(pPlayer, 5, false) && pPlayer->IsMonster () && !pPlayer->IsGuard ())		//自己5格之内距离自己最近的对象
        {
            DWORD dwTmpDistance = this->GetDistance (pPlayer);
            if (dwDistance > dwTmpDistance)
            {
                dwDistance = dwTmpDistance;
                m_idTarget = pPlayer->GetID ();
            }
        }
    }
}

void CPet::AttackAI_3 ()
{
    int nAmount = g_objPlayerSet.GetPlayerAmount ();
    int nWhiteBlack = 0;
    for(int i = 0; i < nAmount; i++)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayerByIndex (i);
        if (pPlayer && this->CheckEnemy(pPlayer, 5, false) && pPlayer->IsMonster () && !pPlayer->IsGuard ())		//自己5格之内距离自己最近的对象
        {
            CMonsterInfo* pInfo = (CMonsterInfo*)g_obj3DRoleData.GetMonsterInfo(pPlayer->GetMonsterTypeId());
            if (pInfo)
            {
                int nTmpWhiteBlack = pInfo->nWhiteBlack;
                if (nWhiteBlack > nTmpWhiteBlack)
                {
                    nWhiteBlack = nTmpWhiteBlack;
                    m_idTarget = pPlayer->GetID ();
                }
            }
        }
    }
}

void CPet::AttackAI_4 ()
{
}

void CPet::AttackAI_5 ()
{
}
