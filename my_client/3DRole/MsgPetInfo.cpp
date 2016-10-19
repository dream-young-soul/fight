
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgPetInfo.cpp: implementation of the CMsgPetInfo class.
//
//////////////////////////////////////////////////////////////////////

#include	"MsgPetInfo.h"
#include	"Pet.h"
#include	"GamePlayerSet.h"
#include	"Hero.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgPetInfo::CMsgPetInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgPetInfo::~CMsgPetInfo()
{
}

//--------------------------------------------------------------------------------
BOOL CMsgPetInfo::Create(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_PETINFO != this->GetType())
    {
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void CMsgPetInfo::Process(void* pInfo)
{
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_pInfo->id);
    if (pPlayer)
    {
        g_objPlayerSet.DelPlayer(m_pInfo->id);
    }
    CPet* pPet = CPet::CreateNewPet(m_pInfo->id, m_pInfo->nAIType, m_pInfo->idItem);
    pPet->SetLook((USHORT)m_pInfo->dwLookFace);
    pPet->SetRoleType(_ROLE_CALL_PET);
    pPet->Create(m_pInfo->dwLookFace, _ROLE_VIEW_ROLE);
    CMyPos posPet;
    posPet.x = m_pInfo->usPosX;
    posPet.y = m_pInfo->usPosY;
    pPet->SetPos(posPet);
    g_objPlayerSet.AddPlayer((CPlayer*)pPet);
    g_objHero.AddPet(m_pInfo->id);
    CItem* pItem = g_objHero.GetItem (m_pInfo->idItem);
    if (pItem)
    {
        char szMsg[128] = "";
        sprintf(szMsg, g_objGameDataSet.GetStr(100174), pItem->GetMonsterName ());
        g_objGameMsg.AddMsg (szMsg);
    }
}
//--------------------------------------------------------------------------------
