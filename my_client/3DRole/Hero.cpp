
//**********************************************************
// 代码编辑器
//**********************************************************

// Hero.cpp: implementation of the CHero class.
//
//////////////////////////////////////////////////////////////////////
#include "basefunc.h"
#include "3DGameMap.h"
#include "GameMsg.h"
#include "Hero.h"
#include "GamePlayerSet.h"
#include "GameDataSet.h"
#include "AllMsg.h"
#include "MsgAllot.h"
#include "ImgNumCounter.h"
#include "3DRoleData.h"
#include "SkyLayer.h"
#include "EncryptData.h"
#include "Pet.h"
#include "ItemObserver.h"
#include "NetCmd.h"
#include "NDsound.h"
// external...
extern UINT g_uStatus;

// globle...
CHero g_objHero;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHero::CHero()
{
    m_nType = MAP_HERO;
    m_dwMoney		= 0;
    m_timeDie		= 0;
    m_timeGhost		= 0;
    m_idAccount		= ID_NONE;
    m_posAlign.x	= -1;
    m_posAlign.y	= -1;
    m_nPkMode = PKMODE_FREE;
    m_setFriend.clear();
    m_nHeightCanJump = 200;
    m_bXpSkillEnable = FALSE;
    m_dwXpFullTime = 0;
    m_dwXpStartTime = 0;
    m_dwKoCount = 0;
    m_dwKoScale = 0;
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        m_pEquipment[i] = NULL;
    }
    m_setSingleHandWeaponSkill.clear();
    m_setDoubleHandWeaponSkill.clear();
    m_setOtherWeaponSkill.clear();
    m_bMagicAttackBusy = false;
    this->CreateLevelExp();
    m_bFriendQueryBusy = false;
    m_idFriendQuery = ID_NONE;
    m_bFriendQueryReturn = false;
    strcpy(m_szSyndicateAnnounce, "");
    strcpy(m_szSyndicateAnnounceTime, "");
    this->SetRoleType(_ROLE_HERO);
    m_dwTimeSynWarReportLastFlash = ::TimeGet();
    memset(m_szWanted, 0L, sizeof(m_szWanted));
    memset(m_szPoliceWanted, 0L, sizeof(m_szPoliceWanted));
    m_dwBeginUseTransformMagicTime = ::TimeGet();
    m_dwTransformBegin = ::TimeGet();
    m_dwTransformDuration = 0;
    m_dwLastBump = 0;
    m_bShowAbortTransformDialog = false;
    m_nSynListIndex = 0;
    m_dwKoXpAdd = 0;
    m_bChargeUp = false;
    m_dwLastTalk = 0;
    m_idWhoAttackMe	=	ID_NONE;
    m_idMyTarget	=	ID_NONE;
    m_idSpecialTarget	=	ID_NONE;
    m_idFriendLastApply =	ID_NONE;
    strcpy(m_szActionMessageBox, "");
    strcpy(m_szFriendLastApplyInfo, "");
    strcpy(m_szFriendLastAcceptInfo, "");
    m_idTaskDialog = ID_NONE;
    m_idAttchPet = ID_NONE;
    this->ClearPet();
    m_setSynKickDoc.clear();
    m_idLastSynAlly	= ID_NONE;
    strcpy(m_szLastSynAllyName, "");
    m_bShowHpBar = true;
    m_pPackageNormal	=	IItemPackage::CreateNew();
    m_pPackagePet	=	IItemPackage::CreateNew();
    m_pPackageSoul	=	IItemPackage::CreateNew();
    m_pPackageEgg	=	IItemPackage::CreateNew();
    m_dwFlashTeamMemberID = ID_NONE;
    m_bQuerySchoolMember		=   false;
    m_idLastTeacherApply		=	ID_NONE;
    m_idLastStudentApply		=	ID_NONE;
    strcpy(m_szLastTeacherApply, "");
    strcpy(m_szLastStudentApply, "");
    m_dwSpeedRate = 100;
    m_bMagicLock = false;
    m_strCurrentMusic = "";
    m_strCurrentMusicPlay = "";
    m_dwLastChangeMusicTimer = 0;
    m_setSecSchoolMember.clear();
    strcpy(m_AuctionName, "");
    strcpy(m_AuctionOwnerName, "");
    m_AuctionItemId = ID_NONE;
    m_AuctionItemMoney = 0;
    m_nFastRunSetp = 0;
    m_dwTimeWhoAttackMe = 0;
    m_nMaxPetCall = 0;
	m_setMagic.clear();
}

CHero::~CHero()
{
    this->Reset();
}

//////////////////////////////////////////////////////////////////////
void CHero::Init(void)
{
    m_nType = MAP_HERO;
    m_dwMoney		= 0;
    m_timeDie		= 0;
    m_timeGhost		= 0;
    m_idAccount		= ID_NONE;
    m_posAlign.x	= -1;
    m_posAlign.y	= -1;
    m_nPkMode = PKMODE_FREE;
    m_setFriend.clear();
    m_nHeightCanJump = 200;
    m_idAttchPet = ID_NONE;
    this->ClearPet();
    m_setSynKickDoc.clear();
    m_bXpSkillEnable = FALSE;
    m_dwXpFullTime = 0;
    m_dwXpStartTime = 0;
    m_dwKoCount = 0;
    m_dwKoScale = 0;
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        m_pEquipment[i] = NULL;
    }
    m_setSingleHandWeaponSkill.clear();
    m_setDoubleHandWeaponSkill.clear();
    m_setOtherWeaponSkill.clear();
    m_bMagicAttackBusy = false;
    m_bFriendQueryBusy = false;
    m_idFriendQuery = ID_NONE;
    m_bFriendQueryReturn = false;
    strcpy(m_szSyndicateAnnounce, "");
    strcpy(m_szSyndicateAnnounceTime, "");
    this->SetRoleType(_ROLE_HERO);
    m_dwTimeSynWarReportLastFlash = ::TimeGet();
    memset(m_szWanted, 0L, sizeof(m_szWanted));
    memset(m_szPoliceWanted, 0L, sizeof(m_szPoliceWanted));
    this->SetData(CPlayer::_PLAYER_CURRENTLIFE, 0);
    m_idLastTeacherApply		=	ID_NONE;
    m_idLastStudentApply		=	ID_NONE;
    strcpy(m_szLastTeacherApply, "");
    strcpy(m_szLastStudentApply, "");
    m_dwSpeedRate = 100;
    strcpy(m_AuctionName, "");
    strcpy(m_AuctionOwnerName, "");
    m_AuctionItem = NULL;
    m_AuctionItemId = ID_NONE;
    m_AuctionItemMoney = 0;
    m_AuctionItem = new CItem;
    m_nMaxPetCall = 0;
}

//////////////////////////////////////////////////////////////////////
void CHero::Reset()
{
    this->ClearSchoolMemberList();
    this->ClearPlayerTaskList();
    this->ClearFriendList();
    this->ClearEnemyList();
    this->DestroyWeaponSkill();
    this->ClearItem();
    this->DestroyMagicSet();
    this->ClearSynWarReport();
    this->ClearHelpTip();
    this->ClearTips();
    g_objPlayerSet.DelAllPlayer();
    m_objTeam.Destroy();
    this->Init();

	m_PlayerInfo.Reset();
	m_PlayerInfo.dwMaxPhysicalForce = MaxPhysicalForce;
	m_PlayerInfo.setMedal.clear();
    m_PlayerInfo.setHonorTitle.clear(); //wh
    
	strcpy(m_szCryOut, "");
    this->ClearSynMember();
    m_bShowAbortTransformDialog = false;
    this->m_objEffect.Clear();
    this->m_objDiceManager.Close(false);
    ::PostCmd(CMD_RESET);
    m_objCheck.Reset();
    m_idWhoAttackMe	=	ID_NONE;
    m_idMyTarget	=	ID_NONE;
    m_idSpecialTarget	=	ID_NONE;
    m_idAttchPet = ID_NONE;
    this->ClearPet();
    m_objDummy.ClearEquipment();
    m_Info.usHair = ID_NONE;
    m_Info.usFace = ID_NONE;
    m_Info.idLWeaponType = ID_NONE;
    m_Info.idRWeaponType = ID_NONE;
    m_Info.idArmorType = ID_NONE;
    m_Info.idArmetType = ID_NONE;
    m_Info.idMountType = ID_NONE;
    m_setSynKickDoc.clear();
    m_idFriendLastApply =	ID_NONE;
    strcpy(m_szActionMessageBox, "");
    strcpy(m_szFriendLastApplyInfo, "");
    strcpy(m_szFriendLastAcceptInfo, "");
    m_pPackageNormal->Clear();
    m_pPackageEgg->Clear();
    m_pPackagePet->Clear();
    m_pPackageSoul->Clear();
    m_idLastTeacherApply		=	ID_NONE;
    m_idLastStudentApply		=	ID_NONE;
    strcpy(m_szLastTeacherApply, "");
    strcpy(m_szLastStudentApply, "");
    m_dwSpeedRate = 100;
    m_bMagicLock = false;
    strcpy(m_AuctionName, "");
    strcpy(m_AuctionOwnerName, "");
    m_AuctionItemId = ID_NONE;
    m_AuctionItemMoney = 0;
    SAFE_DELETE(m_AuctionItem);
    m_bXpSkillEnable = FALSE;
    m_dwXpFullTime = 0;
    m_dwXpStartTime = 0;
    m_dwKoCount = 0;
    m_dwKoScale = 0;
    m_strCurrentMusic = "";
    m_strCurrentMusicPlay = "";
    m_dwLastChangeMusicTimer = 0;
    ::DXSetSoundVolume(0);
    ::DXSetMusicVolume(0);
    g_objHero.SetMusicVolume(0);
    m_dwTimeWhoAttackMe = 0;
    m_nMaxPetCall = 0;
}

//////////////////////////////////////////////////////////////////////
void CHero::Walk(int nPosX, int nPosY)
{
    if (this->IsMagicLock())
    {
        return;
    }
    if (this->TestStatus(USERSTATUS_CANNOTMOVE))
    {
        return;
    }
    this->StopChargeUp();
    int nCommandType = this->GetCommandType();
    if (nCommandType == _COMMAND_ATTACK ||
            nCommandType == _COMMAND_ACTION ||
            nCommandType == _COMMAND_EMOTION ||
            nCommandType == _COMMAND_WOUND ||
            nCommandType == _COMMAND_DEFEND)
    {
        this->ResetActionData();
    }
    //	if (this->TestStatus(USERSTATUS_FLY))
    //	{
    //		this->Jump(nPosX, nPosY);
    //		return;
    //	}
    if (this->GetCommandType() == _COMMAND_ATTACK)
    {
        this->ResetActionData();
    }
    if (m_objBoothManager.IsActive())
    {
        return;
    }
    extern UINT	g_uStatus;
    if (_STATUS_NORMAL != g_uStatus)
    {
        return;
    }
    this->AbortIntone();
    if (this->IsDead() && !this->TestStatus(USERSTATUS_GHOST))
    {
        return;
    }
    // prevent repeat the same target point
    CCommand* pcmdNow	= this->GetCommand();
    if (_COMMAND_WALK == pcmdNow->iType)
    {
        if (nPosX ==  pcmdNow->posTarget.x &&
                nPosY ==  pcmdNow->posTarget .y)
        {
            return;
        }
    }
    // here got player already
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(nPosX, nPosY);
    if ((pPlayer && !pPlayer->IsDead()))// ||  (pPlayer && pPlayer->TestStatus(USERSTATUS_GHOST)))
    {
        return;
    }
    // set command of role
    CCommand cmd;
    cmd.iType			= _COMMAND_WALK;
    cmd.iStatus			= _CMDSTATUS_BEGIN;
    cmd.posTarget.x		= nPosX;
    cmd.posTarget.y		= nPosY;
    this->SetCommand(&cmd);
}

//////////////////////////////////////////////////////////////////////
void CHero::Run(int nPosX, int nPosY)
{
    if (this->IsMagicLock())
    {
        return;
    }
    if (this->TestStatus(USERSTATUS_CANNOTMOVE))
    {
        return;
    }
    this->StopChargeUp();
    int nCommandType = this->GetCommandType();
    if (nCommandType == _COMMAND_ATTACK ||
            nCommandType == _COMMAND_ACTION ||
            nCommandType == _COMMAND_EMOTION ||
            nCommandType == _COMMAND_WOUND ||
            nCommandType == _COMMAND_DEFEND)
    {
        this->ResetActionData();
    }
    //	if (this->TestStatus(USERSTATUS_FLY))
    //	{
    //		this->Jump(nPosX, nPosY);
    //		return;
    //	}
    if (this->GetCommandType() == _COMMAND_ATTACK)
    {
        this->ResetActionData();
    }
    if (m_objBoothManager.IsActive())
    {
        return;
    }
    extern UINT	g_uStatus;
    if (_STATUS_NORMAL != g_uStatus)
    {
        return;
    }
    this->AbortIntone();
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
    if (_COMMAND_RUN == pcmdNow->iType)
    {
        if (nPosX ==  pcmdNow->posTarget.x &&
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

//////////////////////////////////////////////////////////////////////
void CHero::Jump(int nPosX, int nPosY)
{
    g_objGameMsg.AddMsg("Jump is not allowed in BELIEF.");
}

//////////////////////////////////////////////////////////////////////
void CHero::Direction(int nPosX, int nPosY)
{
    if (m_objBoothManager.IsActive())
    {
        return;
    }
    if (this->IsDead() && !this->TestStatus(USERSTATUS_GHOST))
    {
        return;
    }
    CMyPos posTarget;
    posTarget.x	= nPosX;
    posTarget.y	= nPosY;
    int nDir	= this->GetDir(posTarget);
    this->SetDir(nDir);
    CMyPos posHero;
    this->GetPos(posHero);
    CMsgAction msg;
    if (msg.Create(this->GetID(), posHero.x, posHero.y, nDir, actionChgDir))
    {
        msg.Send();
    }
}

//////////////////////////////////////////////////////////////////////
void CHero::Reject(int nType)
{
    char szMsg[64] = "ERROR";
    char szName[64] = "ERROR";
    switch(nType)
    {
    case REJECT_FRIEND:
        strcpy(szMsg, "a");
        strcpy(szName, this->GetNameFriendLastApply());
        break;
    case REJECT_TRADE:
        strcpy(szMsg, "b");
        strcpy(szName, this->m_objTrade.GetLastApplyPlayerName());
        break;
    case REJECT_TEAM:
        strcpy(szMsg, "c");
        strcpy(szName, this->GetTeamApplyerName());
        break;
    case REJECT_TEACHER:
        strcpy(szMsg, "d");
        strcpy(szName, this->GetLastTeacherApply());
        break;
    case REJECT_STUDENT:
        strcpy(szMsg, "e");
        strcpy(szName, this->GetLastStudentApply ());
        break;
    default:
        return;
    }
    if (strlen(szName) >= _MAX_NAMESIZE)
    {
        return;
    }
    this->Talk(szName, szMsg, NULL, 0xffffffff, _TXTATR_REJECT);
}
//////////////////////////////////////////////////////////////////////
void CHero::Talk(const char* pszTarget, const char* pszWords, const char* pszEmotion,
                 DWORD dwColor, unsigned short unAttribute, unsigned short unStyle)
{
    if (!pszTarget || !pszWords)
    {
        return;
    }
    if (this->IsGhost() && unAttribute == _TXTATR_TALK)
    {
        unAttribute = _TXTATR_GHOST;
    }
    if (pszWords[0] == '/')
    {
        // cmd line
        const char* pszCmdLine = pszWords + 1;
        char szCmd[256] = "";
        if (1 == sscanf(pszCmdLine, "%s", szCmd))
        {
            if (0 == _stricmp(szCmd, "BaseScale"))
            {
                float fBaseScale;
                if (2 == sscanf(pszCmdLine, "%s %f", szCmd, &fBaseScale))
                {
                    CRole::s_fBaseScale = fBaseScale;
                    return;
                }
            }
            if (0 == _stricmp(szCmd, "info"))
            {
                g_objGameMap.SwitchScreenInfo();
                return;
            }
            if (0 == _stricmp(szCmd, "scrshot"))
            {
                char szFile[256] = "";
                if (2 == sscanf(pszCmdLine, "%s %s", szCmd, szFile))
                {
                    CMyBitmap::ScreenShot(szFile);
                    return;
                }
            }
            if (0 == _stricmp(szCmd, "lifePer"))
            {
                int percent = 0;
                if (2 == sscanf(pszCmdLine, "%s %d", szCmd, &percent))
                {
                    this->SetMaxLifePercent(percent);
                    return;
                }
            }
            if (0 == _stricmp(szCmd, "test"))
            {
                /*				g_objHero.LoadKongfu("test.kf");
                				CCommand cmd;
                				cmd.iType			=_COMMAND_KONGFU;
                				cmd.iStatus			=_CMDSTATUS_BEGIN;
                				g_objHero.SetCommand(&cmd);*/
                return;
            }
            if (0 == _stricmp(szCmd, "Action"))
            {
                DWORD dwAction;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &dwAction))
                {
                    this->SetIntoneCmd(dwAction);
                    return;
                }
            }
            if (0 == strcmp(szCmd, "开除"))
            {
                char szName[256] = "";
                if (2 == sscanf(pszCmdLine, "%s %s", szCmd, szName))
                {
                    CNetCmd::SynFireMember(szName);
                    return;
                }
            }
            if (0 == strcmp(szCmd, "armor"))
            {
                int nLevel;
                if (2 == sscanf(pszCmdLine, "%s %d", szCmd, &nLevel))
                {
                    g_objHero.SetArmor(nLevel);
                }
            }
            if (0 == _stricmp(szCmd, "addeffect"))
            {
                char szIndex[64];
                if (2 == sscanf(pszCmdLine, "%s %s", szCmd, szIndex))
                {
                    g_objHero.m_objEffect.Add(szIndex, true, this->GetDir());
                }
            }
            if (0 == _stricmp(szCmd, "setmantle"))
            {
                DWORD dwMantle;
                if (2 == sscanf(pszCmdLine, "%s %d", szCmd, &dwMantle))
                {
                    g_objHero.SetMantle(dwMantle);
                }
            }
            if (0 == _stricmp(szCmd, "setrweapon"))
            {
                DWORD dwRWeapon;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &dwRWeapon))
                {
                    g_objHero.SetRWeapon(dwRWeapon);
                }
            }
            if (0 == _stricmp(szCmd, "setmount"))
            {
                DWORD dwMount;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &dwMount))
                {
                    g_objHero.SetMount(dwMount);
                }
            }
