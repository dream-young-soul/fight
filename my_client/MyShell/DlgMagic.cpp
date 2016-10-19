
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgMagic.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgMagic.h"

#include "Hero.h"
#include "Magic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMagic dialog


CDlgMagic::CDlgMagic(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMagic::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMagic)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_nCurBase = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgMagic::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMagic)
    DDX_Control(pDX, IDC_MAGIC_BTN_UP, m_MagicBtnU);
    DDX_Control(pDX, IDC_MAGIC_BTN_DOWN, m_MagicBtnD);
    DDX_Control(pDX, IDC_MAGIC_IMG10, m_MagicImg10);
    DDX_Control(pDX, IDC_MAGIC_IMG9, m_MagicImg9);
    DDX_Control(pDX, IDC_MAGIC_IMG8, m_MagicImg8);
    DDX_Control(pDX, IDC_MAGIC_IMG7, m_MagicImg7);
    DDX_Control(pDX, IDC_MAGIC_IMG6, m_MagicImg6);
    DDX_Control(pDX, IDC_MAGIC_IMG5, m_MagicImg5);
    DDX_Control(pDX, IDC_MAGIC_IMG4, m_MagicImg4);
    DDX_Control(pDX, IDC_MAGIC_IMG3, m_MagicImg3);
    DDX_Control(pDX, IDC_MAGIC_IMG2, m_MagicImg2);
    DDX_Control(pDX, IDC_MAGIC_IMG1, m_MagicImg1);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMagic, CDialog)
    //{{AFX_MSG_MAP(CDlgMagic)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_MAGIC_IMG1, OnMagicImg1)
    ON_BN_CLICKED(IDC_MAGIC_IMG2, OnMagicImg2)
    ON_BN_CLICKED(IDC_MAGIC_IMG3, OnMagicImg3)
    ON_BN_CLICKED(IDC_MAGIC_IMG4, OnMagicImg4)
    ON_BN_CLICKED(IDC_MAGIC_IMG5, OnMagicImg5)
    ON_BN_CLICKED(IDC_MAGIC_IMG6, OnMagicImg6)
    ON_BN_CLICKED(IDC_MAGIC_IMG7, OnMagicImg7)
    ON_BN_CLICKED(IDC_MAGIC_IMG8, OnMagicImg8)
    ON_BN_CLICKED(IDC_MAGIC_IMG9, OnMagicImg9)
    ON_BN_CLICKED(IDC_MAGIC_IMG10, OnMagicImg10)
    ON_BN_CLICKED(IDC_MAGIC_BTN_DOWN, OnMagicBtnDown)
    ON_BN_CLICKED(IDC_MAGIC_BTN_UP, OnMagicBtnUp)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMagic message handlers

LRESULT CDlgMagic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_SHOWWINDOW )
    {
        m_bShow = wParam ;
        if (m_bShow)
        {
            DXPlaySound("Sound/Open.wav");
        }
        else
        {
            DXPlaySound("Sound/Close.wav");
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

HBRUSH CDlgMagic::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgMagic::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgMagic::GetWindowRect ( rect ) ;
    // Init the image
    m_MagicImg1.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_MagicImg2.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_MagicImg3.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_MagicImg4.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_MagicImg5.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_MagicImg6.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_MagicImg7.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_MagicImg8.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_MagicImg9.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_MagicImg10.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    // Init the button
    m_MagicBtnU.Init ( rect.left, rect.top, "Button815" ) ;
    m_MagicBtnU.EnableWindow ( false ) ;
    m_MagicBtnD.Init ( rect.left, rect.top, "Button816" ) ;
    m_MagicBtnD.EnableWindow ( false ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMagic::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgMagic::Show()
{
    // show the image
    if ( m_bShow )
    {
        m_MagicImg1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MagicImg2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MagicImg3.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MagicImg4.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MagicImg5.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MagicImg6.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MagicImg7.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MagicImg8.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MagicImg9.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MagicImg10.Show ( m_Pnt.x, m_Pnt.y ) ;
        if ( m_MagicBtnU.IsWindowEnabled () )
        {
            m_MagicBtnU.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        if ( m_MagicBtnD.IsWindowEnabled () )
        {
            m_MagicBtnD.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
    }
}

void CDlgMagic::InitMagicDialog()
{
    m_nCurBase = 0 ;
    int nAmount = g_objHero.GetMagicAmount() ;
    if ( nAmount > 10 )
    {
        m_MagicBtnU.EnableWindow ( true ) ;
        m_MagicBtnD.EnableWindow ( true ) ;
    }
    else
    {
        m_MagicBtnU.EnableWindow ( false ) ;
        m_MagicBtnD.EnableWindow ( false ) ;
    }
    for ( int i = 0; i < 10; i++ )
    {
        CMagic* pMagic = g_objHero.GetMagicByIndex(i) ;
        if ( pMagic )
        {
            if ( g_objHero.IsTransform () && pMagic->IsTransformMagic () )
            {
                continue ;
            }
            char SkillType[32] ;
            m_MagicIdType[i] = pMagic->GetIDType () ;
            sprintf ( SkillType, "MagicSkillType%d", m_MagicIdType[i] ) ;
            switch ( i )
            {
            case 0:
                m_MagicImg1.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                break ;
            case 1:
                m_MagicImg2.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                break ;
            case 2:
                m_MagicImg3.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                break ;
            case 3:
                m_MagicImg4.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                break ;
            case 4:
                m_MagicImg5.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                break ;
            case 5:
                m_MagicImg6.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                break ;
            case 6:
                m_MagicImg7.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                break ;
            case 7:
                m_MagicImg8.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                break ;
            case 8:
                m_MagicImg9.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                break ;
            case 9:
                m_MagicImg10.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                break ;
            }
        }
    }
    CDlgMagic::ShowWindow ( SW_SHOW ) ;
}

void CDlgMagic::OnMagicImg1()
{
    CDlgMagic::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_MagicIdType[0 + m_nCurBase] ) ;
    CDlgMagic::ShowWindow ( SW_HIDE ) ;
}

void CDlgMagic::OnMagicImg2()
{
    CDlgMagic::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_MagicIdType[1 + m_nCurBase] ) ;
    CDlgMagic::ShowWindow ( SW_HIDE ) ;
}

void CDlgMagic::OnMagicImg3()
{
    CDlgMagic::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_MagicIdType[2 + m_nCurBase] ) ;
    CDlgMagic::ShowWindow ( SW_HIDE ) ;
}

void CDlgMagic::OnMagicImg4()
{
    CDlgMagic::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_MagicIdType[3 + m_nCurBase] ) ;
    CDlgMagic::ShowWindow ( SW_HIDE ) ;
}

void CDlgMagic::OnMagicImg5()
{
    CDlgMagic::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_MagicIdType[4 + m_nCurBase] ) ;
    CDlgMagic::ShowWindow ( SW_HIDE ) ;
}

