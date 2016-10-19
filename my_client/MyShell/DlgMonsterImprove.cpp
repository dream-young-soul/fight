
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgMonsterImprove.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgMonsterImprove.h"
#include "Hero.h"
#include "Gamemsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterImprove dialog


CDlgMonsterImprove::CDlgMonsterImprove(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMonsterImprove::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMonsterImprove)
    m_Pnt = CPoint(0, 0);
    m_IdMonster = 0;
    m_IdSoulStone = 0;
    m_bShow = false;
    m_nIconRealID = 0;
    m_nDlgMode = 0;// NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgMonsterImprove::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMonsterImprove)
    DDX_Control(pDX, IDC_STA_NPCINFO, m_StaNpcInfo);
    DDX_Control(pDX, IDC_IMG_SOULSTONE, m_ImgSoulStone);
    DDX_Control(pDX, IDC_IMG_NPCFACE, m_ImgNpcFace);
    DDX_Control(pDX, IDC_IMG_MONSTER, m_ImgMonster);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    DDX_Control(pDX, IDC_BTN_ALIVEIMP, m_BtnAliveImp);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMonsterImprove, CDialog)
    //{{AFX_MSG_MAP(CDlgMonsterImprove)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_ALIVEIMP, OnBtnAliveimp)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_IMG_MONSTER, OnImgMonster)
    ON_BN_CLICKED(IDC_IMG_SOULSTONE, OnImgSoulstone)
    ON_BN_DOUBLECLICKED(IDC_IMG_MONSTER, OnDoubleclickedImgMonster)
    ON_BN_DOUBLECLICKED(IDC_IMG_SOULSTONE, OnDoubleclickedImgSoulstone)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterImprove message handlers

LRESULT CDlgMonsterImprove::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        {
            m_bShow = wParam ;
            if (m_bShow)
            {
                DXPlaySound("Sound/Open.wav");
            }
            else
            {
                m_IdMonster = 0;
                m_IdSoulStone = 0;
                m_nIconRealID = 0;
                m_ImgMonster.RemoveImage();
                m_ImgSoulStone.RemoveImage();
                DXPlaySound("Sound/window_close.wav");
            }
        }
        break ;
    case WM_COMMAND:
        {
            if ( wParam == 1 )
            {
                return true ;
            }
            else if ( wParam == 2 )
            {
                CDlgMonsterImprove::OnBtnClose();
                return true ;
            }
        }
        break ;
    case WM_MY_MESSAGE:
        switch(wParam)
        {
        case ON_MSGDLG_HIDE:
            CDlgMonsterImprove::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_HIDE);
            break;
        case ON_MSGDLG_POINT:
            CDlgMonsterImprove::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_POINT, lParam ) ;
            break;
        case ON_MSGDLG_IMAGE:
            CDlgMonsterImprove::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_GOODS, DLG_MONSTERIMPROVE ) ;
            break ;
        default:
            break;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgMonsterImprove::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

BOOL CDlgMonsterImprove::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgMonsterImprove::GetWindowRect(rect);
    m_ImgNpcFace.Init(rect.left, rect.top, NULL, FACE_IMAGE);
    m_StaNpcInfo.Init(rect.left, rect.top, 2);
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    m_BtnAliveImp.Init(rect.left, rect.top, "Button652"); //alive ,Button653-improve
    m_ImgMonster.Init(rect.left, rect.top, NULL, ITEM_IMAGE);
    m_ImgSoulStone.Init(rect.left, rect.top, NULL, ITEM_IMAGE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMonsterImprove::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgMonsterImprove::Show()
{
    if (m_bShow)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog82",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0,
                           m_Pnt.x ,
                           m_Pnt.y  );
            ShowAni->Show( 1,
                           m_Pnt.x + 256 ,
                           m_Pnt.y  );
            ShowAni->Show( 2,
                           m_Pnt.x ,
                           m_Pnt.y + 256 );
            ShowAni->Show( 3,
                           m_Pnt.x + 128,
                           m_Pnt.y + 256 );
            ShowAni->Show( 4,
                           m_Pnt.x + 256,
                           m_Pnt.y + 256 );
            ShowAni->Show( 5,
                           m_Pnt.x + 384,
                           m_Pnt.y + 256 );
        }
        else
        {
            return ;
        }
        m_ImgNpcFace.Show(m_Pnt.x, m_Pnt.y);
        m_StaNpcInfo.Show(m_Pnt.x, m_Pnt.y);
        m_ImgMonster.Show(m_Pnt.x, m_Pnt.y);
        m_ImgSoulStone.Show(m_Pnt.x, m_Pnt.y);
        m_BtnAliveImp.Show(m_Pnt.x, m_Pnt.y);
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgMonsterImprove::SetDlgMode(int nMode)
{
    m_nDlgMode = nMode;
    if (m_nDlgMode == 0)	//alive
    {
        m_StaNpcInfo.SetWindowText((char*)g_objGameDataSet.GetStr(10697)); //幻兽复活
        m_BtnAliveImp.ChangeImage("Button652");
    }
    else if (m_nDlgMode == 1) //improve
    {
        m_StaNpcInfo.SetWindowText((char*)g_objGameDataSet.GetStr(10696)); //幻兽强化
        m_BtnAliveImp.ChangeImage("Button653");
    }
}