#ifdef _DEBUG
            if (0 == _stricmp(szCmd, "Status"))
            {
                int nStatus;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &nStatus))
                {
                    switch(nStatus)
                    {
                    case 0:
                        this->ClrStatus(this->GetStatus());
                        break;
                    case 1:
                        this->SetStatus(USERSTATUS_ATTACK);
                        break;
                    case 2:
                        this->SetStatus(USERSTATUS_ADJUST_XP);
                        break;
                    default:
                        break;
                    }
                    return;
                }
            }
            if (0 == _stricmp(szCmd, "Emotion"))
            {
                int nEmotion;
                if (2 == sscanf(pszCmdLine, "%s %d", szCmd, &nEmotion))
                {
                    g_objHero.Emotion(nEmotion);
                }
            }
            if (0 == _stricmp(szCmd, "shoot"))
            {
                OBJID idTarget;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &idTarget))
                {
                    CMagicEffect* pEffect = CMagicEffect::CreateNew(g_objHero.GetID(), idTarget, this->GetRWeapon(), 0);
                    g_objGameMap.AddMagicEffect(pEffect);
                }
            }
            if (0 == _stricmp(szCmd, "Action"))
            {
                g_objObserver.SwitchAction();
            }
            //			if (0 == stricmp(szCmd, "wound"))
            //			{
            //				if (this->TestStatus(USERSTATUS_WOUND))
            //					this->ReplaceStatus(0);
            //				else
            //					this->ReplaceStatus(USERSTATUS_WOUND);
            //			}
            if (0 == _stricmp(szCmd, "item"))
            {
                g_objObserver.Switch();
            }
            if (0 == _stricmp(szCmd, "DropMoney"))
            {
                DWORD dwData;
                CMyPos posMouse, posCell;
                ::MouseCheck(posMouse.x, posMouse.y);
                g_objGameMap.Screen2Cell(posMouse.x, posMouse.y, posCell.x, posCell.y);
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &dwData))
                {
                    g_objHero.DropMoney(dwData, posMouse.x, posMouse.y);
                }
            }
            if (0 == _stricmp(szCmd, "mapitem"))
            {
                CMyPos posMouse, posCell;
                ::MouseCheck(posMouse.x, posMouse.y);
                g_objGameMap.Screen2Cell(posMouse.x, posMouse.y, posCell.x, posCell.y);
                DWORD dwData;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &dwData))
                {
                    g_objGameMap.Add3DMapItem(100 +::RandGet(99999), dwData, posCell);
                }
            }
            if (0 == _stricmp(szCmd, "Radar"))
            {
                int nMode;
                if (2 == sscanf(pszCmdLine, "%s %d", szCmd, &nMode))
                {
                    //	g_objGameMap.m_objRadar.SetMode(nMode);
                }
            }
            if (0 == _stricmp(szCmd, "setpos"))
            {
                CMyPos posHero, posHeroWorld;
                if (3 == sscanf(pszCmdLine, "%s %u %u", szCmd, &posHero.x, &posHero.y))
                {
                    g_objHero.SetPos(posHero.x, posHero.y);
                    g_objGameMap.Cell2World(posHero.x, posHero.y, posHeroWorld.x, posHeroWorld.y);
                    g_objHero.SetAlignPos(posHero.x, posHero.y);
                    CMyPos posWorld;
                    g_objHero.GetWorldPos(posWorld);
                    posWorld.x	-= _SCR_WIDTH / 2;
                    posWorld.y	-= _SCR_HEIGHT / 2;
                    g_objGameMap.SetViewPos(posWorld);
                    g_objGameMap.AddInteractiveObj(&g_objHero);
                }
            }
            if (0 == _stricmp(szCmd, "killpet"))
            {
                int nAmount = g_objPlayerSet.GetPlayerAmount();
                for(int i = 0; i < nAmount; i ++)
                {
                    CPlayer* pPlayer = g_objPlayerSet.GetPlayerByIndex(i);
                    if (pPlayer && pPlayer->IsMyPet())
                    {
                        CMyPos posPet;
                        pPlayer->GetPos(posPet);
                        CCommand cmd;
                        cmd.iType			= _COMMAND_DIE;
                        cmd.iStatus			= _CMDSTATUS_BEGIN;
                        cmd.posTarget.x		= posPet.x;
                        cmd.posTarget.y		= posPet.y;
                        pPlayer->SetCommand(&cmd);
                    }
                }
            }
            if (0 == _stricmp(szCmd, "kill"))
            {
                OBJID idTarget;
                static OBJID idPet = CALLPETID_FIRST;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &idTarget))
                {
                    this->SetSpecialTarget(idTarget);
                }
            }
            if (0 == _stricmp(szCmd, "callpet"))
            {
                int nLookType;
                static OBJID idPet = CALLPETID_FIRST;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &nLookType))
                {
                    idPet++;
                    CPet* pPet = CPet::CreateNewPet(idPet, 0, 0);
                    pPet->SetLook(nLookType);
                    pPet->SetRoleType(_ROLE_CALL_PET);
                    pPet->Create(nLookType, _ROLE_VIEW_ROLE);
                    CMyPos posMouse, posPet;
                    MouseCheck(posMouse.x, posMouse.y);
                    g_objGameMap.Screen2Cell(posMouse.x, posMouse.y, posPet.x, posPet.y);
                    g_objHero.AddPet(idPet);
                    pPet->SetPos(posPet);
                    g_objPlayerSet.AddPlayer(pPet);
                }
            }
            if (0 == _stricmp(szCmd, "die"))
            {
                CMyPos posHero;
                g_objHero.GetPos(posHero);
                CCommand cmd;
                cmd.iType			= _COMMAND_DIE;
                cmd.iStatus			= _CMDSTATUS_BEGIN;
                cmd.posTarget.x		= posHero.x;
                cmd.posTarget.y		= posHero.y;
                this->SetCommand(&cmd);
            }
            if (0 == _stricmp(szCmd, "diefly"))
            {
                CMyPos posHero;
                g_objHero.GetPos(posHero);
                CCommand cmd;
                cmd.iType			= _COMMAND_DIE;
                cmd.iStatus			= _CMDSTATUS_BEGIN;
                cmd.posTarget.x		= posHero.x;
                cmd.posTarget.y		= posHero.y;
                cmd.dwData = 2;
                cmd.nDir = (this->GetDir() + 8) % 8;
                this->SetCommand(&cmd);
            }
            if (0 == _stricmp(szCmd, "float"))
            {
                CMyPos posHero;
                g_objHero.GetPos(posHero);
                CCommand cmd;
                cmd.iType			= _COMMAND_FLOAT;
                cmd.iStatus			= _CMDSTATUS_BEGIN;
                cmd.posTarget.x		= posHero.x;
                cmd.posTarget.y		= posHero.y;
                this->SetCommand(&cmd);
            }
            //			if (0 == stricmp(szCmd, "takeoff"))
            //			{
            //				CMyPos posHero;
            //				g_objHero.GetPos(posHero);
            //				CCommand cmd;
            //				cmd.iType			=_COMMAND_TAKEOFF;
            //				cmd.iStatus			=_CMDSTATUS_BEGIN;
            //				cmd.posTarget.x		=posHero.x;
            //				cmd.posTarget.y		=posHero.y;
            //
            //				this->SetCommand(&cmd);
            //				this->ReplaceStatus(USERSTATUS_FLY);
            //			}
            //			if (0 == stricmp(szCmd, "landing"))
            //			{
            //				CMyPos posHero;
            //				g_objHero.GetPos(posHero);
            //				CCommand cmd;
            //				cmd.iType			=_COMMAND_LANDING;
            //				cmd.iStatus			=_CMDSTATUS_BEGIN;
            //				cmd.posTarget.x		=posHero.x;
            //				cmd.posTarget.y		=posHero.y;
            //
            //				this->SetCommand(&cmd);
            //				this->ReplaceStatus(USERSTATUS_NORMAL);
            //			}
            if (0 == _stricmp(szCmd, "scale"))
            {
                DWORD dwScale = 10000;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &dwScale))
                {
                    g_objGameMap.SetScale(dwScale);
                }
            }
            if (0 == _stricmp(szCmd, "setmantle"))
            {
                DWORD dwMantle;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &dwMantle))
                {
                    g_objHero.SetMantle(dwMantle);
                }
            }
            if (0 == _stricmp(szCmd, "setrweapon"))
            {
                DWORD dwRWeapon;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &dwRWeapon))
                {
                    g_objHero.SetRWeapon(dwRWeapon);
                }
            }
            if (0 == _stricmp(szCmd, "setlweapon"))
            {
                DWORD dwLWeapon;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &dwLWeapon))
                {
                    g_objHero.SetLWeapon(dwLWeapon);
                }
            }
            if (0 == _stricmp(szCmd, "setmount"))
            {
                DWORD dwMount;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &dwMount))
                {
                    g_objHero.SetMount(dwMount);
                }
            }
            if (0 == _stricmp(szCmd, "dir"))
            {
                int nDir;
                if (2 == sscanf(pszCmdLine, "%s %d", szCmd, &nDir))
                {
                    g_objHero.SetDir(nDir);
                }
            }
            if (0 == _stricmp(szCmd, "addplayer"))
            {
                CPlayer* pPlayer	= new CPlayer;
                MYASSERT (pPlayer);
                pPlayer->SetDir(this->GetDir());
                pPlayer->SetID(99999 +::RandGet(10000));
                pPlayer->SetName("LocalTest");
                pPlayer->Create(2, _ROLE_VIEW_ROLE);
                pPlayer->SetRoleType(_ROLE_MONSTER);
                CMyPos posMouse;
                ::MouseCheck(posMouse.x, posMouse.y);
                CMyPos posNpc;
                g_objGameMap.Screen2Cell(posMouse.x, posMouse.y, posNpc.x, posNpc.y);
                pPlayer->SetPos(posNpc);
                pPlayer->SetRoleType(_ROLE_MONSTER);
                g_objPlayerSet.AddPlayer(pPlayer);
            }
            if (0 == _stricmp(szCmd, "addnpc"))
            {
                int nLook = 5;
                //if (2 == sscanf(pszCmdLine, "%s %d", szCmd, &nLook))
                {
                    nLook = nLook * 10 + this->GetDir();
                    CPlayer* pPlayer	= new CPlayer;
                    MYASSERT (pPlayer);
                    pPlayer->SetDir(this->GetDir());
                    pPlayer->SetID(99999 +::RandGet(10000));
                    pPlayer->SetName("LocalTest");
                    pPlayer->Create(nLook, _ROLE_VIEW_TASKNPC);
                    pPlayer->SetRoleType(2);
                    CMyPos posMouse;
                    ::MouseCheck(posMouse.x, posMouse.y);
                    CMyPos posNpc;
                    g_objGameMap.Screen2Cell(posMouse.x, posMouse.y, posNpc.x, posNpc.y);
                    pPlayer->SetPos(posNpc);
                    pPlayer->SetRoleType(_ROLE_TARGET_NPC);
                    g_objPlayerSet.AddPlayer(pPlayer);
                }
            }
            if (0 == _stricmp(szCmd, "delnpc"))
            {
                OBJID idNpc;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &idNpc))
                {
                    g_objPlayerSet.DelPlayer(idNpc);
                }
            }
            if (0 == _stricmp(szCmd, "createbooth"))
            {
                g_objHero.m_objBoothManager.Open();
            }
            if (0 == _stricmp(szCmd, "destroybooth"))
            {
                g_objHero.m_objBoothManager.Close();
            }
            if (0 == _stricmp(szCmd, "addboothitem"))
            {
                OBJID id, price;
                if (3 == sscanf(pszCmdLine, "%s %u %u", szCmd, &id, &price))
                {
                    g_objHero.m_objBoothManager.AddItem(id, price);
                }
            }
            if (0 == _stricmp(szCmd, "delboothItem"))
            {
                OBJID id;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &id))
                {
                    g_objHero.m_objBoothManager.DelItem(id);
                }
            }
            if (0 == _stricmp(szCmd, "openbooth"))
            {
                OBJID id;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &id))
                {
                    g_objHero.m_objBooth.Open(id);
                }
            }
            if (0 == _stricmp(szCmd, "closebooth"))
            {
                OBJID id;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &id))
                {
                    g_objHero.m_objBooth.Close();
                }
            }
            if (0 == _stricmp(szCmd, "buyboothitem"))
            {
                OBJID id;
                if (2 == sscanf(pszCmdLine, "%s %u", szCmd, &id))
                {
                    g_objHero.m_objBooth.BuyItem(id);
                }
            }
            if (0 == _stricmp(szCmd, "SendBulletin"))
            {
                int nChannel;
                char szMsg[512];
                if (3 == sscanf(pszCmdLine, "%s %s %d", szCmd, szMsg, &nChannel))
                {
                    g_objHero.m_objBulletin.SendBulletin(szMsg, nChannel);
                }
            }
            if (0 == _stricmp(szCmd, "Emotion"))
            {
                int nEmotion;
                if (2 == sscanf(pszCmdLine, "%s %d", szCmd, &nEmotion))
                {
                    g_objHero.Emotion(nEmotion);
                }
            }
            if (0 == _stricmp(szCmd, "DelBulletin"))
            {
                int nChannel;
                if (2 == sscanf(pszCmdLine, "%s %d", szCmd, &nChannel))
                {
                    g_objHero.m_objBulletin.DelBulletin(nChannel);
                }
            }
            if (0 == _stricmp(szCmd, "QueryList"))
            {
                int nChannel, nIndex;
                if (3 == sscanf(pszCmdLine, "%s %d %d", szCmd, &nIndex, &nChannel))
                {
                    g_objHero.m_objBulletin.QueryList(nIndex, nChannel);
                }
            }
            if (0 == _stricmp(szCmd, "QueryNote"))
            {
                int nChannel;
                char szSenderName[64];
                if (3 == sscanf(pszCmdLine, "%s %s %d", szCmd, szSenderName, &nChannel))
                {
                    g_objHero.m_objBulletin.QueryNote(szSenderName, nChannel);
                }
            }
            if (0 == _stricmp(szCmd, "setlife"))
            {
                OBJID idNpc;
                int nLife;
                if (3 == sscanf(pszCmdLine, "%s %u %d", szCmd, &idNpc, &nLife))
                {
                    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idNpc);
                    if (pPlayer)
                    {
                        pPlayer->SetLife(nLife);
                    }
                }
            }
            if (0 == _stricmp(szCmd, "setpk"))
            {
                int nData;
                if (2 == sscanf(pszCmdLine, "%s %d", szCmd, &nData))
                {
                    g_objHero.SetPk(nData);
                }
            }
            if (0 == _stricmp(szCmd, "addfriend"))
            {
                if (1 == sscanf(pszCmdLine, "%s", szCmd))
                {
                    for(int i = 0; i < 40; i ++)
                    {
                        char szName[64];
                        sprintf(szName, "f%d", i);
                        CFriend* pFriend = CFriend::CreateNew(9999999 + i,
                                                              szName,
                                                              2,
                                                              ::RandGet(2));
                        if (pFriend)
                        {
                            g_objHero.AddFriend(pFriend);
                        }
                    }
                }
            }
            if (0 == _stricmp(szCmd, "test1"))
            {
                g_objHero.AddMoneyEffect(100);
                //this->ReplaceStatus(USERSTATUS_NORMAL);
            }
            if (0 == _stricmp(szCmd, "FixNpc"))
            {
                g_objHero.FixMousePlayer();
            }
            if (0 == _stricmp(szCmd, "screenshake"))
            {
                g_objGameMap.BeginShakeEffect();
            }
            if (0 == _stricmp(szCmd, "screenscale"))
            {
                g_objGameMap.BeginScaleEffect();
            }
            if (0 == _stricmp(szCmd, "screencolor"))
            {
                g_objGameMap.BeginColorEffect();
            }
            if (0 == _stricmp(szCmd, "screeneffect"))
            {
                g_objGameMap.BeginShakeEffect();
                g_objGameMap.BeginScaleEffect();
                g_objGameMap.BeginColorEffect();
            }
            if (0 == _stricmp(szCmd, "color"))
            {
                DWORD dwColor;
                if (2 == sscanf(pszCmdLine, "%s %x", szCmd, &dwColor))
                {
                    g_objGameMap.SetARGB(dwColor);
                }
            }
