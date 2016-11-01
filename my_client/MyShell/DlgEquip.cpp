
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgEquip.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgEquip.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEquip dialog


CDlgEquip::CDlgEquip(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgEquip::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgEquip)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_fFrameStep = 0.0f ;
    m_nFrameMode = 0 ;
    m_nPntX = _SCR_WIDTH ;
    m_nPickUpItemID = 0 ;
    m_PickUpMousePnt = CPoint ( 0, 0 );
    m_nPickUpItemPosition = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgEquip::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgEquip)
    DDX_Control(pDX, IDC_IMG_MOUNT, m_ImgMount);
    DDX_Control(pDX, IDC_IMG_WEAPON, m_ImgWeapon);
    DDX_Control(pDX, IDC_IMG_MANTLE, m_ImgMantle);
    DDX_Control(pDX, IDC_ROLE_XP, m_StaRoleXp);
    DDX_Control(pDX, IDC_ROLE_MP, m_StaRoleMp);
    DDX_Control(pDX, IDC_ROLE_MAGICDEF, m_StaRoleMagicDef);
    DDX_Control(pDX, IDC_ROLE_MAGIC, m_StaRoleMagic);
    DDX_Control(pDX, IDC_ROLE_HP, m_StaRoleHp);
    DDX_Control(pDX, IDC_ROLE_FIGHTDEF, m_StaRoleFightDef);
    DDX_Control(pDX, IDC_ROLE_FIGHT, m_StaRoleFight);
    DDX_Control(pDX, IDC_ROLE_AVOID, m_StaRoleAvoid);
    DDX_Control(pDX, IDC_IMG_SHOE, m_ImgShoe);
    DDX_Control(pDX, IDC_IMG_NECK, m_ImgNeck);
    DDX_Control(pDX, IDC_IMG_HEAD, m_ImgHead);
    DDX_Control(pDX, IDC_IMG_GEAR, m_ImgGear);
    DDX_Control(pDX, IDC_IMG_BODY, m_ImgBody);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEquip, CDialog)
    //{{AFX_MSG_MAP(CDlgEquip)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEquip message handlers

LRESULT CDlgEquip::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if ( wParam == false )
        {
            //			DXPlaySound ( "Sound/Close.wav" ) ;
        }
        else
        {
            m_dwLastShowTime = TimeGet() ;
            m_nFrameMode = 1 ;
            CRect rect ;
            CDlgEquip::GetWindowRect( rect ) ;
            m_nPntX = _SCR_WIDTH ;
            CDlgEquip::FlashEquip() ;
            CDlgEquip::SetRoleInfo();
        }
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 || wParam == 2 )
        {
            return true ;
        }
    }
    else if ( message == WM_MY_MESSAGE )
    {
        switch ( wParam )
        {
        case WM_LBUTTONDOWN:
            {
                if ( m_nPickUpItemID != -1 )
                {
                    if ( g_objHero.TryItem( m_nPickUpItemID, lParam ) )
                    {
                        CDlgEquip::HeroEquipItem( lParam ) ;
                    }
                    else
                    {
                        return true;
                    }
                }
                else
                {
                    CDlgEquip::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL ) ;
                    CDlgEquip::FlashEquip() ;
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
                        CDlgEquip::HeroEquipItem( lParam ) ;
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
                /*				CItem* pItem = g_objHero.GetEquipment(lParam);
                				if (pItem!=NULL)
                					g_objHero.RepairItem (pItem->GetID()) ;*/
                g_objHero.UnequipItem( lParam ) ;
                CDlgEquip::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL ) ;
            }
            break ;
        case WM_LBUTTONDBLCLK:
            {
                m_nPickUpItemPosition = lParam ;
                ::GetCursorPos( &m_PickUpMousePnt ) ;
                CDlgEquip::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_ICON_PICKUP, DLG_EQUIP ) ;
                m_nPickUpItemID = -1 ;
            }
            break ;
        case ON_MSGDLG_POINT:
            CDlgEquip::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_POINT, lParam ) ;
            break;
        case ON_MSGDLG_IMAGE:
            {
                m_nMouseMovePosition = lParam ;
                CDlgEquip::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_GOODS, DLG_EQUIP ) ;
            }
            break ;
        case ON_MSGDLG_HIDE:
            CDlgEquip::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_HIDE);
            break;
        default:
            break ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgEquip::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgEquip::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgEquip::GetWindowRect ( rect ) ;
    m_fFrameStep = ( float )(rect.Width() + 100) / DLG_FRAME_AMOUNT ;
    //showmode base equip
    m_StaRoleHp.Init(rect.left, rect.top);
    m_StaRoleMp.Init(rect.left, rect.top);
    m_StaRoleXp.Init(rect.left, rect.top);
    m_StaRoleFight.Init(rect.left, rect.top);
    m_StaRoleFightDef.Init(rect.left, rect.top);
    m_StaRoleMagic.Init(rect.left, rect.top);
    m_StaRoleMagicDef.Init(rect.left, rect.top);
    m_StaRoleAvoid.Init(rect.left, rect.top);
    // Init the image
    m_ImgMantle.Init( rect.left, rect.top, NULL, ITEM_IMAGE);
    m_ImgMantle.SetItemPos(ITEMPOSITION_FASHION);
    m_ImgShoe.Init ( rect.left, rect.top, NULL, ITEM_IMAGE );
    m_ImgShoe.SetItemPos( ITEMPOSITION_SHOES ) ;
    m_ImgWeapon.Init ( rect.left, rect.top, NULL, ITEM_IMAGE );
    m_ImgWeapon.SetItemPos( ITEMPOSITION_WEAPONR ) ;
    m_ImgNeck.Init ( rect.left, rect.top, NULL, ITEM_IMAGE );
    m_ImgNeck.SetItemPos( ITEMPOSITION_NECKLACE ) ;
    m_ImgHead.Init ( rect.left, rect.top, NULL, ITEM_IMAGE );
    m_ImgHead.SetItemPos( ITEMPOSITION_HELMET ) ;
    m_ImgGear.Init ( rect.left, rect.top, NULL, ITEM_IMAGE );
    m_ImgGear.SetItemPos( ITEMPOSITION_TREASURE ) ;
    m_ImgBody.Init ( rect.left, rect.top, NULL, ITEM_IMAGE );
    m_ImgBody.SetItemPos( ITEMPOSITION_ARMOR ) ;
    m_ImgMount.Init(rect.left, rect.top, NULL, ITEM_IMAGE);
    m_ImgMount.SetItemPos(ITEMPOSITION_MOUNT);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEquip::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_nPntX = x ;
    m_Pnt.y = y ;
}

