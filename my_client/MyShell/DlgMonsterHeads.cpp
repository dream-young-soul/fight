
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgMonsterHeads.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgMonsterHeads.h"

#include "Hero.h"
#include "3dgamemap.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterHeads dialog


CDlgMonsterHeads::CDlgMonsterHeads(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMonsterHeads::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMonsterHeads)
    m_Pnt = CPoint(0, 0);
    m_bShow = TRUE;
    m_Xp1 = 0;
    m_Xp2 = 0;
    m_Xp3 = 0;
    m_Xp4 = 0;
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgMonsterHeads::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMonsterHeads)
    DDX_Control(pDX, IDC_STA_XP1, m_StaXp1);
    DDX_Control(pDX, IDC_STA_XP2, m_StaXp2);
    DDX_Control(pDX, IDC_STA_XP3, m_StaXp3);
    DDX_Control(pDX, IDC_STA_XP4, m_StaXp4);
    DDX_Control(pDX, IDC_STA_HP1, m_StaHp1);
    DDX_Control(pDX, IDC_STA_HP2, m_StaHp2);
    DDX_Control(pDX, IDC_STA_HP3, m_StaHp3);
    DDX_Control(pDX, IDC_STA_HP4, m_StaHp4);
    DDX_Control(pDX, IDC_STA_EXP1, m_StaExp1);
    DDX_Control(pDX, IDC_STA_EXP2, m_StaExp2);
    DDX_Control(pDX, IDC_STA_EXP3, m_StaExp3);
    DDX_Control(pDX, IDC_STA_EXP4, m_StaExp4);
    DDX_Control(pDX, IDC_STA_NAME8, m_StaName4);
    DDX_Control(pDX, IDC_STA_NAME3, m_StaName3);
    DDX_Control(pDX, IDC_STA_NAME2, m_StaName2);
    DDX_Control(pDX, IDC_STA_NAME1, m_StaName1);
    DDX_Control(pDX, IDC_PROGRESS_XP4, m_ProgressXp4);
    DDX_Control(pDX, IDC_PROGRESS_XP3, m_ProgressXp3);
    DDX_Control(pDX, IDC_PROGRESS_XP2, m_ProgressXp2);
    DDX_Control(pDX, IDC_PROGRESS_XP1, m_ProgressXp1);
    DDX_Control(pDX, IDC_PROGRESS_HP4, m_ProgressHp4);
    DDX_Control(pDX, IDC_PROGRESS_HP3, m_ProgressHp3);
    DDX_Control(pDX, IDC_PROGRESS_HP2, m_ProgressHp2);
    DDX_Control(pDX, IDC_PROGRESS_HP1, m_ProgressHp1);
    DDX_Control(pDX, IDC_PROGRESS_EXP4, m_ProgressExp4);
    DDX_Control(pDX, IDC_PROGRESS_EXP3, m_ProgressExp3);
    DDX_Control(pDX, IDC_PROGRESS_EXP2, m_ProgressExp2);
    DDX_Control(pDX, IDC_PROGRESS_EXP1, m_ProgressExp1);
    DDX_Control(pDX, IDC_IMG_HEADBACK4, m_ImgHeadBack4);
    DDX_Control(pDX, IDC_IMG_HEADBACK3, m_ImgHeadBack3);
    DDX_Control(pDX, IDC_IMG_HEADBACK2, m_ImgHeadBack2);
    DDX_Control(pDX, IDC_IMG_HEADBACK1, m_ImgHeadBack1);
    DDX_Control(pDX, IDC_IMG_HEAD4, m_ImgHead4);
    DDX_Control(pDX, IDC_IMG_HEAD3, m_ImgHead3);
    DDX_Control(pDX, IDC_IMG_HEAD2, m_ImgHead2);
    DDX_Control(pDX, IDC_IMG_HEAD1, m_ImgHead1);
    DDX_Control(pDX, IDC_BTN_CALL4, m_BtnCall4);
    DDX_Control(pDX, IDC_BTN_CALL3, m_BtnCall3);
    DDX_Control(pDX, IDC_BTN_CALL2, m_BtnCall2);
    DDX_Control(pDX, IDC_BTN_CALL1, m_BtnCall1);
    DDX_Control(pDX, IDC_BTN_ATTACH4, m_BtnAttach4);
    DDX_Control(pDX, IDC_BTN_ATTACH3, m_BtnAttach3);
    DDX_Control(pDX, IDC_BTN_ATTACH2, m_BtnAttach2);
    DDX_Control(pDX, IDC_BTN_ATTACH1, m_BtnAttach1);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMonsterHeads, CDialog)
    //{{AFX_MSG_MAP(CDlgMonsterHeads)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_BN_CLICKED(IDC_BTN_ATTACH1, OnBtnAttach1)
    ON_BN_CLICKED(IDC_BTN_ATTACH2, OnBtnAttach2)
    ON_BN_CLICKED(IDC_BTN_ATTACH3, OnBtnAttach3)
    ON_BN_CLICKED(IDC_BTN_ATTACH4, OnBtnAttach4)
    ON_BN_CLICKED(IDC_BTN_CALL1, OnBtnCall1)
    ON_BN_CLICKED(IDC_BTN_CALL2, OnBtnCall2)
    ON_BN_CLICKED(IDC_BTN_CALL3, OnBtnCall3)
    ON_BN_CLICKED(IDC_BTN_CALL4, OnBtnCall4)
    ON_BN_CLICKED(IDC_IMG_HEAD1, OnImgHead1)
    ON_BN_CLICKED(IDC_IMG_HEAD2, OnImgHead2)
    ON_BN_CLICKED(IDC_IMG_HEAD3, OnImgHead3)
    ON_BN_CLICKED(IDC_IMG_HEAD4, OnImgHead4)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterHeads message handlers

