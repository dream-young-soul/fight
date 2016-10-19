
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MyImage.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "MyImage.h"
#include "MyShellDlg.h"
#include "Hero.h"
#include "3DGameMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyImage

CMyImage::CMyImage()
{
    m_bImageLoad = false ;
    m_bMouseMove = false ;
    m_uFrame = 0 ;
    m_ShowRect.left = 0 ;
    m_ShowRect.top = 0  ;
    m_nImageTypeID = 0 ;
    m_dwImageID = 0 ;
    m_bImgZoom = false ;
    m_nCurMoney = 0 ;
    m_btMilNum = 0 ;
    m_btThoNum = 0 ;
    m_btHunNum = 0 ;
    m_btGoldStar = 0 ;
    m_btSillerStar = 0 ;
    m_btCopperStar = 0 ;
    m_nPosition = 0 ;
}

CMyImage::~CMyImage()
{
}


BEGIN_MESSAGE_MAP(CMyImage, CButton)
    //{{AFX_MSG_MAP(CMyImage)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyImage message handlers

BOOL CMyImage::Init(int xPos, int yPos, char* ImageID, BYTE btItemType, int nItemPos )
{
    CRect rect ;
    CMyImage::GetWindowRect ( rect ) ;
    m_IntPoint.x = rect.left - xPos ;
    m_IntPoint.y = rect.top - yPos ;
    m_nWidth = rect.Width() ;
    m_nHeight = rect.Height() ;
    m_btItemType = btItemType ;
    switch ( btItemType )
    {
    case NORMAL_IMAGE:
        strcpy ( m_strAni, g_strControlAni ) ;
        m_nItemPos = nItemPos ;
        break ;
    case ITEM_IMAGE:
        m_nItemPos = nItemPos ;
        break ;
    case BG_IMAGE:
        strcpy ( m_strAni, g_strControlAni ) ;
        m_nItemPos = nItemPos ;
        break ;
    case SKILL_IMAGE:
        strcpy( m_strAni, g_strMagicAni ) ;
        break ;
    case NPCITEM_IMAGE:
        m_nItemPos = nItemPos ;
        break ;
    case BOOTHITEM_IMAGE:
        strcpy(m_strAni, "ani/ItemMinIcon.Ani");
        break;
    default:
        break ;
    }
    if ( ImageID != NULL )
    {
        m_bImageLoad = true ;
        CMySize Size = { 0, 0 } ;
        if ( m_btItemType != ITEM_IMAGE && m_btItemType != NPCITEM_IMAGE )
        {
            strcpy ( m_strImageID, ImageID ) ;
            CAni* pSizeAni = g_objGameDataSet.GetDataAni ( ( char* )m_strAni, ImageID, EXIGENCE_IMMEDIATE ) ;
            if ( pSizeAni != NULL )
            {
                Size = pSizeAni->GetSize ( m_uFrame ) ;
            }
            else
            {
                return false;
            }
            m_ShowRect.right = Size.iWidth ;
            m_ShowRect.bottom = Size.iHeight ;
        }
    }
    return true ;
}

BOOL CMyImage::InsertImage(char* ImageID, DWORD dwImageID, int nImageTypeID, BOOL bImgZoom )
{
    m_bImageLoad = true ;
    m_bImgZoom = bImgZoom ;
    m_dwImageID = dwImageID ;
    m_nImageTypeID = nImageTypeID ;
    CMySize Size = { 0, 0 } ;
    switch ( m_btItemType )
    {
    case ITEM_IMAGE:
        break ;
    case MEDAL_IMAGE:
        break ;
    case FACE_IMAGE:
        break ;
    case NPCITEM_IMAGE:
        break ;
    case EMBLEM_IMAGE:
        break ;
    case PETFACE_IMAGE:
        break;
    case SMALLPETFACE_IMAGE:
        break;
    default:
        {
            strcpy ( m_strImageID, ImageID ) ;
            CAni* pSizeAni = g_objGameDataSet.GetDataAni ( ( char* )m_strAni, ImageID, EXIGENCE_IMMEDIATE ) ;
            if ( pSizeAni != NULL )
            {
                Size = pSizeAni->GetSize ( m_uFrame ) ;
            }
            else
            {
                return false ;
            }
            m_ShowRect.right = Size.iWidth ;
            m_ShowRect.bottom = Size.iHeight ;
        }
        break ;
    }
    return true ;
}

