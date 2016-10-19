
//**********************************************************
// 代码编辑器
//**********************************************************

//-------------------------------------------------
// A*
//----------------------------------------------------//
#include "3DGameMap.h"
#include "GamePlayerSet.h"
#include "Hero.h"

void CGameMap::ClearCloseSet()
{
    m_nCloseNodeAmount = 0;
}
//----------------------------------------------------//
void CGameMap::ClearOpenSet()
{
    m_setOpen.clear();
}
//----------------------------------------------------//
void CGameMap::ClearPathSet()
{
    m_setPath.clear();
}
//----------------------------------------------------//
void CGameMap::BuildPlayerMap(CMyPos posStart)
{
    for(int k = 0; k < 37; k ++)
        for(int j = 0; j < 37; j ++)
        {
            m_bPlayerMap[k][j] = 0;
        }
    if (g_objHero.TestStatus(USERSTATUS_DIE))
    {
        return;
    }
    int nBeginX	= posStart.x - 18;
    int nBeginY	= posStart.y - 18;
    int nEndX	= posStart.x + 18;
    int nEndY	= posStart.y + 18;
    m_posPlayerMap.x = nBeginX;
    m_posPlayerMap.y = nBeginY;
    int nPlayerAmount = g_objPlayerSet.GetPlayerAmount();
    for(int i = 0 ; i < nPlayerAmount + 1; i ++)
    {
        CPlayer* pPlayer = NULL;
        if (i == nPlayerAmount)
        {
            pPlayer = &g_objHero;
        }
        else
        {
            pPlayer = g_objPlayerSet.GetPlayerByIndex(i);
        }
        if (pPlayer && !pPlayer->TestStatus(USERSTATUS_DIE))
        {
            CMyPos posPlayer;
            pPlayer->GetPos(posPlayer);
            if (posPlayer.x >= nBeginX && posPlayer.y >= nBeginY &&
                    posPlayer.x <= nEndX && posPlayer.y <= nEndY)
            {
                m_bPlayerMap[posPlayer.x - nBeginX][posPlayer.y - nBeginY] = 1;
            }
        }
    }
}
//----------------------------------------------------//
BOOL CGameMap::CheckPlayerMap(CMyPos posCell)
{
    int nBeginX	= m_posPlayerMap.x;
    int nBeginY	= m_posPlayerMap.y;
    int nEndX	= m_posPlayerMap.x + 36;
    int nEndY	= m_posPlayerMap.y + 36;
    if (posCell.x >= nBeginX && posCell.y >= nBeginY &&
            posCell.x <= nEndX && posCell.y <= nEndY)
        if (m_bPlayerMap[posCell.x - nBeginX][posCell.y - nBeginY])
        {
            return true;
        }
    return false;
}
//----------------------------------------------------//
void CGameMap::FindPath(CMyPos posStart, CMyPos posTarget)
{
    m_posRealTarget = posStart;
    m_posStart = posStart;
    m_posTarget = posTarget;
    if ((posStart.x ==  posTarget.x) && (posStart.y ==  posTarget.y))
    {
        return;
    }
    this->InitAStar();
    while(true)
    {
        if (m_nCloseNodeAmount >= _MAX_STEP)
        {
            return;
        }
        if (m_setOpen.size() == 0)
        {
            return;
        }
        NodeInfo* pBestNode = this->CheckOpenList();
        if (!pBestNode)
        {
            continue;
        }
        if (this->AddSucNode(pBestNode))
        {
            return;
        }
    }
}
//----------------------------------------------------//
void CGameMap::InitAStar()
{
    //把s压入open
    this->ClearNodeSet();
    this->ClearPathSet();
    this->ClearOpenSet();
    this->ClearCloseSet();
    m_nNodeIndex = 0;
    int nAmount = m_setCellAStar.size();
    for(int i = 0; i < nAmount; i++)
    {
        CellInfo* pCellInfo = m_setCellAStar[i];
        if (pCellInfo)
        {
            pCellInfo->m_bSearched = false;
        }
    }
    m_setCellAStar.clear();
    NodeInfo* pNode = this->CreateNewNode();
    if (pNode)
    {
        pNode->nX = m_posStart.x;
        pNode->nY = m_posStart.y;
        pNode->nG = 0;
        pNode->nH = pNode->nG + this->GetHValue(m_posStart);
        pNode->nStep = 0;
        pNode->pNode = NULL;
        pNode->pChildNode = NULL;
        m_setOpen.push_back(pNode);
    }
    CellInfo* pCellInfo =  this->GetCell(m_posStart.x, m_posStart.y);
    if (pCellInfo)
    {
        pCellInfo->m_bSearched = true;
        m_setCellAStar.push_back(pCellInfo);
    }
}
//----------------------------------------------------//
int CGameMap::GetHValue(CMyPos pos)
{
    int nDx = abs(m_posTarget.x - pos.x);
    int nDy = abs(m_posTarget.y - pos.y);
    if (nDx > nDy)
    {
        return 10 * nDx + 6 * nDy;
    }
    return 10 * nDy + 6 * nDx;
}
//----------------------------------------------------//
NodeInfo* CGameMap::CheckOpenList()
{
    NodeInfo* pNode = this->CheckBestNode();
    if (pNode->nStep >= _MAX_STEP)
    {
        return NULL;
    }
    return pNode;
}
//----------------------------------------------------//
NodeInfo* CGameMap::CheckBestNode()
{
    int nAmount = m_setOpen.size();
    NodeInfo* pBestNode = m_setOpen[0];
    int nBestNodeIndex = 0;
    for(int i = 1; i < nAmount; i++)
    {
        NodeInfo* pNode =  m_setOpen[i];
        if (pNode)
        {
            if (m_nPathMode == _ASTAR_CLOSE)
            {
                if ((pNode->nH) < (pBestNode->nH))
                {
                    pBestNode = pNode;
                    nBestNodeIndex = i;
                }
            }
            else
            {
                if ((pNode->nH) > (pBestNode->nH))
                {
                    pBestNode = pNode;
                    nBestNodeIndex = i;
                }
            }
        }
    }
    //::LogMsg("最佳节点:%d_%d", pBestNode->nX, pBestNode->nY);
    // 从Open表最佳节点
    //pBestNode->nG = 65535555;
    m_setOpen.erase(m_setOpen.begin() + nBestNodeIndex);
    // 添加到Close表
    //m_setClose.push_back(pBestNode);
    m_nCloseNodeAmount ++;
    return pBestNode;
}
//----------------------------------------------------//
BOOL CGameMap::AddSucNode(NodeInfo* pNode)
{
    if (!pNode)
    {
        return false;
    }
    CMyPos posNode;
    posNode.x = pNode->nX;
    posNode.y = pNode->nY;
    CMyPos posNewNode;
    int nArrayX[8] = {0, -1, -1, -1,  0,  1, 1, 1};
    int nArrayY[8] = {1,  1,  0, -1, -1, -1, 0, 1};
    for(int i = 0; i < 8; i++)
    {
        posNewNode.x = posNode.x + nArrayX[i];
        posNewNode.y = posNode.y + nArrayY[i];
        if (posNewNode.x < 0)
        {
            continue;
        }
        if (posNewNode.y < 0)
        {
            continue;
        }
        if (posNewNode.x > m_sizeMap.iWidth)
        {
            continue;
        }
        if (posNewNode.y > m_sizeMap.iHeight)
        {
            continue;
        }
        int nHeight = 0;
        CellInfo* pCellInfo = this->GetCell(pNode->nX, pNode->nY);
        if (pCellInfo)
        {
            LayerInfo* pLayerInfo = this->GetLastLayerInfo(pCellInfo);
            if (pLayerInfo)
            {
                nHeight = pLayerInfo->sAltitude;
            }
        }
        if (!this->CheckNode(posNewNode, nHeight))
        {
            continue;
        }
        NodeInfo* pNewNode =  this->CreateNewNode();
        if (!pNewNode)
        {
            continue;
        }
        pNewNode->nX = posNewNode.x;
        pNewNode->nY = posNewNode.y;
        //#####代价#####
        pNewNode->nG = pNode->nG;// + 8;
        pNewNode->nH = pNode->nG + this->GetHValue(posNewNode);
        pNewNode->nStep = pNode->nStep + 1;
        pNewNode->pNode = pNode;
        pNewNode->nDir  = i;
        pNode->pChildNode = pNewNode;
        m_setOpen.push_back(pNewNode);
        // 算最近点...
        if (_ASTAR_CLOSE == m_nPathMode)
        {
            DWORD dwOldV = (m_posRealTarget.x - m_posTarget.x) *
                           (m_posRealTarget.x - m_posTarget.x) +
                           (m_posRealTarget.y - m_posTarget.y) *
                           (m_posRealTarget.y - m_posTarget.y);
            DWORD dwNewV = (pNewNode->nX - m_posTarget.x) *
                           (pNewNode->nX - m_posTarget.x) +
                           (pNewNode->nY - m_posTarget.y) *
                           (pNewNode->nY - m_posTarget.y);
            if (dwNewV < dwOldV)
            {
                m_posRealTarget.x = pNewNode->nX;
                m_posRealTarget.y = pNewNode->nY;
            }
        }
        else
        {
            DWORD dwOldV = (m_posRealTarget.x - m_posTarget.x) *
                           (m_posRealTarget.x - m_posTarget.x) +
                           (m_posRealTarget.y - m_posTarget.y) *
                           (m_posRealTarget.y - m_posTarget.y);
            DWORD dwNewV = (pNewNode->nX - m_posTarget.x) *
                           (pNewNode->nX - m_posTarget.x) +
                           (pNewNode->nY - m_posTarget.y) *
                           (pNewNode->nY - m_posTarget.y);
            if (dwNewV > dwOldV)
            {
                m_posRealTarget.x = pNewNode->nX;
                m_posRealTarget.y = pNewNode->nY;
                if (dwNewV >= (DWORD)(m_nLeaveDisc * m_nLeaveDisc))
                {
                    this->GetPath(pNewNode);
                    return true;
                }
            }
        }
        //::LogMsg("添加节点:%d_%d", pNewNode->nX, pNewNode->nY);
        if (_ASTAR_CLOSE == m_nPathMode && (pNewNode->nX == m_posTarget.x) && (pNewNode->nY == m_posTarget.y))
        {
            this->GetPath(pNewNode);
            return true;
        }
    }
    return false;
}
//----------------------------------------------------//
void CGameMap::GetPath(NodeInfo* pNode)
{
    NodeInfo* pMyNode = pNode;
    while(true)
    {
        if (pMyNode->pNode)
        {
            m_setPath.push_front(pMyNode);
            pMyNode = pMyNode->pNode;
        }
        else
        {
            break;
        }
    }
}

