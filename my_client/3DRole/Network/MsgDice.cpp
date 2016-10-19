
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgDice.cpp: implementation of the CMsgDice class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgDice.h"
#include "Hero.h"
#include "gamemsg.h"
#include "gamedataset.h"
#include "GamePlayerSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgDice::CMsgDice()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgDice::~CMsgDice()
{
}
//--------------------------------------------------------------
BOOL	CMsgDice::Create		(char* pMsgBuf, DWORD dwSize)
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
//--------------------------------------------------------------
BOOL	CMsgDice::Create		(OBJID idDice, int nAction, DWORD dwMoney, int nType)
{
    this->Init();
    m_pInfo->ucAmount = 1;
    m_pInfo->ucAction = nAction;
    m_pInfo->Data[0].dwData = dwMoney;
    m_pInfo->Data[0].ucType = nType;
    m_pInfo->idDice = idDice;
    m_pInfo->unMsgType = _MSG_DICE;
    m_pInfo->unMsgSize = sizeof(MSG_Info);
    return true;
}
//--------------------------------------------------------------
void	CMsgDice::Process		(void* pInfo)
{
    switch(m_pInfo->ucAction)
    {
    case _ACTION_CHIPIN:
        if (g_objHero.m_objDiceManager.GetID() == m_pInfo->idDice)
        {
            for(int i = 0; i < m_pInfo->ucAmount; i ++)
            {
                DWORD dwTotal = g_objHero.m_objDiceManager.GetTotalAnte(m_pInfo->Data[i].ucType);
                g_objHero.m_objDiceManager.SetTotalAnte(m_pInfo->Data[i].ucType, dwTotal + m_pInfo->Data[i].dwData);
            }
            ::PostCmd(CMD_DICE_FLASH_ANTES);
        }
        break;
    case _ACTION_CHIPIN_CONFIRM:
        if (g_objHero.m_objDiceManager.GetID() == m_pInfo->idDice)
        {
            for(int i = 0; i < m_pInfo->ucAmount; i ++)
            {
                DWORD dwTotal = g_objHero.m_objDiceManager.GetTotalAnte(m_pInfo->Data[i].ucType);
                g_objHero.m_objDiceManager.SetTotalAnte(m_pInfo->Data[i].ucType, dwTotal + m_pInfo->Data[i].dwData);
                DWORD dwSelf = g_objHero.m_objDiceManager.GetSelfAnte(m_pInfo->Data[i].ucType);
                g_objHero.m_objDiceManager.SetSelfAnte(m_pInfo->Data[i].ucType, dwSelf + m_pInfo->Data[i].dwData);
            }
            ::PostCmd(CMD_DICE_FLASH_ANTES);
        }
        break;
    case _ACTION_CANCELCHIP:
        if (g_objHero.m_objDiceManager.GetID() == m_pInfo->idDice)
        {
            for(int i = 0; i < m_pInfo->ucAmount; i ++)
            {
                DWORD dwTotal = g_objHero.m_objDiceManager.GetTotalAnte(m_pInfo->Data[i].ucType);
                g_objHero.m_objDiceManager.SetTotalAnte(m_pInfo->Data[i].ucType, dwTotal > m_pInfo->Data[i].dwData ? dwTotal - m_pInfo->Data[i].dwData : 0);
            }
            ::PostCmd(CMD_DICE_FLASH_ANTES);
        }
        break;
    case _ACTION_CANCELCHIP_CONFIRM:
        if (g_objHero.m_objDiceManager.GetID() == m_pInfo->idDice)
        {
            for(int i = 0; i < m_pInfo->ucAmount; i ++)
            {
                DWORD dwTotal = g_objHero.m_objDiceManager.GetTotalAnte(m_pInfo->Data[i].ucType);
                g_objHero.m_objDiceManager.SetTotalAnte(m_pInfo->Data[i].ucType, dwTotal > m_pInfo->Data[i].dwData ? dwTotal - m_pInfo->Data[i].dwData : 0);
                DWORD dwSelf = g_objHero.m_objDiceManager.GetSelfAnte(m_pInfo->Data[i].ucType);
                g_objHero.m_objDiceManager.SetSelfAnte(m_pInfo->Data[i].ucType, dwSelf > m_pInfo->Data[i].dwData ? dwSelf - m_pInfo->Data[i].dwData : 0);
            }
            ::PostCmd(CMD_DICE_FLASH_ANTES);
        }
        break;
    case _ACTION_BEGINCHIP:
        if (g_objHero.m_objDiceManager.GetID() == m_pInfo->idDice)
        {
            OBJID idStr = 100131;
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));
            g_objHero.m_objDiceManager.ClearBonus();
            g_objHero.m_objDiceManager.ClearAnteList();
            g_objHero.m_objDiceManager.ClearBonus();
            g_objHero.m_objDiceManager.BeginCountDown(m_pInfo->ucTime);
            ::PostCmd(CMD_DICE_FLASH_ANTES);
        }
        break;
    case _ACTION_ENDCHIP:
        if (g_objHero.m_objDiceManager.GetID() == m_pInfo->idDice)
        {
            OBJID idStr = 100132;
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));
            ::PostCmd(CMD_DICE_FLASH_ANTES);
        }
        break;
    case _ACTION_DICE:
        {
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_pInfo->idDice);
            if (pPlayer && pPlayer->m_pIRoleView)
            {
                pPlayer->m_pIRoleView->SetVariable(_VAR_DICE0, m_pInfo->Data[0].ucDice[0]);
                pPlayer->m_pIRoleView->SetVariable(_VAR_DICE1, m_pInfo->Data[0].ucDice[1]);
                pPlayer->m_pIRoleView->SetVariable(_VAR_DICE2, m_pInfo->Data[0].ucDice[2]);
            }
        }
        if (g_objHero.m_objDiceManager.GetID() == m_pInfo->idDice)
        {
            OBJID idStr = 0;
            int nTotal = m_pInfo->Data[0].ucDice[0] + m_pInfo->Data[0].ucDice[1] + m_pInfo->Data[0].ucDice[2];
            if (nTotal > 10)
            {
                idStr = 100133;
            }
            else
            {
                idStr = 100134;
            }
            char szTemp[256];
            sprintf(szTemp, g_objGameDataSet.GetStr(idStr), m_pInfo->Data[0].ucDice[0], m_pInfo->Data[0].ucDice[1], m_pInfo->Data[0].ucDice[2], nTotal);
            g_objGameMsg.AddMsg(szTemp);
            g_objHero.m_objDiceManager.SetDice(0, m_pInfo->Data[0].ucDice[0]);
            g_objHero.m_objDiceManager.SetDice(1, m_pInfo->Data[0].ucDice[1]);
            g_objHero.m_objDiceManager.SetDice(2, m_pInfo->Data[0].ucDice[2]);
            g_objHero.m_objDiceManager.ClearAnteList();
            ::PostCmd(CMD_DICE_FLASH_ANTES);
        }
        break;
    }
}
//--------------------------------------------------------------
