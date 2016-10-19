
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgTeamMember.cpp: implementation of the CMsgTeam class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "Hero.h"
#include "GameMsg.h"
#include "GameDataSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgTeamMember::CMsgTeamMember()
{
    Init();
    m_pInfo	= (MSG_Info* )m_bufMsg;
}

CMsgTeamMember::~CMsgTeamMember()
{
}

//--------------------------------------------------------------------
BOOL CMsgTeamMember::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    if ((unsigned short)dwMsgSize != GetSize(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_TEAMMEMBER != GetType(pbufMsg, dwMsgSize))
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}
//--------------------------------------------------------------------
void CMsgTeamMember::Process(void* pInfo)
{
    int i;
    for(i = 0; i < m_pInfo->ucAmount; i++)
    {
        if (!g_objHero.IsTeamMember(m_pInfo->setMember[i].id))
        {
            if (g_objHero.GetID() != m_pInfo->setMember[i].id)
            {
                // 提示之 ...
                char szMsg[128];
                const OBJID idJoin = 100114;
                sprintf(szMsg, g_objGameDataSet.GetStr(idJoin), m_pInfo->setMember[i].szName);
                g_objGameMsg.AddMsg(szMsg);
            }
        }
    }
    if (m_pInfo->ucAction == _MSG_TEAMMEMBER_DROP_ADDMEMBER)
    {
        g_objHero.ClearTeamMember();
    }
    for(i = 0; i < m_pInfo->ucAmount; i++)
    {
        TeamMemberInfo* pInfo = new TeamMemberInfo;
        MYASSERT(pInfo);
        pInfo->bActive = true;
        pInfo->dwFace = m_pInfo->setMember[i].dwFace / 10000 % 1000;
        pInfo->dwLook = m_pInfo->setMember[i].dwFace % 1000;
        pInfo->dwHp	= m_pInfo->setMember[i].usHp;
        pInfo->dwMaxHp = m_pInfo->setMember[i].usMaxHp;
        pInfo->id = m_pInfo->setMember[i].id;
        strcpy(pInfo->szName, m_pInfo->setMember[i].szName);
        pInfo->bActive = true;
        g_objHero.AddTeamMember(pInfo);
    }
    ::PostCmd(CMD_FLASHTEAM);
}
//--------------------------------------------------------------------
