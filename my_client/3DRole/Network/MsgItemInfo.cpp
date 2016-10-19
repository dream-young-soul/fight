
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgItemInfo.cpp: implementation of the CMsgItemInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "Item.h"
#include "Hero.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgItemInfo::CMsgItemInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgItemInfo::~CMsgItemInfo()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgItemInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_ITEMINFO != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgItemInfo::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogSave("Process CMsgItemInfo, ItemID:%u, ItemType:%u, Action:%u",
              m_pInfo->id,
              m_pInfo->dwType,
              m_pInfo->ucAction);
#endif
    switch(m_pInfo->ucAction)
    {
    case ITEMINFO_OTHERPLAYER_EQUIPMENT:
        {
            if (m_pInfo->ucPosition == ITEMPOSITION_BACKPACK)
            {
                return;
            }
            CItem* pItem	= new CItem;
            if (!pItem || !pItem->Create(m_pInfo->id, m_pInfo->dwType))
            {
                return;
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
            pItem->SetMonsterGrow(m_pInfo->nData);
            if (!g_objHero.m_objPlayerDummy.SetEquipment(m_pInfo->ucPosition, pItem))
            {
                SAFE_DELETE(pItem);
            }
            ::PostCmd(CMD_FLASH_OTHERPLAYER_EQUIPMENT);
        }
        break;
    case ITEMINFO_ADDITEM:
        {
            CItem* pItem	= new CItem;
            if (!pItem || !pItem->Create(m_pInfo->id, m_pInfo->dwType))
            {
                return;
            }
            if (pItem->GetPackageType() == PACKAGE_PET && m_StrPacker.GetStrAmount() > 0)
            {
                char szName[16];
                m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
                pItem->SetMonsterName(szName);
            }
            pItem->SetAmount(m_pInfo->usAmount);
            pItem->SetPosition(m_pInfo->ucPosition);
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
            pItem->SetMonsterGrow(m_pInfo->nData);
            switch(m_pInfo->ucPosition)
            {
            case ITEMPOSITION_BACKPACK:
            case ITEMPOSITION_GHOSTGEM_PACK:
            case ITEMPOSITION_EUDEMONEGG_PACK:
            case ITEMPOSITION_EUDEMON_PACK:
                {
                    if (g_objHero.GetItem(m_pInfo->id))
                    {
                        g_objHero.DelItem(m_pInfo->id);
                    }
                    if (!g_objHero.AddItem(pItem))
                    {
                        SAFE_DELETE(pItem);
                    }
                    else
                    {
                        ::PostCmd(CMD_FLASHITEM);
                    }
                }
                break;
            default:
                g_objHero.SetEquipment(m_pInfo->ucPosition, pItem);
                break;
            }
            //set helptip
            if (g_objHero.m_bCanSetHelpTips)
            {
                g_objHero.SetHelpTip(pItem->GetTypeID());
            }
        }
        break;
    case ITEMINFO_TRADE:
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
            pItem->SetGemAtkType(m_pInfo->dwGemAtkType);
            pItem->SetWarGhostExp(m_pInfo->dwWarGhostExp);
            pItem->SetAvailabeTime(m_pInfo->dwAvailabeTime);
            pItem->SetMonsterGrow(m_pInfo->nData);
            g_objHero.AddPlayerTradeItem(pItem);
        }
        break;
    case ITEMINFO_UPDATE:
        {
            CItem* pItem = g_objHero.GetItem(m_pInfo->id);
            if (!pItem)
            {
                pItem = g_objHero.SearchEquipment(m_pInfo->id);
                if (!pItem)
                {
                    return;
                }
            }
            if (pItem->GetTypeID() != m_pInfo->dwType)
            {
                // type changed
                if (!pItem->Create(m_pInfo->id, m_pInfo->dwType))
                {
                    return;
                }
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
            pItem->SetGemAtkType(m_pInfo->dwGemAtkType);
            pItem->SetWarGhostExp(m_pInfo->dwWarGhostExp);
            pItem->SetAvailabeTime(m_pInfo->dwAvailabeTime);
            pItem->SetMonsterGrow(m_pInfo->nData);
            if (g_objHero.GetItem(m_pInfo->id))
            {
                ::PostCmd(CMD_FLASHITEM);
            }
            else
            {
                g_objHero.SetEquipment(m_pInfo->ucPosition, pItem);
                ::PostCmd(CMD_FLASHEQUIPMENT);
            }
        }
        break;
    case ITEMINFO_AUCTION:               // 拍买查看物品
        {
            CItem* pItem = g_objHero.GetAuctionItem();
            if (!pItem)
            {
                pItem = new CItem;
                if (!pItem->Create(m_pInfo->id, m_pInfo->dwType))
                {
                    return;
                }
            }
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
            pItem->SetGemAtkType(m_pInfo->dwGemAtkType);
            pItem->SetWarGhostExp(m_pInfo->dwWarGhostExp);
            pItem->SetAvailabeTime(m_pInfo->dwAvailabeTime);
            pItem->SetMonsterGrow(m_pInfo->nData);
            g_objHero.SetAuctionItem(pItem);
            ::PostCmd(CMD_ACTION_FLASHAUCTION);
        }
        break;
    default:
        break;
    }
}
