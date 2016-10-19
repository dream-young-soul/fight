
//**********************************************************
// 代码编辑器
//**********************************************************

// MyGrid.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "MyGrid.h"

#include "Hero.h"
#include "Shop.h"
#include "3DGameMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CMyGrid
const int CHAR_WIDTH = 6 ;

CMyGrid::CMyGrid()
{
    m_GriPar.m_bMouseMove = false ;
    m_GriPar.m_bScale = true;
    m_GriPar.m_nGridAbleNum = 0 ;
    for ( int i = 0; i < 256; i++ )
    {
        m_GriPar.m_bGridAble[i] = false ;
    }
    m_GriPar.m_bEnable = true ;
    m_GriPar.m_bPickEnable = true ;
    m_GriPar.m_ClientRect = CRect ( 0, 0, 0, 0 ) ;
}

CMyGrid::~CMyGrid()
{
}


BEGIN_MESSAGE_MAP(CMyGrid, CButton)
    //{{AFX_MSG_MAP(CMyGrid)
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyGrid message handlers
void CMyGrid::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: Add your code to draw the specified item
}

BOOL CMyGrid::Init(int xPos, int yPos, BYTE btLine, BYTE btRow, BYTE btOwnerID)
{
    memset(&m_GriPar, 0L, sizeof(m_GriPar));
    CRect rect ;
    CMyGrid::GetWindowRect ( rect ) ;
    m_GriPar.m_IntPoint.x = rect.left - xPos ;
    m_GriPar.m_IntPoint.y = rect.top - yPos ;
    m_GriPar.m_ClientRect.right = ( rect.right - rect.left ) ;
    m_GriPar.m_ClientRect.bottom = ( rect.bottom - rect.top ) ;
    m_GriPar.m_iGridSpacex = 1;
    m_GriPar.m_iGridSpacey = 1;
    m_GriPar.m_btGridLine = btLine ;
    m_GriPar.m_btGridRow = btRow ;
    m_GriPar.m_btOwnerID = btOwnerID ;
    m_GriPar.m_btGridWidth = ( rect.Width () - (btRow - 1) * m_GriPar.m_iGridSpacex) / btRow ;
    m_GriPar.m_btGridHeight = ( rect.Height () - (btLine - 1) * m_GriPar.m_iGridSpacey ) / btLine ;
    for ( int i = 0; i < 256; i++ )
    {
        m_GriPar.m_bGridAble[i] = false ;
        m_GriPar.m_IconIndex[i] = 0;
        m_GriPar.m_IconTypeID[i] = 0;
        m_GriPar.m_IconRealID[i] = 0;
    }
    m_GriPar.m_bEnable = true;
    m_GriPar.m_bPickEnable = true;
    m_GriPar.m_bScale = true;
    return true ;
}

