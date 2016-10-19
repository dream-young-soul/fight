
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAction.cpp: implementation of the CMsgWalkEx class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "mapgroup.h"
#include "transformation.h"

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
BOOL CMsgWalkEx::Create(OBJID idPlayer, int nDir, int nMode, int x, int y)
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
    m_unMsgType	= _MSG_WALKEX;
    m_pInfo->idUser		= idPlayer;
    m_pInfo->ucDir		= nDir;
    m_pInfo->ucMode		= nMode;
    m_pInfo->usPosX		= x;
    m_pInfo->usPosY		= y;
    return true;
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
    ASSERT(!"CMsgWalkEx::Process(");
}
