
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAllot.cpp: implementation of the CMsgAllot class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "mapgroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAllot::CMsgAllot()
{
    this->Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgAllot::~CMsgAllot()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAllot::Create(unsigned char ucForce, unsigned char ucDexterity, unsigned char ucHealth, unsigned char ucSoul)
{
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_ALLOT;
    m_pInfo->ucForce	= ucForce;
    m_pInfo->ucHealth	= ucHealth;
    m_pInfo->ucSoul		= ucSoul;
    m_pInfo->ucDexterity = ucDexterity;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAllot::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_ALLOT != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgAllot::Process(void* pInfo)
{
    // get obj
    IRole* pRole	= RoleManager()->QueryRole(this);
    if (!pRole)
    {
        return;
    }
    // get User
    CUser* pUser = NULL;
    pRole->QueryObj(OBJ_USER, IPP_OF(pUser));
    if (!pUser)
    {
        return;
    }
    if (pUser->IsAutoAllot() && pUser->GetLev() <= MASTER_USERLEV)
    {
        return;
    }
    int nSum	= m_pInfo->ucForce + m_pInfo->ucHealth + m_pInfo->ucSoul + m_pInfo->ucDexterity;
    if (pUser->GetAddPoint() < nSum)
    {
        pUser->SendSysMsg(STR_CHEAT);
        return;
    }
    pUser->IncAddPoint(-1 * nSum);
    pUser->SetForce(m_pInfo->ucForce + pUser->GetForce());
    pUser->SetHealth(m_pInfo->ucHealth + pUser->GetHealth());
    pUser->SetSoul(m_pInfo->ucSoul + pUser->GetSoul());
    pUser->SetDexterity(m_pInfo->ucDexterity + pUser->GetDexterity());
}
