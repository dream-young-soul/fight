
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgMonsterSkill.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgMonsterSkill.h"

#include "Hero.h"
#include "Magic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int _PAGE_AMOUNT = 4;
/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterSkill dialog


CDlgMonsterSkill::CDlgMonsterSkill(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMonsterSkill::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMonsterSkill)
    m_Pnt = CPoint(0, 0);
    m_bShow = FALSE;
    m_nCurSkillIndex = 0;// NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgMonsterSkill::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMonsterSkill)
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    DDX_Control(pDX, IDC_STA_NAME4, m_StaName4);
    DDX_Control(pDX, IDC_STA_NAME3, m_StaName3);
    DDX_Control(pDX, IDC_STA_NAME2, m_StaName2);
    DDX_Control(pDX, IDC_STA_NAME1, m_StaName1);
    DDX_Control(pDX, IDC_STA_NAME, m_StaName);
    DDX_Control(pDX, IDC_STA_DESCM4, m_StaDescM4);
    DDX_Control(pDX, IDC_STA_DESCM3, m_StaDescM3);
    DDX_Control(pDX, IDC_STA_DESCM2, m_StaDescM2);
    DDX_Control(pDX, IDC_STA_DESCM1, m_StaDescM1);
    DDX_Control(pDX, IDC_IMG_SKILL4, m_ImgSkill4);
    DDX_Control(pDX, IDC_IMG_SKILL3, m_ImgSkill3);
    DDX_Control(pDX, IDC_IMG_SKILL2, m_ImgSkill2);
    DDX_Control(pDX, IDC_IMG_SKILL1, m_ImgSkill1);
    DDX_Control(pDX, IDC_BTN_PAGEUP, m_BtnPageUp);
    DDX_Control(pDX, IDC_BTN_PAGEDOWN, m_BtnPageDown);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMonsterSkill, CDialog)
    //{{AFX_MSG_MAP(CDlgMonsterSkill)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_PAGEDOWN, OnBtnPagedown)
    ON_BN_CLICKED(IDC_BTN_PAGEUP, OnBtnPageup)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterSkill message handlers

