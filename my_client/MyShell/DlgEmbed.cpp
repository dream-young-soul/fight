
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgEmbed.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgEmbed.h"

#include "Hero.h"
#include "Item.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEmbed dialog


CDlgEmbed::CDlgEmbed(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgEmbed::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgEmbed)
    m_bShow = false ;
    m_nRealIcon = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgEmbed::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgEmbed)
    DDX_Control(pDX, IDC_EMBED_IMA_MONSTER, m_EmbedImaMonster);
    DDX_Control(pDX, IDC_EMBED_BTN_CANCEL, m_EmbedBtnCancel);
    DDX_Control(pDX, IDC_EMBED_CLOSE, m_EmbedBtnClose);
    DDX_Control(pDX, IDC_EMBED_IMA_GOOD, m_EmbedImaGood);
    DDX_Control(pDX, IDC_EMBED_IMA_GEM1, m_EmbedImaGem1);
    DDX_Control(pDX, IDC_EMBED_BTN_EMBED, m_EmbedBtnEmbed);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEmbed, CDialog)
    //{{AFX_MSG_MAP(CDlgEmbed)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_EMBED_IMA_GOOD, OnEmbedImaGood)
    ON_BN_CLICKED(IDC_EMBED_CLOSE, OnEmbedClose)
    ON_BN_CLICKED(IDC_EMBED_IMA_GEM1, OnEmbedImaGem1)
    ON_BN_DOUBLECLICKED(IDC_EMBED_IMA_GOOD, OnDoubleclickedEmbedImaGood)
    ON_BN_DOUBLECLICKED(IDC_EMBED_IMA_GEM1, OnDoubleclickedEmbedImaGem1)
    ON_BN_CLICKED(IDC_EMBED_BTN_EMBED, OnEmbedBtnEmbed)
    ON_BN_DOUBLECLICKED(IDC_EMBED_IMA_MONSTER, OnDoubleclickedEmbedImaMonster)
    ON_BN_CLICKED(IDC_EMBED_BTN_CANCEL, OnEmbedBtnCancel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEmbed message handlers

LRESULT CDlgEmbed::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if ( wParam )
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
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgEmbed::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgEmbed::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgEmbed::GetWindowRect ( rect ) ;
    // Init the image
    if ( ! m_EmbedImaGood.Init ( rect.left, rect.top, NULL, ITEM_IMAGE ) )
    {
        return false ;
    }
    if ( ! m_EmbedImaGem1.Init ( rect.left, rect.top, NULL, ITEM_IMAGE ) )
    {
        return false ;
    }
    if ( ! m_EmbedImaMonster.Init ( rect.left, rect.top, NULL, ITEM_IMAGE ) )
    {
        return false ;
    }
    // Init the button
    if ( ! m_EmbedBtnEmbed.Init ( rect.left, rect.top, "Button76" ) )
    {
        return false ;
    }
    if ( ! m_EmbedBtnCancel.Init ( rect.left, rect.top, "Button77" ) )
    {
        return false ;
    }
    if ( ! m_EmbedBtnClose.Init ( rect.left, rect.top, "Button540" ) )
    {
        return false ;
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEmbed::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgEmbed::Show()
{
    if (m_bShow)
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog29",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 1,
                            m_Pnt.x,
                            m_Pnt.y ) ;
        else
        {
            return ;
        }
        // Show the image
        m_EmbedImaGood.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_EmbedImaGem1.Show(m_Pnt.x, m_Pnt.y);
        m_EmbedImaMonster.Show(m_Pnt.x, m_Pnt.y);
        // Show the Button
        m_EmbedBtnEmbed.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_EmbedBtnCancel.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_EmbedBtnClose.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}


void CDlgEmbed::SetPickUpState( int nRealIcon)
{
    m_nRealIcon = nRealIcon ;
}

void CDlgEmbed::OnEmbedImaGood()
{
    if ( g_bPickUp && !m_EmbedImaGood.GetImageLoadState () )
    {
        CItem* pItem = g_objHero.GetItem ( m_nRealIcon ) ;
        // Test if the item is equipment
        if ( pItem->GetSort() == ITEMSORT_HELMET ||
                pItem->GetSort() == ITEMSORT_NECKLACE ||
                pItem->GetSort() == ITEMSORT_ARMOR ||
                pItem->GetSort() == ITEMSORT_WEAPON_SINGLE_HAND ||
                pItem->GetSort() == ITEMSORT_WEAPON_DOUBLE_HAND ||
                pItem->GetSort() == ITEMSORT_SHIELD ||
                pItem->GetSort() == ITEMSORT_RING ||
                pItem->GetSort() == ITEMSORT_SHOES )
        {
            // Test if the item is no broken
            if ( pItem->GetAmount() != pItem->GetAmountLimit () )
            {
                g_objGameMsg.AddMsg ( g_objGameDataSet.GetStr ( 10373 ) ) ;
                m_GoodID = 0 ;
                return ;
            }
            m_GoodID = pItem->GetID () ;
            m_EmbedImaGood.InsertImage (NULL, m_GoodID, 0, true) ;
            CDlgEmbed::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
            CDlgEmbed::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
        }
        else
        {
            m_GoodID = 0 ;
        }
    }
}

void CDlgEmbed::OnEmbedImaGem1()
{
    if ( g_bPickUp && !m_EmbedImaGem1.GetImageLoadState () )
    {
        CItem* pItem = g_objHero.GetItem ( m_nRealIcon ) ;
        if ( pItem != NULL && pItem->IsSoulStone())
        {
            m_Gem1ID = pItem->GetID () ;
            m_EmbedImaGem1.InsertImage (NULL , m_Gem1ID ) ;
            CDlgEmbed::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
            CDlgEmbed::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
        }
        else
        {
            m_Gem1ID = 0;
        }
    }
}

void CDlgEmbed::OnEmbedClose()
{
    m_EmbedImaGem1.RemoveImage () ;
    m_EmbedImaMonster.RemoveImage () ;
    m_EmbedImaGood.RemoveImage () ;
    m_GoodID = 0 ;
    m_Gem1ID = 0 ;
    m_MonsterID = 0 ;
    m_nRealIcon = 0 ;
    CDlgEmbed::EnableWindow(false) ;
    CDlgEmbed::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_EMBED_CLOSE ) ;
}