void CMyImage::RemoveImage()
{
    m_bImageLoad = false ;
}

void CMyImage::Show(int x, int y)
{
    m_Point = CPoint ( x + m_IntPoint.x, y + m_IntPoint.y ) ;
    if ( m_bMouseMove && ( m_btItemType == ITEM_IMAGE || m_btItemType == NPCITEM_IMAGE || m_btItemType == BOOTHITEM_IMAGE ) )
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni, "Image2", EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            RECT rect = { 0,
                          0,
                          32,
                          32
                        } ;
            ShowAni->ShowEx( 0,
                             m_Point.x,
                             m_Point.y,
                             &rect,
                             m_nWidth,
                             m_nHeight ) ;
            if (!m_bImageLoad && m_nPosition != 0
                    && m_nPosition != DLG_IMPROVE
                    && m_nPosition != DLG_IDENTIFY
                    && m_nPosition != DLG_ADDAUCITEM
                    && m_nPosition != DLG_AUCTION
                    && m_btItemType == ITEM_IMAGE)
            {
                CMySize FontSize;
                CMyBitmap::GetFontSize(FontSize);
                g_objGameDataSet.ShowInfoBack(m_Point.x - 5 - FontSize.iWidth + 10,	m_Point.y - FontSize.iHeight - 17,
                                              strlen(g_objGameDataSet.GetStr( 10530 + m_nPosition ))*FontSize.iWidth + 10,
                                              13 + 10) ;
                CMyBitmap::ShowString( m_Point.x - FontSize.iWidth + 10,
                                       m_Point.y - FontSize.iHeight - 12,
                                       0xffffff,
                                       g_objGameDataSet.GetStr( 10530 + m_nPosition ) ) ;
            }
        }
    }
    if ( m_bImageLoad )
    {
        // show effect
        ShowItemEffect();
        switch ( m_btItemType )
        {
        case ITEM_IMAGE:
            {
                CItem* pItem = g_objHero.GetItem( m_dwImageID, false ) ;
                if ( pItem != NULL )
                {
                    BOOL bScale = true ;
                    if ( m_bImgZoom )
                    {
                        bScale = false ;
                    }
                    if ( m_bMouseMove )
                    {
                        pItem->ShowMiniItem( m_Point.x ,
                                             m_Point.y, bScale);
                    }
                    else
                    {
                        pItem->ShowMiniItem( m_Point.x,
                                             m_Point.y, bScale);
                    }
                }
            }
            break ;
        case NPCITEM_IMAGE:
            {
                CItem* pItem = g_objHero.m_objPlayerDummy.GetEquipment( m_nPosition ) ;
                if ( pItem != NULL )
                {
                    BOOL bScale = true ;
                    if ( m_bImgZoom )
                    {
                        bScale = false ;
                    }
                    if ( m_bMouseMove )
                    {
                        pItem->ShowMiniItem( m_Point.x,
                                             m_Point.y, bScale);
                    }
                    else
                    {
                        pItem->ShowMiniItem( m_Point.x,
                                             m_Point.y, bScale );
                    }
                }
            }
            break ;
        case PETFACE_IMAGE:
            {
                CAni* pAni = g_objGameDataSet.GetPetFaceIcon(m_nImageTypeID, EXIGENCE_IMMEDIATE ) ;
                if ( pAni != NULL )
                {
                    if ( m_bImgZoom )
                    {
                        pAni->ShowEx( 0,
                                      m_Point.x,
                                      m_Point.y,
                                      CRect(0, 0, 128, 128),
                                      m_nWidth, m_nHeight) ;
                    }
                    else
                    {
                        pAni->ShowEx( 1,
                                      m_Point.x,
                                      m_Point.y,
                                      CRect(0, 0, 128, 128),
                                      m_nWidth, m_nHeight) ;
                    }
                }
            }
            break;
        case SMALLPETFACE_IMAGE:
            {
                CAni* pAni = g_objGameDataSet.GetPetFaceIcon(m_nImageTypeID, EXIGENCE_IMMEDIATE ) ;
                if ( pAni != NULL )
                {
                    if ( m_bImgZoom )
                    {
                        pAni->Show( 2,
                                    m_Point.x,
                                    m_Point.y ) ;
                    }
                    else
                    {
                        pAni->Show( 3,
                                    m_Point.x,
                                    m_Point.y ) ;
                    }
                }
                CItem* pPet = g_objHero.GetItem (m_dwImageID);
                if (pPet)
                {
                    char strLev[16];
                    sprintf(strLev, "%d", pPet->GetAmountLimit ());
                    CMyBitmap::ShowString (m_Point.x, m_Point.y, 0xffffff, strLev);
                }
            }
            break;
        case FACE_IMAGE:
            {
                CAni* pAni = g_objGameDataSet.GetPlayerFaceIcon( m_dwImageID, m_nImageTypeID, EXIGENCE_IMMEDIATE ) ;
                if ( pAni != NULL )
                {
                    if ( m_bImgZoom )
                    {
                        pAni->ShowEx( 0,
                                      m_Point.x,
                                      m_Point.y,
                                      CRect(0, 0, 128, 128),
                                      m_nWidth, m_nHeight) ;
                    }
                    else
                    {
                        pAni->Show( 1,
                                    m_Point.x,
                                    m_Point.y ) ;
                    }
                }
            }
            break ;
        case EMBLEM_IMAGE:
            {
                CAni* pAni = g_objGameDataSet.GetEmblemIcon( m_dwImageID, EXIGENCE_IMMEDIATE ) ;
                pAni->Show( 0,
                            m_Point.x,
                            m_Point.y ) ;
            }
            break ;
        case MEDAL_IMAGE:
            {
                CAni* pAni = g_objGameDataSet.GetMedal( m_dwImageID, EXIGENCE_IMMEDIATE ) ;
                if ( pAni != NULL )
                {
                    if ( m_bImgZoom )
                    {
                        pAni->Show( 0,
                                    m_Point.x,
                                    m_Point.y ) ;
                    }
                    else
                    {
                        pAni->Show( 1,
                                    m_Point.x,
                                    m_Point.y ) ;
                    }
                }
            }
            break ;
        case SKILL_IMAGE:
            {
                CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) m_strAni,
                                m_strImageID,
                                EXIGENCE_IMMEDIATE ) ;
                if ( ShowAni != NULL )
                {
                    if ( !m_bImgZoom )
                    {
                        ShowAni->Show ( m_uFrame,
                                        m_Point.x,
                                        m_Point.y ) ;
                    }
                    else
                    {
                        RECT rect = { 0, 0, 50, 50 } ;
                        ShowAni->ShowEx( m_uFrame,
                                         m_Point.x,
                                         m_Point.y,
                                         &rect,
                                         32,
                                         32 ) ;
                    }
                    if (!m_nImageTypeID)
                    {
                        CMyBitmap::ShowBlock(m_Point.x, m_Point.y, m_Point.x + 36, m_Point.y + 36, 0xac000000);
                    }
                }
            }
            break ;
        case BOOTHITEM_IMAGE:
            {
                CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) m_strAni,
                                m_strImageID,
                                EXIGENCE_IMMEDIATE ) ;
                if ( ShowAni != NULL )
                {
                    if (!m_bImgZoom )
                        ShowAni->Show ( m_uFrame,
                                        m_Point.x,
                                        m_Point.y ) ;
                    else
                        ShowAni->ShowEx ( m_uFrame,
                                          m_Point.x,
                                          m_Point.y,
                                          m_ShowRect,
                                          m_nWidth,
                                          m_nHeight ) ;
                }
            }
            break;
        default:
            {
                CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) m_strAni,
                                m_strImageID,
                                EXIGENCE_IMMEDIATE ) ;
                if ( ShowAni != NULL )
                {
                    if ( m_btItemType == BG_IMAGE )
                        ShowAni->Show ( m_uFrame,
                                        m_Point.x,
                                        m_Point.y ) ;
                    else
                        ShowAni->ShowEx ( m_uFrame,
                                          m_Point.x,
                                          m_Point.y,
                                          m_ShowRect,
                                          m_nWidth,
                                          m_nHeight ) ;
                }
            }
            break ;
        }
    }
    this->MouseMoveText () ;
}


