
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgDiceList.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgDiceList.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDiceList dialog


CDlgDiceList::CDlgDiceList(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgDiceList::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgDiceList)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    //}}AFX_DATA_INIT
}


void CDlgDiceList::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgDiceList)
    DDX_Control(pDX, IDC_DICELIST_LIST, m_DiceListList);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDiceList, CDialog)
    //{{AFX_MSG_MAP(CDlgDiceList)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDiceList message handlers

LRESULT CDlgDiceList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_SHOWWINDOW )
    {
        m_bShow = wParam ;
    }
    else if ( message == WM_COMMAND )
        if ( wParam == 1 || wParam == 2 )
        {
            return true ;
        }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgDiceList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgDiceList::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgDiceList::GetWindowRect ( rect ) ;
    // Init the list box
    m_DiceListList.Init ( rect.left, rect.top, NULL, SIMPLE_LIST ) ;
    m_DiceListList.SetFontColor ( 0xffff00 ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDiceList::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgDiceList::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog44",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y + 2 ) ;
        else
        {
            return ;
        }
        // Show the list box
        m_DiceListList.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgDiceList::SetDiceList()
{
    m_DiceListList.ResetContent () ;
    int nAmount = g_objHero.m_objDiceManager.GetPlayerListAmount () ;
    for ( int i = 0; i < nAmount; i++ )
    {
        DicePlayer* pDicePlayer = g_objHero.m_objDiceManager.GetPlayerListByIndex ( i ) ;
        if ( pDicePlayer )
        {
            char strText[32] ;
            sprintf ( strText, "%s  %d", pDicePlayer->szName, pDicePlayer->dwBonus ) ;
            m_DiceListList.AddString ( strText ) ;
        }
    }
}
