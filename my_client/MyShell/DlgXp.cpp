
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgXp.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgXp.h"

#include "Hero.h"
#include "Magic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgXp dialog


CDlgXp::CDlgXp(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgXp::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgXp)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_bDie = false ;
    m_bTransform = false ;
    m_bFly = false ;
    m_btBoothType = 0 ;
    m_bMouseMove = false;
    //}}AFX_DATA_INIT
}


void CDlgXp::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgXp)
    DDX_Control(pDX, IDC_XP_IMG5, m_XpImg5);
    DDX_Control(pDX, IDC_XP_IMG4, m_XpImg4);
    DDX_Control(pDX, IDC_XP_IMG3, m_XpImg3);
    DDX_Control(pDX, IDC_XP_IMG2, m_XpImg2);
    DDX_Control(pDX, IDC_XP_IMG1, m_XpImg1);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgXp, CDialog)
    //{{AFX_MSG_MAP(CDlgXp)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_XP_IMG1, OnXpImg1)
    ON_BN_CLICKED(IDC_XP_IMG2, OnXpImg2)
    ON_BN_CLICKED(IDC_XP_IMG3, OnXpImg3)
    ON_BN_CLICKED(IDC_XP_IMG4, OnXpImg4)
    ON_BN_CLICKED(IDC_XP_IMG5, OnXpImg5)
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgXp message handlers

