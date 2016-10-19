
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgInteract.cpp: interface for the CMsgInteract class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "MsgInteract.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgInteract::CMsgInteract()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgInteract::~CMsgInteract()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgInteract::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_INTERACT != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgInteract::Create(USHORT unType, OBJID idSender, OBJID idTarget, USHORT unPosX, USHORT unPosY, DWORD dwData /*= 0*/)
{
    DEBUG_CREATEMSG("Interact", idSender, unType, "", idTarget, dwData)
    CHECKF(unType && idSender);		// && idTarget
    // init
    this->Init();
    // fill info now
    m_pInfo->unType			= unType;
    m_pInfo->idSender		= idSender;
    m_pInfo->idTarget		= idTarget;
    m_pInfo->unPosX			= unPosX;
    m_pInfo->unPosY			= unPosY;
    m_pInfo->dwData			= dwData;
    m_unMsgType	= _MSG_INTERACT;
    m_unMsgSize	= sizeof(MSG_Info);
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgInteract::Create(USHORT unType, OBJID idSender, OBJID idTarget, USHORT unPosX, USHORT unPosY, USHORT usMagicType, USHORT usMagicLev)
{
    DEBUG_CREATEMSG("Interact", idSender, unType, "", idTarget, dwData)
    CHECKF(unType && idSender);		// && idTarget
    // init
    this->Init();
    // fill info now
    m_pInfo->unType			= unType;
    m_pInfo->idSender		= idSender;
    m_pInfo->idTarget		= idTarget;
    m_pInfo->unPosX			= unPosX;
    m_pInfo->unPosY			= unPosY;
    m_pInfo->usMagicType	= usMagicType;
    m_pInfo->usMagicLevel	= usMagicLev;
    m_unMsgType	= _MSG_INTERACT;
    m_unMsgSize	= sizeof(MSG_Info);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgInteract::Process(void* pInfo)
{
    DEBUG_PROCESSMSG("Interact", m_pInfo->idSender, m_pInfo->unType, "", m_pInfo->idTarget, m_pInfo->dwData)
    IRole* pRole = RoleManager()->QueryRole(this, m_pInfo->idSender);
    if (!pRole)
    {
        return;
    }
    //	if (m_pInfo->unType == INTERACT_ATTACK || m_pInfo->unType == INTERACT_SHOOT
    //		|| m_pInfo->unType == INTERACT_RUSHATK || m_pInfo->unType == INTERACT_MAGICATTACK)
    {
        if (pRole->QueryStatus(STATUS_LOCK) || pRole->QueryStatus(STATUS_FREEZE)
                || pRole->QueryStatus(STATUS_FAINT))
        {
#ifdef ZLONG_DEBUG
            pRole->SendSysMsg("Debug: 不可交互");
#endif
            return;
        }
    }
    //ASSERT(m_pInfo->idSender == pRole->GetID());
    //		m_pInfo->idSender = pRole->GetID();
    IRole* pTarget = RoleManager()->QueryRole(m_pInfo->idTarget);
    if (!pTarget && INTERACT_MAGICATTACK != m_pInfo->unType)
    {
        return;
    }
    CUser* pUser = NULL;
    pRole->QueryObj(OBJ_USER, IPP_OF(pUser));
    if (pUser && !pUser->IsAlive())
    {
        pUser->SendSysMsg(STR_DIE);
        return ;
    }
    if (pUser)
    {
        pUser->SetPose(_ACTION_STANDBY);
    }
    // fill id
    m_pInfo->idSender	= pRole->GetID();
    // stop fight
    pRole->ClrAttackTarget();
    // stop mine
    if (pUser)
    {
        pUser->StopMine();
    }
    DEBUG_TRY	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
    switch(m_pInfo->unType)
    {
    case	INTERACT_ATTACK:
    case	INTERACT_SHOOT:
        {
            if (pUser)
            {
                if (!pUser->SynPosition(m_pInfo->unPosX, m_pInfo->unPosY))
                {
                    UserManager()->KickOutSocket(m_idSocket, "CMsgInteract SynPosition");
                    return;
                }
                if (pUser->GetMap()->IsNewbieMap())
                {
                    pUser->SendSysMsg(STR_NO_PK_VILLAGE);
                    return;
                }
                if (pUser->QueryMagic()->IsInLaunch())
                {
#ifdef _DEBUGx
                    pUser->SendSysMsg("DEBUG: magic in launch!");
#endif
                    return;
                }
                if (pUser->QueryMagic()->IsIntone())
                {
                    pUser->QueryMagic()->AbortMagic();
                }
            }
            pRole->QueryStatusSet()->DelObj(STATUS_LURKER);
            if (pUser)
            {
                pUser->ProcessOnAttack();
            }
            pRole->SetAttackTarget(pTarget);
            if (pUser)
            {
                pUser->ProcessAutoAttack();
            }
        }
        break;
    case	INTERACT_RUSHATK:
        {
            if (pUser)
            {
                LOGCHEAT("rush attack: user[%s][%u]", pUser->GetName(), pUser->GetAccountID());
                if (!pUser->SynPosition(m_pInfo->unPosX, m_pInfo->unPosY)
                        || pUser->GetDistance(m_pInfo->usData0, m_pInfo->usData1) >= 2 * CELLS_PER_BLOCK)
                {
                    pUser->SendSysMsg(STR_INVALID_MSG);
                    UserManager()->KickOutSocket(m_idSocket, "RUSHATK 超远");
                    return;
                }
                if (pUser->QueryMagic()->IsInLaunch())
                {
#ifdef _DEBUGx
                    pUser->SendSysMsg("DEBUG: magic in launch!");
#endif
                    return;
                }
                if (pUser->QueryMagic()->IsIntone())
                {
                    pUser->QueryMagic()->AbortMagic();
                }
                pUser->BroadcastRoomMsg(this, INCLUDE_SELF);
                IRole* pTarget = pUser->FindAroundRole(m_pInfo->idTarget);
                if (!pTarget)
                {
                    return;
                }
                pUser->QueryStatusSet()->DelObj(STATUS_LURKER);
                pUser->ProcessOnAttack();
                pUser->SetAttackTarget(pTarget);
                pUser->ProcessAutoAttack();
            }
        }
        break;
    case	INTERACT_MAGICATTACK:
        {
            if (pUser)
            {
                if (pUser->GetMap()->IsNewbieMap())
                {
                    pUser->SendSysMsg(STR_NO_PK_VILLAGE);
                }
                if (pUser->QueryMagic()->IsInLaunch())
                {
#ifdef _DEBUGx
                    pUser->SendSysMsg("DEBUG: magic in launch!");
#endif
                    return;
                }
                if (pUser->QueryMagic()->IsIntone())
                {
                    pUser->QueryMagic()->AbortMagic();
                }
                pUser->ResetEnergyInc();
            }
            pRole->QueryStatusSet()->DelObj(STATUS_LURKER);
            // decode
            if (!this->IsNpcMsg())
            {
                if (!pUser->CheckTimeStamp(m_pInfo->dwTimeStamp))
                {
                    return ;
                }
                bool bTestMagic = false;
                switch(m_pInfo->usData0)
                {
                case 1000:
                case 1001:
                case 1002:
                case 1030:
                case 1125:
                case 1150:
                case 1165:
                case 1160:
                    m_pInfo->usData1 ^= 0x6279;
                    bTestMagic	= true;
                    break;
                default:
                    break;
                }
                USHORT usLev  = (m_pInfo->usData1 ^ 0x3721) & 0xff;
                USHORT usData = (m_pInfo->usData1 ^ 0x3721) >> 8;
                if (usData != (m_pInfo->dwTimeStamp % 0x100))
                {
                    UserManager()->KickOutSocket(m_idSocket, "外挂！");
                    return;
                }
                m_pInfo->usData1 = usLev;
                //* DECODE
                //////////////////////////////////////////////////////////////////////////
#define	ENCODE_MAGICATTACK(idUser,usType,idTarget,usPosX,usPosY) {	\
				usType		= (::ExchangeShortBits((usType - 0x14BE),3) ^ (idUser) ^ 0x915D);	\
				idTarget	= ::ExchangeLongBits(((idTarget - 0x8B90B51A) ^ (idUser) ^ 0x5F2D2463),32-13); \
				usPosX		= (::ExchangeShortBits((usPosX - 0xDD12),1) ^ (idUser) ^ 0x2ED6);	\
				usPosY		= (::ExchangeShortBits((usPosY - 0x76DE),5) ^ (idUser) ^ 0xB99B);	}
#define	DECODE_MAGICATTACK(idUser,usType,idTarget,usPosX,usPosY) {	\
				usType		= 0xFFFF&(::ExchangeShortBits(((usType) ^ (idUser) ^ 0x915D),16-3) + 0x14BE);	\
				idTarget	= (::ExchangeLongBits((idTarget),13) ^ (idUser) ^ 0x5F2D2463) + 0x8B90B51A; \
				usPosX		= 0xFFFF&(::ExchangeShortBits(((usPosX) ^ (idUser) ^ 0x2ED6),16-1) + 0xDD12);	\
				usPosY		= 0xFFFF&(::ExchangeShortBits(((usPosY) ^ (idUser) ^ 0xB99B),16-5) + 0x76DE);	}
                //////////////////////////////////////////////////////////////////////////
                if (!IsNpcMsg())
                    DECODE_MAGICATTACK(pUser->GetID(), m_pInfo->usMagicType, m_pInfo->idTarget, m_pInfo->unPosX, m_pInfo->unPosY)
                    //*/// DECODE
                    if (pUser)
                    {
                        if (bTestMagic && pUser->IsMagicAtkCheat(m_pInfo->idTarget, m_pInfo->unPosX, m_pInfo->unPosY, m_pInfo->dwTimeStamp))
                        {
                            ::MyLogSave("gmlog/cheater", "Cheater found: %s[%d]",
                                        pUser->GetName(),
                                        pUser->GetID());
                            if (1 == ::RandGet(8))
                            {
                                pUser->DoCheaterPunish();
                                //UserManager()->KickOutSocket(m_idSocket, "cheat found!");
                                return;
                            }
                        }
                        if (pUser->QueryMagic())
                        {
                            //
                            IMagicData* pMagicData = pUser->QueryMagic()->FindMagic(m_pInfo->usMagicType);
                            if (pMagicData)
                            {
                                if ((pMagicData->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_RANDOM) != 0)
                                {
                                    // 暂时不做惩罚
                                    //pUser->DoCheaterPunish();
#ifdef _DEBUG
                                    pUser->SendSysMsg("被动技能不能主动触发！");
#endif
                                    return;
                                }
                            }
                        }
                    }
            }
            if (pUser)
            {
                pUser->ProcessOnAttack();
            }
            pRole->MagicAttack(m_pInfo->usMagicType, m_pInfo->idTarget, m_pInfo->unPosX, m_pInfo->unPosY);	// 在内部广播
        }
        break;
    case	INTERACT_ABORTMAGIC:
        {
            if (pUser)
            {
                if (pUser->QueryMagic()->AbortMagic())
                {
                    pUser->BroadcastRoomMsg(this, INCLUDE_SELF);
                }
            }
        }
        break;
    case	INTERACT_COURT:
        {
            if (!pUser)
            {
                break;
            }
            pUser->Court(m_pInfo->idTarget);
        }
        break;
    case	INTERACT_MARRY:
        {
            if (!pUser)
            {
                break;
            }
            pUser->Marry(m_pInfo->idTarget);
        }
        break;
    case	INTERACT_DIVORCE:
        {
            if (!pUser)
            {
                break;
            }
        }
        break;
    case	INTERACT_MINE:
        {
            if (pUser)
            {
                if (!pUser->SynPosition(m_pInfo->unPosX, m_pInfo->unPosY))
                {
                    UserManager()->KickOutSocket(m_idSocket, "CMsgInteract SynPosition");
                    return;
                }
                pUser->Mine(pTarget);
            }
        }
        break;
    default:
        {
            LOGERROR("INVALID MSG INTERACT TYPE %d", m_pInfo->unType);
        }
    } // switch
    DEBUG_CATCH2("switch(MSGINTERACT) [%d]", m_pInfo->unType)		// AAAAAAAAAAAAAAAA
    return;
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgInteract, id:%u", m_pInfo->id);
#endif
}







