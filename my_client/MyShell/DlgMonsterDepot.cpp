
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgMonsterDepot.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgMonsterDepot.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterDepot dialog


CDlgMonsterDepot::CDlgMonsterDepot(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMonsterDepot::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMonsterDepot)
    m_bShow = false;
    m_Pnt = CPoint(0, 0); // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgMonsterDepot::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMonsterDepot)
    DDX_Control(pDX, IDC_STA_MAGICATTACT1, m_StaMagicAttact1);
    DDX_Control(pDX, IDC_STA_NAME1, m_StaName1);
    DDX_Control(pDX, IDC_STA_LEVEL1, m_StaLevel1);
    DDX_Control(pDX, IDC_STA_ATTACT1, m_StaAttact1);
    DDX_Control(pDX, IDC_STA_MAGICATTACT2, m_StaMagicAttact2);
    DDX_Control(pDX, IDC_STA_NAME2, m_StaName2);
    DDX_Control(pDX, IDC_STA_LEVEL2, m_StaLevel2);
    DDX_Control(pDX, IDC_STA_ATTACT2, m_StaAttact2);
    DDX_Control(pDX, IDC_STA_MAGICATTACT3, m_StaMagicAttact3);
    DDX_Control(pDX, IDC_STA_NAME3, m_StaName3);
    DDX_Control(pDX, IDC_STA_LEVEL3, m_StaLevel3);
    DDX_Control(pDX, IDC_STA_ATTACT3, m_StaAttact3);
    DDX_Control(pDX, IDC_STA_MAGICATTACT4, m_StaMagicAttact4);
    DDX_Control(pDX, IDC_STA_NAME4, m_StaName4);
    DDX_Control(pDX, IDC_STA_LEVEL4, m_StaLevel4);
    DDX_Control(pDX, IDC_STA_ATTACT4, m_StaAttact4);
    DDX_Control(pDX, IDC_STA_MAGICATTACT5, m_StaMagicAttact5);
    DDX_Control(pDX, IDC_STA_NAME5, m_StaName5);
    DDX_Control(pDX, IDC_STA_LEVEL5, m_StaLevel5);
    DDX_Control(pDX, IDC_STA_ATTACT5, m_StaAttact5);
    DDX_Control(pDX, IDC_GRID_MONSTER, m_GridMonster);
    DDX_Control(pDX, IDC_BTN_PAGEUP, m_BtnPageUp);
    DDX_Control(pDX, IDC_BTN_PAGEDOWN, m_BtnPageDown);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMonsterDepot, CDialog)
    //{{AFX_MSG_MAP(CDlgMonsterDepot)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_BTN_PAGEDOWN, OnBtnPagedown)
    ON_BN_CLICKED(IDC_BTN_PAGEUP, OnBtnPageup)
    ON_BN_CLICKED(IDC_MONSTER1, OnMonster1)
    ON_WM_RBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterDepot message handlers

LRESULT CDlgMonsterDepot::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        {
            m_bShow = wParam ;
            CDlgMonsterDepot::ResetDepot();
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
                return true;
            }
            if ( wParam == 2)
            {
                CDlgMonsterDepot::OnBtnClose();
                return true;
            }
        }
        break ;
    default:
        break;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgMonsterDepot::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

BOOL CDlgMonsterDepot::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgMonsterDepot::GetWindowRect(rect);
    m_GridMonster.Init(rect.left, rect.top, 5, 1, DLG_MONSTERDEPOT);
    m_GridMonster.ReSetGridSpace(1, 30);
    m_BtnPageDown.Init(rect.left, rect.top, "Button816");
    m_BtnPageUp.Init(rect.left, rect.top, "Button815");
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    m_StaAttact1.Init(rect.left, rect.top);
    m_StaMagicAttact1.Init(rect.left, rect.top);
    m_StaName1.Init(rect.left, rect.top);
    m_StaLevel1.Init(rect.left, rect.top);
    m_StaAttact2.Init(rect.left, rect.top);
    m_StaMagicAttact2.Init(rect.left, rect.top);
    m_StaName2.Init(rect.left, rect.top);
    m_StaLevel2.Init(rect.left, rect.top);
    m_StaAttact3.Init(rect.left, rect.top);
    m_StaMagicAttact3.Init(rect.left, rect.top);
    m_StaName3.Init(rect.left, rect.top);
    m_StaLevel3.Init(rect.left, rect.top);
    m_StaAttact4.Init(rect.left, rect.top);
    m_StaMagicAttact4.Init(rect.left, rect.top);
    m_StaName4.Init(rect.left, rect.top);
    m_StaLevel4.Init(rect.left, rect.top);
    m_StaAttact5.Init(rect.left, rect.top);
    m_StaMagicAttact5.Init(rect.left, rect.top);
    m_StaName5.Init(rect.left, rect.top);
    m_StaLevel5.Init(rect.left, rect.top);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMonsterDepot::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgMonsterDepot::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgMonsterDepot::EnableWindow(false);
}