void CMyGrid::Show(int x, int y)
{
    m_GriPar.m_Point = CPoint ( m_GriPar.m_IntPoint.x + x, m_GriPar.m_IntPoint.y + y ) ;
    int index ;
    int nRowLeft ;
    int nLineLeft ;
    for ( int i = 1; i <= m_GriPar.m_btGridLine; i++ )
        for ( int j = 1; j <= m_GriPar.m_btGridRow; j++ )
        {
            index = ( i - 1 ) * m_GriPar.m_btGridRow + j ;
            nRowLeft = ( j - 1 ) * ( m_GriPar.m_btGridWidth + m_GriPar.m_iGridSpacex ) ;
            nLineLeft = ( i - 1 ) * ( m_GriPar.m_btGridHeight + m_GriPar.m_iGridSpacey ) ;
            // Shwo the focus
            if ( m_GriPar.m_bMouseMove && m_GriPar.m_MouseMoveIconLandR.x == j && m_GriPar.m_MouseMoveIconLandR.y == i )
            {
                CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni, "Image2", EXIGENCE_IMMEDIATE ) ;
                if ( ShowAni != NULL )
                {
                    RECT rect = { 0, 0, 32, 32 } ;
                    ShowAni->ShowEx( 0,
                                     m_GriPar.m_Point.x + nRowLeft,
                                     m_GriPar.m_Point.y + nLineLeft,
                                     &rect,
                                     m_GriPar.m_btGridWidth,
                                     m_GriPar.m_btGridHeight ) ;
                }
                if (!GetGridAble(CPoint(j, i)))
                {
                    char strInfo[32] = "";
                    int  nPosX = 0;
                    int  nPosY = 0;
                    switch(m_GriPar.m_btOwnerID)
                    {
                    case DLG_MPCGOODS:
                        //						sprintf(strInfo,"物品装备");
                        break;
                    case DLG_MONSTEREGG:
                        sprintf(strInfo, g_objGameDataSet.GetStr(10311));		//"幻兽蛋"
                        nPosY = -12;
                        break;
                    case DLG_MONSTERSKILL:
                        sprintf(strInfo, g_objGameDataSet.GetStr(10312));  // "幻兽技能"
                        nPosX = m_GriPar.m_btGridWidth;
                        break;
                    case DLG_STONEGOODS0:
                        //						sprintf(strInfo,"魔魂宝石");
                        break;
                    default:
                        break;
                    }
                    CMySize FontSize;
                    CMyBitmap::GetFontSize(FontSize);
                    if (strlen(strInfo) > 0)
                    {
                        g_objGameDataSet.ShowInfoBack( m_GriPar.m_Point.x + nRowLeft + m_GriPar.m_btGridWidth - strlen(strInfo)*CHAR_WIDTH - 5,
                                                       m_GriPar.m_Point.y + nLineLeft + nPosY - 1 - 15,
                                                       strlen(strInfo)*CHAR_WIDTH + 10,
                                                       FontSize.iHeight + 1 + 10) ;
                        CMyBitmap::ShowString(m_GriPar.m_Point.x + m_GriPar.m_btGridWidth - strlen(strInfo)*CHAR_WIDTH + nRowLeft,
                                              m_GriPar.m_Point.y + nLineLeft + nPosY - 10,
                                              0xffffff, strInfo);
                    }
                }
            }
            if ( GetGridAble ( CPoint ( j, i ) ) )
            {
                // show effect
                {
                    ItemEffectInfo* pInfo = g_objGameDataSet.GetItemEffectInfo(m_GriPar.m_IconTypeID[index - 1]);
                    if (pInfo && _stricmp(pInfo->strItemBgEffect.c_str(), "NULL") != 0)
                    {
                        if (!m_GriPar.m_objEffect[index - 1].TestEffect((char*)pInfo->strItemBgEffect.c_str()))
                        {
                            m_GriPar.m_objEffect[index - 1].Clear();
                            if (::RandGet(30) == 1)
                            {
                                m_GriPar.m_objEffect[index - 1].Add((char*)pInfo->strItemBgEffect.c_str());
                                m_GriPar.m_objEffect[index - 1].EverPlay();
                            }
                        }
                        CMyPos pPosWorld, pPosBg;
                        g_objGameMap.Screen2World(m_GriPar.m_Point.x + nRowLeft,
                                                  m_GriPar.m_Point.y + nLineLeft, pPosWorld.x, pPosWorld.y);
                        g_objGameMap.World2Bg(pPosWorld.x, pPosWorld.y, pPosBg.x, pPosBg.y );
                        m_GriPar.m_objEffect[index - 1].Process();
                        m_GriPar.m_objEffect[index - 1].Show(pPosBg);
                    }
                }
                switch ( m_GriPar.m_btIconType[index - 1] )
                {
                case ICON_TYPE_TASKITEM:
                    if (m_GriPar.m_IconTypeID[index-1])
                    {
                        ShowTypeItem(m_GriPar.m_IconTypeID[index-1],
                                     m_GriPar.m_Point.x + nRowLeft,
                                     m_GriPar.m_Point.y + nLineLeft );
                    }
                    break;
                case ICON_TYPE_ITEM:
                    {
                        // Show the item owner
                        CItem* pItem = g_objHero.GetItem ( m_GriPar.m_IconRealID[index - 1] ) ;
                        if ( pItem != NULL )
                        {
                            pItem->ShowMiniItem( m_GriPar.m_Point.x + nRowLeft,
                                                 m_GriPar.m_Point.y + nLineLeft , m_GriPar.m_bScale) ;
                            if (( pItem->GetTypeID() == _ARROW || pItem->GetTypeID() == _DART || pItem->GetSort() == ITEMSORT_EXPEND)
                                    && pItem->GetPackageType() != PACKAGE_EGG
                                    && pItem->GetAmount() > 1 && pItem->GetAmountLimit() > 1 )
                            {
                                char strNum[32] ;
                                itoa( pItem->GetAmount(), strNum, 10 ) ;
                                CMyBitmap::ShowString( m_GriPar.m_Point.x + nRowLeft + m_GriPar.m_btGridWidth - CHAR_WIDTH * strlen( strNum ),
                                                       m_GriPar.m_Point.y + nLineLeft + m_GriPar.m_btGridHeight - CHAR_WIDTH - 5,
                                                       0xffffffff,
                                                       strNum ) ;
                            }
                        }
                    }
                    break ;
                case ICON_TYPE_QUSE:
                    {
                        // Show the item owner
                        BOOL bLoad = false;
                        int nAmount = g_objHero.GetItemAmount();
                        for(int k = 0; k < nAmount; k ++)
                        {
                            CItem* pMyItem = g_objHero.GetItemByIndex(k);
                            if (pMyItem && pMyItem->GetTypeID() == m_GriPar.m_IconTypeID[index - 1])
                            {
                                pMyItem->ShowMiniItem( m_GriPar.m_Point.x + nRowLeft  ,
                                                       m_GriPar.m_Point.y + nLineLeft );
                                char strNum[8] ;
                                itoa( m_GriPar.m_IconNum[index - 1], strNum, 10 ) ;
                                CMyBitmap::ShowString( m_GriPar.m_Point.x + nRowLeft,
                                                       m_GriPar.m_Point.y + nLineLeft,
                                                       0xffffffff,
                                                       strNum ) ;
                                bLoad = true;
                                break;
                            }
                        }
                        if (!bLoad)
                        {
                            nAmount = g_objHero.GetItemAmount(PACKAGE_SOUL);
                            for(int k = 0; k < nAmount; k ++)
                            {
                                CItem* pMyItem = g_objHero.GetItemByIndex(k, PACKAGE_SOUL);
                                if (pMyItem && pMyItem->GetTypeID() == m_GriPar.m_IconTypeID[index - 1])
                                {
                                    pMyItem->ShowMiniItem( m_GriPar.m_Point.x + nRowLeft  ,
                                                           m_GriPar.m_Point.y + nLineLeft );
                                    char strNum[8] ;
                                    itoa( m_GriPar.m_IconNum[index - 1], strNum, 10 ) ;
                                    CMyBitmap::ShowString( m_GriPar.m_Point.x + nRowLeft,
                                                           m_GriPar.m_Point.y + nLineLeft,
                                                           0xffffffff,
                                                           strNum ) ;
                                    break;
                                }
                            }
                        }
                    }
                    break;
                case ICON_TYPE_SKILL:
                    {
                        char strName[32] ;
                        sprintf( strName, "MagicSkillType%d", m_GriPar.m_IconTypeID[index - 1] ) ;
                        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strMagicAni,
                                        strName,
                                        EXIGENCE_IMMEDIATE ) ;
                        RECT rect = { 0, 0, GRID_ICONMOVE_WIDTH, GRID_ICONMOVE_HEIGHT } ;
                        if ( ShowAni != NULL )
                        {
                            ShowAni->ShowEx( 0,
                                             m_GriPar.m_Point.x + nRowLeft ,
                                             m_GriPar.m_Point.y + nLineLeft ,
                                             &rect,
                                             GRID_ICONQUICK_WIDTH,
                                             GRID_ICONQUICK_HEIGHT ) ;
                        }
                    }
                    break ;
                case ICON_TYPE_SHOP:
                    {
                        // Show the item owner
                        CItem* pItem = g_objShop.GetItem( m_GriPar.m_IconIndex[index - 1] ) ;
                        if ( pItem != NULL )
                        {
                            pItem->ShowMiniItem( m_GriPar.m_Point.x + nRowLeft  ,
                                                 m_GriPar.m_Point.y + nLineLeft , m_GriPar.m_bScale ) ;
                        }
                    }
                    break ;
                case ICON_TYPE_DEPOT:
                    {
                        // Show the item owner
                        CItem* pItem = g_objHero.GetPackageItemByIndex( m_GriPar.m_IconIndex[index - 1] ) ;
                        if ( pItem != NULL )
                        {
                            pItem->ShowMiniItem( m_GriPar.m_Point.x + nRowLeft ,
                                                 m_GriPar.m_Point.y + nLineLeft, m_GriPar.m_bScale ) ;
                        }
                        else
                        {
                            pItem = g_objHero.GetItem(m_GriPar.m_IconRealID[index - 1]);
                            if (pItem != NULL)
                            {
                                pItem->ShowMiniItem( m_GriPar.m_Point.x + nRowLeft ,
                                                     m_GriPar.m_Point.y + nLineLeft, m_GriPar.m_bScale ) ;
                            }
                        }
                    }
                    break ;
                case ICON_TYPE_TRADE:
                    {
                        CItem* pItem = NULL ;
                        if ( m_GriPar.m_btOwnerID == DLG_TRADEMPC )
                        {
                            pItem = g_objHero.GetHeroTradeItemByIndex( m_GriPar.m_IconIndex[index - 1] ) ;
                        }
                        else if ( m_GriPar.m_btOwnerID == DLG_TRADENPC )
                        {
                            pItem = g_objHero.GetPlayerTradeItemByIndex( m_GriPar.m_IconIndex[index - 1] ) ;
                        }
                        if ( pItem != NULL )
                        {
                            pItem->ShowMiniItem( m_GriPar.m_Point.x + nRowLeft ,
                                                 m_GriPar.m_Point.y + nLineLeft , m_GriPar.m_bScale ) ;
                        }
                    }
                    break ;
                default:
                    break ;
                }
            }
        }
    // test if the mouse is out of the client rect
    if ( m_GriPar.m_bMouseMove )
    {
        MouseMoveTest ( ) ;
    }
}


