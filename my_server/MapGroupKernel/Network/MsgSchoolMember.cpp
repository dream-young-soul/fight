
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgSchoolMember.cpp: implementation of the CMsgSchoolMember class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "MapGroup.h"
#include "UserManager.h"
//#pragma	warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSchoolMember::CMsgSchoolMember()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgSchoolMember::~CMsgSchoolMember()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSchoolMember::Create	(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_SCHOOLMEMBER != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSchoolMember::Create(UCHAR ucAction, ST_MEMBERINFO* pMember, UCHAR ucAmount)
{
    if (sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + ucAmount * sizeof(ST_MEMBERINFO) >= MAX_PACKETSIZE)
    {
        return false;
    }
    // init
    this->Init();
    // TODO: init data
    m_pInfo->ucAction	= ucAction;
    if (pMember)
    {
        m_pInfo->ucAmount	= ucAmount;
        memcpy(m_pInfo->setMember, pMember, sizeof(ST_MEMBERINFO)*ucAmount);
    }
    else
    {
        m_pInfo->ucAmount	= 0;
    }
    m_unMsgType	= _MSG_SCHOOLMEMBER;
    m_unMsgSize	= sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + m_pInfo->ucAmount * sizeof(ST_MEMBERINFO);
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSchoolMember::Append(UCHAR ucRelation, UCHAR ucStatus, OBJID idMember, const char* pszName)
{
    if (sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + (m_pInfo->ucAmount + 1)*sizeof(ST_MEMBERINFO) >= MAX_PACKETSIZE)
    {
        return false;
    }
    memset(&m_pInfo->setMember[m_pInfo->ucAmount], 0L, sizeof(ST_MEMBERINFO));
    m_pInfo->setMember[m_pInfo->ucAmount].ucRelation	= ucRelation;
    m_pInfo->setMember[m_pInfo->ucAmount].ucStatus		= ucStatus;
    m_pInfo->setMember[m_pInfo->ucAmount].idMember		= idMember;
    if (pszName)
    {
        ::SafeCopy(m_pInfo->setMember[m_pInfo->ucAmount].szName, pszName, _MAX_NAMESIZE);
    }
    ++m_pInfo->ucAmount;
    m_unMsgSize	= sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + m_pInfo->ucAmount * sizeof(ST_MEMBERINFO);
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSchoolMember::Append(UCHAR ucRelation, UCHAR ucStatus, CUser* pUser)
{
    CHECKF (pUser);
    if (sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + (m_pInfo->ucAmount + 1)*sizeof(ST_MEMBERINFO) >= MAX_PACKETSIZE)
    {
        return false;
    }
    memset(&m_pInfo->setMember[m_pInfo->ucAmount], 0L, sizeof(ST_MEMBERINFO));
    m_pInfo->setMember[m_pInfo->ucAmount].ucRelation	= ucRelation;
    m_pInfo->setMember[m_pInfo->ucAmount].ucStatus		= ucStatus;
    m_pInfo->setMember[m_pInfo->ucAmount].idMember		= pUser->GetID();
    ::SafeCopy(m_pInfo->setMember[m_pInfo->ucAmount].szName, pUser->GetName(), _MAX_NAMESIZE);
    ::SafeCopy(m_pInfo->setMember[m_pInfo->ucAmount].szMate, pUser->GetMate(), _MAX_NAMESIZE);
    m_pInfo->setMember[m_pInfo->ucAmount].ucLevel		= pUser->GetLev();
    m_pInfo->setMember[m_pInfo->ucAmount].ucProfession	= pUser->GetProfession();
    m_pInfo->setMember[m_pInfo->ucAmount].usPkValue		= pUser->GetPk();
    m_pInfo->setMember[m_pInfo->ucAmount].ucNobilityRank = pUser->GetNobilityRank();
    m_pInfo->setMember[m_pInfo->ucAmount].dwSynID_Rank	= (pUser->GetSynRankShow() << MASK_RANK_SHIFT) | (pUser->GetSynID()&MASK_SYNID);
    ++m_pInfo->ucAmount;
    m_unMsgSize	= sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + m_pInfo->ucAmount * sizeof(ST_MEMBERINFO);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgSchoolMember::Process(CGameSocket* pSocket)
{
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgSchoolMember");
#endif
    CUserPtr pUser = UserManager()->GetUser(this);
    if (!pUser)
    {
        if (IsNpcMsg())
        {
            return;
        }
        // TransmitMsg
        switch(m_pInfo->ucAction)
        {
        case MESSAGEBOARD_DELMEMBER:
            {
            }
            break;
        case MESSAGEBOARD_ADDMEMBER:
            {
                CMsgSchoolMember msg;
                msg.Create(MESSAGEBOARD_ADDMEMBER, NULL, 0);
                for (int i = 0; i < m_pInfo->ucAmount; i++)
                {
                    CUser* pTarget = UserManager()->GetUser(m_pInfo->setMember[i].idMember);
                    if (pTarget)
                    {
                        msg.Append(m_pInfo->setMember[i].ucRelation, MEMBERSTATUS_ONLINE, pTarget);
                    }
                }
                SendMsg(&msg);
            }
            break;
        case MESSAGEBOARD_UPDATEMEMBER:
            {
            }
            break;
        }
    }
}
