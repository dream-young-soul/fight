
//**********************************************************
// ¥˙¬Î±‡º≠∆˜
//**********************************************************

// MsgPackage.h: interface for the CMsgPackage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGPACKAGE_H__739F5FDB_46FD_444B_9ADE_C157E831FBF8__INCLUDED_)
#define AFX_MSGPACKAGE_H__739F5FDB_46FD_444B_9ADE_C157E831FBF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

//ucType¿‡±£∫
enum
{
    // type
    MSGPACKAGE_TYPE_NONE		= 0,
    MSGPACKAGE_TYPE_STORAGE		= 10,	//≤÷ø‚
    MSGPACKAGE_TYPE_TRUNK		= 20,	//œ‰◊”
    MSGPACKAGE_TYPE_CHEST		= 30,	//∞Ÿ±¶ƒ“
    MSGPACKAGE_TYPE_EUDEMON_BROODER = 40,	//ª√ ﬁ∑ıªØ
    MSGPACKAGE_TYPE_EUDEMON_STORAGE = 50,	//ª√ ﬁ≤÷ø‚
    MSGPACKAGE_TYPE_AUCTION_STORAGE = 60,	//≈ƒ¬ÙNPC
};

enum
{
    MSGPACKAGE_QUERYLIST		= 0,			// to client / to server
    MSGPACKAGE_CHECKIN			= 1,			// to client / to server
    MSGPACKAGE_CHECKOUT			= 2,			// to client / to server
    MSGPACKAGE_QUERYLIST2		= 3,			// to client / to server
};
typedef struct
{
    OBJID		idItem;
    OBJID		idItemType;
    USHORT		usAmount;
    USHORT		usAmountLimit;
    UCHAR		ucStatus;
    UCHAR		ucGem1;
    UCHAR		ucGem2;
    UCHAR		ucMagic1;
    UCHAR		ucMagic2;
    UCHAR		ucMagic3;

    int nData;
    DWORD dwWarGhostExp;
    DWORD dwGemAtkType;
    DWORD dwAvailabeTime;
    char szName[_MAX_NAMESIZE];
} MsgPackageItemInfo;
class CMsgPackage : public CNetMsg
{
public:
    CMsgPackage();
    virtual ~CMsgPackage();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);
    BOOL			Create		(int nAction, int nType, OBJID id, OBJID idItem);

protected:
    typedef struct
    {
        USHORT	unMsgSize;
        USHORT	unMsgType;
        OBJID		id;						// userid, npcid, trunkid
        unsigned char	ucAction;
        unsigned char	ucType;
        union
        {
            struct
            {
                // to client: size
                USHORT				usItemAmount;
                MsgPackageItemInfo	setItemInfo[1];
            };
            OBJID		idItem;			// to client/server: action
        };
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGPACKAGE_H__739F5FDB_46FD_444B_9ADE_C157E831FBF8__INCLUDED_)
