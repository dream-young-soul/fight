
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Booth.h: interface for the CBooth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOOTH_H__50B7A090_A2BB_4D63_8806_74FDFDA86C51__INCLUDED_)
#define AFX_BOOTH_H__50B7A090_A2BB_4D63_8806_74FDFDA86C51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Item.h"
class CBooth
{
public:
    CBooth();
    virtual ~CBooth();

public:
    DWORD	GetItemAmount();
    CItem*	GetItemByIndex(int nIndex);
    BOOL	Open(OBJID idBooth);
    void	Close();
    void	AddItem(CItem* pItem);
    void	DelItem(OBJID idItem);
    void	BuyItem(OBJID idItem);
    void	SetPrice(OBJID idItem, DWORD dwPrice);
    char*	GetName() {return m_szName;}
    OBJID	GetID() {return m_idNpc;}
private:
    OBJID		m_idNpc;
    DEQUE_ITEM	m_dequeItem;
    char		m_szName[32];
};

#endif // !defined(AFX_BOOTH_H__50B7A090_A2BB_4D63_8806_74FDFDA86C51__INCLUDED_)

