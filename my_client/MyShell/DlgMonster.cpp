
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgMonster.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgMonster.h"

#include "Hero.h"
#include "GameMsg.h"
#include "3dgameMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMonster dialog


CDlgMonster::CDlgMonster(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMonster::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMonster)
    m_Pnt = CPoint(0, 0);
    m_bShow = false;
    m_dwLastShowTime = 0;
    m_fFrameStep = 0.0f;
    m_nFrameMode = 0;
    m_nPntX = 0;
    m_iCurMonster = 0 ;
    m_CurMonsterID = 0;
    //}}AFX_DATA_INIT
}


void CDlgMonster::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMonster)
    DDX_Control(pDX, IDC_BTN_RENAMEOK, m_BtnRenameOk);
    DDX_Control(pDX, IDC_BTN_RENAMECANCEL, m_BtnRenameCancel);
    DDX_Control(pDX, IDC_EDIT_NAME, m_EditName);
    DDX_Control(pDX, IDC_BTN_CALLOUT, m_ChkCallOut);
    DDX_Control(pDX, IDC_EDIT_NAME4, m_EditMonster4);
    DDX_Control(pDX, IDC_EDIT_NAME3, m_EditMonster3);
    DDX_Control(pDX, IDC_EDIT_NAME2, m_EditMonster2);
    DDX_Control(pDX, IDC_EDIT_NAME1, m_EditMonster1);
    DDX_Control(pDX, IDC_BTN_RIDEON, m_BtnRename);
    DDX_Control(pDX, IDC_BTN_MAGIC, m_MagicBtn);
    DDX_Control(pDX, IDC_MONSTER_POTENTIAL, m_StaPotential);
    DDX_Control(pDX, IDC_MONSTER_GROW, m_StaGrow);
    DDX_Control(pDX, IDC_BTN_CALL, m_CallChk);
    DDX_Control(pDX, IDC_MONSTER_SMART, m_StaSmart);
    DDX_Control(pDX, IDC_MONSTER_MASTER, m_StaMaster);
    DDX_Control(pDX, IDC_MONSTER_MAGICDEF, m_StaMagicDef);
    DDX_Control(pDX, IDC_MONSTER_MAGIC, m_StaMagic);
    DDX_Control(pDX, IDC_MONSTER_LEVEL, m_StaLevel);
    DDX_Control(pDX, IDC_MONSTER_HP, m_StaHp);
    DDX_Control(pDX, IDC_MONSTER_FIGHTDEF, m_StaFightDef);
    DDX_Control(pDX, IDC_MONSTER_FIGHT, m_StaFight);
    DDX_Control(pDX, IDC_MONSTER_EXP, m_StaExp);
    DDX_Control(pDX, IDC_IMG_MONSTER, m_ImgMonster);
    DDX_Control(pDX, IDC_GRI_EGGS, m_EggsGri);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_CloseBtn);
    DDX_Control(pDX, IDC_BTN_DROP, m_DropBtn);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMonster, CDialog)
    //{{AFX_MSG_MAP(CDlgMonster)
    ON_BN_CLICKED(IDC_BTN_CALL, OnBtnCall)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_BTN_DROP, OnBtnDrop)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_BN_CLICKED(IDC_BTN_MAGIC, OnBtnMagic)
    ON_BN_CLICKED(IDC_BTN_RIDEON, OnBtnRename)
    ON_BN_CLICKED(IDC_BTN_CALLOUT, OnBtnCallout)
    ON_BN_CLICKED(IDC_BTN_RENAMECANCEL, OnBtnRenamecancel)
    ON_BN_CLICKED(IDC_BTN_RENAMEOK, OnBtnRenameok)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMonster message handlers


void CDlgMonster::OnBtnCall()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (g_objHero.GetItemAmount(PACKAGE_PET) <= 0)
    {
        m_CallChk.SetCheckValue(0);
        return;
    }
    CItem* pMonster = g_objHero.GetItemByIndex(m_iCurMonster, PACKAGE_PET);
    if (pMonster)
    {
        CDlgMonster::ShowHead (pMonster->GetID ());
    }
}