CPoint CMyGrid::GetLineAndRow(CPoint MousePoint)
{
    CPoint LandR ;
    for ( int i = 1; i <= m_GriPar.m_btGridLine; i++ )
    {
        if ( MousePoint.y > i * m_GriPar.m_btGridHeight + (i - 1)*m_GriPar.m_iGridSpacey )
        {
            continue ;
        }
        else
        {
            LandR.y = i ;
            for ( int j = 1; j <= m_GriPar.m_btGridRow; j++ )
            {
                if ( MousePoint.x > j * m_GriPar.m_btGridWidth + ( j - 1)*m_GriPar.m_iGridSpacex)
                {
                    continue ;
                }
                else
                {
                    LandR.x = j ;
                }
                return LandR ;
            }
        }
    }
    return LandR ;
}

void CMyGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
    if ( !m_GriPar.m_bEnable )
    {
        return ;
    }
    CPoint LandR ;
    LandR = GetLineAndRow ( point ) ;
    BOOL IsAvailab = GetGridAble ( LandR ) ;
    if ( IsAvailab && m_GriPar.m_bPickEnable )
        // Full
    {
        if ( g_bPickUp )
            // Already pick up a icon
        {
            if ( m_GriPar.m_btIconType [GetGridIndex ( LandR ) - 1] == ICON_TYPE_ITEM ||
                    m_GriPar.m_btIconType [GetGridIndex ( LandR ) - 1] == ICON_TYPE_SHOP ||
                    m_GriPar.m_btIconType [GetGridIndex ( LandR ) - 1] == ICON_TYPE_DEPOT ||
                    m_GriPar.m_btIconType [GetGridIndex ( LandR ) - 1] == ICON_TYPE_QUSE )
            {
                if (m_GriPar.m_btOwnerID == DLG_MONSTERHATCH && m_GriPar.m_btIconType [GetGridIndex ( LandR ) - 1] == ICON_TYPE_ITEM)
                {
                    return;
                }
                //				if (m_GriPar.m_btOwnerID != DLG_NPCBUY)
                {
                    m_GriPar.m_PickUpIconTypeID = m_GriPar.m_IconTypeID[GetGridIndex ( LandR ) - 1] ;
                    m_GriPar.m_dwPickUpIconIndex = m_GriPar.m_IconIndex[GetGridIndex ( LandR ) - 1] ;
                    m_GriPar.m_btPickUpIconType = m_GriPar.m_btIconType[GetGridIndex ( LandR )- 1] ;
                    m_GriPar.m_PickUpIconRealID = m_GriPar.m_IconRealID[GetGridIndex ( LandR )- 1] ;
                    m_GriPar.m_InsetLandR = LandR ;
                    GetCursorPos( &m_GriPar.m_PickUpMousePoint ) ;
                    CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_TRADE,  m_GriPar.m_btOwnerID) ;
                }
            }
            else
            {
                m_GriPar.m_InsetLandR = LandR ;
                RemoveIcon ( LandR.y, LandR.x ) ;
                CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  m_GriPar.m_btOwnerID) ;
            }
        }
        else
            // Not pick up a icon
        {
            if ( m_GriPar.m_btIconType [GetGridIndex ( LandR ) - 1] == ICON_TYPE_ITEM ||
                    m_GriPar.m_btIconType [GetGridIndex ( LandR ) - 1] == ICON_TYPE_SHOP ||
                    m_GriPar.m_btIconType [GetGridIndex ( LandR ) - 1] == ICON_TYPE_DEPOT ||
                    m_GriPar.m_btIconType [GetGridIndex ( LandR ) - 1] == ICON_TYPE_QUSE )
            {
                if (m_GriPar.m_btOwnerID == DLG_TASKGET)
                {
                    return;
                }
                if (m_GriPar.m_btOwnerID == DLG_MONSTERHATCH && m_GriPar.m_btIconType [GetGridIndex ( LandR ) - 1] == ICON_TYPE_ITEM)
                {
                    return;
                }
                RemoveIcon ( LandR.y, LandR.x ) ;
                m_GriPar.m_PickUpIconLandR = LandR ;
                m_GriPar.m_PickUpIconTypeID = m_GriPar.m_IconTypeID[GetGridIndex ( LandR ) - 1] ;
                m_GriPar.m_dwPickUpIconIndex = m_GriPar.m_IconIndex[GetGridIndex ( LandR ) - 1] ;
                m_GriPar.m_btPickUpIconType = m_GriPar.m_btIconType[GetGridIndex ( LandR ) - 1] ;
                if ( m_GriPar.m_btIconType [GetGridIndex ( LandR ) - 1] == ICON_TYPE_QUSE )
                {
                    bool bLoad = false;
                    for ( int i = 0; i < g_objHero.GetItemAmount(); i++ )
                    {
                        CItem* pMyItem = g_objHero.GetItemByIndex( i );
                        if ( pMyItem && pMyItem->GetTypeID() == m_GriPar.m_PickUpIconTypeID )
                        {
                            m_GriPar.m_PickUpIconRealID = pMyItem->GetID() ;
                            bLoad = true;
                            break ;
                        }
                    }
                    if (!bLoad)
                    {
                        for(int j = 0; j < g_objHero.GetItemAmount(PACKAGE_SOUL); j++)
                        {
                            CItem* pMyItem = g_objHero.GetItemByIndex( j , PACKAGE_SOUL);
                            if ( pMyItem && pMyItem->GetTypeID() == m_GriPar.m_PickUpIconTypeID )
                            {
                                m_GriPar.m_PickUpIconRealID = pMyItem->GetID() ;
                                bLoad = true;
                                break ;
                            }
                        }
                    }
                }
                else
                {
                    m_GriPar.m_PickUpIconRealID = m_GriPar.m_IconRealID[GetGridIndex ( LandR )- 1] ;
                }
                GetCursorPos( &m_GriPar.m_PickUpMousePoint ) ;
                CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_PICKUP,  m_GriPar.m_btOwnerID) ;
            }
            else
            {
                LPARAM lParam ;
                CPoint MousePnt ;
                m_GriPar.m_btPickUpIconType = m_GriPar.m_btIconType [GetGridIndex ( LandR ) - 1] ;
                CMyGrid::SetGridAble ( LandR, false ) ;
                CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_ID, m_GriPar.m_IconTypeID [GetGridIndex ( LandR ) - 1] ) ;
                GetCursorPos ( &MousePnt ) ;
                lParam = MAKELONG ( MousePnt.x, MousePnt.y ) ;
                CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_PNT, lParam ) ;
                if ( m_GriPar.m_btPickUpIconType == ICON_TYPE_ACTION )
                {
                    CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_PICKUP, DLG_ACT ) ;
                }
                else if ( m_GriPar.m_btPickUpIconType == ICON_TYPE_SKILL )
                {
                    CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_PICKUP, DLG_MAGIC ) ;
                }
                else if ( m_GriPar.m_btPickUpIconType == ICON_TYPE_XP )
                {
                    CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_PICKUP, DLG_XP ) ;
                }
            }
        }
    }
    else if ( !IsAvailab )
        // Not Full
    {
        if ( g_bPickUp )
            // Already pick up a icon
        {
            m_GriPar.m_InsetLandR = LandR ;
            CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  m_GriPar.m_btOwnerID) ;
        }
        else
            // Not pick up a icon
        {
        }
    }
    CButton::OnLButtonDown(nFlags, point);
}

