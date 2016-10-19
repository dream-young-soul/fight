
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NpcTask.h: interface for the CNpcTask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCTASK_H__D5ED3EB3_3301_4E3A_94B2_4730EE67D4BF__INCLUDED_)
#define AFX_NPCTASK_H__D5ED3EB3_3301_4E3A_94B2_4730EE67D4BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapGroup.h"
#include "Npc.h"
#include "Myheap.h"

class CNpcTask
{
protected:
    CNpcTask();
    virtual ~CNpcTask();
public:
    static CNpcTask* CreateNew()			{ return new CNpcTask; }
    ULONG	Release()						{ delete this; return 0; }

public:
    void	OnTimer(DWORD tCurr);
    bool	Create(PROCESS_ID idProcess, CNpc* pOwner, INpcData* pData);
    bool	ActivateTask		(CUser* pUser);

protected:
    bool		m_bActiveTime;
    ITaskSet*	m_setTask;
    CNpc*		m_pOwner;

protected: // ctrl
    PROCESS_ID	m_idProcess;

    MYHEAP_DECLARATION(s_heap)
};


#endif // !defined(AFX_NPCTASK_H__D5ED3EB3_3301_4E3A_94B2_4730EE67D4BF__INCLUDED_)