LRESULT CDlgMonsterSkill::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message == WM_ENABLE)
    {
        m_bShow = wParam;
        if (m_bShow == TRUE)
        {
            m_StaName.SetWindowText ("");
            ResetMonsterSkill();
        }
        if (wParam)
        {
            DXPlaySound ( "Sound/Open.wav" ) ;
        }
        else
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
        }
    }
    else if (message == WM_COMMAND)
    {
        if (wParam == 1 || wParam == 2)
        {
            return true;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgMonsterSkill::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgMonsterSkill::GetWindowRect (rect);
    m_ImgSkill1.Init (rect.left, rect.top, NULL, SKILL_IMAGE);
    m_ImgSkill2.Init (rect.left, rect.top, NULL, SKILL_IMAGE);
    m_ImgSkill3.Init (rect.left, rect.top, NULL, SKILL_IMAGE);
    m_ImgSkill4.Init (rect.left, rect.top, NULL, SKILL_IMAGE);
    m_StaName.Init (rect.left, rect.top);
    m_StaName.SetStaticPosType(1);
    m_StaName1.Init (rect.left, rect.top);
    m_StaName2.Init (rect.left, rect.top);
    m_StaName3.Init (rect.left, rect.top);
    m_StaName4.Init (rect.left, rect.top);
    m_StaDescM1.Init (rect.left, rect.top);
    m_StaDescM2.Init (rect.left, rect.top);
    m_StaDescM3.Init (rect.left, rect.top);
    m_StaDescM4.Init (rect.left, rect.top);
    m_BtnPageUp.Init (rect.left, rect.top, "Button815");
    m_BtnPageDown.Init (rect.left, rect.top, "Button816");
    m_BtnClose.Init (rect.left, rect.top, "Button540");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgMonsterSkill::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

void CDlgMonsterSkill::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgMonsterSkill::OnBtnPagedown()
{
    // TODO: Add your control notification handler code here
    CPet* pPet = g_objHero.GetPetByItemID (m_IdMonster);
    if (pPet)
    {
        if (m_nCurSkillIndex + _PAGE_AMOUNT < pPet->GetMagicAmount ())
        {
            m_nCurSkillIndex += _PAGE_AMOUNT;
            CDlgMonsterSkill::SetMonsterSKill ();
        }
    }
}

void CDlgMonsterSkill::OnBtnPageup()
{
    // TODO: Add your control notification handler code here
    if (m_nCurSkillIndex > 0)
    {
        m_nCurSkillIndex -= _PAGE_AMOUNT;
        CDlgMonsterSkill::SetMonsterSKill ();
    }
}

void CDlgMonsterSkill::ResetMonsterSkill()
{
    m_ImgSkill1.RemoveImage();
    m_ImgSkill2.RemoveImage();
    m_ImgSkill3.RemoveImage();
    m_ImgSkill4.RemoveImage();
    m_StaDescM1.SetWindowText("");
    m_StaDescM2.SetWindowText("");
    m_StaDescM3.SetWindowText("");
    m_StaDescM4.SetWindowText("");
    m_StaName1.SetWindowText("");
    m_StaName2.SetWindowText("");
    m_StaName3.SetWindowText("");
    m_StaName4.SetWindowText("");
}

void CDlgMonsterSkill::SetMonsterID (int nIndex)
{
    CItem* pItem  = g_objHero.GetItemByIndex (nIndex, PACKAGE_PET);
    if (pItem)
    {
        m_IdMonster = pItem->GetID ();
        m_StaName.SetWindowText (pItem->GetMonsterName ());
        SetMonsterSKill ();
        return;
    }
    m_IdMonster = 0;
}

void CDlgMonsterSkill::SetMonsterSKill()
{
    CDlgMonsterSkill::ResetMonsterSkill ();
    CPet* pPet = g_objHero.GetPetByItemID (m_IdMonster);
    if (pPet == NULL)
    {
        CDlgMonsterSkill::EnableWindow (false);
    }
    if (pPet)
    {
        int nIndex = 0;
        int nAmount = pPet->GetMagicAmount ();
        if (nAmount >= m_nCurSkillIndex + _PAGE_AMOUNT)
        {
            nAmount = m_nCurSkillIndex + _PAGE_AMOUNT;
            m_BtnPageDown.EnableWindow ();
        }
        else
        {
            m_BtnPageDown.EnableWindow (false);
        }
        if (m_nCurSkillIndex > 0)
        {
            m_BtnPageUp.EnableWindow ();
        }
        else
        {
            m_BtnPageUp.EnableWindow (false);
        }
        for(int i = m_nCurSkillIndex; i < nAmount; i++)
        {
            CMagic* pMagic = pPet->GetMagicByIndex (i);
            if (pMagic)
            {
                char SkillType[32] ;
                sprintf ( SkillType, "MagicSkillType%d", pMagic->GetIDType() ) ;
                char strExp[64] ;
                sprintf( strExp, "等级:%d 经验值:%d/%d ", pMagic->GetLevel() + 1, pMagic->GetExp(), pMagic->GetNextLevelExp() ) ;
                switch(i - m_nCurSkillIndex)
                {
                case 0:
                    m_StaName1.SetWindowText (pMagic->GetName ());
                    m_ImgSkill1.InsertImage( SkillType, 0 , pMagic->GetIDType(), false ) ;
                    m_StaDescM1.SetWindowText (strExp);
                    break;
                case 1:
                    m_StaName2.SetWindowText (pMagic->GetName ());
                    m_ImgSkill2.InsertImage( SkillType, 0 , pMagic->GetIDType(), false ) ;
                    m_StaDescM2.SetWindowText (strExp);
                    break;
                case 2:
                    m_StaName3.SetWindowText (pMagic->GetName ());
                    m_ImgSkill3.InsertImage( SkillType, 0 , pMagic->GetIDType(), false ) ;
                    m_StaDescM3.SetWindowText (strExp);
                    break;
                case 3:
                    m_StaName4.SetWindowText (pMagic->GetName ());
                    m_ImgSkill4.InsertImage( SkillType, 0 , pMagic->GetIDType(), false ) ;
                    m_StaDescM4.SetWindowText (strExp);
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void CDlgMonsterSkill::Show()
{
    if (m_bShow)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog84",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0, m_Pnt.x, m_Pnt.y ) ;
        }
        else
        {
            return ;
        }
        m_ImgSkill1.Show (m_Pnt.x, m_Pnt.y);
        m_ImgSkill2.Show (m_Pnt.x, m_Pnt.y);
        m_ImgSkill3.Show (m_Pnt.x, m_Pnt.y);
        m_ImgSkill4.Show (m_Pnt.x, m_Pnt.y);
        m_StaName.Show (m_Pnt.x, m_Pnt.y);
        m_StaName1.Show (m_Pnt.x, m_Pnt.y);
        m_StaName2.Show (m_Pnt.x, m_Pnt.y);
        m_StaName3.Show (m_Pnt.x, m_Pnt.y);
        m_StaName4.Show (m_Pnt.x, m_Pnt.y);
        m_StaDescM1.Show (m_Pnt.x, m_Pnt.y);
        m_StaDescM2.Show (m_Pnt.x, m_Pnt.y);
        m_StaDescM3.Show (m_Pnt.x, m_Pnt.y);
        m_StaDescM4.Show (m_Pnt.x, m_Pnt.y);
        if (m_BtnPageUp.IsWindowEnabled ())
        {
            m_BtnPageUp.Show (m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnPageDown.IsWindowEnabled ())
        {
            m_BtnPageDown.Show (m_Pnt.x, m_Pnt.y);
        }
        m_BtnClose.Show (m_Pnt.x, m_Pnt.y);
        CDlgMonsterSkill::ShowSkillInfo (m_Pnt.x, m_Pnt.y);
    }
}

void CDlgMonsterSkill::ShowSkillDesc(int nStartX, int nStartY, int nWidth, const char* szString, DWORD dwColor)
{
    char Static[64] ;
    strcpy(Static, szString);
    if (!Static || strlen(Static) == 0)
    {
        return;
    }
    char szTemp[64];
    strcpy(szTemp, Static);
    char* pszTemp = szTemp;
    unsigned char ucSingle[2] = {'\0', '\0'};
    unsigned char ucDouble[3] = {'\0', '\0', '\0' }; // 双字节字符
    //------------------------------------------------------------
    int nShowX = nStartX;
    int nShowY = nStartY;
    int nPosX  = nStartX;
    int nPosY  = nStartY;
    int nPos = 0;
    bool bShow = false;
    char sz[64];
    memset(sz, 0L, sizeof(sz));
    CMySize psizeFont;
    CMyBitmap::GetFontSize(psizeFont);
    //------------------------------------------------------------
    while((ucSingle[0] = *pszTemp))
    {
        sz[nPos++] = ucSingle[0];
        if (ucSingle[0] < 0x80) // 打印一个单字节字符
        {
            nPosX += psizeFont.iWidth;
            if ((nPosX - nStartX) > (nWidth - psizeFont.iWidth * 2)
                    || ucSingle[0] == '#')
            {
                nPosX = nStartX;
                nPosY += psizeFont.iHeight + 2;
                bShow = true;
            }
        }
        else // 打印双字节字符
        {
            if (ucDouble[0] == '\0')
            {
                ucDouble[0] = ucSingle[0];
            }
            else if (ucDouble[1] == '\0')
            {
                ucDouble[1] = ucSingle[0];
                ucDouble[0] = '\0';
                ucDouble[1] = '\0';
                nPosX = nPosX + psizeFont.iWidth * 2;
                if ((nPosX - nStartX) > (nWidth - psizeFont.iWidth * 2))
                {
                    nPosX = nStartX;
                    nPosY += psizeFont.iHeight + 2;
                    bShow = true;
                }
                ucDouble[0] = '\0';
                ucDouble[1] = '\0';
            }
        }
        if (bShow)
        {
            CMyBitmap::ShowString(nShowX, nShowY, dwColor, sz);
            nPos = 0;
            bShow = false;
            nShowX = nPosX;
            nShowY = nPosY;
            memset(sz, 0L, sizeof(sz));
        }
        pszTemp++;
    }
    if (sz || bShow)
    {
        CMyBitmap::ShowString(nShowX, nShowY, dwColor, sz);
    }
}

int CDlgMonsterSkill::GetMouseInMagicIndex()
{
    POINT mousePnt ;
    GetCursorPos ( &mousePnt ) ;
    CRect pPrgRect[4];
    m_ImgSkill1.GetWindowRect(pPrgRect[0]);
    m_ImgSkill2.GetWindowRect(pPrgRect[1]);
    m_ImgSkill3.GetWindowRect(pPrgRect[2]);
    m_ImgSkill4.GetWindowRect(pPrgRect[3]);
    int nIndex = 0;
    for(int i = 0; i < 4; i++)
    {
        if (pPrgRect[i].PtInRect(mousePnt))
        {
            nIndex = i + 1;
            break;
        }
    }
    if (nIndex == 0)
    {
        return 0;
    }
    else
    {
        return nIndex;
    }
}

void CDlgMonsterSkill::ShowSkillInfo(int x, int y)
{
    if (m_IdMonster == 0)
    {
        return;
    }
    CPet* pPet = g_objHero.GetPetByItemID (m_IdMonster);
    if (pPet == NULL)
    {
        return;
    }
    int nIndex = CDlgMonsterSkill::GetMouseInMagicIndex();
    if (nIndex != 0)
    {
        DWORD STRCOLOR_GREEN = 0x00ff00;
        DWORD STRCOLOR_BLUE  = 0x0080ff;
        DWORD STRCOLOR_RED = 0xff0000;
        DWORD STRCOLOR_WHITE = 0xffffff;
        CMagic* pMagic = pPet->GetMagicByIndex(m_nCurSkillIndex + nIndex - 1);
        if (pMagic)
        {
            char strText[3][64];
            DWORD strColor;
            CMySize FontSize;
            CMyBitmap::GetFontSize(FontSize);
            sprintf( strText[0], "%s", pMagic->GetName()) ;
            if (pMagic->GetNeedLevel() == 0)
            {
                strColor = STRCOLOR_RED;
                strcpy( strText[1], (char*)g_objGameDataSet.GetStr(10710));	//10710=不可继续修习
            }
            else if (pMagic->GetNeedLevel() > 0 )
            {
                if (pMagic->GetNeedLevel() <= g_objHero.GetLev())
                {
                    strColor = STRCOLOR_BLUE;
                }
                else
                {
                    strColor = STRCOLOR_RED;
                }
                sprintf( strText[1], "%d%s", pMagic->GetNeedLevel(), (char*)g_objGameDataSet.GetStr(10711));	//10711=可以继续修习
            }
            strcpy(strText[2], pMagic->GetDescribe());
            int nPosX = x + 33 - FontSize.iWidth * 14;
            int nPosY = y + 71 + 48 * (nIndex - 1);
            int nLine = 2 + CDlgMonsterSkill::GetSkillDescLine(0,
                        0,
                        FontSize.iWidth * 14,
                        strText[2]);
            g_objGameDataSet.ShowInfoBack(nPosX - 5,	nPosY - 5,
                                          FontSize.iWidth * 14 + 10, (FontSize.iHeight + 2)*nLine + 10) ;
            CMyBitmap::ShowString ( nPosX,
                                    nPosY ,
                                    STRCOLOR_GREEN,
                                    strText[0] ) ;
            CMyBitmap::ShowString ( nPosX,
                                    nPosY  + FontSize.iHeight + 2,
                                    strColor,
                                    strText[1] ) ;
            CDlgMonsterSkill::ShowSkillDesc( nPosX,
                                             nPosY  + (FontSize.iHeight + 2) * 2,
                                             FontSize.iWidth * 14,
                                             strText[2] , STRCOLOR_WHITE) ;
        }
    }
}

void CDlgMonsterSkill::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    CDlgMonsterSkill::EnableWindow (false);
}

int CDlgMonsterSkill::GetSkillDescLine(int nStartX, int nStartY, int nWidth, const char* szString)
{
    char Static[64] ;
    strcpy(Static, szString);
    if (!Static || strlen(Static) == 0)
    {
        return 0;
    }
    char szTemp[64];
    strcpy(szTemp, Static);
    char* pszTemp = szTemp;
    unsigned char ucSingle[2] = {'\0', '\0'};
    unsigned char ucDouble[3] = {'\0', '\0', '\0' }; // 双字节字符
    //------------------------------------------------------------
    int nShowX = nStartX;
    int nShowY = nStartY;
    int nPosX  = nStartX;
    int nPosY  = nStartY;
    int nPos = 0;
    bool bShow = false;
    int  nLine = 1;
    char sz[64];
    memset(sz, 0L, sizeof(sz));
    CMySize psizeFont;
    CMyBitmap::GetFontSize(psizeFont);
    //------------------------------------------------------------
    while((ucSingle[0] = *pszTemp))
    {
        sz[nPos++] = ucSingle[0];
        if (ucSingle[0] < 0x80) // 打印一个单字节字符
        {
            nPosX += psizeFont.iWidth;
            if (((nPosX - nStartX) > (nWidth - psizeFont.iWidth * 2)
                    || ucSingle[0] == '#')
                    && *(pszTemp + 1) != '\0')
            {
                nPosX = nStartX;
                nPosY += psizeFont.iHeight + 2;
                bShow = true;
                nLine++;
            }
        }
        else // 打印双字节字符
        {
            if (ucDouble[0] == '\0')
            {
                ucDouble[0] = ucSingle[0];
            }
            else if (ucDouble[1] == '\0')
            {
                ucDouble[1] = ucSingle[0];
                ucDouble[0] = '\0';
                ucDouble[1] = '\0';
                nPosX = nPosX + psizeFont.iWidth * 2;
                if ((nPosX - nStartX) > (nWidth - psizeFont.iWidth * 2)
                        && *(pszTemp + 1) != '\0')
                {
                    nPosX = nStartX;
                    nPosY += psizeFont.iHeight + 2;
                    bShow = true;
                    nLine++;
                }
                ucDouble[0] = '\0';
                ucDouble[1] = '\0';
            }
        }
        if (bShow)
        {
            nPos = 0;
            bShow = false;
            nShowX = nPosX;
            nShowY = nPosY;
            memset(sz, 0L, sizeof(sz));
        }
        pszTemp++;
    }
    return nLine;
}