LRESULT CDlgMonsterHeads::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message == WM_ENABLE)
    {
        m_bShow = wParam;
        m_setMonsterHead.clear ();
        ResetMonsterHeads ();
    }
    else if (message == WM_COMMAND)
    {
        if (wParam == 1 || wParam == 2)
        {
            return TRUE;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgMonsterHeads::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

BOOL CDlgMonsterHeads::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgMonsterHeads::GetWindowRect (rect);
    m_ImgHeadBack1.Init(rect.left, rect.top, 1, "Image42");
    m_ImgHeadBack1.InsertImage ();
    m_ImgHeadBack2.Init(rect.left, rect.top, 1, "Image42");
    m_ImgHeadBack2.InsertImage ();
    m_ImgHeadBack3.Init(rect.left, rect.top, 1, "Image42");
    m_ImgHeadBack3.InsertImage ();
    m_ImgHeadBack4.Init(rect.left, rect.top, 1, "Image42");
    m_ImgHeadBack4.InsertImage ();
    m_ImgHead1.Init (rect.left, rect.top, NULL, SMALLPETFACE_IMAGE);
    m_ImgHead2.Init (rect.left, rect.top, NULL, SMALLPETFACE_IMAGE);
    m_ImgHead3.Init (rect.left, rect.top, NULL, SMALLPETFACE_IMAGE);
    m_ImgHead4.Init (rect.left, rect.top, NULL, SMALLPETFACE_IMAGE);
    m_StaName1.Init (rect.left, rect.top);
    m_StaName1.SetStaticPosType (1);
    m_StaName2.Init (rect.left, rect.top);
    m_StaName2.SetStaticPosType (1);
    m_StaName3.Init (rect.left, rect.top);
    m_StaName3.SetStaticPosType (1);
    m_StaName4.Init (rect.left, rect.top);
    m_StaName4.SetStaticPosType (1);
    m_BtnCall1.Init (rect.left, rect.top, "Button570");
    m_BtnCall2.Init (rect.left, rect.top, "Button570");
    m_BtnCall3.Init (rect.left, rect.top, "Button570");
    m_BtnCall4.Init (rect.left, rect.top, "Button570");
    m_BtnAttach1.Init (rect.left, rect.top, "Button571");
    m_BtnAttach2.Init (rect.left, rect.top, "Button571");
    m_BtnAttach3.Init (rect.left, rect.top, "Button571");
    m_BtnAttach4.Init (rect.left, rect.top, "Button571");
    m_ProgressHp1.Init (rect.left, rect.top, "Progress584", 0, 100, MY_PROGRESS_HLEFT);
    m_ProgressHp2.Init (rect.left, rect.top, "Progress584", 0, 100, MY_PROGRESS_HLEFT);
    m_ProgressHp3.Init (rect.left, rect.top, "Progress584", 0, 100, MY_PROGRESS_HLEFT);
    m_ProgressHp4.Init (rect.left, rect.top, "Progress584", 0, 100, MY_PROGRESS_HLEFT);
    m_ProgressXp1.Init (rect.left, rect.top, "Progress585", 0, 100, MY_PROGRESS_HLEFT);
    m_ProgressXp2.Init (rect.left, rect.top, "Progress585", 0, 100, MY_PROGRESS_HLEFT);
    m_ProgressXp3.Init (rect.left, rect.top, "Progress585", 0, 100, MY_PROGRESS_HLEFT);
    m_ProgressXp4.Init (rect.left, rect.top, "Progress585", 0, 100, MY_PROGRESS_HLEFT);
    m_ProgressExp1.Init (rect.left, rect.top, "Progress586", 0, 100, MY_PROGRESS_HLEFT);
    m_ProgressExp2.Init (rect.left, rect.top, "Progress586", 0, 100, MY_PROGRESS_HLEFT);
    m_ProgressExp3.Init (rect.left, rect.top, "Progress586", 0, 100, MY_PROGRESS_HLEFT);
    m_ProgressExp4.Init (rect.left, rect.top, "Progress586", 0, 100, MY_PROGRESS_HLEFT);
    m_StaHp1.Init (rect.left, rect.top);
    m_StaHp1.SetStaticPosType (1);
    m_StaHp2.Init (rect.left, rect.top);
    m_StaHp2.SetStaticPosType (1);
    m_StaHp3.Init (rect.left, rect.top);
    m_StaHp3.SetStaticPosType (1);
    m_StaHp4.Init (rect.left, rect.top);
    m_StaHp4.SetStaticPosType (1);
    m_StaXp1.Init (rect.left, rect.top);
    m_StaXp1.SetStaticPosType (1);
    m_StaXp2.Init (rect.left, rect.top);
    m_StaXp2.SetStaticPosType (1);
    m_StaXp3.Init (rect.left, rect.top);
    m_StaXp3.SetStaticPosType (1);
    m_StaXp4.Init (rect.left, rect.top);
    m_StaXp4.SetStaticPosType (1);
    m_StaExp1.Init (rect.left, rect.top);
    m_StaExp1.SetStaticPosType (1);
    m_StaExp2.Init (rect.left, rect.top);
    m_StaExp2.SetStaticPosType (1);
    m_StaExp3.Init (rect.left, rect.top);
    m_StaExp3.SetStaticPosType (1);
    m_StaExp4.Init (rect.left, rect.top);
    m_StaExp4.SetStaticPosType (1);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

CMyPos CDlgMonsterHeads::GetEffectHeadPos(int x, int y)
{
    CMyPos pPosWord, posBG;
    g_objGameMap.Screen2World(x, y, pPosWord.x, pPosWord.y);
    g_objGameMap.World2Bg(pPosWord.x, pPosWord.y, posBG.x, posBG.y);
    return posBG;
}

void CDlgMonsterHeads::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgMonsterHeads::Show()
{
    if (m_bShow)
    {
        FlashMonsterStatus ();
        if (m_ImgHead1.IsWindowEnabled())
        {
            m_ImgHeadBack1.Show(m_Pnt.x, m_Pnt.y);
            m_ImgHead1.Show(m_Pnt.x, m_Pnt.y);
            m_StaName1.Show (m_Pnt.x, m_Pnt.y);
            m_ProgressHp1.Show (m_Pnt.x, m_Pnt.y, m_Hp1, m_Hp1);
            m_ProgressExp1.Show (m_Pnt.x, m_Pnt.y, m_Exp1, m_Exp1);
            m_ProgressXp1.Show (m_Pnt.x, m_Pnt.y, m_Xp1, m_Xp1);
            m_EffectHead1.Process();
            m_EffectHead1.Show(GetEffectHeadPos(0, 200));
        }
        if (m_ImgHead2.IsWindowEnabled())
        {
            m_ImgHeadBack2.Show(m_Pnt.x, m_Pnt.y);
            m_ImgHead2.Show(m_Pnt.x, m_Pnt.y);
            m_StaName2.Show (m_Pnt.x, m_Pnt.y);
            m_ProgressHp2.Show (m_Pnt.x, m_Pnt.y, m_Hp2, m_Hp2);
            m_ProgressExp2.Show (m_Pnt.x, m_Pnt.y, m_Exp2, m_Exp2);
            m_ProgressXp2.Show (m_Pnt.x, m_Pnt.y, m_Xp2, m_Xp2);
            m_EffectHead2.Process();
            m_EffectHead2.Show(GetEffectHeadPos(170, 200));
        }
        if (m_ImgHead3.IsWindowEnabled())
        {
            m_ImgHeadBack3.Show(m_Pnt.x, m_Pnt.y);
            m_ImgHead3.Show(m_Pnt.x, m_Pnt.y);
            m_StaName3.Show (m_Pnt.x, m_Pnt.y);
            m_ProgressHp3.Show (m_Pnt.x, m_Pnt.y, m_Hp3, m_Hp3);
            m_ProgressExp3.Show (m_Pnt.x, m_Pnt.y, m_Exp3, m_Exp3);
            m_ProgressXp3.Show (m_Pnt.x, m_Pnt.y, m_Xp3, m_Xp3);
            m_EffectHead3.Process();
            m_EffectHead3.Show(GetEffectHeadPos(340, 300));
        }
        if (m_ImgHead4.IsWindowEnabled())
        {
            m_ImgHeadBack4.Show(m_Pnt.x, m_Pnt.y);
            m_ImgHead4.Show(m_Pnt.x, m_Pnt.y);
            m_StaName4.Show (m_Pnt.x, m_Pnt.y);
            m_ProgressHp4.Show (m_Pnt.x, m_Pnt.y, m_Hp4, m_Hp4);
            m_ProgressExp4.Show (m_Pnt.x, m_Pnt.y, m_Exp4, m_Exp4);
            m_ProgressXp4.Show (m_Pnt.x, m_Pnt.y, m_Xp4, m_Xp4);
            m_EffectHead4.Process();
            m_EffectHead4.Show(GetEffectHeadPos(510, 300));
        }
        switch(CDlgMonsterHeads::ShowProgressInfo ())
        {
        case 1:
            m_StaHp1.Show (m_Pnt.x, m_Pnt.y);
            break;
        case 2:
            m_StaXp1.Show (m_Pnt.x, m_Pnt.y);
            break;
        case 3:
            m_StaExp1.Show (m_Pnt.x, m_Pnt.y);
            break;
        case 4:
            m_StaHp2.Show (m_Pnt.x, m_Pnt.y);
            break;
        case 5:
            m_StaXp2.Show (m_Pnt.x, m_Pnt.y);
            break;
        case 6:
            m_StaExp2.Show (m_Pnt.x, m_Pnt.y);
            break;
        case 7:
            m_StaHp3.Show (m_Pnt.x, m_Pnt.y);
            break;
        case 8:
            m_StaXp3.Show (m_Pnt.x, m_Pnt.y);
            break;
        case 9:
            m_StaExp3.Show (m_Pnt.x, m_Pnt.y);
            break;
        case 10:
            m_StaHp4.Show (m_Pnt.x, m_Pnt.y);
            break;
        case 11:
            m_StaXp4.Show (m_Pnt.x, m_Pnt.y);
            break;
        case 12:
            m_StaExp4.Show (m_Pnt.x, m_Pnt.y);
            break;
        default:
            break;
        }
        if (m_BtnCall1.IsWindowEnabled ())
        {
            m_BtnCall1.Show (m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnCall2.IsWindowEnabled ())
        {
            m_BtnCall2.Show (m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnCall3.IsWindowEnabled ())
        {
            m_BtnCall3.Show (m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnCall4.IsWindowEnabled ())
        {
            m_BtnCall4.Show (m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnAttach1.IsWindowEnabled ())
        {
            m_BtnAttach1.Show (m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnAttach2.IsWindowEnabled ())
        {
            m_BtnAttach2.Show (m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnAttach3.IsWindowEnabled ())
        {
            m_BtnAttach3.Show (m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnAttach4.IsWindowEnabled ())
        {
            m_BtnAttach4.Show (m_Pnt.x, m_Pnt.y);
        }
    }
}

void CDlgMonsterHeads::SetMonsterHeads(OBJID idMonster)
{
    if (!this->IsWindowEnabled ())
    {
        CDlgMonsterHeads::EnableWindow ();
    }
    BOOL bShow = TRUE;
    for(int i = m_setMonsterHead.size () - 1; i >= 0; i--)
    {
        if (m_setMonsterHead[i] == idMonster)
        {
            m_setMonsterHead.erase(m_setMonsterHead.begin() + i);
            bShow = FALSE;
        }
    }
    if (bShow)
    {
        m_setMonsterHead.push_back (idMonster);
    }
    FlashMonsterHeads ();
}

void CDlgMonsterHeads::ResetMonsterHeads()
{
    m_BtnAttach1.EnableWindow (false);
    m_BtnAttach2.EnableWindow (false);
    m_BtnAttach3.EnableWindow (false);
    m_BtnAttach4.EnableWindow (false);
    m_BtnCall1.EnableWindow (false);
    m_BtnCall2.EnableWindow (false);
    m_BtnCall3.EnableWindow (false);
    m_BtnCall4.EnableWindow (false);
    m_ImgHead1.EnableWindow (false);
    m_ImgHead1.RemoveImage ();
    m_ImgHead2.EnableWindow (false);
    m_ImgHead2.RemoveImage ();
    m_ImgHead3.EnableWindow (false);
    m_ImgHead3.RemoveImage ();
    m_ImgHead4.EnableWindow (false);
    m_ImgHead4.RemoveImage ();
    m_bHeadClick1 = FALSE;
    m_bHeadClick2 = FALSE;
    m_bHeadClick3 = FALSE;
    m_bHeadClick4 = FALSE;
}

void CDlgMonsterHeads::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgMonsterHeads::GetParent()->PostMessage( WM_LBUTTONDOWN, nFlags, lParam ) ;
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgMonsterHeads::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgMonsterHeads::GetParent()->PostMessage( WM_LBUTTONUP, nFlags, lParam ) ;
    CDialog::OnLButtonUp(nFlags, point);
}

void CDlgMonsterHeads::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgMonsterHeads::GetParent()->PostMessage( WM_MOUSEMOVE, nFlags, lParam ) ;
    CDialog::OnMouseMove(nFlags, point);
}

void CDlgMonsterHeads::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgMonsterHeads::GetParent()->PostMessage( WM_RBUTTONDOWN, nFlags, lParam ) ;
    CDialog::OnRButtonDown(nFlags, point);
}

void CDlgMonsterHeads::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgMonsterHeads::GetParent()->PostMessage( WM_RBUTTONUP, nFlags, lParam ) ;
    CDialog::OnRButtonUp(nFlags, point);
}

void CDlgMonsterHeads::OnBtnAttach1()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (m_ImgHead1.GetImageLoadState ())
    {
        OBJID idMonster = m_ImgHead1.GetImageRealID();
        if (g_objHero.GetAttachPetId () == idMonster)
        {
            g_objHero.UnAttachEudemon (idMonster);
        }
        else if (g_objHero.GetPetByItemID (idMonster) != NULL && g_objHero.GetAttachPetId () != idMonster)
        {
            DelCallMonster(idMonster);
            g_objHero.AttachEudemon (idMonster);
        }
    }
}

void CDlgMonsterHeads::OnBtnAttach2()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (m_ImgHead2.GetImageLoadState ())
    {
        OBJID idMonster = m_ImgHead2.GetImageRealID();
        if (g_objHero.GetAttachPetId () == idMonster)
        {
            g_objHero.UnAttachEudemon (idMonster);
        }
        else if (g_objHero.GetPetByItemID (idMonster) != NULL && g_objHero.GetAttachPetId () != idMonster)
        {
            DelCallMonster(idMonster);
            g_objHero.AttachEudemon (idMonster);
        }
    }
}

void CDlgMonsterHeads::OnBtnAttach3()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (m_ImgHead3.GetImageLoadState ())
    {
        OBJID idMonster = m_ImgHead3.GetImageRealID();
        if (g_objHero.GetAttachPetId () == idMonster)
        {
            g_objHero.UnAttachEudemon (idMonster);
        }
        else if (g_objHero.GetPetByItemID (idMonster) != NULL && g_objHero.GetAttachPetId () != idMonster)
        {
            DelCallMonster(idMonster);
            g_objHero.AttachEudemon (idMonster);
        }
    }
}

void CDlgMonsterHeads::OnBtnAttach4()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (m_ImgHead4.GetImageLoadState ())
    {
        OBJID idMonster = m_ImgHead4.GetImageRealID();
        if (g_objHero.GetAttachPetId () == idMonster)
        {
            g_objHero.UnAttachEudemon (idMonster);
        }
        else if (g_objHero.GetPetByItemID (idMonster) != NULL && g_objHero.GetAttachPetId () != idMonster)
        {
            DelCallMonster(idMonster);
            g_objHero.AttachEudemon (idMonster);
        }
    }
}

void CDlgMonsterHeads::OnBtnCall1()
{
    // TODO: Add your control notification handler code here
    if (m_ImgHead1.GetImageLoadState ())
    {
        OBJID idMonster = m_ImgHead1.GetImageRealID();
        if (g_objHero.GetPetByItemID (idMonster))
        {
            DelCallMonster(idMonster);
            DXPlaySound ( "Sound/Monster_Recede.wav" ) ;
            CDlgMonsterHeads::KillMonster (idMonster);
        }
        else
        {
            AddCallMonster(idMonster);
            DXPlaySound ( "Sound/Monster_out.wav" ) ;
            CDlgMonsterHeads::CallMonster (idMonster);
        }
    }
}

void CDlgMonsterHeads::OnBtnCall2()
{
    // TODO: Add your control notification handler code here
    if (m_ImgHead2.GetImageLoadState ())
    {
        OBJID idMonster = m_ImgHead2.GetImageRealID();
        if (g_objHero.GetPetByItemID (idMonster))
        {
            DelCallMonster(idMonster);
            CDlgMonsterHeads::KillMonster (idMonster);
            DXPlaySound ( "Sound/Monster_Recede.wav" ) ;
        }
        else
        {
            AddCallMonster(idMonster);
            CDlgMonsterHeads::CallMonster (idMonster);
            DXPlaySound ( "Sound/Monster_out.wav" ) ;
        }
    }
}

void CDlgMonsterHeads::OnBtnCall3()
{
    // TODO: Add your control notification handler code here
    if (m_ImgHead3.GetImageLoadState ())
    {
        OBJID idMonster = m_ImgHead3.GetImageRealID();
        if (g_objHero.GetPetByItemID (idMonster))
        {
            DelCallMonster(idMonster);
            CDlgMonsterHeads::KillMonster (idMonster);
            DXPlaySound ( "Sound/Monster_Recede.wav" ) ;
        }
        else
        {
            AddCallMonster(idMonster);
            CDlgMonsterHeads::CallMonster (idMonster);
            DXPlaySound ( "Sound/Monster_out.wav" ) ;
        }
    }
}

void CDlgMonsterHeads::OnBtnCall4()
{
    // TODO: Add your control notification handler code here
    if (m_ImgHead4.GetImageLoadState ())
    {
        OBJID idMonster = m_ImgHead4.GetImageRealID();
        if (g_objHero.GetPetByItemID (idMonster))
        {
            DelCallMonster(idMonster);
            CDlgMonsterHeads::KillMonster (idMonster);
            DXPlaySound ( "Sound/Monster_Recede.wav" ) ;
        }
        else
        {
            AddCallMonster(idMonster);
            CDlgMonsterHeads::CallMonster (idMonster);
            DXPlaySound ( "Sound/Monster_out.wav" ) ;
        }
    }
}

void CDlgMonsterHeads::OnImgHead1()
{
    // TODO: Add your control notification handler code here
    if (m_ImgHead1.GetImageLoadState ())
    {
        OBJID idMonster = m_ImgHead1.GetImageRealID();
        CItem* pMonster = g_objHero.GetItem (idMonster);
        if (pMonster)
        {
            CDlgMonsterHeads::GetParent ()->PostMessage(WM_MY_MESSAGE, ON_MONSTER_OPTION, idMonster);
            //			if ( pMonster->GetWarGhostExp () <= 0)
            //			{
            //				g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10729));
            //				return;
            //			}
        }
    }
}

void CDlgMonsterHeads::OnImgHead2()
{
    // TODO: Add your control notification handler code here
    if (m_ImgHead2.GetImageLoadState ())
    {
        OBJID idMonster = m_ImgHead2.GetImageRealID();
        CItem* pMonster = g_objHero.GetItem (idMonster);
        if (pMonster)
        {
            //			if ( pMonster->GetWarGhostExp () <= 0)
            //			{
            //				g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10729));
            //				return;
            //			}
            CDlgMonsterHeads::GetParent ()->PostMessage(WM_MY_MESSAGE, ON_MONSTER_OPTION, idMonster);
        }
    }
}

void CDlgMonsterHeads::OnImgHead3()
{
    // TODO: Add your control notification handler code here
    if (m_ImgHead3.GetImageLoadState ())
    {
        OBJID idMonster = m_ImgHead3.GetImageRealID();
        CItem* pMonster = g_objHero.GetItem (idMonster);
        if (pMonster)
        {
            CDlgMonsterHeads::GetParent ()->PostMessage(WM_MY_MESSAGE, ON_MONSTER_OPTION, idMonster);
            //			if ( pMonster->GetWarGhostExp () <= 0)
            //			{
            //				g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10729));
            //				return;
            //			}
        }
    }
}

void CDlgMonsterHeads::OnImgHead4()
{
    // TODO: Add your control notification handler code here
    if (m_ImgHead4.GetImageLoadState ())
    {
        OBJID idMonster = m_ImgHead4.GetImageRealID();
        CItem* pMonster = g_objHero.GetItem (idMonster);
        if (pMonster)
        {
            CDlgMonsterHeads::GetParent ()->PostMessage(WM_MY_MESSAGE, ON_MONSTER_OPTION, idMonster);
        }
    }
}

void CDlgMonsterHeads::FlashMonsterHeads()
{
    ResetMonsterHeads();
    int nAmount = m_setMonsterHead.size ();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        CItem* pMonster = g_objHero.GetItem (m_setMonsterHead[i]);
        if (pMonster == NULL)
        {
            m_setMonsterHead.erase (m_setMonsterHead.begin() + i);
        }
    }
    nAmount = m_setMonsterHead.size ();
    if (nAmount <= 0)
    {
        CDlgMonsterHeads::EnableWindow (false);
    }
    CRect rect;
    CDlgMonsterHeads::GetWindowRect (rect);
    CDlgMonsterHeads::MoveWindow (_SCR_WIDTH - 162 * nAmount - 130 ,
                                  0, 162 * nAmount, rect.Height (), false);
    for(int i = 0; i < nAmount; i++)
    {
        CItem* pMonster = g_objHero.GetItem (m_setMonsterHead[i]);
        CPet* pPet = g_objHero.GetPetByItemID(pMonster->GetID());
        if (pMonster != NULL)
        {
            switch(i)
            {
            case 0:
                {
                    m_ImgHead1.EnableWindow ();
                    m_StaName1.SetWindowText (pMonster->GetMonsterName ());
                    if (pPet)
                    {
                        if (!pPet->TestStatus(USERSTATUS_GHOST))
                        {
                            m_ImgHead1.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), true);
                        }
                        else
                        {
                            m_ImgHead1.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), false);
                        }
                    }
                    else
                    {
                        if (pMonster->GetWarGhostExp() > 0 )
                        {
                            m_ImgHead1.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), true);
                        }
                        else
                        {
                            m_ImgHead1.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), false);
                        }
                    }
                }
                break;
            case 1:
                {
                    m_ImgHead2.EnableWindow ();
                    m_StaName2.SetWindowText (pMonster->GetMonsterName ());
                    if (pPet)
                    {
                        if (!pPet->TestStatus(USERSTATUS_GHOST))
                        {
                            m_ImgHead2.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), true);
                        }
                        else
                        {
                            m_ImgHead2.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), false);
                        }
                    }
                    else
                    {
                        if (pMonster->GetWarGhostExp() > 0 )
                        {
                            m_ImgHead2.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), true);
                        }
                        else
                        {
                            m_ImgHead2.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), false);
                        }
                    }
                }
                break;
            case 2:
                {
                    m_ImgHead3.EnableWindow ();
                    m_StaName3.SetWindowText (pMonster->GetMonsterName ());
                    if (pPet)
                    {
                        if (!pPet->TestStatus(USERSTATUS_GHOST))
                        {
                            m_ImgHead3.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), true);
                        }
                        else
                        {
                            m_ImgHead3.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), false);
                        }
                    }
                    else
                    {
                        if (pMonster->GetWarGhostExp() > 0 )
                        {
                            m_ImgHead3.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), true);
                        }
                        else
                        {
                            m_ImgHead3.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), false);
                        }
                    }
                }
                break;
            case 3:
                {
                    m_ImgHead4.EnableWindow ();
                    m_StaName4.SetWindowText (pMonster->GetMonsterName ());
                    if (pPet)
                    {
                        if (!pPet->TestStatus(USERSTATUS_GHOST))
                        {
                            m_ImgHead4.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), true);
                        }
                        else
                        {
                            m_ImgHead4.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), false);
                        }
                    }
                    else
                    {
                        if (pMonster->GetWarGhostExp() > 0 )
                        {
                            m_ImgHead4.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), true);
                        }
                        else
                        {
                            m_ImgHead4.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), false);
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
    }
}

