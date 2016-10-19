
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgRoleCreate.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgRoleCreate.h"

#include "3DRoleData.h"
#include "allmsg.h"
#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _SOUND_DISABLE
#define DXPlaySound //
#endif
/////////////////////////////////////////////////////////////////////////////
// CDlgRoleCreate dialog


CDlgRoleCreate::CDlgRoleCreate(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgRoleCreate::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgRoleCreate)
    m_bShow = false ;
    m_bSelRoleOk = false;
    m_bSelected = false;
    m_btSex = 1 ;
    m_btPro = 0 ;
    m_nRoleSelIndex = 0;
    m_nLookIndex = 0;
    m_nSexRole = 1;
    //}}AFX_DATA_INIT
}


void CDlgRoleCreate::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgRoleCreate)
    DDX_Control(pDX, IDC_ROLECREATE_BTN_RESET, m_BtnReset);
    DDX_Control(pDX, IDC_BTN_CHGFACE, m_BtnChgFace);
    DDX_Control(pDX, IDC_BTN_CHGBODY, m_BtnChgBody);
    DDX_Control(pDX, IDC_STA_SEX, m_StaSex);
    DDX_Control(pDX, IDC_IMG_FACE, m_ImgFace);
    DDX_Control(pDX, IDC_IMG_DATABK, m_ImgDataBk);
    DDX_Control(pDX, IDC_BTN_RIGHT3, m_BtnRight3);
    DDX_Control(pDX, IDC_BTN_RIGHT2, m_BtnRight2);
    DDX_Control(pDX, IDC_BTN_RIGHT1, m_BtnRight1);
    DDX_Control(pDX, IDC_BTN_LEFT3, m_BtnLeft3);
    DDX_Control(pDX, IDC_BTN_LEFT2, m_BtnLeft2);
    DDX_Control(pDX, IDC_BTN_LEFT1, m_BtnLeft1);
    DDX_Control(pDX, IDC_ROLECHOOSE_MIMG3, m_Img3);
    DDX_Control(pDX, IDC_IMG_FACESEL, m_ImgFaceSelBk);
    DDX_Control(pDX, IDC_IMG_ROLESEL, m_ImgRoleSel);
    DDX_Control(pDX, IDC_ROLECHOOSE_MIMG2, m_Img2);
    DDX_Control(pDX, IDC_ROLECHOOSE_MIMG1, m_Img1);
    DDX_Control(pDX, IDC_ROLECREATE_BTN_EXIT, m_RoleCreateExitBtn);
    DDX_Control(pDX, IDC_ROLECREATE_STA_DATA, m_RoleCreateDataSta ) ;
    DDX_Control(pDX, IDC_ROLECREATE_EDT_NAME, m_RoleCreateNameEdt ) ;
    DDX_Control(pDX, IDC_ROLECREATE_BTN_OK, m_RoleCreateOkBtn );
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRoleCreate, CDialog)
    //{{AFX_MSG_MAP(CDlgRoleCreate)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_ROLECREATE_BTN_OK, OnRolecreateBtnOk)
    ON_BN_CLICKED(IDC_ROLECREATE_BTN_EXIT, OnRolecreateBtnExit)
    ON_BN_CLICKED(IDC_ROLECHOOSE_MIMG1, OnRolechooseMimg1)
    ON_BN_CLICKED(IDC_ROLECHOOSE_MIMG2, OnRolechooseMimg2)
    ON_BN_CLICKED(IDC_ROLECREATE_BTN_RESET, OnRolecreateBtnReset)
    ON_BN_CLICKED(IDC_ROLECHOOSE_MIMG3, OnRolechooseMimg3)
    ON_BN_CLICKED(IDC_BTN_LEFT3, OnBtnLeft3)
    ON_BN_CLICKED(IDC_BTN_RIGHT3, OnBtnRight3)
    ON_WM_LBUTTONDOWN()
    ON_BN_CLICKED(IDC_BTN_CHGBODY, OnBtnChgbody)
    ON_BN_CLICKED(IDC_BTN_CHGFACE, OnBtnChgface)
    ON_BN_CLICKED(IDC_BTN_LEFT1, OnBtnLeft1)
    ON_BN_CLICKED(IDC_BTN_RIGHT1, OnBtnRight1)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRoleCreate message handlers

