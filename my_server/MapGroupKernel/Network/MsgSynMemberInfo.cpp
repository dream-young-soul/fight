
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgSynMemberInfo.cpp: interface for the CMsgSynMemberInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "SynAttr.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgSynMemberInfo::CMsgSynMemberInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgSynMemberInfo::~CMsgSynMemberInfo()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSynMemberInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_SYNMEMBERINFO != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSynMemberInfo::Create(CUser* pUser)
{
    CHECKF(pUser);
    // init
    this->Init();
    // fill info now
    SafeCopy(m_pInfo->szName, pUser->GetName(), _MAX_NAMESIZE);
    SafeCopy(m_pInfo->szMate, pUser->GetMate(), _MAX_NAMESIZE);
    if (pUser->QuerySynAttr())
    {
        m_pInfo->ucRank			= pUser->QuerySynAttr()->GetSynRankShow();
        m_pInfo->nProffer		= pUser->QuerySynAttr()->GetProffer();
        m_pInfo->sPk			= pUser->GetPk();
        m_pInfo->ucProfession	= pUser->GetProfession();
        m_pInfo->ucNobilityRank	= pUser->GetNobilityRank();
        m_pInfo->ucLevel		= pUser->GetLev();
        m_pInfo->dwLookFace		= pUser->GetLookFace();
    }
    m_unMsgType	= _MSG_SYNMEMBERINFO;
    m_unMsgSize	= sizeof(MSG_Info);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgSynMemberInfo::Process(void* pInfo)
{
    CUser* pUser = UserManager()->GetUser(this);
    if (!pUser)
    {
        // TransmitMsg
        CUser* pTargetUser = UserManager()->GetUser(m_pInfo->szName);
        if (pTargetUser)
        {
            CHECK(pTargetUser->GetSynID() != ID_NONE);
            IF_OK(Create(pTargetUser))
            SendMsg(this);
        }
        return ;
    }
    if (pUser->GetSynID() == ID_NONE)
    {
        return ;
    }
    CUser* pTargetUser = UserManager()->GetUser(m_pInfo->szName);
    if (pTargetUser)
    {
        CHECK(pTargetUser->GetSynID() != ID_NONE);
        IF_OK(Create(pTargetUser))
        SendMsg(this);
    }
    else
    {
        MapGroup(PID)->QueryIntraMsg()->TransmitMsg(this, GetSocketID(), GetNpcID());
    }
}
