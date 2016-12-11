
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgMapItem.h: interface for the CMsgMapItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGMAPITEM_H__010D6614_C45C_48F7_83F5_640FE069D2D0__INCLUDED_)
#define AFX_MSGMAPITEM_H__010D6614_C45C_48F7_83F5_640FE069D2D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"

enum
{
    MSGMAPITEM_CREATE = 1,	//创建	// to client
    MSGMAPITEM_DELETE = 2,	//删除	// to client
    MSGMAPITEM_PICK = 3,		//捡起	// to server

    MSGMAPITEM_CAST_TRAP	= 10,		// to client
    MSGMAPITEM_SYNCHRO_TRAP	= 11,		// to client
    MSGMAPITEM_DROP_TRAP	= 12,		// to client, id=trap_id
};

class CMsgMapItem  : public CNetMsg
{
public:
    CMsgMapItem();
    virtual ~CMsgMapItem();
public:
    BOOL Create(OBJID idMapItem, int nPosX, int nPosY, unsigned char ucAction = MSGMAPITEM_PICK);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        USHORT			unMsgSize;
        USHORT			unMsgType;

        OBJID			id;
        //union
       // {
            OBJID			idType;		// for map_item
            USHORT			usLook;		// for trap
       // };
        unsigned short	usPosX;
        unsigned short	usPosY;
        unsigned char	ucAction;
        char			szBuf[1]; // 若包含一个字符串，则表示地图物品的名字
    } MSG_Info;

    MSG_Info*	m_pInfo;
private:
    CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGMAPITEM_H__010D6614_C45C_48F7_83F5_640FE069D2D0__INCLUDED_)
