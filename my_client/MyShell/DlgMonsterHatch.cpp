
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgMonsterHatch.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgMonsterHatch.h"
#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterHatch dialog


CDlgMonsterHatch::CDlgMonsterHatch(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMonsterHatch::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMonsterHatch)
    m_Pnt = CPoint(0, 0);
    m_bShow = false;// NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgMonsterHatch::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMonsterHatch)
    DDX_Control(pDX, IDC_BTN_CHECKOK, m_BtnCheckOk);
    DDX_Control(pDX, IDC_BTN_CANCEL, m_BtnCancel);
    DDX_Control(pDX, IDC_STA_NPCINFO, m_StaNpcInfo);
    DDX_Control(pDX, IDC_IMG_NPCFACE, m_ImgNpcFace);
    DDX_Control(pDX, IDC_GRID_EGGS, m_GridEggs);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMonsterHatch, CDialog)
    //{{AFX_MSG_MAP(CDlgMonsterHatch)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
    ON_BN_CLICKED(IDC_BTN_CHECKOK, OnBtnCheckok)
    ON_WM_RBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterHatch message handlers

void CDlgMonsterHatch::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgMonsterHatch::GetParent()->PostMessage(WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL);
    CDlgMonsterHatch::EnableWindow(false);
}

LRESULT CDlgMonsterHatch::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        {
            m_bShow = wParam ;
            m_StaNpcInfo.SetWindowText((char*)g_objGameDataSet.GetStr(10695));
            CDlgMonsterHatch::ResetAddEggs();
            if (m_bShow)
            {
                DXPlaySound("Sound/Open.wav");
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
                return true ;
            }
            else if ( wParam == 2 )
            {
                CDlgMonsterHatch::OnBtnClose();
                return true ;
            }
        }
        break ;
    default:
        break;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgMonsterHatch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

BOOL CDlgMonsterHatch::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgMonsterHatch::GetWindowRect(rect);
    m_StaNpcInfo.Init(rect.left, rect.top, 2);
    m_ImgNpcFace.Init(rect.left, rect.top, NULL, FACE_IMAGE);
    m_GridEggs.Init(rect.left, rect.top, 1, 5, DLG_MONSTERHATCH);
    m_GridEggs.ReSetGridSpace(24, 1);
    m_GridEggs.SetShowScale(false);
    m_BtnCheckOk.Init(rect.left, rect.top, "Button650");
    m_BtnCancel.Init(rect.left, rect.top, "Button77");
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMonsterHatch::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgMonsterHatch::Show()
{
    if (m_bShow)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog80",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0,
                           m_Pnt.x ,
                           m_Pnt.y  );
            ShowAni->Show( 1,
                           m_Pnt.x + 256 ,
                           m_Pnt.y  );
            ShowAni->Show( 2,
                           m_Pnt.x ,
                           m_Pnt.y + 256 );
            ShowAni->Show( 3,
                           m_Pnt.x + 128,
                           m_Pnt.y + 256 );
            ShowAni->Show( 4,
                           m_Pnt.x + 256,
                           m_Pnt.y + 256 );
            ShowAni->Show( 5,
                           m_Pnt.x + 384,
                           m_Pnt.y + 256 );
        }
        else
        {
            return ;
        }
        m_StaNpcInfo.Show(m_Pnt.x, m_Pnt.y);
        m_ImgNpcFace.Show(m_Pnt.x, m_Pnt.y);
        m_GridEggs.Show(m_Pnt.x, m_Pnt.y);
        if (m_BtnCheckOk.IsWindowEnabled())
        {
            m_BtnCheckOk.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnCancel.IsWindowEnabled())
        {
            m_BtnCancel.Show(m_Pnt.x, m_Pnt.y);
        }
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgMonsterHatch::GetMonsterHatch()
{
}



void CDlgMonsterHatch::OpenHatch(OBJID idNpc)
{
    g_objHero.OpenPackage(idNpc, PACKAGE_EUDEMON_BROODER);
}

void CDlgMonsterHatch::SetGrid()
{
    m_GridEggs.RemoveIcon(0, 0, true);
    if (g_objHero.GetPackageType() != PACKAGE_EUDEMON_BROODER)
    {
        return;
    }
    else
    {
        int nAmount = g_objHero.GetPackageItemAmount();
        for(int i = 0; i < nAmount; i++)
        {
            CItem* pMonster = g_objHero.GetPackageItemByIndex(i);
            if (pMonster)
            {
                m_GridEggs.InsertIcon ( 1,
                                        1,
                                        pMonster->GetTypeID(),
                                        i,
                                        ICON_TYPE_DEPOT ) ;
                m_GridEggs.SetIconRealID(i, pMonster->GetID());
            }
        }
    }
}

void CDlgMonsterHatch::AddEgg(OBJID idEgg)
{
    CItem* pItem = g_objHero.GetItem(idEgg) ;
    if ( pItem && pItem->GetID () == idEgg && m_nAddEggs < 5)
    {
        CPoint InsertLandR = m_GridEggs.GetInsertLandR () ;
        m_IdAddEggs[m_nAddEggs++] = pItem->GetID();
        m_GridEggs.InsertIcon ( InsertLandR.y,
                                InsertLandR.x,
                                pItem->GetTypeID(),
                                m_GridEggs.GetGridIndex(InsertLandR),
                                ICON_TYPE_ITEM ) ;
        m_GridEggs.SetIconRealID(m_GridEggs.GetGridIndex(InsertLandR) - 1, pItem->GetID());
        if (!m_BtnCheckOk.IsWindowEnabled())
        {
            m_BtnCheckOk.EnableWindow();
        }
        if (!m_BtnCancel.IsWindowEnabled())
        {
            m_BtnCancel.EnableWindow();
        }
        //		g_objHero.CheckInItem(pItem->GetID());
    }
}

void CDlgMonsterHatch::ResetAddEggs()
{
    m_nAddEggs = 0;
    m_IdAddEggs[0] = 0;
    m_IdAddEggs[1] = 0;
    m_IdAddEggs[2] = 0;
    m_IdAddEggs[3] = 0;
    m_IdAddEggs[4] = 0;
    m_GridEggs.RemoveIcon(0, 0, true);
    m_BtnCheckOk.EnableWindow(false);
    m_BtnCancel.EnableWindow(false);
}

void CDlgMonsterHatch::OnBtnCancel()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgMonsterHatch::ResetAddEggs();
    CDlgMonsterHatch::GetParent()->PostMessage(WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL);
}

void CDlgMonsterHatch::OnBtnCheckok()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_nAddEggs > 0 && m_nAddEggs <= 5)
    {
        for(int i = 0; i < m_nAddEggs; i++)
        {
            CItem* pItem = g_objHero.GetItem(m_IdAddEggs[i]);
            if (pItem)
            {
                g_objHero.CheckInItem(pItem->GetID());
            }
        }
        m_nAddEggs = 0;
        m_IdAddEggs[0] = 0;
        m_IdAddEggs[1] = 0;
        m_IdAddEggs[2] = 0;
        m_IdAddEggs[3] = 0;
        m_IdAddEggs[4] = 0;
        m_BtnCheckOk.EnableWindow(false);
        m_BtnCancel.EnableWindow(false);
    }
}

void CDlgMonsterHatch::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (g_bPickUp)
    {
        CDlgMonsterHatch::GetParent()->PostMessage(WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL);
    }
    CDialog::OnRButtonDown(nFlags, point);
}





















