
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// InfoViewManager.cpp: implementation of the CInfoViewManager class.
//
//////////////////////////////////////////////////////////////////////

#include "InfoViewManager.h"
#include "StringInfoView.h"
#include "AniInfoView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfoViewManager::CInfoViewManager()
{
}

CInfoViewManager::~CInfoViewManager()
{
    this->Clear();
}

//-------------------------------------------------------------------
void CInfoViewManager::Clear()
{
    int nAmount = m_setView.size();
    for(int i = 0; i < nAmount; i ++)
    {
        SAFE_DELETE(m_setView[i]);
    }
    m_setView.clear();
}
//-------------------------------------------------------------------
void CInfoViewManager::Show()
{
    int nAmount = m_setView.size();
    for(int i = 0; i < nAmount; i ++)
    {
        IInfoView* pView = m_setView[i];
        if (pView)
        {
            pView->Show();
        }
    }
}
//-------------------------------------------------------------------
void CInfoViewManager::AddStringView(char* pszStr, CMyPos posShow, DWORD dwColor)
{
    CStringInfoView* pView = CStringInfoView::CreateNew(pszStr, posShow, dwColor);
    if (pView)
    {
        m_setView.push_back(pView);
    }
}
//-------------------------------------------------------------------
void CInfoViewManager::AddAniView(char* pszFileName, char* pszIndex, CMyPos posShow, int nFrame)
{
    CAniInfoView* pView = CAniInfoView::CreateNew(pszFileName, pszIndex, posShow, nFrame);
    if (pView)
    {
        m_setView.push_back(pView);
    }
}
