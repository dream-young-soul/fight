
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgSkill.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgSkill.h"

#include "Hero.h"
#include "GameMsg.h"
#include "Magic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int	AMOUNT_PET_PAGE = 6 ;
/////////////////////////////////////////////////////////////////////////////
// CDlgSkill dialog


CDlgSkill::CDlgSkill(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgSkill::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgSkill)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_fFrameStep = 0.0f ;
    m_nFrameMode = 0 ;
    m_nPntX = 0 ;
    m_nCurIndex = 0 ;
    m_nTMagicIndex = 0;
    m_nSkillListMode = 0;
    //}}AFX_DATA_INIT
}


void CDlgSkill::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSkill)
    DDX_Control(pDX, IDC_SKILLKEY_IMG1, m_SkillKey1Img);
    DDX_Control(pDX, IDC_SKILLKEY_IMG2, m_SkillKey2Img);
    DDX_Control(pDX, IDC_SKILLKEY_IMG3, m_SkillKey3Img);
    DDX_Control(pDX, IDC_SKILLKEY_IMG4, m_SkillKey4Img);
    DDX_Control(pDX, IDC_SKILLKEY_IMG5, m_SkillKey5Img);
    DDX_Control(pDX, IDC_SKILLKEY_IMG6, m_SkillKey6Img);
    DDX_Control(pDX, IDC_STA_NAME6, m_StaName6);
    DDX_Control(pDX, IDC_STA_NAME5, m_StaName5);
    DDX_Control(pDX, IDC_STA_NAME4, m_StaName4);
    DDX_Control(pDX, IDC_STA_NAME3, m_StaName3);
    DDX_Control(pDX, IDC_STA_NAME2, m_StaName2);
    DDX_Control(pDX, IDC_STA_NAME1, m_StaName1);
    DDX_Control(pDX, IDC_STA_EXP6, m_StaExp6);
    DDX_Control(pDX, IDC_STA_EXP5, m_StaExp5);
    DDX_Control(pDX, IDC_STA_EXP4, m_StaExp4);
    DDX_Control(pDX, IDC_STA_EXP3, m_StaExp3);
    DDX_Control(pDX, IDC_STA_EXP2, m_StaExp2);
    DDX_Control(pDX, IDC_STA_EXP1, m_StaExp1);
    DDX_Control(pDX, IDC_STA_DESC6, m_StaDesc6);
    DDX_Control(pDX, IDC_STA_DESC5, m_StaDesc5);
    DDX_Control(pDX, IDC_STA_DESC4, m_StaDesc4);
    DDX_Control(pDX, IDC_STA_DESC3, m_StaDesc3);
    DDX_Control(pDX, IDC_STA_DESC2, m_StaDesc2);
    DDX_Control(pDX, IDC_STA_DESC1, m_StaDesc1);
    DDX_Control(pDX, IDC_SKILL_IMG_6, m_Img6);
    DDX_Control(pDX, IDC_SKILL_IMG_1, m_Img1);
    DDX_Control(pDX, IDC_SKILL_IMG_2, m_Img2);
    DDX_Control(pDX, IDC_SKILL_IMG_3, m_Img3);
    DDX_Control(pDX, IDC_SKILL_IMG_4, m_Img4);
    DDX_Control(pDX, IDC_SKILL_IMG_5, m_Img5);
    DDX_Control(pDX, IDC_SKILL_BTN_UP, m_BtnUp);
    DDX_Control(pDX, IDC_SKILL_BTN_DOWN, m_BtnDown);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSkill, CDialog)
    //{{AFX_MSG_MAP(CDlgSkill)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_SKILL_BTN_DOWN, OnSkillBtnDown)
    ON_BN_CLICKED(IDC_SKILL_IMG_1, OnSkillImg1)
    ON_BN_CLICKED(IDC_SKILL_IMG_2, OnSkillImg2)
    ON_BN_CLICKED(IDC_SKILL_IMG_3, OnSkillImg3)
    ON_BN_CLICKED(IDC_SKILL_IMG_4, OnSkillImg4)
    ON_BN_CLICKED(IDC_SKILL_IMG_5, OnSkillImg5)
    ON_BN_CLICKED(IDC_SKILL_IMG_6, OnSkillImg6)
    ON_BN_CLICKED(IDC_SKILL_BTN_UP, OnSkillBtnUp)
    ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
    ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
    ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
    ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
    ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
    ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSkill message handlers

