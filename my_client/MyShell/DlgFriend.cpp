
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgFriend.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgFriend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _SOUND_DISABLE
#define DXPlaySound //
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFriend dialog


CDlgFriend::CDlgFriend(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgFriend::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgFriend)
    m_bShow = false ;
    m_fFrameStep = 0.0f ;
    m_nFrameMode = 0 ;
    m_nPntX = 0 ;
    m_nCurIndex = 0 ;
    m_btSortType = 0 ;
    m_nOnlineAmount = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgFriend::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgFriend)
    DDX_Control(pDX, IDC_IMG_HEAD1, m_ImgHead1);
    DDX_Control(pDX, IDC_STA_STIRPS1, m_StaStirps1);
    DDX_Control(pDX, IDC_STA_SEX1, m_StaSex1);
    DDX_Control(pDX, IDC_STA_PRO1, m_StaPro1);
    DDX_Control(pDX, IDC_STA_NAME1, m_StaName1);
    DDX_Control(pDX, IDC_STA_LEV1, m_StaLev1);
    DDX_Control(pDX, IDC_BTN_MSG1, m_BtnMsg1);
    DDX_Control(pDX, IDC_BTN_DEL1, m_BtnDel1);
    DDX_Control(pDX, IDC_IMG_HEAD2, m_ImgHead2);
    DDX_Control(pDX, IDC_STA_STIRPS2, m_StaStirps2);
    DDX_Control(pDX, IDC_STA_SEX2, m_StaSex2);
    DDX_Control(pDX, IDC_STA_PRO2, m_StaPro2);
    DDX_Control(pDX, IDC_STA_NAME2, m_StaName2);
    DDX_Control(pDX, IDC_STA_LEV2, m_StaLev2);
    DDX_Control(pDX, IDC_BTN_MSG2, m_BtnMsg2);
    DDX_Control(pDX, IDC_BTN_DEL2, m_BtnDel2);
    DDX_Control(pDX, IDC_IMG_HEAD3, m_ImgHead3);
    DDX_Control(pDX, IDC_STA_STIRPS3, m_StaStirps3);
    DDX_Control(pDX, IDC_STA_SEX3, m_StaSex3);
    DDX_Control(pDX, IDC_STA_PRO3, m_StaPro3);
    DDX_Control(pDX, IDC_STA_NAME3, m_StaName3);
    DDX_Control(pDX, IDC_STA_LEV3, m_StaLev3);
    DDX_Control(pDX, IDC_BTN_MSG3, m_BtnMsg3);
    DDX_Control(pDX, IDC_BTN_DEL3, m_BtnDel3);
    DDX_Control(pDX, IDC_BTN_SORTONLINE, m_BtnSortOnline);
    DDX_Control(pDX, IDC_BTN_SORTNAME, m_BtnSortName);
    DDX_Control(pDX, IDC_BTN_SORT, m_BtnSort);
    DDX_Control(pDX, IDC_BTN_RIGHT, m_BtnRight);
    DDX_Control(pDX, IDC_BTN_LEFT, m_BtnLeft);
    DDX_Control(pDX, IDC_FRIEND_BTN_RESUME, m_FriendBtnResume);
    DDX_Control(pDX, IDC_FRIEND_BTN_ADD, m_FriendBtnAdd ) ;
    DDX_Control(pDX, IDC_FRIEND_BTN_CLOSE, m_FriendBtnClose ) ;
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFriend, CDialog)
    //{{AFX_MSG_MAP(CDlgFriend)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_FRIEND_BTN_CLOSE, OnFriendBtnClose)
    ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
    ON_BN_CLICKED(IDC_FRIEND_BTN_ADD, OnFriendBtnAdd)
    ON_BN_CLICKED(IDC_FRIEND_BTN_RESUME, OnFriendBtnResume)
    ON_BN_CLICKED(IDC_BTN_MSG1, OnBtnMsg1)
    ON_BN_CLICKED(IDC_BTN_MSG2, OnBtnMsg2)
    ON_BN_CLICKED(IDC_BTN_MSG3, OnBtnMsg3)
    ON_BN_CLICKED(IDC_BTN_SORT, OnBtnSort)
    ON_BN_CLICKED(IDC_BTN_RIGHT, OnBtnRight)
    ON_BN_CLICKED(IDC_BTN_LEFT, OnBtnLeft)
    ON_BN_CLICKED(IDC_BTN_DEL1, OnBtnDel1)
    ON_BN_CLICKED(IDC_BTN_DEL2, OnBtnDel2)
    ON_BN_CLICKED(IDC_BTN_DEL3, OnBtnDel3)
    ON_BN_CLICKED(IDC_BTN_SORTONLINE, OnBtnSortonline)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFriend message handlers