void  CDlgMonster::ShowHead (OBJID idMonster)
{
    int nAmount = m_IdCurCallMonster.size ();
    BOOL bShow = TRUE;
    for(int i = nAmount - 1; i >= 0; i--)
    {
        if (m_IdCurCallMonster[i] == idMonster)
        {
            m_IdCurCallMonster.erase (m_IdCurCallMonster.begin () + i);
            bShow = FALSE;
        }
    }
    if (bShow)
    {
        m_IdCurCallMonster.push_back (idMonster);
    }
    CDlgMonster::GetParent()->PostMessage (WM_MY_MESSAGE, ON_MONSTERHEAD_SET, idMonster);
}

LRESULT CDlgMonster::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        {
            m_bShow = wParam ;
            if ( wParam == false )
            {
                DXPlaySound ( "Sound/window_close.wav" ) ;
                if (m_EditName.IsWindowEnabled())
                {
                    m_EditName.EnableWindow(false);
                    m_BtnRenameOk.EnableWindow(false);
                    m_BtnRenameCancel.EnableWindow(false);
                }
                if (m_DlgMonsterSkill.m_bShow)
                {
                    m_DlgMonsterSkill.EnableWindow (false);
                }
            }
            else
            {
                DXPlaySound ( "Sound/Open.wav" ) ;
                if (m_EditName.IsWindowEnabled())
                {
                    m_EditName.EnableWindow(false);
                    m_BtnRenameOk.EnableWindow(false);
                    m_BtnRenameCancel.EnableWindow(false);
                }
                CDlgMonster::SetEggGrid();
                CDlgMonster::SetMonsterList();
                m_dwLastShowTime = TimeGet() ;
                m_nFrameMode = 1 ;
                m_nPntX = _SCR_WIDTH ;
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
                CDlgMonster::OnBtnClose () ;
                return true ;
            }
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgMonster::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgMonster::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgMonster::GetWindowRect(rect);
    m_fFrameStep = ( float ) (rect.Width() + 100) / DLG_FRAME_AMOUNT ;
    //init button
    m_CloseBtn.Init(rect.left, rect.top, "Button540");
    m_CallChk.Init(rect.left, rect.top, "Button907", 2);
    m_ChkCallOut.Init(rect.left, rect.top, "Button901");
    m_MagicBtn.Init(rect.left, rect.top, "Button902");
    m_DropBtn.Init(rect.left, rect.top, "Button903");
    m_BtnRename.Init(rect.left, rect.top, "Button904");
    //	m_BtnEvolve.Init(rect.left, rect.top,"Button905");
    //init skillgri and eggsskill
    m_ImgMonster.Init(rect.left, rect.top, NULL, PETFACE_IMAGE);
    m_EggsGri.Init(rect.left, rect.top, 1, 6, DLG_MONSTEREGG);
    m_EggsGri.ReSetGridSpace(4, 1);
    //init static
    m_EditMonster1.Init(rect.left, rect.top);
    m_EditMonster1.SetLimitText (_MAX_NAMESIZE - 1);
    m_EditMonster2.Init(rect.left, rect.top);
    m_EditMonster2.SetLimitText (_MAX_NAMESIZE - 1);
    m_EditMonster3.Init(rect.left, rect.top);
    m_EditMonster3.SetLimitText (_MAX_NAMESIZE - 1);
    m_EditMonster4.Init(rect.left, rect.top);
    m_EditMonster4.SetLimitText (_MAX_NAMESIZE - 1);
    m_StaHp.Init(rect.left, rect.top);
    m_StaPotential.Init(rect.left, rect.top);
    m_StaGrow.Init(rect.left, rect.top);
    m_StaExp.Init(rect.left, rect.top);
    m_StaMaster.Init(rect.left, rect.top);
    m_StaSmart.Init(rect.left, rect.top);
    m_StaLevel.Init(rect.left, rect.top);
    m_StaMagic.Init(rect.left, rect.top);
    m_StaMagicDef.Init(rect.left, rect.top);
    m_StaFight.Init(rect.left, rect.top);
    m_StaFightDef.Init(rect.left, rect.top);
    //	m_BtnAtkStatus1.Init (rect.left,rect.top,"Button574");
    m_EditName.Init(rect.left, rect.top);
    m_EditName.SetLimitText (_MAX_NAMESIZE - 1);
    m_BtnRenameOk.Init(rect.left, rect.top, "Button150");
    m_BtnRenameCancel.Init(rect.left, rect.top, "Button151");
    m_DlgMonsterSkill.Create(IDD_DIALOG_MONSTERSKILL, this->GetParent());
    CRect Mrect;
    m_DlgMonsterSkill.GetWindowRect(Mrect);
    m_DlgMonsterSkill.MoveWindow(_SCR_WIDTH - rect.Width() - Mrect.Width () - 100,
                                 450 - rect.Height () ,
                                 Mrect.Width (), Mrect.Height (), false);
    m_DlgMonsterSkill.ShowWindow (SW_NORMAL);
    m_DlgMonsterSkill.EnableWindow (false);
    CRect prgRect;
    m_EditMonster1.m_EditRect.GetWindowRect(prgRect);
    m_PrgMonstName[0] = CRect ( prgRect.left - rect.left - 2,
                                prgRect.top - rect.top - 6,
                                prgRect.right - rect.left + 3,
                                prgRect.bottom - rect.top - 4) ;
    m_EditMonster2.m_EditRect.GetWindowRect(prgRect);
    m_PrgMonstName[1] = CRect ( prgRect.left - rect.left - 2,
                                prgRect.top - rect.top - 6,
                                prgRect.right - rect.left + 3,
                                prgRect.bottom - rect.top - 4) ;
    m_EditMonster3.m_EditRect.GetWindowRect(prgRect);
    m_PrgMonstName[2] = CRect ( prgRect.left - rect.left - 2,
                                prgRect.top - rect.top - 6,
                                prgRect.right - rect.left + 3,
                                prgRect.bottom - rect.top - 4) ;
    m_EditMonster4.m_EditRect.GetWindowRect(prgRect);
    m_PrgMonstName[3] = CRect ( prgRect.left - rect.left - 2,
                                prgRect.top - rect.top - 6,
                                prgRect.right - rect.left + 3,
                                prgRect.bottom - rect.top - 4) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMonster::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x;
    m_Pnt.y = y;
    // TODO: Add your message handler code here
}