HBRUSH CDlgSkill::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgSkill::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgSkill::GetWindowRect( rect ) ;
    m_fFrameStep = ( float ) rect.Width () / DLG_FRAME_AMOUNT ;
    // Init the image
    m_Img1.Init( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_Img2.Init( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_Img3.Init( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_Img4.Init( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_Img5.Init( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_Img6.Init( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    // Init the static
    m_StaName6.Init( rect.left, rect.top ) ;
    m_StaName5.Init( rect.left, rect.top ) ;
    m_StaName4.Init( rect.left, rect.top ) ;
    m_StaName3.Init( rect.left, rect.top ) ;
    m_StaName2.Init( rect.left, rect.top ) ;
    m_StaName1.Init( rect.left, rect.top ) ;
    m_StaExp6.Init( rect.left, rect.top ) ;
    m_StaExp5.Init( rect.left, rect.top ) ;
    m_StaExp4.Init( rect.left, rect.top ) ;
    m_StaExp3.Init( rect.left, rect.top ) ;
    m_StaExp2.Init( rect.left, rect.top ) ;
    m_StaExp1.Init( rect.left, rect.top ) ;
    m_StaDesc6.Init( rect.left, rect.top ) ;
    m_StaDesc5.Init( rect.left, rect.top ) ;
    m_StaDesc4.Init( rect.left, rect.top ) ;
    m_StaDesc3.Init( rect.left, rect.top ) ;
    m_StaDesc2.Init( rect.left, rect.top ) ;
    m_StaDesc1.Init( rect.left, rect.top ) ;
    // Init the buttons
    m_SkillKey1Img.Init(rect.left, rect.top, NULL, BG_IMAGE);
    m_SkillKey2Img.Init(rect.left, rect.top, NULL, BG_IMAGE);
    m_SkillKey3Img.Init(rect.left, rect.top, NULL, BG_IMAGE);
    m_SkillKey4Img.Init(rect.left, rect.top, NULL, BG_IMAGE);
    m_SkillKey5Img.Init(rect.left, rect.top, NULL, BG_IMAGE);
    m_SkillKey6Img.Init(rect.left, rect.top, NULL, BG_IMAGE);
    m_BtnUp.Init( rect.left, rect.top, "Button815" ) ;
    m_BtnDown.Init( rect.left, rect.top, "Button816" ) ;
    //Init the HotKey Dialog
    m_DlgSkillHotKey.Create ( IDD_DIALOG_SKILL_HOTKEYS, this->GetParent()->GetParent() );
    m_DlgSkillHotKey.SetParentWnd( this );
    CRect MRect;
    m_DlgSkillHotKey.GetWindowRect(MRect);
    m_DlgSkillHotKey.MoveWindow ( _SCR_WIDTH - rect.Width() - 100 - MRect.Width(),
                                  500 - rect.Height(),
                                  MRect.Width(),
                                  MRect.Height());
    m_DlgSkillHotKey.ShowWindow( SW_SHOW ) ;
    m_DlgSkillHotKey.EnableWindow( false ) ;
    CRect BRect, ERect;
    m_Img1.GetWindowRect(BRect);
    m_SkillKey1Img.GetWindowRect(ERect);
    m_PrgRect[0] = CRect(m_Pnt.x + BRect.left,
                         m_Pnt.y + BRect.top, m_Pnt.x + ERect.right, m_Pnt.y + ERect.bottom);
    m_Img2.GetWindowRect(BRect);
    m_SkillKey2Img.GetWindowRect(ERect);
    m_PrgRect[1] = CRect(BRect.left, BRect.top, ERect.right, ERect.bottom);
    m_Img3.GetWindowRect(BRect);
    m_SkillKey3Img.GetWindowRect(ERect);
    m_PrgRect[2] = CRect(BRect.left, BRect.top, ERect.right, ERect.bottom);
    m_Img4.GetWindowRect(BRect);
    m_SkillKey4Img.GetWindowRect(ERect);
    m_PrgRect[3] = CRect(BRect.left, BRect.top, ERect.right, ERect.bottom);
    m_Img5.GetWindowRect(BRect);
    m_SkillKey5Img.GetWindowRect(ERect);
    m_PrgRect[4] = CRect(BRect.left, BRect.top, ERect.right, ERect.bottom);
    m_Img6.GetWindowRect(BRect);
    m_SkillKey6Img.GetWindowRect(ERect);
    m_PrgRect[5] = CRect(BRect.left, BRect.top, ERect.right, ERect.bottom);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSkill::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_nPntX = x ;
    m_Pnt.y = y ;
}

void CDlgSkill::Show(int x, int y)
{
    if ( m_bShow )
    {
        // Show the images
        m_Img1.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_Img2.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_Img3.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_Img4.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_Img5.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_Img6.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        // Show the static
        m_StaName6.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaName5.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaName4.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaName3.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaName2.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaName1.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaExp6.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaExp5.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaExp4.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaExp3.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaExp2.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaExp1.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaDesc6.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaDesc5.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaDesc4.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaDesc3.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaDesc2.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaDesc1.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        // Show the buttons
        m_SkillKey1Img.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_SkillKey2Img.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_SkillKey3Img.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_SkillKey4Img.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_SkillKey5Img.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_SkillKey6Img.Show(m_Pnt.x + x, m_Pnt.y + y);
        if (m_BtnUp.IsWindowEnabled())
        {
            m_BtnUp.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        }
        if (m_BtnDown.IsWindowEnabled())
        {
            m_BtnDown.Show( m_Pnt.x + x, m_Pnt.y + y ) ;
        }
        if (m_DlgSkillHotKey.m_bShow)
        {
            m_DlgSkillHotKey.Show();
        }
    }
}

LRESULT CDlgSkill::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if ( wParam == false )
        {
            if (m_DlgSkillHotKey.m_bShow)
            {
                m_DlgSkillHotKey.EnableWindow(false);
            }
        }
        else
        {
            if (m_nSkillListMode == 1)
            {
                CDlgSkill::SetTeamMagicSkill();
            }
            else
            {
                CDlgSkill::SetMagicSkill();
            }
            m_dwLastShowTime = TimeGet() ;
            m_nFrameMode = 1 ;
            CRect rect ;
            CDlgSkill::GetWindowRect( rect ) ;
            m_nPntX = _SCR_WIDTH ;
            //			CDlgSkill::GetParent()->PostMessage( WM_MY_MESSAGE, ON_RSIDEDLG_OPEN ) ;
        }
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 )
        {
            return true ;
        }
        else if (wParam == 2)
        {
            return true;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgSkill::SetMagicSkill()
{
    DWORD dwFalseColor = 0xc0c0c0;
    DWORD dwBaseColor = 0xffffff;
    CDlgSkill::ClearMagicSkill() ;
    m_nSkillListMode = 0;
    int nAmount = ( g_objHero.GetMagicAmount() > m_nCurIndex + AMOUNT_PET_PAGE ) ? m_nCurIndex + AMOUNT_PET_PAGE : g_objHero.GetMagicAmount();
    if (m_nCurIndex <= 0)
    {
        m_BtnUp.EnableWindow(false);
    }
    else
    {
        m_BtnUp.EnableWindow();
    }
    if (m_nCurIndex + AMOUNT_PET_PAGE >= g_objHero.GetMagicAmount())
    {
        m_BtnDown.EnableWindow(false);
    }
    else
    {
        m_BtnDown.EnableWindow();
    }
    for ( int i = m_nCurIndex; i < nAmount; i++ )
    {
        CMagic* pMagic = g_objHero.GetMagicByIndex(i) ;
        if ( pMagic != NULL && pMagic->m_infoMagicType.dwActionSort != MAGICSORT_TEAM_MAGIC)
        {
            char SkillType[32] ;
            sprintf ( SkillType, "MagicSkillType%d", pMagic->GetIDType() ) ;
            int j;
            char strExp[64] ;
            sprintf( strExp, g_objGameDataSet.GetStr(10137), pMagic->GetLevel() + 1, pMagic->GetExp(), pMagic->GetNextLevelExp() ) ;
            switch ( i - m_nCurIndex )
            {
            case 0:
                m_StaName1.SetFontColor(dwBaseColor) ;
                m_StaDesc1.SetFontColor(dwBaseColor) ;
                m_Img1.InsertImage( SkillType, 0 , pMagic->GetIDType(), false ) ;
                m_StaName1.SetWindowText( pMagic->GetName() ) ;
                m_StaDesc1.SetWindowText( strExp ) ;
                for(j = 0; j < 8; j++)
                {
                    if (pMagic->GetIDType() == m_DlgSkillHotKey.m_HotKeyIdType[j])
                    {
                        sprintf(strExp, "Check%d", 601 + j);
                        m_SkillKey1Img.InsertImage(strExp);
                        break;
                    }
                }
                if (j == 8)
                {
                    m_SkillKey1Img.InsertImage("Check609");
                }
                if (pMagic->TestTarget(MAGIC_PASSIVE))
                {
                    m_SkillKey1Img.RemoveImage();
                    m_StaName1.SetFontColor(dwFalseColor) ;
                    m_StaDesc1.SetFontColor(dwFalseColor) ;
                }
                else if (pMagic->m_infoMagicType.dwXp == TYPE_XPSKILL)
                {
                    m_SkillKey1Img.RemoveImage();
                }
                break ;
            case 1:
                m_StaName2.SetFontColor(dwBaseColor) ;
                m_StaDesc2.SetFontColor(dwBaseColor) ;
                m_Img2.InsertImage( SkillType, 0, pMagic->GetIDType(), false ) ;
                m_StaName2.SetWindowText( pMagic->GetName() ) ;
                m_StaDesc2.SetWindowText( strExp ) ;
                for(j = 0; j < 8; j++)
                {
                    if (pMagic->GetIDType() == m_DlgSkillHotKey.m_HotKeyIdType[j])
                    {
                        sprintf(strExp, "Check%d", 601 + j);
                        m_SkillKey2Img.InsertImage(strExp);
                        break;
                    }
                }
                if (j == 8)
                {
                    m_SkillKey2Img.InsertImage("Check609");
                }
                if (pMagic->TestTarget(MAGIC_PASSIVE))
                {
                    m_SkillKey2Img.RemoveImage();
                    m_StaName2.SetFontColor(dwFalseColor) ;
                    m_StaDesc2.SetFontColor(dwFalseColor) ;
                }
                else if (pMagic->m_infoMagicType.dwXp == TYPE_XPSKILL)
                {
                    m_SkillKey2Img.RemoveImage();
                }
                break ;
            case 2:
                m_StaName3.SetFontColor(dwBaseColor) ;
                m_StaDesc3.SetFontColor(dwBaseColor) ;
                m_Img3.InsertImage( SkillType, 0, pMagic->GetIDType(), false ) ;
                m_StaName3.SetWindowText( pMagic->GetName() ) ;
                m_StaDesc3.SetWindowText( strExp ) ;
                for(j = 0; j < 8; j++)
                {
                    if (pMagic->GetIDType() == m_DlgSkillHotKey.m_HotKeyIdType[j])
                    {
                        sprintf(strExp, "Check%d", 601 + j);
                        m_SkillKey3Img.InsertImage(strExp);
                        break;
                    }
                }
                if (j == 8)
                {
                    m_SkillKey3Img.InsertImage("Check609");
                }
                if (pMagic->TestTarget(MAGIC_PASSIVE))
                {
                    m_StaName3.SetFontColor(dwFalseColor) ;
                    m_StaDesc3.SetFontColor(dwFalseColor) ;
                    m_SkillKey3Img.RemoveImage();
                }
                else if (pMagic->m_infoMagicType.dwXp == TYPE_XPSKILL)
                {
                    m_SkillKey3Img.RemoveImage();
                }
                break ;
            case 3:
                m_StaName4.SetFontColor(dwBaseColor) ;
                m_StaDesc4.SetFontColor(dwBaseColor) ;
                m_Img4.InsertImage( SkillType, 0, pMagic->GetIDType(), false ) ;
                m_StaName4.SetWindowText( pMagic->GetName() ) ;
                m_StaDesc4.SetWindowText( strExp ) ;
                for(j = 0; j < 8; j++)
                {
                    if (pMagic->GetIDType() == m_DlgSkillHotKey.m_HotKeyIdType[j])
                    {
                        sprintf(strExp, "Check%d", 601 + j);
                        m_SkillKey4Img.InsertImage(strExp);
                        break;
                    }
                }
                if (j == 8)
                {
                    m_SkillKey4Img.InsertImage("Check609");
                }
                if (pMagic->TestTarget(MAGIC_PASSIVE))
                {
                    m_SkillKey4Img.RemoveImage();
                    m_StaName4.SetFontColor(dwFalseColor) ;
                    m_StaDesc4.SetFontColor(dwFalseColor) ;
                }
                else if (pMagic->m_infoMagicType.dwXp == TYPE_XPSKILL)
                {
                    m_SkillKey4Img.RemoveImage();
                }
                break ;
            case 4:
                m_StaName5.SetFontColor(dwBaseColor) ;
                m_StaDesc5.SetFontColor(dwBaseColor) ;
                m_Img5.InsertImage( SkillType, 0, pMagic->GetIDType(), false ) ;
                m_StaName5.SetWindowText( pMagic->GetName() ) ;
                m_StaDesc5.SetWindowText( strExp ) ;
                for(j = 0; j < 8; j++)
                {
                    if (pMagic->GetIDType() == m_DlgSkillHotKey.m_HotKeyIdType[j])
                    {
                        sprintf(strExp, "Check%d", 601 + j);
                        m_SkillKey5Img.InsertImage(strExp);
                        break;
                    }
                }
                if (j == 8)
                {
                    m_SkillKey5Img.InsertImage("Check609");
                }
                if (pMagic->TestTarget(MAGIC_PASSIVE))
                {
                    m_SkillKey5Img.RemoveImage();
                    m_StaName5.SetFontColor(dwFalseColor) ;
                    m_StaDesc5.SetFontColor(dwFalseColor) ;
                }
                else if (pMagic->m_infoMagicType.dwXp == TYPE_XPSKILL)
                {
                    m_SkillKey5Img.RemoveImage();
                }
                break ;
            case 5:
                m_StaName6.SetFontColor(dwBaseColor) ;
                m_StaDesc6.SetFontColor(dwBaseColor) ;
                m_Img6.InsertImage( SkillType, 0, pMagic->GetIDType(), false ) ;
                m_StaName6.SetWindowText( pMagic->GetName() ) ;
                m_StaDesc6.SetWindowText( strExp ) ;
                for(j = 0; j < 8; j++)
                {
                    if (pMagic->GetIDType() == m_DlgSkillHotKey.m_HotKeyIdType[j])
                    {
                        sprintf(strExp, "Check%d", 601 + j);
                        m_SkillKey6Img.InsertImage(strExp);
                        break;
                    }
                }
                if (j == 8)
                {
                    m_SkillKey6Img.InsertImage("Check609");
                }
                if (pMagic->TestTarget(MAGIC_PASSIVE))
                {
                    m_SkillKey6Img.RemoveImage();
                    m_StaName6.SetFontColor(dwFalseColor) ;
                    m_StaDesc6.SetFontColor(dwFalseColor) ;
                }
                else if (pMagic->m_infoMagicType.dwXp == TYPE_XPSKILL)
                {
                    m_SkillKey6Img.RemoveImage();
                }
                break ;
            }
        }
    }
}

void CDlgSkill::OnSkillBtnDown()
{
    DXPlaySound("Sound/up_Page.wav");
    if (m_nSkillListMode == 0 && g_objHero.GetMagicAmount() > m_nCurIndex + AMOUNT_PET_PAGE )
    {
        m_nCurIndex += AMOUNT_PET_PAGE ;
        CDlgSkill::SetMagicSkill() ;
    }
    else if (m_nSkillListMode == 1 && g_objHero.GetTeamMagicAmount() > m_nTMagicIndex + AMOUNT_PET_PAGE)
    {
        m_nTMagicIndex += AMOUNT_PET_PAGE;
        CDlgSkill::SetTeamMagicSkill();
    }
}

void CDlgSkill::ClearMagicSkill()
{
    m_Img1.RemoveImage() ;
    m_Img2.RemoveImage() ;
    m_Img3.RemoveImage() ;
    m_Img4.RemoveImage() ;
    m_Img5.RemoveImage() ;
    m_Img6.RemoveImage() ;
    m_SkillKey1Img.RemoveImage() ;
    m_SkillKey2Img.RemoveImage() ;
    m_SkillKey3Img.RemoveImage() ;
    m_SkillKey4Img.RemoveImage() ;
    m_SkillKey5Img.RemoveImage() ;
    m_SkillKey6Img.RemoveImage() ;
    m_StaName6.SetWindowText( NULL ) ;
    m_StaName5.SetWindowText( NULL ) ;
    m_StaName4.SetWindowText( NULL ) ;
    m_StaName3.SetWindowText( NULL ) ;
    m_StaName2.SetWindowText( NULL ) ;
    m_StaName1.SetWindowText( NULL ) ;
    m_StaExp6.SetWindowText( NULL ) ;
    m_StaExp5.SetWindowText( NULL ) ;
    m_StaExp4.SetWindowText( NULL ) ;
    m_StaExp3.SetWindowText( NULL ) ;
    m_StaExp2.SetWindowText( NULL ) ;
    m_StaExp1.SetWindowText( NULL ) ;
    m_StaDesc6.SetWindowText( NULL ) ;
    m_StaDesc5.SetWindowText( NULL ) ;
    m_StaDesc4.SetWindowText( NULL ) ;
    m_StaDesc3.SetWindowText( NULL ) ;
    m_StaDesc2.SetWindowText( NULL ) ;
    m_StaDesc1.SetWindowText( NULL ) ;
}

void CDlgSkill::SetMagicHotKeys(int nIndex)
{
    CMagic* pMagic = g_objHero.GetMagicByIndex(nIndex);
    if (pMagic)
    {
        if (pMagic->TestTarget(MAGIC_PASSIVE))
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10341));//被动技能不能绑定快捷键!
            return;
        }
        else if (pMagic->m_infoMagicType.dwXp == TYPE_XPSKILL )
        {
            return;
        }
        if (!m_DlgSkillHotKey.m_bShow)
        {
            m_DlgSkillHotKey.EnableWindow();
        }
        if (m_DlgSkillHotKey.m_bShow)
        {
            m_DlgSkillHotKey.PopCurCheck(m_DlgSkillHotKey.m_iMagicHotKey);
            for(int j = 0; j < 8; j++)
            {
                if (pMagic->GetIDType() == m_DlgSkillHotKey.m_HotKeyIdType[j])
                {
                    m_DlgSkillHotKey.m_iMagicHotKey = j;
                    break;
                }
                else
                {
                    m_DlgSkillHotKey.m_iMagicHotKey = 8;
                }
            }
            m_DlgSkillHotKey.PopCurCheck(m_DlgSkillHotKey.m_iMagicHotKey, true);
            m_DlgSkillHotKey.SetStaMagic(pMagic->GetIDType());
        }
    }
    else
    {
        m_DlgSkillHotKey.EnableWindow(false);
    }
}

void CDlgSkill::OnSkillImg1()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (0 == m_nSkillListMode && m_Img1.GetImageLoadState())
    {
        CDlgSkill::SetMagicHotKeys(m_nCurIndex);
    }
    else if (1 == m_nSkillListMode && m_Img1.GetImageLoadState())
    {
        CDlgSkill::SetTeamMagicHotKeys(m_nTMagicIndex);
    }
}

void CDlgSkill::OnSkillImg2()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (0 == m_nSkillListMode && m_Img2.GetImageLoadState())
    {
        CDlgSkill::SetMagicHotKeys(m_nCurIndex + 1);
    }
    else if (1 == m_nSkillListMode && m_Img2.GetImageLoadState())
    {
        CDlgSkill::SetTeamMagicHotKeys(m_nTMagicIndex + 1);
    }
}

