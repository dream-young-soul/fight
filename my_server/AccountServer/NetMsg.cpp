
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NetMsg.cpp: implementation of the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////
//#include "AllMsg.h"
#include "NetMsg.h"

//extern DWORD	SysTimeGetEx(void);

// static
HANDLE	CNetMsg::s_hHeap	= NULL;
UINT	CNetMsg::s_uNumAllocsInHeap	= 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetMsg::CNetMsg()
{
    Init();
}

//////////////////////////////////////////////////////////////////////
CNetMsg::~CNetMsg()
{

}

//////////////////////////////////////////////////////////////////////
void* CNetMsg::operator new(size_t size)
{
    // got heap?
    if (!s_hHeap)
    {
        s_hHeap	=::HeapCreate(0, 0, 0);
        if (!s_hHeap)
        {
            return NULL;
        }
    }

    // alloc mem for new obj
    void* p	=::HeapAlloc(s_hHeap, HEAP_ZERO_MEMORY, size);

    if (p)
    {
        s_uNumAllocsInHeap++;
    }

    return p;
}

//////////////////////////////////////////////////////////////////////
void CNetMsg::operator delete(void* p)
{
    // check param
    if (!s_hHeap || s_uNumAllocsInHeap <= 0)
    {
        return;
    }

    // free it...
    if (::HeapFree(s_hHeap, 0, p))
    {
        s_uNumAllocsInHeap--;
    }

    // time to destroy heap?
    if (s_uNumAllocsInHeap <= 0)
    {
        if (::HeapDestroy(s_hHeap))
        {
            s_hHeap	= NULL;
        }
    }
}

//////////////////////////////////////////////////////////////////////
BOOL CNetMsg::IsValidPt(void* p)
{
    if (!s_hHeap || !p)
    {
        return false;
    }

    if ((DWORD)p < 0x00010000 || (DWORD)p >= 0x7FFEFFFF)	// user address range from 1M--2G-64k
    {
        return false;
    }

    if (::IsBadCodePtr((FARPROC)p))
    {
        return false;
    }

    return ::HeapValidate(s_hHeap, 0, p);
}

//////////////////////////////////////////////////////////////////////
void CNetMsg::Init()
{
    memset(m_bufMsg, 0L, sizeof(m_bufMsg));
    m_unMsgSize	= 0;
    m_unMsgType	= _MSG_NONE;
}

//////////////////////////////////////////////////////////////////////
void CNetMsg::operator = (CNetMsg& msg)
{
    memcpy(m_bufMsg, msg.GetBuf(), _MAX_MSGSIZE);
    m_dwAnyData	= msg.m_dwAnyData;
}

//////////////////////////////////////////////////////////////////////
BOOL CNetMsg::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }

    if ((unsigned short)dwMsgSize != CNetMsg::GetSize(pbufMsg, dwMsgSize))
    {
        return false;
    }

    if (_MSG_NONE == CNetMsg::GetType(pbufMsg, dwMsgSize))
    {
        return false;
    }

    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
// default process is broadcast this msg to all room mates
void CNetMsg::Process(CGameSocket* pSocket)
{
}


//////////////////////////////////////////////////////////////////////
// static functions;
//////////////////////////////////////////////////////////////////////
/*BOOL CNetMsg::SendSysMsg(CGameSocket* pSocket, char* pszMsg, unsigned short unTxtAtr)
{
	if (!pSocket || !pszMsg)
		return false;

	CMsgTalk msg;
	if (!msg.Create((char*)szSYSTEM, (char*)szALLUSERES, pszMsg, NULL, _COLOR_RED, unTxtAtr))
		return false;

	pSocket->SendMsg(msg.GetBuf(), msg.GetSize());
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CNetMsg::SendMsg(CGameSocket* pSocket, void* pbuf, DWORD dwBufSize)
{
	if (!pSocket || !pbuf || dwBufSize <= 0)
		return false;

	pSocket->SendMsg(pbuf, dwBufSize);
	return true;
}
//*/
//////////////////////////////////////////////////////////////////////
unsigned short	CNetMsg::GetType(char* pbufMsg, DWORD dwMsgSize)
{
    // check it...
    if (!pbufMsg || (int)dwMsgSize > CNetMsg::GetMaxSize())
    {
        return 0;
    }

    if (dwMsgSize <= 2 * sizeof(unsigned short))
    {
        return 0;
    }

    unsigned short* punPointer	= (unsigned short*)pbufMsg;
    return punPointer[1];
}

//////////////////////////////////////////////////////////////////////
unsigned short	CNetMsg::GetSize(char* pbufMsg, DWORD dwMsgSize)
{
    // check it...
    if (!pbufMsg || (int)dwMsgSize > CNetMsg::GetMaxSize())
    {
        return 0;
    }

    if (dwMsgSize <= 2 * sizeof(unsigned short))
    {
        return 0;
    }

    return	*((unsigned short*)pbufMsg);
}


