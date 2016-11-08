
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgProgress.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgProgress.h"

#include "Hero.h"
#include "GameMsg.h"
#include "3DGameMap.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog
const int	LONG_PRG_WIDTH = 106 ;
const int	MID_PRG_WIDTH = 74 ;
const int	SHORT_PRG_WIDTH = 46 ;
const int	PRG_HEIGHT = 6 ;

CDlgProgress::CDlgProgress(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgProgress::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgProgress)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_btCurMovePrg = 255 ;
    m_bLevUp = false ;
    m_dwFlashTime = 0;
    m_iMouseMove = 0;
    m_nHeroXp = 0;
    //}}AFX_DATA_INIT
}


void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgProgress)
    DDX_Control(pDX, IDC_IMG_BADSTATUS, m_BadStatusImg);
    DDX_Control(pDX, IDC_PRG_HERO_XP2, m_PrgHeroXpBase);
    DDX_Control(pDX, IDC_IMG_MSGFLASH, m_ImgMsgFlash);
    DDX_Control(pDX, IDC_PRG_HERO_XP, m_PrgHeroXp);
    DDX_Control(pDX, IDC_PRG_HERO_FORCE2, m_PrgHeroForce2);
    DDX_Control(pDX, IDC_PRG_HERO_MP, m_PrgHeroMp);
    DDX_Control(pDX, IDC_PRG_HERO_HP, m_PrgHeroHp);
    DDX_Control(pDX, IDC_PRG_HERO_FORCE, m_PrgHeroForce);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
    //{{AFX_MSG_MAP(CDlgProgress)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_BN_CLICKED(IDC_IMG_MSGFLASH, OnImgMsgflash)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress message handlers

