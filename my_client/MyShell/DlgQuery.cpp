
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgQuery.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgQuery.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		MY_QUERY_ROLE		1
#define		MY_QUERY_WEAPON		2
#define		MY_QUERY_MISSION	3
#define		MY_QUERY_SKILL		4
#define		MY_QUERY_TITLE		5

#ifdef _SOUND_DISABLE
#define DXPlaySound //
#endif
/////////////////////////////////////////////////////////////////////////////
// CDlgQuery dialog


CDlgQuery::CDlgQuery(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgQuery::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgQuery)
    m_bLoad = false ;
    m_bMouseMove = false;
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_btCurChildDlgID = 0 ;
    m_dwLastShowTime = 0 ;
    m_fFrameStep = 0.0f ;
    m_nFrameMode = 0 ;
    m_nPntX = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgQuery::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgQuery)
    DDX_Control(pDX, IDC_CHK_SKILL, m_ChkSkill);
    DDX_Control(pDX, IDC_CHK_ROLE, m_ChkRole);
    DDX_Control(pDX, IDC_CHK_EQUIP, m_ChkEquip);
    DDX_Control(pDX, IDC_CHK_TSKILL, m_ChkTSkill);
    DDX_Control(pDX, IDC_QUERY_NAME, m_StaName);
    DDX_Control(pDX, IDC_QUERY_FACTION, m_StaFaction);
    DDX_Control(pDX, IDC_QUERY_CLOSE, m_QueryCloseBtn);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgQuery, CDialog)
    //{{AFX_MSG_MAP(CDlgQuery)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_QUERY_CLOSE, OnQueryClose)
    ON_BN_CLICKED(IDC_CHK_EQUIP, OnChkEquip)
    ON_BN_CLICKED(IDC_CHK_ROLE, OnChkRole)
    ON_BN_CLICKED(IDC_CHK_SKILL, OnChkSkill)
    ON_BN_CLICKED(IDC_CHK_TSKILL, OnChkTskill)
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgQuery message handlers

void CDlgQuery::Show()
{
    if ( m_bLoad && m_bShow )
    {
        if ( m_nFrameMode != 0 )
        {
            if ( TimeGet() - m_dwLastShowTime <= 300 )
            {
                m_Pnt.x = (LONG)(m_nPntX - ( TimeGet() - m_dwLastShowTime ) / 15 * m_fFrameStep);
            }
            else
            {
                CRect rect;
                CDlgQuery::GetWindowRect(rect);
                m_Pnt.x = _SCR_WIDTH - rect.Width() - 100;
                m_nFrameMode = 0 ;
                m_ChkEquip.SetTipShow ((char*)g_objGameDataSet.GetStr(10735));
                m_ChkRole.SetTipShow ((char*)g_objGameDataSet.GetStr(10736));
                m_ChkSkill.SetTipShow ((char*)g_objGameDataSet.GetStr(10737));
                m_ChkTSkill.SetTipShow ((char*)g_objGameDataSet.GetStr(10738));
                CDlgQuery::ShowCurDlg( m_btCurChildDlgID ) ;
            }
        }
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog54",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            if (m_btCurChildDlgID == 2 )
            {
                ShowAni->Show ( m_btCurChildDlgID,
                                m_Pnt.x - 1 + 14,
                                m_Pnt.y) ;
            }
            else if (m_btCurChildDlgID == 3)
            {
                ShowAni->Show ( 2,
                                m_Pnt.x - 1 + 14,
                                m_Pnt.y) ;
            }
            else
                ShowAni->Show ( m_btCurChildDlgID,
                                m_Pnt.x + 14,
                                m_Pnt.y) ;
            CAni* pShowAni = g_objGameDataSet.GetDataAni((char*)g_strControlAni,
                             "Dialog55", EXIGENCE_IMMEDIATE);
            if (pShowAni != NULL)
            {
                pShowAni->Show( 0, m_Pnt.x, m_Pnt.y + 54);
                pShowAni->Show( 1, m_Pnt.x, m_Pnt.y + 128 + 54);
                pShowAni->Show( 2, m_Pnt.x, m_Pnt.y + 256 + 54);
            }
        }
        else
        {
            return ;
        }
        // Show child dlg
        ShowChildDlg () ;
        // Show button
        char strText[64] = "";
        strcpy(strText, g_objHero.GetName());
        m_StaName.SetWindowText(g_objHero.GetName());
        m_StaName.Show( m_Pnt.x, m_Pnt.y);
        if (strcmp(g_objHero.GetSyndicate(), g_objGameDataSet.GetStr(100122)) == 0)		//ÎÞ
        {
            sprintf(strText, "");
        }
        else
        {
            sprintf(strText, "%s", g_objHero.GetSyndicate());
        }
        m_StaFaction.SetWindowText(strText);
        m_StaFaction.Show(m_Pnt.x, m_Pnt.y);
        m_ChkRole.Show(m_Pnt.x, m_Pnt.y);
        m_ChkEquip.Show(m_Pnt.x, m_Pnt.y);
        m_ChkSkill.Show(m_Pnt.x, m_Pnt.y);
        m_ChkTSkill.Show(m_Pnt.x, m_Pnt.y);
        m_QueryCloseBtn.Show( m_Pnt.x, m_Pnt.y ) ;
        if (m_btCurChildDlgID == 2 || m_btCurChildDlgID == 3)
        {
            m_DlgSkill.ShowSkillInfo(m_Pnt.x, m_Pnt.y);
        }
    }
}

