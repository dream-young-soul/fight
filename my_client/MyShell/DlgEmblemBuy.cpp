
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgEmblemBuy.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgEmblemBuy.h"

#include "3DRoleData.h"
#include "NetCmd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int	EMBLEM_PAGE_AMOUNT = 6 ;
/////////////////////////////////////////////////////////////////////////////
// CDlgEmblemBuy dialog


CDlgEmblemBuy::CDlgEmblemBuy(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgEmblemBuy::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgEmblemBuy)
    m_bShow = false ;
    m_nCurPage = 0 ;
    m_nCurIndex = -1 ;
    //}}AFX_DATA_INIT
}


void CDlgEmblemBuy::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgEmblemBuy)
    DDX_Control(pDX, IDC_BTN_OK, m_BtnOk);
    DDX_Control(pDX, IDC_IMG_EMBLEM6, m_Img6);
    DDX_Control(pDX, IDC_IMG_EMBLEM5, m_Img5);
    DDX_Control(pDX, IDC_IMG_EMBLEM4, m_Img4);
    DDX_Control(pDX, IDC_IMG_EMBLEM3, m_Img3);
    DDX_Control(pDX, IDC_IMG_EMBLEM2, m_Img2);
    DDX_Control(pDX, IDC_IMG_EMBLEM1, m_Img1);
    DDX_Control(pDX, IDC_BTN_UP, m_BtnUp);
    DDX_Control(pDX, IDC_BTN_DOWN, m_BtnDown);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEmblemBuy, CDialog)
    //{{AFX_MSG_MAP(CDlgEmblemBuy)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
    ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_IMG_EMBLEM1, OnImgEmblem1)
    ON_BN_CLICKED(IDC_IMG_EMBLEM2, OnImgEmblem2)
    ON_BN_CLICKED(IDC_IMG_EMBLEM3, OnImgEmblem3)
    ON_BN_CLICKED(IDC_IMG_EMBLEM4, OnImgEmblem4)
    ON_BN_CLICKED(IDC_IMG_EMBLEM5, OnImgEmblem5)
    ON_BN_CLICKED(IDC_IMG_EMBLEM6, OnImgEmblem6)
    ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEmblemBuy message handlers