void CDlgSkill::OnSkillImg3()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (0 == m_nSkillListMode && m_Img3.GetImageLoadState())
    {
        CDlgSkill::SetMagicHotKeys(m_nCurIndex + 2);
    }
    else if (1 == m_nSkillListMode && m_Img3.GetImageLoadState())
    {
        CDlgSkill::SetTeamMagicHotKeys(m_nTMagicIndex + 2);
    }
}

void CDlgSkill::OnSkillImg4()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (0 == m_nSkillListMode && m_Img4.GetImageLoadState())
    {
        CDlgSkill::SetMagicHotKeys(m_nCurIndex + 3);
    }
    else if (1 == m_nSkillListMode &&  m_Img4.GetImageLoadState())
    {
        CDlgSkill::SetTeamMagicHotKeys(m_nTMagicIndex + 3);
    }
}

void CDlgSkill::OnSkillImg5()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (0 == m_nSkillListMode &&  m_Img5.GetImageLoadState())
    {
        CDlgSkill::SetMagicHotKeys(m_nCurIndex + 4);
    }
    else if (1 == m_nSkillListMode &&  m_Img5.GetImageLoadState())
    {
        CDlgSkill::SetTeamMagicHotKeys(m_nTMagicIndex + 4);
    }
}

void CDlgSkill::OnSkillImg6()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (0 == m_nSkillListMode && m_Img6.GetImageLoadState())
    {
        CDlgSkill::SetMagicHotKeys(m_nCurIndex + 5);
    }
    else if (1 == m_nSkillListMode && m_Img6.GetImageLoadState())
    {
        CDlgSkill::SetTeamMagicHotKeys(m_nTMagicIndex + 5);
    }
}