LRESULT CDlgXp::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_SHOWWINDOW )
    {
        m_bShow = wParam ;
        DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
        if ( !m_bShow )
        {
            m_XpImg1.RemoveImage();
            m_XpImg2.RemoveImage();
            m_XpImg3.RemoveImage();
            m_XpImg4.RemoveImage();
            m_XpImg5.RemoveImage();
            CDlgXp::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MSGDLG_HIDE ) ;
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
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgXp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgXp::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgXp::GetWindowRect ( rect ) ;
    // Init the image
    m_XpImg1.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_XpImg2.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_XpImg3.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_XpImg4.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    m_XpImg5.Init ( rect.left, rect.top, NULL, SKILL_IMAGE ) ;
    // Init the button
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgXp::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgXp::Show()
{
    // show the image
    if ( m_bShow )
    {
        if (m_btBoothType  && m_XpImg1.m_bMouseMove)
        {
            // 判断鼠标是否刚刚进入
            // 是
            if (m_bInJustNow)
            {
                // 记录进入的时间
                m_dwInTime = TimeGet();
                m_bInJustNow = false;
            }
            // 判断进入时间是否达到 0.5 秒
            // 否
            else if (TimeGet() - m_dwInTime >= 500)
            {
                CMySize FontSize;
                CMyBitmap::GetFontSize(FontSize);
                g_objGameDataSet.ShowInfoBack(m_Pnt.x - 1 - 5, m_Pnt.y - FontSize.iHeight - 1 - 15,
                                              26 + 10, FontSize.iHeight + 2 + 10) ;
                CMyBitmap::ShowString ( m_Pnt.x,
                                        m_Pnt.y - FontSize.iHeight - 2 - 10,
                                        0xffffff,
                                        g_objGameDataSet.GetStr(10754) ) ;
            }
        }
        else
        {
            m_bInJustNow = true;
            if (!m_bDie && !m_btBoothType)
            {
                CheckXpSkill();
            }
        }
        m_XpImg1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_XpImg2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_XpImg3.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_XpImg4.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_XpImg5.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
    else
    {
        m_bInJustNow = true;
    }
}

void CDlgXp::InitXpDialog()
{
    m_btBoothType = 0 ;
    m_nCurBase = 0 ;
    m_bFly = false ;
    int nAmount = g_objHero.GetXpSkillAmount() ;
    for ( int i = 0; i < nAmount; i++ )
    {
        CMagic* pXp = g_objHero.GetXpSkillByIndex(i) ;
        if ( pXp )
        {
            if ( g_objHero.IsTransform () && pXp->IsTransformMagic () )
            {
                continue ;
            }
            char SkillType[32] ;
            BOOL bDisable = TRUE ;
            if (g_objHero.GetProfession() == 20 && pXp->GetWeaponRequired() != ID_NONE)
            {
                CItem* pItem = g_objHero.GetEquipment(ITEMPOSITION_WEAPONR);
                if (pItem == NULL)
                {
                    pItem = g_objHero.GetEquipment(ITEMPOSITION_WEAPONL);
                    if (pItem != NULL)
                    {
                        if (pXp->GetWeaponRequired() == 400)
                        {
                            if (pItem->GetTypeID() / 1000 != 410 && pItem->GetTypeID() / 1000 != 420)
                            {
                                bDisable = FALSE;
                            }
                        }
                        else if (pItem->GetTypeID() / 1000 != pXp->GetWeaponRequired())
                        {
                            bDisable = FALSE;
                        }
                    }
                    else
                    {
                        bDisable = FALSE;
                    }
                }
                else
                {
                    if (pXp->GetWeaponRequired() == 400)
                    {
                        if (pItem->GetTypeID() / 1000 != 410 && pItem->GetTypeID() / 1000 != 420)
                        {
                            bDisable = FALSE;
                        }
                    }
                    else if (pItem->GetTypeID() / 1000 != pXp->GetWeaponRequired())
                    {
                        bDisable = FALSE;
                    }
                }
            }
            m_XpIdType[i] = pXp->GetIDType () ;
            sprintf ( SkillType, "MagicSkillType%d", m_XpIdType[i] ) ;
            switch ( i )
            {
            case 0:
                m_bSkillEnable[0] = bDisable;
                m_XpImg1.InsertImage ( SkillType, m_XpIdType[i], bDisable ) ;
                break ;
            case 1:
                m_bSkillEnable[1] = bDisable;
                m_XpImg2.InsertImage ( SkillType, m_XpIdType[i], bDisable ) ;
                break ;
            case 2:
                m_bSkillEnable[2] = bDisable;
                m_XpImg3.InsertImage ( SkillType, m_XpIdType[i], bDisable ) ;
                break ;
            case 3:
                m_bSkillEnable[3] = bDisable;
                m_XpImg4.InsertImage ( SkillType, m_XpIdType[i], bDisable ) ;
                break ;
            case 4:
                m_bSkillEnable[4] = bDisable;
                m_XpImg5.InsertImage ( SkillType, m_XpIdType[i], bDisable ) ;
                break ;
            default:
                break;
            }
        }
    }
    CDlgXp::ShowWindow ( SW_SHOW ) ;
}

void CDlgXp::CheckXpSkill()
{
    int nAmount = 5;
    for ( int i = 0; i < nAmount; i++ )
    {
        CMagic* pXp = g_objHero.GetMagic(m_XpIdType[i]) ;
        if ( pXp )
        {
            if ( g_objHero.IsTransform () && pXp->IsTransformMagic () )
            {
                continue ;
            }
            char SkillType[32] ;
            BOOL bDisable = TRUE ;
            if (g_objHero.GetProfession() == 20 && pXp->GetWeaponRequired() != ID_NONE)
            {
                CItem* pItem = g_objHero.GetEquipment(ITEMPOSITION_WEAPONR);
                if (pItem == NULL)
                {
                    pItem = g_objHero.GetEquipment(ITEMPOSITION_WEAPONL);
                    if (pItem != NULL)
                    {
                        if (pXp->GetWeaponRequired() == 400)
                        {
                            if (pItem->GetTypeID() / 1000 != 410 && pItem->GetTypeID() / 1000 != 420)
                            {
                                bDisable = FALSE;
                            }
                        }
                        else if (pItem->GetTypeID() / 1000 != pXp->GetWeaponRequired())
                        {
                            bDisable = FALSE;
                        }
                    }
                    else
                    {
                        bDisable = FALSE;
                    }
                }
                else
                {
                    if (pXp->GetWeaponRequired() == 400)
                    {
                        if (pItem->GetTypeID() / 1000 != 410 && pItem->GetTypeID() / 1000 != 420)
                        {
                            bDisable = FALSE;
                        }
                    }
                    else if (pItem->GetTypeID() / 1000 != pXp->GetWeaponRequired())
                    {
                        bDisable = FALSE;
                    }
                }
            }
            m_XpIdType[i] = pXp->GetIDType () ;
            sprintf ( SkillType, "MagicSkillType%d", m_XpIdType[i] ) ;
            switch ( i )
            {
            case 0:
                m_bSkillEnable[0] = bDisable;
                m_XpImg1.InsertImage ( SkillType, m_XpIdType[i], bDisable ) ;
                break ;
            case 1:
                m_bSkillEnable[1] = bDisable;
                m_XpImg2.InsertImage ( SkillType, m_XpIdType[i], bDisable ) ;
                break ;
            case 2:
                m_bSkillEnable[2] = bDisable;
                m_XpImg3.InsertImage ( SkillType, m_XpIdType[i], bDisable ) ;
                break ;
            case 3:
                m_bSkillEnable[3] = bDisable;
                m_XpImg4.InsertImage ( SkillType, m_XpIdType[i], bDisable ) ;
                break ;
            case 4:
                m_bSkillEnable[4] = bDisable;
                m_XpImg5.InsertImage ( SkillType, m_XpIdType[i], bDisable ) ;
                break ;
            default:
                break;
            }
        }
    }
}

void CDlgXp::OnXpImg1()
{
    if ( m_bDie )
    {
        if ( g_objHero.IsRebornEnable () && !g_objHero.IsMoving () )
        {
            g_objHero.ReBorn () ;
            CDlgXp::ShowWindow ( SW_HIDE ) ;
            m_bDie = false ;
        }
        return ;
    }
    else if (m_btBoothType)
    {
        g_objHero.m_objBoothManager.Close();
        CDlgXp::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCBOOTH_CLOSE ) ;
        m_btBoothType = false;
        CDlgXp::ShowWindow(SW_HIDE);
        return;
    }
    else if (m_bSkillEnable[0])
    {
        CDlgXp::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_XP_CHOOSE, m_XpIdType[0] ) ;
    }
}

