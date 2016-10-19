
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TxtCommand.h: interface for the CTxtCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TXTCOMMAND_H__547B930E_1C73_4DDD_9D4D_1555CE75ED50__INCLUDED_)
#define AFX_TXTCOMMAND_H__547B930E_1C73_4DDD_9D4D_1555CE75ED50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;
typedef vector<char*>DEQUE_TXTCOMMAND;
class CTxtCommand
{
public:
    CTxtCommand();
    virtual ~CTxtCommand();

private:
    DEQUE_TXTCOMMAND m_setCommand;

public:
    void	AddCommand(char* pszCommand);
    void	Clear();
    int		GetAmount();
    char*	GetTxt(int nIndex);

    void	SetHeroPos(char* pszCommand);
    void	OpenShop(char* pszCommand);
    void	CloseShop();
    void	ListShop();
    void	ListPlayer();
    void	ListPackage();
    void	Buy(char* pszCommand);
    void	Sell(char* pszCommand);
    void	SetMsgLine(char* pszCommand);
    void	ShowStatus();
    void	Test(char* pszCommand);
    void	Say(char* pszCommand);
    void	Tell(char* pszCommand);
    void	MakeFriend(char* pszCommand);
    void	BreakOffFriend(char* pszCommand);
    void	ListFriend();
};

#endif // !defined(AFX_TXTCOMMAND_H__547B930E_1C73_4DDD_9D4D_1555CE75ED50__INCLUDED_)
