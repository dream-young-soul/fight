
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// file name: heroEnemy.cpp

// include head file(s) here
#include "hero.h"
#include "GameMsg.h"
#include "allmsg.h"
#include "GamePlayerset.h"
#include "GameDataSet.h"

// the operate interface of Enemy to hero

///////////////////////////////////////////////////////////////

BOOL CHero::AddEnemy(CEnemy* pEnemy)
{
    if (!pEnemy)
    {
        return false;
    }
    m_setEnemy.push_back(pEnemy);
    {
        ::PostCmd(CMD_FLASHENEMYLIST);
        return true;
    }
}
///////////////////////////////////////////////////////////////

BOOL CHero::DelEnemy(OBJID id, BOOL bSendMsg )
{
    if (id == ID_NONE)
    {
        return false;
    }
    int nAmount = m_setEnemy.size();
    for(int i = 0; i < nAmount; i++)
    {
        CEnemy* pEnemy = m_setEnemy[i];
        if (pEnemy && pEnemy->GetID() == id)
        {
            if (bSendMsg)
            {
                CMsgFriend msgFriend;
                msgFriend.Create(_ENEMY_DEL, id, true, pEnemy->GetName());
                msgFriend.Send();
            }
            SAFE_DELETE(pEnemy);
            m_setEnemy.erase(m_setEnemy.begin() + i);
            ::PostCmd(CMD_FLASHENEMYLIST);
            return true;
        }
    }
    return false;
}
///////////////////////////////////////////////////////////////
int  CHero::GetEnemyAmount()
{
    return m_setEnemy.size();
}

///////////////////////////////////////////////////////////////
CEnemy* CHero::GetEnemy(OBJID id)
{
    if (id == ID_NONE)
    {
        return NULL;
    }
    int nAmount = m_setEnemy.size();
    for(int i = 0; i < nAmount; i++)
    {
        CEnemy* pEnemy = m_setEnemy[i];
        if (pEnemy && pEnemy->GetID() == id)
        {
            return pEnemy;
        }
    }
    return NULL;
}
///////////////////////////////////////////////////////////////

CEnemy* CHero::GetEnemyByIndex(int nIndex)
{
    if (nIndex < 0)
    {
        return NULL;
    }
    int nAmount = m_setEnemy.size();
    if (nIndex >= nAmount)
    {
        return NULL;
    }
    return m_setEnemy[nIndex];
}

///////////////////////////////////////////////////////////////
void CHero::ClearEnemyList()
{
    int nAmount = m_setEnemy.size();
    for(int i = 0; i < nAmount; i++)
    {
        CEnemy* pEnemy = m_setEnemy[i];
        SAFE_DELETE(pEnemy);
    }
    m_setEnemy.clear();
}

///////////////////////////////////////////////////////////////
CEnemy* CHero::GetEnemyByName(const char* pszName)
{
    if (!pszName)
    {
        return NULL;
    }
    int nAmount = this->GetEnemyAmount();
    for(int i = 0; i < nAmount; i++)
    {
        CEnemy* pEnemy = this->GetEnemyByIndex(i);
        if ((pEnemy) && (strcmp(pEnemy->GetName(), pszName) == 0 ))
        {
            return pEnemy;
        }
    }
    return NULL;
}
///////////////////////////////////////////////////////////////
void CHero::QueryEnemy	(OBJID id)
{
    if (m_bEnemyQueryBusy)
    {
        return;
    }
    // stary busy
    m_bEnemyQueryBusy = true;
    m_dwEnemyBeginQuery = ::TimeGet();
    m_idEnemyQuery = id;
    m_bEnemyQueryReturn = false;
    // send msg
    CMsgAction msg;
    if (msg.Create(this->GetID(), 0, 0, 0, actionQueryEnemyInfo, id))
    {
        msg.Send();
    }
}

///////////////////////////////////////////////////////////////
void CHero::ProcessEnemy()
{
    const DWORD dwTimeout = 8 * 1000;
    if (!m_bEnemyQueryBusy)
    {
        return;
    }
    if (m_bEnemyQueryReturn)
    {
        // is syndicatename ready?
        CEnemy* pEnemy = this->GetEnemy(m_idEnemyQuery);
        if (pEnemy)
        {
            if (strlen(pEnemy->GetSyndicateName()) != 0)
            {
                m_bEnemyQueryBusy = false;
                ::PostCmd(CMD_QUERYENEMY_OK);
            }
        }
    }
    else if (::TimeGet() - m_dwEnemyBeginQuery > dwTimeout)
    {
        m_bEnemyQueryBusy = false;
        m_idEnemyQuery = ID_NONE;
        ::PostCmd(CMD_QUERYENEMY_TIMEOUT);
    }
}
