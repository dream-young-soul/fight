
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgTboardDetial.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgTboardDetial.h"

#include "Hero.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTboardDetial dialog


CDlgTboardDetial::CDlgTboardDetial(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTboardDetial::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTboardDetial)
    m_bShow = false;
    m_nViewMode = 0;
    m_Pnt = CPoint(0, 0);	// NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgTboardDetial::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTboardDetial)
    DDX_Control(pDX, IDC_STA_TITLE, m_StaTitle);
    DDX_Control(pDX, IDC_STA_TDEGREE, m_StaTDegree);
    DDX_Control(pDX, IDC_STA_NAME, m_StaName);
    DDX_Control(pDX, IDC_STA_LEVEL, m_StaLevel);
    DDX_Control(pDX, IDC_STA_JOB, m_StaJob);
    DDX_Control(pDX, IDC_STA_INFO, m_StaInfo);
    DDX_Control(pDX, IDC_BTN_HELP, m_BtnHelp);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    DDX_Control(pDX, IDC_BTN_CANCELBOARD, m_BtnCancelBoard);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTboardDetial, CDialog)
    //{{AFX_MSG_MAP(CDlgTboardDetial)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CANCELBOARD, OnBtnCancelboard)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTboardDetial message handlers

void CDlgTboardDetial::Show()
{
    if (m_bShow)
    {
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                         "Dialog78",
                         EXIGENCE_IMMEDIATE );
        if (pShowAni != NULL)
        {
            if (m_nViewMode == 1)
            {
                pShowAni->Show(m_nViewMode, m_Pnt.x, m_Pnt.y - 18);
            }
            else
            {
                pShowAni->Show(m_nViewMode, m_Pnt.x, m_Pnt.y );
            }
        }
        if (m_BtnCancelBoard.IsWindowEnabled())
        {
            m_BtnCancelBoard.Show(m_Pnt.x, m_Pnt.y );
        }
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y );
        m_BtnHelp.Show(m_Pnt.x, m_Pnt.y );
        m_StaName.Show(m_Pnt.x, m_Pnt.y );
        m_StaLevel.Show(m_Pnt.x, m_Pnt.y );
        m_StaJob.Show(m_Pnt.x, m_Pnt.y );
        m_StaTDegree.Show(m_Pnt.x, m_Pnt.y );
        m_StaTitle.Show(m_Pnt.x, m_Pnt.y );
        m_StaInfo.Show(m_Pnt.x, m_Pnt.y );
    }
}

LRESULT CDlgTboardDetial::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( message == WM_ENABLE )
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
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgTboardDetial::OnBtnCancelboard();
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgTboardDetial::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgTboardDetial::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgTboardDetial::GetWindowRect(rect);
    m_BtnCancelBoard.Init(rect.left, rect.top, "Button1713");
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    m_BtnHelp.Init(rect.left, rect.top, "Button15");
    m_StaName.Init(rect.left, rect.top);
    m_StaLevel.Init(rect.left, rect.top);
    m_StaJob.Init(rect.left, rect.top);
    m_StaTDegree.Init(rect.left, rect.top);
    m_StaTitle.Init(rect.left, rect.top, 2);
    m_StaInfo.Init(rect.left, rect.top, 2);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTboardDetial::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgTboardDetial::OnBtnCancelboard()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_nViewMode == 1)
    {
        g_objHero.CancelTeacherBoard(g_objHero.m_infoAnnounce.idAnnounce);
        CDlgTboardDetial::EnableWindow(false);
    }
    else	//request teacher announce
    {
        if (g_objHero.GetLev() >= 40 )
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10640));
        }
        else if (g_objHero.IsHaveTeacher())
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10641));
        }
        else
        {
            g_objHero.RequestTeacherBoard(g_objHero.m_infoAnnounce.user_id);
        }
        CDlgTboardDetial::EnableWindow(false);
    }
}

void CDlgTboardDetial::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgTboardDetial::EnableWindow(false);
}

void CDlgTboardDetial::OnBtnHelp()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
}

void CDlgTboardDetial::SetInfoMode(int nMode)
{
    m_nViewMode = nMode;
    if (m_nViewMode == 1)
    {
        CRect rect;
        CDlgTboardDetial::GetWindowRect(rect);
        CDlgTboardDetial::MoveWindow(0,
                                     450 - rect.Height(), rect.Width(), rect.Height(), false);
    }
    else
    {
        CRect rect;
        CDlgTboardDetial::GetWindowRect(rect);
        CDlgTboardDetial::MoveWindow(_SCR_WIDTH - rect.Width() - 350,
                                     446 - rect.Height(), rect.Width(), rect.Height(), false);
    }
}

void CDlgTboardDetial::SetTboardInfo()
{
    m_StaName.SetWindowText(g_objHero.m_infoAnnounce.szName.c_str());
    if (strcmp(g_objHero.m_infoAnnounce.szName.c_str(), g_objHero.GetName()) == 0)
    {
        m_BtnCancelBoard.ChangeImage("Button1713");
        m_nViewMode = 1;
    }
    else
    {
        m_BtnCancelBoard.ChangeImage("Button1711");
        m_nViewMode = 0;
    }
    m_StaJob.SetWindowText(g_objHero.GetProfessionalName(g_objHero.m_infoAnnounce.profession));
    char strmsg[16] = "";
    sprintf(strmsg, "%d", g_objHero.m_infoAnnounce.level);
    m_StaLevel.SetWindowText(strmsg);
    int nStrTeacherLev = 10650 + g_objHero.m_infoAnnounce.teacher_level - 1;
    m_StaTDegree.SetWindowText(g_objGameDataSet.GetStr(nStrTeacherLev));
    m_StaTitle.SetWindowText(g_objHero.m_infoAnnounce.szTitle.c_str());
    m_StaInfo.SetWindowText(g_objHero.m_infoAnnounce.szDesc.c_str());
}
