
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgAddAuction.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgAddAuction.h"

#include "Hero.h"
#include "gamemsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddAuction dialog


CDlgAddAuction::CDlgAddAuction(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgAddAuction::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgAddAuction)
    m_Pnt = CPoint(0, 0);
    m_bShow = FALSE;// NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgAddAuction::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgAddAuction)
    DDX_Control(pDX, IDC_IMG_AUCITEM, m_ImgAucItem);
    DDX_Control(pDX, IDC_BTN_ADDCANCEL, m_BtnAddCancel);
    DDX_Control(pDX, IDC_BTN_ADDOK, m_BtnAddOk);
    DDX_Control(pDX, IDC_EDIT_AUCMONEY, m_EditAucMoney);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddAuction, CDialog)
    //{{AFX_MSG_MAP(CDlgAddAuction)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_ADDCANCEL, OnBtnAddcancel)
    ON_BN_CLICKED(IDC_BTN_ADDOK, OnBtnAddok)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_IMG_AUCITEM, OnImgAucitem)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddAuction message handlers

LRESULT CDlgAddAuction::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message == WM_ENABLE)
    {
        m_bShow = wParam;
        if (m_bShow)
        {
            m_AucNpcID = 0;
            m_AucItemID = 0;
            m_nAucMoney = 0;
            DXPlaySound ( "Sound/Open.wav" ) ;
            m_EditAucMoney.SetWindowText("0");
            m_EditAucMoney.SetFocus();
            m_ImgAucItem.RemoveImage();
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
            CDlgAddAuction::OnBtnAddok();
            return true;
        }
        else if (wParam == 2)
        {
            CDlgAddAuction::OnBtnClose();
            return true;
        }
    }
    else if (message == WM_MY_MESSAGE)
    {
        switch(wParam)
        {
        case ON_MSGDLG_POINT:
            CDlgAddAuction::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_POINT, lParam ) ;
            break;
        case ON_MSGDLG_IMAGE:
            CDlgAddAuction::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_GOODS, DLG_ADDAUCITEM ) ;
            break ;
        case ON_MSGDLG_HIDE:
            CDlgAddAuction::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_HIDE);
            break;
        case WM_LBUTTONDBLCLK:
            if (lParam == DLG_ADDAUCITEM)
            {
                m_EditAucMoney.SetWindowText ("0");
                m_AucItemID = ID_NONE;
            }
            break;
        default:
            break;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgAddAuction::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

BOOL CDlgAddAuction::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgAddAuction::GetWindowRect(rect);
    m_EditAucMoney.Init(rect.left, rect.top);
    m_EditAucMoney.SetLimitText(10);
    m_ImgAucItem.Init(rect.left, rect.top, NULL, ITEM_IMAGE);
    m_ImgAucItem.SetItemPos(DLG_ADDAUCITEM);
    m_BtnAddOk.Init(rect.left, rect.top, "Button76");
    m_BtnAddCancel.Init(rect.left, rect.top, "Button77");
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAddAuction::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgAddAuction::Show()
{
    if (m_bShow)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog62",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show(0, m_Pnt.x, m_Pnt.y);
        }
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
        m_BtnAddOk.Show(m_Pnt.x, m_Pnt.y);
        m_BtnAddCancel.Show(m_Pnt.x, m_Pnt.y);
        m_EditAucMoney.Show(m_Pnt.x, m_Pnt.y);
        m_ImgAucItem.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgAddAuction::SetAuctionNpcId(OBJID idNpc)
{
    m_AucNpcID = idNpc;
}

void CDlgAddAuction::SetPickUpIcon(OBJID idItem)
{
    m_AucItemID = idItem;
}

void CDlgAddAuction::OnBtnAddcancel()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgAddAuction::EnableWindow(false);
}

void CDlgAddAuction::OnBtnAddok()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_AucNpcID && m_ImgAucItem.GetImageLoadState())
    {
        char strNum[16] = "";
        m_EditAucMoney.GetWindowText(strNum, 16);
        if (strlen(strNum) > 0)
        {
            m_nAucMoney = atoi(strNum);
            if (m_nAucMoney >= 100)
            {
                g_objHero.AuctionAddItem(m_AucNpcID, m_AucItemID, m_nAucMoney);
                CDlgAddAuction::EnableWindow(false);
            }
            else if (m_nAucMoney < 100)
            {
                g_objGameMsg.AddMsg ((char*)g_objGameDataSet.GetStr (10689));	//底价不能低于100金币
            }
        }
    }
}

void CDlgAddAuction::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgAddAuction::EnableWindow(false);
}

void CDlgAddAuction::OnImgAucitem()
{
    // TODO: Add your control notification handler code here
    if (m_AucItemID && g_bPickUp && !m_ImgAucItem.GetImageLoadState())
    {
        CItem* pItem = g_objHero.GetItem(m_AucItemID);
        if (pItem != NULL )
        {
            if (pItem->TestMonopolyItem (_NOT_MONOPOLY_DISAPPEAR))
            {
                g_objGameMsg.AddMsg ((char*)g_objGameDataSet.GetStr (10672));	//该物品不允许进行拍卖
            }
            else
            {
                m_ImgAucItem.InsertImage(NULL, pItem->GetID(), 0, TRUE);
                m_EditAucMoney.SetFocus();
            }
            CDlgAddAuction::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
            CDlgAddAuction::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
        }
        else
        {
            m_AucItemID = ID_NONE;
        }
    }
}