void CDlgSkill::OnSkillBtnUp()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/up_Page.wav");
    if (m_nSkillListMode == 0 && m_nCurIndex - AMOUNT_PET_PAGE >= 0 )
    {
        m_nCurIndex -= AMOUNT_PET_PAGE ;
        CDlgSkill::SetMagicSkill() ;
    }
    else if (m_nSkillListMode == 1 && m_nTMagicIndex - AMOUNT_PET_PAGE >= 0)
    {
        m_nTMagicIndex -= AMOUNT_PET_PAGE;
        CDlgSkill::SetTeamMagicSkill();
    }
}

void CDlgSkill::SetTeamMagicSkill()
{
    CDlgSkill::ClearMagicSkill();
    DWORD dwBaseColor = 0xffffff;
    m_StaName1.SetFontColor(dwBaseColor) ;
    m_StaName2.SetFontColor(dwBaseColor) ;
    m_StaName3.SetFontColor(dwBaseColor) ;
    m_StaName4.SetFontColor(dwBaseColor) ;
    m_StaName5.SetFontColor(dwBaseColor) ;
    m_StaName6.SetFontColor(dwBaseColor) ;
    m_StaDesc1.SetFontColor(dwBaseColor) ;
    m_StaDesc2.SetFontColor(dwBaseColor) ;
    m_StaDesc3.SetFontColor(dwBaseColor) ;
    m_StaDesc4.SetFontColor(dwBaseColor) ;
    m_StaDesc5.SetFontColor(dwBaseColor) ;
    m_StaDesc6.SetFontColor(dwBaseColor) ;
    m_nSkillListMode = 1;
    int nAmount = ( g_objHero.GetTeamMagicAmount() > m_nTMagicIndex + AMOUNT_PET_PAGE ) ? m_nTMagicIndex + AMOUNT_PET_PAGE : g_objHero.GetTeamMagicAmount();
    if (m_nTMagicIndex <= 0)
    {
        m_BtnUp.EnableWindow(false);
    }
    else
    {
        m_BtnUp.EnableWindow();
    }
    if (m_nTMagicIndex + AMOUNT_PET_PAGE >= g_objHero.GetTeamMagicAmount())
    {
        m_BtnDown.EnableWindow(false);
    }
    else
    {
        m_BtnDown.EnableWindow();
    }
    for(int i = m_nTMagicIndex; i < nAmount; i++)
    {
        CMagic* pMagic = g_objHero.GetTeamMagicByIndex(i);
        if (pMagic != NULL && pMagic->m_infoMagicType.dwActionSort == MAGICSORT_TEAM_MAGIC)
        {
            char SkillType[32] ;
            sprintf ( SkillType, "MagicSkillType%d", pMagic->GetIDType () ) ;
            char strExp[64] ;
            int j = 0;
            //"等级:%d 经验值:%d/%d "
            sprintf( strExp, g_objGameDataSet.GetStr(10137), pMagic->GetLevel() + 1, pMagic->GetExp(), pMagic->GetNextLevelExp() ) ;
            switch ( i - m_nTMagicIndex )
            {
            case 0:
                m_Img1.InsertImage( SkillType, 0, pMagic->GetIDType()) ;
                m_StaName1.SetWindowText( pMagic->GetName() ) ;
                m_StaDesc1.SetWindowText( strExp ) ;
                if (!g_objHero.IsTeamLeader())
                {
                    break;
                }
                for(j = 0; j < 8; j++)
                {
                    if (pMagic->GetIDType() == m_DlgSkillHotKey.m_HotKeyIdType[j])
                    {
                        sprintf(strExp, "Check%d", 601 + j);
                        m_SkillKey1Img.InsertImage(strExp);
                        break;
                    }
                }
                if (j == 8)
                {
                    m_SkillKey1Img.InsertImage("Check609");
                }
                break ;
            case 1:
                m_Img2.InsertImage( SkillType, 0, pMagic->GetIDType() ) ;
                m_StaName2.SetWindowText( pMagic->GetName() ) ;
                m_StaDesc2.SetWindowText( strExp ) ;
                if (!g_objHero.IsTeamLeader())
                {
                    break;
                }
                for(j = 0; j < 8; j++)
                {
                    if (pMagic->GetIDType() == m_DlgSkillHotKey.m_HotKeyIdType[j])
                    {
                        sprintf(strExp, "Check%d", 601 + j);
                        m_SkillKey2Img.InsertImage(strExp);
                        break;
                    }
                }
                if (j == 8)
                {
                    m_SkillKey2Img.InsertImage("Check609");
                }
                break ;
            case 2:
                m_Img3.InsertImage( SkillType, 0, pMagic->GetIDType()) ;
                m_StaName3.SetWindowText( pMagic->GetName() ) ;
                m_StaDesc3.SetWindowText( strExp ) ;
                if (!g_objHero.IsTeamLeader())
                {
                    break;
                }
                for(j = 0; j < 8; j++)
                {
                    if (pMagic->GetIDType() == m_DlgSkillHotKey.m_HotKeyIdType[j])
                    {
                        sprintf(strExp, "Check%d", 601 + j);
                        m_SkillKey3Img.InsertImage(strExp);
                        break;
                    }
                }
                if (j == 8)
                {
                    m_SkillKey3Img.InsertImage("Check609");
                }
                break ;
            case 3:
                m_Img4.InsertImage( SkillType, 0, pMagic->GetIDType()) ;
                m_StaName4.SetWindowText( pMagic->GetName() ) ;
                m_StaDesc4.SetWindowText( strExp ) ;
                if (!g_objHero.IsTeamLeader())
                {
                    break;
                }
                for(j = 0; j < 8; j++)
                {
                    if (pMagic->GetIDType() == m_DlgSkillHotKey.m_HotKeyIdType[j])
                    {
                        sprintf(strExp, "Check%d", 601 + j);
                        m_SkillKey4Img.InsertImage(strExp);
                        break;
                    }
                }
                if (j == 8)
                {
                    m_SkillKey4Img.InsertImage("Check609");
                }
                break ;
            case 4:
                m_Img5.InsertImage( SkillType, 0, pMagic->GetIDType()) ;
                m_StaName5.SetWindowText( pMagic->GetName() ) ;
                m_StaDesc5.SetWindowText( strExp ) ;
                if (!g_objHero.IsTeamLeader())
                {
                    break;
                }
                for(j = 0; j < 8; j++)
                {
                    if (pMagic->GetIDType() == m_DlgSkillHotKey.m_HotKeyIdType[j])
                    {
                        sprintf(strExp, "Check%d", 601 + j);
                        m_SkillKey5Img.InsertImage(strExp);
                        break;
                    }
                }
                if (j == 8)
                {
                    m_SkillKey5Img.InsertImage("Check609");
                }
                break ;
            case 5:
                m_Img6.InsertImage( SkillType, 0, pMagic->GetIDType()) ;
                m_StaName6.SetWindowText( pMagic->GetName() ) ;
                m_StaDesc6.SetWindowText( strExp ) ;
                if (!g_objHero.IsTeamLeader())
                {
                    break;
                }
                for(j = 0; j < 8; j++)
                {
                    if (pMagic->GetIDType() == m_DlgSkillHotKey.m_HotKeyIdType[j])
                    {
                        sprintf(strExp, "Check%d", 601 + j);
                        m_SkillKey6Img.InsertImage(strExp);
                        break;
                    }
                }
                if (j == 8)
                {
                    m_SkillKey6Img.InsertImage("Check609");
                }
                break ;
            }
        }
    }
}