void CMyImage::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: Add your code to draw the specified item
}

BOOL CMyImage::GetImageLoadState()
{
    return m_bImageLoad ;
}

char* CMyImage::GetImageID()
{
    return m_strImageID ;
}

void CMyImage::OnMouseMove(UINT nFlags, CPoint point)
{
    switch ( m_btItemType )
    {
    case ITEM_IMAGE:
        if ( m_bImageLoad && !m_bMouseMove )
        {
            m_bMouseMove = true ;
            if ( m_nPosition != 0 )
            {
                DWORD lParam = MAKELONG( m_Point.x, m_Point.y ) ;
                if (m_bImgZoom)
                {
                    lParam = MAKELONG( m_Point.x + m_nWidth - 32, m_Point.y + m_nHeight - 32) ;
                }
                CMyImage::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_POINT, lParam ) ;
                CMyImage::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_IMAGE, m_nPosition ) ;
            }
        }
        else if ( !m_bImageLoad && !m_bMouseMove )
        {
            m_bMouseMove = true ;
        }
        break ;
    case NPCITEM_IMAGE:
        if ( m_bImageLoad && !m_bMouseMove )
        {
            m_bMouseMove = true ;
            if ( m_nPosition != 0 )
            {
                DWORD lParam = MAKELONG( m_Point.x, m_Point.y ) ;
                if (m_bImgZoom)
                {
                    lParam = MAKELONG( m_Point.x + m_nWidth - 32, m_Point.y + m_nHeight - 32) ;
                }
                CMyImage::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_POINT, lParam ) ;
                CMyImage::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_IMAGE, m_nPosition ) ;
            }
        }
        else if ( !m_bImageLoad && !m_bMouseMove )
        {
            m_bMouseMove = true ;
        }
        break ;
    case NORMAL_IMAGE:
        if ( m_bImageLoad && !m_bMouseMove )
        {
            m_bMouseMove = true ;
        }
        break ;
    case SKILL_IMAGE:
        if ( m_bImageLoad/* && !m_bMouseMove */)
        {
            m_bMouseMove = true ;
            DWORD lParam = MAKELONG( m_Point.x, m_Point.y ) ;
            CMyImage::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_POINT, lParam ) ;
            CMyImage::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_IMAGE, m_dwImageID ) ;
        }
        break ;
    case FACE_IMAGE:
        if ( m_bImageLoad && !m_bMouseMove )
        {
            m_bMouseMove = true ;
            if ( m_nPosition != 0 )
            {
                DWORD lParam = MAKELONG( m_Point.x, m_Point.y ) ;
                CMyImage::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_POINT, lParam ) ;
                CMyImage::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_FACE, m_nPosition ) ;
            }
        }
        break ;
    case BOOTHITEM_IMAGE:
        if ( m_bImageLoad && !m_bMouseMove )
        {
            m_bMouseMove = true ;
            DWORD lParam = MAKELONG( m_Point.x, m_Point.y ) ;
            if (m_bImgZoom)
            {
                lParam = MAKELONG( m_Point.x + m_nWidth - 32, m_Point.y + m_nHeight - 32) ;
            }
            CMyImage::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_POINT, lParam ) ;
            CMyImage::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_GOODS, m_nPosition ) ;
        }
        else if ( !m_bImageLoad && !m_bMouseMove )
        {
            m_bMouseMove = true ;
        }
        break;
    default:
        break ;
    }
    CButton::OnMouseMove( nFlags, point ) ;
}

