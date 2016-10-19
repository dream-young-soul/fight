
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Bank.h: interface for the CBank class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BANK_H__5939BA75_0046_4474_8E55_5009B0B9A993__INCLUDED_)
#define AFX_BANK_H__5939BA75_0046_4474_8E55_5009B0B9A993__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"

class CBank
{
public:
    CBank();
    virtual ~CBank();
private:
    OBJID m_idBank;
    DWORD m_dwMoney;
public:
    void	Open(OBJID idBank) {m_idBank = idBank; m_dwMoney = 0;}
    void	Close() {m_idBank = ID_NONE; m_dwMoney = 0;}
    DWORD	GetMoney() {return m_dwMoney;}
    void	SetMoney(DWORD dwMoney) {m_dwMoney = dwMoney;}
    OBJID	GetId() {return m_idBank;}
};

#endif // !defined(AFX_BANK_H__5939BA75_0046_4474_8E55_5009B0B9A993__INCLUDED_)