#endif
#ifdef _ANALYSIS_ON
            if (0 == _stricmp(szCmd, "analysis"))
            {
                g_bShowAnalysisInfo = !g_bShowAnalysisInfo;
            }
#endif
        }
    }
    if (_TXTATR_ACTION == unAttribute)
    {
        CONST DWORD INTERVAL_TALK = 5000;
        DWORD dwTimeNow = ::TimeGet();
        if (dwTimeNow - m_dwLastTalk > INTERVAL_TALK)
        {
            m_dwLastTalk = dwTimeNow;
            CMsgTalk msg;
            if (msg.Create((char*)this->GetName(), (char*)pszTarget, (char*)pszWords, (char*)pszEmotion, dwColor, unAttribute, unStyle))
            {
                msg.Send();
            }
        }
    }
    else
    {
        if (!(this->IsGhost() && unAttribute != _TXTATR_TEAM && unAttribute != _TXTATR_GHOST))
        {
            CMsgTalk msg;
            if (msg.Create((char*)this->GetName(), (char*)pszTarget, (char*)pszWords, (char*)pszEmotion, dwColor, unAttribute, unStyle))
            {
                msg.Send();
            }
        }
    }
    g_objGameMsg.AddMsg((char*)this->GetName(), (char*)pszTarget, (char*)pszEmotion, (char*)pszWords, dwColor, unStyle, unAttribute, ::SysTimeGet());
    if (unAttribute != _TXTATR_ACTION &&
            unAttribute != _TXTATR_PRIVATE &&
            unAttribute != _TXTATR_REJECT)
    {
        this->Say(pszWords);
    }
}

//////////////////////////////////////////////////////////////////////
void CHero::Talk(CPlayer* pTarget, const char* pszWords, const char* pszEmotion,
                 DWORD dwColor, unsigned short unAttribute, unsigned short unStyle)
{
    if (!pszWords)
    {
        return;
    }
    char szSender[256];
    strcpy (szSender, this->GetName());
    char szTarget[256];
    const OBJID idEveryOne = 100026;
    if (!pTarget)
    {
        strcpy(szTarget, g_objGameDataSet.GetStr(idEveryOne));
    }
    else
    {
        strcpy(szTarget, pTarget->GetName());
    }
    this->Talk(szTarget, pszWords, pszEmotion, dwColor, unAttribute, unStyle);
}


//////////////////////////////////////////////////////////////////////
BOOL CHero::CanPk(CPlayer* pTarget)
{
    if (!pTarget)
    {
        return false;
    }
    if (pTarget->IsMyPet ())
    {
        return false;
    }
    switch(m_nPkMode)
    {
    case PKMODE_SYNDICATE:
        {
            if (!pTarget->IsNpc())
            {
                if (this->GetDistance(pTarget) == 1)
                {
                    if (this->GetSyndicate() == ID_NONE)
                    {
                        return true;
                    }
                    else if (this->GetSyndicate() == pTarget->GetSyndicate())
                    {
                        const OBJID idCanNotPk = 100027;
                        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idCanNotPk));
                        return false;
                    }
                }
            }
        }
        break;
    case PKMODE_ARRESTMENT:
        {
            if (!pTarget->IsNpc() && !pTarget->IsBaddy())
            {
                if (this->GetDistance(pTarget) == 1)
                {
                    const OBJID idCanNotPk = 100164;
                    g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idCanNotPk));
                }
                return false;
            }
            if (pTarget->IsPlayer () && pTarget->TestStatus (USERSTATUS_RED))
            {
                if (this->GetDistance(pTarget) == 1)
                {
                    const OBJID idCanNotPk = 100164;
                    g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idCanNotPk));
                }
                return false;
            }
            if (pTarget->IsPet() && !pTarget->IsBaddy())
            {
                if (this->GetDistance(pTarget) == 1)
                {
                    const OBJID idCanNotPk = 100164;
                    g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idCanNotPk));
                }
                return false;
            }
        }
        break;
    case PKMODE_MONSTERONLY:
        {
            if (!pTarget->IsNpc())
            {
                if (this->GetDistance(pTarget) == 1)
                {
                    const OBJID idCanNotPk = 100027;
                    g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idCanNotPk));
                }
                return false;
            }
        }
        break;
    case PKMODE_TMEXCLUDE:
        {
            // team members
            if (this->IsTeamMember(pTarget->GetID()))
            {
                if (this->GetDistance(pTarget) == 1)
                {
                    const OBJID idCanNotPkTeamMember = 100165;
                    g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idCanNotPkTeamMember));
                }
                return false;
            }
            // mate
            if (0 == strcmp(this->GetMate(), pTarget->GetName()))
            {
                return false;
            }
        }
        break;
    default:
        break;
    }
    return true;
}


//////////////////////////////////////////////////////////////////////
void CHero::Shoot(CPlayer* pTarget, OBJID idBullet, BOOL bForced)
{
    int nAction = _ACTION_ATTACK0 +::RandGet(3);
    int nDir	= this->GetDir(pTarget);
    this->SetActionCmd(nAction, nDir);
    if (this->GetLWeapon() == ID_NONE)
    {
        return;
    }
    // create arrow
    if (pTarget)
    {
        CMagicEffect* pEffect = CMagicEffect::CreateNewArrow(this->GetID(), pTarget->GetID(), this->GetLWeapon());
        g_objGameMap.AddMagicEffect(pEffect);
        pEffect->AddRoleInfo(pTarget->GetID(), 50);
    }
}

//////////////////////////////////////////////////////////////////////
void CHero::Attack (CPlayer* pTarget, BOOL bForced/*=false*/)
{
    if (!pTarget)
    {
        return;
    }
    if (this->IsDead() || pTarget->IsDead())
    {
        return;
    }
    if (!this->CanPk(pTarget))
    {
        return;
    }
    // set command of role
    CCommand cmd;
    cmd.iType		= _COMMAND_ATTACK;
    cmd.iStatus		= _CMDSTATUS_BEGIN;
    cmd.idTarget	= pTarget->GetID();
    CMyPos posTarget;
    pTarget->GetPos(posTarget);
    cmd.nDir	= this->GetDir(posTarget);
    this->SetCommand(&cmd);
}
//////////////////////////////////////////////////////////////////////
void CHero::TeamFollow()
{
    this->AbortIntone();
    TeamMemberInfo* pInfo = this->GetTeamMemberInfo(0);
    if (!pInfo)
    {
        return;
    }
    if (pInfo->id == this->GetID())
    {
        return;
    }
    CPlayer* pTarget = g_objPlayerSet.GetPlayer(pInfo->id);
    if (!pTarget)
    {
        return;
    }
    // set command of role
    CCommand cmd;
    cmd.iType		= _COMMAND_TEAMFOLLOW;
    cmd.iStatus		= _CMDSTATUS_BEGIN;
    cmd.idTarget	= pTarget->GetID();
    // get my index in the team ...
    int nAmount = this->GetTeamMemberAmount();
    for(int i = 0; i < nAmount; i ++)
    {
        TeamMemberInfo* pInfo = this->GetTeamMemberInfo(i);
        if (pInfo && pInfo->id == this->GetID())
        {
            cmd.dwData = i;
            break;
        }
    }
    // offset
    CMyPos posTarget;
    pTarget->GetPos(posTarget);
    cmd.posTarget.x = m_Info.posMap.x - posTarget.x;
    cmd.posTarget.y = m_Info.posMap.y - posTarget.y;
    cmd.nDir	= this->GetDir(posTarget);
    this->SetCommand(&cmd);
}
//////////////////////////////////////////////////////////////////////
void CHero::LockAttack (CPlayer* pTarget, BOOL bForced, BOOL bMove)
{
    if (this->TestStatus(USERSTATUS_CANNOTMOVE))
    {
        return;
    }
    if (this->IsMagicLock())
    {
        return;
    }
    if (!pTarget)
    {
        return;
    }
    if (pTarget->IsPlayer())   //禁止pk区域检测
    {
        CMyPos posHero;
        this->GetPos(posHero);
        if (!g_objGameMap.IsPkRegion(posHero))
        {
            const OBJID idStr = 100157;
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100157));
            return;
        }
    }
    if (g_objGameMap.GetType() & MAPTYPE_BOOTH_ENABLE)
    {
        return;
    }
    this->AbortIntone();
    if (this->IsDead() || pTarget->IsDead())
    {
        return;
    }
    if (!this->CanPk(pTarget))
    {
        CMyPos posTarget;
        pTarget->GetPos(posTarget);
        int nDir = this->GetDir(posTarget);
        //		if (!this->TestStatus(USERSTATUS_FLY))
        //		{
        posTarget.x -= _DELTA_X[nDir];
        posTarget.y -= _DELTA_Y[nDir];
        this->Walk(posTarget.x, posTarget.y);
        // 		}
        //		else
        //		{
        //			posTarget.x -= _DELTA_X[nDir];
        //			posTarget.y -= _DELTA_Y[nDir];
        //			this->Jump(posTarget.x, posTarget.y);
        //		}
    }
    else
    {
        const OBJID idGuard = 100028;
        if (pTarget->IsGuard())
        {
            if (!(bForced && PKMODE_FREE == this->GetPkMode()))
            {
                const OBJID idCanNotPkGuard = 100029;
                g_objGameMsg.AddMsg(_TXTATR_SYSTEM, g_objGameDataSet.GetStr(idCanNotPkGuard));
                return;
            }
        }
        if (pTarget->IsPlayer() &&
                this->GetSydicateID() != ID_NONE &&
                pTarget->GetSydicateID() == this->GetSydicateID() &&
                !bForced)
        {
            OBJID idStr = 100126;
            g_objGameMsg.AddMsg(_TXTATR_SYSTEM, g_objGameDataSet.GetStr(idStr));
            //g_objGameMsg.AddMsg(_TXTATR_SYSTEM, "攻击帮派兄弟必须使用按住Ctrl键的强制PK模式。");
            return;
        }
        this->SetIDMyTarget(pTarget->GetID());
        if (this->HaveBow())
        {
            if (this->GetLWeapon() == ID_NONE)
            {
                const OBJID idStr = 100140;
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));
                return;
            }
            /*
            	if (this->TestStatus(USERSTATUS_CHARGEUP) || !bMove)
            	{
            		CMyPos posTarget;
            		pTarget->GetPos(posTarget);
            		int nAtkRange = (this->GetAddSize()+pTarget->GetAddSize()+1)/2+this->GetAtkRange();
            		if (abs(m_Info.posMap.x - posTarget.x) > nAtkRange ||
            			abs(m_Info.posMap.y - posTarget.y) > nAtkRange)
            		{
            			const OBJID idStr = 100141;
            			g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));
            			return;
            		}
            	}
            */
        }
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
}
//////////////////////////////////////////////////////////////////////
void  CHero::LockBooth(CPlayer* pTarget)
{
    if (this->IsMagicLock())
    {
        return;
    }
    if (!pTarget)
    {
        return;
    }
    CCommand cmd;
    cmd.iType		= _COMMAND_LOCKBOOTH;
    cmd.iStatus		= _CMDSTATUS_BEGIN;
    cmd.idTarget	= pTarget->GetID();
    cmd.nDir		= pTarget->GetDir();
    this->SetCommand(&cmd);
}
//////////////////////////////////////////////////////////////////////
BOOL CHero::CanBooth(CPlayer* pTarget)
{
    if (this->m_objBoothManager.IsActive())
    {
        return false;
    }
    if (!pTarget)
    {
        return false;
    }
    CMyPos posTarget;
    pTarget->GetPos(posTarget);
    int nDir = pTarget->m_pIRoleView->GetVariable(_VAR_DIR);
    CMyPos posBooth;
    posBooth.x = posTarget.x + _DELTA_X[nDir] * 2;
    posBooth.y = posTarget.y + _DELTA_Y[nDir] * 2;
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(posBooth.x, posBooth.y);
    CMyPos posHero;
    this->GetPos(posHero);
    if (posHero.x == posBooth.x && posHero.y == posBooth.y)
    {
        return false;
    }
    if (pPlayer)
    {
        return false;
    }
    return true;
}
//////////////////////////////////////////////////////////////////////
void CHero::RushAttack (CPlayer* pTarget, BOOL bForced/*=false*/)
{
    if (this->IsMagicLock())
    {
        return;
    }
    if (!pTarget)
    {
        return;
    }
    this->AbortIntone();
    if (this->IsDead() || pTarget->IsDead())
    {
        return;
    }
    if (!this->CanPk(pTarget))
    {
        return;
    }
    // find pos to stand around the target
    CMyPos posTarget;
    pTarget->GetPos(posTarget);
    CMyPos pos;
    int nDir = pTarget->GetDir(this);
    for (int i = 0; i < 8; i++)
    {
        pos.x = posTarget.x + _DELTA_X[nDir];
        pos.y = posTarget.y + _DELTA_Y[nDir];
        if (this->CanJump(pos))
        {
            break;
        }
        nDir++;
    }
    // set command of role
    CCommand cmd;
    cmd.iType		= _COMMAND_RUSHATK;
    cmd.iStatus		= _CMDSTATUS_BEGIN;
    cmd.idTarget	= pTarget->GetID();
    cmd.nDir		= this->GetDir(pos);
    cmd.posTarget.x	= pos.x;
    cmd.posTarget.y	= pos.y;
    this->SetCommand(&cmd);
}

//////////////////////////////////////////////////////////////////////
void CHero::SkillAttack (CPlayer* pTarget, OBJID idSkillType, BOOL bForced/*=false*/)
{
    if (!pTarget || idSkillType == ID_NONE)
    {
        return;
    }
    this->AbortIntone();
    if (this->IsDead() || pTarget->IsDead())
    {
        return;
    }
    if (!this->CanPk(pTarget))
    {
        return;
    }
    // set command of role
    /*
    CCommand cmd;
    cmd.iType		=_COMMAND_SPATTACK;
    cmd.iStatus		=_CMDSTATUS_BEGIN;
    cmd.idTarget	=pTarget->GetID();
    cmd.dwData		=uPowerUsed;
    strcpy(cmd.szString, pszSkill);

    CMyPos posTarget;
    pTarget->GetPos(posTarget);

    cmd.nDir	=this->GetDir(posTarget);

    this->SetCommand(&cmd);
    */
}