LRESULT CDlgRoleCreate::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if ( m_bShow )
        {
            CDlgRoleCreate::ReInit();
            m_RoleCreateNameEdt.SetFocus() ;
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
            DXPlaySound("Sound/window_close.wav");
            if (CDlgRoleCreate::IsSelRoleOk())
            {
                CDlgRoleCreate::CreateReset();
            }
            else
            {
                CDlgRoleCreate::CreateExit();
            }
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgRoleCreate::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgRoleCreate::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Init the Image
    m_ImgRoleSel.Init(0, 0, "Image30", BG_IMAGE);
    m_ImgFaceSelBk.Init(0, 0, "Image31", BG_IMAGE);
    m_ImgDataBk.Init(0, 0, "Image32", BG_IMAGE);
    // Init the edit
    m_RoleCreateNameEdt.Init( 0, 0, false, NULL, 0x000000, 0xffffff );
    m_RoleCreateNameEdt.SetLimitText( 15 ) ;
    m_RoleCreateNameEdt.SetOffset( CPoint ( 4, 6 ) ) ;
    // Init the button
    m_RoleCreateOkBtn.Init( 0, 0, "Button30" );
    m_RoleCreateExitBtn.Init( 0, 0, "Button31" );
    m_BtnReset.Init(0, 0, "Button32");
    m_BtnRight1.Init( 0, 0, "Button61" ) ;
    m_BtnLeft1.Init( 0, 0, "Button60" ) ;
    m_BtnChgFace.Init(0, 0, "Button33");
    m_BtnRight2.Init( 0, 0, "Button61" ) ;
    m_BtnLeft2.Init( 0, 0, "Button60" ) ;
    m_BtnChgBody.Init(0, 0, "Button34");
    m_BtnRight3.Init( 0, 0, "Button61" ) ;
    m_BtnLeft3.Init( 0, 0, "Button60" ) ;
    // Init the statics
    m_RoleCreateDataSta.Init(0, 0, 2);
    m_ImgFace.Init(0, 0, NULL, FACE_IMAGE);
    m_StaSex.Init(0, 0);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRoleCreate::ShowRoleInfo()
{
    CPoint mousePoint;
    ::GetCursorPos(&mousePoint);
    CRect rect;
    m_Img1.GetWindowRect(rect);
    if (rect.PtInRect(mousePoint))
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Image24",
                        EXIGENCE_IMMEDIATE ) ;
        if (ShowAni == NULL)
        {
            return;
        }
        else
        {
            ShowAni->Show(0, 303, 390);
            return;
        }
    }
    m_Img2.GetWindowRect(rect);
    if (rect.PtInRect(mousePoint))
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Image25",
                        EXIGENCE_IMMEDIATE ) ;
        if (ShowAni == NULL)
        {
            return;
        }
        else
        {
            ShowAni->Show(0, 488, 364);
            return;
        }
    }
    m_Img3.GetWindowRect(rect);
    if (rect.PtInRect(mousePoint))
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Image26",
                        EXIGENCE_IMMEDIATE ) ;
        if (ShowAni == NULL)
        {
            return;
        }
        else
        {
            ShowAni->Show(0, 666, 391);
            return;
        }
    }
}

