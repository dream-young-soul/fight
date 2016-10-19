
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgNpcEquip.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgNpcEquip.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNpcEquip dialog


CDlgNpcEquip::CDlgNpcEquip(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgNpcEquip::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgNpcEquip)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_fFrameStep = 0.0f ;
    m_nFrameMode = 0 ;
    m_nPntX = 0 ;
    m_nMouseMovePosition = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgNpcEquip::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgNpcEquip)
    DDX_Control(pDX, IDC_QUERY_NAME, m_StaName);
    DDX_Control(pDX, IDC_QUERY_FACTION, m_StaFaction);
    DDX_Control(pDX, IDC_NPCEQUIP_CLOSE, m_BtnClose);
    DDX_Control(pDX, IDC_IMG_MOUNT, m_ImgMount);
    DDX_Control(pDX, IDC_IMG_WEAPON, m_ImgWeapon);
    DDX_Control(pDX, IDC_IMG_MANTLE, m_ImgMantle);
    DDX_Control(pDX, IDC_IMG_SHOE, m_ImgShoe);
    DDX_Control(pDX, IDC_IMG_NECK, m_ImgNeck);
    DDX_Control(pDX, IDC_IMG_HEAD, m_ImgHead);
    DDX_Control(pDX, IDC_IMG_GEAR, m_ImgGear);
    DDX_Control(pDX, IDC_IMG_BODY, m_ImgBody);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNpcEquip, CDialog)
    //{{AFX_MSG_MAP(CDlgNpcEquip)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_NPCEQUIP_CLOSE, OnNpcequipClose)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNpcEquip message handlers

LRESULT CDlgNpcEquip::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if ( wParam == false )
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
            CDlgNpcEquip::GetParent()->PostMessage( WM_MY_MESSAGE, ON_RSIDEDLG_CLOSE ) ;
        }
        else
        {
            DXPlaySound ( "Sound/Open.wav" ) ;
            m_dwLastShowTime = TimeGet() ;
            m_nFrameMode = 1 ;
            CRect rect ;
            CDlgNpcEquip::GetWindowRect( rect ) ;
            m_nPntX = _SCR_WIDTH;
            CDlgNpcEquip::GetParent()->PostMessage( WM_MY_MESSAGE, ON_RSIDEDLG_OPEN ) ;
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
            CDlgNpcEquip::OnNpcequipClose();
            return true;
        }
    }
    else if ( message == WM_MY_MESSAGE )
    {
        if ( wParam == ON_MSGDLG_IMAGE )
        {
            m_nMouseMovePosition = lParam ;
            CDlgNpcEquip::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_GOODS, DLG_NPCEQUIP ) ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgNpcEquip::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgNpcEquip::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgNpcEquip::GetWindowRect ( rect ) ;
    m_fFrameStep = ( float ) (rect.Width () + 100) / DLG_FRAME_AMOUNT ;
    // Init the button
    m_BtnClose.Init ( rect.left, rect.top, "Button540" ) ;
    // Init the image
    m_ImgMantle.Init( rect.left, rect.top, NULL, NPCITEM_IMAGE);
    m_ImgMantle.SetItemPos(ITEMPOSITION_MANTLE);
    m_ImgShoe.Init ( rect.left, rect.top, NULL, NPCITEM_IMAGE );
    m_ImgShoe.SetItemPos( ITEMPOSITION_SHOES ) ;
    m_ImgWeapon.Init ( rect.left, rect.top, NULL, NPCITEM_IMAGE );
    m_ImgWeapon.SetItemPos( ITEMPOSITION_WEAPONR ) ;
    m_ImgNeck.Init ( rect.left, rect.top, NULL, NPCITEM_IMAGE );
    m_ImgNeck.SetItemPos( ITEMPOSITION_NECKLACE ) ;
    m_ImgHead.Init ( rect.left, rect.top, NULL, NPCITEM_IMAGE );
    m_ImgHead.SetItemPos( ITEMPOSITION_HELMET ) ;
    m_ImgGear.Init ( rect.left, rect.top, NULL, NPCITEM_IMAGE );
    m_ImgGear.SetItemPos( ITEMPOSITION_TREASURE ) ;
    m_ImgBody.Init ( rect.left, rect.top, NULL, NPCITEM_IMAGE );
    m_ImgBody.SetItemPos( ITEMPOSITION_ARMOR ) ;
    m_ImgMount.Init(rect.left, rect.top, NULL, NPCITEM_IMAGE);
    m_ImgMount.SetItemPos(ITEMPOSITION_MOUNT);
    m_StaFaction.Init(rect.left, rect.top);
    m_StaFaction.SetStaticPosType(1);
    m_StaName.Init(rect.left, rect.top);
    m_StaName.SetStaticPosType(1);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNpcEquip::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_nPntX = x ;
    m_Pnt.y = y ;
}

