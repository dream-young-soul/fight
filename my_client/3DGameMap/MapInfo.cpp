
//**********************************************************
// 代码编辑器
//**********************************************************

//-------------------------------------------------------------
#include "3DGameMap.h"
#include "GamedataSet.h"
#include <map>
#include <string>
//-------------------------------------------------------------
//#define _ARHUN_DEBUG_EDITOR
const int _REGIONNAME_POSY = 130;
const int _REGIONDES_POSX = 930;
const int _REGIONDES_POSY = 180;
const char _REGION_FONT[] = "华文行楷";

const DWORD _REGION_COLOR[] = {0X00DB7E5B, 0X009C48A9, 0X00B42BB4, 0X0033419B, 0X004CBACA, 0X0090D158, 0X003B504D,  0X00DBD4C7};

//-------------------------------------------------------------
void CGameMap::ShowCellRange()
{
    CMyPos posT, posB, posL, posR;
    posT.x = m_szieWorld.iWidth / 2;
    posT.y = 0;
    posB.x = m_szieWorld.iWidth / 2;
    posB.y = m_szieWorld.iHeight;
    posL.x = 0;
    posL.y = m_szieWorld.iHeight / 2;
    posR.x = m_szieWorld.iWidth;
    posR.y = m_szieWorld.iHeight / 2;
    CMyPos posST, posSB, posSL, posSR;
    this->World2Screen(posT.x, posT.y, posST.x, posST.y);
    this->World2Screen(posB.x, posB.y, posSB.x, posSB.y);
    this->World2Screen(posL.x, posL.y, posSL.x, posSL.y);
    this->World2Screen(posR.x, posR.y, posSR.x, posSR.y);
    CMyBitmap::ShowLine(posST.x, posST.y, posSL.x, posSL.y, 0xffffffff);
    CMyBitmap::ShowLine(posST.x, posST.y, posSR.x, posSR.y, 0xffffffff);
    CMyBitmap::ShowLine(posSB.x, posSB.y, posSL.x, posSL.y, 0xffffffff);
    CMyBitmap::ShowLine(posSB.x, posSB.y, posSR.x, posSR.y, 0xffffffff);
}
//-------------------------------------------------------------
void CGameMap::ShowCell(BOOL bGride, BOOL bMask, BOOL bArea, BOOL bAltitude)
{
    CMyPos posShow;
    this->Screen2Cell(0, 0, posShow.x, posShow.y);
    int nCellHeight = _CELL_HEIGHT * this->GetScale() / _NORMAL_SCALE;
    int nCellWidth = _CELL_WIDTH * this->GetScale() / _NORMAL_SCALE;
    for(int i = posShow.x - 12 * _NORMAL_SCALE / this->GetScale() ; i < posShow.x + 31 * _NORMAL_SCALE / this->GetScale(); i++)
    {
        for(int j = posShow.y - 12 * _NORMAL_SCALE / this->GetScale(); j < posShow.y + 31 * _NORMAL_SCALE / this->GetScale(); j++)
        {
            if (i < 0 || j < 0)
            {
                continue;
            }
            if (i >= m_sizeMap.iWidth && j >= m_sizeMap.iHeight)
            {
                continue;
            }
            CellInfo* pCellInfo = this->GetCell(i, j);
            if (!pCellInfo)
            {
                continue;
            }
            LayerInfo* pLayerInfo = this->GetLastLayerInfo(pCellInfo);
            if (pLayerInfo)
            {
                CMyPos posShow;
                CMyPos posWorld;
                this->Cell2World(i, j, posWorld.x, posWorld.y);
                this->World2Screen(posWorld.x, posWorld.y, posShow.x, posShow.y);
                this->MapScaleShowPos(posShow.x, posShow.y);
                if ((posShow.x >= -32) &&
                        (posShow.y >= -16) &&
                        (posShow.x <= 800 * _NORMAL_SCALE / this->GetScale() + 32) &&
                        (posShow.y <= 600 * _NORMAL_SCALE / this->GetScale() + 16))
                {
                    BOOL bShow = false;
                    if (bMask && pLayerInfo->usMask != 0)
                    {
                        CMyBitmap::ShowLine(posShow.x, posShow.y - nCellHeight / 2, posShow.x, posShow.y + nCellHeight / 2, 0xffff00);
                        CMyBitmap::ShowLine(posShow.x - nCellWidth / 2, posShow.y, posShow.x + nCellWidth / 2, posShow.y, 0xffff00);
                        bShow = true;
                    }
                    if (bArea && pLayerInfo->usTerrain != 0)
                    {
#ifdef _ARHUN_DEBUG_EDITOR
                        extern char g_szTerrianFlag[256][16];
                        CMySize sizeFont;
                        CMyBitmap::GetFontSize(sizeFont);
                        int nLen = strlen(g_szTerrianFlag[pLayerInfo->usTerrain]);
                        CMyBitmap::ShowString(posShow.x - nLen * sizeFont.iWidth / 2, posShow.y - sizeFont.iHeight / 2 - 6,
                                              0xffff00, g_szTerrianFlag[pLayerInfo->usTerrain]);
                        bShow = true;
#endif
                    }
                    if (bAltitude && pLayerInfo->sAltitude != 0)
                    {
                        char szInfo[16];
                        sprintf(szInfo, "%d", pLayerInfo->sAltitude);
                        CMySize sizeFont;
                        CMyBitmap::GetFontSize(sizeFont);
                        int nLen = strlen(szInfo);
                        CMyBitmap::ShowString(posShow.x - nLen * sizeFont.iWidth / 2, posShow.y - sizeFont.iHeight / 2 + 6,
                                              0xffff00, szInfo);
                        bShow = true;
                    }
                    //--------------------------------------------------------------------------//
#ifdef _ARHUN_DEBUG_EDITOR
                    if (m_bShowRes)
                    {
                        char szInfo[32];
                        CellRes* pCellRes = this->GetCellRes (i, j);
                        if (pCellRes)
                        {
                            extern std::map<DWORD, std::string> g_setResMap;
                            std::map<DWORD, std::string>::iterator it;
                            it = g_setResMap.find(pCellRes->pResTmpID);
                            if (it != g_setResMap.end())
                            {
                                sprintf(szInfo, "%s", it->second.c_str());
                                CMySize sizeFont;
                                CMyBitmap::GetFontSize(sizeFont);
                                int nLen = strlen(szInfo);
                                CMyBitmap::ShowString(posShow.x - nLen * sizeFont.iWidth / 2, posShow.y - sizeFont.iHeight / 2 + 2,
                                                      0xff0000, szInfo);
                            }
                        }
                    }
#endif
                    //--------------------------------------------------------------------------//
                    CMyPos posCell = {i, j};
                    int nIndex = g_objGameMap.GetExitIndex(posCell);
                    if (nIndex >= 0)
                    {
                        char szInfo[16];
                        sprintf(szInfo, "E%d", nIndex);
                        CMySize sizeFont;
                        CMyBitmap::GetFontSize(sizeFont);
                        int nLen = strlen(szInfo);
                        CMyBitmap::ShowString(posShow.x - nLen * sizeFont.iWidth / 2, posShow.y - sizeFont.iHeight / 2,
                                              0xffff00, szInfo);
                        bShow = true;
                    }
                    if (bGride || bShow)
                    {
                        CMyBitmap::ShowLine(posShow.x - nCellWidth / 2, posShow.y, posShow.x, posShow.y - nCellHeight / 2, 0x00ff00);
                        CMyBitmap::ShowLine(posShow.x - nCellWidth / 2, posShow.y, posShow.x, posShow.y + nCellHeight / 2, 0x00ff00);
                        CMyBitmap::ShowLine(posShow.x + nCellWidth / 2, posShow.y, posShow.x, posShow.y - nCellHeight / 2, 0x00ff00);
                        CMyBitmap::ShowLine(posShow.x + nCellWidth / 2, posShow.y, posShow.x, posShow.y + nCellHeight / 2, 0x00ff00);
                    }
                }
            }
        }
    }
}
//-------------------------------------------------------------
void CGameMap::InitMask()
{
    if (!m_pCellInfo)
    {
        return;
    }
    // 算出BG的世界坐标范围
    CMyPos posBgS, posBgE;
    this->Bg2World(0, 0, posBgS.x, posBgS.y);
    CMySize sizeBg;
    this->GetBgSize(sizeBg);
    posBgE.x = posBgS.x + sizeBg.iWidth;
    posBgE.y = posBgS.y + sizeBg.iHeight;
    for(int j = 0; j < m_sizeMap.iHeight; j ++)
    {
        for(int i = 0; i < m_sizeMap.iWidth; i ++)
        {
            // 判断Cell中心是否不在BG内
            CMyPos posWorld;
            this->Cell2World(i, j, posWorld.x, posWorld.y);
            if ((posWorld.x < posBgS.x + 32) ||
                    (posWorld.y < posBgS.y + 16) ||
                    (posWorld.x > posBgE.x - 32) ||
                    (posWorld.y > posBgE.y - 16))
            {
                CellInfo* pInfo = m_pCellInfo + m_sizeMap.iWidth * j + i;
                LayerInfo* pLayer = CGameMap::GetLastLayerInfo(pInfo);
                if (pLayer)
                {
                    pLayer->usMask = 1;
                }
            }
        }
    }
}
//-------------------------------------------------------------
void CGameMap::CleanForEditor()
{
    int nAmount = m_setMapLayer.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        CMapLayer* pLayer = m_setMapLayer[i];
        if (pLayer &&
                ((pLayer->GetType() == CMapLayer::LAYER_FLOOR) || (pLayer->GetType() == CMapLayer::LAYER_SKY)))
        {
            SAFE_DELETE(pLayer);
            m_setMapLayer.erase(m_setMapLayer.begin() + i);
        }
    }
}
//-------------------------------------------------------------
void CGameMap::GetMapSizeByPuzzle(CMySize& sizeMap)
{
    CMySize sizeBg;
    this->GetBgSize(sizeBg);
    sizeMap.iWidth = 1;
    sizeMap.iHeight = 1;
    while(true)
    {
        m_sizeMap.iWidth        = sizeMap.iWidth;
        m_sizeMap.iHeight       = sizeMap.iHeight;
        m_posOrigin.x	= _CELL_WIDTH * m_sizeMap.iWidth / 2;
        m_posOrigin.y	= _CELL_HEIGHT / 2;
        CMyPos posCenter;
        posCenter.x         = m_sizeMap.iWidth * _CELL_WIDTH / 2;
        posCenter.y         = m_sizeMap.iHeight * _CELL_HEIGHT / 2;
        CMyPos posRD;
        posRD.x             = (m_sizeMap.iWidth * _CELL_WIDTH + sizeBg.iWidth) / 2;
        posRD.y             = (m_sizeMap.iHeight * _CELL_HEIGHT + sizeBg.iHeight) / 2;
        CMyPos posCell;
        this->World2Cell(posRD.x, posRD.y, posCell.x, posCell.y);
        if ((posCell.x <= m_sizeMap.iWidth) &&
                (posCell.y <= m_sizeMap.iHeight))
        {
            sizeMap.iWidth ++;
            sizeMap.iHeight ++;
            return;
        }
        sizeMap.iWidth ++;
        sizeMap.iHeight ++;
    }
}
//-------------------------------------------------------------
void CGameMap::CreateMapByPuzzle(char* pszPuzzleFile)
{
    if (!pszPuzzleFile)
    {
        return;
    }
    this->Reset();
    this->DestroyCell();
    this->CleanForEditor();
    CTerrainLayer* pLayer = this->GetTerrainLayer();
    if (pLayer)
    {
        pLayer->LoadPuzzle(pszPuzzleFile);
    }
    CMySize infoSize = {0, 0};
    this->GetMapSizeByPuzzle(infoSize);
    this->SetMapSize(infoSize);
}
//-------------------------------------------------------------
void CGameMap::ProcessRegion(CMyPos posCell)
{
    CTerrainLayer* pLayer = this->GetTerrainLayer();
    if (pLayer)
    {
        int nAmount = pLayer->GetRegionAmount();
        int i = 0;
        for(; i < nAmount; i ++)
        {
            DWORD dwTimeNow = ::TimeGet();
            C2DMapRegion* pRegion = pLayer->GetRegionByINdex(i);
            if (pRegion)
            {
                RegionInfo* pInfo =  pRegion->GetInfo();
                if (pInfo->m_dwType == REGION_DESC &&
                        posCell.x >= pInfo->m_posCell.x &&
                        posCell.y >= pInfo->m_posCell.y &&
                        posCell.x <= (int)(pInfo->m_posCell.x + pInfo->m_dwCx) &&
                        posCell.y <= (int)(pInfo->m_posCell.y + pInfo->m_dwCy) &&
                        (!pInfo->m_bAccess || dwTimeNow - pInfo->m_dwAccessTime > 60 * 10 * 1000))
                {
                    if (strcmp(m_szRegionName, pInfo->m_szString[1]) == 0)
                    {
                        break;
                    }
                    else
                    {
                        if (!m_bRegionNameChangeCount)
                        {
                            m_bRegionNameChangeCount = true;
                            m_dwRegionNameChangeBegin = dwTimeNow;
                            pInfo->m_dwAccessTime = dwTimeNow;
                            break;
                        }
                        else
                        {
                            if (::TimeGet() - m_dwRegionNameChangeBegin > 3000)
                            {
                                strcpy(m_szRegionEffect, pInfo->m_szString[0]);
                                strcpy(m_szRegionName, pInfo->m_szString[1]);
                                strcpy(m_szRegionDes, pInfo->m_szString[2]);
                                m_dwNameColor = _REGION_COLOR[pInfo->m_nNColor];
                                m_dwDesColor = _REGION_COLOR[pInfo->m_nDColor];
                                m_RegionNShowType = pInfo->m_nNShowType;
                                m_RegionDShowType = pInfo->m_nDShowType;
                                m_RegionDPos	= pInfo->m_nDShowPos;
                                m_dwRegionEffectBegin = dwTimeNow;
                                m_bShowRegionEffect = true;
                                m_bRegionNameChangeCount = false;
                                pInfo->m_bAccess = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (i >= nAmount)
        {
            if (strcmp(m_szRegionName, "") != 0)
            {
                if (!m_bRegionNameChangeCount)
                {
                    m_bRegionNameChangeCount = true;
                    m_dwRegionNameChangeBegin = ::TimeGet();
                }
                else
                {
                    if (::TimeGet() - m_dwRegionNameChangeBegin > 3000)
                    {
                        strcpy(m_szRegionName, "");
                        m_bRegionNameChangeCount = false;
                    }
                }
            }
        }
        else
        {
            m_bShowGobalRegionEffect = false;
        }
        if (m_bShowGobalRegionEffect)
        {
            m_bShowGobalRegionEffect = false;
            int nAmount = pLayer->GetRegionAmount();
            for(int i = 0; i < nAmount; i ++)
            {
                C2DMapRegion* pRegion = pLayer->GetRegionByINdex(i);
                if (pRegion)
                {
                    RegionInfo* pInfo =  pRegion->GetInfo();
                    if (pInfo->m_dwType == REGION_GOBALDESC)
                    {
                        strcpy(m_szRegionEffect, pInfo->m_szString[0]);
                        strcpy(m_szRegionName, pInfo->m_szString[1]);
                        strcpy(m_szRegionDes, pInfo->m_szString[2]);
                        m_dwNameColor = _REGION_COLOR[pInfo->m_nNColor];
                        m_dwDesColor = _REGION_COLOR[pInfo->m_nDColor];
                        m_RegionNShowType = pInfo->m_nNShowType;
                        m_RegionDShowType = pInfo->m_nDShowType;
                        m_RegionDPos	= pInfo->m_nDShowPos;
                        m_dwRegionEffectBegin = ::TimeGet();
                        m_bShowRegionEffect = true;
                        break;
                    }
                }
            }
        }
    }
}
//-------------------------------------------------------------
void CGameMap::ShowRegionName()
{
    CMySize sizeFont = {1, 1};
    CMyBitmap::GetFontSize(sizeFont);
    int nX = _SCR_WIDTH - 60 - strlen(m_szRegionName) * sizeFont.iWidth;
    CMyBitmap::ShowString(nX, 0, 0xffffff00, m_szRegionName);
    CMyPos posBg, posWorld;
    this->Screen2World(_SCR_WIDTH / 2, _SCR_HEIGHT / 2, posWorld.x, posWorld.y);
    this->World2Bg(posWorld.x, posWorld.y, posBg.x, posBg.y);
    m_objRegionEffect.Show(posBg);
    m_objRegionEffect.Process();
    if (m_bShowRegionEffect)
    {
        // show region effect ...
        if (strcmp(m_szRegionEffect, "NULL") == 0)
        {
            BOOL bShowOk = true;
            switch(m_RegionNShowType)
            {
            case 1:
                this->ShowRegionNameFade();
                break;
            case 2:
                this->ShowRegionNameRoll();
                break;
            case 3:
                this->ShowRegionNameWindow();
                break;
            default:
                break;
            }
            switch(m_RegionDShowType)
            {
            case 1:
                bShowOk = !this->ShowRegionDesFade(m_RegionDPos);
                break;
            case 2:
                bShowOk = !this->ShowRegionDesRoll(m_RegionDPos);
                break;
            case 3:
                bShowOk = !this->ShowRegionDesWindow(m_RegionDPos);
                break;
            default:
                break;
            }
            m_bShowRegionEffect = bShowOk;
        }
        else
        {
            m_objRegionEffect.Add(m_szRegionEffect);
            m_bShowRegionEffect = false;
        }
    }
}
//-------------------------------------------------------------
BOOL	CGameMap::ShowRegionNameFade()
{
    const DWORD dwShowDelay = 1000;
    const DWORD dwStayTime = 2000;
    DWORD dwTimePass = ::TimeGet() - m_dwRegionEffectBegin;
    if (dwTimePass > dwShowDelay * 2 + dwStayTime)
    {
        return TRUE;
    }
    else
    {
        DWORD dwAlpha = 0xfd;
        if (dwTimePass < dwShowDelay)
        {
            // fade in ...
            dwAlpha = dwAlpha * dwTimePass / dwShowDelay;
            if (dwAlpha > 0xfd)
            {
                dwAlpha = 0xfd;
            }
        }
        else if (dwTimePass > dwShowDelay + dwStayTime)
        {
            // fade in ...
            dwAlpha = 0xfd - dwAlpha * (dwTimePass - dwShowDelay - dwStayTime) / dwShowDelay;
            if (dwAlpha > 0xfd)
            {
                dwAlpha = 0xfd;
            }
            if (dwAlpha < 0)
            {
                dwAlpha = 0;
            }
        }
        else
        {
            dwAlpha = 0xff;
        }
        const int nNameFontSize = 48;
        CMyPos posStr = {_SCR_WIDTH / 2 - nNameFontSize / 2 * strlen(m_szRegionName) / 2, _REGIONNAME_POSY};
        CMyBitmap::ShowString(posStr.x, posStr.y, m_dwNameColor | (dwAlpha << 24), m_szRegionName, (char*)_REGION_FONT, nNameFontSize, true);
    }
    return FALSE;
}

BOOL	CGameMap::ShowRegionDesFade(BOOL bTline)
{
    const DWORD dwShowDelay = 1000;
    const DWORD dwStayTime = 2000;
    DWORD dwTimePass = ::TimeGet() - m_dwRegionEffectBegin;
    if (dwTimePass > dwShowDelay * 2 + dwStayTime)
    {
        return TRUE;
    }
    else
    {
        DWORD dwAlpha = 0xfd;
        if (dwTimePass < dwShowDelay)
        {
            // fade in ...
            dwAlpha = dwAlpha * dwTimePass / dwShowDelay;
            if (dwAlpha > 0xfd)
            {
                dwAlpha = 0xfd;
            }
        }
        else if (dwTimePass > dwShowDelay + dwStayTime)
        {
            // fade in ...
            dwAlpha = 0xfd - dwAlpha * (dwTimePass - dwShowDelay - dwStayTime) / dwShowDelay;
            if (dwAlpha > 0xfd)
            {
                dwAlpha = 0xfd;
            }
            if (dwAlpha < 0)
            {
                dwAlpha = 0;
            }
        }
        else
        {
            dwAlpha = 0xff;
        }
        const int nFontSize = 24;
        char Static[128] ;
        strcpy(Static, m_szRegionDes);
        if (!Static || strlen(Static) == 0)
        {
            return TRUE;
        }
        int nMaxLen = 32;
        if (bTline)
        {
            nMaxLen = 32;
        }
        else
        {
            nMaxLen = 20;
        }
        int nDescLine = 0 ;
        int nCharIndex[10];
        nCharIndex[0] = 0;
        int nSingle = 0;
        int nLineNum = 0;
       int i = 0;
        for (  ; i < (int)strlen ( Static); i++ )
        {
            if ( CMyBitmap::IsDBCSLeadByte( Static, i ) )
            {
                nLineNum += 2;
                i++;
            }
            else
            {
                nSingle++;
                nLineNum++;
            }
            if ( nLineNum >= nMaxLen )
            {
                //----------Begin the english version's process(jump line)----------//
                if ( strcmp( g_objGameDataSet.GetVersionInfo(), "English" ) == 0 )
                {
                    // Search the last word
                    for ( int j = i; j >= 0; j-- )
                    {
                        if ( Static[j] == '~' )
                        {
                            i -= i - j;
                            break;
                        }
                    }
                }
                //----------End the english version's process(jump line)----------//
                if ( nSingle % 2 == 0 )
                {
                    nSingle = 0;
                    nLineNum = 0;
                    nDescLine++;
                    nCharIndex[nDescLine] = i + 1;
                }
                else
                {
                    nDescLine++;
                    nCharIndex[nDescLine] = i - ( nLineNum - nMaxLen );
                    i -= nLineNum - 2;
                    nSingle = 0;
                    nLineNum = 0;
                }
            }
        }
        if ( nLineNum < nMaxLen && nLineNum != 0 )
        {
            nDescLine++ ;
            nCharIndex[nDescLine] = i;
        }
        char strTemp[32] = "";
        int nOffset = 0;
        for (int i = 0; i < nDescLine; i++ )
        {
            strncpy ( strTemp, Static + nCharIndex[i], nCharIndex[i + 1] - nCharIndex[i] );
            strTemp[nCharIndex[i + 1] - nCharIndex[i]] = '\0';
            if (bTline)
            {
                CMyPos posStr = {_REGIONDES_POSX, _REGIONDES_POSY};
                CMyBitmap::ShowTString (posStr.x - nOffset,
                                        posStr.y ,
                                        m_dwDesColor | (dwAlpha << 24),
                                        strTemp, (char*)_REGION_FONT, nFontSize, true) ;
                nOffset += 26;
            }
            else
            {
                CMyPos posStr = {_SCR_WIDTH / 2 - nFontSize / 2 * strlen(strTemp) / 2, _REGIONNAME_POSY + 50};
                CMyBitmap::ShowString (posStr.x,
                                       posStr.y + nOffset,
                                       m_dwDesColor | (dwAlpha << 24),
                                       strTemp, (char*)_REGION_FONT, nFontSize, true) ;
                nOffset += 26;
            }
        }
    }
    return FALSE;
}

BOOL	CGameMap::ShowRegionNameRoll()
{
    const DWORD dwShowDelay = 1000;
    const DWORD dwStayTime = 2000;
    DWORD dwAlpha = 0xfd;
    const int nFontSize = 48;
    CMyPos posStrE = {_SCR_WIDTH / 2 - nFontSize / 2 * strlen(m_szRegionName) / 2, _REGIONNAME_POSY};
    CMyPos posStrS = {_SCR_WIDTH / 2 + nFontSize / 2 * strlen(m_szRegionName) / 2, _REGIONNAME_POSY};
    DWORD dwTimePass = ::TimeGet() - m_dwRegionEffectBegin;
    int nStrlen = strlen(m_szRegionName);
    int MoveX = posStrS.x - dwTimePass / 45;
    if (MoveX <= posStrE.x)
    {
        MoveX = posStrE.x;
    }
    CMyBitmap::ShowString(MoveX, posStrE.y, m_dwNameColor | (dwAlpha << 24), m_szRegionName , (char*)_REGION_FONT, nFontSize, true);
    return FALSE;
}

BOOL	CGameMap::ShowRegionDesRoll(BOOL bTline)
{
    const DWORD dwShowDelay = 1000;
    const DWORD dwStayTime = 2000;
    DWORD dwAlpha = 0xfd;
    DWORD dwTimePass = ::TimeGet() - m_dwRegionEffectBegin;
    char Static[128] ;
    strcpy(Static, m_szRegionDes);
    if (!Static || strlen(Static) == 0)
    {
        return TRUE;
    }
    int nMaxLen = 32;
    if (bTline)
    {
        nMaxLen = 32;
    }
    else
    {
        nMaxLen = 20;
    }
    int nDescLine = 0 ;
    int nCharIndex[10];
    nCharIndex[0] = 0;
    int nSingle = 0;
    int nLineNum = 0;
   int i = 0;
    for ( ;  i < (int)strlen ( Static); i++ )
    {
        if ( CMyBitmap::IsDBCSLeadByte( Static, i ) )
        {
            nLineNum += 2;
            i++;
        }
        else
        {
            nSingle++;
            nLineNum++;
        }
        if ( nLineNum >= nMaxLen )
        {
            //----------Begin the english version's process(jump line)----------//
            if ( strcmp( g_objGameDataSet.GetVersionInfo(), "English" ) == 0 )
            {
                // Search the last word
                for ( int j = i; j >= 0; j-- )
                {
                    if ( Static[j] == '~' )
                    {
                        i -= i - j;
                        break;
                    }
                }
            }
            //----------End the english version's process(jump line)----------//
            if ( nSingle % 2 == 0 )
            {
                nSingle = 0;
                nLineNum = 0;
                nDescLine++;
                nCharIndex[nDescLine] = i + 1;
            }
            else
            {
                nDescLine++;
                nCharIndex[nDescLine] = i - ( nLineNum - nMaxLen );
                i -= nLineNum - 2;
                nSingle = 0;
                nLineNum = 0;
            }
        }
    }
    if ( nLineNum < nMaxLen && nLineNum != 0 )
    {
        nDescLine++ ;
        nCharIndex[nDescLine] = i;
    }
    char strTemp[64] = "";
    int nOffset = 0;
    const int nFontSize = 24;
    for (int i = 0; i < nDescLine; i++ )
    {
        strncpy ( strTemp, Static + nCharIndex[i], nCharIndex[i + 1] - nCharIndex[i] );
        strTemp[nCharIndex[i + 1] - nCharIndex[i]] = '\0';
        if (bTline)
        {
            CMyPos posStrS = {_REGIONDES_POSX, _REGIONDES_POSY + 400};
            CMyPos posStrE = {_REGIONDES_POSX, _REGIONDES_POSY};
            int nStrlen = strlen(strTemp);
            int MoveY = posStrS.y - dwTimePass / 45;
            if (MoveY <= posStrE.y)
            {
                MoveY = posStrE.y;
                return TRUE;
            }
            CMyBitmap::ShowTString(posStrE.x + nOffset, MoveY, m_dwDesColor | (dwAlpha << 24), strTemp , (char*)_REGION_FONT, nFontSize, true);
            nOffset += 26;
        }
        else
        {
            CMyPos posStrE = {_SCR_WIDTH / 2 - nFontSize / 2 * strlen(strTemp) / 2, _REGIONNAME_POSY + 50};
            CMyPos posStrS = {_SCR_WIDTH / 2 + nFontSize / 2 * strlen(strTemp) / 2, _REGIONNAME_POSY + 50};
            int nStrlen = strlen(strTemp);
            int MoveX = posStrS.x - dwTimePass / 33;
            if (MoveX <= posStrE.x)
            {
                MoveX = posStrE.x;
                return TRUE;
            }
            CMyBitmap::ShowString(MoveX , posStrS.y + nOffset, m_dwDesColor | (dwAlpha << 24), strTemp , (char*)_REGION_FONT, nFontSize, true);
            nOffset += 26;
        }
    }
    return FALSE;
}

BOOL	CGameMap::ShowRegionNameWindow()
{
    const DWORD dwShowDelay = 1000;
    const DWORD dwStayTime = 2000;
    DWORD dwAlpha = 0xfd;
    DWORD dwTimePass = ::TimeGet() - m_dwRegionEffectBegin;
    if (dwTimePass > dwShowDelay * 2 + dwStayTime)
    {
        return TRUE;
    }
    char szRegionDes1[128] = "";
    char szRegionDes2[128] = "";
    char ss[2] = "";
    int  nIndex = 0;
    for(int i = 0; i < (int)strlen(m_szRegionName); i++)
    {
        ss[0] = m_szRegionName[i];
        ss[1] = m_szRegionName[i+1];
        if ( ( BYTE )ss[0] > 0x80  && ( BYTE )ss[0] < 0xfe)
        {
            i++;
            nIndex++;
            if (nIndex % 2 == 0)
            {
                strncat(szRegionDes2, ss, 2);
                strcat(szRegionDes1, "  \0");
            }
            else
            {
                strncat(szRegionDes1, ss, 2);
                strcat(szRegionDes2, "  \0");
            }
        }
        else
        {
            nIndex++;
            if (nIndex % 2 == 0)
            {
                strncat(szRegionDes2, ss, 1);
                strcat(szRegionDes1, " \0");
            }
            else
            {
                strncat(szRegionDes1, ss, 1);
                strcat(szRegionDes2, " \0");
            }
        }
    }
    const int nFontSize = 48;
    CMyPos posStr = {_SCR_WIDTH / 2 - nFontSize / 2 * strlen(m_szRegionName) / 2, _REGIONNAME_POSY};
    if (dwTimePass < dwShowDelay)
    {
        CMyBitmap::ShowString(posStr.x, posStr.y, m_dwNameColor | (dwAlpha << 24), szRegionDes1, (char*)_REGION_FONT, nFontSize, true);
    }
    else if (dwTimePass > dwShowDelay + dwStayTime)
    {
        CMyBitmap::ShowString(posStr.x, posStr.y, m_dwNameColor | (dwAlpha << 24), szRegionDes2, (char*)_REGION_FONT, nFontSize, true);
    }
    else
    {
        CMyBitmap::ShowString(posStr.x, posStr.y, m_dwNameColor | (dwAlpha << 24), szRegionDes2, (char*)_REGION_FONT, nFontSize, true);
        CMyBitmap::ShowString(posStr.x, posStr.y, m_dwNameColor | (dwAlpha << 24), szRegionDes1, (char*)_REGION_FONT, nFontSize, true);
    }
    return FALSE;
}

BOOL	CGameMap::ShowRegionDesWindow(BOOL bTline)
{
    const DWORD dwShowDelay = 1000;
    const DWORD dwStayTime = 2000;
    DWORD dwAlpha = 0xfd;
    DWORD dwTimePass = ::TimeGet() - m_dwRegionEffectBegin;
    if (dwTimePass > dwShowDelay * 2 + dwStayTime)
    {
        return TRUE;
    }
    char Static[128] ;
    strcpy(Static, m_szRegionDes);
    if (!Static || strlen(Static) == 0)
    {
        return TRUE;
    }
    int nMaxLen = 32;
    if (bTline)
    {
        nMaxLen = 32;
    }
    else
    {
        nMaxLen = 20;
    }
    int nDescLine = 0 ;
    int nCharIndex[10];
    nCharIndex[0] = 0;
    int nSingle = 0;
    int nLineNum = 0;
   int i = 0;
    for ( ; i < (int)strlen ( Static); i++ )
    {
        if ( CMyBitmap::IsDBCSLeadByte( Static, i ) )
        {
            nLineNum += 2;
            i++;
        }
        else
        {
            nSingle++;
            nLineNum++;
        }
        if ( nLineNum >= nMaxLen )
        {
            //----------Begin the english version's process(jump line)----------//
            if ( strcmp( g_objGameDataSet.GetVersionInfo(), "English" ) == 0 )
            {
                // Search the last word
                for ( int j = i; j >= 0; j-- )
                {
                    if ( Static[j] == '~' )
                    {
                        i -= i - j;
                        break;
                    }
                }
            }
            //----------End the english version's process(jump line)----------//
            if ( nSingle % 2 == 0 )
            {
                nSingle = 0;
                nLineNum = 0;
                nDescLine++;
                nCharIndex[nDescLine] = i + 1;
            }
            else
            {
                nDescLine++;
                nCharIndex[nDescLine] = i - ( nLineNum - nMaxLen );
                i -= nLineNum - 2;
                nSingle = 0;
                nLineNum = 0;
            }
        }
    }
    if ( nLineNum < nMaxLen && nLineNum != 0 )
    {
        nDescLine++ ;
        nCharIndex[nDescLine] = i;
    }
    char strTemp[32] = "";
    int nOffset = 0;
    for (int i = 0; i < nDescLine; i++ )
    {
        strncpy ( strTemp, Static + nCharIndex[i], nCharIndex[i + 1] - nCharIndex[i] );
        strTemp[nCharIndex[i + 1] - nCharIndex[i]] = '\0';
        char szRegionDes1[128] = "";
        char szRegionDes2[128] = "";
        char ss[2] = "";
        int  nIndex = 0;
        for(int i = 0; i <  (int)strlen(strTemp); i++)
        {
            ss[0] = strTemp[i];
            ss[1] = strTemp[i+1];
            if ( ( BYTE )ss[0] > 0x80  && ( BYTE )ss[0] < 0xfe)
            {
                i++;
                nIndex++;
                if (nIndex % 2 == 0)
                {
                    strncat(szRegionDes2, ss, 2);
                    if (bTline)
                    {
                        strcat(szRegionDes1, " \0");
                    }
                    else
                    {
                        strcat(szRegionDes1, " \0");
                    }
                }
                else
                {
                    strncat(szRegionDes1, ss, 2);
                    if (bTline)
                    {
                        strcat(szRegionDes2, " \0");
                    }
                    else
                    {
                        strcat(szRegionDes2, "  \0");
                    }
                }
            }
            else
            {
                nIndex++;
                if (nIndex % 2 == 0)
                {
                    strncat(szRegionDes2, ss, 1);
                    strcat(szRegionDes1, " \0");
                }
                else
                {
                    strncat(szRegionDes1, ss, 1);
                    strcat(szRegionDes2, " \0");
                }
            }
        }
        const int nFontSize = 24;
        if (bTline)
        {
            CMyPos posStr = {_REGIONDES_POSX, _REGIONDES_POSY};
            if (dwTimePass < dwShowDelay)
            {
                CMyBitmap::ShowTString(posStr.x - nOffset, posStr.y, m_dwDesColor | (dwAlpha << 24), szRegionDes1, (char*)_REGION_FONT, nFontSize, true);
            }
            else if (dwTimePass > dwShowDelay + dwStayTime)
            {
                CMyBitmap::ShowTString(posStr.x - nOffset, posStr.y , m_dwDesColor | (dwAlpha << 24), szRegionDes2, (char*)_REGION_FONT, nFontSize, true);
                CMyBitmap::ShowTString(posStr.x - nOffset, posStr.y , m_dwDesColor | (dwAlpha << 24), szRegionDes1, (char*)_REGION_FONT, nFontSize, true);
            }
            else
            {
                CMyBitmap::ShowTString(posStr.x - nOffset, posStr.y, m_dwDesColor | (dwAlpha << 24), szRegionDes2, (char*)_REGION_FONT, nFontSize, true);
            }
            nOffset += nFontSize;
        }
        else
        {
            CMyPos posStr = {_SCR_WIDTH / 2 - nFontSize / 2 * strlen(strTemp) / 2, _REGIONNAME_POSY + 50};
            if (dwTimePass < dwShowDelay)
            {
                CMyBitmap::ShowString(posStr.x, posStr.y + nOffset, m_dwDesColor | (dwAlpha << 24), szRegionDes1, (char*)_REGION_FONT, nFontSize, true);
            }
            else if (dwTimePass > dwShowDelay + dwStayTime)
            {
                CMyBitmap::ShowString(posStr.x, posStr.y + nOffset, m_dwDesColor | (dwAlpha << 24), szRegionDes2, (char*)_REGION_FONT, nFontSize, true);
            }
            else
            {
                CMyBitmap::ShowString(posStr.x, posStr.y + nOffset, m_dwDesColor | (dwAlpha << 24), szRegionDes1, (char*)_REGION_FONT, nFontSize, true);
                CMyBitmap::ShowString(posStr.x, posStr.y + nOffset, m_dwDesColor | (dwAlpha << 24), szRegionDes2, (char*)_REGION_FONT, nFontSize, true);
            }
            nOffset += nFontSize;
        }
    }
    return FALSE;
}

//-------------------------------------------------------------
DWORD	CGameMap::CellIndexCounter()
{
    DWORD dwAmount =  m_sizeMap.iWidth * m_sizeMap.iHeight;
    CellInfo* pMyCellInfo = m_pCellInfo;
    if (!pMyCellInfo)
    {
        return NULL;
    }
    IndexLayerInfo info;
    for(DWORD i = 0; i < dwAmount; i ++)
    {
        // search ...
        DWORD dwIndexAmount = m_setIndexLayerInfo.size();
        BOOL bFound = false;
        for(DWORD j = 0; j < dwIndexAmount; j++)
        {
            if ((pMyCellInfo->m_infoLayer.usTerrain == m_setIndexLayerInfo[j].usTerrain) &&
                    (pMyCellInfo->m_infoLayer.usMask == m_setIndexLayerInfo[j].usMask) &&
                    (pMyCellInfo->m_infoLayer.sAltitude == m_setIndexLayerInfo[j].sAltitude))
            {
                bFound = true;
                break;
            }
        }
        if (!bFound)
        {
            info.usTerrain = pMyCellInfo->m_infoLayer.usTerrain;
            info.usMask = pMyCellInfo->m_infoLayer.usMask;
            info.sAltitude = pMyCellInfo->m_infoLayer.sAltitude;
            m_setIndexLayerInfo.push_back(info);
        }
        pMyCellInfo ++;
    }
    return m_setIndexLayerInfo.size();
}
//-------------------------------------------------------------
void	CGameMap::CellIndexUniteCounter(DWORD& dwSame, DWORD& dwDif)
{
    CellInfo infoCell;
    dwSame = dwDif = 0;
    if (!m_pCellInfo)
    {
        return;
    }
    int j = 0;
    const DWORD dwStepLength = 8;
    DWORD dwStepAmount = m_sizeMap.iWidth / dwStepLength;
    if (m_sizeMap.iWidth % dwStepLength != 0)
    {
        dwStepAmount ++;
    }
    for(int i = 0; i < m_sizeMap.iHeight; i ++)
    {
        for(DWORD j = 0; j < dwStepAmount; j ++)
        {
            BOOL bSame = true;
            for(int k = 0; k < dwStepLength; k ++)
            {
                if ((int)(j* dwStepLength + k) >= m_sizeMap.iWidth)
                {
                    bSame = false;
                    break;
                }
                if (k == 0)
                {
                    CellInfo* pCellInfo = m_pCellInfo + i * m_sizeMap.iWidth + k * dwStepLength + j;
                    infoCell.m_infoLayer.usTerrain = pCellInfo->m_infoLayer.usTerrain;
                    infoCell.m_infoLayer.usMask = pCellInfo->m_infoLayer.usMask;
                    infoCell.m_infoLayer.sAltitude = pCellInfo->m_infoLayer.sAltitude;
                }
                else
                {
                    CellInfo* pCellInfo = m_pCellInfo + i * m_sizeMap.iWidth + k * dwStepLength + j;
                    if ((infoCell.m_infoLayer.usTerrain != pCellInfo->m_infoLayer.usTerrain) ||
                            (infoCell.m_infoLayer.usMask != pCellInfo->m_infoLayer.usMask) ||
                            (infoCell.m_infoLayer.sAltitude != pCellInfo->m_infoLayer.sAltitude))
                    {
                        bSame = false;
                        break;
                    }
                }
            }
            if (bSame)
            {
                dwSame ++;
            }
            else
            {
                dwDif ++;
            }
        }
    }
}
//----------------------------------------------------------------------------------------------------
