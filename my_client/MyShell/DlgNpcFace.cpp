
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgNpcFace.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgNpcFace.h"

#include "3DRoleData.h"
#include "Role.h"
#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNpcFace dialog


CDlgNpcFace::CDlgNpcFace(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgNpcFace::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgNpcFace)
    m_bShow = false ;
    m_fFrameStep = 0.0f ;
    m_nFrameMode = 0 ;
    m_nPntX = 0 ;
    m_nCurrentAmount = 0 ;
    m_uFocusImage = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgNpcFace::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgNpcFace)
    DDX_Control(pDX, IDC_NPCFACE_BTN_UP, m_NpcFaceBtnUp);
    DDX_Control(pDX, IDC_NPCFACE_BTN_DOWN, m_NpcFaceBtnDown);
    DDX_Control(pDX, IDC_NPCFACE_IMA6, m_NpcFaceIma6);
    DDX_Control(pDX, IDC_NPCFACE_IMA5, m_NpcFaceIma5);
    DDX_Control(pDX, IDC_NPCFACE_IMA4, m_NpcFaceIma4);
    DDX_Control(pDX, IDC_NPCFACE_IMA3, m_NpcFaceIma3);
    DDX_Control(pDX, IDC_NPCFACE_IMA2, m_NpcFaceIma2);
    DDX_Control(pDX, IDC_NPCFACE_IMA1, m_NpcFaceIma1);
    DDX_Control(pDX, IDC_NPCFACE_BTN_CLOSE, m_NpcFaceBtnClose);
    DDX_Control(pDX, IDC_NPCFACE_BTN_AGREE, m_NpcFaceBtnAgree);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNpcFace, CDialog)
    //{{AFX_MSG_MAP(CDlgNpcFace)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_NPCFACE_IMA1, OnNpcfaceIma1)
    ON_BN_CLICKED(IDC_NPCFACE_IMA2, OnNpcfaceIma2)
    ON_BN_CLICKED(IDC_NPCFACE_IMA3, OnNpcfaceIma3)
    ON_BN_CLICKED(IDC_NPCFACE_IMA4, OnNpcfaceIma4)
    ON_BN_CLICKED(IDC_NPCFACE_IMA5, OnNpcfaceIma5)
    ON_BN_CLICKED(IDC_NPCFACE_IMA6, OnNpcfaceIma6)
    ON_BN_CLICKED(IDC_NPCFACE_BTN_UP, OnNpcfaceBtnUp)
    ON_BN_CLICKED(IDC_NPCFACE_BTN_DOWN, OnNpcfaceBtnDown)
    ON_BN_CLICKED(IDC_NPCFACE_BTN_AGREE, OnNpcfaceBtnAgree)
    ON_BN_CLICKED(IDC_NPCFACE_BTN_CLOSE, OnNpcfaceBtnClose)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNpcFace message handlers