void CDlgSkill::UseTeamMagic(int nIndex)
{
    /*	if (m_TMagicIdType[nIndex])
    	{
    		CMagic* pMagic = g_objHero.GetMagic(m_TMagicIdType[nIndex]);
    		if (pMagic)
    		{
    			g_objHero.MagicAttack(pMagic->GetIDType(),g_objHero.GetID());
    			CDlgSkill::GetParent()->GetParent()->PostMessage(WM_MY_MESSAGE,ON_TEAMMAGIC_SEL,pMagic->GetIDType());
    		}
    	}*/
}

void CDlgSkill::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgSkill::GetParent()->PostMessage(WM_MOUSEMOVE, nFlags, lParam);
    CDialog::OnMouseMove(nFlags, point);
}

void CDlgSkill::OnButton1()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_ok.wav");
    CDlgSkill::OnSkillImg1();
}

void CDlgSkill::OnButton2()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_ok.wav");
    CDlgSkill::OnSkillImg2();
}

void CDlgSkill::OnButton3()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_ok.wav");
    CDlgSkill::OnSkillImg3();
}

void CDlgSkill::OnButton4()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_ok.wav");
    CDlgSkill::OnSkillImg4();
}

void CDlgSkill::OnButton5()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_ok.wav");
    CDlgSkill::OnSkillImg5();
}