LRESULT CDlgFriend::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if ( wParam == false )
        {
            DXPlaySound ( "Sound/DlgClose.wav" ) ;
        }
        else
        {
            m_dwLastShowTime = TimeGet() ;
            m_nFrameMode = 1 ;
            CRect rect ;
            CDlgFriend::GetWindowRect( rect ) ;
            m_nPntX = _SCR_WIDTH ;
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgFriend::OnFriendBtnClose () ;
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgFriend::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgFriend::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgFriend::GetWindowRect ( rect ) ;
    m_fFrameStep = ( float ) rect.Width () / DLG_FRAME_AMOUNT ;
    // Init the image
    m_ImgHead1.Init ( rect.left, rect.top, NULL, FACE_IMAGE ) ;
    m_ImgHead2.Init ( rect.left, rect.top, NULL, FACE_IMAGE ) ;
    m_ImgHead3.Init ( rect.left, rect.top, NULL, FACE_IMAGE ) ;
    // Init the static
    m_StaStirps1.Init ( rect.left, rect.top ) ;
    m_StaSex1.Init ( rect.left, rect.top ) ;
    m_StaPro1.Init ( rect.left, rect.top ) ;
    m_StaName1.Init ( rect.left, rect.top ) ;
    m_StaLev1.Init ( rect.left, rect.top ) ;
    m_StaStirps2.Init ( rect.left, rect.top ) ;
    m_StaSex2.Init ( rect.left, rect.top ) ;
    m_StaPro2.Init ( rect.left, rect.top ) ;
    m_StaName2.Init ( rect.left, rect.top ) ;
    m_StaLev2.Init ( rect.left, rect.top ) ;
    m_StaStirps3.Init ( rect.left, rect.top ) ;
    m_StaSex3.Init ( rect.left, rect.top ) ;
    m_StaPro3.Init ( rect.left, rect.top ) ;
    m_StaName3.Init ( rect.left, rect.top ) ;
    m_StaLev3.Init ( rect.left, rect.top ) ;
    // Init the button
    m_FriendBtnAdd.Init ( rect.left, rect.top, "Button170" );
    m_FriendBtnClose.Init ( rect.left, rect.top, "Button540" );
    m_FriendBtnResume.Init ( rect.left, rect.top, "Button175" ) ;
    m_BtnSortOnline.Init ( rect.left, rect.top, "Button176" ) ;
    m_BtnSortOnline.EnableWindow( false ) ;
    //	m_BtnSortName.Init ( rect.left, rect.top, "Button177" ) ;
    m_BtnSortName.EnableWindow( false ) ;
    m_BtnSort.Init ( rect.left, rect.top, "Button178" ) ;
    m_BtnRight.Init ( rect.left, rect.top, "Button61" ) ;
    m_BtnLeft.Init ( rect.left, rect.top, "Button60" ) ;
    m_BtnMsg1.Init ( rect.left, rect.top, "Button179" ) ;
    m_BtnDel1.Init ( rect.left, rect.top, "Button1710" ) ;
    m_BtnMsg2.Init ( rect.left, rect.top, "Button179" ) ;
    m_BtnDel2.Init ( rect.left, rect.top, "Button1710" ) ;
    m_BtnMsg3.Init ( rect.left, rect.top, "Button179" ) ;
    m_BtnDel3.Init ( rect.left, rect.top, "Button1710" ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFriend::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_nPntX = x ;
    m_Pnt.y = y ;
}

void CDlgFriend::OnFriendBtnClose()
{
    DXPlaySound ( "Sound/BtnClick.wav" ) ;
    CDlgFriend::EnableWindow( false ) ;
}

void CDlgFriend::Show()
{
    if ( m_bShow )
    {
        if ( m_nFrameMode != 0 )
        {
            if ( TimeGet() - m_dwLastShowTime <= 300 )
            {
                m_Pnt.x =(LONG)(m_nPntX - ( TimeGet() - m_dwLastShowTime ) / 15 * m_fFrameStep) ;
            }
            else
            {
                CRect rect ;
                CDlgFriend::GetWindowRect( rect ) ;
                m_Pnt.x = _SCR_WIDTH -  rect.Width();
                m_nFrameMode = 0 ;
            }
        }
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog17",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y ) ;
        else
        {
            return ;
        }
        // Show the image
        m_StaName1.Show ( m_Pnt.x, m_Pnt.y ) ;
        if ( m_ImgHead1.GetImageLoadState() )
        {
            m_ImgHead1.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_StaStirps1.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_StaSex1.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_StaPro1.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_StaLev1.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        m_StaName2.Show ( m_Pnt.x, m_Pnt.y ) ;
        if ( m_ImgHead2.GetImageLoadState() )
        {
            m_ImgHead2.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_StaStirps2.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_StaSex2.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_StaPro2.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_StaLev2.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        m_StaName3.Show ( m_Pnt.x, m_Pnt.y ) ;
        if ( m_ImgHead3.GetImageLoadState() )
        {
            m_ImgHead3.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_StaStirps3.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_StaSex3.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_StaPro3.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_StaLev3.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        // Show the button
        m_FriendBtnAdd.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_FriendBtnClose.Show ( m_Pnt.x, m_Pnt.y ) ;
        if ( m_BtnSortOnline.IsWindowEnabled() )
        {
            m_BtnSortOnline.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        //		if ( m_BtnSortName.IsWindowEnabled() )
        //			m_BtnSortName.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnSort.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnRight.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnLeft.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnMsg1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnDel1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnMsg2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnDel2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnMsg3.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnDel3.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgFriend::OnBtnHelp()
{
    CDlgFriend::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_HELPDLG_SHOW, DLG_FRIEND ) ;
}

void CDlgFriend::OnFriendBtnAdd()
{
    CDlgFriend::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_FRIEND_ADD ) ;
}

void CDlgFriend::SetFriendList()
{
    // Test the online friend num
    CDlgFriend::ClearFriendList() ;
    int i = 0 ;
    int nAmount = ( g_objHero.GetFriendAmount() > m_nCurIndex + 3 ) ? m_nCurIndex + 3 : g_objHero.GetFriendAmount() ;
    switch ( m_btSortType )
    {
    case 0:// not sort
        for ( i = m_nCurIndex; i < nAmount; i++ )
        {
            CFriend* pFriend = g_objHero.GetFriendByIndex( i ) ;
            if ( pFriend != NULL )
            {
                CDlgFriend::InsertFriendInfo( i - m_nCurIndex, pFriend ) ;
            }
        }
        break ;
    case 1:// sort by online
        {
            m_nOnlineAmount = 0 ;
            int nInsertNum = 0 ;
            for ( i = 0; i < g_objHero.GetFriendAmount(); i++ )// Search online
            {
                CFriend* pFriend = g_objHero.GetFriendByIndex( i ) ;
                if ( pFriend != NULL && pFriend->IsOnline() )
                {
                    if ( m_nOnlineAmount >= m_nCurIndex )
                    {
                        CDlgFriend::InsertFriendInfo( m_nOnlineAmount - m_nCurIndex, pFriend ) ;
                        nInsertNum++ ;
                        if ( nInsertNum == 3 )// Test if insert over
                        {
                            return ;
                        }
                    }
                    m_nOnlineAmount++ ;
                }
            }
            int nSortIndex = m_nOnlineAmount ;
            for ( i = 0; i < g_objHero.GetFriendAmount(); i++ )// Search offline
            {
                CFriend* pFriend = g_objHero.GetFriendByIndex( i ) ;
                if ( pFriend != NULL && !pFriend->IsOnline() )
                {
                    if ( nSortIndex >= m_nCurIndex )
                    {
                        CDlgFriend::InsertFriendInfo( nSortIndex - m_nCurIndex, pFriend ) ;
                        nInsertNum++ ;
                        if ( nInsertNum == 3 )// Test if insert over
                        {
                            return ;
                        }
                    }
                    nSortIndex++ ;
                }
            }
        }
        break ;
    default:
        break ;
    }
}


void CDlgFriend::SetFriendState()
{
}

void CDlgFriend::OnFriendBtnResume()
{
}

void CDlgFriend::OnBtnMsg1()
{
    if ( m_StaName1.GetWindowTextLength() > 0 )
    {
        char* strName = new char[32] ;
        m_StaName1.GetWindowText( strName, 32 ) ;
        CDlgFriend::GetParent()->PostMessage( WM_MY_MESSAGE, ON_FRIEND_LEAVE, (LPARAM)strName ) ;
    }
}

void CDlgFriend::OnBtnMsg2()
{
    if ( m_StaName2.GetWindowTextLength() > 0 )
    {
        char* strName = new char[32] ;
        m_StaName2.GetWindowText( strName, 32 ) ;
        CDlgFriend::GetParent()->PostMessage( WM_MY_MESSAGE, ON_FRIEND_LEAVE, (LPARAM)strName ) ;
    }
}

void CDlgFriend::OnBtnMsg3()
{
    if ( m_StaName3.GetWindowTextLength() > 0 )
    {
        char* strName = new char[32] ;
        m_StaName3.GetWindowText( strName, 32 ) ;
        CDlgFriend::GetParent()->PostMessage( WM_MY_MESSAGE, ON_FRIEND_LEAVE, (LPARAM)strName ) ;
    }
}

void CDlgFriend::OnBtnSort()
{
    if ( m_BtnSortOnline.IsWindowEnabled() )
    {
        m_BtnSortOnline.EnableWindow( false ) ;
    }
    else
    {
        m_BtnSortOnline.EnableWindow() ;
    }
    /*	if ( m_BtnSortName.IsWindowEnabled() )
    	{
    		m_BtnSortName.EnableWindow( false ) ;
    	}
    	else
    	{
    		m_BtnSortName.EnableWindow() ;
    	}*/
}

void CDlgFriend::OnBtnRight()
{
    if ( g_objHero.GetFriendAmount() > m_nCurIndex + 3 )
    {
        m_nCurIndex += 3 ;
        CDlgFriend::SetFriendList() ;
    }
}

void CDlgFriend::OnBtnLeft()
{
    if ( m_nCurIndex > 0 )
    {
        m_nCurIndex -= 3 ;
        CDlgFriend::SetFriendList() ;
    }
}

void CDlgFriend::OnBtnDel1()
{
    if ( m_StaName1.GetWindowTextLength() > 0 )
    {
        char strName[32] ;
        m_StaName1.GetWindowText( strName, 32 ) ;
        CFriend* pFriend = g_objHero.GetFriendByName( strName ) ;
        if ( pFriend != NULL )
        {
            g_objHero.BreakOffFriend( pFriend->GetID() ) ;
        }
    }
}

void CDlgFriend::OnBtnDel2()
{
    if ( m_StaName2.GetWindowTextLength() > 0 )
    {
        char strName[32] ;
        m_StaName2.GetWindowText( strName, 32 ) ;
        CFriend* pFriend = g_objHero.GetFriendByName( strName ) ;
        if ( pFriend != NULL )
        {
            g_objHero.BreakOffFriend( pFriend->GetID() ) ;
        }
    }
}

void CDlgFriend::OnBtnDel3()
{
    if ( m_StaName3.GetWindowTextLength() > 0 )
    {
        char strName[32] ;
        m_StaName3.GetWindowText( strName, 32 ) ;
        CFriend* pFriend = g_objHero.GetFriendByName( strName ) ;
        if ( pFriend != NULL )
        {
            g_objHero.BreakOffFriend( pFriend->GetID() ) ;
        }
    }
}

void CDlgFriend::ClearFriendList()
{
    m_ImgHead1.RemoveImage() ;
    m_StaStirps1.SetWindowText( NULL ) ;
    m_StaSex1.SetWindowText( NULL ) ;
    m_StaPro1.SetWindowText( NULL ) ;
    m_StaName1.SetWindowText( NULL ) ;
    m_StaLev1.SetWindowText( NULL ) ;
    m_ImgHead2.RemoveImage() ;
    m_StaStirps2.SetWindowText( NULL ) ;
    m_StaSex2.SetWindowText( NULL ) ;
    m_StaPro2.SetWindowText( NULL ) ;
    m_StaName2.SetWindowText( NULL ) ;
    m_StaLev2.SetWindowText( NULL ) ;
    m_ImgHead3.RemoveImage() ;
    m_StaStirps3.SetWindowText( NULL ) ;
    m_StaSex3.SetWindowText( NULL ) ;
    m_StaPro3.SetWindowText( NULL ) ;
    m_StaName3.SetWindowText( NULL ) ;
    m_StaLev3.SetWindowText( NULL ) ;
}

void CDlgFriend::OnBtnSortonline()
{
    m_btSortType = 1 ;
    CDlgFriend::SetFriendList() ;
}

void CDlgFriend::InsertFriendInfo(int nIndex, CFriend* pFriend)
{
    char strText[64] ;
    switch ( nIndex )
    {
    case 0:
        m_StaName1.SetWindowText( pFriend->GetName() ) ;
        if ( pFriend->IsOnline() )
        {
            if ( m_BtnMsg1.IsWindowEnabled() )
            {
                m_BtnMsg1.EnableWindow( false ) ;
            }
            m_ImgHead1.InsertImage( NULL, pFriend->GetFace(), pFriend->GetLook(), false ) ;
            sprintf ( strText, g_objGameDataSet.GetStr( 10520 + pFriend->GetRace() - 1 ) ) ;
            m_StaStirps1.SetWindowText( strText ) ;
            sprintf ( strText, g_objGameDataSet.GetStr( 10030 + pFriend->GetLook() % 2 ) ) ;
            m_StaSex1.SetWindowText( strText ) ;
            m_StaPro1.SetWindowText( pFriend->GetProfessionalName() ) ;
            itoa( pFriend->GetLevel(), strText, 10 ) ;
            m_StaLev1.SetWindowText( strText ) ;
        }
        else
        {
            if ( !m_BtnMsg1.IsWindowEnabled() )
            {
                m_BtnMsg1.EnableWindow() ;
            }
        }
        break ;
    case 1:
        m_StaName2.SetWindowText( pFriend->GetName() ) ;
        if ( pFriend->IsOnline() )
        {
            if ( m_BtnMsg2.IsWindowEnabled() )
            {
                m_BtnMsg2.EnableWindow( false ) ;
            }
            m_ImgHead2.InsertImage( NULL, pFriend->GetFace(), pFriend->GetLook(), false ) ;
            sprintf ( strText, g_objGameDataSet.GetStr( 10520 + pFriend->GetRace() - 1 ) ) ;
            m_StaStirps2.SetWindowText( strText ) ;
            sprintf ( strText, g_objGameDataSet.GetStr( 10030 + pFriend->GetLook() % 2 ) ) ;
            m_StaSex2.SetWindowText( strText ) ;
            m_StaPro2.SetWindowText( pFriend->GetProfessionalName() ) ;
            itoa( pFriend->GetLevel(), strText, 10 ) ;
            m_StaLev2.SetWindowText( strText ) ;
        }
        else
        {
            if ( !m_BtnMsg2.IsWindowEnabled() )
            {
                m_BtnMsg2.EnableWindow() ;
            }
        }
        break ;
    case 2:
        m_StaName3.SetWindowText( pFriend->GetName() ) ;
        if ( pFriend->IsOnline() )
        {
            if ( m_BtnMsg3.IsWindowEnabled() )
            {
                m_BtnMsg3.EnableWindow( false ) ;
            }
            m_ImgHead3.InsertImage( NULL, pFriend->GetFace(), pFriend->GetLook(), false ) ;
            sprintf ( strText, g_objGameDataSet.GetStr( 10520 + pFriend->GetRace() - 1 ) ) ;
            m_StaStirps3.SetWindowText( strText ) ;
            sprintf ( strText, g_objGameDataSet.GetStr( 10030 + pFriend->GetLook() % 2 ) ) ;
            m_StaSex3.SetWindowText( strText ) ;
            m_StaPro3.SetWindowText( pFriend->GetProfessionalName() ) ;
            itoa( pFriend->GetLevel(), strText, 10 ) ;
            m_StaLev3.SetWindowText( strText ) ;
        }
        else
        {
            if ( !m_BtnMsg3.IsWindowEnabled() )
            {
                m_BtnMsg3.EnableWindow() ;
            }
        }
        break ;
    default:
        break ;
    }
}
