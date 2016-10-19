
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgMpcGoods.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgMpcGoods.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int		MPCGOODS_PAGE_AMOUNT = 40 ;
/////////////////////////////////////////////////////////////////////////////
// CDlgMpcGoods dialog


CDlgMpcGoods::CDlgMpcGoods(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMpcGoods::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMpcGoods)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_fFrameStep = 0.0f ;
    m_nFrameMode = 0 ;
    m_nPntX = 0 ;
    m_nPickUpItemID = 0 ;
    m_PickUpMousePnt = CPoint ( 0, 0 );
    m_nPickUpItemPosition = 0 ;
    m_btCurPage = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgMpcGoods::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMpcGoods)
    DDX_Control(pDX, IDC_MPCGOODS_STONEBOX, m_MpcStonesGri);
    DDX_Control(pDX, IDC_CHK_STONES, m_ChkStones);
    DDX_Control(pDX, IDC_CHK_GOODS, m_ChkGoods);
    DDX_Control(pDX, IDC_MPCGOODS_GOODBOX, m_MpcGoodsGri);
    DDX_Control(pDX, IDC_MPCGOODS_STA_MONEY, m_MpcGoodsMoneySta);
    DDX_Control(pDX, IDC_MPCGOODS_BTN_CLOSE, m_MpcGoodsCloseBtn);
    DDX_Control(pDX, IDC_MPCGOODS_BTN_MONEY, m_MpcGoodsMoneyBtn);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMpcGoods, CDialog)
    //{{AFX_MSG_MAP(CDlgMpcGoods)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_MPCGOODS_BTN_CLOSE, OnMpcgoodsBtnClose)
    ON_BN_CLICKED(IDC_MPCGOODS_BTN_MONEY, OnMpcgoodsBtnMoney)
    ON_WM_MOUSEMOVE()
    ON_WM_RBUTTONDOWN()
    ON_BN_CLICKED(IDC_CHK_GOODS, OnChkGoods)
    ON_BN_CLICKED(IDC_CHK_STONES, OnChkStones)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_SETCURSOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMpcGoods message handlers

LRESULT CDlgMpcGoods::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if ( wParam == false )
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
            CDlgMpcGoods::GetParent()->PostMessage( WM_MY_MESSAGE, ON_RSIDEDLG_CLOSE ) ;
        }
        else
        {
            m_dwLastShowTime = TimeGet() ;
            m_nFrameMode = 1 ;
            CRect rect ;
            CDlgMpcGoods::GetWindowRect( rect ) ;
            m_nPntX = _SCR_WIDTH ;
            m_MpcGoodsGri.EnableWindow(false);
            m_MpcStonesGri.EnableWindow(false);
            DXPlaySound ( "Sound/Open.wav" ) ;
            CDlgMpcGoods::GetParent()->PostMessage( WM_MY_MESSAGE, ON_RSIDEDLG_OPEN ) ;
        }
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 )
        {
            return true ;
        }
        else if (wParam == 2)
        {
            CDlgMpcGoods::OnMpcgoodsBtnClose();
            return true;
        }
    }
    else if ( message == WM_MY_MESSAGE )
    {
        switch ( wParam )
        {
            /*		case WM_LBUTTONDOWN:
            			{
            				if ( m_nPickUpItemID != -1 )
            				{
            					if ( g_objHero.TryItem( m_nPickUpItemID, lParam ) )
            					{
            						CDlgMpcGoods::HeroEquipItem( lParam ) ;
            					}
            					else
            					{
            						return true;
            					}
            				}
            				else
            				{
            					CDlgMpcGoods::GetParent()->PostMessage( WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL ) ;
            				}
            			}
            			break ;
            		case WM_RBUTTONDBLCLK:
            			{
            				if ( m_nPickUpItemID != -1 )
            				{
            					if ( g_objHero.TryItem( m_nPickUpItemID, lParam ) )
            					{
            						g_objHero.UnequipItem( lParam ) ;
            						CDlgMpcGoods::HeroEquipItem( lParam ) ;
            					}
            					else
            					{
            						return true ;
            					}
            				}
            			}
            			break ;
            		case WM_RBUTTONDOWN:
            			{
            				g_objHero.UnequipItem( lParam ) ;
            				CDlgMpcGoods::GetParent()->PostMessage( WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL ) ;
            			}
            			break ;
            		case WM_LBUTTONDBLCLK:
            			{
            				m_nPickUpItemPosition = lParam ;
            				::GetCursorPos( &m_PickUpMousePnt ) ;
            				CDlgMpcGoods::GetParent()->PostMessage( WM_MY_MESSAGE, ON_ICON_PICKUP, DLG_EQUIP ) ;
            				m_nPickUpItemID = -1 ;
            			}
            			break ;*/
        case ON_MSGDLG_IMAGE:
            {
                m_nMouseMovePosition = lParam ;
                CDlgMpcGoods::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_GOODS, DLG_EQUIP ) ;
            }
            break ;
        default:
            break ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgMpcGoods::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgMpcGoods::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgMpcGoods::GetWindowRect ( rect ) ;
    m_fFrameStep = ( float ) (rect.Width () + 100) / DLG_FRAME_AMOUNT ;
    // Init the grid
    m_MpcGoodsGri.Init ( rect.left, rect.top, 5, 8, DLG_MPCGOODS );
    m_MpcStonesGri.Init ( rect.left, rect.top, 5, 8, DLG_STONEGOODS0 );
    m_MpcStonesGri.EnableWindow(false);
    // Init the static
    m_MpcGoodsMoneySta.Init( rect.left, rect.top );
    m_MpcGoodsMoneySta.SetFontColor( 0xffffff ) ;
    // Init the button
    m_MpcGoodsCloseBtn.Init ( rect.left, rect.top, "Button540" );
    m_MpcGoodsMoneyBtn.Init ( rect.left, rect.top, "Button120" );
    m_ChkStones.Init(rect.left, rect.top, "Button122", 2);
    m_ChkGoods.Init(rect.left, rect.top, "Button121", 2);
    m_ChkGoods.SetCheckValue(1);
    m_ChkStones.SetCheckValue(0);
    m_MpcGoodsMoneyBtn.SetTipShow ((char*)g_objGameDataSet.GetStr(10753));
    m_ChkGoods.SetTipShow((char*)g_objGameDataSet.GetStr(10742));
    m_ChkStones.SetTipShow((char*)g_objGameDataSet.GetStr(10743));
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMpcGoods::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_nPntX = x ;
    m_Pnt.y = y ;
}

