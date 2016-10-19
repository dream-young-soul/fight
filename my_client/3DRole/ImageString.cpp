
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ImageString.cpp: implementation of the CImageString class.
//
//////////////////////////////////////////////////////////////////////

#include "Ani.h"
#include "GameDataSet.h"
#include "ImageString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageString::CImageString()
{
    memset(m_szString, 0L, sizeof(m_szString));
    m_posShow.x = m_posShow.y = 0;
    m_dwColor = 0x00ffffff;
    m_dwAlpha = 0xff;
    m_dwScalePercent = 100;
}

CImageString::~CImageString()
{
}

//////////////////////////////////////////////////////////////////////
CImageString* CImageString::CreateNew(const char* pszString, CMyPos posShow)
{
    if (!pszString)
    {
        return NULL;
    }
    CImageString* pObj = new CImageString;
    if (!pObj)
    {
        return NULL;
    }
    pObj->SetString(pszString);
    pObj->SetShowPos(posShow);
    return pObj;
}

//////////////////////////////////////////////////////////////////////
void CImageString::SetString	(const char* pszString)
{
    if (!pszString || strlen(pszString) >= sizeof(m_szString))
    {
        return;
    }
    strcpy(m_szString, pszString);
}

//////////////////////////////////////////////////////////////////////
void CImageString::Show	(void)
{
    int nStrLen = strlen(m_szString);
    if (nStrLen <= 0)
    {
        return;
    }
    const char	AniFile[]	= "ani/common.ani";
    const char	AniTitle[]	= "Num";
    // Show (posShow is the center)
    CAni* pAni = g_objGameDataSet.GetDataAni((char*)AniFile, (char*)AniTitle);
    if (!pAni)
    {
        return;
    }
    // get ani size
    CMySize sizeAni = pAni->GetSize(0);
    sizeAni.iWidth	= sizeAni.iWidth * m_dwScalePercent / 100;
    sizeAni.iHeight	= sizeAni.iHeight * m_dwScalePercent / 100;
    //	int nOffsetX = iWidth/2;
    // get num size
    CMySize sizeStr;
    sizeStr.iWidth	= nStrLen * sizeAni.iWidth;
    sizeStr.iHeight = sizeAni.iHeight;
    // get real show pos
    CMyPos posShow;
    posShow.x = m_posShow.x - sizeStr.iWidth / 2;
    posShow.y = m_posShow.y - sizeStr.iHeight / 2;
    // show time!
    for(int i = 0; i < nStrLen; i ++)
    {
        int nFrame = m_szString[i] - '0';
        CMyBitmap* pBmp = pAni->GetFrame(nFrame);
        if (pBmp)
        {
            pBmp->SetColor((BYTE)m_dwAlpha, (BYTE)((m_dwColor & 0xff0000) >> 16), (BYTE)((m_dwColor & 0xff00) >> 8), (BYTE)((m_dwColor & 0xff)));
            pBmp->ShowEx(posShow.x + sizeAni.iWidth * i * 3 / 5, posShow.y, NULL, sizeAni.iWidth, sizeAni.iHeight);
        }
    }
}
