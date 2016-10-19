
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// SupermanList.cpp: implementation of the CSupermanList class.
//
//////////////////////////////////////////////////////////////////////

#define	_WINSOCKAPI_		// ×èÖ¹¼ÓÔØwinsock.h
#include "SupermanList.h"
#include "AllMsg.h"
#include "I_MessagePort.h"
#include "define.h"
#include "MessageBoard.h"
#include "UserList.h"
#include "SynWorldShell.h"
#include "WorldKernel.h"

const int	SUPERMAN_GROUP1			= 3;
const int	SUPERMAN_GROUP2			= 50;
const int	SUPERMAN_SIZE			= 100;
enum
{
    RANK_0	= 0,
    RANK_3	= 1,
    RANK_2	= 2,
    RANK_1	= 3,
};
//////////////////////////////////////////////////////////////////////
int RankOfIndex(int nPos)
{
    int nRank = RANK_0;
    if (nPos >= 0 && nPos < SUPERMAN_GROUP1)
    {
        nRank = RANK_1;
    }
    else if (nPos >= SUPERMAN_GROUP1 && nPos < SUPERMAN_GROUP2)
    {
        nRank = RANK_2;
    }
    else if (nPos >= SUPERMAN_GROUP2 && nPos < SUPERMAN_SIZE)
    {
        nRank = RANK_3;
    }
    return nRank;
}