void CDlgMpcGoods::Show()
{
    if ( m_bShow )
    {
        if ( m_nFrameMode != 0 )
        {
            if ( TimeGet() - m_dwLastShowTime <= 300 )
            {
                m_Pnt.x =(LONG)( m_nPntX - ( TimeGet() - m_dwLastShowTime ) / 15 * m_fFrameStep);
            }
            else
            {
                CRect rect ;
                CDlgMpcGoods::GetWindowRect( rect ) ;
                m_Pnt.x = _SCR_WIDTH -  rect.Width() - 100;
                m_nFrameMode = 0 ;
                if (m_ChkGoods.GetCheckValue() == 1 )
                {
                    m_MpcGoodsGri.EnableWindow();
                }
                else if (m_ChkStones.GetCheckValue() == 1)
                {
                    m_MpcStonesGri.EnableWindow();
                }
            }
        }
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog12",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( 0, m_Pnt.x, m_Pnt.y );
            ShowAni->Show ( 1, m_Pnt.x + 256, m_Pnt.y );
            ShowAni->Show ( 2, m_Pnt.x + 256, m_Pnt.y + 128);
        }
        else
        {
            return ;
        }
        // Show the grid
        if (m_MpcGoodsGri.IsWindowEnabled())
        {
            m_MpcGoodsGri.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        if (m_MpcStonesGri.IsWindowEnabled())
        {
            m_MpcStonesGri.Show(m_Pnt.x, m_Pnt.y);
        }
        // Show the static
        static DWORD dwShowTimes = 0;
        if ((++dwShowTimes) % 32 == 1)
        {
            char szMsg[256] = "";
            sprintf(szMsg, "%u", g_objHero.GetMoney());
            m_MpcGoodsMoneySta.SetWindowText (szMsg) ;
        }
        m_MpcGoodsMoneySta.Show( m_Pnt.x, m_Pnt.y ) ;
        // Show the button
        m_MpcGoodsCloseBtn.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MpcGoodsMoneyBtn.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChkStones.Show(m_Pnt.x, m_Pnt.y);
        m_ChkGoods.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgMpcGoods::SetGridContain()
{
    m_MpcGoodsGri.RemoveIcon ( 0, 0, true ) ;
    DWORD dwMoney = g_objHero.GetMoney () ;
    char strText[32] ;
    itoa ( dwMoney, strText, 10 ) ;
    m_MpcGoodsMoneySta.SetWindowText ( strText ) ;
    int nAmount = g_objHero.GetItemAmount();
    for ( int i = 0; i < nAmount; i++ )
    {
        CItem* pItem = g_objHero.GetItemByIndex ( i ) ;
        if ( pItem != NULL )
        {
            OBJID ItemTypeId = pItem->GetTypeID() ;
            int nIndex = m_MpcGoodsGri.InsertIcon( 1, 1, ItemTypeId, i, ICON_TYPE_ITEM ) ;
            m_MpcGoodsGri.SetIconRealID( nIndex, pItem->GetID () ) ;
        }
    }
}

void CDlgMpcGoods::OnMpcgoodsBtnClose()
{
    DXPlaySound( "Sound/window_close.wav" ) ;
	// CDlgMpcGoods::EnableWindow(false);
    CDlgMpcGoods::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCGOODS_CLOSE ) ;
}

void CDlgMpcGoods::OnMpcgoodsBtnMoney()
{
    DXPlaySound( "Sound/Dlg_Ok.wav" ) ;
    CDlgMpcGoods::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCGOODS_MONEYDROP ) ;
}



