
//**********************************************************
// 代码编辑器
//**********************************************************

// GameInput.cpp: implementation of the CGameInput class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseFunc.h"
#include "GameInput.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
typedef struct
{
    int		nVKey;
    char	cUppercaseChar;
    char	cLowercaseChar;
} CharMapInfo;

const int MAX_CHARMAP = 46;
CharMapInfo g_infoCharMap[MAX_CHARMAP] =
{
    {GI_1, '1', '1'},
    {GI_2, '2', '2'},
    {GI_3, '3', '3'},
    {GI_4, '4', '4'},
    {GI_5, '5', '5'},
    {GI_6, '6', '6'},
    {GI_7, '7', '7'},
    {GI_8, '8', '8'},
    {GI_9, '9', '9'},
    {GI_0, '0', '0'},

    {GI_NUM1, '1', '1'},
    {GI_NUM2, '2', '2'},
    {GI_NUM3, '3', '3'},
    {GI_NUM4, '4', '4'},
    {GI_NUM5, '5', '5'},
    {GI_NUM6, '6', '6'},
    {GI_NUM7, '7', '7'},
    {GI_NUM8, '8', '8'},
    {GI_NUM9, '9', '9'},
    {GI_NUM0, '0', '0'},


    {GI_A, 'A', 'a'},
    {GI_B, 'B', 'b'},
    {GI_C, 'C', 'c'},
    {GI_D, 'D', 'd'},
    {GI_E, 'E', 'e'},
    {GI_F, 'F', 'f'},
    {GI_G, 'G', 'g'},
    {GI_H, 'H', 'h'},
    {GI_I, 'I', 'i'},
    {GI_J, 'J', 'j'},
    {GI_K, 'K', 'k'},
    {GI_L, 'L', 'l'},
    {GI_M, 'M', 'm'},
    {GI_N, 'N', 'n'},
    {GI_O, 'O', 'o'},
    {GI_P, 'P', 'p'},
    {GI_Q, 'Q', 'q'},
    {GI_R, 'R', 'r'},
    {GI_S, 'S', 's'},
    {GI_T, 'T', 't'},
    {GI_U, 'U', 'u'},
    {GI_V, 'V', 'v'},
    {GI_W, 'W', 'w'},
    {GI_X, 'X', 'x'},
    {GI_Y, 'Y', 'y'},
    {GI_Z, 'Z', 'z'},
};

CGameInput::CGameInput()
{
    m_pDI = NULL;
    m_pKeyboard = NULL;
    m_hWnd = NULL;
    m_dwDelay = 0;
    m_dwBeginDelay = ::TimeGet();
}

