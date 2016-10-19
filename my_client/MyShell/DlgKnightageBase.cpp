
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgKnightageBase.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgKnightageBase.h"

#include "Hero.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageBase dialog


CDlgKnightageBase::CDlgKnightageBase(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgKnightageBase::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgKnightageBase)
    m_Pnt = CPoint(0, 0);
    m_bShow = false;
    //}}AFX_DATA_INIT
}


void CDlgKnightageBase::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgKnightageBase)
    DDX_Control(pDX, IDC_BTN_GIVEOK, m_BtnGiveOk);
    DDX_Control(pDX, IDC_BTN_GIVECANCEL, m_BtnGiveCancel);
    DDX_Control(pDX, IDC_EDIT_MONEY, m_EditMoney);
    DDX_Control(pDX, IDC_STA_HEROTEAM, m_StaHeroTeam);
    DDX_Control(pDX, IDC_STA_HERONAME, m_StaHeroName);
    DDX_Control(pDX, IDC_STA_HEROLEV, m_StaHeroLev);
    DDX_Control(pDX, IDC_BTN_GIVE, m_BtnGiveMoney);
    DDX_Control(pDX, IDC_BTN_ADD, m_BtnWantJoin);
    DDX_Control(pDX, IDC_STA_OFFER, m_StaOffer);
    DDX_Control(pDX, IDC_STA_KNIGHTNUM, m_StaKnightNum);
    DDX_Control(pDX, IDC_STA_KNIGHTNAME, m_StaKnightName);
    DDX_Control(pDX, IDC_STA_KNIGHTMONEY, m_StaKnightMoney);
    DDX_Control(pDX, IDC_STA_KNIGHTLEV, m_StaKnightLev);
    DDX_Control(pDX, IDC_STA_KNIGHTFAMOR, m_StaKnightFamor);
    DDX_Control(pDX, IDC_STA_JOB, m_StaJob);
    DDX_Control(pDX, IDC_STA_HEADNAME, m_StaHeadName);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgKnightageBase, CDialog)
    //{{AFX_MSG_MAP(CDlgKnightageBase)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
    ON_BN_CLICKED(IDC_BTN_GIVE, OnBtnGive)
    ON_EN_CHANGE(IDC_EDIT_MONEY, OnChangeEditMoney)
    ON_BN_CLICKED(IDC_BTN_GIVEOK, OnBtnGiveok)
    ON_BN_CLICKED(IDC_BTN_GIVECANCEL, OnBtnGivecancel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageBase message handlers

void CDlgKnightageBase::Show(int x, int y)
{
    if (m_bShow)
    {
        m_StaHeadName.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_StaJob.Show(m_Pnt.x + x, m_Pnt.y + y);
        /*
        		int nPosX = 90;
        		if (g_objHero.GetSydicateID() != 0)
        		{
        			int nLevel = g_objHero.GetSyndicateLevel();
        			CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char * )g_strControlAni,
        				"SynLevel",
        				EXIGENCE_IMMEDIATE ) ;
        			if ( ShowAni == NULL )
        				return;
        			if (nLevel == 0)
        			{
        				ShowAni->Show ( 0,
        					m_Pnt.x + x + nPosX,
        					m_Pnt.y + y + 263) ;
        			}
        			else if (nLevel >0)
        			{
        				for(int i=0;i<nLevel;i++)
        				{
        					ShowAni->Show ( 1,
        						m_Pnt.x + x + nPosX + (i-1)*12,
        						m_Pnt.y + y + 263) ;
        				}
        			}
        		}*/
        m_StaKnightFamor.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_StaKnightMoney.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_StaKnightNum.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_StaKnightLev.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_StaOffer.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_StaKnightName.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_StaHeroName.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_StaHeroLev.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_StaHeroTeam.Show(m_Pnt.x + x, m_Pnt.y + y);
        if (m_EditMoney.IsWindowEnabled())
        {
            m_EditMoney.Show(m_Pnt.x + x, m_Pnt.y + y);
        }
        if (m_BtnGiveOk.IsWindowEnabled())
        {
            m_BtnGiveOk.Show(m_Pnt.x + x, m_Pnt.y + y);
        }
        if (m_BtnGiveCancel.IsWindowEnabled())
        {
            m_BtnGiveCancel.Show(m_Pnt.x + x, m_Pnt.y + y);
        }
        if (m_BtnWantJoin.IsWindowEnabled())
        {
            m_BtnWantJoin.Show(m_Pnt.x + x, m_Pnt.y + y);
        }
        if (m_BtnGiveMoney.IsWindowEnabled())
        {
            m_BtnGiveMoney.Show(m_Pnt.x + x, m_Pnt.y + y);
        }
    }
}

LRESULT CDlgKnightageBase::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message == WM_ENABLE)
    {
        m_bShow = wParam;
        if (m_bShow == TRUE )
        {
            m_nEdtNum = 0;
            m_nSkip = 0;
            m_EditMoney.EnableWindow(false);
            m_BtnGiveCancel.EnableWindow(false);
            m_BtnGiveOk.EnableWindow(false);
            SetKnightBaseData();
        }
    }
    else if (message == WM_COMMAND)
    {
        if (wParam == 1)
        {
            return true;
        }
        else if (wParam == 2)
        {
            return true;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgKnightageBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // TODO: Return a different brush if the default is not desired
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgKnightageBase::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgKnightageBase::GetWindowRect(rect);
    m_StaHeroName.Init(rect.left, rect.top);
    m_StaHeroName.SetFontType(2);
    m_StaHeroTeam.Init(rect.left, rect.top);
    m_StaHeroTeam.SetStaticPosType(1);
    m_StaHeroTeam.SetFontType(2);
    m_StaHeroLev.Init(rect.left, rect.top);
    m_StaKnightName.Init(rect.left, rect.top);
    m_StaKnightName.SetStaticPosType(1);
    m_StaKnightName.SetFontType(2);
    m_StaKnightFamor.Init(rect.left, rect.top);
    m_StaKnightNum.Init(rect.left, rect.top);
    m_StaKnightLev.Init(rect.left, rect.top);
    m_StaHeadName.Init(rect.left, rect.top);
    m_StaKnightMoney.Init(rect.left, rect.top);
    m_StaOffer.Init(rect.left, rect.top);
    m_StaJob.Init(rect.left, rect.top);
    m_EditMoney.Init(rect.left, rect.top, false, "Edit40");
    m_EditMoney.SetLimitText(12);
    m_BtnGiveMoney.Init(rect.left, rect.top, "Check86");
    m_BtnGiveMoney.SetTipShow((char*)g_objGameDataSet.GetStr(10850), 0xFFE800);	//点击可输入捐献金钱金额
    m_BtnGiveOk.Init(rect.left, rect.top, "Button150");
    m_BtnGiveCancel.Init( rect.left, rect.top, "Button151");
    m_BtnWantJoin.Init(rect.left, rect.top, "Button831");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgKnightageBase::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgKnightageBase::SetKnightBaseData()
{
    if (g_objHero.GetSydicateID() != ID_NONE)
    {
        char strText[64];
        m_StaJob.SetWindowText(g_objHero.GetSyndicateTitle());
        itoa(g_objHero.GetSyndicateProffer(), strText, 10);
        m_StaOffer.SetWindowText(strText);
        m_StaKnightName.SetWindowText(g_objHero.GetSyndicate());
        m_StaHeadName.SetWindowText(g_objHero.GetSyndicateLeaderName());
        sprintf(strText, g_objGameDataSet.GetStr(10136), g_objHero.GetSyndicatePopulation());
        m_StaKnightNum.SetWindowText(strText);
        itoa(g_objHero.GetSyndicateFamer(), strText, 10);
        m_StaKnightFamor.SetWindowText(strText);
        DWORD dwSynLev = 10830 + g_objHero.GetSyndicateLevel();		//军团等级
        m_StaKnightLev.SetWindowText((char*)g_objGameDataSet.GetStr(dwSynLev));
        DWORD dwHeroRank = 10840 + g_objHero.GetSyndicateSynRank() - 1;	//玩家的军团等级
        m_StaHeroLev.SetWindowText((char*)g_objGameDataSet.GetStr(dwHeroRank));
        m_StaHeroName.SetWindowText(g_objHero.GetName());
        itoa(g_objHero.GetSyndicateFund(), strText, 10);
        m_StaKnightMoney.SetWindowText(strText);
        m_BtnWantJoin.ChangeImage("Check87");
        if (!g_objHero.IsSynLeader())
        {
            m_BtnWantJoin.EnableWindow();
        }
        else
        {
            m_BtnWantJoin.EnableWindow(false);
        }
        m_BtnGiveMoney.EnableWindow(true);
    }
    else
    {
        m_StaJob.SetWindowText("");
        m_StaOffer.SetWindowText("");
        m_StaKnightName.SetWindowText("");
        m_StaHeadName.SetWindowText("");
        m_StaKnightNum.SetWindowText("");
        m_StaKnightFamor.SetWindowText("");
        m_StaKnightLev.SetWindowText("");
        m_StaKnightMoney.SetWindowText("");
        m_StaHeroLev.SetWindowText("");
        m_StaHeroName.SetWindowText("");
        m_StaHeroTeam.SetWindowText("");
        m_BtnWantJoin.ChangeImage("Button831");
        m_BtnWantJoin.EnableWindow();
        m_BtnGiveMoney.EnableWindow(false);
    }
}

void CDlgKnightageBase::OnBtnAdd()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (g_objHero.GetSydicateID() == ID_NONE)
    {
        CDlgKnightageBase::GetParent()->GetParent()->PostMessage(WM_MY_MESSAGE, ON_FACTION_JOIN);
    }
    else
    {
        if (g_objHero.GetSyndicateRank() > 10)  //检查贡献度 -- 预备团员不用检查
        {
            if (g_objHero.GetSyndicateProffer() < 200 )
            {
                g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10851)) ;	//您的军团贡献度小于200，不允许退团！
                return;
            }
        }
        CDlgKnightageBase::GetParent()->GetParent()->PostMessage(WM_MY_MESSAGE, ON_FACTION_OUT);
    }
}

