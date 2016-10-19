
//**********************************************************
// 代码编辑器
//**********************************************************

// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "AllMsg.h"
#include "MsgDataArray.h"
#include "Package.h"
#include "User.h"
#include "MapGroup.h"
#include "netmsg.h"
#include "ItemType.h"
#include "team.h"
#include "StatusOnce.h"
#include "Transformation.h"
#include "WantedList.h"
#include "PoliceWanted.h"
#include "MapTrap.h"
#include "I_MessagePort.h"
#include "DeadLoop.h"
#include "AutoPtr.h"
#include "I_Shell.h"
#include "MercenaryTaskData.h"
#include "MercenaryTask.h"
#include "math.h"
#include "TaskDetail.h"


//extern long	g_sKickoutCheat;
long	g_sKickoutCheat = 0;
//////////////////////////////////////////////////////////////////////
const int	ADD_ENERGY_STAND_SECS			= 5;			// 每多少秒增加一次
const int	ADD_ENERGY_STAND				= 50;			// 站着增加数量
const int	DEL_ENERGY_PELT_SECS			= 3;			// 疾行状态每3秒减一次
const int	DEL_ENERGY_PELT					= 2;			// 每次扣2点
const int	KEEP_STAND_MS					= 1500;			// 站稳需要的时间，站稳后能挡住玩家。
const int	CRUSH_ENERGY_NEED				= 100;			// 挤开需要多少点
const int	CRUSH_ENERGY_EXPEND				= 100;			// 挤开消耗多少点
const int	SYNWAR_PROFFER_PERCENT			= 1;			// 打柱子得经验值的百分比
const int	SYNWAR_MONEY_PERCENT			= 2;			// 打柱子得钱的百分比
const int	MIN_SUPERMAP_KILLS				= 25;			// 无双列表低限
//const int	NEWBIE_LEVEL					= 70;			// 新手的等级
const int	VETERAN_DIFF_LEVEL				= 20;			// 老手的等级差
//const int	_MIN_TUTOR_LEVEL				= 50;							// 导师最低等级
const int	HIGHEST_WATER_WIZARD_PROF		= 135;			// 水真人
const int	MOUNT_ADD_INTIMACY_SECS			= 3600;			//
const int	INTIMACY_DEAD_DEC				= 20;			//
const int	SLOWHEALLIFE_MS					= 1000;							// 吃药补血慢慢补上去的豪秒数。
const int	AUTOHEALLIFE_TIME				= 10;							// 每隔10秒自动补血1次。
const int	AUTOHEALLIFE_EACHPERIOD			= 6;							// 每次补血6。
const int	AUTOHEAL_MAXLIFE_TIME			= 60;							// 每隔60秒自动恢复maxlife
const int	AUTOHEAL_MAXLIFE_FLESH_WOUND	= 16;							// 轻伤状态下每次恢复maxlife的千分比
const int	AUTOHEAL_MAXLIFE_GBH			= 4;							// 重伤状态下每次恢复maxlife的千分比
const int	TICK_SECS						= 10;
const int	MAX_PKLIMIT						= 10000;						// PK值的最大最小限制(防回卷)  //change huang 2004.1.11
const int	PILEMONEY_CHANGE				= 5000;							// 大量现金变动(需要即时存盘)
const int	_ADDITIONALPOINT_NUM			= 3;							// 升1级加多少属性点
const int	PK_DEC_TIME						= 60;							// 每间隔60秒降低pk值     //change huang 2004.1.11
const int   PKVALUE_DEC_ONCE                = 1;                            // 按时间减少PK值。
const int	PKVALUE_DEC_ONCE_IN_PRISON		= 3;							// 按时间减少PK值——在监狱中的情况
const int	PKVALUE_DEC_PERDEATH			= 10;							// 被PK死一次减少的PK值
const int	TIME_TEAMPRC					= 5;							// 队伍信息处理的时间
const int	DURABILITY_DEC_TIME				= 12;							// 每12秒降低一次按时间消耗的装备耐久度
const int	USER_ATTACK_SPEED				= 800;							// 玩家徒手攻击频率
const int	POISONDAMAGE_INTERVAL			= 2;							// 中毒每2秒伤血一次
const int	WARGHOST_CHECK_INTERVAL			= 10;							// 每隔10秒检查一次战魂等级(武器经验)
const int	AUTO_REBORN_SECS				= 30;							// 复活宝石30秒后自动使用
const int	INC_POTENTIAL_SECS				= 6 * 60;							// 每隔6分钟增加一次潜能
const int	INC_POTENTIAL_NUM				= 1;							// 每次增加1点潜能
const int	ADD_POTENTIAL_RELATIONSHIP		= 6;							// 关系值之和每达到6增加1点潜力值

// add by zlong 2003-11-28
const int	SPRITE_ADD_EXP_SECS				= 60;			//元素精灵每分钟增加一次经验
const int	SPRITE_ADD_EXP					= 1;			//经验增加数量

const int	EUDEMON_ADD_FIDELITY_SECS		= 60 * 6;			// 幻兽每6分钟增加一次亲密度
const int	EUDEMON_ADD_FIDELITY			= 1;			// 每次增加的亲密度
const int	MAX_EUDEMON_FIDELITY			= 1000;			// 最大亲密度1000
const int	MAX_RETURN_BORN_POS_LEV			= 4;			// 死亡后返回初始出生点的最大等级
//////////////////////////////////////////////////////////////////////
MYHEAP_IMPLEMENTATION(CUser, s_heap)
char	szActionTable[]			= _TBL_ACTION;
char	szTaskTable[]			= _TBL_TASK;
//char	szMagicTypeTable[]		= _TBL_MAGICTYPE;
char	szTrackTable[]			= _TBL_TRACK;
char	szBonusTable[]			= _TBL_BONUS;
char	szAddPointTable[]		= _TBL_POINTALLOT;
char	szTrapTypeTable[]		= _TBL_TRAPTYPE;
char	szMonsterTypeTable[]	= _TBL_MONSTERTYPE;
char	szRebirthTable[]		= _TBL_REBIRTH;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction CHEAT
//////////////////////////////////////////////////////////////////////
bool CUser::SetCheatLevel(int nLevel)
{
#if	!defined(_DEBUG)
    if (IsGM() || GetLev() < LOGIN_FREE_LEVEL)
    {
        return false;
    }
#endif
    // init count
    if (m_nCheatLevel == 0)
    {
        m_nCheatCount = UserManager()->GetCheatCount(GetID());
    }
    // is max?
    if (nLevel <= m_nCheatLevel)
    {
        return false;
    }
    if (!IsCheater(_TYPE_MAGIC))
    {
        m_nCheatLevel	= nLevel;
        if (m_nCheatLevel + m_nCheatCount * 10 >= 30)
        {
            SetCheatMark(_TYPE_MAGIC, 0);
            // add to manager
            int nAmount, nTimes;
            UserManager()->AddCheat(GetID(), &nAmount, &nTimes);
            LOGCHEAT("magic: user[%s], account[%u], type[zffy], level[%d], amount[%d], times[%d]", GetName(), GetAccountID(), nLevel, nAmount, nTimes);
            return true;
        }
    }
    else
    {
        m_nCheatLevel	= nLevel;
        LOGCHEAT("magic: user[%s], account[%u], type[zffy], level[%d]", GetName(), GetAccountID(), nLevel);
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsCheater(OBJID idKind)
{
    CHECKF(idKind < 32);
    return (m_maskCheat & (1 << idKind)) != 0;
}

char* g_setName[] = { "_TYPE_MAGIC", "_TYPE_WS", "_TYPE_FY", "_TYPE_USE_LIFE", "_TYPE_USE_MANA",
                      "null", "null", "null", "null", "null", "null", "null", "null", "null", "null",
                      "null", "null", "null", "null", "null", "null", "null", "null", "null", "null",
                      "null", "null", "null", "null", "null", "null", "null", "null", "null", "null",
                    };
//////////////////////////////////////////////////////////////////////
void CUser::SetCheatMark(OBJID idCheatKind, DWORD dwCheatMark)
{
    CHECK(idCheatKind < 32);
    LOGCHEAT("client: user[%s], account[%u], type[%s], mark[%d]", GetName(), GetAccountID(), g_setName[idCheatKind], dwCheatMark);
    m_maskCheat = m_maskCheat | (1 << idCheatKind);
}

//////////////////////////////////////////////////////////////////////
void CUser::KickoutCheat(OBJID idKind)
{
    CHECK(idKind < 32);
    CHECK(IsCheater(idKind));
#if	!defined(_DEBUG)
    //	if (UserManager()->GetCheatAmount() < 5)
    //		return ;
#endif
    // kickout
    if (GetLev() >= LOGIN_FREE_LEVEL && (g_sKickoutCheat&(1 << idKind)) != 0 )
    {
#if	defined(_DEBUG)
        SendSysMsg("★CHEAT★");
#endif
        UserManager()->KickOutSocket(m_idSocket, "cheat");
    }
    else
    {
        LOGCHEAT("need kick: user[%s], account[%u], type[%s]", GetName(), GetAccountID(), g_setName[idKind]);
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsMagicAtkCheat (OBJID idTarget, int nTargetX, int nTargetY, DWORD dwTimeStamp)
{
    IRole* pRole = RoleManager()->QueryRole(idTarget);
    if (!pRole)
    {
        return false;
    }
    // action mode test
    {
        if (dwTimeStamp - m_dwLastJump < 100)
        {
            //::MyLogSave("gmlog/test", "---------suspect action found!");
            return true;
        }
    }
    // interval test
    CONST int TEST_SIZE = 8;
    m_setActionTime.push_back(dwTimeStamp);
    if (m_setActionTime.size() > TEST_SIZE)
    {
        m_setActionTime.pop_front();
    }
    if (m_setActionTime.size() != TEST_SIZE)
    {
        return false;
    }
    {
        deque<int> setDeltaTime;
        for (int i = 1; i < TEST_SIZE; i++)
        {
            setDeltaTime.push_back(m_setActionTime[i] - m_setActionTime[i-1]);
        }
        CONST int SUSPECT_TIME = 20;
        int nSuspectCount = 0;
        for (int i = 1; i < TEST_SIZE - 1; i++)
        {
            if (abs(setDeltaTime[0] - setDeltaTime[i]) <= SUSPECT_TIME)
            {
                nSuspectCount++;
            }
        }
        if (nSuspectCount >= TEST_SIZE - 2)
        {
            //::MyLogSave("gmlog/test", "---------repeat action found!");
            return true;
        }
    }
    // target pos test
    if (nTargetX == 0 && nTargetY == 0)
    {
        return true;
    }
    // dislocation test
    // ....
    return false;
}

//////////////////////////////////////////////////////////////////////
void CUser::DoCheaterPunish	(int nData/* = 0*/)
{
    CONST OBJID idMapCheaterPrison = 6010;
    CONST int nPosX = 34, nPosY = 74;
    if (this->GetMapID() == idMapCheaterPrison)
    {
        return;
    }
    this->SetRecordPos(idMapCheaterPrison, nPosX, nPosY, true);
    this->FlyMap(idMapCheaterPrison, nPosX, nPosY);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUser::CUser()
{
    SetObjType(OBJ_USER);
    m_link.Init(this);
    m_nStep			= STEP_LOGIN;		// 地图组首先是进入地图
    // item ----------------------
    m_pWeaponR		= NULL;
    m_pWeaponL		= NULL;
    m_pHelmet		= NULL;
    m_pNecklace		= NULL;
    m_pArmor		= NULL;
    m_pRingR		= NULL;
    m_pRingL		= NULL;
    m_pShoes		= NULL;
    m_pMount		= NULL;
    m_pSprite		= NULL;
    m_pMantle		= NULL;
    m_bUpdateWeight	= true;			// true: 需要重新计算
    m_nPose			= 0;	//DEFAULT_LOGIN_POSE;
    m_nEmotin		= 0;	//DEFAULT_LOGIN_EMOTION;
    m_dwXP			= 0;
    m_pTeam			= NULL;
    // tick -----------
    m_dwFirstServerTick	= 0;
    m_dwLastServerTick	= 0;
    m_dwLastClientTick	= 0;
    m_dwFirstClientTick	= 0;
    m_dwLastCoolShow	= 0;
    m_dwLastRcvClientTick = 0;
    // interface
    m_ifaceSquareDeal.m_pParent	= this;
    m_pBattleSystem	= NULL;
    m_pStorage		= NULL;
    m_idTaskNpc		= ID_NONE;
    m_idTaskItem	= ID_NONE;
    m_idTaskItemAction	= ID_NONE;
    m_bDebugAction	= false;
    m_pPackage		= CUserPackage::CreateNew();
    m_pUserWeaponSkillSet	= CUserWeaponSkillSet::CreateNew(true);
    m_pMagic		= NULL;
    m_pSyn			= NULL;
    m_bGhost		= false;
    m_bHaveLeaveWord	= true;
    m_pTransformation	= NULL;
    //	m_bSynchro		= true;
    m_pBooth		= NULL;
    m_idxLastArrow	= -1;		// error
    // foot print
    m_tFootPrint	= 0;
    m_posFootPrint.x	= 0;
    m_posFootPrint.y	= 0;
    // superman list
    m_nCurrSupermanKills	= 0;
    m_nTopSupermanKills		= 0;
    m_nSupermanOrder		= 0;
    // cheat test
    m_dwLastJump = 0;
    m_dwMsgCount = 0;
    // tutor
    m_pTutor	= NULL;
    m_dwExpToTutor	= 0;
    // mine
    m_pMineTarget	= NULL;
    //帮派服装的armor type;
    m_nSynDressArmorType = 0;
    m_pTaskDetail	= NULL;
    m_bIsEnableWarGhostExp = false;
    m_idLinkEudemonItem	= ID_NONE;
    m_idLinkEudemonType	= ID_NONE;
    m_nLinkValue		= 0;
}

//////////////////////////////////////////////////////////////////////
CUser::~CUser()
{
    LeaveMapGroup();
    if (m_pBattleSystem)
    {
        delete m_pBattleSystem;
        m_pBattleSystem	= NULL;
    }
    if (m_pStorage)
    {
        m_pStorage->Release();
    }
    if (m_pPackage)
    {
        m_pPackage->Release();
    }
    if (m_pUserWeaponSkillSet)
    {
        m_pUserWeaponSkillSet->Release();
    }
    if (m_pMagic)
    {
        m_pMagic->ReleaseByOwner();
    }
    if (m_pSyn)
    {
        m_pSyn->ReleaseByOwner();
    }
    SAFE_RELEASE (m_setStatus);
    if (m_pTeam)
    {
        m_pTeam->ReleaseByOwner();
    }
    if (m_pTransformation)
    {
        m_pTransformation->Release();
    }
    if (m_pTaskDetail)
    {
        m_pTaskDetail->Release();
    }
    DEBUG_TRY
    Announce()->DeleteAnnounceByUserID(this->GetID(), false);
    DEBUG_CATCH("Announce()->DeleteAnnounceByUserID(this->GetID()) ERROR!");
    this->DestroyBooth();
}

//////////////////////////////////////////////////////////////////////
CUser* CUser::CreateNewUser(PROCESS_ID idProcess, SOCKET_ID idSocket, UserInfoStruct* pInfo)	// join from other map_group
{
    CHECKF(CMapGroup::IsValidMapGroupProcessID(idProcess));
    CHECKF(pInfo);
    CUserPtr pUser = CUser::CreateNew();
    CHECKF(pUser);
    if (pUser)
    {
        pUser->m_idProcess	= idProcess;
        pUser->m_idSocket	= idSocket;
        memcpy(&pUser->m_data.m_Info, pInfo, sizeof(UserInfoStruct));
        pUser->m_data.m_pRes = MapGroup(idProcess)->QueryDataDefault()->GetUserData()->CreateNewRecord(pInfo->id);
        if (!pUser->m_data.m_pRes)
        {
            pUser->ReleaseByOwner();
            return NULL;
        }
    }
    pUser->Init();
    pUser->QueryEnemy()->Create(idProcess, pUser);
    pUser->m_pMap	= NULL;			//? 保证为空，用于控制连续切屏。
    // transformation
    if (pUser->m_data.m_Info.dwMonsterType && pUser->m_data.m_Info.dwMonsterSecs)
    {
        ASSERT(pUser->Transform(pUser->m_data.m_Info.dwMonsterType, pUser->m_data.m_Info.dwMonsterSecs));
    }
    return pUser;
}

//////////////////////////////////////////////////////////////////////
// login, return false: logout_user
bool CUser::EnterMap()
{
    if (m_nStep != STEP_LOGIN)
    {
        return false;
    }
    SendLight();
    CMapPtr pMap = GetMap();
    if (pMap)
    {
        UpdateBroadcastSet();
        pMap->EnterRoom(this->QueryMapThing(), WITH_BLOCK);
        //		pMap->SendBlockInfo((IRole*)this);
        pMap->SendRegionInfo(this);
        pMap->SendMapInfo(this);
        //		SendSelfToBlock();
    }
    //	if (!GetMap()->IsWingEnable())
    //		QueryStatusSet()->DelObj(STATUS_WING);
    // 进入市场地图解除XPFULL状态
    if (pMap && pMap->IsBoothEnable())
    {
        DetachStatus((IRole*)this, STATUS_XPFULL);
    }
    CRole::AttachStatus(this->QueryRole(), STATUS_PK_PROTECT, 0, CHGMAP_LOCK_SECS);
    //	m_tLock.Startup(CHGMAP_LOCK_SECS);
    m_nStep = STEP_ENTERMAP;
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::SendSelfToBlock()
{
    // brocast my info
    CMsgPlayer msg;
    if (msg.Create((IRole*)this))
    {
        this->BroadcastRoomMsg(&msg, EXCLUDE_SELF);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::SetMaxLifePercent(int nPercent, BOOL bUpdate /*= TRUE*/)
{
    bool bAboveHalfLife = this->GetLife() * 2 >= this->GetMaxLife();
    m_data.SetMaxLifePercent(nPercent);		// 由SetMaxLifePercent保证不低于1/8
    bool bBelowHalfLife = this->GetLife() * 2 < this->GetMaxLife();
    bool bSpeedChanged = (bAboveHalfLife && bBelowHalfLife) || (!bAboveHalfLife && !bBelowHalfLife);
    // 通知客户端
    CMsgUserAttrib	msg;
    if (msg.Create(GetID(), _USERATTRIB_MAXLIFE, GetMaxLife()))
    {
        // 是否影响到速度？
        IStatus* pStatus = this->QueryStatus(STATUS_SLOWDOWN2);
        if (pStatus && bSpeedChanged)
        {
            msg.Append(_USERATTRIB_SPEED, AdjustSpeed(GetSpeed()));
        }
        SendMsg(&msg);
    }
    const bool bMaxLife = true;
    this->BroadcastTeamLife(bMaxLife);
}

//////////////////////////////////////////////////////////////////////
// modify attrib
//////////////////////////////////////////////////////////////////////
bool CUser::AddAttrib(int idxAttr, __int64 i64Data, int nSynchro)
{
    CMsgUserAttrib	msg;
    IF_NOT (msg.Create(GetID(), _USERATTRIB_NONE, 0))			// _USERATTRIB_NONE : 不添加
    return false;
    if (i64Data)
    {
        switch(idxAttr)
        {
        case	_USERATTRIB_LEV:
            {
                IF_NOT (i64Data >= 0)
                return false;
                int nOldLev = this->GetLev();
                int nNewLev = __min(MAX_USERLEV, nOldLev + i64Data);
                m_data.SetLev(nNewLev, true);
                IF_NOT (msg.Append(_USERATTRIB_LEV, this->GetLev()))
                return false;
                // 等级在70级上下发生变化，需要调整经验值存储格式
                if (nNewLev > ADJUST_EXPFORMAT_LEVEL && nOldLev <= ADJUST_EXPFORMAT_LEVEL)
                {
                    this->SetAttrib(_USERATTRIB_EXP, this->GetExp() / 10, nSynchro);
                }
                else if (nNewLev <= ADJUST_EXPFORMAT_LEVEL && nOldLev > ADJUST_EXPFORMAT_LEVEL)
                {
                    this->SetAttrib(_USERATTRIB_EXP, this->GetExp() * 10, nSynchro);
                }
            }
            break;
        case	_USERATTRIB_LIFE:
            {
                if ((int)this->GetLife() + i64Data <= 0)
                {
                    //this->SetStatus(STATUS_DIE);
                    SetLife(0);
                }
                else
                {
                    SetLife(__min(this->GetMaxLife(), this->GetLife() + i64Data));
                }
                // 血超过70%的时候解除狂暴状态
                if (GetLife() * 100 / GetMaxLife() > MAX_FRENZY_LIFE_PERCENT)
                {
                    CRole::DetachStatus(this->QueryRole(), STATUS_FRENZY);
                }
                IF_NOT (msg.Append(_USERATTRIB_LIFE, this->GetLife()))
                return false;
            }
            break;
        case	_USERATTRIB_MANA:
            {
                if ((int)this->GetMana() + i64Data <= 0)
                {
                    m_data.SetMana(0);
                }
                else
                {
                    m_data.SetMana(__min(this->GetMaxMana(), this->GetMana() + i64Data));
                }
                IF_NOT (msg.Append(_USERATTRIB_MANA, this->GetMana()))
                return false;
            }
            break;
        case	_USERATTRIB_EXP:
            {
                // exp allow negative
                m_data.SetExp(this->GetExp() + i64Data);
                IF_NOT (msg.Append(_USERATTRIB_EXP, this->GetExp()))
                return false;
            }
            break;
        case	_USERATTRIB_PK:
            {
                // pk allow negative
                m_data.SetPk(this->GetPk() + i64Data);
                IF_NOT (msg.Append(_USERATTRIB_PK, this->GetPk()))
                return false;
            }
            break;
        case	_USERATTRIB_MONEY:
            {
                if (i64Data < 0 && (int)this->GetMoney() + i64Data < 0)
                {
                    // if not enough money, return false only
                    return false;
                }
                else
                {
                    m_data.SetMoney(GetMoney() + i64Data);
                }
                IF_NOT (msg.Append(_USERATTRIB_MONEY, this->GetMoney()))
                return false;
            }
            break;
        case _USERATTRIB_ADDPOINT:
            {
                if (i64Data < 0)
                {
                    return false;	// 断言错。
                }
                m_data.SetAddPoint(m_data.GetAddPoint() + i64Data);
                IF_NOT (msg.Append(_USERATTRIB_ADDPOINT, this->GetAddPoint()))
                return false;
            }
            break;
        case _USERATTRIB_SOUL:
            {
                m_data.SetSoul(__max(0, m_data.GetSoul() + i64Data));
                IF_NOT (msg.Append(_USERATTRIB_SOUL, this->GetSoul()))
                return false;
            }
            break;
        case _USERATTRIB_HEALTH:
            {
                m_data.SetHealth(__max(0, m_data.GetHealth() + i64Data));
                IF_NOT (msg.Append(_USERATTRIB_HEALTH, this->GetHealth()))
                return false;
            }
            break;
        case _USERATTRIB_FORCE:
            {
                m_data.SetForce(__max(0, m_data.GetForce() + i64Data));
                IF_NOT (msg.Append(_USERATTRIB_FORCE, this->GetForce()))
                return false;
            }
            break;
        case _USERATTRIB_SPEED:
            {
                m_data.SetSpeed(__max(0, m_data.GetSpeed() + i64Data));
                IF_NOT (msg.Append(_USERATTRIB_SPEED, AdjustSpeed(this->GetSpeed())))
                return false;
            }
            break;
        case _USERATTRIB_DEX:
            {
                m_data.SetDexterity(__max(0, (int)m_data.GetDexterity() + i64Data));
                IF_NOT (msg.Append(_USERATTRIB_DEX, this->GetDex()))
                return false;
            }
            break;
        case _USERATTRIB_POTENTIAL:
            {
                m_data.SetPotential(__max(0, (int)this->GetPotential() + i64Data));
                IF_NOT (msg.Append(_USERATTRIB_POTENTIAL, this->GetPotential()))
                return false;
            }
            break;
        case _USERATTRIB_XP:
            {
                if ((int)i64Data != 0)
                {
                    m_dwXP = ::CutRange(m_dwXP + (int)i64Data, 0, MAX_USER_XP);
                    if (m_dwXP == MAX_USER_XP)
                    {
                        if (m_pMagic && m_pMagic->HaveXpMagic())
                        {
                            CMsgAction msgAction;
                            const int XPFULLSOUNDEFFECT = 1000000;
                            if (msgAction.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionSoundEffect, 1000000))
                            {
                                this->BroadcastRoomMsg(&msgAction, INCLUDE_SELF);
                            }
                        }
                    }
                    IF_NOT (msg.Append(_USERATTRIB_XP, m_dwXP))
                    return false;
                }
            }
            break;
        case _USERATTRIB_ENERGY:
            {
                AddEnergy(i64Data);
                IF_NOT (msg.Append(_USERATTRIB_ENERGY, this->GetEnergy()))
                return false;
#ifdef	PALED_DEBUG
                SendSysMsg("ENERGY: [%d]", GetEnergy());
#endif
            }
            break;
        case _USERATTRIB_MAXENERGY:
            {
                // no operation
                IF_NOT (msg.Append(_USERATTRIB_MAXENERGY, this->GetMaxEnergy()))
                return false;
            }
            break;
        case _USERATTRIB_MERCENARYEXP:
            {
                m_data.SetMercenaryExp(__max(0, (int)m_data.m_Info.dwMercenaryExp + i64Data));
                IF_NOT (msg.Append(_USERATTRIB_MERCENARYEXP, this->GetMercenaryExp()))
                return false;
            }
            break;
        case _USERATTRIB_MERCENARYRANK:
            {
                m_data.SetMercenaryRank(__max(0, (int)m_data.m_Info.ucMercenaryRank + i64Data));
                IF_NOT (msg.Append(_USERATTRIB_MERCENARYRANK, this->GetMercenaryRank()))
                return false;
            }
            break;
        case _USERATTRIB_NOBILITYRANK:
            {
                m_data.SetNobilityRank(__max(0, (int)m_data.GetNobilityRank() + i64Data));
                IF_NOT (msg.Append(_USERATTRIB_NOBILITYRANK, this->GetNobilityRank()))
                return false;
            }
            break;
        case _USERATTRIB_EXPLOIT:
            {
                m_data.SetExploit(__max(0, (int)m_data.GetExploit() + i64Data));
                IF_NOT (msg.Append(_USERATTRIB_EXPLOIT, this->GetExploit()))
                return false;
            }
            break;
        case _USERATTRIB_TUTOR_EXP:
            {
                m_data.SetTutorExp(this->GetTutorExp() + i64Data);
                IF_NOT (msg.Append(_USERATTRIB_TUTOR_EXP, this->GetTutorExp()))
                return false;
            }
            break;
        case _USERATTRIB_TUTOR_LEVEL:
            {
                m_data.SetTutorLevel(__max(0, (int)this->GetTutorLevel() + i64Data));
                IF_NOT (msg.Append(_USERATTRIB_TUTOR_LEVEL, this->GetTutorLevel()))
                return false;
            }
            break;
        case _USERATTRIB_MAXEUDEMON:
            {
                m_data.SetMedalSelect(__max(0, (int)this->GetMedalSelect() + i64Data));
                IF_NOT (msg.Append(_USERATTRIB_MAXEUDEMON, this->GetMedalSelect()))
                return false;
            }
            break;
        default:
            return false;
        }
    }
    if (nSynchro != SYNCHRO_FALSE)
    {
        switch(nSynchro)
        {
        case SYNCHRO_TRUE:
            this->SendMsg(&msg);
            break;
        case SYNCHRO_BROADCAST:
            this->BroadcastRoomMsg(&msg, INCLUDE_SELF);
            break;
        default:
            break;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::SetAttrib(int idxAttr, __int64 i64Data, int nSynchro)
{
    CMsgUserAttrib	msg;
    if (!msg.Create(GetID(), _USERATTRIB_NONE, 0))			// _USERATTRIB_NONE : 不添加
    {
        return;
    }
    switch(idxAttr)
    {
    case	_USERATTRIB_ENERGY:
        {
            this->SetEnergy(i64Data);
            if (!msg.Append(_USERATTRIB_ENERGY, this->GetEnergy()))
            {
                return;
            }
        }
        break;
    case	_USERATTRIB_MAXENERGY:
        {
            // no operation
            if (!msg.Append(_USERATTRIB_MAXENERGY, this->GetMaxEnergy()))
            {
                return;
            }
        }
        break;
    case	_USERATTRIB_PK:
        {
            m_data.SetPk(i64Data, true);
            if (!msg.Append(_USERATTRIB_PK, this->GetPk()))
            {
                return;
            }
        }
        break;
    case	_USERATTRIB_LIFE:
        {
            i64Data = __max(0, __min(this->GetMaxLife(), i64Data));
            SetLife(i64Data);
            // 血超过70%的时候解除狂暴状态
            if (GetLife() * 100 / GetMaxLife() > MAX_FRENZY_LIFE_PERCENT)
            {
                CRole::DetachStatus(this->QueryRole(), STATUS_FRENZY);
            }
            if (!msg.Append(_USERATTRIB_LIFE, GetLife()))
            {
                return;
            }
        }
        break;
    case _USERATTRIB_LEV:
        {
            int nOldLev = this->GetLev();
            int nNewLev = __max(0, __min(MAX_USERLEV, i64Data));
            m_data.SetLev(nNewLev);
            if (!msg.Append(_USERATTRIB_LEV, GetLev()))
            {
                return;
            }
            // 等级在70级上下发生变化，需要调整经验值存储格式
            if (nNewLev > ADJUST_EXPFORMAT_LEVEL && nOldLev <= ADJUST_EXPFORMAT_LEVEL)
            {
                this->SetAttrib(_USERATTRIB_EXP, this->GetExp() / 10, nSynchro);
            }
            else if (nNewLev <= ADJUST_EXPFORMAT_LEVEL && nOldLev > ADJUST_EXPFORMAT_LEVEL)
            {
                this->SetAttrib(_USERATTRIB_EXP, this->GetExp() * 10, nSynchro);
            }
        }
        break;
    case _USERATTRIB_ADDPOINT:
        {
            i64Data = __max(0, i64Data);
            m_data.SetAddPoint(i64Data);
            if (!msg.Append(_USERATTRIB_ADDPOINT, GetAddPoint()))
            {
                return;
            }
        }
        break;
    case	_USERATTRIB_MANA:
        {
            i64Data = __max(0, __min(this->GetMaxMana(), i64Data));
            m_data.SetMana(i64Data);
            if (!msg.Append(_USERATTRIB_MANA, GetMana()))
            {
                return;
            }
        }
        break;
    case _USERATTRIB_FORCE:
        {
            m_data.SetForce(__max(0, i64Data));
            if (!msg.Append(_USERATTRIB_FORCE, GetForce()))
            {
                return;
            }
        }
        break;
    case _USERATTRIB_SOUL:
        {
            m_data.SetSoul(__max(0, i64Data));
            if (!msg.Append(_USERATTRIB_SOUL, GetSoul()))
            {
                return;
            }
        }
        break;
    case _USERATTRIB_HEALTH:
        {
            m_data.SetHealth(__max(0, i64Data));
            if (!msg.Append(_USERATTRIB_HEALTH, GetHealth()))
            {
                return;
            }
        }
        break;
    case _USERATTRIB_SPEED:
        {
            m_data.SetSpeed(__max(0, i64Data));
            if (!msg.Append(_USERATTRIB_SPEED, AdjustSpeed(GetSpeed())))
            {
                return;
            }
        }
        break;
    case _USERATTRIB_DEX:
        {
            m_data.SetDexterity(__max(0, i64Data));
            if (!msg.Append(_USERATTRIB_DEX, GetDex()))
            {
                return;
            }
        }
        break;
    case _USERATTRIB_POTENTIAL:
        {
            m_data.SetPotential(__max(0, i64Data));
            if (!msg.Append(_USERATTRIB_POTENTIAL, GetPotential()))
            {
                return;
            }
        }
        break;
    case _USERATTRIB_XP:
        {
            m_dwXP = ::CutRange((int)i64Data, 0, MAX_USER_XP);
            IF_NOT (msg.Append(_USERATTRIB_XP, m_dwXP))
            return;
        }
        break;
    case	_USERATTRIB_EXP:
        {
            // exp allow negative
            m_data.SetExp(i64Data);
            if (!msg.Append(_USERATTRIB_EXP, GetExp()))
            {
                return;
            }
        }
        break;
    case	_USERATTRIB_MONEY:
        {
            m_data.SetMoney(__max(0, i64Data));
            if (!msg.Append(_USERATTRIB_MONEY, GetMoney()))
            {
                return;
            }
        }
        break;
    case	_USERATTRIB_KEEPEFFECT:
        {
            ResetEffect();		// reset all
            this->SetEffect(i64Data);
            if (!msg.Append(idxAttr, this->GetEffect()))
            {
                return;
            }
        }
        break;
    case	_USERATTRIB_HAIR:
        {
            m_data.SetHair(__max(0, i64Data));
            if (!msg.Append(idxAttr, m_data.GetHair()))
            {
                return;
            }
            // brocast any way
            if (nSynchro != SYNCHRO_FALSE)
            {
                nSynchro = SYNCHRO_BROADCAST;
            }
        }
        break;
    case	_USERATTRIB_PORFESSION:
        {
            m_data.SetProfession(i64Data, true);
            if (!msg.Append(idxAttr, m_data.GetProfession()))
            {
                return;
            }
        }
        break;
    case	_USERATTRIB_LOOKFACE:
        {
            // 不支持修改LOOKFACE属性
            if (!msg.Append(idxAttr, i64Data))
            {
                return;
            }
        }
        break;
    case	_USERATTRIB_SIZEADD:
        {
            // 不支持修改SIZEADD属性
            if (!msg.Append(idxAttr, i64Data))
            {
                return;
            }
        }
        break;
    case	_USERATTRIB_SUPERMAP_ORDER:
        {
            // 不支持修改SIZEADD属性
            if (!msg.Append(idxAttr, i64Data))
            {
                return;
            }
        }
        break;
    case	_USERATTRIB_MERCENARYEXP:
        {
            m_data.SetMercenaryExp(i64Data, true);
            if (!msg.Append(idxAttr, m_data.m_Info.dwMercenaryExp))
            {
                return;
            }
        }
        break;
    case	_USERATTRIB_MERCENARYRANK:
        {
            m_data.SetMercenaryRank(i64Data, true);
            if (!msg.Append(idxAttr, m_data.m_Info.ucMercenaryRank))
            {
                return;
            }
        }
        break;
    case _USERATTRIB_NOBILITYRANK:
        {
            m_data.SetNobilityRank(__max(0, i64Data));
            if (!msg.Append(idxAttr, m_data.GetNobilityRank()))
            {
                return;
            }
        }
        break;
    case	_USERATTRIB_EXPLOIT:
        {
            m_data.SetExploit(__max(0, i64Data), true);
            if (!msg.Append(idxAttr, m_data.GetExploit()))
            {
                return;
            }
        }
        break;
    case	_USERATTRIB_ACTIONSPEED:
        {
            // TODO:
        }
        break;
    case	_USERATTRIB_TUTOR_EXP:
        {
            m_data.SetTutorExp(i64Data);
            IF_NOT (msg.Append(_USERATTRIB_TUTOR_EXP, this->GetTutorExp()))
            return ;
        }
        break;
    case	_USERATTRIB_TUTOR_LEVEL:
        {
            m_data.SetTutorLevel(__max(0, i64Data));
            IF_NOT (msg.Append(_USERATTRIB_TUTOR_LEVEL, this->GetTutorLevel()))
            return ;
        }
        break;
    case _USERATTRIB_MAXEUDEMON:
        {
            m_data.SetMedalSelect(__max(0, i64Data));
            IF_NOT (msg.Append(_USERATTRIB_MAXEUDEMON, this->GetMedalSelect()))
            return ;
        }
        break;
    default:
        return;
    }
    switch(nSynchro)
    {
    case SYNCHRO_TRUE:
        this->SendMsg(&msg);
        break;
    case SYNCHRO_BROADCAST:
        this->BroadcastRoomMsg(&msg, INCLUDE_SELF);
        break;
    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////
// return false mean fatal syn error.
bool CUser::SynPosition(int nPosX, int nPosY, int nMaxDislocation/* = 8*/)
{
    if (nMaxDislocation <= 0 || (nPosX == 0 && nPosY == 0))	// ignore in this condition
    {
        return true;
    }
    int nDislocation = this->GetDistance(nPosX, nPosY);
    if (nDislocation >= nMaxDislocation)
    {
        return false;
    }
    if (nDislocation <= 0)
    {
        return true;
    }
    if (this->IsGM())
        this->SendSysMsg("syn move: (%u, %u)->(%u, %u)",
                         this->GetPosX(), this->GetPosY(),
                         nPosX, nPosY);
    CMapPtr pMap = this->GetMap();
    IF_NOT(pMap && pMap->IsValidPoint(nPosX, nPosY))
    return false;
    // correct pos
    CMsgAction msg;
    if (msg.Create(this->GetID(), nPosX, nPosY, this->GetDir(), actionSynchro, nPosX, nPosY))
    {
        this->BroadcastRoomMsg(&msg, EXCLUDE_SELF);
    }
    // process move
    this->ProcessOnMove(MOVEMODE_SYNCHRO);
    this->JumpPos(nPosX, nPosY);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcessAfterMove()
{
    IThingSet* pSet = GetMap()->QueryBlock(GetPosX(), GetPosY()).QuerySet();
    for(int i = 0; i < pSet->GetAmount(); i++)
    {
        IMapThing* pTarget = pSet->GetObjByIndex(i);
        CMapTrap* pTrap;
        if (pTarget && pTarget->QueryObj(OBJ_TRAP, IPP_OF(pTrap))
                && pTrap->IsTrapSort() && pTarget->GetPosX() == GetPosX() && pTarget->GetPosY() == GetPosY())
        {
            pTrap->TrapAttack(QueryRole());
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcessOnMove(int nMoveMode)
{
    if (QueryBooth())
    {
        QueryBooth()->LeaveMap();
    }
    if (nMoveMode != MOVEMODE_TRACK)
    {
        SetPose(_ACTION_STANDBY);
    }
    // stop fight
    if (nMoveMode != MOVEMODE_TRACK && nMoveMode != MOVEMODE_JUMPMAGICATTCK
            && nMoveMode != MOVEMODE_COLLIDE && nMoveMode != MOVEMODE_SYNCHRO)
    {
        ClrAttackTarget();
    }
    if (nMoveMode == MOVEMODE_RUN || nMoveMode == MOVEMODE_JUMP || nMoveMode == MOVEMODE_WALK
            || (nMoveMode >= MOVEMODE_RUN_DIR0 && nMoveMode <= MOVEMODE_RUN_DIR7))		// && QueryMagic()->IsKeepBow()
    {
        if (QueryMagic())
        {
            QueryMagic()->AbortMagic(true);
        }
        // run
        if (nMoveMode == MOVEMODE_RUN || (nMoveMode >= MOVEMODE_RUN_DIR0 && nMoveMode <= MOVEMODE_RUN_DIR7))
        {
#ifdef	PALED_DEBUG
            if (!m_bRun)
            {
                SendSysMsg("RUN");
            }
#endif
            m_bRun	= true;
        }
        else
        {
            ResetEnergyInc();
        }
    }
    //	if (nMoveMode != MOVEMODE_SHIFT && nMoveMode != MOVEMODE_SYNCHRO)
    //		QueryStatusSet()->DelObj(STATUS_WEAPONDAMAGE);
    //	if (nMoveMode != MOVEMODE_SHIFT && nMoveMode != MOVEMODE_SYNCHRO)
    //		QueryStatusSet()->DelObj(STATUS_KEEPBOW);
    // stop mine
    this->StopMine();
    // foot print
    m_tFootPrint = ::TimeGet();
    m_posFootPrint.x = this->GetPosX();
    m_posFootPrint.y = this->GetPosY();
    // unlock
    CRole::DetachStatus(this->QueryRole(), STATUS_PK_PROTECT);
    //m_tLock.Clear();
}


//////////////////////////////////////////////////////////////////////
void CUser::ProcessOnAttack()
{
    CRole::DetachStatus(this->QueryRole(), STATUS_PK_PROTECT);
    //m_tLock.Clear();
}
//////////////////////////////////////////////////////////////////////
void CUser::GetFootPrint	(int& nPosX, int& nPosY)
{
    if (::TimeGet() - m_tFootPrint >= TIME_FOOTPRINT)
    {
        // time out
        nPosX = this->GetPosX();
        nPosY = this->GetPosY();
    }
    else
    {
        nPosX = m_posFootPrint.x;
        nPosY = m_posFootPrint.y;
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::SetLife(int nLife, BOOL bUpdate)
{
    int nOldLife = this->GetLife();
    if (QueryTransformation())
    {
        if (nLife > 0)
        {
            QueryTransformation()->SetLife(nLife);
            // recalc user life
            nLife = ::CutTrail(1, MulDiv(nLife, GetMaxLife(), QueryTransformation()->GetMaxLife()));
        }
        else
        {
            SAFE_RELEASE(m_pTransformation);
            SynchroTransform();
        }
    }
    m_data.SetLife(nLife, bUpdate);
    // 调整移动速度
    IStatus* pStatus = this->QueryStatus(STATUS_SLOWDOWN2);
    if (pStatus)
    {
        if ((nOldLife * 2 < this->GetMaxLife() && this->GetLife() * 2 >= this->GetMaxLife())
                || (nOldLife * 2 >= this->GetMaxLife() && this->GetLife() * 2 < this->GetMaxLife()))
        {
            CMsgUserAttrib msg;
            if (msg.Create(this->GetID(), _USERATTRIB_SPEED, AdjustSpeed(this->GetSpeed())))
            {
                this->BroadcastRoomMsg(&msg, INCLUDE_SELF);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
// get attrib
//////////////////////////////////////////////////////////////////////
DWORD CUser::GetLife()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetLife();
    }
    int nMaxLife = this->GetMaxLife();
    if (m_data.GetLife() > nMaxLife)
    {
        m_data.SetLife(nMaxLife);
    }
    return m_data.GetLife();
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetMaxLife()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetMaxLife();
    }
    //int nMaxLife = (this->GetLev()-1) * 3 + 30;	// 没有等级、职业加成
    /*switch(this->GetProfessionSort())
    {
    case PROFSORT_SOLDIER:
    	nMaxLife += m_data.GetHealth() * 5;
    	break;
    case PROFSORT_ARCHER:
    	nMaxLife += m_data.GetHealth() * 7;
    	break;
    case PROFSORT_MAGICIAN:
    	nMaxLife += m_data.GetHealth() * 6;
    	break;
    }*/
    int nMaxLife = m_data.GetHealth() * 10;	// 基础生命 = 体质 * 10;	// 体质就是耐力
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItemPtr* ppEquip = GetEquipItemPtr(i);
        CHECKF(ppEquip);
        CItemPtr& pEquip = *ppEquip;
        if (pEquip)
        {
            nMaxLife += pEquip->GetInt(ITEMDATA_LIFE);
        }
    }
    nMaxLife	= nMaxLife * m_data.GetMaxLifePercent() / 1000;		// 因为有个受伤度的问题，这里需要调整最大血量
    IStatus* pStatus = QueryStatus(STATUS_MAXLIFE);
    if (pStatus)
    {
        nMaxLife	= ::CutTrail(0, AdjustData(nMaxLife, pStatus->GetPower(), 100));
    }
    // 防御型幻兽合体增加最大生命
    if (m_idLinkEudemonType == ITEMTYPE_EUDEMON_DEF)
    {
        nMaxLife += m_nLinkValue;
    }
    return __max(0, nMaxLife);
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetMaxMana()
{
    /*int nMaxMana = 0;
    switch(this->GetProfessionSort())
    {
    case PROFSORT_SOLDIER:
    	nMaxMana += GetSoulSum()*5 + (GetLev()-1)*3;
    	break;

    case PROFSORT_ARCHER:
    	nMaxMana += GetSoulSum()*5 + (GetLev()-1)*3;
    	break;

    case PROFSORT_MAGICIAN:
    	nMaxMana += GetSoulSum()*5 + (GetLev()-1)*3;
    	break;

    }*/
    int nMaxMana = GetSoulSum() * 20;
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItemPtr* ppEquip = GetEquipItemPtr(i);
        CHECKF(ppEquip);
        CItemPtr& pEquip = *ppEquip;
        if (pEquip)
        {
            nMaxMana += pEquip->GetInt(ITEMDATA_MANA);
        }
    }
    return __max(0, nMaxMana);
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetAttack()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetAttack();
    }
    /*float fAtk = 0.0f;
    switch(this->GetProfessionSort())
    {
    case PROFSORT_SOLDIER:
    	fAtk += m_data.GetForce()*1.0f;
    	break;

    case PROFSORT_ARCHER:
    	fAtk += m_data.GetSpeed()*0.5f;
    	break;

    case PROFSORT_MAGICIAN:
    	fAtk += m_data.GetForce()*0.5f;
    	break;
    }

    return (fAtk+0.5f);*/
    return m_data.GetForce();
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetMagicAttack()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetAttack();
    }
    /*float fAtk = 0.0f;
    switch(this->GetProfessionSort())
    {
    case PROFSORT_MAGICIAN:
    	fAtk += GetSoulSum()*0.5f;
    	break;
    }

    return (fAtk+0.5f);*/
    return GetSoulSum();
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetAtkHitRate()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetHitRate();
    }
    int nAtkHitRate = 0;
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItemPtr* ppEquip = GetEquipItemPtr(i);
        CHECKF(ppEquip);
        CItemPtr& pEquip = *ppEquip;
        if (pEquip)
        {
            nAtkHitRate += pEquip->GetHitRate();
        }
    }
    return nAtkHitRate;
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetMinAtk()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetMinAtk();
    }
    int nMinAtk	= this->GetAttack();
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItemPtr* ppEquip = GetEquipItemPtr(i);
        CHECKF(ppEquip);
        CItemPtr& pEquip = *ppEquip;
        if (pEquip)
        {
            int nEquipAtk = pEquip->GetMinAtk();
            if (ITEMPOSITION_WEAPONL == i && !pEquip->IsShield())
            {
                nEquipAtk = nEquipAtk / 2;
            }
            else if (ITEMPOSITION_SPRITE == i)	// 精灵存储的附加属性是*100以后的值
            {
                nEquipAtk = nEquipAtk / 100;
            }
            nMinAtk += nEquipAtk;
        }
    }
    return __max(0, nMinAtk);
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetMaxAtk()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetMaxAtk();
    }
    int nMaxAtk	= this->GetAttack();
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItemPtr* ppEquip = GetEquipItemPtr(i);
        CHECKF(ppEquip);
        CItemPtr& pEquip = *ppEquip;
        if (pEquip)
        {
            int nEquipAtk = pEquip->GetMaxAtk();
            if (ITEMPOSITION_WEAPONL == i && !pEquip->IsShield())
            {
                nEquipAtk = nEquipAtk / 2;
            }
            else if (ITEMPOSITION_SPRITE == i)	// 精灵存储的附加属性是*100以后的值
            {
                nEquipAtk = nEquipAtk / 100;
            }
            nMaxAtk += nEquipAtk;
        }
    }
    return __max(0, nMaxAtk);
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetMgcMinAtk()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetMgcMinAtk();
    }
    int nMinAtk	= this->GetMagicAttack();
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItemPtr* ppEquip = GetEquipItemPtr(i);
        CHECKF(ppEquip);
        CItemPtr& pEquip = *ppEquip;
        if (pEquip)
        {
            int nEquipAtk = pEquip->GetMgcMinAtk();
            if (ITEMPOSITION_WEAPONL == i && !pEquip->IsShield())
            {
                nEquipAtk = nEquipAtk / 2;
            }
            else if (ITEMPOSITION_SPRITE == i)	// 精灵存储的附加属性是*100以后的值
            {
                nEquipAtk = nEquipAtk / 100;
            }
            nMinAtk += nEquipAtk;
        }
    }
    return __max(0, nMinAtk);
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetMgcMaxAtk()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetMaxAtk();
    }
    int nMaxAtk	= this->GetMagicAttack();
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItemPtr* ppEquip = GetEquipItemPtr(i);
        CHECKF(ppEquip);
        CItemPtr& pEquip = *ppEquip;
        if (pEquip)
        {
            int nEquipAtk = pEquip->GetMgcMaxAtk();
            if (ITEMPOSITION_WEAPONL == i && !pEquip->IsShield())
            {
                nEquipAtk = nEquipAtk / 2;
            }
            else if (ITEMPOSITION_SPRITE == i)	// 精灵存储的附加属性是*100以后的值
            {
                nEquipAtk = nEquipAtk / 100;
            }
            nMaxAtk += nEquipAtk;
        }
    }
    return __max(0, nMaxAtk);
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetDefence()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetDefence();
    }
    /*float fDef = 0.0f;
    switch(this->GetProfessionSort())
    {
    case PROFSORT_SOLDIER:
    	fDef += m_data.GetHealth()*0.5f;
    	break;

    case PROFSORT_ARCHER:
    	fDef += m_data.GetHealth()*0.5f;
    	break;

    case PROFSORT_WIZARD:
    	fDef += m_data.GetHealth()*1.5f;
    	break;

    case PROFSORT_MAGICIAN:
    	fDef += m_data.GetHealth()*0.5f;
    	break;
    }

    return (fDef+0.5f);*/
    return 0;
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetDef()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetDef();
    }
    int nDef = this->GetDefence();
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItemPtr* ppEquip = GetEquipItemPtr(i);
        CHECKF(ppEquip);
        CItemPtr& pEquip = *ppEquip;
        if (pEquip)
        {
            int nEquipDef = pEquip->GetDef();
            if (ITEMPOSITION_SPRITE == i)
            {
                nEquipDef = nEquipDef / 100;    // 精灵存储的附加属性是*100以后的值
            }
            nDef += nEquipDef;
        }
    }
    return __max(0, nDef);
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetSoulSum()
{
    int nData = this->GetSoul();
    /*
    	for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    	{
    		CItem* pEquip = this->GetEquipItemByPos(i);
    		if (pEquip)
    			nData += pEquip->GetInt(ITEMDATA_ADD_SOUL);
    	}
    */
    return __max(0, nData);
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetDefence2()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetDefence2();
    }
    return (GetMetempsychosis() && this->GetProfessionLevel() >= 3) ? 7000 : DEFAULT_DEFENCE2;		// 3: 70级
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetDexterity()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetDexterity();
    }
    return m_data.GetDexterity();
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetDex()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetDex();
    }
    int nDex = this->GetDexterity();
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItem* pEquip = this->GetEquipItemByPos(i);
        if (pEquip)
        {
            nDex += pEquip->GetDexteriy();
        }
    }
    return nDex;
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetDodge()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetDodge();
    }
    //return 0;
    return GetDexterity();
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetDdg()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetDdg();
    }
    int nDge = this->GetDodge();
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItemPtr* ppEquip = GetEquipItemPtr(i);
        CHECKF(ppEquip);
        CItemPtr& pEquip = *ppEquip;
        if (pEquip)
        {
            nDge +=	pEquip->GetDodge();    //pEquip->GetInt(ITEMDATA_DODGE);
        }
    }
    //---jinggy---begin
    //玩家特殊状态调节 dodge 值
    IStatus* pStatus = QueryStatus(STATUS_ADJUST_DODGE);
    if (pStatus)
    {
        nDge = ::CutTrail(0, AdjustData(nDge, pStatus->GetPower()));
    }
    //---jinggy---end
    // 与附身型幻兽合体提高躲避率
    if (m_idLinkEudemonType == ITEMTYPE_EUDEMON_ATTACH)
    {
        nDge += m_nLinkValue;
    }
    return __max(0, nDge);
}

