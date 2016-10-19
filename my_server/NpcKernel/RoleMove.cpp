
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// RoleMove.cpp: implementation of the CRoleMove class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <math.h>
#include "BaseFunc.h"

#include "AllMsg.h"
#include "define.h"
#include "RoleMove.h"
#include "Agent.h"
#include "NpcWorld.h"

inline bool	Move(POINT& pos, int nDir)
{
    CHECKF(nDir >= 0 && nDir < MAX_DIRSIZE);
    pos.x	+= _DELTA_X[nDir];
    pos.y	+= _DELTA_Y[nDir];
    return true;
}

inline bool KeepDistence(int x0, int y0, int x, int y, int nDist)
{
    if (!nDist)
    {
        return true;
    }
    if (::Distance(x0, y0, x, y) < nDist)
    {
        return false;
    }
    return true;
}

bool CalcFarthestPos(POINT* pPos, const POINT& pos, const POINT& posTarget, int nRange)
{
    CHECKF(nRange > 0);
    CHECKF(pos.x != posTarget.x || pos.y != posTarget.y);
    if (Distance(pos.x, pos.y, posTarget.x, posTarget.y) <= nRange)
    {
        *pPos = posTarget;
        return true;
    }
    int nDeltaX = posTarget.x - pos.x;
    int nDeltaY = posTarget.y - pos.y;
    if (abs(nDeltaX) > abs(nDeltaY))
    {
        pPos->x	= pos.x + nRange * ((nDeltaX > 0) ? 1 : -1);
        pPos->y	= pos.y + MulDiv(nRange, nDeltaY, abs(nDeltaX));
    }
    else
    {
        pPos->x	= pos.x + MulDiv(nRange, nDeltaX, abs(nDeltaY));
        pPos->y	= pos.y + nRange * ((nDeltaY > 0) ? 1 : -1);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRoleMove::CRoleMove()
{
}

//////////////////////////////////////////////////////////////////////
CRoleMove::~CRoleMove()
{
}

//////////////////////////////////////////////////////////////////////
bool CRoleMove::Create(CAgent* pAgent)
{
    m_pOwner	= pAgent;
    m_bLock		= false;
    m_bJump		= false;
    return true;
}

//////////////////////////////////////////////////////////////////////
void CRoleMove::OnTimer()
{
    if (IsMoveEnable() && IsMoving() && QueryTime().ToNextTime())
    {
        if (m_bJump)
        {
            SetPos(m_posTarget.x, m_posTarget.y);
            CMsgAction	msg;
            IF_OK(msg.Create(m_pOwner->GetID(), GetPosX(), GetPosY(), 0, actionJump, m_posTarget.x, m_posTarget.y))
            m_pOwner->SendMsg(&msg);
            QueryTime().Startup(JUMP_DELAY_MS);
            ClearMove();
        }
        else
        {
            CHECK(m_setPath.size());
            int nDir = m_setPath.pop();
            // move to
            POINT pos = m_pos;
            CHECK(Move(pos, nDir));
            if (GetMap())
            {
                GetMap()->MoveTo(Cast<INpc>(m_pOwner), pos.x, pos.y);
            }
            m_pos	= pos;
            // msg
            CMsgWalk	msg;
            IF_OK(msg.Create(m_pOwner->GetID(), nDir, m_nMoveMode))
            m_pOwner->SendMsg(&msg);
            if (m_nMoveMode == WALKMODE_MOVE)
            {
                QueryTime().Startup(WALK_DELAY_MS);
            }
            else
            {
                QueryTime().Startup(RUN_DELAY_MS);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CRoleMove::EnterMap(OBJID idMap, int x, int y)		// process msg
{
    m_bLock		= false;
    m_pMap		= MapManager()->QueryMap(idMap);
    CHECK(m_pMap);
    m_pos.x		= x;
    m_pos.y		= y;
    m_pMap->EnterMap(Cast<INpc>(m_pOwner));
    LOGDUMP("Agent EnterMap() OK");
}

//////////////////////////////////////////////////////////////////////
void CRoleMove::LeaveMap()
{
    m_bLock = true;
    m_pMap->LeaveMap(Cast<INpc>(m_pOwner));
}

//////////////////////////////////////////////////////////////////////
bool CRoleMove::WalkTo(int x, int y, int nDist/*=0*/)
{
    if (!IsMoveEnable())	// || idMap != m_pMap->GetID())
    {
        return false;
    }
    if (GetDistance(x, y) <= nDist)
    {
        return false;
    }
    ClearMove();
    m_nMoveMode		= WALKMODE_MOVE;
    m_posTarget.x	= x;
    m_posTarget.y	= y;
    return FindPath(x, y, nDist);
}

//////////////////////////////////////////////////////////////////////
bool CRoleMove::RunTo(int x, int y, int nDist/*=0*/)
{
    if (!IsMoveEnable())	// || idMap != m_pMap->GetID())
    {
        return false;
    }
    if (GetDistance(x, y) <= nDist)
    {
        return false;
    }
    ClearMove();
    m_nMoveMode		= WALKMODE_RUN;
    m_posTarget.x	= x;
    m_posTarget.y	= y;
    return FindPath(x, y, nDist);
}

//////////////////////////////////////////////////////////////////////
bool CRoleMove::JumpTo(int x, int y, int nDist/*=0*/)		// may be walk
{
    if (!IsMoveEnable())	// || idMap != m_pMap->GetID())
    {
        return false;
    }
    if (GetDistance(x, y) <= nDist)
    {
        return false;
    }
    ClearMove();
    m_nMoveMode		= WALKMODE_JUMP;
    m_posTarget.x	= x;
    m_posTarget.y	= y;
    POINT	posTarget;
    posTarget.x	= x;
    posTarget.y	= y;
    // step 1
    POINT	pos;
    CalcFarthestPos(&pos, m_pos, posTarget, JUMP_DISTANCE);
    if (KeepDistence(pos.x, pos.y, x, y, nDist) && m_pMap->IsJumpEnable(pos.x, pos.y))
    {
        m_posTarget	= pos;
        m_bJump		= true;
        return true;
    }
    // step 2
    vector<POINT>	setLine;
    DDALineEx(m_pos.x, m_pos.y, pos.x, pos.y, setLine);
    for(int i = setLine.size() - 1; i >= 0; i--)					// far to near
    {
        POINT& temp = setLine[i];
        if ((temp.x != m_pos.x || temp.y != m_pos.y) && KeepDistence(temp.x, temp.y, x, y, nDist) && m_pMap->IsJumpEnable(temp.x, temp.y))
        {
            m_posTarget	= temp;
            m_bJump		= true;
            return true;
        }
    }
    // step 3
    int nTargetDist	= GetDistance(x, y);
    int	nBestDist	= nTargetDist;
    POINT	posBest	= m_pos;
    for(int i = m_pos.x - JUMP_DISTANCE; i <= m_pos.x + JUMP_DISTANCE; i++)
    {
        for(int j = m_pos.y - JUMP_DISTANCE; j <= m_pos.y + JUMP_DISTANCE; j++)
        {
            int	dist	= ::Distance(i, j, x, y);
            if (dist < nBestDist && m_pMap->IsJumpEnable(i, j) && KeepDistence(i, j, x, y, nDist))
            {
                nBestDist	= dist;
                posBest.x	= i;
                posBest.y	= j;
            }
        }
    }
    if (posBest.x != m_pos.x || posBest.y != m_pos.y)
    {
        m_posTarget	= posBest;
        m_bJump		= true;
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CRoleMove::ChgMap(OBJID idMap, int x, int y)		// may be walk
{
    if (!IsMoveEnable())
    {
        return false;
    }
    ClearMove();
    if (idMap != m_pMap->GetID() || GetDistance(x, y) >= CHGMAP_DISTANCE)
    {
        CMsgAction	msg;
        IF_OK(msg.Create(m_pOwner->GetID(), x, y, 0, actionChgMap, idMap))
        m_pOwner->SendMsg(&msg);
        LeaveMap();
        QueryTime().Startup(CHGMAP_DELAY_MS);
        return true;
    }
    else if (GetDistance(x, y) > CELLS_PER_VIEW)
    {
        return JumpTo(x, y);
    }
    else
    {
        return RunTo(x, y);
    }
}

//////////////////////////////////////////////////////////////////////
bool CRoleMove::JumpSafePos(int nSafeRange/*=2*/)							// may be walk
{
    CHECKF(nSafeRange > 0 && nSafeRange <= JUMP_DISTANCE);
    if (!IsMoveEnable())
    {
        return false;
    }
    vector<IRole*>	setRole;
    m_pMap->CollectRole(&setRole, m_pos, JUMP_DISTANCE);
    int		nWidth	= JUMP_DISTANCE * 2 + 1;
    int		nCount	= nWidth * nWidth;
    vector<int>		setFlag;
    setFlag.resize(nWidth * nWidth);
    POINT	posLT;
    POS2LT(&posLT, m_pos, JUMP_DISTANCE);
    for(int idx = 0; idx < nWidth * nWidth; idx++)
    {
        POINT	pos = posLT;
        POS_ADD(&pos, INDEX2X(idx, JUMP_DISTANCE, JUMP_DISTANCE), INDEX2Y(idx, JUMP_DISTANCE, JUMP_DISTANCE));
        if (!m_pMap->IsJumpEnable(pos.x, pos.y))
        {
            setFlag[idx] = 1;
            nCount--;
        }
    }
    for(int i = 0; i < setRole.size(); i++)
    {
        IRole* pRole = setRole[i];
        if (pRole && pRole->IsEvil())
        {
            POINT	posRoleLT;
            posRoleLT.x = pRole->GetPosX();
            posRoleLT.y = pRole->GetPosY();
            POS2LT(&posRoleLT, posRoleLT, nSafeRange);
            int nSafeSize = nSafeRange * nSafeRange + 1;
            for(int idx = 0; idx < nSafeSize * nSafeSize; idx++)
            {
                POINT	pos = posRoleLT;
                POS_ADD(&pos, INDEX2X(idx, nSafeRange, nSafeRange), INDEX2Y(idx, nSafeRange, nSafeRange));
                POS_SUB(&pos, posLT.x, posLT.y);
                if (pos.x >= 0 && pos.x < nWidth && pos.y >= 0 && pos.y < nWidth)
                {
                    int nIndex = POS2INDEX(pos.x, pos.y, JUMP_DISTANCE, JUMP_DISTANCE);
                    if (setFlag[nIndex] == 0)
                    {
                        setFlag[nIndex] = 1;
                        nCount--;
                    }
                }
            }
        }
    }
    ASSERT(nCount >= 0);
    if (nCount > 0)
    {
        int	nPos = ::RandGet(nCount);
        for(int i = 0; i < setFlag.size(); i++)
        {
            if (setFlag[i] == 0)
            {
                if (nPos == 0)
                {
                    m_posTarget.x	= m_pos.x - JUMP_DISTANCE + INDEX2X(i, JUMP_DISTANCE, JUMP_DISTANCE);
                    m_posTarget.y	= m_pos.y - JUMP_DISTANCE + INDEX2Y(i, JUMP_DISTANCE, JUMP_DISTANCE);
                    m_bJump	= true;
                    return true;
                }
                else
                {
                    nPos--;
                }
            }
        }
        ASSERT(!"nCount");
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CRoleMove::FindPath(int x, int y, int nDist)
{
    ASSERT(m_setPath.size() == 0);
    // get dir set
    Array<int>	setPath;
    FindPath(&setPath,  x, y);
    if (setPath.Size() == 0)
    {
        return false;
    }
    // get pos set
    Array<POINT>	setPos;
    POINT	pos	= m_pos;
    for(int i = 0; i < setPath.Size(); i++)
    {
        Move(pos, setPath[i]);
        setPos.Push(pos);
    }
    // find last pos, push to m_setPath
    for(int i = setPath.Size() - 1; i >= 0; i--)
    {
        POINT temp = setPos[i];
        if (KeepDistence(temp.x, temp.y, x, y, nDist) && m_pMap->IsJumpEnable(temp.x, temp.y))
        {
            for(int j = 0; j < i; j++)
            {
                m_setPath.push(setPath[j]);
            }
            break;
        }
    }
    return m_setPath.size() > 0;
}

//////////////////////////////////////////////////////////////////////
bool CRoleMove::FindPath(Array<int>* psetPath, int x, int y)
{
    char	buf[sizeof(PASSPATH0) + _MAX_PATHSTEP];
    PASSPATH0* pPath = (PASSPATH0*)buf;
    POINT	posTarget;
    posTarget.x	= x;
    posTarget.y	= y;
    int	nEscapeSteps = 0;		// forward but back
    m_pMap->QueryMapData()->FindPath(pPath, sizeof(buf), m_pos, posTarget, m_pOwner->QueryRoleAttr(), nEscapeSteps);
    m_posTarget		= pPath->posStop;
    for(int i = 0; i < pPath->nSize; i++)
    {
        psetPath->Push(pPath->pPath[i]);
    }
    return psetPath->Size() > 0;
}

//////////////////////////////////////////////////////////////////////
void CRoleMove::SetPos(int nPosX, int nPosY)
{
    ASSERT(!m_bLock);
    GetMap()->MoveTo(Cast<INpc>(m_pOwner), nPosX, nPosY);
    ClearMove();
    m_pos.x		= nPosX;
    m_pos.y		= nPosY;
}

//////////////////////////////////////////////////////////////////////
POINT CRoleMove::FindRandomPos()
{
    int nSize = JUMP_DISTANCE * 2 + 1;
    POINT	pos;
    pos.x		= GetPosX() + ::RandGet(nSize) - JUMP_DISTANCE;
    pos.y		= GetPosY() + ::RandGet(nSize) - JUMP_DISTANCE;
    return pos;
}

//////////////////////////////////////////////////////////////////////
void CRoleMove::FlyMap(OBJID idMap, int nPosX, int nPosY)
{
    CMapPtr pMap = GetMap();
    CMapPtr pTarget = MapManager()->QueryMap(idMap);
    if (!pMap || !pTarget)
    {
        return ;
    }
    IF_NOT(pTarget->IsValidPoint(nPosX, nPosY))
    return ;
    ClearMove();
    pMap->LeaveMap(Cast<INpc>(m_pOwner));
    m_pMap		= pTarget;
    m_pos.x		= nPosX;
    m_pos.y		= nPosY;
    pTarget->EnterMap(Cast<INpc>(m_pOwner));
}
