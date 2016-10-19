
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgStoneGoods.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgStoneGoods.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStoneGoods dialog


CDlgStoneGoods::CDlgStoneGoods(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgStoneGoods::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgStoneGoods)
    m_bShow = false;
    m_Pnt = CPoint( 0, 0 );
    m_dwLastShowTime = 0 ;
    m_fFrameStep = 0.0f ;
    m_nFrameMode = 0 ;
    m_nPntX = 0 ;
    m_uCurStoneKind = 0;
    //}}AFX_DATA_INIT
}


void CDlgStoneGoods::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgStoneGoods)
    DDX_Control(pDX, IDC_BTN_STONE3, m_ChkStone3);
    DDX_Control(pDX, IDC_BTN_STONE2, m_ChkStone2);
    DDX_Control(pDX, IDC_BTN_STONE1, m_ChkStone1);
    DDX_Control(pDX, IDC_STONE_GRI0, m_StoneGri0);
    DDX_Control(pDX, IDC_STONE_GRI1, m_StoneGri1);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_CloseBtn);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStoneGoods, CDialog)
    //{{AFX_MSG_MAP(CDlgStoneGoods)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_BTN_STONE1, OnBtnStone1)
    ON_BN_CLICKED(IDC_BTN_STONE2, OnBtnStone2)
    ON_BN_CLICKED(IDC_BTN_STONE3, OnBtnStone3)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStoneGoods message handlers

LRESULT CDlgStoneGoods::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if ( wParam == false)
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
            return true;
        }
        else
        {
            //			SetCurGridMode();
            //			CDlgStoneGoods::FlashGhostGem();
            m_dwLastShowTime = TimeGet() ;
            m_nFrameMode = 1 ;
            m_nPntX = _SCR_WIDTH ;
            DXPlaySound ( "Sound/Open.wav" ) ;
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

HBRUSH CDlgStoneGoods::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgStoneGoods::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgStoneGoods::GetWindowRect( rect );
    m_fFrameStep = ( float ) (rect.Width() + 100) / DLG_FRAME_AMOUNT ;
    // Init the button
    m_CloseBtn.Init ( rect.left, rect.top, "Button540");
    m_ChkStone1.Init(rect.left, rect.top, "Check50", 2);
    m_ChkStone2.Init(rect.left, rect.top, "Check51", 2);
    m_ChkStone3.Init(rect.left, rect.top, "Check52", 2);
    // Init the Grids
    m_StoneGri0.MoveWindow(46, 61, 185, 70, false);
    m_StoneGri0.Init( rect.left, rect.top, 2, 5, DLG_STONEGOODS0);
    m_StoneGri0.ReSetGridSpace(5, 4);
    m_StoneGri1.MoveWindow(46, 134, 185, 70, false);
    m_StoneGri1.Init( rect.left, rect.top, 2, 5, DLG_STONEGOODS1);
    m_StoneGri1.ReSetGridSpace(5, 4);
    //no used
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgStoneGoods::Show()
{
    if ( m_bShow)
    {
        if ( m_nFrameMode != 0 )
        {
            if ( TimeGet() - m_dwLastShowTime <= 300 )
            {
                m_Pnt.x = (LONG)(m_nPntX - ( TimeGet() - m_dwLastShowTime ) / 15 * m_fFrameStep);
            }
            else
            {
                CRect rect;
                CDlgStoneGoods::GetWindowRect(rect);
                m_Pnt.x = _SCR_WIDTH - rect.Width() - 100;
                m_nFrameMode = 0 ;
            }
        }
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog13",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0, m_Pnt.x, m_Pnt.y ) ;
        }
        else
        {
            return ;
        }
        //Show the buttons
        m_CloseBtn.Show( m_Pnt.x , m_Pnt.y);
        m_ChkStone1.Show(m_Pnt.x, m_Pnt.y);
        m_ChkStone2.Show(m_Pnt.x, m_Pnt.y);
        m_ChkStone3.Show(m_Pnt.x, m_Pnt.y);
        //Show the Girds
        m_StoneGri0.Show(m_Pnt.x, m_Pnt.y);
        m_StoneGri1.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgStoneGoods::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_nPntX = x;
    m_Pnt.y = y;
}

void CDlgStoneGoods::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    CDlgStoneGoods::EnableWindow(false);
}

