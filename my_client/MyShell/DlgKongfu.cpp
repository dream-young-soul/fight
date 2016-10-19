
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgKongfu.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgKongfu.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgKongfu dialog


CDlgKongfu::CDlgKongfu(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgKongfu::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgKongfu)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    //}}AFX_DATA_INIT
}


void CDlgKongfu::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgKongfu)
    DDX_Control(pDX, IDC_KONGFU_BTN_UP, m_KongfuBtnU);
    DDX_Control(pDX, IDC_KONGFU_BTN_DOWN, m_KongfuBtnD);
    DDX_Control(pDX, IDC_KONGFU_IMG1, m_KongfuImg1);
    DDX_Control(pDX, IDC_KONGFU_IMG2, m_KongfuImg2);
    DDX_Control(pDX, IDC_KONGFU_IMG3, m_KongfuImg3);
    DDX_Control(pDX, IDC_KONGFU_IMG4, m_KongfuImg4);
    DDX_Control(pDX, IDC_KONGFU_IMG5, m_KongfuImg5);
    DDX_Control(pDX, IDC_KONGFU_IMG6, m_KongfuImg6);
    DDX_Control(pDX, IDC_KONGFU_IMG7, m_KongfuImg7);
    DDX_Control(pDX, IDC_KONGFU_IMG8, m_KongfuImg8);
    DDX_Control(pDX, IDC_KONGFU_IMG9, m_KongfuImg9);
    DDX_Control(pDX, IDC_KONGFU_IMG10, m_KongfuImg10);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgKongfu, CDialog)
    //{{AFX_MSG_MAP(CDlgKongfu)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_KONGFU_IMG1, OnKongfuImg1)
    ON_BN_CLICKED(IDC_KONGFU_IMG2, OnKongfuImg2)
    ON_BN_CLICKED(IDC_KONGFU_IMG3, OnKongfuImg3)
    ON_BN_CLICKED(IDC_KONGFU_IMG4, OnKongfuImg4)
    ON_BN_CLICKED(IDC_KONGFU_IMG5, OnKongfuImg5)
    ON_BN_CLICKED(IDC_KONGFU_IMG6, OnKongfuImg6)
    ON_BN_CLICKED(IDC_KONGFU_IMG7, OnKongfuImg7)
    ON_BN_CLICKED(IDC_KONGFU_IMG8, OnKongfuImg8)
    ON_BN_CLICKED(IDC_KONGFU_IMG9, OnKongfuImg9)
    ON_BN_CLICKED(IDC_KONGFU_IMG10, OnKongfuImg10)
    ON_BN_CLICKED(IDC_KONGFU_BTN_UP, OnKongfuBtnUp)
    ON_BN_CLICKED(IDC_KONGFU_BTN_DOWN, OnKongfuBtnDown)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgKongfu message handlers

LRESULT CDlgKongfu::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_SHOWWINDOW )
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

