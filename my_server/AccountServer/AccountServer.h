
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// AccountServer.h : main header file for the ACCOUNTSERVER application
//

#if !defined(AFX_ACCOUNTSERVER_H__39D76464_2ADC_424A_A3B1_72C382ACA9C7__INCLUDED_)
#define AFX_ACCOUNTSERVER_H__39D76464_2ADC_424A_A3B1_72C382ACA9C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
//*/
#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAccountServerApp:
// See AccountServer.cpp for the implementation of this class
//

class CAccountServerApp : public CWinApp
{
public:
    CAccountServerApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAccountServerApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CAccountServerApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNTSERVER_H__39D76464_2ADC_424A_A3B1_72C382ACA9C7__INCLUDED_)