void CDlgSkill::OnButton6()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_ok.wav");
    CDlgSkill::OnSkillImg6();
}

int CDlgSkill::GetMouseInMagicIndex()
{
    if (!CDlgSkill::IsWindowEnabled())
    {
        return 0;
    }
    POINT mousePnt ;
    GetCursorPos ( &mousePnt ) ;
    CRect BRect, ERect;
    m_Img1.GetWindowRect(BRect);
    m_SkillKey1Img.GetWindowRect(ERect);
    m_PrgRect[0] = CRect(BRect.left,
                         BRect.top, ERect.right, ERect.bottom);
    m_Img2.GetWindowRect(BRect);
    m_SkillKey2Img.GetWindowRect(ERect);
    m_PrgRect[1] = CRect(BRect.left, BRect.top, ERect.right, ERect.bottom);
    m_Img3.GetWindowRect(BRect);
    m_SkillKey3Img.GetWindowRect(ERect);
    m_PrgRect[2] = CRect(BRect.left, BRect.top, ERect.right, ERect.bottom);
    m_Img4.GetWindowRect(BRect);
    m_SkillKey4Img.GetWindowRect(ERect);
    m_PrgRect[3] = CRect(BRect.left, BRect.top, ERect.right, ERect.bottom);
    m_Img5.GetWindowRect(BRect);
    m_SkillKey5Img.GetWindowRect(ERect);
    m_PrgRect[4] = CRect(BRect.left, BRect.top, ERect.right, ERect.bottom);
    m_Img6.GetWindowRect(BRect);
    m_SkillKey6Img.GetWindowRect(ERect);
    m_PrgRect[5] = CRect(BRect.left, BRect.top, ERect.right, ERect.bottom);
    int nIndex = 0;
    for(int i = 0; i < 6; i++)
    {
        if (m_PrgRect[i].PtInRect(mousePnt))
        {
            nIndex = i + 1;
            break;
        }
    }
    if (nIndex == 0)
    {
        return 0;
    }
    else
    {
        return nIndex;
    }
}

