
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef _SAYHELLO_H
#define _SAYHELLO_H

//include head files
#include "windows.h"
#include "basefunc.H"

class CSayHello
{
public:
    CSayHello();
    ~CSayHello();

private:
    char	m_szWords[128];
    int		m_iX, m_iY, m_iDir;
    CMySize	m_objFontSize;
    DWORD	m_dwTimeStart;
    int		m_iOff;

private:
    void DrawHelloBox();
    void Clear();
    void ShowHelloWords();

public:
    void ShowWords(int iScrX, int iScrY, int iDir);
    void SetWords(const char* pszWords);
};
#endif

