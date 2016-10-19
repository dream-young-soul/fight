
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgMonsterDepot.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgMonsterEvolve.h"
#include "Gamemsg.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterEvolve dialog


CDlgMonsterEvolve::CDlgMonsterEvolve(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMonsterEvolve::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMonsterEvolve)
    m_bShow = false;
    m_Pnt = CPoint(0, 0);
    m_IdMonster = 0;
    m_nEvoMode  = 0;
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgMonsterEvolve::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMonsterEvolve)
    DDX_Control(pDX, IDC_MONSTEREVOLVE_STA_NAME, m_StaMonsterName);
    DDX_Control(pDX, IDC_IMG_BEFOREEVOLVED, m_ImgBeforeEvolve);
    DDX_Control(pDX, IDC_IMG_AFTEREVOLVE, m_ImgAfterEvolve);
    DDX_Control(pDX, IDC_IMG_MONSTER, m_ImgMonster);
    DDX_Control(pDX, IDC_BTN_EVOLVE, m_BtnEvolve);
    DDX_Control(pDX, IDC_BTN_CANCEL, m_BtnCancel);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMonsterEvolve, CDialog)
    //{{AFX_MSG_MAP(CDlgMonsterEvolve)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_IMG_MONSTER, OnBtnMonster)
    ON_BN_CLICKED(IDC_BTN_EVOLVE, OnBtnEvolve)
    ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_DOUBLECLICKED(IDC_IMG_MONSTER, OnDoubleclickedImgMonster)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterEvolve message handlers

LRESULT CDlgMonsterEvolve::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        {
            m_bShow = wParam ;
            if (m_bShow)
            {
                DXPlaySound("Sound/Open.wav");
                m_IdMonster = 0;
                m_idTypeBefore = 0;
                m_idTypeAfter = 0;
                m_3dPetEvoAfter.Reset();
                m_3dPetEvoBefore.Reset();
                m_StaMonsterName.SetWindowText("");
                m_ImgMonster.RemoveImage();
                m_BtnEvolve.EnableWindow(false);
                m_BtnCancel.EnableWindow(false);
            }
            else
            {
                DXPlaySound("Sound/window_close.wav");
            }
        }
        break ;
    case WM_COMMAND:
        {
            if ( wParam == 1 )
            {
                return true;
            }
            if ( wParam == 2)
            {
                CDlgMonsterEvolve::OnBtnClose();
                return true;
            }
        }
        break ;
    case WM_MY_MESSAGE:
        {
            switch(wParam)
            {
            case WM_RBUTTONDOWN:
                if (m_ImgMonster.GetImageLoadState() && !g_bPickUp && m_IdMonster)
                {
                    m_ImgMonster.RemoveImage();
                    m_BtnEvolve.EnableWindow(false);
                    m_BtnCancel.EnableWindow(false);
                    m_StaMonsterName.SetWindowText("");
                    m_IdMonster = 0;
                }
                break;
            default:
                break;
            }
        }
    default:
        break;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgMonsterEvolve::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

BOOL CDlgMonsterEvolve::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgMonsterEvolve::GetWindowRect(rect);
    m_StaMonsterName.Init(rect.left, rect.top);
    m_ImgBeforeEvolve.Init(rect.left, rect.top, NULL, PETFACE_IMAGE);
    m_ImgAfterEvolve.Init(rect.left, rect.top, NULL, PETFACE_IMAGE);
    m_ImgMonster.Init(rect.left, rect.top, NULL, PETFACE_IMAGE);
    m_BtnEvolve.Init(rect.left, rect.top, "Button905");
    m_BtnCancel.Init(rect.left, rect.top, "Button906");
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMonsterEvolve::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgMonsterEvolve::InsertMonster(OBJID idMonster)
{
    if (!m_ImgMonster.GetImageLoadState())
    {
        CItem* pMonster = g_objHero.GetItem(idMonster);
        if (pMonster)
        {
            m_IdMonster = idMonster;
        }
    }
}

void CDlgMonsterEvolve::OnBtnMonster()
{
    // TODO: Add your control notification handler code here
    if (!m_ImgMonster.GetImageLoadState() && g_bPickUp && m_IdMonster)
    {
        m_ImgMonster.RemoveImage();
        CItem* pPet = g_objHero.GetItem(m_IdMonster);
        if (pPet )
        {
            if (!CheckEvoMonster(m_nEvoMode))
            {
                m_IdMonster  = 0;
            }
            else
            {
                m_idTypeBefore = pPet->GetTypeID();
                m_ImgMonster.InsertImage(NULL, pPet->GetID(), pPet->GetTypeID(), true);
                m_StaMonsterName.SetWindowText(pPet->GetMonsterName());
                m_BtnEvolve.EnableWindow();
                m_BtnCancel.EnableWindow();
            }
            CDlgMonsterEvolve::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
            CDlgMonsterEvolve::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
        }
    }
    else if (!m_ImgMonster.GetImageLoadState() && g_bPickUp)
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10819));	//错误，请放入需要进化的幻兽!
    }
}

void CDlgMonsterEvolve::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    CDlgMonsterEvolve::EnableWindow(false);
}

