
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NpcServer.h : main header file for the NPCSERVER application
//

#if !defined(AFX_NPCSERVER_H__8E808F2F_DE9F_4DAA_B75A_F9ACC61FDE4A__INCLUDED_)
#define AFX_NPCSERVER_H__8E808F2F_DE9F_4DAA_B75A_F9ACC61FDE4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNpcServerApp:
// See NpcServer.cpp for the implementation of this class
//

class CNpcServerApp : public CWinApp
{
public:
    CNpcServerApp();

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNpcServerApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

    // Implementation

    //{{AFX_MSG(CNpcServerApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NPCSERVER_H__8E808F2F_DE9F_4DAA_B75A_F9ACC61FDE4A__INCLUDED_)
