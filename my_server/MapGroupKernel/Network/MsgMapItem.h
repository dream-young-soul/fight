
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgItem.h: interface for the CMsgMapItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MSGMAPITEM_HEADER)
#define MSGMAPITEM_HEADER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum
{
    MSGMAPITEM_CREATE		= 1,			// to client
    MSGMAPITEM_DELETE		= 2,			// to client
    MSGMAPITEM_PICK			= 3,			// to server, client: perform action of pick

    MSGMAPITEM_CAST_TRAP	= 10,		// to client
    MSGMAPITEM_SYNCHRO_TRAP	= 11,		// to client
    MSGMAPITEM_DROP_TRAP	= 12,		// to client, id=trap_id
};

class CMapItem;
class CMapTrap;
class CMsgMapItem : public CNetMsg
{
public:
    CMsgMapItem();
    virtual ~CMsgMapItem();

    BOOL	Create	(int nAction, CMapItem* pMapItem);
    BOOL	Create	(int nAction, CMapTrap* pTrap);
    //	BOOL	Create	(int nAction, OBJID idUser);
    BOOL	Create	(OBJID id, int nPosX, int nPosY, int nAction = MSGMAPITEM_PICK);

public:
    BOOL	Create		(char* pMsgBuf, DWORD dwSize);
    void	Process		(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID			id;
       // union
      //  {
            OBJID			idType;		// for map_item
            USHORT			usLook;		// for trap
       // };
        USHORT			nPosX;
        USHORT			nPosY;
        UCHAR			nAction;

        char			szBuf[1];		// 若包含一个字符串，则表示地图物品的名字
    } MSG_Info;

    MSG_Info*	m_pInfo;

private:
    CNetStringPacker	m_StrPacker;
};

#endif // !defined(MSGMAPITEM_HEADER)