void CDlgMonster::Show()
{
    if (m_bShow)
    {
        if ( m_nFrameMode != 0 )
        {
            if ( TimeGet() - m_dwLastShowTime <= 300 )
            {
                m_Pnt.x =(LONG)( m_nPntX - ( TimeGet() - m_dwLastShowTime ) / 15 * m_fFrameStep);
            }
            else
            {
                CRect rect;
                CDlgMonster::GetWindowRect(rect);
                m_Pnt.x = _SCR_WIDTH - rect.Width() - 100;
                m_nFrameMode = 0 ;
            }
        }
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog9",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0, m_Pnt.x, m_Pnt.y ) ;
            if (m_EditName.IsWindowEnabled())
            {
                ShowAni->Show(1 , m_Pnt.x + 136 , m_Pnt.y + 45);
            }
        }
        else
        {
            return ;
        }
        if (m_EditName.IsWindowEnabled())
        {
            m_EditName.Show(m_Pnt.x, m_Pnt.y);
            m_BtnRenameOk.Show(m_Pnt.x, m_Pnt.y );
            m_BtnRenameCancel.Show(m_Pnt.x, m_Pnt.y);
        }
        else
        {
            if (g_objHero.GetItemAmount(PACKAGE_PET) > 0)
            {
                CMyBitmap::ShowBlock(m_Pnt.x + m_PrgMonstName[m_iCurMonster].left,
                                     m_Pnt.y + m_PrgMonstName[m_iCurMonster].top,
                                     m_Pnt.x + m_PrgMonstName[m_iCurMonster].right,
                                     m_Pnt.y + m_PrgMonstName[m_iCurMonster].bottom,
                                     0XFF0000FF);
            }
            //Show static
            m_EditMonster4.Show (m_Pnt.x, m_Pnt.y);
            m_EditMonster3.Show(m_Pnt.x, m_Pnt.y);
            m_EditMonster1.Show(m_Pnt.x, m_Pnt.y);
            m_EditMonster2.Show(m_Pnt.x, m_Pnt.y);
        }
        //Show Button
        m_CloseBtn.Show(m_Pnt.x, m_Pnt.y);
        if (m_MagicBtn.IsWindowEnabled ())
        {
            m_MagicBtn.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_CallChk.IsWindowEnabled ())
        {
            m_CallChk.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnRename.IsWindowEnabled ())
        {
            m_BtnRename.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_DropBtn.IsWindowEnabled ())
        {
            m_DropBtn.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_ChkCallOut.IsWindowEnabled())
        {
            m_ChkCallOut.Show(m_Pnt.x, m_Pnt.y);
        }
        //Show image
        m_ImgMonster.Show(m_Pnt.x, m_Pnt.y);
        // show selected monster
        m_StaHp.Show(m_Pnt.x, m_Pnt.y);
        m_StaPotential.Show(m_Pnt.x, m_Pnt.y);
        m_StaExp.Show(m_Pnt.x, m_Pnt.y);
        m_StaLevel.Show(m_Pnt.x, m_Pnt.y);
        m_StaMaster.Show(m_Pnt.x, m_Pnt.y);
        m_StaSmart.Show(m_Pnt.x, m_Pnt.y);
        m_StaMagic.Show(m_Pnt.x, m_Pnt.y);
        m_StaMagicDef.Show(m_Pnt.x, m_Pnt.y);
        m_StaFight.Show(m_Pnt.x, m_Pnt.y);
        m_StaFightDef.Show(m_Pnt.x, m_Pnt.y);
        m_StaGrow.Show(m_Pnt.x, m_Pnt.y);
        if (m_DlgMonsterSkill.m_bShow)
        {
            m_DlgMonsterSkill.Show ();
        }
        //Show Gri
        m_EggsGri.Show(m_Pnt.x, m_Pnt.y);
    }
}


