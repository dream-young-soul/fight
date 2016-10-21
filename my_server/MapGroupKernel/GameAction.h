
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// GameAction.h: interface for the CGameAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEACTION_H__7F7A15BB_A4E8_4B07_A932_4B9ABA6BC9EC__INCLUDED_)
#define AFX_GAMEACTION_H__7F7A15BB_A4E8_4B07_A932_4B9ABA6BC9EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ActionManagerConfig.h"
class CGameAction
{
protected:
    CGameAction(PROCESS_ID idProcess)				{ m_idProcess = idProcess; }
    virtual ~CGameAction()							{}
public:
    static CGameAction* CreateNew(PROCESS_ID idProcess)		{ return new CGameAction(idProcess); }
    void	Release()								{ delete this; }

public:
    bool	ProcessAction	(OBJID idAction, CUser* pUser = NULL, IRole* pRole = NULL, CItem* pItem = NULL, LPCTSTR pszAccept = NULL);
protected:
    void	ReplaceAttrStr		(char* pszTarget, const char* pszSource, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept = NULL);
    bool	ProcessActionSys	(tagAction* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
    bool	ProcessActionNpc	(tagAction* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
    bool	ProcessActionMap	(tagAction* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
    bool	ProcessActionItemOnly	(tagAction* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
    bool	ProcessActionItem	(tagAction* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
    bool	ProcessActionNpcOnly	(tagAction* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
    bool	ProcessActionSyn	(tagAction* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
    bool	ProcessActionUser	(tagAction* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
    bool	ProcessActionEvent	(tagAction* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
    bool	ProcessActionWanted	(tagAction* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
    bool	ProcessActionMonster(tagAction* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
    bool	ProcessActionMagic	(tagAction* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pazAccept);

private:
    CUser*	m_pUser;
    IRole*	m_pRole;
    CItem*	m_pItem;

protected:
    PROCESS_ID	m_idProcess;

protected: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_GAMEACTION_H__7F7A15BB_A4E8_4B07_A932_4B9ABA6BC9EC__INCLUDED_)