//////////////////////////////////////////////////////////////////////
int	 CUser::GetLuck()
{
    //	if (QueryTransformation())
    //		return QueryTransformation()->GetDdg();
    int nLuck = 0;
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItem* pEquip = GetEquipItemByPos(i);
        if (pEquip)
        {
            nLuck += pEquip->GetInt(ITEMDATA_LUCK);
        }
    }
    ASSERT(nLuck <= 35);
    return __max(0, nLuck);
}

//////////////////////////////////////////////////////////////////////
/*DWORD CUser::GetMagicAtk()		//? 可优化
{
	if (QueryTransformation())
		return QueryTransformation()->GetMagicAtk();

	float fAtk = 0.0f;
	switch(this->GetProfessionSort())
	{
	case PROFSORT_SOLDIER:
		break;

	case PROFSORT_ARCHER:
		break;

	case PROFSORT_WIZARD:
		fAtk += m_data.GetSoul()*0.5f;
		break;

	case PROFSORT_MAGICIAN:
		fAtk += m_data.GetSoul()*0.5f;
		break;
	}

	for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
	{
		CItemPtr* ppEquip = GetEquipItemPtr(i);
		CHECKF(ppEquip);
		CItemPtr& pEquip = *ppEquip;
		if (pEquip)
		{
			fAtk += pEquip->GetInt(ITEMDATA_MAGICATK);
		}
	}

	return __max(0, fAtk+0.5);
}*/

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetMagicDef()		//? 可优化
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetMagicDef();
    }
    float fDef = 0.0f;
    /*switch(this->GetProfessionSort())
    {
    case PROFSORT_SOLDIER:
    	fDef += m_data.GetHealth()*0.5f;
    	break;

    case PROFSORT_ARCHER:
    	fDef += GetSoulSum()*0.5f;
    	break;

    case PROFSORT_MAGICIAN:
    	fDef += GetSoulSum()*0.5f;
    	break;
    }*/
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItemPtr* ppEquip = GetEquipItemPtr(i);
        CHECKF(ppEquip);
        CItemPtr& pEquip = *ppEquip;
        if (pEquip)
        {
            int nEquipDef = pEquip->GetMgcDef();
            if (ITEMPOSITION_SPRITE == i)
            {
                nEquipDef = nEquipDef / 100;    // 精灵存储的附加属性是*100以后的值
            }
            fDef += nEquipDef;
        }
    }
    fDef += 0.5f;
    //	fDef = __min(90.0f, fDef);
    //	fDef = __max(0.0f, fDef);
    return fDef;
}