void CDlgMonster::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound( "Sound/window_close.wav" );
    CDlgMonster::EnableWindow(false);
}

void CDlgMonster::SetCurMonster(int mode)
{
    CItem* pMonster = g_objHero.GetItemByIndex(mode, PACKAGE_PET);
    if (pMonster == NULL)
    {
        m_iCurMonster = 0;
        return;
    }
    m_CallChk.EnableWindow();
    m_DropBtn.EnableWindow();
    m_ChkCallOut.EnableWindow();
    if (g_objHero.GetPetByItemID (pMonster->GetID()) != NULL)
    {
        MoveButtons(true);
        m_ChkCallOut.ChangeImage("Button908");
    }
    else
    {
        m_ChkCallOut.ChangeImage("Button901");
        MoveButtons (false);
    }
    char pText[64];
    m_CurMonsterID = pMonster->GetID();
    m_ImgMonster.RemoveImage();
    m_BtnRename.EnableWindow();
    char strName[32] = "";
    //等级
    itoa(pMonster->GetAmountLimit(), pText, 10);
    m_StaLevel.SetWindowText(pText);
    //忠诚
    itoa(pMonster->GetAmount(), pText, 10);
    m_StaMaster.SetWindowText(pText);
    //exp
    DWORD nPetExp = g_objHero.GetPetLevelExp(pMonster->GetAmountLimit());
    if (nPetExp == 0)
    {
        int idMsg = 100168;
        strcpy(pText, (char*)g_objGameDataSet.GetStr(idMsg));
    }
    else
    {
        double dRate = (double)pMonster->GetGemAtkType() * 100 / nPetExp;
        sprintf(pText, "%0.2f%%", dRate);
    }
    m_StaExp.SetWindowText(pText);
    //life
    sprintf(pText, "%d/%d", pMonster->GetWarGhostExp(), pMonster->GetMonsterMaxLife ());
    m_StaHp.SetWindowText(pText);
    if (!pMonster->IsMonsterDie())
    {
        m_ImgMonster.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), true);
    }
    else
    {
        m_ImgMonster.InsertImage(NULL, pMonster->GetID(), pMonster->GetTypeID(), false);
    }
    //potential
    itoa(pMonster->GetAddition(), pText, 10);
    m_StaPotential.SetWindowText(pText);
    //grow
    itoa(pMonster->GetMonsterGrow(), pText, 10);
    m_StaGrow.SetWindowText(pText);
    //atk
    sprintf(pText, "%d/%d", pMonster->GetMonsterMinAtk(), pMonster->GetMonsterMaxAtk());
    m_StaFight.SetWindowText(pText);
    //def
    itoa(pMonster->GetMonsterDef(), pText, 10);
    m_StaFightDef.SetWindowText(pText);
    //magic
    sprintf(pText, "%d/%d", pMonster->GetMonsterMagicMinAtk(), pMonster->GetMonsterMagicMaxAtk());
    m_StaMagic.SetWindowText(pText);
    //Magicdef
    itoa(pMonster->GetMonsterMagicDef(), pText, 10);
    m_StaMagicDef.SetWindowText(pText);
    //dex
    itoa(pMonster->GetDexterity(), pText, 10);
    m_StaSmart.SetWindowText(pText);
    m_iCurMonster = mode;
    if (CheckMonster())
    {
        m_CallChk.SetCheckValue(1);
    }
    else
    {
        m_CallChk.SetCheckValue(0);
    }
}