void CDlgMagic::OnMagicImg6()
{
    CDlgMagic::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_MagicIdType[5 + m_nCurBase] ) ;
    CDlgMagic::ShowWindow ( SW_HIDE ) ;
}

void CDlgMagic::OnMagicImg7()
{
    CDlgMagic::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_MagicIdType[6 + m_nCurBase] ) ;
    CDlgMagic::ShowWindow ( SW_HIDE ) ;
}

void CDlgMagic::OnMagicImg8()
{
    CDlgMagic::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_MagicIdType[7 + m_nCurBase] ) ;
    CDlgMagic::ShowWindow ( SW_HIDE ) ;
}

void CDlgMagic::OnMagicImg9()
{
    CDlgMagic::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_MagicIdType[8 + m_nCurBase] ) ;
    CDlgMagic::ShowWindow ( SW_HIDE ) ;
}

void CDlgMagic::OnMagicImg10()
{
    CDlgMagic::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MAGIC_CHOOSE, m_MagicIdType[9 + m_nCurBase] ) ;
    CDlgMagic::ShowWindow ( SW_HIDE ) ;
}

void CDlgMagic::OnMagicBtnDown()
{
    int nAmount = g_objHero.GetMagicAmount() ;
    if ( nAmount > 10 && m_nCurBase + 10 < nAmount )
    {
        m_nCurBase += 10 ;
        nAmount = ( nAmount > m_nCurBase + 10 ) ? m_nCurBase + 10 : nAmount ;
        CDlgMagic::MoveWindow ( _SCR_WIDTH - ( nAmount - m_nCurBase ) * MAGIC_ICON_SIZE - 18 - MAIN_DLG_SIZE,
                                _SCR_HEIGHT - MAIN_DLG_SIZE - MAGIC_ICON_SIZE * 3,
                                nAmount * MAGIC_ICON_SIZE + 18,
                                MAGIC_ICON_SIZE ) ;
        for ( int i = m_nCurBase; i < nAmount; i++ )
        {
            CMagic* pMagic = g_objHero.GetMagicByIndex(i) ;
            if ( pMagic )
            {
                char SkillType[32] ;
                m_MagicIdType[i] = pMagic->GetIDType () ;
                sprintf ( SkillType, "MagicSkillType%d", m_MagicIdType[i] ) ;
                switch ( i - m_nCurBase )
                {
                case 0:
                    m_MagicImg1.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 1:
                    m_MagicImg2.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 2:
                    m_MagicImg3.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 3:
                    m_MagicImg4.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 4:
                    m_MagicImg5.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 5:
                    m_MagicImg6.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 6:
                    m_MagicImg7.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 7:
                    m_MagicImg8.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 8:
                    m_MagicImg9.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 9:
                    m_MagicImg10.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                }
            }
        }
    }
}

void CDlgMagic::OnMagicBtnUp()
{
    if ( m_nCurBase > 0 )
    {
        CDlgMagic::MoveWindow ( _SCR_WIDTH - 10 * MAGIC_ICON_SIZE - 18 - MAIN_DLG_SIZE,
                                _SCR_HEIGHT - MAIN_DLG_SIZE - MAGIC_ICON_SIZE * 3,
                                10 * MAGIC_ICON_SIZE + 18,
                                MAGIC_ICON_SIZE ) ;
        m_nCurBase -= 10 ;
        for ( int i = m_nCurBase; i < m_nCurBase + 10; i++ )
        {
            CMagic* pMagic = g_objHero.GetMagicByIndex(i) ;
            if ( pMagic )
            {
                char SkillType[32] ;
                m_MagicIdType[i] = pMagic->GetIDType () ;
                sprintf ( SkillType, "MagicSkillType%d", m_MagicIdType[i] ) ;
                switch ( i - m_nCurBase )
                {
                case 0:
                    m_MagicImg1.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 1:
                    m_MagicImg2.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 2:
                    m_MagicImg3.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 3:
                    m_MagicImg4.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 4:
                    m_MagicImg5.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 5:
                    m_MagicImg6.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 6:
                    m_MagicImg7.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 7:
                    m_MagicImg8.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 8:
                    m_MagicImg9.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                case 9:
                    m_MagicImg10.InsertImage ( SkillType, 0, m_MagicIdType[i], true ) ;
                    break ;
                }
            }
        }
    }
}
