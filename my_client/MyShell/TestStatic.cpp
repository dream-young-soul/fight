
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TestStatic.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "TestStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestStatic

CTestStatic::CTestStatic()
{
}

CTestStatic::~CTestStatic()
{
}


BEGIN_MESSAGE_MAP(CTestStatic, CStatic)
    //{{AFX_MSG_MAP(CTestStatic)
    ON_WM_KEYDOWN()
    ON_WM_SYSKEYDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestStatic message handlers

void CTestStatic::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if ( nChar != 17 )
    {
        LPARAM lParam ;
        lParam = MAKELONG ( nRepCnt, 0 ) ;
        CTestStatic::GetParent ()->PostMessage ( WM_KEYDOWN, nChar, lParam ) ;
    }
}

LRESULT CTestStatic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_COMMAND )
    {
        if ( wParam == 1 || wParam == 2 )
        {
            return true ;
        }
    }
    return CStatic::WindowProc(message, wParam, lParam);
}

void CTestStatic::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CStatic::OnSysKeyDown(nChar, nRepCnt, nFlags);
}
