
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// AniInfoView.cpp: implementation of the CAniInfoView class.
//
//////////////////////////////////////////////////////////////////////

#include "AniInfoView.h"
#include "GameDataSet.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAniInfoView::CAniInfoView()
{
}

CAniInfoView::~CAniInfoView()
{
}

//-----------------------------------------------------------------------
void CAniInfoView::Show()
{
    CAni* pAni = g_objGameDataSet.GetDataAni(m_szFile, m_szIndex);
    if (pAni)
    {
        pAni->Show(m_nFrame, m_posShow.x, m_posShow.y);
    }
}
//-----------------------------------------------------------------------
CAniInfoView* CAniInfoView::CreateNew(char* pszFile, char* pszIndex, CMyPos posShow, int nFrame)
{
    CAniInfoView* pView = new CAniInfoView;
    MYASSERT(pView);
    if (pView->Create(pszFile, pszIndex, posShow, nFrame))
    {
        return pView;
    }
    SAFE_DELETE(pView);
    return NULL;
}
//-----------------------------------------------------------------------
BOOL CAniInfoView::Create(char* pszFile, char* pszIndex, CMyPos posShow, int nFrame)
{
    if (!pszFile || !pszIndex)
    {
        return false;
    }
    strcpy(m_szFile, pszFile);
    strcpy(m_szIndex, pszIndex);
    m_posShow.x = posShow.x;
    m_posShow.y = posShow.y;
    m_nFrame = nFrame;
    return true;
}
//-----------------------------------------------------------------------
