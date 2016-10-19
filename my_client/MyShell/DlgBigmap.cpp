
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgBigmap.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgBigmap.h"

#include "3dGameMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBigmap dialog


CDlgBigmap::CDlgBigmap(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgBigmap::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgBigmap)
    // NOTE: the ClassWizard will add member initialization here
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    //}}AFX_DATA_INIT
}


void CDlgBigmap::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgBigmap)
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBigmap, CDialog)
    //{{AFX_MSG_MAP(CDlgBigmap)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_WM_MOVE()
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBigmap message handlers

void CDlgBigmap::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgBigmap::EnableWindow(false);
}

void CDlgBigmap::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

BOOL CDlgBigmap::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgBigmap::GetWindowRect(rect);
    m_BtnClose.Init(rect.left, rect.top, "Button532");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBigmap::Show()
{
    if (m_bShow)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog531",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0, m_Pnt.x, m_Pnt.y ) ;
            ShowAni->Show( 1, m_Pnt.x, m_Pnt.y + 256) ;
            ShowAni->Show( 2, m_Pnt.x + 128, m_Pnt.y + 256) ;
            ShowAni->Show( 3, m_Pnt.x + 256, m_Pnt.y ) ;
            ShowAni->Show( 4, m_Pnt.x + 512, m_Pnt.y ) ;
            ShowAni->Show( 5, m_Pnt.x + 512, m_Pnt.y + 128) ;
            ShowAni->Show( 6, m_Pnt.x + 256, m_Pnt.y + 256) ;
            ShowAni->Show( 7, m_Pnt.x + 384, m_Pnt.y + 256 ) ;
            ShowAni->Show( 8, m_Pnt.x + 512, m_Pnt.y + 256) ;
        }
        else
        {
            return ;
        }
        CMyPos posMap;
        posMap.x = m_Pnt.x + 8;
        posMap.y = m_Pnt.y + 7;
        CMySize sizeDlg;
        sizeDlg.iWidth = 512;
        sizeDlg.iHeight = 328;
        g_objGameMap.m_objRadar.ShowBigMap(posMap, g_objGameMap.GetIDDoc(), sizeDlg);
        ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                                                "Dialog532",
                                                EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0, m_Pnt.x, m_Pnt.y ) ;
            ShowAni->Show( 1, m_Pnt.x + 256, m_Pnt.y ) ;
            ShowAni->Show( 2, m_Pnt.x , m_Pnt.y + 256) ;
            ShowAni->Show( 3, m_Pnt.x + 128, m_Pnt.y + 256) ;
            ShowAni->Show( 4, m_Pnt.x + 256, m_Pnt.y + 256) ;
            ShowAni->Show( 5, m_Pnt.x + 384, m_Pnt.y + 256) ;
            ShowAni->Show( 6, m_Pnt.x + 512, m_Pnt.y  ) ;
            ShowAni->Show( 7, m_Pnt.x + 512, m_Pnt.y + 128 ) ;
            ShowAni->Show( 8, m_Pnt.x + 512, m_Pnt.y + 256) ;
        }
        else
        {
            return ;
        }
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
    }
}

LRESULT CDlgBigmap::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message == WM_ENABLE)
    {
        m_bShow = wParam;
        if (m_bShow)
        {
            DXPlaySound("Sound/Open.wav");
        }
        else
        {
            DXPlaySound("Sound/window_close.wav");
        }
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 )
        {
            return true ;
        }
        else if (wParam == 2)
        {
            CDlgBigmap::OnBtnClose();
            return true;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgBigmap::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // TODO: Return a different brush if the default is not desired
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}