void CDlgKnightageBase::OnBtnGive()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (g_objHero.GetSydicateID() != ID_NONE && !m_EditMoney.IsWindowEnabled())
    {
        m_EditMoney.SetWindowText("");
        m_EditMoney.EnableWindow();
        m_BtnGiveCancel.EnableWindow();
        m_BtnGiveOk.EnableWindow();
        m_EditMoney.SetFocus();
    }
}

void CDlgKnightageBase::OnChangeEditMoney()
{
    char strTemp[12] ;
    m_EditMoney.GetWindowText ( strTemp, 12 ) ;
    if ( !strcmp ( strTemp, "0" ) )
    {
        m_EditMoney.SetWindowText ( NULL ) ;
        return ;
    }
    m_nSkip++ ;
    if ( m_nSkip % 2 == 0 )
    {
        return ;
    }
    int nLength = m_EditMoney.GetWindowTextLength () ;
    if ( nLength >= 3 )
    {
        char* pstrTemp = strchr ( strTemp, ',' ) ;
        if ( pstrTemp != NULL )
        {
            char strText[12] ;
            char strNum[12] ;
            strncpy ( strText, strTemp, pstrTemp - strTemp ) ;
            strText[pstrTemp - strTemp] = 0 ;
            strcat ( strText, pstrTemp + 1 ) ;
            pstrTemp = strchr ( strText, ',' ) ;
            if ( pstrTemp != NULL )
            {
                strncpy ( strTemp, strText, pstrTemp - strText ) ;
                strTemp[pstrTemp - strText] = 0 ;
                strcat ( strTemp, pstrTemp + 1 ) ;
                strcpy ( strNum, strTemp ) ;
            }
            else
            {
                strcpy ( strNum, strText ) ;
            }
            if ( strNum != NULL )
            {
                m_nEdtNum = atoi ( strNum ) ;
                int nlen = strlen ( strNum ) ;
                CString cstrText = strNum;
                if ( nlen > 3 && nlen < 7 )
                {
                    cstrText.Insert ( nlen - 3, ',' ) ;
                }
                else if ( nlen >= 7 )
                {
                    cstrText.Insert ( nlen - 6, ',' ) ;
                    cstrText.Insert ( nlen - 2, ',' ) ;
                }
                m_EditMoney.SetWindowText ( cstrText ) ;
                m_EditMoney.SetSel ( 12, 12 ) ;
            }
        }
        else if ( strlen ( strTemp ) == 4 )
        {
            m_nEdtNum = atoi ( strTemp ) ;
            CString cstrText = strTemp;
            cstrText.Insert ( 1, ',' ) ;
            m_EditMoney.SetWindowText ( cstrText ) ;
            m_EditMoney.SetSel ( 5, 5 ) ;
        }
        else
        {
            m_nEdtNum = atoi ( strTemp ) ;
            m_nSkip++ ;
        }
    }
    else
    {
        m_nSkip = 0 ;
        m_nEdtNum = atoi ( strTemp ) ;
    }
}

void CDlgKnightageBase::OnBtnGiveok()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/button_confirm.wav" ) ;
    if (g_objHero.GetSydicateID() != ID_NONE
            && m_EditMoney.IsWindowEnabled()
            && m_nEdtNum > 0 )
    {
        if (m_nEdtNum > (int)g_objHero.GetMoney ())
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10024));	//金钱不足
            return;
        }
        g_objHero.SynContribute ( m_nEdtNum ) ;
        m_EditMoney.EnableWindow(false);
        m_BtnGiveCancel.EnableWindow(false);
        m_BtnGiveOk.EnableWindow(false);
    }
}

void CDlgKnightageBase::OnBtnGivecancel()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/button_cancel.wav" ) ;
    if ( m_EditMoney.IsWindowEnabled() )
    {
        m_EditMoney.EnableWindow(false);
        m_BtnGiveCancel.EnableWindow(false);
        m_BtnGiveOk.EnableWindow(false);
    }
}
