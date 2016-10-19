
//**********************************************************
// 代码编辑器
//**********************************************************

// DiceManager.h: interface for the DiceManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DICEMANAGER_H__5C431B5C_E9D0_4F22_B3CC_DCECF3A03CEC__INCLUDED_)
#define AFX_DICEMANAGER_H__5C431B5C_E9D0_4F22_B3CC_DCECF3A03CEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "BaseFunc.h"
#include "ImageString.h"
using namespace std;
typedef struct
{
    char	szName[_MAX_NAMESIZE];
    DWORD	dwBonus;
} DicePlayer;

const int _MAX_ANTE_SORT_AMOUNT = 23;
const int _DICE_AMOUNT = 3;
enum
{
    DICE_STATUS_CHIPIN,			// 投注时间
    DICE_STATUS_STOP_CHIPIN,	// 买定离手
    DICE_STATUS_DICE,			// 骰子信息时间
    DICE_STATUS_BONUS,			// 报奖金时间
};
enum
{
    DICE_VALUE_SMALL	= 0,
    DICE_VALUE_BIG		= 1,
    DICE_VALUE_111		= 2,
    DICE_VALUE_222		= 3,
    DICE_VALUE_333		= 4,
    DICE_VALUE_444		= 5,
    DICE_VALUE_555		= 6,
    DICE_VALUE_666		= 7,
    DICE_VALUE_4		= 8,
    DICE_VALUE_5		= 9,
    DICE_VALUE_6		= 10,
    DICE_VALUE_7		= 11,
    DICE_VALUE_8		= 12,
    DICE_VALUE_9		= 13,
    DICE_VALUE_10		= 14,
    DICE_VALUE_11		= 15,
    DICE_VALUE_12		= 16,
    DICE_VALUE_13		= 17,
    DICE_VALUE_14		= 18,
    DICE_VALUE_15		= 19,
    DICE_VALUE_16		= 20,
    DICE_VALUE_17		= 21,
    DICE_VALUE_ALL		= 22,
};

class DiceManager
{
public:
    DiceManager();
    virtual ~DiceManager();

private:
    OBJID m_idDic;
    DWORD m_dwSelfAnte[_MAX_ANTE_SORT_AMOUNT];
    DWORD m_dwTotalAnte[_MAX_ANTE_SORT_AMOUNT];
    int	  m_nStatus;
    DWORD m_dwDice[3];
    vector<DicePlayer*> m_setPlayerList;
    DWORD m_dwTimeCountBegin;
    BOOL  m_bCountDown;
    CImageString	m_objImgString;
public:
    int		GetStatus();
    void	SetStatus(int nStatus);
    OBJID	GetID();
    void	SetID(OBJID id);
    DWORD	GetSelfAnte(int nAnteType);
    void	SetSelfAnte(int nAnteType, DWORD dwAnte);
    DWORD	GetTotalAnte(int nAnteType);
    void	SetTotalAnte(int nAnteType, DWORD dwAnte);
    DWORD	GetDice(int nIndex);
    void	SetDice(int nIndex, DWORD dwValue);
    void	UpdatePlayer(char* pszName, DWORD dwMoney = 0);
    void	DelPlayer(char* pszName);
    void	ClearPlayer();
    int		GetPlayerListAmount();
    DicePlayer* GetPlayerListByIndex(int nIndex);

    void	Open(OBJID idDic, BOOL bSendMsg = true);
    void	Close(BOOL bSendMsg = true);

    void	ChipIn(int nType, DWORD dwMoney);
    void	CancelChipIn(int nType);
    void	ClearAnteList();
    void	ClearBonus();
    void	BeginCountDown(DWORD dwTime);
    void	Process();
    void	Show();
};

#endif // !defined(AFX_DICEMANAGER_H__5C431B5C_E9D0_4F22_B3CC_DCECF3A03CEC__INCLUDED_)
