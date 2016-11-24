
//**********************************************************
// 代码编辑器
//**********************************************************

// MyShell.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MyShell.h"
#include "GameDataset.h"
#include "3droledata.h"
#include "BkWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyShellApp

BEGIN_MESSAGE_MAP(CMyShellApp, CWinApp)
    //{{AFX_MSG_MAP(CMyShellApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyShellApp construction
//GemInfo CMyShellApp::m_GemInfo [21] ;
char	g_strControlAni[32] ;
char	g_strMagicAni[32] ;
BOOL	g_bPickUp ;
BOOL	g_bMouseMoveIn ;
CBrush  g_StockBrush;

CMyShellApp::CMyShellApp()
{
    // Init the Ani file name
    strcpy ( g_strControlAni, "ani/Control.Ani" ) ;
    strcpy ( g_strMagicAni, "ani/Magic.Ani" ) ;
    g_bPickUp = false ;
    g_bMouseMoveIn = false ;
    // Init the Gem struct value
    // 700001
    /*	m_GemInfo[0].btAmount = 0 ;
    	m_GemInfo[0].btAtk = 0 ;
    	m_GemInfo[0].btHit = 0 ;
    	m_GemInfo[0].btMagicAtk = 5 ;
    	m_GemInfo[0].btMagicExp = 0 ;
    	m_GemInfo[0].btRoleExp = 0 ;
    	m_GemInfo[0].btWeaponSkill = 0 ;
    	m_GemInfo[0].strID = 10200 ;

    	// 700002
    	m_GemInfo[1].btAmount = 0 ;
    	m_GemInfo[1].btAtk = 0 ;
    	m_GemInfo[1].btHit = 0 ;
    	m_GemInfo[1].btMagicAtk = 10 ;
    	m_GemInfo[1].btMagicExp = 0 ;
    	m_GemInfo[1].btRoleExp = 0 ;
    	m_GemInfo[1].btWeaponSkill = 0 ;
    	m_GemInfo[1].strID = 10201 ;

    	// 700003
    	m_GemInfo[2].btAmount = 0 ;
    	m_GemInfo[2].btHit = 0 ;
    	m_GemInfo[2].btMagicAtk = 15 ;
    	m_GemInfo[2].btMagicExp = 0 ;
    	m_GemInfo[2].btRoleExp = 0 ;
    	m_GemInfo[2].btWeaponSkill = 0 ;
    	m_GemInfo[2].strID = 10202 ;

    	// 700011
    	m_GemInfo[3].btAmount = 0 ;
    	m_GemInfo[3].btAtk = 5 ;
    	m_GemInfo[3].btHit = 0 ;
    	m_GemInfo[3].btMagicAtk = 0 ;
    	m_GemInfo[3].btMagicExp = 0 ;
    	m_GemInfo[3].btRoleExp = 0 ;
    	m_GemInfo[3].btWeaponSkill = 0 ;
    	m_GemInfo[3].strID = 10203 ;

    	// 700012
    	m_GemInfo[4].btAmount = 0 ;
    	m_GemInfo[4].btAtk = 10 ;
    	m_GemInfo[4].btHit = 0 ;
    	m_GemInfo[4].btMagicAtk = 0 ;
    	m_GemInfo[4].btMagicExp = 0 ;
    	m_GemInfo[4].btRoleExp = 0 ;
    	m_GemInfo[4].btWeaponSkill = 0 ;
    	m_GemInfo[4].strID = 10204 ;

    	// 700013
    	m_GemInfo[5].btAmount = 0 ;
    	m_GemInfo[5].btAtk = 15 ;
    	m_GemInfo[5].btHit = 0 ;
    	m_GemInfo[5].btMagicAtk = 0 ;
    	m_GemInfo[5].btMagicExp = 0 ;
    	m_GemInfo[5].btRoleExp = 0 ;
    	m_GemInfo[5].btWeaponSkill = 0 ;
    	m_GemInfo[5].strID = 10205 ;

    	// 700021
    	m_GemInfo[6].btAmount = 0 ;
    	m_GemInfo[6].btAtk = 0 ;
    	m_GemInfo[6].btHit = 5 ;
    	m_GemInfo[6].btMagicAtk = 0 ;
    	m_GemInfo[6].btMagicExp = 0 ;
    	m_GemInfo[6].btRoleExp = 0 ;
    	m_GemInfo[6].btWeaponSkill = 0 ;
    	m_GemInfo[6].strID = 10206 ;

    	// 700022
    	m_GemInfo[7].btAmount = 0 ;
    	m_GemInfo[7].btAtk = 0 ;
    	m_GemInfo[7].btHit = 10 ;
    	m_GemInfo[7].btMagicAtk = 0 ;
    	m_GemInfo[7].btMagicExp = 0 ;
    	m_GemInfo[7].btRoleExp = 0 ;
    	m_GemInfo[7].btWeaponSkill = 0 ;
    	m_GemInfo[7].strID = 10207 ;

    	// 700023
    	m_GemInfo[8].btAmount = 0 ;
    	m_GemInfo[8].btAtk = 0 ;
    	m_GemInfo[8].btHit = 15 ;
    	m_GemInfo[8].btMagicAtk = 0 ;
    	m_GemInfo[8].btMagicExp = 0 ;
    	m_GemInfo[8].btRoleExp = 0 ;
    	m_GemInfo[8].btWeaponSkill = 0 ;
    	m_GemInfo[8].strID = 10208 ;

    	// 700031
    	m_GemInfo[9].btAmount = 0 ;
    	m_GemInfo[9].btAtk = 0 ;
    	m_GemInfo[9].btHit = 0 ;
    	m_GemInfo[9].btMagicAtk = 0 ;
    	m_GemInfo[9].btMagicExp = 0 ;
    	m_GemInfo[9].btRoleExp = 10 ;
    	m_GemInfo[9].btWeaponSkill = 0 ;
    	m_GemInfo[9].strID = 10209 ;

    	// 700032
    	m_GemInfo[10].btAmount = 0 ;
    	m_GemInfo[10].btAtk = 0 ;
    	m_GemInfo[10].btHit = 0 ;
    	m_GemInfo[10].btMagicAtk = 0 ;
    	m_GemInfo[10].btMagicExp = 0 ;
    	m_GemInfo[10].btRoleExp = 15 ;
    	m_GemInfo[10].btWeaponSkill = 0 ;
    	m_GemInfo[10].strID = 10210 ;

    	// 700033
    	m_GemInfo[11].btAmount = 0 ;
    	m_GemInfo[11].btAtk = 0 ;
    	m_GemInfo[11].btHit = 0 ;
    	m_GemInfo[11].btMagicAtk = 0 ;
    	m_GemInfo[11].btMagicExp = 0 ;
    	m_GemInfo[11].btRoleExp = 25 ;
    	m_GemInfo[11].btWeaponSkill = 0 ;
    	m_GemInfo[11].strID = 10211 ;

    	// 700041
    	m_GemInfo[12].btAmount = 50 ;
    	m_GemInfo[12].btAtk = 0 ;
    	m_GemInfo[12].btHit = 0 ;
    	m_GemInfo[12].btMagicAtk = 0 ;
    	m_GemInfo[12].btMagicExp = 0 ;
    	m_GemInfo[12].btRoleExp = 0 ;
    	m_GemInfo[12].btWeaponSkill = 0 ;
    	m_GemInfo[12].strID = 10212 ;

    	// 700042
    	m_GemInfo[13].btAmount = 100 ;
    	m_GemInfo[13].btAtk = 0 ;
    	m_GemInfo[13].btHit = 0 ;
    	m_GemInfo[13].btMagicAtk = 0 ;
    	m_GemInfo[13].btMagicExp = 0 ;
    	m_GemInfo[13].btRoleExp = 0 ;
    	m_GemInfo[13].btWeaponSkill = 0 ;
    	m_GemInfo[13].strID = 10213 ;

    	// 700043
    	m_GemInfo[14].btAmount = 200 ;
    	m_GemInfo[14].btAtk = 0 ;
    	m_GemInfo[14].btHit = 0 ;
    	m_GemInfo[14].btMagicAtk = 0 ;
    	m_GemInfo[14].btMagicExp = 0 ;
    	m_GemInfo[14].btRoleExp = 0 ;
    	m_GemInfo[14].btWeaponSkill = 0 ;
    	m_GemInfo[14].strID = 10214 ;

    	// 700051
    	m_GemInfo[15].btAmount = 0 ;
    	m_GemInfo[15].btAtk = 0 ;
    	m_GemInfo[15].btHit = 0 ;
    	m_GemInfo[15].btMagicAtk = 0 ;
    	m_GemInfo[15].btMagicExp = 0 ;
    	m_GemInfo[15].btRoleExp = 0 ;
    	m_GemInfo[15].btWeaponSkill = 30 ;
    	m_GemInfo[15].strID = 10215 ;

    	// 700052
    	m_GemInfo[16].btAmount = 0 ;
    	m_GemInfo[16].btAtk = 0 ;
    	m_GemInfo[16].btHit = 0 ;
    	m_GemInfo[16].btMagicAtk = 0 ;
    	m_GemInfo[16].btMagicExp = 0 ;
    	m_GemInfo[16].btRoleExp = 0 ;
    	m_GemInfo[16].btWeaponSkill = 50 ;
    	m_GemInfo[16].strID = 10216 ;

    	// 700053
    	m_GemInfo[17].btAmount = 0 ;
    	m_GemInfo[17].btAtk = 0 ;
    	m_GemInfo[17].btHit = 0 ;
    	m_GemInfo[17].btMagicAtk = 0 ;
    	m_GemInfo[17].btMagicExp = 0 ;
    	m_GemInfo[17].btRoleExp = 0 ;
    	m_GemInfo[17].btWeaponSkill = 100 ;
    	m_GemInfo[17].strID = 10217 ;

    	// 700061
    	m_GemInfo[18].btAmount = 0 ;
    	m_GemInfo[18].btAtk = 0 ;
    	m_GemInfo[18].btHit = 0 ;
    	m_GemInfo[18].btMagicAtk = 0 ;
    	m_GemInfo[18].btMagicExp = 15 ;
    	m_GemInfo[18].btRoleExp = 0 ;
    	m_GemInfo[18].btWeaponSkill = 0 ;
    	m_GemInfo[18].strID = 10218 ;

    	// 700062
    	m_GemInfo[19].btAmount = 0 ;
    	m_GemInfo[19].btAtk = 0 ;
    	m_GemInfo[19].btHit = 0 ;
    	m_GemInfo[19].btMagicAtk = 0 ;
    	m_GemInfo[19].btMagicExp = 30 ;
    	m_GemInfo[19].btRoleExp = 0 ;
    	m_GemInfo[19].btWeaponSkill = 0 ;
    	m_GemInfo[19].strID = 10219 ;

    	// 700063
    	m_GemInfo[20].btAmount = 0 ;
    	m_GemInfo[20].btAtk = 0 ;
    	m_GemInfo[20].btHit = 0 ;
    	m_GemInfo[20].btMagicAtk = 0 ;
    	m_GemInfo[20].btMagicExp = 50 ;
    	m_GemInfo[20].btRoleExp = 0 ;
    	m_GemInfo[20].btWeaponSkill = 0 ;
    	m_GemInfo[20].strID = 10220 ;*/
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyShellApp object

CMyShellApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMyShellApp initialization
CBkWnd* g_pBkWnd = NULL ;

BOOL CMyShellApp::InitInstance()
{
	//取操作系统信息- 麻痹 win7以上 -画线画不了，日狗- 调试一晚上2016.11.10
	OSVERSIONINFOA version;
	memset(&version,0,sizeof(OSVERSIONINFOA));
	version.dwOSVersionInfoSize=sizeof(OSVERSIONINFOA);
	::GetVersionEx(&version);
	if(version.dwMajorVersion >= 6)//win8以上
	{
		g_FuckWindows = true;
	}
    AfxEnableControlContainer();
    // init global obj
    g_objGameDataSet.Init();
#ifndef _DEBUG
    if (strcmp(this->m_lpCmdLine, "blacknull") != 0)
    {
        ::MessageBox ( NULL, g_objGameDataSet.GetStr ( 10047 ), g_objGameDataSet.GetStr ( 10017 ), MB_OK | MB_ICONERROR ) ;
        return false;
    }
#endif
#ifdef _AFXDLL
    Enable3dControls();			// Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
    g_StockBrush.CreateStockObject( NULL_BRUSH );
    g_pBkWnd = new CBkWnd ;
    m_pMainWnd = g_pBkWnd ;
    g_pBkWnd->Create() ;
    g_pBkWnd->ShowWindow( SW_SHOW ) ;
    g_pBkWnd->SetWindowText( (char*)g_objGameDataSet.GetStr(10375) ) ;
	//禁止改变窗口大小 2013.10.11 去除了无边框窗口
	//g_pBkWnd->ModifyStyle( WS_THICKFRAME, 0, SWP_FRAMECHANGED|SWP_DRAWFRAME ); 
	// ::SetWindowPos(g_pBkWnd->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
   // ::SetWindowPos(g_pBkWnd->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    return TRUE;
}

const char* CMyShellApp::StringChange(const char* strText, BYTE btType)
{
    CString strTemp;
    strTemp = strText ;
    switch ( btType )
    {
    case 0:
        strTemp.Replace ( "\\t", "\t" ) ;
        strTemp.Replace ( "\\n", "\n" ) ;
        strcpy( ( char* )strText, strTemp ) ;
        break ;
    case 1:
        if ( -1 == strTemp.Find ( "~" ) )
        {
            return strText ;
        }
        strTemp.Replace ( "~", " " ) ;
        strcpy ( ( char* ) strText, strTemp ) ;
        break ;
    case 2:
        {
            int nLen = strlen ( strText ) ;
            if ( nLen > 3 && nLen < 7 )
            {
                strTemp.Insert ( nLen - 3, ',' ) ;
            }
            else if ( nLen >= 7 )
            {
                strTemp.Insert ( nLen - 6, ',' ) ;
                strTemp.Insert ( nLen - 2, ',' ) ;
            }
            strcpy ( ( char* ) strText, strTemp ) ;
        }
        break ;
    }
    return strText ;
}

BOOL CMyShellApp::OnIdle(LONG lCount)
{
    static DWORD timeLoop	= 0;
    DWORD timeNow	=::TimeGet();
	const DWORD FPS = 30;
    if (timeNow < timeLoop + FPS && timeNow != timeLoop)
    {
        ::Sleep( timeLoop + FPS - timeNow ) ;
    }
    else
    {
        timeLoop	=::TimeGet();
        g_pBkWnd->m_MyShellDlg.OnTimer( 1 ) ;
    }
    return TRUE ;
}

int CMyShellApp::ExitInstance()
{
    // TODO: Add your specialized code here and/or call the base class
    if (g_pBkWnd != NULL)
    {
        g_pBkWnd->DestroyWindow();
        delete g_pBkWnd;
    }
    return CWinApp::ExitInstance();
}