void CDlgMonsterDepot::OnBtnPagedown()
{
    // TODO: Add your control notification handler code here
}

void CDlgMonsterDepot::OnBtnPageup()
{
    // TODO: Add your control notification handler code here
}

void CDlgMonsterDepot::Show()
{
    if (m_bShow)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog81",
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
                           m_Pnt.x + 256,
                           m_Pnt.y + 128 );
            ShowAni->Show( 3,
                           m_Pnt.x + 256,
                           m_Pnt.y + 256 );
            ShowAni->Show( 4,
                           m_Pnt.x + 256,
                           m_Pnt.y + 384 );
            ShowAni->Show( 5,
                           m_Pnt.x ,
                           m_Pnt.y + 256 );
        }
        else
        {
            return ;
        }
        m_GridMonster.Show(m_Pnt.x, m_Pnt.y);
        //		m_BtnPageUp.Show(m_Pnt.x, m_Pnt.y);
        //		m_BtnPageDown.Show(m_Pnt.x, m_Pnt.y);
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
        if (m_GridMonster.GetGridAbleByIndex(0))
        {
            m_StaName1.Show(m_Pnt.x, m_Pnt.y);
            m_StaLevel1.Show(m_Pnt.x, m_Pnt.y);
            m_StaAttact1.Show(m_Pnt.x, m_Pnt.y);
            m_StaMagicAttact1.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_GridMonster.GetGridAbleByIndex(1))
        {
            m_StaName2.Show(m_Pnt.x, m_Pnt.y);
            m_StaLevel2.Show(m_Pnt.x, m_Pnt.y);
            m_StaAttact2.Show(m_Pnt.x, m_Pnt.y);
            m_StaMagicAttact2.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_GridMonster.GetGridAbleByIndex(2))
        {
            m_StaName3.Show(m_Pnt.x, m_Pnt.y);
            m_StaLevel3.Show(m_Pnt.x, m_Pnt.y);
            m_StaAttact3.Show(m_Pnt.x, m_Pnt.y);
            m_StaMagicAttact3.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_GridMonster.GetGridAbleByIndex(3))
        {
            m_StaName4.Show(m_Pnt.x, m_Pnt.y);
            m_StaLevel4.Show(m_Pnt.x, m_Pnt.y);
            m_StaAttact4.Show(m_Pnt.x, m_Pnt.y);
            m_StaMagicAttact4.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_GridMonster.GetGridAbleByIndex(4))
        {
            m_StaName5.Show(m_Pnt.x, m_Pnt.y);
            m_StaLevel5.Show(m_Pnt.x, m_Pnt.y);
            m_StaAttact5.Show(m_Pnt.x, m_Pnt.y);
            m_StaMagicAttact5.Show(m_Pnt.x, m_Pnt.y);
        }
    }
}

void CDlgMonsterDepot::ResetDepot()
{
    m_GridMonster.RemoveIcon(0, 0, true);
    m_StaName1.SetWindowText("");
    m_StaAttact1.SetWindowText("");
    m_StaMagicAttact1.SetWindowText("");
    m_StaLevel1.SetWindowText("");
    m_StaName2.SetWindowText("");
    m_StaAttact2.SetWindowText("");
    m_StaMagicAttact2.SetWindowText("");
    m_StaLevel2.SetWindowText("");
    m_StaName3.SetWindowText("");
    m_StaAttact3.SetWindowText("");
    m_StaMagicAttact3.SetWindowText("");
    m_StaLevel3.SetWindowText("");
    m_StaName4.SetWindowText("");
    m_StaAttact4.SetWindowText("");
    m_StaMagicAttact4.SetWindowText("");
    m_StaLevel4.SetWindowText("");
    m_StaName5.SetWindowText("");
    m_StaAttact5.SetWindowText("");
    m_StaMagicAttact5.SetWindowText("");
    m_StaLevel5.SetWindowText("");
}

