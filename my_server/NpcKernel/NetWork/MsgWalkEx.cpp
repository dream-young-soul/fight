
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAction.cpp: implementation of the CMsgWalkEx class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "npcworld.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgWalkEx::CMsgWalkEx()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgWalkEx::~CMsgWalkEx()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgWalkEx::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_WALKEX != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgWalkEx::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgWalkEx, idUser:%u, dir:%u, mode:%u",
             m_pInfo->idUser, m_pInfo->ucDir, m_pInfo->ucMode);
#endif
    if (IsSceneID(m_pInfo->idUser))
    {
        ASSERT(!"CMsgWalkEx");
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
            pUser->SetPos(m_pInfo->usPosX, m_pInfo->usPosY);
        }
    }
}