//////////////////////////////////////////////////////////////////////
void CHero::Emotion	(int nEmotionType, CPlayer* pTarget)
{
    if (this->IsDead())// || this->TestStatus(USERSTATUS_FLY))
    {
        return;
    }
    this->AbortIntone();
    if (nEmotionType == _ACTION_DANCE1 &&
            (m_pEquipment[ITEMPOSITION_WEAPONR-1] || m_pEquipment[ITEMPOSITION_WEAPONL-1]))
    {
        const OBJID idCanNotDanceWithWeapon = 100030;
        g_objGameMsg.AddMsg(_TXTATR_SYSTEM, g_objGameDataSet.GetStr(idCanNotDanceWithWeapon));
        return;
    }
    // set command of role
    CCommand cmd;
    cmd.iType		= _COMMAND_EMOTION;
    cmd.iStatus		= _CMDSTATUS_BEGIN;
    cmd.nData		= nEmotionType;
    if (pTarget)
    {
        CMyPos posTarget;
        pTarget->GetPos(posTarget);
        cmd.nDir	= this->GetDir(posTarget);
    }
    else
    {
        cmd.nDir	= this->GetDir();
    }
    this->SetCommand(&cmd);
    if (_ACTION_COOLPOSE == nEmotionType)
    {
        if (this->IsAllEquipmentNonsuch())
        {
            this->AddProfessionalCoolposEffect(this->GetProfession(), true);
        }
        else
        {
            if ((this->GetArmor() % 10) == 9)
            {
                this->AddProfessionalCoolposEffect(this->GetProfession(), false);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CHero::Stop(void)
{
    CCommand cmd;
    cmd.iType		= _COMMAND_STOP;
    cmd.iStatus		= _CMDSTATUS_BEGIN;
    this->SetCommand(&cmd);
}

//////////////////////////////////////////////////////////////////////
void CHero::Poison(CPlayer* pTarget, OBJID idItem)
{
    /*	if (!pTarget || idItem == ID_NONE)
    		return;

    	if (this->IsDead() || pTarget->IsDead())
    		return;

    	if (!this->GetItem(idItem))
    	{
    		const OBJID idItemNotFound = 100031;
    		g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idItemNotFound));
    		return;
    	}

    	// set command of role
    	CCommand cmd;
    	cmd.iType		=_COMMAND_ACTION;
    	cmd.iStatus		=_CMDSTATUS_BEGIN;
    	cmd.nData		=_ACTION_CAST;
    	cmd.nDir		=this->GetDir(pTarget);

    	this->SetCommand(&cmd);

    	// send msg
    	CMyPos posHero;
    	this->GetPos(posHero);

    	CMsgInteract msg;
    	if (msg.Create(INTERACT_POISON, this->GetID(), pTarget->GetID(), posHero.x, posHero.y, idItem))
    		msg.Send();*/
}

//////////////////////////////////////////////////////////////////////
void CHero::Dart(CPlayer* pTarget, OBJID idItem)
{
    /*	if (!pTarget || idItem == ID_NONE)
    		return;

    	if (this->IsDead() || pTarget->IsDead())
    		return;

    	if (!this->GetItem(idItem))
    	{
    		const OBJID idItemNotFound = 100031;
    		g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idItemNotFound));
    		return;
    	}

    	// set command of role
    	CCommand cmd;
    	cmd.iType		=_COMMAND_ACTION;
    	cmd.iStatus		=_CMDSTATUS_BEGIN;
    	cmd.nData		=_ACTION_CAST;
    	cmd.nDir		=this->GetDir(pTarget);

    	this->SetCommand(&cmd);

    	// send msg
    	CMyPos posHero;
    	this->GetPos(posHero);

    	CMsgInteract msg;
    	if (msg.Create(INTERACT_ASSASSINATE, this->GetID(), pTarget->GetID(), posHero.x, posHero.y, idItem))
    		msg.Send();*/
}

//////////////////////////////////////////////////////////////////////

void CHero::Steal(CPlayer* pTarget)
{
    if (!pTarget)
    {
        return;
    }
    if (this->IsDead() || pTarget->IsDead())
    {
        return;
    }
    // send msg
    CMyPos posHero;
    this->GetPos(posHero);
    CMsgInteract msg;
    if (msg.Create(INTERACT_STEAL, this->GetID(), pTarget->GetID(), posHero.x, posHero.y))
    {
        msg.Send();
    }
}

//////////////////////////////////////////////////////////////////////
int nAcxcount = 0;

DWORD CHero::SetCommand(CCommand* pCommand)
{
    if (!pCommand)
    {
        return false;
    }
    m_dwCommandIndex++;
    pCommand->dwIndex = m_dwCommandIndex;
	
    // lock attack 命令不可积累
    if (pCommand->iType == _COMMAND_LOCKATK || 
		pCommand->iType == _COMMAND_WALK || 
		pCommand->iType == _COMMAND_RUN)
    {
        for(int i = 0; i < (int)m_PlayerInfo.setCmd.size(); i ++)
        {
            if (m_PlayerInfo.setCmd[i].iType ==  _COMMAND_LOCKATK ||
                pCommand->iType == _COMMAND_WALK || 
				pCommand->iType == _COMMAND_RUN)
            {
                m_PlayerInfo.setCmd.erase(m_PlayerInfo.setCmd.begin() + i);
                break;
            }
        }
    }
    // 受伤动作不累积
    // dodge动作不累积
    if (pCommand->iType == _COMMAND_ACTION)
	{
        if (pCommand->nData == _ACTION_DODGE0 || pCommand->nData == _ACTION_BRUISE0)
        {
            if (m_PlayerInfo.setCmd.size() != 0)
            {
                return false;
            }
        }
	}
	
    if (pCommand->iType == _COMMAND_INTONE && !pCommand->bAddUp)
    {
        if (m_PlayerInfo.setCmd.size() != 0)
        {
			//交互动作- 比如释放技能 xp技能-播放一下动画，所以不return。重置动作 2016.10.24
			if(m_PlayerInfo.setCmd.size() == 1 && 
				m_PlayerInfo.setCmd[0].iType == _COMMAND_STOP)
			{
				this->ResetActionData();
				m_PlayerInfo.setCmd.clear();
			}else
			{
				return false;
			}
        }
    }
    m_PlayerInfo.setCmd.push_back(*pCommand);
    if (this->IsMoving() && _COMMAND_ATTACK == pCommand->iType)
    {
        return m_dwCommandIndex;
    }
    if (pCommand->iType == _COMMAND_INTONE || pCommand->iType == _COMMAND_EMOTION || pCommand->iType == _COMMAND_ACTION)
    {
        if (((pCommand->nData == _ACTION_BRUISE0) || (pCommand->nData == _ACTION_DODGE0)) && (this->m_Info.cmdProc.iType == _COMMAND_EMOTION) &&
                (m_Info.cmdProc.nData == _ACTION_INTONE || m_Info.cmdProc.nData == _ACTION_INTONE_DURATION))
        {
            return m_dwCommandIndex;
        }
    }
    if (_COMMAND_STANDBY != pCommand->iType)
    {
        // attack can be interrupted by Jump, Walk, Run, at any time.
        if (_COMMAND_ATTACK == m_Info.cmdProc.iType ||
            _COMMAND_SHOOT == m_Info.cmdProc.iType ||
            _COMMAND_WOUND == m_Info.cmdProc.iType ||
            _COMMAND_DEFEND == m_Info.cmdProc.iType ||
            _COMMAND_ACTION == m_Info.cmdProc.iType)
        {
            if (pCommand->iType == _COMMAND_WALK ||
                    pCommand->iType == _COMMAND_RUN ||
                    pCommand->iType == _COMMAND_JUMP)
            {
                this->ResetActionData();
            }
        }
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
            if (_ACTION_STANDBY == m_Info.iActType || 
				_ACTION_ALERT == m_Info.iActType || 
				_ACTION_REST1 == m_Info.iActType || 
				_ACTION_REST2 == m_Info.iActType || 
				_ACTION_REST3 == m_Info.iActType)
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
    if (pCommand->iType == _COMMAND_EMOTION)
    {
        if (pCommand->nData == _ACTION_INTONE)
        {
            nAcxcount ++;
        }
    }
    memcpy(&(m_Info.cmdProc), pCommand, sizeof(CCommand));
    return m_dwCommandIndex;
}

//////////////////////////////////////////////////////////////////////
BOOL CHero::ProcessCommand(void)
{
    int iPosX, iPosY;
    this->GetPos(iPosX, iPosY);
    //--------------------------------------------------------
    if (_COMMAND_JUMP == m_Info.cmdProc.iType)
    {
        if (_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)		// is a new jump cmd
        {
/*			CONST MAX_JUMPDISTANCE = 16;
            if (MAX_JUMPDISTANCE < g_objGameMap.GetDistance(iPosX, iPosY, m_Info.cmdProc.posTarget.x, m_Info.cmdProc.posTarget.y))
            {
            	this->SetActionCmd(_ACTION_STANDBY);
            	const OBJID idCanNotJumpSoFar = 100032;
            	g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idCanNotJumpSoFar));
            	return false;
            }
*/
            CMsgAction msg;
            if (msg.Create(this->GetID(), iPosX, iPosY, m_Info.cmdProc.nDir, actionJump, m_Info.cmdProc.posTarget.x, m_Info.cmdProc.posTarget.y, m_Info.cmdProc.dwData))
            {
                msg.Send();
            }
        }
    }
    //--------------------------------------------------------
    if (_COMMAND_EMOTION == m_Info.cmdProc.iType)
    {
        if (_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus && m_Info.cmdProc.bHeroSendMsg)		// is a new emotion cmd
        {
            CMsgAction	msg;
            if (msg.Create(this->GetID(), iPosX, iPosY, m_Info.cmdProc.nDir, actionEmotion, m_Info.cmdProc.nData))
            {
                msg.Send();
            }
        }
    }
    //--------------------------------------------------------
    if (_COMMAND_ATTACK == m_Info.cmdProc.iType)
    {
        if (_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)		// is a new atk cmd
        {
            CMsgInteract msg;
            if (msg.Create(INTERACT_ATTACK, this->GetID(), m_Info.cmdProc.idTarget, iPosX, iPosY))
            {
                msg.Send();
            }
        }
    }
    //--------------------------------------------------------
    if (_COMMAND_SHOOT == m_Info.cmdProc.iType)
    {
        if (_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)		// is a new atk cmd
        {
            // 转移到普通攻击
            m_Info.cmdProc.iType = _COMMAND_ATTACK;
            CMsgInteract msg;
            if (msg.Create(INTERACT_SHOOT, this->GetID(), m_Info.cmdProc.idTarget, m_Info.cmdProc.dwData))
            {
                msg.Send();
                CMagicEffect* pEffect = CMagicEffect::CreateNewArrow(this->GetID(), m_Info.cmdProc.idTarget, m_Info.cmdProc.nData);
                g_objGameMap.AddMagicEffect(pEffect);
            }
        }
    }
    //--------------------------------------------------------
    if (_COMMAND_RUSHATK == m_Info.cmdProc.iType)
    {
        if (_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
        {
            CMsgInteract msg;
            if (msg.Create(INTERACT_RUSHATK, this->GetID(), m_Info.cmdProc.idTarget, iPosX, iPosY, m_Info.cmdProc.posTarget.x, m_Info.cmdProc.posTarget.y, m_Info.cmdProc.idTarget))
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
    case _ACTION_PICKUP:
        {
            CMsgMapItem msg;
            if (msg.Create(m_Info.cmdProc.idTarget, iPosX, iPosY))
            {
                msg.Send();
            }
        }
        break;
    case _ACTION_BUMP:
        {
            CONST int MAGIC_BUMP = 1051;
            CMagic* pMagic = this->GetMagic(MAGIC_BUMP);
            if (pMagic)
            {
                int nDir = this->GetDir();
                CMsgInteract msg;
                OBJID idUser = this->GetID();
                unsigned short usMagicType = MAGIC_BUMP;
                OBJID idTarget = 0;
                int nPosX = iPosX;
                int nPosY = iPosY;
                ENCODE_MAGICATTACK((unsigned short)idUser, usMagicType, nDir, nPosX, nPosY)
                IF_SUC (msg.Create( INTERACT_MAGICATTACK,
                                    idUser,
                                    nDir,
                                    nPosX,
                                    nPosY,
                                    usMagicType,
                                    (USHORT)pMagic->m_infoMagicType.dwLevel,
                                    nDir))
                msg.Send();
            }
        }
        break;
    case _ACTION_WALKL:
    case _ACTION_WALKR:
        {
            if (m_Info.posActionBegin.x != m_Info.posActionEnd.x ||
                    m_Info.posActionBegin.y != m_Info.posActionEnd.y)
            {
                CMyPos posHero;
                this->GetPos(posHero);
                CMsgWalk msg;
                if (msg.Create(this->GetID(), this->GetDir() + 8 *::RandGet(31)))
                {
                    msg.Send();
                }
            }
        }
        break;
    case _ACTION_RUNL:
    case _ACTION_RUNR:
        {
            CMyPos posHero;
            this->GetPos(posHero);
            CMsgWalk msg;
            if (msg.Create(this->GetID(), this->GetDir() + 8 *::RandGet(31), m_Info.nNextStep))
            {
                msg.Send();
            }
        }
        break;
    }
    if (this->GetLastCommand() == _COMMAND_LOCKBOOTH &&
            this->GetCommandStatus() == _CMDSTATUS_ACCOMPLISH)
    {
        CMyPos posHero;
        this->GetPos(posHero);
        this->m_objBoothManager.Open();
        CMsgAction msg;
        if (msg.Create(this->GetID(), posHero.x, posHero.y, this->GetDir(), actionChgDir))
        {
            msg.Send();
        }
    }
    if (this->GetLastCommand() == _COMMAND_LOCKATK &&
            this->GetCommandStatus() == _CMDSTATUS_ACCOMPLISH)
    {
        if (!this->HaveBow())
        {
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_Info.cmdProc.idTarget);
            if (pPlayer && (pPlayer->GetRoleType() == _ROLE_STONE_MINE ))
                //							||pPlayer->GetRoleType() == _ROLE_WOOD_NPC))
            {
                CMsgInteract msg;
                if (msg.Create(INTERACT_MINE, this->GetID(), m_Info.cmdProc.idTarget, iPosX, iPosY))
                {
                    msg.Send();
                }
            }
            else
            {
                CMsgInteract msg;
                if (msg.Create(INTERACT_ATTACK, this->GetID(), m_Info.cmdProc.idTarget, iPosX, iPosY))
                {
                    msg.Send();
                }
            }
        }
        else
        {
            CMsgInteract msg;
            if (msg.Create(INTERACT_SHOOT, this->GetID(), m_Info.cmdProc.idTarget, m_Info.cmdProc.dwData))
            {
                msg.Send();
            }
        }
    }
    //--------------------------------------------------------
    return bAcomplished;
}

//////////////////////////////////////////////////////////////////////
void CHero::SetAlignPos(int nPosX, int nPosY)
{
    CMyPos posHeroAlign	= g_objHero.GetAlignPos();
    g_objPlayerSet.ProcessManager(posHeroAlign.x, posHeroAlign.y);
    g_objGameMap.ProcessManager();
    m_posAlign.x	= nPosX;
    m_posAlign.y	= nPosY;
    g_objGameMap.ProcessRegion(m_posAlign);
    // 自动召唤幻兽
    RECT CallRegion = {504, 541, 521, 574};
    if (this->GetLev() < 2 && m_posAlign.x > CallRegion.left && m_posAlign.x < CallRegion.right
            && m_posAlign.y > CallRegion.top && m_posAlign.y < CallRegion.bottom)
    {
        ::PostCmd(CMD_CALL_PET_OUT);
    }
    //幻兽走动
    int iPosX, iPosY;
    this->GetPos(iPosX, iPosY);
    //--------------------------------------
    //	if (_COMMAND_JUMP == m_Info.cmdProc.iType || _COMMAND_WALK== m_Info.cmdProc.iType
    //		|| _COMMAND_RUN == m_Info.cmdProc.iType )
    {
        const int MAX_CALL_PET = 3;
        CMyPos posTemp[MAX_CALL_PET];
        int nHeroDir = g_objHero.GetDir();
        if (this->GetProfession() == PROFESSIONAL_WARRIOR )
        {
            posTemp[0].x = iPosX + _DELTA_X[(nHeroDir+4)%8] * 4;
            posTemp[0].y = iPosY + _DELTA_Y[(nHeroDir+4)%8] * 4;
            posTemp[1].x = iPosX + _DELTA_X[(nHeroDir+3)%8] * 4;
            posTemp[1].y = iPosY + _DELTA_Y[(nHeroDir+3)%8] * 4;
            posTemp[2].x = iPosX + _DELTA_X[(nHeroDir+5)%8] * 4;
            posTemp[2].y = iPosY + _DELTA_Y[(nHeroDir+5)%8] * 4;
        }
        else if (this->GetProfession() == PROFESSIONAL_ENCHANTER)
        {
            posTemp[0].x = iPosX + _DELTA_X[(nHeroDir+1)%8] * 4;
            posTemp[0].y = iPosY + _DELTA_Y[(nHeroDir+1)%8] * 4;
            posTemp[1].x = iPosX + _DELTA_X[(nHeroDir+7)%8] * 4;
            posTemp[1].y = iPosY + _DELTA_Y[(nHeroDir+7)%8] * 4;
        }
        CPet* pPet = NULL;
        for(int i = 0 ; i < (int)m_setPetInfo.size(); i++)
        {
            pPet = GetPetByIndex(i);
            if (pPet)
            {
                pPet->SetNextPos(posTemp[i].x, posTemp[i].y);
            }
        }
    }
    //--------------------------------------
    //	::DebugMsg("英雄对齐坐标(%d, %d)", posHeroAlign.x, posHeroAlign.y);
}

//////////////////////////////////////////////////////////////////////
DWORD	CHero::GetDef()
{
    float fDef = 0.0f;
    fDef += this->GetHealth() * 0.5f;
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        if (m_pEquipment[i])
        {
            fDef += m_pEquipment[i]->GetDefense();
        }
    }
    fDef += 0.5f ;
    return (DWORD)fDef;
}
//////////////////////////////////////////////////////////////////////
DWORD	CHero::GetMagicDef()
{
    float fDef = 0.0f;
    switch(this->GetProfessionSort())
    {
    case PROFSORT_SOLDIER:
        fDef += this->GetHealth() * 0.5f;
        break;
    case PROFSORT_ARCHER:
        fDef += this->GetSoul() * 0.5f;
        break;
    case PROFSORT_MAGICIAN:
        fDef += this->GetSoul() * 0.5f;
        break;
    }
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        if (m_pEquipment[i])
        {
            fDef += m_pEquipment[i]->GetMagicDefence();
        }
    }
    fDef += 0.5f;
    return (DWORD)fDef;
}
//////////////////////////////////////////////////////////////////////

DWORD CHero::GetMaxMagicAtk()
{
    float fAtk = 0.0f;
    switch(this->GetProfessionSort())
    {
    case PROFSORT_MAGICIAN:
        fAtk += this->GetSoul() * 0.5f;
        break;
    }
    int nAttack = (int)(fAtk + 0.5f);
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        if (m_pEquipment[i])
        {
            nAttack += m_pEquipment[i]->GetMaxMagicAttack();
        }
    }
    return __max(0, nAttack);
}
//////////////////////////////////////////////////////////////////////
DWORD CHero::GetMinMagicAtk()
{
    float fAtk = 0.0f;
    switch(this->GetProfessionSort())
    {
    case PROFSORT_MAGICIAN:
        fAtk += this->GetSoul() * 0.5f;
        break;
    }
    int nAttack = (int)(fAtk + 0.5f);
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        if (m_pEquipment[i])
        {
            nAttack += m_pEquipment[i]->GetMinMagicAttack();
        }
    }
    return __max(0, nAttack);
}
//////////////////////////////////////////////////////////////////////
DWORD	CHero::GetAdditionAtk()
{
    int nAttack = 0;
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        CItem* pItem = m_pEquipment[i];
        if (pItem && !pItem->TestStatus(_ITEM_STATUS_MAGIC_ADD))
        {
            if ( i + 1 == ITEMPOSITION_SPRITE )
            {
                if ( pItem->GetProfessionRequired() == _SPRITE_ADDITION_PATK )
                {
                    nAttack += pItem->GetAmountLimit() ;
                }
            }
            else if (i + 1 == ITEMPOSITION_WEAPONR || i + 1 == ITEMPOSITION_RINGR ||  i + 1 == ITEMPOSITION_SHOES)
            {
                nAttack += m_pEquipment[i]->GetAddition();
            }
        }
    }
    return nAttack;
}
//////////////////////////////////////////////////////////////////////
DWORD	CHero::GetAdditionDef()
{
    int nDef = 0;
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        CItem* pItem = m_pEquipment[i];
        if (pItem && !pItem->TestStatus(_ITEM_STATUS_MAGIC_ADD))
        {
            if ( i + 1 == ITEMPOSITION_SPRITE )
            {
                if ( pItem->GetProfessionRequired() == _SPRITE_ADDITION_PDEF )
                {
                    nDef += pItem->GetAmountLimit() ;
                }
            }
            else if (i + 1 == ITEMPOSITION_ARMOR || i + 1 == ITEMPOSITION_HELMET
                     ||  i + 1 == ITEMPOSITION_NECKLACE || i + 1 == ITEMPOSITION_WEAPONL)
            {
                nDef += m_pEquipment[i]->GetAddition();
            }
        }
    }
    return nDef;
}
//////////////////////////////////////////////////////////////////////
DWORD	CHero::GetAdditionMAtk()
{
    int nAttack = 0;
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        CItem* pItem = m_pEquipment[i];
        if (pItem && pItem->TestStatus(_ITEM_STATUS_MAGIC_ADD))
        {
            if ( i + 1 == ITEMPOSITION_SPRITE )
            {
                if ( pItem->GetProfessionRequired() == _SPRITE_ADDITION_MATK )
                {
                    nAttack += pItem->GetAmountLimit() ;
                }
            }
            else if (i + 1 == ITEMPOSITION_WEAPONR || i + 1 == ITEMPOSITION_RINGR ||  i + 1 == ITEMPOSITION_SHOES)
            {
                nAttack += m_pEquipment[i]->GetAddition();
            }
        }
    }
    return nAttack;
}
//////////////////////////////////////////////////////////////////////
DWORD	CHero::GetAdditionMDef()
{
    int nDef = 0;
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        CItem* pItem = m_pEquipment[i];
        if (pItem && pItem->TestStatus(_ITEM_STATUS_MAGIC_ADD))
        {
            if ( i + 1 == ITEMPOSITION_SPRITE )
            {
                if ( pItem->GetProfessionRequired() == _SPRITE_ADDITION_MDEF )
                {
                    nDef += pItem->GetAmountLimit() ;
                }
            }
            else if (i + 1 == ITEMPOSITION_ARMOR || i + 1 == ITEMPOSITION_HELMET
                     ||  i + 1 == ITEMPOSITION_NECKLACE || i + 1 == ITEMPOSITION_WEAPONL)
            {
                nDef += m_pEquipment[i]->GetAddition();
            }
        }
    }
    return nDef;
}
//////////////////////////////////////////////////////////////////////