void CDlgMonster::SetEggGrid()
{
    //Clear Egg Grid
    m_EggsGri.RemoveIcon(0, 0, TRUE);
    //GET eggs from Egg package
    int nAmount = g_objHero.GetItemAmount(PACKAGE_EGG);
    for(int i = 0 ; i < nAmount ; i++)
    {
        CItem* pEgg = g_objHero.GetItemByIndex(i, PACKAGE_EGG);
        if (pEgg != NULL)
        {
            int nIndex = m_EggsGri.InsertIcon( 1, 1, i, ICON_TYPE_ITEM ) ;
            m_EggsGri.SetIconRealID(nIndex, pEgg->GetID());
        }
    }
}

void CDlgMonster::SetMonsterList()
{
    m_EditMonster1.SetWindowText("");
    m_EditMonster1.EnableWindow(false);
    m_EditMonster2.SetWindowText("");
    m_EditMonster2.EnableWindow(false);
    m_EditMonster3.SetWindowText("");
    m_EditMonster3.EnableWindow(false);
    m_EditMonster4.SetWindowText("");
    m_EditMonster4.EnableWindow(false);
    m_StaExp.SetWindowText("");
    m_StaFight.SetWindowText("");
    m_StaFightDef.SetWindowText("");
    m_StaGrow.SetWindowText("");
    m_StaHp.SetWindowText("");
    m_StaLevel.SetWindowText("");
    m_StaMagic.SetWindowText("");
    m_StaMagicDef.SetWindowText("");
    m_StaMaster.SetWindowText("");
    m_StaPotential.SetWindowText("");
    m_StaSmart.SetWindowText("");
    m_ImgMonster.RemoveImage();
    m_MagicBtn.EnableWindow (false);
    m_BtnRename.EnableWindow(false);
    m_DropBtn.EnableWindow (false);
    m_CallChk.EnableWindow (false);
    m_ChkCallOut.EnableWindow(false);
    int nAmount = g_objHero.GetItemAmount(PACKAGE_PET);
    BOOL bFind = false;
    for(int i = 0; i < nAmount; i++)
    {
        CItem* pMonster = g_objHero.GetItemByIndex(i, PACKAGE_PET);
        if (pMonster != NULL)
        {
            switch(i)
            {
            case 0:
                {
                    m_EditMonster1.SetWindowText(pMonster->GetMonsterName());
                }
                break;
            case 1:
                {
                    m_EditMonster2.SetWindowText(pMonster->GetMonsterName());
                }
                break;
            case 2:
                {
                    m_EditMonster3.SetWindowText(pMonster->GetMonsterName());
                }
                break;
            case 3:
                {
                    m_EditMonster4.SetWindowText(pMonster->GetMonsterName());
                }
                break;
            default:
                break;
            }
        }
        if (m_iCurMonster == i)
        {
            CDlgMonster::SetCurMonster(m_iCurMonster);
            bFind = true;
        }
    }
    if (!bFind)
    {
        m_iCurMonster = 0;
        CDlgMonster::SetCurMonster(m_iCurMonster);
    }
}

