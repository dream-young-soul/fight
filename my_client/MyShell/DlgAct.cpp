
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgAct.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgAct.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAct dialog


CDlgAct::CDlgAct(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgAct::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgAct)
    m_bLoad = false ;
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    //}}AFX_DATA_INIT
}


void CDlgAct::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgAct)
    DDX_Control(pDX, IDC_ACT_BTN1, m_ActBtn1);
    DDX_Control(pDX, IDC_ACT_BTN2, m_ActBtn2);
    DDX_Control(pDX, IDC_ACT_BTN3, m_ActBtn3);
    DDX_Control(pDX, IDC_ACT_BTN4, m_ActBtn4);
    DDX_Control(pDX, IDC_ACT_BTN5, m_ActBtn5);
    DDX_Control(pDX, IDC_ACT_BTN6, m_ActBtn6);
    DDX_Control(pDX, IDC_ACT_BTN7, m_ActBtn7);
    DDX_Control(pDX, IDC_ACT_BTN8, m_ActBtn8);
    DDX_Control(pDX, IDC_ACT_BTN9, m_ActBtn9);
    DDX_Control(pDX, IDC_ACT_BTN10, m_ActBtn10);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_CloseBtn);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAct, CDialog)
    //{{AFX_MSG_MAP(CDlgAct)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_ACT_BTN1, OnActBtn1)
    ON_BN_CLICKED(IDC_ACT_BTN2, OnActBtn2)
    ON_BN_CLICKED(IDC_ACT_BTN3, OnActBtn3)
    ON_BN_CLICKED(IDC_ACT_BTN4, OnActBtn4)
    ON_BN_CLICKED(IDC_ACT_BTN5, OnActBtn5)
    ON_BN_CLICKED(IDC_ACT_BTN6, OnActBtn6)
    ON_BN_CLICKED(IDC_ACT_BTN7, OnActBtn7)
    ON_BN_CLICKED(IDC_ACT_BTN8, OnActBtn8)
    ON_BN_CLICKED(IDC_ACT_BTN9, OnActBtn9)
    ON_BN_CLICKED(IDC_ACT_BTN10, OnActBtn10)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAct message handlers

