
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgDataArray.cpp: implementation of the CMsgCharArray class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgDataArray.h"
#include "Hero.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgDataArray::CMsgDataArray()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgDataArray::~CMsgDataArray()
{
}
//--------------------------------------------------------------------------------------
BOOL CMsgDataArray::Create(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_DATAARRAY != this->GetType())
    {
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------------
void CMsgDataArray::Process(void* pInfo)
{
    switch(m_pInfo->ucAction)
    {
    case _DATAARRAY_ACTION_MEDAL:
        {
            for(int i = 0; i < m_pInfo->ucAmount; i ++)
            {
                g_objHero.AddMedaleTitle(m_pInfo->ucChar[i]);
            }
            break;
        }
    case _DATAARRAY_ACTION_HONORTITLE:
        {
            for(int i = 0; i < m_pInfo->ucAmount; i ++)
            {
                g_objHero.AddHororTitle(m_pInfo->ucChar[i]);
            }
            break;
        }
        break;
    }
}
//--------------------------------------------------------------------------------------
BOOL CMsgDataArray::Create(int nAction, int nAmount, char setData[])
{
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgSize		= sizeof(MSG_Info)  - sizeof(char) + nAmount * sizeof(char);
    m_pInfo->unMsgType		= _MSG_DATAARRAY;
    m_pInfo->ucAction	= nAction;
    m_pInfo->ucAmount	= nAmount;
    memcpy(m_pInfo->ucChar, setData, nAmount * sizeof(char));
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgDataArray::Create(int nAction, int nAmount, short setData[])
{
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgSize		= sizeof(MSG_Info) - sizeof(short) + nAmount * sizeof(short);
    m_pInfo->unMsgType		= _MSG_DATAARRAY;
    m_pInfo->ucAction	= nAction;
    m_pInfo->ucAmount	= nAmount;
    memcpy(m_pInfo->usShort, setData, nAmount * sizeof(short));
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgDataArray::Create(int nAction, int nAmount, long setData[])
{
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgSize		= sizeof(MSG_Info) - sizeof(unsigned long) + nAmount * sizeof(unsigned long);
    m_pInfo->unMsgType		= _MSG_DATAARRAY;
    m_pInfo->ucAction	= nAction;
    m_pInfo->ucAmount	= nAmount;
    memcpy(m_pInfo->dwLong, setData, nAmount * sizeof(long));
    return true;
}
