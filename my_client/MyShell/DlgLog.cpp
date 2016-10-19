
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgLog.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgLog.h"

#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLog dialog


CDlgLog::CDlgLog(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgLog::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgLog)
    m_EdtHbr = CreateSolidBrush ( RGB ( 15, 47, 57 ) ) ;
    m_bFirst = true ;
    m_nLastLine = 0 ;
    m_nLogChannel = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgLog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgLog)
    DDX_Control(pDX, IDC_LOG_LOGSYS, m_LogSys);
    DDX_Control(pDX, IDC_LOG_LOGMY, m_LogMy);
    DDX_Control(pDX, IDC_LOG_LOGTEAM, m_LogTeam);
    DDX_Control(pDX, IDC_LOG_LOGS, m_LogS);
    DDX_Control(pDX, IDC_LOG_LOGG, m_LogG);
    DDX_Control(pDX, IDC_LOG_LOGFRD, m_LogFrd);
    DDX_Control(pDX, IDC_LOG_LOGFAC, m_LogFac);
    DDX_Control(pDX, IDC_LOG_LOGALL, m_LogAll);
    DDX_Control(pDX, IDC_LOG_CLOSE, m_LogClose);
    DDX_Control(pDX, IDC_LOG_DELALL, m_LogDelAll);
    DDX_Control(pDX, IDC_LOG_DEL, m_LogDel);
    DDX_Control(pDX, IDC_LOG_RECORD, m_LogRecord);
    DDX_Control(pDX, IDC_LOG_DATA, m_LogData);
    DDX_Control(pDX, IDC_LOG_FILE, m_LogFile);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLog, CDialog)
    //{{AFX_MSG_MAP(CDlgLog)
    ON_WM_DESTROY()
    ON_CBN_SELCHANGE(IDC_LOG_FILE, OnSelchangeLogFile)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_LOG_DEL, OnLogDel)
    ON_BN_CLICKED(IDC_LOG_DELALL, OnLogDelall)
    ON_BN_CLICKED(IDC_LOG_RECORD, OnLogRecord)
    ON_BN_CLICKED(IDC_LOG_CLOSE, OnLogClose)
    ON_BN_CLICKED(IDC_LOG_LOGG, OnLogLogg)
    ON_BN_CLICKED(IDC_LOG_LOGS, OnLogLogs)
    ON_BN_CLICKED(IDC_LOG_LOGFAC, OnLogLogfac)
    ON_BN_CLICKED(IDC_LOG_LOGFRD, OnLogLogfrd)
    ON_BN_CLICKED(IDC_LOG_LOGTEAM, OnLogLogteam)
    ON_BN_CLICKED(IDC_LOG_LOGALL, OnLogLogall)
    ON_BN_CLICKED(IDC_LOG_LOGSYS, OnLogLogsys)
    ON_BN_CLICKED(IDC_LOG_LOGMY, OnLogLogmy)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLog message handlers

BOOL CDlgLog::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    GetDlgItem( IDC_LOG_LOGALL )->GetWindowRect ( rect ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLog::OnDestroy()
{
    CDialog::OnDestroy();
    if (m_Hbr)
    {
        DeleteObject( m_Hbr ) ;
    }
    if ( m_EdtHbr )
    {
        DeleteObject ( m_EdtHbr ) ;
    }
}

void CDlgLog::SetLog(BYTE btType)
{
    switch ( btType )
    {
    case 0:
        {
            m_LogFile.ResetContent () ;
            int nAmount = g_objGameMsg.m_objChatLog.GetLogFileAmount () ;
            for ( int i = 0; i < nAmount; i++ )
            {
                ChatLogFile* pLogFile ;
                pLogFile = g_objGameMsg.m_objChatLog.GetLogFileByIndex ( i ) ;
                if ( pLogFile != NULL )
                {
                    char strTemp[64] ;
                    sprintf ( strTemp, "%d-%d-%d", pLogFile->nYear, pLogFile->nMonth, pLogFile->nDay ) ;
                    m_LogFile.InsertString ( 0, strTemp ) ;
                }
            }
            m_LogFile.SetCurSel ( 0 ) ;
            CDlgLog::OnSelchangeLogFile () ;
        }
        break ;
    case 1:
        {
            int nCurIndex = m_LogData.LineIndex ( m_LogData.GetFirstVisibleLine () ) ;
            int nLineAmount = g_objGameMsg.m_objChatLog.GetLineAmount () ;
            CString strText ;
            for ( int i = m_nLastLine; i < nLineAmount; i++ )
            {
                ChatLogLine* pLogLine ;
                pLogLine = g_objGameMsg.m_objChatLog.GetLineByIndex ( i ) ;
                if ( pLogLine != NULL && g_objGameMsg.m_objChatLog.TestLine ( m_nLogChannel, pLogLine ))
                {
                    int nLastSel = m_LogData.GetWindowTextLength () ;
                    m_LogData.SetSel ( nLastSel, nLastSel, true ) ;
                    strText = pLogLine->szTxt ;
                    strText.Insert ( strText.GetLength (), "\r\n\r\n" ) ;
                    m_LogData.ReplaceSel ( strText ) ;
                }
            }
            m_LogData.SetSel ( nCurIndex, nCurIndex ) ;
            m_nLastLine = nLineAmount ;
        }
        break ;
    }
}

void CDlgLog::OnSelchangeLogFile()
{
    ChatLogFile* pLogFile = g_objGameMsg.m_objChatLog.GetLogFileByIndex ( m_LogFile.GetCount () - m_LogFile.GetCurSel () - 1 ) ;
    if ( pLogFile != NULL )
    {
        m_nLastLine = 0 ;
        m_LogData.SetWindowText ( NULL ) ;
        g_objGameMsg.m_objChatLog.LoadLog ( pLogFile->szFileName ) ;
    }
}

HBRUSH CDlgLog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if ( nCtlColor == CTLCOLOR_DLG )
    {
        return m_Hbr ;
    }
    else
    {
        pDC->SetBkColor ( RGB ( 15, 47, 57 ) ) ;
        pDC->SetTextColor ( RGB ( 214, 213, 211 ) ) ;
        return m_EdtHbr ;
    }
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    return hbr;
}