//////////////////////////////////////////////////////////////////////
int CUser::GetAddPoint()
{
    return m_data.GetAddPoint();
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetLev()
{
    return m_data.GetLev();
}

//////////////////////////////////////////////////////////////////////
void CUser::IncLev(int nLev)
{
    int nOldLev = this->GetLev();
    int nNewLev	= nOldLev + nLev;
    this->SetAttrib(_USERATTRIB_LEV, nNewLev, SYNCHRO_TRUE);
    // start pay point fee?
    if (nOldLev < LOGIN_FREE_LEVEL && nNewLev >= LOGIN_FREE_LEVEL)
    {
        MapGroup(PID)->QueryIntraMsg()->LevelUp(GetID(), nNewLev);
    }
    if (QueryMagic())
    {
        QueryMagic()->OwnerUpLevel(nNewLev);
    }
    if (GetSynID() != ID_NONE)
    {
        QuerySynAttr()->UpLevel(nNewLev);
        if (QuerySynAttr()->GetSynRank() == RANK_RESERVED && GetLev() >= SYN_CONDITION_NEWBIE)
        {
            QuerySynAttr()->SetRank(RANK_NEWBIE);
        }
    }
    // tutor exp record
    if (this->GetLev() >= _MIN_TUTOR_LEVEL)
    {
        CTutorexpData*	pExpData;
        pExpData = CTutorexpData::CreateNew();
        if (pExpData && pExpData->Create(GetID(), Database()))
        {
            if (pExpData->GetID() == ID_NONE)
            {
                pExpData->SetInt(TUTOREXPDATA_ID, this->GetID());
                pExpData->SetInt(TUTOREXPDATA_TUTOR_LEV, this->GetLev());
                pExpData->InsertRecord();
            }
            else
            {
                if (pExpData->GetInt(TUTOREXPDATA_TUTOR_LEV) != this->GetLev())
                {
                    pExpData->SetInt(TUTOREXPDATA_TUTOR_LEV, this->GetLev());
                    pExpData->Update();
                }
            }
        }
        SAFE_RELEASE (pExpData);
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::AllotPoint()
{
    int nProf = this->GetProfessionSort();
    if (nProf > 10)
    {
        nProf = (nProf / 10) * 10;
    }
    int nAmount = AddPointSet()->GetAmount();
    for(int i = 0; i < nAmount; i++)
    {
        CAddPointData* pData = AddPointSet()->GetObjByIndex(i);
        if (pData
                && pData->GetInt(ADDPOINTDATA_PROF) == nProf
                && pData->GetInt(ADDPOINTDATA_LEVEL) == this->GetLev())
        {
            m_data.SetForce(pData->GetInt(ADDPOINTDATA_FORCE));
            m_data.SetHealth(pData->GetInt(ADDPOINTDATA_HEALTH));
            m_data.SetSoul(pData->GetInt(ADDPOINTDATA_SOUL));
            m_data.SetDexterity(pData->GetInt(ADDPOINTDATA_DEX), UPDATE_TRUE);
            {
                CMsgUserAttrib	msg;
                if (msg.Create(GetID(), _USERATTRIB_FORCE, GetForce())
                        && msg.Append(_USERATTRIB_DEX, GetDex())
                        && msg.Append(_USERATTRIB_HEALTH, GetHealth())
                        && msg.Append(_USERATTRIB_SOUL, GetSoul())	)
                {
                    SendMsg(&msg);
                }
            }
            return ;
        }
    }
    // lev data not found
    this->AddAttrib(_USERATTRIB_ADDPOINT, _ADDITIONALPOINT_NUM, SYNCHRO_TRUE);
    return;
}

//////////////////////////////////////////////////////////////////////
int CUser::GetPose()
{
    return m_nPose;
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsAllNonsuchEquip (void)
{
    CItemPtr pWeaponR = NULL;
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        if (i == ITEMPOSITION_RINGL)
        {
            continue;
        }
        CItem* pEquip = GetEquipItemByPos(i);
        if (i == ITEMPOSITION_WEAPONR)
        {
            pWeaponR = pEquip;
        }
        if (!pEquip)
        {
            if (i == ITEMPOSITION_WEAPONL)
            {
                switch(this->GetProfessionSort())
                {
                case PROFSORT_SOLDIER:
                    {
                        if (this->GetLev() >= 40)
                        {
                            if (pWeaponR && !pWeaponR->IsWeapon2())
                            {
                                return false;
                            }
                        }
                    }
                    break;
                default:
                    break;
                }
                continue;
            }
            else
            {
                return false;
            }
        }
        if (!pEquip->IsNonsuch())
        {
            return false;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
I64 CUser::GetEffect()
{
    return m_data.m_Info.i64Effect;
}

//////////////////////////////////////////////////////////////////////
int CUser::GetDir()
{
    return m_nDir;
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetLookFace()
{
    enum {LOOK_MALEGHOST = 98, LOOK_FEMALEGHOST};
    if (IsGhost())
    {
        if (SEX_MAN == this->GetSex())
        {
            return m_data.GetLookFace() + MASK_CHANGELOOK * LOOK_MALEGHOST;
        }
        else
        {
            return m_data.GetLookFace() + MASK_CHANGELOOK * LOOK_FEMALEGHOST;
        }
    }
    else if (QueryTransformation())
    {
        return m_data.GetLookFace()
               + MASK_CHANGELOOK * QueryTransformation()->GetLook();
    }
    else
    {
        return m_data.GetLookFace();
    }
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetWeightLimit()
{
    if (this->GetProfessionSort() == PROFSORT_SOLDIER)
    {
        return 30 - 3 + 3 * GetLev();
    }
    else
    {
        return 20 - 2 + 2 * GetLev();
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool CUser::LeaveMap()
{
    if (m_nStep != STEP_ENTERMAP)
    {
        return false;
    }
    StopMine();
    DestroyBooth();
    KillCallPet();
    CallBackAllEudemon();
    CMapPtr pMap = GetMap();
    if (pMap)
    {
        // BeKill的时候调用了DecRole，因此这里需要判断是否为死亡状态，以免LeaveRoom的时候多减一次角色数
        pMap->LeaveRoom(QueryMapThing(), this->IsAlive());	//WITH_BLOCK);
        StandTurnoff();
        pMap->ClearRegionInfo(this);
        // brocast my info
        CMsgAction msg;
        if (msg.Create(GetID(), 0, 0, 0, actionLeaveMap))
        {
            this->BroadcastRoomMsg(&msg, EXCLUDE_SELF);
        }
        ClrBroadcastSet();
    }
    m_nStep = STEP_LOGIN;
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::SendItemSet()
{
    SendAllItemInfo();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::SendGoodFriend()
{
    SendFriendInfo();
    return true;
}

//////////////////////////////////////////////////////////////////////
// fight
//////////////////////////////////////////////////////////////////////
bool CUser::SetAttackTarget(IRole* pTarget /*= NULL*/)
{
    if (!m_pBattleSystem)
    {
        return false;
    }
    if (pTarget == NULL)
    {
        m_pBattleSystem->ResetBattle();
        return true;
    }
    if (pTarget->QueryOwnerUser() && GetMap()->IsInRegionType(REGION_PK_PROTECTED, GetPosX(), GetPosY()))
    {
        return false;
    }
    if (!this->IsAtkable(pTarget, true))
    {
        return false;
    }
    if (pTarget->IsWing() && !this->IsWing() && !(this->IsBowman() || this->IsSimpleMagicAtk()))
    {
        return false;
    }
    // goal
    CNpc* pNpc;
    pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc));
    if (pNpc && pNpc->IsGoal() && (GetLev() < pNpc->GetLev() || pNpc->GetType() != _WEAPONGOAL_NPC))
    {
        return false;
    }
    if (this->GetDistance(pTarget->QueryMapThing()) > GetAttackRange(pTarget->GetSizeAdd()))
    {
        m_pBattleSystem->ResetBattle();
        return false;
    }
    CheckCrime(pTarget);
    if (IsBowman() && !IsArrowPass(pTarget->GetPosX(), pTarget->GetPosY()))
    {
        return false;
    }
    m_pBattleSystem->CreateBattle(pTarget->GetID());
    CalcFightRate();
    if (QueryMagic())
    {
        QueryMagic()->AbortMagic(true);
    }
    // unequip magic arrow, hard code@@@
    CItem* pEquipL = GetEquipItemByPos(ITEMPOSITION_WEAPONL);
    if (!pEquipL || pEquipL->GetInt(ITEMDATA_MAGIC1) == MAGIC_NONE)
    {
        UnequipMagicItem(ITEMPOSITION_WEAPONL);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
int CUser::GetGemMgcExpEffect()
{
    int nAddPercent = 0;
    for (int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItem* pEquip = GetEquipItemByPos(i);
        if (pEquip)
        {
            nAddPercent += pEquip->GetGemMgcExpEffect();
        }
    }
    return nAddPercent;
}

//////////////////////////////////////////////////////////////////////
int CUser::GetGemWpnExpEffect()
{
    int nAddPercent = 0;
    for (int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItem* pEquip = GetEquipItemByPos(i);
        if (pEquip)
        {
            nAddPercent += pEquip->GetGemWpnExpEffect();
        }
    }
    return nAddPercent;
}

//////////////////////////////////////////////////////////////////////
int CUser::GetGemExpEffect()
{
    int nAddPercent = 0;
    for (int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItem* pEquip = GetEquipItemByPos(i);
        if (pEquip)
        {
            nAddPercent += pEquip->GetGemExpEffect();
        }
    }
    return nAddPercent;
}
//////////////////////////////////////////////////////////////////////
int	CUser::GetGemAtkEffect		(void)
{
    int nAddEffect = 0;
    for (int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItem* pEquip = GetEquipItemByPos(i);
        if (pEquip)
        {
            nAddEffect += pEquip->GetGemDmgEffect();
        }
    }
    return nAddEffect;
}

//////////////////////////////////////////////////////////////////////
int CUser::GetGemMgcAtkEffect	(void)
{
    int nAddEffect = 0;
    for (int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItem* pEquip = GetEquipItemByPos(i);
        if (pEquip)
        {
            nAddEffect += pEquip->GetGemMgcAtkEffect();
        }
    }
    return nAddEffect;
}

//////////////////////////////////////////////////////////////////////
void CUser::SendGemEffect(void)
{
    vector<int> setGem;
    int nGem = 0;
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItem* pEquip = GetEquipItemByPos(i);
        if (pEquip)
        {
            nGem = pEquip->GetInt(ITEMDATA_GEM1);
            if (nGem != GEM_NONE && nGem != GEM_HOLE)
            {
                setGem.push_back(nGem);
            }
            nGem = pEquip->GetInt(ITEMDATA_GEM2);
            if (nGem != GEM_NONE && nGem != GEM_HOLE)
            {
                setGem.push_back(nGem);
            }
        }
    }
    int nGems = setGem.size();
    if (nGems <= 0)
    {
        return;
    }
    string strEffect;
    switch(setGem[::RandGet(nGems)])
    {
    case 3:
        strEffect = "phoenix";
        break;
    case 13:
        strEffect = "goldendragon";
        break;
    case 23:
        strEffect = "fastflash";
        break;
    case 33:
        strEffect = "rainbow";
        break;
    case 43:
        strEffect = "goldenkylin";
        break;
    case 53:
        strEffect = "purpleray";
        break;
    case 63:
        strEffect = "moon";
        break;
    default:
        return;
    }
    CMsgName msg;
    IF_OK (msg.Create(NAMEACT_ROLEEFFECT, strEffect.c_str(), this->GetID()))
    this->BroadcastRoomMsg(&msg, true);
}

//////////////////////////////////////////////////////////////////////
// precondition is dodge failed
int CUser::Attack(IRole* pTarget)		// return : lose life
{
    CHECKF(pTarget);
    CNpc* pNpc;
    pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc));
    int nAtk = 0;
    int nDamage = CBattleSystem::CalcPower(IsSimpleMagicAtk(), QueryRole(), pTarget, &nAtk, true);
    // 目标护盾状态转移伤害
    if (nDamage > 0 && pTarget->TransferShield(IsSimpleMagicAtk(), QueryRole(), nDamage))
    {
    }
    else
    {
        int nLoseLife = ::CutOverflow(nDamage, (int)pTarget->GetLife());
        if (nLoseLife > 0)
        {
            pTarget->AddAttrib(_USERATTRIB_LIFE, -1 * nLoseLife, SYNCHRO_TRUE);
        }
        // adjust synflag damage
        if (pNpc && pNpc->IsSynFlag() && pNpc->IsSynMoneyEmpty())
        {
            nDamage	*= SYNWAR_NOMONEY_DAMAGETIMES;
        }
        if (nDamage > 0)
        {
            if (::RandGet(10) == 7)
            {
                this->SendGemEffect();
            }
        }
        pTarget->BeAttack(IsSimpleMagicAtk(), QueryRole(), nDamage);
    }
    CGameMap* pMap = this->GetMap();
    if (pMap)
    {
        // crime
        if (!pTarget->IsEvil()				// 只有pk白名才闪蓝
                && !pMap->IsDeadIsland())	// 死亡岛杀人不闪蓝
        {
            if (!QueryStatus(STATUS_CRIME))
            {
                this->SetCrimeStatus();
            }
        }
        // equipment durability cost
        if (!pMap->IsTrainMap())
        {
            this->DecEquipmentDurability(ATTACK_TIME, IsSimpleMagicAtk(), (nDamage < nAtk / 10) ? 10 : 1);
        }
    }
    IStatus* pStatus = QueryStatus(STATUS_DMG2LIFE);
    if (pStatus)
    {
        int nLifeGot = ::CutTrail(0, MulDiv(nDamage, pStatus->GetPower(), 100));
        if (nLifeGot > 0)
        {
            this->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
        }
    }
    return nDamage;
}

//////////////////////////////////////////////////////////////////////
bool CUser::BeAttack(bool bMagic, IRole* pTarget, int nPower, bool bReflectEnable/*=true*/)
{
    CHECKF(pTarget);
    QueryStatusSet()->DelObj(STATUS_LURKER);
    CRole::DetachStatus(this->QueryRole(), STATUS_FREEZE);
    StopMine();
    // reflect, only use for weapon attack!!!
    IStatus* pStatus = QueryStatus(bMagic ? STATUS_REFLECTMAGIC : STATUS_REFLECT);
    if (nPower > 0 && pStatus && bReflectEnable)
    {
        int nPower2 = AdjustData(nPower, pStatus->GetPower());
        if (nPower2 > 0)
        {
            int nLoseLife = ::CutOverflow((int)pTarget->GetLife(), nPower2);
            pTarget->AddAttrib(_USERATTRIB_LIFE, -1 * nLoseLife, SYNCHRO_TRUE);
            pTarget->BeAttack(bMagic, this, nPower2, false);
            // 传送结果消息
            CMsgInteract msg;
            if (msg.Create(bMagic ? INTERACT_REFLECTMAGIC : INTERACT_REFLECTWEAPON, GetID(), pTarget->GetID(), GetPosX(), GetPosY(), nPower2))
            {
                BroadcastRoomMsg(&msg, INCLUDE_SELF);
            }
            // kill?
            if (!pTarget->IsAlive())
            {
                pTarget->BeKill();
            }
        }
    }
    // equipment durability cost
    if (!this->GetMap()->IsTrainMap())
    {
        this->DecEquipmentDurability(BEATTACK_TIME, bMagic, (nPower > this->GetMaxLife() / 4) ? 10 : 1);
    }
    // self_defence     // add huang 2004.1.14
    //	if (QueryStatus(STATUS_SELFDEFENCE))
    //		this->SetSelfDefStatus();
    if (nPower > 0)
    {
        BroadcastTeamLife();
    }
    // abort magic
    if (QueryMagic() && QueryMagic()->IsIntone())
    {
        QueryMagic()->AbortMagic(true);
    }
    /*/ stamina lost
    if (_ACTION_SITDOWN ==  this->GetPose())
    {
    	this->SetPose(_ACTION_STANDBY);
    	this->SetAttrib(_USERATTRIB_ENERGY, this->GetEnergy()/2, SYNCHRO_TRUE);
    }*/
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::SendDamageMsg(OBJID idTarget, int nDamage)
{
    OBJID idMagicType = ID_NONE;
    if (QueryTransformation())
    {
        idMagicType = QueryTransformation()->GetMagicType();
    }
    if (idMagicType != ID_NONE)
    {
        CMsgMagicEffect	msg;
        IF_OK(msg.Create(GetID(), idMagicType, 0, idTarget, nDamage, GetDir()))
        BroadcastRoomMsg(&msg, INCLUDE_SELF);
    }
    else if (IsBowman())
    {
        CMsgInteract msg;
        IF_OK(msg.Create(INTERACT_SHOOT, GetID(), idTarget, GetPosX(), GetPosY(), nDamage))
        BroadcastRoomMsg(&msg, INCLUDE_SELF);
    }
    else
    {
        CMsgInteract msg;
        IF_OK(msg.Create(INTERACT_ATTACK, GetID(), idTarget, GetPosX(), GetPosY(), nDamage))
        BroadcastRoomMsg(&msg, INCLUDE_SELF);
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::TransferShield(bool bMagic, IRole* pAtker, int nDamage)
{
    IStatus* pStatus = QueryStatus(STATUS_TRANSFER_SHIELD);
    if (!pStatus)
    {
        return false;
    }
    int nMagicType = pStatus->GetPower();
    vector<CMonster*>	setEudemon;
    // 寻找符合条件的已出征幻兽
    for (int i = 0; i < this->GetEudemonAmount(); i++)
    {
        CMonster* pEudemon = this->QueryEudemonByIndex(i);
        if (pEudemon && pEudemon->QueryMagic() && pEudemon->QueryMagic()->FindMagic(nMagicType))
        {
            setEudemon.push_back(pEudemon);
        }
    }
    if (setEudemon.empty())
    {
        return false;
    }
    nDamage = nDamage / setEudemon.size();
    for (int i = 0; i < setEudemon.size(); i++)
    {
        int nLostLife = ::CutOverflow(nDamage, (int)setEudemon[i]->GetLife());
        if (nLostLife > 0)
        {
            setEudemon[i]->AddAttrib(_USERATTRIB_LIFE, -1 * nLostLife, SYNCHRO_TRUE);
        }
        setEudemon[i]->BeAttack(bMagic, pAtker, nDamage);
        // 这里这样处理在连招中应用会导致先杀死后出连招动作的bug
        // 等待确定解决方案
        if (!setEudemon[i]->IsAlive())
        {
            pAtker->Kill(setEudemon[i], bMagic ? DIE_MAGIC : DIE_NORMAL);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::BroadcastTeamLife(bool bMaxLife)
{
    // 2003.2.13 如果对方是玩家而且组队了。被攻击玩家去血值要在队内广播。
    CTeam* pTeam	= GetTeam();
    if (pTeam && pTeam->GetMemberAmount() > 1)
    {
        pTeam->BroadcastMemberLife(this, bMaxLife);
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::DecEquipmentDurability(bool bBeAttack, bool bMagic, int bDurValue/*=1*/)
{
    int nInc = -1 * bDurValue;
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        if (!bMagic)
        {
            if (i == ITEMPOSITION_RINGR ||
                    i == ITEMPOSITION_RINGL ||
                    i == ITEMPOSITION_SHOES ||
                    i == ITEMPOSITION_WEAPONR ||
                    i == ITEMPOSITION_WEAPONL)
            {
                if (!bBeAttack)
                {
                    AddEquipmentDurability(i, nInc);
                }
            }
            else
            {
                if (bBeAttack)
                {
                    AddEquipmentDurability(i, nInc);
                }
            }
        }
        else
        {
            if (i == ITEMPOSITION_RINGR ||
                    i == ITEMPOSITION_RINGL ||
                    i == ITEMPOSITION_SHOES ||
                    i == ITEMPOSITION_WEAPONR ||
                    i == ITEMPOSITION_WEAPONL)
            {
                if (!bBeAttack)
                {
                    AddEquipmentDurability(i, -5);
                }
            }
            else
            {
                if (bBeAttack)
                {
                    AddEquipmentDurability(i, nInc);
                }
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::MagicAttack(int nType, OBJID idTarget, int x, int y)
{
    if (QueryMagic())
    {
        return QueryMagic()->MagicAttack(nType, idTarget, x, y);
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
int	CUser::CalcExpLostOfDeath (IRole* pKiller)
{
    CUser* pUserKiller = NULL;
    if (!pKiller->QueryObj(OBJ_USER, IPP_OF(pUserKiller)))
    {
        return 0;
    }
    // normal condition
    int nParam = 50;
    if (this->GetEffect()&KEEPEFFECT_RED)
    {
        nParam = 10;
    }
    else if (this->GetEffect()&KEEPEFFECT_DEEPRED)
    {
        nParam = 5;
    }
    else
    {
        nParam = 50;
    }
    int nExpLost = __max(0, this->GetExp() / nParam);
    //if (true)
    //	return nExpLost;
    // syndicate member && not the killer wanted guy
    if (this->GetSynID() != ID_NONE && !pUserKiller->IsMyWantedTarget(this))
    {
        int nDecPercent = 0;
        CSyndicate* pSyn = this->GetSyndicate();
        IF_OK (pSyn)
        {
            int nExpPayBySyn = 0;
            int nProffer = this->QuerySynProffer();
            if (nProffer > 0)
            {
                int nRank = this->QuerySynRank();
                //				switch(nRank)
                //				{
                //				case RANK_LEADER:
                //					nDecPercent = 80;
                //					break;
                //
                //				case RANK_SUBLEADER:
                //					nDecPercent = 60;
                //					break;
                //				case RANK_TEAMLEADER:
                //					nDecPercent = 40;
                //					break;
                //				default:
                //					nDecPercent = 25;
                //					break;
                //				}
                CONST int MONEYCOST_PEREXP = 100;
                int nFundSyn  = pSyn->GetInt(SYNDATA_MONEY);
                int nFundLost = ::MulDiv(nExpLost, nDecPercent, 100 * MONEYCOST_PEREXP);	// 100点经验1两
                if (nFundLost > nFundSyn)
                {
                    nFundLost = nFundSyn;
                }
                pSyn->QueryModify()->AddData(SYNDATA_MONEY, -1 * nFundLost, true);
                if (nRank != RANK_LEADER)
                {
                    this->SetSynProffer(nProffer - nFundLost);
                }
                nExpPayBySyn = nFundLost * MONEYCOST_PEREXP;
                nExpLost -= nExpPayBySyn;//nFundLost*MONEYCOST_PEREXP;
            }
            else
            {
                if (pSyn->IsWhiteSynMapOwner() || pSyn->IsBlackSynMapOwner())
                {
                    nDecPercent = 20;
                }
                else
                {
                    nDecPercent = 10;
                }
                CONST int MONEYCOST_PEREXP = 100;
                int nFundSyn  = pSyn->GetInt(SYNDATA_MONEY);
                int nFundLost = ::MulDiv(nExpLost, nDecPercent, 100 * MONEYCOST_PEREXP);	// 100点经验1两
                if (nFundLost > nFundSyn)
                {
                    nFundLost = nFundSyn;
                }
                pSyn->QueryModify()->AddData(SYNDATA_MONEY, -1 * nFundLost, true);
                if (this->QuerySynRank() != RANK_LEADER)
                {
                    this->SetSynProffer(nProffer - nFundLost);
                }
                nExpPayBySyn = nFundLost * MONEYCOST_PEREXP;
                nExpLost -= nExpPayBySyn;//nFundLost*MONEYCOST_PEREXP;
            }
            this->SendSysMsg(_TXTATR_NORMAL, EXPLOST_PAYBY_SYNFUND, nExpPayBySyn);
        }
    }
    return __max(0, nExpLost);
}

//////////////////////////////////////////////////////////////////////
void CUser::Kill(IRole* pTarget, DWORD dwDieWay)
{
    CHECK(pTarget);
    // inc xp
    USHORT usRemainTime = 0;
    //	IStatus* pStatus = QueryStatus(STATUS_XPFULL);
    //	if (pStatus && pTarget->IsMonster())
    //	{
    //		pStatus->IncTime(ADD_XPFULL_SECS_KILLMONSTER*1000, MAX_XPFULL_SECS*1000);
    //		m_nCurrSupermanKills++;
    //		StatusInfoStruct	info;
    //		if (pStatus->GetInfo(&info))
    //			usRemainTime = info.nSecs;
    //	}
    CMsgInteract msg;
    if (msg.Create(INTERACT_KILL, this->GetID(), pTarget->GetID(), pTarget->GetPosX(), pTarget->GetPosY(), dwDieWay, usRemainTime))
    {
        pTarget->BroadcastRoomMsg(&msg, true);
    }
    // earth magic
    //???
    if (QueryMagic())
    {
        QueryMagic()->UserKillTarget(pTarget);
    }
    CUser* pTargetUser	= NULL;
    if (!pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
    {
        pTarget->BeKill((IRole*)this);		// NPC return
        return;
    }
    ProcessPk(pTargetUser);
    // target...
    pTarget->BeKill((IRole*)this);
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcessPk (CUser* pTargetUser)
{
    ASSERT(pTargetUser);
    // map...
    CGameMap* pMap = MapManager()->QueryMap(this->GetMapID());
    IF_OK (pMap)
    {
        if (pMap->IsPkField())
        {
        }
        else if (pMap->IsPkGameMap())
        {
            pTargetUser->DelAllBadge();
            AddBadge();
        }
        else if (pMap->IsSynMap())
        {
        }
        else if (pMap->IsPrisonMap())
        {
        }
        else
        {
            // innocent kill      //change huang 2004.1.11
            if (!(pTargetUser->GetEffect()&KEEPEFFECT_RED)
                    && !(pTargetUser->GetEffect()&KEEPEFFECT_DEEPRED))
            {
                if (this->GetEffect()&(KEEPEFFECT_DEEPRED | KEEPEFFECT_RED))
                {
                    PoliceWanted().AddWanted(this);
                }
            }
            /*			// 新案子没有计算这个损失 --- zlong 2004.10.28
            			int nManaLost = pTargetUser->GetMana()/2;
            			pTargetUser->AddAttrib(_USERATTRIB_MANA, -1*nManaLost, SYNCHRO_TRUE);		// , &dwDirty
            */
            // pk increase    // tudo 2004.1.11   // 没改
            //			if (!pTargetUser->IsEvil()
            //					&& pTargetUser->GetPk() < PKVALUE_REDNAME
            //						&& !pMap->IsDeadIsland())	// 死亡岛杀人不闪蓝，不加PK值
            // && pTargetUser->GetExp() >= 0)	// 圣战中取消这个设定
            // pk increase	—— PK白名红名都要加PK值 -- zlong 2005-04-28
            if (!pMap->IsDeadIsland()
                    && !pTargetUser->QueryStatus(STATUS_CRIME)
                    && pTargetUser->GetPk() < PKVALUE_BLACKNAME)
            {
                int nPkValue = PKVALUE_ONEKILL;
                if (pTargetUser->GetPk() >= PKVALUE_REDNAME)	// PK红名加的PK值减半
                {
                    nPkValue /= 2;
                }
                switch (pTargetUser->GetNobilityRank())
                {
                case NOBILITY_RANK_VISCOUNT:		// 杀子爵+10%PK值
                    nPkValue += nPkValue * 10 / 100;
                    break;
                case NOBILITY_RANK_EARL:			// 杀伯爵+20%PK值
                    nPkValue += nPkValue * 20 / 100;
                    break;
                case NOBILITY_RANK_MARQUESS:		// 杀侯爵+40%PK值
                    nPkValue += nPkValue * 40 / 100;
                    break;
                case NOBILITY_RANK_DUKE:			// 杀公爵+50%PK值
                    nPkValue += nPkValue * 50 / 100;
                    break;
                }
                // syn effect
                if (this->IsHostileSynMember(pTargetUser))
                {
                    nPkValue /= 2;
                }
                this->AddPk(nPkValue);
                // crime
                if (!QueryStatus(STATUS_CRIME))
                {
                    CONST int CRIME_TIME_PERMURDER = 30;	//2*60;		// 闪蓝时间改为30秒
                    this->SetCrimeStatus(STATUS_CRIME, CRIME_TIME_PERMURDER);
                }
                // 提示 —— zlong 2005-04-28
                if (this->GetPk() < PKVALUE_REDNAME)
                {
                    this->SendSysMsg(STR_INCPK_REDNAME_PUNISH);
                }
                else if (this->GetPk() < PKVALUE_BLACKNAME)
                {
                    this->SendSysMsg(STR_INCPK_BLACKNAME_PUNISH);
                }
                else
                {
                    this->SendSysMsg(STR_INCPK_BLACKNAME_PUNISH2);
                }
            }
            // 奖励经验
            {
                // 根据2004.10.28的案子增加 ---- zlong
                __int64	nLevExp = 0;	// 升级所需经验
                int		nStuExp	= 0;	// 被带最高经验
                CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + pTargetUser->GetLev());
                if (!pLevupexp)
                {
                    pLevupexp = LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + __max(0, pTargetUser->GetLev() - 1));
                }
                if (pLevupexp)
                {
                    nLevExp = pLevupexp->GetInt(LEVUPEXPDATA_EXP);
                    nStuExp	= pLevupexp->GetInt(LEVUPEXPDATA_STU_EXP);	//nStuExp将被作为分母，不能为0
                }
                pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev());
                if (!pLevupexp)
                {
                    pLevupexp = LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + __max(0, this->GetLev() - 1));
                }
                int nKillerStuExp	= 0;
                if (pLevupexp)
                {
                    nKillerStuExp	= pLevupexp->GetInt(LEVUPEXPDATA_STU_EXP);
                }
                int nLossExpPercent = 0;
                if (pTargetUser->GetPk() < PKVALUE_REDNAME)					// 白名
                {
                    nLossExpPercent	= 1;    // 损失升级所需1%经验
                }
                else if (pTargetUser->GetPk() < PKVALUE_BLACKNAME)			// 红名
                {
                    nLossExpPercent	= 2;    // 损失升级所需2%经验
                }
                else														// 黑名
                {
                    nLossExpPercent	= 3;    // 损失升级所需3%经验
                }
                __int64 nExpGet = (nLevExp) * nLossExpPercent * 70 / 10000;			// 杀人得到对方损失经验的70%
                // 上面的公式修改了,
                // 杀人的人得到经验=（被杀人损失经验/被杀人被带1次可得最高经验）*杀人的人被带1次可得最高经验*70%
                if (nStuExp == 0)
                {
                    nExpGet	= 0;
                }
                else
                {
                    nExpGet	= nExpGet * nKillerStuExp / nStuExp;
                }
                if (nExpGet > 0)
                {
                    this->AwardBattleExp(nExpGet);
                }
                // 贵族杀白名要扣功勋值
                if (!pTargetUser->IsEvil()
                        && pTargetUser->GetPk() < PKVALUE_REDNAME
                        && !pMap->IsDeadIsland()	// 死亡岛杀人不闪蓝，不加PK值
                        && this->GetNobilityRank() > NOBILITY_RANK_NONE)
                {
                    int nExploitLost = 0;
                    switch (this->GetNobilityRank())
                    {
                    case NOBILITY_RANK_VISCOUNT:		// 子爵杀人功勋值扣对方损失经验的1/80
                        nExploitLost = (nLevExp * nLossExpPercent) / 100 / 80;
                        break;
                    case NOBILITY_RANK_EARL:			// 伯爵杀人功勋值扣对方损失经验的1/60
                        nExploitLost = (nLevExp * nLossExpPercent) / 100 / 60;
                        break;
                    case NOBILITY_RANK_MARQUESS:		// 侯爵杀人功勋值扣对方损失经验的1/40
                        nExploitLost = (nLevExp * nLossExpPercent) / 100 / 40;
                        break;
                    case NOBILITY_RANK_DUKE:			// 公爵杀人功勋值扣对方损失经验的1/20
                        nExploitLost = (nLevExp * nLossExpPercent) / 100 / 20;
                        break;
                    }
                    if (nExploitLost > 0)
                    {
                        this->AddAttrib(_USERATTRIB_EXPLOIT, -1 * nExploitLost, SYNCHRO_TRUE);
                    }
                }
                // 贵族杀红黑名奖励功勋值
                if (pTargetUser->GetPk() >= PKVALUE_REDNAME
                        && !pMap->IsDeadIsland()
                        && this->GetNobilityRank() > NOBILITY_RANK_NONE)
                {
                    int nExploitGet = (nLevExp * nLossExpPercent) / 100 / 100;
                    if (nExploitGet > 0)
                    {
                        this->AddAttrib(_USERATTRIB_EXPLOIT, nExploitGet, SYNCHRO_TRUE);
                    }
                }
            }
            // wanted kill
            if (this->IsMyWantedTarget(pTargetUser))
            {
                OBJID idWanted = this->WantedInfo().idWanted;
                CWantedData* pData = CWantedList::s_WantedList.GetWanted(idWanted);
                // award bonuty!
                int nBonuty = 0;
                if (pData)
                {
                    nBonuty = pData->GetInt(DATA_BOUNTY);
                }
                this->AddAttrib(_USERATTRIB_MONEY, nBonuty*(100 - _BONUTY_TAX) / 100, SYNCHRO_TRUE);
                // del wanted from list
                CWantedList::s_WantedList.DelWanted(idWanted);
                // reset my wanted info
                this->ResetWantedInfo();
                // inform client
                CMsgName msg;
                IF_OK_ (msg.Create(NAMEACT_WANTED, ""))
                this->SendMsg(&msg);
                // lost of wanted target
                int nLevExp = 0;
                CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev());
                if (pLevupexp)
                {
                    nLevExp = pLevupexp->GetInt(LEVUPEXPDATA_EXP);
                }
                int nCurExp = pTargetUser->GetExp();
                int nExpLost = __min(nBonuty, nLevExp);
                if (nCurExp >= 0)
                {
                    if (nCurExp - nExpLost <= 0)
                    {
                        nExpLost = nCurExp + (nExpLost - nCurExp) / 2;
                    }
                }
                else
                {
                    int nFactor = (-1 * nCurExp / nLevExp + 1) * 2;
                    nExpLost = nExpLost / nFactor;
                }
                pTargetUser->AddAttrib(_USERATTRIB_EXP, -1 * nExpLost, SYNCHRO_TRUE);
            }
            // 佣兵系统中的杀人任务 -- 以后考虑这里优化处理，不要每次都去查询自己接的任务
            CMercenaryTaskData* pTaskData = MercenaryTask()->QueryTaskDataAccepted(GetID());
            if (pTaskData && pTaskData->GetInt(MTASKDATA_TYPE) == _WANT_PK
                    && pTaskData->GetInt(MTASKDATA_STATE) == MTASKSTATE_ACCEPTED)
            {
                if (strcmp(pTaskData->GetStr(MTASKDATA_TARGET_NAME), pTargetUser->GetName()) == 0)
                {
                    pTaskData->SetInt(MTASKDATA_STATE, MTASKSTATE_FINISHED, true);
                    // task finished.
                    this->SendSysMsg(STR_TASK_FINISHED);
                }
            }
            // police wanted kill
            if (pTargetUser->GetID() == this->PoliceWantedID())
            {
                this->PoliceWantedID() = ID_NONE;
                int nKillAward = __min(-1, -1 * (int(pTargetUser->GetLev()) - 40) / 10);
                this->AddAttrib(_USERATTRIB_PK, nKillAward, SYNCHRO_TRUE);
            }
            else
            {
                // team hunter
                CTeam* pTeam = this->GetTeam();
                if (pTeam)
                {
                    CUser* pLeader = UserManager()->GetUser(pTeam->GetLeader());
                    if (pLeader)
                    {
                        if (pLeader->PoliceWantedID() == pTargetUser->GetID())
                        {
                            pLeader->TeamHuntKill(pTargetUser->GetID(), pTargetUser->GetLev(), pTargetUser->GetName());
                        }
                    }
                    else	// in other thread?
                    {
                        char buf[1024] = {0};
                        REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
                        pInfo->ucFuncType	= REMOTE_CALL_TEAM_HUNTKILL;
                        pInfo->idUser		= pTeam->GetLeader();
                        pInfo->IntParam[0]	= pTargetUser->GetID();
                        pInfo->IntParam[1]	= pTargetUser->GetLev();
                        strcpy((char*)(pInfo->IntParam[2]), pTargetUser->GetName());
                        pInfo->nSize		= sizeof(REMOTE_CALL0) + 2 * sizeof(int) + strlen(pTargetUser->GetName()) + 1;
                        MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
                    }
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::TeamHuntKill (OBJID idTarget, int nTargetLev, const char* pszTargetName)
{
    if (idTarget != this->PoliceWantedID())
    {
        return;
    }
    CTeam* pTeam = this->GetTeam();
    IF_NOT (pTeam)
    return;
    IF_NOT (pTeam->GetLeader() == this->GetID())
    return;
    // ok now
    this->PoliceWantedID() = ID_NONE;
    int nKillAward = __min(-1, -1 * (nTargetLev - 40) / 10);
    this->AddAttrib(_USERATTRIB_PK, nKillAward, SYNCHRO_TRUE);
    // broadcast msg
    MSGBUF szMsg = "";
    sprintf(szMsg, STR_TEAM_HUNT, pszTargetName, this->GetName());
    this->BroadcastMapMsg(szMsg, true);
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsMyWantedTarget (CUser* pTarget)
{
    IF_NOT (pTarget)
    return false;
    OBJID idWanted = this->WantedInfo().idWanted;
    if (ID_NONE == idWanted)
    {
        return false;
    }
    CWantedData* pData = CWantedList::s_WantedList.GetWanted(idWanted);
    return (pData && 0 == strcmp(pData->GetStr(DATA_TARGET_NAME), pTarget->GetName()));
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsPoliceWantedTarget(CUser* pTarget)
{
    IF_NOT (pTarget)
    return false;
    // police wanted kill
    if (pTarget->GetID() == this->PoliceWantedID())
    {
        return true;
    }
    // team hunter, leader get the police wanted for this target
    CTeam* pTeam = this->GetTeam();
    if (pTeam)
    {
        CUser* pLeader = UserManager()->GetUser(pTeam->GetLeader());
        if (pLeader && pLeader->PoliceWantedID() == pTarget->GetID())
        {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CUser::BeKill(IRole* pRole /*= NULL*/)
{
    bool bDropItem	= true;
    bool bDropExp	= true;
    CItem* pItem = this->GetItemByType(GHOST_GEM_AVOID_DEATH);
    if (pItem)
    {
        this->SendSysMsg(STR_AVOID_DEATH, pItem->GetStr(ITEMDATA_NAME));
        this->EraseItem(pItem->GetID(), true);
        this->SetAttrib(_USERATTRIB_LIFE, this->GetMaxLife(), SYNCHRO_TRUE);
        return;
    }
    else if (pItem = this->GetItemByType(GHOST_GEM_AMULET))
    {
        this->SendSysMsg(STR_AVOID_DEATH, pItem->GetStr(ITEMDATA_NAME));
        this->EraseItem(pItem->GetID(), true);
        this->SetAttrib(_USERATTRIB_LIFE, 1, SYNCHRO_TRUE);
        return;
    }
    else if (pItem = this->GetItemByType(GHOST_GEM_SCAPEGOAT))
    {
        this->SendSysMsg(STR_AUTO_USE_SCAPEGOAT, pItem->GetStr(ITEMDATA_NAME));
        this->EraseItem(pItem->GetID(), true);
        bDropExp = bDropItem = false;
    }
    else if (pItem = this->GetItemByType(GHOST_GEM_REBORN))
    {
        m_tAutoReborn.Startup(AUTO_REBORN_SECS);
    }
    // 死亡状态清除其他所有的状态？
    ClrAttackTarget();
    if (QueryMagic())
    {
        QueryMagic()->AbortMagic(true);
    }
    CallBackAllEudemon();
    DetachEudemon();
    //	CRole::DetachStatus(this->QueryRole(), STATUS_LOCK);
    //	CRole::DetachStatus(this->QueryRole(), STATUS_FREEZE);
    this->SetStatus(STATUS_DIE);
    StandTurnoff();
    m_bGhost	= false;
    OBJID	idRebornMap = ID_NONE;
    POINT	pos;
    if (this->GetLev() <= MAX_RETURN_BORN_POS_LEV)
    {
        CGameMap* pMap = MapManager()->GetGameMap(DEFAULT_LOGIN_MAPID);
        IF_OK (pMap)
        this->SetRecordPos(pMap->GetID(), DEFAULT_LOGIN_POSX, DEFAULT_LOGIN_POSY);
    }
    else
    {
        IF_OK(GetMap()->GetRebornMap(&idRebornMap, &pos))
        this->SetRecordPos(idRebornMap, pos.x, pos.y);
    }
    KillCallPet();
    if (m_pMount)
    {
        m_pMount->SetInt(ITEMDATA_INTIMACY, ::CutTrail(0, m_pMount->GetInt(ITEMDATA_INTIMACY) - INTIMACY_DEAD_DEC), UPDATE_TRUE);
        this->SendSysMsg(STR_DEC_MOUNT_INTIMACY);
        // synchro
        CMsgItemInfo	msg;
        IF_OK(msg.Create(m_pMount, ITEMINFO_UPDATE))
        SendMsg(&msg);
    }
    // no pk pulishment in pk field
    CGameMap* pMap = MapManager()->QueryMap(this->GetMapID());
    IF_OK (pMap)
    {
        // 因为打开了WITH_BLOCK开关，为了在死亡后尸体允许穿透，需要减地图上的角色数
        pMap->DecRole(this->GetPosX(), this->GetPosY());
        if (!pMap->IsNeverWoundMap())
        {
            // 受伤
            SetMaxLifePercent(m_data.GetMaxLifePercent() / 2);		// 由SetMaxLifePercent保证不低于1/8
        }
        // 掉物品 设置重生点
        if (pMap->IsPkField() || pMap->IsSynMap())
        {
            // drop item
            {
                int nChance = 30;
                if (!pMap->IsDeadIsland() && bDropItem) // 死亡岛不掉物品
                {
                    m_pPackage->RandDropItem(_MAP_PKFIELD, nChance);
                }
            }
            // fly out
            /*if (pMap->IsPkField())
            {
            	if (idRebornMap != ID_NONE)
            		this->FlyMap(idRebornMap, pos.x, pos.y);
            }
            */
            // set reborn pos to DoubleDragon city if not in syn war but in syn map
            if (pMap->IsSynMap() && !pMap->IsWarTime())
            {
                CONST OBJID ID_DDCITY	= 1002;
                CONST POINT POS_REBORN	= { 438, 398 };
                this->SetRecordPos(ID_DDCITY, POS_REBORN.x, POS_REBORN.y);
            }
            return;
        }
        else if (pMap->IsPrisonMap())
        {
            // drop item
            {
                if (!pMap->IsDeadIsland() && bDropItem) // 死亡岛不掉物品
                {
                    int nChance = __min(90, 20 + this->GetPk() / 2);
                    m_pPackage->RandDropItem(_MAP_PRISON, nChance);
                }
            }
            return;
        }
    }
    else
    {
        return;
    }
    if (!pRole)		// no killer
    {
        return ;
    }
    // pk pulishment
    CUser* pUserKiller = NULL;
    pRole->QueryObj(OBJ_USER, IPP_OF(pUserKiller));
    if (pUserKiller && !pMap->IsDeadIsland())// 死亡岛 帮派贡献 掉钱 掉物品 掉装备无效
    {
        // kill by other player
        CSyndicate* pSyn = this->GetSyndicate();
        CSyndicate* pSynKiller = pUserKiller->GetSyndicate();
        if (pSyn && pSynKiller && (pSyn->IsHostile(pSynKiller->GetID()) || pSynKiller->IsHostile(pSyn->GetID())))
        {
            // 对方属于敌对帮派
            //	this->QuerySynAttr()->DecProffer(PROFFER_BEKILL);
            pSyn->QueryModify()->AddData(SYNDATA_REPUTE, -1 * SYNREPUTE_BEKILL);
            //pSynKiller->QueryModify()->AddData(SYNDATA_REPUTE, SYNREPUTE_BEKILL);
            //添加声望增量
            //pUserKiller->QuerySynAttr()->AddReputeInc(SYNREPUTE_BEKILL);
        }
        // 自己不是处于闪蓝状态才加仇人
        if (!QueryStatus(STATUS_CRIME))
        {
            QueryEnemy()->Add(pUserKiller->GetID(), pUserKiller->GetName(), SYNCHRO_TRUE, UPDATE_TRUE);
        }
        // drop money
        {
            // 按照2004.10.28的案子修改	---- zlong
            DWORD dwMoneyDroped = 0;
            if (GetPk() < PKVALUE_REDNAME)								// 白名
            {
                dwMoneyDroped = this->GetMoney() * (RandGet(40) + 10) / 100;    // 掉钱 10% ~ 50%
            }
            else if (GetPk() < PKVALUE_BLACKNAME)						// 红名
            {
                dwMoneyDroped = this->GetMoney() * (RandGet(50) + 50) / 100;    // 掉钱 50% ~ 100%
            }
            else														// 黑名
            {
                dwMoneyDroped = this->GetMoney();    // 掉钱 100%
            }
            if (dwMoneyDroped > 0)
            {
                this->DropMoney(dwMoneyDroped, this->GetPosX(), this->GetPosY());
            }
        }
        // drop item    //change huang 2004.1.12   //todo
        if (bDropItem)
        {
            // 按照2004.10.28的案子修改	---- zlong
            bool bDropItem = false;
            int nChance = 0;
            if (GetPk() < PKVALUE_REDNAME)								// 白名
            {
                nChance = 10 + RandGet(40);    // 掉物品 10% ~ 50%
            }
            else if (GetPk() < PKVALUE_BLACKNAME)						// 红名
            {
                nChance = 50 + RandGet(50);    // 掉物品 50% ~ 100%
            }
            else														// 黑名
            {
                nChance	= 100;    // 掉物品 100%
            }
            //int nItems	  = m_setItem.size();
            int nItems		= m_pPackage->GetAmount();
            int nDropItem = (nItems * nChance) / 100;
            m_pPackage->RandDropItem(nDropItem);
        }
        // drop equipment                 //change huang 2004.1.12
        if (bDropItem)
        {
            // 按照2004.10.28的案子修改	---- zlong
            BOOL bDropEquipment = false;
            int nEquipDropNum = 0;                  //must lost equip item num.
            if (this->GetPk() >= PKVALUE_BLACKNAME)
            {
                nEquipDropNum = 1;    // 黑名被PK死身上装备随机掉一件
            }
            std::vector<OBJID>	setPosition;
            for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
            {
                CItemPtr pEquip = GetEquipItemByPos(i);
                if (pEquip && !pEquip->IsNeverDropWhenDead())
                {
                    setPosition.push_back(i);
                }
            }
            if (setPosition.size() > 0)
            {
                for (int i = 0; i < __min(nEquipDropNum, setPosition.size()); i++)
                {
                    int nIndex = ::RandGet(setPosition.size());
                    OBJID idPosition = setPosition[nIndex];
                    CItemPtr pEquip = GetEquipItemByPos(idPosition);
                    if (pEquip)
                    {
                        if (ITEMPOSITION_WEAPONR == idPosition)
                        {
                            CItem* pWeaponL = this->GetEquipItemByPos(ITEMPOSITION_WEAPONL);
                            if (pWeaponL)
                            {
                                pEquip = pWeaponL;
                            }
                        }
                        POINT	pos;
                        pos.x	= GetPosX();
                        pos.y	= GetPosY();
                        if (GetMap()->FindDropItemCell(USERDROPITEM_RANGE, &pos))
                        {
                            this->DropItem(pEquip->GetID(), pos.x, pos.y);
                            bDropEquipment = true;
                        }
                    }
                    else
                    {
                        ::LogSave("GetEquipItemByPos failed item does not exists in position [ %u ].", idPosition);
                    }
                }
                setPosition.clear();
                if (bDropEquipment)
                {
                    CMsgPlayer msg;
                    if (msg.Create(this->QueryRole()))
                    {
                        this->BroadcastRoomMsg(&msg, EXCLUDE_SELF);
                    }
                }
            }
        }
        // loss exp
        if (bDropExp)
        {
            // 按照2004.10.28的案子增加的	---- zlong
            int nLossExpPercent = 0;
            if (GetPk() < PKVALUE_REDNAME)								// 白名
            {
                nLossExpPercent	= 1;    // 损失升级所需1%经验
            }
            else if (GetPk() < PKVALUE_BLACKNAME)						// 红名
            {
                nLossExpPercent	= 2;    // 损失升级所需2%经验
            }
            else														// 黑名
            {
                nLossExpPercent	= 3;    // 损失升级所需3%经验
            }
            int nLevExp = 0;
            CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev());
            if (!pLevupexp)
            {
                pLevupexp = LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + __max(0, this->GetLev() - 1));
            }
            if (pLevupexp)
            {
                nLevExp = pLevupexp->GetInt(LEVUPEXPDATA_EXP);
            }
            __int64 nExpLost = ((__int64)nLevExp) * nLossExpPercent / 100;
            if (nExpLost > 0)
            {
                AddAttrib(_USERATTRIB_EXP, -1 * nExpLost, SYNCHRO_TRUE);
            }
        }
        // bad guy in police wanted
        if (PoliceWanted().GetWanted(this->GetID()))
        {
            this->GotoJail();
            PoliceWanted().DelWanted(this->GetID());
        }
        // dec sprite's growth -- add by zlong 2003-12-01
        if (m_pSprite)
        {
            m_pSprite->AddGrowth(-1);
        }
        // dec pk value ---- 根据2004.10.28案子增加 -- zlong
        if (this->GetPk() > 0)
        {
            this->AddPk(-1 * PKVALUE_DEC_PERDEATH);
        }
    }
    else if (pUserKiller && pMap->IsDeadIsland())
    {
        // 自己不是处于闪蓝状态才加仇人
        if (!QueryStatus(STATUS_CRIME))
        {
            QueryEnemy()->Add(pUserKiller->GetID(), pUserKiller->GetName(), SYNCHRO_TRUE, UPDATE_TRUE);
        }
    }
    else	// kill by monster
    {
        CMonster* pMonster = NULL;
        if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
        {
            CUser* pUser = pMonster->QueryOwnerUser();
            // 自己不是处于闪蓝状态才加仇人
            if (pUser && !QueryStatus(STATUS_CRIME))
            {
                QueryEnemy()->Add(pUser->GetID(), pUser->GetName(), SYNCHRO_TRUE, UPDATE_TRUE);
            }
        }
        if (!pMap->IsDeadIsland())
        {
            // money
            DWORD dwMoneyLost = ::RandGet(this->GetMoney() / 3);
            if (dwMoneyLost > 0)
            {
                this->DropMoney(dwMoneyLost, this->GetPosX(), this->GetPosY());
            }
            // item
            if (bDropItem)
            {
                int nChance = 33;
                if (this->GetLev() < 10)
                {
                    nChance = 5;
                }
                m_pPackage->RandDropItem(_MAP_NONE, nChance);
            }
            if (m_pSprite)
            {
                m_pSprite->AddGrowth(-1);
            }
            // guard
            if (pMonster && (pMonster->IsGuard() || pMonster->IsPkKiller()))
            {
                if (this->GetPk() >= PKVALUE_BLACKNAME)
                {
                    this->GotoJail();
                }
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
void CUser::AwardBattleExp(int nExp, bool bGemEffect/*=true*/)
{
    // 本函数操作战斗中得到或者扣除经验值。
    if (GetLev() > ADJUST_EXPFORMAT_LEVEL)
    {
        nExp /= 10;
    }
    if (nExp == 0)
    {
        return;
    }
    if (nExp < 0)
    {
        // 扣经验
        this->AddAttrib(_USERATTRIB_EXP, nExp, SYNCHRO_TRUE);
        return;
    }
    // gem effect
    if (bGemEffect)
    {
        int nAddPercent = this->GetGemExpEffect();
        nExp += nExp * nAddPercent / 100;
#ifdef _DEBUG
        if (nAddPercent != 0)
        {
            this->SendSysMsg(_TXTATR_NORMAL, "got gem exp add percent: %d", nAddPercent);
        }
#endif
    }
    if (this->GetLev() >= MAX_USERLEV)
    {
        return;
    }
    if (this->GetLev() >= MASTER_USERLEV)
    {
        nExp /= 2;
    }
#ifdef _DEBUG
    this->SendSysMsg(_TXTATR_NORMAL, "got battle exp: %d", nExp);
#endif
    // 增加经验
    CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev());
    if (!pLevupexp)
    {
        return;
    }
    int nLevupExp	= pLevupexp->GetInt(LEVUPEXPDATA_EXP);
    int nOrgProgress = ::MulDiv(this->GetExp(), 1000, nLevupExp);
    int nNewExp = nExp + this->GetExp();
    if (nNewExp >= nLevupExp)
    {
        this->SetAttrib(_USERATTRIB_EXP, 0, SYNCHRO_TRUE);
        this->IncLev(1);
        if (IsAutoAllot())
        {
            this->AllotPoint();    //? add and allot
        }
        else
        {
            this->AddAttrib(_USERATTRIB_ADDPOINT, _ADDITIONALPOINT_NUM, SYNCHRO_TRUE);
        }
        // 更新最大血量
        CMsgUserAttrib	msg;
        if (msg.Create(GetID(), _USERATTRIB_MAXLIFE, GetMaxLife()))
        {
            SendMsg(&msg);
        }
        // 2003.2.9 升级自动补满血。
        this->SetAttrib(_USERATTRIB_LIFE, this->GetMaxLife(), SYNCHRO_TRUE);
        this->SetAttrib(_USERATTRIB_MANA, this->GetMaxMana(), SYNCHRO_TRUE);
        CMsgAction msgAction;
        if (msgAction.Create(this->GetID(), 0, 0, 0, actionUplev, 0, 0))
        {
            this->BroadcastRoomMsg(&msgAction, INCLUDE_SELF);
        }
        // team life broacast
        if (this->GetTeam())
        {
            this->BroadcastTeamLife(true);
        }
        // save info
        m_data.SaveInfo();
    }
    else
    {
        this->SetAttrib(_USERATTRIB_EXP, nNewExp, SYNCHRO_TRUE);
        int nCurProgress = ::MulDiv(this->GetExp(), 1000, nLevupExp);
        bool bSave = false;
        int nLev = this->GetLev();
        if (nLev >= 110)
        {
            if (nOrgProgress / 5 != nCurProgress / 5)
            {
                bSave = true;
            }
        }
        else if (nLev >= 100)
        {
            if (nOrgProgress / 10 != nCurProgress / 10)
            {
                bSave = true;
            }
        }
        else if (nLev >= 80)
        {
            if (nOrgProgress / 20 != nCurProgress / 20)
            {
                bSave = true;
            }
        }
        else if (nLev >= 60)
        {
            if (nOrgProgress / 50 != nCurProgress / 50)
            {
                bSave = true;
            }
        }
        else
        {
            if (nOrgProgress / 100 != nCurProgress / 100)
            {
                bSave = true;
            }
        }
        if (bSave)
        {
            m_data.SaveInfo();
        }
    }
    // 奖励导师经验
    if (GetTutor() && GetLev() >= _MIN_GIVEEXP_LEV)
    {
        m_dwExpToTutor += nExp * TUTOREXP_PERCENT / 100;
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsAtkable(IRole* pTarget, bool bSendHint)
{
    if (!pTarget)
    {
        return false;
    }
    if (!this->IsAlive())
    {
        if (bSendHint)
        {
            this->SendSysMsg(STR_DIE_FORGIVE);
        }
        return false;
    }
    if (!pTarget->IsAlive())
    {
        //	if (bSendHint)
        //		this->SendSysMsg("死者为大，禁止鞭尸。");
        return false;
    }
    if (!this->IsPkEnable(pTarget))
    {
        if (bSendHint)
        {
            this->SendSysMsg(STR_NOT_PK_MODE);
        }
        return false;
    }
    if (GetID() == pTarget->GetID())
    {
        return false;
    }
    if (!pTarget->IsBeAtkable())
    {
        return false;
    }
    // 不能攻击自己的幻兽
    if (this->QueryEudemonByID(pTarget->GetID()))
    {
        return false;
    }
    CNpc* pNpc;
    if (pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc)))
    {
        if (pNpc->IsSynFlag() && GetSynID() != ID_NONE && pNpc->GetSynID() != ID_NONE)
        {
            //---jinggy---若是攻打帮派，必须是申请攻打的帮派成员才可以打---begin
            /*	CGameMap * pMapSyn = this->GetMap();
            	if (pMapSyn->IsSynMap())
            	{
            		if (pMapSyn->IsWarTime())
            		{
            			//不是申请攻打的帮派，就不能打
            			CNpc * pWarLetterNpc = pMapSyn->GetNpcSynWarLetter();
            			if (!pWarLetterNpc)
            				return false;

            			if (this->GetSynID() != (pWarLetterNpc->GetApplyAttackerSynID()))
            				return false;

            		}
            		else //非战争时间不允许攻打
            			return false;
            	}*/
            //---jinggy---若是攻打帮派，必须是申请攻打的帮派成员才可以打---end
            CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());
            if (pSyn && pSyn->IsNearSyn(pNpc->GetSynID()))
            {
                return false;
            }
        }
    }
    CUser* pUserTarget = NULL;
    pTarget->QueryObj(OBJ_USER, IPP_OF(pUserTarget));
    // 圣战修改为经验值为负仍然允许PK
    //	if (pUserTarget && this->GetExp() < 0)
    //	{
    //		if (bSendHint)
    //			this->SendSysMsg(STR_NOT_PK_EXP);
    //		return false;
    //	}
    // map check
    if (!(this->GetMap() && pTarget->GetMap() && this->GetMap() == pTarget->GetMap()))
    {
        return false;
    }
    if (GetMap()->IsNewbieMap() || (GetMap()->IsPkDisable() && pUserTarget))
    {
        if (bSendHint)
        {
            this->SendSysMsg(STR_PK_FORBIDDEN);
        }
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
bool CUser::Create(PROCESS_ID idProcess, SOCKET_ID idSocket, OBJID idUser)
{
    CHECKF(CMapGroup::IsValidMapGroupProcessID(idProcess));
    m_idProcess	= idProcess;
    m_idSocket	= idSocket;
    m_nDir		= 0;	//DEFAULT_LOGIN_DIR;
    m_nPose		= 0;	//DEFAULT_LOGIN_POSE;
    m_nEmotin	= 0;	//DEFAULT_LOGIN_EMOTION;
    // 装入DATA
    if (!m_data.Create(idUser, Database()))
    {
        return false;
    }
    // temp attrib
    m_data.m_Info.usMaxEnergy	= 100;
    m_data.m_Info.usEnergy		= m_data.m_Info.usMaxEnergy;
    // 进入地图
    m_nPosX		= this->m_data.m_Info.usRecordX;
    m_nPosY		= this->m_data.m_Info.usRecordY;
    OBJID idMap	= this->m_data.m_Info.idRecordmap;
    m_pMap		= MapManager()->GetGameMap(idMap);
    if (!m_pMap)		//? 活动地图
    {
        m_nPosX		= DEFAULT_LOGIN_POSX;
        m_nPosY		= DEFAULT_LOGIN_POSY;
        OBJID idMap	= DEFAULT_LOGIN_MAPID;
        m_pMap		= MapManager()->GetGameMap(idMap);
        CHECKF(m_pMap);
    }
    Init();
    // 装入USER管理对象
    IDatabase* pDb = Database();
    if (!CreateAllFriend(pDb))
    {
        return false;
    }
    if (!CreateAllTutor(pDb))
    {
        return false;
    }
    if (!QueryEnemy()->Create(idProcess, this, pDb))
    {
        return false;
    }
    if (!CreateAllItem(pDb))
    {
        return false;
    }
    if (!CreateWeaponSkill(pDb))
    {
        return false;
    }
    if (!m_pBattleSystem)
    {
        return false;
    }
    if (!m_pMagic || !m_pMagic->CreateAll())
    {
        return false;
    }
    if (!m_pSyn || !m_pSyn->Create())
    {
        return false;
    }
    if (!m_setStatus)
    {
        return false;
    }
    //---jinggy---2004-11-19---初始化装备经验值计数器---begin
    UpdateEquipmentExp_Init();
    //---jinggy---2004-11-19---初始化装备经验值计数器---end
    //---任务系统---begin
    m_pTaskDetail = CTaskDetail::CreateNew();
    IF_NOT(m_pTaskDetail && m_pTaskDetail->Create(m_idProcess, this))
    return false;
    //---任务系统---end
    // divoice
    /*	if (this->GetTaskItemByType(TYPE_DIVOICEITEM))
    	{
    		m_data.SetMate(NOMATE_NAME, false);
    	}
    */
    // status
    SetStatus(STATUS_NORMAL);
    m_data.SetLastLogin();
    // life?...
    if (this->GetLife() <= 0)
    {
        this->AddAttrib(_USERATTRIB_LIFE, 1, SYNCHRO_TRUE);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::Init			()		// login 和 change map_group都要执行的操作
{
    m_tFight.SetInterval(this->GetIntervalAtkRate());		// 徒手攻频率
    m_tFight.Update();
    m_tXp.SetInterval(ADD_XP_SECS);
    m_tXp.Update();
    m_tSlowHealLife.SetInterval(SLOWHEALLIFE_MS);
    m_tSlowHealLife.Update();
    m_tSlowHealMana.SetInterval(SLOWHEALLIFE_MS);
    m_tSlowHealMana.Update();
    m_tAutoHealLife.SetInterval(AUTOHEALLIFE_TIME);
    m_tAutoHealLife.Update();
    m_tEquipmentDurability.SetInterval(DURABILITY_DEC_TIME);		// every 12s change durability
    m_tEquipmentDurability.Update();
    m_tPK.SetInterval(PK_DEC_TIME);		// every 1 minutes reduce 2 pk value //change huang 2004.1.11
    m_tPK.Update();
    m_tTeam.SetInterval(TIME_TEAMPRC);
    m_tTeam.Update();
    m_pBattleSystem	= new CBattleSystem(this->m_idProcess, this);
    ASSERT(m_pBattleSystem);
    m_pMagic = CMagic::CreateNew(m_idProcess, this->QueryRole());
    ASSERT(m_pMagic);
    m_pSyn	= CSynAttr::CreateNew(m_idProcess, this);
    ASSERT(m_pSyn);
    m_setStatus	= CStatusSet::CreateNew(true);
    ASSERT(m_setStatus);
    DEBUG_TRY
    if (m_nStep == STEP_LOGIN)
        if (this->IsStudentFull())
        {
            Announce()->DeleteAnnounceByUserID(this->GetID(), true);
        }
        else
        {
            Announce()->Create(Database(), this->GetID());
        }
    DEBUG_CATCH("Announce()->Create(Database(),this->GetID()) ERROR!");
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::SaveInfo			()
{
    m_data.SaveInfo();
    SaveItemInfo();
    SaveAllWeaponSkillInfo();
    m_pMagic->SaveInfo();
    m_pSyn->SaveInfo();
    //@ 其它需要存盘的东东
    //...
    // 存贡献给导师的经验
    if (GetTutor() && GetLev() >= _MIN_GIVEEXP_LEV)
    {
        if (m_dwExpToTutor > 0)
        {
            int nTutorExp2	= m_dwExpToTutor * TUTOREXP2_PERCENT / TUTOREXP_PERCENT;
            SQLBUF szSQL;
            sprintf(szSQL, "UPDATE %s SET exp=exp+%u, tutor_exp=tutor_exp+%u WHERE tutor_id=%u AND tutor_lev>%u LIMIT 1",
                    _TBL_TUTOREXP, m_dwExpToTutor, nTutorExp2, m_pTutor->GetTutorID(), GetLev());
            Database()->ExecuteSQL(szSQL);
            m_dwExpToTutor	= 0;	// 必须清0
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::SaveAllWeaponSkillInfo()
{
    for(int i = 0; i < m_pUserWeaponSkillSet->GetAmount(); i++)
    {
        CUserWeaponSkill* pData = m_pUserWeaponSkillSet->GetObjByIndex(i);
        if (pData)
        {
            pData->SaveInfo();
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::AddLeaveWord(LPCTSTR szRecv, LPCTSTR szWords)
{
    for(int i = 0; i < m_setFriend.size(); i++)
    {
        CFriend* pFriend = m_setFriend[i];
        if (pFriend && strcmp(pFriend->GetFriendName(), szRecv) == 0)
        {
            ASSERT(UserManager()->GetUser(pFriend->GetFriendID()) == NULL);
            if (pFriend->IsLeaveWordEnable())
            {
                LeaveWord()->InsertWords(GetName(), szRecv, szWords);
                SendSysMsg(STR_MSG_SENT);
                return true;
            }
            else
            {
                SendSysMsg(STR_MSG_FAILED, LEAVEWORD_INTERVAL_MINS);
            }
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
// application
//////////////////////////////////////////////////////////////////////
/*
int	 CUser::AdjustData(int nData, int nAdjust, int nMaxData/ *=0* /)
{
	if (nAdjust>=ADJUST_PERCENT)
		return MulDiv(nData, nAdjust-ADJUST_PERCENT, 100);

	if (nAdjust<=ADJUST_SET)
		return -1*nAdjust + ADJUST_SET;

	if (nAdjust==ADJUST_FULL)
	{
		ASSERT(nMaxData != 0);
		return nMaxData;
	}

	return nData + nAdjust;
}
*/

//////////////////////////////////////////////////////////////////////
bool CUser::SendMsg(CNetMsg* pMsg)
{
    if (false
            //		||pMsg->GetType() == _MSG_ACTION
            || pMsg->GetType() == _MSG_INTERACT
            //		||pMsg->GetType() == _MSG_PLAYER
            //		||pMsg->GetType() == _MSG_TRADE
      )
    {
        DEBUG_SENDMSG(pMsg->GetType(), m_idSocket)
    }
    return MapGroup(PID)->QueryIntraMsg()->SendClientMsg(m_idSocket, pMsg);
}

//////////////////////////////////////////////////////////////////////
void CUser::BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf /*= false*/)
{
    CRole::BroadcastRoomMsg(pMsg, bSendSelf);
}

//////////////////////////////////////////////////////////////////////
void CUser::BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf /*= false*/)
{
    CMsgTalk	msg;
    if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
    {
        this->BroadcastRoomMsg(&msg, bSendSelf);
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::BroadcastMapMsg(CNetMsg* pMsg, bool bSendSelf /*= false*/)
{
    //CMapPtr pMap = MapManager()->GetGameMap(GetMapID());
    CMapPtr pMap = this->GetMap();
    if (pMap)
    {
        UserManager()->BroadcastMapMsg(pMap->GetID(), pMsg, bSendSelf ? NULL : this);
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::BroadcastMapMsg(LPCTSTR szMsg, bool bSendSelf /*= false*/)
{
    CMsgTalk	msg;
    if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
    {
        this->BroadcastMapMsg(&msg, bSendSelf);
    }
}

//////////////////////////////////////////////////////////////////////

bool CUser::SendSysMsg(const char* fmt, ...)
{
    MSGBUF	szMsg;
    vsprintf( szMsg, fmt, (char*) ((&fmt) + 1) );
    CMsgTalk msg;
    if (msg.Create(SYSTEM_NAME, GetName(), szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
    {
        return this->SendMsg(&msg);
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::SendSysMsg(int nChannel, const char* fmt, ...)
{
    MSGBUF	szMsg;
    vsprintf( szMsg, fmt, (char*) ((&fmt) + 1) );
    CMsgTalk msg;
    if (msg.Create(SYSTEM_NAME, GetName(), szMsg, NULL, 0x00ffff00, nChannel))
    {
        return this->SendMsg(&msg);
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::ChangeMap()
{
    POINT	pos;
    pos.x	= GetPosX();
    pos.y	= GetPosY();
    int nX[9] = {0, 1, -1, 1, 0, 1, -1, -1, 0};
    int nY[9] = {0, 1, -1, 0, 1, -1, 1, 0, -1};
    for (int i = 0; i < 9; i++)
    {
        POINT posTest = pos;
        posTest.x += nX[i];
        posTest.y += nY[i];
        OBJID	idMap = ID_NONE;
        POINT	posTarget = {0, 0};
        if (GetMap()->GetPassageMap(&idMap, &posTarget, posTest))
        {
            if (FlyMap(idMap, posTarget.x, posTarget.y) != FLYMAP_ERROR)
            {
                return true;
            }
            else
            {
                break;
            }
        }
    }
    FlyMap(DEFAULT_LOGIN_MAPID, DEFAULT_LOGIN_POSX, DEFAULT_LOGIN_POSY);
    UserManager()->KickOutSocket(m_idSocket, "无切屏点！");
    return false;
}

//////////////////////////////////////////////////////////////////////
OBJID CUser::GetMapID()
{
    if (m_pMap)
    {
        return m_pMap->GetID();
    }
    return ID_NONE;
}

//////////////////////////////////////////////////////////////////////
OBJID CUser::GetRecordMapID()
{
    return m_data.GetRecordMap();
}

//////////////////////////////////////////////////////////////////////
void CUser::GotoJail(void)
{
    CGameMap* pMap = MapManager()->QueryMap(MAP_PRISON);
    IF_OK (pMap)
    {
        char szMsg[1024] = "";
        sprintf(szMsg, STR_GOTO_JAIL, this->GetName());
        UserManager()->BroadcastMsg(szMsg);
        this->QuitTeam();
        POINT pos = pMap->GetPortal();
        this->FlyMap(MAP_PRISON, pos.x, pos.y);
        // reset reborn pos
        {
            OBJID	idRebornMap = ID_NONE;
            POINT	pos;
            IF_OK(GetMap()->GetRebornMap(&idRebornMap, &pos))
            this->SetRecordPos(idRebornMap, pos.x, pos.y);
        }
    }
}

//////////////////////////////////////////////////////////////////////
// return false: can't fly
int CUser::FlyMap(OBJID idMap, int nPosX, int nPosY)				// call - may be delete this;
{
    if (m_pMap == NULL)			// not in map
    {
        LOGWARNING("玩家出现连续切屏。");
        return FLYMAP_ERROR;
    }
    if (idMap == ID_NONE)
    {
        idMap = this->GetMapID();
    }
    PROCESS_ID idProcess = MapManager()->GetProcessID(idMap);
    if (!CMapGroup::IsValidMapGroupProcessID(idProcess) || idProcess == m_idProcess)
    {
        CGameMap* pNewMap = MapManager()->GetGameMap(idMap);
        if (!pNewMap || !pNewMap->IsValidPoint(nPosX, nPosY))
        {
            LOGMSG("FLY 地图[%u]的坐标[%u][%u]非法!", idMap, nPosX, nPosY);
            return FLYMAP_ERROR;
        }
        DEBUG_TRY	// VVVVVVVVVVVVVV
        LeaveMap();
        m_pMap		= pNewMap;
        m_nPosX		= nPosX;
        m_nPosY		= nPosY;
        // 发送回应消息
        CMsgAction	msg;
        if (msg.Create(GetID(), nPosX, nPosY, GetDir(), actionFlyMap, pNewMap->GetDocID()))
        {
            SendMsg(&msg);
        }
        EnterMap();
        DEBUG_CATCH("FlyMap")		// AAAAAAAAAAA
        return FLYMAP_NORMAL;
    }
    else
    {
        ChangeMapGroup(idProcess, idMap, nPosX, nPosY);					// call - delete this;
        return FLYMAP_MAPGROUP;
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::OnTimer(time_t tCurr)
{
    if (m_pMap == NULL)		// not join map ok.
    {
        return;
    }
    // tick ------------------------------------
#ifndef _DEBUG
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "m_dequeServerTick")
    if (!IsAgent())
    {
        DWORD dwTimeNow = ::TimeGet();
        if (m_dwFirstServerTick == 0)
        {
            // this code will run only once with each user
            m_dwFirstServerTick = dwTimeNow;
            m_dwLastServerTick	= dwTimeNow;
            m_dwLastRcvClientTick = dwTimeNow;	// pre set this
            CMsgTick msg;
            if (msg.Create(GetID()))
            {
                this->SendMsg(&msg);
            }
            m_dequeServerTick.push_back(dwTimeNow);
        }
        else
        {
            if (dwTimeNow >= m_dwLastServerTick + TICK_SECS * 1000)
            {
                m_dwLastServerTick	= dwTimeNow;
                m_dequeServerTick.push_back(dwTimeNow);
                CMsgTick msg;
                if (msg.Create(GetID()))
                {
                    this->SendMsg(&msg);
                }
            }
        }
        if (m_dwLastRcvClientTick != 0)
        {
            if (dwTimeNow - m_dwLastRcvClientTick >= TICK_SECS * 1000 * 2.5)
            {
                UserManager()->KickOutSocket(m_idSocket);    // 	, "tick not feedback"
            }
        }
    }
    DEBUG_CATCH("@m_dwServerTickCount@")
#endif
    //---jinggy---2004-11-12--Begin---检测身上各个装备是否到升级战魂经验点时候
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "UpdateEquipmentExp")
    UpdateEquipmentExp();
    DEBUG_CATCH("CUser UpdateEquipmentExp crash");
    //---jinggy---2004-11-12--End---检测身上各个装备是否到升级战魂经验点时候
    //---jinggy---若玩家加入帮派，则检测连续在线---begin
    DEBUG_TRY
    if (this->GetSyndicate())
    {
        if (!m_tTimeOnline.IsActive())
        {
            m_tTimeOnline.Startup(SYNMEM_TIMEONLINE_INTERVAL);
        }
        if (this->m_tTimeOnline.ToNextTime(SYNMEM_TIMEONLINE_INTERVAL))
        {
            this->QuerySynAttr()->IncTimeOnline(SYNMEM_TIMEONLINE_INTERVAL / 60);
        }
    }
    DEBUG_CATCH("this->QuerySynAttr()->IncTimeOnline(TIMEONLINE_INTERVAL_DETECT/60); //增加10分钟的在线时间");
    //---jinggy---若玩家加入帮派，则检测连续在线---end
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "ProcessAutoAttack")
    ProcessAutoAttack();
    DEBUG_CATCH("CUser ProcAttack_Hand2Hand carsh")
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "ProcXpVal")
    if (m_tXp.ToNextTime())
    {
        this->ProcXpVal();
    }
    DEBUG_CATCH("CUser ProcXpVal crash")
    DEBUG_TRY
    // 2003.2.8如果以后有做吃药时慢慢补血，再coding here
    if (m_tSlowHealLife.ToNextTime() && m_setSlowHealUp2Life.Size() > 0)
    {
        this->ProcSlowHealLifeUp();
    }
    DEBUG_CATCH("CUser ProcSlowHealLifeUp crash")
    DEBUG_TRY
    if (m_tSlowHealMana.ToNextTime() && m_setSlowHealUp2Mana.Size() > 0)
    {
        this->ProcSlowHealManaUp();
    }
    DEBUG_CATCH("CUser ProcSlowHealManaUp crash")
    //	DEBUG_TRY	//以需要检查宿营技能
    //	DEADLOOP_CHECK(PID, "ProcAutoHealLife")
    //	if (m_tAutoHealLife.ToNextTime())
    //		this->ProcAutoHealLife();
    //	DEBUG_CATCH("CUser autoheallife timer crash.")
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "ENERGY timer")
    if (IsAlive())		// && !this->QueryTransformation()
    {
        //		if (!this->IsWing())
        {
            if (QueryStatus(STATUS_PELT))
            {
                if (m_tEnergy.ToNextTime(DEL_ENERGY_PELT_SECS))
                {
                    if (this->GetEnergy() < DEL_ENERGY_PELT)
                    {
                        CRole::DetachStatus(this->QueryRole(), STATUS_PELT);
                    }
                    else
                    {
                        AddAttrib(_USERATTRIB_ENERGY, -1 * DEL_ENERGY_PELT, SYNCHRO_TRUE);
                    }
                }
            }
            else if (GetEnergy() < GetMaxEnergy())
            {
#ifdef	PALED_DEBUG
                SendSysMsg("WALK --------------");
#endif
                if (m_tEnergy.ToNextTime(ADD_ENERGY_STAND_SECS))
                {
                    AddAttrib(_USERATTRIB_ENERGY, ADD_ENERGY_STAND, SYNCHRO_TRUE);
                }
            }
        }
    }
    DEBUG_CATCH("CUser ENERGY timer crash.")
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "QueryMagic()->OnTimer")
    if (QueryMagic())
    {
        QueryMagic()->OnTimer(tCurr);
    }
    DEBUG_CATCH("CUser magic timer crash.")
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "status")
    // 以下增加部分代码用于判断是否解除结界系统状态
    // bDetachTeamStatus = true 表示需要解除所有与结界相关的状态
    bool	bDetachTeamStatus = false;
    bool	bDetachAddExpStatus = false;	// 是否解除STATUS_ADD_EXP
    CTeam* pTeam = GetTeam();
    if (IsAlive() && pTeam)
    {
        if (pTeam->GetMemberAmount() < TEAM_STATUS_REQ_ROLES)
        {
            bDetachTeamStatus = true;
        }
        OBJID idLeader = pTeam->GetLeader();
        if (idLeader != this->GetID())
        {
            CUser* pLeader = UserManager()->GetUser(idLeader);
            if (!pLeader || !pLeader->IsAlive()
                    || this->GetMapID() != pLeader->GetMapID()
                    || this->GetDistance(pLeader->QueryMapThing()) > _RANGE_TEAM_STATUS)
            {
                bDetachTeamStatus = true;
            }
            if (!pLeader || !pLeader->QueryStatus(STATUS_ADD_EXP))
            {
                bDetachAddExpStatus = true;
            }
        }
    }
    else
    {
        bDetachTeamStatus = true;
        bDetachAddExpStatus = true;
    }
    for(int i = QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
    {
        IStatus* pStatus = QueryStatusSet()->GetObjByIndex(i);
        if (pStatus)
        {
            pStatus->OnTimer(tCurr);
            if (!pStatus->IsValid()
                    || (bDetachTeamStatus && pStatus->GetID() >= STATUS_TEAM_BEGIN && pStatus->GetID() <= STATUS_TEAM_END)
                    || (bDetachAddExpStatus && pStatus->GetID() == STATUS_ADD_EXP))
            {
                OBJID idStatus = pStatus->GetID();
                QueryStatusSet()->DelObj(idStatus);
                // superman list
                if (idStatus == STATUS_XPFULL)
                {
                    CalcSupermanKills();
                    CalcFightRate();
                }
            }
        }
    }
    DEBUG_CATCH("CUser status timer crash.")
    /*
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "UpdateEquipmentDurability")
    if (m_tEquipmentDurability.ToNextTime())
    	this->UpdateEquipmentDurability();
    DEBUG_CATCH("UpdateEquipmentDurability crashed.")
    */
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "ProcessPkTimer")
    if (m_tPK.ToNextTime())
    {
        this->ProcessPkTimer();
    }
    DEBUG_CATCH("CUser pk timer crash.")
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "ProcessMineTimer")
    if (m_tMine.IsActive() && m_tMine.ToNextTime())
    {
        this->ProcessMineTimer();
        ResetEnergyInc();
    }
    DEBUG_CATCH("CUser mine timer crash.")
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "ProcessTeamTimer")
    if (this->GetTeam() && m_tTeam.ToNextTime())
    {
        this->ProcessTeamTimer();
    }
    DEBUG_CATCH("CUser team timer crash.")
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "AbortTransform")
    if (m_pTransformation && m_tTransformation.TimeOver())
    {
        AbortTransform();
    }
    DEBUG_CATCH("CUser Transformation crash.")
    DEBUG_TRY
    DEADLOOP_CHECK(PID, "KillCallPet")
    if (m_tCallPet.IsActive() && m_tCallPet.TimeOver())
    {
        KillCallPet();
    }
    DEBUG_CATCH("CUser CallPet crash.")
    DEBUG_TRY
    if (m_pMount)
    {
        DEADLOOP_CHECK(PID, "m_pMount")
        if (!m_tMount.IsActive())
        {
            m_tMount.Startup(MOUNT_ADD_INTIMACY_SECS);
        }
        else if (m_tMount.ToNextTime())
        {
            if (m_pMount->GetInt(ITEMDATA_INTIMACY) < MAX_INTIMACY)
            {
                m_pMount->SetInt(ITEMDATA_INTIMACY, m_pMount->GetInt(ITEMDATA_INTIMACY) + 1, UPDATE_TRUE);
                this->SendSysMsg(STR_INC_MOUNT_INTIMACY);
                // synchro
                CMsgItemInfo	msg;
                if (msg.Create(m_pMount, ITEMINFO_UPDATE))
                {
                    SendMsg(&msg);
                }
            }
        }
    }
    DEBUG_CATCH("CUser CallPet crash.")
    // add by zlong 2003-11-28
    DEBUG_TRY
    if (m_pSprite)
    {
        DEADLOOP_CHECK(PID, "m_pSprite")
        if (!m_tSprite.IsActive())
        {
            m_tSprite.Startup(SPRITE_ADD_EXP_SECS);
        }
        else if (m_tSprite.ToNextTime())
        {
            if (m_pSprite->AwardExp(SPRITE_ADD_EXP, this->GetLuck()) > 0)
            {
                // synchro
                CMsgItemInfo	msg;
                if (msg.Create(m_pSprite, ITEMINFO_UPDATE))
                {
                    SendMsg(&msg);
                }
            }
        }
    }
    DEBUG_CATCH("CUser Sprite crash.")
    DEBUG_TRY
    EUDEMON_SET::iterator it = m_setEudemon.begin();
    for (; it != m_setEudemon.end(); it++)
    {
        ST_EUDEMON* ptEudemon = *it;
        IF_NOT (ptEudemon)
        {
            m_setEudemon.erase(it);
            break;
        }
        if (ptEudemon->pEudemonItem && ptEudemon->pEudemon)
        {
            DEADLOOP_CHECK(PID, "ptEudemon")
            if (!ptEudemon->tEudemon.IsActive())
            {
                ptEudemon->tEudemon.Startup(EUDEMON_ADD_FIDELITY_SECS);
            }
            else if (ptEudemon->tEudemon.ToNextTime() && ptEudemon->pEudemonItem->GetInt(ITEMDATA_FIDELITY) < MAX_EUDEMON_FIDELITY)
            {
                int nFidelity = __min(MAX_EUDEMON_FIDELITY, ptEudemon->pEudemonItem->GetInt(ITEMDATA_FIDELITY) + EUDEMON_ADD_FIDELITY);
                ptEudemon->pEudemonItem->SetInt(ITEMDATA_FIDELITY, nFidelity, true);
                CMsgItemAttrib msg;
                if (msg.Create(ptEudemon->pEudemonItem->GetID(), _ITEMATTRIB_FIDELITY, ptEudemon->pEudemonItem->GetInt(ITEMDATA_FIDELITY)))
                {
                    this->SendMsg(&msg);
                }
            }
        }
    }
    DEBUG_CATCH("CUser Eudemon crash.")
    /*
    	DEBUG_TRY
    	DEADLOOP_CHECK(PID, "LeaveWord()->ShowWords")
    	if (IsMoreLeaveWord() && m_tLeaveWord.ToNextTime(5))
    	{
    		LeaveWord()->ShowWords(this);
    	}
    	DEBUG_CATCH("CUser LeaveWord crash.")
    */
    DEBUG_TRY
    if (!m_bStand && m_tStand.TimeOver(KEEP_STAND_MS) && IsAlive())
    {
        StandTurnon();
    }
    DEBUG_CATCH("Stand crash.")
    // 军团正式成员在线6分钟，军团声望增加10点
    DEBUG_TRY
    if (this->GetSynID() != ID_NONE)
    {
        if (!m_tIncRepute.IsActive())
        {
            m_tIncRepute.Startup(SYNREPUTE_SECS);
        }
        else if (m_tIncRepute.ToNextTime())
        {
            CSyndicate* pSyn = this->GetSyndicate();
            if (pSyn)
            {
                if (this->GetSynRank() > RANK_RESERVED)
                {
                    //加帮派声望
                    pSyn->QueryModify()->AddData(SYNDATA_REPUTE, SYNREPUTE_VAL_PEREACH, false);
                    //加帮派贡献度
                    this->QuerySynAttr()->AddProffer(SYNREPUTE_VAL_PROFFER);
                }
            }
        }
    }
    DEBUG_CATCH("Auto inc syndicate repute crash.");
    // 潜能值每6分钟增长1点
    DEBUG_TRY
    if (!m_tIncPotential.IsActive())
    {
        m_tIncPotential.Startup(INC_POTENTIAL_SECS);
    }
    else if (m_tIncPotential.ToNextTime())
    {
        this->AddAttrib(_USERATTRIB_POTENTIAL, INC_POTENTIAL_NUM, SYNCHRO_TRUE);
    }
    DEBUG_CATCH("Inc potential crash.");
    //
    DEBUG_TRY
    if (IsAlive() && m_data.GetMaxLifePercent() < 1000)
    {
        if (!m_tAutoHealMaxLife.IsActive())
        {
            m_tAutoHealMaxLife.Startup(AUTOHEAL_MAXLIFE_TIME);
        }
        else if (m_tAutoHealMaxLife.ToNextTime())
        {
            int nMaxLifePercent = m_data.GetMaxLifePercent();
            if (nMaxLifePercent >= 500)
            {
                nMaxLifePercent += AUTOHEAL_MAXLIFE_FLESH_WOUND;    // 轻伤恢复
            }
            else
            {
                nMaxLifePercent += AUTOHEAL_MAXLIFE_GBH;    // 重伤恢复
            }
            SetMaxLifePercent(nMaxLifePercent);
            if (nMaxLifePercent >= 1000)
            {
                m_tAutoHealMaxLife.Clear();
            }
        }
    }
    DEBUG_CATCH("Auto heal max life crash.");
    DEBUG_TRY
    if (m_tAutoReborn.IsActive() && m_tAutoReborn.IsTimeOut())
    {
        m_tAutoReborn.Clear();
        if (!IsAlive())
        {
            CItem* pItem = this->GetItemByType(GHOST_GEM_REBORN);
            if (pItem)
            {
                this->SendSysMsg(STR_AUTO_USE_REBORN, pItem->GetStr(ITEMDATA_NAME));
                this->EraseItem(pItem->GetID(), true);
                this->Reborn(false);	// 原地复活
            }
        }
    }
    DEBUG_CATCH("Auto use reborn gem crash");
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcessAutoAttack		()
{
    // 速度型幻兽合体提高攻击速度
    int nFightPause = m_nFightRate + m_nFightPause;
    if (m_idLinkEudemonType == ITEMTYPE_EUDEMON_SPEED)
    {
        nFightPause = __max(0, nFightPause - m_nLinkValue);
    }
    if (m_pBattleSystem && m_pBattleSystem->IsActived()
            && m_tFight.ToNextTick(nFightPause)
            && (!QueryMagic() || !QueryMagic()->IsActive()))
    {
        m_pBattleSystem->ProcAttack_Hand2Hand();
        m_nFightPause	= 0;
        if (m_pBattleSystem->GetTargetID() != ID_NONE)
        {
            ResetEnergyInc();
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::ClearAllStatus		()
{
    ClsXpVal();
    for(int i = QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
    {
        IStatus* pStatus = QueryStatusSet()->GetObjByIndex(i);
        if (pStatus)
        {
            QueryStatusSet()->DelObj(pStatus->GetID());
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcessPkTimer()
{
    if (this->GetPk() > 0)
    {
        CGameMap* pMap = MapManager()->QueryMap(this->GetMapID());
        IF_OK (pMap)
        {
            if (pMap->IsPrisonMap())
            {
                this->AddPk(-1 * PKVALUE_DEC_ONCE_IN_PRISON);
                return;
            }
        }
        this->AddPk(-1 * PKVALUE_DEC_ONCE);
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsPkEnable(IRole* pRole)
{
    if (!pRole || this->GetID() == pRole->GetID())
    {
        return false;
    }
    if (pRole->IsMonster())
    {
        return true;
    }
    CNpc* pNpc = NULL;
    pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc));
    if (pNpc)
    {
        return true;
    }
    bool bCanPk = false;
    switch(this->GetPkMode())
    {
    case	PKMODE_FREE:
        bCanPk = true;
        break;
    case	PKMODE_TEAM:               //change huang 2004.1.15
        {
            bCanPk = true;
            CTeam* pTeam = this->GetTeam();
            if (pTeam && pTeam->IsTeamMember(pRole->GetID()))
            {
                bCanPk = false;
            }
            if (this->GetFriend(pRole->GetID()))
            {
                bCanPk = false;
            }
            CUser* pUser = NULL;
            if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
            {
                if (this->IsMate(pUser))
                {
                    bCanPk = false;
                }
            }
            return bCanPk;
        }
        break;
    case    PKMODE_SYNDICATE:     //add huang 2004.1.15
        {
            bool bCanPk = true;
            CUser* pUser = NULL;
            if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)) && this->GetSynID() != ID_NONE)
            {
                if (this->GetMasterSynID() == pUser->GetMasterSynID() || this->IsMate(pUser))
                {
                    bCanPk = false;
                }
                else
                {
                    CSyndicate* pSyn = SynManager()->QuerySyndicate(this->GetMasterSynID());
                    if (pSyn)
                    {
                        if ( pSyn->IsFriendly(pUser->GetSynID()))
                        {
                            bCanPk = false;
                        }
                    }
                }
            }
            return bCanPk;
        }
        break;
    case	PKMODE_ARRESTMENT:
        {
            if (pRole->IsEvil())
            {
                bCanPk = true;
            }
        }
        break;
    case	PKMODE_SAFE:
        bCanPk = false;
        break;
    default:
        ASSERT(!"switch pk mode");
        bCanPk = false;
        break;
    }
    return bCanPk;
}

//////////////////////////////////////////////////////////////////////
void CUser::Reborn(bool bChgPos/*=true*/)
{
    if (this->IsAlive())
    {
        return;
    }
    SetStatus(STATUS_NORMAL);
    m_bGhost	= false;
    ClsXpVal();					//? 客户端重新开始计时
    m_tAutoHealMaxLife.Clear();
    SetAttrib(_USERATTRIB_LIFE, GetMaxLife(), SYNCHRO_TRUE);
    SetAttrib(_USERATTRIB_MANA, GetMaxMana(), SYNCHRO_TRUE);
    SetAttrib(_USERATTRIB_KEEPEFFECT, GetEffect(), SYNCHRO_BROADCAST);
    SetAttrib(_USERATTRIB_LOOKFACE, GetLookFace(), SYNCHRO_BROADCAST);
    AddAttrib(_USERATTRIB_ENERGY, 100, SYNCHRO_TRUE);
    AddAttrib(_USERATTRIB_MAXENERGY, 0, SYNCHRO_TRUE);	// 这里仅仅是为了同步
    const bool bMaxLife = true;
    this->BroadcastTeamLife(bMaxLife);
    if (bChgPos)
    {
        if (ID_NONE != m_data.GetRecordMap())
        {
            this->FlyMap(m_data.GetRecordMap(), m_data.GetRecordX(), m_data.GetRecordY());
        }
    }
    // lock
    CRole::AttachStatus(this->QueryRole(), STATUS_PK_PROTECT, 0, CHGMAP_LOCK_SECS);
    //	m_tLock.Startup(CHGMAP_LOCK_SECS);
}

//////////////////////////////////////////////////////////////////////
void CUser::TransformGhost()
{
    m_bGhost = true;
    SetAttrib(_USERATTRIB_KEEPEFFECT, SetEffect(KEEPEFFECT_GHOST), SYNCHRO_BROADCAST);
    SetAttrib(_USERATTRIB_LOOKFACE, GetLookFace(), SYNCHRO_BROADCAST);
}

//////////////////////////////////////////////////////////////////////
// 用于任务系统
//////////////////////////////////////////////////////////////////////
int CUser::AddPk(int nAdd)			// auto synchro
{
    ASSERT(nAdd != 0);
    if (abs(m_data.GetPk() + nAdd) < MAX_PKLIMIT)
    {
        m_data.SetPk(__max(0, m_data.GetPk() + nAdd));
    }
    else
    {
        m_data.SetPk(MAX_PKLIMIT);
    }
    if (nAdd > 0)
    {
        SetStatus(STATUS_PKVALUE, SYNCHRO_TRUE);
    }
    else
    {
        ClsStatus(STATUS_PKVALUE, SYNCHRO_TRUE);
    }
    this->SetAttrib(_USERATTRIB_PK, this->GetPk(), SYNCHRO_TRUE);
    return m_data.GetPk();
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcXpVal()
{
    // 每30秒+15，满了，30秒清空
    if (!IsAlive())
    {
        ClsXpVal();
        return;
    }
    IStatus* pStatus = QueryStatus(STATUS_XPFULL);	//XPFULL期间不加，也不清除
    if (pStatus)
    {
        return;
    }
    if (m_dwXP == MAX_USER_XP)	// full already
    {
        SetXp(0);
        m_tXp.Update();
    }
    else
    {
        if (GetMap() && GetMap()->IsBoothEnable())		// 进入摆摊地图就不加XP了
        {
            return;
        }
        //---jinggy---begin
        //玩家特殊状态调节XP值
        IStatus* pStatus = QueryStatus(STATUS_ADJUST_XP);
        int nAddXpValue = ADD_XP_VALUE;
        if (pStatus)
        {
            nAddXpValue = ::CutTrail(0, AdjustData(nAddXpValue, pStatus->GetPower()));
        }
        //---jinggy---end
        AddXp(nAddXpValue);
    }
#ifdef _DEBUG
    this->SendSysMsg("[Debug]玩家积累了%u的Xp。Xp=%d则满。", m_dwXP, MAX_USER_XP);
#endif
}

//////////////////////////////////////////////////////////////////////
void CUser::ClsXpVal(bool bSynchro)
{
    SetXp(0);
    m_tXp.Update();
    if (bSynchro)
    {
        CRole::DetachStatus(this->QueryRole(), STATUS_XPFULL);
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::BurstXp()
{
    if (m_dwXP < MAX_USER_XP)
    {
        return false;
    }
    IStatus* pStatus = QueryStatus(STATUS_XPFULL);
    if (pStatus)
    {
        return true;
    }
    CRole::AttachStatus(this->QueryRole(), STATUS_XPFULL, 0, MAX_XPFULL_SECS);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::SetXp(int nXp)
{
    if (nXp < 0 || nXp > MAX_USER_XP)
    {
        return;
    }
    this->SetAttrib(_USERATTRIB_XP, nXp, SYNCHRO_TRUE);
}

//////////////////////////////////////////////////////////////////////
void CUser::AddXp(int nXp)
{
    if (nXp < 0 || nXp > MAX_USER_XP)
    {
        return;
    }
    this->AddAttrib(_USERATTRIB_XP, nXp, SYNCHRO_TRUE);
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcSlowHealLifeUp()
{
    CHECK(m_setSlowHealUp2Life.Size());
    if (!IsAlive() || GetLife() >= GetMaxLife())
    {
        m_setSlowHealUp2Life.Clear();
        return ;
    }
    int nAddLife = m_setSlowHealUp2Life.At(0);
    m_setSlowHealUp2Life.DelObj(0);
    AddAttrib(_USERATTRIB_LIFE, nAddLife, SYNCHRO_TRUE);
    BroadcastTeamLife();
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcSlowHealManaUp()
{
    CHECK(m_setSlowHealUp2Mana.Size());
    if (!IsAlive() || GetMana() >= GetMaxMana())
    {
        m_setSlowHealUp2Mana.Clear();
        return ;
    }
    int nAddMana = m_setSlowHealUp2Mana.At(0);
    m_setSlowHealUp2Mana.DelObj(0);
    AddAttrib(_USERATTRIB_MANA, nAddMana, SYNCHRO_TRUE);
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcAutoHealLife()
{
    if (this->GetPose() != _ACTION_SITDOWN ||
            this->GetLife() >= this->GetMaxLife() ||
            !this->IsAlive())
    {
        return;
    }
    this->AddAttrib(_USERATTRIB_LIFE, AUTOHEALLIFE_EACHPERIOD, SYNCHRO_TRUE);
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcessTick(DWORD dwClientTime)
{
#ifdef _DEBUG
    return;
#endif
    if (m_dwFirstClientTick == 0)
    {
        m_dwFirstClientTick = dwClientTime;
    }
    if (dwClientTime < m_dwLastClientTick)
    {
        // ridiculous time stamp
        this->SendSysMsg(STR_CONNECTION_OFF);
        UserManager()->KickOutSocket(m_idSocket, "ProcessTick");
        return;
    }
    const int CRITICAL_TICK_PERCENT = 5;
    int nServerTicks = m_dequeServerTick.size();
    if (m_dwLastClientTick != 0 &&
            nServerTicks >= 2 &&
            dwClientTime > m_dwLastClientTick + TICK_SECS*(100 + CRITICAL_TICK_PERCENT) * 10)	// *10 means 1000ms/100percent
    {
        // suspicious time stamp
        DWORD dwTimeServer = ::TimeGet();
        DWORD dwTimeServerTickInterval = m_dequeServerTick[1] - m_dequeServerTick[0];
        int nEchoTime = dwTimeServer - m_dequeServerTick[1];
        if (nEchoTime < (int)dwClientTime - (int)m_dwLastClientTick - (int)dwTimeServerTickInterval)
        {
            ::GmLogSave("发现玩家（%s）使用加速器。", this->GetName());
            this->SendSysMsg(STR_CONNECTION_OFF);
            UserManager()->KickOutSocket(m_idSocket, "加速器");
            return;
        }
    }
    m_dwLastClientTick = dwClientTime;
    if (m_dequeServerTick.size() >= 2)
    {
        m_dequeServerTick.pop_front();
    }
    m_dwLastRcvClientTick = ::TimeGet();
}

//////////////////////////////////////////////////////////////////////
// friend
//////////////////////////////////////////////////////////////////////
CFriend* CUser::GetFriend(OBJID idFriend)
{
    for(int i = 0; i < m_setFriend.size(); i++)
    {
        CFriend* pFriend = m_setFriend[i];
        if (pFriend && pFriend->GetFriendID() == idFriend)
        {
            return m_setFriend[i];
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CUser::AddFriend(OBJID idFriend, LPCTSTR szFriendName)		// 未检查重复添加
{
    CFriend* pFriend = CFriend::CreateNew();
    if (pFriend && pFriend->CreateRecord(GetID(), idFriend, szFriendName, Database()))
    {
        m_setFriend.push_back(pFriend);
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::DelFriend(OBJID idFriend)
{
    for(int i = 0; i < m_setFriend.size(); i++)
    {
        CFriend* pFriend = m_setFriend[i];
        if (pFriend && pFriend->GetFriendID() == idFriend)
        {
            pFriend->DeleteRecord(Database());
            delete pFriend;
            m_setFriend.erase(m_setFriend.begin() + i);
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
BOOL CUser::CreateWeaponSkill(IDatabase* pDb)
{
    CHECKF(pDb);
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT * FROM %s WHERE owner_id=%u && unlearn=0", _TBL_WEAPONSKILL, this->GetID());
    IRecordset* pRes = pDb->CreateNewRecordset(szSQL);	//VVVVVVVVV
    if (pRes)
    {
        for (int i = 0; i < pRes->RecordCount(); i++)
        {
            CUserWeaponSkill*	pSkill	= CUserWeaponSkill::CreateNew(pRes);
            if (pSkill)
            {
                m_pUserWeaponSkillSet->AddObj(pSkill);
            }
            pRes->MoveNext();
        }
        pRes->Release();		//AAAAAAAAAAAAAA
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::CheckWeaponSubType(int nSubType, int nNum)
{
    CHECKF(nSubType);
    /*	if (nSubType == 900)		// shield
    	{
    		if (m_pWeaponL && m_pWeaponL->IsShield() && m_pWeaponL->GetInt(ITEMDATA_AMOUNT) >= nNum)
    			return true;

    		return false;
    	}
    */
    if (m_pWeaponR && m_pWeaponR->GetItemSubType() == nSubType && m_pWeaponR->GetInt(ITEMDATA_AMOUNT) >= nNum)
    {
        return true;
    }
    if (m_pWeaponL && m_pWeaponL->GetItemSubType() == nSubType && m_pWeaponL->GetInt(ITEMDATA_AMOUNT) >= nNum)
    {
        return true;
    }
    if (nSubType == 400)
    {
        int setTypes[2]	= {410, 420};
        if (m_pWeaponR && (m_pWeaponR->GetItemSubType() == setTypes[0] || m_pWeaponR->GetItemSubType() == setTypes[1])
                && m_pWeaponR->GetInt(ITEMDATA_AMOUNT) >= nNum)
        {
            return true;
        }
        if (m_pWeaponL && (m_pWeaponL->GetItemSubType() == setTypes[0] || m_pWeaponL->GetItemSubType() == setTypes[1])
                && m_pWeaponL->GetInt(ITEMDATA_AMOUNT) >= nNum)
        {
            return true;
        }
    }
    // 500分类是征服中剑的分类，魔域中已经没有这个分类，所以下面这段代码无效 -- zlong 2005-03-03
    /*
    	if (nSubType == 500)
    	{
    		nSubType	= 501;	//? 法剑可以完全代替剑
    		if (m_pWeaponR && m_pWeaponR->GetItemSubType() == nSubType && m_pWeaponR->GetInt(ITEMDATA_AMOUNT) >= nNum)
    			return true;
    		if (m_pWeaponL && m_pWeaponL->GetItemSubType() == nSubType && m_pWeaponL->GetInt(ITEMDATA_AMOUNT) >= nNum)
    			return true;
    	}
    */
    return false;
}

//////////////////////////////////////////////////////////////////////
void CUser::AddWeaponSkillExp(int nSkillType, int nExp)
{
    //int nSkillType = GetWeaponSkillType();
    if (nSkillType == ARROW_SUBTYPE)
    {
        return ;
    }
    for (int i = 0; i < m_pUserWeaponSkillSet->GetAmount(); i++)
    {
        CUserWeaponSkill* pSkill	= m_pUserWeaponSkillSet->GetObjByIndex(i);
        if (pSkill && pSkill->GetType() == nSkillType)
        {
            int nIncreaseLev	= 0;
            // exp adjusted by lev effect
            if (pSkill->GetLevel() > MASTER_WEAPONSKILLLEVEL)
            {
                int nRatio = 100 - (pSkill->GetLevel() - MASTER_WEAPONSKILLLEVEL) * 20;
                if (nRatio < 10)
                {
                    nRatio = 10;
                }
                nExp = ::MulDiv(nExp, nRatio, 100) / 2;
            }
            // new exp
            int nNewExp	= __max(nExp + pSkill->GetExp(), pSkill->GetExp());
#ifdef _DEBUG
            this->SendSysMsg(_TXTATR_NORMAL, "Add Weapon Skill exp: %u, CurExp: %u", nExp, nNewExp);
#endif
            int nLevel = pSkill->GetLevel();
            if (nLevel >= 1 && nLevel < MAX_WEAPONSKILLLEVEL)
            {
                if (nNewExp > nWeaponSkillLeveUpNeedExp[nLevel] || nLevel >= pSkill->GetOldLev() / 2 && nLevel < pSkill->GetOldLev())
                {
                    nNewExp			= 0;
                    nIncreaseLev	= 1;
                }
            }
            else
            {
                return;
            }
            if (pSkill->GetLevel() < this->GetLev() / 10 + 1		// base level is 1
                    || pSkill->GetLevel() >= MASTER_WEAPONSKILLLEVEL)
            {
                // every 10 percent save exp once.
                bool bSave = false;
                CHECK(nWeaponSkillLeveUpNeedExp[nLevel]);
                int nOldExpPercent = ::MulDiv(pSkill->GetExp(), 100, nWeaponSkillLeveUpNeedExp[nLevel]);
                int nNewExpPercent = ::MulDiv(nNewExp, 100, nWeaponSkillLeveUpNeedExp[nLevel]);
                if ((nOldExpPercent % 10) != 0 && (nNewExpPercent % 10) == 0)
                {
                    bSave = true;
                }
                // inc lev save exp too
                if (nIncreaseLev > 0)
                {
                    bSave = true;
                }
                // set new exp
                pSkill->SetExp(nNewExp, bSave);
                if (nIncreaseLev > 0)
                {
                    pSkill->SetLevel(pSkill->GetLevel() + nIncreaseLev, true);
                    CMsgWeaponSkill msg;
                    if (msg.Create(pSkill->GetType(), pSkill->GetLevel(), pSkill->GetExp()))
                    {
                        this->SendMsg(&msg);
                    }
                    this->SendSysMsg(STR_WEAPON_SKILL_UP);
                    if (QueryMagic())
                    {
                        QueryMagic()->UserWeaponSkillUpLevel(nSkillType, pSkill->GetLevel());
                    }
                }
                else
                {
                    CMsgFlushExp msg;
                    if (msg.Create(MSGFLUSHEXP_WEAPONSKILL, pSkill->GetType(), nNewExp))
                    {
                        this->SendMsg(&msg);
                    }
                    if (nExp && QueryMagic())
                    {
                        QueryMagic()->AutoLearnWeaponSkill(nSkillType, pSkill->GetLevel(), nExp);
                    }
                }
            }
            return;
        }
        else if (!pSkill)
        {
            LOGERROR("发现错误的武器熟练度技能数据。");
        }
    }
    // 玩家没有nSkillType这种武器熟练度，创建新记录。
    UserWeaponSkillInfoStruct info;
    info.id			= ID_NONE;
    info.idowner	= this->GetID();
    info.nExp		= __max(0, nExp);
    info.nType		= nSkillType;
    info.ucLevel	= 1;
    info.ucOldLev	= 1;
    // check unlearn magic
    OBJID idNew = ID_NONE;
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT id FROM %s WHERE owner_id=%u && type=%d", _TBL_WEAPONSKILL, GetID(), nSkillType);
    CAutoPtr<IRecordset> pRes = _Database(PID)->CreateNewRecordset(szSQL);
    if (pRes)
    {
        idNew = pRes->GetInt(0);
    }
    CUserWeaponSkill* pSkill	= CUserWeaponSkill::InsertNewRecord(&info, GameDataDefault()->GetUserWeaponSkill(), idNew);
    IF_OK(pSkill)
    {
        m_pUserWeaponSkillSet->AddObj(pSkill);
        CMsgWeaponSkill msg;
        if (msg.Create(pSkill->GetType(), pSkill->GetLevel(), pSkill->GetExp()))
        {
            this->SendMsg(&msg);
        }
    }
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetWeaponSkillLev (OBJID idWeaponType)
{
    int nSkillType = (idWeaponType % 1000000) / 1000;
    int nWeaponSkills = m_pUserWeaponSkillSet->GetAmount();
    for (int i = 0; i < nWeaponSkills; i++)
    {
        CUserWeaponSkill* pSkill	= m_pUserWeaponSkillSet->GetObjByIndex(i);
        if (pSkill && pSkill->GetType() == nSkillType)
        {
            return pSkill->GetLevel();
        }
    }
    return 1;
}

//////////////////////////////////////////////////////////////////////
CUserWeaponSkill* CUser::GetWeaponSkill (int nSkillType)
{
    IF_NOT (m_pUserWeaponSkillSet)
    return NULL;
    int nWeaponSkills = m_pUserWeaponSkillSet->GetAmount();
    for (int i = 0; i < nWeaponSkills; i++)
    {
        CUserWeaponSkill* pSkill	= m_pUserWeaponSkillSet->GetObjByIndex(i);
        if (pSkill && pSkill->GetType() == nSkillType)
        {
            return pSkill;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
BOOL CUser::AwardWeaponSkill (int nSkillType, int nLev)
{
    CUserWeaponSkill* pSkill = this->GetWeaponSkill(nSkillType);
    if (pSkill)
    {
        pSkill->SetLevel(nLev, true);
        // update to client
        CMsgWeaponSkill msg;
        IF_OK (msg.Create(pSkill->GetType(), pSkill->GetLevel(), pSkill->GetExp()))
        this->SendMsg(&msg);
        SendSysMsg(STR_WEAPON_SKILL_UPLEV);
        return true;
    }
    else
    {
        // 玩家没有nSkillType这种武器熟练度，创建新记录。
        UserWeaponSkillInfoStruct info;
        info.id			= ID_NONE;
        info.idowner	= this->GetID();
        info.nExp		= 0;
        info.nType		= nSkillType;
        info.ucLevel	= nLev;
        info.ucOldLev	= 1;
        // check unlearn magic
        OBJID idNew = ID_NONE;
        SQLBUF	szSQL;
        sprintf(szSQL, "SELECT id FROM %s WHERE owner_id=%u && type=%d", _TBL_WEAPONSKILL, GetID(), nSkillType);
        CAutoPtr<IRecordset> pRes = _Database(PID)->CreateNewRecordset(szSQL);
        if (pRes)
        {
            idNew = pRes->GetInt(0);
        }
        CUserWeaponSkill* pSkill = CUserWeaponSkill::InsertNewRecord(&info, GameDataDefault()->GetUserWeaponSkill(), idNew);
        IF_OK (pSkill)
        {
            m_pUserWeaponSkillSet->AddObj(pSkill);
            // update to client
            CMsgWeaponSkill msg;
            IF_OK (msg.Create(pSkill->GetType(), pSkill->GetLevel(), pSkill->GetExp()))
            this->SendMsg(&msg);
        }
        return true;
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::SendAllWeaponSkillInfo()
{
    for (int i = 0; i < m_pUserWeaponSkillSet->GetAmount(); i++)
    {
        CUserWeaponSkill* pSkill	= m_pUserWeaponSkillSet->GetObjByIndex(i);
        if (pSkill)
        {
            CMsgWeaponSkill msg;
            if (msg.Create(pSkill->GetType(), pSkill->GetLevel(), pSkill->GetExp()))
            {
                this->SendMsg(&msg);
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////
bool CUser::UnlearnAllSkill()
{
    CHECKF(m_pUserWeaponSkillSet);
    int nWeaponSkills = m_pUserWeaponSkillSet->GetAmount();
    for (int i = nWeaponSkills - 1; i >= 0; i--)		// del obj
    {
        CUserWeaponSkill* pSkill	= m_pUserWeaponSkillSet->GetObjByIndex(i);
        if (pSkill)
        {
            pSkill->SetOldLev(pSkill->GetLevel(), UPDATE_FALSE);
            pSkill->SetUnlearn(true, UPDATE_TRUE);
            //pSkill->Update());
            //if (m_pMagic && m_pMagic->GetID() == pData->GetID())
            //	m_pMagic = NULL;
            int nType = pSkill->GetType();
            m_pUserWeaponSkillSet->DelObj(pSkill->GetID());
            // synchro
            CMsgAction	msg;
            IF_OK(msg.Create(this->GetID(), this->GetPosX(), this->GetPosY(), this->GetDir(), actionDropSkill, nType))
            this->SendMsg(&msg);
        }
    }
    return true;
}
//////////////////////////////////////////////////////////////////////
bool CUser::CreateAllFriend(IDatabase* pDb)
{
    SQLBUF		szSQL;
    sprintf(szSQL, "SELECT * FROM %s WHERE userid = %u LIMIT %d", _TBL_FRIEND, GetID(), _MAX_USERFRIENDSIZE);
    IRecordset*	pRes = pDb->CreateNewRecordset(szSQL);
    if (pRes)
    {
        for(int i = 0; i < pRes->RecordCount(); i++)
        {
            CFriendPtr	pFriend = CFriend::CreateNew();
            if (pFriend)
            {
                if (pFriend->Create(pRes))
                {
                    m_setFriend.push_back(pFriend);
                }
                else
                {
                    delete pFriend;
                }
            }
            pRes->MoveNext();
        }
        pRes->Release();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::DeleteAllFriend()
{
    for(int i = 0; i < m_setFriend.size(); i++)
    {
        SAFE_DELETE(m_setFriend[i]);
    }
    m_setFriend.clear();
}

//////////////////////////////////////////////////////////////////////
void CUser::SendFriendInfo()
{
    int	nAmount = m_setFriend.size();
    CHECK(nAmount <= _MAX_USERFRIENDSIZE);
    for(int i = 0; i < nAmount; i++)
    {
        CFriend* pFriend = m_setFriend[i];
        if (pFriend)
        {
            OBJID	idFriend	= pFriend->GetFriendID();
            CUser* pFriendUser	= UserManager()->GetUser(idFriend);
            bool	bOnline		= pFriendUser != 0;
            DWORD	dwLookFace	= pFriendUser ? pFriendUser->GetLookFace() : 0;
            int		nLevel		= pFriendUser ? pFriendUser->GetLev() : 0;
            CMsgFriend	msg;
            if (msg.Create(_FRIEND_GETINFO, idFriend, pFriend->GetFriendName(), bOnline, dwLookFace, nLevel))
            {
                SendMsg(&msg);
            }
            if (!bOnline)
            {
                MapGroup(PID)->QueryIntraMsg()->TransmitMsg(&msg, GetSocketID(), GetID());    // 通知其它地图组，下传给我好友在线的消息
            }
        }
    }
    // 通过WORLD核心转发给其它在线玩家
    for(int i = 0; i < nAmount; i++)
    {
        CFriend* pFriend = m_setFriend[i];
        if (pFriend)
        {
            OBJID	idFriend	= pFriend->GetFriendID();
            CMsgFriend	msg;
            IF_OK(msg.Create(_FRIEND_ONLINE, GetID(), GetName(), CMsgFriend::ONLINE_STATUS, GetLookFace(), GetLev()))
            MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, idFriend);
        }
    }
    return;
}

//////////////////////////////////////////////////////////////////////
void CUser::BroadcastFriendsMsg	(CNetMsg* pMsg)
{
    if (!pMsg)
    {
        return;
    }
    int	nAmount = m_setFriend.size();
    CHECK(nAmount <= _MAX_USERFRIENDSIZE);
    for(int i = 0; i < nAmount; i++)
    {
        CFriend* pFriend = m_setFriend[i];
        if (pFriend)
        {
            OBJID	idFriend	= pFriend->GetFriendID();
            CUser* pUserFriend = UserManager()->GetUser(idFriend);
            if (pUserFriend)
            {
                pUserFriend->SendMsg(pMsg);
            }
            else
            {
                MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pMsg, pFriend->GetFriendID());
            }
        }
    }
    // 通过WORLD核心查其它在线玩家
    return;
}

//////////////////////////////////////////////////////////////////////
// mapgroup
//////////////////////////////////////////////////////////////////////
bool CUser::ChangeMapGroup(PROCESS_ID idProcess, OBJID idMap, int nPosX, int nPosY)					// call - delete this;
{
    SaveInfo();		// SaveAllInfo
#ifdef	PALED_DEBUG
    SendSysMsg("从地图[%s][%d][%d]切屏到[%d][%d]...", GetMap()->GetName(), GetMapID(), m_idProcess - 3, idMap, idProcess - 3);
#endif
    // transformation
    m_data.m_Info.dwMonsterType	= 0;
    m_data.m_Info.dwMonsterSecs = m_tTransformation.GetRemain();
    if (QueryTransformation() && m_data.m_Info.dwMonsterSecs > 0)
    {
        m_data.m_Info.dwMonsterType	= QueryTransformation()->GetMonsterType();
    }
    MapGroup(PID)->QueryIntraMsg()->SendSocketUserInfo(idProcess, m_idSocket, GetInfo());
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItem* pEquip = GetEquipItemByPos(i);
        if (pEquip)
        {
            ItemInfoStruct info;
            pEquip->GetInfo(&info);
            MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_ITEM, &info, sizeof(info));
        }
    }
    m_pPackage->SendAllObjInfo(MapGroup(PID), idProcess);
    /*for(int i = 0; i < m_setItem.size(); i++)
    {
    	CItemPtr pItem = m_setItem[i];
    	if (pItem)
    	{
    		ItemInfoStruct info;
    		pItem->GetInfo(&info);
    		MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_ITEM, &info, sizeof(info));
    	}
    }*/
    for(int i = 0; i < m_setFriend.size(); i++)
    {
        CFriendPtr pFriend = m_setFriend[i];
        if (pFriend)
        {
            MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_FRIEND, pFriend->GetInfo(), sizeof(FriendInfoStruct));
        }
    }
    {
        CEnemyInfoStruct	info;
        if (QueryEnemy()->GetInfo(&info))
        {
            MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_ENEMY, &info, sizeof(CEnemyInfoStruct));
        }
    }
    for(int i = 0; i < QueryWeapenSkill()->GetAmount(); i++)
    {
        CUserWeaponSkill* pSkill = QueryWeapenSkill()->GetObjByIndex(i);
        if (pSkill)
        {
            MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_WEAPONSKILL, pSkill->GetInfo(), sizeof(UserWeaponSkillInfoStruct));
        }
    }
    if (QueryMagic())
    {
        for(int i = 0; i < QueryMagic()->QuerySet()->GetAmount(); i++)
        {
            MagicInfoStruct	info;
            if (QueryMagic()->GetInfoByIndex(&info, i))
            {
                MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_MAGIC, &info, sizeof(info));
            }
        }
    }
    {
        SynAttrInfoStruct info;
        QuerySynAttr()->GetInfo(&info);
        MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_SYNATTR, &info, sizeof(info));
    }
    // status info
    for(int i = 0; i < QueryStatusSet()->GetAmount(); i++)
    {
        IStatus* pStatus = QueryStatusSet()->GetObjByIndex(i);
        if (pStatus)
        {
            StatusInfoStruct	info;
            if (pStatus->GetInfo(&info))					//? 无效INFO不发送
            {
                MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_STATUS, &info, sizeof(info));
            }
        }
    }
    if (GetTeam())
    {
        TeamInfoStruct info;
        GetTeam()->GetInfo(&info);
        MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_TEAM, &info, sizeof(info));
        ClrTeam();			//? LeaveMapGroup()时不退出队伍
    }
    MapGroup(PID)->QueryIntraMsg()->ChangeMapGroup(idProcess, GetID(), idMap, nPosX, nPosY);
    //析购时调用	LeaveMapGroup();
    UserManager()->LogoutUser(GetSocketID(), false);	// 删除对象，进入另一地图组, false: not offline
    {
        // stat
        extern struct STAT_STRUCT	g_stat;
        InterlockedIncrement(&g_stat.setDebug[3]);		// debug
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::LeaveMapGroup()
{
    //	ASSERT(m_nStep == STEP_ENTERMAP);
    // leave map first(加速离开地图，后面会重复发送此消息)
    CMsgAction msg;
    if (msg.Create(GetID(), 0, 0, 0, actionLeaveMap))
    {
        this->BroadcastRoomMsg(&msg, EXCLUDE_SELF);
    }
    DestroyBooth();
    if (m_pTeam)
    {
        this->QuitTeam();
    }
    /*
    	if (IsAlive())
    	{
    		if (GetMap()->IsStandEnable(GetPosX(), GetPosY()) && !GetMap()->IsRecordDisable())
    			this->SetRecordPos(GetMap()->GetID(), GetPosX(), GetPosY());
    	}
    */
    SaveInfo();		// SaveAllInfo
    // 必须在DeleteAllItem之前收回幻兽操作
    CallBackAllEudemon();
    // 切换地图不需要与幻兽解体
    DeleteAllTutor();
    DeleteAllFriend();
    DeleteAllItem();
    LeaveMap();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::JoinMap(OBJID idMap, int nPosX, int nPosY)
{
    m_pMap		= MapManager()->GetGameMap(idMap);
    m_nPosX		= nPosX;
    m_nPosY		= nPosY;
    // 发送回应消息
    CMsgAction	msg;
    if (msg.Create(GetID(), nPosX, nPosY, GetDir(), actionFlyMap, m_pMap->GetDocID()))
    {
        SendMsg(&msg);
    }
    EnterMap();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::AppendObjInfo(INFO_ID nType, void* pInfo)
{
    switch(nType)
    {
    case	INFO_ITEM:
        AppendItem((ItemInfoStruct*)pInfo);
        break;
    case	INFO_FRIEND:
        AppendFriend((FriendInfoStruct*)pInfo);
        break;
    case	INFO_ENEMY:
        QueryEnemy()->AppendInfo((CEnemyInfoStruct*)pInfo);
        break;
    case	INFO_WEAPONSKILL:
        AppendWeaponSkill((UserWeaponSkillInfoStruct*)pInfo);
        break;
    case	INFO_MAGIC:
        if (QueryMagic())
        {
            QueryMagic()->AppendMagic((MagicInfoStruct*)pInfo);
        }
        break;
    case	INFO_SYNATTR:
        QuerySynAttr()->AppendSynAttr((SynAttrInfoStruct*)pInfo);
        break;
    case	INFO_STATUS:
        AppendStatus((StatusInfoStruct*)pInfo);
        break;
    case	INFO_TEAM:
        AppendTeam((TeamInfoStruct*)pInfo);
        break;
    default:
        ASSERT(!"switch INFO_TYPE");
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::AppendItem(ItemInfoStruct* pInfo)
{
    CHECKF(pInfo->idOwner == GetID());
    CItemPtr pItem = CItem::CreateNew();
    IF_NOT(pItem)
    return false;
    IF_NOT(pItem->Create(GameDataDefault()->GetGameItemData(), pInfo, false))
    {
        pItem->ReleaseByOwner();
        return false;
    }
    if (pInfo->nPosition >= ITEMPOSITION_EQUIPBEGIN && pInfo->nPosition < ITEMPOSITION_EQUIPEND)
    {
        CItemPtr& refpEquip = this->GetEquipItemRef(pInfo->nPosition);
        IF_OK(!refpEquip)
        {
            refpEquip = pItem;
            pItem = NULL;		// 不放入背包
        }
    }
    if (pItem)
    {
        m_pPackage->AddItem(pItem);
    }
    //	m_setItem.push_back(pItem);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::AppendFriend(FriendInfoStruct* pInfo)
{
    CHECKF(pInfo->idUser == GetID());
    CFriendPtr pFriend;
    pFriend = CFriend::CreateNewFriend(pInfo);
    if (!pFriend)
    {
        return false;
    }
    m_setFriend.push_back(pFriend);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::AppendWeaponSkill(UserWeaponSkillInfoStruct* pInfo)
{
    CHECKF(pInfo->idowner == GetID());
    CUserWeaponSkill*	pSkill	= CUserWeaponSkill::CreateNew(pInfo, GameDataDefault()->GetUserWeaponSkill());
    if (!pSkill)
    {
        return false;
    }
    m_pUserWeaponSkillSet->AddObj(pSkill);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::AppendStatus(StatusInfoStruct* pInfo)
{
    if (pInfo->nTimes > 0)		// more status
    {
        CStatusMore* pStatus = CStatusMore::CreateNew();
        if (pStatus)
        {
            if (pStatus->Create(QueryRole(), pInfo->nStatus, pInfo->nPower, pInfo->nSecs, pInfo->nTimes))
            {
                QueryStatusSet()->AddObj(pStatus->QueryInterface());
            }
            else
            {
                pStatus->ReleaseByOwner();
            }
        }
    }
    else
    {
        CStatusOnce* pStatus = CStatusOnce::CreateNew();
        if (pStatus)
        {
            if (pStatus->Create(QueryRole(), pInfo->nStatus, pInfo->nPower, pInfo->nSecs))
            {
                QueryStatusSet()->AddObj(pStatus->QueryInterface());
            }
            else
            {
                pStatus->ReleaseByOwner();
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
// money
//////////////////////////////////////////////////////////////////////
bool CUser::SpendMoney(DWORD dwMoney, bool bSynchro)
{
    if (dwMoney == 0)
    {
        return true;
    }
    if (this->GetMoney() < dwMoney)
    {
        return false;
    }
    DWORD dwNewMoney = this->GetMoney() - dwMoney;
    m_data.SetMoney(dwNewMoney);
    // update to client...
    if (bSynchro)
    {
        CMsgUserAttrib	msg;
        if (msg.Create(this->GetID(), _USERATTRIB_MONEY, dwNewMoney))
        {
            SendMsg(&msg);
        }
    }
    return true;
}

bool CUser::GainMoney(DWORD dwMoney, bool bSynchro /*= true*/)
{
    if (dwMoney == 0)
    {
        return true;
    }
    if (dwMoney < 0)
    {
        LOGWARNING("GainMoney: dwMoney < 0");
    }
    DWORD dwNewMoney = this->GetMoney() + dwMoney;
    if (dwNewMoney > _MAX_MONEYLIMIT)
    {
        return false;
    }
    m_data.SetMoney(dwNewMoney);
    // update to client...
    if (bSynchro)
    {
        CMsgUserAttrib	msg;
        if (msg.Create(this->GetID(), _USERATTRIB_MONEY, dwNewMoney))
        {
            SendMsg(&msg);
        }
    }
    return true;
}

bool CUser::SaveMoney(DWORD dwMoney, bool bSynchro /*= true*/)
{
    if (this->GetMoney() < dwMoney)
    {
        this->SendSysMsg(STR_NOT_SO_MUCH_MONEY);
        return false;
    }
    DWORD dwNewMoney = this->GetMoney() - dwMoney;
    DWORD dwNewMoneySaved = this->GetMoneySaved() + dwMoney;
    if (dwNewMoneySaved > _MAX_MONEYLIMIT)
    {
        this->SendSysMsg(STR_TOOMUCH_MONEY_SAVED);
        return false;
    }
    m_data.SetMoney(dwNewMoney);
    m_data.SetMoneySaved(dwNewMoneySaved);
    // update to client...
    if (bSynchro)
    {
        CMsgUserAttrib	msg;
        if (msg.Create(this->GetID(), _USERATTRIB_MONEY, dwNewMoney))
        {
            msg.Append(_USERATTRIB_MONEYSAVED, this->GetMoneySaved());
            SendMsg(&msg);
        }
    }
    return true;
}

bool CUser::DrawMoney(DWORD dwMoney, bool bSynchro /*= true*/)
{
    if (this->GetMoneySaved() < dwMoney)
    {
        SendSysMsg(STR_NOT_SO_MUCH_MONEY);
        return false;
    }
    DWORD dwNewMoney = this->GetMoney() + dwMoney;
    DWORD dwNewMoneySaved = this->GetMoneySaved() - dwMoney;
    if (dwNewMoney > _MAX_MONEYLIMIT)
    {
        SendSysMsg(STR_TOOMUCH_MONEY_TO_DRAW);
        return false;
    }
    m_data.SetMoney(dwNewMoney);
    m_data.SetMoneySaved(dwNewMoneySaved);
    // update to client...
    if (bSynchro)
    {
        CMsgUserAttrib	msg;
        if (msg.Create(this->GetID(), _USERATTRIB_MONEY, dwNewMoney))
        {
            msg.Append(_USERATTRIB_MONEYSAVED, this->GetMoneySaved());
            SendMsg(&msg);
        }
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
// role
/////////////////////////////////////////////////////////////////////////////
void CUser::SendShow(IRole* pRole)
{
    CMsgPlayer	msg;
    if (msg.Create((IRole*)this))
    {
        pRole->SendMsg(&msg);
    }
}

/////////////////////////////////////////////////////////////////////////////
void CUser::SetStatus(int nStatus, bool bSynchro)
{
    I64 i64OldEffect = GetEffect();
    switch (nStatus)
    {
    case STATUS_ADJUST_DODGE:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_ADJUST_DODGE;
        }
        break;
    case STATUS_ADJUST_XP:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_ADJUST_XP;
        }
        break;
    case STATUS_ADJUST_DROPMONEY:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_ADJUST_DROPMONEY;
        }
        break;
    case	STATUS_NORMAL:           //change huang 2004.1.11
        {
            m_data.m_Info.dwStatus	= nStatus;
            m_data.m_Info.i64Effect	= KEEPEFFECT_NORMAL;
            if (m_data.GetPk() >= PKVALUE_BLACKNAME)
            {
                m_data.m_Info.i64Effect	|= KEEPEFFECT_DEEPRED;
            }
            else if (m_data.GetPk() >= PKVALUE_REDNAME)
            {
                m_data.m_Info.i64Effect	|= KEEPEFFECT_RED;
            }
            CTeam* pTeam = GetTeam();
            if (pTeam && pTeam->GetLeader() == GetID())
            {
                m_data.m_Info.i64Effect	|= KEEPEFFECT_TEAMLEADER;
            }
        }
        break;
    case	STATUS_DIE:
        {
            m_data.m_Info.dwStatus	= nStatus;
            m_data.m_Info.i64Effect	&= KEEPEFFECT_NOT_CLEAR;
            m_data.m_Info.i64Effect	|= KEEPEFFECT_DIE;
            ClearAllStatus();
        }
        break;
    case	STATUS_TEAMLEADER:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_TEAMLEADER;
        }
        break;
    case	STATUS_CRIME:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_CRIME;
        }
        break;
    case	STATUS_SYNCRIME:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_SYNCRIME;
        }
        break;
    case	STATUS_POISON:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_POISON;
        }
        break;
    case	STATUS_PKVALUE:         //change huang 2004.1.11
        {
            // 取消黄名状态，保留黑名、红名
            if (m_data.GetPk() >= PKVALUE_BLACKNAME)
            {
                m_data.m_Info.i64Effect	|= KEEPEFFECT_DEEPRED;
            }
            else if (m_data.GetPk() >= PKVALUE_REDNAME)
            {
                m_data.m_Info.i64Effect	|= KEEPEFFECT_RED;
            }
        }
        break;
    case	STATUS_DEFENCE1:
        {
            m_data.m_Info.i64Effect	&= ~(KEEPEFFECT_DEFENCE2 | KEEPEFFECT_DEFENCE3);
            m_data.m_Info.i64Effect	|= KEEPEFFECT_DEFENCE1;
        }
        break;
    case	STATUS_DEFENCE2:
        {
            m_data.m_Info.i64Effect	&= ~(KEEPEFFECT_DEFENCE1 | KEEPEFFECT_DEFENCE3);
            m_data.m_Info.i64Effect	|= KEEPEFFECT_DEFENCE2;
        }
        break;
    case	STATUS_DEFENCE3:
        {
            m_data.m_Info.i64Effect	&= ~(KEEPEFFECT_DEFENCE1 | KEEPEFFECT_DEFENCE2);
            m_data.m_Info.i64Effect	|= KEEPEFFECT_DEFENCE3;
        }
        break;
    case	STATUS_FREEZE:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_FREEZE;
        }
        break;
    case	STATUS_SMOKE:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_SMOKE;
        }
        break;
    case	STATUS_DARKNESS:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_DARKNESS;
        }
        break;
    case	STATUS_PALSY:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_PALSY;
        }
        break;
    case	STATUS_ATTACK:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_ATTACK;
        }
        break;
    case	STATUS_MAGICDEFENCE:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_MAGICDEFENCE;
        }
        break;
    case	STATUS_REFLECT:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_REFLECT;
        }
        break;
    case	STATUS_REFLECTMAGIC:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_REFLECTMAGIC;
        }
        break;
    case	STATUS_MAGICDAMAGE:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_MAGICDAMAGE;
        }
        break;
    case	STATUS_ATKSPEED:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_ATKSPEED;
        }
        break;
    case	STATUS_LURKER:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_LURKER;
        }
        break;
    case	STATUS_SLOWDOWN1:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_SLOWDOWN1;
        }
        break;
    case	STATUS_SLOWDOWN2:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_SLOWDOWN2;
        }
        break;
    case	STATUS_MAXLIFE:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_MAXLIFE;
        }
        break;
    case	STATUS_MAXENERGY:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_MAXENERGY;
        }
        break;
    case	STATUS_HIDDEN:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_HIDDEN;
        }
        break;
    case	STATUS_ADD_EXP:
        {
            m_data.m_Info.i64Effect	|= KEEPEFFECT_ADD_EXP;
        }
        break;
    case	STATUS_ATTRACT_MONSTER:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_ATTRACT_MONSTER;
        }
        break;
    case	STATUS_XPFULL:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_XPFULL;
        }
        break;
    case	STATUS_PK_PROTECT:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_PK_PROTECT;
        }
        break;
    case	STATUS_PELT:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_PELT;
        }
        break;
    case	STATUS_ADJUST_EXP:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_ADJUST_EXP;
        }
        break;
        // 以下是结界系统相关的状态 ================
    case	STATUS_TEAMHEALTH:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAM_ALL;
            m_data.m_Info.i64Effect	|= KEEPEFFECT_TEAM_HEALTH;
        }
        break;
    case	STATUS_TEAMATTACK:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAM_ALL;
            m_data.m_Info.i64Effect	|= KEEPEFFECT_TEAM_ATTACK;
        }
        break;
    case	STATUS_TEAMDEFENCE:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAM_ALL;
            m_data.m_Info.i64Effect	|= KEEPEFFECT_TEAM_DEFENCE;
        }
        break;
    case	STATUS_TEAMSPEED:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAM_ALL;
            m_data.m_Info.i64Effect	|= KEEPEFFECT_TEAM_SPEED;
        }
        break;
    case	STATUS_TEAMEXP:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAM_ALL;
            m_data.m_Info.i64Effect	|= KEEPEFFECT_TEAM_EXP;
        }
        break;
    case	STATUS_TEAMSPIRIT:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAM_ALL;
            m_data.m_Info.i64Effect	|= KEEPEFFECT_TEAM_SPIRIT;
        }
        break;
    case	STATUS_TEAMCLEAN:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAM_ALL;
            m_data.m_Info.i64Effect	|= KEEPEFFECT_TEAM_CLEAN;
        }
        break;
        // 以上是结界系统相关状态 ===============
    case	STATUS_HEAL:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_HEAL;
        }
        break;
    case	STATUS_FAINT:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_FAINT;
        }
        break;
    case	STATUS_TRUCULENCE:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_TRUCULENCE;
        }
        break;
    case	STATUS_DAMAGE:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_DAMAGE;
        }
        break;
    case	STATUS_ATKER_DAMAGE:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_ATKER_DAMAGE;
        }
        break;
    case	STATUS_FRENZY:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_FRENZY;
        }
        break;
    case	STATUS_TRANSFER_SHIELD:
        {
            m_data.m_Info.i64Effect |= KEEPEFFECT_TRANSFER_SHIELD;
        }
        break;
    case	STATUS_LOCK:
    case	STATUS_SUPER_MATK:
        break;
    default:
        ::LogSave("Invalid status: [%u]", nStatus);
        //		ASSERT(!"switch status");
    }
    if (STATUS_FREEZE == nStatus || STATUS_FAINT == nStatus)
    {
        this->ClrAttackTarget();	// 停止自动战斗
    }
    if (bSynchro && i64OldEffect != GetEffect())
    {
        CMsgUserAttrib msg;
        if (msg.Create(GetID(), _USERATTRIB_KEEPEFFECT, GetEffect()))
        {
            // 速度结界，增加移动速度
            // 增加移动速度
            if (STATUS_TEAMSPEED == nStatus || STATUS_SLOWDOWN1 == nStatus || STATUS_SLOWDOWN2 == nStatus
                    || STATUS_PELT == nStatus)
            {
                // 只是通知客户端修改移动速度，并不真的改变移动速度
                msg.Append(_USERATTRIB_SPEED, AdjustSpeed(this->GetSpeed()));
            }
            else if (STATUS_MAXLIFE == nStatus)
            {
                msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
                msg.Append(_USERATTRIB_LIFE, this->GetLife());
            }
            else if (STATUS_MAXENERGY == nStatus)
            {
                msg.Append(_USERATTRIB_MAXENERGY, this->GetMaxEnergy());
                msg.Append(_USERATTRIB_ENERGY, this->GetEnergy());
            }
            BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
void CUser::ClsStatus(int nStatus, bool bSynchro)
{
    I64 i64OldEffect = GetEffect();
    switch (nStatus)
    {
    case STATUS_ADJUST_DODGE:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_ADJUST_DODGE;
        }
        break;
    case STATUS_ADJUST_XP:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_ADJUST_XP;
        }
        break;
    case STATUS_ADJUST_DROPMONEY:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_ADJUST_DROPMONEY;
        }
        break;
    case	STATUS_NORMAL:
        {
            ASSERT(!"STATUS_NORMAL");
        }
        break;
    case	STATUS_DIE:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_DIE;
        }
        break;
    case	STATUS_TEAMLEADER:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAMLEADER;
        }
        break;
    case	STATUS_CRIME:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_CRIME;
        }
        break;
    case	STATUS_SYNCRIME:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_SYNCRIME;
        }
        break;
    case	STATUS_POISON:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_POISON;
        }
        break;
    case	STATUS_PKVALUE:        // change huang 2004.1.11  todo
        {
            // 取消黄名状态，保留黑名、红名
            if (m_data.GetPk() < PKVALUE_REDNAME)
            {
                m_data.m_Info.i64Effect	&= ~(KEEPEFFECT_RED | KEEPEFFECT_DEEPRED);
            }
            else if (m_data.GetPk() < PKVALUE_BLACKNAME)
            {
                m_data.m_Info.i64Effect	&= ~KEEPEFFECT_DEEPRED;
                m_data.m_Info.i64Effect	|= KEEPEFFECT_RED;
            }
        }
        break;
    case	STATUS_DEFENCE1:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_DEFENCE1;
        }
        break;
    case	STATUS_DEFENCE2:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_DEFENCE2;
        }
        break;
    case	STATUS_DEFENCE3:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_DEFENCE3;
        }
        break;
    case	STATUS_FREEZE:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_FREEZE;
        }
        break;
    case	STATUS_SMOKE:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_SMOKE;
        }
        break;
    case	STATUS_DARKNESS:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_DARKNESS;
        }
        break;
    case	STATUS_PALSY:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_PALSY;
        }
        break;
    case	STATUS_ATTACK:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_ATTACK;
        }
        break;
    case	STATUS_MAGICDEFENCE:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_MAGICDEFENCE;
        }
        break;
    case	STATUS_REFLECT:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_REFLECT;
        }
        break;
    case	STATUS_REFLECTMAGIC:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_REFLECTMAGIC;
        }
        break;
    case	STATUS_MAGICDAMAGE:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_MAGICDAMAGE;
        }
        break;
    case	STATUS_ATKSPEED:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_ATKSPEED;
        }
        break;
    case	STATUS_LURKER:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_LURKER;
        }
        break;
    case	STATUS_SLOWDOWN1:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_SLOWDOWN1;
        }
        break;
    case	STATUS_SLOWDOWN2:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_SLOWDOWN2;
        }
        break;
    case	STATUS_MAXLIFE:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAXLIFE;
        }
        break;
    case	STATUS_MAXENERGY:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAXENERGY;
        }
        break;
    case	STATUS_HIDDEN:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_HIDDEN;
        }
        break;
    case	STATUS_ADD_EXP:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_ADD_EXP;
        }
        break;
    case	STATUS_ATTRACT_MONSTER:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_ATTRACT_MONSTER;
        }
        break;
    case	STATUS_XPFULL:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_XPFULL;
            ClsXpVal(SYNCHRO_FALSE);
        }
        break;
    case	STATUS_PK_PROTECT:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_PK_PROTECT;
        }
        break;
    case	STATUS_PELT:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_PELT;
        }
        break;
    case	STATUS_ADJUST_EXP:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_ADJUST_EXP;
        }
        break;
        // 以下是结界系统相关的状态 ================
    case	STATUS_TEAMHEALTH:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAM_HEALTH;
        }
        break;
    case	STATUS_TEAMATTACK:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAM_ATTACK;
        }
        break;
    case	STATUS_TEAMDEFENCE:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAM_DEFENCE;
        }
        break;
    case	STATUS_TEAMSPEED:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAM_SPEED;
        }
        break;
    case	STATUS_TEAMEXP:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAM_EXP;
        }
        break;
    case	STATUS_TEAMSPIRIT:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAM_SPIRIT;
        }
        break;
    case	STATUS_TEAMCLEAN:
        {
            m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAM_CLEAN;
        }
        break;
        // 以上是结界系统相关状态 ===============
    case	STATUS_HEAL:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_HEAL;
        }
        break;
    case	STATUS_FAINT:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_FAINT;
        }
        break;
    case	STATUS_TRUCULENCE:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_TRUCULENCE;
        }
        break;
    case	STATUS_DAMAGE:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_DAMAGE;
        }
        break;
    case	STATUS_ATKER_DAMAGE:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_ATKER_DAMAGE;
        }
        break;
    case	STATUS_FRENZY:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_FRENZY;
        }
        break;
    case	STATUS_TRANSFER_SHIELD:
        {
            m_data.m_Info.i64Effect &= ~KEEPEFFECT_TRANSFER_SHIELD;
        }
        break;
    case	STATUS_LOCK:
    case	STATUS_SUPER_MATK:
        break;
    default:
        ::LogSave("Invalid status: [%u]", nStatus);
        //		ASSERT(!"switch status");
    }
    if (nStatus == STATUS_LOCK)
    {
        CMsgAction	msg;
        if (msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionUnlockUser))
        {
            BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
    }
    if (bSynchro && i64OldEffect != GetEffect())
    {
        CMsgUserAttrib msg;
        if (msg.Create(GetID(), _USERATTRIB_KEEPEFFECT, GetEffect()))
        {
            // 撤消速度结界，恢复原来的移动速度
            if (STATUS_TEAMSPEED == nStatus || STATUS_SLOWDOWN1 == nStatus || STATUS_SLOWDOWN2 == nStatus
                    || STATUS_PELT == nStatus)
            {
                msg.Append(_USERATTRIB_SPEED, AdjustSpeed(this->GetSpeed()));
            }
            else if (STATUS_MAXLIFE == nStatus)
            {
                msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
                msg.Append(_USERATTRIB_LIFE, this->GetLife());
            }
            else if (STATUS_MAXENERGY == nStatus)
            {
                msg.Append(_USERATTRIB_MAXENERGY, this->GetMaxEnergy());
                msg.Append(_USERATTRIB_ENERGY, this->GetEnergy());
            }
            BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
void CUser::IncAddPoint(int nAddPoint)
{
    int nNewPoint	= nAddPoint + m_data.GetAddPoint();
    if (nNewPoint < 0)
    {
        nNewPoint	= 0;
    }
    m_data.SetAddPoint(nNewPoint);
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::IsAlive()
{
    if ((GetStatus() & STATUS_DIE) == STATUS_DIE || this->GetLife() <= 0)
    {
        return false;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
void CUser::SetForce(DWORD dwForce)
{
    m_data.SetForce(dwForce);
}

/////////////////////////////////////////////////////////////////////////////
void CUser::SetDexterity(DWORD dwDex)
{
    m_data.SetDexterity(dwDex);
}

/////////////////////////////////////////////////////////////////////////////
void CUser::SetSpeed(DWORD dwSpeed)
{
    m_data.SetSpeed(dwSpeed);
}

/////////////////////////////////////////////////////////////////////////////
void CUser::SetSoul(DWORD dwSoul)
{
    m_data.SetSoul(dwSoul);
}

/////////////////////////////////////////////////////////////////////////////
void CUser::SetHealth(DWORD dwHealth)
{
    m_data.SetHealth(dwHealth);
}

/////////////////////////////////////////////////////////////////////////////
void CUser::SetFace(DWORD dwFace)
{
    DWORD dwLookFace = m_data.GetLookFace();
    dwLookFace = (dwLookFace - this->GetFace() * 10000) + (dwFace % 1000) * 10000;
    m_data.SetLookFace(dwLookFace, true);
}

/////////////////////////////////////////////////////////////////////////////
void CUser::SetLook(DWORD dwLook)
{
    DWORD dwLookFace = m_data.GetLookFace();
    dwLookFace = (dwLookFace / 1000) * 1000 + (dwLook % 1000);
    m_data.SetLookFace(dwLookFace, true);
}

/*////////////////////////////////////////////////////////////////////////////
BOOL CUser::CheckStatus(int nStatus)
{
	if (nStatus == STATUS_NORMAL || STATUS_DIE == nStatus)
	{
		if (m_infoStatus.dwStatus == nStatus)
			return true;
		else
			return false;
	}
	else
	{
		if ((m_infoStatus.dwStatus & nStatus) == nStatus)
			return true;
		else
			return false;
	}
}*/

/////////////////////////////////////////////////////////////////////////////
bool CUser::IsJumpPass(int x, int y, int nAlt)
{
    vector<POINT>	setLine;
    DDALineEx(GetPosX(), GetPosY(), x, y, setLine);
    int nSize = setLine.size();
    if (nSize <= 2)
    {
        return true;
    }
    ASSERT(x == setLine[nSize-1].x);
    ASSERT(y == setLine[nSize-1].y);
    float	fAlt	= (float)(GetMap()->GetFloorAlt(GetPosX(), GetPosY()) + nAlt) + 0.5;
    float	fDelta	= (float)(GetMap()->GetFloorAlt(x, y) - fAlt) / nSize;
    for(int i = 0; i < nSize; i++)
    {
        if (GetMap()->IsAltOver(setLine[i], (int)fAlt))
        {
            return false;
        }
        //		fAlt	+= fDelta;
    }
    return true;
}
/////////////////////////////////////////////////////////////////////////////
bool CUser::IsArrowPass(int x, int y, int nAlt/*=ARROW_FLY_ALT*/)
{
    return true;		//? 暂不检查 /////////////////////////////////////////////////////////////
    vector<POINT>	setLine;
    DDALineEx(GetPosX(), GetPosY(), x, y, setLine);
    int nSize = setLine.size();
    if (nSize <= 2)
    {
        return true;
    }
    ASSERT(x == setLine[nSize-1].x);
    ASSERT(y == setLine[nSize-1].y);
    float	fAlt	= (float)(GetMap()->GetFloorAlt(GetPosX(), GetPosY()) + nAlt) + 0.5;
    float	fDelta	= (float)(GetMap()->GetFloorAlt(x, y) - fAlt) / nSize;
    for(int i = 0; i < nSize; i++)
    {
        if (GetMap()->IsAltOver(setLine[i], (int)fAlt))
        {
            return false;
        }
        fAlt	+= fDelta;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
// posCenter: 玩家自己(不是posSource!!!)向posSource的引线为中心线
// posSource：扇形中心点
// nWidth: 1 - 359
/*
bool CUser::IsInFan(POINT pos, POINT posSource, int nRange, int nWidth, POINT posCenter)
{
	CHECKF(nWidth > 0 && nWidth < 360);

	if (posCenter.x == GetPosX() && posCenter.y == GetPosY())
		return false;
	if (pos.x == posSource.x && pos.y == posSource.y)
		return false;

	if (GetDistance(pos.x, pos.y) > nRange)
		return false;

	float PI = static_cast<float>(3.1415926535);
	float fRadianDelta	= (PI * nWidth / 180) / 2;
	float fCenterLine	= GetRadian(GetPosX(), GetPosY(), posCenter.x, posCenter.y);
	float fTargetLine	= GetRadian(posSource.x, posSource.y, pos.x, pos.y);
	float fDelta	= fabs(fCenterLine - fTargetLine);
	if (fDelta <= fRadianDelta || fDelta >= 2*PI - fRadianDelta)
		return true;

	return false;
}
*/

/////////////////////////////////////////////////////////////////////////////
bool	CUser::EquipMagicItem(CItem* pItem, int nPosition)
{
    int idxMagic = pItem->GetInt(ITEMDATA_MAGIC1);
    if (idxMagic != MAGIC_NONE && QueryMagic())
    {
        ASSERT(QueryMagic()->EquipWeaponMagic(POSITION2MAGIC_ID(nPosition), MagicType(idxMagic), WEAPONMAGIC_LEVEL));
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
bool	CUser::UnequipMagicItem(int nPosition)
{
    OBJID idMagic = POSITION2MAGIC_ID(nPosition);
    if (QueryMagic())
    {
        QueryMagic()->UnequipWeaponMagic(idMagic);
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
void	CUser::SynchroSupermanOrder(int nOrder)
{
    m_nSupermanOrder	= nOrder;
    // synchro
    SetAttrib(_USERATTRIB_SUPERMAP_ORDER, nOrder, SYNCHRO_TRUE);
}

/////////////////////////////////////////////////////////////////////////////
void	CUser::CalcSupermanKills()
{
    if (IsGM() || m_nCurrSupermanKills < MIN_SUPERMAP_KILLS || m_nCurrSupermanKills <= m_nTopSupermanKills)
    {
        m_nCurrSupermanKills	= 0;
        return ;
    }
    m_nTopSupermanKills		= m_nCurrSupermanKills;
    m_nCurrSupermanKills	= 0;
    char	buf[sizeof(long)*2];
    VARIANT_SET0* pInfo = (VARIANT_SET0*)buf;
    pInfo->IntParam(0)	= GetID();
    pInfo->IntParam(1)	= m_nTopSupermanKills;
    MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, KERNEL_SUPERMANLIST, STRUCT_TYPE(buf), buf);
}

/////////////////////////////////////////////////////////////////////////////
void	CUser::QuerySupermanSecs()
{
    char	buf[sizeof(long)*2];
    VARIANT_SET0* pInfo = (VARIANT_SET0*)buf;
    pInfo->IntParam(0)	= GetID();
    pInfo->IntParam(1)	= 0;			// 0: for query
    MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, KERNEL_SUPERMANLIST, STRUCT_TYPE(buf), buf);
}

/////////////////////////////////////////////////////////////////////////////
void	CUser::QuerySupermanList(int nIndex, OBJID idAction, int nNumber)
{
    char	buf[sizeof(long)*4];
    VARIANT_SET0* pInfo = (VARIANT_SET0*)buf;
    pInfo->IntParam(0)	= GetID();
    pInfo->IntParam(1)	= nIndex;
    pInfo->IntParam(2)	= idAction;
    pInfo->IntParam(3)	= nNumber;
    MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, KERNEL_QUERYSUPERMANLIST, STRUCT_TYPE(buf), buf);
}

/////////////////////////////////////////////////////////////////////////////
CUser*	CUser::FindAroundUser(OBJID id)
{
    IRole* pRole = FindAroundRole(id);
    CUser* pUser = NULL;
    if (pRole)
    {
        pRole->QueryObj(OBJ_USER, IPP_OF(pUser));
    }
    return pUser;
}

/////////////////////////////////////////////////////////////////////////////
DWORD	CUser::GetInterAtkRate()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetInterAtkRate();
    }
    int nRate = GetIntervalAtkRate();
    int nRateR = 0, nRateL = 0;
    if (m_pWeaponR && m_pWeaponR->IsWeapon())		// not arrow
    {
        nRateR = m_pWeaponR->GetInt(ITEMDATA_ATKSPEED);
    }
    if (m_pWeaponL && m_pWeaponL->IsWeapon())		// not shield
    {
        nRateL = m_pWeaponL->GetInt(ITEMDATA_ATKSPEED);
    }
    if (nRateR && nRateL)
    {
        nRate = (nRateR + nRateL) / 2;
    }
    else if (nRateR)
    {
        nRate = nRateR;
    }
    else if (nRateL)
    {
        nRate = nRateL;
    }
    IStatus* pStatus = QueryStatus(STATUS_ATKSPEED);
    if (pStatus)
    {
        nRate	= ::CutTrail(0, AdjustData(nRate, pStatus->GetPower()));
        if (this->IsPM())
        {
            this->SendSysMsg(_TXTATR_NORMAL, "atack speed+: %d", pStatus->GetPower());
        }
    }
    //	pStatus = QueryStatus(STATUS_SUPERMAN);
    //	if (pStatus)
    //	{
    //		nRate	= nRate/2;
    //		if (this->IsPM())
    //			this->SendSysMsg(_TXTATR_NORMAL, "superman +: %d", pStatus->GetPower());
    //	}
    //	pStatus = QueryStatus(STATUS_TORNADO);
    //	if (pStatus)
    //	{
    //		nRate	= ::CutTrail(0, AdjustData(nRate, pStatus->GetPower()));
    //		if (this->IsPM())
    //			this->SendSysMsg(_TXTATR_NORMAL, "tornado +: %d", pStatus->GetPower());
    //	}
    return nRate;
}

/////////////////////////////////////////////////////////////////////////////
DWORD	CUser::GetIntervalAtkRate()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetIntervalAtkRate();
    }
    return USER_ATTACK_SPEED;
}

/////////////////////////////////////////////////////////////////////////////
int	CUser::GetAttackRange(int nTargetSizeAdd)
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetAttackRange(nTargetSizeAdd);
    }
    int nRange = 1;
    int nRangeR = 0, nRangeL = 0;
    if (m_pWeaponR && m_pWeaponR->IsWeapon())		// not arrow
    {
        nRangeR = m_pWeaponR->GetInt(ITEMDATA_ATKRANGE);
    }
    if (m_pWeaponL && m_pWeaponL->IsWeapon())		// not shield
    {
        nRangeL = m_pWeaponL->GetInt(ITEMDATA_ATKRANGE);
    }
    if (nRangeR && nRangeL)
    {
        nRange = (nRangeR + nRangeL) / 2;
    }
    else if (nRangeR)
    {
        nRange = nRangeR;
    }
    else if (nRangeL)
    {
        nRange = nRangeL;
    }
    nRange += (GetSizeAdd() + nTargetSizeAdd + 1) / 2;
    //	IStatus* pStatus = QueryStatus(STATUS_ATKRANGE);
    //	if (pStatus)
    //		nRange	= ::CutTrail(1, AdjustData(nRange, pStatus->GetPower()));
    if (QueryMagic())
    {
        nRange += QueryMagic()->GetExtendAtkRange(m_pWeaponR, m_pWeaponL);
    }
    return nRange;
}

/////////////////////////////////////////////////////////////////////////////
int	CUser::GetSizeAdd()
{
    if (QueryTransformation())
    {
        return QueryTransformation()->GetSizeAdd();
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// adjust
/////////////////////////////////////////////////////////////////////////////
int CUser::AdjustHitRate(int nHitRate)
{
    //	IStatus* pStatus = QueryStatus(STATUS_HITRATE);
    //	if (pStatus)
    //	{
    //		nHitRate = ::CutTrail(0, AdjustData(nHitRate, pStatus->GetPower()));
    //#ifdef _DEBUG
    //		this->SendSysMsg(_TXTATR_NORMAL, "hit rate adjust+: %d", pStatus->GetPower());
    //#endif
    //	}
    // 与变异型幻兽合体提高命中率
    if (m_idLinkEudemonType == ITEMTYPE_EUDEMON_VARIATIONAL)
    {
        nHitRate += m_nLinkValue;
    }
    return nHitRate;
}

/////////////////////////////////////////////////////////////////////////////
int CUser::AdjustAttack(int nAtk)
{
    int nAddAtk = 0;
    IStatus* pStatus = QueryStatus(STATUS_DEF2ATK);
    if (pStatus)
    {
        nAddAtk += ::CutTrail(0, MulDiv(this->GetDef(), pStatus->GetPower(), 100));
#ifdef _DEBUG
        this->SendSysMsg(_TXTATR_NORMAL, "attack adjust STATUS_DEF2ATK+: %d%% of def", pStatus->GetPower());
#endif
    }
    pStatus = QueryStatus(STATUS_ATTACK);
    if (pStatus)
    {
        nAddAtk += ::CutTrail(0, AdjustData(nAtk, pStatus->GetPower())) - nAtk;
#ifdef _DEBUG
        this->SendSysMsg(_TXTATR_NORMAL, "atack adjust STATUS_ATTACK+: %d", pStatus->GetPower());
#endif
    }
    pStatus = QueryStatus(STATUS_SUPER_ATK);
    if (pStatus)
    {
        nAddAtk += ::CutTrail(0, AdjustData(nAtk, pStatus->GetPower())) - nAtk;
#ifdef _DEBUG
        this->SendSysMsg(_TXTATR_NORMAL, "atack adjust STATUS_SUPER_ATK+: %d", pStatus->GetPower());
#endif
    }
    /*
    	pStatus	= QueryStatus(STATUS_TEAMATTACK);
    	if (pStatus)
    	{
    		nAddAtk += ::CutTrail(0, AdjustData(nAtk, pStatus->GetPower())) - nAtk;
    #ifdef _DEBUG
    		this->SendSysMsg(_TXTATR_NORMAL, "atack adjust STATUS_TEAMATTACK+: %d", pStatus->GetPower());
    #endif
    	}
    */
    // 攻击型幻兽合体增加攻击
    if (m_idLinkEudemonType == ITEMTYPE_EUDEMON_PATK)
    {
        nAddAtk += m_nLinkValue;
    }
    return nAtk + nAddAtk;
}

/////////////////////////////////////////////////////////////////////////////
int CUser::AdjustDefence(int nDef)
{
    int nAddDef = 0;
    IStatus* pStatus = QueryStatus(STATUS_DEF2ATK);
    if (pStatus)
    {
#ifdef _DEBUG
        this->SendSysMsg(_TXTATR_NORMAL, "def adjust STATUS_DEF2ATK-: %d%%", pStatus->GetPower());
#endif
        nAddDef += ::CutTrail(0, MulDiv(nDef, 100 - __max(100, pStatus->GetPower()), 100)) - nDef;
    }
    pStatus = QueryStatus(STATUS_DEFENCE3);
    if (!pStatus)
    {
        pStatus = QueryStatus(STATUS_DEFENCE2);
    }
    if (!pStatus)
    {
        pStatus = QueryStatus(STATUS_DEFENCE1);
    }
    if (pStatus)
    {
        nAddDef += ::CutTrail(0, AdjustData(nDef, pStatus->GetPower())) - nDef;
#ifdef _DEBUG
        this->SendSysMsg(_TXTATR_NORMAL, "def adjust STATUS_DEFENCE+: %d", pStatus->GetPower());
#endif
    }
    pStatus = QueryStatus(STATUS_SUPER_DEF);
    if (pStatus)
    {
        nAddDef += ::CutTrail(0, AdjustData(nDef, pStatus->GetPower())) - nDef;
#ifdef _DEBUG
        this->SendSysMsg(_TXTATR_NORMAL, "def adjust STATUS_SUPER_DEF+: %d", pStatus->GetPower());
#endif
    }
    pStatus = QueryStatus(STATUS_DEF2ATK);
    if (pStatus)
    {
#ifdef _DEBUG
        this->SendSysMsg(_TXTATR_NORMAL, "def adjust STATUS_DEF2ATK-: %d%%", pStatus->GetPower());
#endif
        return (::CutTrail(0, MulDiv(nDef + nAddDef, 100 - __max(100, pStatus->GetPower()), 100)));
    }
    // 与保护型幻兽合体提高物理防御
    if (m_idLinkEudemonType == ITEMTYPE_EUDEMON_PROTECTIVE)
    {
        nAddDef += m_nLinkValue;
    }
    return nDef + nAddDef;
}

/////////////////////////////////////////////////////////////////////////////
int CUser::AdjustMagicAtk(int nAtk)
{
    IStatus* pStatus = QueryStatus(STATUS_SUPER_MATK);
    if (pStatus)
    {
        return ::CutTrail(0, AdjustData(nAtk, pStatus->GetPower()));
    }
    // 与魔攻型幻兽合体提高魔攻
    if (m_idLinkEudemonType == ITEMTYPE_EUDEMON_MATK)
    {
        nAtk += m_nLinkValue;
    }
    return nAtk;
}

/////////////////////////////////////////////////////////////////////////////
int CUser::AdjustMagicDef(int nDef)
{
    int nAddDef = 0;
    IStatus* pStatus = QueryStatus(STATUS_SUPER_MDEF);
    if (pStatus)
    {
        nAddDef += ::CutTrail(0, AdjustData(nDef, pStatus->GetPower()));
    }
    pStatus = QueryStatus(STATUS_MAGICDEFENCE);
    if (pStatus)
    {
        nAddDef += ::CutTrail(0, AdjustData(nDef, pStatus->GetPower()));
    }
    // 与爆破型幻兽合体提高魔防
    if (m_idLinkEudemonType == ITEMTYPE_EUDEMON_BOMB)
    {
        nAddDef += m_nLinkValue;
    }
    return nDef + nAddDef;
}

/////////////////////////////////////////////////////////////////////////////
int CUser::AdjustBowDefence(int nDef)
{
    //	IStatus* pStatus = QueryStatus(STATUS_BOWDEFENCE);
    //	if (pStatus)
    //	{
    //		nDef = ::CutTrail(0, AdjustData(nDef, pStatus->GetPower()));
    //#ifdef _DEBUG
    //		this->SendSysMsg(_TXTATR_NORMAL, "def adjust STATUS_BOWDEFENCE+: %d", pStatus->GetPower());
    //#endif
    //	}
    return nDef;
}

/////////////////////////////////////////////////////////////////////////////
int CUser::AdjustWeaponDamage(int nDamage)
{
    //	IStatus* pStatus = QueryStatus(STATUS_WEAPONDAMAGE);
    //	if (pStatus)
    //	{
    //		nDamage = ::CutTrail(0, AdjustData(nDamage, pStatus->GetPower()));
    //#ifdef _DEBUG
    //		this->SendSysMsg(_TXTATR_NORMAL, "adjust STATUS_WEAPONDAMAGE+: %d", pStatus->GetPower());
    //#endif
    //	}
    nDamage = MulDiv(nDamage, GetDefence2(), DEFAULT_DEFENCE2);
    return nDamage;
}

/////////////////////////////////////////////////////////////////////////////
int CUser::AdjustMagicDamage(int nDamage)
{
    IStatus* pStatus = QueryStatus(STATUS_MAGICDAMAGE);
    if (pStatus)
    {
        nDamage = ::CutTrail(0, AdjustData(nDamage, pStatus->GetPower()));
#ifdef _DEBUG
        this->SendSysMsg(_TXTATR_NORMAL, "adjust STATUS_MAGICDAMAGE+: %d", pStatus->GetPower());
#endif
    }
    int nMagicDefence2 = (GetMetempsychosis() && this->GetProfessionLevel() >= 3) ? 7000 : DEFAULT_DEFENCE2;		// 3: 70级
    nDamage = MulDiv(nDamage, nMagicDefence2, DEFAULT_DEFENCE2);
    return nDamage;
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::AdjustSpeed(int nSpeed)
{
    if (!m_setStatus)
    {
        return nSpeed;
    }
    int nAddSpeed = 0;
    IStatus* pStatus = QueryStatus(STATUS_TEAMSPEED);
    if (pStatus)
    {
        nAddSpeed += nSpeed * pStatus->GetPower() / 100;
    }
    pStatus = QueryStatus(STATUS_PELT);
    if (pStatus)
    {
        nAddSpeed += ::CutTrail(0, AdjustData(nSpeed, pStatus->GetPower())) - nSpeed;
    }
    pStatus = QueryStatus(STATUS_SLOWDOWN1);
    if (pStatus)
    {
        nAddSpeed += ::CutTrail(0, AdjustData(nSpeed, pStatus->GetPower())) - nSpeed;
    }
    pStatus = QueryStatus(STATUS_SLOWDOWN2);
    if (pStatus && (this->GetLife() * 2 < this->GetMaxLife()))
    {
        nAddSpeed += ::CutTrail(0, AdjustData(nSpeed, pStatus->GetPower())) - nSpeed;
    }
    nSpeed = __min(255, __max(1, nSpeed + nAddSpeed));
    return nSpeed;
}

//////////////////////////////////////////////////////////////////////
void CUser::SendBonusInfo(void)
{
    // read record from db
    char szSQL[256] = "";
    sprintf(szSQL, "SELECT * FROM %s WHERE id_account=%u", _TBL_BONUS, this->GetAccountID());
    IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
    if (!pRes)
    {
        return;
    }
    int nBonus = pRes->RecordCount();
    if (nBonus > 0)
    {
        this->SendSysMsg(_TXTATR_GM, STR_BONUS, nBonus);
    }
    pRes->Release();
}

//////////////////////////////////////////////////////////////////////
bool CUser::DoBonus(void)
{
    // condition check
    for (int i = ITEMPOSITION_BACKPACK; i < ITEMPOSITION_PACK_END; i++)
    {
        if (this->IsItemFull(0, 0, i))
        {
            this->SendSysMsg(STR_BAG_FULL);
            return false;
        }
    }
    /*
    	if (this->IsItemFull(0))
    	{
    		this->SendSysMsg(STR_BAG_FULL);
    		return false;
    	}
    */
    // read record from db
    enum
    {
        BONUSDATA_ACTION = 0,
        BONUSDATA_ID,
        BONUSDATA_ACCOUNT,
    };
    char szSQL[256] = "";
    sprintf(szSQL, "SELECT * FROM %s WHERE id_account=%u LIMIT 1", _TBL_BONUS, this->GetAccountID());
    CAutoPtr<IRecordset> pRes = Database()->CreateNewRecordset(szSQL);
    if (!pRes || pRes->RecordCount() <= 0)
    {
        return false;
    }
    OBJID idRecord	= pRes->GetInt(BONUSDATA_ID);
    OBJID idAction	= pRes->GetInt(BONUSDATA_ACTION);
    // del recordset obj
    //	pRes->Release();
    DEBUG_TRY
    {
        if (!GameAction()->ProcessAction(idAction, this))		//? 此处不会调用玩家切屏，不必是最后一条代码。
        {
            this->SendSysMsg(STR_BAG_FULL);		// 在ACTION中加重量检查
            return false;
        }

        // record info
        ::MyLogSave("gmlog/bonus", "###Bonus: AccountID(%u), Player(%u) take bonus(%u).", this->GetAccountID(), this->GetID(), idAction);

        // delete
        char szSQL[256] = "";
        sprintf(szSQL, "DELETE from %s WHERE id=%u LIMIT 1", _TBL_BONUS, idRecord);

        ASSERT(g_db.ExecuteSQL(szSQL));

        return true;
    }
    DEBUG_CATCH("DoBonus(void)")
    return false;
}

//////////////////////////////////////////////////////////////////////
void CUser::RemoteCall(REMOTE_CALL0* pInfo)
{
    switch(pInfo->ucFuncType)
    {
    case	REMOTE_CALL_TEAM_HUNTKILL:
        {
            this->TeamHuntKill((OBJID)pInfo->IntParam[0], pInfo->IntParam[1], (const char*)pInfo->IntParam[2]);
        }
        break;
    case	REMOTE_CALL_DROP_LEADER:
        {
            CHECK(GetSynRank() == RANK_LEADER);
            // update new leader
            QuerySynAttr()->SetIdRank(pInfo->IntParam[0], pInfo->IntParam[1]);
        }
        break;
    case	REMOTE_CALL_LEAVE_SYN:
        {
            if (GetSynID() == pInfo->IntParam[0])
            {
                QuerySynAttr()->LeaveSyn(true, DELRECORD_FALSE, SYNCHRO_TRUE);		// false: need not delete recorder again
                SendSysMsg(STR_KICKOUT_ME_s, (char*)&pInfo->IntParam[1]);
            }
        }
        break;
    case	REMOTE_CALL_TEAM_CLOSE_MONEY:
        {
            CTeam* pTeam = GetTeam();
            if (pTeam)
            {
                if (pInfo->IntParam[0])
                {
                    pTeam->SetCloseMoney(true);
                }
                else
                {
                    pTeam->SetCloseMoney(false);
                }
                this->SendSysMsg(_TXTATR_TEAM, STR_TEAM_MONEY_s, pInfo->IntParam[0] ? STR_CLOSE : STR_OPEN);
            }
        }
        break;
    case	REMOTE_CALL_TEAM_CLOSE_ITEM:
        {
            CTeam* pTeam = GetTeam();
            if (pTeam)
            {
                if (pInfo->IntParam[0])
                {
                    pTeam->SetCloseItem(true);
                }
                else
                {
                    pTeam->SetCloseItem(false);
                }
                this->SendSysMsg(_TXTATR_TEAM, STR_TEAM_ITEM_s, pInfo->IntParam[0] ? STR_CLOSE : STR_OPEN);
            }
        }
        break;
    case	REMOTE_CALL_TEAM_CLOSE_GEM_ACCESS:
        {
            CTeam* pTeam = GetTeam();
            if (pTeam)
            {
                if (pInfo->IntParam[0])
                {
                    pTeam->SetCloseGemAccess(true);
                }
                else
                {
                    pTeam->SetCloseGemAccess(false);
                }
                this->SendSysMsg(_TXTATR_TEAM, STR_TEAM_GEM_s, pInfo->IntParam[0] ? STR_CLOSE : STR_OPEN);
            }
        }
        break;
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::Transform(DWORD dwType, int nKeepSecs, bool bSynchro/*=true*/)
{
    bool bBack = false;
    if (m_pTransformation)
    {
        m_pTransformation->Release();
        m_pTransformation = NULL;
        bBack = true;
    }
    CNpcType* pType = MonsterType()->GetObj(dwType);
    CHECKF(pType);
    CTransformation* pTransform = CTransformation::CreateNew();		// VVVVVVVVVVVVVVVVVVVVVVVVvv
    IF_OK(pTransform)
    {
        IF_OK(pTransform->Create(this, pType))
        {
            m_pTransformation = pTransform;
            m_tTransformation.Startup(nKeepSecs);
            if (bSynchro)
            {
                SynchroTransform();
            }
            this->StopMine();
            return true;
        }
        else
        {
            pTransform->Release();    // AAAAAAAAAAAAAAAAAAAAAAAA
        }
    }
    if (bBack)
    {
        SynchroTransform();
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CUser::AbortTransform()
{ SAFE_RELEASE(m_pTransformation); SynchroTransform(); }

//////////////////////////////////////////////////////////////////////
bool CUser::SynchroTransform()
{
    CMsgUserAttrib	msg;
    IF_OK( msg.Create(GetID(), _USERATTRIB_LOOKFACE, GetLookFace())
           && msg.Append(_USERATTRIB_MAXLIFE, GetMaxLife())
           && msg.Append(_USERATTRIB_LIFE, GetLife())
           && msg.Append(_USERATTRIB_SIZEADD, GetSizeAdd()) )
    BroadcastRoomMsg(&msg, INCLUDE_SELF);
    this->BroadcastTeamLife(true);		// true: with max life
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::CallPet(OBJID idMonsterType, int x, int y, int nKeepSecs/*=0*/)
{
    KillCallPet();
    CMonster* pMonster = NpcManager()->CreateCallPet(this, idMonsterType, x, y);
    CHECKF(pMonster);
    m_pCallPet = pMonster->QueryLink();
    if (nKeepSecs)
    {
        m_tCallPet.Startup(nKeepSecs);
    }
    else
    {
        m_tCallPet.Clear();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::KillCallPet(bool bNow/*=false*/)
{
    if (!m_pCallPet)
    {
        return ;
    }
    if (!m_pCallPet->IsDeleted())
    {
        m_pCallPet->DelMonster(bNow);
    }
}

//////////////////////////////////////////////////////////////////////
OBJID CUser::GetCallPetID()			{ if (m_pCallPet) { return QueryCallPet()->GetID(); } return ID_NONE; }

//////////////////////////////////////////////////////////////////////
bool CUser::IsSimpleMagicAtk	()
{ return QueryTransformation() && QueryTransformation()->GetMagicType() != ID_NONE; }

//////////////////////////////////////////////////////////////////////
int CUser::GetSimpleMagicHitRate()
{ IF_OK(QueryTransformation()) return QueryTransformation()->GetMagicHitRate(); else { return 100; } }

//////////////////////////////////////////////////////////////////////
bool CUser::SetWeather(int nType, int nIntensity, int nDir, DWORD nColor, DWORD nKeepSecs, DWORD nSpeedSecs)
{
    if (m_idWeather != ID_NONE)
    {
        CWeatherRegion* pRegion = GetMap()->QueryWeatherRegion(m_idWeather);
        IF_OK(pRegion)
        return pRegion->QueryWeather()->SetNewWeather(nType, nIntensity, nDir, nColor, nKeepSecs, nSpeedSecs);
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CUser::SetLight(int dwRGB, int nSecs/*=0*/)
{
    if (nSecs == 0)		// keep light
    {
        GetMap()->SetLight(dwRGB);
    }
    CMsgAction	msg;
    IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), nSecs, actionMapARGB, dwRGB))
    UserManager()->BroadcastMapMsg(GetMapID(), &msg, NULL);
}

//////////////////////////////////////////////////////////////////////
void CUser::SendLight()
{
    int	nKeepSecs = 0;		// keep light
    CMsgAction	msg;
    IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), nKeepSecs, actionMapARGB, GetMap()->GetLight()))
    SendMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
void CUser::AwardSynWarScore(CNpc* pNpc, int nScore)
{
    CHECK(pNpc);
    CHECK(nScore > 0);
    if (GetSynID() == ID_NONE || GetSynID() == pNpc->GetSynID())
    {
        return ;
    }
    int nAddProffer = MulDiv(nScore, SYNWAR_PROFFER_PERCENT, 100);
    if (nAddProffer)
    {
        QuerySynAttr()->AddProffer(nAddProffer);
    }
    int nAddMoney	= MulDiv(nScore, SYNWAR_MONEY_PERCENT, 100);
    if (nAddMoney > 0)
    {
        CSyndicate* pSyn = SynManager()->QuerySyndicate(pNpc->GetSynID());
        if (pSyn)
        {
            nAddMoney	= ::CutOverflow(nAddMoney, pSyn->GetInt(SYNDATA_MONEY));
            pSyn->QueryModify()->AddData(SYNDATA_MONEY, -1 * nAddMoney, true);
            this->GainMoney(nAddMoney, SYNCHRO_TRUE);
            //
        }
    }
    CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());
    if (pSyn)
    {
        pSyn = pSyn->GetMasterSyn();
        pSyn->AddSynWarScore(pNpc->GetID(), nScore);
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsHostileSynMember	(CUser* pTarget)
{
    IF_NOT (pTarget)
    return false;
    OBJID idSynTarget	= pTarget->GetSynID();
    OBJID idSyn			= this->GetSynID();
    if (ID_NONE != idSynTarget && ID_NONE != idSyn)
    {
        CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
        if (pSyn && pSyn->IsHostile(idSynTarget))
        {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
CSyndicate* CUser::GetSyndicate(void)
{
    if (this->GetSynID() != ID_NONE)
    {
        return SynManager()->QuerySyndicate(this->GetSynID());
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsMemberOfSyn(OBJID idSyn)
{
    if (idSyn == ID_NONE)
    {
        return false;
    }
    CSyndicate* pSyn = this->GetSyndicate();
    while (pSyn)
    {
        if (pSyn->GetID() == idSyn)
        {
            return true;
        }
        OBJID idFealty	= pSyn->GetInt(SYNDATA_FEALTY);
        if (idFealty == ID_NONE)
        {
            return false;
        }
        pSyn = SynManager()->QuerySyndicate(idFealty);
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
OBJID CUser::GetMateHomeID()
{
    LPCTSTR szMateName = GetMate();
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT home_id FROM %s WHERE name='%s' LIMIT 1", _TBL_USER, szMateName);
    CAutoPtr<IRecordset> pRes = _Database(PID)->CreateNewRecordset(szSQL);
    if (pRes)
    {
        return pRes->GetInt(0);
    }
    return ID_NONE;
}

//////////////////////////////////////////////////////////////////////
void CUser::SetRecordPos(OBJID idMap, DWORD dwX, DWORD dwY, BOOL bUpdate/* = false*/)
{
    PROCESS_ID idProcess = MapManager()->GetProcessID(idMap);
    CHECK(idProcess != PROCESS_NONE);
    if (idProcess == m_idProcess)
    {
        CGameMap* pMap = MapManager()->GetGameMap(idMap);
        IF_NOT (pMap)
        return;
        IF_NOT(pMap->IsStandEnable(dwX, dwY))
        return;
    }
    m_data.SetRecordPos(idMap, dwX, dwY, bUpdate);
}

//////////////////////////////////////////////////////////////////////
bool CUser::KickBack()
{
    if (!this->IsSynchro())
    {
        return false;
    }
    CMsgAction	msg;
    IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionKickBack, GetPosX(), GetPosY()))
    SendMsg(&msg);
    this->SetSynchro(false);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::Synchro()
{
    CMsgAction	msg;
    IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionSynchro, GetPosX(), GetPosY()))
    SendMsg(&msg);
    this->SetSynchro(false);
    return true;
}

//////////////////////////////////////////////////////////////////////
OBJID CUser::GetMateID()
{
    if (!IsMarried())
    {
        return ID_NONE;
    }
    CUser* pMate = UserManager()->GetUser(this->GetMate());
    if (pMate)
    {
        return pMate->GetID();
    }
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT id FROM %s where name='%s' LIMIT 1", _TBL_USER, GetMate());
    CAutoPtr<IRecordset> pRes = _Database(PID)->CreateNewRecordset(szSQL);
    if (pRes)
    {
        return pRes->GetInt(0);
    }
    return ID_NONE;
}

//////////////////////////////////////////////////////////////////////
int CUser::AdjustExp(IRole* pTarget, int nRawExp, bool bNewbieBonusMsg)
{
    CHECKF(pTarget);
    int nExp = nRawExp;
    // kill bonus
    //	if (this && !pTarget->IsAlive())
    //	{
    //		CNpc* pNpc = NULL;
    //		pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc));
    //
    //		if (pTarget->IsMonster() || pNpc && pNpc->IsGoal())
    //		{
    //			// team bonus
    //			CTeam* pTeam = this->GetTeam();
    //			if (pTeam && pTeam->IsTeamWithNewbie(this->GetID(), pTarget))
    //			{
    //				nExp *= 2;
    //
    //				if (bNewbieBonusMsg)
    //					this->SendSysMsg(STR_KILLING_EXPEX, nExp);
    //			}
    //		}
    //	}
    // adjust
    nExp = CBattleSystem::AdjustExp(nExp, this->GetLev(), pTarget->GetLev());
    CNpc* pNpc = NULL;
    pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc));
    if (pTarget->IsMonster() || (pNpc && pNpc->IsGoal()))
    {
        if (this->GetTutor())
        {
            CTeam* pTeam	= this->GetTeam();
            if (pTeam && pTeam->IsTeamWithTutor(this->GetID(), pTarget))
            {
                nExp += nExp * WITHTUTORBONUS_PERCENT / 100;    // 和导师一起练级，多20%经验
            }
            else
            {
                nExp += nExp * HAVETUTORBONUS_PERCENT / 100;    // 没和导师一起练级，多10%经验
            }
        }
    }
    // 修炼界结增加组队获得的经验
    IStatus* pStatus = this->QueryStatus(STATUS_TEAMEXP);
    if (pStatus)
    {
        nExp = ::CutTrail(0, MulDiv(nExp, 100 + pStatus->GetPower(), 100));
    }
    // 幸运 -- 增加战斗经验的状态
    pStatus = this->QueryStatus(STATUS_ADD_EXP);
    if (pStatus)
    {
        nExp = ::CutTrail(0, MulDiv(nExp, 100 + pStatus->GetPower(), 100));
    }
    pStatus = this->QueryStatus(STATUS_ADJUST_EXP);
    if (pStatus)
    {
        nExp = ::CutTrail(0, AdjustData(nExp, pStatus->GetPower()));
    }
    return nExp;
}

//////////////////////////////////////////////////////////////////////
bool CUser::CheckCrime(IRole* pRole)
{
    CHECKF(pRole);
    if (!pRole->IsEvil())
    {
        if (!GetMap() || !GetMap()->IsDeadIsland())
        {
            SetCrimeStatus();
        }
        return true;
    }
    OBJID idSyn = GetMap()->GetSynID();
    if (idSyn != ID_NONE && pRole->GetSynID() == idSyn)
    {
        CNpc* pNpc;
        if (pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsGoal())		// 靶子
        {
            return false;
        }
        if (!GetMap() || !GetMap()->IsDeadIsland())
        {
            SetCrimeStatus(STATUS_SYNCRIME);
        }
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsImmunity(IRole* pRole)
{
    CHECKF(pRole);
    const bool	ATK_IT = false;
    const bool	IMMUNITY = true;
    OBJID idRole = pRole->GetID();
    if (IsSceneID(idRole))		// is npc
    {
        CNpc* pNpc;
        pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc));
        if (pNpc && !pNpc->IsBeAtkable())
        {
            return true;
        }
        return false;
    }
    else if (IsNpcID(idRole))	// is monster
    {
        CMonster* pMonster = NULL;
        CHECKF(pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)));
        if (pMonster->GetMasterID() == GetID())		// call pet of mine
        {
            return true;
        }
        switch(this->GetPkMode())
        {
        case PKMODE_FREE:
            return ATK_IT;
            break;
        case PKMODE_TEAM:
            if (pMonster->IsCallPet() || pMonster->IsEudemon())
            {
                CTeam* pTeam = GetTeam();
                if (pTeam && pTeam->IsTeamMember(pMonster->GetMasterID()))
                {
                    return IMMUNITY;
                }
                CUser* pUser = this->QueryOwnerUser();
                if (this->IsMate(pUser))
                {
                    return IMMUNITY;
                }
            }
            else
            {
                return ATK_IT;
            }
            break;
        case PKMODE_SYNDICATE:          //add huang 2004.1.15  todo.
            if (pMonster->IsCallPet() || pMonster->IsEudemon())
            {
                CUser* pOwnerUser = pRole->QueryOwnerUser();
                if (this->GetSynID() == pOwnerUser->GetSynID() || this->IsMate(pOwnerUser))
                {
                    return IMMUNITY;
                }
                else
                {
                    CSyndicate* pSyn = SynManager()->QuerySyndicate(this->GetSynID());
                    if (pSyn)
                    {
                        if ( pSyn->IsFriendly(pOwnerUser->GetSynID())
                                || pSyn->IsNearSyn(pOwnerUser->GetSynID()))
                        {
                            return IMMUNITY;
                        }
                    }
                }
            }
            else if (pMonster->IsSynPet())
            {
                CSyndicate* pSyn = SynManager()->QuerySyndicate(this->GetSynID());
                if (pSyn)
                {
                    if ( pSyn->IsFriendly(pMonster->GetSynID())
                            || pSyn->IsNearSyn(pMonster->GetSynID()))
                    {
                        return IMMUNITY;
                    }
                }
            }
            return ATK_IT;
            break;
        case PKMODE_ARRESTMENT:
            if (pMonster->IsMonster() || pMonster->IsSynMonster() || pMonster->IsEvil())
            {
                return ATK_IT;
            }
            else
            {
                return IMMUNITY;
            }
            break;
        case PKMODE_SAFE:
            if (pMonster->IsMonster() || pMonster->IsSynMonster())
            {
                return ATK_IT;
            }
            else
            {
                return IMMUNITY;
            }
            break;
        }
        return false;
    }
    else	// user
    {
        switch(this->GetPkMode())
        {
        case	PKMODE_FREE:
            return false;
            break;
        case	PKMODE_TEAM:    //change huang 2004.1.11 (添加骑士团模式)
            {
                bool bImmunity = false;
                CTeam* pTeam = this->GetTeam();
                if (pTeam && pTeam->IsTeamMember(idRole))
                {
                    bImmunity = true;
                }
                if (this->GetFriend(pRole->GetID()))
                {
                    bImmunity = true;
                }
                CUser* pUser = NULL;
                if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
                {
                    if (this->IsMate(pUser))
                    {
                        bImmunity = true;
                    }
                }
                return bImmunity;
            }
            break;
        case	PKMODE_SYNDICATE:     //add huang 2004.1.15
            {
                bool bImmunity = false;
                CUser* pUser = NULL;
                if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)) && this->GetSynID() != ID_NONE)
                {
                    if (this->GetSynID() == pUser->GetSynID())
                    {
                        bImmunity = true;
                    }
                    else
                    {
                        CSyndicate* pSyn = SynManager()->QuerySyndicate(this->GetSynID());
                        if (pSyn)
                        {
                            if (pSyn->IsNearSyn(pUser->GetSynID())
                                    || pSyn->IsFriendly(pUser->GetSynID()))
                            {
                                bImmunity = true;
                            }
                        }
                    }
                    if (this->IsMate(pUser))
                    {
                        bImmunity = true;
                    }
                }
                return bImmunity;
            }
            break;
        case	PKMODE_ARRESTMENT:
            {
                if (pRole->IsEvil())
                {
                    return false;
                }
                return true;
            }
            break;
        case	PKMODE_SAFE:
            return true;
            break;
        default:
            ASSERT(!"switch pk mode");
            break;
        }
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::SetCrimeStatus(int nStatus/*=STATUS_CRIME*/, int nTime/*=CRIME_KEEP_SECS*/)
{
    ASSERT(nStatus == STATUS_CRIME || nStatus == STATUS_SYNCRIME);
    int nPower = 1;		// 0: decrement secs
    CRole::AttachStatus(QueryRole(), nStatus, nPower, nTime);
    if (QueryCallPet())
    {
        CRole::AttachStatus(QueryCallPet()->QueryRole(), nStatus, nPower, nTime);
    }
    for (int i = 0; i < GetEudemonAmount(); i++)
    {
        CMonster* pEudemon = QueryEudemonByIndex(i);
        if (pEudemon)
        {
            CRole::AttachStatus(pEudemon->QueryRole(), nStatus, nPower, nTime);
        }
    }
}
//////////////////////////////////////////////////////////////////////
void CUser::SetSelfDefStatus(int nTime)      //add huang 2004.1.14
{
    //	int nPower = 1;
    //	CRole::AttachStatus(QueryRole(),STATUS_SELFDEFENCE,nPower,nTime);
}
//////////////////////////////////////////////////////////////////////
bool CUser::IsBeAtkable			()
{
    return !(QueryStatus(STATUS_PK_PROTECT)	//m_tLock.IsActive() && !m_tLock.IsTimeOut()
             || m_pCallPet && m_pCallPet->IsInsteadMagic()
             || QueryStatus(STATUS_HIDDEN)
             || QueryStatus(STATUS_LOCK));
}

//////////////////////////////////////////////////////////////////////
// rebirth
//////////////////////////////////////////////////////////////////////
bool CUser::CheckRebirth(int nProf, CRebirthData* pData)
{
    if (this->GetMetempsychosis())
    {
        return false;
    }
    if (GetLev() < pData->GetInt(REBIRTHDATA_NEED_LEVEL))
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::Rebirth(int nProf, int nLook)
{
    for(int i = 0; i < RebirthSet()->GetAmount(); i++)
    {
        CRebirthData* pData = RebirthSet()->GetObjByIndex(i);
        if (pData && GetProfession() == pData->GetInt(REBIRTHDATA_NEED_PROF) && nProf == pData->GetInt(REBIRTHDATA_NEW_PROF))
        {
            if (CheckRebirth(nProf, pData))
            {
                int nOldProf	= GetProfession();
                int nOldLevel	= GetLev();
                ResetEquipLevel(pData->GetInt(REBIRTHDATA_NEW_LEVEL));
                ResetUserAttrib(nOldProf, nOldLevel, pData->GetInt(REBIRTHDATA_NEW_PROF), pData->GetInt(REBIRTHDATA_NEW_LEVEL), nLook);
                return true;
            }
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CUser::ResetEquipLevel(int nNewLevel)
{
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItem* pEquip = this->GetEquipItemByPos(i);
        if (pEquip)
        {
            ASSERT(DownEquipmentLevel(i, nNewLevel));
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::ResetUserAttrib(int nOldProf, int nOldLevel, int nNewProf, int nNewLevel, int nNewLook)
{
    // default attribute
    int nProfession = nNewProf;
    if (nProfession == 0)
    {
        nProfession = 20;
    }
    int nForce = 6, nHealth = 3, nDex = 1, nSoul = 0;
    if (nProfession == 100)
    {
        nForce	= 0;
        nDex	= 2;
        nHealth = 3;
        nSoul	= 5;
    }
    else if (nProfession == 40)
    {
        nForce	= 2;
        nDex	= 7;
        nHealth = 1;
        nSoul	= 0;
    }
    int nMaxLife = nHealth * 30 + 30;
    int nMaxMana = nSoul * 20;
    int nDefaultMoney = 100;
    int nDefaultHair = 310 +::RandGet(7) * 100;
    int nAddPoint	= GetRebirthAddPoint(nOldProf, nOldLevel);
    // modify
    this->SetAutoAllot(true);
    this->SetMetempsychosis(1);
    this->SetLook(nNewLook);
    SetAttrib(_USERATTRIB_LOOKFACE, GetLookFace(), SYNCHRO_BROADCAST);		// no database
    SetAttrib(_USERATTRIB_PORFESSION, nNewProf, SYNCHRO_TRUE);
    SetAttrib(_USERATTRIB_LEV, nNewLevel, SYNCHRO_TRUE);
    SetAttrib(_USERATTRIB_EXP, 0, SYNCHRO_TRUE);
    SetAttrib(_USERATTRIB_FORCE, nForce, SYNCHRO_TRUE);
    SetAttrib(_USERATTRIB_DEX, nDex, SYNCHRO_TRUE);
    SetAttrib(_USERATTRIB_HEALTH, nHealth, SYNCHRO_TRUE);
    SetAttrib(_USERATTRIB_SOUL, nSoul, SYNCHRO_TRUE);
    SetAttrib(_USERATTRIB_LIFE, nMaxLife, SYNCHRO_TRUE);
    SetAttrib(_USERATTRIB_MANA, nMaxMana, SYNCHRO_TRUE);
    SetAttrib(_USERATTRIB_ADDPOINT, nAddPoint, SYNCHRO_TRUE);
    SaveInfo();
}

//////////////////////////////////////////////////////////////////////
int CUser::GetRebirthAddPoint(int nOldProf, int nOldLevel)
{
    if (nOldProf == HIGHEST_WATER_WIZARD_PROF)
    {
        return 30 + (1 + (nOldLevel - 110) / 2 ) * ((nOldLevel - 110) / 2) / 2;
    }
    else
    {
        return 30 + (1 + (nOldLevel - 120) ) * (nOldLevel - 120) / 2;
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::SendSystemBBS(LPCTSTR szText)
{
    OBJID idUser = this->GetID();
    CMsgTalk	msg;
    IF_OK(msg.Create(this->GetName(), ALLUSERS_NAME, szText, NULL, 0xffffff, _TXTATR_MSG_SYSTEM))
    MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, idUser);
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcessMsgAction(int nAction, int nData, OBJID id)
{
    switch(nAction)
    {
    case actionQueryMedal:
        {
            CMsgDataArray	msg;
            IF_OK(msg.Create(_DATAARRAY_ACTION_MEDAL, strlen(this->GetMedal()), this->GetMedal()))
            SendMsg(&msg);
        }
        break;
    case actionDelMedal:
        {
            char* ptr = strchr(this->GetMedal(), nData);
            if (ptr)
            {
                memcpy(ptr, ptr + 1, (ptr - this->GetMedal()));
            }
        }
        break;
        /*
        	case actionSelectMedal:
        		{
        			if (strchr(this->GetMedal(), nData))
        			{
        				this->SetAttrib(_USERATTRIB_MEDALSELECTED, nData, SYNCHRO_BROADCAST);
        			}
        		}
        		break;
        */
    case actionQueryHonorTitle:
        {
            CMsgDataArray	msg;
            IF_OK(msg.Create(_DATAARRAY_ACTION_HONORTITLE, strlen(this->GetTitle()), this->GetTitle()))
            SendMsg(&msg);
        }
        break;
    case actionDelHonorTitle:
        {
            char* ptr = strchr(this->GetTitle(), nData);
            if (ptr)
            {
                memcpy(ptr, ptr + 1, (ptr - this->GetTitle()));
            }
        }
        break;
    case actionSelectHonorTitle:
        {
            /*
            			if (strchr(this->GetTitle(), nData))
            			{
            				this->SetAttrib(_USERATTRIB_HONORTITLESELECTED, nData, SYNCHRO_BROADCAST);
            			}
            */
        }
        break;
    default:
        ASSERT(!"CUser::ProcessMsgAction");
    }
}

//////////////////////////////////////////////////////////////////////
// lock key
//////////////////////////////////////////////////////////////////////
bool CUser::SetLock(DWORD dwKey)
{
    // check key
    if (m_data.GetRealLockKey())
    {
        return false;
    }
    if (dwKey <= 99999)		// || dwKey >= 1000000000
    {
        return false;
    }
    if (dwKey == 123456 || dwKey == 654321)
    {
        return false;
    }
    if (dwKey == 111111 || dwKey == 222222 || dwKey == 333333 || dwKey == 666666 || dwKey == 888888 || dwKey == 999999)
    {
        return false;
    }
    m_data.SetLockKey(dwKey);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::OpenLock(DWORD dwKey)
{
    if (!m_data.GetRealLockKey() || m_data.GetRealLockKey() != dwKey)
    {
        return false;
    }
    m_data.SetCurrLockKey(0, UPDATE_FALSE);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::ClearLock(DWORD dwKey)
{
    if (m_data.GetRealLockKey() && m_data.GetRealLockKey() != dwKey)
    {
        return false;
    }
    m_data.SetLockKey(0, UPDATE_FALSE);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::CheckLock(bool bSynchro/*=true*/)
{
    if (m_data.GetCurrLockKey())
    {
        if (bSynchro)
        {
            SendSysMsg(STR_LOCKKEY_DENY);
        }
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
// week pk game badge
//////////////////////////////////////////////////////////////////////
bool CUser::AddBadge()
{
    CItem* pBadge = NULL;
    // TODO: 从CUserPackage中搜索周赛徽章
    pBadge	= QueryPackage()->GetWPGBadge();
    /*for(int i = m_setItem.size()-1; i >= 0; i--)
    {
    	CItem* pItem = m_setItem[i];
    	if (pItem && CItem::IsWPGBadge(pItem->GetInt(ITEMDATA_TYPE)))
    	{
    		pBadge	= pItem;
    		break;
    	}
    }*/
    if (pBadge && pBadge->GetInt(ITEMDATA_AMOUNT) == 1)
    {
        pBadge->SetInt(ITEMDATA_AMOUNT, 2);
        CMsgItem	msg;
        IF_OK(msg.Create(pBadge->GetID(), ITEMACT_SYNCHRO_AMOUNT, pBadge->GetInt(ITEMDATA_AMOUNT)))
        SendMsg(&msg);
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::DelAllBadge()
{
    CItem* pBadge = NULL;
    while (pBadge = QueryPackage()->GetWPGBadge())		// 用while循环不太好，以后最好改掉 -- zlong 2004.11.22
    {
        EraseItem(pBadge->GetID(), SYNCHRO_TRUE);
    }
    /*for(int i = m_setItem.size()-1; i >= 0; i--)
    {
    	CItem* pItem = m_setItem[i];
    	if (pItem && CItem::IsWPGBadge(pItem->GetInt(ITEMDATA_TYPE)))
    		EraseItem(pItem->GetID(), SYNCHRO_TRUE);
    }*/
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::CheckBadge(int nItemType/*=0*/)
{
    return QueryPackage()->CheckWPGBadge(nItemType);
    /*
    	CItem* pBadge = NULL;
    	for(int i = m_setItem.size()-1; i >= 0; i--)
    	{
    		CItem* pItem = m_setItem[i];
    		if (pItem && CItem::IsWPGBadge(pItem->GetInt(ITEMDATA_TYPE)))
    		{
    			if (nItemType && pItem->GetInt(ITEMDATA_TYPE) == nItemType && pItem->GetInt(ITEMDATA_AMOUNT) == 1)
    			{
    				nItemType = 0;		// no ignore again
    				continue;
    			}

    			return false;
    		}
    	}

    	return true;
    */
}

//////////////////////////////////////////////////////////////////////
bool CUser::CreateEudemon(CItem* pItem, int x, int y)
{
    CHECKF (pItem);
    if (QueryEudemon(pItem->GetID()))
    {
        return false;
    }
    DetachEudemon(pItem);
    CMonster* pMonster = NpcManager()->CreateEudemon(this, pItem, x, y);
    CHECKF(pMonster);
    ST_EUDEMON* ptEudemon = new ST_EUDEMON;
    ASSERT(ptEudemon);
    ptEudemon->pEudemonItem	= pItem;
    ptEudemon->pEudemon		= pMonster->QueryLink();
    ptEudemon->tEudemon.Clear();
    // 计算关系值对潜力值的影响
    OBJID	idDivine	= pItem->GetDivineID();
    if (idDivine != ID_NONE)
    {
        EUDEMON_SET::iterator it = m_setEudemon.begin();
        for (; it != m_setEudemon.end(); it++)
        {
            ST_EUDEMON* ptEudemon = *it;
            if (ptEudemon && ptEudemon->pEudemonItem && ptEudemon->pEudemon)
            {
                OBJID idDivine2 = ptEudemon->pEudemonItem->GetDivineID();
                int nRelationShip = pItem->GetRelationShip(idDivine2);
                nRelationShip += ptEudemon->pEudemonItem->GetRelationShip(idDivine);
                int nAddPotential = -1 * nRelationShip / ADD_POTENTIAL_RELATIONSHIP;
                if (nAddPotential != 0)
                {
                    pMonster->AddAttrib(_USERATTRIB_POTENTIAL, nAddPotential, SYNCHRO_TRUE);
                    ptEudemon->pEudemon->AddAttrib(_USERATTRIB_POTENTIAL, nAddPotential, SYNCHRO_TRUE);
                }
            }
        }
    }
    m_setEudemon.push_back(ptEudemon);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::CallBackEudemon(OBJID idItem, bool bNow)
{
    OBJID	idDivine	= ID_NONE;
    UCHAR	setRelationShip[MAX_DIVINE_ID-MIN_DIVINE_ID+1];
    EUDEMON_SET::iterator it = m_setEudemon.begin();
    for (; it != m_setEudemon.end(); it++)
    {
        ST_EUDEMON* ptEudemon = *it;
        if (ptEudemon && ptEudemon->pEudemonItem && ptEudemon->pEudemonItem->GetID() == idItem)
        {
            if (ptEudemon->pEudemon && !ptEudemon->pEudemon->IsDeleted())
            {
                m_setEudemon.erase(it);
                idDivine = ptEudemon->pEudemonItem->GetDivineID();
                for (int i = 0; i < MAX_DIVINE_ID; i++)
                {
                    setRelationShip[i] = ptEudemon->pEudemonItem->GetRelationShip(i + MIN_DIVINE_ID);
                }
                // 通知客户端
                CMsgItem msg;
                if (msg.Create(idItem, ITEMACT_KILL_EUDEMON, ptEudemon->pEudemon->GetID()))
                {
                    BroadcastRoomMsg(&msg, INCLUDE_SELF);
                }
                ptEudemon->pEudemon->DelMonster(bNow);
                SAFE_DELETE (ptEudemon);
                break;
            }
        }
    }
    // 根据关系值修正潜力值
    if (idDivine != ID_NONE)
    {
        for (it = m_setEudemon.begin(); it != m_setEudemon.end(); it++)
        {
            ST_EUDEMON* ptEudemon = *it;
            if (ptEudemon && ptEudemon->pEudemonItem && ptEudemon->pEudemon)
            {
                OBJID idDivine2 = ptEudemon->pEudemonItem->GetDivineID();
                if (idDivine2 >= MIN_DIVINE_ID && idDivine2 <= MAX_DIVINE_ID)
                {
                    // 计算关系和
                    int nRelationShip = ptEudemon->pEudemonItem->GetRelationShip(idDivine) + setRelationShip[idDivine2-MIN_DIVINE_ID];
                    int nAddPotential = -1 * nRelationShip / ADD_POTENTIAL_RELATIONSHIP;
                    if (nAddPotential != 0)
                    {
                        ptEudemon->pEudemon->AddAttrib(_USERATTRIB_POTENTIAL, nAddPotential, SYNCHRO_TRUE);
                    }
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::CallBackAllEudemon(bool bNow/*=true*/)
{
    EUDEMON_SET::iterator it = m_setEudemon.begin();
    for (; it != m_setEudemon.end(); it++)
    {
        ST_EUDEMON* ptEudemon = *it;
        if (ptEudemon)
        {
            if (ptEudemon->pEudemon && ptEudemon->pEudemonItem && !ptEudemon->pEudemon->IsDeleted())
            {
                // 通知客户端
                CMsgItem msg;
                if (msg.Create(ptEudemon->pEudemonItem->GetID(), ITEMACT_KILL_EUDEMON, ptEudemon->pEudemon->GetID()))
                {
                    BroadcastRoomMsg(&msg, INCLUDE_SELF);
                }
                ptEudemon->pEudemon->DelMonster(bNow);
                SAFE_DELETE (ptEudemon);
            }
        }
    }
    m_setEudemon.clear();
}

//////////////////////////////////////////////////////////////////////
CMonster* CUser::QueryEudemon(OBJID idItem)
{
    CHECKF (idItem != ID_NONE);
    ST_EUDEMON* ptEudemon = this->QueryEudemonSt(idItem);
    if (ptEudemon && ptEudemon->pEudemonItem)
    {
        return ptEudemon->pEudemon;
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CMonster* CUser::QueryEudemonByID(OBJID idEudemon)
{
    CHECKF (idEudemon != ID_NONE);
    EUDEMON_SET::iterator it = m_setEudemon.begin();
    for (; it != m_setEudemon.end(); it++)
    {
        ST_EUDEMON* ptEudemon = *it;
        if (ptEudemon && ptEudemon->pEudemonItem
                && ptEudemon->pEudemon && ptEudemon->pEudemon->GetID() == idEudemon)
        {
            return ptEudemon->pEudemon;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CMonster* CUser::QueryEudemonByIndex(int nIdx)
{
    CHECKF (nIdx >= 0 && nIdx < m_setEudemon.size());
    ST_EUDEMON* ptEudemon = m_setEudemon[nIdx];
    if (ptEudemon && ptEudemon->pEudemonItem)
    {
        return ptEudemon->pEudemon;
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CUser::ST_EUDEMON* CUser::QueryEudemonSt(OBJID idItem)
{
    CHECKF (idItem != ID_NONE);
    EUDEMON_SET::iterator it = m_setEudemon.begin();
    for (; it != m_setEudemon.end(); it++)
    {
        ST_EUDEMON* ptEudemon = *it;
        if (ptEudemon && ptEudemon->pEudemonItem && ptEudemon->pEudemonItem->GetID() == idItem)
        {
            return ptEudemon;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CUser::HatchEudemon(CItem* pItem)
{
    CHECKF (pItem);
    IF_NOT (pItem->IsEudemonEgg())
    return false;
    // 80000是幻兽蛋，70000是幻兽
    OBJID idType = ITEMSORT_EXPEND * 100000 + ITEMTYPE_EUDEMON + pItem->GetInt(ITEMDATA_TYPE) % 10000;
    CItemTypeData* pTypeData = ItemType()->QueryItemType(idType);
    if (!pTypeData)
    {
        return false;
    }
    // 由于对幻兽来说ITEMDATA_NAME转意了，
    // 因此必须在ChangeType之前读ITEMDATA_NAME的数据，否则将出错
    IF_NOT (pItem->CreateEudemonData(GameDataDefault()->GetEudemonData(), pItem->GetID(), pTypeData->GetStr(ITEMTYPEDATA_NAME)))
    return false;
    if (pItem->ChangeType(idType))
    {
        pItem->SetInt(ITEMDATA_AVAILABLETIME, 0);
        pItem->SetInt(ITEMDATA_EUDEMON_LEVEL, 1);
        pItem->SetInt(ITEMDATA_EUDEMON_LIFE, pItem->GetEudemonMaxLife(), true);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::EvolveEudemon(OBJID idEudemonItem, int nType)
{
    CItem* pEudemon = this->GetItem(idEudemonItem);
    CHECKF (pEudemon && pEudemon->IsEudemon());
    CHECKF (nType > 0 && nType <= EUDEMON_EVOLVE_MAXTYPE);
    if (!pEudemon->IsEvolveEnable())
    {
        // can not evolve
        this->SendSysMsg(STR_EUDEMON_CANNOT_EVOLVE);
        return false;
    }
    //	if (pEudemon->GetEudemonLevel() < EUDEMON_EVOLVE_LEVEL)
    //	{
    //		// low level
    //		this->SendSysMsg(STR_EUDEMON_LOW_LEVEL, EUDEMON_EVOLVE_LEVEL);
    //		return false;
    //	}
    // 召回幻兽
    CallBackEudemon(pEudemon->GetID());
    DetachEudemon(pEudemon);
    OBJID idType = pEudemon->GetInt(ITEMDATA_TYPE) + nType;
    IF_NOT (pEudemon->ChangeType(idType))
    return false;
    CMsgItemInfo	msg;
    if (msg.Create(pEudemon, ITEMINFO_UPDATE))
    {
        this->SendMsg(&msg);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::Evolve2Eudemon(OBJID idEudemonItem, int nType)
{
    CItem* pEudemon = this->GetItem(idEudemonItem);
    CHECKF (pEudemon && pEudemon->IsEudemon());
    CHECKF (nType >= MIN_DIVINE_ID && nType <= MAX_DIVINE_ID);
    if (!pEudemon->IsEvolve2Enable())
    {
        // can not evolve
        this->SendSysMsg(STR_EUDEMON_CANNOT_EVOLVE);
        return false;
    }
    // 召回幻兽
    CallBackEudemon(pEudemon->GetID());
    DetachEudemon(pEudemon);
    OBJID idType = pEudemon->GetInt(ITEMDATA_TYPE) + nType * 10;
    IF_NOT (pEudemon->ChangeType(idType))
    return false;
    // 成长+0.5
    pEudemon->SetInt(ITEMDATA_GROWTH, pEudemon->GetInt(ITEMDATA_GROWTH) + 50, true);
    CMsgItemInfo	msg;
    if (msg.Create(pEudemon, ITEMINFO_UPDATE))
    {
        this->SendMsg(&msg);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::RebornEudemon(OBJID idEudemonItem, OBJID idGem)
{
    CItem* pEudemon = this->GetItem(idEudemonItem);
    CHECKF (pEudemon && pEudemon->IsEudemon());
    if (pEudemon->IsAliveEudemon())
    {
        // 幻兽没有死亡，不能复活
        return false;
    }
    CItem* pGem		= this->GetItem(idGem);
    CHECKF (pGem && pGem->GetInt(ITEMDATA_TYPE) == GEM_EUDEMON_REBORN);
    this->EraseItem(idGem, SYNCHRO_TRUE);		// 先删除宝石
    pEudemon->SetInt(ITEMDATA_EUDEMON_LIFE, pEudemon->GetEudemonMaxLife(), true);	// 复活幻兽
    CMsgItemInfo	msg;
    if (msg.Create(pEudemon, ITEMINFO_UPDATE))
    {
        this->SendMsg(&msg);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::EnhanceEudemon(OBJID idEudemonItem, OBJID idGem)
{
    CItem* pEudemon = this->GetItem(idEudemonItem);
    CHECKF (pEudemon && pEudemon->IsEudemon() && pEudemon->IsAliveEudemon());
    // 召回幻兽
    CallBackEudemon(pEudemon->GetID());
    DetachEudemon(pEudemon);
    CItem* pGem		= this->GetItem(idGem);
    CHECKF (pGem && pGem->GetInt(ITEMDATA_TYPE) == GEM_EUDEMON_ENHANCE);
    int nRate = 2000;	// 追加为0时是20%成功率，为了提高精度，这里都用*100以后的数值计算
    int nAdditionLevel = pEudemon->GetInt(ITEMDATA_ADDITION);
    nRate /= pow(2.f, nAdditionLevel);	// 每+1成功率降低一半
    this->EraseItem(idGem, SYNCHRO_TRUE);
    if (::RandGet(10000) < nRate)
    {
        return pEudemon->ChangeAddition(nAdditionLevel + 1);
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CUser::AwardEudemonExp(OBJID idItem, int nExp, bool bGemEffect/*= true*/)
{
    // TODO: 给幻兽加经验...coding here
    if (nExp == 0)
    {
        return;
    }
    ST_EUDEMON* ptEudemon = this->QueryEudemonSt(idItem);
    if (!ptEudemon || !ptEudemon->pEudemon || !ptEudemon->pEudemonItem)
    {
        return;
    }
    CItem* pEudemonItem = ptEudemon->pEudemonItem;
    //	if (pEudemonItem)
    {
        if (nExp < 0)
        {
            // 扣经验
            int nNewExp = __max(0, pEudemonItem->GetInt(ITEMDATA_EXP) + nExp);
            pEudemonItem->SetInt(ITEMDATA_EXP, nNewExp, true);
            CMsgItemAttrib msg;
            if (msg.Create(pEudemonItem->GetID(), _ITEMATTRIB_EUDEMON_EXP, pEudemonItem->GetInt(ITEMDATA_EXP)))
            {
                this->SendMsg(&msg);
            }
            return;
        }
#ifdef _DEBUG
        this->SendSysMsg(_TXTATR_NORMAL, "eudemon got battle exp: %d", nExp);
#endif
        // 增加经验 -- 由于人物、幻兽共用升级经验表，因此level字段的千位被用于区分
        CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_EUDEMON * _EXP_TYPE + pEudemonItem->GetEudemonLevel());
        if (!pLevupexp)
        {
            return;
        }
        int nLevupExp	= pLevupexp->GetInt(LEVUPEXPDATA_EXP);
        // 计算原来经验的百分比——为了提高精度，精确到千份位
        int nOrgProgress = ::MulDiv(pEudemonItem->GetInt(ITEMDATA_EXP), 1000, nLevupExp);
        // 魔兽获得的经验多50%
        if (pEudemonItem->IsEvilEudemon())
        {
            const int	EVIL_EUDEMON_EXTRA_EXP_PERCENT	= 50;
            nExp = ::MulDiv(nExp, 100 + EVIL_EUDEMON_EXTRA_EXP_PERCENT, 100);
        }
        int nNewExp = nExp + pEudemonItem->GetInt(ITEMDATA_EXP);
        // 魔兽经验达到一半以上的时候有50的概率经验减半
        if (pEudemonItem->IsEvilEudemon() && nNewExp * 2 >= nLevupExp)
        {
            const int	HALF_EXP_PERCENT	= 50;
            if (::RandGet(100) < HALF_EXP_PERCENT)
            {
                nNewExp /= 2;
            }
        }
        if (nNewExp >= nLevupExp)
        {
            pEudemonItem->SetInt(ITEMDATA_EXP, 0);
            pEudemonItem->SetInt(ITEMDATA_EUDEMON_LEVEL, pEudemonItem->GetEudemonLevel() + 1, true);
            CMsgItemInfo msg;
            if (msg.Create(pEudemonItem, ITEMINFO_UPDATE))
            {
                this->SendMsg(&msg);
            }
            CMonster* pEudemon = ptEudemon->pEudemon;
            pEudemon->AddAttrib(_USERATTRIB_POTENTIAL, ADD_POTENTIAL_LEVUP, SYNCHRO_TRUE);
            if (pEudemon->QueryMagic())
            {
                pEudemon->QueryMagic()->OwnerUpLevel(pEudemonItem->GetInt(ITEMDATA_EUDEMON_LEVEL));
                pEudemon->QueryMagic()->AutoLearnMagic();
            }
            {
                CMsgUserAttrib msg;
                if (msg.Create(pEudemon->GetID(), _USERATTRIB_MAXLIFE, pEudemon->GetMaxLife()))
                {
                    if (msg.Append(_USERATTRIB_LIFE, pEudemon->GetLife()))
                    {
                        this->BroadcastRoomMsg(&msg, INCLUDE_SELF);
                    }
                }
                CMsgAction msgAction;
                if (msgAction.Create(pEudemon->GetID(), 0, 0, 0, actionUplev, 0, 0))
                {
                    pEudemon->BroadcastRoomMsg(&msgAction, INCLUDE_SELF);
                }
            }
        }
        else
        {
            pEudemonItem->SetInt(ITEMDATA_EXP, nNewExp);
            int nCurProgress = ::MulDiv(pEudemonItem->GetInt(ITEMDATA_EXP), 1000, nLevupExp);
            if (nOrgProgress / 200 != nCurProgress / 200)	//每隔1/5升级经验存一次
            {
                this->SaveInfo();
            }
            CMsgItemAttrib msg;
            if (msg.Create(pEudemonItem->GetID(), _ITEMATTRIB_EUDEMON_EXP, pEudemonItem->GetInt(ITEMDATA_EXP)))
            {
                this->SendMsg(&msg);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::AttachEudemon(OBJID idItem)
{
    CHECKF (idItem != ID_NONE);
    CItem* pItem = this->GetItem(idItem);
    CMonster* pEudemon = this->QueryEudemon(idItem);
    if (!(pItem && pEudemon))
    {
        return false;
    }
    // 解除旧有的附身状态
    DetachEudemon();
    m_idLinkEudemonItem	= pItem->GetID();
    m_idLinkEudemonType	= ((pItem->GetInt(ITEMDATA_TYPE) % 100000) / 1000) * 1000;
    // TODO：确定附加属性值
    switch (m_idLinkEudemonType)
    {
    case	ITEMTYPE_EUDEMON_SPEED:			// 速度型
        m_nLinkValue = pEudemon->GetInterAtkRate() / 10;
        break;
    case	ITEMTYPE_EUDEMON_PATK:			// 攻击型
        m_nLinkValue = pEudemon->GetAttack() / 10;
        break;
    case	ITEMTYPE_EUDEMON_DEF:			// 防御型
        m_nLinkValue = pEudemon->GetLife() / 10;
        break;
    case	ITEMTYPE_EUDEMON_MATK:			// 魔法攻击型
        m_nLinkValue = ((pEudemon->GetMgcMaxAtk() + pEudemon->GetMgcMinAtk()) / 2) / 10;
        break;
    case	ITEMTYPE_EUDEMON_BOMB:			// 爆破型
        m_nLinkValue = pEudemon->GetMagicDef() / 10;
        break;
    case	ITEMTYPE_EUDEMON_PROTECTIVE:	// 保护型
        m_nLinkValue = pEudemon->GetDef() / 10;
        break;
    case	ITEMTYPE_EUDEMON_ATTACH:		// 附身型
        m_nLinkValue = pEudemon->GetDdg() / 10;
        break;
    case	ITEMTYPE_EUDEMON_VARIATIONAL:	// 变异型
        m_nLinkValue = pEudemon->GetAtkHitRate() / 10;
        break;
    }
    // TODO: 添加魔法
    CMagic* pMagic = pEudemon->QueryMagic();
    if (pMagic && this->QueryMagic())
    {
        IMagicSet* pMagicSet = pMagic->QuerySet();
        if (pMagicSet)
        {
            for (int i = 0; i < pMagicSet->GetAmount(); i++)
            {
                IMagicData* pData = pMagicSet->GetObjByIndex(i);
                if (pData)
                {
                    const int	EUDEMON2USER_MAGIC_OFFSET	= 1000;
                    int nType	= pData->GetInt(MAGICDATA_TYPE) + EUDEMON2USER_MAGIC_OFFSET;
                    int nLevel	= pData->GetInt(MAGICDATA_LEVEL);
                    if (CMagic::FindMagicType(nType, nLevel) && !this->QueryMagic()->FindMagic(nType))
                    {
                        bool bSaveMagic = false;
                        OBJID idMagic = this->QueryMagic()->LearnMagic(pData->GetInt(MAGICDATA_TYPE), pData->GetInt(MAGICDATA_LEVEL), bSaveMagic);
                        if (idMagic != ID_NONE)
                        {
                            m_setLinkMagic.push_back(nType);
                        }
                    }
                }
            }
        }
    }
    // 收回幻兽
    CallBackEudemon(idItem);
    // 通知客户端
    CMsgItem msg;
    if (msg.Create(idItem, ITEMACT_ATTACH_EUDEMON))
    {
        SendMsg(&msg);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::DetachEudemon(CItem* pItem)
{
    if (ID_NONE == m_idLinkEudemonItem)
    {
        return;
    }
    if (pItem && pItem->GetID() != m_idLinkEudemonItem)
    {
        return;
    }
    if (!pItem)
    {
        pItem = this->GetItem(m_idLinkEudemonItem);
    }
    CHECK (pItem);
    // 通知客户端
    CMsgItem msg;
    if (msg.Create(m_idLinkEudemonItem, ITEMACT_DETACH_EUDEMON))
    {
        SendMsg(&msg);
    }
    m_idLinkEudemonItem	= ID_NONE;
    m_idLinkEudemonType = ID_NONE;
    m_nLinkValue = 0;
    // TODO: 删除魔法
    if (this->QueryMagic())
    {
        LINKMAGIC_SET::iterator it = m_setLinkMagic.begin();
        for (; it != m_setLinkMagic.end(); it++)
        {
            int nType = *it;
            this->QueryMagic()->UnlearnMagic(nType, true);
        }
        m_setLinkMagic.clear();
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsItemFull(int nWeight, OBJID idItemType, DWORD dwData)
{
    // 不考虑负重
    return m_pPackage->IsPackFull(idItemType, nWeight, dwData);
}


//////////////////////////////////////////////////////////////////////
// tutor

bool CUser::AddTutor(OBJID idTutor, LPCTSTR szName)
{
    CHECKF(szName);
    m_pTutor	= CTutor::CreateNew();
    if (m_pTutor)
    {
        if (m_pTutor->CreateRecord(this->GetID(), idTutor, this->GetName(), szName, Database()))
        {
            // 这里还需要通知对方
            CUser* pTarget = UserManager()->GetUser(idTutor);
            if (pTarget)
            {
                CMsgSchoolMember msg;
                if (msg.Create(MESSAGEBOARD_ADDMEMBER, NULL, 0))
                {
                    msg.Append(RELATION_TEACHER, MEMBERSTATUS_ONLINE, pTarget);
                    SendMsg(&msg);
                }
                pTarget->AwardTutorExp(-1 * ADDSTUDENT_TUTOREXP);
                pTarget->AddStudentRef(m_pTutor->GetInfo());
            }
            else
            {
                // TODO: 通过世界核心在其它MapGroup中寻找并通知
                CMsgSchoolMember msg;
                if (msg.Create(MESSAGEBOARD_ADDMEMBER, NULL, 0))
                {
                    if (msg.Append(RELATION_TEACHER, MEMBERSTATUS_OFFLINE, idTutor, szName))
                    {
                        SendMsg(&msg);
                        MapGroup(PID)->QueryIntraMsg()->TransmitMsg(&msg, this->GetSocketID(), this->GetID());
                    }
                }
            }
            return true;
        }
        else
        {
            SAFE_DELETE (m_pTutor);
        }
    }
    return false;
}

bool CUser::DelTutor()
{
    if (!m_pTutor)
    {
        return false;
    }
    this->SendSysMsg(STR_LEAVE_TEACHER, m_pTutor->GetTutorName());
    OBJID idTutor = m_pTutor->GetTutorID();
    m_pTutor->DeleteRecord(Database());
    SAFE_DELETE (m_pTutor);
    if (this->GetLev() >= LEAVETUTOR_LEVEL)
    {
        // 超过10级的玩家叛师，扣前3级升级经验
        int nExpPunish	= 0;
        for (int i = 1; i <= 3; i++)
        {
            CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev() - i);
            if (pLevupexp)
            {
                nExpPunish	+= pLevupexp->GetInt(LEVUPEXPDATA_EXP);
            }
        }
        this->AwardBattleExp(-1 * nExpPunish);
    }
    CMsgSchoolMember	msg;
    if (msg.Create(MESSAGEBOARD_DELMEMBER, NULL, 0))
    {
        if (msg.Append(RELATION_TEACHER, MEMBERSTATUS_OFFLINE, idTutor))
        {
            SendMsg(&msg);
        }
    }
    // 这里只是删除了师傅的数据库记录，还应该通知师傅删除内存数据——如果师傅在线的话
    CUser* pUser = UserManager()->GetUser(idTutor);
    if (pUser)
    {
        pUser->DelStudentRef(GetID());
    }
    else
    {
        // 通知其它MapGroup寻找师傅并删除徒弟
    }
    return true;
}

bool CUser::DelTutorRef()
{
    if (!m_pTutor)
    {
        return false;
    }
    SendSysMsg(STR_LEAVE_TEACHER4, m_pTutor->GetTutorName());
    OBJID idTutor = m_pTutor->GetTutorID();
    SAFE_DELETE (m_pTutor);
    CMsgSchoolMember	msg;
    if (msg.Create(MESSAGEBOARD_DELMEMBER, NULL, 0))
    {
        if (msg.Append(RELATION_TEACHER, MEMBERSTATUS_OFFLINE, idTutor))
        {
            SendMsg(&msg);
        }
    }
    return true;
}

CTutor* CUser::GetStudent(OBJID idStudent)
{
    TUTOR_SET::iterator it = m_setStudent.begin();
    for (; it != m_setStudent.end(); it++)
    {
        CTutor* pTutor = *it;
        if (pTutor->GetUserID() == idStudent)
        {
            return pTutor;
        }
    }
    return NULL;
}

bool CUser::AddStudentRef(const TutorInfoStruct* pInfo)
{
    CHECKF (pInfo);
    TUTOR_SET::iterator it = m_setStudent.begin();
    for (; it != m_setStudent.end(); it++)
    {
        CTutor* pTutor = *it;
        if (pTutor->GetUserID() == pInfo->idUser && pTutor->GetTutorID() == this->GetID())
        {
            return false;
        }
    }
    CTutor* pTutor = CTutor::CreateNewTutor(pInfo);
    if (pTutor)
    {
        m_setStudent.push_back(pTutor);
        CMsgSchoolMember	msg;
        if (msg.Create(MESSAGEBOARD_ADDMEMBER, NULL, 0))
        {
            CUser* pUser = UserManager()->GetUser(pTutor->GetUserID());
            if (pUser)
            {
                if (msg.Append(RELATION_STUDENT, MEMBERSTATUS_ONLINE, pUser))
                {
                    SendMsg(&msg);
                }
            }
            else
            {
                if (msg.Append(RELATION_STUDENT, MEMBERSTATUS_OFFLINE, pTutor->GetUserID(), pTutor->GetUserName()))
                {
                    SendMsg(&msg);
                    MapGroup(PID)->QueryIntraMsg()->TransmitMsg(&msg, this->GetSocketID(), this->GetID());
                }
            }
        }
        return true;
    }
    return false;
}
DWORD   CUser::GetAllTutorLevel()
{
    //为了在新手工会中排序公告而查找所有学员的等级和。LW
    /*	DWORD dLevel = 0;
    	TUTOR_SET::iterator it = m_setStudent.begin();
    	for (; it != m_setStudent.end(); it++)
    	{
    		CTutor* pTutor = *it;
    		OBJID idStudent = pTutor->GetUserID();
    		dLevel += UserManager()->GetUser(idStudent)->GetLev();
    	}*/
    return 0;
}
bool CUser::DelStudent(OBJID idStudent)
{
    TUTOR_SET::iterator it = m_setStudent.begin();
    for (; it != m_setStudent.end(); it++)
    {
        CTutor* pTutor = *it;
        if (pTutor->GetUserID() == idStudent && pTutor->GetTutorID() == GetID())
        {
            this->SendSysMsg(STR_LEAVE_TEACHER, pTutor->GetUserName());
            OBJID idStudent = pTutor->GetUserID();
            pTutor->DeleteRecord(Database());
            SAFE_DELETE (pTutor);
            m_setStudent.erase(it);
            // 开除学员要扣导师经验作为惩罚
            m_data.SetTutorExp(GetTutorExp() - DELSTUDENT_TUTOREXP);
            CMsgSchoolMember	msg;
            if (msg.Create(MESSAGEBOARD_DELMEMBER, NULL, 0))
            {
                if (msg.Append(RELATION_STUDENT, MEMBERSTATUS_OFFLINE, idStudent))
                {
                    SendMsg(&msg);
                }
            }
            CUser* pUser = UserManager()->GetUser(idStudent);
            if (pUser)
            {
                pUser->DelTutorRef();
            }
            else
            {
                // 通知其它MapGroup寻找Student并删除导师
            }
            return true;
        }
    }
    return false;
}

bool CUser::DelStudentRef(OBJID idStudent)
{
    TUTOR_SET::iterator it = m_setStudent.begin();
    for (; it != m_setStudent.end(); it++)
    {
        CTutor* pTutor = *it;
        if (pTutor->GetUserID() == idStudent && pTutor->GetTutorID() == GetID())
        {
            this->SendSysMsg(STR_LEAVE_TEACHER3, pTutor->GetUserName());
            SAFE_DELETE (pTutor);
            m_setStudent.erase(it);
            CMsgSchoolMember	msg;
            if (msg.Create(MESSAGEBOARD_DELMEMBER, NULL, 0))
            {
                if (msg.Append(RELATION_STUDENT, MEMBERSTATUS_OFFLINE, idStudent))
                {
                    SendMsg(&msg);
                }
            }
            return true;
        }
    }
    return false;
}

bool CUser::CreateAllTutor(IDatabase* pDb)
{
    // create tutor
    SQLBUF		szSQL;
    sprintf(szSQL, "SELECT * FROM %s WHERE user_id=%u LIMIT 1", _TBL_TUTOR, GetID());
    IRecordset*	pRes = pDb->CreateNewRecordset(szSQL);
    if (pRes)
    {
        m_pTutor = CTutor::CreateNew();
        if (m_pTutor)
        {
            if (!m_pTutor->Create(pRes))
            {
                SAFE_DELETE (m_pTutor);
            }
        }
        SAFE_RELEASE (pRes);
    }
    // create student
    sprintf(szSQL, "SELECT * FROM %s WHERE tutor_id=%u LIMIT %d", _TBL_TUTOR, GetID(), _MAX_TUTORSIZE);
    pRes = pDb->CreateNewRecordset(szSQL);
    if (pRes)
    {
        for(int i = 0; i < pRes->RecordCount(); i++)
        {
            CTutor*	pTutor = CTutor::CreateNew();
            if (pTutor)
            {
                if (pTutor->Create(pRes))
                {
                    m_setStudent.push_back(pTutor);
                }
                else
                {
                    SAFE_DELETE (pTutor);
                }
            }
            pRes->MoveNext();
        }
        SAFE_RELEASE (pRes);
    }
    return true;
}

void CUser::DeleteAllTutor()
{
    SAFE_DELETE (m_pTutor);
    TUTOR_SET::iterator it = m_setStudent.begin();
    for (; it != m_setStudent.end(); it++)
    {
        CTutor* pTutor = *it;
        SAFE_DELETE (pTutor);
    }
    m_setStudent.clear();
}

void CUser::SendTutorInfo()
{
    if (!m_pTutor && GetStudentAmount() == 0)
    {
        return;
    }
    CMsgSchoolMember	msg;
    msg.Create(MESSAGEBOARD_ADDMEMBER, NULL, 0);
    if (m_pTutor)
    {
        // 本地图组找不到的都作为离线处理，回头由其它地土组负责返回在线消息
        CUser* pUser = UserManager()->GetUser(m_pTutor->GetTutorID());
        if (pUser)
        {
            msg.Append(RELATION_TEACHER, MEMBERSTATUS_ONLINE, pUser);
        }
        else
        {
            msg.Append(RELATION_TEACHER, MEMBERSTATUS_OFFLINE, m_pTutor->GetTutorID(), m_pTutor->GetTutorName());
        }
    }
    TUTOR_SET::iterator it = m_setStudent.begin();
    for (; it != m_setStudent.end(); it++)
    {
        CTutor* pTutor = *it;
        if (pTutor)
        {
            CUser* pUser = UserManager()->GetUser(pTutor->GetUserID());
            if (pUser)
            {
                msg.Append(RELATION_STUDENT, MEMBERSTATUS_ONLINE, pUser);
            }
            else
            {
                msg.Append(RELATION_STUDENT, MEMBERSTATUS_OFFLINE, pTutor->GetUserID(), pTutor->GetUserName());
            }
        }
    }
    this->SendMsg(&msg);
    MapGroup(PID)->QueryIntraMsg()->TransmitMsg(&msg, this->GetSocketID(), this->GetID());		// 通知其它地图组，下传给我师徒在线消息
}

void CUser::AwardTutorExp(int nTutorExp)
{
    int nExp = GetTutorExp() + nTutorExp;
    m_data.SetTutorExp(nExp);
}

bool CUser::TakeExpFromStudent(int nExpTake)
{
    if (GetLev() < _MIN_TUTOR_LEVEL)
    {
        return false;
    }
    CTutorexpData*	pExpData;
    pExpData = CTutorexpData::CreateNew();
    if (!pExpData)
    {
        return false;
    }
    if (pExpData->Create(GetID(), Database()))
    {
        // 获得徒弟贡献的经验
        int nExp = pExpData->GetInt(TUTOREXPDATA_EXP);
        if (nExpTake > nExp)
        {
            return false;
        }
        // 根据导师等级以及所取得的贡献经验计算消耗的导师经验
        int nGetExpPerTutorexp = GETEXP_PER_TUTOREXP0;
        if (this->GetTutorLevel() == 1)
        {
            nGetExpPerTutorexp = GETEXP_PER_TUTOREXP1;
        }
        else if (this->GetTutorLevel() == 2)
        {
            nGetExpPerTutorexp = GETEXP_PER_TUTOREXP2;
        }
        int nCostTutorExp = nExpTake / nGetExpPerTutorexp + ((nExpTake % nGetExpPerTutorexp > 0) ? 1 : 0);
        if (this->GetTutorExp() < nCostTutorExp)
        {
            this->SendSysMsg(STR_NOT_ENOUGH_TUTOREXP);
            return false;
        }
        this->AwardTutorExp(-1 * nCostTutorExp);		// 先扣除导师经验
        pExpData->SetInt(TUTOREXPDATA_EXP, nExp - nExpTake);
        this->AwardBattleExp(nExpTake);			// 得到贡献经验
        // 向导师传递经验
        int nTutorExp = pExpData->GetInt(TUTOREXPDATA_TUTOREXP);
        pExpData->SetInt(TUTOREXPDATA_TUTOREXP, 0);
        pExpData->Update();
        if (GetTutor() && nTutorExp > 0)
        {
            SQLBUF szSQL;
            sprintf(szSQL, "UPDATE %s SET exp=exp+%u WHERE tutor_id=%u AND tutor_lev>%u LIMIT 1",
                    _TBL_TUTOREXP, nTutorExp, m_pTutor->GetTutorID(), GetLev());
            Database()->ExecuteSQL(szSQL);
        }
    }
    SAFE_RELEASE (pExpData);
    return true;
}

void CUser::NotifyTutorStatus(int nStatus)
{
    if (!m_pTutor && m_setStudent.size() <= 0)
    {
        return;
    }
    // 通知自己所有的师徒自己目前状态
    CMsgSchoolMember	msg;
    if (m_pTutor)
    {
        msg.Create(MESSAGEBOARD_UPDATEMEMBER, NULL, 0);
        msg.Append(RELATION_STUDENT, nStatus, this);
        MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, m_pTutor->GetTutorID());
    }
    char szMsg[_MAX_WORDSSIZE];
    if (nStatus == MEMBERSTATUS_ONLINE)
    {
        sprintf(szMsg, STR_TUTOR_LOGIN, this->GetName());
    }
    else
    {
        sprintf(szMsg, STR_TUTOR_LOGOUT, this->GetName());
    }
    msg.Create(MESSAGEBOARD_UPDATEMEMBER, NULL, 0);
    msg.Append(RELATION_TEACHER, nStatus, this);
    TUTOR_SET::iterator it = m_setStudent.begin();
    for (; it != m_setStudent.end(); it++)
    {
        CTutor* pTutor = *it;
        if (pTutor)
        {
            CUser* pUser = UserManager()->GetUser(pTutor->GetUserID());
            if (pUser)
            {
                pUser->SendMsg(&msg);
            }
            else
            {
                MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, pTutor->GetUserID());
            }
            CMsgTalk	msgNotify;
            if (msgNotify.Create(SYSTEM_NAME, pTutor->GetUserName(), szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
            {
                if (pUser)
                {
                    pUser->SendMsg(&msgNotify);
                }
                else
                {
                    MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msgNotify, pTutor->GetUserID());
                }
            }
        }
    }
}

//---jinggy---2004-11-12---用武器上嵌入的攻击类宝石攻击对方
bool CUser::UseSkillAtk(OBJID idDes)
{
    /*

    for(int i=ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
    	CItemPtr pEquipItem = GetEquipItemByPos(i);

    	if (!pEquipItem)
    		continue;

    	//得到宝石类型判断是不是有效类型
    	DWORD dwGemType = pEquipItem->GetInt(ITEMDATA_GEMTYPE);
    	CItemTypeData * pItemTypeData = ItemType()->QueryItemType(dwGemType);
    	if (!pItemTypeData)
    		return false ;

    	//该类型宝石可使用的次数
    //		DWORD dwGemAvailableTime = pEquipItem->GetInt(ITEMDATA_AVAILABLETIME);

    	if (dwGemAvailableTime<=0)
    		return false ;

    	if (RandGet(100)<=1) {

    		//用该宝石攻击
    		UserItemToByType(idDes,dwGemType);

    		//装备攻击的宝石攻击次数减一。
    //			pEquipItem->SetInt(ITEMDATA_AVAILABLETIME, -- dwGemAvailableTime);
    		pEquipItem->SaveInfo();

    		CMsgItemInfo msg;
    		IF_OK(msg.Create(pEquipItem,ITEMINFO_UPDATE)) //通知客户端更新 装备
    			SendMsg(&msg);

    		return true ;
    	}
    }
    */
    return false ;
}
//更新穿在身上的装备的经验值
/*
  只有穿在身上 10 分钟后才给装备 升一点的经验值
*/
//---jinggy---2004-11-17--
void CUser::UpdateEquipmentExp()
{
    if (!m_bIsEnableWarGhostExp)
    {
        return;
    }
    if (m_tEquipWarGhostExp.ToNextTime())
    {
        for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
        {
            if (m_arrTimeOutForEquipExp[i-ITEMPOSITION_EQUIPBEGIN].IsActive()
                    && m_arrTimeOutForEquipExp[i-ITEMPOSITION_EQUIPBEGIN].ToNextTime())
            {
                CItemPtr pItem = GetEquipItemByPos(i);
                if (pItem)
                {
                    pItem->WarGhostLevelUpgrade(this);
#ifdef _DEBUG
                    char szInfo[255];
                    sprintf(szInfo, "物品 %s 的战魂经验值 %d", pItem->GetStr(ITEMDATA_NAME), pItem->GetInt(ITEMDATA_WARGHOSTEXP));
                    this->SendSysMsg(_TXTATR_NORMAL, szInfo);
#endif
                    if (pItem->GetWarGhostLevel() == MAX_LEVEL_WARLEVEL - 1)
                    {
                        UpdateEquipmentExp_Startup(i, EQUIPMENT_EXP_ADD_SECS_AFTER9);
                    }
                    else if (pItem->GetWarGhostLevel() >= MAX_LEVEL_WARLEVEL)
                    {
                        UpdateEquipmentExp_Stop(i);
                    }
                    if (pItem->GetWarGhostLevel() >= MAX_LEVEL_WARLEVEL - 1)
                    {
                        m_arrEquipIncExp[i-ITEMPOSITION_EQUIPBEGIN]++;
                        if (pItem->IsWeapon())
                        {
                            if (m_arrEquipIncExp[i-ITEMPOSITION_EQUIPBEGIN] >= WEAPON_UPEXP_MAXVALUE)
                            {
                                UpdateEquipmentExp_Stop(i);
                            }
                        }
                        else if (pItem->IsArmor())
                        {
                            if (m_arrEquipIncExp[i-ITEMPOSITION_EQUIPBEGIN] >= ARMOR_UPEXP_MAXVALUE)
                            {
                                UpdateEquipmentExp_Stop(i);
                            }
                        }
                        else if (pItem->IsNecklace())
                        {
                            if (m_arrEquipIncExp[i-ITEMPOSITION_EQUIPBEGIN] >= NECKLACE_UPEXP_MAXVALUE)
                            {
                                UpdateEquipmentExp_Stop(i);
                            }
                        }
                        else if (pItem->IsShoes())
                        {
                            if (m_arrEquipIncExp[i-ITEMPOSITION_EQUIPBEGIN] >= SHOE_UPEXP_MAXVALUE)
                            {
                                UpdateEquipmentExp_Stop(i);
                            }
                        }
                        else if (pItem->IsBangle() || pItem->IsRing())
                        {
                            if (m_arrEquipIncExp[i-ITEMPOSITION_EQUIPBEGIN] >= BANGLE_UPEXP_MAXVALUE)
                            {
                                UpdateEquipmentExp_Stop(i);
                            }
                        }
                    }
                }
            }
        }
    }
}

//停止指定设备的记数器
void CUser::UpdateEquipmentExp_Stop(DWORD dwPos)
{
    DEBUG_TRY
    IF_NOT(dwPos >= ITEMPOSITION_EQUIPBEGIN && dwPos < ITEMPOSITION_EQUIPEND)
    return;
    m_arrTimeOutForEquipExp[dwPos-ITEMPOSITION_EQUIPBEGIN].Clear();
    m_arrEquipIncExp[dwPos-ITEMPOSITION_EQUIPBEGIN] = 0;
    DEBUG_CATCH("void CUser::UpdateEquipmentExp_Stop(DWORD dwPos)")
}
//启动指定设备的记数器
void CUser::UpdateEquipmentExp_Startup(DWORD dwPos, int nSecs)
{
    DEBUG_TRY
    IF_NOT(dwPos >= ITEMPOSITION_EQUIPBEGIN && dwPos < ITEMPOSITION_EQUIPEND)
    return;
    m_arrTimeOutForEquipExp[dwPos-ITEMPOSITION_EQUIPBEGIN].Startup(nSecs);
    DEBUG_CATCH("void CUser::UpdateEquipmentExp_Startup(DWORD dwPos)")
}
//初始化所有设备的记数器
void CUser::UpdateEquipmentExp_Init()
{
    DEBUG_TRY
    if (DateStamp() != this->GetLastLogin())
    {
        m_bIsEnableWarGhostExp = true;
    }
    //---初始化计数器
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; ++i)
    {
        m_arrTimeOutForEquipExp[i-ITEMPOSITION_EQUIPBEGIN].Clear();
        m_arrEquipIncExp[i-ITEMPOSITION_EQUIPBEGIN] = 0;
    }
    if (DateStamp() != this->GetLastLogin())
    {
        time_t	tCurr = time(NULL);
        tm	tmComp;
        memset(&tmComp, 0, sizeof(tm));
        tmComp.tm_year	= DateStamp() / 10000 - 1900;
        tmComp.tm_mon	= (DateStamp() / 100) % 100 - 1;
        tmComp.tm_mday	= DateStamp() % 100;
        time_t	tCur = mktime(&tmComp);
        memset(&tmComp, 0, sizeof(tm));
        tmComp.tm_year	= GetLastLogin() / 10000 - 1900;
        tmComp.tm_mon	= (GetLastLogin() / 100) % 100 - 1;
        tmComp.tm_mday	= GetLastLogin() % 100;
        time_t	tLast = mktime(&tmComp);
        time_t tResult = tCur - tLast;
        int nDays = tResult / 86400 - 1;
        m_tEquipWarGhostExp.Startup(WARGHOST_CHECK_INTERVAL);
        for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
        {
            CItemPtr pItem = GetEquipItemByPos(i);
            if (pItem)
            {
                //降低战魂等级
                pItem->WarGhostLevelDec(nDays * 2);
                if (pItem->GetWarGhostLevel() == MAX_LEVEL_WARLEVEL)
                {
                    continue;
                }
                if (pItem->GetWarGhostLevel() == MAX_LEVEL_WARLEVEL - 1 )
                {
                    UpdateEquipmentExp_Startup(i, EQUIPMENT_EXP_ADD_SECS_AFTER9);
                }
                else if (pItem->GetWarGhostLevel() < MAX_LEVEL_WARLEVEL - 1 )
                {
                    UpdateEquipmentExp_Startup(i, EQUIPMENT_EXP_ADD_SECS);
                }
            }
        }
    }
    DEBUG_CATCH("void CUser::UpdateEquipmentExp_Init()S")
}

//是否是帮派帮主
bool CUser::IsSynLeader()
{
    return GetSynRank() == RANK_LEADER;
}

void CUser::AddMoney(DWORD dwMoney)
{
    AddAttrib(_USERATTRIB_MONEY, dwMoney, true);
}
//得到最后登陆时间
DWORD CUser::GetLastLogin()
{
    return this->m_data.GetLastLogin();
}
//是否是嵌入装备的宝石的类型
bool CUser::IsEmbedGemType(int nGemType)
{
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItemPtr pItem = GetEquipItemByPos(i);
        if (pItem)
        {
            if (pItem->GetInt(ITEMDATA_GEMTYPE) == nGemType)
            {
                return true;
            }
        }
    }
    return false;
}

bool CUser::FlyToMainMap()
{
    if (QueryMagic())
    {
        if (QueryMagic()->IsInLaunch())	// 魔法施展过程中
        {
            return false;
        }
        QueryMagic()->AbortMagic();
    }
    OBJID idMap = this->GetMapID();
    idMap = (idMap / 1000) * 1000; //主地图ID
    CGameMap* pMap = MapManager()->QueryMap(idMap);
    if (pMap)
    {
        OBJID idRebornMap ;
        POINT pt;
        pMap->GetRebornMap(&idRebornMap, &pt);
        ProcessOnMove(MOVEMODE_CHGMAP);
        if (this->FlyMap(idRebornMap, pt.x, pt.y) == FLYMAP_NORMAL)
        {
            ProcessAfterMove();
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        SendSysMsg(STR_ACTION_USER_FLYTOMAINMAP_ERROR);
        return false;
    }
}
//得到可以用的帮派基金
int CUser::GetAvailableSynFund()
{
    CSyndicate* pSyn = this->GetSyndicate();
    if (pSyn)
    {
        return MulDiv(pSyn->GetMoney(), 1, 2);
    }
    return 0;
}

bool CUser::ApplyAttackSyn(CNpc* pNpc)
{
    if (!IsSynLeader())
    {
        return false;
    }
    if (!pNpc->IsSynWarLetter())
    {
        return false;
    }
    {
        CGameMap* pSynMap = MapManager()->GetGameMap(pNpc->GetMapID());
        if (!pSynMap->IsSynMap())
        {
            return false;
        }
        //判断军团等级
        int nRank = GetSynRank();
        if (nRank == SYNRANK_ONE || nRank == SYNRANK_TWO)
        {
            return false;
        }
        //检查是否已经有人下了战书
        if (pNpc->GetData(SYNWARLETTER_ISATTACK) > 0)
        {
            SendSysMsg(STR_SYN_APPLYATTACKSYN_HASBEENAPPLIED);
            return false;
        }
        if (pNpc->GetData(SYNWARLETTER_OCCUPYTIME) == 0)	//没有被占领
        {
            //记录占领的时间---begin
            pNpc->GetMap()->SetSynOccupyTime();
            //记录占领的时间---end
            CNpc* pSynFlagNpc = pSynMap->GetNpcSynFlag();
            IF_NOT(pSynFlagNpc)
            return false;
            pSynFlagNpc->SetSynOwnerID(GetSyndicate()->GetID(), false);		// true: with link map
            pSynFlagNpc->GetMap()->SetSynID(GetSyndicate()->GetID(), true);		// with all map npc
            //			pSynFlagNpc->SendSelfToBlock();
            MSGBUF	szMsg;
            sprintf(szMsg, STR_GOT_WIN_s, GetSyndicate()->GetStr(SYNDATA_NAME));
            CMsgTalk msg;
            IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, _COLOR_WHITE, _TXTATR_GM))
            UserManager()->BroadcastMsg(&msg);
            // owner flag
            CGameMap* pMap = pNpc->GetMap();
            if (pMap->GetID() == WHITE_SYN_MAP_ID)
            {
                CMsgSyndicate	msg;
                IF_OK(msg.Create(SET_WHITE_SYN, pMap->GetOwnerID()))
                UserManager()->BroadcastMsg(&msg);
            }
            else if (pMap->GetID() == BLACK_SYN_MAP_ID)
            {
                CMsgSyndicate	msg;
                IF_OK(msg.Create(SET_BLACK_SYN, pMap->GetOwnerID()))
                UserManager()->BroadcastMsg(&msg);
            }
            return false;
        }
        //计算此地图的价值
        int nSynMapValue = pSynMap->GetSynMapValue();
        CSyndicate* pOwnerSyn = SynManager()->QuerySyndicate(pSynMap->GetSynID());
        //判断申请者是否符合要求
        CSyndicate* pSyn = GetSyndicate();
        if (pSyn->GetMoney() < nSynMapValue)//资金不够
        {
            SendSysMsg(STR_SYN_APPLYATTACKSYN_FAIL);
            return false;
        }
        //写下战书的信息
        pNpc->SetData(SYNWARLETTER_ISATTACK, 1, true);
        pNpc->SetData(SYNWARLETTER_ATTACK_SYNID, pSyn->GetID(), true);
        pNpc->SetData(SYNWARLETTER_DEPOSIT, nSynMapValue, true);
        //扣除帮派押金
        pSyn->QueryModify()->AddData(SYNDATA_MONEY, -nSynMapValue);
        //计算战争时间
        CNpc* pNpcSynFlag = pSynMap->GetNpcSynFlag();
        char szAnnounce[255], szAnnounce2[255];
        if (pNpcSynFlag)
        {
            time_t	tCur;
            tCur = time(NULL);
            tm* pTm = localtime(&tCur);
            tm	tmS;
            memset(&tmS, 0, sizeof(tm));
            tmS = *pTm;
            tmS.tm_mday	+= 3;
            tmS.tm_hour = 20;
            tmS.tm_min = 0;
            tmS.tm_sec = 0;
            time_t	tS	= mktime(&tmS);
            sprintf(szAnnounce, STR_SYNWAR_ANNOUNCE, tmS.tm_year + 1900, tmS.tm_mon + 1, tmS.tm_mday, pOwnerSyn->GetStr(SYNDATA_NAME));
            sprintf(szAnnounce2, STR_SYNWAR_ANNOUNCE2, tmS.tm_year + 1900, tmS.tm_mon + 1, tmS.tm_mday, pSyn->GetStr(SYNDATA_NAME));
            tmS.tm_hour = 23;
            time_t	tE	= mktime(&tmS);
            pNpcSynFlag->SetData(SYNFLAGNPC_FIGHTBEGIN, (int)tS, true);
            pNpcSynFlag->SetData(SYNFLAGNPC_FIGHTEND, (int)tE, true);
        }
        GetSyndicate()->QueryModify()->SetTenet(szAnnounce);
        GetSyndicate()->BroadcastSubSynMsg(GetSocketID(), ID_NONE, szAnnounce, NULL, NULL, 0xFFFFFF, _TXTATR_SYNANNOUNCE);
        pOwnerSyn->QueryModify()->SetTenet(szAnnounce2);
        pOwnerSyn->BroadcastSubSynMsg(GetSocketID(), ID_NONE, szAnnounce2, NULL, NULL, 0xFFFFFF, _TXTATR_SYNANNOUNCE);
    }
    //	SendSysMsg(_TXTATR_MSG_SYN,STR_SYN_APPLYATTACKSYN_SUCCESS);
    return true;
}

//向WorldKernel发送玩家离线消息
//void CUser::NotifyChatRoomOffline()
//{
//	CMsgChatRoom	msg;
//	if (msg.Create(ACTION_OFFLINE))
//	{
//		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this->GetSocketID(),&msg);
//	}
//
//}

void CUser::SetSynDressArmorType(int nArmorType)
{
    if (nArmorType <= 0)
    {
        return ;
    }
    m_nSynDressArmorType = nArmorType;
}


bool CUser::FlyToRandomPos()
{
    if (QueryMagic())
    {
        if (QueryMagic()->IsInLaunch())	// 魔法施展过程中
        {
            return false;
        }
        QueryMagic()->AbortMagic();
    }
    int nPosX = 0;
    int nPosY = 0;
    if (GetMap()->GetRandomPos(nPosX, nPosY))
    {
        ActiveTransPos(nPosX, nPosY);
        return true;
    }
    return false;
}

CTaskDetail* CUser::GetTaskDetail()
{
    return m_pTaskDetail;
}
