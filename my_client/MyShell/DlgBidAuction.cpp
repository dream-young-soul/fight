
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgBidAuction.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgBidAuction.h"

#include "Hero.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBidAuction dialog


CDlgBidAuction::CDlgBidAuction(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgBidAuction::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgBidAuction)
    m_bShow = false;
    m_Pnt = CPoint(0, 0);
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgBidAuction::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgBidAuction)
    DDX_Control(pDX, IDC_STATIC_VALUE, m_StaNowValue);
    DDX_Control(pDX, IDC_STATIC_NAME, m_StaBidName);
    DDX_Control(pDX, IDC_IMG_AUCTIONITEM, m_ImgAuctionItem);
    DDX_Control(pDX, IDC_EDIT_BIDVALUE, m_EditBidValue);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    DDX_Control(pDX, IDC_BTN_BID, m_BtnBid);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBidAuction, CDialog)
    //{{AFX_MSG_MAP(CDlgBidAuction)
    ON_BN_CLICKED(IDC_BTN_BID, OnBtnBid)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBidAuction message handlers

void CDlgBidAuction::OnBtnBid()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (g_objHero.GetAuctionItem() == NULL)
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10677)); //10677 =当前没有物品正在拍卖!
        return;
    }
    if (strcmp(g_objHero.GetAuctionOwnerName(), g_objHero.GetName()) == 0)
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10688));	//10688=自己的东西，不能参与竞拍!
        return;
    }
    char strMoney[16] = "";
    m_EditBidValue.GetWindowText(strMoney, 10);
    int nMoney = atoi(strMoney);
    if (nMoney > 0 && nMoney > g_objHero.GetAuctionItemMoney() && g_objHero.GetAuctionItem())
    {
        g_objHero.AuctionBid(m_AuctionNpcId, g_objHero.GetAuctionItem()->GetID(), nMoney);
    }
    else
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10028));    //10028=输入金额有问题！
    }
    m_EditBidValue.SetWindowText ("0");
}

void CDlgBidAuction::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgBidAuction::EnableWindow(false);
}

LRESULT CDlgBidAuction::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message == WM_ENABLE)
    {
        m_bShow = wParam;
        m_EditBidValue.SetWindowText("");
        m_StaBidName.SetWindowText("");
        m_StaNowValue.SetWindowText("");
        m_ImgAuctionItem.RemoveImage();
        if (wParam)
        {
            DXPlaySound ( "Sound/Open.wav" ) ;
        }
        else
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
        }
        if (m_bShow)
        {
            m_EditBidValue.SetFocus();
        }
    }
    else if (message == WM_COMMAND)
    {
        if (wParam == 1 )
        {
            if (m_BtnBid.IsWindowEnabled())
            {
                CDlgBidAuction::OnBtnBid();
            }
            return true;
        }
        else if (wParam == 2)
        {
            CDlgBidAuction::OnBtnClose();
            return true;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgBidAuction::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

BOOL CDlgBidAuction::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgBidAuction::GetWindowRect(rect);
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    m_BtnBid.Init(rect.left, rect.top, "Button66");
    m_StaBidName.Init(rect.left, rect.top);
    m_StaNowValue.Init(rect.left, rect.top);
    m_EditBidValue.Init(rect.left, rect.top);
    m_EditBidValue.SetLimitText(10);
    m_EditBidValue.SetWindowText("0");
    m_ImgAuctionItem.Init(rect.left, rect.top, NULL, BOOTHITEM_IMAGE);
    m_ImgAuctionItem.SetItemPos(DLG_AUCTION);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBidAuction::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgBidAuction::Show()
{
    if (m_bShow)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog61",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0, m_Pnt.x, m_Pnt.y ) ;
        }
        else
        {
            return ;
        }
        m_StaNowValue.Show(m_Pnt.x, m_Pnt.y);
        m_StaBidName.Show(m_Pnt.x, m_Pnt.y);
        m_EditBidValue.Show(m_Pnt.x, m_Pnt.y);
        m_ImgAuctionItem.Show(m_Pnt.x, m_Pnt.y);
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
        m_BtnBid.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgBidAuction::SetAuctionNpcId(OBJID idNpc)
{
    m_AuctionNpcId = idNpc;
    if (g_objHero.GetAuctionItemId())
    {
        g_objHero.AuctionItemInfo(idNpc, g_objHero.GetAuctionItemId());
    }
}

void CDlgBidAuction::SetAuctionData()
{
    m_ImgAuctionItem.RemoveImage();
    CItem* pItem = g_objHero.GetAuctionItem();
    if (pItem != NULL)
    {
        char strName[32] ;
        sprintf ( strName, "Item%u", pItem->GetTypeID () ) ;
        m_ImgAuctionItem.InsertImage(strName, 0, 0, true);
        m_StaBidName.SetWindowText(g_objHero.GetAuctionName());
        itoa(g_objHero.GetAuctionItemMoney(), strName, 10);
        m_StaNowValue.SetWindowText(strName);
    }
}
