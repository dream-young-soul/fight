
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NetCmd.cpp: implementation of the CNetCmd class.
//
//////////////////////////////////////////////////////////////////////

#include "NetCmd.h"
#include "MsgTaskDialog.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CNetCmd::SendCmd(int nIndex, const char* fmt, ...)
{
    if (fmt)
    {
        char szMsg[1024];
        vsprintf( szMsg, fmt, (char*) ((&fmt) + 1) );
        CMsgTaskDialog msg;
        if (msg.Create(0, szMsg, nIndex, MSGDIALOG_TASKID))
        {
            msg.Send();
        }
    }
}
//--------------------------------------------------------------------------------------
BOOL CNetCmd::SynDonate(DWORD dwMoney)
{
    CNetCmd::SendCmd(_ID_SYN_DONATE, "%u", dwMoney);
    return true;
}
BOOL CNetCmd::SynDemise(const char* pszName)
{
    if (!pszName)
    {
        return false;
    }
    CNetCmd::SendCmd(_ID_SYN_DEMISE, "%s", pszName);
    return true;
}
BOOL CNetCmd::SynUnAppoint(const char* pszName)
{
    if (!pszName)
    {
        return false;
    }
    CNetCmd::SendCmd(_ID_SYN_UNAPPOINT, "%s", pszName);
    return true;
}
BOOL CNetCmd::SynCreateSubsyn(OBJID idSubLeader, const char* pszSynName)
{
    if (!pszSynName)
    {
        return false;
    }
    CNetCmd::SendCmd(_ID_SYN_CREATE_SUBSYN, "%d %s", idSubLeader, pszSynName);
    return true;
}
BOOL CNetCmd::SynUpdateSubsyn(OBJID idSubLeader, const char* pszSynName)
{
    if (!pszSynName)
    {
        return false;
    }
    CNetCmd::SendCmd(_ID_SYN_UPDATE_SUBSYN_LEADER, "%d %s", idSubLeader, pszSynName);
    return true;
}
BOOL CNetCmd::SynAntagonize(const char* pszSynName)
{
    if (!pszSynName)
    {
        return false;
    }
    CNetCmd::SendCmd(_ID_SYN_ANTAGONIZE, "%s", pszSynName);
    return true;
}
BOOL CNetCmd::SynUnAntagonize(const char* pszSynName)
{
    if (!pszSynName)
    {
        return false;
    }
    CNetCmd::SendCmd(_ID_SYN_UNANTAGONIZE, "%s", pszSynName);
    return true;
}
BOOL CNetCmd::SynAlly(const char* pszSynName)
{
    if (!pszSynName)
    {
        return false;
    }
    CNetCmd::SendCmd(_ID_SYN_ALLY, "%s", pszSynName);
    return true;
}
BOOL CNetCmd::SynUnAlly(const char* pszSynName)
{
    if (!pszSynName)
    {
        return false;
    }
    CNetCmd::SendCmd(_ID_SYN_UNALLY, "%s", pszSynName);
    return true;
}
BOOL CNetCmd::SynFireMember(const char* pszMemberName)
{
    if (!pszMemberName)
    {
        return false;
    }
    CNetCmd::SendCmd(_ID_SYN_FIRE_MEMBER, "%s", pszMemberName);
    return true;
}
BOOL CNetCmd::SynMoveMember(const OBJID idMember, const char* pszSubSynName)
{
    if (!pszSubSynName)
    {
        return false;
    }
    CNetCmd::SendCmd(_ID_SYN_MOVE_MEMBER, "%u %s", idMember, pszSubSynName);
    return true;
}
BOOL CNetCmd::SynBuyEmblem(DWORD dwIndex)
{
    dwIndex = dwIndex + 1;
    CNetCmd::SendCmd(_ID_SYN_BUY_EMBLEM, "%u", dwIndex);
    return true;
}
//--------------------------------------------------------------------------------------