DWORD CHero::GetMaxAtk()
{
    float fAtk = 0.0f;
    switch(this->GetProfessionSort())
    {
    case PROFSORT_SOLDIER:
        fAtk += this->GetForce() * 1.0f;
        break;
    case PROFSORT_ARCHER:
        fAtk += this->GetSpeed() * 0.5f;
        break;
    case PROFSORT_MAGICIAN:
        fAtk += this->GetForce() * 0.5f;
        break;
    }
    int nAttack = (int)(fAtk + 0.5f);
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        if (m_pEquipment[i])
        {
            nAttack += m_pEquipment[i]->GetMaxAttack();
        }
    }
    return __max(0, nAttack);
}

//////////////////////////////////////////////////////////////////////
DWORD CHero::GetMinAtk()
{
    float fAtk = 0.0f;
    switch(this->GetProfessionSort())
    {
    case PROFSORT_SOLDIER:
        fAtk += this->GetForce() * 1.0f;
        break;
    case PROFSORT_ARCHER:
        fAtk += this->GetSpeed() * 0.5f;
        break;
    case PROFSORT_MAGICIAN:
        fAtk += this->GetForce() * 0.5f;
        break;
    }
    int nAttack = (int)(fAtk + 0.5f);
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        if (m_pEquipment[i])
        {
            nAttack += m_pEquipment[i]->GetMinAttack();
        }
    }
    return __max(0, nAttack);
}

//////////////////////////////////////////////////////////////////////
DWORD CHero::GetDex()
{
    int nDex = this->GetSpeed();
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        if (m_pEquipment[i])
        {
            nDex += m_pEquipment[i]->GetDodge();
        }
    }
    return __max(0, nDex);
}

//////////////////////////////////////////////////////////////////////
DWORD CHero::GetMaxLife()
{
    /*
    int nStep = 0;
    switch(this->GetProfessionSort())
    {
    case PROFSORT_SOLDIER:
    	nStep = 5;
    	break;

    case PROFSORT_ARCHER:
    	nStep = 7;
    	break;

    case PROFSORT_MAGICIAN:
    	nStep = 6;
    	break;
    }
    int nLife = (this->GetLev() - 1) * 3 + this->GetHealth() * nStep + 30;*/
    int nLife = CPlayer::GetMaxLife();
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        if (m_pEquipment[i])
        {
            nLife += m_pEquipment[i]->GetLife();
        }
    }
    return __max(0, nLife);
}

//////////////////////////////////////////////////////////////////////
DWORD CHero::GetMaxXP()
{
    // todo ...
    return 100;
}
//////////////////////////////////////////////////////////////////////
DWORD CHero::GetMaxMana()
{
    /*	int nStep = 0;
    	int nLevStep = 0;
    	switch(this->GetProfessionSort())
    	{
    	case PROFSORT_SOLDIER:
    		nStep = 5;
    		nLevStep = 3;
    		break;

    	case PROFSORT_ARCHER:
    		nStep = 5;
    		nLevStep = 3;
    		break;

    	case PROFSORT_MAGICIAN:
    		nStep = 5;
    		nLevStep = 3;
    		break;
    	}
    	int nMaxMana = this->GetSoul() * nStep + (this->GetLev() - 1) * nLevStep;
    	*/
    int nMaxMana = this->GetSoul() * 20;
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        if (m_pEquipment[i])
        {
            nMaxMana += m_pEquipment[i]->GetMana();
        }
    }
    return __max(0, nMaxMana);
}

//////////////////////////////////////////////////////////////////////
BOOL CHero::CanJump(CMyPos posTarGet)
{
    // Can I Jump Over It?
    int nCanJumpHeight =  m_nHeightCanJump;
    //	if (this->TestStatus(USERSTATUS_FLY))
    //		nCanJumpHeight = 100;
    CMyPos posStart, posEnd;
    posEnd = posTarGet;
    DEQUE_CELL setCell;
    this->GetPos(posStart);
    CMyPos posHighest;
    g_objGameMap.Sampling(setCell, posHighest, posStart, posEnd, 32);
    CellInfo* pTargetCell = g_objGameMap.GetCell(posHighest.x, posHighest.y);
    if (!pTargetCell)
    {
        return false;
    }
    CellInfo* pCellInfo = g_objGameMap.GetCell(posStart.x, posStart.y);
    if (!pCellInfo)
    {
        return false;
    }
    if (g_objGameMap.GetGroundAltitude(pTargetCell) - g_objGameMap.GetGroundAltitude(pCellInfo) > nCanJumpHeight)
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CHero::JoinSynMember	(CPlayer* pTarget)
{
    if (!pTarget)
    {
        return;
    }
    CMyPos posHero;
    this->GetPos(posHero);
    CMsgInteract msg;
    MYASSERT (msg.Create(INTERACT_JOINSYN, this->GetID(), pTarget->GetID(), posHero.x, posHero.y));
    msg.Send();
}

//////////////////////////////////////////////////////////////////////
void CHero::AcceptSynMember	(CPlayer* pTarget)
{
    if (!pTarget)
    {
        return;
    }
    CMyPos posHero;
    this->GetPos(posHero);
    CMsgInteract msg;
    MYASSERT (msg.Create(INTERACT_ACCEPTSYNMEMBER, this->GetID(), pTarget->GetID(), posHero.x, posHero.y));
    msg.Send();
}

//////////////////////////////////////////////////////////////////////
void CHero::TitleSynMember	(CPlayer* pTarget, const char* pszTitle)
{
    if (!pTarget || !pszTitle)
    {
        return;
    }
    CMsgName msg;
    MYASSERT (msg.Create(NAMEACT_CHANGE_SYNTITLE, pszTitle, pTarget->GetID()));
    msg.Send();
}

//////////////////////////////////////////////////////////////////////
void CHero::KickoutSynMember(CPlayer* pTarget)
{
    if (!pTarget)
    {
        return;
    }
    CMsgSyndicate msg;
    if (msg.Create(KICKOUT_MEMBER, pTarget->GetID()))
    {
        msg.Send();
    }
}

//////////////////////////////////////////////////////////////////////
void CHero::QuitSyn(void)
{
    CMyPos posHero;
    this->GetPos(posHero);
    CMsgAction msg;
    MYASSERT (msg.Create(this->GetID(), posHero.x, posHero.y, this->GetDir(), actionQuitSyn));
    msg.Send();
}

//////////////////////////////////////////////////////////////////////
void CHero::ReBorn()
{
    if (this->IsMoving())
    {
        const OBJID idCanNotReBorn = 100034;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idCanNotReBorn));
        return;
    }
    if (!this->TestStatus(USERSTATUS_GHOST))
    {
        return;
    }
    CMsgAction msg;
    if (msg.Create(this->GetID(), 0, 0, 0, actionReborn))
    {
        msg.Send();
    }
    /*
    	CMyPos posCell;
    	g_objHero.GetPos(posCell);
    	if (msg.Create(g_objHero.GetID(), 0, 0, g_objHero.GetDir(), actionSynchro, posCell.x, posCell.y))
    		msg.Send();*/
}

