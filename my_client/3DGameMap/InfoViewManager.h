
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// InfoViewManager.h: interface for the CInfoViewManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOVIEWMANAGER_H__6306B58A_F18A_4B16_A465_D8C4BF356FFF__INCLUDED_)
#define AFX_INFOVIEWMANAGER_H__6306B58A_F18A_4B16_A465_D8C4BF356FFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IInfoView.h"
#include "BaseFunc.h"

#include <vector>
using namespace std;

class CInfoViewManager
{
public:
    CInfoViewManager();
    virtual ~CInfoViewManager();
private:
    vector<IInfoView*> m_setView;
public:
    void Clear();
    void Show();
public:
    void AddStringView(char* pszStr, CMyPos posShow, DWORD dwColor);
    void AddAniView(char* pszFileName, char* pszIndex, CMyPos posShow, int nFrame);
};

#endif // !defined(AFX_INFOVIEWMANAGER_H__6306B58A_F18A_4B16_A465_D8C4BF356FFF__INCLUDED_)
