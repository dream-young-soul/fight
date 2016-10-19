
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgHelp.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgHelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHelp dialog


CDlgHelp::CDlgHelp(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgHelp::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgHelp)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgHelp::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgHelp)
    DDX_Control(pDX, IDC_HELP_MORE, m_HelpMore);
    DDX_Control(pDX, IDC_BUTTON_CLOSE, m_HelpClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHelp, CDialog)
    //{{AFX_MSG_MAP(CDlgHelp)
    ON_WM_DESTROY()
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
    ON_BN_CLICKED(IDC_HELP_MORE, OnHelpMore)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHelp message handlers

LRESULT CDlgHelp::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgHelp::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgHelp::GetWindowRect ( rect ) ;
    if ( ! m_Web.Create( NULL,
                         WS_CHILD | WS_VISIBLE,
                         CRect( 10, 80, rect.Width() - 15, rect.Height () - 10 ),
                         this,
                         100 ) )
    {
        return false;
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHelp::OnDestroy()
{
    CDialog::OnDestroy();
    if (m_Hbr)
    {
        DeleteObject( m_Hbr ) ;
    }
}

HBRUSH CDlgHelp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    if ( nCtlColor == CTLCOLOR_DLG )
    {
        return m_Hbr ;
    }
    return g_StockBrush ;
}

void CDlgHelp::OpenHelpDlg(char* strFileName)
{
    COleVariant varEmpty;
    if ( strFileName != NULL )
    {
        char strBuffer[64];
        GetCurrentDirectory ( 64, strBuffer ) ;
        strcat ( strBuffer, "\\Help\\" ) ;
        strcat ( strBuffer, strFileName ) ;
        m_Web.Navigate( strBuffer, &varEmpty, &varEmpty,
                        &varEmpty, &varEmpty );
        CDlgHelp::ShowWindow ( SW_SHOW ) ;
    }
}

void CDlgHelp::OnButtonClose()
{
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    CDlgHelp::ShowWindow ( SW_HIDE ) ;
}

void CDlgHelp::OnHelpMore()
{
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    ::ShellExecute(m_hWnd, NULL, "http://www.tqzf.com/", NULL, NULL, SW_SHOW );
}

void CDlgHelp::ResetBk(int nMode)
{
    char strName[32] ;
    char strMain[8] ;
    if ( nMode == 0 )
    {
        strcpy ( strMain, "Main" ) ;
    }
    else
    {
        sprintf ( strMain, "Main%d", nMode ) ;
    }
    sprintf ( strName, "Data/%s/HelpBk.bmp", strMain ) ;
    HBITMAP hBmp ;
    hBmp =  (HBITMAP )::LoadImage ( AfxGetInstanceHandle(),
                                    strName,
                                    IMAGE_BITMAP,
                                    0,
                                    0,
                                    LR_LOADFROMFILE ) ;
    m_Hbr = CreatePatternBrush ( hBmp ) ;
    sprintf ( strName, "Data/%s/Close.bmp", strMain ) ;
    hBmp =  (HBITMAP )::LoadImage ( AfxGetInstanceHandle(),
                                    strName,
                                    IMAGE_BITMAP,
                                    0,
                                    0,
                                    LR_LOADFROMFILE ) ;
    m_HelpClose.SetBitmap ( hBmp ) ;
    sprintf ( strName, "Data/%s/HelpMore.bmp", strMain ) ;
    hBmp =  (HBITMAP )::LoadImage ( AfxGetInstanceHandle(),
                                    strName,
                                    IMAGE_BITMAP,
                                    0,
                                    0,
                                    LR_LOADFROMFILE ) ;
    m_HelpMore.SetBitmap ( hBmp ) ;
    CDlgHelp::RedrawWindow () ;
}
