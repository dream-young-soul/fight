
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGLOG_H__F300D4BA_C329_4DCF_B231_3DEBAFA1B728__INCLUDED_)
#define AFX_DLGLOG_H__F300D4BA_C329_4DCF_B231_3DEBAFA1B728__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLog.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgLog dialog

class CDlgLog : public CDialog
{
    // Construction
public:
    void ResetBk( int nMode );
    void SetLog( BYTE btType );
    void PopupCurChannel(int nChannels);
    CDlgLog(CWnd* pParent = NULL);   // standard constructor
    HBRUSH			m_Hbr ;
    HBRUSH			m_EdtHbr ;
    BOOL			m_bFirst ;
    int				m_nLastLine ;
    int				m_nLogChannel ;
    // Dialog Data
    //{{AFX_DATA(CDlgLog)
    enum { IDD = IDD_DIALOG_LOG };
    CButton	m_LogSys;
    CButton	m_LogMy;
    CButton	m_LogTeam;
    CButton	m_LogS;
    CButton	m_LogG;
    CButton	m_LogFrd;
    CButton	m_LogFac;
    CButton	m_LogAll;
    CButton	m_LogClose;
    CButton	m_LogDelAll;
    CButton	m_LogDel;
    CButton	m_LogRecord;
    CEdit		m_LogData;
    CComboBox	m_LogFile;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgLog)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgLog)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnSelchangeLogFile();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnLogDel();
    afx_msg void OnLogDelall();
    afx_msg void OnLogRecord();
    afx_msg void OnLogClose();
    afx_msg void OnLogLogg();
    afx_msg void OnLogLogs();
    afx_msg void OnLogLogfac();
    afx_msg void OnLogLogfrd();
    afx_msg void OnLogLogteam();
    afx_msg void OnLogLogall();
    afx_msg void OnLogLogsys();
    afx_msg void OnLogLogmy();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOG_H__F300D4BA_C329_4DCF_B231_3DEBAFA1B728__INCLUDED_)
