
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Register.h: interface for the CRegister class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTER_H__829C49BE_B4B9_4F4F_A93F_5A0C1C4CB329__INCLUDED_)
#define AFX_REGISTER_H__829C49BE_B4B9_4F4F_A93F_5A0C1C4CB329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TxtMsgSet.h"

class CRegister
{
public:
    CRegister();
    virtual ~CRegister();

    BOOL	AddMsg			(const char* pszMsg)	{return m_TxtMsgSet.AddMsg(pszMsg);}
    void	ClearMsg		(void)	{m_TxtMsgSet.ClearMsg();}
    const char*	GetLastMsg	(void)	{return m_TxtMsgSet.GetLastMsg();}

private:
    CTxtMsgSet	m_TxtMsgSet;
};

extern CRegister	g_objRegister;
#endif // !defined(AFX_REGISTER_H__829C49BE_B4B9_4F4F_A93F_5A0C1C4CB329__INCLUDED_)