HBRUSH CDlgKongfu::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgKongfu::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgKongfu::GetWindowRect ( rect ) ;
    // Init the image
    m_KongfuImg1.Init ( rect.left, rect.top, NULL, 3 ) ;
    m_KongfuImg2.Init ( rect.left, rect.top, NULL, 3 ) ;
    m_KongfuImg3.Init ( rect.left, rect.top, NULL, 3 ) ;
    m_KongfuImg4.Init ( rect.left, rect.top, NULL, 3 ) ;
    m_KongfuImg5.Init ( rect.left, rect.top, NULL, 3 ) ;
    m_KongfuImg6.Init ( rect.left, rect.top, NULL, 3 ) ;
    m_KongfuImg7.Init ( rect.left, rect.top, NULL, 3 ) ;
    m_KongfuImg8.Init ( rect.left, rect.top, NULL, 3 ) ;
    m_KongfuImg9.Init ( rect.left, rect.top, NULL, 3 ) ;
    m_KongfuImg10.Init ( rect.left, rect.top, NULL, 3 ) ;
    // Init the button
    m_KongfuBtnU.Init ( rect.left, rect.top, "Button173" ) ;
    m_KongfuBtnU.EnableWindow ( false ) ;
    m_KongfuBtnD.Init ( rect.left, rect.top, "Button174" ) ;
    m_KongfuBtnD.EnableWindow ( false ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgKongfu::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgKongfu::Show()
{
    // show the image
    if ( m_bShow )
    {
        m_KongfuImg1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_KongfuImg2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_KongfuImg3.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_KongfuImg4.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_KongfuImg5.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_KongfuImg6.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_KongfuImg7.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_KongfuImg8.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_KongfuImg9.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_KongfuImg10.Show ( m_Pnt.x, m_Pnt.y ) ;
        if ( m_KongfuBtnU.IsWindowEnabled () )
        {
            m_KongfuBtnU.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        if ( m_KongfuBtnD.IsWindowEnabled () )
        {
            m_KongfuBtnD.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
    }
}

void CDlgKongfu::InitKongfuDialog()
{
    m_nCurBase = 0 ;
    int nAmount = g_objHero.GetKongfuAmount() ;
    if ( nAmount > 10 )
    {
        m_KongfuBtnU.EnableWindow ( true ) ;
        m_KongfuBtnD.EnableWindow ( true ) ;
    }
    else
    {
        m_KongfuBtnU.EnableWindow ( false ) ;
        m_KongfuBtnD.EnableWindow ( false ) ;
    }
    for ( int i = 0; i < nAmount; i++ )
    {
        CMagic* pKongfu = g_objHero.GetKongfuByIndex(i) ;
        if ( pKongfu )
        {
            if ( g_objHero.IsTransform () && pKongfu->IsTransformMagic () )
            {
                continue ;
            }
            char SkillType[32] ;
            BOOL bDisable = !pKongfu->IsEnable () ;
            m_KongfuIdType[i] = pKongfu->GetIDType () ;
            sprintf ( SkillType, "MagicSkillType%d", m_KongfuIdType[i] ) ;
            switch ( i )
            {
            case 0:
                m_KongfuImg1.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                break ;
            case 1:
                m_KongfuImg2.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                break ;
            case 2:
                m_KongfuImg3.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                break ;
            case 3:
                m_KongfuImg4.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                break ;
            case 4:
                m_KongfuImg5.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                break ;
            case 5:
                m_KongfuImg6.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                break ;
            case 6:
                m_KongfuImg7.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                break ;
            case 7:
                m_KongfuImg8.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                break ;
            case 8:
                m_KongfuImg9.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                break ;
            case 9:
                m_KongfuImg10.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                break ;
            }
        }
    }
    CDlgKongfu::ShowWindow ( SW_SHOW ) ;
}

void CDlgKongfu::OnKongfuImg1()
{
    CDlgKongfu::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_KongfuIdType[0 + m_nCurBase] ) ;
    CDlgKongfu::ShowWindow ( SW_HIDE ) ;
}

void CDlgKongfu::OnKongfuImg2()
{
    CDlgKongfu::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_KongfuIdType[1 + m_nCurBase] ) ;
    CDlgKongfu::ShowWindow ( SW_HIDE ) ;
}

void CDlgKongfu::OnKongfuImg3()
{
    CDlgKongfu::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_KongfuIdType[2 + m_nCurBase] ) ;
    CDlgKongfu::ShowWindow ( SW_HIDE ) ;
}

void CDlgKongfu::OnKongfuImg4()
{
    CDlgKongfu::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_KongfuIdType[3 + m_nCurBase] ) ;
    CDlgKongfu::ShowWindow ( SW_HIDE ) ;
}

void CDlgKongfu::OnKongfuImg5()
{
    CDlgKongfu::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_KongfuIdType[4 + m_nCurBase] ) ;
    CDlgKongfu::ShowWindow ( SW_HIDE ) ;
}

void CDlgKongfu::OnKongfuImg6()
{
    CDlgKongfu::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_KongfuIdType[5 + m_nCurBase] ) ;
    CDlgKongfu::ShowWindow ( SW_HIDE ) ;
}