LRESULT CDlgNpcFace::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_SHOWWINDOW:
        m_bShow = wParam ;
        if ( wParam )
        {
            CRect rect ;
            CDlgNpcFace::GetWindowRect ( rect ) ;
            if ( m_nFrameMode == 0 )
            {
                DXPlaySound ( "Sound/DlgOpen.wav" ) ;
                m_dwLastShowTime = 0 ;
                m_nFrameMode = 1 ;
                CDlgNpcFace::MoveWindow ( -rect.Width(), m_Pnt.y, rect.Width(), rect.Height(), true ) ;
            }
            else if ( m_nFrameMode == 2 )
            {
                CDlgNpcFace::MoveWindow ( 0, m_Pnt.y, rect.Width(), rect.Height(), true ) ;
                m_nFrameMode = 0 ;
            }
        }
        else
        {
            DXPlaySound ( "Sound/DlgClose.wav" ) ;
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgNpcFace::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgNpcFace::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgNpcFace::GetWindowRect ( rect ) ;
    // Init the button
    if ( !m_NpcFaceBtnClose.Init ( rect.left, rect.top, "Button5" ) )
    {
        return false ;
    }
    if ( !m_NpcFaceBtnAgree.Init ( rect.left, rect.top, "Button270" ) )
    {
        return false ;
    }
    if ( !m_NpcFaceBtnUp.Init ( rect.left, rect.top, "Button271" ) )
    {
        return false ;
    }
    if ( !m_NpcFaceBtnDown.Init ( rect.left, rect.top, "Button272" ) )
    {
        return false ;
    }
    // Init the image
    if ( ! m_NpcFaceIma1.Init ( rect.left, rect.top, NULL, 2, 1 ) )
    {
        return false ;
    }
    if ( ! m_NpcFaceIma2.Init ( rect.left, rect.top, NULL, 2, 1 ) )
    {
        return false ;
    }
    if ( ! m_NpcFaceIma3.Init ( rect.left, rect.top, NULL, 2, 1 ) )
    {
        return false ;
    }
    if ( ! m_NpcFaceIma4.Init ( rect.left, rect.top, NULL, 2, 1 ) )
    {
        return false ;
    }
    if ( ! m_NpcFaceIma5.Init ( rect.left, rect.top, NULL, 2, 1 ) )
    {
        return false ;
    }
    if ( ! m_NpcFaceIma6.Init ( rect.left, rect.top, NULL, 2, 1 ) )
    {
        return false ;
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNpcFace::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_nPntX = x ;
    m_Pnt.y = y ;
}

void CDlgNpcFace::Show()
{
    if ( m_bShow )
    {
        switch ( m_nFrameMode )
        {
        case 0:
            break ;
        case 1:
            if ( m_dwLastShowTime == 0 )
            {
                m_dwLastShowTime = TimeGet() ;
                m_Pnt.x = m_nPntX ;
            }
            else
            {
                if ( TimeGet() - m_dwLastShowTime <= 300 )
                {
                    m_Pnt.x = m_nPntX ;
                    m_Pnt.x += (LONG)(( TimeGet() - m_dwLastShowTime ) / 15 * m_fFrameStep) ;
                }
                else
                {
                    m_Pnt.x = 0;
                    m_nFrameMode = 2 ;
                    CDlgNpcFace::WindowProc ( WM_SHOWWINDOW, true, NULL ) ;
                }
            }
            break ;
        case 2:
            break ;
        }
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog27",
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
        m_NpcFaceBtnClose.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcFaceBtnAgree.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcFaceBtnUp.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcFaceBtnDown.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the image
        m_NpcFaceIma1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcFaceIma2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcFaceIma3.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcFaceIma4.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcFaceIma5.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcFaceIma6.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the focus image
        switch ( m_uFocusImage )
        {
        case 1:
            m_NpcFaceIma1.ShowFocus () ;
            break ;
        case 2:
            m_NpcFaceIma2.ShowFocus () ;
            break ;
        case 3:
            m_NpcFaceIma3.ShowFocus () ;
            break ;
        case 4:
            m_NpcFaceIma4.ShowFocus () ;
            break ;
        case 5:
            m_NpcFaceIma5.ShowFocus () ;
            break ;
        case 6:
            m_NpcFaceIma6.ShowFocus () ;
            break ;
        }
    }
}

void CDlgNpcFace::InitNpcFace(BYTE btType)
{
    m_nMaleFaceAmount = 0 ;
    m_nFemaleFaceAmount = 0 ;
    m_nCurrentAmount = 0 ;
    m_uFocusImage = 0 ;
    switch ( btType )
    {
    case SEX_MALE:
        {
            m_nMaleFaceAmount = g_obj3DRoleData.GetMaleFaceAmount () ;
            int nAmount = ( m_nMaleFaceAmount > m_nCurrentAmount + 6 ) ? m_nCurrentAmount + 6 : m_nMaleFaceAmount ;
            for ( int i = 1; i <= nAmount; i++ )
            {
                CDlgNpcFace::InsertNpcFace ( i, i ) ;
            }
        }
        break ;
    case SEX_FEMALE:
        {
            m_nFemaleFaceAmount = g_obj3DRoleData.GetFemaleFaceAmount () ;
            int nAmount = ( m_nMaleFaceAmount > m_nCurrentAmount + 6 ) ? m_nCurrentAmount + 6 : m_nFemaleFaceAmount ;
            for ( int i = 1; i <= nAmount; i++ )
            {
                CDlgNpcFace::InsertNpcFace ( i, i + 200 ) ;
            }
        }
        break ;
    }
}

void CDlgNpcFace::OnNpcfaceIma1()
{
    if ( m_NpcFaceIma1.GetImageLoadState () )
    {
        m_uFocusImage = 1 ;
    }
}

void CDlgNpcFace::OnNpcfaceIma2()
{
    if ( m_NpcFaceIma2.GetImageLoadState () )
    {
        m_uFocusImage = 2 ;
    }
}

void CDlgNpcFace::OnNpcfaceIma3()
{
    if ( m_NpcFaceIma3.GetImageLoadState () )
    {
        m_uFocusImage = 3 ;
    }
}

void CDlgNpcFace::OnNpcfaceIma4()
{
    if ( m_NpcFaceIma4.GetImageLoadState () )
    {
        m_uFocusImage = 4 ;
    }
}

void CDlgNpcFace::OnNpcfaceIma5()
{
    if ( m_NpcFaceIma5.GetImageLoadState () )
    {
        m_uFocusImage = 5 ;
    }
}

void CDlgNpcFace::OnNpcfaceIma6()
{
    if ( m_NpcFaceIma6.GetImageLoadState () )
    {
        m_uFocusImage = 6 ;
    }
}

void CDlgNpcFace::OnNpcfaceBtnUp()
{
    if ( m_nCurrentAmount > 0 )
    {
        m_nCurrentAmount -= 6 ;
        if ( m_nMaleFaceAmount != 0 )
        {
            for ( int i = 1; i <= 6; i++ )
            {
                CDlgNpcFace::InsertNpcFace ( i, i + m_nCurrentAmount ) ;
                m_uFocusImage = 0 ;
            }
        }
        else if ( m_nFemaleFaceAmount != 0 )
        {
            for ( int i = 1; i <= 6; i++ )
            {
                CDlgNpcFace::InsertNpcFace ( i, i + 200 + m_nCurrentAmount ) ;
                m_uFocusImage = 0 ;
            }
        }
    }
}

void CDlgNpcFace::OnNpcfaceBtnDown()
{
    if ( m_nMaleFaceAmount != 0 && m_nMaleFaceAmount > m_nCurrentAmount + 6 )
    {
        m_NpcFaceIma1.RemoveImage ( ) ;
        m_NpcFaceIma2.RemoveImage ( ) ;
        m_NpcFaceIma3.RemoveImage ( ) ;
        m_NpcFaceIma4.RemoveImage ( ) ;
        m_NpcFaceIma5.RemoveImage ( ) ;
        m_NpcFaceIma6.RemoveImage ( ) ;
        m_nCurrentAmount += 6 ;
        int nAmount = 6;
        if ( m_nMaleFaceAmount < m_nCurrentAmount + 6 )
        {
            nAmount = m_nMaleFaceAmount - m_nCurrentAmount ;
        }
        for ( int i = 1; i <= nAmount; i++ )
        {
            CDlgNpcFace::InsertNpcFace ( i, i + m_nCurrentAmount ) ;
            m_uFocusImage = 0 ;
        }
    }
    else if ( m_nFemaleFaceAmount != 0 && m_nFemaleFaceAmount > m_nCurrentAmount + 6 )
    {
        m_NpcFaceIma1.RemoveImage ( ) ;
        m_NpcFaceIma2.RemoveImage ( ) ;
        m_NpcFaceIma3.RemoveImage ( ) ;
        m_NpcFaceIma4.RemoveImage ( ) ;
        m_NpcFaceIma5.RemoveImage ( ) ;
        m_NpcFaceIma6.RemoveImage ( ) ;
        m_nCurrentAmount += 6 ;
        int nAmount = 6;
        if ( m_nFemaleFaceAmount < m_nCurrentAmount + 6 )
        {
            nAmount = m_nFemaleFaceAmount - m_nCurrentAmount ;
        }
        for ( int i = 1; i <= nAmount; i++ )
        {
            CDlgNpcFace::InsertNpcFace ( i, i + 200 + m_nCurrentAmount ) ;
            m_uFocusImage = 0 ;
        }
    }
}

void CDlgNpcFace::OnNpcfaceBtnAgree()
{
    if ( m_uFocusImage != 0 )
    {
        int nFace = m_nCurrentAmount + m_uFocusImage;
        if (g_objHero.GetSex() == SEX_FEMALE)
        {
            nFace += 200;
        }
        g_objHero.ChangeFace ( nFace ) ;
        CDlgNpcFace::OnNpcfaceBtnClose () ;
    }
}

void CDlgNpcFace::OnNpcfaceBtnClose()
{
    CDlgNpcFace::ShowWindow ( SW_HIDE ) ;
    CDlgNpcFace::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_NPCFACE_CLOSE ) ;
}

void CDlgNpcFace::InsertNpcFace(int nImgIndex, int nFaceIndex)
{
    char strID[16] ;
    itoa ( nFaceIndex, strID, 10 ) ;
    switch ( nImgIndex )
    {
    case 1:
        m_NpcFaceIma1.InsertImage ( strID ) ;
        break ;
    case 2:
        m_NpcFaceIma2.InsertImage ( strID ) ;
        break ;
    case 3:
        m_NpcFaceIma3.InsertImage ( strID ) ;
        break ;
    case 4:
        m_NpcFaceIma4.InsertImage ( strID ) ;
        break ;
    case 5:
        m_NpcFaceIma5.InsertImage ( strID ) ;
        break ;
    case 6:
        m_NpcFaceIma6.InsertImage ( strID ) ;
        break ;
    }
}
