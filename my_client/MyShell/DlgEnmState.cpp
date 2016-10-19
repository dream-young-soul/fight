
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgEnmState.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgEnmState.h"

#include "Hero.h"
#include "Enemy.h"
#include "3dRoleData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEnmState dialog


CDlgEnmState::CDlgEnmState(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgEnmState::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgEnmState)
    m_bShow = false ;
    m_iShowMode = 0;
    //}}AFX_DATA_INIT
}


void CDlgEnmState::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgEnmState)
    DDX_Control(pDX, IDC_STA_NOBILITY, m_StaNobility);
    DDX_Control(pDX, IDC_ENMSTATE_IMAFACE, m_EnmStateImaface);
    DDX_Control(pDX, IDC_STA6, m_Sta6);
    DDX_Control(pDX, IDC_STA5, m_Sta5);
    DDX_Control(pDX, IDC_STA1, m_Sta1);
    DDX_Control(pDX, IDC_STA2, m_Sta2);
    DDX_Control(pDX, IDC_STA3, m_Sta3);
    DDX_Control(pDX, IDC_STA4, m_Sta4);
    DDX_Control(pDX, IDC_STA7, m_Sta7);
    DDX_Control(pDX, IDC_ENMSTATE_BTN_CLOSE, m_EnmStateBtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEnmState, CDialog)
    //{{AFX_MSG_MAP(CDlgEnmState)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_ENMSTATE_BTN_CLOSE, OnEnmstateBtnClose)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnmState message handlers

LRESULT CDlgEnmState::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if ( wParam )
        {
            DXPlaySound("Sound/Open.wav");
            SetEnemyNull("");
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
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgEnmState::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgEnmState::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgEnmState::GetWindowRect ( rect ) ;
    // Init the image
    m_EnmStateImaface.Init ( rect.left, rect.top, NULL, FACE_IMAGE ) ;
    // Init the Static
    m_Sta1.Init ( rect.left, rect.top ) ;
    m_Sta2.Init ( rect.left, rect.top ) ;
    m_Sta3.Init ( rect.left, rect.top ) ;
    m_Sta4.Init ( rect.left, rect.top ) ;
    m_Sta5.Init ( rect.left, rect.top ) ;
    m_Sta6.Init ( rect.left, rect.top ) ;
    m_Sta7.Init ( rect.left, rect.top ) ;
    m_StaNobility.Init(rect.left, rect.top);
    // Init the button
    m_EnmStateBtnClose.Init ( rect.left, rect.top, "Button540" ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEnmState::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgEnmState::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog47",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            if (m_iShowMode == 2)
            {
                ShowAni->Show ( m_iShowMode,
                                m_Pnt.x - 2,
                                m_Pnt.y - 1 ) ;
            }
            else
                ShowAni->Show ( m_iShowMode,
                                m_Pnt.x,
                                m_Pnt.y  ) ;
        }
        else
        {
            return ;
        }
        // Show the image
        m_EnmStateImaface.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the static
        m_Sta1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Sta2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Sta3.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Sta4.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Sta5.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Sta6.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Sta7.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaNobility.Show(m_Pnt.x, m_Pnt.y);
        // Show the button
        m_EnmStateBtnClose.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgEnmState::OnEnmstateBtnClose()
{
    CDlgEnmState::EnableWindow ( false ) ;
}

void CDlgEnmState::SetEnemyState(char* strEnemyName, int mode)
{
    m_iShowMode = mode;
    if (m_iShowMode == 0)
    {
        CFriend* pFriend = g_objHero.GetFriendByName ( strEnemyName ) ;
        if ( pFriend != NULL )
        {
            m_EnmStateImaface.RemoveImage () ;
            m_EnmStateImaface.InsertImage ( NULL, pFriend->GetFace(), pFriend->GetLook(), true) ;
            m_Sta1.SetWindowText ( pFriend->GetName () ) ;
            m_Sta3.SetWindowText ( pFriend->GetProfessionalName () ) ;
            m_Sta6.SetWindowText ( pFriend->GetRankName () ) ;
            m_Sta7.SetWindowText ( pFriend->GetMate () ) ;
            char strText[64] = "";
            itoa ( pFriend->GetLevel (), strText, 10 ) ;
            m_Sta2.SetWindowText ( strText ) ;
            itoa ( pFriend->GetPK (), strText, 10 ) ;
            m_Sta4.SetWindowText ( strText ) ;
            sprintf(strText, "%s", pFriend->GetSyndicateName ());
            m_Sta5.SetWindowText ( strText ) ;
            m_StaNobility.SetWindowText((char*)g_obj3DRoleData.GetNobilityTitle(pFriend->GetNobilityRank()));
        }
    }
    else
    {
        CEnemy* pEnemy = g_objHero.GetEnemyByName ( strEnemyName ) ;
        if ( pEnemy != NULL )
        {
            m_EnmStateImaface.RemoveImage () ;
            //		m_EnmStateImaface.InsertImage ( NULL, pEnemy->GetFace(), pEnemy->GetLook(), true) ;
            m_Sta1.SetWindowText ( pEnemy->GetName () ) ;
            m_Sta3.SetWindowText ( pEnemy->GetProfessionalName () ) ;
            m_Sta6.SetWindowText ( pEnemy->GetRankName () ) ;
            m_Sta7.SetWindowText ( pEnemy->GetMate () ) ;
            char strText[64] ;
            itoa ( pEnemy->GetLevel (), strText, 10 ) ;
            m_Sta2.SetWindowText ( strText ) ;
            itoa ( pEnemy->GetPK (), strText, 10 ) ;
            m_Sta4.SetWindowText ( strText ) ;
            sprintf(strText, "%s", pEnemy->GetSyndicateName ());
            m_Sta5.SetWindowText ( strText ) ;
            m_StaNobility.SetWindowText((char*)g_obj3DRoleData.GetNobilityTitle(pEnemy->GetNobilityRank()));
        }
    }
}


void CDlgEnmState::SetEnemyNull(char* strName)
{
    m_EnmStateImaface.RemoveImage () ;
    m_Sta1.SetWindowText ( strName ) ;
    m_Sta2.SetWindowText ( NULL ) ;
    m_Sta3.SetWindowText ( NULL ) ;
    m_Sta4.SetWindowText ( NULL ) ;
    m_Sta5.SetWindowText ( NULL ) ;
    m_Sta6.SetWindowText ( NULL ) ;
    m_Sta7.SetWindowText ( NULL ) ;
    m_StaNobility.SetWindowText (NULL);
}

void CDlgEnmState::SetStuInfo(char* szName)
{
    CSchoolMember* pSchoolMember = g_objHero.GetSchoolMemberByName ( szName ) ;
    if ( pSchoolMember != NULL )
    {
        m_EnmStateImaface.RemoveImage () ;
        m_Sta1.SetWindowText ( pSchoolMember->GetName () ) ;
        m_Sta3.SetWindowText ( pSchoolMember->GetProfession () ) ;
        m_Sta6.SetWindowText ( pSchoolMember->GetSynRank () ) ;
        m_Sta7.SetWindowText ( pSchoolMember->GetMate () ) ;
        m_StaNobility.SetWindowText ( pSchoolMember->GetNobilityRank());
        char strText[64] ;
        itoa ( pSchoolMember->GetLevel (), strText, 10 ) ;
        m_Sta2.SetWindowText ( strText ) ;
        itoa ( pSchoolMember->GetPk (), strText, 10 ) ;
        m_Sta4.SetWindowText ( strText ) ;
        sprintf(strText, "%s", pSchoolMember->GetSynName ());
        m_Sta5.SetWindowText ( strText ) ;
    }
}