void CMyGrid::OnRButtonDown(UINT nFlags, CPoint point)
{
    if ( !m_GriPar.m_bEnable )
    {
        return ;
    }
    CPoint LandR ;
    LandR = GetLineAndRow ( point ) ;
    GetCursorPos( &m_GriPar.m_PickUpMousePoint ) ;
    BOOL IsAvailab = GetGridAble ( LandR ) ;
    if (g_bPickUp)
    {
        CMyGrid::GetParent()->GetParent()->PostMessage(WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL);
    }
    if ( IsAvailab )
    {
        // Send message to use icon
        int nPosX = m_GriPar.m_Point.x + (m_GriPar.m_btGridWidth + m_GriPar.m_iGridSpacex) * ( LandR.x - 1 ) ;
        int nPosY = m_GriPar.m_Point.y + (m_GriPar.m_btGridHeight + m_GriPar.m_iGridSpacey) * ( LandR.y - 1 ) ;
        LPARAM Point = MAKELONG( nPosX, nPosY ) ;
        if (m_GriPar.m_bScale == false)
        {
            Point = MAKELONG( nPosX + 32, nPosY + 32) ;
        }
        CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MSGDLG_POINT, Point ) ;
        if ( m_GriPar.m_btOwnerID == DLG_MAIN )
        {
            m_GriPar.m_dwPickUpIconIndex = m_GriPar.m_IconIndex [GetGridIndex ( LandR ) - 1] ;
            LPARAM lParam = (GetGridIndex( LandR ) - 1) | m_GriPar.m_IconTypeID [GetGridIndex (LandR) - 1] << 8 ;
            CMyGrid::GetParent()->GetParent()->PostMessage ( WM_MY_MESSAGE, ON_ICON_QUSE, lParam ) ;
        }
        else if ( m_GriPar.m_btOwnerID == DLG_STONEGOODS0 )
        {
            LPARAM lParam = MAKELONG(GetGridIndex(LandR) - 1, m_GriPar.m_btOwnerID);
            CMyGrid::GetParent()->GetParent()->PostMessage ( WM_MY_MESSAGE, ON_ICON_USE, lParam ) ;
        }
        else if ( m_GriPar.m_btOwnerID == DLG_MPCGOODS )
        {
            m_GriPar.m_dwPickUpIconIndex = m_GriPar.m_IconIndex [GetGridIndex ( LandR ) - 1] ;
            LPARAM lParam = MAKELONG(m_GriPar.m_dwPickUpIconIndex, m_GriPar.m_btOwnerID);
            CMyGrid::GetParent()->GetParent()->PostMessage ( WM_MY_MESSAGE, ON_ICON_USE, lParam ) ;
        }
        else if ( m_GriPar.m_btOwnerID == DLG_NPCBUY )
        {
            m_GriPar.m_PickUpIconTypeID = m_GriPar.m_IconTypeID [GetGridIndex ( LandR ) - 1] ;
            m_GriPar.m_PickUpIconRealID = m_GriPar.m_IconRealID[GetGridIndex ( LandR )- 1] ;
            CMyGrid::GetParent()->GetParent()->PostMessage ( WM_MY_MESSAGE, ON_ICON_BUY, m_GriPar.m_PickUpIconTypeID ) ;
        }
        else if ( m_GriPar.m_btOwnerID == DLG_MONSTERDEPOT)
        {
            m_GriPar.m_dwPickUpIconIndex = m_GriPar.m_IconIndex [GetGridIndex ( LandR ) - 1] ;
            LPARAM lParam = MAKELONG(m_GriPar.m_dwPickUpIconIndex, m_GriPar.m_btOwnerID);
            CMyGrid::GetParent()->GetParent()->PostMessage ( WM_MY_MESSAGE, ON_ICON_USE, lParam ) ;
        }
        else if (m_GriPar.m_btOwnerID == DLG_MONSTERHATCH)
        {
            m_GriPar.m_dwPickUpIconIndex = m_GriPar.m_IconIndex [GetGridIndex ( LandR ) - 1] ;
            LPARAM lParam = MAKELONG(m_GriPar.m_dwPickUpIconIndex, m_GriPar.m_btOwnerID);
            CMyGrid::GetParent()->GetParent()->PostMessage ( WM_MY_MESSAGE, ON_ICON_USE, lParam ) ;
        }
        else if ( m_GriPar.m_btOwnerID == DLG_AUCPACKAGE)
        {
            m_GriPar.m_dwPickUpIconIndex = m_GriPar.m_IconIndex [GetGridIndex ( LandR ) - 1] ;
            LPARAM lParam = MAKELONG(m_GriPar.m_dwPickUpIconIndex, m_GriPar.m_btOwnerID);
            CMyGrid::GetParent()->GetParent()->PostMessage ( WM_MY_MESSAGE, ON_ICON_USE, lParam ) ;
        }
        else if ( m_GriPar.m_btOwnerID == DLG_DEPOT )
        {
            m_GriPar.m_dwPickUpIconIndex = m_GriPar.m_IconIndex [GetGridIndex ( LandR ) - 1] ;
            LPARAM lParam = MAKELONG(m_GriPar.m_dwPickUpIconIndex, m_GriPar.m_btOwnerID);
            CMyGrid::GetParent()->GetParent()->PostMessage ( WM_MY_MESSAGE, ON_ICON_USE, lParam ) ;
        }
    }
    CButton::OnRButtonDown(nFlags, point);
}

