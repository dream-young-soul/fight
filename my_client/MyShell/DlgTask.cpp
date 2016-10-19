
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgTask.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgTask.h"
#include "GameMsg.h"
#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTask dialog


CDlgTask::CDlgTask(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTask::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTask)
    m_bShow = false ;
    m_bMouseMove = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_nCloseIndex = 0 ;
    m_nTextAmount = 0 ;
    m_nLinkAmount = 0 ;
    m_nEditAmount = 0 ;
    m_nDlgHeight = 150 ;
    m_nAnswerIndex = 0 ;
    m_nEditIndex = 0 ;
    m_DlgRect = CRect ( 0, 0, 512, 64 ) ;
    //}}AFX_DATA_INIT
}


void CDlgTask::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTask)
    DDX_Control(pDX, IDC_TASK_EDT_ANSWER, m_TaskEdtAnswer);
    DDX_Control(pDX, IDC_TASK_BTN_ANSWER, m_TaskBtnAnswer);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_CloseBtn);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTask, CDialog)
    //{{AFX_MSG_MAP(CDlgTask)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_BN_CLICKED(IDC_TASK_BTN_ANSWER, OnTaskBtnAnswer)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTask message handlers

LRESULT CDlgTask::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        if ( wParam )
        {
            InitTaskDialog () ;
            DXPlaySound("Sound/Open.wav");
        }
        else
        {
            DXPlaySound("Sound/window_close.wav");
            ResetTaskDialog() ;
        }
        m_bShow = wParam ;
        break ;
    case WM_COMMAND:
        if ( wParam == 2 )
        {
            return true ;
        }
        else if ( wParam == 1 )
        {
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgTask::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgTask::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgTask::GetWindowRect ( rect ) ;
    // Init the edit
    m_TaskEdtAnswer.Init ( rect.left, rect.top ) ;
    // Init the button
    m_TaskBtnAnswer.Init ( rect.left, rect.top, "Button0") ;
    m_CloseBtn.Init ( rect.left, rect.top, "Button540" ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTask::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgTask::InitTaskDialog()
{
    m_nPicIndex = -1 ;
    m_TaskEdtAnswer.EnableWindow ( false ) ;
    m_TaskBtnAnswer.EnableWindow ( false ) ;
    int nAmount = g_objHero.GetTaskDialogLineAmount () ;
    if ( nAmount > 0 )
    {
        // Get the format
        m_nAnswerAmount = 0 ;
        for ( int j = 0; j < nAmount; j++ )
        {
            TaskDialogLine* pLine = g_objHero.GetTaskDialogLineByIndex ( j ) ;
            if ( pLine->nType == _TASKDIALOG_LINK || pLine->nType == _TASKDIALOG_EDIT )
            {
                m_nAnswerAmount++ ;
            }
        }
        for ( int i = 0; i < nAmount; i++ )
        {
            TaskDialogLine* pLine = g_objHero.GetTaskDialogLineByIndex ( i ) ;
            switch ( pLine->nType )
            {
            case _TASKDIALOG_TEXT:
                if ( pLine->szStr )
                {
                    strcpy ( m_TaskText[m_nTextAmount].strText, pLine->szStr ) ;
                    this->SetLineIndex ( 0 ) ;
                    m_nTextAmount++ ;
                }
                break ;
            case _TASKDIALOG_LINK:
                if ( pLine->szStr )
                {
                    m_nAnswerIndex++ ;
                    strcpy ( m_TaskLink[m_nLinkAmount].strText, pLine->szStr ) ;
                    this->SetLineIndex ( 1 ) ;
                    m_TaskLink[m_nLinkAmount].nIndex = pLine->nIndex ;
                    m_nLinkAmount++ ;
                }
                break ;
            case _TASKDIALOG_EDIT:
                {
                    m_nAnswerIndex++ ;
                    m_nEditIndex = m_nAnswerIndex ;
                    m_TaskEdtAnswer.EnableWindow ( true ) ;
                    m_TaskBtnAnswer.EnableWindow ( true ) ;
                    strcpy ( m_TaskEdit.strTitle, pLine->szStr ) ;
                    m_TaskEdit.nIndex = pLine->nIndex ;
                    m_TaskEdit.dwData = pLine->dwData ;
                    this->SetLineIndex ( 2 ) ;
                    m_nEditAmount++ ;
                }
                break ;
            case _TASKDIALOG_PIC:
                m_nPicIndex = pLine->dwData ;
                m_PicPos.x = pLine->posLine.x;
                m_PicPos.y = pLine->posLine.y;
                break ;
            case _TASKDIALOG_DEFAULT:
                m_nCloseIndex = pLine->nIndex ;
                break ;
            }
        }
    }
}

void CDlgTask::ResetTaskDialog()
{
    m_nCloseIndex = 0 ;
    m_nTextAmount = 0 ;
    m_nLinkAmount = 0 ;
    m_nEditAmount = 0 ;
    m_nDlgHeight = 150 ;
    m_nAnswerIndex = 0 ;
    m_nEditIndex = 0 ;
    for(int i = 0 ; i < 8; i++)
    {
        strcpy(m_TaskText[i].strText , "");
        strcpy(m_TaskLink[i].strText , "");
    }
    strcpy(m_TaskEdit.strTitle , "");
    m_TaskEdtAnswer.SetWindowText ( NULL ) ;
}

void CDlgTask::SetLineIndex(BYTE btType)
{
    switch ( btType )
    {
    case 0:
        {
            int	nSingle = 0;
            int nLineNum = 0 ;
            int nLineIndex = 1 ;
            m_TaskText[m_nTextAmount].nLineIndex [0] = 0 ;
            int i = 0;
            for ( ; i <  (int)strlen ( m_TaskText[m_nTextAmount].strText ); i++ )
            {
                if ( CMyBitmap::IsDBCSLeadByte( m_TaskText[m_nTextAmount].strText, i ) )
                {
                    nLineNum += 2;
                    i++;
                }
                else
                {
                    nLineNum++;
                    nSingle++;
                }
                if ( nLineNum >= TEXT_LIMIT )
                {
                    //----------Begin the english version's process(jump line)----------//
                    if ( strcmp( g_objGameDataSet.GetVersionInfo(), "English" ) == 0 )
                    {
                        // Search the last word
                        for ( int j = i; j >= 0; j-- )
                        {
                            if ( m_TaskText[m_nTextAmount].strText[j] == ' ' )
                            {
                                i -= i - j;
                                break;
                            }
                        }
                    }
                    //----------End the english version's process(jump line)----------//
                    if ( nSingle % 2 == 0 )
                    {
                        nSingle = 0 ;
                        nLineNum = 0 ;
                        m_TaskText[m_nTextAmount].nLineIndex [nLineIndex] = i + 1 ;
                        nLineIndex++ ;
                    }
                    else
                    {
                        i -= nLineNum - TEXT_LIMIT;
                        m_TaskText[m_nTextAmount].nLineIndex [nLineIndex] = i + 1 - ( nLineNum - TEXT_LIMIT );
                        nSingle = 0 ;
                        nLineNum = 0 ;
                        nLineIndex++ ;
                    }
                }
            }
            m_TaskText[m_nTextAmount].nLineNum = nLineIndex ;
            if ( nSingle % 2 == 0 )
            {
                m_TaskText[m_nTextAmount].nLineIndex [nLineIndex] = i + 1 ;
            }
            else
            {
                m_TaskText[m_nTextAmount].nLineIndex [nLineIndex] = i ;
            }
        }
        break ;
    case 1:
        {
            int	nSingle = 0;
            int nLineNum = 0 ;
            int nLineIndex = 1 ;
            m_TaskLink[m_nLinkAmount].nLineIndex [0] = 0 ;
            int nLastHeight = m_nDlgHeight ;
            int i = 0;
            for ( ; i <  (int)strlen ( m_TaskLink[m_nLinkAmount].strText ); i++ )
            {
                if ( CMyBitmap::IsDBCSLeadByte( m_TaskLink[m_nLinkAmount].strText, i ) )
                {
                    nLineNum += 2;
                    i++;
                }
                else
                {
                    nLineNum++;
                    nSingle++;
                }
                if ( nLineNum >= LINK_LIMIT )
                {
                    //----------Begin the english version's process(jump line)----------//
                    if ( strcmp( g_objGameDataSet.GetVersionInfo(), "English" ) == 0 )
                    {
                        // Search the last word
                        for ( int j = i; j >= 0; j-- )
                        {
                            if ( m_TaskLink[m_nLinkAmount].strText[j] == ' ' )
                            {
                                i -= i - j;
                                break;
                            }
                        }
                    }
                    //----------End the english version's process(jump line)----------//
                    if ( nLineNum != strlen ( m_TaskLink[m_nLinkAmount].strText ) )
                    {
                        m_nDlgHeight += 18 ;
                    }
                    if ( nSingle % 2 == 0 )
                    {
                        nSingle = 0 ;
                        nLineNum = 0 ;
                        m_TaskLink[m_nLinkAmount].nLineIndex [nLineIndex] = i + 1 ;
                        nLineIndex++ ;
                    }
                    else
                    {
                        i -= nLineNum - TEXT_LIMIT;
                        m_TaskLink[m_nLinkAmount].nLineIndex [nLineIndex] = i + 1 - ( nLineNum - TEXT_LIMIT );
                        nSingle = 0 ;
                        nLineNum = 0 ;
                        nLineIndex++ ;
                    }
                }
            }
            m_TaskLink[m_nLinkAmount].nLineNum = nLineIndex ;
            if ( nSingle % 2 == 0 )
            {
                m_TaskLink[m_nLinkAmount].nLineIndex [nLineIndex] = i + 1 ;
            }
            else
            {
                m_TaskLink[m_nLinkAmount].nLineIndex [nLineIndex] = i ;
            }
            m_TaskLink[m_nLinkAmount].nLineNum = nLineIndex ;
            m_TaskLink[m_nLinkAmount].Rect = CRect( 164, nLastHeight, 164 + LINK_LIMIT * 6, m_nDlgHeight + 18 ) ;
            m_nDlgHeight += 18 ;
        }
        break ;
    case 2:
        {
            CRect rect ;
            m_TaskEdtAnswer.GetWindowRect ( rect ) ;
            m_TaskEdtAnswer.MoveWindow ( 164,
                                         m_nDlgHeight + 18,
                                         rect.Width (),
                                         rect.Height (),
                                         false ) ;
            m_TaskEdtAnswer.m_EditRect.MoveWindow(164 ,
                                                  m_nDlgHeight + 18,
                                                  180,
                                                  18,
                                                  false);
            m_TaskBtnAnswer.MoveWindow ( 164,
                                         m_nDlgHeight + 42,
                                         50,
                                         20,
                                         false ) ;
            CDlgTask::GetWindowRect ( rect ) ;
            m_TaskEdtAnswer.ReInit(rect.left, rect.top);
            m_TaskEdtAnswer.SetEditBkID("Edit230");
            m_TaskEdtAnswer.SetLimitText ( m_TaskEdit.dwData ) ;
            m_TaskBtnAnswer.Init ( rect.left, rect.top, "Button0" ) ;
            m_nDlgHeight += 60 ;
        }
    }
    if (m_nDlgHeight + 65 > 216 )
    {
        CRect pRect, bRect;
        CDlgTask::GetWindowRect(pRect);
        CDlgTask::MoveWindow(m_Pnt.x, m_Pnt.y, pRect.Width(), m_nDlgHeight + 65);
        m_CloseBtn.GetWindowRect(bRect);
        m_CloseBtn.MoveWindow(bRect.left - pRect.left, m_nDlgHeight + 65 - bRect.Height(), bRect.Width(), bRect.Height());
        m_CloseBtn.ReInit(pRect.left, pRect.top);
    }
    else
    {
        CRect pRect, bRect;
        CDlgTask::GetWindowRect(pRect);
        CDlgTask::MoveWindow(m_Pnt.x, m_Pnt.y, pRect.Width(), 235);
        m_CloseBtn.GetWindowRect(bRect);
        m_CloseBtn.MoveWindow(bRect.left - pRect.left, 209, bRect.Width(), bRect.Height());
        m_CloseBtn.ReInit(pRect.left, pRect.top);
    }
}

void CDlgTask::Show()
{
    int ShowHeight = 0 ;
    int j = 0;
    // Show the Dialog
    CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                     "Dialog23",
                     EXIGENCE_IMMEDIATE ) ;
    if ( pShowAni != NULL )
    {
        CRect rect;
        CDlgTask::GetWindowRect (rect);
        //			CMyBitmap::ShowRect (m_Pnt.x, m_Pnt.y ,m_Pnt.x + rect.Width (), m_Pnt.y + rect.Height (),0xffff0000);
        pShowAni->Show( 0,
                        m_Pnt.x,
                        m_Pnt.y ) ;
        int nHeight = rect.Height () - 135 - 100;
        if (nHeight > 0)
            pShowAni->ShowEx( 1,
                              m_Pnt.x + 6,
                              m_Pnt.y + 135,
                              m_DlgRect,
                              m_DlgRect.Width(),
                              nHeight ) ;
        if (nHeight < 0)
        {
            pShowAni->Show(2, m_Pnt.x , m_Pnt.y + 135);
        }
        else
        {
            pShowAni->Show (2, m_Pnt.x, m_Pnt.y + rect.Height () - 100);
        }
    }
    else
    {
        return ;
    }
    // Show the headpic
    if ( m_nPicIndex != -1 )
    {
        CAni* FaceAni = g_objGameDataSet.GetNpcFaceIcon ( m_nPicIndex,
                        EXIGENCE_IMMEDIATE ) ;
        if ( FaceAni != NULL )
        {
            m_PicSize = FaceAni->GetSize ( 0 ) ;
            FaceAni->ShowEx ( 0,
                              m_Pnt.x + 47,
                              m_Pnt.y + 47,
                              CRect ( 0, 0, m_PicSize.iWidth, m_PicSize.iHeight ),
                              HEADPIC_WIDTH,
                              HEADPIC_HEIGHT ) ;
        }
    }
    // Show the ask text
    for ( int i = 0; i < m_nTextAmount; i++ )
    {
        for ( j = 0; j < m_TaskText[i].nLineNum; j++ )
        {
            char tempChar[TEXT_LIMIT + 1] = "";
            strncpy ( tempChar,
                      m_TaskText[i].strText + m_TaskText[i].nLineIndex[j],
                      m_TaskText[i].nLineIndex[j + 1] - m_TaskText[i].nLineIndex[j] ) ;
            if ( m_TaskText[i].nLineIndex[j + 1] - m_TaskText[i].nLineIndex[j] == TEXT_LIMIT )
            {
                tempChar[TEXT_LIMIT] = NULL ;
            }
            else
            {
                tempChar[m_TaskText[i].nLineIndex[j + 1] - m_TaskText[i].nLineIndex[j]] = NULL ;
            }
            CMyBitmap::ShowString ( m_Pnt.x + 164,
                                    m_Pnt.y + 46 + ShowHeight,
                                    0xffff00,
                                    tempChar ) ;
            ShowHeight += 18 ;
        }
    }
    // Show the link text
    for ( int i = 0; i < m_nLinkAmount; i++ )
    {
        ShowHeight = 0 ;
        for ( j = 0; j < m_TaskLink[i].nLineNum; j++ )
        {
            char tempChar[LINK_LIMIT + 1] = "";
            int nLen = abs(m_TaskLink[i].nLineIndex[j + 1] - m_TaskLink[i].nLineIndex[j]);
            strncpy ( tempChar,
                      m_TaskLink[i].strText + m_TaskLink[i].nLineIndex[j],
                      nLen ) ;
            if ( m_TaskLink[i].nLineIndex[j + 1] - m_TaskLink[i].nLineIndex[j] == LINK_LIMIT )
            {
                tempChar[LINK_LIMIT] = NULL ;
            }
            else
            {
                tempChar[m_TaskLink[i].nLineIndex[j + 1] - m_TaskLink[i].nLineIndex[j]] = NULL ;
            }
            if ( m_btCurLink == i && m_bMouseMove )
                CMyBitmap::ShowString ( m_Pnt.x + m_TaskLink[i].Rect.left,
                                        m_Pnt.y + m_TaskLink[i].Rect.top + ShowHeight,
                                        0xff0000,
                                        tempChar ) ;
            else
                CMyBitmap::ShowString ( m_Pnt.x + m_TaskLink[i].Rect.left,
                                        m_Pnt.y + m_TaskLink[i].Rect.top + ShowHeight,
                                        0xffffff,
                                        tempChar ) ;
            ShowHeight += 18 * ( j + 1 ) ;
        }
    }
    // Show the edit and the button
    if ( m_nEditAmount > 0 )
    {
        if ( m_TaskEdit.strTitle )
        {
            CRect rect, prect ;
            m_TaskEdtAnswer.GetWindowRect ( rect ) ;
            CDlgTask::GetWindowRect ( prect ) ;
            CMyBitmap::ShowString ( m_Pnt.x + ( rect.left - prect.left ),
                                    m_Pnt.y + ( rect.top - prect.top ) - 18,
                                    0xffff00,
                                    m_TaskEdit.strTitle ) ;
        }
        m_TaskEdtAnswer.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TaskBtnAnswer.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
    // Show the close button
    m_CloseBtn.Show ( m_Pnt.x, m_Pnt.y ) ;
}

void CDlgTask::OnMouseMove(UINT nFlags, CPoint point)
{
    if ( m_bMouseMove )
    {
        if ( !m_TaskLink[m_btCurLink].Rect.PtInRect ( point ) )
        {
            m_bMouseMove = false ;
        }
    }
    else
    {
        for ( int i = 0; i < m_nLinkAmount; i++ )
        {
            if ( m_TaskLink[i].Rect.PtInRect ( point ) )
            {
                DXPlaySound("Sound/Mouse_Move.wav");
                m_bMouseMove = true ;
                m_btCurLink = i ;
                break ;
            }
        }
    }
    CDialog::OnMouseMove(nFlags, point);
}

void CDlgTask::OnLButtonDown(UINT nFlags, CPoint point)
{
    if ( m_bMouseMove )
    {
        DXPlaySound("Sound/Server_Sgroup.wav");
        g_objHero.AnswerTaskDialog ( m_TaskLink[m_btCurLink].nIndex, NULL ) ;
        CDlgTask::EnableWindow( false ) ;
    }
    else
    {
        LPARAM lParam ;
        lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
        CDlgTask::GetParent()->PostMessage( WM_LBUTTONDOWN, nFlags, lParam ) ;
    }
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgTask::OnTaskBtnAnswer()
{
    if ( m_TaskEdtAnswer.GetWindowTextLength () > 0 )
    {
        char strAnswer[32] ;
        m_TaskEdtAnswer.GetWindowText ( strAnswer, 32) ;
        if (!StringCheck(strAnswer))
        {
            const int TASK_EDIT_ERROR = 100500;
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(TASK_EDIT_ERROR));
            m_TaskEdtAnswer.SetWindowText("");
            return ;
        }
        g_objHero.AnswerTaskDialog ( m_TaskEdit.nIndex, strAnswer ) ;
        CDlgTask::EnableWindow( false ) ;
    }
}

void CDlgTask::OnBtnClose()
{
    DXPlaySound("Sound/window_close.wav");
    g_objHero.AnswerTaskDialog ( m_nCloseIndex, NULL ) ;
    CDlgTask::EnableWindow( false ) ;
}

void CDlgTask::OnLButtonUp(UINT nFlags, CPoint point)
{
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgTask::GetParent()->PostMessage( WM_LBUTTONUP, nFlags, lParam ) ;
    CDialog::OnLButtonUp(nFlags, point);
}

void CDlgTask::OnRButtonDown(UINT nFlags, CPoint point)
{
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgTask::GetParent()->PostMessage( WM_RBUTTONDOWN, nFlags, lParam ) ;
    CDialog::OnRButtonDown(nFlags, point);
}

void CDlgTask::OnRButtonUp(UINT nFlags, CPoint point)
{
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgTask::GetParent()->PostMessage( WM_RBUTTONUP, nFlags, lParam ) ;
    CDialog::OnRButtonUp(nFlags, point);
}


