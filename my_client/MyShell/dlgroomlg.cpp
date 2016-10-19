
//**********************************************************\\
// ´úÂë±à¼­Æ÷\\
//**********************************************************\\
\\
// dlgroomlg.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "dlgroomlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRoomLg dialog
CDlgRoomLg::CDlgRoomLg(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgRoomLg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgRoomLg)
    // NOTE: the ClassWizard will add member initialization here
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    g_obj91Sdk.Init();
    //}}AFX_DATA_INIT
}


void CDlgRoomLg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgRoomLg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    DDX_Control(pDX, IDC_ROOMLG_BTN_RETURN, m_BtnReturn);
    DDX_Control(pDX, IDC_LIST_ROOMINFO, m_ListInfo);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRoomLg, CDialog)
    //{{AFX_MSG_MAP(CDlgRoomLg)
    ON_WM_MOVE()
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_ROOMLG_BTN_RETURN, OnRoomlgBtnReturn)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRoomLg message handlers

void CDlgRoomLg::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
    // TODO: Add your message handler code here
}
BOOL CDlgRoomLg::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgRoomLg::GetWindowRect ( rect ) ;
    m_BtnReturn.Init ( rect.left, rect .top, "Button540" );
    m_ListInfo.Init(rect.left, rect.top);
    m_ListInfo.SetFontColor(0xffffff);
    m_ListInfo.SetParentCob ( this ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
}

HBRUSH CDlgRoomLg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

void CDlgRoomLg::Show()
{
    if ( m_bShow )
    {
        // Show the Owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog86",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0,
                           m_Pnt.x,
                           m_Pnt.y );
            ShowAni->Show( 1,
                           m_Pnt.x + 256,
                           m_Pnt.y);
        }
        else
        {
            return ;
        }
        // Show the button
        m_BtnReturn.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the List box
        m_ListInfo.Show(m_Pnt.x , m_Pnt.y);
    }
}

LRESULT CDlgRoomLg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( message == WM_ENABLE )
    {
        if ( wParam == false )
        {
            m_bShow = false ;
            //DXPlaySound ( "Sound/Close.wav" ) ;
        }
        else
        {
            m_bShow = true ;
            //			DXPlaySound ( "Sound/Open.wav" ) ;
            //			ProcessCheck(0);
            //			m_SetupBtnGraph.SetCheckValue(1);
            //			SetScreenMode(m_nScreenMode);
        }
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 )
        {
            //		CDlgRoomLg::OnRoomlgBtnReturn();
            return true ;
        }
        //		else if ( wParam == 2 )
        //		{
        //			CDlgSetup::OnBtnExit () ;
        //			return true ;
        //		}
    }
    //	else if ( message == WM_HSCROLL )
    //	{
    //	DXSetMusicVolume ( m_SetupSliMusic.GetSliderValue ( ) ) ;
    //	DXSetSoundVolume ( m_SetupSliEffect.GetSliderValue ( ) ) ;
    //	}
    return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgRoomLg::OnRoomlgBtnReturn()
{
    // TODO: Add your control notification handler code here
    //	DXPlaySound("Sound/Dlg_Clear.wav");
    g_obj91Sdk.EnterRoom(1020);
    //	CDlgRoomLg::EnableWindow(false);
}
void CDlgRoomLg::SetParentWnd(CWnd* pParentWnd)
{
    m_ParentWnd = pParentWnd ;
}

void CDlgRoomLg::SetListInfo( )
{
    m_ListInfo.ResetContent();
    CString str;
    int nAmount = g_obj91Sdk.GetRoomInfoAmount();
    for( int i = 0; i < nAmount; ++i )
    {
        CGroupSummary* pInfo = g_obj91Sdk.GetRoomInfoList(i);
        if (pInfo)
        {
            str.Format("%s   %u    %u", pInfo->m_strGroupName.c_str(),
                       pInfo->m_dwMemberTotal, pInfo->m_dwManager);
            m_ListInfo.InsertString(i, str);
        }
    }
}