BOOL CMyGrid::GetGridAble(CPoint lrPoint)
{
    int Index = ( lrPoint.y - 1 ) * m_GriPar.m_btGridRow + lrPoint.x;
    if ( Index < 0 || Index > m_GriPar.m_btGridLine * m_GriPar.m_btGridRow )
    {
        return false;
    }
    else
    {
        return m_GriPar.m_bGridAble[Index - 1];
    }
}

int CMyGrid::InsertIcon(int nLine, int nRow, OBJID ItemTypeID, DWORD dwItemIndex, BYTE btItemType, BOOL bAuto)
{
    CPoint lrPoint ;
    lrPoint.x = nRow ;
    lrPoint.y = nLine ;
    if ( ! (nLine <= m_GriPar.m_btGridLine && nRow <= m_GriPar.m_btGridRow ) )
    {
        return -1 ;
    }
    if ( bAuto )
    {
        if ( !GetGridAble ( lrPoint ) )
            // Not full can insert
        {
            if ( LoadIcon( lrPoint, ItemTypeID, dwItemIndex, btItemType) )
            {
                m_GriPar.m_nGridAbleNum++ ;
                return ( lrPoint.y - 1 ) * m_GriPar.m_btGridRow + lrPoint.x - 1;
            }
        }
        // full, auto insert
        else
        {
            for ( int i = 1; i <= m_GriPar.m_btGridLine; i++ )
                for ( int j = 1; j <= m_GriPar.m_btGridRow; j++ )
                {
                    if ( !GetGridAble ( CPoint ( j, i ) ) )
                    {
                        if ( LoadIcon ( CPoint ( j, i ), ItemTypeID, dwItemIndex, btItemType ) )
                        {
                            m_GriPar.m_nGridAbleNum++ ;
                            return ( i - 1 ) * m_GriPar.m_btGridRow + j - 1; ;
                        }
                    }
                }
        }
    }
    else
    {
        if ( LoadIcon( lrPoint, ItemTypeID, dwItemIndex, btItemType ) )
        {
            m_GriPar.m_nGridAbleNum++ ;
            return ( lrPoint.y - 1 ) * m_GriPar.m_btGridRow + lrPoint.x - 1;
        }
    }
    return -1 ;
}

