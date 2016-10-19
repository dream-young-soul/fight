
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// BoothManager.h: interface for the CBoothManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOOTHMANAGER_H__18780E75_C33E_46DC_9296_113A56A4820F__INCLUDED_)
#define AFX_BOOTHMANAGER_H__18780E75_C33E_46DC_9296_113A56A4820F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Item.h"

class CBoothManager
{
public:
    CBoothManager();
    virtual ~CBoothManager();

public:
    DWORD	GetItemAmount();
    CItem*	GetItemByIndex(int nIndex);
    BOOL	Open(OBJID idBooth = ID_NONE);
    void	Close();
    void	AddItem(OBJID idItem, DWORD dwBoothPrice, BOOL bMsg = true);
    void	DelItem(OBJID idItem, BOOL bMsg = true);
    OBJID	GetID() {return m_idNpc;}
    void	PauseBooth();
    BOOL	ContinueBooth(OBJID idNpc = ID_NONE);
    void	SetActive(BOOL bActive) {m_bActive = bActive;}
    BOOL	IsActive() {return m_bActive;}
    void	SetCryOut(char* pszStr);
    BOOL	BeShowOpenDialog();
    BOOL	BeShowCloseDialog();
private:
    OBJID		m_idNpc;
    BOOL		m_bActive;
    char		m_szCryOut[512];
public:
    DEQUE_ITEM	m_dequeItem;
};

#endif // !defined(AFX_BOOTHMANAGER_H__18780E75_C33E_46DC_9296_113A56A4820F__INCLUDED_)