HBRUSH CDlgQuery::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgQuery::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgQuery::GetWindowRect ( rect ) ;
    m_Height = rect.Height() ;
    m_Width = rect.Width() ;
    m_fFrameStep = ( float ) (rect.Width() + 100) / DLG_FRAME_AMOUNT ;
    // Init the button
    m_QueryCloseBtn.Init ( rect.left, rect.top, "Button540" );
    m_ChkEquip.Init(rect.left, rect.top, "Check71", 2);
    m_ChkRole.Init(rect.left, rect.top, "Check70", 2);
    m_ChkSkill.Init(rect.left, rect.top, "Check72", 2);
    m_ChkTSkill.Init(rect.left, rect.top, "Check73", 2);
    m_StaName.Init( rect.left, rect.top );
    m_StaName.SetStaticPosType(1);
    m_StaFaction.Init( rect.left, rect.top);
    m_StaFaction.SetStaticPosType(1);
    m_DlgEquip.Create(IDD_DIALOG_EQUIP, this);
    m_DlgEquip.ShowWindow(SW_SHOW);
    m_DlgEquip.EnableWindow();
    m_DlgRole.Create(IDD_DIALOG_QUERY_ROLE, this);
    m_DlgRole.ShowWindow(SW_SHOW);
    m_DlgRole.EnableWindow(false);
    m_DlgSkill.Create(IDD_DIALOG_SKILL, this);
    m_DlgSkill.ShowWindow(SW_SHOW);
    m_DlgSkill.EnableWindow(false);
    m_bLoad = true ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgQuery::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_nPntX = x ;
    m_Pnt.y = y ;
}

void CDlgQuery::OnQueryClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound( "Sound/Dlg_Ok.wav" ) ;
    CDlgQuery::EnableWindow( false ) ;
    this->GetParent()->PostMessage(WM_MY_MESSAGE, ON_NPCBUY_FIX, DLG_QUERY);
}