void CMyGrid::SetGridAble(CPoint lrPoint, BOOL bAble)
{
    int Index = ( lrPoint.y - 1 ) * m_GriPar.m_btGridRow + lrPoint.x ;
    m_GriPar.m_bGridAble[Index - 1] = bAble ;
}

BOOL CMyGrid::LoadIcon(CPoint lrPoint, OBJID ItemTypeID, DWORD dwItemIndex, BYTE btItemType)
{
    SetGridAble ( lrPoint, true ) ;
    int Index = ( lrPoint.y - 1 ) * m_GriPar.m_btGridRow + lrPoint.x ;
    m_GriPar.m_IconTypeID [Index-1] = ItemTypeID ;
    m_GriPar.m_IconIndex[Index-1] = dwItemIndex ;
    m_GriPar.m_btIconType[Index-1] = btItemType ;
    if ( btItemType == ICON_TYPE_QUSE )
    {
        m_GriPar.m_QUseItem[Index-1].Create( Index, ItemTypeID ) ;
    }
    return true ;
}

int CMyGrid::GetGridIndex(CPoint lrPoint)
{
    return ( lrPoint.y - 1 ) * m_GriPar.m_btGridRow + lrPoint.x ;
}

OBJID CMyGrid::GetPickUpTypeID ( )
{
    return m_GriPar.m_PickUpIconTypeID ;
}

int CMyGrid::GetPickUpIndex()
{
    return m_GriPar.m_dwPickUpIconIndex ;
}

DWORD CMyGrid::GetMouseMoveIndex()
{
    return m_GriPar.m_dwMouseMoveIconIndex ;
}

DWORD CMyGrid::GetMouseMoveTypeID()
{
    return m_GriPar.m_dwMouseMoveIconTypeID ;
}

CPoint CMyGrid::GetPickUpMousePnt()
{
    return m_GriPar.m_PickUpMousePoint ;
}

