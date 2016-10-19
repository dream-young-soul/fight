
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgServerDlg.h : header file
//

#if !defined(AFX_MSGSERVERDLG_H__2770DC74_B31B_4419_8F62_919970CD2D80__INCLUDED_)
#define AFX_MSGSERVERDLG_H__2770DC74_B31B_4419_8F62_919970CD2D80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "typedef.h"
#include "InternetPort.h"
#include "TimeOut.h"
#include "I_Shell.h"
#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CMsgServerDlg dialog
enum
{
    SHELLSTATE_NONE,
    SHELLSTATE_INIT,
    SHELLSTATE_RUNNING,
    SHELLSTATE_CLOSING,
    SHELLSTATE_END,
};

/////////////////////////////////////////////////////////////////////////////
class	IMessagePort;
class	CMapGroupThread;
class	CSocketThread;
class	CWorldThread;
class CMsgServerDlg : public CDialog
{
    // Construction
public:
    bool LoadConfigIni();
    bool ProcessMsg(OBJID idPacket, void* pMsg, int nType, int nSource);
    void PrintText(LPCTSTR szMsg);
    bool Process();
    bool ProcessInterMsg();
    CMsgServerDlg(CWnd* pParent = NULL);	// standard constructor

    // Dialog Data
    //{{AFX_DATA(CMsgServerDlg)
    enum { IDD = IDD_MSGSERVER_DIALOG };
    CString	m_sKernelState;
    CString	m_sShellState;
    CString	m_sTalk;
    CString	m_sText;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMsgServerDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    NAMESTR		m_szServer;
    char m_szStartServer[20];
    long m_nLogoutPlayers;
    long m_nLoginPlayers;
    CTimeOut m_tStat5Min;
    CTimeOut m_tStat;
    typedef	vector<CMapGroupThread*>	MAPGROUP_SET;
    MAPGROUP_SET m_setMapGroupThread;
    CSocketThread* m_pSocketThread;
    CWorldThread* m_pWorldThread;
    IMessagePort* m_pMsgPort;
    IMessagePort* m_pInterPort;
    int m_nTextLines;
    int m_nState;
    int m_nAllPlayers;
    int m_nMaxPlayers;
    HICON m_hIcon;

    STAT_STRUCT m_stat;
    HANDLE	m_hMutexThread;
    HANDLE	m_hMutexServer;

    // Generated message map functions
    //{{AFX_MSG(CMsgServerDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTimer(UINT nIDEvent);
    virtual void OnCancel();
    afx_msg void OnSend();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGSERVERDLG_H__2770DC74_B31B_4419_8F62_919970CD2D80__INCLUDED_)
