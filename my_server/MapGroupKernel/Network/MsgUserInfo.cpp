
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgUserInfo.cpp: interface for the CMsgUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "UserData.h"
#pragma	warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgUserInfo::CMsgUserInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgUserInfo::~CMsgUserInfo()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgUserInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_USERINFO != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgUserInfo::Create(CUser* pUser)
{
    CHECKF(pUser);
    const UserInfoStruct* pInfo = pUser->GetInfo();
    // init
    this->Init();
    // fill info now
    m_pInfo->id					= pInfo->id;
    m_pInfo->dwLookFace			= pInfo->dwLookface;
    m_pInfo->usHair				= pInfo->dwHair;
    m_pInfo->cLength			= pInfo->cLength;
    m_pInfo->cFat				= pInfo->cFat;
    m_pInfo->dwMoney			= pInfo->dwMoney;
    m_pInfo->nExp				= pInfo->nExp;
    m_pInfo->nTutorExp			= pInfo->nTutorExp;
    m_pInfo->dwMercenaryExp		= pInfo->dwMercenaryExp;
    m_pInfo->nPotential			= pInfo->nPotential;
    m_pInfo->usForce			= pInfo->usForce;
    m_pInfo->usConstitution		= pInfo->usConstitution;	// zlong 2004-1-30
    m_pInfo->usDexterity		= pInfo->usDexterity;
    m_pInfo->usSpeed			= pInfo->usSpeed;
    m_pInfo->usHealth			= pInfo->usHealth;
    m_pInfo->usSoul				= pInfo->usSoul;
    m_pInfo->usAdditionalPoint	= pInfo->usAdditional_point;
    m_pInfo->usLife				= pInfo->usLife;
    m_pInfo->usMaxLife			= pUser->GetMaxLife();
    m_pInfo->usMana				= pInfo->usMana;
    m_pInfo->sPk				= pInfo->sPk;
    m_pInfo->ucLevel			= pInfo->ucLevel;
    m_pInfo->ucProfession		= pInfo->ucProfession;
    m_pInfo->ucNobility			= pInfo->ucNobility;
    m_pInfo->ucMetempsychosis	= pInfo->ucMetempsychosis;
    m_pInfo->ucAutoAllot		= pInfo->ucAuto_allot;
    m_pInfo->ucTutorLevel		= pInfo->ucTutorLevel;
    m_pInfo->ucMercenaryRank	= pInfo->ucMercenaryRank;
    m_pInfo->ucNobilityRank		= pInfo->ucNobilityRank;
    m_pInfo->usMaxEudemon		= pUser->GetMedalSelect();
    m_pInfo->dwExploit			= pInfo->dwExploit;
    BOOL bSucMake	= true;
    bSucMake	&= m_StrPacker.AddString((char*)pInfo->szName);
    bSucMake	&= m_StrPacker.AddString((char*)pInfo->szMate);
    m_unMsgType	= _MSG_USERINFO;
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgUserInfo::Process(void* pInfo)
{
    ASSERT(!"CMsgUserInfo::Process()");
    return;
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgUserInfo, id:%u", m_pInfo->id);
#endif
    char szName[_MAX_NAMESIZE]		= "";
    char szMate[_MAX_NAMESIZE]		= "";
    char szSyndicate[_MAX_NAMESIZE]	= "";
    char szTitle[_MAX_NAMESIZE]		= "";
    m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
    m_StrPacker.GetString(1, szMate, _MAX_NAMESIZE);
}