//----------------------------------------------------//
BOOL CGameMap::CheckNode(CMyPos posNode, int nHeight)
{
    // 可达 ...?
    CellInfo* pCellInfo = g_objGameMap.GetCell(posNode.x, posNode.y);
    if (!pCellInfo)
    {
        return false;
    }
    if (pCellInfo->m_bSearched)
    {
        return false;
    }
    LayerInfo* pLayerInfo = g_objGameMap.GetLastLayerInfo(pCellInfo);
    if (!pLayerInfo)
    {
        return false;
    }
    if (!CheckPlayerMap(posNode) && (pLayerInfo->usMask == 0) && abs(nHeight - pLayerInfo->sAltitude) <= 25)
    {
        pCellInfo->m_bSearched = true;
        m_setCellAStar.push_back(pCellInfo);
        return true;
    }
    return false;
}
//----------------------------------------------------//
int  CGameMap::GetPathAmount()
{
    return m_setPath.size();
}
//----------------------------------------------------//
NodeInfo* CGameMap::GetNodeByIndex(int nIndex)
{
    int nAmount = m_setPath.size();
    if (nIndex < 0)
    {
        return NULL;
    }
    if (nIndex >= nAmount)
    {
        return NULL;
    }
    return m_setPath[nIndex];
}
//----------------------------------------------------//
int  CGameMap::GetStep(CMyPos posCell)
{
    int nAmount =  m_setPath.size();
    for(int i = 0; i < nAmount; i++)
    {
        NodeInfo* pNod = this->GetNodeByIndex(i);
        if (!pNod)
        {
            return -1;
        }
        if ((pNod->nX == posCell.x) && (pNod->nY == posCell.y))
        {
            return pNod->nStep;
        }
    }
    return -1;
}
//----------------------------------------------------//
void CGameMap::ClearNodeSet()
{
    int nAmount = m_setNode.size();
    for(int i = 0; i < nAmount; i++)
    {
        NodeInfo* pNod = m_setNode[i];
        SAFE_DELETE(pNod);
    }
    m_setNode.clear();
    m_nNodeIndex = 0;
}

