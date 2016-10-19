
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Agent.h: interface for the CAgent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AGENT_H__F52C1E05_69FA_4094_A1DE_CCCCE4F4FF51__INCLUDED_)
#define AFX_AGENT_H__F52C1E05_69FA_4094_A1DE_CCCCE4F4FF51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "User.h"
#include "mycom.h"

class CAgent : public CUser
{
    COM_STYLE(CAgent)
protected:
    CAgent();
    virtual ~CAgent();

public:
    bool Create(PROCESS_ID idProcess, OBJID idAgent);

public: // overload
    bool FullItem(int nItemType, DWORD dwData);
    virtual bool SendMsg(CNetMsg* pMsg);
    virtual bool IsAgent()									{ return true; }
    virtual CAgent*	QueryAgent		()						{ return this; }			// agent overload
    bool LeaveMapGroup	();		//@@@ Í¬²½PROCESS_ID

protected: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_AGENT_H__F52C1E05_69FA_4094_A1DE_CCCCE4F4FF51__INCLUDED_)
