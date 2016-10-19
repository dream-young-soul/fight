
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ZoomNum.cpp: implementation of the CZoomNum class.
//
//////////////////////////////////////////////////////////////////////

#include "ZoomNum.h"
#include "GamedataSet.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZoomNum::CZoomNum()
{
    m_setInfo.clear();
}

CZoomNum::~CZoomNum()
{
    this->Destroy();
}
//------------------------------------------------------------------
void CZoomNum::Destroy()
{
    int nAmount = m_setInfo.size();
    for(int i = 0; i < nAmount; i++)
    {
        ZoomNumInfo* pInfo = m_setInfo[i];
        SAFE_DELETE(pInfo);
    }
    m_setInfo.clear();
}
//------------------------------------------------------------------
void CZoomNum::Add(int nNum, DWORD dwColor, DWORD dwDelay, int nRandOffset)
{
    ZoomNumInfo* pInfo = new ZoomNumInfo;
    MYASSERT(pInfo);
    m_setInfo.push_back(pInfo);
    pInfo->nNum = nNum;
    pInfo->dwTimeBegin = ::TimeGet();
    pInfo->dwColor = dwColor;
    pInfo->dwDelay = dwDelay;
    if (nRandOffset != 0)
    {
        pInfo->nOffsetX = ::RandGet(nRandOffset * 2) - nRandOffset;
    }
    else
    {
        pInfo->nOffsetX = 0;
    }
}
//------------------------------------------------------------------
void CZoomNum::Show(CMyPos posShow)
{
    const char	AniFile[] = "ani/common.ani";
    const char	AniTitle[] = "Num";
    const DWORD dwZoomOutTime = 200;
    const DWORD dwFadeOutTime = 1600;
    const int	nMinSize = 32;
    const int	nMaxSize = 48;
    const int	nMaxOffsetY = 40;
    const int	nMaxOffsetY_C = 25;
    const int	nMaxOffsetX_C = 25;
    DWORD dwCurrentTime = ::TimeGet();
    int nAmount = m_setInfo.size();
    int i;
    for(i = 0; i < nAmount; i ++)
    {
        ZoomNumInfo* pInfo = m_setInfo[i];
        MYASSERT (pInfo);
        char szNum[16];
        sprintf(szNum, "%d", pInfo->nNum);
        // get timepass
        DWORD dwTimePass = dwCurrentTime - pInfo->dwTimeBegin;
        if (dwTimePass < pInfo->dwDelay)
        {
            return;
        }
        // ZoomOut or FadeOut?
        if (dwTimePass < (dwZoomOutTime + pInfo->dwDelay))
        {
            // ZoomOut ...
            // get the size of num
            CMySize sizeNum;
            //sizeNum.iHeight	=	nMinSize;
            //sizeNum.iWidth	=	nMinSize/2;
            sizeNum.iHeight	=	nMaxSize - (nMaxSize - nMinSize) * (dwTimePass - pInfo->dwDelay) / dwZoomOutTime;
            sizeNum.iWidth	=	sizeNum.iHeight / 2;
            // get the showpos
            CMyPos posView;
            posView.x = posShow.x - strlen(szNum) * sizeNum.iWidth / 2;
            posView.y = posShow.y - sizeNum.iHeight;
            int nOffset = nMaxOffsetY * (dwTimePass - pInfo->dwDelay) / dwZoomOutTime;
            // show
            CAni* pAni = g_objGameDataSet.GetDataAni((char*)AniFile, (char*)AniTitle);
            if (pAni)
            {
                if (pInfo->nNum == 0)
                {
                    CMyBitmap* pBmp = pAni->GetFrame(10);
                    if (pBmp)
                    {
                        unsigned char ucR, ucG, ucB;
                        ucR = (unsigned char)((pInfo->dwColor & 0x00ff0000) >> 16);
                        ucG = (unsigned char)((pInfo->dwColor & 0x0000ff00) >> 8);
                        ucB = (unsigned char)(pInfo->dwColor & 0x000000ff);
                        pBmp->SetColor(0xff, ucR, ucG, ucB);
                    }
                    pAni->ShowEx(10, posView.x + sizeNum.iWidth + pInfo->nOffsetX,
                                 posView.y - nOffset, NULL, sizeNum.iWidth * 4, sizeNum.iHeight * 4, 0);
                }
                else
                {
                    for(int j = 0; j < (int)strlen(szNum); j++)
                    {
                        int nFrame = szNum[j] - '0';
                        CMyBitmap* pBmp = pAni->GetFrame(nFrame);
                        if (pBmp)
                        {
                            unsigned char ucR, ucG, ucB;
                            ucR = (unsigned char)((pInfo->dwColor & 0x00ff0000) >> 16);
                            ucG = (unsigned char)((pInfo->dwColor & 0x0000ff00) >> 8);
                            ucB = (unsigned char)(pInfo->dwColor & 0x000000ff);
                            pBmp->SetColor(0xff, ucR, ucG, ucB);
                        }
                        pAni->ShowEx(nFrame, posView.x + sizeNum.iWidth * j + pInfo->nOffsetX,
                                     posView.y - nOffset, NULL, sizeNum.iWidth, sizeNum.iHeight, 0);
                    }
                }
            }
        }
        else if (dwTimePass < dwZoomOutTime + dwFadeOutTime + pInfo->dwDelay)
        {
            // FadeOut ...
            // get the size of num
            CMySize sizeNum;
            sizeNum.iHeight	=	nMinSize;
            sizeNum.iWidth	=	nMinSize / 2;
            // get the showpos
            // Y×ø±êµ×²¿¶ÔÆë
            unsigned char ucAlpha = (unsigned char)(255 - 255 * (dwTimePass - dwZoomOutTime - pInfo->dwDelay) / dwFadeOutTime);
            CMyPos posView;
            posView.x = posShow.x - strlen(szNum) * sizeNum.iWidth / 2;
            posView.y = posShow.y - sizeNum.iHeight;
            int nOffsetY = nMaxOffsetY + nMaxOffsetY_C * (dwTimePass - dwZoomOutTime - pInfo->dwDelay) / dwFadeOutTime;
            int nOffsetX = nMaxOffsetX_C * (dwTimePass - dwZoomOutTime - pInfo->dwDelay) / dwFadeOutTime;
            // show
            CAni* pAni = g_objGameDataSet.GetDataAni((char*)AniFile, (char*)AniTitle);
            if (pAni)
            {
                if (pInfo->nNum == 0)
                {
                    CMyBitmap* pBmp = pAni->GetFrame(10);
                    if (pBmp)
                    {
                        unsigned char ucR, ucG, ucB;
                        ucR =(unsigned char)( (pInfo->dwColor & 0x00ff0000) >> 16);
                        ucG = (unsigned char)( (pInfo->dwColor & 0x0000ff00) >> 8);
                        ucB = (unsigned char)( pInfo->dwColor & 0x000000ff);
                        pBmp->SetColor(ucAlpha, ucR, ucG, ucB);
                    }
                    pAni->ShowEx(10, posView.x + sizeNum.iWidth + pInfo->nOffsetX + nOffsetX,
                                 posView.y - nOffsetY, NULL, sizeNum.iWidth * 4, sizeNum.iHeight * 4, 0);
                }
                else
                {
                    for(int j = 0; j < (int)strlen(szNum); j++)
                    {
                        int nFrame = szNum[j] - '0';
                        CMyBitmap* pBmp = pAni->GetFrame(nFrame);
                        unsigned char ucR, ucG, ucB;
                        ucR = (unsigned char)((pInfo->dwColor & 0x00ff0000) >> 16);
                        ucG = (unsigned char)((pInfo->dwColor & 0x0000ff00) >> 8);
                        ucB =(unsigned char)( pInfo->dwColor & 0x000000ff);
                        pBmp->SetColor(ucAlpha, ucR, ucG, ucB);
                        pAni->ShowEx(nFrame, posView.x + sizeNum.iWidth * j + pInfo->nOffsetX + nOffsetX,
                                     posView.y - nOffsetY, NULL, sizeNum.iWidth, sizeNum.iHeight, 0);
                    }
                }
            }
        }
    }
    //judge...
    for(i = nAmount - 1; i >= 0; i --)
    {
        ZoomNumInfo* pInfo = m_setInfo[i];
        MYASSERT (pInfo);
        {
            DWORD dwTimePass = dwCurrentTime - pInfo->dwTimeBegin;
            if (dwTimePass >= dwZoomOutTime + dwFadeOutTime + pInfo->dwDelay)
            {
                // delete ...
                SAFE_DELETE(pInfo);
                m_setInfo.erase(m_setInfo.begin() + i);
            }
        }
    }
}
//------------------------------------------------------------------