void CDlgEmbed::OnDoubleclickedEmbedImaGood()
{
    if ( m_EmbedImaGood.GetImageLoadState () )
    {
        m_EmbedImaGood.RemoveImage () ;
        m_GoodID = 0;
        CDlgEmbed::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
    }
}

void CDlgEmbed::OnDoubleclickedEmbedImaGem1()
{
    if ( m_EmbedImaGem1.GetImageLoadState() )
    {
        m_EmbedImaGem1.RemoveImage () ;
        m_GoodID = 0;
        CDlgEmbed::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
    }
}

void CDlgEmbed::OnEmbedBtnEmbed()
{
    if (m_EmbedImaGood.GetImageLoadState()
            && m_EmbedImaGem1.GetImageLoadState())
    {
        g_objHero.ImproveSoulDegree(m_GoodID, m_Gem1ID);
        CDlgEmbed::OnEmbedClose();
    }
}

void CDlgEmbed::InsertMonster(OBJID idMonster)
{
    if (m_EmbedImaMonster.GetImageLoadState())
    {
        return;
    }
    else
    {
        m_EmbedImaMonster.RemoveImage();
        m_EmbedImaMonster.InsertImage(NULL, idMonster);
        m_MonsterID = idMonster;
    }
}

void CDlgEmbed::OnDoubleclickedEmbedImaMonster()
{
    // TODO: Add your control notification handler code here
    if (m_EmbedImaMonster.GetImageLoadState())
    {
        m_EmbedImaMonster.RemoveImage();
        m_MonsterID = 0;
    }
}

void CDlgEmbed::OnEmbedBtnCancel()
{
    // TODO: Add your control notification handler code here
    CDlgEmbed::OnEmbedClose();
}

void CDlgEmbed::OnCloseB()
{
    CDlgEmbed::OnEmbedClose();
}