//////////////////////////////////////////////////////////////////////
/*CNetMsg* CNetMsg::CreateMsg(char* pbufMsg, DWORD dwMsgSize)
{
	// check it...
	if (!pbufMsg || (int)dwMsgSize > GetMaxSize())
		return NULL;

	// length check
	if ((DWORD)CNetMsg::GetSize(pbufMsg, dwMsgSize) != dwMsgSize)
		return NULL;

	// make it...
	CNetMsg* pMsg	=NULL;

	unsigned short unMsgType	=CNetMsg::GetType(pbufMsg, dwMsgSize);
	switch(unMsgType)
	{
	case _MSG_LOGIN:
		pMsg	=new CMsgLogin;
		break;
	case _MSG_ROOM:
		pMsg	=new CMsgRoom;
		break;
	case _MSG_WALK:
		pMsg	=new CMsgWalk;
		break;
	case _MSG_TALK:
		pMsg	=new CMsgTalk;
		break;
	case _MSG_POSITION:
		pMsg	=new CMsgPosition;
		break;
	case _MSG_EMOTION:
		pMsg	=new CMsgEmotion;
		break;
	case _MSG_USERINFO:
		pMsg	=new CMsgUserInfo;
		break;
	case _MSG_PLAYER:
		pMsg	=new CMsgPlayer;
		break;
	case _MSG_BATTLE:
		pMsg	=new CMsgBattle;
		break;
	case _MSG_BATTLEACT:
		pMsg	=new CMsgBattleAct;
		break;
	case _MSG_ROLE:
		pMsg	=new CMsgRole;
		break;
	case _MSG_ACCOUNT:
		pMsg	=new CMsgAccount;
		break;
	case _MSG_CONNECT:
		pMsg	=new CMsgConnect;
		break;
	case _MSG_ITEMINFO:
		pMsg	=new CMsgItemInfo;
		break;
	case _MSG_ITEM:
		pMsg	=new CMsgItem;
		break;
	case _MSG_ATTACK:
		pMsg	=new CMsgAtk;
		break;
	case _MSG_PET:
		pMsg	=new CMsgPet;
		break;
	case _MSG_PETINFO:
		pMsg	=new CMsgPetInfo;
		break;
	case _MSG_SKILLINFO:
		pMsg	=new CMsgSkillInfo;
		break;
	case _MSG_DIR:
		pMsg	=new CMsgDir;
		break;
	case _MSG_BEHEAL:
		pMsg	=new CMsgBeHeal;
		break;
	case _MSG_NAME:
		pMsg	=new CMsgName;
		break;
	case _MSG_USERATTRIB:
		pMsg	=new CMsgUserAttrib;
		break;
	case _MSG_POINTALLOT:
		pMsg	=new CMsgPointAllot;
		break;
	case _MSG_DELETEROLE:
		pMsg	=new CMsgDeleteRole;
		break;
	case _MSG_TEAM:
		pMsg	=new CMsgTeam;
		break;
	case _MSG_TEAMINFO:
		pMsg	=new CMsgTeamInfo;
		break;
	case _MSG_STEAL:
		pMsg	=new CMsgSteal;
		break;
	case _MSG_EXCHANGE:
		pMsg	=new CMsgExchange;
		break;

	/*
	case _MSG_SCHOOL:
		pMsg	=new CMsgSchool;
		break;
	case _MSG_DIE:
		pMsg	=new CMsgDie;
		break;
	case _MSG_REBORN:
		pMsg	=new CMsgReborn;
		break;
	case _MSG_REGISTER:
		pMsg	=new CMsgRegister;
		break;
	case _MSG_MARRIAGE:
		pMsg	=new CMsgMarriage;
		break;
	case _MSG_GM:
		pMsg	=new CMsgGM;
		break;
	case _MSG_LOGOUT:
		pMsg	=new CMsgLogout;
		break;
* /
	default:
		break;
	}

#ifdef _DEBUG
	if (!pMsg)
	{
		DWORD dwTime	=::SysTimeGet();

		char szName[256];
		sprintf(szName, "dump%u.msg", dwTime);
		FILE* fp	=fopen(szName, "wb");
		if (fp)
		{
			fwrite(pbufMsg, dwMsgSize, 1L, fp);
			fclose(fp);
		}

		return NULL;
	}
#else
	if (!pMsg)
		return NULL;
#endif

	if (!pMsg->Create(pbufMsg, dwMsgSize))
	{
		delete pMsg;
		return NULL;
	}
	else
		return pMsg;
}


//////////////////////////////////////////////////////////////////////
void CNetMsg::DumpMsg(CNetMsg* pMsg)
{
	if (!pMsg)
		return;

	char szName[256];
	sprintf(szName, "dump%u.msg", ::SysTimeGetEx());

	FILE* fp	=fopen(szName, "wb");
	if (fp)
	{
		try {
			fwrite(pMsg->GetBuf(), pMsg->GetSize(), 1L, fp);
		}
		catch(...)
		{
			::LogSave("invalid dump msg.");
		}

		fclose(fp);
	}
}

//////////////////////////////////////////////////////////////////////
void CNetMsg::DumpMsg(void* pBuf, DWORD dwSize)
{
	if (!pBuf)
		return;

	char szName[256];
	sprintf(szName, "dump%u.msg", ::SysTimeGetEx());

	FILE* fp	=fopen(szName, "wb");
	if (fp)
	{
		try {
			fwrite(pBuf, dwSize, 1L, fp);
		}
		catch(...)
		{
			::LogSave("invalid dump msg.");
		}

		fclose(fp);
	}
}
//*/