void CDlgSkill::SetMagicHotKey(int nIndex, int HotKey)
{
    CMagic* pMagic = g_objHero.GetMagicByIndex(m_nCurIndex + nIndex);
    if (pMagic)
    {
        if (pMagic->TestTarget(MAGIC_PASSIVE))
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10341));//被动技能不能绑定快捷键!
            return;
        }
        else if (pMagic->m_infoMagicType.dwXp == TYPE_XPSKILL )
        {
            return;
        }
        m_DlgSkillHotKey.SetMouseHotKey(pMagic->GetIDType(), HotKey);
    }
}

void CDlgSkill::SetTeamMagicHotKeys(int nIndex)
{
    if (!g_objHero.IsTeamLeader())
    {
        return;
    }
    CMagic* pMagic = g_objHero.GetTeamMagicByIndex(nIndex);
    if (pMagic)
    {
        if (!m_DlgSkillHotKey.m_bShow)
        {
            m_DlgSkillHotKey.EnableWindow();
        }
        if (m_DlgSkillHotKey.m_bShow)
        {
            m_DlgSkillHotKey.PopCurCheck(m_DlgSkillHotKey.m_iMagicHotKey);
            for(int j = 0; j < 8; j++)
            {
                if (pMagic->GetIDType() == m_DlgSkillHotKey.m_HotKeyIdType[j])
                {
                    m_DlgSkillHotKey.m_iMagicHotKey = j;
                    break;
                }
                else
                {
                    m_DlgSkillHotKey.m_iMagicHotKey = 8;
                }
            }
            m_DlgSkillHotKey.PopCurCheck(m_DlgSkillHotKey.m_iMagicHotKey, true);
            m_DlgSkillHotKey.SetStaMagic(pMagic->GetIDType());
        }
    }
    else
    {
        m_DlgSkillHotKey.EnableWindow(false);
    }
}

void CDlgSkill::SetTeamMagicHotKey(int nIndex, int HotKey)
{
    CMagic* pMagic = g_objHero.GetTeamMagicByIndex(m_nCurIndex + nIndex);
    if (pMagic)
    {
        m_DlgSkillHotKey.SetMouseHotKey(pMagic->GetIDType(), HotKey);
    }
}

void CDlgSkill::ShowSkillInfo(int x, int y)
{
    DWORD STRCOLOR_GREEN = 0x00ff00;
    DWORD STRCOLOR_BLUE  = 0x0080ff;
    DWORD STRCOLOR_RED = 0xff0000;
    DWORD STRCOLOR_WHITE = 0xffffff;
    int nIndex = CDlgSkill::GetMouseInMagicIndex();
    if (nIndex != 0)
    {
        CMagic* pMagic = NULL;
        if (m_nSkillListMode == 0)
        {
            pMagic = g_objHero.GetMagicByIndex(m_nCurIndex + nIndex - 1);
        }
        else if (m_nSkillListMode == 1)
        {
            pMagic = g_objHero.GetTeamMagicByIndex(m_nTMagicIndex + nIndex - 1);
        }
        if (pMagic)
        {
            char strText[3][64];
            DWORD strColor;
            CMySize FontSize;
            CMyBitmap::GetFontSize(FontSize);
            sprintf( strText[0], "%s", pMagic->GetName()) ;
            if (pMagic->GetNeedLevel() == 0)
            {
                strColor = STRCOLOR_RED;
                strcpy( strText[1], (char*)g_objGameDataSet.GetStr(10710));	//10710=不可继续修习
            }
            else if (pMagic->GetNeedLevel() > 0 )
            {
                if (pMagic->GetNeedLevel() <= g_objHero.GetLev())
                {
                    strColor = STRCOLOR_BLUE;
                }
                else
                {
                    strColor = STRCOLOR_RED;
                }
                sprintf( strText[1], "%d%s", pMagic->GetNeedLevel(), (char*)g_objGameDataSet.GetStr(10711));	//10711=可以继续修习
            }
            strcpy(strText[2], pMagic->GetDescribe());
            int nPosX = x + 50 - FontSize.iWidth * 14;
            int nPosY = y + 80 + 49 * (nIndex - 1);
            int nLine = 2 + CDlgSkill::GetSkillDescLine(0,
                        0,
                        FontSize.iWidth * 14,
                        strText[2]);
            g_objGameDataSet.ShowInfoBack(	nPosX - 5,	nPosY - 5,
                                            FontSize.iWidth * 14 + 10, (FontSize.iHeight + 2)*nLine + 10) ;
            CMyBitmap::ShowString ( nPosX,
                                    nPosY ,
                                    STRCOLOR_GREEN,
                                    strText[0] ) ;
            CMyBitmap::ShowString ( nPosX,
                                    nPosY  + FontSize.iHeight + 2,
                                    strColor,
                                    strText[1] ) ;
            CDlgSkill::ShowSkillDesc( nPosX,
                                      nPosY  + (FontSize.iHeight + 2) * 2,
                                      FontSize.iWidth * 14,
                                      strText[2] , STRCOLOR_WHITE) ;
        }
    }
}

