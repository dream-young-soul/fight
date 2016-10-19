
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// XpCounterDown.cpp: implementation of the CXpCounterDown class.
//
//////////////////////////////////////////////////////////////////////

#include "XpCounterDown.h"
#include "GameDataSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXpCounterDown::CXpCounterDown()
{
    m_nOldNum = -1;
}

CXpCounterDown::~CXpCounterDown()
{
}
//---------------------------------------------------------------------
void CXpCounterDown::Show(DWORD dwTimeLeft, int nHeight, DWORD dwMax, BOOL bLeftAlign)
{
    //const DWORD MaxXpNum = 20*1000;
    if (dwTimeLeft < 2000)
    {
        dwTimeLeft = 2000;
    }
    if (dwTimeLeft > dwMax)
    {
        return;
    }
    CMySize sizeScr;
    sizeScr.iWidth = _SCR_WIDTH;
    sizeScr.iHeight = _SCR_HEIGHT;
    DWORD dwBarColor;
    if (dwTimeLeft > dwMax / 2)
    {
        DWORD dwColorAdditive = 0;
        dwColorAdditive = 255 * (dwMax - dwTimeLeft) / (dwMax / 2);
        dwBarColor = 0xff00ff00 + (dwColorAdditive << 16);
    }
    else
    {
        DWORD dwColorAdditive = 0;
        dwColorAdditive = 255 * ((dwMax / 2) - dwTimeLeft) / (dwMax / 2);
        dwBarColor = 0xffffff00 - (dwColorAdditive << 8);
    }
    if (bLeftAlign)
    {
        CMyPos posBeginShow	= {100, nHeight};
        CMyPos posEndShow	= {_SCR_WIDTH - 10, nHeight + 10};
        CMyPos posXPEndShow	= {100 + (_SCR_WIDTH - 110)*(dwTimeLeft - 3000) / dwMax, nHeight + 10};
        CMyPos posTailBegin = {100 + (_SCR_WIDTH - 110)*(dwTimeLeft - 3000) / dwMax, nHeight};
        CMyPos posTailEnd	= {100 + (_SCR_WIDTH - 110)*(dwTimeLeft) / dwMax, nHeight + 10};
        DWORD dwColor[4];
        dwColor[0] = dwColor[1] = dwBarColor;
        dwColor[2] = dwColor[3] = dwBarColor & 0x00ffffff;
        if (dwTimeLeft > 3000)
            CMyBitmap::ShowBlock(posBeginShow.x, posBeginShow.y,
                                 posXPEndShow.x, posXPEndShow.y, dwBarColor);
        CMyBitmap::ShowBlock(posTailBegin.x, posTailBegin.y,
                             posTailEnd.x, posTailEnd.y, dwColor[0], dwColor[1], dwColor[2], dwColor[3]);
    }
    else
    {
        CMyPos posBeginShow	= {100 + (_SCR_WIDTH - 110)*(dwMax - (dwTimeLeft - 3000)) / dwMax, nHeight};
        CMyPos posEndShow	= {_SCR_WIDTH - 10, nHeight + 10};
        CMyPos posXPEndShow	= {_SCR_WIDTH - 10, nHeight + 10};
        CMyPos posTailBegin = {posBeginShow.x - (_SCR_WIDTH - 110) * 2000 / dwMax, nHeight};
        CMyPos posTailEnd	= {posBeginShow.x, nHeight + 10};
        if (posTailEnd.x > posEndShow.x)
        {
            posTailEnd.x = posEndShow.x;
        }
        DWORD dwColor[4];
        dwColor[0] = dwColor[1] = dwBarColor & 0x00ffffff;
        dwColor[2] = dwColor[3] = dwBarColor;
        if (dwTimeLeft > 3000)
            CMyBitmap::ShowBlock(posBeginShow.x, posBeginShow.y,
                                 posXPEndShow.x, posXPEndShow.y, dwBarColor);
        if (posTailEnd.x > posTailBegin.x)
            CMyBitmap::ShowBlock(posTailBegin.x, posTailBegin.y,
                                 posTailEnd.x, posTailEnd.y, dwColor[0], dwColor[1], dwColor[2], dwColor[3]);
    }
}
//---------------------------------------------------------------------
void CXpCounterDown::Show(int nNum, CMyPos posShow)
{
    if (nNum < 0)
    {
        return;
    }
    if (m_nOldNum != nNum)
    {
        m_dwTimeBegin = ::TimeGet();
        m_nOldNum = nNum;
    }
    DWORD dwTimePass = ::TimeGet() - m_dwTimeBegin;
    unsigned char ucAlpha, ucR, ucG, ucB, ucMiniSize, ucMaxSize;
    if (dwTimePass > 1000)
    {
        return;
    }
    if (nNum > 10)
    {
        ucAlpha = 0xff;
        ucR = 0x00;
        ucG = 0xff;
        ucB = 0x00;
        ucMiniSize = 36;
        ucMaxSize = 36;
    }
    else if (nNum > 5)
    {
        ucAlpha = 0xff;
        ucR = 0xff;
        ucG = 0xff;
        ucB = 0x00;
        ucMiniSize = 4;
        ucMaxSize = 48;
    }
    else
    {
        ucAlpha = 0xff;
        ucR = 0xff;
        ucG = 0x00;
        ucB = 0x00;
        ucMiniSize = 4;
        ucMaxSize = 64;
    }
    const char	AniFile[] = "ani/common.ani";
    const char	AniTitle[] = "Num";
    // Show (posShow is the center)
    CAni* pAni = g_objGameDataSet.GetDataAni((char*)AniFile, (char*)AniTitle);
    if (!pAni)
    {
        return;
    }
    char szNum[64];
    sprintf(szNum, "%d", nNum);
    int nLength = strlen(szNum);
    // get num size
    CMySize sizeNum;
    sizeNum.iHeight = ucMiniSize + dwTimePass * (ucMaxSize - ucMiniSize) / 1000;
    sizeNum.iWidth = sizeNum.iHeight / 2;
    // get show pos
    CMyPos posView;
    posView.x = posShow.x - sizeNum.iWidth * nLength / 2;
    posView.y = posShow.y - sizeNum.iHeight / 2;
    for(int i = 0; i < nLength; i ++)
    {
        int nFrame = szNum[i] - '0';
        CMyBitmap* pBmp = pAni->GetFrame(nFrame);
        if (pBmp)
        {
            pBmp->SetColor(ucAlpha, ucR, ucG, ucB);
        }
        pAni->ShowEx(nFrame, posView.x + sizeNum.iWidth * i,
                     posView.y, NULL, sizeNum.iWidth, sizeNum.iHeight);
    }
}
//---------------------------------------------------------------------

