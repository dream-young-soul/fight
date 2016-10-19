
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgTrade.cpp: implementation of the CMsgTrade class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "AllMsg.h"
#include "User.h"
#include "SquareDeal.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgTrade::CMsgTrade()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgTrade::~CMsgTrade()
{
}

//-----------------------------------------------------
BOOL CMsgTrade::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_TRADE != this->GetType())
    {
        return false;
    }
    return true;
}

//-----------------------------------------------------
BOOL CMsgTrade::Create(unsigned short usAction, OBJID id)
{
    // reset
    this->Init();
    // fill
    m_unMsgType		= _MSG_TRADE;
    m_unMsgSize		= sizeof(MSG_Info);
    m_pInfo->usAction	= usAction;
    m_pInfo->id			= id;
    return true;
}

//-----------------------------------------------------
void CMsgTrade::Process(CGameSocket* pSocket)
{
#ifdef _MYDEBUG
    ::LogSave("Process CMsgTrade, id:%u, action:%u",
              m_pInfo->id,
              m_pInfo->usAction);
#endif
    //	DEBUG_PROCESSMSG("TRADE",m_pInfo->id,m_pInfo->usAction,"",m_pInfo->nData,0);
    CUserPtr pUser	= UserManager()->GetUser(this);
    if (!pUser)
    {
        return;
    }
    /*	if (pUser->GetLockKey())
    	{
    		pUser->SendSysMsg("请先打开保护锁。");
    		return;
    	}

    	if (pUser->IsFighting())
    	{
    		pUser->SendSysMsg("错误：战斗中不能执行交易指令！");
    		return;
    	}

    	if (pUser->GetTeam())
    	{
    		pUser->SendSysMsg("错误：组队状态不能执行交易指令！");
    		return;
    	}
    */
    DEBUG_TRY		// VVVVVVVVVVVVVVVVVVVVVVVVVVV
    switch(m_pInfo->usAction)
    {
    case _TRADE_APPLY:
        {
            if (m_pInfo->id == ID_NONE)
            {
                return;
            }
            OBJID	idTarget = m_pInfo->id;
            CUser* pTarget = NULL;
            if (!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, idTarget, IPP_OF(pTarget)))
            {
                pUser->SendSysMsg(STR_NO_TRADE_TARGET);
                return;
            }
            ISquareDeal* pSquareDeal = pUser->QuerySquareDeal();
            if (pSquareDeal)
            {
                LOGWARNING("玩家重复交易!");
                pSquareDeal->Release();
                return ;
            }
            if (pTarget->QuerySquareDeal())
            {
                pUser->SendSysMsg(STR_TARGET_TRADING);
                return ;
            }
            if (!pTarget->FetchApply(CUser::APPLY_TRADE, pUser->GetID()))
            {
                pUser->SetApply(CUser::APPLY_TRADE, idTarget);
                IF_NOT(this->Create(_TRADE_APPLY, pUser->GetID()))
                return;
                pTarget->SendMsg(this);
                pUser->SendSysMsg(STR_TRADING_REQEST_SENT);
                return;		// ok
            }
            if (!pUser->CreateSquareDeal(pTarget))		// 同时发送消息
            {
                pUser->SendSysMsg(STR_TARGET_TRADING);
                return;
            }
        }
        break;
    case _TRADE_QUIT:
        {
            ISquareDeal* pSquareDeal =	pUser->QuerySquareDeal();
            if (pSquareDeal)
            {
                // inform target
                CUserPtr pTarget	= pSquareDeal->GetTarget();
                if (pTarget)
                {
                    //					CHECK(this->Create(_TRADE_FALSE, 0));
                    //					pTarget->SendMsg(this);
                }
                // feed back
                {
                    //					CHECK(this->Create(_TRADE_FALSE, 0));
                    //					pUser->SendMsg(this);
                }
                pSquareDeal->Release();
            }
        }
        break;
    case _TRADE_OPEN:
        {
            ASSERT(!"_TRADE_OPEN");
        }
        break;
    case _TRADE_SUCCESS:
        {
            ASSERT(!"_TRADE_SUCCESS");
        }
        break;
    case _TRADE_FALSE:
        {
            ASSERT(!"_TRADE_FALSE");
        }
        break;
    case _TRADE_ADDITEM:
        {
            ISquareDeal* pSquareDeal =	pUser->QuerySquareDeal();
            if (!pSquareDeal)
            {
                return;
            }
            CItemPtr pItem	= pUser->GetItem(m_pInfo->id);
            if (!pItem || !pItem->IsExchangeEnable())
            {
                pUser->SendSysMsg(STR_NOT_FOR_TRADE);
                IF_OK(this->Create(_TRADE_ADDITEMFAIL, m_pInfo->id))
                pUser->SendMsg(this);
                return;
            }
            if (pItem->IsEudemon())
            {
                pUser->CallBackEudemon(pItem->GetID());
                pUser->DetachEudemon(pItem);
            }
            if (!pSquareDeal->AddItem(pItem))
            {
                IF_OK(this->Create(_TRADE_ADDITEMFAIL, m_pInfo->id))
                pUser->SendMsg(this);
                return;
            }
        }
        break;
    case _TRADE_ADDMONEY:
        {
            ISquareDeal* pSquareDeal =	pUser->QuerySquareDeal();
            if (!pSquareDeal)
            {
                return;
            }
            int	nAllMoney = pSquareDeal->AddMoney(m_pInfo->nData);		// return allmoney
            CHECK(this->Create(_TRADE_SELFMONEYALL, nAllMoney));
            pUser->SendMsg(this);
            if (nAllMoney <= 0)
            {
                pUser->AddAttrib(_USERATTRIB_MONEY, 0, SYNCHRO_TRUE);    // recruit money(note: because set money once, so no conflict
            }
        }
        break;
    case _TRADE_OK:
        {
            ISquareDeal* pSquareDeal =	pUser->QuerySquareDeal();
            if (!pSquareDeal)
            {
                return;
            }
            if (pSquareDeal->ClickOK())		// return true: 交易完成
            {
                pSquareDeal->Release();
            }
        }
        break;
    default:
        break;
    }
    DEBUG_CATCH2("switch(MSGTRADE) [%d]", m_pInfo->usAction)		// AAAAAAAAAAAAAAAA
}
