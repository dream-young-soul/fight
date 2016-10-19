
//**********************************************************\\
// ´úÂë±à¼­Æ÷\\
//**********************************************************\\
\\
// DlgGemInlay.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgGemInlay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGemInlay dialog


CDlgGemInlay::CDlgGemInlay(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgGemInlay::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgGemInlay)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgGemInlay::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgGemInlay)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGemInlay, CDialog)
    //{{AFX_MSG_MAP(CDlgGemInlay)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGemInlay message handlers