void CDlgStoneGoods::FlashGhostGem()
{
    //clear Grids
    m_StoneGri0.RemoveIcon(0, 0, true);
    m_StoneGri1.RemoveIcon(0, 0, true);
    int nAmount;
    nAmount = g_objHero.GetItemAmount(PACKAGE_SOUL);
    for(int i = 0; i < nAmount; i++)
    {
        CItem* pItem = g_objHero.GetItemByIndex(i, PACKAGE_SOUL);
        if (pItem != NULL)
        {
            int nIndex;
            int ItemType = pItem->GetSubType() / 1000 * 1000;
            switch(m_uCurStoneKind)
            {
            case 0:
                {
                    switch(ItemType)
                    {
                    case ITEMTYPE_GHOSTGEM_EUDEMON: // 幻兽类
                        nIndex = m_StoneGri0.InsertIcon( 1, 1, i, ICON_TYPE_ITEM ) ;
                        m_StoneGri0.SetIconRealID( nIndex, pItem->GetID () ) ;
                        break;
                    case ITEMTYPE_GHOSTGEM_RELEASE:// 解除类
                        nIndex = m_StoneGri0.InsertIcon( 1, 1, i, ICON_TYPE_ITEM ) ;
                        m_StoneGri0.SetIconRealID( nIndex, pItem->GetID () ) ;
                        break;
                    case ITEMTYPE_GHOSTGEM_TRACE:// 追杀类
                        nIndex = m_StoneGri1.InsertIcon( 1, 1, i, ICON_TYPE_ITEM ) ;
                        m_StoneGri1.SetIconRealID( nIndex, pItem->GetID () ) ;
                        break;
                    case ITEMTYPE_GHOSTGEM_PROTECTIVE: // 护身类
                        nIndex = m_StoneGri1.InsertIcon( 1, 1, i, ICON_TYPE_ITEM ) ;
                        m_StoneGri1.SetIconRealID( nIndex, pItem->GetID () ) ;
                        break;
                    }
                }
                break;
            case 1:
                {
                    switch(ItemType)
                    {
                    case ITEMTYPE_GHOSTGEM_ACTIVE_ATK:// 状态攻击类
                        nIndex = m_StoneGri0.InsertIcon( 1, 1, i, ICON_TYPE_ITEM ) ;
                        m_StoneGri0.SetIconRealID( nIndex, pItem->GetID () ) ;
                        break;
                    case ITEMTYPE_GHOSTGEM_PASSIVE_ATK:// 状态被动类
                        nIndex = m_StoneGri1.InsertIcon( 1, 1, i, ICON_TYPE_ITEM ) ;
                        m_StoneGri1.SetIconRealID( nIndex, pItem->GetID () ) ;
                        break;
                    }
                }
                break;
            case 2:
                if (ItemType == ITEMTYPE_GHOSTGEM_SPECIAL // 特殊类
                        || ItemType == ITEMTYPE_GHOSTGEM_INVITE) //锻造类
                {
                    nIndex = m_StoneGri0.InsertIcon( 1, 1, i, ICON_TYPE_ITEM ) ;
                    m_StoneGri0.SetIconRealID( nIndex, pItem->GetID () ) ;
                }
                break;
            default:
                break;
            }
        }
    }
}

void CDlgStoneGoods::OnBtnStone1()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/DlgStone_Item08.wav" ) ;
    if (m_uCurStoneKind != 0)
    {
        PopUpCurChk();
        m_uCurStoneKind = 0;
        SetCurGridMode();
        m_ChkStone1.SetCheckValue(1);
        CDlgStoneGoods::FlashGhostGem();
    }
}

void CDlgStoneGoods::OnBtnStone2()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/DlgStone_Item08.wav" ) ;
    if (m_uCurStoneKind != 1)
    {
        PopUpCurChk();
        m_uCurStoneKind = 1;
        m_ChkStone2.SetCheckValue(1);
        SetCurGridMode();
        CDlgStoneGoods::FlashGhostGem();
    }
}

void CDlgStoneGoods::OnBtnStone3()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/DlgStone_Item08.wav" ) ;
    if (m_uCurStoneKind != 2)
    {
        PopUpCurChk();
        m_uCurStoneKind = 2;
        SetCurGridMode();
        m_ChkStone3.SetCheckValue(1);
        CDlgStoneGoods::FlashGhostGem();
    }
}

void CDlgStoneGoods::PopUpCurChk()
{
    switch(m_uCurStoneKind)
    {
    case 0:
        m_ChkStone1.SetCheckValue(0);
        break;
    case 1:
        m_ChkStone2.SetCheckValue(0);
        break;
    case 2:
        m_ChkStone3.SetCheckValue(0);
        break;
    default:
        break;
    }
    m_uCurStoneKind = -1;
}

void CDlgStoneGoods::SetCurGridMode()
{
    CRect rect;
    CDlgStoneGoods::GetWindowRect( rect );
    switch(m_uCurStoneKind)
    {
    case 0:
        m_StoneGri0.MoveWindow(46, 61, 185, 70, false);
        m_StoneGri0.Init( rect.left, rect.top, 2, 5, DLG_STONEGOODS0);
        m_StoneGri0.ReSetGridSpace(5, 4);
        m_StoneGri1.EnableWindow(true);
        m_StoneGri1.MoveWindow(46, 134, 185, 70, false);
        m_StoneGri1.Init( rect.left, rect.top, 2, 5, DLG_STONEGOODS1);
        m_StoneGri1.ReSetGridSpace(5, 4);
        break;
    case 1:
        m_StoneGri0.MoveWindow(46, 61, 185, 70, false);
        m_StoneGri0.Init( rect.left, rect.top, 2, 5, DLG_STONEGOODS0);
        m_StoneGri0.ReSetGridSpace(5, 4);
        m_StoneGri1.EnableWindow(true);
        m_StoneGri1.Init( rect.left, rect.top, 2, 5, DLG_STONEGOODS1);
        m_StoneGri1.ReSetGridSpace(5, 4);
        break;
    case 2:
        m_StoneGri0.MoveWindow(46, 61, 185, 144, false);
        m_StoneGri0.Init( rect.left, rect.top, 4, 5, DLG_STONEGOODS0);
        m_StoneGri0.ReSetGridSpace(5, 4);
        m_StoneGri1.EnableWindow(false);
        break;
    default:
        break;
    }
}
