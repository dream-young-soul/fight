
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Entrance.h: interface for the CEntrance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENTRANCE_H__62B457E1_0B69_4DE3_AEA5_E9896402EF87__INCLUDED_)
#define AFX_ENTRANCE_H__62B457E1_0B69_4DE3_AEA5_E9896402EF87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TxtMsgSet.h"

class CEntrance
{
public:
    CEntrance();
    virtual ~CEntrance();

    void	ClearMsg		(void)			{m_TxtMsgSet.ClearMsg();}
    BOOL	AddMsg			(const char* pszMsg) {return m_TxtMsgSet.AddMsg(pszMsg);}
    const char*	GetLastMsg	(void)			{return m_TxtMsgSet.GetLastMsg();}

private:
    CTxtMsgSet	m_TxtMsgSet;
};

extern CEntrance	g_objEntrance;

#endif // !defined(AFX_ENTRANCE_H__62B457E1_0B69_4DE3_AEA5_E9896402EF87__INCLUDED_)
