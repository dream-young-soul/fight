
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgImprove.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgImprove.h"

#include "Hero.h"
#include "Item.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImprove dialog


CDlgImprove::CDlgImprove(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgImprove::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgImprove)
    m_bShow = false ;
    m_nImproveType = 0;
    m_nIconRealID = 0 ;
    m_GoodID = 0;
    m_GemID = 0;
    //}}AFX_DATA_INIT
}


void CDlgImprove::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgImprove)
    DDX_Control(pDX, IDC_IMPROVE_BTN_CLOSEB, m_ImproveBtnCloseB);
    DDX_Control(pDX, IDC_IMPROVE_BTN_CLOSE, m_ImproveBtnClose);
    DDX_Control(pDX, IDC_IMPROVE_IMA_GOOD, m_ImproveImaGood);
    DDX_Control(pDX, IDC_IMPROVE_IMA_GEM, m_ImproveImaGem);
    DDX_Control(pDX, IDC_IMPROVE_BTN_IMP, m_ImproveBtnImp);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImprove, CDialog)
    //{{AFX_MSG_MAP(CDlgImprove)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_IMPROVE_IMA_GEM, OnImproveImaGem)
    ON_BN_CLICKED(IDC_IMPROVE_IMA_GOOD, OnImproveImaGood)
    ON_BN_CLICKED(IDC_IMPROVE_BTN_IMP, OnImproveBtnImp)
    ON_BN_CLICKED(IDC_IMPROVE_BTN_CLOSE, OnImproveBtnClose)
    ON_BN_DOUBLECLICKED(IDC_IMPROVE_IMA_GEM, OnDoubleclickedImproveImaGem)
    ON_BN_DOUBLECLICKED(IDC_IMPROVE_IMA_GOOD, OnDoubleclickedImproveImaGood)
    ON_BN_CLICKED(IDC_IMPROVE_BTN_CLOSEB, OnImproveBtnCloseb)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImprove message handlers

LRESULT CDlgImprove::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        CDlgImprove::ResetImproveDlg();
        if (wParam)
        {
            DXPlaySound ( "Sound/Open.wav" ) ;
        }
        else
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgImprove::OnCloseB();
            return true ;
        }
        break ;
    case WM_MY_MESSAGE:
        switch(wParam)
        {
        case ON_MSGDLG_HIDE:
            CDlgImprove::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_HIDE);
            break;
        case ON_MSGDLG_POINT:
            CDlgImprove::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_POINT, lParam ) ;
            break;
        case ON_MSGDLG_IMAGE:
            CDlgImprove::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_GOODS, DLG_IMPROVE ) ;
            break ;
        default:
            break;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgImprove::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgImprove::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgImprove::GetWindowRect ( rect ) ;
    // Init the image
    if ( ! m_ImproveImaGood.Init ( rect.left, rect.top, NULL, ITEM_IMAGE ) )
    {
        return false ;
    }
    m_ImproveImaGood.SetItemPos(DLG_IMPROVE);
    if ( ! m_ImproveImaGem.Init ( rect.left, rect.top, NULL, ITEM_IMAGE ) )
    {
        return false ;
    }
    m_ImproveImaGem.SetItemPos(DLG_IMPROVE);
    // Init the Button
    if ( ! m_ImproveBtnImp.Init ( rect.left, rect.top, "Button76" ) )
    {
        return false ;
    }
    if ( ! m_ImproveBtnClose.Init ( rect.left, rect.top, "Button77" ) )
    {
        return false ;
    }
    m_ImproveBtnCloseB.Init ( rect.left, rect.top, "Button540" ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgImprove::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgImprove::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog29",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( 0, m_Pnt.x, m_Pnt.y ) ;
        }
        else
        {
            return ;
        }
        // Show the image
        m_ImproveImaGem.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ImproveImaGood.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the button
        m_ImproveBtnImp.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ImproveBtnClose.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ImproveBtnCloseB.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgImprove::OnImproveImaGem()
{
    if ( g_bPickUp && !m_ImproveImaGem.GetImageLoadState () )
    {
        CItem* pItem = g_objHero.GetItem ( m_nIconRealID ) ;
        if ( pItem != NULL )
        {
            int ItemType = pItem->GetSubType() / 1000 * 1000;
            BOOL bPickInOk = false;
            switch(m_nImproveType)
            {
            case 0:
                if (pItem->IsDragonBall())
                {
                    bPickInOk = true;
                }
                break;
            case 1:
                if (pItem->IsSoulStone())
                {
                    bPickInOk = true;
                }
                break;
            case 2:
                if (pItem->IsShootingStar())
                {
                    bPickInOk = true;
                }
                break;
            case 3:
                if (ItemType == ITEMTYPE_GHOSTGEM_INVITE)
                {
                    bPickInOk = true;
                }
                break;
            default:
                break;
            }
            if (bPickInOk)
            {
                m_GemID = pItem->GetID () ;
                m_ImproveImaGem.InsertImage (NULL , m_GemID ) ;
                CDlgImprove::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
                CDlgImprove::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
            }
            else
            {
                m_GemID = 0;
            }
        }
    }
}