void CDlgRoleCreate::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Image3",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( 0, 0, 0 );
        }
        else
        {
            return ;
        }
        // Show the simple role
        CMyPos posA = { 330, 650 } ;
        CMyPos posB = { 510, 630 } ;
        CMyPos posC = { 690, 650 } ;
        if (m_nRoleSelIndex != 0 && m_bSelRoleOk != TRUE)
        {
            ProcessRoleShow(m_bSelected);
        }
        switch(m_nRoleSelIndex)
        {
        case 0:
            if (!m_Img1.IsWindowEnabled())
            {
                m_Img1.EnableWindow();
            }
            if (!m_Img2.IsWindowEnabled())
            {
                m_Img2.EnableWindow();
            }
            if (!m_Img3.IsWindowEnabled())
            {
                m_Img3.EnableWindow();
            }
            m_ShowRoleA.ShowRole( _MALE_ROLE_A, posA, 120, 0, 1 ) ;
            m_ShowRoleB.ShowRole( _MALE_ROLE_B, posB, 120, 0, 1 ) ;
            m_ShowRoleC.ShowRole( _MALE_ROLE_C, posC, 120, 0, 1 ) ;
            ShowRoleInfo();
            break;
        case 1:
            {
                if (m_bSelected)
                {
                    if (m_ShowRoleA.ShowRole(_MALE_ROLE_A, m_RolePos, (int)m_RoleZoom, 0, 3))
                    {
                        ReInit();
                    }
                }
                else
                {
                    int nIndex = 2;
                    if (m_btSex == 2)
                    {
                        nIndex = 1;
                    }
                    BOOL bShowOk = m_ShowRoleA.ShowRole ( _MALE_ROLE_A + (m_btSex - 1) * 3, m_RolePos, (int)m_RoleZoom, 0, nIndex );
                    if (bShowOk && !m_bSelRoleOk)
                    {
                        m_bSelRoleOk = true;
                        EnableCreate(true);
                    }
                }
            }
            break;
        case 2:
            {
                if (m_bSelected)
                {
                    if (m_ShowRoleB.ShowRole(_MALE_ROLE_B, m_RolePos, (int)m_RoleZoom, 0, 3))
                    {
                        ReInit();
                    }
                }
                else
                {
                    int nIndex = 2;
                    if (m_btSex == 2)
                    {
                        nIndex = 1;
                    }
                    BOOL bShowOk = m_ShowRoleB.ShowRole ( _MALE_ROLE_B + (m_btSex - 1) * 3, m_RolePos,(int) m_RoleZoom, 0, nIndex );
                    if (bShowOk && !m_bSelRoleOk)
                    {
                        m_bSelRoleOk = true;
                        EnableCreate(true);
                    }
                }
            }
            break;
        case 3:
            {
                if (m_bSelected)
                {
                    if (m_ShowRoleC.ShowRole(_MALE_ROLE_C, m_RolePos, (int)m_RoleZoom, 0, 3))
                    {
                        ReInit();
                    }
                }
                else
                {
                    int nIndex = 2;
                    if (m_btSex == 2)
                    {
                        nIndex = 1;
                    }
                    BOOL bShowOk = m_ShowRoleC.ShowRole ( _MALE_ROLE_C + (m_btSex - 1) * 3, m_RolePos, (int)m_RoleZoom, 0, nIndex );
                    if (bShowOk && !m_bSelRoleOk)
                    {
                        m_bSelRoleOk = true;
                        EnableCreate(true);
                    }
                }
            }
            break;
        default:
            break;
        }
        if (m_nRoleSelIndex != 0 && m_bSelRoleOk)
        {
            m_ImgRoleSel.Show(0, 0);
            m_ImgFaceSelBk.Show(0, 0);
            m_ImgDataBk.Show(0, 0);
            // Show the edit
            m_RoleCreateNameEdt.Show ( -1, -3 ) ;
            // Show the button
            m_BtnRight1.Show ( 0, 0 ) ;
            m_BtnLeft1.Show ( 0, 0 ) ;
            m_BtnChgFace.Show(0, 0);
            m_ImgFace.Show(0, 0);
            m_BtnRight2.Show ( 0, 0 ) ;
            m_BtnLeft2.Show ( 0, 0 ) ;
            m_BtnChgBody.Show(0, 0);
            m_BtnRight3.Show ( 0, 0 ) ;
            m_BtnLeft3.Show ( 0, 0 ) ;
            m_RoleCreateOkBtn.Show ( 0, 0 ) ;
            m_RoleCreateExitBtn.Show ( 0, 0 ) ;
            m_BtnReset.Show(0, 0);
            // Show the statics
            m_RoleCreateDataSta.Show(0, 0);
            m_StaSex.Show(0, 0);
        }
    }
}
void CDlgRoleCreate::OnRolecreateBtnOk()
{
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    if ( m_strAccount[0] == 0 )
    {
        CDlgRoleCreate::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSG_WARNING, 10035 ) ;
        return;
    }
    if ( m_strPassword[0] == 0 )
    {
        CDlgRoleCreate::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSG_WARNING, 10036 ) ;
        return;
    }
    m_RoleCreateNameEdt.GetWindowText ( m_strRoleName, 16 ) ;
    if (!StringCheck(m_strRoleName))
    {
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100500));
        return;
    }
    if ( m_strRoleName[0] == 0 || strlen ( m_strRoleName ) < 4 )
    {
        CDlgRoleCreate::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSG_WARNING, 10037 ) ;
        return;
    }
    CDlgRoleCreate::GetParent()->PostMessage( WM_MY_MESSAGE, ON_ROLECREATE_CREATE ) ;
}