void CDlgMonsterDepot::SetMonster()
{
    CDlgMonsterDepot::ResetDepot();
    if (g_objHero.GetPackageType() != PACKAGE_EUDEMON_STORAGE)
    {
        return;
    }
    int nAmount = g_objHero.GetPackageItemAmount();
    for(int i = 0; i < nAmount; i++)
    {
        CItem* pMonster = g_objHero.GetPackageItemByIndex(i);
        if (pMonster)
        {
            m_GridMonster.InsertIcon ( 1,
                                       1,
                                       pMonster->GetTypeID(),
                                       i,
                                       ICON_TYPE_DEPOT ) ;
            m_GridMonster.SetIconRealID(i, pMonster->GetID());
            char strInfo[64] = "";
            switch(i)
            {
            case 0:
                {
                    m_StaName1.SetWindowText(pMonster->GetMonsterName());
                    sprintf(strInfo, "%d/%d", pMonster->GetMonsterMinAtk(), pMonster->GetMonsterMaxAtk());
                    m_StaAttact1.SetWindowText(strInfo);
                    sprintf(strInfo, "%d/%d", pMonster->GetMonsterMagicMinAtk(), pMonster->GetMonsterMagicMaxAtk());
                    m_StaMagicAttact1.SetWindowText(strInfo);
                    if (pMonster->GetPackageType() == PACKAGE_EGG)
                    {
                        sprintf(strInfo, "0");
                    }
                    else
                    {
                        sprintf(strInfo, "%d", pMonster->GetAmountLimit());
                    }
                    m_StaLevel1.SetWindowText(strInfo);
                }
                break;
            case 1:
                {
                    m_StaName2.SetWindowText(pMonster->GetMonsterName());
                    sprintf(strInfo, "%d/%d", pMonster->GetMonsterMinAtk(), pMonster->GetMonsterMaxAtk());
                    m_StaAttact2.SetWindowText(strInfo);
                    sprintf(strInfo, "%d/%d", pMonster->GetMonsterMagicMinAtk(), pMonster->GetMonsterMagicMaxAtk());
                    m_StaMagicAttact2.SetWindowText(strInfo);
                    if (pMonster->GetPackageType() == PACKAGE_EGG)
                    {
                        sprintf(strInfo, "0");
                    }
                    else
                    {
                        sprintf(strInfo, "%d", pMonster->GetAmountLimit());
                    }
                    m_StaLevel2.SetWindowText(strInfo);
                }
                break;
            case 2:
                {
                    m_StaName3.SetWindowText(pMonster->GetMonsterName());
                    sprintf(strInfo, "%d/%d", pMonster->GetMonsterMinAtk(), pMonster->GetMonsterMaxAtk());
                    m_StaAttact3.SetWindowText(strInfo);
                    sprintf(strInfo, "%d/%d", pMonster->GetMonsterMagicMinAtk(), pMonster->GetMonsterMagicMaxAtk());
                    m_StaMagicAttact3.SetWindowText(strInfo);
                    if (pMonster->GetPackageType() == PACKAGE_EGG)
                    {
                        sprintf(strInfo, "0");
                    }
                    else
                    {
                        sprintf(strInfo, "%d", pMonster->GetAmountLimit());
                    }
                    m_StaLevel3.SetWindowText(strInfo);
                }
                break;
            case 3:
                {
                    m_StaName4.SetWindowText(pMonster->GetMonsterName());
                    sprintf(strInfo, "%d/%d", pMonster->GetMonsterMinAtk(), pMonster->GetMonsterMaxAtk());
                    m_StaAttact4.SetWindowText(strInfo);
                    sprintf(strInfo, "%d/%d", pMonster->GetMonsterMagicMinAtk(), pMonster->GetMonsterMagicMaxAtk());
                    m_StaMagicAttact4.SetWindowText(strInfo);
                    if (pMonster->GetPackageType() == PACKAGE_EGG)
                    {
                        sprintf(strInfo, "0");
                    }
                    else
                    {
                        sprintf(strInfo, "%d", pMonster->GetAmountLimit());
                    }
                    m_StaLevel4.SetWindowText(strInfo);
                }
                break;
            case 4:
                {
                    m_StaName5.SetWindowText(pMonster->GetMonsterName());
                    sprintf(strInfo, "%d/%d", pMonster->GetMonsterMinAtk(), pMonster->GetMonsterMaxAtk());
                    m_StaAttact5.SetWindowText(strInfo);
                    sprintf(strInfo, "%d/%d", pMonster->GetMonsterMagicMinAtk(), pMonster->GetMonsterMagicMaxAtk());
                    m_StaMagicAttact5.SetWindowText(strInfo);
                    if (pMonster->GetPackageType() == PACKAGE_EGG)
                    {
                        sprintf(strInfo, "0");
                    }
                    else
                    {
                        sprintf(strInfo, "%d", pMonster->GetAmountLimit());
                    }
                    m_StaLevel5.SetWindowText(strInfo);
                }
                break;
            default:
                break;
            }
        }
    }
}


void CDlgMonsterDepot::OnMonster1()
{
    // TODO: Add your control notification handler code here
    if (g_bPickUp)
    {
        CDlgMonsterDepot::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP, DLG_MONSTERDEPOT) ;
    }
}


void CDlgMonsterDepot::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (g_bPickUp)
    {
        CDlgMonsterDepot::GetParent()->PostMessage(WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL);
    }
    CDialog::OnRButtonDown(nFlags, point);
}
