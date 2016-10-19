
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgBetOutside.cpp: implementation of the CMsgAtk class.
// ÏÉ½£ÐÞ£¬2002.1.8
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "UserManager.h"
#include "MsgBetOutside.h"
#include "common.h"
#include "BetOutside.h"
#include "GameMap.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgBODice::CMsgBODice()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgBODice::~CMsgBODice()
{
}

//////////////////////////////////////////////////////////////////////
BOOL	CMsgBODice::Create (int nAction, OBJID idNpc, int nTime/*=0*/)
{
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_DICE;
    m_pInfo->ucAction	= nAction;
    m_pInfo->idDiceNpc	= idNpc;
    m_pInfo->ucAmount	= 0;
    if (nAction == _ACTION_BEGINCHIP)
    {
        m_pInfo->ucTime		= nTime;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL	CMsgBODice::Append (int nType, DWORD dwData)
{
    CHECKF(m_unMsgSize + sizeof(m_pInfo->Data) < MAX_PACKETSIZE);
    m_pInfo->Data[m_pInfo->ucAmount].ucType	= nType;
    m_pInfo->Data[m_pInfo->ucAmount].dwData	= dwData;
    m_pInfo->ucAmount++;
    m_unMsgSize	+= sizeof(m_pInfo->Data);
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL	CMsgBODice::Create (int nAction, OBJID idNpc, char aDice[4])
{
    // check param
    for(int i = 0; i < 4; i++)
    {
        if (aDice[i] < 1 || aDice[i] > 6)
        {
            LOGERROR("CMsgBODice::Create () ²ÎÊýÒì³£");
            return false;
        }
    }
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_DICE;
    m_pInfo->ucAction	= nAction;
    m_pInfo->idDiceNpc	= idNpc;
    m_pInfo->ucAmount	= 1;
    m_pInfo->Data[0].ucDice[0]	= aDice[0];
    m_pInfo->Data[0].ucDice[1]	= aDice[1];
    m_pInfo->Data[0].ucDice[2]	= aDice[2];
    m_pInfo->Data[0].ucDice[3]	= aDice[3];
    return true;
}

BOOL	CMsgBODice::Create		(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_DICE != this->GetType())
    {
        return false;
    }
    return true;
}

void	CMsgBODice::Process		(CGameSocket* pSocket)
{
    CUser* pUser = UserManager()->GetUser(this);
    if (!pUser)
    {
        return ;
    }
    switch(m_pInfo->ucAction)
    {
    case	_ACTION_CHIPIN:
        {
            IRole* pRole = pUser->FindAroundRole(m_pInfo->idDiceNpc);
            CNpc* pNpc;
            if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsBetNpc() && m_pInfo->Data[0].dwData)
            {
                pNpc->QueryBet()->ChipIn(pUser, m_pInfo->Data[0].ucType, m_pInfo->Data[0].dwData);
            }
        }
        break;
    case	_ACTION_CANCELCHIP:
        {
            IRole* pRole = pUser->FindAroundRole(m_pInfo->idDiceNpc);
            CNpc* pNpc;
            if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsBetNpc())
            {
                pNpc->QueryBet()->ChipIn(pUser, m_pInfo->Data[0].ucType, 0);
            }
        }
        break;
    default:
        ASSERT(!"switch(m_pInfo->ucAction)");
    }
}