MYHEAP_IMPLEMENTATION(CSupermanList, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSupermanList::CSupermanList()
{
}

//////////////////////////////////////////////////////////////////////
CSupermanList::~CSupermanList()
{
}

//////////////////////////////////////////////////////////////////////
bool CSupermanList::Create(IDatabase* pDb)
{
    char szSQL[1024];
    sprintf(szSQL, "SELECT s.id,s.number,u.name FROM %s AS s LEFT JOIN %s AS u ON u.id=s.id ORDER BY number DESC LIMIT 100", _TBL_SUPERMAN, _TBL_USER);
    IRecordset* pRes = pDb->CreateNewRecordset(szSQL, false);
    if (pRes)
    {
        int	nAmount = pRes->RecordCount();
        for(int i = 0; i < nAmount; i++, pRes->MoveNext())
        {
            SUPERMAN_INFO	info;
            info.id			= pRes->LoadDWord("id");
            info.dwNumber	= pRes->LoadDWord("number");
            pRes->LoadString(info.szName, "name", _MAX_NAMESIZE);
            m_setUser.push_back(info);
        }
        pRes->Release();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSupermanList::AddMember(OBJID idUser, int nNumber)
{
    CPlayer* pUser = UserList()->GetPlayer(idUser);
    CHECKF(pUser);
    if (m_setUser.size() >= SUPERMAN_SIZE)
    {
        int nMin = m_setUser[m_setUser.size() - 1].dwNumber;
        if (nNumber <= nMin)
        {
            return false;
        }
    }
    int nOldPos = -1, nNewPos = -1;
    int nAmount = m_setUser.size();
    for(int i = 0; i < nAmount; i++)
    {
        if (nOldPos == -1 && m_setUser[i].id == idUser)
        {
            nOldPos	= i;
        }
        if (nNewPos == -1 && m_setUser[i].dwNumber < nNumber)
        {
            nNewPos	= i;
        }
    }
    int nOldRank = RankOfIndex(nOldPos);
    if (nOldPos != -1)
    {
        m_setUser.erase(m_setUser.begin() + nOldPos);
        if (nOldPos < nNewPos)
        {
            nNewPos--;
        }
    }
    if (nNewPos == -1)
    {
        nNewPos	= m_setUser.size();
    }
    int nNewRank = RankOfIndex(nNewPos);
    SUPERMAN_INFO	info;
    info.id			= idUser;
    info.dwNumber	= nNumber;
    SafeCopy(info.szName, pUser->szName, _MAX_NAMESIZE);
    m_setUser.insert(m_setUser.begin() + nNewPos, info);
    if (m_setUser.size() > SUPERMAN_SIZE)
    {
        SynchroInfo(m_setUser[SUPERMAN_SIZE].id, RANK_0);
        DeleteInfo(m_setUser[SUPERMAN_SIZE].id);
        m_setUser.resize(SUPERMAN_SIZE);
    }
    if (nOldRank != nNewRank)
    {
        SynchroInfo(idUser, nNewRank);
        if (nNewRank == RANK_1 && m_setUser.size() > SUPERMAN_GROUP1)
        {
            SynchroInfo(m_setUser[SUPERMAN_GROUP1].id, RANK_2);
        }
        if ((nNewRank == RANK_1 && nOldRank < RANK_2 || nNewRank == RANK_2) && m_setUser.size() > SUPERMAN_GROUP2)
        {
            SynchroInfo(m_setUser[SUPERMAN_GROUP2].id, RANK_3);
        }
    }
    if (nOldRank == RANK_0)
    {
        InsertInfo(idUser, nNumber, pUser->szName);
    }
    else
    {
        UpdateInfo(idUser, nNumber);
    }
    if (nNewRank > nOldRank)
    {
        MSGBUF	szMsg;
        sprintf(szMsg, STR_SUPERMAN_BROADCAST_sddd, pUser->szName, nNumber, nNewPos + 1, nNewRank);		// +1: 0 base to 1 base
        CMsgTalk	msg;
        IF_OK(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg))
        UserList()->BroadcastMsg(&msg);
    }
    else //if (nNewPos < nOldPos)
    {
        MSGBUF	szMsg;
        sprintf(szMsg, STR_SUPERMAN_BROADCAST_sddd, pUser->szName, nNumber, nNewPos + 1, nNewRank);		// +1: 0 base to 1 base
        CMsgTalk	msg;
        IF_OK(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg))
        pUser->SendMsg(&msg);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CSupermanList::QueryMember(OBJID idUser)
{
    int nAmount = m_setUser.size();
    for(int i = 0; i < nAmount; i++)
    {
        if (m_setUser[i].id == idUser)
        {
            SynchroInfo(idUser, RankOfIndex(i));
            return ;
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CSupermanList::SynchroInfo(OBJID idUser, int nRank)
{
    char	buf[sizeof(long)*2];
    VARIANT_SET0* pInfo = (VARIANT_SET0*)buf;
    pInfo->IntParam(0)	= idUser;
    pInfo->IntParam(1)	= nRank;
    IMessagePort* pPort = GameWorld()->GetMsgPort();
    for(int i = MSGPORT_MAPGROUP_FIRST; i < pPort->GetSize(); i++)
    {
        pPort->Send(i, KERNEL_SUPERMANLIST, STRUCT_TYPE(buf), buf);
    }
}

//////////////////////////////////////////////////////////////////////
void CSupermanList::InsertInfo(OBJID idUser, int nNumber, LPCTSTR szName)
{
    SQLBUF	szSQL;
    sprintf(szSQL, "INSERT %s SET id=%u,number=%d,name='%s'", _TBL_SUPERMAN, idUser, nNumber, szName);
    GameWorld()->GetDatabase()->ExecuteSQL(szSQL);
}

//////////////////////////////////////////////////////////////////////
void CSupermanList::UpdateInfo(OBJID idUser, int nNumber)
{
    SQLBUF	szSQL;
    sprintf(szSQL, "UPDATE %s SET number=%d WHERE id=%u LIMIT 1", _TBL_SUPERMAN, nNumber, idUser);
    GameWorld()->GetDatabase()->ExecuteSQL(szSQL);
}

//////////////////////////////////////////////////////////////////////
void CSupermanList::DeleteInfo(OBJID idUser)
{
    SQLBUF	szSQL;
    sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_SUPERMAN, idUser);
    GameWorld()->GetDatabase()->ExecuteSQL(szSQL);
}

//////////////////////////////////////////////////////////////////////
void CSupermanList::SendList(OBJID idUser, int idx, int nNumber, OBJID idxTask)
{
    CHECK(idx >= 0);
    CPlayer* pUser = UserList()->GetPlayer(idUser);
    if (!pUser)
    {
        return ;
    }
    // title
    {
        int		nData = 0;
        CMsgDialog	msg;
        IF_OK(msg.Create(MSGDIALOG_TEXT, STR_KO_LIST_TITLE, INDEX_NONE, nData))
        pUser->SendMsg(&msg);
        IF_OK_(msg.Create(MSGDIALOG_TEXT, "-------------------------", INDEX_NONE, nData))
        pUser->SendMsg(&msg);
    }
    int i = idx;
    for(; i < idx + nNumber && i < m_setUser.size(); i++)
    {
        {
            char	szMsg[256];
            sprintf(szMsg, "%3d %-16s %d", i + 1, m_setUser[i].szName, m_setUser[i].dwNumber);
            int		nData = 0;
            CMsgDialog	msg;
            IF_OK(msg.Create(MSGDIALOG_TEXT, szMsg, INDEX_NONE, nData))
            pUser->SendMsg(&msg);
        }
    }
    {
        int	nData = 0;
        CMsgDialog	msg;
        IF_OK_(msg.Create(MSGDIALOG_TEXT, "-------------------------", INDEX_NONE, nData))
        pUser->SendMsg(&msg);
    }
    if (i >= idx + nNumber)
    {
        int		nData = 0;
        CMsgDialog	msg;
        IF_OK(msg.Create(MSGDIALOG_LINK, STR_NEXT_PAGE, idxTask, nData))
        pUser->SendMsg(&msg);
    }
    {
        int		nData = 0;
        CMsgDialog	msg;
        IF_OK(msg.Create(MSGDIALOG_LINK, STR_CLOSE_BUTTON, INDEX_NONE, nData))
        pUser->SendMsg(&msg);
    }
    CMsgDialog	msg;
    IF_OK(msg.Create(MSGDIALOG_CREATE, INDEX_NONE))
    pUser->SendMsg(&msg);
}




