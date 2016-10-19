
//**********************************************************
// 代码编辑器
//**********************************************************

//-----------------------------------------------------------------
//sayhello.cpp
//include head files
//-----------------------------------------------------------------
#include "sayhello.h"
#include "Gamedataset.h"
//-----------------------------------------------------------------
const int _SAY_HELLO_SHOW_TIME = 6000;	// show words for 6s time

//-----------------------------------------------------------------
CSayHello::CSayHello()
{
}

CSayHello::~CSayHello()
{
}

//-----------------------------------------------------------------
void CSayHello::SetWords(const char* pszWords)
{
    if (!pszWords)
    {
        return;
    }
    if (strlen(pszWords) >= 128)
    {
        return;
    }
    m_dwTimeStart	=::TimeGet();
    strcpy(m_szWords, pszWords);
}

//-----------------------------------------------------------------
void CSayHello::ShowWords(int iScrX, int iScrY, int iDir)
{
    if (strlen(m_szWords) == 0)
    {
        return;
    }
    if ( ::TimeGet() - m_dwTimeStart >= _SAY_HELLO_SHOW_TIME )
    {
        return;
    }
    iDir	= ( iDir + 8 - 1 ) % 8;
    if ( iDir < 4 )
    {
        iDir	= 0;
    }
    else
    {
        iDir	= 1;
    }
    m_iX = iScrX;
    m_iY = iScrY;
    m_iDir = iDir;
    CMyBitmap::GetFontSize(m_objFontSize);
    DrawHelloBox();
    ShowHelloWords();
}
//-----------------------------------------------------------------
void CSayHello::DrawHelloBox()
{
    char szAniFile[] = "ani/common.ani";
    char szDialog[] = "Dialog";
    CAni* pAni = g_objGameDataSet.GetDataAni(szAniFile, szDialog, EXIGENCE_IMMEDIATE);
    if (!pAni)
    {
        return;
    }
    if (m_iDir == 0)
    {
        pAni->Show(0, m_iX, m_iY - 200);
    }
    else
    {
        pAni->Show(1, m_iX - 170, m_iY - 200);
    }
}
//-----------------------------------------------------------------
void CSayHello::ShowHelloWords()
{
    // 根据图片位置定文字的位置
    CMyPos posPic;
    if (m_iDir == 0)
    {
        posPic.x = m_iX + 12;
        posPic.y = m_iY - 200 + 8;
    }
    else
    {
        posPic.x = m_iX - 170 + 12;
        posPic.y = m_iY - 200 + 8;
    }
    // check words
    if ( strlen( m_szWords ) <= 0 )
    {
        return;
    }
    const int _COLOR_WORDS = _COLOR_YELLOW;
    //int iCounter,
    int iWordsCounter, iTotalCounter ;
    iWordsCounter = 0;
    iTotalCounter = 0;
    int iHeight, iWide;
    char szAniFile[] = "ani/common.ani";
    char szDialog[] = "Dialog";
    CAni* pAni = g_objGameDataSet.GetDataAni(szAniFile, szDialog, EXIGENCE_IMMEDIATE);
    if (!pAni)
    {
        return;
    }
    CMyBitmap* pBmp = pAni->GetFrame(0);
    if (!pBmp)
    {
        return;
    }
    const CMySize sizeBmp = {160, 102};
    int nMaxLine = (sizeBmp.iHeight - 48) / (m_objFontSize.iHeight + 6);
    int nMaxWord = (sizeBmp.iWidth - 24) / (m_objFontSize.iWidth + 1);
    iHeight = m_objFontSize.iHeight * nMaxLine;
    iWide = m_objFontSize.iWidth * nMaxWord + 2;
    unsigned char ucTemp;
    char cETemp[2], cCTemp[3] , cFTemp[4];
    while(1)
    {
        // 判断能不能写的下去
        if (nMaxLine < iTotalCounter / nMaxWord)
        {
            break;
        }
        if ((iWordsCounter) > (int)strlen(m_szWords))
        {
            break;
        }
        ucTemp = (unsigned char)m_szWords[iWordsCounter];
        if (ucTemp < 0x80)	//英文
        {
            if (ucTemp == '#' && iWordsCounter + 2 <= (int)strlen (m_szWords)
                    && (m_szWords[iWordsCounter+1] >= '0' && m_szWords[iWordsCounter+1] <= '9')
                    && (m_szWords[iWordsCounter+2] >= '0' && m_szWords[iWordsCounter+2] <= '9') )
            {
                if ((iTotalCounter % nMaxWord) == (nMaxWord - 1))
                {
                    iWordsCounter -= 3;
                    iTotalCounter --;
                    cFTemp[0] = ' ';
                    cFTemp[1] = ' ';
                    cFTemp[2] = ' ';
                    cFTemp[3] = '\0';
                }
                else
                {
                    cFTemp[0] = m_szWords[iWordsCounter];
                    cFTemp[1] = m_szWords[iWordsCounter + 1];
                    cFTemp[2] = m_szWords[iWordsCounter + 2];
                    cFTemp[3] = '\0';
                }
                if (m_iDir == 0)
                    CMyBitmap::ShowEmotionString(posPic.x  + iTotalCounter % nMaxWord * (m_objFontSize.iWidth),
                                                 posPic.y + iTotalCounter / nMaxWord * (m_objFontSize.iHeight + 4),
                                                 _COLOR_WORDS, cFTemp, m_dwTimeStart);
                else
                    CMyBitmap::ShowEmotionString(posPic.x  + iTotalCounter % nMaxWord * (m_objFontSize.iWidth),
                                                 posPic.y + iTotalCounter / nMaxWord * (m_objFontSize.iHeight + 4),
                                                 _COLOR_WORDS, cFTemp, m_dwTimeStart);
                iWordsCounter += 3;
                iTotalCounter += 3;
            }
            else
            {
                cETemp[0] = m_szWords[iWordsCounter];
                cETemp[1] = '\0';
                if (m_iDir == 0)
                    CMyBitmap::ShowString(posPic.x  + iTotalCounter % nMaxWord * m_objFontSize.iWidth,
                                          posPic.y + iTotalCounter / nMaxWord * (m_objFontSize.iHeight + 4),
                                          _COLOR_WORDS, cETemp);
                else
                    CMyBitmap::ShowString(posPic.x  + iTotalCounter % nMaxWord * m_objFontSize.iWidth,
                                          posPic.y + iTotalCounter / nMaxWord * (m_objFontSize.iHeight + 4),
                                          _COLOR_WORDS, cETemp);
                iWordsCounter ++;
                iTotalCounter ++;
            }
        }
        else //汉字
        {
            if ((iTotalCounter % nMaxWord) == (nMaxWord - 1))
            {
                iWordsCounter -= 2;
                iTotalCounter --;
                cCTemp[0] = ' ';
                cCTemp[1] = ' ';
                cCTemp[2] = '\0';
            }
            else
            {
                cCTemp[0] = m_szWords[iWordsCounter];
                cCTemp[1] = m_szWords[iWordsCounter + 1];
                cCTemp[2] = '\0';
            }
            if (m_iDir == 0)
                CMyBitmap::ShowString(posPic.x  + iTotalCounter % nMaxWord * m_objFontSize.iWidth,
                                      posPic.y + iTotalCounter / nMaxWord * (m_objFontSize.iHeight + 4),
                                      _COLOR_WORDS, cCTemp);
            else
                CMyBitmap::ShowString(posPic.x  + iTotalCounter % nMaxWord * m_objFontSize.iWidth,
                                      posPic.y + iTotalCounter / nMaxWord * (m_objFontSize.iHeight + 4),
                                      _COLOR_WORDS, cCTemp);
            iWordsCounter += 2;
            iTotalCounter += 2;
        }
    }
}
//-----------------------------------------------------------------
void CSayHello::Clear()
{
    strcpy(m_szWords, "");
}
//-----------------------------------------------------------------