void CDlgMonsterImprove::OnBtnAliveimp()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_ImgSoulStone.GetImageLoadState() && m_ImgMonster.GetImageLoadState())
    {
        if (m_nDlgMode == 0)
        {
            g_objHero.ReboneEudemon(m_IdMonster, m_IdSoulStone);    // 幻兽复活，幻兽ID 宝石ID
        }
        else
        {
            g_objHero.EnhanceEudemon(m_IdMonster, m_IdSoulStone);    // 幻兽强化，幻兽ID, 宝石ID
        }
        CDlgMonsterImprove::EnableWindow(false);
    }
    else if (!m_ImgSoulStone.GetImageLoadState())
    {
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10622));
    }
    else if (!m_ImgMonster.GetImageLoadState())
    {
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10623));
    }
}

void CDlgMonsterImprove::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    CDlgMonsterImprove::EnableWindow(false);
}

void CDlgMonsterImprove::InsertMonster(OBJID idMonster)
{
    CItem* pMonster = g_objHero.GetItem(idMonster);
    if (pMonster && pMonster->GetPackageType() == PACKAGE_PET && !m_ImgMonster.GetImageLoadState())
    {
        if (m_nDlgMode == 0 && pMonster->GetWarGhostExp() > 0)
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10725)); //幻兽状态正常，不需要复活!
            m_IdMonster = 0;
        }
        else if (m_nDlgMode == 1 && pMonster->GetWarGhostExp() <= 0)
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10726)); //你的幻兽已经死亡，不允许进行强化!
            m_IdMonster = 0;
        }
        else
        {
            m_ImgMonster.RemoveImage();
            m_IdMonster = idMonster;
            m_ImgMonster.InsertImage(NULL, idMonster, NULL, true);
        }
        CDlgMonsterImprove::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
        CDlgMonsterImprove::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
    }
}

void CDlgMonsterImprove::OnImgMonster()
{
    // TODO: Add your control notification handler code here
    if (!m_ImgMonster.GetImageLoadState() && g_bPickUp && m_IdMonster)
    {
        CDlgMonsterImprove::InsertMonster(m_IdMonster);
    }
    else if (g_bPickUp && !m_ImgMonster.GetImageLoadState ())
    {
        if (m_nDlgMode == 0 )
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10800));    //	操作错误！请放置需要复活的幻兽
        }
        else if (m_nDlgMode == 1)
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10801));    //	操作错误！请放置需要强化的幻兽
        }
    }
}

void CDlgMonsterImprove::OnImgSoulstone()
{
    // TODO: Add your control notification handler code here
    if (g_bPickUp && !m_ImgSoulStone.GetImageLoadState() && m_nIconRealID)
    {
        CItem* pItem = g_objHero.GetItem ( m_nIconRealID ) ;
        if (pItem != NULL)
        {
            if ( (pItem->IsAliveSoulStone() && m_nDlgMode == 0)
                    || (pItem->IsImproveSoulStone() && m_nDlgMode == 1))
            {
                m_IdSoulStone = pItem->GetID () ;
                m_ImgSoulStone.InsertImage (NULL , m_IdSoulStone, 9, true ) ;
                CDlgMonsterImprove::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
                CDlgMonsterImprove::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
            }
            else
            {
                m_IdSoulStone = 0;
            }
        }
    }
}

void CDlgMonsterImprove::SetPickUpSoulStone(OBJID idSoulStone)
{
    m_nIconRealID = idSoulStone;
}

OBJID CDlgMonsterImprove::GetSoulStoneID()
{
    return	m_IdSoulStone;
}

void CDlgMonsterImprove::SetPickUpMonsterId(OBJID idMonster)
{
    m_IdMonster = idMonster;
}

void CDlgMonsterImprove::OnDoubleclickedImgMonster()
{
    if (m_ImgMonster.GetImageLoadState() && m_IdMonster)
    {
        m_IdMonster = 0;
        m_ImgMonster.RemoveImage();
    }
}

void CDlgMonsterImprove::OnDoubleclickedImgSoulstone()
{
    if (m_ImgSoulStone.GetImageLoadState() && m_IdSoulStone)
    {
        m_ImgSoulStone.RemoveImage();
        m_IdSoulStone = 0;
        m_nIconRealID = 0;
    }
}