void CDlgLog::OnLogDel()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    g_objGameMsg.m_objChatLog.DelLogLine(m_nLogChannel);
    m_LogData.SetWindowText ( NULL ) ;
    m_nLastLine = 0 ;
    CDlgLog::SetLog(1);
}

void CDlgLog::OnLogDelall()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    ChatLogFile* pLogFile = g_objGameMsg.m_objChatLog.GetLogFileByIndex ( m_LogFile.GetCount () - m_LogFile.GetCurSel () - 1 ) ;
    if ( pLogFile != NULL )
    {
        g_objGameMsg.m_objChatLog.DelLogFile ( pLogFile->szFileName ) ;
    }
    //	g_objGameMsg.m_objChatLog.DelAllLogFile () ;
}

void CDlgLog::OnLogRecord()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_LogRecord.GetCheck () )
    {
        g_objGameMsg.m_objChatLog.LogOn () ;
    }
    else
    {
        g_objGameMsg.m_objChatLog.LogOff () ;
    }
}

void CDlgLog::OnLogClose()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgLog::ShowWindow ( SW_HIDE ) ;
}

void CDlgLog::OnLogLogg()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( _TXTATR_TALK != m_nLogChannel )
    {
        m_LogData.SetWindowText ( NULL ) ;
        m_nLastLine = 0 ;
        m_nLogChannel = _TXTATR_TALK ;
        CDlgLog::SetLog ( 1 ) ;
        PopupCurChannel(m_nLogChannel);
    }
}

void CDlgLog::OnLogLogs()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( _TXTATR_PRIVATE != m_nLogChannel )
    {
        m_LogData.SetWindowText ( NULL ) ;
        m_nLastLine = 0 ;
        m_nLogChannel = _TXTATR_PRIVATE ;
        CDlgLog::SetLog ( 1 ) ;
        PopupCurChannel(m_nLogChannel);
    }
}

void CDlgLog::OnLogLogfac()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( _TXTATR_SYNDICATE != m_nLogChannel )
    {
        m_LogData.SetWindowText ( NULL ) ;
        m_nLastLine = 0 ;
        m_nLogChannel = _TXTATR_SYNDICATE ;
        CDlgLog::SetLog ( 1 ) ;
        PopupCurChannel(m_nLogChannel);
    }
}

void CDlgLog::OnLogLogfrd()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( _TXTATR_FRIEND != m_nLogChannel )
    {
        m_LogData.SetWindowText ( NULL ) ;
        m_nLastLine = 0 ;
        m_nLogChannel = _TXTATR_FRIEND ;
        CDlgLog::SetLog ( 1 ) ;
        PopupCurChannel(m_nLogChannel);
    }
}

void CDlgLog::OnLogLogteam()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( _TXTATR_TEAM != m_nLogChannel )
    {
        m_LogData.SetWindowText ( NULL ) ;
        m_nLastLine = 0 ;
        m_nLogChannel = _TXTATR_TEAM ;
        CDlgLog::SetLog ( 1 ) ;
        PopupCurChannel(m_nLogChannel);
    }
}

void CDlgLog::OnLogLogall()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( 0 != m_nLogChannel )
    {
        m_LogData.SetWindowText ( NULL ) ;
        m_nLastLine = 0 ;
        m_nLogChannel = 0 ;
        CDlgLog::SetLog ( 1 ) ;
        PopupCurChannel(m_nLogChannel);
    }
}