void CDlgRoleCreate::SetAcnAndPsw(const char* strAccount, const char* strPassword)
{
    strcpy ( m_strAccount, strAccount ) ;
    strcpy ( m_strPassword, strPassword ) ;
}


void CDlgRoleCreate::ReInit()
{
    m_ShowRoleA.Reset();
    m_ShowRoleB.Reset();
    m_ShowRoleC.Reset();
    m_RoleZoom = 120;
    m_RolePos.x = 0;
    m_RolePos.y = 0;
    m_bSelected = false;
    EnableCreate(false);
    m_nRoleSelIndex = 0;
    m_nLookIndex = 0;
    m_btSex = 1;
    m_StaSex.SetWindowText((char*)g_objGameDataSet.GetStr(10031));
    m_ImgFace.RemoveImage();
    m_ImgFace.InsertImage(NULL, m_nLookIndex , m_btSex , true);
    SetRoleFace();
    m_bSelRoleOk = false;
}

void CDlgRoleCreate::OnRolecreateBtnExit()
{
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    CreateExit();
}

void CDlgRoleCreate::OnRolechooseMimg1()
{
    m_Img1.EnableWindow(false);
    m_Img2.EnableWindow(false);
    m_Img3.EnableWindow(false);
    m_nRoleSelIndex = 1;
    m_RoleCreateDataSta.SetWindowText((char*)g_objGameDataSet.GetStr(10383));	//职业介绍-魔法师
    m_RolePos.x = 330;
    m_RolePos.y = 650;
    SetRoleFace();
}

void CDlgRoleCreate::OnRolechooseMimg2()
{
    m_Img1.EnableWindow(false);
    m_Img2.EnableWindow(false);
    m_Img3.EnableWindow(false);
    m_nRoleSelIndex = 2;
    m_RoleCreateDataSta.SetWindowText((char*)g_objGameDataSet.GetStr(10384));	//职业介绍-战士
    m_RolePos.x = 510;
    m_RolePos.y = 630;
    SetRoleFace();
    CDlgRoleCreate::SetFocus();
}


