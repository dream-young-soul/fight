
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgMipmap.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgMipmap.h"

#include "3DGameMap.h"
#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMipmap dialog


CDlgMipmap::CDlgMipmap(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMipmap::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMipmap)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_MapID = ID_NONE ;
    m_iMipmapmode = 0;
    //}}AFX_DATA_INIT
}


void CDlgMipmap::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMipmap)
    DDX_Control(pDX, IDC_BTN_VIEWCHG, m_BtnViewchg);
    DDX_Control(pDX, IDC_BTN_BIGOUT, m_BtnBigout);
    DDX_Control(pDX, IDC_STA_POSY, m_StaPosy);
    DDX_Control(pDX, IDC_STA_POSX, m_StaPosx);
    DDX_Control(pDX, IDC_STA_NAME, m_StaName);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMipmap, CDialog)
    //{{AFX_MSG_MAP(CDlgMipmap)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_VIEWCHG, OnBtnViewchg)
    ON_BN_CLICKED(IDC_BTN_BIGOUT, OnBtnBigout)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_LBUTTONUP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMipmap message handlers

LRESULT CDlgMipmap::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgMipmap::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgMipmap::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgMipmap::GetWindowRect ( rect ) ;
    // Init the button
    m_BtnViewchg.Init ( rect.left, rect.top, "Button531" ) ;
    m_BtnBigout.Init( rect.left, rect.top, "Button530" ) ;
    // Init the static
    m_StaName.Init( rect.left, rect.top ) ;
    m_StaName.SetStaticPosType(1);
    m_StaPosx.Init( rect.left, rect.top ) ;
    m_StaPosx.SetStaticPosType(1);
    m_StaPosy.Init( rect.left, rect.top ) ;
    m_StaPosy.SetStaticPosType(1);
    m_BtnViewchg.SetTipShow ((char*)g_objGameDataSet.GetStr(10739));
    m_BtnBigout.SetTipShow ((char*)g_objGameDataSet.GetStr(10740));
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMipmap::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgMipmap::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog53",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 1, m_Pnt.x + 4, m_Pnt.y + 41) ;
        }
        else
        {
            return ;
        }
        // Show the frame rate
//#ifdef _DEBUG
        static int nFrameRate = 0;
        static DWORD timeLoop	= 0;
        DWORD timeNow	=::TimeGet();
        if (timeNow < timeLoop + 30)
        {
            ::Sleep(timeLoop + 30 - timeNow);
        }
        nFrameRate	= (nFrameRate + 1000 / __max(1, int(::TimeGet() - timeLoop))) / 2;
        timeLoop	=::TimeGet();
        // show position of player
        CMyPos posHero;
        g_objHero.GetPos(posHero);
        char szMsg[256] = "";
        CMyPos posAlign = g_objHero.GetAlignPos();
        /*		// Test code

        		int nWorldX, nWorldY, nBgX, nBgY ;
        		g_objGameMap.Cell2World( posHero.x, posHero.y, nWorldX, nWorldY ) ;
        		g_objGameMap.World2Bg( nWorldX, nWorldY, nBgX, nBgY ) ;
        		CMySize BgSize ;
        		g_objGameMap.GetBgSize( BgSize ) ;

        		int nX, nY ;
        		nX = nBgX ;
        		nY = BgSize.iHeight - nBgY ;
        		nY /= 32 ;
        		nX /= 64 ;*/
        sprintf( szMsg, "(%03d,%03d) [%03d,%03d]", posHero.x, posHero.y, posAlign.x, posAlign.y );
        CMyBitmap::ShowString( 700,
                               0,
                               _COLOR_YELLOW,
                               szMsg);
        sprintf( szMsg, "Fps:%d", nFrameRate ) ;
        CMyBitmap::ShowString( 700,
                               20,
                               _COLOR_YELLOW,
                               szMsg);
//#endif
        // Show the static
        if ( m_MapID != g_objGameMap.GetIDDoc() )
        {
            m_StaName.SetWindowText( g_objGameMap.GetMapName() ) ;
            m_MapID = g_objGameMap.GetIDDoc() ;
        }
        // Show the map
        CMyPos pos ;
        pos.x = m_Pnt.x + 2;
        pos.y = m_Pnt.y + 41;
        g_objGameMap.m_objRadar.Show( pos, g_objGameMap.GetIDDoc(), m_iMipmapmode ) ;
        ShowAni->Show(0, m_Pnt.x, m_Pnt.y);
        char szPos[256] = "";
        CMyPos posHeroAt;
        g_objHero.GetPos(posHeroAt);
        sprintf( szPos, "%d", posHeroAt.x);
        m_StaPosx.SetWindowText(szPos);
        sprintf( szPos, "%d", posHeroAt.y);
        m_StaPosy.SetWindowText(szPos);
        m_StaName.Show( m_Pnt.x, m_Pnt.y ) ;
        m_StaPosx.Show( m_Pnt.x, m_Pnt.y ) ;
        m_StaPosy.Show( m_Pnt.x, m_Pnt.y ) ;
        // Show the button
        m_BtnViewchg.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnBigout.Show( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgMipmap::OnBtnViewchg()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    if (m_iMipmapmode == 0  )
    {
        m_iMipmapmode = 1;
    }
    else if (m_iMipmapmode == 1)
    {
        m_iMipmapmode = 2;
    }
    else if (m_iMipmapmode == 2)
    {
        m_iMipmapmode = 0;
    }
}

void CDlgMipmap::OnBtnBigout()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    CDlgMipmap::GetParent()->PostMessage(WM_MY_MESSAGE, ON_BIGMAP_OPEN);
}

void CDlgMipmap::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgMipmap::GetParent()->PostMessage( WM_LBUTTONDOWN, nFlags, lParam ) ;
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgMipmap::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgMipmap::GetParent()->PostMessage( WM_LBUTTONDBLCLK, nFlags, lParam ) ;
    CDialog::OnLButtonDblClk(nFlags, point);
}


void CDlgMipmap::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgMipmap::GetParent()->PostMessage( WM_RBUTTONDOWN, nFlags, lParam ) ;
    CDialog::OnRButtonDown(nFlags, point);
}

void CDlgMipmap::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgMipmap::GetParent()->PostMessage( WM_RBUTTONUP, nFlags, lParam ) ;
    CDialog::OnRButtonUp(nFlags, point);
}

void CDlgMipmap::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgMipmap::GetParent()->PostMessage( WM_LBUTTONUP, nFlags, lParam ) ;
    CDialog::OnLButtonUp(nFlags, point);
}

void CDlgMipmap::OpenBigmap()
{
    CDlgMipmap::OnBtnBigout();
}