//////////////////////////////////////////////////////////////////////
BOOL CHero::ProcessChgMap(void)
{
    if (this->IsDead() || this->IsGhost())
    {
        return false;
    }
    if (_CMDSTATUS_ACCOMPLISH != this->GetCommandStatus())
    {
        return false;
    }
    int nActionType = this->GetActionType();
    if (_ACTION_STANDBY != nActionType &&
            _ACTION_REST1 != nActionType &&
            _ACTION_REST2 != nActionType &&
            _ACTION_REST3 != nActionType &&
            _ACTION_STANDBY_I != nActionType &&
            _ACTION_FLY_STANDBY != nActionType &&
            _ACTION_FLY_ALERT != nActionType)
    {
        return false;
    }
    CMyPos posHero;
    this->GetPos(posHero);
    // 看九格
    int nX[9] = {0, 1, -1, 1, 0, 1, -1, -1, 0};
    int nY[9] = {0, 1, -1, 0, 1, -1, 1, 0, -1};
    for(int i = 0; i < 9; i ++)
    {
        CMyPos posTest;
        posTest.x = posHero.x + nX[i];
        posTest.y = posHero.y + nY[i];
        if (g_objGameMap.GetExitIndex(posTest) >= 0)
        {
            CMsgAction msg;
            IF_SUC (msg.Create (this->GetID(), posHero.x, posHero.y, this->GetDir(), actionChgMap, posTest.x, posTest.y))
            {
                msg.Send();
                g_uStatus = _STATUS_WAITING;
                return true;
            }
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CHero::ProcessReborn(void)
{
    // reborn...
    if (this->IsGhost() && m_timeGhost != 0 && ::TimeGet() - m_timeGhost >= 20 * 1000)
    {
        m_timeGhost = 0;
    }
    // to ghost...
    if (this->IsDead() && m_timeDie != 0 && ::TimeGet() - m_timeDie >= 3000)
    {
        m_timeDie = 0;
        CMsgAction msg;
        if (msg.Create(this->GetID(), 0, 0, 0, actionGhost))
        {
            msg.Send();
        }
    }
}


//////////////////////////////////////////////////////////////////////
void CHero::Process(void* pInfo)
{
    if (::TimeGet()	- m_dwTimeWhoAttackMe > 30 * 1000)
    {
        m_idWhoAttackMe = ID_NONE;
    }
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idMyTarget);
    if (!pPlayer || pPlayer->IsDead())
    {
        this->SetIDMyTarget(ID_NONE);
    }
    this->ProcessMusic();
    if (this->GetPhysicalForce() == 0)
    {
        m_Info.bAdjustRun = true;
    }
    else
    {
        m_Info.bAdjustRun = false;
    }
    m_objCheck.Process();
    CPlayer::Process(pInfo);
    m_objTeam.Process();
    m_objDummy.ProcessDummy();
    m_objPlayerDummy.ProcessDummy();
    this->ProcessMagicBusy();
    this->ProcessReborn();
    this->ProcessXp();
    this->ProcessChgMap();
    this->ProcessFriend();
    this->ProcessEnemy();
    this->ProcessMousePlayer();
    this->ProcessMagic();
    this->ProcessAbortTransformDialog();
    this->ProcessLifeForeCast();
    m_objDiceManager.Process();
    //int nLife = this->GetLife();
    //int nCurLife = this->GetCurLife();
    int nLife = this->GetData(CPlayer::_PLAYER_LIFE);
    int nCurLife = this->GetData(CPlayer::_PLAYER_CURRENTLIFE);
    int nMaxLife = this->GetMaxLife();
    if (nCurLife < nLife)
    {
        if (nMaxLife >= 80)
        {
            nCurLife += nMaxLife / 80;
        }
        else
        {
            nCurLife ++;
        }
    }
    if (nCurLife > nLife)
    {
        nCurLife = nLife;
    }
    this->SetData(CPlayer::_PLAYER_CURRENTLIFE, nCurLife);
    // a puzzle ...
    this->SetCurLife(::RandGet(INT_MAX));
    /*
    	//-----------------------------------------------------------------
    	{
    		DWORD dwKey[4] = {::RandGet(INT_MAX), ::RandGet(INT_MAX), ::RandGet(INT_MAX), ::RandGet(INT_MAX)};
    		this->m_PlayerInfo.dataCurLife.set_key(dwKey, 4);
    		this->SetCurLife(nCurLife);
    	}
    	//-----------------------------------------------------------------
    	{
    		DWORD dwKey[4] = {::RandGet(INT_MAX), ::RandGet(INT_MAX), ::RandGet(INT_MAX), ::RandGet(INT_MAX)};
    		this->m_PlayerInfo.dataLife.set_key(dwKey, 4);
    		this->SetLife(nLife);
    	}
    */
    int nMana = this->GetData(CPlayer::_PLAYER_MANA);
    int nCurMana = this->GetCurMana();
    int nMaxMana = this->GetMaxMana();
    if (nCurMana < nMana)
    {
        if (nMaxMana >= 80)
        {
            nCurMana += nMaxMana / 80;
        }
        else
        {
            nCurMana ++;
        }
    }
    if (nCurMana > nMana)
    {
        nCurMana = nMana;
    }
    //-----------------------------------------------------------------
    {
        DWORD dwKey[4] = {::RandGet(INT_MAX), ::RandGet(INT_MAX), ::RandGet(INT_MAX), ::RandGet(INT_MAX)};
        this->m_PlayerInfo.dataCurMana.set_key(dwKey, 4);
        this->SetCurMana(nCurMana);
    }
    //-----------------------------------------------------------------
    {
        DWORD dwKey[4] = {::RandGet(INT_MAX), ::RandGet(INT_MAX), ::RandGet(INT_MAX), ::RandGet(INT_MAX)};
        this->m_PlayerInfo.dataMana.set_key(dwKey, 4);
        this->SetMana(nMana);
    }
}

//////////////////////////////////////////////////////////////////////
void CHero::DoLevelUp()
{
    OBJID idLevelUp = 100035;
    g_objGameMsg.AddMsg(_TXTATR_SYSTEM, g_objGameDataSet.GetStr(idLevelUp));
    CPlayer::DoLevelUp();
    ::PostCmd(CMD_LEVELUP);
    /*	if (m_dwAutoAllot == _AUTOALLOT)
    	{
    		// to do autoalloc here ...
    		FourAttribInfo infoAttrib;
    		if (!m_objAllot.GetAllot(this->GetLev(), this->GetProfession(), infoAttrib))
    			return;

    		unsigned char ucSpeed		=	(unsigned char)(infoAttrib.dwSpeed - this->GetSpeed());
    		unsigned char ucForce		=	(unsigned char)(infoAttrib.dwForce - this->GetForce());
    		unsigned char ucHealth		=	(unsigned char)(infoAttrib.dwHealth - this->GetHealth());
    		unsigned char ucSoul		=	(unsigned char)(infoAttrib.dwSoul - this->GetSoul());

    		int nAddPoint = this->GetAddPoint();
    		if ((ucSpeed + ucForce + ucHealth + ucSoul) > nAddPoint)
    		{
    			// 检查合法性
    			return;
    		}

    		if (this->GetSpeed() > infoAttrib.dwSpeed ||
    				this->GetForce() > infoAttrib.dwForce ||
    					this->GetHealth() > infoAttrib.dwHealth ||
    						this->GetSoul() > infoAttrib.dwSoul)
    			return;

    		this->SetSpeed(infoAttrib.dwSpeed);
    		this->SetForce(infoAttrib.dwForce);
    		this->SetSoul(infoAttrib.dwSoul);
    		this->SetHealth(infoAttrib.dwHealth);

    		this->SetAddPoint(nAddPoint-(ucSpeed + ucForce + ucHealth + ucSoul));

    		CMsgAllot msg;
    		if (msg.Create(ucForce, ucSpeed, ucHealth, ucSoul))
    			msg.Send();
    	}*/
}

//////////////////////////////////////////////////////////////////////
void CHero::AllotPoint(int nForce, int nSpeed, int nHealth, int nSoul)
{
    int nAddPoint = this->GetAddPoint();
    if (nAddPoint <= 0)
    {
        return;
    }
    int nAddSpeed	= (nSpeed - this->GetSpeed());
    int nAddForce	= (nForce - this->GetForce());
    int nAddHealth	= (nHealth - this->GetHealth());
    int nAddSoul	= (nSoul - this->GetOriginalSoul());
    if (nAddSpeed == 0 && nAddForce == 0 && nAddHealth == 0 && nAddSoul == 0)
    {
        return;
    }
    if ((nAddSpeed + nAddForce + nAddHealth + nAddSoul) > nAddPoint)
    {
        return;
    }
    this->SetSpeed(nSpeed);
    this->SetForce(nForce);
    this->SetSoul(nSoul);
    this->SetHealth(nHealth);
    this->SetAddPoint(nAddPoint - (nAddSpeed + nAddForce + nAddHealth + nAddSoul));
    CMsgAllot msg;
    if (msg.Create(nAddForce, nAddSpeed, nAddHealth, nAddSoul))
    {
        msg.Send();
    }
}

//////////////////////////////////////////////////////////////////////
void CHero::Show(void* pInfo)
{
    CPlayer::Show(pInfo);
    this->ShowChargeUp();
}

//////////////////////////////////////////////////////////////////////
void CHero::ShowXp(int talk_width)
{
    this->ShowFly();
    this->ShowTransform();
    if (this->TestStatus(USERSTATUS_XPFULL) && this->GetXP() == this->GetMaxXP())
    {
        DWORD dwTime = ::TimeGet();
        m_dwXpFullTime = XPFULL_TIME - (dwTime - m_dwXpStartTime);
        // show time counter
        if (m_dwXpFullTime >= 0 && m_dwXpFullTime <= XPFULL_TIME)
        {
            int x1 = (_SCR_WIDTH - talk_width) / 2 + 20;
            int y1 = _SCR_HEIGHT - 116;
            int x2 = (_SCR_WIDTH - talk_width) / 2 + 20 + m_dwXpFullTime / 100;
            int y2 = _SCR_HEIGHT - 116 + 16;
#ifdef _DEBUG
            char szTemp[64];
            sprintf(szTemp, "%d %d", m_dwXpFullTime , m_dwXpStartTime);
            CMyBitmap::ShowString(25, 617, 0xffff0000, szTemp, "宋体", 16);
#endif
			CMyBitmap::ShowString((_SCR_WIDTH - talk_width) / 2, _SCR_HEIGHT - 116, 0xffff0000, "XP", "宋体", 16);
            CMyBitmap::ShowBlock(x1, y1, x2, y2, 0xc00ff00, 0xc000ff00, 0xff00ff00, 0xff00ff00);
        }
		
        // show ko counter
        CMyPos posKo ;
		posKo.x = _SCR_WIDTH - 160 + 80;
		posKo.y = _SCR_HEIGHT - 170;
        m_objImgNumber.SetShowPos(posKo);
        char szInfo[256] = "";
        sprintf(szInfo, "%02u", m_dwKoCount);
        m_objImgNumber.SetString(szInfo);
        if (m_dwKoScale > 100)
        {
            m_dwKoScale -= 10;
        }
        m_objImgNumber.SetScalePercent(m_dwKoScale);
        m_objImgNumber.SetColor(_COLOR_RED, 0xff);
        m_objImgNumber.Show();
        CAni* pAni = g_objGameDataSet.GetDataAni("ani/common.ani", "Ko", EXIGENCE_IMMEDIATE);
        if (pAni)
        {
            CMyBitmap* pBmp = pAni->GetFrame(0);
            if (pBmp)
            {
                CMySize sizeKO;
                pBmp->GetSize(sizeKO.iWidth, sizeKO.iHeight);
                sizeKO.iWidth = sizeKO.iWidth * m_dwKoScale / 100;
                sizeKO.iHeight = sizeKO.iHeight * m_dwKoScale / 100;
                CMyPos posShow;
				posShow.x = _SCR_WIDTH - 160 + 80;
				posShow.y = _SCR_HEIGHT - 165;
                posShow.x = posShow.x - sizeKO.iWidth * 4 / 3 - strlen(szInfo) * 16;
                posShow.y = posShow.y - sizeKO.iHeight / 2;
                pBmp->ShowEx(posShow.x, posShow.y, NULL, sizeKO.iWidth, sizeKO.iHeight);
            }
        }
    }
    g_objImgNumCounter.Show();
    // name alert show
    if (m_tmNameAlert.IsActive() && !m_tmNameAlert.TimeOver())
    {
        if (!(CPlayer::s_bShowName || g_objGameMap.GetShowMapObjInfoFlag() || this->IsDead()))
        {
            this->ShowName();
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CHero::BurstXp()
{
    if (this->IsDead())
    {
        return;
    }
    if (this->GetXP() >= this->GetMaxXP())
    {
        CMsgAction msg;
        if (msg.Create(this->GetID(), 0, 0, 0, actionBurstXp))
        {
            msg.Send();
        }
    }
}
//////////////////////////////////////////////////////////////////////
void CHero::ProcessXp(void)
{
    if (this->IsDead())
    {
        return;
    }
    //	if (this->TestStatus(USERSTATUS_XPFULL) )
    {
    }
}
//////////////////////////////////////////////////////////////////////
void CHero::ResetXpFullTime(DWORD dwFullTime)
{
    DWORD dwTime = ::TimeGet();
    m_dwXpFullTime = dwFullTime * 1000;
    m_dwXpStartTime = dwTime + m_dwXpFullTime - XPFULL_TIME;
}
//////////////////////////////////////////////////////////////////////
void CHero::AddKoCount()
{
    if (this->TestStatus(USERSTATUS_XPFULL))
    {
        m_dwKoCount ++;
        SetKoAmount(m_dwKoCount);
        m_dwKoScale = 200;
        //		m_dwXpFullTime = __min(m_dwXpStartTime+XPFULL_TIME, m_dwXpFullTime+KILLAWARD_XPTIME);
    }
    /*	else if (this->TestStatus(USERSTATUS_TORNADO))
    	{
    		m_dwKoCount ++;
    		m_dwKoScale = 200;
    		m_dwSuperSoldierBegin += KILL_AWARD_TORNADO;
    		if (m_dwSuperSoldierBegin >= ::TimeGet()+(TICKS_SPSOLDIER - TICKS_SPSOLDIER_INIT) * INTERVAL_SPSOLDIER)
    			m_dwSuperSoldierBegin = ::TimeGet()+(TICKS_SPSOLDIER - TICKS_SPSOLDIER_INIT) * INTERVAL_SPSOLDIER;
    	}*/
}
//////////////////////////////////////////////////////////////////////
void CHero::ReplaceStatus(__int64 dwStatus)
{
    if (!this->TestStatus(USERSTATUS_PELT))
    {
        if ((dwStatus & USERSTATUS_PELT) != 0)
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100169));
            m_nFastRunSetp = 0;
        }
    }
    else
    {
        if ((dwStatus & USERSTATUS_PELT) == 0)
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100170));
        }
    }
    // status of pk
    if (!this->TestStatus(USERSTATUS_RED))
    {
        // white to red
        if ((dwStatus & USERSTATUS_RED) != 0)
        {
            //m_tmNameAlert.Startup(60*1000);	// 60s
            //g_objGameMsg.AddMsg(_TXTATR_GM, g_objGameDataSet.GetStr(100153));
        }
    }
    if (!this->TestStatus(USERSTATUS_DARKRED))
    {
        // red to black
        if ((dwStatus & USERSTATUS_DARKRED) != 0)
        {
            //m_tmNameAlert.Startup(60*1000);	// 60s
            //g_objGameMsg.AddMsg(_TXTATR_GM, g_objGameDataSet.GetStr(100154));
        }
    }
    if (this->TestStatus(USERSTATUS_RED))
    {
        // red to white
        if ((dwStatus & USERSTATUS_RED) == 0)
        {
            //g_objGameMsg.AddMsg(_TXTATR_GM, g_objGameDataSet.GetStr(100155));
        }
    }
    if (this->TestStatus(USERSTATUS_DARKRED))
    {
        // black to red
        if ((dwStatus & USERSTATUS_DARKRED) == 0)
        {
            //	g_objGameMsg.AddMsg(_TXTATR_GM, g_objGameDataSet.GetStr(100156));
        }
    }
    // xp
    if (!this->TestStatus(USERSTATUS_XPFULL))
    {
        if ((dwStatus & USERSTATUS_XPFULL))
        {
            m_dwKoCount = 0;
            SetKoAmount(m_dwKoCount);
            m_dwKoScale = 100;
            m_dwXpFullTime = XPFULL_TIME;
            m_dwXpStartTime = ::TimeGet();
        }
    }
    else
    {
        if ((dwStatus & USERSTATUS_XPFULL) == 0)
        {
            m_dwXpStartTime = 0;
            m_dwKoCount = 0;
            SetKoAmount(m_dwKoCount);
            m_dwXpFullTime = 0;
        }
    }
    // ghost
    if (!this->TestStatus(USERSTATUS_GHOST))
    {
        if (dwStatus & USERSTATUS_GHOST)
        {
            this->UnMagicLock();
            __int64 _64Status = this->GetStatus();
            this->ReplaceStatus(_64Status & (0xffffffffffffffff - USERSTATUS_CANNOTMOVE));
            // switch to ghost status
            m_timeGhost = ::TimeGet();
            const int TIME_DELAY = 20;
            g_objImgNumCounter.Start(TIME_DELAY);
            CMyPos posShow = {_SCR_WIDTH - 80, _SCR_HEIGHT - 130};
            g_objImgNumCounter.SetShowPos(posShow);
            OBJID idHowtoReborn = 100036;
            g_objGameMsg.AddMsg(_TXTATR_SYSTEM, g_objGameDataSet.GetStr(idHowtoReborn));
            ::PostCmd(CMD_GHOST_BEGIN);
        }
    }
    else
    {
        if ((dwStatus & USERSTATUS_GHOST) == 0)
        {
            // reborn now...
            ::PostCmd(CMD_REBORN);
            ::PostCmd(CMD_GHOST_END);
            g_objImgNumCounter.End();
        }
    }
    // die
    if (this->TestStatus(USERSTATUS_DIE))
    {
        // die already
        if (0 == (dwStatus & USERSTATUS_DIE))
        {
            // reborn
            this->StandBy();
        }
    }
    else	// still alive
    {
        if (0 != (dwStatus & USERSTATUS_DIE))
        {
            // die
            m_timeDie = ::TimeGet();
            this->QuitTrade();
            this->CloseTrade(false);
        }
    }
    // tornado
    /*	if (dwStatus&USERSTATUS_TORNADO)
    	{
    		if (!this->TestStatus(USERSTATUS_TORNADO))
    		{
    			m_dwKoCount	= 0;
    			m_dwKoScale = 100;
    			m_dwSuperSoldierBegin = ::TimeGet() - (TICKS_SPSOLDIER - TICKS_SPSOLDIER_INIT) * INTERVAL_SPSOLDIER;
    			m_dwSuperSoldierBeginCopy = m_dwSuperSoldierBegin;

    			m_dwSuperSoldierBegin += this->GetKoXpAdd()*1000;
    			if (m_dwSuperSoldierBegin >= ::TimeGet()+(TICKS_SPSOLDIER - TICKS_SPSOLDIER_INIT) * INTERVAL_SPSOLDIER)
    				m_dwSuperSoldierBegin = ::TimeGet()+(TICKS_SPSOLDIER - TICKS_SPSOLDIER_INIT) * INTERVAL_SPSOLDIER;
    		}
    	}*/
    // chargeup
    //	if (this->TestStatus(USERSTATUS_CHARGEUP))
    //	{
    //		if (!(dwStatus&USERSTATUS_CHARGEUP))
    //		{
    //			m_bChargeUp = false;
    //		}
    //	}
    /*	if (!this->TestStatus(USERSTATUS_HITRATE))
    	{
    		if (dwStatus&USERSTATUS_HITRATE)
    		{
    			const OBJID idMagicNote = 100063;
    			g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMagicNote));
    		}
    	}
    	if (!this->TestStatus(USERSTATUS_DEFENCE))
    	{
    		if (dwStatus&USERSTATUS_DEFENCE)
    		{
    			const OBJID idMagicNote = 100064;
    			g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMagicNote));
    		}
    	}*/
    // do part of player
    CPlayer::ReplaceStatus(dwStatus);
}
//////////////////////////////////////////////////////////////////////
void CHero::CreateLevelExp()
{
    const char szPass[] = "★就这样被你蒸熟★";
    char szDataFile[] = "ini/LevelExp.dat";
    FILE* fp = fopen(szDataFile, "rb");
    MYASSERT(fp);
    fread(m_setLevelExp, sizeof(DWORD), _MAX_LEVEL * 2 - 1, fp);
    for(int i = 0; i < _MAX_LEVEL * 2 - 1; i++)
    {
        m_setLevelExp[i] = m_setLevelExp[i] ^ szPass[i%strlen(szPass)];
    }
    fclose(fp);
}
//////////////////////////////////////////////////////////////////////
DWORD CHero::GetNextLevelExp()
{
    if (this->GetLev() < 1)
    {
        return 0;
    }
    if (this->GetLev() >= _MAX_LEVEL)
    {
        return 0;
    }
    return m_setLevelExp[this->GetLev()-1];
}
//////////////////////////////////////////////////////////////////////
DWORD CHero::GetPetLevelExp(int nLev)
{
    if (nLev < 1)
    {
        return 0;
    }
    if (nLev >= _MAX_LEVEL)
    {
        return 0;
    }
    return m_setLevelExp[nLev + _MAX_LEVEL -1];
}
//////////////////////////////////////////////////////////////////////
void CHero::DeleteRole(void)
{
    CMyPos posHero;
    this->GetPos(posHero);
    CMsgAction msg;
    if (msg.Create(this->GetID(), posHero.x, posHero.y, this->GetDir(), actionDelRole))
    {
        msg.Send();
    }
}