void CMyImage::MouseMoveText()
{
    POINT MousePoint ;
    GetCursorPos ( &MousePoint ) ;
    CRect rect ;
    CMyImage::GetWindowRect ( rect ) ;
    if ( m_bMouseMove )
    {
        if ( !rect.PtInRect ( MousePoint ) )
        {
            m_bMouseMove = false ;
            if (m_btItemType)
            {
                if (m_btItemType == NPCITEM_IMAGE || m_btItemType == SKILL_IMAGE || m_btItemType == BOOTHITEM_IMAGE)
                {
                    CMyImage::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_HIDE ) ;
                }
                else
                {
                    CMyImage::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_HIDE ) ;
                }
            }
        }
    }
}

void CMyImage::ShowFocus()
{
    if ( !m_bImageLoad )
    {
        return ;
    }
    if (m_btItemType == FACE_IMAGE)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Image4",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            //			CMySize AniSize	= ShowAni->GetSize(0);
            ShowAni->Show ( 0,
                            m_Point.x,
                            m_Point.y);
            //			CRect ( 0, 0, AniSize.iWidth, AniSize.iHeight ),
            //			m_nWidth,
            //			m_nHeight ) ;
        }
    }
    else
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Image2",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->ShowEx ( 0,
                              m_Point.x,
                              m_Point.y,
                              CRect ( 0, 0, 100, 100 ),
                              m_nWidth,
                              m_nHeight ) ;
    }
}

void CMyImage::ShowBk(const char* strBkID)
{
    if ( strBkID != NULL )
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        strBkID,
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Point.x,
                            m_Point.y ) ;
    }
}

