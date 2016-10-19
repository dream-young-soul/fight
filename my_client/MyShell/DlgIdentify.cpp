
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgIdentify.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgIdentify.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgIdentify dialog


CDlgIdentify::CDlgIdentify(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgIdentify::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgIdentify)
    m_bShow = false ;
    m_nPickUpItemID = -1 ;
    //}}AFX_DATA_INIT
}


void CDlgIdentify::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgIdentify)
    DDX_Control(pDX, IDC_STA_IDENTMONEY, m_StaIdentMoney);
    DDX_Control(pDX, IDC_IMG_GOOD, m_ImgGood);
    DDX_Control(pDX, IDC_BTN_OK, m_BtnOk);
    DDX_Control(pDX, IDC_BTN_CANCEL, m_BtnCancel);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgIdentify, CDialog)
    //{{AFX_MSG_MAP(CDlgIdentify)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
    ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
    ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_IMG_GOOD, OnImgGood)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgIdentify message handlers

LRESULT CDlgIdentify::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if (wParam)
        {
            DXPlaySound ( "Sound/Open.wav" ) ;
        }
        else
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
        }
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            return true ;
        }
    }
    else if ( message == WM_MY_MESSAGE )
    {
        switch(wParam)
        {
        case ON_MSGDLG_HIDE:
            CDlgIdentify::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_HIDE);
            break;
        case ON_MSGDLG_POINT:
            CDlgIdentify::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_POINT, lParam ) ;
            break;
        case ON_MSGDLG_IMAGE:
            CDlgIdentify::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_GOODS, DLG_IDENTIFY ) ;
            break ;
        default:
            break;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgIdentify::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgIdentify::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgIdentify::GetWindowRect ( rect ) ;
    // Init the buttons
    m_BtnOk.Init( rect.left, rect.top, "Button560" ) ;
    m_BtnCancel.Init( rect.left, rect.top, "Button77" ) ;
    // Init the image
    m_StaIdentMoney.Init(rect.left, rect.top);
    m_StaIdentMoney.SetWindowText("100");
    m_ImgGood.Init( rect.left, rect.top, NULL, ITEM_IMAGE );
    m_ImgGood.SetItemPos(DLG_IDENTIFY);
    m_ImgGood.InsertImage( NULL, 0, 0, true ) ;
    m_ImgGood.RemoveImage() ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgIdentify::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgIdentify::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog20",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0,
                           m_Pnt.x,
                           m_Pnt.y  );
        }
        else
        {
            return ;
        }
        // Show the button
        m_BtnOk.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnCancel.Show( m_Pnt.x, m_Pnt.y ) ;
        // Show the image
        m_StaIdentMoney.Show(m_Pnt.x, m_Pnt.y);
        m_ImgGood.Show( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgIdentify::SetPickUpItemID(int nID)
{
    m_nPickUpItemID = nID ;
}

void CDlgIdentify::OnBtnCancel()
{
    CDlgIdentify::GetParent()->PostMessage( WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL ) ;
    m_nPickUpItemID = -1 ;
    m_ImgGood.RemoveImage() ;
    CDlgIdentify::EnableWindow( false ) ;
}

void CDlgIdentify::OnBtnOk()
{
    if ( m_ImgGood.GetImageLoadState() && m_nPickUpItemID != -1 )
    {
        if (g_objHero.GetMoney() < 100)
        {
            CDlgIdentify::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSG_WARNING, 10024 ) ;
            return;
        }
        g_objHero.IdentItem( m_nPickUpItemID ) ;
        CDlgIdentify::OnBtnCancel() ;
    }
}

DWORD CDlgIdentify::GetIdentifyItemId()
{
    return m_nPickUpItemID;
}

void CDlgIdentify::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnMouseMove(nFlags, point);
}

void CDlgIdentify::OnImgGood()
{
    // TODO: Add your control notification handler code here
    if ( m_nPickUpItemID != -1 && !m_ImgGood.GetImageLoadState() )
    {
        /*			if ( g_objHero.GetMoney() < 50 )
        			{
        				CDlgIdentify::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSG_WARNING, 10024 ) ;
        				CDlgIdentify::GetParent()->PostMessage( WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL ) ;
        				return true ;
        			}*/
        CItem* pItem = g_objHero.GetItem( m_nPickUpItemID ) ;
        if ( pItem != NULL && pItem->TestStatus( _ITEM_STATUS_NOT_IDENT ) )
        {
            m_ImgGood.InsertImage( NULL, m_nPickUpItemID, 0, true ) ;
            CDlgIdentify::GetParent()->PostMessage( WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL ) ;
        }
        else
        {
            m_nPickUpItemID = -1;
            CDlgIdentify::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSG_WARNING, 10025 ) ;
            CDlgIdentify::GetParent()->PostMessage( WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL ) ;
        }
    }
}
