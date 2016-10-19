
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgHead.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgHead.h"

#include "Hero.h"
#include "Player.h"
#include "GamePlayerSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHead dialog


CDlgHead::CDlgHead(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgHead::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgHead)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_nFace = 0 ;
    m_nLook = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgHead::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgHead)
    DDX_Control(pDX, IDC_STA_TITLE, m_StaTitle);
    DDX_Control(pDX, IDC_STA_DUKE, m_StaDuke);
    DDX_Control(pDX, IDC_IMG_HEADS, m_ImgHeadS);
    DDX_Control(pDX, IDC_IMG_HEADB, m_ImgHeadB);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHead, CDialog)
    //{{AFX_MSG_MAP(CDlgHead)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_IMG_HEADB, OnImgHeadb)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHead message handlers

LRESULT CDlgHead::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
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

HBRUSH CDlgHead::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgHead::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgHead::GetWindowRect ( rect ) ;
    // Init the image
    m_ImgHeadS.Init ( rect.left, rect.top ) ;
    m_ImgHeadB.Init ( rect.left, rect.top, NULL, FACE_IMAGE ) ;
    // Init the static
    m_StaTitle.Init ( rect.left, rect.top ) ;
    m_StaDuke.Init ( rect.left, rect.top ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHead::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgHead::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog52",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( 0, m_Pnt.x, m_Pnt.y ) ;
        }
        else
        {
            return ;
        }
        // Show the image
        m_ImgHeadS.Show ( m_Pnt.x, m_Pnt.y ) ;
        if ( g_objPlayerSet.GetFocusPlayer() != NULL && g_objPlayerSet.GetFocusPlayer()->IsPlayer() )
        {
            CPlayer* pPlayer = g_objPlayerSet.GetFocusPlayer() ;
            if ( pPlayer != NULL &&
                    ( pPlayer->GetFace() != m_nFace ||
                      pPlayer->GetLook() != m_nLook ) )
            {
                m_nFace = pPlayer->GetFace() ;
                m_nLook = pPlayer->GetLook() ;
                m_StaTitle.SetWindowText( pPlayer->GetHororTitle( pPlayer->GetSelectedHonorTitle() ) ) ;
                m_StaDuke.SetWindowText( pPlayer->GetNobilityTitle() ) ;
                m_ImgHeadB.InsertImage( NULL, m_nFace, m_nLook, true ) ;
            }
            m_ImgHeadB.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_ImgHeadS.Show ( m_Pnt.x, m_Pnt.y ) ;
            // Show the static
            m_StaTitle.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_StaDuke.Show ( m_Pnt.x, m_Pnt.y ) ;
            return ;
        }
        else
        {
            if ( g_objHero.GetFace() != m_nFace || g_objHero.GetLook() != m_nLook )
            {
                m_nFace = g_objHero.GetFace() ;
                m_nLook = g_objHero.GetLook() ;
                m_StaTitle.SetWindowText( g_objHero.GetHororTitle( g_objHero.GetSelectedHonorTitle() ) ) ;
                m_StaDuke.SetWindowText( g_objHero.GetNobilityTitle() ) ;
                m_ImgHeadB.InsertImage( NULL, m_nFace, m_nLook, true ) ;
            }
            m_ImgHeadB.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_ImgHeadS.Show ( m_Pnt.x, m_Pnt.y ) ;
            // Show the static
            m_StaTitle.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_StaDuke.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
    }
}

void CDlgHead::OnImgHeadb()
{
}
