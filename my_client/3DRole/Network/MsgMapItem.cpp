
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgMapItem.cpp: implementation of the CMsgMapItem class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "3DGameMap.h"
#include "Player.h"
#include "GamePlayerSet.h"
#include "Item.h"
#include "../NdSound/NdSound.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgMapItem::CMsgMapItem()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgMapItem::~CMsgMapItem()
{
}

//-----------------------------------------------------------------------
BOOL CMsgMapItem::Create(OBJID idMapItem, int nPosX, int nPosY, unsigned char ucAction)
{
    if (idMapItem == ID_NONE)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_MAPITEM;
    m_pInfo->id = idMapItem;
    m_pInfo->ucAction = ucAction;
    m_pInfo->usPosX = (unsigned short)nPosX;
    m_pInfo->usPosY = (unsigned short)nPosY;
    return true;
}
//-----------------------------------------------------------------------
BOOL CMsgMapItem::Create(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_MAPITEM != this->GetType())
    {
        return false;
    }
    return true;
}
//-----------------------------------------------------------------------
void CMsgMapItem::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogSave("Process CMsgMapItem);
#endif
              switch(m_pInfo->ucAction)
          {
              case MSGMAPITEM_CREATE:
          {
              CMyPos posMapItem;
              posMapItem.x = m_pInfo->usPosX;
              posMapItem.y = m_pInfo->usPosY;
              char strName[_MAX_NAMESIZE]="";
              if (m_StrPacker.GetStrAmount()>0)
              m_StrPacker.GetString(0,strName,_MAX_NAMESIZE);
              g_objGameMap.AddMapItem(m_pInfo->id, m_pInfo->idType, posMapItem );
              if (g_objGameMap.IsPosVisible(posMapItem, 2))
          {
              ItemEffectInfo* pEffect = g_objGameDataSet.GetItemEffectInfo(m_pInfo->idType);
              if (pEffect && _strcmpi((char*)pEffect->strItemDropSound.c_str(), "NULL") != 0)
          {
              CMyPos posWorld;
              g_objGameMap.Cell2World(posMapItem.x, posMapItem.y, posWorld.x, posWorld.y);
              ::DXPlaySound((char*)pEffect->strItemDropSound.c_str(), posWorld.x, posWorld.y, 2000);
          }
          }
          }
              break;
              case MSGMAPITEM_DELETE:
          {
              g_objGameMap.DelMapItem(m_pInfo->id);
          }
              break;
              case MSGMAPITEM_PICK:
          {
              CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_pInfo->id);
              if (pPlayer)
          {
              pPlayer->ResetStepDir();
              pPlayer->SetActionCmd(_ACTION_PICKUP);
              pPlayer->SetPos(m_pInfo->usPosX, m_pInfo->usPosY);
          }
          }
              break;
              case MSGMAPITEM_CAST_TRAP:
          {
              CMyPos pos = {m_pInfo->usPosX, m_pInfo->usPosY};
              g_objGameMap.Add3DMapMagicItem(m_pInfo->idType, m_pInfo->id, pos);
          }
              break;
              case MSGMAPITEM_SYNCHRO_TRAP:
          {
              CMyPos pos = {m_pInfo->usPosX, m_pInfo->usPosY};
              g_objGameMap.Add3DMapMagicItem(m_pInfo->idType, m_pInfo->id, pos);
              g_objGameMap.LastMapMagicItem(m_pInfo->id);
          }
              break;
              case MSGMAPITEM_DROP_TRAP:
          {
              g_objGameMap.EndMapMagicItem(m_pInfo->id);
          }
              break;
              default:
              break;
          }
          }
              //-----------------------------------------------------------------------