void CDlgMonster::OnBtnDrop()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Monster_Drop.wav" ) ;
    if (g_objHero.GetItemAmount(PACKAGE_PET) <= 0)
    {
        return;
    }
    if (g_objHero.IsDead())
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10694));//"亡灵状态无法对幻兽进行任何操作!"
        return;
    }
    CDlgMonster::GetParent()->PostMessage(WM_MY_MESSAGE, ON_MONSTER_DROP);
}


void CDlgMonster::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if ( g_bPickUp)
    {
        CDlgMonster::GetParent()->PostMessage(WM_MY_MESSAGE, ON_ICON_DROP, DLG_MONSTER);
    }
    else
    {
        int nAmount = g_objHero.GetItemAmount(PACKAGE_PET);
        for(int i = 0; i < nAmount; i++)
        {
            if (m_PrgMonstName[i].PtInRect(point))
            {
                if (i == m_iCurMonster)
                {
                    CDlgMonster::GetParent()->PostMessage(WM_MY_MESSAGE, ON_ICON_PICKUP, DLG_MONSTER);
                }
                else
                {
                    CDlgMonster::SetCurMonster(i);
                    if (m_DlgMonsterSkill.m_bShow)
                    {
                        m_DlgMonsterSkill.EnableWindow(false);
                    }
                }
                break;
            }
        }
    }
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgMonster::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnLButtonDblClk(nFlags, point);
}


void CDlgMonster::ResetMonsterDlg()
{
    m_iCurMonster = 0;
    m_IdCurCallMonster.clear ();
    m_CallChk.SetCheckValue(0);
}

BOOL CDlgMonster::CheckMonster()
{
    CItem* pMonster = g_objHero.GetItemByIndex(m_iCurMonster, PACKAGE_PET);
    if (pMonster != NULL)
    {
        for(int i = 0; i <(int) m_IdCurCallMonster.size (); i++)
            if (m_IdCurCallMonster[i] == pMonster->GetID())
            {
                return TRUE;
            }
    }
    return FALSE;
}

void CDlgMonster::DropMonster()
{
    if (g_objHero.GetItemAmount(PACKAGE_PET) <= 0)
    {
        return;
    }
    if (g_objHero.IsDead())
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10694));//"亡灵状态无法对幻兽进行任何操作!"
        return;
    }
    CItem* pMonster = g_objHero.GetItemByIndex(m_iCurMonster, PACKAGE_PET);
    if (pMonster)
    {
        CMyPos pPos;
        g_objHero.GetPos(pPos);
        g_objHero.DropItem(pMonster->GetID(), pPos.x, pPos.y);
    }
}

OBJID CDlgMonster::GetCurMonsterID()
{
    return m_CurMonsterID;
}

void CDlgMonster::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (g_bPickUp)
    {
        CDlgMonster::GetParent()->PostMessage(WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL);
    }
    CDialog::OnRButtonDown(nFlags, point);
}

void CDlgMonster::ResetMonstStatus()
{
    int nAmount = m_IdCurCallMonster.size ();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        CItem* pMonster = g_objHero.GetItem (m_IdCurCallMonster[i]);
        if (pMonster == NULL)
        {
            m_IdCurCallMonster.erase (m_IdCurCallMonster.begin () + i);
        }
    }
}

void CDlgMonster::OnBtnMagic()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (m_DlgMonsterSkill.m_bShow)
    {
        m_DlgMonsterSkill.EnableWindow (false);
    }
    else
    {
        m_DlgMonsterSkill.EnableWindow ();
        m_DlgMonsterSkill.SetMonsterID (m_iCurMonster);
    }
}

void CDlgMonster::MoveButtons(BOOL bCall)
{
    CRect rect;
    CDlgMonster::GetWindowRect (rect);
    if (bCall)
    {
        m_MagicBtn.EnableWindow (true);
        m_BtnRename.MoveWindow (184, 148, 32, 48, false);
        m_BtnRename.ReInit (rect.left, rect.top);
        m_DropBtn.MoveWindow (234, 148, 32, 48, false);
        m_DropBtn.ReInit (rect.left, rect.top);
    }
    else
    {
        m_MagicBtn.EnableWindow (false);
        m_BtnRename.MoveWindow (134, 148, 32, 48, false);
        m_BtnRename.ReInit (rect.left, rect.top);
        m_DropBtn.MoveWindow (184, 148, 32, 48, false);
        m_DropBtn.ReInit (rect.left, rect.top);
    }
}