//----------------------------------------------------//
NodeInfo* CGameMap::CreateNewNode()
{
    int nAmount =  m_setNode.size();
    NodeInfo* pNode = NULL;
    m_nNodeIndex ++;
    if (m_nNodeIndex >= nAmount)
    {
        pNode = new(NodeInfo);
        m_setNode.push_back(pNode);
    }
    else
    {
        pNode = m_setNode[m_nNodeIndex];
    }
    return pNode;
}
//----------------------------------------------------//
BOOL CGameMap::TestPath(CMapObj* pObj, CMyPos posTarget)
{
    if (!pObj)
    {
        return false;
    }
    int nType = pObj->GetObjType();
    if (nType == MAP_ROLE || nType == MAP_HERO || nType == MAP_PLAYER)
    {
        CPath objPath;
        CMyPos posObj = {0, 0};
        objPath.SetPlayer(pObj);
        pObj->GetPos(posObj);
        this->FindPath(objPath, posObj, posTarget);
        return objPath.IsReachable();
    }
    return false;
}
//----------------------------------------------------//
void CGameMap::FindPath(CPath& objPath, CMyPos posStart, CMyPos posTarget)
{
    this->BuildPlayerMap(posStart);
    m_nLeaveDisc = objPath.GetData();
    m_nLeaveDisc = 15;
    this->FindPath(posStart, posTarget);
    objPath.ClearStep();
    int nAmount = this->GetPathAmount();
    m_nPathMode = objPath.GetSearchMethod();
    if (nAmount == 0 || m_nPathMode == _ASTAR_LEAVE)
    {
        objPath.ClearStep();
        if (m_nPathMode == _ASTAR_LEAVE)
        {
            m_nPathMode = _ASTAR_CLOSE;
            this->FindPath(posStart, m_posRealTarget);
            m_nPathMode = _ASTAR_LEAVE;
        }
        else
        {
            this->FindPath(posStart, m_posRealTarget);
        }
        objPath.SetReachable(false);
    }
    else
    {
        objPath.SetReachable(true);
    }
    if ((posStart.x == posTarget.x) && (posStart.y == posTarget.y) && (m_nPathMode != _ASTAR_LEAVE))
    {
        objPath.SetReachable(true);
        objPath.ClearStep();
        return;
    }
    nAmount = this->GetPathAmount();
    for(int i = 0; i < nAmount; i++)
    {
        NodeInfo* pInfo = g_objGameMap.GetNodeByIndex(i);
        if (pInfo)
        {
            objPath.AddStep(pInfo->nDir, 0);
        }
    }
}
//----------------------------------------------------//
void CGameMap::ShowPath()
{
    int nAmount = this->GetPathAmount();
    for(int i = 0; i < nAmount; i++)
    {
        NodeInfo* pInfo = g_objGameMap.GetNodeByIndex(i);
        if (pInfo)
        {
            char szStep[64];
            sprintf(szStep, "%d", i);
            CMyPos posScr, posWorld;
            this->Cell2World(pInfo->nX, pInfo->nY, posWorld.x, posWorld.y);
            this->World2Screen(posWorld.x, posWorld.y, posScr.x, posScr.y);
            CMyBitmap::ShowString(posScr.x + 10, posScr.y, 0xffffffff, szStep);
        }
    }
}
//----------------------------------------------------//
/*void CGameMap::FindPath(CMyPos posStart, CMyPos posTarget)
{
	const DWORD dwStepLimit = 0;

	while(true)
	{
		if (posStart.x == posTarget.x && posStart.y == posTarget.y)
			return;
		if ()
	}
}*/
//----------------------------------------------------//
