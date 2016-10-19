
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgPlayer.cpp: implementation of the CMsgPlayer class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "Player.h"
#include "GamePlayerSet.h"
#include "Hero.h"
#include "3droledata.H"
#include "3dGameMap.h"
#include "GameMsg.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgPlayer::CMsgPlayer()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgPlayer::~CMsgPlayer()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgPlayer::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_PLAYER != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgPlayer::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgPlayer, id:%u, pos(%u,%u)", m_pInfo->id, m_pInfo->usPosX, m_pInfo->usPosY);
#endif
#ifdef _DEBUG
    static int nIndex = 0;
    char szDebug[128] = "";
    sprintf(szDebug, "CMsgPlayer: %d\n", nIndex++);
    ::OutputDebugString(szDebug);
#endif
    if (m_pInfo->id == g_objHero.GetID())
    {
        //::LogMsg("Got a player with hero id! %u", m_pInfo->id);
        return;
    }
    if (m_pInfo->dwLookFace == 0)
    {
        return;
    }
    // disable other player in newbie map
    if (g_objGameMap.m_idDoc == 1010)
    {
        return;
    }
    char szName[_MAX_NAMESIZE]		= "";
    m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
    if (m_pInfo->id == 0)
    {
        return;
    }
    CPlayer* pPlayer	= g_objPlayerSet.GetPlayer(m_pInfo->id);
    if (!pPlayer)
    {
        pPlayer	= new CPlayer;
        MYASSERT (pPlayer);
        pPlayer->SetID(m_pInfo->id);
        pPlayer->SetName(szName);
        if (CRole::GetIDType(m_pInfo->id) == _PLAYER_ID_DYNNPC)
        {
            pPlayer->SetRoleType(_ROLE_STATUARY_NPC);
            pPlayer->Create(m_pInfo->dwLookFace % 1000, _ROLE_VIEW_STATUARY);
        }
        else
        {
            pPlayer->SetRoleType(_ROLE_PLAYER);
            pPlayer->Create(m_pInfo->dwLookFace % 1000);
            // wanted note
            if (0 == strcmp(g_objHero.m_szWanted, szName))
            {
                CMsgName msg;
                if (msg.Create(NAMEACT_QUERY_WANTED, ""))
                {
                    msg.Send();
                }
            }
            // police wanted note
            if (0 == strcmp(g_objHero.m_szPoliceWanted, szName))
            {
                CMsgName msg;
                if (msg.Create(NAMEACT_QUERY_POLICEWANTED, ""))
                {
                    msg.Send();
                }
            }
        }
        MYASSERT (g_objPlayerSet.AddPlayer(pPlayer));
    }
    pPlayer->SetName(szName);
    if (pPlayer->IsNpc())
    {
        pPlayer->SetLev(m_pInfo->usLevel);
        pPlayer->SetData(CPlayer::_PLAYER_LIFE, m_pInfo->usLife);
        //pPlayer->SetLife(m_pInfo->usLife);
        const CMonsterInfo* pInfo = g_obj3DRoleData.GetMonsterInfo(m_pInfo->dwMonsterType);
        if (pInfo)
        {
            pPlayer->SetMonsterTypeId(m_pInfo->dwMonsterType);
            pPlayer->SetMaxLife(pInfo->nMaxLife);
            pPlayer->SetAddSize(pInfo->nAddSize);
            pPlayer->SetZoomPercent(pInfo->nZoomPercent);
        }
    }
    else
    {
        pPlayer->SetLev(m_pInfo->usLevel);
        pPlayer->SetTutorLevel(m_pInfo->ucTutorLevel);
        //		pPlayer->SetMedalSelect(m_pInfo->ucSelectedMedal);
        //		pPlayer->SetSelectedHonorTitle(m_pInfo->ucSelectedHonorTitle);
        //pPlayer->SetNobilityRank((m_pInfo->dwLookFace/1000000)%10);
        pPlayer->SetNobilityRank(m_pInfo->ucNobilityRank);
    }
    if (pPlayer->IsPet())
    {
        const CMonsterInfo* pInfo = g_obj3DRoleData.GetMonsterInfo(m_pInfo->dwMonsterType);
        if (pInfo)
        {
            pPlayer->SetMonsterTypeId(m_pInfo->dwMonsterType);
            pPlayer->SetMaxLife(m_pInfo->usMaxLife);
            pPlayer->SetData(CPlayer::_PLAYER_LIFE, m_pInfo->usMonsterLife);
            pPlayer->SetAddSize(pInfo->nAddSize);
            pPlayer->SetZoomPercent(pInfo->nZoomPercent);
        }
        //		pPlayer->SetLife(m_pInfo->usMonsterLife);
    }
    __int64 i64Status = m_pInfo->dwStatus[1];
    i64Status = i64Status << 32;
    i64Status += m_pInfo->dwStatus[0];
    pPlayer->ReplaceStatus(i64Status);
    pPlayer->SetDir(m_pInfo->ucDir);
    pPlayer->m_pIRoleView->SetVariable(_VAR_DIR, m_pInfo->ucDir);
    if (m_pInfo->ucPose == _ACTION_SITDOWN)
    {
        m_pInfo->ucPose = _ACTION_SITDOWN_STATIC;
    }
    pPlayer->SetPose(m_pInfo->ucPose);
    //	int nPosX = m_pInfo->usPosX, nPosY = m_pInfo->usPosY;
    //	pPlayer->SetPos(nPosX, nPosY);
    // outlooks
    //pPlayer->SetAddSize((m_pInfo->dwLookFace/1000000)%10);
    pPlayer->SetFace((m_pInfo->dwLookFace / 10000) % 1000);
    pPlayer->SetSex((m_pInfo->dwLookFace / 1000) % 10);
    //-----------------------------------------------------------------
    // 消耗Cpu的操作
    if (pPlayer->IsPet() && pPlayer->TestStatus(USERSTATUS_GHOST))
    {
        pPlayer->SetLook(999);
    }
    else
    {
        pPlayer->SetLook(m_pInfo->dwLookFace % 1000);    // set default body here
    }
    pPlayer->Transform(m_pInfo->dwLookFace / 10000000);
    pPlayer->SetHair(m_pInfo->usHair);
    pPlayer->SetArmor(m_pInfo->dwArmorType);
    pPlayer->SetRWeapon(m_pInfo->dwWeaponRType);
    //pPlayer->SetLWeapon(m_pInfo->dwWeaponLType);
    if (pPlayer->IsMonster())
    {
        pPlayer->SetLife(m_pInfo->usMonsterLife);
        //		pPlayer->SetLev(m_pInfo->usLevel);
    }
    else
    {
        pPlayer->SetMount(m_pInfo->dwMountType);
    }
    /*	if (m_pInfo->usMantleType != 0)
    	{
    		DWORD dwMantle = m_pInfo->usMantleType + 999000;
    		pPlayer->SetMantle(dwMantle);
    	}
    	else
    	{
    		pPlayer->SetMantle(ID_NONE);
    	}*/
    //-----------------------------------------------------------------
    OBJID idSyndicate = m_pInfo->dwSynID_Rank & 0x00ffffff;
    DWORD dwRank = m_pInfo->dwSynID_Rank >> 24;
    if (pPlayer->IsPet())
    {
        pPlayer->SetPetOwnerId(m_pInfo->idOwner);
    }
    else
    {
        pPlayer->SetSyndicateID(idSyndicate);
        pPlayer->SetSyndicateRank(dwRank);
    }
    // set pose of player
    if (pPlayer->IsDead() && !pPlayer->TestStatus(USERSTATUS_GHOST))
    {
        CCommand cmd;
        cmd.iType		= _COMMAND_POSE;
        cmd.iStatus		= _CMDSTATUS_BEGIN;
        cmd.nData		= _ACTION_BODY0;
        cmd.nDir		= m_pInfo->ucDir;
        pPlayer->SetCommand(&cmd);
    }
    else if (!pPlayer->IsPet())
    {
        CCommand cmd;
        cmd.iType		= _COMMAND_EMOTION;
        cmd.iStatus		= _CMDSTATUS_BEGIN;
        cmd.nData		= m_pInfo->ucPose;
        cmd.nDir		= m_pInfo->ucDir;
        pPlayer->SetCommand(&cmd);
    }
    // set pos
    int nPosX = m_pInfo->usPosX, nPosY = m_pInfo->usPosY;
    pPlayer->SetPos(nPosX, nPosY);
    /*	if (m_pInfo->GetRoleType() == _ROLE_BOOTH_NPC)
    	{
    		CMsgAction msgAction;
    		msgAction.Create()
    		g_objHero.queryc
    	}*/
    if (CRole::GetIDType(m_pInfo->id) == _PLAYER_ID_DYNNPC)
    {
        pPlayer->m_pIRoleView->SetVariable(_VAR_FRAME, m_pInfo->usStatuaryFrame);
        if (dwRank == RANK_LEADER)
        {
            pPlayer->SetMaxLife(g_obj3DRoleData.GetLeaderMaxLife());
            pPlayer->SetZoomPercent(g_obj3DRoleData.GetLeaderScale());
            pPlayer->SetData(CPlayer::_PLAYER_LIFE, m_pInfo->usStatuaryLife * 100);
            //pPlayer->SetLife(m_pInfo->usLife*100);
        }
        else if (dwRank == RANK_ASSISTANT)
        {
            pPlayer->SetMaxLife(g_obj3DRoleData.GetAssistantMaxLife());
            pPlayer->SetZoomPercent(g_obj3DRoleData.GetAssistantScale());
            pPlayer->SetData(CPlayer::_PLAYER_LIFE, m_pInfo->usStatuaryLife * 100);
            //pPlayer->SetLife(m_pInfo->usLife*100);
        }
        else if (dwRank == RANK_SUBLEADER)
        {
            pPlayer->SetMaxLife(g_obj3DRoleData.GetSubLeaderMaxLife());
            pPlayer->SetZoomPercent(g_obj3DRoleData.GetSubLeaderScale());
            pPlayer->SetData(CPlayer::_PLAYER_LIFE, m_pInfo->usStatuaryLife * 100);
            //pPlayer->SetLife(m_pInfo->usLife*100);
        }
    }
    // Set sprite effect
    // pPlayer->SetSpriteEffect( m_pInfo->ucSprite ) ;
    pPlayer->SetSpeedRate(m_pInfo->ucActionSpeed);
    pPlayer->SetMercenaryLevel(m_pInfo->ucMercenaryLevel);
}