void CDlgMonster::OnBtnRename()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (m_EditName.IsWindowEnabled())
    {
        CItem* pPet = g_objHero.GetItemByIndex(m_iCurMonster, PACKAGE_PET);
        if (pPet)
        {
            char szMsg[128] = "";
            sprintf(szMsg, (char*)g_objGameDataSet.GetStr(10724), pPet->GetMonsterName());
            g_objGameMsg.AddMsg( szMsg ); //放弃改名!
        }
        m_EditName.EnableWindow(false);
        m_BtnRenameOk.EnableWindow(false);
        m_BtnRenameCancel.EnableWindow(false);
    }
    else
    {
        m_EditName.EnableWindow();
        m_BtnRenameOk.EnableWindow();
        m_BtnRenameCancel.EnableWindow();
        m_EditName.SetWindowText("");
        m_EditName.SetFocus();
        g_objGameMsg.AddMsg ((char*)g_objGameDataSet.GetStr (10728));
    }
}


void CDlgMonster::OnBtnCallout()
{
    // TODO: Add your control notification handler code here
    if (g_objHero.IsDead () )
    {
        return;
    }
    CItem* pMonster = g_objHero.GetItemByIndex(m_iCurMonster, PACKAGE_PET);
    if (pMonster)
    {
        if (g_objHero.GetPetByItemID(pMonster->GetID()))
        {
            DXPlaySound ( "Sound/Monster_Recede.wav" ) ;
            g_objHero.KillEudenmon(pMonster->GetID());
            CDlgMonster::GetParent()->PostMessage (WM_MY_MESSAGE, ON_MONSTER_KILL, pMonster->GetID());
        }
        else
        {
            if (pMonster->GetAmountLimit() >= g_objHero.GetLev() + 10)
            {
                g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10808));	//10808=幻兽等级太高，您暂时无法召唤！
                return;
            }
            // 			if (pMonster->GetWarGhostExp() > 0)
            {
                DXPlaySound ( "Sound/Monster_Out.wav" ) ;
                g_objHero.CallEudenmon (pMonster->GetID());
                CDlgMonster::GetParent()->PostMessage (WM_MY_MESSAGE, ON_MONSTER_CALL, pMonster->GetID());
            }
            //			else
            //				g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10729));
        }
    }
}

void CDlgMonster::OnBtnRenamecancel()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/button_cancel.wav" ) ;
    if (m_EditName.IsWindowEnabled())
    {
        CItem* pPet = g_objHero.GetItemByIndex(m_iCurMonster, PACKAGE_PET);
        if (pPet)
        {
            char szMsg[128] = "";
            sprintf(szMsg, (char*)g_objGameDataSet.GetStr(10724), pPet->GetMonsterName());
            g_objGameMsg.AddMsg( szMsg ); //放弃改名!
        }
        m_EditName.EnableWindow(false);
        m_BtnRenameOk.EnableWindow(false);
        m_BtnRenameCancel.EnableWindow(false);
    }
}

void CDlgMonster::OnBtnRenameok()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/button_confirm.wav" ) ;
    char szName[_MAX_NAMESIZE] = "";
    m_EditName.GetWindowText(szName, _MAX_NAMESIZE);
    if (!StringCheck(szName))
    {
        g_objGameMsg.AddMsg( (char*)g_objGameDataSet.GetStr(10802) ); //名字不合法,不能有空格!
        return;
    }
    CItem* pPet = g_objHero.GetItemByIndex(m_iCurMonster, PACKAGE_PET);
    if (pPet)
    {
        g_objHero.RenameEudenmon(pPet->GetID(), szName);
    }
    m_EditName.EnableWindow(false);
    m_BtnRenameOk.EnableWindow(false);
    m_BtnRenameCancel.EnableWindow(false);
}
