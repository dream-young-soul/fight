
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgTalk.cpp: implementation of the CMsgMessageBoard class.
//
//////////////////////////////////////////////////////////////////////

//#define _WINSOCKAPI_
#include "AllMsg.h"
#ifdef	WORLD_KERNEL
#include "WorldKernel.h"
#include "MessageBoard.h"
using namespace world_kernel;
#include "userlist.h"
#else
#pragma warning(disable:4786)
#include "MapGroup.h"
#include "usermanager.h"
#endif

//WORLDKERNEL_BEGIN

const int	MSGTITLE_SIZE		= 44;
const int	MSGLIST_SIZE		= 5;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgMessageBoard::CMsgMessageBoard()
{
    Init();
    m_pInfo	= (MSG_Info* )m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgMessageBoard::~CMsgMessageBoard()
{
}

/*/////////////////////////////////////////////////////////////////////
BOOL CMsgMessageBoard::Create(int nAction, int nChannel, int nIndex, CMessageBoard* pObj)
{
	// fill
	this->Init();

	// fill structure
	m_pInfo->ucAction		=nAction;
	m_pInfo->usChannel		=nChannel;
	m_pInfo->usIndex		=nIndex;

	BOOL bSucMake	=true;
	if (pObj)
	{
		for(int i = m_pInfo->usIndex; i < m_pInfo->usIndex + MSGLIST_SIZE; i++)
		{
			if (!pObj->IsValid(i))
				break;

			bSucMake	&=m_StrPacker.AddString(pObj->GetMsgName(i));
			char	buf[MSGTITLE_SIZE];
			SafeCopy(buf, pObj->GetMsgWords(i), MSGTITLE_SIZE);
			bSucMake	&=m_StrPacker.AddString(buf);
			char	bufTime[_MAX_NAMESIZE];
			pObj->GetMsgTime(i, bufTime);
			bSucMake	&=m_StrPacker.AddString(bufTime);
		}
	}

	m_unMsgType	=_MSG_MESSAGEBOARD;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return bSucMake;
}//*/

//////////////////////////////////////////////////////////////////////
BOOL CMsgMessageBoard::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgMessageBoard::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgMessageBoard, Sender:%s, Receiver:%s, Words:%s",
             szSender,
             szReceiver,
             szWords);
#endif
    if (m_pInfo->usChannel == _TXTATR_MSG_SYN)
    {
        CUser* pUser = UserManager()->GetUser(this);
        if (!pUser)
        {
            return ;
        }
        OBJID idSyn = pUser->GetSynID();
        if (idSyn == ID_NONE)
        {
            return ;
        }
        SetTransData(idSyn);
    }
    MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
}

//WORLDKERNEL_END