//////////////////////////////////////////////////////////////////////
char* CHero::GetProfessionalName(DWORD dwIndex)
{
    return m_objProfessional.GetProfessionalName(dwIndex);
}
//////////////////////////////////////////////////////////////////////
void CHero::SetPkMode(int nPkMode, BOOL bSubmit)
{
    // if bSubmit send netmsg else post winmsg
    if (bSubmit)
    {
        CMsgAction msg;
        if (msg.Create(g_objHero.GetID(), 0, 0, 0, actionSetPkMode, nPkMode))
        {
            msg.Send();
        }
    }
    else
    {
        m_nPkMode = nPkMode;
        ::PostCmd(CMD_CHANGEPKMODE);
    }
}
//////////////////////////////////////////////////////////////////////
void CHero::AbortIntone()
{
    // server have done it for me
    return;
    if (this->GetCommandType() == _COMMAND_INTONE)
    {
        if (this->GetCommandFlag())
        {
            this->ResetActionData();
        }
        CMsgInteract msg;
        if (msg.Create(INTERACT_ABORTMAGIC, this->GetID(), ID_NONE, 0, 0))
        {
            msg.Send();
        }
    }
}
//////////////////////////////////////////////////////////////////////
void CHero::Relife()
{
    if (this->TestStatus(USERSTATUS_GHOST))
    {
        CMsgAction msg;
        if (msg.Create(this->GetID(), 0, 0, 0, actionReborn))
        {
            msg.Send();
        }
    }
}
//////////////////////////////////////////////////////////////////////
void CHero::Body2Ghost()
{
    CMsgAction msg;
    if (msg.Create(this->GetID(), 0, 0, 0, actionGhost))
    {
        msg.Send();
    }
}
//////////////////////////////////////////////////////////////////////
void CHero::ChangeFace(int nFace)
{
    if (this->GetMoney() < 500)
    {
        OBJID idMoneyNotEnough = 100037;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMoneyNotEnough));
        return;
    }
    CMsgAction msgAction;
    if (msgAction.Create(this->GetID(), 0, 0, 0, actionChangeFace, nFace))
    {
        msgAction.Send();
    }
}
//////////////////////////////////////////////////////////////////////
void CHero::SetSyndicateRank(DWORD dwRank)
{
    if (this->GetSyndicateRank() != dwRank)
    {
        ::PostCmd(CMD_FLASH_SYNDICATE);
    }
    this->CPlayer::SetSyndicateRank(dwRank);
}
//////////////////////////////////////////////////////////////////////
void CHero::ProcessMousePlayer()
{
    if (!n_bMousePlayerTerrain)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(MOUSE_PLAYER_ID);
        if (pPlayer)
        {
            pPlayer->Process(NULL);
            if (!m_bMousePlayerFixed)
            {
                CMyPos posMouseCell, posMouseScr, posMouseWorld;
                ::MouseCheck(posMouseScr.x, posMouseScr.y);
                g_objGameMap.Mouse2Cell(posMouseScr.x, posMouseScr.y, posMouseCell.x, posMouseCell.y);
                pPlayer->SetPos(posMouseCell);
                g_objGameMap.Cell2World(posMouseCell.x, posMouseCell.y, posMouseWorld.x, posMouseWorld.y);
                pPlayer->SetWorldPos(posMouseWorld);
            }
        }
    }
    else
    {
        if (!m_bMousePlayerFixed)
        {
            CMyPos posMouseCell, posMouseScr;
            ::MouseCheck(posMouseScr.x, posMouseScr.y);
            g_objGameMap.Mouse2Cell(posMouseScr.x, posMouseScr.y, posMouseCell.x, posMouseCell.y);
            g_objGameMap.SetTerrainNpcPos(MOUSE_PLAYER_ID, posMouseCell);
        }
    }
}
//////////////////////////////////////////////////////////////////////
void CHero::FixMousePlayer()
{
    m_bMousePlayerFixed = true;
    ::PostCmd(CMD_OPEN_PLACE_NPC_MODIFY_DIALOG);
}
//////////////////////////////////////////////////////////////////////
void CHero::CreateMousePlayer(int nLookType, BOOL bTerrain)
{
    n_bMousePlayerTerrain = bTerrain;
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(MOUSE_PLAYER_ID);
    if (pPlayer)
    {
        g_objPlayerSet.DelPlayer(MOUSE_PLAYER_ID);
    }
    g_objGameMap.DeleteTerrainNpc(MOUSE_PLAYER_ID);
    m_bMousePlayerFixed = false;
    if (!bTerrain)
    {
        pPlayer	= new CPlayer;
        MYASSERT (pPlayer);
        pPlayer->SetID(MOUSE_PLAYER_ID);
        pPlayer->SetName("");
        pPlayer->Create(nLookType, _ROLE_VIEW_TASKNPC);
        pPlayer->SetRoleType(_ROLE_MOUSE_NPC);
        NpcTypeInfo* pInfo = g_obj3DRoleData.GetNpcTypeInfo(nLookType / 10);
        MYASSERT(pInfo);
        pPlayer->SetDir(nLookType % 10);
        pPlayer->m_objEffect.Add(pInfo->szEffectIndex, false, 0, pPlayer->GetLook());
        this->ProcessMousePlayer();
        g_objPlayerSet.AddPlayer(pPlayer);
    }
    else
    {
        CMyPos posCell;
        CMyPos posMouse;
        ::MouseCheck(posMouse.x, posMouse.y);
        g_objGameMap.Screen2Cell(posMouse.x, posMouse.y, posCell.x, posCell.y);
        g_objGameMap.AddTerrainNpc(nLookType / 10, nLookType % 10, MOUSE_PLAYER_ID, posCell);
    }
    ::PostCmd(CMD_OPEN_PLACE_NPC_INFO_DIALOG);
}
//////////////////////////////////////////////////////////////////////
void CHero::SetMousePlayerDir(int nDir)
{
    if (!n_bMousePlayerTerrain)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(MOUSE_PLAYER_ID);
        if (pPlayer)
        {
            pPlayer->m_pIRoleView->SetVariable(_VAR_DIRECT_DIR, nDir);
        }
    }
    else
    {
        g_objGameMap.SetTerrainNpcDir(MOUSE_PLAYER_ID, nDir);
    }
}
//////////////////////////////////////////////////////////////////////
void CHero::PutMousePlayer()
{
    if (!n_bMousePlayerTerrain)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(MOUSE_PLAYER_ID);
        if (pPlayer)
        {
            CMsgNpcInfo msg;
            CMyPos posNpc;
            pPlayer->GetPos(posNpc);
            if (msg.Create(posNpc, pPlayer->m_pIRoleView->GetVariable(_VAR_LOOK)))
            {
                msg.Send();
            }
            this->DelMousePlayer();
        }
    }
    else
    {
        CMyPos posCell;
        g_objGameMap.GetTerrainNpcPos(MOUSE_PLAYER_ID, posCell);
        int nLook = g_objGameMap.GetTerrainNpcLook(MOUSE_PLAYER_ID);
        CMsgNpcInfo msg;
        if (msg.Create(posCell, nLook))
        {
            msg.Send();
        }
        this->DelMousePlayer();
    }
}
//////////////////////////////////////////////////////////////////////
void CHero::DelMousePlayer()
{
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(MOUSE_PLAYER_ID);
    if (pPlayer)
    {
        g_objPlayerSet.DelPlayer(MOUSE_PLAYER_ID);
    }
    g_objGameMap.DeleteTerrainNpc(MOUSE_PLAYER_ID);
    m_bMousePlayerFixed = true;
}
//////////////////////////////////////////////////////////////////////
void CHero::DelNpc(OBJID idNpc)
{
    CMsgNpc msg;
    if (msg.Create(EVENT_DELNPC, idNpc))
    {
        msg.Send();
    }
}
//////////////////////////////////////////////////////////////////////
BOOL CHero::IsTransform()
{
    if (!m_pIRoleView)
    {
        return false;
    }
    return m_pIRoleView->GetVariable(_VAR_LOOKCLONE);
}
//////////////////////////////////////////////////////////////////////
void CHero::BeginTransForm(DWORD dwMagicType, DWORD dwLevel)
{
    MagicTypeInfo infoMagic;
    if (!CMagic::s_objMagicData.GetMagicTypeInfo(dwMagicType, dwLevel, infoMagic))
    {
        return;
    }
    if (infoMagic.dwActionSort != MAGICSORT_TRANSFORM)
    {
        return;
    }
    m_dwTransformDuration = infoMagic.dwDuration * 1000;
    m_dwTransformBegin = ::TimeGet();
}
//////////////////////////////////////////////////////////////////////
void  CHero::BeginFly(DWORD dwMagicType, DWORD dwLevel)
{
    MagicTypeInfo infoMagic;
    if (!CMagic::s_objMagicData.GetMagicTypeInfo(dwMagicType, dwLevel, infoMagic))
    {
        return;
    }
    //	if ((infoMagic.dwStatus  & USERSTATUS_FLY) == 0)
    //		return;
    m_dwFlyDuration = infoMagic.dwDuration * 1000;
    m_dwFlyBegin = ::TimeGet();
}
//////////////////////////////////////////////////////////////////////
void	CHero::ShowFly()
{
    /*
    	if (!this->TestStatus(USERSTATUS_FLY))
    	{
    		m_dwFlyDuration = 0;
    		return;
    	}
    	else
    	{
    		DWORD dwTimePass = ::TimeGet()-m_dwFlyBegin;
    		if (dwTimePass > m_dwFlyDuration)
    			return;
    		CMyPos posTF = {_SCR_WIDTH-200, _SCR_HEIGHT-220};
    		m_objImgNumber.SetShowPos(posTF);

    		char szInfo[256] = "";
    		if (m_dwFlyDuration/1000 > 100)
    			sprintf(szInfo, "%03u", (m_dwFlyDuration - dwTimePass)/1000);
    		else
    			sprintf(szInfo, "%02u", (m_dwFlyDuration - dwTimePass)/1000);
    		m_objImgNumber.SetString(szInfo);

    		m_objImgNumber.SetScalePercent(130);
    		if ((m_dwFlyDuration - dwTimePass)/1000 < 10)
    			m_objImgNumber.SetColor(_COLOR_RED, 0x99);
    		else
    			m_objImgNumber.SetColor(_COLOR_YELLOW, 0x77);
    		m_objImgNumber.Show();
    	}*/
}
//////////////////////////////////////////////////////////////////////

void CHero::ShowTransform()
{
    if (!this->IsTransform())
    {
        m_dwTransformDuration = 0;
        return;
    }
    else
    {
        DWORD dwTimePass = ::TimeGet() - m_dwTransformBegin;
        if (dwTimePass > m_dwTransformDuration)
        {
            return;
        }
        CMyPos posTF = {_SCR_WIDTH - 200, _SCR_HEIGHT - 220};
        m_objImgNumber.SetShowPos(posTF);
        char szInfo[256] = "";
        if (m_dwTransformDuration / 1000 > 100)
        {
            sprintf(szInfo, "%03u", (m_dwTransformDuration - dwTimePass) / 1000);
        }
        else
        {
            sprintf(szInfo, "%02u", (m_dwTransformDuration - dwTimePass) / 1000);
        }
        m_objImgNumber.SetString(szInfo);
        m_objImgNumber.SetScalePercent(130);
        if ((m_dwTransformDuration - dwTimePass) / 1000 < 10)
        {
            m_objImgNumber.SetColor(_COLOR_RED, 0x99);
        }
        else
        {
            m_objImgNumber.SetColor(_COLOR_YELLOW, 0x77);
        }
        m_objImgNumber.Show();
    }
}
//////////////////////////////////////////////////////////////////////
void CHero::ProcessAbortTransformDialog()
{
    BOOL bShow = true;
    if (! this->IsTransform() || this->m_pIRoleView->GetVariable(_VAR_LOOKCLONE) == 98 || this->m_pIRoleView->GetVariable(_VAR_LOOKCLONE) == 99)
    {
        bShow = false;
    }
    if (this->IsDead())
    {
        bShow = false;
    }
    DWORD dwTimePass = ::TimeGet() - m_dwTransformBegin;
    const DWORD dwShowDelay = 30 * 1000;
    if (dwTimePass < dwShowDelay)
    {
        bShow = false;
    }
    if (m_bShowAbortTransformDialog != bShow)
    {
        m_bShowAbortTransformDialog = bShow;
        if (!bShow)
        {
            ::PostCmd(CMD_CLOSE_ABORT_TRANSFORM_DLG);
        }
        else
        {
            ::PostCmd(CMD_OPEN_ABORT_TRANSFORM_DLG);
        }
    }
}

