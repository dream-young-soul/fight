
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgNpc.cpp: implementation of the CMsgNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "Npc.h"
#include "NpcManager.h"
#include "GameMap.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgNpc::CMsgNpc()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgNpc::~CMsgNpc()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgNpc::Create(int nAction, OBJID id, int nType /*= 0*/, DWORD dwData /*= 0*/)
{
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_NPC;
    m_pInfo->id			= id;
    m_pInfo->usAction	= (unsigned short)nAction;
    m_pInfo->usType		= nType;
    m_pInfo->dwData		= dwData;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgNpc::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgNpc::Process(CGameSocket* pSocket)
{
    //返回本NPC要说的话。CMsgTaskBarInfo
#ifdef _MYDEBUG
    ::LogSave("Process CMsgName, id:%u, usAction:%u",
              m_pInfo->id,
              m_pInfo->usAction);
#endif
    if (!pSocket)
    {
        return;
    }
    CUserPtr pUser	= UserManager()->GetUser(this);
    if (!pUser)
    {
        return ;
    }
    if (!pUser->IsAlive())
    {
        pUser->SendSysMsg(STR_DIE);
        return ;
    }
    switch(m_pInfo->usAction)
    {
    case EVENT_BEACTIVED:
        {
            CNpc* pNpc;
            IRole* pRole = RoleManager()->QuerySet()->GetObj(m_pInfo->id);
            if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pUser->GetMapID() == pNpc->GetMapID())
            {
                pNpc->ActivateNpc(pUser->QueryRole(), 0);
            }
        }
        break;
    case EVENT_DELNPC:
        {
            CNpc* pNpc;
            IRole* pRole = RoleManager()->QuerySet()->GetObj(m_pInfo->id);
            if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pUser->GetMapID() == pNpc->GetMapID())
            {
                if (pNpc->IsDeleted())
                {
                    return ;
                }
                if (pNpc->IsDelEnable() && pNpc->IsOwnerOf(pUser))		// 帮派柱子不能删除
                {
                    if (pNpc->IsRecycleEnable())
                    {
                        // 只检查普通背包是否满
                        if (pUser->IsItemFull(CItem::GetWeight(pNpc->GetInt(NPCDATA_ITEMTYPE)), ID_NONE, ITEMPOSITION_BACKPACK))
                        {
                            pUser->SendSysMsg(STR_YOUR_BAG_FULL);
                            return ;
                        }
                        CItem* pItem = pNpc->Recycle(pUser->GetID());
                        if (pItem)
                        {
                            pUser->AddItem(pItem, SYNCHRO_TRUE);
                            pUser->SendSysMsg(STR_GOT_ITEM, pItem->GetStr(ITEMDATA_NAME));
                        }
                        else
                        {
                            pUser->SendSysMsg(STR_ITEM_DAMAGED);
                        }
                    }
                    if (!pNpc->DelNpc())
                    {
                        pUser->SendSysMsg(STR_DELETE_FAILED);
                    }
                }
            }
        }
        break;
    case EVENT_CHANGEPOS:
        {
            CNpc* pNpc;
            IRole* pRole = RoleManager()->QuerySet()->GetObj(m_pInfo->id);
            if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pUser->GetMapID() == pNpc->GetMapID())
            {
                int	nLook = ::MaskLook(m_pInfo->usType);
                if (::MaskLook(pNpc->GetLookFace()) == nLook)
                {
                    pNpc->ChangeDir(nLook);
                }
                pNpc->TransPos(m_pInfo->usPosX, m_pInfo->usPosY);		// synchro true
            }
        }
        break;
    default:
        ::LogSave("ERROR: CMsgNpc::Process() 内部异常");
    }
}

