
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgServerSel.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgServerSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgServerSel dialog


CDlgServerSel::CDlgServerSel(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgServerSel::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgServerSel)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgServerSel::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgServerSel)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgServerSel, CDialog)
    //{{AFX_MSG_MAP(CDlgServerSel)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgServerSel message handlers