LRESULT CDlgEmblemBuy::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if ( wParam == false )
        {
            m_nCurPage = 0 ;
            DXPlaySound ( "Sound/DlgClose.wav" ) ;
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
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgEmblemBuy::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgEmblemBuy::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgEmblemBuy::GetWindowRect ( rect ) ;
    // Init the image
    m_Img6.Init( rect.left, rect.top, NULL, EMBLEM_IMAGE ) ;
    m_Img5.Init( rect.left, rect.top, NULL, EMBLEM_IMAGE ) ;
    m_Img4.Init( rect.left, rect.top, NULL, EMBLEM_IMAGE ) ;
    m_Img3.Init( rect.left, rect.top, NULL, EMBLEM_IMAGE ) ;
    m_Img2.Init( rect.left, rect.top, NULL, EMBLEM_IMAGE ) ;
    m_Img1.Init( rect.left, rect.top, NULL, EMBLEM_IMAGE ) ;
    // Init the button
    m_BtnUp.Init( rect.left, rect.top, "Button111" ) ;
    m_BtnDown.Init( rect.left, rect.top, "Button112" ) ;
    m_BtnClose.Init( rect.left, rect.top, "Button540" ) ;
    m_BtnOk.Init( rect.left, rect.top, "Button0" ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEmblemBuy::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgEmblemBuy::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog11",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0,
                           m_Pnt.x - 36,
                           m_Pnt.y  );
            ShowAni->Show( 1,
                           m_Pnt.x,
                           m_Pnt.y + 150 ) ;
            ShowAni->Show( 4,
                           m_Pnt.x + 56,
                           m_Pnt.y + 118 ) ;
        }
        else
        {
            return ;
        }
        // Show the image
        m_Img1.Show( m_Pnt.x, m_Pnt.y ) ;
        m_Img2.Show( m_Pnt.x, m_Pnt.y ) ;
        m_Img3.Show( m_Pnt.x, m_Pnt.y ) ;
        m_Img4.Show( m_Pnt.x, m_Pnt.y ) ;
        m_Img5.Show( m_Pnt.x, m_Pnt.y ) ;
        m_Img6.Show( m_Pnt.x, m_Pnt.y ) ;
        switch ( m_nCurIndex )
        {
        case 0:
            m_Img1.ShowFocus() ;
            break ;
        case 1:
            m_Img2.ShowFocus() ;
            break ;
        case 2:
            m_Img3.ShowFocus() ;
            break ;
        case 3:
            m_Img4.ShowFocus() ;
            break ;
        case 4:
            m_Img5.ShowFocus() ;
            break ;
        case 5:
            m_Img6.ShowFocus() ;
            break ;
        default:
            break ;
        }
        // Show the button
        m_BtnUp.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnDown.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnClose.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnOk.Show( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgEmblemBuy::SetEmblemBuy()
{
    m_nCurPage = 0 ;
    m_nCurIndex = -1 ;
    CDlgEmblemBuy::InsertEmblem() ;
}

void CDlgEmblemBuy::InsertEmblem()
{
    m_Img1.RemoveImage() ;
    m_Img2.RemoveImage() ;
    m_Img3.RemoveImage() ;
    m_Img4.RemoveImage() ;
    m_Img5.RemoveImage() ;
    m_Img6.RemoveImage() ;
    int nAmount = ( g_obj3DRoleData.GetEmblemAmount() > m_nCurPage * EMBLEM_PAGE_AMOUNT + EMBLEM_PAGE_AMOUNT ) ? EMBLEM_PAGE_AMOUNT : g_obj3DRoleData.GetEmblemAmount() - m_nCurPage * EMBLEM_PAGE_AMOUNT ;
    for ( int i = 0; i < nAmount; i++ )
    {
        switch ( i )
        {
        case 0:
            m_Img1.InsertImage( NULL, m_nCurPage * EMBLEM_PAGE_AMOUNT + i ) ;
            break ;
        case 1:
            m_Img2.InsertImage( NULL, m_nCurPage * EMBLEM_PAGE_AMOUNT + i ) ;
            break ;
        case 2:
            m_Img3.InsertImage( NULL, m_nCurPage * EMBLEM_PAGE_AMOUNT + i ) ;
            break ;
        case 3:
            m_Img4.InsertImage( NULL, m_nCurPage * EMBLEM_PAGE_AMOUNT + i ) ;
            break ;
        case 4:
            m_Img5.InsertImage( NULL, m_nCurPage * EMBLEM_PAGE_AMOUNT + i ) ;
            break ;
        case 5:
            m_Img6.InsertImage( NULL, m_nCurPage * EMBLEM_PAGE_AMOUNT + i ) ;
            break ;
        default:
            break ;
        }
    }
}

void CDlgEmblemBuy::OnBtnUp()
{
    if ( m_nCurPage > 0 )
    {
        m_nCurPage-- ;
        m_nCurIndex = -1 ;
        CDlgEmblemBuy::InsertEmblem() ;
    }
}

void CDlgEmblemBuy::OnBtnDown()
{
    if ( g_obj3DRoleData.GetEmblemAmount() > m_nCurPage * EMBLEM_PAGE_AMOUNT + EMBLEM_PAGE_AMOUNT )
    {
        m_nCurPage++ ;
        m_nCurIndex = -1 ;
        CDlgEmblemBuy::InsertEmblem() ;
    }
}

void CDlgEmblemBuy::OnBtnClose()
{
    CDlgEmblemBuy::EnableWindow( false ) ;
}

void CDlgEmblemBuy::OnImgEmblem1()
{
    m_nCurIndex = 0 ;
}

void CDlgEmblemBuy::OnImgEmblem2()
{
    m_nCurIndex = 1 ;
}

void CDlgEmblemBuy::OnImgEmblem3()
{
    m_nCurIndex = 2 ;
}

void CDlgEmblemBuy::OnImgEmblem4()
{
    m_nCurIndex = 3 ;
}

void CDlgEmblemBuy::OnImgEmblem5()
{
    m_nCurIndex = 4 ;
}

void CDlgEmblemBuy::OnImgEmblem6()
{
    m_nCurIndex = 5 ;
}

void CDlgEmblemBuy::OnBtnOk()
{
    if ( m_nCurIndex != -1 )
    {
        CNetCmd::SynBuyEmblem( m_nCurPage * EMBLEM_PAGE_AMOUNT + m_nCurIndex ) ;
        CDlgEmblemBuy::EnableWindow( false ) ;
    }
}
