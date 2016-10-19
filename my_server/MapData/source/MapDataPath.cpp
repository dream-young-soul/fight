
//**********************************************************
// 代码编辑器
//**********************************************************

//----------------------------------------------------//
// InterActiveLayer.cpp
//----------------------------------------------------//
#include "typedef.h"
#include "MapData.h"
#include "TerrainObj.h"
#include "common.h"


///////////////////////////////////////////////////////////////////////////////////////
// posSource(in), pposTarget(in/out), pPathBuf(out), pBufSize(in/out)
bool CMapData::FindPath(PASSPATH0* pPathBuf, int nBufSize, POINT posSource, POINT posTarget, IRoleAttr* pAttr, int nEscapeSteps /*= 0*/)
{
    m_nLeaveDisc = nEscapeSteps;
    m_nPathMode = (nEscapeSteps == 0 ? _ASTAR_CLOSE : _ASTAR_LEAVE);
    this->FindPath(posSource, posTarget);
    memset(pPathBuf, 0, nBufSize);
    char*	pStep	= pPathBuf->pPath;
    int nAmount = this->GetPathAmount();
    if (nAmount == 0 || m_nPathMode == _ASTAR_LEAVE)
    {
        if (m_nPathMode == _ASTAR_LEAVE)
        {
            m_nPathMode = _ASTAR_CLOSE;
            this->FindPath(posSource, m_posRealTarget);
            m_nPathMode = _ASTAR_LEAVE;
        }
        else
        {
            this->FindPath(posSource, m_posRealTarget);
        }
    }
    nAmount = this->GetPathAmount();
    if (nAmount > nBufSize - sizeof(PASSPATH0))
    {
        nAmount =  nBufSize - sizeof(PASSPATH0);
    }
    for(int i = 0; i < nAmount; i++)
    {
        NodeInfo* pInfo = GetNodeByIndex(i);
        if (pInfo)
        {
            *(pStep++)	= pInfo->nDir;
        }
    }
    pPathBuf->nSize	= pStep - pPathBuf->pPath;
    pPathBuf->posStart	= posSource;
    pPathBuf->posStop	= m_posRealTarget;
    return true;
}

//----------------------------------------------------//
void CMapData::FindPath(POINT posStart, POINT posTarget)
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
        if (m_nCloseNodeAmount >= _MAX_PATHSTEP)
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
void CMapData::InitAStar()
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
        CCell* pCell = m_setCellAStar[i];
        if (pCell)
        {
            pCell->SetSearchFlag(false);
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
    CCell* pCell =  this->GetCell(m_posStart.x, m_posStart.y);
    if (pCell)
    {
        pCell->SetSearchFlag(true);
        m_setCellAStar.push_back(pCell);
    }
}
//----------------------------------------------------//
int CMapData::GetHValue(POINT pos)
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
NodeInfo* CMapData::CheckOpenList()
{
    NodeInfo* pNode = this->CheckBestNode();
    if (pNode->nStep >= _MAX_PATHSTEP)
    {
        return NULL;
    }
    return pNode;
}
//----------------------------------------------------//
NodeInfo* CMapData::CheckBestNode()
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
BOOL CMapData::AddSucNode(NodeInfo* pNode)
{
    if (!pNode)
    {
        return false;
    }
    POINT posNode;
    posNode.x = pNode->nX;
    posNode.y = pNode->nY;
    POINT posNewNode;
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
        if (posNewNode.x >= m_sizeMap.cx)
        {
            continue;
        }
        if (posNewNode.y >= m_sizeMap.cy)
        {
            continue;
        }
        int nHeight = 0;
        CCell* pCell = this->GetCell(pNode->nX, pNode->nY);
        if (pCell)
        {
            nHeight = pCell->GetFloorAlt(m_set2Layer);
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
                if (dwNewV >= m_nLeaveDisc * m_nLeaveDisc)
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
void CMapData::GetPath(NodeInfo* pNode)
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
BOOL CMapData::CheckNode(POINT posNode, int nHeight)
{
    // 可达 ...?
    CCell* pCell = GetCell(posNode.x, posNode.y);
    if (!pCell)
    {
        return false;
    }
    if (pCell->BeSearched())
    {
        return false;
    }
    if ((pCell->GetFloorMask(m_set2Layer) == 0) && abs(nHeight - pCell->GetFloorAlt(m_set2Layer)) <= 25)
    {
        pCell->SetSearchFlag(true);
        m_setCellAStar.push_back(pCell);
        return true;
    }
    return false;
}
//----------------------------------------------------//
int  CMapData::GetPathAmount()
{
    return m_setPath.size();
}
//----------------------------------------------------//
NodeInfo* CMapData::GetNodeByIndex(int nIndex)
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
int  CMapData::GetStep(POINT posCell)
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
void CMapData::ClearNodeSet()
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
NodeInfo* CMapData::CreateNewNode()
{
    int nAmount =  m_setNode.size();
    NodeInfo* pNode = NULL;
    m_nNodeIndex ++;
    if (m_nNodeIndex >= nAmount)
    {
        pNode = new NodeInfo;
        m_setNode.push_back(pNode);
    }
    else
    {
        pNode = m_setNode[m_nNodeIndex];
    }
    return pNode;
}

