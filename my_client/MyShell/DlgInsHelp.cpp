
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgInsHelp.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgInsHelp.h"

#include "Hero.h"
#include "3DRoleData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int _PAGE_LINE = 10;
/////////////////////////////////////////////////////////////////////////////
// CDlgInsHelp dialog


CDlgInsHelp::CDlgInsHelp(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgInsHelp::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgInsHelp)
    m_bShow = false ;
    m_Pnt = CPoint (0, 0 ) ;
    m_bFlash = false ;
    m_bTipShow = false ;
    m_nCurTipIndex = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgInsHelp::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgInsHelp)
    DDX_Control(pDX, IDC_INSHELP_BTN_TIP, m_InsHelpBtnTip);
    DDX_Control(pDX, IDC_INSHELP_BTN_PAGEUP, m_InsHelpBtnPageUp);
    DDX_Control(pDX, IDC_INSHELP_BTN_PAGEDOWN, m_InsHelpBtnPageDown);
    DDX_Control(pDX, IDC_INSHELP_BTN_MORE, m_InsHelpBtnMore);
    DDX_Control(pDX, IDC_INSHELP_BTN_CLOSES, m_BtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInsHelp, CDialog)
    //{{AFX_MSG_MAP(CDlgInsHelp)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_INSHELP_BTN_CLOSES, OnInshelpBtnCloses)
    ON_BN_CLICKED(IDC_INSHELP_BTN_MORE, OnInshelpBtnMore)
    ON_BN_CLICKED(IDC_INSHELP_BTN_TIP, OnInshelpBtnTip)
    ON_BN_CLICKED(IDC_INSHELP_BTN_PAGEUP, OnInshelpBtnPageup)
    ON_BN_CLICKED(IDC_INSHELP_BTN_PAGEDOWN, OnInshelpBtnPagedown)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInsHelp message handlers

HBRUSH CDlgInsHelp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgInsHelp::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgInsHelp::GetWindowRect ( rect ) ;
    // Init the button
    m_BtnClose.Init ( rect.left, rect.top, "Button540" ) ;
    m_InsHelpBtnMore.Init ( rect.left, rect.top, "Button491" ) ;
    m_InsHelpBtnTip.Init ( rect.left, rect.top, "Button492" ) ;
    m_InsHelpBtnPageUp.Init ( rect.left, rect.top, "Button271" ) ;
    m_InsHelpBtnPageDown.Init ( rect.left, rect.top, "Button272" ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInsHelp::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgInsHelp::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog49",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y ) ;
            if (m_bTipShow)
            {
                ShowAni->Show( 1, m_Pnt.x + 160 , m_Pnt.y );
            }
            else
            {
                ShowAni->Show( 2, m_Pnt.x + 160 , m_Pnt.y );
            }
        }
        else
        {
            return ;
        }
        // Show the button
        m_BtnClose.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_InsHelpBtnMore.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_InsHelpBtnTip.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_InsHelpBtnPageUp.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_InsHelpBtnPageDown.Show ( m_Pnt.x, m_Pnt.y ) ;
        if ( m_bTipShow )
        {
            // Show the tip text
            int nAmount = g_objHero.GetTipsAmount ( ) ;
            nAmount = nAmount - m_nCurTipIndex ;
            if (nAmount > _PAGE_LINE)
            {
                nAmount = _PAGE_LINE;
            }
            int nLine = 0;
            for ( int i = m_nCurTipIndex; i < m_nCurTipIndex + nAmount; i++ )
            {
                char strTip[_MAX_WORDSSIZE] = "";
                strcpy(strTip, g_objHero.GetTipsByLine( i ));
                if (strcmp(strTip, "$") == 0)
                {
                    if (i - m_nCurTipIndex != 0)
                    {
                        nLine++;
                    }
                    continue;
                }
                CMyBitmap::ShowString ( m_Pnt.x + 30,
                                        m_Pnt.y + 82 + nLine * 18,
                                        0xffffff,
                                        g_objHero.GetTipsByLine(  i ) ) ;
                nLine++;
            }
        }
        else
        {
            // Show the normal text
            int nLine = 0;
            for(int n = m_nCurTipIndex; n < g_objHero.GetFlyHelpAmount(); n++)
            {
                int nAmount = g_objHero.GetHelpTipAmount (n) ;
                for ( int i = 0; i < nAmount; i++ )
                {
                    CMyBitmap::ShowString ( m_Pnt.x + 30,
                                            m_Pnt.y + 82 + nLine * 18,
                                            0xffffff,
                                            g_objHero.GetHelpTipLineByIndex (n, i ) ) ;
                    nLine++;
                    if (nLine >= _PAGE_LINE)
                    {
                        return;
                    }
                }
                nLine++;
                if (nLine >= _PAGE_LINE)
                {
                    break;
                }
            }
        }
    }
}

