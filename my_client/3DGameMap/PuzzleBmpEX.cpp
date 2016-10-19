
//**********************************************************
// 代码编辑器
//**********************************************************

// PuzzleBmpEX.cpp: implementation of the CPuzzleBmpEX class.
//
//////////////////////////////////////////////////////////////////////

#include "PuzzleBmpEX.h"
#include "3DGameMap.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPuzzleBmpEX::CPuzzleBmpEX()
{
    m_nLayerCount = 0;
    m_pdwDiffuse = NULL;
    m_nVetexAmount = 0;
}

CPuzzleBmpEX::~CPuzzleBmpEX()
{
    Destroy();
}
BOOL CPuzzleBmpEX::Load(char* pszFile)
{
    if (!pszFile)
    {
        return false;
    }
    FILE* fp = fopen(pszFile, "rb");
    if (!fp)
    {
        return false;
    }
    // header..(8 bytes)
    char szHeader[8] = "";
    fread(szHeader, 1, 8, fp);
    // check header ...
    if (strcmp(szHeader, "PUZZLE3") != 0)
    {
        return FALSE;
    }
    // AniFile.. (256 bytes)
    fread(m_szAniFile, 1, 256, fp);
    // Size...
    fread(&m_infoSize.iWidth, sizeof(UINT), 1, fp);
    fread(&m_infoSize.iHeight, sizeof(UINT), 1, fp);
    fread(&m_nLayerCount, sizeof(UINT), 1, fp);
    SetSize(m_infoSize, m_nLayerCount);
    // Grid...
    fread(m_usPuzzleIndex, sizeof(unsigned short), m_nPuzzleAmount * m_nLayerCount, fp);
    fread(m_pdwDiffuse, sizeof(DWORD), m_nVetexAmount, fp);
    fread(&m_nRollSpeedX, sizeof(int), 1, fp);
    fread(&m_nRollSpeedY, sizeof(int), 1, fp);
    fclose(fp);
    return true;
}
//------------------------------------------------
void CPuzzleBmpEX::Save(char* pszFile)
{
    if (!pszFile)
    {
        return;
    }
    FILE* fp = fopen(pszFile, "wb");
    if (!fp)
    {
        return;
    }
    // header..(8 bytes)
    char szHeader[8] = "PUZZLE3";
    fwrite(szHeader, 1, 8, fp);
    // AniFile.. (256 bytes)
    char szAni[256];
    strcpy(szAni, this->GetAniFile());
    fwrite(szAni, 1, 256, fp);
    // Size
    CMySize infoSize;
    GetSizeByGride(infoSize);
    fwrite(&infoSize.iWidth, sizeof(UINT), 1, fp);
    fwrite(&infoSize.iHeight, sizeof(UINT), 1, fp);
    fwrite(&m_nLayerCount, sizeof(UINT), 1, fp);
    // Grids
    fwrite(m_usPuzzleIndex, sizeof(unsigned short), m_nPuzzleAmount * m_nLayerCount, fp);
    fwrite(m_pdwDiffuse, sizeof(DWORD), m_nVetexAmount, fp);
    fwrite(&m_nRollSpeedX, sizeof(int), 1, fp);
    fwrite(&m_nRollSpeedY, sizeof(int), 1, fp);
    fclose(fp);
    return;
}
//------------------------------------------------
void CPuzzleBmpEX::SetSize(CMySize infoSize, int nLayer)
{
    this->Destroy();
    m_nPuzzleAmount = infoSize.iWidth * infoSize.iHeight;
    m_nVetexAmount  = (infoSize.iWidth + 1) * (infoSize.iHeight + 1);
    m_infoSize = infoSize;
    m_nLayerCount = nLayer;
    int TotalAmount = nLayer * m_nPuzzleAmount;
    m_usPuzzleIndex = new USHORT[TotalAmount];
    for(int i = 0; i < TotalAmount; i++)
    {
        unsigned short* pusIndex = m_usPuzzleIndex + i;
        *pusIndex = 65535;
    }
    m_pdwDiffuse = new DWORD[m_nVetexAmount];
    for(int i = 0; i < m_nVetexAmount; i++)
    {
        m_pdwDiffuse[i] = 0x00FFFFFF;
    }
}
void CPuzzleBmpEX::Destroy()
{
    CPuzzleBmp::Destroy();
    if (m_pdwDiffuse)
    {
        delete m_pdwDiffuse;
    }
}
void CPuzzleBmpEX::Show(int iPosX, int iPosY, BOOL bGrid, BOOL bPos)
{
    DWORD dwFrame = (::TimeGet() - m_dwTimer) / 160;
    CMySize sizeBmp;
    this->GetSize(sizeBmp);
    BOOL bRollBack = true;
    if ((m_nRollSpeedX == 0) &&
            (m_nRollSpeedY == 0))
    {
        bRollBack = false;
    }
    if (!m_bRoll)
    {
        bRollBack = false;
    }
    this->PreLoad(iPosX, iPosY);
    CMyPos posCell;
    posCell.x = -iPosX / m_nGrideSize;
    posCell.y = -iPosY / m_nGrideSize;
    if (iPosX > 0)
    {
        posCell.x --;
    }
    if (iPosY > 0)
    {
        posCell.y --;
    }
    CMySize infoSize = m_infoSize;
    int nRealGridrSize = m_nGrideSize * g_objGameMap.GetScale() / _NORMAL_SCALE;
    int nAddPuzzleX = PUZZLE_SCRX / m_nGrideSize - PUZZLE_SCRX / nRealGridrSize;
    int nAddPuzzleY = PUZZLE_SCRY / m_nGrideSize - PUZZLE_SCRY / nRealGridrSize;
    int nAmountX = PUZZLE_SCRX / nRealGridrSize + 3;
    int nAmountY = PUZZLE_SCRY / nRealGridrSize + 3;
    CMyPos posView;
    posView.x  =  iPosX + posCell.x * m_nGrideSize;
    posView.y  =  iPosY + posCell.y * m_nGrideSize;
    if (nRealGridrSize > m_nGrideSize)
    {
        nAmountX += nAddPuzzleX;
        nAmountY += nAddPuzzleY;
        nAddPuzzleX = nAddPuzzleY = 0;
    }
    if (nRealGridrSize < m_nGrideSize)
    {
        nAmountX += nAddPuzzleX + 1;
        nAmountY += nAddPuzzleY + 1;
        nAddPuzzleX = nAddPuzzleY = -1;
    }
    for(int i = nAddPuzzleY; i < nAmountY; i++)
    {
        for(int j = nAddPuzzleX; j < nAmountX; j++)
        {
            CMyPos posCellShow;
            posCellShow.x = posCell.x + j;
            posCellShow.y = posCell.y + i;
            if (posCellShow.x < 0)
            {
                if (!bRollBack)
                {
                    continue;
                }
                else
                {
                    posCellShow.x += infoSize.iWidth;
                }
            }
            if (posCellShow.y < 0)
            {
                if (!bRollBack)
                {
                    continue;
                }
                else
                {
                    posCellShow.y += infoSize.iHeight;
                }
            }
            if (posCellShow.x >= infoSize.iWidth)
            {
                if (!bRollBack)
                {
                    continue;
                }
                else
                {
                    posCellShow.x -= infoSize.iWidth;
                }
            }
            if (posCellShow.y >= infoSize.iHeight)
            {
                if (!bRollBack)
                {
                    continue;
                }
                else
                {
                    posCellShow.y -= infoSize.iHeight;
                }
            }
            CMyPos posShow;
            posShow.x = posView.x + j * m_nGrideSize;
            posShow.y = posView.y + i * m_nGrideSize;
            g_objGameMap.MapScaleShowPos(posShow.x, posShow.y);
            DWORD	dwDiffuse[4];
            dwDiffuse[0] = GetDiffuse(posCellShow.x, posCellShow.y);
            dwDiffuse[1] = GetDiffuse(posCellShow.x + 1, posCellShow.y);
            dwDiffuse[2] = GetDiffuse(posCellShow.x, posCellShow.y + 1);
            dwDiffuse[3] = GetDiffuse(posCellShow.x + 1, posCellShow.y + 1);
            CMyBitmap* pBmp[2];
            for(int k = 0; k < 2; k++)
            {
                pBmp[k] = NULL;
                int	nIndex = GetGrid(posCellShow.x, posCellShow.y, k);
                if (nIndex == 65535)
                {
                    // 透明
                    if (bGrid)
                    {
                        CMyBitmap::ShowRect(posShow.x, posShow.y,
                                            posShow.x + m_nGrideSize, posShow.y + m_nGrideSize, 0xffffff00);
                    }
                    if (bPos)
                    {
                        char szPos[64];
                        sprintf(szPos, "【%d,%d】", posCellShow.x, posCellShow.y);
                        CMySize sizeFont;
                        CMyBitmap::GetFontSize(sizeFont);
                        CMyBitmap::ShowString(posShow.x, posShow.y, 0xffffff, szPos);
                    }
                    if (k != 0)
                    {
                        if (pBmp[0])
                        {
                            pBmp[0]->ShowEx(posShow.x, posShow.y, NULL, nRealGridrSize, nRealGridrSize, 0xFF);
                        }
                    }
                }
                else
                {
                    char szTitle[64];
                    sprintf(szTitle, "Puzzle%d", nIndex);
                    CAni* pAni = g_objGameDataSet.GetDataAni(m_szAniFile, szTitle, EXIGENCE_IMMEDIATE, g_objGameDataSet.m_dwPuzzleLife);
                    if (pAni)
                    {
                        pBmp[k] = pAni->GetFrame(0);
                        if (pBmp[k])
                        {
                            if (k != 0)
                            {
                                pBmp[k]->SetVertexColor(dwDiffuse[0], dwDiffuse[1], dwDiffuse[2], dwDiffuse[3]);
                                DWORD Alpha[4];
                                for(int l = 0; l < 4; l++)
                                {
                                    Alpha[l] = (dwDiffuse[l] >> 24);
                                }
                                if (pBmp[0])
                                {
                                    CMyBitmap::Show(pBmp[k], pBmp[0], (UCHAR)Alpha[0],(UCHAR) Alpha[1], (UCHAR)Alpha[2],(UCHAR) Alpha[3], posShow.x, posShow.y, NULL, nRealGridrSize, nRealGridrSize);
                                }
                                else
                                {
                                    pBmp[k]->Show(posShow.x, posShow.y, 0xFF);
                                }
                            }
                        }
                        else
                        {
                            if (k != 0)
                            {
                                if (pBmp[0])
                                {
                                    pBmp[0]->ShowEx(posShow.x, posShow.y, NULL, nRealGridrSize, nRealGridrSize, 0xFF);
                                }
                            }
                        }
                        if (m_bTerrain)
                        {
                            CMyRect rect;
                            rect.l = posShow.x;
                            rect.t = posShow.y;
                            rect.r = posShow.x + nRealGridrSize;
                            rect.b = posShow.y + nRealGridrSize;
                            pAni->ShowEx(dwFrame, posShow.x, posShow.y, NULL, nRealGridrSize, nRealGridrSize);
                        }
                        if (bGrid)
                        {
                            CMyBitmap::ShowRect(posShow.x, posShow.y,
                                                posShow.x + m_nGrideSize, posShow.y + nRealGridrSize, 0xffffff00);
                        }
                        if (bPos)
                        {
                            char szPos[64];
                            sprintf(szPos, "【%d,%d,%d】", posCellShow.x, posCellShow.y, nIndex);
                            CMySize sizeFont;
                            CMyBitmap::GetFontSize(sizeFont);
                            CMyBitmap::ShowString(posShow.x, posShow.y, 0xffffff00, szPos);
                        }
                    }
                    else
                    {
                        // 无图片资源
                    }
                }
            }
        }
    }
}

