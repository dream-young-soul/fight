
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DiceManager.cpp: implementation of the DiceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "DiceManager.h"
#include "AllMsg.h"
#include "Hero.h"
#include "GamePlayerset.h"

const DWORD DICEBET_TIME = 50 * 1000;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DiceManager::DiceManager()
{
    this->m_idDic = ID_NONE;
    m_bCountDown = false;
}

DiceManager::~DiceManager()
{
    this->ClearPlayer();
}

//---------------------------------------------------------------------
int		DiceManager::GetStatus()
{
    return this->m_nStatus;
}
//---------------------------------------------------------------------
void	DiceManager::SetStatus(int nStatus)
{
    this->m_nStatus = nStatus;
}
//---------------------------------------------------------------------
OBJID	DiceManager::GetID()
{
    return this->m_idDic;
}
//---------------------------------------------------------------------
void	DiceManager::SetID(OBJID id)
{
    this->m_idDic = id;
}
//---------------------------------------------------------------------
DWORD	DiceManager::GetSelfAnte(int nAnteType)
{
    if ((nAnteType < 0) || (nAnteType >= _MAX_ANTE_SORT_AMOUNT))
    {
        return 0;
    }
    if (nAnteType != DICE_VALUE_ALL)
    {
        return this->m_dwSelfAnte[nAnteType];
    }
    else
    {
        DWORD dwTatol = 0;
        for(int i = 0; i < DICE_VALUE_ALL; i ++)
        {
            dwTatol += m_dwSelfAnte[i];
        }
        return dwTatol;
    }
    return 0;
}
//---------------------------------------------------------------------
void	DiceManager::SetSelfAnte(int nAnteType, DWORD dwAnte)
{
    if ((nAnteType < 0) || (nAnteType >= _MAX_ANTE_SORT_AMOUNT))
    {
        return;
    }
    this->m_dwSelfAnte[nAnteType] = dwAnte;
}
//---------------------------------------------------------------------
DWORD	DiceManager::GetTotalAnte(int nAnteType)
{
    if ((nAnteType < 0) || (nAnteType >= _MAX_ANTE_SORT_AMOUNT))
    {
        return 0;
    }
    if (nAnteType != DICE_VALUE_ALL)
    {
        return this->m_dwTotalAnte[nAnteType];
    }
    else
    {
        DWORD dwTatol = 0;
        for(int i = 0; i < DICE_VALUE_ALL; i ++)
        {
            dwTatol += m_dwTotalAnte[i];
        }
        return dwTatol;
    }
    return 0;
}
//---------------------------------------------------------------------
void	DiceManager::SetTotalAnte(int nAnteType, DWORD dwAnte)
{
    if ((nAnteType < 0) || (nAnteType >= _MAX_ANTE_SORT_AMOUNT))
    {
        return;
    }
    this->m_dwTotalAnte[nAnteType] = dwAnte;
}
//---------------------------------------------------------------------
DWORD	DiceManager::GetDice(int nIndex)
{
    if ((nIndex < 0) || (nIndex >= _DICE_AMOUNT))
    {
        return 0;
    }
    return this->m_dwDice[nIndex];
}
//---------------------------------------------------------------------
void	DiceManager::SetDice(int nIndex, DWORD dwValue)
{
    if ((nIndex < 0) || (nIndex >= _DICE_AMOUNT))
    {
        return;
    }
    m_dwDice[nIndex] = dwValue;
}
//---------------------------------------------------------------------
void	DiceManager::UpdatePlayer(char* pszName, DWORD dwMoney)
{
    if (!pszName)
    {
        return;
    }
    // search ...
    int nAmount = this->m_setPlayerList.size();
    for(int i = 0; i < nAmount; i ++)
    {
        DicePlayer* pPlayer = this->m_setPlayerList[i];
        if (pPlayer && strcmp(pPlayer->szName, pszName) == 0)
        {
            pPlayer->dwBonus = dwMoney;
            return;
        }
    }
    // not found ...
    DicePlayer* pPlayer = new DicePlayer;
    if (pPlayer)
    {
        strcpy(pPlayer->szName, pszName);
        pPlayer->dwBonus = dwMoney;
        this->m_setPlayerList.push_back(pPlayer);
    }
}
//---------------------------------------------------------------------
void	DiceManager::DelPlayer(char* pszName)
{
    if (!pszName)
    {
        return;
    }
    // search ...
    int nAmount = this->m_setPlayerList.size();
    for(int i = nAmount - 1; i  >= 0; i --)
    {
        DicePlayer* pPlayer = this->m_setPlayerList[i];
        if (pPlayer && strcmp(pPlayer->szName, pszName) == 0)
        {
            SAFE_DELETE(pPlayer);
            this->m_setPlayerList.erase(m_setPlayerList.begin() + i);
        }
    }
}
//---------------------------------------------------------------------
void	DiceManager::ClearPlayer()
{
    int nAmount = this->m_setPlayerList.size();
    for(int i = 0; i < nAmount; i ++)
    {
        DicePlayer* pPlayer = this->m_setPlayerList[i];
        SAFE_DELETE(pPlayer);
    }
    this->m_setPlayerList.clear();
}
//---------------------------------------------------------------------
int		DiceManager::GetPlayerListAmount()
{
    return this->m_setPlayerList.size();
}
//---------------------------------------------------------------------
DicePlayer* DiceManager::GetPlayerListByIndex(int nIndex)
{
    int nAmount = this->m_setPlayerList.size();
    if (nIndex <  0 || nIndex >= nAmount)
    {
        return NULL;
    }
    return this->m_setPlayerList[nIndex];
}
//---------------------------------------------------------------------
void	DiceManager::Open(OBJID idDic, BOOL bSendMsg)
{
    if (bSendMsg)
    {
        if (idDic == ID_NONE)
        {
            return;
        }
        CMsgName msg;
        if (msg.Create(NAMEACT_ADDDICE_PLAYER, g_objHero.GetName(), idDic))
        {
            msg.Send();
        }
    }
    else
    {
        m_idDic = idDic;
        this->ClearAnteList();
        ::PostCmd(CMD_DICE_JOIN);
        m_bCountDown = true;
    }
    ::PostCmd(CMD_DICE_FLASH_ANTES);
    ::PostCmd(CMD_DICE_FLASH_PLAYERLIST);
}
//---------------------------------------------------------------------
void	DiceManager::Close(BOOL bSendMsg)
{
    if (m_idDic == ID_NONE)
    {
        return;
    }
    if (bSendMsg)
    {
        CMsgName msg;
        if (msg.Create(NAMEACT_DELDICE_PLAYER, g_objHero.GetName(), m_idDic))
        {
            msg.Send();
        }
        m_idDic = ID_NONE;
        this->ClearPlayer();
        this->ClearAnteList();
        ::PostCmd(CMD_DICE_QUIT);
    }
    ::PostCmd(CMD_DICE_FLASH_ANTES);
    ::PostCmd(CMD_DICE_FLASH_PLAYERLIST);
}
//---------------------------------------------------------------------
void	DiceManager::ChipIn(int nType, DWORD dwMoney)
{
    if (m_idDic == ID_NONE)
    {
        return;
    }
    if ((nType < DICE_VALUE_SMALL) && (nType > DICE_VALUE_17))
    {
        return;
    }
    if (dwMoney > g_objHero.GetMoney())
    {
        return;
    }
    CMsgDice msg;
    if (msg.Create(m_idDic, _ACTION_CHIPIN, dwMoney, nType))
    {
        msg.Send();
    }
}
//---------------------------------------------------------------------
void	DiceManager::CancelChipIn(int nType)
{
    if (m_idDic == ID_NONE)
    {
        return;
    }
    if ((nType < DICE_VALUE_SMALL) && (nType > DICE_VALUE_17))
    {
        return;
    }
    if (this->GetSelfAnte(nType) == 0)
    {
        return;
    }
    CMsgDice msg;
    if (msg.Create(m_idDic, _ACTION_CANCELCHIP, this->GetSelfAnte(nType), nType))
    {
        msg.Send();
    }
}
//---------------------------------------------------------------------
void	DiceManager::ClearAnteList()
{
    for(int i = 0; i < _MAX_ANTE_SORT_AMOUNT; i ++)
    {
        m_dwSelfAnte[i] = 0;
        m_dwTotalAnte[i] = 0;
    }
}
//---------------------------------------------------------------------
void	DiceManager::ClearBonus()
{
    int nAmount = m_setPlayerList.size();
    for(int i = 0; i < nAmount; i ++)
    {
        DicePlayer* pPlayer = m_setPlayerList[i];
        if (pPlayer)
        {
            pPlayer->dwBonus = 0;
        }
    }
}
//---------------------------------------------------------------------
void	DiceManager::Process()
{
    if (m_idDic == ID_NONE)
    {
        return;
    }
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idDic);
    if (!pPlayer)
    {
        this->Close();
    }
}

