
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// FloatingStr.h: interface for the CFloatingStr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLOATINGSTR_H__6762C612_CC11_4162_990D_42C63710E792__INCLUDED_)
#define AFX_FLOATINGSTR_H__6762C612_CC11_4162_990D_42C63710E792__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"

typedef struct
{
    char	szStr[_MAX_NAMESIZE];
    int		nMaxHeight;
    int		nHeight;
    int		nDelayTime;
    int		nStepHeight;
    DWORD	dwStartTime;
    DWORD	dwColor;
} FStrInfo;

#include <deque>
using namespace std;
typedef deque<FStrInfo*> DEQUE_FSTRINFO;

class CFloatingStr
{
public:
    CFloatingStr();
    virtual ~CFloatingStr();

public:
    void AddStr(char* pszStr, int nMaxHeight, DWORD dwColor, int nStepHeight , int nDelayTime = 0);
    void Show(CMyPos posScr);
    void Process();
private:
    DEQUE_FSTRINFO m_setStr;

private:
    void Destory();
};

#endif // !defined(AFX_FLOATINGSTR_H__6762C612_CC11_4162_990D_42C63710E792__INCLUDED_)