int	 CPuzzleBmpEX::GetGrid(int iPosX, int iPosY, int iLayer)
{
    unsigned short usIndex;
    if ((iPosX < 0) || (iPosY < 0) || (iPosX >= m_infoSize.iWidth) || ((iPosY >= m_infoSize.iHeight)))
    {
        return 65535;
    }
    if ((iPosY * m_infoSize.iWidth + iPosX) >= m_nPuzzleAmount)
    {
        return 65535;
    }
    unsigned short* pusIndex = m_usPuzzleIndex + iPosY * m_infoSize.iWidth + iPosX + iLayer * m_nPuzzleAmount;
    usIndex = *pusIndex;
    return  usIndex;
}
int	 CPuzzleBmpEX::GetGrid(CMyPos myPos, int iLayer)
{
    CMyPos posGrid;
    posGrid.x = -myPos.x / m_nGrideSize;
    posGrid.y = -myPos.y / m_nGrideSize;
    return GetGrid(posGrid.x, posGrid.y, iLayer);
}

DWORD   CPuzzleBmpEX::GetDiffuse(int iPosX, int iPosY)
{
    if ((iPosX < 0) || (iPosY < 0) || (iPosX >= m_infoSize.iWidth) || ((iPosY >= m_infoSize.iHeight)))
    {
        return 0;
    }
    if ((iPosY * m_infoSize.iWidth + iPosX) >= m_nPuzzleAmount)
    {
        return 0;
    }
    DWORD* pDiffuse = m_pdwDiffuse + iPosY * (m_infoSize.iWidth + 1) + iPosX;
    return *pDiffuse;
}
void CPuzzleBmpEX::PreLoad(int iPosX, int iPosY)
{
    // 计算Cell坐标
    CMyPos posCell;
    posCell.x = -iPosX / m_nGrideSize;
    posCell.y = -iPosY / m_nGrideSize;
    CMyPos posView;
    posView.x  =  iPosX + posCell.x * m_nGrideSize;
    posView.y  =  iPosY + posCell.y * m_nGrideSize;
    // 求显示范围
    int nAmountX = PUZZLE_SCRX / m_nGrideSize + 2;
    int nAmountY = PUZZLE_SCRY / m_nGrideSize + 2;
    int nPreLoadSize =  g_objGameDataSet.m_dwPuzzlePreloadSize;
    // 开始显示
    for(int i = 0 - nPreLoadSize; i < nAmountY + nPreLoadSize; i++)
    {
        for(int j = 0 - nPreLoadSize; j < nAmountX + nPreLoadSize; j++)
        {
            CMyPos posCellShow;
            // 求要加载的Cell
            posCellShow.x = posCell.x + j;
            posCellShow.y = posCell.y + i;
            // 剔除空Cell
            if (posCellShow.x < 0)
            {
                continue;
            }
            if (posCellShow.y < 0)
            {
                continue;
            }
            CMySize infoSize = m_infoSize;
            if (posCellShow.x >= infoSize.iWidth)
            {
                continue;
            }
            if (posCellShow.y >= infoSize.iHeight)
            {
                continue;
            }
            // 剔除非法Cell
            int nDequeIndex = posCellShow.y * m_infoSize.iWidth + posCellShow.x;
            if (nDequeIndex >= m_nPuzzleAmount)
            {
                continue;
            }
            if (nDequeIndex < 0)
            {
                continue;
            }
            for(int i = 0; i < m_nLayerCount; i++)
            {
                unsigned short* pusIndex = m_usPuzzleIndex + nDequeIndex + i * m_nPuzzleAmount;
                int nIndex = (int) * pusIndex;
                if (nIndex == 65535)
                {
                    // 透明, 不加载
                }
                else
                {
                    // PreLoad
                    char szTitle[64];
                    sprintf(szTitle, "Puzzle%d", nIndex);
                    g_objGameDataSet.GetDataAni(m_szAniFile, szTitle, EXIGENCE_NORMAL, g_objGameDataSet.m_dwPuzzleLife);
                }
            }
        }
    }
}