void CDlgImprove::OnImproveImaGood()
{
    if ( g_bPickUp && !m_ImproveImaGood.GetImageLoadState ())
    {
        CItem* pItem = g_objHero.GetItem ( m_nIconRealID ) ;
        // Test if the item is equipment
        if ( pItem->GetSort() == ITEMSORT_HELMET ||
                pItem->GetSort() == ITEMSORT_NECKLACE ||
                pItem->GetSort() == ITEMSORT_ARMOR ||
                pItem->GetSort() == ITEMSORT_WEAPON_SINGLE_HAND ||
                pItem->GetSort() == ITEMSORT_WEAPON_DOUBLE_HAND ||
                pItem->GetSort() == ITEMSORT_SHIELD ||
                pItem->GetSort() == ITEMSORT_RING ||
                pItem->GetSort() == ITEMSORT_TREASURE ||
                pItem->GetSort() == ITEMSORT_SHOES )
        {
            // Test if the item is no broken
            if ( pItem->GetAmount() != pItem->GetAmountLimit () )
            {
                g_objGameMsg.AddMsg ( g_objGameDataSet.GetStr ( 10373 ) ) ;
                m_GoodID = 0 ;
                return ;
            }
            else
            {
                switch(m_nImproveType)
                {
                case 0:
                    {
                        OBJID idType = pItem->GetTypeID() % 10;
                        if (idType >= 4)
                        {
                            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10680));//该物品已到最高品质，无法再升级品质!
                            m_GoodID = 0;
                            return;
                        }
                        ItemTypeInfo pInfo;
                        if (!CItem::s_ItemData.GetItemTypeInfo(pItem->GetTypeID() + 1, pInfo))
                        {
                            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10900));//该物品无法再升级品质!
                            m_GoodID = 0;
                            return;
                        }
                    }
                    break;
                case 1:
                    if (pItem->GetAddition() >= 9)
                    {
                        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10681));//该物品已到最高魔魂等级，无法再升级魔魂等级!
                        m_GoodID = 0;
                        return;
                    }
                    ItemAdditionInfo pInfo;
                    if (!CItem::s_ItemData.GetItemAdditionInfo(pItem->GetTypeID(), pItem->GetAddition() + 1, pInfo))
                    {
                        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10902));//该物品无法提升魔魂!
                        m_GoodID = 0;
                        return;
                    }
                    break;
                case 2:
                    {
                        int nLevel = (pItem->GetTypeID() / 10) % 100;
                        if ((pItem->GetSort() == ITEMSORT_WEAPON_SINGLE_HAND
                                || pItem->GetSort() == ITEMSORT_WEAPON_DOUBLE_HAND)
                                && nLevel >= 22)	//武器
                        {
                            if (nLevel >= 22)
                            {
                                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10682));//该物品已到最高等级，无法再升级等级!
                                m_GoodID = 0;
                                return;
                            }
                        }
                        else if (pItem->GetSort() != ITEMSORT_WEAPON_SINGLE_HAND
                                 && pItem->GetSort() != ITEMSORT_WEAPON_DOUBLE_HAND && nLevel >= 9) //其它装备
                        {
                            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10682));//该物品已到最高等级，无法再升级等级!
                            m_GoodID = 0;
                            return;
                        }
                        ItemTypeInfo pInfo;
                        if (!CItem::s_ItemData.GetItemTypeInfo(pItem->GetTypeID() + 10, pInfo))
                        {
                            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10901));//该物品无法再升级等级!
                            m_GoodID = 0;
                            return;
                        }
                    }
                    break;
                case 3:
                    break;
                default:
                    break;
                }
            }
            m_GoodID = pItem->GetID () ;
            m_ImproveImaGood.InsertImage (NULL, m_GoodID, 0, true) ;
            CDlgImprove::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
            CDlgImprove::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
        }
        else
        {
            m_GoodID = 0 ;
        }
    }
}

