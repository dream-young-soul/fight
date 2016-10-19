
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgSetup_Music.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgSetup_Music.h"
#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup_Music dialog


CDlgSetup_Music::CDlgSetup_Music(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgSetup_Music::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgSetup_Music)
    m_bShow = false;
    m_Pnt = CPoint( 0, 0 );
    //}}AFX_DATA_INIT
}


void CDlgSetup_Music::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSetup_Music)
    DDX_Control(pDX, IDC_SLIDER_SOUND, m_SliderSound);
    DDX_Control(pDX, IDC_SLIDER_MUSIC, m_SliderMusic);
    DDX_Control(pDX, IDC_STA_SOUND, m_StaSound);
    DDX_Control(pDX, IDC_STA_MUSIC, m_StaMusic);
    DDX_Control(pDX, IDC_CHK_SOUND, m_SoundChk);
    DDX_Control(pDX, IDC_CHK_MUSIC, m_MusicChk);
    DDX_Control(pDX, IDC_BTN_SOUNDRIGHT, m_SoundRightBtn);
    DDX_Control(pDX, IDC_BTN_SOUNDLEFT, m_SoundLeftBtn);
    DDX_Control(pDX, IDC_BTN_MUSICRIGHT, m_MusicRightBtn);
    DDX_Control(pDX, IDC_BTN_MUSICLEFT, m_MusicLeftBtn);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetup_Music, CDialog)
    //{{AFX_MSG_MAP(CDlgSetup_Music)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_CHK_MUSIC, OnChkMusic)
    ON_BN_CLICKED(IDC_CHK_SOUND, OnChkSound)
    ON_BN_CLICKED(IDC_BTN_MUSICLEFT, OnBtnMusicleft)
    ON_BN_CLICKED(IDC_BTN_MUSICRIGHT, OnBtnMusicright)
    ON_BN_CLICKED(IDC_BTN_SOUNDLEFT, OnBtnSoundleft)
    ON_BN_CLICKED(IDC_BTN_SOUNDRIGHT, OnBtnSoundright)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup_Music message handlers

void CDlgSetup_Music::Show(int x, int y)
{
    if (m_bShow)
    {
        m_MusicChk.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_SoundChk.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_StaMusic.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_StaSound.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_SliderSound.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_SliderMusic.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_SoundLeftBtn.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_MusicLeftBtn.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_SoundRightBtn.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_MusicRightBtn.Show(m_Pnt.x + x, m_Pnt.y + y);
    }
}

LRESULT CDlgSetup_Music::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            return true ;
        }
    }
    else if ( message == WM_HSCROLL )
    {
        if (wParam == 1)
        {
            if (m_MusicChk.GetCheckValue() == 1)
            {
                DXSetMusicVolume ( m_SliderMusic.GetSliderValue ( ) ) ;
                g_objHero.SetMusicVolume(m_SliderMusic.GetSliderValue ( ));
            }
            else
            {
                DXSetMusicVolume (0) ;
                g_objHero.SetMusicVolume(0);
            }
            if (m_SoundChk.GetCheckValue() == 1)
            {
                DXSetSoundVolume(m_SliderSound.GetSliderValue());
            }
            else
            {
                DXSetSoundVolume(0);
            }
        }
        SetSliderStatic();
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgSetup_Music::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // TODO: Return a different brush if the default is not desired
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgSetup_Music::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgSetup_Music::GetWindowRect(rect);
    m_MusicChk.Init(rect.left, rect.top, "Check190", 2);
    m_SoundChk.Init(rect.left, rect.top, "Check190", 2);
    m_SoundLeftBtn.Init(rect.left, rect.top, "Button109");
    m_MusicLeftBtn.Init(rect.left, rect.top, "Button109");
    m_SoundRightBtn.Init(rect.left, rect.top, "Button1010");
    m_MusicRightBtn.Init(rect.left, rect.top, "Button1010");
    m_SliderMusic.Init(rect.left, rect.top, "Slider0");
    m_SliderMusic.SetSliderWidth ( 100 ) ;
    m_SliderMusic.SetMaxValue ( 100 ) ;
    m_SliderSound.Init(rect.left, rect.top, "Slider0");
    m_SliderSound.SetSliderWidth ( 100 ) ;
    m_SliderSound.SetMaxValue ( 100 ) ;
    m_StaMusic.Init(rect.left, rect.top);
    m_StaSound.Init(rect.left, rect.top);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetup_Music::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x;
    m_Pnt.y = y;
    // TODO: Add your message handler code here
}

void CDlgSetup_Music::SetSliderStatic()
{
    char strValue[16];
    itoa(m_SliderSound.GetSliderValue(), strValue, 10);
    m_StaSound.SetWindowText(strValue);
    itoa(m_SliderMusic.GetSliderValue(), strValue, 10);
    m_StaMusic.SetWindowText(strValue);
}

void CDlgSetup_Music::OnChkMusic()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_MusicChk.GetCheckValue() == 1)
    {
        DXSetMusicVolume(m_SliderMusic.GetSliderValue());
        g_objHero.SetMusicVolume(m_SliderMusic.GetSliderValue ( ));
        m_SliderMusic.EnableWindow();
        m_MusicLeftBtn.EnableWindow (true);
        m_MusicRightBtn.EnableWindow (true);
    }
    else
    {
        DXSetMusicVolume ( 0 ) ;
        g_objHero.SetMusicVolume(0);
        m_SliderMusic.EnableWindow(false);
        m_MusicLeftBtn.EnableWindow (false);
        m_MusicRightBtn.EnableWindow (false);
    }
}

void CDlgSetup_Music::OnChkSound()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_SoundChk.GetCheckValue() == 1)
    {
        DXSetSoundVolume(m_SliderSound.GetSliderValue());
        m_SliderSound.EnableWindow(true);
        m_SoundLeftBtn.EnableWindow (true);
        m_SoundRightBtn.EnableWindow (true);
    }
    else
    {
        DXSetSoundVolume(0);
        m_SliderSound.EnableWindow(false);
        m_SoundLeftBtn.EnableWindow (false);
        m_SoundRightBtn.EnableWindow (false);
    }
}

void CDlgSetup_Music::OnBtnMusicleft()
{
    // TODO: Add your control notification handler code here
    m_SliderMusic.SetSliderValue(m_SliderMusic.GetSliderValue() - 1);
}

void CDlgSetup_Music::OnBtnMusicright()
{
    // TODO: Add your control notification handler code here
    m_SliderMusic.SetSliderValue(m_SliderMusic.GetSliderValue() + 1);
}

void CDlgSetup_Music::OnBtnSoundleft()
{
    // TODO: Add your control notification handler code here
    m_SliderSound.SetSliderValue(m_SliderSound.GetSliderValue() - 1);
}

void CDlgSetup_Music::OnBtnSoundright()
{
    // TODO: Add your control notification handler code here
    m_SliderSound.SetSliderValue(m_SliderSound.GetSliderValue() + 1);
}