void CMyImage::OnLButtonDown(UINT nFlags, CPoint point)
{
    switch ( m_btItemType )
    {
    case ITEM_IMAGE:
        if ( g_bPickUp )
        {
            if ( m_bImageLoad )
            {
                CMyImage::GetParent()->PostMessage( WM_MY_MESSAGE, WM_RBUTTONDBLCLK, m_nPosition ) ;
            }
            else
            {
                CMyImage::GetParent()->PostMessage( WM_MY_MESSAGE, WM_LBUTTONDOWN, m_nPosition ) ;
            }
        }
        else
        {
            if ( m_bImageLoad && m_nPosition != 0)
            {
                CMyImage::GetParent()->PostMessage( WM_MY_MESSAGE, WM_LBUTTONDBLCLK, m_nPosition ) ;
                extern CMyShellDlg* gpDlgShell;
                if (!gpDlgShell || !gpDlgShell->m_bIconFix)
                {
                    m_bImageLoad = false ;
                }
            }
        }
        break ;
    case FACE_IMAGE:
        break;
    case SKILL_IMAGE:
        if ( m_bImageLoad )
        {
            LPARAM lParam ;
            CPoint MousePnt ;
            CMyImage::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_ID, m_nImageTypeID ) ;
            GetCursorPos ( &MousePnt ) ;
            lParam = MAKELONG ( MousePnt.x, MousePnt.y ) ;
            CMyImage::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_PNT, lParam ) ;
            //			CMyImage::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_PICKUP, DLG_MAGIC ) ;
        }
        break ;
    default:
        break ;
    }
    CButton::OnLButtonDown(nFlags, point);
}

void CMyImage::OnRButtonDown(UINT nFlags, CPoint point)
{
    switch ( m_btItemType )
    {
    case ITEM_IMAGE:
        if ( m_nPosition != 0 && m_bImageLoad )
        {
            CMyImage::GetParent()->PostMessage( WM_MY_MESSAGE, WM_RBUTTONDOWN, m_nPosition ) ;
        }
        else
        {
            LPARAM lParam;
            lParam = MAKELONG( point.x, point.y );
            CMyImage::PostMessage(WM_LBUTTONDBLCLK, nFlags, lParam);
        }
        break ;
    case PETFACE_IMAGE:
        if (m_bImageLoad)
        {
            LPARAM lParam;
            lParam = MAKELONG( point.x, point.y );
            CMyImage::GetParent()->PostMessage( WM_MY_MESSAGE, WM_RBUTTONDOWN, m_nPosition ) ;
        }
        break;
    default:
        break ;
    }
}

void CMyImage::OnLButtonUp(UINT nFlags, CPoint point)
{
    switch ( m_btItemType )
    {
    case SKILL_IMAGE:
        CMyImage::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP, DLG_MAGIC ) ;
        break ;
    default:
        break ;
    }
    CButton::OnLButtonUp(nFlags, point);
}

void CMyImage::SetCurFrame(UINT uFrame)
{
    m_uFrame = uFrame ;
}

void CMyImage::ShowMoney(int x, int y)
{
    // Test the mouse
    if ( m_bMouseMove )
    {
        this->MouseMoveText () ;
    }
    m_Point = CPoint ( x + m_IntPoint.x, y + m_IntPoint.y ) ;
    int nLine = 0 ;
    int nRow = 0 ;
    // Show the million number
    if ( m_btMilNum > 0 )
    {
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* ) m_strAni,
                         "Button435",
                         EXIGENCE_IMMEDIATE ) ;
        if ( pShowAni )
        {
            for ( int i = 0; i < m_btMilNum; i++ )
            {
                pShowAni->Show ( 0,
                                 m_Point.x + nRow * 16,
                                 m_Point.y + nLine * 8) ;
                nLine++ ;
                if ( nLine * 8 + 16 > m_nHeight )
                {
                    break ;
                }
            }
            nRow++ ;
        }
    }
    // Show the thousand number
    nLine = 0 ;
    if ( m_btThoNum > 0 )
    {
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* ) m_strAni,
                         "Button434",
                         EXIGENCE_IMMEDIATE ) ;
        if ( pShowAni )
        {
            for ( int i = 0; i < m_btThoNum; i++ )
            {
                pShowAni->Show ( 0,
                                 m_Point.x + nRow * 16,
                                 m_Point.y + nLine * 8) ;
                nLine++ ;
                if ( nLine * 8 + 16 > m_nHeight )
                {
                    break ;
                }
            }
            nRow++ ;
        }
    }
    // Show the hundred number
    nLine = 0 ;
    if ( m_btHunNum > 0 )
    {
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* ) m_strAni,
                         "Button433",
                         EXIGENCE_IMMEDIATE ) ;
        if ( pShowAni )
        {
            for ( int i = 0; i < m_btHunNum; i++ )
            {
                pShowAni->Show ( 0,
                                 m_Point.x + nRow * 16,
                                 m_Point.y + nLine * 8) ;
                nLine++ ;
                if ( nLine * 8 + 16 > m_nHeight )
                {
                    break ;
                }
            }
            nRow++ ;
        }
    }
}