BOOL CDlgMonsterHeads::CallMonster(OBJID idMonster)
{
    if (g_objHero.IsDead () )
    {
        return FALSE;
    }
    CItem* pMonster = g_objHero.GetItem (idMonster);
    if (pMonster && g_objHero.GetPetByItemID (idMonster) == NULL)
    {
        if (pMonster->GetAmountLimit() >= g_objHero.GetLev() + 10)
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10808));	//10808=幻兽等级太高，您暂时无法召唤！
            return FALSE;
        }
        g_objHero.CallEudenmon (idMonster);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void CDlgMonsterHeads::ReCallMonsters ()
{
    if (g_objHero.IsDead ())
    {
        return;
    }
    for(int i = 0; i < 4; i++)
    {
        OBJID idMonster = m_setMonsterCall[i];
        if (idMonster != 0)
        {
            CItem* pMonster = g_objHero.GetItem (idMonster);
            if (pMonster && g_objHero.GetPetByItemID (idMonster) == NULL)
            {
                if (pMonster->GetAmountLimit() >= g_objHero.GetLev() + 10)
                {
                    g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10808));	//10808=幻兽等级太高，您暂时无法召唤！
                    continue;
                }
                g_objHero.CallEudenmon (idMonster);
            }
        }
    }
}

void CDlgMonsterHeads::ResetCallMonster()
{
    m_setMonsterCall[0] = 0;
    m_setMonsterCall[1] = 0;
    m_setMonsterCall[2] = 0;
    m_setMonsterCall[3] = 0;
}