BOOL CDlgMonsterEvolve::CheckEvoMonster(OBJID idMonster)
{
    CItem* pPet = g_objHero.GetItem(m_IdMonster);
    if (pPet == NULL)
    {
        return false;
    }
    if (pPet->GetWarGhostExp() <= 0)
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10803));		//幻兽已经死亡，不能进化幻兽
        return false;
    }
    if (m_nEvoMode < 3)
    {
        if (pPet->GetTypeID() % 100 == 0 && pPet->GetAmountLimit() < 20 )
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10804));		//您的幻兽等级过低，无法进化
            return false;
        }
        else if (( pPet->GetTypeID() % 100 == 1 || pPet->GetTypeID() % 100 == 2 ) && pPet->GetAmountLimit() < 40 )
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10805));		//该幻兽已经经过一次进化
            return false;
        }
        else if ((pPet->GetTypeID() % 100 == 1 || pPet->GetTypeID() % 100 == 2 ) && pPet->GetAmountLimit() >= 40)
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10806));		//该幻兽可以进行二次进化
            return false;
        }
        else if (pPet->GetTypeID() % 100 > 10 )
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10807));		//当前幻兽已经为最终形态，不需要在进化
            return false;
        }
    }
    else if (m_nEvoMode > 2)
    {
        if (pPet->GetTypeID() % 100 == 0)
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10809));		//的幻兽还未经过一次进化，无法二次进化
            return false;
        }
        else if (pPet->GetTypeID() % 100 > 10 )
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10807));		//当前幻兽已经为最终形态，不需要在进化
            return false;
        }
        else if (pPet->GetAmountLimit() < 40 )
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10810));		//您的幻兽等级过低，无法二次进化
            return false;
        }
        else if (pPet->GetTypeID() % 100 == 1 &&  m_nEvoMode > 6 )
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10810 + m_nEvoMode - 2));		//圣兽无法进化成为魔，请重新选择进化类型
            return false;
        }
        else if (pPet->GetTypeID() % 100 == 2 && m_nEvoMode < 7 )
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10810 + m_nEvoMode - 2));		//魔兽无法进化成为神，请重新选择进化类型
            return false;
        }
    }
    m_idTypeAfter = pPet->IsMonsterCanEvolve(m_nEvoMode);
    if (m_idTypeAfter == 0)
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10821));		//没有能够进化的数据，不能进化幻兽
        return false;
    }
    return true;
}


void CDlgMonsterEvolve::OnBtnEvolve()
{
    // TODO: Add your control notification handler code here
    if (m_ImgMonster.GetImageLoadState() && m_IdMonster && m_idTypeAfter)
    {
        if (m_nEvoMode == 1 || m_nEvoMode == 2)
        {
            g_objHero.EvolveEudemon(m_IdMonster, m_nEvoMode);
        }
        else
        {
            g_objHero.ReEvolveEudemon(m_IdMonster, m_nEvoMode - 2);
        }
    }
}

void CDlgMonsterEvolve::EvolveEudemonOk()
{
    if (m_ImgMonster.GetImageLoadState() && m_IdMonster)
    {
        CItem* pPet = g_objHero.GetItem(m_IdMonster);
        if (pPet)
        {
            m_3dPetEvoBefore.SetFlashEffect();
            char szMsg[64] = "";
            sprintf(szMsg, g_objGameDataSet.GetStr(10820), pPet->GetMonsterName());
            g_objGameMsg.AddMsg(szMsg);		//您的幻兽 %s 进化成功
            m_idTypeBefore = pPet->GetTypeID();
            m_ImgMonster.RemoveImage();
            m_ImgMonster.InsertImage(NULL, pPet->GetID(), pPet->GetTypeID(), true);
            m_BtnEvolve.EnableWindow(false);
            m_BtnCancel.EnableWindow(false);
            m_idTypeAfter = 0;
        }
        else
        {
            m_idTypeAfter = 0;
        }
    }
}

void CDlgMonsterEvolve::OnBtnCancel()
{
    // TODO: Add your control notification handler code here
    m_ImgMonster.RemoveImage();
    m_BtnEvolve.EnableWindow(false);
    m_BtnCancel.EnableWindow(false);
    m_StaMonsterName.SetWindowText("");
}

void CDlgMonsterEvolve::Show()
{
    if (m_bShow)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog8000",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0,
                           m_Pnt.x ,
                           m_Pnt.y  );
        }
        else
        {
            return ;
        }
        m_StaMonsterName.Show(m_Pnt.x, m_Pnt.y);
        if (m_BtnEvolve.IsWindowEnabled())
        {
            m_BtnEvolve.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnCancel.IsWindowEnabled())
        {
            m_BtnCancel.Show(m_Pnt.x, m_Pnt.y);
        }
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
        m_ImgMonster.Show(m_Pnt.x, m_Pnt.y);
        if (m_ImgMonster.GetImageLoadState())
        {
            if (m_idTypeBefore)
            {
                CMyPos posBefore = {160, 400};
                m_3dPetEvoBefore.ShowPet(m_idTypeBefore % 10000, posBefore);
            }
            if (m_idTypeAfter)
            {
                CMyPos posAfter = {400, 400};
                m_3dPetEvoAfter.ShowPet(m_idTypeAfter % 10000, posAfter);
            }
        }
    }
}

void CDlgMonsterEvolve::SetEvoMode(int nMode)
{
    m_nEvoMode = nMode;
}

void CDlgMonsterEvolve::OnDoubleclickedImgMonster()
{
    return;
}