void CDlgXp::SetBooth(BOOL bOpen)
{
    m_XpImg1.RemoveImage();
    m_XpImg2.RemoveImage();
    m_XpImg3.RemoveImage();
    m_XpImg4.RemoveImage();
    m_XpImg5.RemoveImage();
    if (bOpen)
    {
        m_btBoothType = true;
        CRect rect;
        CDlgXp::GetWindowRect(rect);
        CDlgXp::MoveWindow ( _SCR_WIDTH  - 110,
                             _SCR_HEIGHT - 110,
                             rect.Width(),
                             rect.Height() ) ;
        m_XpImg1.ChangeImageType(NORMAL_IMAGE);
        m_XpImg1.InsertImage("Image141", 0, 0, true);
        CDlgXp::ShowWindow( SW_SHOW );
    }
    else
    {
        m_btBoothType = false;
        m_XpImg1.ChangeImageType(SKILL_IMAGE);
        CDlgXp::ShowWindow ( SW_HIDE ) ;
    }
}

void CDlgXp::SetTransform(BOOL bTrans)
{
}

void CDlgXp::SetFly(BOOL bFly)
{
}

void CDlgXp::SetReborn(BOOL bDie)
{
    m_XpImg1.RemoveImage();
    m_XpImg2.RemoveImage();
    m_XpImg3.RemoveImage();
    m_XpImg4.RemoveImage();
    m_XpImg5.RemoveImage();
    if (bDie)
    {
        m_bDie = true ;
        m_XpImg1.ChangeImageType(NORMAL_IMAGE);
        m_XpImg1.InsertImage ( "Image140", 0, 0, true ) ;
        CDlgXp::ShowWindow ( SW_SHOW ) ;
    }
    else
    {
        m_bDie = false;
        m_XpImg1.ChangeImageType(SKILL_IMAGE);
        CDlgXp::ShowWindow ( SW_HIDE ) ;
    }
}

void CDlgXp::SetTeamMagicImg(OBJID idMagic)
{
    char SkillType[32] ;
    sprintf ( SkillType, "MagicSkillType%d", idMagic ) ;
    m_XpImg1.ChangeImageType(SKILL_IMAGE);
    m_XpImg1.InsertImage(SkillType, 0, idMagic, false);
}

void CDlgXp::OnXpImg2()
{
    // TODO: Add your control notification handler code here
    if (m_bSkillEnable[1])
    {
        CDlgXp::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_XP_CHOOSE, m_XpIdType[1] ) ;
    }
}

void CDlgXp::OnXpImg3()
{
    // TODO: Add your control notification handler code here
    if (m_bSkillEnable[2])
    {
        CDlgXp::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_XP_CHOOSE, m_XpIdType[2] ) ;
    }
}

void CDlgXp::OnXpImg4()
{
    // TODO: Add your control notification handler code here
    if (m_bSkillEnable[3])
    {
        CDlgXp::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_XP_CHOOSE, m_XpIdType[3] ) ;
    }
}

void CDlgXp::OnXpImg5()
{
    // TODO: Add your control notification handler code here
    if (m_bSkillEnable[4])
    {
        CDlgXp::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_XP_CHOOSE, m_XpIdType[4] ) ;
    }
}

void CDlgXp::ChooseXp(OBJID XpId)
{
    CDlgXp::MoveWindow ( _SCR_WIDTH - 105,
                         _SCR_HEIGHT - 115,
                         50,
                         50 ) ;
    m_XpImg1.RemoveImage();
    m_XpImg2.RemoveImage();
    m_XpImg3.RemoveImage();
    m_XpImg4.RemoveImage();
    m_XpImg5.RemoveImage();
    for(int i = 0; i < 5; i++)
    {
        m_XpIdType[i] = 0;
    }
    CMagic* pXp = g_objHero.GetMagic(XpId) ;
    if ( pXp && pXp->GetIDType() == XpId)
    {
        BOOL bDisable = !pXp->IsEnable () ;
        char SkillType[32] = "" ;
        m_XpIdType[0] = XpId;
        sprintf ( SkillType, "MagicSkillType%d", m_XpIdType[0] ) ;
        m_XpImg1.InsertImage(SkillType, m_XpIdType[0], bDisable);
    }
}

void CDlgXp::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnMouseMove(nFlags, point);
}