void CDlgRoleCreate::CreateRole()
{
    extern UINT g_uStatus ;
    g_uStatus	= _STATUS_REGISTER ;
    switch(m_nRoleSelIndex)
    {
    case 0:
        return;
        break;
    case 1:
        m_btPro = PROFESSIONAL_ENCHANTER; //法师 10
        break;
    case 2:
        m_btPro = PROFESSIONAL_WARRIOR;	// 战士 =	20
        break;
    case 3:
        m_btPro = PROFESSIONAL_KORADJI; // 异能者
        break;
    default:
        break;
    }
    DWORD dwRealLook;
    dwRealLook = (m_nLookIndex + 1) * 10000 + m_btSex; // male = 1,female = 2	m_cLength = 1;
    m_cFat	  = 1;
	m_cLength = 1;
    CMsgRegister msg;
    if ( msg.Create( m_strAccount,
                     m_strRoleName,
                     m_strPassword,
                     dwRealLook,
                     m_btPro,
                     g_objHero.m_idAccount,
                     m_cLength,
                     m_cFat))
    {
        msg.Send();
    }
    DXPlaySound ( "Sound/Role_CreateO.wav" ) ;
    CDlgRoleCreate::EnableWindow(false);
    m_RoleCreateNameEdt.SetWindowText ( NULL ) ;
}

void CDlgRoleCreate::CreateReset()
{
    m_btSex = 1;
    m_StaSex.SetWindowText((char*)g_objGameDataSet.GetStr(10031));
    m_nLookIndex = 0;
    m_ImgFace.RemoveImage();
    m_ImgFace.InsertImage(NULL, m_nLookIndex , m_btSex , true);
    m_bSelected = true;
    SetRoleFace();
    EnableCreate(false);
    CDlgRoleCreate::SetFocus();
}

void CDlgRoleCreate::CreateExit()
{
    CDlgRoleCreate::EnableWindow(false) ;
    CDlgRoleCreate::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ROLECHOOSE_RETURN ) ;
}

void CDlgRoleCreate::OnRolecreateBtnReset()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    CreateReset();
}

void CDlgRoleCreate::OnRolechooseMimg3()
{
    // TODO: Add your control notification handler code here
    this->MessageBox( g_objGameDataSet.GetStr ( 10039 ), g_objGameDataSet.GetStr ( 10040 ) ) ; //这个职业未开放！
    return;
    m_Img1.EnableWindow(false);
    m_Img2.EnableWindow(false);
    m_Img3.EnableWindow(false);
    m_nRoleSelIndex = 3;
    m_RoleCreateDataSta.SetWindowText((char*)g_objGameDataSet.GetStr(10385));	//职业介绍-异能者
    SetRoleFace();
    m_RolePos.x = 690;
    m_RolePos.y = 650;
    CDlgRoleCreate::SetFocus();
}

void CDlgRoleCreate::EnableCreate(BOOL bFlag)
{
    // Init the button
    m_RoleCreateOkBtn.EnableWindow(bFlag);
    m_RoleCreateExitBtn.EnableWindow(bFlag);
    m_BtnReset.EnableWindow(bFlag);
    m_BtnRight1.EnableWindow(bFlag);
    m_BtnLeft1.EnableWindow(bFlag) ;
    m_BtnChgFace.EnableWindow(bFlag);
    m_BtnRight2.EnableWindow(bFlag) ;
    m_BtnLeft2.EnableWindow(bFlag) ;
    m_BtnChgBody.EnableWindow(bFlag);
    m_BtnRight3.EnableWindow(bFlag) ;
    m_BtnLeft3.EnableWindow(bFlag) ;
    if (bFlag)
    {
        m_RoleCreateNameEdt.SetFocus();
    }
    else
    {
        CDlgRoleCreate::SetFocus();
    }
}

void CDlgRoleCreate::OnBtnLeft3()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/up_Page.wav");
    if (m_btSex == 2)
    {
        m_btSex = 1;
        m_StaSex.SetWindowText((char*)g_objGameDataSet.GetStr(10031));
    }
    else
    {
        m_btSex = 2;
        m_StaSex.SetWindowText((char*)g_objGameDataSet.GetStr(10030));
    }
    m_nLookIndex = 0;
    SetRoleFace();
    m_ImgFace.RemoveImage();
    m_ImgFace.InsertImage(NULL, m_nLookIndex , m_btSex , true);
}

