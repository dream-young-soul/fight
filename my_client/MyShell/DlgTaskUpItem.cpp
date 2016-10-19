
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgTaskUpItem.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgTaskUpItem.h"

#include "Hero.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTaskUpItem dialog


CDlgTaskUpItem::CDlgTaskUpItem(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTaskUpItem::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTaskUpItem)
    m_bShow = false;
    m_Pnt = CPoint(0, 0);
    //}}AFX_DATA_INIT
}


void CDlgTaskUpItem::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTaskUpItem)
    DDX_Control(pDX, IDC_IMG_TASKITEM, m_ImgTaskItem);
    DDX_Control(pDX, IDC_BTN_UPOK, m_BtnUpOk);
    DDX_Control(pDX, IDC_BTN_UPCANCEL, m_BtnUpCancel);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTaskUpItem, CDialog)
    //{{AFX_MSG_MAP(CDlgTaskUpItem)
    ON_BN_CLICKED(IDC_IMG_TASKITEM, OnImgTaskitem)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_UPCANCEL, OnBtnUpcancel)
    ON_BN_CLICKED(IDC_BTN_UPOK, OnBtnUpok)
    ON_BN_DOUBLECLICKED(IDC_IMG_TASKITEM, OnDoubleclickedImgTaskitem)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTaskUpItem message handlers

void CDlgTaskUpItem::Show()
{
    if (m_bShow)
    {
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                         "Dialog76",
                         EXIGENCE_IMMEDIATE ) ;
        if ( pShowAni != NULL )
        {
            pShowAni->Show(0, m_Pnt.x, m_Pnt.y);
        }
        m_ImgTaskItem.Show(m_Pnt.x, m_Pnt.y);
        m_BtnUpOk.Show(m_Pnt.x, m_Pnt.y);
        m_BtnUpCancel.Show(m_Pnt.x, m_Pnt.y);
    }
}



void CDlgTaskUpItem::OnImgTaskitem()
{
    // TODO: Add your control notification handler code here
    if (g_bPickUp && m_TaskItemId)
    {
        CItem* pItem = g_objHero.GetItem(m_TaskItemId);
        if (pItem)
        {
            m_ImgTaskItem.InsertImage(NULL, pItem->GetID());
            m_TaskItemId = pItem->GetID();
            CDlgTaskUpItem::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
            CDlgTaskUpItem::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
        }
    }
}


LRESULT CDlgTaskUpItem::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if (m_bShow)
        {
            DXPlaySound("Sound/Open.wav");
            m_TaskItemId = 0;
            m_ImgTaskItem.RemoveImage();
        }
        else
        {
            DXPlaySound("Sound/window_close.wav");
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgTaskUpItem::OnBtnUpcancel();
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgTaskUpItem::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgTaskUpItem::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgTaskUpItem::GetWindowRect(rect);
    m_ImgTaskItem.Init(rect.left, rect.top, NULL, ITEM_IMAGE);
    m_BtnUpOk.Init(rect.left, rect.top, "Button76");
    m_BtnUpCancel.Init(rect.left, rect.top, "Button77");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTaskUpItem::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgTaskUpItem::SetUptaskId(UCHAR idTask)
{
    m_ucIdTask = idTask;
}

void CDlgTaskUpItem::OnBtnUpcancel()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Clear.wav");
    CDlgTaskUpItem::EnableWindow(false);
}

void CDlgTaskUpItem::OnBtnUpok()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_ucIdTask && m_TaskItemId)
    {
        g_objHero.CompletePlayerTaskForItem(m_ucIdTask, m_TaskItemId);
    }
    else
    {
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10621));
    }
    CDlgTaskUpItem::EnableWindow(false);
}

void CDlgTaskUpItem::SetTaskUpItem(OBJID idTaskItem)
{
    m_TaskItemId = idTaskItem;
}

void CDlgTaskUpItem::OnDoubleclickedImgTaskitem()
{
    // TODO: Add your control notification handler code here
    if (m_ImgTaskItem.GetImageLoadState())
    {
        m_TaskItemId = 0;
        m_ImgTaskItem.RemoveImage();
        CDlgTaskUpItem::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
    }
}
