
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgItemInfoEx.cpp: implementation of the CMsgItemInfoEx class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgItemInfoEx.h"
#include "GamePlayerSet.h"
#include "Hero.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgItemInfoEx::CMsgItemInfoEx()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgItemInfoEx::~CMsgItemInfoEx()
{
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgItemInfoEx::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_ITEMINFOEX != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgItemInfoEx::Process(void* pInfo)
{
    switch(m_pInfo->ucAction)
    {
    case ITEMINFOEX_BOOTH:
        {
            if (g_objHero.m_objBooth.GetID() == m_pInfo->idOwner)
            {
                CItem* pItem	= new CItem;
                if (!pItem || !pItem->Create(m_pInfo->id, m_pInfo->dwType))
                {
                    return;
                }
                if (pItem->GetPackageType() == PACKAGE_PET
                        && m_StrPacker.GetStrAmount() > 0)
                {
                    char szName[_MAX_NAMESIZE];
                    m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
                    pItem->SetMonsterName(szName);
                }
                pItem->SetAmount(m_pInfo->usAmount);
                pItem->SetAmountLimit(m_pInfo->usAmountLimit);
                pItem->SetStatus(m_pInfo->ucStatus);
                pItem->SetGem1(m_pInfo->ucGem1);
                pItem->SetGem2(m_pInfo->ucGem2);
                pItem->SetMagic1(m_pInfo->ucMagic1);
                pItem->SetMagic2(m_pInfo->ucMagic2);
                pItem->SetMagic3(m_pInfo->ucMagic3);
                pItem->SetOwnerID(m_pInfo->idOwner);
                pItem->SetBoothSellPrice(m_pInfo->dwPrice);
                pItem->SetGemAtkType(m_pInfo->dwGemAtkType);
                pItem->SetWarGhostExp(m_pInfo->dwWarGhostExp);
                pItem->SetAvailabeTime(m_pInfo->dwAvailabeTime);
                pItem->SetMonsterGrow(m_pInfo->nData);
                g_objHero.m_objBooth.AddItem(pItem);
            }
        }
        break;
    case ITEMINFOEX_EQUIPMENT:
        {
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_pInfo->idOwner, false);
            if (pPlayer)	// player
            {
                switch(m_pInfo->ucPosition)
                {
                case ITEMPOSITION_HELMET:
                    pPlayer->SetArmet(m_pInfo->dwType);
                    break;
                case ITEMPOSITION_ARMOR:
                    pPlayer->SetArmor(m_pInfo->dwType);
                    break;
                case ITEMPOSITION_WEAPONR:
                    pPlayer->SetRWeapon(m_pInfo->dwType);
                    break;
                case ITEMPOSITION_WEAPONL:
                    pPlayer->SetLWeapon(m_pInfo->dwType);
                    break;
                }
            }
            else if (m_pInfo->idOwner == g_objHero.GetID())
            {
                CItem* pItem = g_objHero.SearchEquipment(m_pInfo->id);
                if (!pItem)
                {
                    return;
                }
                if (pItem->GetTypeID() != m_pInfo->dwType)
                {
                    // type changed
                    if (!pItem->Create(m_pInfo->id, m_pInfo->dwType))
                    {
                        return;
                    }
                }
                pItem->SetAmount(m_pInfo->usAmount);
                pItem->SetAmountLimit(m_pInfo->usAmountLimit);
                pItem->SetStatus(m_pInfo->ucStatus);
                pItem->SetGem1(m_pInfo->ucGem1);
                pItem->SetGem2(m_pInfo->ucGem2);
                pItem->SetMagic1(m_pInfo->ucMagic1);
                pItem->SetMagic2(m_pInfo->ucMagic2);
                pItem->SetMagic3(m_pInfo->ucMagic3);
                pItem->SetGemAtkType(m_pInfo->dwGemAtkType);
                pItem->SetWarGhostExp(m_pInfo->dwWarGhostExp);
                pItem->SetAvailabeTime(m_pInfo->dwAvailabeTime);
                g_objHero.SetEquipment(m_pInfo->ucPosition, pItem);
                ::PostCmd(CMD_FLASHEQUIPMENT);
            }
        }
        break;
    }
}
//////////////////////////////////////////////////////////////////////
