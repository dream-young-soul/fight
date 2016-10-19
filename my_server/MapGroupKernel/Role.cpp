
//**********************************************************
// 代码编辑器
//**********************************************************

// Role.cpp: implementation of the CRole class.
//
//////////////////////////////////////////////////////////////////////

#include "Role.h"
#include "StatusOnce.h"
#include "StatusMore.h"
#include "MsgAction.h"
#include "User.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRole::CRole()
    : m_setStatus(NULL)
{
    m_idProcess		= ID_NONE;
    m_pMap			= NULL;
    m_nPosX			= 0;	//DEFAULT_LOGIN_POSX;
    m_nPosY			= 0;	//DEFAULT_LOGIN_POSY;
    m_nDir			= 0;
}

CRole::~CRole()
{
    // 由各个子类创建的，那么就由各个子类负责释放，这里只是重复检查避免出现内存泄漏
    ASSERT(!m_setStatus);
    SAFE_RELEASE (m_setStatus);
}

//////////////////////////////////////////////////////////////////////
bool CRole::IsInFan(POINT pos, POINT posSource, int nRange, int nWidth, POINT posCenter)
{
    CHECKF(nWidth > 0 && nWidth < 360);
    if (posCenter.x == GetPosX() && posCenter.y == GetPosY())
    {
        return false;
    }
    if (pos.x == posSource.x && pos.y == posSource.y)
    {
        return false;
    }
    if (GetDistance(pos.x, pos.y) > nRange)
    {
        return false;
    }
    float PI = static_cast<float>(3.1415926535);
    float fRadianDelta	= (PI * nWidth / 180) / 2;
    float fCenterLine	= GetRadian(GetPosX(), GetPosY(), posCenter.x, posCenter.y);
    float fTargetLine	= GetRadian(posSource.x, posSource.y, pos.x, pos.y);
    float fDelta	= fabs(fCenterLine - fTargetLine);
    if (fDelta <= fRadianDelta || fDelta >= 2 * PI - fRadianDelta)
    {
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
int	 CRole::AdjustDataEx(int nData, int nAdjust, int nMaxData/*=0*/)
{
    if (nAdjust >= ADJUST_PERCENT)
    {
        return MulDiv(nData, nAdjust - ADJUST_PERCENT, 100);
    }
    if (nAdjust <= ADJUST_SET)
    {
        return -1 * nAdjust + ADJUST_SET;
    }
    if (nAdjust == ADJUST_FULL)
    {
        ASSERT(nMaxData != 0);
        return nMaxData;
    }
    return nData + nAdjust;
}


//////////////////////////////////////////////////////////////////////
bool CRole::AttachStatus(IRole* pRole, int nStatus, int nPower, int nSecs, int nTimes)
{
    CGameMap* pMap = pRole->GetMap();
    IF_NOT (pMap)
    return false;
    if (STATUS_CRIME == nStatus
            && (pMap->IsPkField() || pMap->IsSynMap() || pMap->IsDeadIsland()))
    {
        return false;
    }
    // 净化结界 -- 对不良状态免疫
    if (pRole->QueryStatus(STATUS_TEAMCLEAN))
    {
        // 目前根据解除不良状态的魔法，只有这两种不良状态，如果有新的不良状态，可能需要同时修改两边
        if (CRole::IsBadlyStatus(nStatus))
        {
            return false;
        }
    }
    IStatus* pStatus = pRole->QueryStatus(nStatus);
    if (pStatus)
    {
        // 魔域状态替换新规则 —— 2005-04-21
        bool bChangeData = false;
        if (pStatus->GetPower() == nPower)	// power相同，直接替换
        {
            bChangeData = true;
        }
        else
        {
            int nMinPower = __min(nPower, pStatus->GetPower());
            int nMaxPower = __max(nPower, pStatus->GetPower());
            if (nPower <= ADJUST_SET)
            {
                // 新power <= -30000，一定替换
                bChangeData = true;
            }
            else
            {
                // 如果新power大于-30000，则只有旧power也大于-30000才需要考虑替换
                if (pStatus->GetPower() > ADJUST_SET)
                {
                    // 新旧power同为百分比或者具体数值，才考虑做替换
                    if (nMinPower >= ADJUST_PERCENT || nMaxPower < ADJUST_PERCENT)
                    {
                        if (nMinPower >= ADJUST_PERCENT + 100 || nMinPower > 0 && nMaxPower < ADJUST_PERCENT)
                        {
                            // 均为正影响的时候，取power大的状态
                            if (nPower > pStatus->GetPower())
                            {
                                bChangeData = true;
                            }
                        }
                        else if (nMaxPower < 0 || nMinPower > ADJUST_PERCENT && nMaxPower < ADJUST_PERCENT + 100)
                        {
                            // 均为负影响的时候，取power小的状态
                            if (nPower < pStatus->GetPower())
                            {
                                bChangeData = true;
                            }
                        }
                    }
                }
            }
        }
        if (bChangeData)
        {
            pStatus->ChangeData(nPower, nSecs, nTimes);
        }
        return true;
    }
    else
    {
        // 界结状态不允许叠加
        if (nStatus >= STATUS_TEAM_BEGIN && nStatus <= STATUS_TEAM_END)
        {
            for (int i = STATUS_TEAM_BEGIN; i <= STATUS_TEAM_END; i++)
            {
                pRole->QueryStatusSet()->DelObj(i);
            }
        }
        if (nTimes)		// 多次触发的状态
        {
            CStatusMore*	pNewStatus = CStatusMore::CreateNew();
            IF_OK(pNewStatus)
            {
                IF_OK(pNewStatus->Create(pRole, nStatus, nPower, nSecs, nTimes))
                {
                    pRole->QueryStatusSet()->AddObj(pNewStatus->QueryInterface());
                    pRole->SetStatus(nStatus, SYNCHRO_TRUE);
                    return true;
                }
                else
                {
                    pNewStatus->ReleaseByOwner();
                }
            }
        }
        else
        {
            CStatusOnce*	pNewStatus = CStatusOnce::CreateNew();
            IF_OK(pNewStatus)
            {
                IF_OK(pNewStatus->Create(pRole, nStatus, nPower, nSecs))
                {
                    pRole->QueryStatusSet()->AddObj(pNewStatus->QueryInterface());
                    pRole->SetStatus(nStatus, SYNCHRO_TRUE);
                    return true;
                }
                else
                {
                    pNewStatus->ReleaseByOwner();
                }
            }
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CRole::DetachWellStatus(IRole* pRole)
{
    CHECK(pRole);
    for(int i = pRole->QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
    {
        IStatus* pStatus =  pRole->QueryStatusSet()->GetObjByIndex(i);
        if (pStatus && pStatus->GetID() != STATUS_CRIME)
        {
            if (CRole::IsWellStatus(pStatus->GetID()))
            {
                pRole->QueryStatusSet()->DelObj(pStatus->GetID());
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CRole::DetachBadlyStatus(IRole* pRole)
{
    CHECK(pRole);
    for(int i =  pRole->QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
    {
        IStatus* pStatus =  pRole->QueryStatusSet()->GetObjByIndex(i);
        if (pStatus && pStatus->GetID() != STATUS_CRIME)
        {
            if (CRole::IsBadlyStatus(pStatus->GetID()))
            {
                pRole->QueryStatusSet()->DelObj(pStatus->GetID());
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CRole::IsWellStatus(int nStatus)
{
    switch (nStatus)
    {
    case	STATUS_DEFENCE1:
    case	STATUS_DEFENCE2:
    case	STATUS_DEFENCE3:
    case	STATUS_ATTACK:
    case	STATUS_MAGICDEFENCE:
    case	STATUS_REFLECT:
    case	STATUS_REFLECTMAGIC:
    case	STATUS_MAGICDAMAGE:
    case	STATUS_ATKSPEED:
    case	STATUS_LURKER:
        return true;
        break;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CRole::IsBadlyStatus(int nStatus)
{
    switch (nStatus)
    {
    case	STATUS_POISON:
    case	STATUS_FREEZE:
    case	STATUS_SMOKE:
    case	STATUS_DARKNESS:
    case	STATUS_PALSY:
    case	STATUS_SLOWDOWN2:
    case	STATUS_SYNCRIME:
        return true;
        break;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CRole::DetachStatus(IRole* pRole, int nStatus)
{
    CHECKF(pRole);
    for(int i =  pRole->QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
    {
        IStatus* pStatus =  pRole->QueryStatusSet()->GetObjByIndex(i);
        if (pStatus && pStatus->GetID() == nStatus)
        {
            pRole->QueryStatusSet()->DelObj(pStatus->GetID());
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CRole::DetachAllStatus(IRole* pRole)
{
    CHECK(pRole);
    for(int i =  pRole->QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
    {
        IStatus* pStatus =  pRole->QueryStatusSet()->GetObjByIndex(i);
        if (pStatus && pStatus->GetID() != STATUS_CRIME)
        {
            pRole->QueryStatusSet()->DelObj(pStatus->GetID());
        }
    }
}

//////////////////////////////////////////////////////////////////////
// 服务器主动瞬移到坐标
void CRole::ActiveTransPos(int nPosX, int nPosY)
{
    if (!GetMap() || !GetMap()->IsStandEnable(nPosX, nPosY))
    {
        return;
    }
    this->ClrAttackTarget();
    CUser* pUser = NULL;
    if (this->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        pUser->StopMine();
        pUser->DestroyBooth();
    }
    ProcessOnMove(MOVEMODE_SYNCHRO);
    TransPos(nPosX, nPosY) ;
    CMsgAction	msg;
    if (msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionTransPos, GetPosX(), GetPosY()))
    {
        SendMsg(&msg);
    }
    return;
}

//////////////////////////////////////////////////////////////////////
bool CRole::MoveToward(int nDir, bool bSync)
{
    m_nDir	= nDir % 8;
    int	nDirX	= _DELTA_X[m_nDir];
    int	nDirY	= _DELTA_Y[m_nDir];
    CMapPtr pMap = this->GetMap();
    CHECKF(pMap);
    int nNewPosX = GetPosX() + nDirX;
    int nNewPosY = GetPosY() + nDirY;
    IF_NOT(GetMap()->IsValidPoint(nNewPosX, nNewPosY))
    return false;
    CUser* pUser = NULL;
    if (GetMap() && !GetMap()->IsStandEnable(nNewPosX, nNewPosY)
            && this->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        SendSysMsg(STR_INVALID_COORDINATE);
        //		MSGBUF	szMsg;
        //		sprintf(szMsg, "玩家[%s]走到非法坐标[%d,%d,%d]。", GetName(), GetMapID(), nNewPosX, nNewPosY);
        //		::GmLogSave(szMsg);
        pUser->KickBack();
        //UserManager()->KickOutSocket(GetSocketID(), szMsg);
        return false;
    }
    CRole::DetachStatus(this, STATUS_HIDDEN);
    pMap->MoveTo(this, nNewPosX, nNewPosY, WITH_BLOCK, WITH_BLOCK);
    if (pUser)
    {
        pUser->StandRestart();
        pMap->ChangeRegion(pUser, nNewPosX, nNewPosY);
    }
    m_nPosX = nNewPosX;
    m_nPosY = nNewPosY;
    // 其它MapThing	-- !!!下面也需要根据
    {
        /*
        		int x,y,z;
        		IRole*	pRole = NULL;
        		FOR_9_BLOCKTHINGS(pMap, GetPosX(), GetPosY())
        		{
        			IMapThing* pTarget = pMap->QueryThingByIndex(x,y,z);
        			if (pTarget)
        			{
        				int	nDistX	= (pTarget->GetPosX() - this->GetPosX()) * nDirX;			// 前方玩家为正，后方为负。
        				int	nDistY	= (pTarget->GetPosY() - this->GetPosY()) * nDirY;			// 前方玩家为正，后方为负。
        				if (nDirX && nDistX == CELLS_PER_VIEW || nDirY && nDistY == CELLS_PER_VIEW)
        				{
        					pTarget->SendShow(this);
        					//				this->SendSysMsg("走了");//@

        					if (bSync && pTarget->QueryRole(IPP_OF(pRole)))		// 遇见的是角色
        						this->SendShow(pRole);
        				}
        			}
        		}
        */
    }
    if (bSync)
    {
        UpdateBroadcastSet();
    }
    ProcessAfterMove();
    return true;
}

//////////////////////////////////////////////////////////////////////
void CRole::TransPos(int nPosX, int nPosY)
{
    if (nPosX == GetPosX() && nPosY == GetPosY())
    {
        return;
    }
    CMapPtr pMap = this->GetMap();
    IF_NOT(pMap && pMap->IsValidPoint(nPosX, nPosY))
    return;
    CUser* pUser = NULL;
    if (GetMap() && !GetMap()->IsStandEnable(nPosX, nPosY)
            && this->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        SendSysMsg(STR_INVALID_COORDINATE);
        MSGBUF	szMsg;
        sprintf(szMsg, "玩家[%s]飞到非法坐标[%d,%d,%d]。", GetName(), GetMap()->GetID(), nPosX, nPosY);
        ::GmLogSave(szMsg);
        pUser->KickBack();
        //UserManager()->KickOutSocket(GetSocketID(), szMsg);
        return ;
    }
    CRole::DetachStatus(this, STATUS_HIDDEN);
    pMap->MoveTo(this, nPosX, nPosY, WITH_BLOCK, WITH_BLOCK);
    if (pUser)
    {
        pUser->StandRestart();
        pMap->ChangeRegion(pUser, nPosX, nPosY);
    }
    m_nPosX = nPosX;
    m_nPosY = nPosY;
    // 其它玩家
    //	if (IsSendBlockInfo())
    //		pMap->SendBlockInfo(this);
    //	SendSelfToBlock();
    ClrBroadcastSet();
    UpdateBroadcastSet();
    ProcessAfterMove();
}

//////////////////////////////////////////////////////////////////////
void CRole::JumpPos(int nPosX, int nPosY)
{
    if (nPosX == GetPosX() && nPosY == GetPosY())
    {
        return;
    }
    CMapPtr pMap = this->GetMap();
    IF_NOT(pMap && pMap->IsValidPoint(nPosX, nPosY))
    return;
    CUser* pUser = NULL;
    if (this->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        int	nDeltaX	= abs(nPosX - m_nPosX);
        int	nDeltaY	= abs(nPosY - m_nPosY);
        // 检查外挂
        if (nDeltaX > CELLS_PER_BLOCK || nDeltaY > CELLS_PER_BLOCK)
        {
            //		::GmLogSave("玩家[%s]跳的距离非法[%d][%d]。", GetName(), nDeltaX, nDeltaY);
            return ;
        }
        if (GetMap() && !GetMap()->IsStandEnable(nPosX, nPosY))
        {
            SendSysMsg(STR_INVALID_COORDINATE);
            //			MSGBUF	szMsg;
            //			sprintf(szMsg, "玩家[%s]跳到非法坐标[%d,%d,%d]。", GetName(), GetMapID(), nPosX, nPosY);
            //			::GmLogSave(szMsg);
            pUser->KickBack();
            //UserManager()->KickOutSocket(GetSocketID(), szMsg);
            return ;
        }
    }
    CRole::DetachStatus(this, STATUS_HIDDEN);
    pMap->MoveTo(this, nPosX, nPosY, WITH_BLOCK, WITH_BLOCK);
    if (pUser)
    {
        pUser->StandRestart();
        pMap->ChangeRegion(pUser, nPosX, nPosY);
    }
    int	nOldX	= m_nPosX;
    int	nOldY	= m_nPosY;
    m_nPosX = nPosX;
    m_nPosY = nPosY;
    // 其它玩家
    /*
    	int x,y,z;
    	IRole*	pRole = NULL;
    	FOR_9_BLOCKTHINGS(pMap, GetPosX(), GetPosY())
    	{
    		IMapThing* pTarget = pMap->QueryThingByIndex(x,y,z);
    		CUser* pTargetUser = NULL;
    		if (pTarget &&
    			(pUser													// 玩家，一定要发
    			|| pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser))))	// 非玩家，仅在看到玩家才发
    		{
    			if (abs(pTarget->GetPosX() - nOldX) > CELLS_PER_VIEW
    				&& abs(pTarget->GetPosX() - this->GetPosX()) <= CELLS_PER_VIEW
    				|| abs(pTarget->GetPosY() - nOldY) > CELLS_PER_VIEW
    				&& abs(pTarget->GetPosY() - this->GetPosY()) <= CELLS_PER_VIEW )
    			{
    				pTarget->SendShow(this);
    				//				this->SendSysMsg("跳了");//@

    				if (pTarget->QueryRole(IPP_OF(pRole)))
    					this->SendShow(pRole);
    			}
    		}
    	}
    */
    ClrBroadcastSet();
    UpdateBroadcastSet();
    ProcessAfterMove();
}

//////////////////////////////////////////////////////////////////////
bool CRole::SyncTrackTo(int nPosX, int nPosY, int nDir, DWORD dwAction)
{
    m_nDir	= nDir % 8;
    if (GetPosX() != nPosX || GetPosY() != nPosY)
    {
        CMapPtr pMap = this->GetMap();
        IF_NOT(pMap && pMap->IsValidPoint(nPosX, nPosY))
        return false;
        if (GetMap() && !GetMap()->IsStandEnable(nPosX, nPosY))
        {
            //SendSysMsg(STR_INVALID_COORDINATE);
            //::LogSave("Invalid coordinate (%d, %d)", nPosX, nPosY);
            return false;
        }
        CRole::DetachStatus(this, STATUS_HIDDEN);
        pMap->MoveTo(this, nPosX, nPosY, WITH_BLOCK, WITH_BLOCK);
        CUser* pUser = NULL;
        if (this->QueryObj(OBJ_USER, IPP_OF(pUser)))
        {
            pUser->StandRestart();
            pMap->ChangeRegion(pUser, nPosX, nPosY);
        }
        //		int	nOldX	= m_nPosX;
        //		int	nOldY	= m_nPosY;
        m_nPosX	= nPosX;
        m_nPosY	= nPosY;
        /*
        		int x,y,z;
        		IRole*	pRole = NULL;
        		FOR_9_BLOCKTHINGS(pMap, GetPosX(), GetPosY())
        		{
        			IMapThing* pTarget = pMap->QueryThingByIndex(x,y,z);
        			CUser* pTargetUser = NULL;
        			if (pTarget &&
        				(pUser													// 玩家，一定要发
        				|| pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser))))	// 非玩家，仅在看到玩家才发
        			{
        				if (abs(pTarget->GetPosX() - nOldX) > CELLS_PER_VIEW
        					&& abs(pTarget->GetPosX() - this->GetPosX()) <= CELLS_PER_VIEW
        					|| abs(pTarget->GetPosY() - nOldY) > CELLS_PER_VIEW
        					&& abs(pTarget->GetPosY() - this->GetPosY()) <= CELLS_PER_VIEW )
        				{
        					pTarget->SendShow(this);

        					if (pTarget->QueryRole(IPP_OF(pRole)))		// 遇见的是角色
        						this->SendShow(pRole);
        				}
        			}
        		}
        */
    }
    UpdateBroadcastSet();
    //	ProcessAfterMove();
    // 同步轨迹
    CMsgAction	msg;
    IF_OK (msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionMagicTrack, dwAction))
    BroadcastRoomMsg(&msg, true);
    return true;
}


//////////////////////////////////////////////////////////////////////
bool CRole::UpdateBroadcastSet(bool bClearSet/*=false*/)
{
    CHECKF (GetMap());
    CUser* pUser = NULL;
    QueryObj(OBJ_USER, IPP_OF(pUser));
    bool	bIsCallPet = false;
    CMonster* pMonster;
    if (QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
    {
        if (pMonster->IsCallPet() || pMonster->IsEudemon())
        {
            bIsCallPet = true;
        }
    }
    // 寻找新的目标集
    typedef std::vector<IRole*>		ROLE_SET;
    ROLE_SET		setNewRole;
    BROADCAST_SET	setNewMapItem;
    int x, y, z;
    IRole*	pRole = NULL;
    FOR_9_BLOCKTHINGS(GetMap(), GetPosX(), GetPosY())
    {
        IMapThing* pTarget = GetMap()->QueryThingByIndex(x, y, z);
        if (!pTarget)
        {
            continue;
        }
        if (abs(pTarget->GetPosX() - GetPosX()) <= CELLS_PER_VIEW
                && abs(pTarget->GetPosY() - GetPosY()) <= CELLS_PER_VIEW)
        {
            if (pTarget->QueryRole(IPP_OF(pRole)))
            {
                CUser* pTargetUser = NULL;
                if (pRole->GetID() != this->GetID()
                        && (bIsCallPet || pUser || pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser))
                            || (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster))
                                && (pMonster->IsCallPet() || pMonster->IsEudemon()))))
                {
                    // 自己是玩家、幻兽，或者目标是玩家、幻兽
                    setNewRole.push_back(pRole);
                }
            }
            else if (pUser)	// 地图上的物品不需要发给NPC服务器
            {
                setNewMapItem.push_back(pTarget->GetID());
            }
        }
    }
    // 清除原有目标集 —— 重新搜索所有目标
    //	if (bClearSet)
    //	{
    //		m_setBCRole.clear();
    //		m_setBCMapItem.clear();
    //	}
    //	else
    {
        // 遍历原目标集，删除不在新目标集中的id，压入已删除队列
        // 同时也删除新目标集中原来已经存在的id
        BROADCAST_SET::iterator it = m_setBCRole.begin();
        for (; it != m_setBCRole.end(); )
        {
            OBJID id = *it;
            bool  bMatch = false;
            ROLE_SET::iterator iter = setNewRole.begin();
            for (; iter != setNewRole.end(); iter++)
            {
                IRole* pRole = *iter;
                if (pRole && pRole->GetID() == id)
                {
                    bMatch = true;
                    setNewRole.erase(iter);
                    break;
                }
            }
            BROADCAST_SET::iterator itErase = it;
            ++it;
            if (!bMatch)
            {
                m_setBCRole.erase(itErase);
                // 通知自己对方消失
                RemoveFromBCRoleSet(id);
                // 通知对方自己消失
                IRole* pRole = RoleManager()->QueryRole(id);
                if (pRole)
                {
                    pRole->RemoveFromBCRoleSet(this->GetID());
                }
            }
        }
        for (it = m_setBCMapItem.begin(); it != m_setBCMapItem.end(); )
        {
            OBJID id = *it;
            bool bMatch = false;
            BROADCAST_SET::iterator iter = setNewMapItem.begin();
            for (; iter != setNewMapItem.end(); iter++)
            {
                if (id == *iter)
                {
                    bMatch = true;
                    setNewMapItem.erase(iter);
                    break;
                }
            }
            BROADCAST_SET::iterator itErase = it;
            ++it;
            if (!bMatch)
            {
                m_setBCMapItem.erase(itErase);
                RemoveFromBCMapItemSet(id);
            }
        }
    }
    // 将整理后的新旧目标集合并
    CUser* pTargetUser = NULL;
    if (!setNewRole.empty())
    {
        ROLE_SET::iterator it = setNewRole.begin();
        for (; it != setNewRole.end(); it++)
        {
            IRole* pRole = *it;
            if (pRole)
            {
                // 将对方加入自己的目标集，同时也将自己加入对方目标集
                // 自己是玩家，或者自己是NPC且对方是玩家或幻兽的时候才需要发送
                bool bSendSelf = (pUser ||
                                  (!bIsCallPet && (pRole->QueryObj(OBJ_USER, IPP_OF(pTargetUser))
                                                   || (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster))
                                                       && (pMonster->IsCallPet() || pMonster->IsEudemon())))));
                AddToBCRoleSet(pRole->GetID(), bSendSelf);
                // 对方不是幻兽，且，自己是玩家或幻兽或者对方是玩家的时候才需要发送
                pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster));
                bool bSendTarget = ((!pMonster || (!pMonster->IsCallPet() && !pMonster->IsEudemon()))
                                    && (pUser || bIsCallPet || pRole->QueryObj(OBJ_USER, IPP_OF(pTargetUser))));
                pRole->AddToBCRoleSet(GetID(), bSendTarget);
            }
        }
    }
    if (!setNewMapItem.empty())
    {
        BROADCAST_SET::iterator it = setNewMapItem.begin();
        for (; it != setNewMapItem.end(); it++)
        {
            AddToBCMapItemSet(*it);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CRole::ClrBroadcastSet()
{
    BROADCAST_SET::iterator it = m_setBCRole.begin();
    for (; it != m_setBCRole.end(); it++)
    {
        OBJID id = *it;
        // 通知对方自己消失
        IRole* pRole = RoleManager()->QueryRole(id);
        if (pRole)
        {
            pRole->RemoveFromBCRoleSet(this->GetID());
        }
    }
    m_setBCRole.clear();
    m_setBCMapItem.clear();
}

//////////////////////////////////////////////////////////////////////
void CRole::AddToBCRoleSet(OBJID idRole, bool bSendMsg)
{
    BROADCAST_SET::iterator it = m_setBCRole.begin();
    for (; it != m_setBCRole.end(); it++)
    {
        if (idRole == *it)
        {
            return;
        }
    }
    m_setBCRole.push_back(idRole);
    if (bSendMsg)
    {
        IRole* pRole =  RoleManager()->QueryRole(idRole);
        if (pRole)
        {
            pRole->QueryMapThing()->SendShow((IRole*)this);
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CRole::RemoveFromBCRoleSet(OBJID idRole)
{
    //NPC服务器机制暂时不改，这里只需通知客户端玩家
    CUser* pUser = NULL;
    if (QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        // 通知自己对方消失
        CMsgAction msg;
        if (msg.Create(idRole, 0, 0, 0, actionDisappear, 0))
        {
            SendMsg(&msg);
        }
    }
    BROADCAST_SET::iterator it = m_setBCRole.begin();
    for (; it != m_setBCRole.end(); it++)
    {
        if (idRole == *it)
        {
            m_setBCRole.erase(it);
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CRole::AddToBCMapItemSet(OBJID idMapItem)
{
    BROADCAST_SET::iterator it = m_setBCMapItem.begin();
    for (; it != m_setBCMapItem.end(); it++)
    {
        if (idMapItem == *it)
        {
            return;
        }
    }
    m_setBCMapItem.push_back(idMapItem);
    CMapItem* pMapItem = MapManager()->QueryMapItem(idMapItem);
    if (pMapItem)
    {
        pMapItem->QueryMapThing()->SendShow(this);
    }
}

//////////////////////////////////////////////////////////////////////
void CRole::RemoveFromBCMapItemSet(OBJID idMapItem)
{
    //NPC服务器机制暂时不改，这里只需通知客户端
    CUser* pUser = NULL;
    if (QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        CMsgAction msg;
        if (msg.Create(idMapItem, 0, 0, 0, actionDisappear, 1))
        {
            SendMsg(&msg);
        }
    }
    BROADCAST_SET::iterator it = m_setBCMapItem.begin();
    for (; it != m_setBCMapItem.end(); it++)
    {
        if (idMapItem == *it)
        {
            m_setBCMapItem.erase(it);
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CRole::BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf)
{
    CHECK(pMsg);
    CGameMap* pMap = GetMap();
    CHECK (pMap);
    CUser*	pUser		= NULL;
    IRole*	pRole		= NULL;
    CMonster* pMonster	= NULL;
    bool bSendToNpc	= (QueryObj(OBJ_USER, IPP_OF(pUser))
                       || (QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && (pMonster->IsCallPet() || pMonster->IsEudemon()))
                       || bSendSelf);
    if (pMap->IsNewbieMap() && bSendSelf)
    {
        SendMsg(pMsg);
        return;
    }
    if (bSendSelf)
    {
        m_setBCRole.push_back(this->GetID());
    }
    DEBUG_TRY
    BROADCAST_SET::iterator it = m_setBCRole.begin();
    for (; it != m_setBCRole.end(); it++)
    {
        OBJID idRole = *it;
        IRole* pRole = RoleManager()->QueryRole(idRole);
        if (pRole)
        {
            if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && bSendToNpc)
            {
                if (!pMonster->IsCallPet() && !pMonster->IsEudemon())
                {
                    pRole->SendMsg(pMsg);
                    bSendToNpc = false;
                }
            }
            if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
            {
                pRole->SendMsg(pMsg);
                if (pUser->IsAgent() && bSendToNpc)
                {
                    bSendToNpc = false;
                }
            }
        }
    }
    DEBUG_CATCH("CRole::BroadcastRoomMsg")
    if (bSendSelf)
    {
        m_setBCRole.pop_back();
    }
}
