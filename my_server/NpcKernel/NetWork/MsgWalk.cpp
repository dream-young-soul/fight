
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAction.cpp: implementation of the CMsgWalk class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "npcworld.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgWalk::CMsgWalk()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgWalk::~CMsgWalk()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgWalk::Create(OBJID idPlayer, int nDir, int nMode /*= WALKMODE_MOVE*/)
{
    // param check
    if (idPlayer == ID_NONE)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_WALK;
    m_pInfo->idUser		= idPlayer;
    m_pInfo->ucDir		= nDir;
    m_pInfo->ucMode		= nMode;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgWalk::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_ACTION != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgWalk::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgWalk, idUser:%u, data:%u",
             m_pInfo->idUser,
             m_pInfo->dwData);
#endif
    if (IsSceneID(m_pInfo->idUser))
    {
        ASSERT(!"CMsgWalk");
    }
    else if (IsNpcID(m_pInfo->idUser))
    {
        //		CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idUser);
        //		if (pNpc)
        //			pNpc->MoveForward(m_pInfo->ucDir);
    }
    else
    {
        CUserPtr pUser = UserManager()->QueryUser(m_pInfo->idUser);
        if (pUser)
        {
            pUser->MoveForward(m_pInfo->ucDir);
            // run mode step 2
            if (m_pInfo->ucMode >= WALKMODE_RUN_DIR0 && m_pInfo->ucMode <= WALKMODE_RUN_DIR7)
            {
                pUser->MoveForward(m_pInfo->ucMode - WALKMODE_RUN_DIR0);
            }
#ifdef	PALED_DEBUG_X
            MSGBUF	szMsg;
            sprintf(szMsg, "MSGWALK£ºÍæ¼Ò£¬%s(%d,%d)", pUser->GetName(), pUser->GetPosX(), pUser->GetPosY());
            LOGWARNING(szMsg);
#endif
        }
        else
        {
#ifdef	PALED_DEBUG_X
            MSGBUF	szMsg;
            sprintf(szMsg, "MSGWALK£ºAGENT£¬%d(%d)", m_pInfo->idUser, m_pInfo->ucDir);
            LOGWARNING(szMsg);
#endif
        }
    }
}