LRESULT CDlgProgress::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_nHeroXp = 0;
        m_bShow = wParam ;
    }
    else if (message == WM_DESTROY)
    {
        m_EffectExXp.Clear();
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

HBRUSH CDlgProgress::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgProgress::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgProgress::GetWindowRect ( rect ) ;
    // Init the Image msg flash
    m_ImgMsgFlash.Init(rect.left, rect.top, "Image40");
    // Init the progress
    m_PrgHeroMp.Init( rect.left, rect.top, "Progress580", 0, 100, 0) ;
    m_PrgHeroMp.SetWidthAndHeight(34, 74, 34);
    m_PrgHeroHp.Init( rect.left, rect.top, "Progress581", 0, 100, 0 ) ;
    m_PrgHeroHp.SetWidthAndHeight(34, 74, 34);
    m_PrgHeroXp.Init( rect.left, rect.top, "Progress582", 0, 100, 0 ) ;
    m_PrgHeroXp.SetWidthAndHeight(89, 90, 89);
    m_PrgHeroXpBase.Init(rect.left, rect.top, "Progress587", 0, 100, 0 );
    m_PrgHeroXpBase.SetWidthAndHeight(89, 90, 89);
    m_PrgHeroForce.Init( rect.left, rect.top, "Progress583", 0, 100, 0 ) ;
    m_PrgHeroForce.SetWidthAndHeight(3, 22, 3);
    m_PrgHeroForce2.Init( rect.left, rect.top, "Progress583", 0, 100, 0 ) ;
    m_PrgHeroForce2.SetWidthAndHeight(3, 22, 3);
    m_BadStatusImg.Init(rect.left, rect.top, "Image150");
    CRect SRect;
    m_PrgHeroHp.GetWindowRect(SRect);
    m_PrgRect[0] = CRect ( SRect.left - rect.left,
                           SRect.top - rect.top,
                           SRect.right - rect.left,
                           SRect.bottom - rect.top) ;
    m_PrgHeroMp.GetWindowRect(SRect);
    m_PrgRect[1] = CRect ( SRect.left - rect.left,
                           SRect.top - rect.top,
                           SRect.right - rect.left,
                           SRect.bottom - rect.top) ;
    m_PrgRect[2] = CRect ( m_PrgRect[0].right, m_PrgRect[0].top,
                           m_PrgRect[1].left, m_PrgRect[1].bottom);
    m_PrgHeroXp.GetWindowRect(SRect);
    m_PrgRect[3] = CRect ( SRect.left - rect.left,
                           SRect.top - rect.top,
                           SRect.right - rect.left,
                           SRect.bottom - rect.top) ;
    m_EffectExXp.Add("XP");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProgress::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgProgress::Show(int talk_width)
{
    if ( m_bShow )
    {
        // Show the progress
        SetProgressValue();
        m_PrgHeroHp.Show( m_Pnt.x, m_Pnt.y, g_objHero.GetData(CPlayer::_PLAYER_LIFE), g_objHero.GetData(CPlayer::_PLAYER_CURRENTLIFE));
        int nWidth = 39;
        if (g_objHero.GetMaxMana() <= 0)
        {
            m_PrgHeroMp.Show( m_Pnt.x, m_Pnt.y, g_objHero.GetData(CPlayer::_PLAYER_LIFE), g_objHero.GetData(CPlayer::_PLAYER_CURRENTLIFE));
            nWidth = 78;
        }
        else
        {
            m_PrgHeroMp.Show( m_Pnt.x, m_Pnt.y, g_objHero.GetData(CPlayer::_PLAYER_MANA), g_objHero.GetCurMana());
        }
        if (!g_objHero.IsDead())
        {
            m_PrgHeroHp.ShowMaxPercent(m_Pnt.x, m_Pnt.y, g_objHero.GetMaxLife(), g_objHero.GetMaxLifePercent(), nWidth);
        }
   
        m_PrgHeroForce.Show( m_Pnt.x, m_Pnt.y, g_objHero.GetPhysicalForce(), g_objHero.GetPhysicalForce()  ) ;
        int nForce = 0;
        if (g_objHero.GetPhysicalForce() >  g_objHero.GetMaxPhysicalForce() / 2)
        {
            nForce = g_objHero.GetPhysicalForce() - g_objHero.GetMaxPhysicalForce() / 2 ;
        }
        m_PrgHeroForce2.Show( m_Pnt.x, m_Pnt.y, nForce, nForce ) ;
        // 	m_PrgHeroXpBase.Show( m_Pnt.x,m_Pnt.y, g_objHero.GetXP(),g_objHero.GetXP() ) ;
        int nXp = g_objHero.GetXP();
        if (m_nHeroXp != nXp)
        {
            if (m_nHeroXp > nXp)
            {
                m_nHeroXp--;
            }
            else
            {
                m_nHeroXp++;
            }
        }
        m_PrgHeroXp.Show( m_Pnt.x, m_Pnt.y, g_objHero.GetXP(), m_nHeroXp ) ;
        ShowMsgFlash();
        if (m_BadStatusImg.IsWindowEnabled())
        {
            m_BadStatusImg.Show(m_Pnt.x, m_Pnt.y );
        }
        if (g_objHero.GetXP() >= g_objHero.GetMaxXP() && g_objHero.GetXpSkillAmount() > 0 && !(g_objGameMap.GetType() & MAPTYPE_BOOTH_ENABLE) )
        {
            CMyPos pPos ;
			pPos.x = (_SCR_WIDTH - talk_width) / 2 - 50;
			pPos.y =  _SCR_HEIGHT + 25;
            CMyPos pPosWord, posBG;
            g_objGameMap.Screen2World(pPos.x, pPos.y, pPosWord.x, pPosWord.y);
            g_objGameMap.World2Bg(pPosWord.x, pPosWord.y, posBG.x, posBG.y);
            m_EffectExXp.Process();
            m_EffectExXp.Show(posBG);
        }
    }
}

void CDlgProgress::SetProgressValue()
{
    if (g_objHero.GetMaxLifePercent() > 0 && g_objHero.GetMaxLifePercent() < 1000 && !g_objHero.IsDead())
    {
        m_PrgHeroHp.ResetValue(0, g_objHero.GetMaxLife() * 1000 / g_objHero.GetMaxLifePercent());
        char szTip[64] = "";
        sprintf(szTip, g_objGameDataSet.GetStr(10760), (float)(1000 - g_objHero.GetMaxLifePercent()) / 10);
        m_BadStatusImg.SetTipShow(szTip);
        m_BadStatusImg.SetTipPos(65, 25);
        if (!m_BadStatusImg.IsWindowEnabled())
        {
            m_BadStatusImg.EnableWindow();
        }
    }
    else
    {
        m_PrgHeroHp.ResetValue(0, g_objHero.GetMaxLife());
        if (m_BadStatusImg.IsWindowEnabled())
        {
            m_BadStatusImg.EnableWindow(false);
        }
    }
    if (g_objHero.GetMaxMana() <= 0)
    {
        if (strcmp(m_PrgHeroMp.GetProgressID(), "Progress603") != 0)
        {
            m_PrgHeroMp.SetProgressID("Progress603");
        }
        if (g_objHero.GetMaxLifePercent() > 0 && g_objHero.GetMaxLifePercent() < 1000 && !g_objHero.IsDead())
        {
            m_PrgHeroMp.ResetValue(0, g_objHero.GetMaxLife() * 1000 / g_objHero.GetMaxLifePercent());
        }
        else
        {
            m_PrgHeroMp.ResetValue(0, g_objHero.GetMaxLife());
        }
    }
    else
    {
        if (strcmp(m_PrgHeroMp.GetProgressID(), "Progress580") != 0)
        {
            m_PrgHeroMp.SetProgressID("Progress580");
        }
        m_PrgHeroMp.ResetValue(0, g_objHero.GetMaxMana());
    }
    m_PrgHeroForce.ResetValue(0, g_objHero.GetMaxPhysicalForce() / 2);
    m_PrgHeroForce2.ResetValue(0, g_objHero.GetMaxPhysicalForce() / 2);
    m_PrgHeroXp.ResetValue(0, g_objHero.GetMaxXP());
    m_PrgHeroXpBase.ResetValue(0, g_objHero.GetMaxXP());
}

void CDlgProgress::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if ( m_iMouseMove == 0)
    {
        for(int i = 0; i < 4; i++)
        {
            if ( m_PrgRect[i].PtInRect( point ) == TRUE)
            {
                if (i == 3)
                {
                    if (!m_PrgRect[0].PtInRect(point) && !m_PrgRect[1].PtInRect(point))
                    {
                        CDlgProgress::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_STATIC, i ) ;
                        m_iMouseMove = i + 1 ;
                    }
                }
                else
                {
                    CDlgProgress::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_STATIC, i ) ;
                    m_iMouseMove = i + 1 ;
                }
                return;
            }
        }
    }
    else
    {
        for(int j = 0; j < 3; j++)
        {
            if ( !m_PrgRect[j].PtInRect( point ) )
            {
                if (m_iMouseMove == j + 1 )
                {
                    m_iMouseMove = 0 ;
                }
            }
            else
            {
                if (m_iMouseMove != j + 1 )
                {
                    m_iMouseMove = 0;
                }
            }
        }
        if (m_iMouseMove == 0)
        {
            CDlgProgress::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_HIDE ) ;
        }
    }
    CDialog::OnMouseMove(nFlags, point);
}