void CDlgAct::Show()
{
    if ( m_bLoad && m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog22",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y ) ;
        else
        {
            return ;
        }
        // Show the button
        m_ActBtn1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn3.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn4.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn5.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn6.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn7.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn8.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn9.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn10.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_CloseBtn.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

LRESULT CDlgAct::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_SHOWWINDOW )
    {
        if ( wParam == false )
        {
            m_bShow = false ;
        }
        else
        {
            m_bShow = true ;
        }
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 2 )
        {
            CDlgAct::OnBtnClose () ;
            return true ;
        }
        else if ( wParam == 1 )
        {
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgAct::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgAct::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgAct::GetWindowRect ( rect ) ;
    // Init the buttons
    /*	char strName[32] ;
    	sprintf ( strName, "ButtonA%d", _ACTION_COOL ) ;
    	if ( ! ( m_ActBtn1.Init ( rect.left, rect.top, strName, DLG_ACT, _ACTION_COOL ) ) )
    		return false ;
    	sprintf ( strName, "ButtonA%d", _ACTION_GENUFLECT ) ;
    	if ( ! ( m_ActBtn2.Init ( rect.left, rect.top, strName, DLG_ACT, _ACTION_GENUFLECT ) ) )
    		return false ;
    	sprintf ( strName, "ButtonA%d", _ACTION_SAD ) ;
    	if ( ! ( m_ActBtn3.Init ( rect.left, rect.top, strName, DLG_ACT, _ACTION_SAD ) ) )
    		return false ;
    	sprintf ( strName, "ButtonA%d", _ACTION_LAUGH ) ;
    	if ( ! ( m_ActBtn4.Init ( rect.left, rect.top, strName, DLG_ACT, _ACTION_LAUGH ) ) )
    		return false ;
    	sprintf ( strName, "ButtonA%d", _ACTION_FURY ) ;
    	if ( ! ( m_ActBtn5.Init ( rect.left, rect.top, strName, DLG_ACT, _ACTION_FURY ) ) )
    		return false ;
    	sprintf ( strName, "ButtonA%d", _ACTION_FAINT ) ;
    	if ( ! ( m_ActBtn6.Init ( rect.left, rect.top, strName, DLG_ACT, _ACTION_FAINT ) ) )
    		return false ;
    	sprintf ( strName, "ButtonA%d", _ACTION_DANCE1 ) ;
    	if ( ! ( m_ActBtn7.Init ( rect.left, rect.top, strName, DLG_ACT, _ACTION_DANCE1 ) ) )
    		return false ;
    	sprintf ( strName, "ButtonA%d", _ACTION_SAYHELLO ) ;
    	if ( ! ( m_ActBtn8.Init ( rect.left, rect.top, strName, DLG_ACT, _ACTION_SAYHELLO ) ) )
    		return false ;
    	sprintf ( strName, "ButtonA%d", _ACTION_SALUTE ) ;
    	if ( ! ( m_ActBtn9.Init ( rect.left, rect.top, strName, DLG_ACT, _ACTION_SALUTE ) ) )
    		return false ;
    	sprintf ( strName, "ButtonA%d", _ACTION_SITDOWN ) ;
    	if ( ! ( m_ActBtn10.Init ( rect.left, rect.top, strName, DLG_ACT, _ACTION_SITDOWN ) ) )
    		return false ;
    	if ( ! ( m_CloseBtn.Init ( rect.left, rect.top, "Button5" ) ) )
    		return false ;*/
    m_bLoad = true ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAct::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgAct::OnActBtn1()
{
    g_objHero.Emotion(_ACTION_COOL);
}

void CDlgAct::OnActBtn2()
{
    g_objHero.Emotion(_ACTION_GENUFLECT);
}

void CDlgAct::OnActBtn3()
{
    g_objHero.Emotion(_ACTION_SAD);
}

void CDlgAct::OnActBtn4()
{
    g_objHero.Emotion(_ACTION_LAUGH);
}

void CDlgAct::OnActBtn5()
{
    g_objHero.Emotion(_ACTION_FURY);
}

void CDlgAct::OnActBtn6()
{
    g_objHero.Emotion(_ACTION_FAINT);
}

void CDlgAct::OnActBtn7()
{
    g_objHero.Emotion(_ACTION_DANCE1);
}

void CDlgAct::OnActBtn8()
{
    g_objHero.Emotion(_ACTION_SAYHELLO);
}

void CDlgAct::OnActBtn9()
{
    g_objHero.Emotion(_ACTION_SALUTE);
}

void CDlgAct::OnActBtn10()
{
    g_objHero.Emotion(_ACTION_SITDOWN);
}

void CDlgAct::OnBtnClose()
{
    CDlgAct::ShowWindow ( SW_HIDE ) ;
}

void CDlgAct::SetCurAct(int nIndex)
{
    /*	switch ( nIndex )
    	{
    	case _ACTION_COOL:
    		CDlgAct::OnActBtn1() ;
    		break ;
    	case _ACTION_GENUFLECT:
    		CDlgAct::OnActBtn2() ;
    		break ;
    	case _ACTION_SAD:
    		CDlgAct::OnActBtn3() ;
    		break ;
    	case _ACTION_LAUGH:
    		CDlgAct::OnActBtn4() ;
    		break ;
    	case _ACTION_FURY:
    		CDlgAct::OnActBtn5() ;
    		break ;
    	case _ACTION_FAINT:
    		CDlgAct::OnActBtn6() ;
    		break ;
    	case _ACTION_DANCE1:
    		CDlgAct::OnActBtn7() ;
    		break ;
    	case _ACTION_SAYHELLO:
    		CDlgAct::OnActBtn8() ;
    		break ;
    	case _ACTION_SALUTE:
    		CDlgAct::OnActBtn9() ;
    		break ;
    	case _ACTION_SITDOWN:
    		CDlgAct::OnActBtn10() ;
    		break ;
    	}*/
}