void CDlgKongfu::OnKongfuImg7()
{
    CDlgKongfu::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_KongfuIdType[6 + m_nCurBase] ) ;
    CDlgKongfu::ShowWindow ( SW_HIDE ) ;
}

void CDlgKongfu::OnKongfuImg8()
{
    CDlgKongfu::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_KongfuIdType[7 + m_nCurBase] ) ;
    CDlgKongfu::ShowWindow ( SW_HIDE ) ;
}

void CDlgKongfu::OnKongfuImg9()
{
    CDlgKongfu::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_KongfuIdType[8 + m_nCurBase] ) ;
    CDlgKongfu::ShowWindow ( SW_HIDE ) ;
}

void CDlgKongfu::OnKongfuImg10()
{
    CDlgKongfu::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_KongfuIdType[9 + m_nCurBase] ) ;
    CDlgKongfu::ShowWindow ( SW_HIDE ) ;
}

void CDlgKongfu::OnKongfuBtnUp()
{
    if ( m_nCurBase > 0 )
    {
        CDlgKongfu::MoveWindow ( _SCR_WIDTH - 10 * 50 - 18,
                                 _SCR_HEIGHT - DLG_MAIN_HEIGHT + 18 - 100,
                                 10 * 50 + 18,
                                 50 ) ;
        m_nCurBase -= 10 ;
        for ( int i = m_nCurBase; i < m_nCurBase + 10; i++ )
        {
            CMagic* pKongfu = g_objHero.GetKongfuByIndex(i) ;
            if ( pKongfu )
            {
                char SkillType[32] ;
                BOOL bDisable = !pKongfu->IsEnable () ;
                m_KongfuIdType[i] = pKongfu->GetIDType () ;
                sprintf ( SkillType, "MagicSkillType%d", m_KongfuIdType[i] ) ;
                switch ( i - m_nCurBase )
                {
                case 0:
                    m_KongfuImg1.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 1:
                    m_KongfuImg2.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 2:
                    m_KongfuImg3.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 3:
                    m_KongfuImg4.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 4:
                    m_KongfuImg5.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 5:
                    m_KongfuImg6.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 6:
                    m_KongfuImg7.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 7:
                    m_KongfuImg8.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 8:
                    m_KongfuImg9.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 9:
                    m_KongfuImg10.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                }
            }
        }
    }
}

void CDlgKongfu::OnKongfuBtnDown()
{
    int nAmount = g_objHero.GetKongfuAmount() ;
    if ( nAmount > 10 && m_nCurBase + 10 < nAmount )
    {
        m_nCurBase += 10 ;
        nAmount = ( nAmount > m_nCurBase + 10 ) ? m_nCurBase + 10 : nAmount ;
        CDlgKongfu::MoveWindow ( _SCR_WIDTH - ( nAmount - m_nCurBase ) * 50 - 18,
                                 _SCR_HEIGHT - DLG_MAIN_HEIGHT + 18 - 100,
                                 ( nAmount - m_nCurBase ) * 50 + 18,
                                 50 ) ;
        for ( int i = m_nCurBase; i < nAmount; i++ )
        {
            CMagic* pKongfu = g_objHero.GetKongfuByIndex(i) ;
            if ( pKongfu )
            {
                char SkillType[32] ;
                BOOL bDisable = !pKongfu->IsEnable () ;
                m_KongfuIdType[i] = pKongfu->GetIDType () ;
                sprintf ( SkillType, "MagicSkillType%d", m_KongfuIdType[i] ) ;
                switch ( i - m_nCurBase )
                {
                case 0:
                    m_KongfuImg1.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 1:
                    m_KongfuImg2.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 2:
                    m_KongfuImg3.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 3:
                    m_KongfuImg4.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 4:
                    m_KongfuImg5.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 5:
                    m_KongfuImg6.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 6:
                    m_KongfuImg7.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 7:
                    m_KongfuImg8.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 8:
                    m_KongfuImg9.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                case 9:
                    m_KongfuImg10.InsertImage ( SkillType, m_KongfuIdType[i], bDisable ) ;
                    break ;
                }
            }
        }
    }
}