void CDlgImprove::OnImproveBtnImp()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_ImproveImaGood.GetImageLoadState() && m_ImproveImaGem.GetImageLoadState())
    {
        CItem* pItem = g_objHero.GetItem(m_GemID);
        if (pItem)
        {
            if (pItem->IsDragonBall())
            {
                g_objHero.ImproveQuality( m_GoodID, m_GemID ) ;
            }
            else if (pItem->IsSoulStone())
            {
                g_objHero.ImproveSoulDegree(m_GoodID, m_GemID );
            }
            else if (pItem->IsShootingStar())
            {
                g_objHero.UpLevelItem(m_GoodID, m_GemID);
            }
            else
            {
                g_objHero.ImproveMagic(m_GoodID, m_GemID);
            }
            CDlgImprove::EnableWindow(false) ;
            CDlgImprove::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_IMPROVE_CLOSE ) ;
        }
    }
}

void CDlgImprove::OnImproveBtnClose()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    m_ImproveImaGem.RemoveImage () ;
    m_ImproveImaGood.RemoveImage () ;
    CDlgImprove::EnableWindow(false) ;
    CDlgImprove::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_IMPROVE_CLOSE ) ;
}

void CDlgImprove::SetPickUpState(int nIconRealID)
{
    m_nIconRealID = nIconRealID ;
}

DWORD CDlgImprove::GetMouseMoveId()
{
    if ( m_ImproveImaGem.m_bMouseMove )
    {
        return m_GemID ;
    }
    else
    {
        return m_GoodID ;
    }
}

void CDlgImprove::OnDoubleclickedImproveImaGem()
{
    if ( m_ImproveImaGem.GetImageLoadState () )
    {
        m_ImproveImaGem.RemoveImage () ;
        m_GemID = 0;
        CDlgImprove::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
    }
}

void CDlgImprove::OnDoubleclickedImproveImaGood()
{
    if ( m_ImproveImaGood.GetImageLoadState () )
    {
        m_ImproveImaGood.RemoveImage () ;
        m_GoodID = 0;
        CDlgImprove::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
    }
}

void CDlgImprove::OnImproveBtnCloseb()
{
    DXPlaySound("Sound/window_close.wav");
    CDlgImprove::OnImproveBtnClose () ;
}

void CDlgImprove::ResetImproveDlg()
{
    m_ImproveImaGood.RemoveImage();
    m_ImproveImaGem.RemoveImage();
    m_GoodID = 0;
    m_GemID = 0;
}

void CDlgImprove::OnCloseB()
{
    CDlgImprove::OnImproveBtnClose();
}