void CDlgMpcGoods::SetPickUpItemID(int nID)
{
    m_nPickUpItemID = nID ;
}

CPoint CDlgMpcGoods::GetPickUpMousePnt()
{
    return m_PickUpMousePnt ;
}

int CDlgMpcGoods::GetPickUpPosition()
{
    return m_nPickUpItemPosition ;
}

int CDlgMpcGoods::GetMouseMovePosition()
{
    return m_nMouseMovePosition ;
}

void CDlgMpcGoods::HeroEquipItem(int nPos)
{
    DXPlaySound ( "Sound/Equip.wav" ) ;
    g_objHero.EquipItem( nPos, m_nPickUpItemID ) ;
    g_bPickUp = false ;
    m_nPickUpItemID = 0 ;
    CDlgMpcGoods::GetParent ()->PostMessage( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
}

void CDlgMpcGoods::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnMouseMove(nFlags, point);
}

void CDlgMpcGoods::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (g_bPickUp)
    {
        CDlgMpcGoods::GetParent()->PostMessage(WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL);
    }
    CDialog::OnRButtonDown(nFlags, point);
}

void CDlgMpcGoods::SetStoneGrid()
{
    m_MpcStonesGri.RemoveIcon ( 0, 0, true ) ;
    int nAmount = g_objHero.GetItemAmount(PACKAGE_SOUL);
    for ( int i = 0; i < nAmount; i++ )
    {
        CItem* pItem = g_objHero.GetItemByIndex ( i, PACKAGE_SOUL ) ;
        if ( pItem != NULL )
        {
            OBJID ItemTypeId = pItem->GetTypeID() ;
            int nIndex = m_MpcStonesGri.InsertIcon( 1, 1, ItemTypeId, i, ICON_TYPE_ITEM ) ;
            m_MpcStonesGri.SetIconRealID( nIndex, pItem->GetID () ) ;
        }
    }
}

void CDlgMpcGoods::OnChkGoods()
{
    // TODO: Add your control notification handler code here
    DXPlaySound( "Sound/Dlg_Ok.wav" ) ;
    if (m_ChkGoods.GetCheckValue() == 1)
    {
        m_ChkStones.SetCheckValue(0);
        m_MpcStonesGri.EnableWindow(false);
        m_MpcGoodsGri.EnableWindow();
    }
    else
    {
        m_ChkGoods.SetCheckValue(1);
    }
}

void CDlgMpcGoods::OnChkStones()
{
    // TODO: Add your control notification handler code here
    DXPlaySound( "Sound/Dlg_Ok.wav" ) ;
    if (m_ChkStones.GetCheckValue() == 1)
    {
        m_ChkGoods.SetCheckValue(0);
        m_MpcStonesGri.EnableWindow();
        m_MpcGoodsGri.EnableWindow(false);
    }
    else
    {
        m_ChkStones.SetCheckValue(1);
    }
}


void CDlgMpcGoods::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgMpcGoods::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnLButtonUp(nFlags, point);
}

BOOL CDlgMpcGoods::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: Add your message handler code here and/or call default
    return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