LRESULT CDlgQuery::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        {
            m_bShow = wParam ;
            if ( wParam == false )
            {
                CDlgQuery::HideCurDlg( m_btCurChildDlgID ) ;
                DXPlaySound( "Sound/window_close.wav" ) ;
            }
            else
            {
                m_dwLastShowTime = TimeGet() ;
                m_nFrameMode = 1 ;
                CRect rect ;
                CDlgQuery::GetWindowRect( rect ) ;
                m_nPntX = _SCR_WIDTH ;
                m_ChkEquip.SetTipShow ("");
                m_ChkRole.SetTipShow ("");
                m_ChkSkill.SetTipShow ("");
                m_ChkTSkill.SetTipShow ("");
                DXPlaySound("Sound/Open.wav");
            }
            if ( !m_bShow )
            {
                CDlgQuery::SetAddPoint () ;
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
                CDlgQuery::OnQueryClose () ;
                return true ;
            }
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgQuery::ShowCurDlg(BYTE btCurDlgID)
{
    switch(btCurDlgID)
    {
    case 0:
        m_DlgEquip.EnableWindow();
        break;
    case 1:
        m_DlgRole.EnableWindow();
        break;
    case 2:
        m_DlgSkill.EnableWindow();
        break;
    case 3:
        m_DlgSkill.EnableWindow();
    default:
        break;
    }
}

void CDlgQuery::HideCurDlg(BYTE btCurDlgID)
{
    switch(btCurDlgID)
    {
    case 0:
        m_DlgEquip.EnableWindow(false);
        break;
    case 1:
        m_DlgRole.EnableWindow(false);
        break;
    case 2:
        m_DlgSkill.EnableWindow(false);
        break;
    case 3:
        m_DlgSkill.EnableWindow(false);
        break;
    default:
        break;
    }
}

void CDlgQuery::ShowChildDlg()
{
    switch( m_btCurChildDlgID )
    {
    case 0:
        m_DlgEquip.Show(m_Pnt.x, m_Pnt.y);
        break;
    case 1:
        m_DlgRole.Show(m_Pnt.x, m_Pnt.y);
        break;
    case 2:
        m_DlgSkill.Show(m_Pnt.x, m_Pnt.y);
        break;
    case 3:
        m_DlgSkill.Show(m_Pnt.x, m_Pnt.y);
        break;
    default:
        break;
    }
}

void CDlgQuery::PopupCurChk()
{
    switch(m_btCurChildDlgID)
    {
    case 0:
        m_ChkEquip.SetCheckValue(0);
        break;
    case 1:
        m_ChkRole.SetCheckValue(0);
        break;
    case 2:
        m_ChkSkill.SetCheckValue(0);
        break;
    case 3:
        m_ChkTSkill.SetCheckValue(0);
        break;
    default:
        break;
    }
}


void CDlgQuery::SetAddPoint()
{
    /*	char strText[16] ;
    	int nForce, nSpeed, nHealth, nSoul ;

    	m_DlgRole.m_RoleSta8.GetWindowText ( strText, 16 ) ;
    	nForce = atoi ( strText ) ;
    	m_DlgRole.m_RoleSta9.GetWindowText ( strText, 16 ) ;
    	nSpeed = atoi ( strText ) ;
    	m_DlgRole.m_RoleSta10.GetWindowText ( strText, 16 ) ;
    	nHealth = atoi ( strText ) ;
    	m_DlgRole.m_RoleSta11.GetWindowText ( strText, 16 ) ;
    	nSoul = atoi ( strText ) ;

    	g_objHero.AllotPoint ( nForce, nSpeed, nHealth, nSoul ) ;*/
}

void CDlgQuery::OnChkEquip()
{
    // TODO: Add your control notification handler code here
    DXPlaySound( "Sound/up_Page.wav" ) ;
    if (m_ChkEquip.GetCheckValue() == 1)
    {
        HideCurDlg(m_btCurChildDlgID);
        PopupCurChk();
        m_btCurChildDlgID = 0;
        ShowCurDlg(m_btCurChildDlgID);
    }
    else
    {
        m_ChkEquip.SetCheckValue(1);
    }
}

void CDlgQuery::OnChkRole()
{
    // TODO: Add your control notification handler code here
    DXPlaySound( "Sound/up_Page.wav" ) ;
    if (m_ChkRole.GetCheckValue() == 1)
    {
        HideCurDlg(m_btCurChildDlgID);
        PopupCurChk();
        m_btCurChildDlgID = 1;
        ShowCurDlg(m_btCurChildDlgID);
    }
    else
    {
        m_ChkRole.SetCheckValue(1);
    }
    this->GetParent()->PostMessage(WM_MY_MESSAGE, ON_NPCBUY_FIX, DLG_QUERY);
}

void CDlgQuery::OnChkSkill()
{
    // TODO: Add your control notification handler code here
    DXPlaySound( "Sound/up_Page.wav" ) ;
    if (m_ChkSkill.GetCheckValue() == 1)
    {
        HideCurDlg(m_btCurChildDlgID);
        PopupCurChk();
        m_btCurChildDlgID = 2;
        ShowCurDlg(m_btCurChildDlgID);
        m_DlgSkill.SetMagicSkill();
    }
    else
    {
        m_ChkSkill.SetCheckValue(1);
    }
    this->GetParent()->PostMessage(WM_MY_MESSAGE, ON_NPCBUY_FIX, DLG_QUERY);
}

void CDlgQuery::OnChkTskill()
{
    // TODO: Add your control notification handler code here
    DXPlaySound( "Sound/up_Page.wav" ) ;
    if (m_ChkTSkill.GetCheckValue() == 1)
    {
        HideCurDlg(m_btCurChildDlgID);
        PopupCurChk();
        m_btCurChildDlgID = 3;
        ShowCurDlg(m_btCurChildDlgID);
        m_DlgSkill.SetTeamMagicSkill();
    }
    else
    {
        m_ChkTSkill.SetCheckValue(1);
    }
    this->GetParent()->PostMessage(WM_MY_MESSAGE, ON_NPCBUY_FIX, DLG_QUERY);
}

void CDlgQuery::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnMouseMove(nFlags, point);
}