LRESULT CDlgInsHelp::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if (m_bShow)
        {
            DXPlaySound("Sound/Open.wav");
            CDlgInsHelp::ResetAcitvedHelp();
        }
        else
        {
            DXPlaySound("Sound/window_close.wav");
            g_objHero.ActivedHelpTips(m_nActivedHelpAmount);
            g_objHero.ResetFlyHelp();
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgInsHelp::OnInshelpBtnCloses();
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgInsHelp::OnInshelpBtnCloses()
{
    m_bTipShow = false ;
    m_nCurTipIndex = 0 ;
    CDlgInsHelp::EnableWindow(false);
}

void CDlgInsHelp::OnInshelpBtnMore()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    //	::ShellExecute(m_hWnd, NULL, g_objGameDataSet.GetStr ( 10367 ), NULL, NULL, SW_SHOW );
}


void CDlgInsHelp::OnInshelpBtnTip()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_bTipShow)
    {
        //		if (g_objHero.GetFlyHelpAmount() > 0)
        {
            m_bTipShow = false;
            m_nCurTipIndex = 0;
            m_InsHelpBtnTip.ChangeImage("Button492");
        }
    }
    else
    {
        g_objHero.SetTips();
        //		if (g_objHero.GetTipsAmount()>0)
        {
            m_bTipShow = true;
            m_nCurTipIndex = 0;
            m_InsHelpBtnTip.ChangeImage("Button493");
        }
    }
}

void CDlgInsHelp::OnInshelpBtnPageup()
{
    DXPlaySound("Sound/up_Page.wav");
    if (m_bTipShow)
    {
        if ( m_nCurTipIndex > 0 )
        {
            m_nCurTipIndex -= _PAGE_LINE ;
        }
    }
    else
    {
        if ( m_nCurTipIndex > 0 )
        {
            m_nCurTipIndex-- ;
        }
    }
}

void CDlgInsHelp::OnInshelpBtnPagedown()
{
    DXPlaySound("Sound/up_Page.wav");
    if (m_bTipShow)
    {
        if ( m_nCurTipIndex < g_objHero.GetTipsAmount () - _PAGE_LINE )
        {
            m_nCurTipIndex += _PAGE_LINE ;
        }
    }
    else
    {
        if ( m_nCurTipIndex + GetCurPageHelpAmount() < g_objHero.GetFlyHelpAmount ())
        {
            m_nCurTipIndex++ ;
            if (m_nActivedHelpAmount <= g_objHero.GetFlyHelpAmount())
            {
                m_nActivedHelpAmount++;
                g_objHero.ResetFlyHelpIndex(m_nActivedHelpAmount + 1);
            }
        }
    }
}

void CDlgInsHelp::ResetAcitvedHelp()
{
    m_nActivedHelpAmount = GetCurPageHelpAmount();
    g_objHero.ResetFlyHelpIndex(m_nActivedHelpAmount + 1);
    m_nCurTipIndex = 0;
    if (g_objHero.GetFlyHelpAmount() == 0)
    {
        m_InsHelpBtnTip.ChangeImage("Button493");
        g_objHero.SetTips();
        m_bTipShow = true;
    }
    else
    {
        m_InsHelpBtnTip.ChangeImage("Button492");
        m_bTipShow = false;
        m_nCurTipIndex = 0;
    }
}

int CDlgInsHelp::GetCurPageHelpAmount()
{
    int nLine = 0;
    int n = m_nCurTipIndex;
    for(; n < g_objHero.GetFlyHelpAmount(); n++)
    {
        int nAmount = g_objHero.GetHelpTipAmount (n) ;
        for ( int i = 0; i < nAmount; i++ )
        {
            nLine++;
            if (nLine >= _PAGE_LINE)
            {
                return n;
            }
        }
        nLine++;
        if (nLine >= _PAGE_LINE)
        {
            return n;
        }
    }
    return n;
}
