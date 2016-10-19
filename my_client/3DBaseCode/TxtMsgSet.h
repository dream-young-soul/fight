
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TxtMsgSet.h: interface for the CTxtMsgSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TXTMSGSET_H__C853758B_746C_421F_8215_E9BF34CE82A2__INCLUDED_)
#define AFX_TXTMSGSET_H__C853758B_746C_421F_8215_E9BF34CE82A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <deque>
using namespace std;

class CTxtMsgSet
{
public:
    CTxtMsgSet();
    virtual ~CTxtMsgSet();

    BOOL	AddMsg	(const char* pszMsg);
    void	ClearMsg(void);

    DWORD	GetMsgAmount(void) const {return m_dequeMsg.size();}
    const char*	GetMsg		(DWORD dwIndex) const;
    const char*	GetLastMsg	(void);

    void	SetMaxMsg	(DWORD dwMaxMsg);

    typedef deque<char* >  DEQUE_STR;

private:
    DWORD		m_dwMsgTake;
    DWORD		m_dwMaxMsg;
    DEQUE_STR	m_dequeMsg;
};

#endif // !defined(AFX_TXTMSGSET_H__C853758B_746C_421F_8215_E9BF34CE82A2__INCLUDED_)