void CMyGrid::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if ( !m_GriPar.m_ClientRect.PtInRect ( point ) )
    {
        return  ;
    }
    CPoint LandR ;
    LandR = GetLineAndRow ( point ) ;
    BOOL IsAvailab = GetGridAble ( LandR ) ;
    if ( IsAvailab )
    {
        if ( m_GriPar.m_btIconType [ GetGridIndex ( LandR ) - 1 ] == ICON_TYPE_ACTION )
        {
            return ;
        }
        if ( !m_GriPar.m_bMouseMove )
        {
            m_GriPar.m_bMouseMove = true ;
            m_GriPar.m_MouseMoveIconLandR = LandR ;
            int nPosX = m_GriPar.m_Point.x + (m_GriPar.m_btGridWidth + m_GriPar.m_iGridSpacex) * ( LandR.x - 1 ) ;
            int nPosY = m_GriPar.m_Point.y + (m_GriPar.m_btGridHeight + m_GriPar.m_iGridSpacey) * ( LandR.y - 1 ) ;
            m_GriPar.m_dwMouseMoveIconIndex = m_GriPar.m_IconIndex [GetGridIndex ( LandR ) - 1 ];
            m_GriPar.m_dwMouseMoveIconTypeID = m_GriPar.m_IconTypeID [GetGridIndex ( LandR ) - 1 ];
            m_GriPar.m_PickUpIconRealID = m_GriPar.m_IconRealID[GetGridIndex ( LandR )- 1] ;
            LPARAM lParam = MAKELONG( nPosX, nPosY ) ;
            if (m_GriPar.m_bScale == false)
            {
                lParam = MAKELONG( nPosX + 32, nPosY + 32 );
            }
            if ( m_GriPar.m_btIconType [ GetGridIndex ( LandR ) - 1 ] == ICON_TYPE_SKILL ||
                    m_GriPar.m_btIconType [ GetGridIndex ( LandR ) - 1 ] == ICON_TYPE_XP )
            {
                CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MSGDLG_POINT, lParam ) ;
                CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MSGDLG_IMAGE, m_GriPar.m_dwMouseMoveIconTypeID ) ;
                return ;
            }
            CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MSGDLG_POINT, lParam ) ;
            CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MSGDLG_GOODS, m_GriPar.m_btOwnerID ) ;
        }
        else if ( m_GriPar.m_bMouseMove && m_GriPar.m_MouseMoveIconLandR != LandR )
        {
            m_GriPar.m_bMouseMove = true ;
            m_GriPar.m_MouseMoveIconLandR = LandR ;
            int nPosX = m_GriPar.m_Point.x + (m_GriPar.m_btGridWidth + m_GriPar.m_iGridSpacex) * ( LandR.x - 1 ) ;
            int nPosY = m_GriPar.m_Point.y + (m_GriPar.m_btGridHeight + m_GriPar.m_iGridSpacey) * ( LandR.y - 1 ) ;
            m_GriPar.m_dwMouseMoveIconIndex = m_GriPar.m_IconIndex [GetGridIndex ( LandR ) - 1 ];
            m_GriPar.m_dwMouseMoveIconTypeID = m_GriPar.m_IconTypeID [GetGridIndex ( LandR ) - 1 ];
            m_GriPar.m_PickUpIconRealID = m_GriPar.m_IconRealID[GetGridIndex ( LandR )- 1] ;
            if ( m_GriPar.m_btIconType [ GetGridIndex ( LandR ) - 1 ] == ICON_TYPE_QUSE )
            {
                nPosX = m_GriPar.m_Point.x + (m_GriPar.m_btGridWidth + m_GriPar.m_iGridSpacex) * ( LandR.x - 1 );
                nPosY = m_GriPar.m_Point.y + (m_GriPar.m_btGridHeight + m_GriPar.m_iGridSpacey) * ( LandR.y - 1 );
            }
            LPARAM lParam = MAKELONG( nPosX, nPosY ) ;
            if (m_GriPar.m_bScale == false)
            {
                lParam = MAKELONG( nPosX + 32, nPosY + 32 );
            }
            if ( m_GriPar.m_btIconType [ GetGridIndex ( LandR ) - 1 ] == ICON_TYPE_SKILL ||
                    m_GriPar.m_btIconType [ GetGridIndex ( LandR ) - 1 ] == ICON_TYPE_XP )
            {
                CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MSGDLG_POINT, lParam ) ;
                CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MSGDLG_IMAGE, m_GriPar.m_dwMouseMoveIconTypeID ) ;
                return ;
            }
            CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MSGDLG_POINT, lParam ) ;
            CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MSGDLG_GOODS, m_GriPar.m_btOwnerID ) ;
        }
    }
    else
    {
        if ( m_GriPar.m_bMouseMove && !CMyGrid::IsWindowVisible() )
        {
            m_GriPar.m_bMouseMove = false ;
            CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MSGDLG_HIDE ) ;
        }
        else if ( m_GriPar.m_bMouseMove && m_GriPar.m_MouseMoveIconLandR != LandR )
        {
            m_GriPar.m_MouseMoveIconLandR = LandR ;
            CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MSGDLG_HIDE ) ;
        }
        else if ( !m_GriPar.m_bMouseMove && CMyGrid::IsWindowVisible() )
        {
            m_GriPar.m_bMouseMove = true ;
            m_GriPar.m_MouseMoveIconLandR = LandR ;
        }
    }
    CButton::OnMouseMove(nFlags, point);
}

void CMyGrid::MouseMoveTest()
{
    POINT MousePoint ;
    GetCursorPos ( &MousePoint ) ;
    CRect rect ;
    CMyGrid::GetWindowRect ( rect ) ;
    if ( !rect.PtInRect ( MousePoint ) )
    {
        m_GriPar.m_bMouseMove = false ;
        CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MSGDLG_HIDE ) ;
    }
}

CPoint CMyGrid::GetInsertLandR()
{
    return m_GriPar.m_InsetLandR ;
}

int CMyGrid::ChangeIcon(CPoint IconLandR, OBJID ItemTypeID, OBJID ItemRealID, DWORD dwItemIndex, BYTE btItemType)
{
    int nIndex = ( IconLandR.y - 1 ) * m_GriPar.m_btGridRow + IconLandR.x ;
    m_GriPar.m_PickUpIconTypeID = m_GriPar.m_IconTypeID [nIndex - 1] ;
    m_GriPar.m_dwPickUpIconIndex = m_GriPar.m_IconIndex [nIndex - 1] ;
    m_GriPar.m_PickUpIconRealID = m_GriPar.m_IconRealID [nIndex - 1] ;
    m_GriPar.m_IconTypeID [nIndex-1] = ItemTypeID ;
    m_GriPar.m_IconRealID [nIndex-1] = ItemRealID ;
    m_GriPar.m_IconIndex[nIndex-1] = dwItemIndex ;
    m_GriPar.m_btIconType[nIndex-1] = btItemType ;
    return nIndex - 1 ;
}

