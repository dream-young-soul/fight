
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Package.h: interface for the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKAGE_H__97D8AEC6_7C3B_4935_9FFD_364EE8198F85__INCLUDED_)
#define AFX_PACKAGE_H__97D8AEC6_7C3B_4935_9FFD_364EE8198F85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Item.h"

enum // type
{
    PACKAGE_STORAGE			= 10,	// ¼Ä´æ´¦
    PACKAGE_TRUNK			= 20,	// Ïä×Ó
    PACKAGE_CHEST			= 30,	// °Ù±¦ÄÒ
    PACKAGE_EUDEMON_BROODER = 40,	// »ÃÊÞ·õ»¯Æ÷
    PACKAGE_EUDEMON_STORAGE = 50,	// »ÃÊÞ´æ´¢²Ö¿â
    PACKAGE_AUCTION_STORAGE = 60,	// ÅÄÂôNPC
};

class CPackage
{
public:
    CPackage();
    virtual ~CPackage();

private:
    DEQUE_ITEM	m_setItem;
    int			m_nType;
    OBJID		m_idPackage;
    int			m_nMaxItemAmount;
public:
    void	Reset();
    int		GetItemAmount();
    CItem*	GetItem(OBJID id);
    CItem*	GetItemByIndex(int nIndex);
    void	AddItem(CItem* pItem);
    void	DelItem(OBJID idItem);
    void	SetId(OBJID idPackage);
    OBJID	GetId();
    int		GetType();
    int		GetMaxItemAmount() {return m_nMaxItemAmount;}
    void	SetMaxItemAmount(int nAmount) {m_nMaxItemAmount = nAmount;}
    void	SetType(int nType);
};

#endif // !defined(AFX_PACKAGE_H__97D8AEC6_7C3B_4935_9FFD_364EE8198F85__INCLUDED_)