void CDlgSkill::ShowSkillDesc(int nStartX, int nStartY, int nWidth, const char* szString , DWORD dwColor)
{
    char Static[64] ;
    strcpy(Static, szString);
    if (!Static || strlen(Static) == 0)
    {
        return;
    }
    CMySize psizeFont;
    CMyBitmap::GetFontSize(psizeFont);
    int nMaxLen = nWidth / psizeFont.iWidth;
    int nDescLine = 1 ;
    int nCharIndex[10];
    nCharIndex[0] = 0;
    int nSingle = 0;
    int nLineNum = 0;
    int i = 0;
    for ( ; i <(int) strlen ( Static ); i++ )
    {
        if ( CMyBitmap::IsDBCSLeadByte( Static, i ) )
        {
            nLineNum += 2;
            i++;
        }
        else
        {
            nSingle++;
            nLineNum++;
        }
        if ( nLineNum >= nMaxLen )
        {
            //----------Begin the english version's process(jump line)----------//
            if ( strcmp( g_objGameDataSet.GetVersionInfo(), "English" ) == 0 )
            {
                // Search the last word
                for ( int j = i; j >= 0; j-- )
                {
                    if (Static[j] == ' ' )
                    {
                        i -= i - j;
                        break;
                    }
                }
            }
            //----------End the english version's process(jump line)----------//
            if ( nSingle % 2 == 0 )
            {
                nSingle = 0;
                nLineNum = 0;
                nCharIndex[nDescLine] = i + 1;
                nDescLine++;
            }
            else
            {
                i -= nLineNum - nMaxLen;
                nCharIndex[nDescLine] = i + 1 - ( nLineNum - nMaxLen );
                nSingle = 0;
                nLineNum = 0;
                nDescLine++;
            }
        }
    }
    if ( nSingle % 2 == 0 )
    {
        nCharIndex[nDescLine] = i + 1 ;
    }
    else
    {
        nCharIndex[nDescLine] = i ;
    }
    int nPosX  = nStartX;
    int nPosY  = nStartY;
    CMyBitmap::GetFontSize(psizeFont);
    char strTemp[32] = "" ;
    int nOffset = 0;
    for ( i = 0; i < nDescLine; i++ )
    {
        strncpy ( strTemp, Static + nCharIndex[i], nCharIndex[i + 1] - nCharIndex[i] );
        strTemp[nCharIndex[i + 1] - nCharIndex[i]] = '\0';
        CMyBitmap::ShowString (nPosX,
                               nPosY + nOffset,
                               dwColor,
                               CMyShellApp::StringChange ( strTemp, 1 ) ) ;
        nOffset += psizeFont.iHeight + 2 ;
    }
}



int CDlgSkill::GetSkillDescLine(int nStartX, int nStartY, int nWidth, const char* szString)
{
    char Static[64] ;
    strcpy(Static, szString);
    if (!Static || strlen(Static) == 0)
    {
        return 0;
    }
    char szTemp[64];
    strcpy(szTemp, Static);
    char* pszTemp = szTemp;
    unsigned char ucSingle[2] = {'\0', '\0'};
    unsigned char ucDouble[3] = {'\0', '\0', '\0' }; // 双字节字符
    //------------------------------------------------------------
    int nShowX = nStartX;
    int nShowY = nStartY;
    int nPosX  = nStartX;
    int nPosY  = nStartY;
    int nPos = 0;
    bool bShow = false;
    int  nLine = 1;
    char sz[64];
    memset(sz, 0L, sizeof(sz));
    CMySize psizeFont;
    CMyBitmap::GetFontSize(psizeFont);
    //------------------------------------------------------------
    while((ucSingle[0] = *pszTemp))
    {
        sz[nPos++] = ucSingle[0];
        if (ucSingle[0] < 0x80) // 打印一个单字节字符
        {
            nPosX += psizeFont.iWidth;
            if (((nPosX - nStartX) > (nWidth - psizeFont.iWidth * 2)
                    || ucSingle[0] == '#')
                    && *(pszTemp + 1) != '\0')
            {
                nPosX = nStartX;
                nPosY += psizeFont.iHeight + 2;
                bShow = true;
                nLine++;
            }
        }
        else // 打印双字节字符
        {
            if (ucDouble[0] == '\0')
            {
                ucDouble[0] = ucSingle[0];
            }
            else if (ucDouble[1] == '\0')
            {
                ucDouble[1] = ucSingle[0];
                nPosX = nPosX + psizeFont.iWidth * 2;
                if ((nPosX - nStartX) > (nWidth - psizeFont.iWidth * 2)
                        && *(pszTemp + 1) != '\0')
                {
                    nPosX = nStartX;
                    nPosY += psizeFont.iHeight + 2;
                    bShow = true;
                    nLine++;
                }
                ucDouble[0] = '\0';
                ucDouble[1] = '\0';
            }
        }
        if (bShow)
        {
            nPos = 0;
            bShow = false;
            nShowX = nPosX;
            nShowY = nPosY;
            memset(sz, 0L, sizeof(sz));
        }
        pszTemp++;
    }
    return nLine;
}
