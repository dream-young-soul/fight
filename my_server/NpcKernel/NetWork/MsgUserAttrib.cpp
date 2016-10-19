
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

////////////////////////////////////////////////////////////////
//
//		MsgUserAttrib.cpp
//

#include "AllMsg.h"
#include "NpcWorld.h"

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


//////////////////////////////////////////////////////////////////////
BOOL CMsgUserAttrib::Create(OBJID idUser, int nAction, DWORD dwAttrib)
{
    // init
    this->Init();
    // fill info now
    m_unMsgSize		= sizeof(MSG_Info);
    m_unMsgType		= _MSG_USERATTRIB;
    m_pInfo->idUser	= idUser;
    m_pInfo->dwAttributeNum	= 0;
    if (nAction >= 0)
    {
        m_unMsgSize		+= sizeof(UserAttribStruct);
        m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].ucAttributeType	= nAction;
        m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= dwAttrib;
        m_pInfo->dwAttributeNum++;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgUserAttrib::Append(int nAction, DWORD dwAttrib)
{
    CHECKF(nAction >= 0);
    // fill info now
    m_unMsgSize		+= sizeof(UserAttribStruct);
    m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].ucAttributeType	= nAction;
    m_pInfo->setUserAttrib[m_pInfo->dwAttributeNum].dwAttributeData	= dwAttrib;
    m_pInfo->dwAttributeNum++;
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
    IRole* pRole = UserManager()->QueryRole(m_pInfo->idUser);
    if (!pRole)
    {
        return ;
    }
    for(int i = 0; i < m_pInfo->dwAttributeNum; i++)
    {
        switch(m_pInfo->setUserAttrib[i].ucAttributeType)
        {
        case	_USERATTRIB_LIFE:
            pRole->SetLife(m_pInfo->setUserAttrib[i].dwAttributeData);
            break;
        case	_USERATTRIB_MANA:
            pRole->SetPower(m_pInfo->setUserAttrib[i].dwAttributeData);
            break;
        case	_USERATTRIB_KEEPEFFECT:
            {
                I64 i64Effect = pRole->GetEffect() & 0xFFFFFFFF00000000;
                pRole->SetEffect(m_pInfo->setUserAttrib[i].dwAttributeData + i64Effect);
            }
            break;
        case	_USERATTRIB_KEEPEFFECT2:
            {
                I64 i64Effect = m_pInfo->setUserAttrib[i].dwAttributeData;
                i64Effect = (i64Effect << 32) + (pRole->GetEffect() & 0x00000000FFFFFFFF);
                pRole->SetEffect(i64Effect);
            }
            break;
        case	_USERATTRIB_SIZEADD:
            pRole->SetSizeAdd(m_pInfo->setUserAttrib[i].dwAttributeData);
            break;
        }
    }
}
