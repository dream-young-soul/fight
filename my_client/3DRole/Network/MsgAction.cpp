
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgAction.cpp: implementation of the CMsgAction class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "GamePlayerSet.h"
#include "Hero.h"
#include "3DGameMap.h"
#include "GameMsg.h"
#include "Pet.h"
#include "NDsound.h"


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
    // param check
    if (idPlayer == ID_NONE)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_ACTION;
    m_pInfo->dwTimeStamp	=::TimeGet();
    m_pInfo->idUser		= idPlayer;
    m_pInfo->unPosX		= (USHORT)nPosX;
    m_pInfo->unPosY		= (USHORT)nPosY;
    m_pInfo->unDir		= usDir;
    m_pInfo->dwData		= dwData;
    m_pInfo->usAction	= usAction;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAction::Create(OBJID idPlayer, int nPosX, int nPosY, USHORT usDir, USHORT usAction, USHORT usTargetPosX, USHORT usTargetPosY, DWORD dwTimeStamp/*=0*/)
{
    // param check
    if (idPlayer == ID_NONE)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_ACTION;
    if (0 == dwTimeStamp)
    {
        m_pInfo->dwTimeStamp	=::TimeGet();
    }
    else
    {
        m_pInfo->dwTimeStamp	= dwTimeStamp;
    }
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
    // get obj
    if (m_pInfo->usAction == actionDisappear)
    {
        if (m_pInfo->dwData == 0)
        {
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_pInfo->idUser);
            if (pPlayer && !pPlayer->IsMyPet())
            {
                g_objGameMap.DelInteractiveObj(pPlayer);
                g_objPlayerSet.DelPlayer(pPlayer->GetID());
            }
            g_objGameMap.EndMapMagicItem(m_pInfo->idUser);
        }
        else
        {
            g_objGameMap.DelMapItem(m_pInfo->idUser);
        }
        return;
    }
    if (m_pInfo->usAction == actionChangeFace)
    {
        g_objHero.SetTeamMemberFace(m_pInfo->idUser, m_pInfo->dwData);
    }
    if (m_pInfo->usAction == actionTeamMemeberPos)
    {
        if (!g_objHero.IsTeamLeader())
        {
            CMyPos pos = { m_pInfo->unPosX, m_pInfo->unPosY };
            g_objHero.SetTeamLeaderPos(pos);
        }
    }
    if (m_pInfo->usAction == actionQueryTeamMember)
    {
        CMyPos posTeamMember;
        posTeamMember.x = m_pInfo->unPosX;
        posTeamMember.y = m_pInfo->unPosY;
        g_objGameMap.SetFriendPos(posTeamMember);
    }
    if (m_pInfo->usAction == actionEnterMap)
    {
        m_pInfo->idUser = g_objHero.GetID();
    }
    CPlayer* pPlayer	= g_objPlayerSet.GetPlayer(m_pInfo->idUser, true);
    if (!pPlayer)
    {
        switch(m_pInfo->usAction)
        {
        case actionJump:
        case actionMine:
        case actionChgDir:
        case actionEmotion:
            {
                CMyPos posHero = g_objHero.GetAlignPos();
                //if (abs(posHero.x-(int)m_pInfo->unPosX) <= _BLOCK_SIZE
                //		&& abs(posHero.y-(int)m_pInfo->unPosY) <= _BLOCK_SIZE)
                {
                    CMsgAction msg;
                    if (msg.Create(g_objHero.GetID(), posHero.x, posHero.y, g_objHero.GetDir(), actionQueryPlayer, m_pInfo->idUser))
                    {
                        msg.Send();
                    }
                }
                if (strstr(g_objHero.GetName(), "PM"))
                {
                    g_objGameMsg.AddMsg("QueryPlayer: %u", m_pInfo->idUser);
                }
            }
            break;
        case actionMapARGB:
            {
                g_objGameMap.SetARGB(m_pInfo->dwData);
            }
            break;
        default:
            break;
        }
        return;
    }
    // actions...
    switch(m_pInfo->usAction)
    {
    case actionPostCmd:
        ::PostCmd(m_pInfo->dwData);
        break;
    case actionSynchro:
        {
            if (pPlayer->GetID() != g_objHero.GetID() && !pPlayer->IsMyPet())
            {
                pPlayer->ResetStepDir();
                CMyPos posPlayer;
                pPlayer->GetPos(posPlayer);
                if (posPlayer.x != m_pInfo->usTargetPosX
                        || posPlayer.y != m_pInfo->usTargetPosY)
                {
                    pPlayer->SetActionCmd(_ACTION_STANDBY);
                    pPlayer->SetPos(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY);
                }
            }
            else if (pPlayer->GetID() == g_objHero.GetID())
            {
                g_objHero.ResetActionData();
                CMyPos posHero;
                g_objHero.GetPos(posHero);
                if (posHero.x != m_pInfo->usTargetPosX
                        || posHero.y != m_pInfo->usTargetPosY)
                {
                    m_pInfo->usTargetPosX = posHero.x;
                    m_pInfo->usTargetPosY = posHero.y;
                    this->Send();
                }
            }
            else
            {
                CPet* pPet = (CPet*)pPlayer;
                if (pPet && pPet->IsMyPet())
                {
#ifdef LW_DEBUG
                    g_objGameMsg.AddMsg("PET POS TICK BACK!");
#endif
                    return;
                }
                pPet->ResetActionData();
                CMyPos posHero;
                pPet->GetPos(posHero);
                if (posHero.x != m_pInfo->usTargetPosX
                        || posHero.y != m_pInfo->usTargetPosY)
                {
                    m_pInfo->usTargetPosX = posHero.x;
                    m_pInfo->usTargetPosY = posHero.y;
                    this->Send();
                }
            }
        }
        break;
    case actionMine:
        {
            pPlayer->ResetStepDir();
            pPlayer->SetActionCmd(_ACTION_MINE);
            int nChance = m_pInfo->dwData / 30;
            if (::RandGet(5) < nChance)
            {
                int nPosX, nPosY;
                pPlayer->GetPos(nPosX, nPosY);
                const int _DELTA_X[8] = {0, -1, -1, -1, 0, 1, 1, 1};
                const int _DELTA_Y[8] = {1, 1, 0, -1, -1, -1, 0, 1};
                nPosX += _DELTA_X[pPlayer->GetDir()];
                nPosY += _DELTA_Y[pPlayer->GetDir()];
                CMyPos posWorld;
                g_objGameMap.Cell2World(nPosX, nPosY, posWorld.x, posWorld.y);
                g_objGameMap.Add3DMapEffect(posWorld, "Mine");
            }
        }
        break;
    case actionChgDir:
        {
            pPlayer->SetDir(m_pInfo->unDir);
        }
        break;
    case actionEmotion:
        {
            if (m_pInfo->dwData == 180)
            {
                return;
            }
            struct
            {
                WORD	wEmotion;
                BYTE	btProf;
                BYTE	btFlagAllNosuch;
            } infoEmotion;
            memcpy(&infoEmotion, &m_pInfo->dwData, sizeof(DWORD));
            if (g_objHero.GetID() != m_pInfo->idUser)
            {
                CCommand cmdDemo;
                cmdDemo.iType		= _COMMAND_EMOTION;
                cmdDemo.iStatus		= _CMDSTATUS_BEGIN;
                cmdDemo.nData		= infoEmotion.wEmotion;
                cmdDemo.idTarget	= ID_NONE;
                cmdDemo.nDir		= m_pInfo->unDir;
                pPlayer->SetCommand(&cmdDemo);
            }
            if (infoEmotion.btProf)
            {
                pPlayer->AddProfessionalCoolposEffect(infoEmotion.btProf / 10, infoEmotion.btFlagAllNosuch);
            }
        }
        break;
    case actionPosition:
        {
        }
        break;
    case actionRun:
    case actionWalk:
        {
            /*
            int nPosX, nPosY;
            pPlayer->GetPos(nPosX, nPosY);

            int nLocalDistance	=CGameMap::GetDistance(nPosX, nPosY, (int)m_pInfo->usTargetPosX, (int)m_pInfo->usTargetPosY);
            int nRemoteDistance	=CGameMap::GetDistance((int)m_pInfo->unPosX, (int)m_pInfo->unPosY, (int)m_pInfo->usTargetPosX, (int)m_pInfo->usTargetPosY);
            if (nLocalDistance > nRemoteDistance)
            {
            	pPlayer->SetPos(m_pInfo->unPosX, m_pInfo->unPosY);
            	pPlayer->ResetActionData();
            }

            CCommand cmd;
            if (m_pInfo->usAction == actionWalk)
            	cmd.iType	=_COMMAND_WALK;
            else
            	cmd.iType	=_COMMAND_RUN;

            cmd.iStatus			=_CMDSTATUS_BEGIN;
            cmd.posTarget.x		=m_pInfo->usTargetPosX;
            cmd.posTarget.y		=m_pInfo->usTargetPosY;

            pPlayer->SetCommand(&cmd);
            */
        }
        break;
    case actionJump:
        {
            // align pos?
            if (g_objHero.GetID() == m_pInfo->idUser)
            {
                g_objHero.SetAlignPos(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY);
            }
            else if (pPlayer->IsMyPet())
            {
                CPet* pPet = (CPet*)pPlayer;
                //pPet->SetAlignPos(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY);
            }
            else
            {
                pPlayer->ResetStepDir();
                pPlayer->SetPos(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY);
                /*
                				CCommand cmd;
                				cmd.iType			=_COMMAND_JUMP;
                				cmd.iStatus			=_CMDSTATUS_BEGIN;
                				cmd.posTarget.x		=m_pInfo->usTargetPosX;
                				cmd.posTarget.y		=m_pInfo->usTargetPosY;

                				pPlayer->SetCommand(&cmd);*/
            }
        }
        break;
    case actionEnterMap:		// map info end
        {
            extern unsigned int	g_uStatus;
            g_uStatus	= _STATUS_NORMAL;
            //MYASSERT (m_pInfo->idUser == g_objHero.GetID());
            //m_pInfo->idUser is map id
            {
                MYASSERT (g_objGameMap.Create(m_pInfo->dwData));
                // set position and dir
                int nPosX	= m_pInfo->unPosX;
                int nPosY	= m_pInfo->unPosY;
                g_objHero.SetPos(nPosX, nPosY);
                g_objHero.SetAlignPos(nPosX, nPosY);
                g_objHero.SetDir(m_pInfo->unDir);
                // set command
                CCommand cmd;
                cmd.iType	= _COMMAND_STANDBY;
                cmd.iStatus	= _CMDSTATUS_BEGIN;
                g_objHero.SetCommand(&cmd);
                // get to the next step
                CMsgAction msg;
                MYASSERT (msg.Create(g_objHero.GetID(), 0, 0, 0, actionGetItemSet));
                msg.Send();
            }
        }
        break;
    case actionFlyMap:
        {
            MYASSERT (m_pInfo->idUser == g_objHero.GetID());
            {
                extern UINT	g_uStatus;
                g_uStatus = _STATUS_WAITING;
                // load map
                MYASSERT (g_objGameMap.Create(m_pInfo->dwData));
                // set position and dir
                g_objHero.SetPos(m_pInfo->unPosX, m_pInfo->unPosY);
                g_objHero.SetAlignPos(m_pInfo->unPosX, m_pInfo->unPosY);
                g_objHero.SetDir(m_pInfo->unDir);
                int nAmount = g_objPlayerSet.GetPlayerAmount();
                for(int i = 0; i < nAmount; i ++)
                {
                    CPlayer* pPlayer = g_objPlayerSet.GetPlayerByIndex(i);
                    if (pPlayer && pPlayer->IsMyPet() && !pPlayer->IsDead())
                    {
                        pPlayer->ResetActionData();
                        pPlayer->ResetStepDir();
                        pPlayer->SetPos(m_pInfo->unPosX, m_pInfo->unPosY);
                        CPet* pPet = (CPet*)pPlayer;
                        //pPet->SetAlignPos(m_pInfo->unPosX, m_pInfo->unPosY);
                    }
                }
                g_objHero.ResetActionData();
                // view port
                CMyPos posWorld;
                g_objHero.GetWorldPos(posWorld);
                posWorld.x	-= _SCR_WIDTH / 2;
                posWorld.y	-= _SCR_HEIGHT / 2;
                g_objGameMap.SetViewPos(posWorld);
                // set command
                CCommand cmd;
                cmd.iType	= _COMMAND_STANDBY;
                cmd.iStatus	= _CMDSTATUS_BEGIN;
                g_objHero.SetCommand(&cmd);
                g_uStatus = _STATUS_NORMAL;
            }
        }
        break;
    case actionLeaveMap:
        {
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_pInfo->idUser);
            if (pPlayer)
            {
                if (pPlayer->IsNpc() && pPlayer->IsDead())
                {
                    CMyPos posWorld;
                    pPlayer->GetWorldPos(posWorld);
                    g_objGameMap.Add3DMapEffect(posWorld, "BodyDisapear");
                }
                BOOL bDelObj	= true;
                g_objPlayerSet.DelPlayer(m_pInfo->idUser, bDelObj);
            }
        }
        break;
    case actionGetItemSet:		// items info end
        {
            CMsgAction msg;
            MYASSERT (msg.Create(g_objHero.GetID(), 0, 0, 0, actionGetGoodFriend));
            msg.Send();
            // 要求界面同步PK状态
            PostCmd(CMD_SETPKMODE);
        }
        break;
    case actionGetGoodFriend:	// friends info end
        {
            CMsgAction msg;
            MYASSERT (msg.Create(g_objHero.GetID(), 0, 0, 0, actionGetWeaponSkillSet));
            msg.Send();
        }
        break;
    case actionGetWeaponSkillSet:
        {
            CMsgAction msg;
            MYASSERT (msg.Create(g_objHero.GetID(), 0, 0, 0, actionGetMagicSet));
            msg.Send();
        }
        break;
    case actionGetMagicSet:
        {
            CMsgAction msg;
            MYASSERT (msg.Create(g_objHero.GetID(), 0, 0, 0, actionGetSynAttr));
            msg.Send();
            g_objHero.SetPhysicalForce(100);
            ::PostCmd(CMD_PPCHANGE);
        }
        break;
    case actionGetSynAttr: //接受帮派信息
        {
            g_objHero.SetQuerySchoolMember(false);
            g_objHero.QuerySchoolMember();
        }
        break;
    case actionQuerySchoolMember:
        {
            g_objHero.SetQuerySchoolMember(true);
        }
        break;
    case actionChgMap:
        {
        }
        break;
    case actionChgWeather:
        break;
    case actionEquip:
        {
            pPlayer->SetRWeapon(m_pInfo->dwData);
        }
        break;
    case actionUnequip:
        {
            pPlayer->SetRWeapon(ID_NONE);
        }
        break;
    case actionUplev:
        {
            if (pPlayer->IsMyPet())
            {
                char szMsg[128] = "";
                sprintf(szMsg, g_objGameDataSet.GetStr(100178), pPlayer->GetName() );	//100178=恭喜你的幻兽%s升了一级，幻兽能力得到增强
                g_objGameMsg.AddMsg(szMsg);
            }
            pPlayer->DoLevelUp();
        }
        break;
    case actionSetPkMode:
        {
            g_objHero.SetPkMode(m_pInfo->iData, false);
        }
        break;
    case actionChangeFace:
        {
            pPlayer->SetFace((USHORT)m_pInfo->dwData);
            if (g_objHero.GetID() == m_pInfo->idUser)
            {
                ::PostCmd(CMD_FLASH_HERO_FACE);
            }
        }
        break;
    case actionAbortMagic:
        {
            if (pPlayer->GetID() == g_objHero.GetID())
            {
                g_objHero.EndMagicBusy();
                g_objHero.StopChargeUp();
            }
            //pPlayer->m_objEffect.Add("BodyDisapear");
        }
        break;
    case actionCreateBooth:
        {
            if (pPlayer->GetID() == g_objHero.GetID())
            {
                g_objHero.m_objBoothManager.Open(m_pInfo->idTarget);
            }
        }
        break;
    case actionResumeBooth:
        {
            if (pPlayer->GetID() == g_objHero.GetID())
            {
                g_objHero.m_objBoothManager.Open(m_pInfo->idTarget);
            }
        }
        break;
    case actionCombineSubSyn:
        {
            g_objHero.UniteSyn(m_pInfo->idUser, m_pInfo->idTarget);
        }
        break;
    case actionGetMoney:
        {
            if (pPlayer)
            {
                pPlayer->AddMoneyEffect(m_pInfo->dwData);
            }
        }
        break;
    case actionKickBack:
        {
            if (pPlayer->GetID() == g_objHero.GetID())
            {
                CMyPos posCell;
                g_objHero.GetPos(posCell);
                //if (m_pInfo->unPosX == posCell.x && m_pInfo->unPosY == posCell.y)
                //	return;
                posCell.x = m_pInfo->unPosX;
                posCell.y = m_pInfo->unPosY;
                g_objHero.ResetActionData();
                g_objHero.ResetStepDir();
                g_objHero.SetPos(posCell);
                g_objHero.SetAlignPos(posCell.x, posCell.y);
                CMsgAction msg;
                if (msg.Create(g_objHero.GetID(), 0, 0, g_objHero.GetDir(), actionSynchro, posCell.x, posCell.y))
                {
                    msg.Send();
                }
            }
            if (pPlayer->IsMyPet())
            {
                CMyPos posCell;
                pPlayer->GetPos(posCell);
                //if (m_pInfo->unPosX == posCell.x && m_pInfo->unPosY == posCell.y)
                //	return;
                posCell.x = m_pInfo->unPosX;
                posCell.y = m_pInfo->unPosY;
                pPlayer->ResetActionData();
                pPlayer->ResetStepDir();
                pPlayer->SetPos(posCell);
                CPet* pPet = (CPet*)pPlayer;
                //pPet->SetAlignPos(posCell.x, posCell.y);
                CMsgAction msg;
                if (msg.Create(pPlayer->GetID(), 0, 0, pPlayer->GetDir(), actionSynchro, posCell.x, posCell.y))
                {
                    msg.Send();
                }
            }
        }
        break;
    case actionOpenDialog:
        {
            switch(m_pInfo->dwData)
            {
            case 1:	// shop
                {
                    ::PostCmd(CMD_ACTION_OPENSHOP);
                }
                break;
                /*				case 2:	// IMPORVE
                					{
                						::PostCmd(CMD_ACTION_OPENIMPORVE);
                					}
                					break;*/
            case 3:	// DEPOT
                {
                    ::PostCmd(CMD_ACTION_OPENDEPOT);
                }
                break;
            case 4: // EMBED
                {
                    ::PostCmd(CMD_ACTION_OPENEMBED);
                }
                break;
            case 5:
                {
                    ::PostCmd(CMD_ACTION_PLAYERTASKUP);
                }
                break;
            case 6:
                {
                    ::PostCmd(CMD_ACTION_UNACCEPTEDTASKLIST);
                }
                break;
            case 7: //
                {
                    ::PostCmd(CMD_ACTION_ACCEPTEDTASKLIST);
                }
                break;
            case 8:
                {
                    ::PostCmd(CMD_ACTION_MYTASKLIST);
                }
                break;
            case 10:		// up my announce
                {
                    ::PostCmd(CMD_ACTION_OPENUPANNOUNCE);
                }
                break;
            case 11:		// annnounce list
                {
                    ::PostCmd(CMD_ACTION_OPENANNOUNCELIST);
                }
                break;
            case 12:		// my announce
                {
                    ::PostCmd(CMD_ACTION_OPENMYANNOUNCE);
                }
                break;
            case 13:
                {
                    ::PostCmd(CMD_ACTION_OPENMONSTERHATCH);
                }
                break;
            case 14:
                {
                    ::PostCmd(CMD_ACTION_OPENMONSTERALIVE);
                }
                break;
            case 15:
                {
                    ::PostCmd(CMD_ACTION_OPENMONSTERIMPROVE);
                }
                break;
            case 16:
                {
                    ::PostCmd(CMD_ACTION_OPENMONSTERDEPOT);
                }
                break;
            case 17:	//头像改变npc dialog
                {
                    ::PostCmd(CMD_ACTION_OPENNPCFACE);
                }
                break;
            case 18:
                {
                    ::PostCmd(CMD_ACTION_OPENBOOTH);
                }
                break;
            case 19:
                {
                    ::PostCmd(CMD_ACTION_OPENBOOTHFLAG);
                }
                break;
            case 20:
                {
                    ::PostCmd(CMD_ACTION_OPENDICECFM);
                }
                break;
            case 21:	//加进拍卖物品
                {
                    ::PostCmd(CMD_ACTION_OPENADDAUCTION);
                }
                break;
            case 22:   //拍卖叫价界面
                {
                    ::PostCmd(CMD_ACTION_OPENAUCTIONBID);
                }
                break;
            case 23:	//品质锻造
                {
                    ::PostCmd(CMD_ACTION_OPENIMPROVE);
                }
                break;
            case 24:	//魔魂锻造
                {
                    ::PostCmd(CMD_ACTION_OPENIMPROVESOUL);
                }
                break;
            case 25:	//武器升级
                {
                    ::PostCmd(CMD_ACTION_OPENIMPROVELEVEL);
                }
                break;
            case 26:    //宝石注入
                {
                    ::PostCmd(CMD_ACTION_OPENIMPROVEGEM);
                }
                break;
            case 27:  //拍卖仓库
                {
                    ::PostCmd(CMD_ACTION_OPENAUCPACKAGE);
                }
                break;
            case 28:    //鉴定
                {
                    ::PostCmd(CMD_ACTION_IDENTBOX);
                }
                break;
            case 29:	//幻兽一次进化 -圣兽
                {
                    ::PostCmd(CMD_ACTION_EVOMONSTERG);
                }
                break;
            case 30:	//幻兽一次进化-魔兽
                {
                    ::PostCmd(CMD_ACTION_EVOMONSTERS);
                }
                break;
            case 31:	//幻兽2次进化	生命之神
                {
                    ::PostCmd(CMD_ACTION_EVOMONSTER1);
                }
                break;
            case 32:	//幻兽2次进化	公正之神
                {
                    ::PostCmd(CMD_ACTION_EVOMONSTER2);
                }
                break;
            case 33:	//幻兽2次进化	智慧之神
                {
                    ::PostCmd(CMD_ACTION_EVOMONSTER3);
                }
                break;
            case 34:	//幻兽2次进化	锻造之神
                {
                    ::PostCmd(CMD_ACTION_EVOMONSTER4);
                }
                break;
            case 35:	//幻兽2次进化	混沌死神
                {
                    ::PostCmd(CMD_ACTION_EVOMONSTER5);
                }
                break;
            case 36:	//幻兽2次进化	阴谋之神
                {
                    ::PostCmd(CMD_ACTION_EVOMONSTER6);
                }
                break;
            case 37:	//幻兽2次进化	战争之神
                {
                    ::PostCmd(CMD_ACTION_EVOMONSTER7);
                }
                break;
            case 38:	//幻兽2次进化	腐败之神
                {
                    ::PostCmd(CMD_ACTION_EVOMONSTER8);
                }
                break;
            }
        }
        break;
    case actionAgreeStudentApply:
        break;
    case actionAgreeTeacherApply:
        break;
    case actionStudentApply:
        {
            g_objHero.SetLastStudentApply(m_pInfo->idUser);
        }
        break;
    case actionTeacherApply:
        {
            g_objHero.SetLastTeacherApply(m_pInfo->idUser);
        }
        break;
    case actionDismissStudent:
        break;
    case actionLeaveTeacher:
        break;
    case actionLockUser:
        {
            if (m_pInfo->idUser == g_objHero.GetID())
            {
                g_objHero.MagicLock();
                g_objHero.SetAlignPos(m_pInfo->unPosX, m_pInfo->unPosY);
            }
            pPlayer->ResetStepDir();
            pPlayer->ResetActionData();
            CMyPos posCell = {m_pInfo->unPosX, m_pInfo->unPosY};
            CMyPos posWorld;
            g_objGameMap.Cell2World(posCell.x, posCell.y, posWorld.x, posWorld.y);
            pPlayer->SetDir(m_pInfo->unDir);
            pPlayer->SetPos(posCell);
            pPlayer->SetWorldPos(posWorld);
        }
        break;
    case actionUnLockUser:
        {
            if (m_pInfo->idUser == g_objHero.GetID())
            {
                g_objHero.UnMagicLock();
                g_objHero.SetAlignPos(m_pInfo->unPosX, m_pInfo->unPosY);
            }
            /*pPlayer->ResetStepDir();
            pPlayer->ResetActionData();
            pPlayer->SetDir(m_pInfo->unDir);
            CMyPos posCell = {m_pInfo->unPosX, m_pInfo->unPosY};
            CMyPos posWorld;
            g_objGameMap.Cell2World(posCell.x, posCell.y,posWorld.x, posWorld.y);
            pPlayer->SetPos(posCell);
            pPlayer->SetWorldPos(posWorld);*/
        }
        break;
    case actionSoundEffect:
        {
            EffectIndex* pIndex = g_obj3DRoleData.GetEffectIndexInfo(m_pInfo->dwData);
            if (pIndex)
            {
                CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_pInfo->idUser, true);
                if (pPlayer)
                {
                    pPlayer->m_objEffect.Add((char*)pIndex->strEffect.c_str(), false, 0, pPlayer->GetLook());
                    pPlayer->Disappear(pIndex->dwData);
                }
                CMyPos posWorld;
                g_objGameMap.Cell2World(m_pInfo->unPosX, m_pInfo->unPosY, posWorld.x, posWorld.y);
                ::DXPlaySound((char*)pIndex->strWave.c_str(), posWorld.x, posWorld.y, 1000);
            }
        }
        break;
    case actionMagicTrack:
        {
            if (m_pInfo->idUser == g_objHero.GetID())
            {
                g_objHero.SetAlignPos(m_pInfo->unPosX, m_pInfo->unPosY);
            }
            pPlayer->ResetStepDir();
            CMyPos posWorld, posCell;
            pPlayer->GetActionEndPos(posWorld.x, posWorld.y);
            pPlayer->SetWorldPos(posWorld);
            g_objGameMap.World2Cell(posWorld.x, posWorld.y, posCell.x, posCell.y);
            pPlayer->SetPos(posCell);
            pPlayer->ResetActionData();
            pPlayer->SetDir(m_pInfo->unDir);
            CCommand cmd;
            cmd.iType	= _COMMAND_INTONE;
            cmd.iStatus	= _CMDSTATUS_BEGIN;
            //cmd.nData	= m_pInfo->dwData%1000;
            cmd.nDir	= m_pInfo->unDir;
            cmd.bData	= false;
            cmd.bAddUp  = true;
            cmd.dwData  = 2;
            cmd.posTarget.x = m_pInfo->unPosX;
            cmd.posTarget.y = m_pInfo->unPosY;
            //cmd.nTargetZ = m_pInfo->dwData/1000;
            TrackInfo* pInfo = g_obj3DRoleData.GetTrackInfo(m_pInfo->dwData);
            if (pInfo)
            {
                cmd.nTargetZ = pInfo->nHeight;
                cmd.nData = pInfo->nAction;
                strcpy(cmd.szString0, pInfo->strEffect.c_str());
                strcpy(cmd.szString1, pInfo->strWave.c_str());
                cmd.nFrameInterval = 0;
				int nFrameCount = pInfo->nEndFrame - pInfo->nStartFrame;
				if( nFrameCount <= 0 )
				{
					cmd.timeActionTime = 0;
					cmd.nFrameInterval = pInfo->nFrameInterval;
					cmd.bUseFrameInterval = true;
					//cmd.timeActionTime = pInfo->nFrameInterval * nFrameCount;
					//cmd.timeActionTime = 1000;//pInfo->nFrameInterval * nFrameCount;//( pInfo->nEndFrame - pInfo->nStartFrame);
				}else
				{
					cmd.timeActionTime = pInfo->nFrameInterval * nFrameCount;//( pInfo->nEndFrame - pInfo->nStartFrame);
					
				}
                cmd.nFrameStart = pInfo->nStartFrame;
                cmd.nFrameEnd = pInfo->nEndFrame;
            }
            else
            {
                cmd.nData = _ACTION_STANDBY;
            }
            CCommand* pCmd = new CCommand;
            memcpy(pCmd, &cmd, sizeof(CCommand));
            pPlayer->m_setCmd.push_back(pCmd);
        }
        break;
    case actionBurstXp:
        g_objHero.ReplaceStatus(USERSTATUS_XPFULL);
        break;
    case actionChangeToRandomPos: //随机移动
        if (m_pInfo->idUser == g_objHero.GetID())
        {
            g_objHero.SetPos(m_pInfo->unPosX, m_pInfo->unPosY);
            g_objHero.SetAlignPos(m_pInfo->unPosX, m_pInfo->unPosY);
            int nAmount = g_objPlayerSet.GetPlayerAmount();
            for(int i = 0; i < nAmount; i ++)
            {
                CPlayer* pPlayer = g_objPlayerSet.GetPlayerByIndex(i);
                if (pPlayer && pPlayer->IsMyPet())
                {
                    pPlayer->ResetActionData();
                    pPlayer->ResetStepDir();
                    pPlayer->SetPos(m_pInfo->unPosX, m_pInfo->unPosY);
                    //					CPet* pPet = (CPet*)pPlayer;
                    //					pPet->SetAlignPos(m_pInfo->unPosX, m_pInfo->unPosY);
                }
            }
            g_objHero.ResetActionData();
        }
        else
        {
            int nAmount = g_objPlayerSet.GetPlayerAmount();
            for(int i = 0; i < nAmount; i ++)
            {
                CPlayer* pPlayer = g_objPlayerSet.GetPlayerByIndex(i);
                if (pPlayer && pPlayer->GetID() == m_pInfo->idUser)
                {
                    pPlayer->ResetActionData();
                    pPlayer->ResetStepDir();
                    pPlayer->SetPos(m_pInfo->unPosX, m_pInfo->unPosY);
                }
            }
        }
        break;
    case actionDisappear:
        {
        }
        break;
    }
}