void CDlgProgress::ShowMsgFlash()
{
    if (g_objGameMsg.GetLeaveWordAmount() > 0)
    {
        if (0 == m_dwFlashTime)
        {
            m_dwFlashTime = ::TimeGet();
        }
        else if (::TimeGet() - m_dwFlashTime > 2400)
        {
            m_dwFlashTime = 0;
        }
        int nFrame = (::TimeGet() - m_dwFlashTime) / 600;
        nFrame = nFrame % 2;
        m_ImgMsgFlash.SetCurFrame(nFrame);
        if (m_ImgMsgFlash.IsWindowEnabled())
        {
            m_ImgMsgFlash.Show(m_Pnt.x, m_Pnt.y );
        }
    }
}

void CDlgProgress::OnImgMsgflash()
{
    // TODO: Add your control notification handler code here
    if (g_objGameMsg.GetLeaveWordAmount())
    {
        DXPlaySound ( "Sound/Message_Cue.wav" ) ;
        CDlgProgress::GetParent()->PostMessage(WM_MY_MESSAGE, ON_MSGSEND_VIEW, -1);
        m_ImgMsgFlash.EnableWindow(false);
    }
}


void CDlgProgress::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    for(int i = 0; i < 4; i++)
    {
        if ( m_PrgRect[i].PtInRect( point ) == TRUE)
        {
            if (i == 3)
            {
                if (!m_PrgRect[0].PtInRect(point) && !m_PrgRect[1].PtInRect(point))
                {
                    CDlgProgress::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_STATIC, i | 0xFF000000) ;
                    m_iMouseMove = i + 1 ;
                }
            }
            else
            {
                CDlgProgress::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_STATIC, i | 0xFF000000) ;
                m_iMouseMove = i + 1 ;
            }
            return;
        }
    }
    CDialog::OnLButtonDown(nFlags, point);
}