BOOL CDlgMonsterHeads::KillMonster(OBJID idMonster)
{
    CItem* pMonster = g_objHero.GetItem (idMonster);
    if (pMonster && g_objHero.GetPetByItemID (idMonster) != NULL)
    {
        g_objHero.KillEudenmon (idMonster);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


void CDlgMonsterHeads::FlashMonsterStatus()
{
    int nAmount = m_setMonsterHead.size ();
    if (nAmount <= 0)
    {
        return;
    }
    for(int i = 0; i < nAmount; i++)
    {
        CItem* pMonster = g_objHero.GetItem (m_setMonsterHead[i]);
        if (pMonster != NULL)
        {
            switch(i)
            {
            case 0:
                {
                    m_StaName1.SetWindowText (pMonster->GetMonsterName ());
                    m_Hp1 = pMonster->GetWarGhostExp ();
                    m_Exp1 = pMonster->GetGemAtkType ();
                    m_ProgressHp1.ResetValue (0, pMonster->GetMonsterMaxLife ());
                    m_ProgressExp1.ResetValue (0, g_objHero.GetPetLevelExp(pMonster->GetAmountLimit()));
                    CPet* pPet = g_objHero.GetPetByItemID (pMonster->GetID ());
                    if (pPet)
                    {
                        m_Xp1 = pPet->GetPotential ();
                        m_BtnCall1.ChangeImage ("Button572");
                        if (!m_BtnCall1.IsWindowEnabled())
                        {
                            m_BtnCall1.EnableWindow ();
                        }
                        if (g_objHero.GetAttachPetId () != pMonster->GetID () && pMonster->GetTypeID () % 10 == 2) //是否二次进化
                        {
                            if (!m_BtnAttach1.IsWindowEnabled())
                            {
                                m_BtnAttach1.EnableWindow ();
                            }
                            m_BtnAttach1.ChangeImage("Button571");
                        }
                    }
                    else
                    {
                        m_Xp1 = 0;
                        if (g_objHero.GetAttachPetId () == pMonster->GetID())
                        {
                            m_BtnAttach1.ChangeImage("Button573");
                            m_BtnAttach1.EnableWindow ();
                            m_BtnCall1.EnableWindow (false);
                        }
                        else
                        {
                            m_BtnAttach1.EnableWindow (false);
                            m_BtnCall1.EnableWindow ();
                            m_BtnCall1.ChangeImage ("Button570");
                        }
                    }
                    if (pMonster->GetWarGhostExp() > 0 )
                    {
                        if (pMonster->GetWarGhostExp() <= (DWORD)(pMonster->GetMonsterMaxLife() * 3 / 10)
                                && pMonster->GetWarGhostExp() > (DWORD)(pMonster->GetMonsterMaxLife() * 2 / 10))
                        {
                            if (!m_EffectHead1.TestEffect("baby_d1"))
                            {
                                m_EffectHead1.Add("baby_d1");    //警告光xiao
                            }
                            m_ProgressHp1.SetProgressID("Progress5841");		//浅红色
                        }
                        else if (pMonster->GetWarGhostExp() <= (DWORD)(pMonster->GetMonsterMaxLife() * 2 / 10)
                                 && pMonster->GetWarGhostExp() > (DWORD)(pMonster->GetMonsterMaxLife() / 10))
                        {
                            m_ProgressHp1.SetProgressID("Progress5842");		//暗红色
                        }
                        else if (pMonster->GetWarGhostExp() <= (DWORD)(pMonster->GetMonsterMaxLife() / 10))
                        {
                            if (!m_EffectHead1.TestEffect("baby_d1"))
                            {
                                m_EffectHead1.Add("baby_d1");    //警告光xiao
                            }
                        }
                        else
                        {
                            m_EffectHead1.Clear();					//clear effect
                            m_ProgressHp1.SetProgressID("Progress584");		//NORMAL
                        }
                    }
                    else
                    {
                        m_EffectHead1.Clear();					//clear effect
                    }
                    if (!pMonster->IsMonsterDie())
                    {
                        m_ImgHead1.InsertImage (NULL, pMonster->GetID (), pMonster->GetTypeID (), true);
                    }
                    else
                    {
                        m_ImgHead1.InsertImage (NULL, pMonster->GetID (), pMonster->GetTypeID (), false);
                    }
                    m_ProgressXp1.ResetValue (0, 100);
                    char szInfo[32] = "";
                    sprintf (szInfo, "%d/%d", m_Hp1, pMonster->GetMonsterMaxLife ());
                    m_StaHp1.SetWindowText (szInfo);
                    sprintf (szInfo, "%d/%d", m_Xp1, 100);
                    m_StaXp1.SetWindowText (szInfo);
                    DWORD nPetExp = g_objHero.GetPetLevelExp(pMonster->GetAmountLimit());
                    if (nPetExp == 0)
                    {
                        int idMsg = 100168;
                        strcpy(szInfo, (char*)g_objGameDataSet.GetStr(idMsg));
                    }
                    else
                    {
                        double dRate = (double)pMonster->GetGemAtkType() * 100 / nPetExp;
                        sprintf(szInfo, "%0.2f%%", dRate);
                    }
                    m_StaExp1.SetWindowText (szInfo);
                }
                break;
            case 1:
                {
                    m_StaName2.SetWindowText (pMonster->GetMonsterName ());
                    m_Hp2 = pMonster->GetWarGhostExp ();
                    m_Exp2 = pMonster->GetGemAtkType ();
                    m_ProgressHp2.ResetValue (0, pMonster->GetMonsterMaxLife ());
                    m_ProgressExp2.ResetValue (0, g_objHero.GetPetLevelExp(pMonster->GetAmountLimit()));
                    CPet* pPet = g_objHero.GetPetByItemID (pMonster->GetID ());
                    if (pPet)
                    {
                        m_Xp2 = pPet->GetPotential ();
                        m_BtnCall2.ChangeImage ("Button572");
                        m_BtnCall2.EnableWindow ();
                        if (g_objHero.GetAttachPetId () != pMonster->GetID () && pMonster->GetTypeID () % 10 == 2) //是否二次进化
                        {
                            m_BtnAttach2.EnableWindow ();
                            m_BtnAttach2.ChangeImage("Button571");
                        }
                    }
                    else
                    {
                        m_Xp2 = 0;
                        if (g_objHero.GetAttachPetId () == pMonster->GetID())
                        {
                            m_BtnAttach2.ChangeImage("Button573");
                            m_BtnAttach2.EnableWindow ();
                            m_BtnCall2.EnableWindow (false);
                        }
                        else
                        {
                            m_BtnAttach2.EnableWindow (false);
                            m_BtnCall2.ChangeImage ("Button570");
                            m_BtnCall2.EnableWindow ();
                        }
                    }
                    if (pMonster->GetWarGhostExp() > 0 )
                    {
                        if (pMonster->GetWarGhostExp() <= (DWORD)(pMonster->GetMonsterMaxLife() * 3 / 10)
                                && pMonster->GetWarGhostExp() > (DWORD)(pMonster->GetMonsterMaxLife() * 2 / 10))
                        {
                            if (!m_EffectHead2.TestEffect("baby_d1"))
                            {
                                m_EffectHead2.Add("baby_d1");    //警告光xiao
                            }
                            m_ProgressHp2.SetProgressID("Progress5841");		//浅红色
                        }
                        else if (pMonster->GetWarGhostExp() <= (DWORD)(pMonster->GetMonsterMaxLife() * 2 / 10)
                                 && pMonster->GetWarGhostExp() > (DWORD)(pMonster->GetMonsterMaxLife() / 10))
                        {
                            m_ProgressHp2.SetProgressID("Progress5842");		//暗红色
                        }
                        else if (pMonster->GetWarGhostExp() <= (DWORD)(pMonster->GetMonsterMaxLife() / 10))
                        {
                            if (!m_EffectHead2.TestEffect("baby_d1"))
                            {
                                m_EffectHead2.Add("baby_d1");    //警告光xiao
                            }
                        }
                        else
                        {
                            m_EffectHead2.Clear();					//clear effect
                            m_ProgressHp2.SetProgressID("Progress584");		//NORMAL
                        }
                    }
                    else
                    {
                        m_EffectHead2.Clear();					//clear effect
                    }
                    if (!pMonster->IsMonsterDie())
                    {
                        m_ImgHead2.InsertImage (NULL, pMonster->GetID (), pMonster->GetTypeID (), true);
                    }
                    else
                    {
                        m_ImgHead2.InsertImage (NULL, pMonster->GetID (), pMonster->GetTypeID (), false);
                    }
                    m_ProgressXp2.ResetValue (0, 100);
                    char szInfo[32] = "";
                    sprintf (szInfo, "%d/%d", m_Hp2, pMonster->GetMonsterMaxLife ());
                    m_StaHp2.SetWindowText (szInfo);
                    sprintf (szInfo, "%d/%d", m_Xp2, 100);
                    m_StaXp2.SetWindowText (szInfo);
                    DWORD nPetExp = g_objHero.GetPetLevelExp(pMonster->GetAmountLimit());
                    if (nPetExp == 0)
                    {
                        int idMsg = 100168;
                        strcpy(szInfo, (char*)g_objGameDataSet.GetStr(idMsg));
                    }
                    else
                    {
                        double dRate = (double)pMonster->GetGemAtkType() * 100 / nPetExp;
                        sprintf(szInfo, "%0.2f%%", dRate);
                    }
                    m_StaExp2.SetWindowText (szInfo);
                }
                break;
            case 2:
                {
                    m_StaName3.SetWindowText (pMonster->GetMonsterName ());
                    m_Hp3 = pMonster->GetWarGhostExp ();
                    m_Exp3 = pMonster->GetGemAtkType ();
                    m_ProgressHp3.ResetValue (0, pMonster->GetMonsterMaxLife ());
                    m_ProgressExp3.ResetValue (0, g_objHero.GetPetLevelExp(pMonster->GetAmountLimit()));
                    CPet* pPet = g_objHero.GetPetByItemID (pMonster->GetID ());
                    if (pPet)
                    {
                        m_Xp3 = pPet->GetPotential ();
                        m_BtnCall3.ChangeImage ("Button572");
                        m_BtnCall3.EnableWindow ();
                        if (g_objHero.GetAttachPetId () != pMonster->GetID () && pMonster->GetTypeID () % 10 == 2) //是否二次进化
                        {
                            m_BtnAttach3.ChangeImage("Button571");
                            m_BtnAttach3.EnableWindow ();
                        }
                    }
                    else
                    {
                        m_Xp3 = 0;
                        if (g_objHero.GetAttachPetId () == pMonster->GetID())
                        {
                            m_BtnAttach3.EnableWindow ();
                            m_BtnAttach3.ChangeImage("Button573");
                            m_BtnCall3.EnableWindow (false);
                        }
                        else
                        {
                            m_BtnAttach3.EnableWindow (false);
                            m_BtnCall3.EnableWindow ();
                            m_BtnCall3.ChangeImage ("Button570");
                        }
                    }
                    if (pMonster->GetWarGhostExp() > 0 )
                    {
                        if (pMonster->GetWarGhostExp() <= (DWORD)(pMonster->GetMonsterMaxLife() * 3 / 10)
                                && pMonster->GetWarGhostExp() > (DWORD)(pMonster->GetMonsterMaxLife() * 2 / 10))
                        {
                            if (!m_EffectHead3.TestEffect("baby_d1"))
                            {
                                m_EffectHead3.Add("baby_d1");    //警告光xiao
                            }
                            m_ProgressHp3.SetProgressID("Progress5841");		//浅红色
                        }
                        else if (pMonster->GetWarGhostExp() <= (DWORD)(pMonster->GetMonsterMaxLife() * 2 / 10)
                                 && pMonster->GetWarGhostExp() >(DWORD)( pMonster->GetMonsterMaxLife() / 10))
                        {
                            m_ProgressHp3.SetProgressID("Progress5842");		//暗红色
                        }
                        else if (pMonster->GetWarGhostExp() <=(DWORD)( pMonster->GetMonsterMaxLife() / 10))
                        {
                            if (!m_EffectHead3.TestEffect("baby_d1"))
                            {
                                m_EffectHead3.Add("baby_d1");    //警告光xiao
                            }
                        }
                        else
                        {
                            m_EffectHead3.Clear();					//clear effect
                            m_ProgressHp3.SetProgressID("Progress584");		//NORMAL
                        }
                    }
                    else
                    {
                        m_EffectHead3.Clear();					//clear effect
                    }
                    if (!pMonster->IsMonsterDie())
                    {
                        m_ImgHead3.InsertImage (NULL, pMonster->GetID (), pMonster->GetTypeID (), true);
                    }
                    else
                    {
                        m_ImgHead3.InsertImage (NULL, pMonster->GetID (), pMonster->GetTypeID (), false);
                    }
                    m_ProgressXp3.ResetValue (0, 100);
                    char szInfo[32] = "";
                    sprintf (szInfo, "%d/%d", m_Hp3, pMonster->GetMonsterMaxLife ());
                    m_StaHp3.SetWindowText (szInfo);
                    sprintf (szInfo, "%d/%d", m_Xp3, 100);
                    m_StaXp3.SetWindowText (szInfo);
                    DWORD nPetExp = g_objHero.GetPetLevelExp(pMonster->GetAmountLimit());
                    if (nPetExp == 0)
                    {
                        int idMsg = 100168;
                        strcpy(szInfo, (char*)g_objGameDataSet.GetStr(idMsg));
                    }
                    else
                    {
                        double dRate = (double)pMonster->GetGemAtkType() * 100 / nPetExp;
                        sprintf(szInfo, "%0.2f%%", dRate);
                    }
                    m_StaExp3.SetWindowText (szInfo);
                }
                break;
            case 3:
                {
                    m_StaName4.SetWindowText (pMonster->GetMonsterName ());
                    m_Hp4 = pMonster->GetWarGhostExp ();
                    m_Exp4 = pMonster->GetGemAtkType ();
                    m_ProgressHp4.ResetValue (0, pMonster->GetMonsterMaxLife ());
                    m_ProgressExp4.ResetValue (0, g_objHero.GetPetLevelExp(pMonster->GetAmountLimit()));
                    CPet* pPet = g_objHero.GetPetByItemID (pMonster->GetID ());
                    if (pPet)
                    {
                        m_Xp4 = pPet->GetPotential ();
                        m_BtnCall4.ChangeImage ("Button572");
                        m_BtnCall4.EnableWindow ();
                        if (g_objHero.GetAttachPetId () != pMonster->GetID () && pMonster->GetTypeID () % 10 == 2) //是否二次进化
                        {
                            m_BtnAttach4.ChangeImage("Button571");
                            m_BtnAttach4.EnableWindow ();
                        }
                    }
                    else
                    {
                        m_Xp4 = 0;
                        if (g_objHero.GetAttachPetId () == pMonster->GetID())
                        {
                            m_BtnAttach4.EnableWindow ();
                            m_BtnAttach4.ChangeImage("Button573");
                            m_BtnCall4.EnableWindow (false);
                        }
                        else
                        {
                            m_BtnAttach4.EnableWindow (false);
                            m_BtnCall4.EnableWindow ();
                            m_BtnCall4.ChangeImage ("Button570");
                        }
                    }
                    if (pMonster->GetWarGhostExp() > 0 )
                    {
                        if (pMonster->GetWarGhostExp() <=(DWORD)( pMonster->GetMonsterMaxLife() * 3 / 10)
                                && pMonster->GetWarGhostExp() > (DWORD)(pMonster->GetMonsterMaxLife() * 2 / 10))
                        {
                            if (!m_EffectHead4.TestEffect("baby_d1"))
                            {
                                m_EffectHead4.Add("baby_d1");    //警告光xiao
                            }
                            m_ProgressHp4.SetProgressID("Progress5841");		//浅红色
                        }
                        else if (pMonster->GetWarGhostExp() <= (DWORD)(pMonster->GetMonsterMaxLife() * 2 / 10)
                                 && pMonster->GetWarGhostExp() > (DWORD)(pMonster->GetMonsterMaxLife() / 10))
                        {
                            m_ProgressHp4.SetProgressID("Progress5842");		//暗红色
                        }
                        else if (pMonster->GetWarGhostExp() <=(DWORD)( pMonster->GetMonsterMaxLife() / 10))
                        {
                            if (!m_EffectHead4.TestEffect("baby_d1"))
                            {
                                m_EffectHead4.Add("baby_d1");    //警告光xiao
                            }
                        }
                        else
                        {
                            m_EffectHead4.Clear();					//clear effect
                        }
                    }
                    else
                    {
                        m_EffectHead4.Clear();					//clear effect
                        m_EffectHead4.Add("petdie");			//add pet die effect
                    }
                    m_ProgressXp4.ResetValue (0, 100);
                    if (!pMonster->IsMonsterDie())
                    {
                        m_ImgHead4.InsertImage (NULL, pMonster->GetID (), pMonster->GetTypeID (), true);
                    }
                    else
                    {
                        m_ImgHead4.InsertImage (NULL, pMonster->GetID (), pMonster->GetTypeID (), false);
                    }
                    char szInfo[32] = "";
                    sprintf (szInfo, "%d/%d", m_Hp4, pMonster->GetMonsterMaxLife ());
                    m_StaHp4.SetWindowText (szInfo);
                    sprintf (szInfo, "%d/%d", m_Xp4, 100);
                    m_StaXp4.SetWindowText (szInfo);
                    DWORD nPetExp = g_objHero.GetPetLevelExp(pMonster->GetAmountLimit());
                    if (nPetExp == 0)
                    {
                        int idMsg = 100168;
                        strcpy(szInfo, (char*)g_objGameDataSet.GetStr(idMsg));
                    }
                    else
                    {
                        double dRate = (double)pMonster->GetGemAtkType() * 100 / nPetExp;
                        sprintf(szInfo, "%0.2f%%", dRate);
                    }
                    m_StaExp4.SetWindowText (szInfo);
                }
                break;
            default:
                break;
            }
        }
    }
}

void CDlgMonsterHeads::AddEffect(char* szEffect, OBJID idPet)
{
    CPet* pPet = g_objHero.GetPetByID(idPet);
    if (!pPet)
    {
        return;
    }
    int nAmount = m_setMonsterHead.size ();
    if (nAmount <= 0)
    {
        return;
    }
    for(int i = 0; i < nAmount; i++)
    {
        CItem* pMonster = g_objHero.GetItem (m_setMonsterHead[i]);
        if (pPet->GetItemID() == pMonster->GetID())
        {
            switch(i)
            {
            case 0:
                m_EffectHead1.Add(szEffect);
                break;
            case 1:
                m_EffectHead2.Add(szEffect);
                break;
            case 2:
                m_EffectHead3.Add(szEffect);
                break;
            case 3:
                m_EffectHead4.Add(szEffect);
                break;
            default:
                break;
            }
        }
    }
}


int CDlgMonsterHeads::ShowProgressInfo ()
{
    POINT mousePnt ;
    GetCursorPos ( &mousePnt ) ;
    CRect PrgRect[12];
    int nAmount = 0;
    if (m_ImgHead1.GetImageLoadState ())
    {
        m_ProgressHp1.GetWindowRect(PrgRect[0]);
        m_ProgressXp1.GetWindowRect(PrgRect[1]);
        m_ProgressExp1.GetWindowRect (PrgRect[2]);
        nAmount += 3;
    }
    if (m_ImgHead2.GetImageLoadState ())
    {
        m_ProgressHp2.GetWindowRect(PrgRect[3]);
        m_ProgressXp2.GetWindowRect(PrgRect[4]);
        m_ProgressExp2.GetWindowRect (PrgRect[5]);
        nAmount += 3;
    }
    if (m_ImgHead3.GetImageLoadState ())
    {
        m_ProgressHp3.GetWindowRect(PrgRect[6]);
        m_ProgressXp3.GetWindowRect(PrgRect[7]);
        m_ProgressExp3.GetWindowRect (PrgRect[8]);
        nAmount += 3;
    }
    if (m_ImgHead4.GetImageLoadState ())
    {
        m_ProgressHp4.GetWindowRect(PrgRect[9]);
        m_ProgressXp4.GetWindowRect(PrgRect[10]);
        m_ProgressExp4.GetWindowRect (PrgRect[11]);
        nAmount += 3;
    }
    int nIndex = 0;
    for(int i = 0; i < nAmount; i++)
    {
        if (PrgRect[i].PtInRect(mousePnt))
        {
            nIndex = i + 1;
            break;
        }
    }
    return nIndex;
}

void CDlgMonsterHeads::HideCurMonsterBtns (int nIndex)
{
    if (nIndex != 0)
    {
        m_BtnCall1.EnableWindow (false);
        m_BtnAttach1.EnableWindow (false);
        m_bHeadClick1 = false;
    }
    if (nIndex != 1)
    {
        m_BtnCall2.EnableWindow (false);
        m_BtnAttach2.EnableWindow (false);
        m_bHeadClick2 = false;
    }
    if (nIndex != 2)
    {
        m_BtnCall3.EnableWindow (false);
        m_BtnAttach3.EnableWindow (false);
        m_bHeadClick3 = false;
    }
    if (nIndex != 3)
    {
        m_BtnCall4.EnableWindow (false);
        m_BtnAttach4.EnableWindow (false);
        m_bHeadClick4 = false;
    }
}

void CDlgMonsterHeads::AddCallMonster(OBJID idPet)
{
    int i = 0 ;
    for(i = 0; i < 4; i++)
    {
        if (m_setMonsterCall[i] == idPet)
        {
            return;
        }
    }
    for(i = 0; i < 4; i++)
    {
        if (m_setMonsterCall[i] == 0)
        {
            m_setMonsterCall[i] = idPet;
            return;
        }
    }
}

void CDlgMonsterHeads::DelCallMonster(OBJID idPet)
{
    for(int i = 0; i < 4; i++)
    {
        if (m_setMonsterCall[i] == idPet)
        {
            m_setMonsterCall[i] = 0;
            return;
        }
    }
}

void CDlgMonsterHeads::UpdataCallMonster()
{
    for(int i = 0; i < 4; i++)
    {
        if (m_setMonsterCall[i] != 0)
        {
            CItem* pPet  = g_objHero.GetItem(m_setMonsterCall[i]);
            if (pPet && pPet->GetWarGhostExp() > 0)
            {
                continue;
            }
            else
            {
                m_setMonsterCall[i] = 0;
            }
        }
    }
}
