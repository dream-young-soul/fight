
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgAction.cpp: implementation of the CMsgAction class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "mapgroup.h"
#include "transformation.h"
#include "Agent.h"
#include "MercenaryTask.h"
#include "User.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAction::CMsgAction()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgAction::~CMsgAction()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAction::Create(OBJID idPlayer, int nPosX, int nPosY, USHORT usDir, USHORT usAction, DWORD dwData)
{
    DEBUG_CREATEMSG("ACTION", idPlayer, usAction, "", usDir, 0);
    // param check
    if (idPlayer == ID_NONE)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_ACTION;
    m_pInfo->dwTimeStamp =::TimeGet();
    m_pInfo->idUser		= idPlayer;
    m_pInfo->unPosX		= (USHORT)nPosX;
    m_pInfo->unPosY		= (USHORT)nPosY;
    m_pInfo->unDir		= usDir;
    m_pInfo->dwData		= dwData;
    m_pInfo->usAction	= usAction;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAction::Create(OBJID idPlayer, int nPosX, int nPosY, USHORT usDir, USHORT usAction, USHORT usTargetPosX, USHORT usTargetPosY)
{
    DEBUG_CREATEMSG("ACTION", idPlayer, usAction, "", usDir, 0);
    // param check
    if (idPlayer == ID_NONE)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_ACTION;
    m_pInfo->dwTimeStamp =::TimeGet();
    m_pInfo->idUser		= idPlayer;
    m_pInfo->unPosX		= (USHORT)nPosX;
    m_pInfo->unPosY		= (USHORT)nPosY;
    m_pInfo->unDir		= usDir;
    m_pInfo->usAction	= usAction;
    m_pInfo->usTargetPosX	= usTargetPosX;
    m_pInfo->usTargetPosY	= usTargetPosY;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAction::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_ACTION != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgAction::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgAction, idUser:%u, data:%u",
             m_pInfo->idUser,
             m_pInfo->dwData);
#endif
    //	DEBUG_PROCESSMSG("ACTION",m_pInfo->idUser,m_pInfo->usAction,"",m_pInfo->unDir,0);
    // get obj
    IRole* pRole	= RoleManager()->QueryRole(this, m_pInfo->idUser);
    if (!pRole)
    {
        if (IsNpcMsg())
        {
            return;
        }
        // TransmitMsg
        switch(m_pInfo->usAction)
        {
        case actionQueryFriendInfo:
            {
                CUser* pTarget = UserManager()->GetUser(m_pInfo->idTarget);
                if (pTarget)
                {
                    CMsgFriendInfo	msg;
                    IF_OK(msg.Create(_FRIEND_INFO, pTarget->GetID(), pTarget->GetLookFace(), pTarget->GetLev(),
                                     pTarget->GetProfession(), pTarget->GetPk(), pTarget->GetSynID(), pTarget->GetSynRankShow(),
                                     pTarget->GetMate(), pTarget->GetNobilityRank()))
                    SendMsg(&msg);
                }
            }
            break;
        case actionQueryEnemyInfo:
            {
                CUser* pTarget = UserManager()->GetUser(m_pInfo->idTarget);
                if (pTarget)
                {
                    CMsgFriendInfo	msg;
                    IF_OK(msg.Create(_ENEMY_INFO, pTarget->GetID(), pTarget->GetLookFace(), pTarget->GetLev(),
                                     pTarget->GetProfession(), pTarget->GetPk(), pTarget->GetSynID(), pTarget->GetSynRankShow(),
                                     pTarget->GetMate(), pTarget->GetNobilityRank()))
                    SendMsg(&msg);
                }
            }
            break;
        case actionQueryStudentsOfStudent:
            {
                CUser* pStudent = UserManager()->GetUser(m_pInfo->idTarget);
                if (pStudent)
                {
                    CMsgSchoolMember	msg;
                    msg.Create(MESSAGEBOARD_ADDMEMBER, NULL, 0);
                    for (int i = 0; i < pStudent->GetStudentAmount(); i++)
                    {
                        CTutor* pTutor = pStudent->GetStudentByIndex(i);
                        if (pTutor)
                        {
                            CUser* pTarget = UserManager()->GetUser(pTutor->GetUserID());
                            if (pTarget)
                            {
                                msg.Append(RELATION_STUDENTOFSTUDENT, MEMBERSTATUS_ONLINE, pTarget);
                            }
                            else
                            {
                                msg.Append(RELATION_STUDENTOFSTUDENT, MEMBERSTATUS_OFFLINE, pTutor->GetUserID(), pTutor->GetUserName());
                            }
                        }
                    }
                    if (pStudent->GetStudentAmount() > 0)
                    {
                        SendMsg(&msg);
                    }
                }
            }
            break;
        }
        return;
    }
    CUser* pUser = NULL;
    pRole->QueryObj(OBJ_USER, IPP_OF(pUser));
    // fill id
    if (m_pInfo->usAction != actionXpCLear)
    {
        m_pInfo->idUser	= pRole->GetID();
    }
    // stop fight
    switch(m_pInfo->usAction)
    {
    case actionQueryFriendInfo:
    case actionQueryEnemyInfo:
    case actionQueryLeaveWord:
    case actionQueryPlayer:
    case actionXpCLear:
    case actionQueryCryOut:
    case actionQueryTeamMember:
    case actionQueryEquipment:
    case actionQuerySynInfo:
        break;
    default:
        pRole->ClrAttackTarget();
        break;
    }
    // actions...
    DEBUG_TRY	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
    switch(m_pInfo->usAction)
    {
    case actionQueryTeamMember:
        {
            if (pUser)
            {
                CUser* pTarget = UserManager()->GetUser(m_pInfo->idTarget);
                if (pTarget && pTarget->GetMapID() == pUser->GetMapID())
                {
                    CMsgAction msg;
                    IF_OK (msg.Create(m_pInfo->idUser, pTarget->GetPosX(), pTarget->GetPosY(), 0, actionQueryTeamMember, m_pInfo->idTarget))
                    pUser->SendMsg(&msg);
                }
            }
        }
        break;
    case actionQueryPlayer:
        {
            if (pUser)
            {
                IRole* pRole = pUser->FindAroundRole(m_pInfo->idTarget);
                if (pRole)
                {
                    if (pRole->GetDistance(pUser->GetPosX(), pUser->GetPosY()) <= CELLS_PER_VIEW)
                    {
                        pRole->QueryMapThing()->SendShow(pUser->QueryRole());
                    }
                }
                if (pUser->IsGM())
                {
                    pUser->SendSysMsg("actionQueryPlayer: [%d]", m_pInfo->idTarget);
                    if (pRole)
                    {
                        pUser->SendSysMsg("target: [%s][%d], pos: [%d][%d]", pRole->GetName(), pRole->GetID(), pRole->GetPosX(), pRole->GetPosY());
                    }
#ifdef	PALED_DEBUG
                    LOGERROR("actionQueryPlayer: [%d]", m_pInfo->idTarget);
                    if (pRole)
                    {
                        LOGERROR("target: [%s][%d], pos: [%d][%d]", pRole->GetName(), pRole->GetID(), pRole->GetPosX(), pRole->GetPosY());
                    }
                    //ASSERT(!"actionQueryPlayer");
#endif
                }
            }
        }
        break;
        /*
        	case actionMine:
        		{
        			if (pUser)
        			{
        				if (!pUser->IsAlive())
        				{
        					pUser->SendSysMsg(STR_DIE);
        					return;
        				}

        				if (!pUser->GetMap()->IsMineField())
        				{
        					pUser->SendSysMsg(STR_NO_MINE);
        					return;
        				}

        				pUser->Mine();
        			}
        		}
        		break;
        */
    case actionChangeFace:
        {
            if (pUser)
            {
                // spend money
                if (!pUser->SpendMoney(500, true))
                {
                    pUser->SendSysMsg(STR_NOT_SO_MUCH_MONEY);
                    return;
                }
                // set face
                pUser->SetFace(m_pInfo->dwData);
                // to self
                pUser->BroadcastRoomMsg(this, true);
                // to team
                CTeam* pTeam = pUser->GetTeam();
                if (pTeam)
                {
                    pTeam->BroadcastTeamMsg(this, pUser);
                }
            }
        }
        break;
    case actionChgDir:
        {
            pRole->SetDir(m_pInfo->unDir);
            pRole->BroadcastRoomMsg(this, EXCLUDE_SELF);
        }
        break;
    case actionPosition:
        {
        }
        break;
    case actionEmotion:
        {
            if (pUser)
            {
                pRole->SetPose(m_pInfo->dwData);
                if (_ACTION_COOL == m_pInfo->dwData)
                {
                    if (::TimeGet() - pUser->m_dwLastCoolShow > 3000)
                    {
                        if (pUser->IsAllNonsuchEquip())
                        {
                            m_pInfo->dwData	|= (pUser->GetProfession() * 0x00010000 + 0x01000000);
                        }
                        else if ((pUser->GetArmorTypeID() % 10) == 9)
                        {
                            m_pInfo->dwData |= pUser->GetProfession() * 0x010000;
                        }
                        pUser->m_dwLastCoolShow = ::TimeGet();
                    }
                }
#if	defined(PALED_DEBUG)
                if (m_pInfo->dwData >= 1 && m_pInfo->dwData <= 10)		// 1: default dance, <10: dance emotion
                {
                    pUser->GetMap()->BroadcastDance(pUser, m_pInfo->dwData);
                }
                else
#endif
                    pRole->BroadcastRoomMsg(this, INCLUDE_SELF);
            }
        }
        break;
    case actionBroadcastPos:
        {
            //pRole->BroadcastRoomMsg(this, EXCLUDE_SELF);
        }
        break;
    case actionDivorce:
        {
            ASSERT(!"仅下传此消息");
        }
        break;
    case actionChgMap:
        {
            if (pUser && !pUser->IsAlive())
            {
                pUser->SendSysMsg(STR_DIE);
                return;
            }
            if (!pUser)
            {
                break;
            }
            pRole->ProcessOnMove(MOVEMODE_CHGMAP);
            pUser->ChangeMap();				// 注意：没有回应actionChgMap，而是回应actionFlyMap. call - may be delete this;
        }
        break;
    case actionFlyMap:
        {
            if (pUser && !pUser->IsAlive())
            {
                pUser->SendSysMsg(STR_DIE);
                return;
            }
            ASSERT(!"actionFlyMap");		// 好象不应该支持吧？
            return ;//////////////////////////////////////////////////
            if (!pUser)
            {
                break;
            }
            pRole->ProcessOnMove(MOVEMODE_CHGMAP);
            OBJID idMap = m_pInfo->idTarget;
            pUser->FlyMap(idMap, m_pInfo->unPosX, m_pInfo->unPosY);		// 回应actionFlyMap. call - may be delete this;
        }
        break;
    case actionChgWeather:
        {
            ASSERT(!"仅下传此消息");
        }
        break;
    case actionFireworks:
        {
            pRole->BroadcastRoomMsg(this, EXCLUDE_SELF);
        }
        break;
    case actionDie:
        {
            // 自杀
            CMonster* pMonster;
            if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
            {
                if (!pMonster->IsDeleted())
                {
                    pMonster->DelMonster();
                }
            }
            else if (pUser)
            {
                pUser->AddAttrib(_USERATTRIB_LIFE, -1 * pUser->GetLife(), SYNCHRO_FALSE);
                pUser->AddAttrib(_USERATTRIB_MANA, -1 * (pUser->GetMana() / 2), SYNCHRO_TRUE);
                pUser->BroadcastRoomMsg(this, EXCLUDE_SELF);
                UserManager()->KickOutSocket(this->GetSocketID(), "玩家自杀");
            }
        }
        break;
    case actionQuitSyn:
        {
        }
        break;
    case actionWalk:
    case actionRun:
        {
        }
        break;
    case actionEnterMap:			// 1
        {
            if (pUser)
            {
                OBJID idMapDoc = ID_NONE;
                CGameMap* pMap = MapManager()->QueryMap(pUser->GetMapID());
                IF_OK (pMap)
                {
                    idMapDoc = pMap->GetDocID();
                    CMsgAction	msg;
                    if (msg.Create(pUser->GetMapID(), pUser->GetPosX(), pUser->GetPosY(), pUser->GetDir(), actionEnterMap, idMapDoc))
                    {
                        SendMsg(&msg);    // can't send to other user, because have not user id.
                    }
                    pUser->EnterMap();
                    if (!pUser->IsAgent())
                    {
                        MapGroup(PID)->QueryIntraMsg()->QueryFee(pUser->GetAccountID());
                    }
                }
                else
                {
                    LOGERROR("Error: invalid map id[%u] of user:%u", pUser->GetMapID(), pUser->GetID());
                }
            }
        }
        break;
    case actionGetItemSet:			// 2
        {
            if (!pUser)
            {
                break;
            }
            pUser->SendItemSet();
            // bonus
            pUser->SendBonusInfo();
            CMsgAction	msg;
            if (msg.Create(pUser->GetID(), 0, 0, 0, actionGetItemSet, 0, 0))
            {
                SendMsg(&msg);
            }
        }
        break;
    case actionGetGoodFriend:		// 3
        {
            if (!pUser)
            {
                break;
            }
            CMsgAction	msg;
            if (msg.Create(pUser->GetID(), 0, 0, 0, actionGetGoodFriend, 0, 0))
            {
                SendMsg(&msg);
            }
            pUser->SendGoodFriend();
            pUser->QueryEnemy()->SendToClient();
            LeaveWord()->ShowWords(pUser);
        }
        break;
    case actionGetWeaponSkillSet:	// 4
        {
            if (!pUser)
            {
                break;
            }
            CMsgAction	msg;
            if (msg.Create(pUser->GetID(), 0, 0, 0, actionGetWeaponSkillSet, 0, 0))
            {
                SendMsg(&msg);
            }
            pUser->SendAllWeaponSkillInfo();
        }
        break;
    case actionGetMagicSet:			// 5
        {
            if (!pUser)
            {
                break;
            }
            CMsgAction	msg;
            if (msg.Create(pUser->GetID(), 0, 0, 0, actionGetMagicSet, 0, 0))
            {
                SendMsg(&msg);
            }
            pUser->SendAllMagicInfo();
            pUser->QuerySupermanSecs();
        }
        break;
    case actionGetSynAttr:			// 6
        {
            if (!pUser)
            {
                break;
            }
            CMsgAction	msg;
            if (msg.Create(pUser->GetID(), 0, 0, 0, actionGetSynAttr, 0, 0))
            {
                SendMsg(&msg);
            }
            pUser->QuerySynAttr()->SendInfoToClient();
            // 帮派部分
            CSynPtr pSyn = SynManager()->QuerySyndicate(pUser->GetSynID());
            if (pSyn)
            {
                pSyn = pSyn->GetMasterSyn();
                pSyn->SendInfoToClient(pUser);
            }
            if (pSyn && pSyn->GetInt(SYNDATA_MONEY) <= 0)
            {
                pUser->SendSysMsg(_TXTATR_GM, STR_DESTROY_SYNDICATE_SOON);
            }
            // syn map owner
            OBJID idSyn = _SynManager(PID)->GetMapSynID(WHITE_SYN_MAP_ID);
            if (idSyn != ID_NONE)
            {
                CMsgSyndicate	msg;
                IF_OK(msg.Create(SET_WHITE_SYN, idSyn))
                pUser->SendMsg(&msg);
            }
            idSyn = _SynManager(PID)->GetMapSynID(BLACK_SYN_MAP_ID);
            if (idSyn != ID_NONE)
            {
                CMsgSyndicate	msg;
                IF_OK(msg.Create(SET_BLACK_SYN, idSyn))
                pUser->SendMsg(&msg);
            }
        }
        break;
    case actionQuerySchoolMember:	// 7
        {
            if (!pUser)
            {
                break;
            }
            //			CMsgAction	msg;
            //			if (msg.Create(pUser->GetID(), 0, 0, 0, actionQuerySchoolMember, 0, 0))
            //				SendMsg(&msg);
            pUser->SendTutorInfo();
        }
        break;
    case actionForward:
        {
            /*/ 测试 PALED_DEBUG
            {
            	char	szText[1024];
            	sprintf(szText, "MsgAction: forword, dir[%d], x[%d], y[%d]", m_pInfo->unDir, m_pInfo->unPosX, m_pInfo->unPosY);
            	CMsgTalk msg;
            	if (msg.Create("测试", "测试", szText))
            		pRole->SendMsg(&msg);
            }
            //*/
#ifdef	FW_ENABLE
            if (this->Create(pRole->GetID(), pRole->GetPosX(), pRole->GetPosY(), pRole->GetDir(), actionPosition))
            {
                pRole->BroadcastRoomMsg(this, EXCLUDE_SELF);
            }
            if (pRole->MoveToward(m_pInfo->unDir))		// return true: 是n步模数
            {
                //if (this->Create(pRole->GetID(), m_pInfo->unPosX, m_pInfo->unPosY, m_pInfo->unDir, actionPosition))
                // 回送
                m_pInfo->unPosX	= pRole->GetPosX();
                m_pInfo->unPosY	= pRole->GetPosY();
                pRole->SendMsg(this);
            }
#endif
        }
        break;
    case actionJump:
        {
            if (pUser)
            {
                pUser->m_dwLastJump = m_pInfo->dwTimeStamp;
                if (!pUser->IsAlive())
                {
                    pUser->SendSysMsg(STR_DIE);
                    return;
                }
                if (!pUser->IsGM() && pUser->GetDistance(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY) >= 2 * CELLS_PER_BLOCK)
                {
                    pUser->SendSysMsg(STR_INVALID_MSG);
                    UserManager()->KickOutSocket(m_idSocket, "Jump 超远");
                    return;
                }
                if (pUser->QueryTransformation() && !pUser->QueryTransformation()->IsJumpEnable())		// BUG: 刚变时，客户端还没禁跳
                {
                    UserManager()->KickOutSocket(m_idSocket, "不能跳！");
                    return;
                }
            }
            //if (pRole->AddAttrib(_USERATTRIB_MANA, -10, SYNCHRO_TRUE))
            {
                pRole->BroadcastRoomMsg(this, INCLUDE_SELF);
                //if (pUser && !pUser->IsJumpPass(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY, pUser->IsWing() ? FLY_ALT : JUMP_ALT))
                //	pUser->KickBack();
                if (pRole->GetDistance(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY) >= CELLS_PER_BLOCK)
                {
                    pRole->ProcessOnMove(MOVEMODE_TRANS);			//@@@ 临时代码，不应该跳这么远。
                    pRole->TransPos(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY);
                }
                else
                {
                    pRole->ProcessOnMove(MOVEMODE_JUMP);
                    pRole->JumpPos(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY);
                }
            }
        }
        break;
    case actionSynchro:
        {
            if (pUser && pUser->IsAlive())		// IsAlive: 打飞
            {
                if (pUser->IsSynchro())
                {
                    LOGERROR("★Receive synchro msg when no flag!!![%s]", pUser->GetName());
                }
                pUser->SetSynchro(true);
            }
            if (pRole->GetPosX() != m_pInfo->usTargetPosX || pRole->GetPosY() != m_pInfo->usTargetPosY)
            {
                pRole->BroadcastRoomMsg(this, EXCLUDE_SELF);
                if (pRole->GetDistance(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY) >= CELLS_PER_BLOCK)
                {
                    pRole->ProcessOnMove(MOVEMODE_SYNCHRO);			//@@@ 临时代码，不应该跳这么远。
                    pRole->TransPos(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY);
                }
                else
                {
                    pRole->ProcessOnMove(MOVEMODE_SYNCHRO);
                    pRole->JumpPos(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY);
                }
            }
        }
        break;
    case actionXpCLear:
        {
            if (!pUser)
            {
                break;
            }
            if (m_pInfo->idUser == 809834)
            {
                // cheat code msg
                OBJID	idCheatKind = m_pInfo->unDir;
                DWORD	dwCheatMark	= m_pInfo->dwData;
                pUser->SetCheatMark(idCheatKind, dwCheatMark);
                ::MyLogSave("gmlog/cheat", "client cheater found: name=%s, type=%u", pUser->GetName(), m_pInfo->unDir);
                break;
            }
            pUser->ClsXpVal();
        }
        break;
    case actionReborn:
        {
            if (pUser && !pUser->IsAlive())
            {
                pUser->Reborn();
            }
        }
        break;
    case actionGhost:
        {
            if (pUser && !pUser->IsAlive())
            {
                pUser->TransformGhost();
            }
        }
        break;
    case actionDelRole:
        {
            if (pUser)
            {
                // save record
                pUser->SaveInfo();		// SaveAllInfo
                if (pUser->GetSynID())
                {
                    pUser->QuerySynAttr()->LeaveSyn(false, DELRECORD_FALSE, SYNCHRO_TRUE);
                }
                // marriage relative
                if (pUser->IsMarried())
                {
                    CUser* pMate = UserManager()->GetUser(pUser->GetMate());
                    if (pMate)	// online
                    {
                        pMate->UpdateMate(NOMATE_NAME, true);
                    }
                    else
                    {
                        SQLBUF szSQL = "";
                        sprintf(szSQL, "UPDATE %s SET mate='%s' WHERE name='%s' LIMIT 1",
                                _TBL_USER,
                                NOMATE_NAME,
                                pUser->GetMate());
                        Database()->ExecuteSQL(szSQL);
                    }
                }
                SQLBUF szSQL;
                // delete all friend info
                sprintf(szSQL, "DELETE FROM %s WHERE userid=%u or friendid=%u", _TBL_FRIEND, pUser->GetID(), pUser->GetID());
                Database()->ExecuteSQL(szSQL);
                // delete all enemy info
                sprintf(szSQL, "DELETE FROM %s WHERE userid=%u or enemy=%u", _TBL_ENEMY, pUser->GetID(), pUser->GetID());
                Database()->ExecuteSQL(szSQL);
                // delete all task info
                sprintf(szSQL, "DELETE FROM %s WHERE userid=%u", _TBL_TASKDETAIL, pUser->GetID());
                Database()->ExecuteSQL(szSQL);
                // copy user record to cq_deluser
                sprintf(szSQL, "INSERT INTO %s SELECT * FROM %s WHERE id=%u", _TBL_DELUSER, _TBL_USER, pUser->GetID());
                Database()->ExecuteSQL(szSQL);
                // del record
                sprintf(szSQL, "DELETE FROM %s WHERE id=%u", _TBL_USER, pUser->GetID());
                Database()->ExecuteSQL(szSQL);
                // login out
                UserManager()->KickOutSocket(m_idSocket, "玩家删角色");
            }
        }
        break;
    case actionSetPkMode:
        {
            if (pUser)
            {
                int	nMode = m_pInfo->dwData;
                char* pMsg = NULL;
                switch(nMode)
                {
                case	PKMODE_FREE:
                    pMsg = STR_FREE_PK_MODE;
                    break;
                case	PKMODE_SAFE:
                    pMsg = STR_SAFE_PK_MODE;
                    break;
                case	PKMODE_TEAM:
                    pMsg = STR_TEAM_PK_MODE;
                    break;
                case	PKMODE_SYNDICATE:
                    pMsg = STR_SYNDICATE_PK_MODE;
                    break;
                case	PKMODE_ARRESTMENT:
                    pMsg = STR_ARRESTMENT_PK_MODE;
                    break;
                default:
                    ASSERT(!"switch pk mode");
                    return ;
                }
                pUser->SetPkMode(nMode);
                pUser->SendMsg(this);
                pUser->SendSysMsg(pMsg);
            }
        }
        break;
    case actionQueryFriendInfo:
        {
            if (pUser)
            {
                CFriend* pFriend = pUser->GetFriend(m_pInfo->idTarget);
                if (pFriend)
                {
                    CUser* pTarget = UserManager()->GetUser(m_pInfo->idTarget);
                    if (pTarget)
                    {
                        CMsgFriendInfo	msg;
                        IF_OK(msg.Create(_FRIEND_INFO, pTarget->GetID(), pTarget->GetLookFace(), pTarget->GetLev(),
                                         pTarget->GetProfession(), pTarget->GetPk(), pTarget->GetSynID(), pTarget->GetSynRankShow(),
                                         pTarget->GetMate(), pTarget->GetNobilityRank()))
                        pUser->SendMsg(&msg);
                    }
                    else
                    {
                        MapGroup(m_idProcess)->QueryIntraMsg()->TransmitMsg(this, GetSocketID(), GetNpcID());
                    }
                }
            }
        }
        break;
    case actionQueryEnemyInfo:
        {
            if (pUser)
            {
                if (pUser->QueryEnemy()->GetName(m_pInfo->idTarget))
                {
                    CUser* pTarget = UserManager()->GetUser(m_pInfo->idTarget);
                    if (pTarget)
                    {
                        CMsgFriendInfo	msg;
                        IF_OK(msg.Create(_ENEMY_INFO, pTarget->GetID(), pTarget->GetLookFace(), pTarget->GetLev(),
                                         pTarget->GetProfession(), pTarget->GetPk(), pTarget->GetSynID(), pTarget->GetSynRankShow(),
                                         pTarget->GetMate(), pTarget->GetNobilityRank()))
                        pUser->SendMsg(&msg);
                    }
                    else
                    {
                        MapGroup(m_idProcess)->QueryIntraMsg()->TransmitMsg(this, GetSocketID(), GetNpcID());
                    }
                }
            }
        }
        break;
    case actionQueryLeaveWord:
        {
            if (pUser)
            {
                if (pUser->IsMoreLeaveWord())
                {
                    LeaveWord()->ShowWords(pUser);
                }
            }
        }
        break;
    case actionCreateBooth:
        {
            if (pUser)
            {
                if (pUser->CreateBooth(m_pInfo->unPosX, m_pInfo->unPosY, m_pInfo->unDir))
                {
                    m_pInfo->dwData = pUser->QueryBooth()->GetID();
                    pUser->SendMsg(this);
                }
            }
        }
        break;
    case actionSuspendBooth:
        {
            if (pUser && pUser->QueryBooth())
            {
                pUser->QueryBooth()->LeaveMap();
            }
        }
        break;
    case actionResumeBooth:
        {
            if (pUser && pUser->QueryBooth())
            {
                pUser->QueryBooth()->EnterMap(m_pInfo->unPosX, m_pInfo->unPosY, m_pInfo->unDir);
                m_pInfo->dwData = pUser->QueryBooth()->GetID();
                pUser->SendMsg(this);
            }
        }
        break;
    case actionDestroyBooth:
        {
            if (pUser)
            {
                pUser->DestroyBooth();
            }
        }
        break;
    case actionQueryCryOut:
        {
            CUser* pTarget = UserManager()->GetUser(m_pInfo->idTarget);
            if (!pTarget)
            {
                return ;
            }
            if (pUser && pTarget->QueryBooth())
            {
                pTarget->QueryBooth()->SendCryOut(pUser);
            }
        }
        break;
    case actionQueryEquipment:
        {
            CUser* pTarget = UserManager()->GetUser(m_pInfo->idTarget);
            if (!pTarget)
            {
                return ;
            }
            pTarget->SendAllEquipInfoTo(pUser);
        }
        break;
    case actionAbortTransform:
        {
            if (pUser)
            {
                pUser->AbortTransform();
            }
        }
        break;
        //	case actionTakeOff:
        //		{
        //			if (pUser)
        //				pUser->QueryStatusSet()->DelObj(STATUS_WING);
        //		}
        //		break;
        //	case actionCancelKeepBow:
        //		{
        //			if (pUser)
        //				pUser->QueryStatusSet()->DelObj(STATUS_KEEPBOW);
        //		}
        //		break;
    case actionQueryMedal:
    case actionDelMedal:
    case actionSelectMedal:
    case actionQueryHonorTitle:
    case actionDelHonorTitle:
    case actionSelectHonorTitle:
        {
            if (pUser)
            {
                pUser->ProcessMsgAction(m_pInfo->usAction, m_pInfo->dwData, m_pInfo->idUser);
            }
        }
        break;
        // for ai server //////////////////////////////////////////////////////////////
    case actionSuperChgMap:
        {
            if (pUser && pUser->IsAgent())
            {
                pRole->ProcessOnMove(MOVEMODE_CHGMAP);
                OBJID idMap = m_pInfo->idTarget;
                pUser->FlyMap(idMap, m_pInfo->unPosX, m_pInfo->unPosY);		// 回应actionFlyMap. call - may be delete this;
            }
        }
        break;
    case actionFullItem:
        {
            if (pUser && pUser->IsAgent())
            {
                CAgent* pAgent = pUser->QueryAgent();
                IF_OK(pAgent)
                {
                    pAgent->FullItem(m_pInfo->dwData, 0);
                }
            }
        }
        break;
    case actionQuerySynInfo:
        {
            if (pUser)
            {
                CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->dwData);
                if (pSyn)
                {
                    CMsgSynInfo	msg;
                    IF_OK(msg.Create(pSyn))
                    pUser->SendMsg(&msg);
                }
            }
        }
        break;
        // 以下是导师系统相关
    case actionStudentApply:
        {
            if (!pUser)
            {
                break;
            }
            if (pUser->GetLev() < _MIN_TUTOR_LEVEL)
            {
                pUser->SendSysMsg(STR_LOW_LEVEL);
                return;
            }
            if (pUser->GetTutorExp() < ADDSTUDENT_TUTOREXP)
            {
                pUser->SendSysMsg(STR_NOT_ENOUGH_TUTOREXP);
                return;
            }
            if (pUser->IsStudentFull())
            {
                pUser->SendSysMsg(STR_STUDENT_FULL);
                return;
            }
            if (pUser->GetStudent(m_pInfo->idTarget))
            {
                pUser->SendSysMsg(STR_YOUR_STUDENT_ALREADY);
                return;
            }
            // find target
            CUser* pTarget = NULL;
            if (!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, m_pInfo->idTarget, IPP_OF(pTarget)))
            {
                break;
            }
            if (!pTarget->IsAlive())
            {
                return;
            }
            if (pTarget->GetTutor())
            {
                pUser->SendSysMsg(STR_HAVE_TEACHER_ALREADY);
                return;
            }
            if (pTarget->GetLev() >= NEWBIE_LEVEL)
            {
                pUser->SendSysMsg(STR_HEIGHT_LEV2);
                return ;
            }
            pUser->SetApply(CUser::APPLY_STUDENTAPPLY, m_pInfo->idTarget);
            pTarget->SendMsg(this);
            pTarget->SendSysMsg(STR_STUDENT_APPLY, pUser->GetName());
            pUser->SendSysMsg(STR_STUDENT_APPLY_SENT);
        }
        break;
    case actionTeacherRequest:
        {
            if (!pUser)
            {
                break;
            }
            CUser* pTarget = UserManager()->GetUser(m_pInfo->idTarget);
            if (pTarget)
            {
                pTarget->SendSysMsg(STR_TUTOR_REQUEST, pUser->GetName());
            }
            else
            {
                // find target name
                CAnnounceData* pData = Announce()->QueryAnnounceDataByOwner(m_pInfo->idTarget);
                if (!pData)
                {
                    pUser->SendSysMsg(STR_TUTOR_NO_ONLINE);
                    return;
                }
                else
                {
                    char bufText[1024];
                    sprintf(bufText, STR_TUTOR_REQUEST, pUser->GetName());
                    CMsgTalk	msg;
                    if (msg.Create(SYSTEM_NAME, pData->GetStr(ANNOUNCEDATA_USER_NAME), bufText))
                    {
                        MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, m_pInfo->idTarget);
                    }
                }
            }
            pUser->SendSysMsg(STR_TUTOR_REQUEST_SEND);
        }
        break;
    case actionTeacherApply:
        {
            if (!pUser)
            {
                break;
            }
            if (pUser->GetLev() >= NEWBIE_LEVEL)
            {
                pUser->SendSysMsg(STR_HEIGHT_LEV);
                return;
            }
            if (pUser->GetTutor())
            {
                //				pUser->SendSysMsg(STR_HAVE_TEACHER_ALREADY);
                return;
            }
            // find target
            CUser* pTarget = NULL;
            if (!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, m_pInfo->idTarget, IPP_OF(pTarget)))
            {
                break;
            }
            if (pTarget->GetLev() < _MIN_TUTOR_LEVEL)
            {
                pUser->SendSysMsg(STR_LOW_LEV2);
                return ;
            }
            if (pTarget->GetTutorExp() < ADDSTUDENT_TUTOREXP)
            {
                pUser->SendSysMsg(STR_NOT_ENOUGH_TUTOREXP2);
                return;
            }
            if (pTarget->IsStudentFull())
            {
                pUser->SendSysMsg(STR_STUDENT_FULL2);
                return;
            }
            pUser->SetApply(CUser::APPLY_TEACHERAPPLY, m_pInfo->idTarget);
            pTarget->SendMsg(this);
            pTarget->SendSysMsg(STR_TEACHER_APPLY, pUser->GetName());
            pUser->SendSysMsg(STR_TEACHER_APPLY_SENT);
        }
        break;
    case actionAgreeStudentApply:
        {
            if (!pUser)
            {
                break;
            }
            if (pUser->GetLev() >= NEWBIE_LEVEL)
            {
                pUser->SendSysMsg(STR_HEIGHT_LEV);
                return;
            }
            if (pUser->GetTutor())
            {
                //				pUser->SendSysMsg(STR_HAVE_TEACHER_ALREADY);
                return;
            }
            // find target
            CUser* pTarget = NULL;
            if (!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, m_pInfo->idTarget, IPP_OF(pTarget)))
            {
                break;
            }
            if (pTarget->FetchApply(CUser::APPLY_STUDENTAPPLY, pUser->GetID()))
            {
                if (pUser->AddTutor(pTarget->GetID(), pTarget->GetName()))
                {
                    // synchronize all statuses
                    MSGBUF szMsg;
                    sprintf(szMsg, STR_MAKE_TEACHERSTUDENT, pTarget->GetName(), pUser->GetName());
                    pUser->BroadcastRoomMsg(szMsg, true);
                }
                else
                {
                    ::LogSave("Error: add tutor failed.");
                }
            }
            else
            {
                pUser->SendSysMsg(STR_NO_STUDENT_APPLY);
            }
        }
        break;
    case actionAgreeTeacherApply:
        {
            if (!pUser)
            {
                break;
            }
            if (pUser->GetLev() < _MIN_TUTOR_LEVEL)
            {
                pUser->SendSysMsg(STR_LOW_LEVEL);
                return;
            }
            if (pUser->GetTutorExp() < ADDSTUDENT_TUTOREXP)
            {
                pUser->SendSysMsg(STR_NOT_ENOUGH_TUTOREXP);
                return;
            }
            if (pUser->IsStudentFull())
            {
                pUser->SendSysMsg(STR_STUDENT_FULL2);
                return;
            }
            if (pUser->GetStudent(m_pInfo->idTarget))
            {
                pUser->SendSysMsg(STR_YOUR_STUDENT_ALREADY);
                return;
            }
            // find target
            CUser* pTarget = NULL;
            if (!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, m_pInfo->idTarget, IPP_OF(pTarget)))
            {
                break;
            }
            if (pTarget->FetchApply(CUser::APPLY_TEACHERAPPLY, pUser->GetID()))
            {
                if (pTarget->AddTutor(pUser->GetID(), pUser->GetName()))
                {
                    // synchronize all statuses
                    if (pUser->IsStudentFull())
                    {
                        DEBUG_TRY
                        if (Announce()->QueryAnnounceDataByOwner(pUser->GetID()))
                        {
                            Announce()->DeleteAnnounceByUserID(pUser->GetID(), true);
                        }
                        DEBUG_CATCH("DeleteAnnounceByOwer(pUser->GetID()) ERROR");
                    }
                    MSGBUF szMsg;
                    sprintf(szMsg, STR_MAKE_TEACHERSTUDENT, pUser->GetName(), pTarget->GetName());
                    pTarget->BroadcastRoomMsg(szMsg, true);
                }
                else
                {
                    ::LogSave("Error: add tutor failed.");
                }
            }
            else
            {
                pUser->SendSysMsg(STR_NO_TEACHER_APPLY);
            }
        }
        break;
    case actionDismissStudent:
        {
            if (!pUser)
            {
                break;
            }
            if (pUser->DelStudent(m_pInfo->idTarget))
            {
                // send msg: str_dismiss_student
            }
            else
            {
                // failed.
            }
        }
        break;
    case actionLeaveTeacher:
        {
            if (!pUser)
            {
                break;
            }
            pUser->DelTutor();
        }
        break;
    case actionQueryStudentsOfStudent:
        {
            if (!pUser)
            {
                break;
            }
            CUser* pStudent = UserManager()->GetUser(m_pInfo->idTarget);
            if (pStudent)
            {
                CMsgSchoolMember	msg;
                msg.Create(MESSAGEBOARD_ADDMEMBER, NULL, 0);
                for (int i = 0; i < pStudent->GetStudentAmount(); i++)
                {
                    CTutor* pTutor = pStudent->GetStudentByIndex(i);
                    if (pTutor)
                    {
                        CUser* pTarget = UserManager()->GetUser(pTutor->GetUserID());
                        if (pTarget)
                        {
                            msg.Append(RELATION_STUDENTOFSTUDENT, MEMBERSTATUS_ONLINE, pTarget);
                        }
                        else
                        {
                            msg.Append(RELATION_STUDENTOFSTUDENT, MEMBERSTATUS_OFFLINE, pTutor->GetUserID(), pTutor->GetUserName());
                        }
                    }
                }
                if (pStudent->GetStudentAmount() > 0)
                {
                    SendMsg(&msg);
                }
            }
            else
            {
                MapGroup(m_idProcess)->QueryIntraMsg()->TransmitMsg(this, GetSocketID(), GetNpcID());
            }
        }
        break;
        // ==========佣兵任务系统相关============
    case actionQueryPlayerTaskAcceptedList:
        {
            MercenaryTask()->SendTaskList(pUser->QueryRole(), TASKLIST_ACCEPTED, m_pInfo->dwData);
        }
        break;
    case actionQueryPlayerTaskUnacceptedList:
        {
            MercenaryTask()->SendTaskList(pUser->QueryRole(), TASKLIST_UNACCEPTED, m_pInfo->dwData);
        }
        break;
    case actionQueryPlayerMyTaskList:
        {
            MercenaryTask()->SendTaskList(pUser->QueryRole(), TASKLIST_MYTASK, m_pInfo->dwData);
        }
        break;
    case actionQueryPlayerTaskDetail:
        {
            MercenaryTask()->SendTaskInfo(pUser->QueryRole(), m_pInfo->dwData);
        }
        break;
    case actionAcceptPlayerTask:
        {
            pUser->AcceptMercenaryTask(m_pInfo->dwData);
        }
        break;
    case actionCancelPlayerTask:
        {
            pUser->CancelMercenaryTask(m_pInfo->dwData);
        }
        break;
        // =======================================
    case actionBurstXp:
        {
            pUser->BurstXp();
        }
        break;
    default:
        ASSERT(!"switch");
        break;
    }
    DEBUG_CATCH2("switch(MSGACTION) [%d]", m_pInfo->usAction)		// AAAAAAAAAAAAAAAA
}