void CDlgLog::ResetBk(int nMode)
{
    char strName[32] ;
    char strMain[8] ;
    if ( nMode == 0 )
    {
        strcpy ( strMain, "Main" ) ;
    }
    else
    {
        sprintf ( strMain, "Main", nMode ) ;
    }
    sprintf ( strName, "Data/%s/DlgLogBk.BMP", strMain ) ;
    HBITMAP hBmp ;
    hBmp =  (HBITMAP )::LoadImage ( AfxGetInstanceHandle(),
                                    strName,
                                    IMAGE_BITMAP,
                                    0,
                                    0,
                                    LR_LOADFROMFILE ) ;
    m_Hbr = CreatePatternBrush ( hBmp ) ;
    sprintf ( strName, "Data/%s/LogDel.bmp", strMain ) ;
    hBmp = (HBITMAP)::LoadImage ( AfxGetInstanceHandle(),
                                  strName,
                                  IMAGE_BITMAP,
                                  0,
                                  0,
                                  LR_LOADFROMFILE ) ;
    m_LogDel.SetBitmap ( hBmp ) ;
    sprintf ( strName, "Data/%s/LogDelAll.bmp", strMain ) ;
    hBmp = (HBITMAP)::LoadImage ( AfxGetInstanceHandle(),
                                  strName,
                                  IMAGE_BITMAP,
                                  0,
                                  0,
                                  LR_LOADFROMFILE ) ;
    m_LogDelAll.SetBitmap ( hBmp ) ;
    sprintf ( strName, "Data/%s/LogAll.bmp", strMain ) ;
    hBmp = (HBITMAP)::LoadImage ( AfxGetInstanceHandle(),
                                  strName,
                                  IMAGE_BITMAP,
                                  0,
                                  0,
                                  LR_LOADFROMFILE ) ;
    m_LogAll.SetBitmap ( hBmp ) ;
    sprintf ( strName, "Data/%s/LogFac.bmp", strMain ) ;
    hBmp = (HBITMAP)::LoadImage ( AfxGetInstanceHandle(),
                                  strName,
                                  IMAGE_BITMAP,
                                  0,
                                  0,
                                  LR_LOADFROMFILE ) ;
    m_LogFac.SetBitmap ( hBmp ) ;
    sprintf ( strName, "Data/%s/LogFrd.bmp", strMain ) ;
    hBmp = (HBITMAP)::LoadImage ( AfxGetInstanceHandle(),
                                  strName,
                                  IMAGE_BITMAP,
                                  0,
                                  0,
                                  LR_LOADFROMFILE ) ;
    m_LogFrd.SetBitmap ( hBmp ) ;
    sprintf ( strName, "Data/%s/LogG.bmp", strMain ) ;
    hBmp = (HBITMAP)::LoadImage ( AfxGetInstanceHandle(),
                                  strName,
                                  IMAGE_BITMAP,
                                  0,
                                  0,
                                  LR_LOADFROMFILE ) ;
    m_LogG.SetBitmap ( hBmp ) ;
    sprintf ( strName, "Data/%s/LogS.bmp", strMain ) ;
    hBmp = (HBITMAP)::LoadImage ( AfxGetInstanceHandle(),
                                  strName,
                                  IMAGE_BITMAP,
                                  0,
                                  0,
                                  LR_LOADFROMFILE ) ;
    m_LogS.SetBitmap ( hBmp ) ;
    sprintf ( strName, "Data/%s/LogTeam.bmp", strMain ) ;
    hBmp = (HBITMAP)::LoadImage ( AfxGetInstanceHandle(),
                                  strName,
                                  IMAGE_BITMAP,
                                  0,
                                  0,
                                  LR_LOADFROMFILE ) ;
    m_LogTeam.SetBitmap ( hBmp ) ;
    sprintf ( strName, "Data/%s/Close.bmp", strMain ) ;
    hBmp = (HBITMAP)::LoadImage ( AfxGetInstanceHandle(),
                                  strName,
                                  IMAGE_BITMAP,
                                  0,
                                  0,
                                  LR_LOADFROMFILE ) ;
    m_LogClose.SetBitmap ( hBmp ) ;
    sprintf(strName, "Data/%s/LogMy.bmp", strMain);
    hBmp = (HBITMAP)::LoadImage( AfxGetInstanceHandle(),
                                 strName,
                                 IMAGE_BITMAP,
                                 0,
                                 0,
                                 LR_LOADFROMFILE ) ;
    m_LogMy.SetBitmap(hBmp);
    sprintf(strName, "Data/%s/LogSys.bmp", strMain);
    hBmp = (HBITMAP)::LoadImage( AfxGetInstanceHandle(),
                                 strName,
                                 IMAGE_BITMAP,
                                 0,
                                 0,
                                 LR_LOADFROMFILE ) ;
    m_LogSys.SetBitmap(hBmp);
    CDlgLog::RedrawWindow () ;
}

