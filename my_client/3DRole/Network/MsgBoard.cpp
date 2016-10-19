
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgBoard.cpp: implementation of the CMsgBoard class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgBoard.h"
#include "Hero.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgMessageBoard::CMsgMessageBoard()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgMessageBoard::~CMsgMessageBoard()
{
}

//-------------------------------------------------------------------

BOOL CMsgMessageBoard::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_MESSAGEBOARD != this->GetType())
    {
        return false;
    }
    return true;
}

//-------------------------------------------------------------------

BOOL CMsgMessageBoard::Create(unsigned short usIndex, unsigned char ucAction, unsigned short usChannel, char* pszStr)
{
    this->Init();
    // fill info now
    m_pInfo->usIndex = usIndex;
    m_pInfo->usChannel = usChannel;
    m_pInfo->ucAction = ucAction;
    m_pInfo->unMsgType	= _MSG_MESSAGEBOARD;
    if (pszStr)
    {
        m_StrPacker.AddString(pszStr);
    }
    m_pInfo->unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}

//-------------------------------------------------------------------
void CMsgMessageBoard::Process(void* pInfo)
{
    switch(m_pInfo->ucAction)
    {
    case MESSAGEBOARD_LIST:
        g_objHero.m_objBulletin.SetList(m_pInfo->usIndex, m_pInfo->usChannel, m_StrPacker);
        break;
    default:
        return;
    }
}
//-------------------------------------------------------------------
