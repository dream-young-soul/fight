
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgMonsterNpc.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgMonsterNpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterNpc dialog


CDlgMonsterNpc::CDlgMonsterNpc(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMonsterNpc::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMonsterNpc)
    m_bShow = false;
    m_Pnt = CPoint(0, 0); // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgMonsterNpc::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMonsterNpc)
    DDX_Control(pDX, IDC_BTN_OPENHATCH, m_BtnOpenHatch);
    DDX_Control(pDX, IDC_BTN_OPENDEPOT, m_BtnOpenDepot);
    DDX_Control(pDX, IDC_BTN_IMPROVE, m_BtnImprove);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    DDX_Control(pDX, IDC_BTN_ALIVE, m_BtnAlive);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMonsterNpc, CDialog)
    //{{AFX_MSG_MAP(CDlgMonsterNpc)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_BTN_ALIVE, OnBtnAlive)
    ON_BN_CLICKED(IDC_BTN_IMPROVE, OnBtnImprove)
    ON_BN_CLICKED(IDC_BTN_OPENDEPOT, OnBtnOpendepot)
    ON_BN_CLICKED(IDC_BTN_OPENHATCH, OnBtnOpenhatch)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterNpc message handlers

LRESULT CDlgMonsterNpc::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        {
            m_bShow = wParam ;
            if (m_bShow)
            {
                DXPlaySound("Sound/Open.wav");
            }
            else
            {
                DXPlaySound("Sound/window_close.wav");
            }
        }
        break ;
    case WM_COMMAND:
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
        break ;
    default:
        break;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgMonsterNpc::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

BOOL CDlgMonsterNpc::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgMonsterNpc::GetWindowRect(rect);
    m_BtnAlive.Init(rect.left, rect.top, "Button657");
    m_BtnImprove.Init(rect.left, rect.top, "Button654");
    m_BtnOpenHatch.Init(rect.left, rect.top, "Button655");
    m_BtnOpenDepot.Init(rect.left, rect.top, "Button656");
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMonsterNpc::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgMonsterNpc::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    CDlgMonsterNpc::EnableWindow(false);
}

void CDlgMonsterNpc::OnBtnAlive()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgMonsterNpc::GetParent()->PostMessage(WM_MY_MESSAGE, ON_MONSTER_ALIVE, m_IdNpc);
    CDlgMonsterNpc::OnBtnClose();
}

void CDlgMonsterNpc::OnBtnImprove()
{
    // TODO: Add your control notification handler code here
    CDlgMonsterNpc::GetParent()->PostMessage(WM_MY_MESSAGE, ON_MONSTER_IMP, m_IdNpc);
    CDlgMonsterNpc::OnBtnClose();
}

void CDlgMonsterNpc::OnBtnOpendepot()
{
    // TODO: Add your control notification handler code here
    CDlgMonsterNpc::GetParent()->PostMessage(WM_MY_MESSAGE, ON_MONSTER_DEPOT, m_IdNpc);
    CDlgMonsterNpc::OnBtnClose();
}

void CDlgMonsterNpc::OnBtnOpenhatch()
{
    // TODO: Add your control notification handler code here
    CDlgMonsterNpc::GetParent()->PostMessage(WM_MY_MESSAGE, ON_MONSTER_HATCH, m_IdNpc);
    CDlgMonsterNpc::OnBtnClose();
}

void CDlgMonsterNpc::Show()
{
    if (m_bShow)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog83",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0,
                           m_Pnt.x ,
                           m_Pnt.y  );
        }
        else
        {
            return;
        }
        m_BtnOpenHatch.Show(m_Pnt.x, m_Pnt.y);
        m_BtnOpenDepot.Show(m_Pnt.x, m_Pnt.y);
        m_BtnAlive.Show(m_Pnt.x, m_Pnt.y);
        m_BtnImprove.Show(m_Pnt.x, m_Pnt.y);
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y );
    }
}

void CDlgMonsterNpc::OpenMonsterNpc(OBJID idNpc)
{
    m_IdNpc = idNpc;
    CDlgMonsterNpc::EnableWindow(true);
}