void CPuzzleBmpEX::SetGrid(int iPosX, int iPosY, unsigned short usIndex, int iLayer)
{
    CMyPos posGrid;
    posGrid.x = -iPosX / m_nGrideSize;
    posGrid.y = -iPosY / m_nGrideSize;
    unsigned short* pusIndex = m_usPuzzleIndex + posGrid.y * m_infoSize.iWidth + posGrid.x + iLayer * m_nPuzzleAmount;
    *pusIndex = usIndex;
}
void CPuzzleBmpEX::SetDiffuse(int iPosX, int iPosY, DWORD dwDiffuse[4])
{
    iPosX = -iPosX / m_nGrideSize;
    iPosY = -iPosY / m_nGrideSize;
    if ((iPosX < 0) || (iPosY < 0) || (iPosX >= m_infoSize.iWidth) || ((iPosY >= m_infoSize.iHeight)))
    {
        return ;
    }
    if ((iPosY * m_infoSize.iWidth + iPosX) >= m_nPuzzleAmount)
    {
        return ;
    }
    DWORD* pDiffuse = m_pdwDiffuse + iPosY * (m_infoSize.iWidth + 1) + iPosX;
    *pDiffuse = dwDiffuse[0];
    pDiffuse = m_pdwDiffuse + iPosY * (m_infoSize.iWidth + 1) + iPosX + 1;
    *pDiffuse = dwDiffuse[1];
    pDiffuse = m_pdwDiffuse + (iPosY + 1) * (m_infoSize.iWidth + 1) + iPosX;
    *pDiffuse = dwDiffuse[2];
    pDiffuse = m_pdwDiffuse + (iPosY + 1) * (m_infoSize.iWidth + 1) + iPosX + 1;
    *pDiffuse = dwDiffuse[3];
}