void CDlgNpcEquip::Show()
{
    if ( m_bShow )
    {
        if ( m_nFrameMode != 0 )
        {
            if ( TimeGet() - m_dwLastShowTime <= 300 )
            {
                m_Pnt.x = (LONG)(m_nPntX - ( TimeGet() - m_dwLastShowTime ) / 15 * m_fFrameStep);
            }
            else
            {
                CRect rect ;
                CDlgNpcEquip::GetWindowRect( rect ) ;
                m_Pnt.x = _SCR_WIDTH -  rect.Width() - 100;
                m_nFrameMode = 0 ;
            }
        }
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog541",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( 0, m_Pnt.x, m_Pnt.y );
        }
        else
        {
            return ;
        }
        // show 3drole
        CMyPos RolePos;
        RolePos.x = m_Pnt.x + 152;
        RolePos.y = 350;
        g_objHero.m_objPlayerDummy.ShowDummy(RolePos, 140, -15);
        // show the static
        m_StaFaction.Show(m_Pnt.x, m_Pnt.y );
        m_StaName.Show(m_Pnt.x, m_Pnt.y );
        // Show the button
        m_BtnClose.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the image
        m_ImgShoe.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ImgWeapon.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ImgNeck.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ImgHead.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ImgGear.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ImgBody.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ImgMantle.Show(m_Pnt.x, m_Pnt.y ) ;
        m_ImgMount.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgNpcEquip::SetNpcEquip()
{
    if (strcmp(g_objHero.m_objPlayerDummy.GetMate(), g_objGameDataSet.GetStr(100122)) == 0)
    {
        m_StaFaction.SetWindowText("");
    }
    else
    {
        m_StaFaction.SetWindowText(g_objHero.m_objPlayerDummy.GetMate());
    }
    m_StaName.SetWindowText(g_objHero.m_objPlayerDummy.GetName());
    CItem* pItem = NULL;
    m_ImgShoe.RemoveImage() ;
    pItem = g_objHero.m_objPlayerDummy.GetEquipment( ITEMPOSITION_SHOES ) ;
    if ( pItem != NULL )
    {
        m_ImgShoe.InsertImage( NULL, pItem->GetID(), pItem->GetTypeID () );
    }
    m_ImgWeapon.RemoveImage() ;
    pItem = g_objHero.m_objPlayerDummy.GetEquipment( ITEMPOSITION_WEAPONR ) ;
    if ( pItem != NULL )
    {
        m_ImgWeapon.InsertImage( NULL, pItem->GetID() , pItem->GetTypeID () );
    }
    m_ImgNeck.RemoveImage() ;
    pItem = g_objHero.m_objPlayerDummy.GetEquipment( ITEMPOSITION_NECKLACE ) ;
    if ( pItem != NULL )
    {
        m_ImgNeck.InsertImage( NULL, pItem->GetID() , pItem->GetTypeID () );
    }
    m_ImgHead.RemoveImage() ;
    pItem = g_objHero.m_objPlayerDummy.GetEquipment( ITEMPOSITION_HELMET ) ;
    if ( pItem != NULL )
    {
        m_ImgHead.InsertImage( NULL, pItem->GetID() , pItem->GetTypeID () );
    }
    m_ImgGear.RemoveImage() ;
    pItem = g_objHero.m_objPlayerDummy.GetEquipment( ITEMPOSITION_TREASURE ) ;
    if ( pItem != NULL )
    {
        m_ImgGear.InsertImage( NULL, pItem->GetID(), pItem->GetTypeID () );
    }
    m_ImgBody.RemoveImage() ;
    pItem = g_objHero.m_objPlayerDummy.GetEquipment( ITEMPOSITION_ARMOR ) ;
    if ( pItem != NULL )
    {
        m_ImgBody.InsertImage( NULL, pItem->GetID() , pItem->GetTypeID () );
    }
    m_ImgMount.RemoveImage() ;
    pItem = g_objHero.m_objPlayerDummy.GetEquipment( ITEMPOSITION_MOUNT ) ;
    if ( pItem != NULL )
    {
        m_ImgMount.InsertImage( NULL, pItem->GetID(), pItem->GetTypeID ()  );
    }
    m_ImgMantle.RemoveImage();
    pItem = g_objHero.m_objPlayerDummy.GetEquipment(ITEMPOSITION_MANTLE);
    if (pItem != NULL )
    {
        m_ImgMantle.InsertImage(NULL, pItem->GetID(), pItem->GetTypeID () );
    }
}

int CDlgNpcEquip::GetMouseMovePosition()
{
    return m_nMouseMovePosition ;
}



void CDlgNpcEquip::OnNpcequipClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgNpcEquip::EnableWindow(false);
}
