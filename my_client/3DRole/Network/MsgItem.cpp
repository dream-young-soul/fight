
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgItem.cpp: implementation of the CMsgItem class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "hero.h"
#include "GameMsg.h"
#include "GameDataSet.h"
#include "skylayer.h"
#include "3dgamemap.h"
#include "gameplayerset.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgItem::CMsgItem()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgItem::~CMsgItem()
{
}

//////////////////////////////////////////////////////////////////////

BOOL CMsgItem::Create(OBJID id, int nAction)
{
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_ITEM;
    m_pInfo->id			= id;
    m_pInfo->usAction	= nAction;
    return true;
}

BOOL CMsgItem::Create(OBJID id, DWORD dwData, int nAction)
{
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_ITEM;
    m_pInfo->id			= id;
    m_pInfo->dwData		= dwData;
    m_pInfo->usAction	= nAction;
    return true;
}
//////////////////////////////////////////////////////////////////////
BOOL	CMsgItem::CreateForUseItem (OBJID id, DWORD data, int nAction, OBJID idTarget)
{
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_ITEM;
    m_pInfo->id			= id;
    m_pInfo->dwData		= data;
    m_pInfo->usAction	= nAction;
    m_pInfo->idTarget	= idTarget;
    return true;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgItem::Create(OBJID id, USHORT usPosX, USHORT usPosY, int nAction)
{
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_ITEM;
    m_pInfo->id			= id;
    m_pInfo->usPosX		= usPosX;
    m_pInfo->usPosY		= usPosY;
    m_pInfo->usAction	= nAction;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgItem::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_ITEM != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgItem::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogSave("Process MsgItem, ItemID:%d, ItemAction:%d",
              m_pInfo->id,
              m_pInfo->nAction);
#endif
    switch(m_pInfo->usAction)
    {
    case ITEMACT_DROP:
        {
            g_objHero.DelItem(m_pInfo->id, true);
            ::PostCmd(CMD_FLASHITEM);
        }
        break;
    case ITEMACT_QUERYMONEYSAVED:
        {
            g_objHero.SetSavedMoney(m_pInfo->dwData);
        }
    case ITEMACT_EQUIP:
        {
            CItem* pNewEquipment = g_objHero.GetItem(m_pInfo->id);
            if (pNewEquipment) // check
            {
                g_objHero.DelItem(pNewEquipment->GetID(), _DEL_PT);
                int nPosition = m_pInfo->dwData;
                CItem* pItem = g_objHero.GetEquipment(nPosition);
                if (pItem)
                {
                    g_objHero.AddItem(pItem);
                }
                if (pItem && !pNewEquipment->IsBow() && pItem->IsBow() && m_pInfo->dwData == ITEMPOSITION_WEAPONR)
                {
                    // 把左手武器也卸下
                    CItem* pLItem = g_objHero.GetEquipment(ITEMPOSITION_WEAPONL);
                    if (pLItem && pLItem->IsArrow())
                    {
                        g_objHero.SetEquipment(ITEMPOSITION_WEAPONL, NULL);
                    }
                    g_objHero.AddItem(pLItem);
                }
                g_objHero.SetEquipment(nPosition, pNewEquipment);
                if ( nPosition == ITEMPOSITION_SPRITE )
                {
                    g_objHero.SetSpriteEffect( pNewEquipment->GetLevelRequired() * 5 + pNewEquipment->GetProfessionRequired() ) ;
                }
                ::PostCmd(CMD_FLASHITEM);
                ::PostCmd(CMD_FLASHEQUIPMENT);
            }
        }
        break;
    case ITEMACT_UNEQUIP:
        {
            CItem* pItem = g_objHero.GetEquipment(m_pInfo->dwData);
            if (pItem)
            {
                if (pItem->IsBow() && m_pInfo->dwData == ITEMPOSITION_WEAPONR)
                {
                    // 把左手武器也卸下
                    CItem* pLItem = g_objHero.GetEquipment(ITEMPOSITION_WEAPONL);
                    if (pLItem && pLItem->IsArrow())
                    {
                        g_objHero.SetEquipment(ITEMPOSITION_WEAPONL, NULL);
                    }
                    g_objHero.AddItem(pLItem);
                }
                int nPosition = m_pInfo->dwData;
                g_objHero.SetEquipment(nPosition, NULL);
                g_objHero.AddItem(pItem);
                if ( nPosition == ITEMPOSITION_SPRITE )
                {
                    g_objHero.SetSpriteEffect( 255 ) ;
                }
                ::PostCmd(CMD_FLASHITEM);
                ::PostCmd(CMD_FLASHEQUIPMENT);
            }
        }
        break;
    case ITEMACT_DURABILITY:
        {
            int nPosition = m_pInfo->id;
            CItem* pItem = g_objHero.GetEquipment(nPosition);
            if (pItem)
            {
                int nDurability = m_pInfo->dwData;
                if (nDurability <= 0)	// this equipment is damaged
                {
                    /*	const OBJID idAttaint = 100105;
                    	g_objGameMsg.AddMsg(_TXTATR_SYSTEM, g_objGameDataSet.GetStr(idAttaint), pItem->GetName());

                    	g_objHero.SetEquipment(nPosition, NULL);
                    	SAFE_DELETE (pItem);*/
                }
                else
                {
                    /*
                    const OBJID idAttaint = 100106;
                    if (pItem->GetAmount() > nDurability)
                    	g_objGameMsg.AddMsg(_TXTATR_SYSTEM, g_objGameDataSet.GetStr(idAttaint), pItem->GetName());
                    */
                    pItem->SetAmount(nDurability);
                }
                ::PostCmd(CMD_FLASHEQUIPMENT);
            }
        }
        break;
    case ITEMACT_DROPEQUIPMENT:
        {
            int nPosition = m_pInfo->dwData;
            CItem* pItem = g_objHero.GetEquipment(nPosition);
            if (pItem)
            {
                MYASSERT (pItem->GetID() == m_pInfo->id);
                g_objHero.SetEquipment(nPosition, NULL);
                SAFE_DELETE (pItem);
                ::PostCmd(CMD_FLASHEQUIPMENT);
            }
        }
        break;
    case ITEMACT_BOOTH_ADD:
        {
            g_objHero.m_objBoothManager.AddItem(m_pInfo->id, m_pInfo->dwData, false);
        }
        break;
    case ITEMACT_BOOTH_DEL:
        {
            if (m_pInfo->dwData == g_objHero.m_objBooth.GetID())
            {
                g_objHero.m_objBooth.DelItem(m_pInfo->id);
            }
            if (m_pInfo->dwData == g_objHero.m_objBoothManager.GetID())
            {
                g_objHero.m_objBoothManager.DelItem(m_pInfo->id, false);
            }
        }
        break;
    case ITEMACT_BOOTH_BUY:
        {
            if (m_pInfo->dwData == g_objHero.m_objBooth.GetID())
            {
                g_objHero.m_objBooth.DelItem(m_pInfo->id);
            }
        }
        break;
    case ITEMACT_FIREWORKS:
        {
            CSkyLayer* pSky = g_objGameMap.GetSkyLayer();
            pSky->CreateFireWork(rand() % 200 + _SCR_WIDTH / 2 - 100 , _SCR_HEIGHT / 2 - rand() % 200 , 260 + rand() % 160, rand() % 5 + 1, 0);
            pSky->CreateFireWork(rand() % 200 + _SCR_WIDTH / 2 - 100 , _SCR_HEIGHT / 2 - rand() % 200 , 260 + rand() % 120, rand() % 5 + 1, 700 + rand() % 300);
            pSky->CreateFireWork(rand() % 200 + _SCR_WIDTH / 2 - 100 , _SCR_HEIGHT / 2 - rand() % 200 , 200 + rand() % 300, rand() % 5 + 1, 1000 + rand() % 600);
        }
        break;
    case ITEMACT_SYNCHRO_AMOUNT:
        {
            CItem* pItem = g_objHero.GetItem(m_pInfo->id, false);
            if (pItem)
            {
                pItem->SetAmount((unsigned short)m_pInfo->dwData);
            }
            ::PostCmd(CMD_FLASHITEM);
            ::PostCmd(CMD_FLASHEQUIPMENT);
        }
        break;
    case ITEMACT_EUDEMON_EVOLVE:
    case ITEMACT_EUDEMON_EVOLVE2:
        ::PostCmd(CMD_EVOMONSTER_SUC);
        break;
    case ITEMACT_ATTACH_EUDEMON:
        {
            CItem* pItem = g_objHero.GetItem(m_pInfo->id);
            if (pItem && pItem->GetPackageType () == PACKAGE_PET)
            {
                g_objHero.SetAttachPetId(m_pInfo->id);
                char szMsg[128] = "";
                sprintf(szMsg, g_objGameDataSet.GetStr(100172), pItem->GetMonsterName ());
                g_objGameMsg.AddMsg (szMsg); //合体成功
            }
            else
            {
                g_objHero.SetAttachPetId(ID_NONE);
            }
            ::PostCmd(CMD_FLASH_PETSTATUS);
        }
        break;
    case ITEMACT_DETACH_EUDEMON:
        {
            CItem* pItem = g_objHero.GetItem (m_pInfo->id);
            if (pItem && pItem->GetPackageType () == PACKAGE_PET)
            {
                char szMsg[128] = "";
                sprintf(szMsg, g_objGameDataSet.GetStr(100173), pItem->GetMonsterName ());	//解体
                g_objGameMsg.AddMsg (szMsg);
            }
            g_objHero.SetAttachPetId(ID_NONE);
            ::PostCmd(CMD_FLASH_PETSTATUS);
        }
        break;
    case ITEMACT_CALL_EUDEMON:
        break;
    case ITEMACT_KILL_EUDEMON:
        {
            static int nTime = 0;
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_pInfo->dwData);
            if (pPlayer)
            {
                CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
                if (!pLayer)
                {
                    return;
                }
                CMyPos posWorld;
                pPlayer->GetWorldPos(posWorld);
                C3DMapEffect* pObj = C3DMapEffect::CreateNew(posWorld, "CallbackPet");
                if (pObj)
                {
                    pLayer->AddMapObj(pObj);
                }
                if (pPlayer->IsMyPet())
                {
                    char szMsg[128] = "";
                    sprintf(szMsg, g_objGameDataSet.GetStr(100175), pPlayer->GetName ());
                    g_objGameMsg.AddMsg (szMsg);
                }
            }
        }
        break;
    }
}