void CDlgEquip::Show(int x, int y)
{
    if ( m_bShow )
    {
        // show 3drole
        CMyPos RolePos;
        RolePos.x = x + 160;
        RolePos.y = 290;
        g_objHero.m_objDummy.ShowDummy(RolePos, 130, -15);
        // Show the curmode
        m_ImgShoe.Show ( m_Pnt.x + x, m_Pnt.y + y) ;
        m_ImgWeapon.Show ( m_Pnt.x + x, m_Pnt.y + y) ;
        m_ImgNeck.Show ( m_Pnt.x + x, m_Pnt.y + y) ;
        m_ImgHead.Show ( m_Pnt.x + x, m_Pnt.y + y) ;
        m_ImgGear.Show ( m_Pnt.x + x, m_Pnt.y + y) ;
        m_ImgBody.Show ( m_Pnt.x + x, m_Pnt.y + y) ;
        m_ImgMantle.Show(m_Pnt.x + x, m_Pnt.y + y) ;
        m_ImgMount.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_StaRoleHp.Show ( m_Pnt.x + x, m_Pnt.y + y) ;
        m_StaRoleMp.Show ( m_Pnt.x + x, m_Pnt.y + y) ;
        m_StaRoleXp.Show ( m_Pnt.x + x, m_Pnt.y + y) ;
        m_StaRoleAvoid.Show ( m_Pnt.x + x, m_Pnt.y + y) ;
        m_StaRoleFight.Show ( m_Pnt.x + x, m_Pnt.y + y) ;
        m_StaRoleFightDef.Show ( m_Pnt.x + x, m_Pnt.y + y ) ;
        m_StaRoleMagic.Show ( m_Pnt.x + x, m_Pnt.y + y) ;
        m_StaRoleMagicDef.Show ( m_Pnt.x + x, m_Pnt.y + y ) ;
    }
}

void CDlgEquip::HeroEquipItem(int nPos)
{
    switch(nPos)
    {
    case ITEMPOSITION_HELMET:
        DXPlaySound("Sound/Role_Armet.wav");
        break;
    case ITEMPOSITION_NECKLACE:
        DXPlaySound("Sound/Role_Necklace.wav");
        break;
    case ITEMPOSITION_SHOES:
        DXPlaySound("Sound/Role_Shoes.wav");
        break;
    case ITEMPOSITION_WEAPONR:
        DXPlaySound("Sound/Role_Weapon.wav");
        break;
    case ITEMPOSITION_ARMOR:
        DXPlaySound("Sound/Role_Armor.wav");
        break;
    case ITEMPOSITION_TREASURE:
        DXPlaySound("Sound/Role_Bangle.wav");
        break;
    case ITEMPOSITION_MOUNT:
        DXPlaySound("Sound/Role_Armet.wav");
        break;
    case ITEMPOSITION_MANTLE:
        DXPlaySound("Sound/Role_Bangle.wav");
        break;
    default:
        DXPlaySound("Sound/Role_Bangle.wav");
        break;
    }
    g_objHero.EquipItem( nPos, m_nPickUpItemID ) ;
    g_bPickUp = false ;
    m_nPickUpItemID = 0 ;
    CDlgEquip::GetParent ()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
}

