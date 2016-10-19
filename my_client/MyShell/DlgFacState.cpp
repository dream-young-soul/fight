
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgFacState.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgFacState.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFacState dialog


CDlgFacState::CDlgFacState(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgFacState::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgFacState)
    m_bShow = false ;
    //}}AFX_DATA_INIT
}


void CDlgFacState::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgFacState)
    DDX_Control(pDX, IDC_STATE_IMAFACE, m_ImgFace);
    DDX_Control(pDX, IDC_STA_SYNJOB, m_StaSynJob);
    DDX_Control(pDX, IDC_STA_PROSITION, m_StaProsition);
    DDX_Control(pDX, IDC_STA_PK, m_StaPk);
    DDX_Control(pDX, IDC_STA_NOBILITY, m_StaNobility);
    DDX_Control(pDX, IDC_STA_NAME, m_StaName);
    DDX_Control(pDX, IDC_STA_MATE, m_StaMate);
    DDX_Control(pDX, IDC_STA_LEVEL, m_StaLevel);
    DDX_Control(pDX, IDC_FACSTATE_BTN_CLOSE, m_BtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFacState, CDialog)
    //{{AFX_MSG_MAP(CDlgFacState)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_FACSTATE_BTN_CLOSE, OnFacstateBtnClose)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFacState message handlers

LRESULT CDlgFacState::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if ( wParam )
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

HBRUSH CDlgFacState::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgFacState::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgFacState::GetWindowRect ( rect ) ;
    // Init the Static
    m_StaName.Init ( rect.left, rect.top ) ;
    m_StaMate.Init ( rect.left, rect.top ) ;
    m_StaProsition.Init ( rect.left, rect.top ) ;
    m_StaLevel.Init ( rect.left, rect.top ) ;
    m_StaNobility.Init ( rect.left, rect.top ) ;
    m_StaPk.Init ( rect.left, rect.top ) ;
    m_StaSynJob.Init ( rect.left, rect.top ) ;
    m_ImgFace.Init(rect.left, rect.top, NULL, FACE_IMAGE) ;
    // Init the button
    m_BtnClose.Init ( rect.left, rect.top, "Button540" ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFacState::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgFacState::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog47",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 3,
                            m_Pnt.x,
                            m_Pnt.y ) ;
        else
        {
            return ;
        }
        // Show the static
        m_StaName.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaMate.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaProsition.Show( m_Pnt.x, m_Pnt.y ) ;
        m_StaLevel.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaNobility.Show(m_Pnt.x, m_Pnt.y ) ;
        m_StaPk.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaSynJob.Show(m_Pnt.x, m_Pnt.y ) ;
        // Show the button
        m_ImgFace.Show(m_Pnt.x, m_Pnt.y ) ;
        m_BtnClose.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgFacState::SetFacState( const char* strMemName )
{
    m_ImgFace.RemoveImage();
    m_StaName.SetWindowText ( strMemName ) ;
    m_StaSynJob.SetWindowText ("") ;
    m_StaMate.SetWindowText ("") ;
    m_StaProsition.SetWindowText("");
    m_StaNobility.SetWindowText("");
    m_StaPk.SetWindowText("");
    m_StaLevel.SetWindowText("");
    SynMemberInfo* pMemberInfo = g_objHero.GetReturnSynMemberInfo () ;
    if ( pMemberInfo )
    {
        m_StaSynJob.SetWindowText ( pMemberInfo->szRank ) ;
        m_StaMate.SetWindowText (pMemberInfo->szMate) ;
        m_StaProsition.SetWindowText(g_objHero.GetProfessionalName(pMemberInfo->ucProfession));
        m_StaNobility.SetWindowText((char*)g_obj3DRoleData.GetNobilityTitle(pMemberInfo->ucNobilityRank));
        char szMsg[64] = "";
        sprintf(szMsg, "%d", pMemberInfo->sPk);
        m_StaPk.SetWindowText(szMsg);
        sprintf(szMsg, "%d", pMemberInfo->ucLevel);
        m_StaLevel.SetWindowText(szMsg);
        int nLook = pMemberInfo->dwLookFace % 1000;
        DWORD dwFace = (pMemberInfo->dwLookFace / 10000) % 1000;
        m_ImgFace.InsertImage(NULL, dwFace, nLook, true);
    }
    CDlgFacState::EnableWindow();
}

void CDlgFacState::OnFacstateBtnClose()
{
    CDlgFacState::EnableWindow(false);
}
