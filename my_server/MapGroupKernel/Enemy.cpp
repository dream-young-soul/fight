
//**********************************************************
// 代码编辑器
//**********************************************************

// Enemy.cpp: implementation of the CEnemy class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "define.h"
#include "Enemy.h"
#include "i_mydb.h"
#include "User.h"
#include "AllMsg.h"
#include "MapGroup.h"

MYHEAP_IMPLEMENTATION(CEnemy, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnemy::CEnemy()
{
}

CEnemy::~CEnemy()
{
}

//////////////////////////////////////////////////////////////////////
bool CEnemy::Create(PROCESS_ID idProcess, CUser* pOwner, IDatabase* pDb)
{
    m_idProcess	= idProcess;
    m_pOwner	= pOwner;
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT * FROM %s WHERE userid=%d ORDER BY time", _TBL_ENEMY, m_pOwner->GetID());
    IRecordset*	pRes = pDb->CreateNewRecordset(szSQL, false);
    if (pRes)
    {
        for(int i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
        {
            NAMESTR	szName;
            pRes->LoadString(szName, "enemyname", _MAX_NAMESIZE);
            Add(pRes->LoadDWord("enemy"), szName, SYNCHRO_FALSE, UPDATE_FALSE);
        }
        pRes->Release();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CEnemy::Create(PROCESS_ID idProcess, CUser* pOwner)
{
    m_idProcess	= idProcess;
    m_pOwner	= pOwner;
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CEnemy::GetInfo(CEnemyInfoStruct* pInfo)
{
    CHECKF(pInfo);
    memset(pInfo, 0, sizeof(CEnemyInfoStruct));
    for(int i = 0; i < MAX_ENEMYSIZE && i < m_set.Size(); i++)
    {
        pInfo->setEnemy[i] = m_set[i].first;
        SafeCopy(pInfo->setName[i], m_set[i].second, _MAX_NAMESIZE);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CEnemy::AppendInfo(const CEnemyInfoStruct* pInfo)
{
    CHECKF(pInfo);
    for(int i = 0; i < MAX_ENEMYSIZE; i++)
    {
        if (pInfo->setEnemy[i])
        {
            m_set.Push(make_pair(pInfo->setEnemy[i], String(pInfo->setName[i])));
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CEnemy::Add(OBJID idEnemy, LPCTSTR szName, bool bSynchro, bool bUpdate)
{
    bool	bInsertNew = true;
    ENEMY_SET::Iterator pEnemy = m_set.NewEnum();
    while(pEnemy.Next())
    {
        if (pEnemy && pEnemy->first == idEnemy)
        {
            m_set.DelObj(pEnemy);
            bInsertNew	= false;
        }
    }
    m_set.Push(make_pair(idEnemy, String(szName)));
    if (bSynchro)
    {
        CMsgFriend	msg;
        IF_OK(msg.Create(_ENEMY_ADD, idEnemy, szName, true))		// true: online
        m_pOwner->SendMsg(&msg);
    }
    if (bUpdate)
    {
        if (bInsertNew)
        {
            SQLBUF	szSQL;
            sprintf(szSQL, "INSERT %s VALUES(0, %u, %u, '%s', %u)", _TBL_ENEMY, m_pOwner->GetID(), idEnemy, szName, time(NULL));
            Database()->ExecuteSQL(szSQL);
        }
        else
        {
            SQLBUF	szSQL;
            sprintf(szSQL, "UPDATE %s SET time=%u WHERE userid=%u && enemy=%u LIMIT 1", _TBL_ENEMY, time(NULL), m_pOwner->GetID(), idEnemy);
            Database()->ExecuteSQL(szSQL);
        }
    }
    if (m_set.Size() > MAX_ENEMYSIZE)
    {
        Del(m_set[0].first, UPDATE_TRUE);
    }
}

//////////////////////////////////////////////////////////////////////
void CEnemy::Del(OBJID idEnemy, bool bUpdate)
{
    ENEMY_SET::Iterator pEnemy = m_set.NewEnum();
    while(pEnemy.Next())
    {
        if (pEnemy && pEnemy->first == idEnemy)
        {
            m_set.DelObj(pEnemy);
            // synchro
            {
                CMsgFriend	msg;
                IF_OK_(msg.Create(_ENEMY_DEL, idEnemy, ""))
                m_pOwner->SendMsg(&msg);
            }
            if (bUpdate)
            {
                SQLBUF	szSQL;
                sprintf(szSQL, "DELETE FROM %s WHERE userid=%u && enemy=%u LIMIT 1", _TBL_ENEMY, m_pOwner->GetID(), idEnemy);
                Database()->ExecuteSQL(szSQL);
            }
            return ;
        }
    }
}

//////////////////////////////////////////////////////////////////////
LPCTSTR CEnemy::GetName(OBJID idEnemy)
{
    ENEMY_SET::Iterator pEnemy = m_set.NewEnum();
    while(pEnemy.Next())
    {
        if (pEnemy && pEnemy->first == idEnemy)
        {
            return pEnemy->second;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
void CEnemy::BeforeLogout()
{
    CMsgFriend	msg;
    IF_NOT(msg.Create(_ENEMY_OFFLINE, m_pOwner->GetID(), m_pOwner->GetName()))
    return;
    ENEMY_SET::Iterator pEnemy = m_set.NewEnum();
    pEnemy.Reset();
    while(pEnemy.Next())
    {
        OBJID	idEnemy	= pEnemy->first;
        CUser* pUser = UserManager()->GetUser(idEnemy);
        if (pUser)
        {
            pUser->SendMsg(&msg);
        }
        else
        {
            MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, idEnemy);
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CEnemy::SendToClient()
{
    ENEMY_SET::Iterator pEnemy = m_set.NewEnum();
    while(pEnemy.Next())
    {
        OBJID	idEnemy	= pEnemy->first;
        bool	bOnline = (UserManager()->GetUser(idEnemy) != NULL);
        CMsgFriend	msg;
        if (msg.Create(_ENEMY_ADD, idEnemy, pEnemy->second, bOnline))
        {
            m_pOwner->SendMsg(&msg);
        }
        if (!bOnline)
        {
            MapGroup(PID)->QueryIntraMsg()->TransmitMsg(&msg, m_pOwner->GetSocketID(), m_pOwner->GetID());    // 通知其它地图组，下传给我好友在线的消息
        }
    }
    // broadcast self online to all enemy
    pEnemy.Reset();
    while(pEnemy.Next())
    {
        OBJID	idFriend	= pEnemy->first;
        CMsgFriend	msg;
        IF_OK(msg.Create(_ENEMY_ONLINE, m_pOwner->GetID(), m_pOwner->GetName()))
        MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, idFriend);
    }
}
