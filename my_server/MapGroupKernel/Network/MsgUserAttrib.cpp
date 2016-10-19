
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

////////////////////////////////////////////////////////////////
//
//		MsgUserAttrib.cpp
//

#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgUserAttrib::CMsgUserAttrib()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgUserAttrib::~CMsgUserAttrib()
{
}


/*
//////////////////////////////////////////////////////////////////////
BOOL CMsgUserAttrib::Create(OBJID idUser, int nType, DWORD dwData)
{
	CHECKF(nType == _USERATTRIB_NONE || nType >= 0);

	// init
	this->Init();

	// fill info now
	m_unMsgSize		= sizeof(MSG_Info);
	m_unMsgType		= _MSG_USERATTRIB;

	m_pInfo->idUser	=idUser;
	m_pInfo->dwAttributeNum	= 0;

	if (nType >= 0)
	{
		m_unMsgSize		+= sizeof(UserAttribStruct);
		m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].ucAttributeType	= nType;
		m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= dwData;
		m_pInfo->dwAttributeNum++;
	}

	return true;
}
*/

//////////////////////////////////////////////////////////////////////
BOOL CMsgUserAttrib::Create(OBJID idUser, int nType, I64 i64Data)
{
    CHECKF(nType == _USERATTRIB_NONE || nType >= 0);
    // init
    this->Init();
    // fill info now
    m_unMsgSize		= sizeof(MSG_Info) - sizeof(UserAttribStruct);
    m_unMsgType		= _MSG_USERATTRIB;
    m_pInfo->idUser	= idUser;
    m_pInfo->dwAttributeNum	= 0;
    if (nType >= 0)
    {
        m_unMsgSize		+= sizeof(UserAttribStruct);
        m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].ucAttributeType	= nType;
        m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= i64Data & 0xFFFFFFFF;
        m_pInfo->dwAttributeNum++;
        if (nType == _USERATTRIB_KEEPEFFECT)
        {
            m_unMsgSize		+= sizeof(UserAttribStruct);
            m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].ucAttributeType	= _USERATTRIB_KEEPEFFECT2;
            m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= (i64Data >> 32) & 0xFFFFFFFF;
            m_pInfo->dwAttributeNum++;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
/*
BOOL CMsgUserAttrib::Append(int nType, DWORD dwData)
{
	CHECKF(nType >= 0);
	CHECKF(m_unMsgSize + sizeof(UserAttribStruct) < MAX_PACKETSIZE);

	// fill info now
	m_unMsgSize		+= sizeof(UserAttribStruct);
	m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].ucAttributeType	= nType;
	m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= dwData;
	m_pInfo->dwAttributeNum++;

	return true;
}
*/

//////////////////////////////////////////////////////////////////////
BOOL CMsgUserAttrib::Append(int nType, I64 i64Data)
{
    CHECKF(nType >= 0);
    CHECKF(m_unMsgSize + sizeof(UserAttribStruct) < MAX_PACKETSIZE);
    if (nType == _USERATTRIB_KEEPEFFECT)
    {
        CHECKF (m_unMsgSize + 2 * sizeof(UserAttribStruct) < MAX_PACKETSIZE);
    }
    // fill info now
    m_unMsgSize		+= sizeof(UserAttribStruct);
    m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].ucAttributeType	= nType;
    m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= i64Data & 0xFFFFFFFF;
    m_pInfo->dwAttributeNum++;
    if (nType == _USERATTRIB_KEEPEFFECT)
    {
        m_unMsgSize		+= sizeof(UserAttribStruct);
        m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].ucAttributeType	= _USERATTRIB_KEEPEFFECT2;
        m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= (i64Data >> 32) & 0xFFFFFFFF;
        m_pInfo->dwAttributeNum++;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgUserAttrib::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_USERATTRIB != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgUserAttrib::Process(CGameSocket* pSocket)
{
    ::LogSave("invalid msg CMsgUserAttrib.");
}