void CMyImage::SetMoney ( int nMoney )
{
    if ( m_nCurMoney != nMoney )
    {
        m_nCurMoney = nMoney ;
        m_btMilNum = nMoney / 10000 ;
        nMoney -= 10000 * m_btMilNum ;
        m_btThoNum = nMoney / 1000 ;
        nMoney -= 1000 * m_btThoNum ;
        m_btHunNum = nMoney / 100 ;
    }
}

void CMyImage::ShowHp(int x, int y, int nCurHp, int nMaxHp)
{
    if ( nCurHp > nMaxHp )
    {
        nCurHp = nMaxHp ;
    }
    if (nCurHp <= 0)
    {
        return ;
    }
    float fUnit ;
    fUnit = 52.0f / nMaxHp ;
    int nUpLeft = (int)(52 - nCurHp * fUnit) ;
    CPoint Point = CPoint( x + m_IntPoint.x , y + m_IntPoint.y + 52) ;
#ifdef _DEBUG
    char szHp[64] = "";
    sprintf(szHp, "%d/%d", nCurHp, nMaxHp);
    CMyBitmap::ShowString (Point.x - 10, Point.y + 3, 0xffff00, szHp);
#endif
    CAni* ShowAni = g_objGameDataSet.GetDataAni( g_strControlAni,
                    "ImageSHp",
                    EXIGENCE_IMMEDIATE ) ;
    RECT rect = { nUpLeft,
                  0 ,
                  52,
                  4
                } ;
    if ( ShowAni != NULL )
    {
        ShowAni->ShowEx( 0,
                         Point.x,
                         Point.y,
                         &rect,
                         52 - nUpLeft,
                         4) ;
    }
}

void CMyImage::SetItemPos(int nPos)
{
    if ( m_btItemType == ITEM_IMAGE || m_btItemType == FACE_IMAGE || m_btItemType == NPCITEM_IMAGE || m_btItemType == BOOTHITEM_IMAGE )
    {
        m_nPosition = nPos ;
    }
}

void CMyImage::ChangeImageType(int nType)
{
    m_btItemType = nType;
    switch ( m_btItemType )
    {
    case NORMAL_IMAGE:
        strcpy ( m_strAni, g_strControlAni ) ;
        break ;
    case ITEM_IMAGE:
        break ;
    case BG_IMAGE:
        strcpy ( m_strAni, g_strControlAni ) ;
        break ;
    case SKILL_IMAGE:
        strcpy( m_strAni, g_strMagicAni ) ;
        break ;
    case NPCITEM_IMAGE:
        break ;
    default:
        break ;
    }
}

void CMyImage::ShowItemEffect()
{
    ItemEffectInfo* pInfo = g_objGameDataSet.GetItemEffectInfo(m_nImageTypeID);
    if (pInfo && _stricmp(pInfo->strItemBgEffect.c_str(), "NULL") != 0)
    {
        if (!m_objEffect.TestEffect((char*)pInfo->strItemBgEffect.c_str()))
        {
            m_objEffect.Clear();
            if (::RandGet(30) == 1)
            {
                m_objEffect.Add((char*)pInfo->strItemBgEffect.c_str());
                m_objEffect.EverPlay();
            }
        }
        CMyPos pPosWorld, pPosBg;
        g_objGameMap.Screen2World(m_Point.x,
                                  m_Point.y, pPosWorld.x, pPosWorld.y);
        g_objGameMap.World2Bg(pPosWorld.x, pPosWorld.y, pPosBg.x, pPosBg.y );
        m_objEffect.Process();
        m_objEffect.Show(pPosBg);
    }
}

void CMyImage::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    return ;
}
