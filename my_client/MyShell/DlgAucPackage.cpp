
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgAucPackage.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgAucPackage.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAucPackage dialog


CDlgAucPackage::CDlgAucPackage(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgAucPackage::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgAucPackage)
    m_bShow = FALSE;
    m_Pnt = CPoint(0, 0); // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgAucPackage::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgAucPackage)
    DDX_Control(pDX, IDC_IMG_MONEYITEM, m_ImgMoneyItem);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    DDX_Control(pDX, IDC_AUCITEM_GRID, m_GridAucitem);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAucPackage, CDialog)
    //{{AFX_MSG_MAP(CDlgAucPackage)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_IMG_MONEYITEM, OnImgMoneyitem)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_WM_RBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAucPackage message handlers

void CDlgAucPackage::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgAucPackage::EnableWindow(false);
}

void CDlgAucPackage::OnImgMoneyitem()
{
    // TODO: Add your control notification handler code here
    if (m_ImgMoneyItem.GetImageLoadState())
    {
        g_objHero.CheckOutItem(m_ImgMoneyItem.GetImageRealID());
    }
}

LRESULT CDlgAucPackage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message == WM_ENABLE)
    {
        m_bShow = wParam;
        m_AuctionNpcId = 0;
        if (wParam)
        {
            DXPlaySound ( "Sound/Open.wav" ) ;
        }
        else
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
        }
    }
    else if (message == WM_COMMAND)
    {
        if (wParam == 1)
        {
            return true;
        }
        else if (wParam == 2)
        {
            CDlgAucPackage::OnBtnClose();
            return true;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgAucPackage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

BOOL CDlgAucPackage::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgAucPackage::GetWindowRect(rect);
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    m_GridAucitem.Init(rect.left, rect.top, 1, 5, DLG_AUCPACKAGE);
    m_GridAucitem.ReSetGridSpace(4, 1);
    m_ImgMoneyItem.Init(rect.left, rect.top, NULL, BOOTHITEM_IMAGE);
    m_ImgMoneyItem.SetItemPos(DLG_AUCPACKAGE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAucPackage::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgAucPackage::Show()
{
    if (m_bShow)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog63",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0, m_Pnt.x, m_Pnt.y ) ;
            ShowAni->Show( 1, m_Pnt.x + 128, m_Pnt.y ) ;
        }
        else
        {
            return ;
        }
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
        m_GridAucitem.Show(m_Pnt.x, m_Pnt.y);
        m_ImgMoneyItem.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgAucPackage::SetAuctionNpcId(OBJID idNpc)
{
    m_AuctionNpcId = idNpc;
    g_objHero.OpenPackage(m_AuctionNpcId, PACKAGE_AUCTION_STORAGE);
}

void CDlgAucPackage::SetAucPackage()
{
    m_GridAucitem.RemoveIcon(0, 0, true);
    m_ImgMoneyItem.RemoveImage();
    int dwItemAmount = g_objHero.GetPackageItemAmount() ;
    for(int i = 0; i < dwItemAmount; i++)
    {
        CItem* pItem = g_objHero.GetPackageItemByIndex ( i ) ;
        if ( pItem != NULL )
        {
            OBJID ItemTypeID = pItem->GetTypeID () ;
            if (ItemTypeID != MONEYITEM_TYPE)
            {
                int nIndex = m_GridAucitem.InsertIcon( 1, 1, ItemTypeID, i, ICON_TYPE_DEPOT ) ;
                m_GridAucitem.SetIconRealID(nIndex, pItem->GetID());
            }
            else
            {
                char strImage[32] = "";
                sprintf(strImage, "Item%d", ItemTypeID);
                m_ImgMoneyItem.InsertImage(strImage, pItem->GetID(), ItemTypeID, true);
            }
        }
    }
}

void CDlgAucPackage::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (g_bPickUp)
    {
        CDlgAucPackage::GetParent()->PostMessage(WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL);
    }
    CDialog::OnRButtonDown(nFlags, point);
}
