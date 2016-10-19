
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgItemAttrib.cpp: implementation of the CMsgItemAttrib class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "Item.h"
#include "Hero.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgItemAttrib::CMsgItemAttrib()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgItemAttrib::~CMsgItemAttrib()
{
}

BOOL CMsgItemAttrib::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_ITEMATTRIB != this->GetType())
    {
        return false;
    }
    return true;
}

void CMsgItemAttrib::Process(void* pInfo)
{
    CItem* pItem = g_objHero.GetItem(m_pInfo->idItem);
    if ( pItem == NULL)
    {
        return;
    }
    int nAmount = (int)m_pInfo->dwAttributeNum;
    for(int i = 0; i < nAmount; i++)
    {
        switch(m_pInfo->setItemAttrib[i].ucAttributeType)
        {
        case _ITEMATTRIB_GEMTYPE:
            //		case _ITEMATTRIB_EUDEMON_EXP:  //»ÃÊÞ¾­Ñé
            pItem->SetGemAtkType(m_pInfo->setItemAttrib[i].dwAttributeData);
            break;
        case _ITEMATTRIB_AMOUNT_LIMIT:
            //		case _ITEMATTRIB_EUDEMON_LEVEL: //»ÃÊÞµÈ¼¶
            pItem->SetAmountLimit((unsigned short)m_pInfo->setItemAttrib[i].dwAttributeData);
            break;
        case _ITEMATTRIB_AMOUNT:
            //		case _ITEMATTRIB_EUDEMON_FIDELITY: //»ÃÊÞÖÒ³Ï
            pItem->SetAmount((unsigned short)m_pInfo->setItemAttrib[i].dwAttributeData);
            break;
        case _ITEMATTRIB_WARGHOSTEXP:
            //		case _ITEMATTRIB_EUDEMON_LIFE:  //»ÃÊÞÉúÃü
            pItem->SetWarGhostExp(m_pInfo->setItemAttrib[i].dwAttributeData);
            break;
        default:
            break;
        }
    }
    if (pItem->GetPackageType() == PACKAGE_PET)
    {
        ::PostCmd(CMD_FLASH_MYPETINFO);
    }
}