void	DiceManager::BeginCountDown(DWORD dwTimeLeft)
{
    DWORD dwTimePassBy = DICEBET_TIME - __min(DICEBET_TIME, dwTimeLeft * 1000);
    m_dwTimeCountBegin = ::TimeGet() - dwTimePassBy;
    m_bCountDown = true;
}

//---------------------------------------------------------------------
void	DiceManager::Show()
{
    if (m_idDic == ID_NONE)
    {
        return;
    }
    const DWORD dwTotalTime = 50 * 1000;
    if (m_bCountDown)
    {
        DWORD dwTimeNow = ::TimeGet();
        if (m_dwTimeCountBegin + dwTotalTime > dwTimeNow)
        {
            DWORD dwLeftTime = dwTotalTime + m_dwTimeCountBegin - dwTimeNow;
            if (dwLeftTime > dwTotalTime)
            {
                return;
            }
            char szStr[64];
            sprintf(szStr, "%u", dwLeftTime / 1000);
            CMyPos posShow = {_SCR_WIDTH - 350, 30};
            m_objImgString.SetShowPos(posShow);
            m_objImgString.SetColor(0xffffff00, 0xff);
            m_objImgString.SetScalePercent(150);
            m_objImgString.SetString(szStr);
            m_objImgString.Show();
        }
    }
}
