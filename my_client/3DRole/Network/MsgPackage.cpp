
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgPackage.cpp: implementation of the CMsgPackage class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgPackage.h"
#include "Hero.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgPackage::CMsgPackage()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgPackage::~CMsgPackage()
{
}
//--------------------------------------------------------------------------------
BOOL CMsgPackage::Create(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_PACKAGE != this->GetType())
    {
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
BOOL CMsgPackage::Create(int nAction, int nType, OBJID id, OBJID idItem)
{
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_PACKAGE;
    m_pInfo->id = id;
    m_pInfo->ucAction =	(unsigned char)nAction;
    m_pInfo->idItem	=	idItem;
    m_pInfo->ucType	= (unsigned char)nType;
    return true;
}
//--------------------------------------------------------------------------------
void CMsgPackage::Process(void* pInfo)
{
    switch(m_pInfo->ucAction)
    {
    case MSGPACKAGE_QUERYLIST:
        {
            g_objHero.ResetPackage();
            for(int i = 0; i < m_pInfo->usItemAmount; i ++)
            {
                CItem* pItem = new CItem;
                MYASSERT(pItem);
                pItem->Create(m_pInfo->setItemInfo[i].idItem, m_pInfo->setItemInfo[i].idItemType);
                pItem->SetStatus(m_pInfo->setItemInfo[i].ucStatus);
                pItem->SetGem1(m_pInfo->setItemInfo[i].ucGem1);
                pItem->SetGem2(m_pInfo->setItemInfo[i].ucGem2);
                pItem->SetMagic1(m_pInfo->setItemInfo[i].ucMagic1);
                pItem->SetMagic2(m_pInfo->setItemInfo[i].ucMagic2);
                pItem->SetMagic3(m_pInfo->setItemInfo[i].ucMagic3);
                pItem->SetAmount(m_pInfo->setItemInfo[i].usAmount);
                pItem->SetAmountLimit(m_pInfo->setItemInfo[i].usAmountLimit);
                pItem->SetGemAtkType(m_pInfo->setItemInfo[i].dwGemAtkType);
                pItem->SetWarGhostExp(m_pInfo->setItemInfo[i].dwWarGhostExp);
                pItem->SetAvailabeTime(m_pInfo->setItemInfo[i].dwAvailabeTime);
                pItem->SetMonsterGrow(m_pInfo->setItemInfo[i].nData);
                if (pItem->GetPackageType() == PACKAGE_PET && strlen(m_pInfo->setItemInfo[i].szName))
                {
                    pItem->SetMonsterName(m_pInfo->setItemInfo[i].szName);
                }
                g_objHero.AddPackageItem(pItem);
            }
            ::PostCmd(CMD_FLASHPACKAGEITEM);
        }
        break;
    case MSGPACKAGE_QUERYLIST2:
        {
            for(int i = 0; i < m_pInfo->usItemAmount; i ++)
            {
                CItem* pItem = new CItem;
                MYASSERT(pItem);
                pItem->Create(m_pInfo->setItemInfo[i].idItem, m_pInfo->setItemInfo[i].idItemType);
                pItem->SetStatus(m_pInfo->setItemInfo[i].ucStatus);
                pItem->SetGem1(m_pInfo->setItemInfo[i].ucGem1);
                pItem->SetGem2(m_pInfo->setItemInfo[i].ucGem2);
                pItem->SetMagic1(m_pInfo->setItemInfo[i].ucMagic1);
                pItem->SetMagic2(m_pInfo->setItemInfo[i].ucMagic2);
                pItem->SetMagic3(m_pInfo->setItemInfo[i].ucMagic3);
                pItem->SetAmount(m_pInfo->setItemInfo[i].usAmount);
                pItem->SetAmountLimit(m_pInfo->setItemInfo[i].usAmountLimit);
                pItem->SetGemAtkType(m_pInfo->setItemInfo[i].dwGemAtkType);
                pItem->SetWarGhostExp(m_pInfo->setItemInfo[i].dwWarGhostExp);
                pItem->SetAvailabeTime(m_pInfo->setItemInfo[i].dwAvailabeTime);
                pItem->SetMonsterGrow(m_pInfo->setItemInfo[i].nData);
                if (pItem->GetPackageType() == PACKAGE_PET && strlen(m_pInfo->setItemInfo[i].szName))
                {
                    pItem->SetMonsterName(m_pInfo->setItemInfo[i].szName);
                }
                g_objHero.AddPackageItem(pItem);
            }
            ::PostCmd(CMD_FLASHPACKAGEITEM);
        }
        break;
    case MSGPACKAGE_CHECKIN:
        {
            // add
            // 有一个Info
            if (m_pInfo->usItemAmount == 0)
            {
                return;
            }
            CItem* pItem = new CItem;
            MYASSERT(pItem);
            pItem->Create(m_pInfo->setItemInfo[0].idItem, m_pInfo->setItemInfo[0].idItemType);
            pItem->SetStatus(m_pInfo->setItemInfo[0].ucStatus);
            pItem->SetGem1(m_pInfo->setItemInfo[0].ucGem1);
            pItem->SetGem2(m_pInfo->setItemInfo[0].ucGem2);
            pItem->SetMagic1(m_pInfo->setItemInfo[0].ucMagic1);
            pItem->SetMagic2(m_pInfo->setItemInfo[0].ucMagic2);
            pItem->SetMagic3(m_pInfo->setItemInfo[0].ucMagic3);
            pItem->SetAmount(m_pInfo->setItemInfo[0].usAmount);
            pItem->SetAmountLimit(m_pInfo->setItemInfo[0].usAmountLimit);
            pItem->SetGemAtkType(m_pInfo->setItemInfo[0].dwGemAtkType);
            pItem->SetWarGhostExp(m_pInfo->setItemInfo[0].dwWarGhostExp);
            pItem->SetAvailabeTime(m_pInfo->setItemInfo[0].dwAvailabeTime);
            pItem->SetMonsterGrow(m_pInfo->setItemInfo[0].nData);
            if (pItem->GetPackageType() == PACKAGE_PET && strlen(m_pInfo->setItemInfo[0].szName))
            {
                pItem->SetMonsterName(m_pInfo->setItemInfo[0].szName);
            }
            g_objHero.AddPackageItem(pItem);
            ::PostCmd(CMD_FLASHPACKAGEITEM);
        }
        break;
    case MSGPACKAGE_CHECKOUT:
        {
            // del
            // 看ID
            g_objHero.DelPackageItem(m_pInfo->idItem);
            ::PostCmd(CMD_FLASHPACKAGEITEM);
        }
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