//--------------------------------------------------------------------------------------
DWORD CHero::GetBowShootRange()
{
    return 8;
}
//--------------------------------------------------------------------------------------
void CHero::ApplyLanding()
{
    /*
    	if (g_objHero.TestStatus(USERSTATUS_FLY))
    	{
    		CMsgAction msg;
    		if (msg.Create(g_objHero.GetID(), 0, 0, 0, actionLanding))
    			msg.Send();
    	}*/
}
//--------------------------------------------------------------------------------------
int	CHero::GetUserHelpInfoLineAmount()
{
    return g_obj3DRoleData.GetUserHelpInfoLineAmount(this->GetProfession(), this->GetLev(), this->GetSex(), 0, 0, 0, 0, 0);
}
//--------------------------------------------------------------------------------------
const char* CHero::GetUserHelpInfoLineByIndex(int nIndex)
{
    return g_obj3DRoleData.GetUserHelpInfoLineByIndex(this->GetProfession(), this->GetLev(), this->GetSex(), 0, 0, 0, 0, 0, nIndex);
}
//--------------------------------------------------------------------------------------
void CHero::ShowArrow(int nOffset)
{
    if (!this->HaveBow())
    {
        return;
    }
    const int nMinArrow = 50;
    int nArrowAmount = 0;
    CItem* pItem = this->GetEquipment(ITEMPOSITION_WEAPONL);
    if (pItem && pItem->GetTypeID() / 10000 == 105)
    {
        nArrowAmount = pItem->GetAmount();
    }
    if (nArrowAmount > nMinArrow)
    {
        return;
    }
    if (!m_bShowArrow)
    {
        const OBJID idStr = 100143;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));
    }
    m_bShowArrow = true;
    CAni* pAni = g_objGameDataSet.GetDataAni("ani/common.ani", "NoArrow");
    if (pAni)
    {
        CMyPos posShow;
        posShow.x = _SCR_WIDTH - pAni->GetSize(0).iWidth - nOffset - 10;
        posShow.y = 5;
        pAni->Show(0, posShow.x, posShow.y);
        char szAmount[64];
        sprintf(szAmount, "%d", nArrowAmount);
        CMyBitmap::ShowString(posShow.x - 1, posShow.y - 1, 0xff000000, szAmount);
        CMyBitmap::ShowString(posShow.x, posShow.y, 0xffffffff, szAmount);
    }
}
//--------------------------------------------------------------------------------------
void	CHero::SetSpecialTarget(OBJID idTarget)
{
    m_idSpecialTarget = idTarget;
    m_dwTimeSpecialTarget = ::TimeGet();
}
//--------------------------------------------------------------------------------------
OBJID	CHero::GetSpecialTarget()
{
    return m_idSpecialTarget;
}
//--------------------------------------------------------------------------------------
void	CHero::ProcessSpecialTarget()
{
    if (m_idSpecialTarget == ID_NONE)
    {
        return;
    }
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idSpecialTarget, true);
    if (!pPlayer)
    {
        if (::TimeGet() - m_dwTimeSpecialTarget > 40000)
        {
            m_dwTimeSpecialTarget = ID_NONE;
        }
    }
    else
    {
        if (pPlayer->IsDead())
        {
            m_dwTimeSpecialTarget = ID_NONE;
        }
        else
        {
            m_dwTimeSpecialTarget = ::TimeGet();
        }
    }
}
//--------------------------------------------------------------------------------------
void	CHero::AddPet(OBJID idPet)
{
    int nAmount = m_setPetInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        if (m_setPetInfo[i] == idPet)
        {
            return;
        }
    }
    m_setPetInfo.push_back(idPet);
    ::PostCmd(CMD_FLASH_PETSTATUS);
}
//--------------------------------------------------------------------------------------
void	CHero::DelPet(OBJID idPet)
{
    int nAmount = m_setPetInfo.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        if (m_setPetInfo[i] == idPet)
        {
            m_setPetInfo.erase(m_setPetInfo.begin() + i);
            ::PostCmd(CMD_FLASH_PETSTATUS);
            return;
        }
    }
}
//--------------------------------------------------------------------------------------
void	CHero::ClearPet()
{
    m_setPetInfo.clear();
}
//--------------------------------------------------------------------------------------
BOOL	CHero::CheckPet(OBJID idPet)
{
    int nAmount = m_setPetInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        OBJID idMyPet = m_setPetInfo[i];
        if (idMyPet == idPet)
        {
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------------
CPet*	CHero::GetPetByID (OBJID idPet)
{
    int nAmount = m_setPetInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        OBJID idMyPet = m_setPetInfo[i];
        if (idMyPet == idPet)
        {
            CPet* pPet = (CPet*)g_objPlayerSet.GetPlayer (idMyPet);
            if (pPet)
            {
                return pPet;
            }
        }
    }
    return NULL;
}
//--------------------------------------------------------------------------------------
CPet*   CHero::GetPetByIndex(int nIndex)
{
    int nAmount = m_setPetInfo.size();
    if (nIndex >= nAmount)
    {
        return NULL;
    }
    OBJID idMyPet = m_setPetInfo[nIndex];
    CPet* pPet = (CPet*)g_objPlayerSet.GetPlayer (idMyPet);
    if (pPet)
    {
        return pPet;
    }
    else
    {
        return NULL;
    }
}
//--------------------------------------------------------------------------------------
CPet*	CHero::GetPetByItemID (OBJID idItem)
{
    int nAmount = m_setPetInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        OBJID idMyPet = m_setPetInfo[i];
        CPet* pPet = (CPet*)g_objPlayerSet.GetPlayer (idMyPet);
        if (pPet && pPet->GetItemID() == idItem)
        {
            return pPet;
        }
    }
    return NULL;
}
//--------------------------------------------------------------------------------------
void CHero::SetActionMessage(const char* pszStr)
{
    if (!pszStr)
    {
        return;
    }
    if (strlen(pszStr) >= _MAX_STRING)
    {
        return;
    }
    strcpy(m_szActionMessageBox, pszStr);
}
//--------------------------------------------------------------------------------------
char* CHero::GetActionMessage()
{
    return m_szActionMessageBox;
}
//--------------------------------------------------------------------------------------
void CHero::SelectMedal(DWORD dwMedalSelect)
{
    CMyPos posHero;
    this->GetPos(posHero);
    CMsgAction msg;
    if (msg.Create(this->GetID(), posHero.x, posHero.y, this->GetDir(), actionSelectMedal, dwMedalSelect))
    {
        msg.Send();
        CPlayer::SetMedalSelect(dwMedalSelect);
    }
}
//--------------------------------------------------------------------------------------
void CHero::SelectHonorTitle(DWORD dwSelectedTitle)
{
    CMyPos posHero;
    this->GetPos(posHero);
    CMsgAction msg;
    if (msg.Create(this->GetID(), posHero.x, posHero.y, this->GetDir(), actionSelectHonorTitle, dwSelectedTitle))
    {
        msg.Send();
        CPlayer::SetMedalSelect(dwSelectedTitle);
    }
}
//--------------------------------------------------------------------------------------
DWORD	CHero::GetOriginalSoul()
{
    return m_PlayerInfo.dwSoul;
}
//--------------------------------------------------------------------------------------
DWORD	CHero::GetAdditionalSoul()
{
    DWORD dwSoul = 0;
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        if (m_pEquipment[i])
        {
            if ( i + 1 == ITEMPOSITION_SPRITE )
            {
                if ( m_pEquipment[i]->GetProfessionRequired() == _SPRITE_ADDITION_SOUL )
                {
                    dwSoul += m_pEquipment[i]->GetAmountLimit() ;
                }
            }
            else
            {
                dwSoul += m_pEquipment[i]->GetSoul();
            }
        }
    }
    return dwSoul;
}
//--------------------------------------------------------------------------------------
DWORD	CHero::GetSoul()
{
    DWORD dwSoul = m_PlayerInfo.dwSoul;
    /*	for (int i=0; i<_MAX_EQUIPMENT; i++)
    	{
    		if (m_pEquipment[i])
    		{
    			dwSoul += m_pEquipment[i]->GetSoul();
    		}
    	}*/
    return dwSoul;
}

//--------------------------------------------------------------------------------------
DWORD	CHero::GetMonsterNameColor(int nMonsterLevel)
{
    int nPlayerLevel = g_objHero.GetLev();
    //魔域要求 --XUELIANG CODE 2005.5.12
    int nDeltaLev = nPlayerLevel - nMonsterLevel;
    if (nDeltaLev >= 3)
    {
        return _COLOR_GREEN;
    }
    else if (nDeltaLev >= 0)
    {
        return _COLOR_WHITE;
    }
    else if (nDeltaLev >= -5)
    {
        return _COLOR_RED;
    }
    else
    {
        return _COLOR_BLACK;
    }
    //END
    /*	if (nPlayerLevel >= nMonsterLevel)
    	{
    		int nDeltaLevel = nPlayerLevel - nMonsterLevel;
    		if (nPlayerLevel <= 19)
    		{
    			if (nDeltaLevel < 3)
    				return _COLOR_WHITE;
    			else
    				return _COLOR_GREEN;
    		}
    		if (nPlayerLevel <= 49)
    		{
    			if (nDeltaLevel < 5)
    				return _COLOR_WHITE;
    			else
    				return _COLOR_GREEN;
    		}
    		if (nPlayerLevel <= 85)
    		{
    			if (nDeltaLevel < 4)
    				return _COLOR_WHITE;
    			else
    				return _COLOR_GREEN;
    		}
    		if (nPlayerLevel <= 112)
    		{
    			if (nDeltaLevel < 3)
    				return _COLOR_WHITE;
    			else
    				return _COLOR_GREEN;
    		}
    		if (nDeltaLevel < 2)
    			return _COLOR_WHITE;
    		else
    			return _COLOR_GREEN;
    	}
    	else
    	{
    		int nDeltaLevel = nMonsterLevel - nPlayerLevel;
    		if (nPlayerLevel <= 19)
    		{
    			if (nDeltaLevel < 3)
    				return _COLOR_WHITE;
    			if (nDeltaLevel < 6)
    				return _COLOR_RED;
    			return _COLOR_BLACK;
    		}
    		if (nPlayerLevel <= 49)
    		{
    			if (nDeltaLevel < 5)
    				return _COLOR_WHITE;
    			if (nDeltaLevel < 10)
    				return _COLOR_RED;
    			return _COLOR_BLACK;
    		}
    		if (nPlayerLevel <= 85)
    		{
    			if (nDeltaLevel < 4)
    				return _COLOR_WHITE;
    			if (nDeltaLevel < 8)
    				return _COLOR_RED;
    			return _COLOR_BLACK;
    		}
    		if (nPlayerLevel <= 112)
    		{
    			if (nDeltaLevel < 3)
    				return _COLOR_WHITE;
    			if (nDeltaLevel < 6)
    				return _COLOR_RED;
    			return _COLOR_BLACK;
    		}
    		if (nDeltaLevel < 2)
    			return _COLOR_WHITE;
    		if (nDeltaLevel < 4)
    			return _COLOR_RED;
    		return _COLOR_BLACK;
    	}*/
}

//--------------------------------------------------------------------------------------
void	CHero::ProcessLifeForeCast()
{
    if (m_dwLifeForeCast != this->GetData(CPlayer::_PLAYER_LIFE) && (::TimeGet() - m_dwLastLifeSync > 3000))
    {
        m_dwLifeForeCast = this->GetData(CPlayer::_PLAYER_LIFE);
    }
}
//--------------------------------------------------------------------------------------
void	CHero::SetData(DWORD dwDataType, DWORD dwData)
{
    if (dwDataType == CPlayer::_PLAYER_LIFE)
    {
        DWORD dwOldLife = this->GetData(CPlayer::_PLAYER_LIFE);
        if (dwOldLife < dwData)
        {
            m_dwLastLifeSync = ::TimeGet();
        }
        if (dwOldLife > dwData)
        {
            m_dwLifeForeCast -= (dwOldLife - dwData);
        }
    }
    CPlayer::SetData(dwDataType, dwData);
}
//--------------------------------------------------------------------------------------
void	CHero::EvolveEudemon(OBJID idPetItem, int nMode)// 幻兽进化, 幻兽ID
{
    CMsgItem msg;
    if (msg.Create(idPetItem, nMode, ITEMACT_EUDEMON_EVOLVE))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------------------
void    CHero::ReEvolveEudemon(OBJID idPetItem, int nMode)
{
    CMsgItem msg;
    if (msg.Create(idPetItem, nMode, ITEMACT_EUDEMON_EVOLVE2))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------------------
void	CHero::ReboneEudemon(OBJID idPetItem, OBJID idGem)// 幻兽复活，幻兽ID 宝石ID
{
    CMsgItem msg;
    if (msg.Create(idPetItem, idGem, ITEMACT_EUDEMON_REBORN))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------------------
void	CHero::EnhanceEudemon(OBJID idPetItem, OBJID idGem)// 幻兽强化，幻兽ID, 宝石ID
{
    CMsgItem msg;
    if (msg.Create(idPetItem, idGem, ITEMACT_EUDEMON_ENHANCE))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------------------
void	CHero::CallEudenmon(OBJID idPetItem)
{
    CMsgItem msg;
    if (msg.Create(idPetItem, ITEMACT_CALL_EUDEMON))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------------------
void    CHero::RenameEudenmon(OBJID idPetItem, char* petName)
{
    CMsgName msg;
    if (msg.Create(NAMEACT_CHANGE_EUDEMON_NAME, petName, idPetItem))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------------------
void	CHero::KillEudenmon(OBJID idPetItem)
{
    CMsgItem msg;
    if (msg.Create(idPetItem, ITEMACT_KILL_EUDEMON))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------------------
void    CHero::AttachEudemon (OBJID idPetItem)
{
    CMsgItem msg;
    if (msg.Create(idPetItem, ITEMACT_ATTACH_EUDEMON))
    {
        msg.Send ();
    }
}
//--------------------------------------------------------------------------------------
void    CHero::UnAttachEudemon (OBJID idPetItem)
{
    CMsgItem msg;
    if (msg.Create(idPetItem, ITEMACT_DETACH_EUDEMON))
    {
        msg.Send ();
    }
}
//--------------------------------------------------------------------------------------
void	CHero::CollectRes(OBJID idRes)
{
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idRes);
    if (this->IsMoving())
    {
        return;
    }
    OBJID idItem = this->GetRWeapon();
    if (idItem / 1000 != 470)
    {
        OBJID idStr = 100167;// 您未带挖矿工具
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));
        return;
    }
    m_Info.cmdProc.idTarget = idRes;
    if (pPlayer)
    {
        //pPlayer->SetAddSize(4);
        this->LockAttack(pPlayer);
    }
}
//--------------------------------------------------------------------------------------
void	CHero::LoadMusicInfo()
{
    m_setMusicInfo.setRangeMusicInfo.clear();
    m_setMusicInfo.strBgMusic = "";
    char pszIniFile[] = "ini/bgmusic.ini";
    char szTitle[256];
    sprintf(szTitle, "Map%d", g_objGameMap.GetIDDoc());
    char szStr[1024];
    if (::IniStrGet(pszIniFile, szTitle, "Bgmusic", szStr))
    {
        m_setMusicInfo.strBgMusic = szStr;
    }
    int nAmount = 0;
    if (::IniDataGet(pszIniFile, szTitle, "RangMusicAmount", nAmount))
    {
        for(int i = 0; i < nAmount; i ++)
        {
            RangeMusicInfo infoMusic;
            char szSubtitle[64];
            sprintf(szSubtitle, "x%d", i);
            ::IniDataGet(pszIniFile, szTitle, szSubtitle, infoMusic.x);
            sprintf(szSubtitle, "y%d", i);
            ::IniDataGet(pszIniFile, szTitle, szSubtitle, infoMusic.y);
            sprintf(szSubtitle, "cx%d", i);
            ::IniDataGet(pszIniFile, szTitle, szSubtitle, infoMusic.cx);
            sprintf(szSubtitle, "cy%d", i);
            ::IniDataGet(pszIniFile, szTitle, szSubtitle, infoMusic.cy);
            sprintf(szSubtitle, "RangMusic%d", i);
            ::IniStrGet(pszIniFile, szTitle, szSubtitle, szStr);
            infoMusic.strMusic = szStr;
            m_setMusicInfo.setRangeMusicInfo.push_back(infoMusic);
        }
    }
    m_dwLastChangeMusicTimer = 0;
}
//--------------------------------------------------------------------------------------
void	CHero::ProcessMusic()
{
    std::string strMusic = "";
    int nAmount = m_setMusicInfo.setRangeMusicInfo.size();
    int i = 0;
    for(; i < nAmount; i ++)
    {
        RangeMusicInfo infoMusic = m_setMusicInfo.setRangeMusicInfo[i];
        CMyPos posHero;
        this->GetPos(posHero);
        if (posHero.x > infoMusic.x && posHero.y > infoMusic.y
                && posHero.x < (infoMusic.x + infoMusic.cx)
                && posHero.y < (infoMusic.y + infoMusic.cy))
        {
            strMusic = infoMusic.strMusic;
            break;
        }
    }
    if (i >= nAmount)
    {
        strMusic = m_setMusicInfo.strBgMusic;
    }
    if (strMusic != m_strCurrentMusic)
    {
        if (m_dwLastChangeMusicTimer != 0)
        {
            m_dwLastChangeMusicTimer = ::TimeGet();
        }
        else
        {
            m_dwLastChangeMusicTimer = 1;
        }
        m_strCurrentMusic = strMusic;
    }
    if (::TimeGet() - m_dwLastChangeMusicTimer > 1000 * 4)
    {
        if (m_strCurrentMusicPlay != m_strCurrentMusic)
        {
            ::DXCloseAllMedia();
            m_strCurrentMusicPlay = m_strCurrentMusic;
            ::DXPlayMedia((char*)m_strCurrentMusic.c_str(), 99999999);
            ::DXSetMusicVolume(m_nMusicVolume);	//xueliang coding
        }
        else
        {
            DWORD dwRest = ::TimeGet() - m_dwLastChangeMusicTimer - 1000 * 4;
            if (dwRest > 2000)
            {
                return;
            }
            if (dwRest >= 1000)
            {
                ::DXSetMusicVolume(m_nMusicVolume);
            }
            else
            {
                ::DXSetMusicVolume(m_nMusicVolume * dwRest / 1000);
            }
        }
    }
    else if (::TimeGet() - m_dwLastChangeMusicTimer > 1000 * 3)
    {
        if (m_nMusicVolume <= 0)
        {
            return;
        }
        DWORD dwRest = ::TimeGet() - m_dwLastChangeMusicTimer - 1000 * 3;
        if (dwRest >= 1000)
        {
            ::DXSetMusicVolume(0);
        }
        else
        {
            ::DXSetMusicVolume(m_nMusicVolume*(1000 - dwRest) / 1000);
        }
    }
}
//--------------------------------------------------------------------------------------
void	CHero::SetMusicVolume(int nVolume)
{
    m_nMusicVolume = nVolume;
}
//--------------------------------------------------------------------------------------
void	CHero::SetIDMyTarget(OBJID id)
{
    m_idMyTarget = id;
    int nAmount = m_setPetInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CPet* pPet = (CPet*)g_objPlayerSet.GetPlayer(m_setPetInfo[i]);
        if (pPet)
        {
            pPet->SetHeroTarget(m_idMyTarget);
        }
    }
}
//--------------------------------------------------------------------------------------
