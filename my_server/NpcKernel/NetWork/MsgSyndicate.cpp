
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgBetOutside.cpp: implementation of the CMsgAtk class.
// ÏÉ½£ÐÞ£¬2002.1.8
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "basefunc.h"
#include "NpcWorld.h"
#include "SynManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSyndicate::CMsgSyndicate()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgSyndicate::~CMsgSyndicate()
{
}

//////////////////////////////////////////////////////////////////////
BOOL	CMsgSyndicate::Create(unsigned short usAction, OBJID idTarget)
{
    // check param
    if (usAction == 0 || idTarget == ID_NONE)
    {
        ASSERT(!"CMsgSyndicate::Create()");
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_SYNDICATE;
    m_pInfo->usAction	= usAction;
    m_pInfo->idTarget	= idTarget;
    return true;
}

BOOL	CMsgSyndicate::Create		(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_SYNDICATE != this->GetType())
    {
        return false;
    }
    return true;
}

void	CMsgSyndicate::Process		(CGameSocket* pSocket)
{
    OBJID idSyn = GetNpcID();		// npc id is syn id.
    CHECK(idSyn != ID_NONE);
    CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
    if (m_pInfo->usAction == NPCMSG_CREATE_SYN)
    {
        CHECK(!pSyn);
    }
    else if (!( m_pInfo->usAction == SET_WHITE_SYN || m_pInfo->usAction == SET_BLACK_SYN ))
    {
        CHECK(pSyn);
    }
    switch(m_pInfo->usAction)
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case	SET_ALLY:
        {
            pSyn->QueryAllySet()->push_back(m_pInfo->idTarget);
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case	CLEAR_ALLY:
        {
            for(int i = 0; i < pSyn->QueryAllySet()->size(); i++)
            {
                if (pSyn->QueryAllySet()->at(i) == m_pInfo->idTarget)
                {
                    pSyn->QueryAllySet()->erase(pSyn->QueryAllySet()->begin() + i);
                    break;		// mast out for
                }
            }
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case	SET_ANTAGONIZE:
        {
            pSyn->QueryEnemySet()->push_back(m_pInfo->idTarget);
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case	CLEAR_ANTAGONIZE:
        {
            for(int i = 0; i < pSyn->QueryEnemySet()->size(); i++)
            {
                if (pSyn->QueryEnemySet()->at(i) == m_pInfo->idTarget)
                {
                    pSyn->QueryEnemySet()->erase(pSyn->QueryEnemySet()->begin() + i);
                    break;		// mast out for
                }
            }
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case	NPCMSG_CREATE_SYN:
        {
            CSyndicate* pSyn = CSyndicate::CreateNew();
            IF_OK(pSyn)
            {
                IF_OK(pSyn->Create(idSyn, m_pInfo->idTarget))
                SynManager()->QuerySynSet()->AddObj(pSyn);
                else
                {
                    pSyn->ReleaseByOwner();
                }
            }
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case	NPCMSG_DESTROY_SYN:
        {
            SynManager()->QuerySynSet()->DelObj(idSyn);
        }
        break;
    default:
        return;
    }
}