void CDlgEquip::FlashEquip()
{
    CItem* pEquip = NULL ;
    m_ImgShoe.RemoveImage() ;
    pEquip = g_objHero.GetEquipment( ITEMPOSITION_SHOES ) ;
    if ( pEquip != NULL )
    {
        m_ImgShoe.InsertImage( NULL, pEquip->GetID(), pEquip->GetTypeID () );
    }
    m_ImgWeapon.RemoveImage() ;
    pEquip = g_objHero.GetEquipment( ITEMPOSITION_WEAPONR ) ;
    if ( pEquip != NULL )
    {
        m_ImgWeapon.InsertImage( NULL, pEquip->GetID(), pEquip->GetTypeID () );
    }
    m_ImgNeck.RemoveImage() ;
    pEquip = g_objHero.GetEquipment( ITEMPOSITION_NECKLACE ) ;
    if ( pEquip != NULL )
    {
        m_ImgNeck.InsertImage( NULL, pEquip->GetID(), pEquip->GetTypeID () );
    }
    m_ImgHead.RemoveImage() ;
    pEquip = g_objHero.GetEquipment( ITEMPOSITION_HELMET ) ;
    if ( pEquip != NULL )
    {
        m_ImgHead.InsertImage( NULL, pEquip->GetID(), pEquip->GetTypeID () );
    }
    m_ImgGear.RemoveImage() ;
    pEquip = g_objHero.GetEquipment( ITEMPOSITION_TREASURE ) ;
    if ( pEquip != NULL )
    {
        m_ImgGear.InsertImage( NULL, pEquip->GetID(), pEquip->GetTypeID () );
    }
    m_ImgBody.RemoveImage() ;
    pEquip = g_objHero.GetEquipment( ITEMPOSITION_ARMOR ) ;
    if ( pEquip != NULL )
    {
        m_ImgBody.InsertImage( NULL, pEquip->GetID(), pEquip->GetTypeID () );
    }
    m_ImgMount.RemoveImage();
    pEquip = g_objHero.GetEquipment( ITEMPOSITION_MOUNT );
    if (pEquip != NULL )
    {
        m_ImgMount.InsertImage(NULL, pEquip->GetID(), pEquip->GetTypeID ());
    }
    m_ImgMantle.RemoveImage();
    pEquip = g_objHero.GetEquipment(ITEMPOSITION_FASHION);
    if (pEquip != NULL )
    {
        m_ImgMantle.InsertImage(NULL, pEquip->GetID(), pEquip->GetTypeID ());
    }
}


void CDlgEquip::SetPickUpItemID(int nID)
{
    m_nPickUpItemID = nID ;
}

CPoint CDlgEquip::GetPickUpMousePnt()
{
    return m_PickUpMousePnt ;
}

int CDlgEquip::GetPickUpPosition()
{
    return m_nPickUpItemPosition ;
}

int CDlgEquip::GetMouseMovePosition()
{
    return m_nMouseMovePosition ;
}

void CDlgEquip::SetRoleInfo()
{
    char strtext[64];
    itoa(g_objHero.GetMaxLife(), strtext, 10);
    m_StaRoleHp.SetWindowText(strtext);
    itoa(g_objHero.GetMaxMana(), strtext, 10);
    m_StaRoleMp.SetWindowText(strtext);
    itoa(g_objHero.GetMaxPhysicalForce(), strtext, 10);
    m_StaRoleXp.SetWindowText(strtext);
    itoa(g_objHero.GetDex(), strtext, 10);
    m_StaRoleAvoid.SetWindowText(strtext);
    itoa(g_objHero.GetMaxAtk(), strtext, 10);
    m_StaRoleFight.SetWindowText(strtext);
    itoa(g_objHero.GetDef(), strtext, 10);
    m_StaRoleFightDef.SetWindowText(strtext);
    itoa(g_objHero.GetMagicDef(), strtext, 10);
    m_StaRoleMagicDef.SetWindowText(strtext);
    itoa(g_objHero.GetMaxMagicAtk(), strtext, 10);
    m_StaRoleMagic.SetWindowText(strtext);
}

void CDlgEquip::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if ( m_nPickUpItemID != -1 )
    {
        bool bEquip = false;
        CItem* pItem = g_objHero.GetItem(m_nPickUpItemID);
        if (pItem && g_objHero.TryItem(pItem)
                && (pItem->GetSort() == ITEMSORT_HELMET
                    || pItem->GetSort() == ITEMSORT_TREASURE
                    || pItem->GetSort() == ITEMSORT_WEAPON_SINGLE_HAND || pItem->GetSort() == ITEMSORT_WEAPON_DOUBLE_HAND
                    || pItem->GetSort() == ITEMSORT_RING
                    || pItem->GetSort() == ITEMSORT_ARMOR
                    || pItem->GetSort() == ITEMSORT_NECKLACE
                    || pItem->GetSort() == ITEMSORT_MOUNT))
        {
            g_objHero.UseItem(pItem->GetID());
            CDlgEquip::GetParent()->GetParent()->PostMessage( WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL ) ;
            CDlgEquip::FlashEquip() ;
        }
    }
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgEquip::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgEquip::GetParent()->PostMessage(WM_MOUSEMOVE, nFlags, lParam);
    CDialog::OnMouseMove(nFlags, point);
}
