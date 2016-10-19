
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: MsgEudemonAttrib.cpp
// Create by: Huang Yuhang
// Create on: 2004/5/31 10:32
//--------------------------------------------------------------------------------------
#include "MsgEudemonAttrib.h"
//--------------------------------------------------------------------------------------
CMsgEudemonAttrib::CMsgEudemonAttrib()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgEudemonAttrib::~CMsgEudemonAttrib()
{
}

BOOL CMsgEudemonAttrib::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_EUDEMONATTRIB != this->GetType())
    {
        return false;
    }
    return true;
}

void CMsgEudemonAttrib::Process(void* pInfo)
{
    // todo ...
    return;
}
