
//**********************************************************
// 代码编辑器
//**********************************************************

// FloatingStr.cpp: implementation of the CFloatingStr class.
//
//////////////////////////////////////////////////////////////////////

#include "FloatingStr.h"
#include "ND_BITMAP.H"
// constant ...
const int _MAXFLOATING_STEP = 3;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFloatingStr::CFloatingStr()
{
    m_setStr.clear();
}

CFloatingStr::~CFloatingStr()
{
    this->Destory();
}

//------------------------------------------------------------------
void CFloatingStr::AddStr(char* pszStr, int nMaxHeight, DWORD dwColor, int nStepHeight, int nDelayTime)
{
    if (!pszStr)
    {
        return;
    }
    if (strlen(pszStr) >= _MAX_NAMESIZE)
    {
        return;
    }
    CMySize sizeFont;
    CMyBitmap::GetFontSize(sizeFont);
    int nAmount = m_setStr.size();
    if (nAmount > 0)
    {
        FStrInfo* pLastInfo = m_setStr[nAmount-1];
        if (pLastInfo)
        {
            if (pLastInfo->nHeight < sizeFont.iHeight)
            {
                // 所有字符串抬升..
                int nOffset = sizeFont.iHeight - pLastInfo->nHeight;
                for(int i = 0; i < nAmount; i++)
                {
                    FStrInfo* pMyInfo = m_setStr[i];
                    pMyInfo->nHeight += nOffset;
                }
            }
        }
    }
    FStrInfo* pInfo = new FStrInfo;
    MYASSERT(pInfo);
    pInfo->dwStartTime = ::TimeGet();
    pInfo->nDelayTime = nDelayTime;
    pInfo->nHeight = 0;
    pInfo->nMaxHeight = nMaxHeight;
    pInfo->dwColor = dwColor;
    pInfo->nStepHeight = nStepHeight;
    strcpy(pInfo->szStr, pszStr);
    m_setStr.push_back(pInfo);
}
//------------------------------------------------------------------
void CFloatingStr::Show(CMyPos posScr)
{
    int nAmount = m_setStr.size();
    CMySize sizeFont;
    CMyBitmap::GetFontSize(sizeFont);
    for(int i = 0; i < nAmount; i++)
    {
        FStrInfo* pInfo = m_setStr[i];
        if (!pInfo)
        {
            continue;
        }
        if (pInfo  && ((int)(::TimeGet() - pInfo->dwStartTime)) < pInfo->nDelayTime)
        {
            continue;
        }
        CMyPos posShow;
        posShow.x = posScr.x - strlen(pInfo->szStr) * sizeFont.iWidth / 2;
        posShow.y = posScr.y - pInfo->nHeight;
        CMyBitmap::ShowString(posShow.x, posShow.y, pInfo->dwColor, pInfo->szStr);
    }
}
//------------------------------------------------------------------
void CFloatingStr::Process()
{
    int nAmount = m_setStr.size();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        FStrInfo* pInfo = m_setStr[i];
        if (pInfo && ((int)(::TimeGet() - pInfo->dwStartTime)) > pInfo->nDelayTime)
        {
            // 判断生命，增加高度
            pInfo->nHeight += pInfo->nStepHeight;
            if (pInfo->nHeight >= pInfo->nMaxHeight)
            {
                SAFE_DELETE(pInfo);
                m_setStr.erase(m_setStr.begin() + i);
            }
        }
    }
}
//------------------------------------------------------------------
void CFloatingStr::Destory()
{
    int nAmount = m_setStr.size();
    for(int i = 0; i < nAmount; i++)
    {
        FStrInfo* pInfo = m_setStr[i];
        SAFE_DELETE(pInfo);
    }
    m_setStr.clear();
}
//------------------------------------------------------------------