void CMyGrid::RemoveIcon(int nLine, int nRow, BOOL bAll)
{
    if ( bAll )
    {
        m_GriPar.m_nGridAbleNum = 0;
        for ( int i = 0; i < 256; i++ )
        {
            m_GriPar.m_IconIndex[i] = 0;
            m_GriPar.m_IconTypeID[i] = 0;
            m_GriPar.m_IconRealID[i] = 0;
            m_GriPar.m_bGridAble[i] = false ;
        }
        return ;
    }
    else
    {
        SetGridAble( CPoint ( nRow, nLine ), false ) ;
        m_GriPar.m_nGridAbleNum-- ;
    }
}

void CMyGrid::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    CButton::OnLButtonDblClk(nFlags, point);
}



void CMyGrid::ReSumePickUp()
{
    LoadIcon( m_GriPar.m_PickUpIconLandR, m_GriPar.m_PickUpIconTypeID, m_GriPar.m_dwPickUpIconIndex, m_GriPar.m_btPickUpIconType ) ;
}

void CMyGrid::SetEnable(BOOL bEnable)
{
    m_GriPar.m_bEnable = bEnable ;
}

void CMyGrid::SetPickEnable(BOOL bEnable)
{
    m_GriPar.m_bPickEnable = bEnable ;
}

int CMyGrid::GetIconTypeID(int nIndex)
{
    return m_GriPar.m_IconTypeID [nIndex] ;
}

void CMyGrid::SetIconNum(int nIndex, int nNum)
{
    m_GriPar.m_IconNum[nIndex] = nNum ;
}

void CMyGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
    if ( m_GriPar.m_btPickUpIconType == ICON_TYPE_ACTION )
    {
        CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP, DLG_ACT ) ;
    }
    else if ( m_GriPar.m_btPickUpIconType == ICON_TYPE_SKILL )
    {
        CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP, DLG_MAGIC ) ;
    }
    else if ( m_GriPar.m_btPickUpIconType == ICON_TYPE_XP )
    {
        CMyGrid::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP, DLG_XP ) ;
    }
    CButton::OnLButtonUp(nFlags, point);
}

BYTE CMyGrid::GetPickUpType()
{
    return m_GriPar.m_btPickUpIconType ;
}

BYTE CMyGrid::GetIconType(int nIndex)
{
    return m_GriPar.m_btIconType [nIndex] ;
}

BYTE CMyGrid::GetPickUpActType()
{
    return m_GriPar.m_btPickUpIconActType ;
}

void CMyGrid::SetIconRealID(int nIndex, OBJID rID)
{
    m_GriPar.m_IconRealID [nIndex] = rID ;
}


int CMyGrid::GetPickUpRealID()
{
    return m_GriPar.m_PickUpIconRealID ;
}

int CMyGrid::GetIconRealID(int nIndex)
{
    return m_GriPar.m_IconRealID[nIndex] ;
}

void CMyGrid::ReSetGridSpace(int x, int y)
{
    CRect rect ;
    CMyGrid::GetWindowRect ( rect ) ;
    m_GriPar.m_iGridSpacex = x;
    m_GriPar.m_iGridSpacey = y;
    m_GriPar.m_btGridWidth = ( rect.Width () - (m_GriPar.m_btGridRow - 1) * m_GriPar.m_iGridSpacex) / m_GriPar.m_btGridRow  ;
    m_GriPar.m_btGridHeight = ( rect.Height () - (m_GriPar.m_btGridLine - 1) * m_GriPar.m_iGridSpacey ) / m_GriPar.m_btGridLine ;
}

BOOL CMyGrid::GetGridAbleByIndex(int nIndex)
{
    if ( nIndex < 0 || nIndex >= m_GriPar.m_btGridLine * m_GriPar.m_btGridRow )
    {
        return false;
    }
    else
    {
        return m_GriPar.m_bGridAble[nIndex];
    }
}

void CMyGrid::ShowTypeItem(OBJID idType, int nX, int nY)
{
    char szAni[256] = "";
    sprintf(szAni, "Item%u", idType);
    CAni* pAni = g_objGameDataSet.GetDataAni("ani/ItemMinIcon.Ani", szAni);
    if (!pAni)
    {
        return;
    }
    CMyBitmap* pBmp = pAni->GetFrame(0);
    if (!pBmp)
    {
        return;
    }
    CMySize sizeBmp;
    pBmp->GetSize(sizeBmp.iWidth, sizeBmp.iHeight);
    pBmp->ShowEx(nX, nY , NULL, sizeBmp.iWidth / 2, sizeBmp.iHeight / 2);
}

CPoint	CMyGrid::GetGridPnt(int Line, int Row)
{
    CPoint Pnt;
    Pnt.x = m_GriPar.m_Point.x + (m_GriPar.m_btGridWidth + m_GriPar.m_iGridSpacex) * ( Row - 1 ) ;
    Pnt.y = m_GriPar.m_Point.y + (m_GriPar.m_btGridHeight + m_GriPar.m_iGridSpacey) * ( Line - 1 ) ;
    return Pnt;
}