CGameInput::~CGameInput()
{
    this->Destroy();
}
//------------------------------------------------------------------
void CGameInput::Create(HINSTANCE hInst, HWND hWnd)
{
    if (!hWnd)
    {
        return;
    }
    m_hWnd = hWnd;
    // 对象创建
    if (FAILED(DirectInput8Create(hInst, 0x0800,
                                  IID_IDirectInput8, (VOID**)&m_pDI, NULL)))
    {
        return;
    }
    // 设备创建
    if (FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL)))
    {
        return;
    }
    if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
    {
        return;
    }
    // 设置合作等级
    DWORD dwCoopFlags = DISCL_EXCLUSIVE;
    dwCoopFlags |= DISCL_FOREGROUND;
    HRESULT hr = m_pKeyboard->SetCooperativeLevel(hWnd, dwCoopFlags);
    if (hr == DIERR_INVALIDPARAM )
    {
        return;
    }
    else if (hr == DIERR_NOTINITIALIZED )
    {
        return;
    }
    else if (hr == E_HANDLE)
    {
        return;
    }
    // 获得设备
    m_pKeyboard->Acquire();
}
//------------------------------------------------------------------
void CGameInput::Destroy()
{
    if ( m_pKeyboard )
    {
        m_pKeyboard->Unacquire();
    }
    if (m_pKeyboard)
    {
        m_pKeyboard->Release();
        m_pKeyboard = NULL;
    }
    if (m_pDI)
    {
        m_pDI->Release();
        m_pDI = NULL;
    }
}
//------------------------------------------------------------------
unsigned char CGameInput::GetKey()
{
    if (IsBusy())
    {
        return 0;
    }
    HRESULT hr;
    if (NULL == m_pKeyboard)
    {
        return 0;
    }
    memcpy(&m_oldDiks, &m_diks, sizeof(m_diks));
    ZeroMemory(&m_diks, sizeof(m_diks));
    hr = m_pKeyboard->GetDeviceState(sizeof(m_diks), &m_diks);
    if (FAILED(hr))
    {
        hr = m_pKeyboard->Acquire();
        while( hr == DIERR_INPUTLOST )
        {
            hr = m_pKeyboard->Acquire();
        }
        return 0;
    }
    for(int i = 0; i < 256; i ++)
    {
        if ((m_diks[i] & 0x80) && !(m_oldDiks[i] & 0x80))
        {
            if (this->CheckKey(i))
            {
                return i;
            }
        }
    }
    /*	for(int j = 0; j < 256; j ++)
    	{
    		if (m_diks[j] & 0x80)
    		{
    			if (this->CheckKey(j))
    				return j;
    		}
    	}
    */
    if (m_diks[GI_CAPSLOCK]  & 0x80)
    {
        this->CapsLock();
    }
    return 0;
}
//------------------------------------------------------------------
BOOL CGameInput::CheckKey(unsigned char ucKey)
{
    // 目前允许数字和字母
    // 数字
    if (ucKey >= GI_1 && ucKey <= GI_0)
    {
        return true;
    }
    // 小键盘数字
    if (ucKey >= GI_NUM7 && ucKey <= GI_NUM9)
    {
        return true;
    }
    if (ucKey >= GI_NUM4 && ucKey <= GI_NUM6)
    {
        return true;
    }
    if (ucKey >= GI_NUM1 && ucKey <= GI_NUM0)
    {
        return true;
    }
    // 字母
    if (ucKey >= GI_Q && ucKey <= GI_P)
    {
        return true;
    }
    if (ucKey >= GI_A && ucKey <= GI_L)
    {
        return true;
    }
    if (ucKey >= GI_Z && ucKey <= GI_M)
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------
void CGameInput::CapsLock()
{
    m_pKeyboard->Unacquire();
    keybd_event( VK_CAPITAL,
                 GI_CAPSLOCK,
                 KEYEVENTF_EXTENDEDKEY | 0,
                 0 );
    keybd_event( VK_CAPITAL,
                 GI_CAPSLOCK,
                 KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                 0);
    m_pKeyboard->Acquire();
}
//------------------------------------------------------------------
BOOL CGameInput::IsUppercase()
{
    BOOL bShift = false;
    if (this->TestKey(GI_LSHIFT) || this->TestKey(GI_RSHIFT))
    {
        bShift = true;
    }
    unsigned short usState = ::GetKeyState(VK_CAPITAL);
    if ((usState & 0x00ff) && !bShift)
    {
        return true;
    }
    if (!(usState & 0x00ff) && bShift)
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------
void CGameInput::StartBusy(DWORD dwTime)
{
    m_dwBeginDelay = ::TimeGet();
    m_dwDelay = dwTime;
}
//------------------------------------------------------------------
BOOL CGameInput::IsBusy()
{
    if (::TimeGet() - m_dwBeginDelay < m_dwDelay)
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------
BOOL CGameInput::TestKey(int nKey)
{
    if (nKey > 256)
    {
        return false;
    }
    if (nKey == GI_TABLE)
    {
        return ((m_diks[nKey] & 0x80)  && !(m_oldDiks[nKey] & 0x80));
    }
    return m_diks[nKey] & 0x80;
}

//###############################################################################################//


//------------------------------------------------------------------
void CGameInputStr::Init(HINSTANCE hInst, HWND hWnd, int nMaxLength, BOOL bMask)
{
    strcpy(m_szStr, "");
    m_objGameInput.Create(hInst, hWnd);
    m_nMaxLength = nMaxLength;
    if (m_nMaxLength > 255)
    {
        m_nMaxLength = 255;
    }
    m_nCursorPos = 0;
    m_ucOldChar = 0;
    m_bRepeat = false;
    m_bBackSpaceRepate = false;
    m_bDelRepate = false;
    m_bArrowRepate = false;
    m_bMask = bMask;
}
//------------------------------------------------------------------
BOOL CGameInputStr::Process(int& nSelectBegin, int& nSelectEnd)
{
    unsigned char ucKey = m_objGameInput.GetKey();
    m_nCursorPos = nSelectBegin;
    if (ucKey == 0)
    {
        m_bRepeat = false;
        //		m_ucOldChar = 0;
        //		::OutputDebugString("==============================\n");
    }
    if (ucKey != 0 && m_ucOldChar == ucKey)
    {
        // if not repeatdelay, begin repeatdelay.
        if (!m_bRepeat)
        {
            //			::OutputDebugString("开始重复\n");
            m_bRepeat = true;
            m_dwRepeat = ::TimeGet();
            return false;
        }
        else
        {
            // is time out?
            if (::TimeGet() - m_dwRepeat > KEYREPEAT_INTERVAL)
            {
                //				::OutputDebugString("重复时间未到\n");
                m_bRepeat = false;
            }
            else
            {
                return false;
            }
        }
    }
    if (ucKey != 0)
    {
        m_bRepeat = false;
        m_ucOldChar = ucKey;
    }
    // 如果有选中，消除选中
    if (nSelectBegin > nSelectEnd)
    {
        int nExchange = nSelectBegin;
        nSelectBegin = nSelectEnd;
        nSelectEnd = nExchange;
    }
    if ((nSelectEnd - nSelectBegin != 0) && !m_objGameInput.TestKey(GI_LARROW) && !m_objGameInput.TestKey(GI_RARROW))
    {
        if ((ucKey != 0) ||
                m_objGameInput.TestKey(GI_BACKSPACE) ||
                m_objGameInput.TestKey(GI_DEL) ||
                m_objGameInput.TestKey(GI_NUMDEL))
        {
            m_bBackSpaceRepate = true;
            m_dwBackSpaceRepeat = ::TimeGet();
            m_bDelRepate = true;
            m_dwDelRepeat = ::TimeGet();
            char szTemp[256] = "";
            int nLength = (int)strlen(m_szStr);
            int nTempIndex = 0;
            for(int i = 0; i < nLength + 1; i ++)
            {
                if (i >= nSelectEnd || i < nSelectBegin)
                {
                    szTemp[nTempIndex] = m_szStr[i];
                    nTempIndex ++;
                }
            }
            strcpy(m_szStr, szTemp);
            nSelectEnd = nSelectBegin;
            if (ucKey == 0)
            {
                return true;
            }
        }
    }
    else
    {
        // 功能键处理
        if (m_objGameInput.TestKey(GI_BACKSPACE) && nSelectBegin != 0)
        {
            if (!m_bBackSpaceRepate)
            {
                m_bBackSpaceRepate = true;
                m_dwBackSpaceRepeat = ::TimeGet();
            }
            else
            {
                if (::TimeGet() - m_dwBackSpaceRepeat < KEYREPEAT_INTERVAL)
                {
                    return false;
                }
            }
            // 删除前一个
            int nLength = strlen(m_szStr);
            char szTemp[256] = "";
            int nTempIndex = 0;
            for(int i = 0; i < nLength + 1; i ++)
            {
                if (i != nSelectBegin - 1)
                {
                    szTemp[nTempIndex] = m_szStr[i];
                    nTempIndex ++;
                }
            }
            strcpy(m_szStr, szTemp);
            m_bBackSpaceRepate = true;
            m_dwBackSpaceRepeat = ::TimeGet();
            nSelectBegin --;
            nSelectEnd --;
            return true;
        }
        else
        {
            m_bBackSpaceRepate = false;
        }
        if (m_objGameInput.TestKey(GI_DEL) || m_objGameInput.TestKey(GI_NUMDEL))
        {
            if (nSelectBegin != (int)strlen(m_szStr))
            {
                if (!m_bDelRepate)
                {
                    m_bDelRepate = true;
                    m_dwDelRepeat = ::TimeGet();
                }
                else
                {
                    if (::TimeGet() - m_dwDelRepeat < KEYREPEAT_INTERVAL)
                    {
                        return false;
                    }
                }
                // 删除后一个
                int nLength = strlen(m_szStr);
                char szTemp[256] = "";
                int nTempIndex = 0;
                for(int i = 0; i < nLength + 1; i ++)
                {
                    if (i !=  nSelectBegin)
                    {
                        szTemp[nTempIndex] = m_szStr[i];
                        nTempIndex ++;
                    }
                }
                strcpy(m_szStr, szTemp);
                m_bDelRepate = true;
                m_dwDelRepeat = ::TimeGet();
                return true;
            }
        }
        else
        {
            m_bDelRepate = false;
        }
        if (m_objGameInput.TestKey(GI_LARROW) || m_objGameInput.TestKey(GI_RARROW))
        {
            if (!m_bArrowRepate)
            {
                m_bArrowRepate = true;
                m_dwArrowRepate = ::TimeGet();
            }
            else
            {
                if (::TimeGet() - m_dwArrowRepate < KEYREPEAT_INTERVAL)
                {
                    return false;
                }
            }
            if (m_objGameInput.TestKey(GI_LARROW))
            {
                nSelectBegin --;
                if (!m_objGameInput.TestKey(GI_LSHIFT) && !m_objGameInput.TestKey(GI_RSHIFT))
                {
                    nSelectEnd = nSelectBegin;
                }
            }
            else
            {
                nSelectEnd++;
                if (!m_objGameInput.TestKey(GI_LSHIFT) && !m_objGameInput.TestKey(GI_RSHIFT))
                {
                    nSelectBegin = nSelectEnd;
                }
            }
            m_bArrowRepate = true;
            m_dwArrowRepate = ::TimeGet();
            return true;
        }
        else
        {
            m_bArrowRepate = false;
        }
    }
    if (m_ucOldChar != 0 && !m_objGameInput.TestKey(m_ucOldChar))
    {
        m_ucOldChar = 0;
    }
    if (ucKey == 0)
    {
        return false;
    }
    if ((int)strlen(m_szStr) >= m_nMaxLength)
    {
        return false;
    }
    char cChar = this->Key2Char(ucKey);
    int nLength = strlen(m_szStr);
    if (cChar == 0)
    {
        return true;
    }
    // 插入一个字符
    /*
    	char szMsg[128];
    	sprintf(szMsg, "%c\n", cChar);
    	::OutputDebugString(szMsg);
    */
    char szTemp[256] = "";
    for(int i = 0; i < nLength + 1; i ++)
    {
        if (i < nSelectBegin)
        {
            szTemp[i] = m_szStr[i];
        }
        if (i == nSelectBegin)
        {
            szTemp[i] = cChar;
            szTemp[i+1] = m_szStr[i];
        }
        if (i > nSelectBegin)
        {
            szTemp[i+1] = m_szStr[i];
        }
    };
    strcpy(m_szStr, szTemp);
    m_szStr[nLength+2] = '\0';
    m_objGameInput.StartBusy(30);
    nSelectBegin = nSelectBegin + 1;
    nSelectEnd = nSelectBegin;
    return true;
}
//------------------------------------------------------------------
char* CGameInputStr::GetStr()
{
    return m_szStr;
}
//------------------------------------------------------------------
void CGameInputStr::Destroy()
{
    m_objGameInput.Destroy();
}
//------------------------------------------------------------------
char CGameInputStr::Key2Char(unsigned char ucKey)
{
    for(int i = 0; i < MAX_CHARMAP; i++)
    {
        if (g_infoCharMap[i].nVKey == ucKey)
        {
            if (m_objGameInput.IsUppercase())
            {
                return g_infoCharMap[i].cUppercaseChar;
            }
            else
            {
                return g_infoCharMap[i].cLowercaseChar;
            }
        }
    }
    return 0;
}
//------------------------------------------------------------------
char* CGameInputStr::GetShowStr()
{
    if (m_bMask)
    {
        int nLength = strlen(m_szStr);
        int i = 0;
        for(; i < nLength; i ++)
        {
            m_szStrShow[i] = '*';
        }
        m_szStrShow[i+1] = '\0';
        return m_szStrShow;
    }
    else
    {
        return m_szStr;
    }
}
//------------------------------------------------------------------
void CGameInputStr::SetStr(char* pszStr)
{
    if (!pszStr)
    {
        return;
    }
    strcpy(m_szStr, pszStr);
    m_szStr[256] = '\0';
}
//------------------------------------------------------------------
