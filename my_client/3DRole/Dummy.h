
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Dummy.h: interface for the CDummy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUMMY_H__DED7C5B4_516C_41BB_82CD_B9BB673F4DE1__INCLUDED_)
#define AFX_DUMMY_H__DED7C5B4_516C_41BB_82CD_B9BB673F4DE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Player.h"
#include "Item.h"
class CDummy
{
public:
    CDummy();
    virtual ~CDummy();
private:
    int		m_nCurrentDummyAction;
    int		m_nCurrentDummyDir;
    int		m_nDummyFrameIndex;
    BOOL	m_bDummyPlay;
    BOOL	m_bLock;
    DWORD	m_dwDummyTimer;
    CPlayer m_objDummy;
    OBJID	m_idDummyTemplate;
    CItem*	m_pEquipment[_MAX_EQUIPMENT];
    char	m_szName[_MAX_NAMESIZE];
    char	m_szMate[_MAX_NAMESIZE];
public:
    void	ShowDummy(CMyPos posScr, int nZoomPercent = 100, int nRotateX = -45);
    void	SetDummyAction(int nAction);
    void	SetDummyDir(int nDir);
    void	PlayDummy();
    void	PauseDummy();
    void	ProcessDummy();
    BOOL	LockDummy(OBJID idPlayer = ID_NONE);

    int		GetAction() {return m_nCurrentDummyAction;}
    int		GetDir() {return m_nCurrentDummyDir;}
    int		GetFrameIndex() {return m_nDummyFrameIndex;}
    char*	GetName() {return m_szName;}
    char*	GetMate() {return m_szMate;}
    void	SetMate(char* pszMate) {if (pszMate) { strcpy(m_szMate, pszMate); }}
    CItem*	GetEquipment(int nPosition)	{ MYASSERT(nPosition - 1 >= 0 && nPosition - 1 < _MAX_EQUIPMENT); return m_pEquipment[nPosition-1]; }
    BOOL	SetEquipment(int nPosition, CItem* pItem);
    void	ClearEquipment();
};

#endif // !defined(AFX_DUMMY_H__DED7C5B4_516C_41BB_82CD_B9BB673F4DE1__INCLUDED_)
