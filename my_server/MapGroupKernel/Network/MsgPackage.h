
//**********************************************************
// 代码编辑器
//**********************************************************

///////////////////////////////////////////////////////////
//
//		CMsgPackage.h
//

#ifndef		MSGPACKAGE_H
#define		MSGPACKAGE_H

#include "NetMsg.h"

enum
{
    MSGPACKAGE_QUERYLIST			= 0,
    MSGPACKAGE_CHECKIN				= 1,
    MSGPACKAGE_CHECKOUT				= 2,
    MSGPACKAGE_QUERYLIST2			= 3,	// 客户端收到此消息表示追加仓库物品，不清空原来的
};

enum  	// type
{
    MSGPACKAGE_TYPE_NONE			= 0,
    MSGPACKAGE_TYPE_STORAGE			= 10,
    MSGPACKAGE_TYPE_TRUNK			= 20,
    MSGPACKAGE_TYPE_CHEST			= 30,
    MSGPACKAGE_TYPE_EUDEMON_BROODER	= 40,
    MSGPACKAGE_TYPE_EUDEMON_STORAGE	= 50,
    MSGPACKAGE_TYPE_AUCTION_STORAGE = 60,//拍买所得到的物品LW
};


typedef struct
{
    OBJID idItem;
    OBJID idItemType;
    USHORT		usAmount;
    USHORT		usAmountLimit;
    UCHAR		ucIdent;
    UCHAR		ucGem1;
    UCHAR		ucGem2;
    UCHAR		ucMagic1;
    UCHAR		ucMagic2;
    UCHAR		ucMagic3;

    int			nData;
    DWORD		dwWarGhostExp;
    DWORD		dwGemAtkType;
    DWORD		dwAvailableTime;
    char		szName[_MAX_NAMESIZE];
} MsgPackageItemInfo;

class CMsgPackage : public CNetMsg
{
public:
    CMsgPackage();
    virtual ~CMsgPackage();

public:		//接口函数
    // modified by zlong 2003-11-24 ---- Create函数添加ucType参数指定类别
    BOOL			Create(int nAction, OBJID id, const MsgPackageItemInfo* buf, int nSize, UCHAR ucType);
    BOOL			Create(int nAction, OBJID id, OBJID idItem, UCHAR ucType);
public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(CGameSocket* pSocket);

private:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID		id;						// userid, npcid, trunkid
        UCHAR		ucAction;
        UCHAR		ucType;
        union
        {
            struct 							// to client: size
            {
                USHORT		usSize;
                MsgPackageItemInfo	setItemInfo[1];
            };
            OBJID		idItem;			// to client/server: action
        };
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // MSGPACKAGE_H
