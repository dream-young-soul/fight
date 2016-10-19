
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgAction.cpp: implementation of the CMsgAction class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "npcworld.h"
#include "BaseFunc.h"
#include "agent.h"

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
    // actions...
    switch(m_pInfo->usAction)
    {
    case actionChgDir:
        {
            CUser* pUser	= UserManager()->QueryUser(m_pInfo->idUser);
            if (pUser)
            {
                pUser->SetDir(m_pInfo->unDir);
            }
        }
        break;
    case actionPosition:
        {
            //@
        }
        break;
    case actionEmotion:
        {
            CUser* pUser	= UserManager()->QueryUser(m_pInfo->idUser);
            if (pUser)
            {
                pUser->SetPose(m_pInfo->dwData);
            }
        }
        break;
    case actionBroadcastPos:
        {
            //@
        }
        break;
    case actionDivorce:
        {
            ASSERT(!"actionDivorce");
        }
        break;
    case actionSelfUnfreeze:
        {
            ASSERT(!"actionSelfUnfreeze");
        }
        break;
    case actionChgMap:				// 注意：没有回应actionChgMap，而是回应actionFly
        {
            ASSERT(!"actionChgMap");
        }
        break;
    case actionFlyMap:
        {
            CUser* pUser	= UserManager()->QueryUser(m_pInfo->idUser);
            if (pUser)
            {
                OBJID idMap = m_pInfo->idTarget;
                pUser->FlyMap(idMap, m_pInfo->unPosX, m_pInfo->unPosY);		// 回应actionFlyMap
                return ;
            }
            CAgent* pAgent = NpcManager()->QueryAgent(m_pInfo->idUser);
            if (pAgent)
            {
                OBJID idMap = m_pInfo->idTarget;
                Cast<CRoleMove>(pAgent)->FlyMap(idMap, m_pInfo->unPosX, m_pInfo->unPosY);		// 回应actionFlyMap
                return ;
            }
        }
        break;
    case actionChgWeather:
        {
            //@
        }
        break;
    case actionFireworks:
        {
            //@
        }
        break;
    case actionDie:
        {
            // 自杀
            if (IsSceneID(m_pInfo->idUser))
            {
                ;
            }
            else
            {
                INpc* pNpc = NpcManager()->QueryNpc(m_pInfo->idUser);
                IF_OK(pNpc)
                pNpc->SetDie();
            }
        }
        break;
    case actionQuitSyn:
        {
            ASSERT(!"actionQuitSyn");
        }
        break;
    case actionWalk:
    case actionRun:
        {
            //pUser->SetPos(m_pInfo->unPosX, m_pInfo->unPosY);
        }
        break;
    case actionEnterMap:			// 1
        {
            CAgent* pAgent = NpcManager()->QueryAgent(GetNpcID());
            if (pAgent)
            {
                OBJID idMap = m_pInfo->idUser;			//? idUser is idMap
                Cast<CRoleMove>(pAgent)->EnterMap(idMap, m_pInfo->unPosX, m_pInfo->unPosY);
                CMsgAction	msg;
                IF_OK(msg.Create(pAgent->GetID(), pAgent->GetPosX(), pAgent->GetPosY(), pAgent->GetDir(), actionGetItemSet))
                SendMsg(&msg);
            }
        }
        break;
    case actionGetItemSet:			// 2
        {
            CAgent* pAgent = NpcManager()->QueryAgent(GetNpcID());
            if (pAgent)
            {
                CMsgAction	msg;
                IF_OK(msg.Create(pAgent->GetID(), pAgent->GetPosX(), pAgent->GetPosY(), pAgent->GetDir(), actionGetGoodFriend))
                SendMsg(&msg);
            }
        }
        break;
    case actionGetGoodFriend:			// 3
        {
            CAgent* pAgent = NpcManager()->QueryAgent(GetNpcID());
            if (pAgent)
            {
                CMsgAction	msg;
                IF_OK(msg.Create(pAgent->GetID(), pAgent->GetPosX(), pAgent->GetPosY(), pAgent->GetDir(), actionGetWeaponSkillSet))
                SendMsg(&msg);
            }
        }
        break;
    case actionGetWeaponSkillSet:			// 4
        {
            CAgent* pAgent = NpcManager()->QueryAgent(GetNpcID());
            if (pAgent)
            {
                CMsgAction	msg;
                IF_OK(msg.Create(pAgent->GetID(), pAgent->GetPosX(), pAgent->GetPosY(), pAgent->GetDir(), actionGetMagicSet))
                SendMsg(&msg);
            }
        }
        break;
    case actionGetMagicSet:			// 5
        {
            CAgent* pAgent = NpcManager()->QueryAgent(GetNpcID());
            if (pAgent)
            {
                CMsgAction	msg;
                IF_OK(msg.Create(pAgent->GetID(), pAgent->GetPosX(), pAgent->GetPosY(), pAgent->GetDir(), actionGetSynAttr))
                SendMsg(&msg);
            }
        }
        break;
    case actionGetSynAttr:			// 6
        {
            CAgent* pAgent = NpcManager()->QueryAgent(GetNpcID());
            if (pAgent)
            {
                pAgent->LoginOK();
            }
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
            		pUser->SendMsg(&msg);
            }
            //*/
#ifdef	FW_ENABLE
            CUser* pUser	= UserManager()->QueryUser(m_pInfo->idUser);
            if (pUser)
            {
                pUser->MoveForward(m_pInfo->unDir);    // return true: 是n步模数
            }
#endif
        }
        break;
    case actionJump:
        {
            CUser* pUser	= UserManager()->QueryUser(m_pInfo->idUser);
            if (pUser)
            {
                pUser->JumpPos(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY, m_pInfo->unDir);
            }
        }
        break;
    case actionLeaveMap:
        {
            if (IsSceneID(m_pInfo->idUser))
            {
                ;
            }
            else if (IsNpcID(m_pInfo->idUser))
            {
                NpcManager()->QuerySet()->DelObj(m_pInfo->idUser);
            }
            else
            {
                UserManager()->QuerySet()->DelObj(m_pInfo->idUser);
            }
        }
        break;
    case actionEquip:
    case actionUnequip:
    case actionUplev:
        {
        }
        break;
    case actionSynchro:
        {
            CUser* pUser	= UserManager()->QueryUser(m_pInfo->idUser);
            if (pUser)
            {
                IF_NOT(pUser->GetMap()->IsValidPoint(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY))
                return ;
                pUser->SetPos(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY);
            }
            else if (GetNpcID() == m_pInfo->idUser)		// send to one self
            {
                INpc* pNpc = NpcManager()->QueryNpc(m_pInfo->idUser);
                if (pNpc)
                {
                    IF_NOT(pNpc->GetMap()->IsValidPoint(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY))
                    return ;
                    pNpc->SynchroPos(m_pInfo->usTargetPosX, m_pInfo->usTargetPosY, m_pInfo->usTargetPosX, m_pInfo->usTargetPosY);
                }
            }
        }
        break;
    case	actionKickBack:
        {
            INpc* pNpc = NpcManager()->QueryNpc(m_pInfo->idUser);
            if (pNpc)
            {
                IF_NOT(pNpc->GetMap()->IsValidPoint(m_pInfo->unPosX, m_pInfo->unPosY))
                return ;
                pNpc->KickBack(m_pInfo->unPosX, m_pInfo->unPosY);
            }
        }
        break;
    case	actionMapStatus:
        {
            OBJID idMap = m_pInfo->idUser;
            int nStatus = m_pInfo->iData;
            CGameMap* pMap = MapManager()->QueryMap(idMap);
            IF_OK(pMap)
            pMap->SetStatus(nStatus);
        }
        break;
    case	actionMoveStop:
        {
            INpc* pNpc = NpcManager()->QueryNpc(GetNpcID());
            if (pNpc && Cast<CNpc>(pNpc))
            {
                Cast<CNpc>(pNpc)->LockMove(m_pInfo->dwData);
            }
        }
        break;
    case	actionChangeMapDoc:
        {
            OBJID idMap = m_pInfo->idUser;
            OBJID idDoc = m_pInfo->dwData;
            CGameMap* pMap = MapManager()->QueryMap(idMap);
            IF_OK(pMap)
            {
                pMap->ChangeMapDoc(idDoc);
            }
        }
        break;
    case	actionAddTerrainObj:
        {
            OBJID idMap = m_pInfo->idUser;
            OBJID idOwner = m_pInfo->dwData;
            OBJID idTerrainObj = GetNpcID();
            CGameMap* pMap = MapManager()->QueryMap(idMap);
            IF_OK(pMap)
            {
                pMap->AddTerrainObj(idOwner, m_pInfo->unPosX, m_pInfo->unPosY, idTerrainObj);
            }
        }
        break;
    case	actionDelTerrainObj:
        {
            OBJID idMap = m_pInfo->idUser;
            OBJID idOwner = m_pInfo->dwData;
            CGameMap* pMap = MapManager()->QueryMap(idMap);
            IF_OK(pMap)
            {
                pMap->DelTerrainObj(idOwner);
            }
        }
        break;
    case	actionLockUser:
        {
            INpc* pNpc = NpcManager()->QueryNpc(GetNpcID());
            if (pNpc && Cast<CNpc>(pNpc))
            {
                pNpc->Lock(true);
            }
        }
        break;
    case	actionUnlockUser:
        {
            INpc* pNpc = NpcManager()->QueryNpc(GetNpcID());
            if (pNpc && Cast<CNpc>(pNpc))
            {
                pNpc->Lock(false);
            }
        }
        break;
    case	actionMagicTrack:
        {
            INpc* pNpc = NpcManager()->QueryNpc(m_pInfo->idUser);
            if (pNpc && Cast<CNpc>(pNpc))
            {
                Cast<CNpc>(pNpc)->SetPos(m_pInfo->unPosX, m_pInfo->unPosY);
                Cast<CNpc>(pNpc)->SetDir(m_pInfo->unDir);
            }
        }
        break;
    default:
        //		ASSERT(!"switch");
        break;
    }
}