void CDlgLog::OnLogLogsys()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( _TXTATR_SYSTEM != m_nLogChannel )
    {
        m_LogData.SetWindowText ( NULL ) ;
        m_nLastLine = 0 ;
        m_nLogChannel = _TXTATR_SYSTEM ;
        CDlgLog::SetLog ( 1 ) ;
        PopupCurChannel(m_nLogChannel);
    }
}

void CDlgLog::OnLogLogmy()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( _TXTATR_MY != m_nLogChannel )
    {
        m_LogData.SetWindowText ( NULL ) ;
        m_nLastLine = 0 ;
        m_nLogChannel = _TXTATR_MY ;
        CDlgLog::SetLog ( 1 ) ;
        PopupCurChannel(m_nLogChannel);
    }
}

void CDlgLog::PopupCurChannel(int nChannels)
{
    CDlgLog::ResetBk(0);
    char strName[32] ;
    char strMain[8] ;
    strcpy ( strMain, "Main" ) ;
    HBITMAP hBmp ;
    switch(nChannels)
    {
    case 0:
        {
            sprintf ( strName, "Data/%s/LogAllClick.BMP", strMain ) ;
            hBmp =  (HBITMAP )::LoadImage ( AfxGetInstanceHandle(),
                                            strName,
                                            IMAGE_BITMAP,
                                            0,
                                            0,
                                            LR_LOADFROMFILE ) ;
            m_LogAll.SetBitmap(hBmp);
        }
        break;
    case _TXTATR_TALK:
        {
            sprintf ( strName, "Data/%s/LogGClick.BMP", strMain ) ;
            hBmp =  (HBITMAP )::LoadImage ( AfxGetInstanceHandle(),
                                            strName,
                                            IMAGE_BITMAP,
                                            0,
                                            0,
                                            LR_LOADFROMFILE ) ;
            m_LogG.SetBitmap(hBmp);
        }
        break;
    case _TXTATR_PRIVATE:
        {
            sprintf ( strName, "Data/%s/LogSClick.BMP", strMain ) ;
            hBmp =  (HBITMAP )::LoadImage ( AfxGetInstanceHandle(),
                                            strName,
                                            IMAGE_BITMAP,
                                            0,
                                            0,
                                            LR_LOADFROMFILE ) ;
            m_LogS.SetBitmap(hBmp);
        }
        break;
    case _TXTATR_FRIEND:
        {
            sprintf ( strName, "Data/%s/LogFrdClick.BMP", strMain ) ;
            hBmp =  (HBITMAP )::LoadImage ( AfxGetInstanceHandle(),
                                            strName,
                                            IMAGE_BITMAP,
                                            0,
                                            0,
                                            LR_LOADFROMFILE ) ;
            m_LogFrd.SetBitmap(hBmp);
        }
        break;
    case _TXTATR_TEAM:
        {
            sprintf ( strName, "Data/%s/LogTeamClick.BMP", strMain ) ;
            hBmp =  (HBITMAP )::LoadImage ( AfxGetInstanceHandle(),
                                            strName,
                                            IMAGE_BITMAP,
                                            0,
                                            0,
                                            LR_LOADFROMFILE ) ;
            m_LogTeam.SetBitmap(hBmp);
        }
        break;
    case _TXTATR_SYNDICATE:
        {
            sprintf ( strName, "Data/%s/LogFacClick.BMP", strMain ) ;
            hBmp =  (HBITMAP )::LoadImage ( AfxGetInstanceHandle(),
                                            strName,
                                            IMAGE_BITMAP,
                                            0,
                                            0,
                                            LR_LOADFROMFILE ) ;
            m_LogFac.SetBitmap(hBmp);
        }
        break;
    case _TXTATR_SYSTEM:
        {
            sprintf ( strName, "Data/%s/LogSysClick.BMP", strMain ) ;
            hBmp =  (HBITMAP )::LoadImage ( AfxGetInstanceHandle(),
                                            strName,
                                            IMAGE_BITMAP,
                                            0,
                                            0,
                                            LR_LOADFROMFILE ) ;
            m_LogSys.SetBitmap(hBmp);
        }
        break;
    case _TXTATR_MY:
        {
            sprintf ( strName, "Data/%s/LogMyClick.BMP", strMain ) ;
            hBmp =  (HBITMAP )::LoadImage ( AfxGetInstanceHandle(),
                                            strName,
                                            IMAGE_BITMAP,
                                            0,
                                            0,
                                            LR_LOADFROMFILE ) ;
            m_LogMy.SetBitmap(hBmp);
        }
        break;
    default:
        break;
    }
    CDlgLog::RedrawWindow () ;
}
