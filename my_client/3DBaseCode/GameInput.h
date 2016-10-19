
//**********************************************************
// 代码编辑器
//**********************************************************

// GameInput.h: interface for the CGameInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEINPUT_H__53895A70_1055_43B7_9C43_4015FCD30FFB__INCLUDED_)
#define AFX_GAMEINPUT_H__53895A70_1055_43B7_9C43_4015FCD30FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <dinput.h>

const int GI_LSHIFT			= 42;
const int GI_LCTRL			= 29;
const int GI_LALT			= 56;
const int GI_RSHIFT			= 54;
const int GI_RCTRL			= 157;
const int GI_RALT			= 184;
const int GI_BACKSPACE		= 14;
const int GI_DEL			= 211;
const int GI_LARROW			= 203;
const int GI_UARROW			= 200;
const int GI_DARROW			= 208;
const int GI_RARROW			= 205;
const int GI_CAPSLOCK		= 58;
const int GI_ENTER			= 28;
const int GI_TABLE			= 15;
const int GI_ESCAPE         = 1;

const int GI_1				= 2;
const int GI_2				= 3;
const int GI_3				= 4;
const int GI_4				= 5;
const int GI_5				= 6;
const int GI_6				= 7;
const int GI_7				= 8;
const int GI_8				= 9;
const int GI_9				= 10;
const int GI_0				= 11;

const int GI_NUM1			= 79;
const int GI_NUM2			= 80;
const int GI_NUM3			= 81;
const int GI_NUM4			= 75;
const int GI_NUM5			= 76;
const int GI_NUM6			= 77;
const int GI_NUM7			= 71;
const int GI_NUM8			= 72;
const int GI_NUM9			= 73;
const int GI_NUM0			= 82;
const int GI_NUMENTER		= 156;
const int GI_NUMDEL			= 83;

const int GI_A				= 30;
const int GI_B				= 48;
const int GI_C				= 46;
const int GI_D				= 32;
const int GI_E				= 18;
const int GI_F				= 33;
const int GI_G				= 34;
const int GI_H				= 35;
const int GI_I				= 23;
const int GI_J				= 36;
const int GI_K				= 37;
const int GI_L				= 38;
const int GI_M				= 50;
const int GI_N				= 49;
const int GI_O				= 24;
const int GI_P				= 25;
const int GI_Q				= 16;
const int GI_R				= 19;
const int GI_S				= 31;
const int GI_T				= 20;
const int GI_U				= 22;
const int GI_V				= 47;
const int GI_W				= 17;
const int GI_X				= 45;
const int GI_Y				= 21;
const int GI_Z				= 44;

const DWORD KEYREPEAT_INTERVAL = 200;

class CGameInput
{
public:
    CGameInput();
    virtual ~CGameInput();

private:
    LPDIRECTINPUT8       m_pDI;
    LPDIRECTINPUTDEVICE8 m_pKeyboard;
    BYTE				 m_diks[256];
    BYTE				 m_oldDiks[256];

    HWND				 m_hWnd;
    DWORD				 m_dwDelay;
    DWORD				 m_dwBeginDelay;
private:
    BOOL			CheckKey(unsigned char ucKey);
    void			CapsLock();
    BOOL			IsBusy();
public:
    void			StartBusy(DWORD dwTime);
    void			Create(HINSTANCE hInst, HWND hWnd);
    void			Destroy();
    unsigned char	GetKey();
    BOOL			TestKey(int nKey);
    BOOL			IsUppercase();
};

class CGameInputStr
{
public:
    CGameInput	m_objGameInput;
private:
    char		m_szStr[256];
    char		m_szStrShow[256];
    int			m_nMaxLength;
    int			m_nCursorPos;
    unsigned char	m_ucOldChar;
    DWORD		m_dwRepeat;
    BOOL		m_bRepeat;
    BOOL		m_bMask;

    BOOL		m_bBackSpaceRepate;
    DWORD		m_dwBackSpaceRepeat;
    BOOL		m_bDelRepate;
    DWORD		m_dwDelRepeat;
    BOOL		m_bArrowRepate;
    DWORD		m_dwArrowRepate;


public:
    // 实例句柄      窗体句柄        最大长度        密码
    void	Init(HINSTANCE hInst, HWND hWnd, int nMaxLength, BOOL bMask = false);
    // 选中起始 选中终结
    BOOL	Process(int& nSelectBegin, int& nSelectEnd);
    // 得到字符串
    char*	GetStr();
    char*   GetShowStr();

    char	Key2Char(unsigned char ucKey);
    // 摧毁
    void	Destroy();

    void	SetStr(char* pszStr);
};
#endif // !defined(AFX_GAMEINPUT_H__53895A70_1055_43B7_9C43_4015FCD30FFB__INCLUDED_)