void CDlgRoleCreate::OnBtnRight3()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/up_Page.wav");
    if (m_btSex == 2)
    {
        m_btSex = 1;
        m_StaSex.SetWindowText((char*)g_objGameDataSet.GetStr(10031));
    }
    else
    {
        m_btSex = 2;
        m_StaSex.SetWindowText((char*)g_objGameDataSet.GetStr(10030));
    }
    m_nLookIndex = 0;
    SetRoleFace();
    m_ImgFace.RemoveImage();
    m_ImgFace.InsertImage(NULL, m_nLookIndex , m_btSex , true);
}

void CDlgRoleCreate::ProcessRoleShow(BOOL bBack)
{
    if (!bBack)
    {
        switch(m_nRoleSelIndex)
        {
        case 1:
            if (m_RoleZoom <= 150)
            {
                m_RoleZoom++;
            }
            if (m_RolePos.x <= 510)
            {
                m_RolePos.x += 2;
            }
            break;
        case 2:
            if (m_RoleZoom <= 150)
            {
                m_RoleZoom++;
            }
            break;
        case 3:
            if (m_RoleZoom <= 150)
            {
                m_RoleZoom++;
            }
            if (m_RolePos.x >= 510)
            {
                m_RolePos.x -= 2;
            }
            break;
        default:
            break;
        }
    }
}

void CDlgRoleCreate::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgRoleCreate::OnBtnChgbody()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
}

void CDlgRoleCreate::OnBtnChgface()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
}

void CDlgRoleCreate::OnBtnLeft1()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/up_Page.wav");
    int nAmount = g_obj3DRoleData.GetMaleFaceAmount();
    if (m_btSex == 2)
    {
        nAmount = g_obj3DRoleData.GetFemaleFaceAmount();
    }
    if (m_nLookIndex > 0)
    {
        m_nLookIndex--;
    }
    else
    {
        m_nLookIndex = nAmount - 1;
    }
    m_ImgFace.RemoveImage();
    m_ImgFace.InsertImage(NULL, m_nLookIndex , m_btSex , true);
    SetRoleFace();
}

void CDlgRoleCreate::OnBtnRight1()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/up_Page.wav");
    int nAmount = g_obj3DRoleData.GetMaleFaceAmount();
    if (m_btSex == 2)
    {
        nAmount = g_obj3DRoleData.GetFemaleFaceAmount();
    }
    if (m_nLookIndex < nAmount - 1)
    {
        m_nLookIndex++;
    }
    else
    {
        m_nLookIndex = 0;
    }
    m_ImgFace.RemoveImage();
    m_ImgFace.InsertImage(NULL, m_nLookIndex , m_btSex , true);
    SetRoleFace();
}


void CDlgRoleCreate::SetRoleFace()
{
    switch(m_nRoleSelIndex)
    {
    case 0:
        m_ShowRoleA.SetLook(m_btSex);
        m_ShowRoleA.SetHead(m_nLookIndex);
        m_ShowRoleB.SetLook(m_btSex);
        m_ShowRoleB.SetHead(m_nLookIndex);
        m_ShowRoleC.SetLook(m_btSex);
        m_ShowRoleC.SetHead(m_nLookIndex);
        break;
    case 1:
        m_ShowRoleA.SetLook(m_btSex);
        m_ShowRoleA.SetHead(m_nLookIndex);
        break;
    case 2:
        m_ShowRoleB.SetLook(m_btSex);
        m_ShowRoleB.SetHead(m_nLookIndex);
        break;
    case 3:
        m_ShowRoleC.SetLook(m_btSex);
        m_ShowRoleC.SetHead(m_nLookIndex);
        break;
    default:
        break;
    }
}
