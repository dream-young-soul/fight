
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgEudemonAttrib.cpp: implementation of the CMsgEudemonAttrib class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "AiNpc.h"
#include "User.h"
#include "UserManager.h"
#include "MapGroup.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgEudemonAttrib::CMsgEudemonAttrib()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgEudemonAttrib::~CMsgEudemonAttrib()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgEudemonAttrib::Create(USHORT usAction, OBJID idEudemon, int nType, DWORD dwData)
{
    if (usAction == _EA_ACTION_NONE)
    {
        return false;
    }
    IF_NOT (nType >= 0 && nType < _MAX_EUDEMON_ATTRIB)
    return false;
    // init
    this->Init();
    // fill info now
    m_unMsgSize		= sizeof(MSG_Info);
    m_unMsgType		= _MSG_EUDEMONATTRIB;
    m_pInfo->usAction	= usAction;
    m_pInfo->idEudemon	= idEudemon;
    m_pInfo->dwAttributeNum		= 0;
    if (nType >= 0 && nType < _MAX_EUDEMON_ATTRIB)
    {
        m_unMsgSize	+= sizeof(ST_EUDEMON_ATTRIB);
        m_pInfo->setEudemonAttrib[m_pInfo->dwAttributeNum].dwAttributeType	= nType;
        m_pInfo->setEudemonAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= dwData;
        m_pInfo->dwAttributeNum ++;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgEudemonAttrib::Append(int nType, DWORD dwData)
{
    CHECKF(nType >= 0 && nType < _MAX_EUDEMON_ATTRIB);
    CHECKF(m_unMsgSize + sizeof(ST_EUDEMON_ATTRIB) < MAX_PACKETSIZE);
    // fill info now
    m_unMsgSize		+= sizeof(ST_EUDEMON_ATTRIB);
    m_pInfo->setEudemonAttrib[m_pInfo->dwAttributeNum].dwAttributeType	= nType;
    m_pInfo->setEudemonAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= dwData;
    m_pInfo->dwAttributeNum ++;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgEudemonAttrib::Create(USHORT usAction, OBJID idEudemon, int nAttribNum, ST_EUDEMON_ATTRIB setEudemonAttrib[])
{
    if (usAction == _EA_ACTION_NONE)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_unMsgSize		= sizeof(MSG_Info);
    m_unMsgType		= _MSG_EUDEMONATTRIB;
    m_pInfo->usAction	= usAction;
    m_pInfo->idEudemon	= idEudemon;
    IF_NOT (m_unMsgSize + nAttribNum * sizeof(ST_EUDEMON_ATTRIB) < MAX_PACKETSIZE)
    return false;
    m_pInfo->dwAttributeNum		= nAttribNum;
    for (int i = 0; i < nAttribNum; i++)
    {
        IF_NOT (setEudemonAttrib[i].dwAttributeType >= 0 && setEudemonAttrib[i].dwAttributeType < _MAX_EUDEMON_ATTRIB)
        return false;
        m_unMsgSize	+= sizeof(ST_EUDEMON_ATTRIB);
        m_pInfo->setEudemonAttrib[i].dwAttributeType	= setEudemonAttrib[i].dwAttributeType;
        m_pInfo->setEudemonAttrib[i].dwAttributeData	= setEudemonAttrib[i].dwAttributeData;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgEudemonAttrib::Create(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_EUDEMONATTRIB != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgEudemonAttrib::Process(CGameSocket* pSocket)
{
#ifdef _DEBUGx
    ::LogSave("Process msg: CMsgEudemonAttrib.");
#endif
    /*
    	CUser* pUser = UserManager()->GetUser(this);
    	if (!pUser)
    		return ;
    	if (!pUser->IsAlive())
    	{
    		pUser->SendSysMsg(STR_DIE);
    		return ;
    	}

    	DEBUG_TRY
    	switch (m_pInfo->usAction)
    	{
    	case _EA_ACTION_QUERY:
    		{
    			CItem* pEudemonItem = pUser->GetItem(m_pInfo->idEudemon);
    			if (pEudemonItem)
    				pEudemonItem->SendEudemonAttrib(pUser);
    		}
    	}
    	DEBUG_CATCH("Process Msg CMsgEudemonAttrib");
    */
}

//////////////////////////////////////////////////////////////////////
