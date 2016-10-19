
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgLogo.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgLogo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLogo dialog


CDlgLogo::CDlgLogo(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgLogo::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgLogo)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgLogo::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgLogo)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLogo, CDialog)
    //{{AFX_MSG_MAP(CDlgLogo)
    ON_WM_DESTROY()
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLogo message handlers

BOOL CDlgLogo::OnInitDialog()
{
    CDialog::OnInitDialog();
    char strName[32] ;
    sprintf ( strName, "Data/Main/Logo1.bmp") ;
    HBITMAP hBmp ;
    hBmp =  (HBITMAP )::LoadImage ( AfxGetInstanceHandle(),
                                    strName,
                                    IMAGE_BITMAP,
                                    0,
                                    0,
                                    LR_LOADFROMFILE ) ;
    m_Hbr = CreatePatternBrush ( hBmp ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLogo::OnDestroy()
{
    CDialog::OnDestroy();
    if (m_Hbr)
    {
        DeleteObject( m_Hbr ) ;
    }
}

HBRUSH CDlgLogo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    if ( nCtlColor == CTLCOLOR_DLG )
    {
        return m_Hbr ;
    }
    return g_StockBrush ;
}
