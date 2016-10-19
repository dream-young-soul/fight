
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// SynManager.h: interface for the CSynWorldManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNMANAGER_H__500BB831_F770_4231_8391_E7B38A5EF78A__INCLUDED_2)
#define AFX_SYNMANAGER_H__500BB831_F770_4231_8391_E7B38A5EF78A__INCLUDED_2

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "protocol.h"
#include "GameObjSet.h"
#include "SynWorldShell.h"
#include "I_mydb.h"

//////////////////////////////////////////////////////////////////////
typedef	IGameObjSet<CSyndicateWorld>		ISynWorldSet;
typedef	CGameObjSet<CSyndicateWorld>		CSynWorldSet;

//////////////////////////////////////////////////////////////////////
class ISynMngModify
{
public:
    virtual OBJID	CreateSyndicate(const CreateSyndicateInfo* pInfo)		PURE_VIRTUAL_FUNCTION_0
    virtual bool	DestroySyndicate(OBJID idSyn, OBJID idTarget = ID_NONE)	PURE_VIRTUAL_FUNCTION_0
    virtual bool	CombineSyndicate(OBJID idSyn, OBJID idTarget)			PURE_VIRTUAL_FUNCTION_0
};

//////////////////////////////////////////////////////////////////////
class CSynWorldManager : ISynMngModify
{
protected:
    virtual ~CSynWorldManager();
public:
    CSynWorldManager();
    ULONG	Release()				{ delete this; return 0; }
    bool	Create(IDatabase* pDb);
    CSyndicateWorld* QuerySyndicate(OBJID idSyn)		{ return m_setSyn->GetObj(idSyn); }
    ISynMngModify* QueryModify()	{ return &m_obj; }
    ISynMngModify* QuerySynchro()	{ return (ISynMngModify*)this; }

protected:
    OBJID	CreateSyndicate(const CreateSyndicateInfo* pInfo);
    bool	DestroySyndicate(OBJID idSyn, OBJID idTarget = ID_NONE);
    bool	CombineSyndicate(OBJID idSyn, OBJID idTarget);

protected:
    ISynWorldSet*		m_setSyn;
    IRecordset*			m_pDefault;

protected:
    class XSynMngModify : public ISynMngModify
    {
    public:
        OBJID	CreateSyndicate(const CreateSyndicateInfo* pInfo);
        bool	DestroySyndicate(OBJID idSyn, OBJID idTarget = ID_NONE);
        bool	CombineSyndicate(OBJID idSyn, OBJID idTarget);
    protected:
        CSynWorldManager*	This()	{ return m_pOwner; }
        CSynWorldManager*	m_pOwner;
        friend class CSynWorldManager;
    } m_obj;
    friend class CSynWorldManager::XSynMngModify;

public: // ctrl
    CSyndicateWorld* GetSynByIndex(int nIndex);
    int GetSynAmount();
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_SYNMANAGER_H__500BB831_F770_4231_8391_E7B38A5EF78A__INCLUDED_2)
