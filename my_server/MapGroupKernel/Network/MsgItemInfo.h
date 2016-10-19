
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgItemInfo.h: interface for the CMsgItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGITEMINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
#define AFX_MSGITEMINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "AuctionSystemItemData.h"


enum {	ITEMINFO_NONE = 0,
        ITEMINFO_ADDITEM,				// 添加物品
        ITEMINFO_TRADE,					// 公平交易对方添加的物品
        ITEMINFO_UPDATE,				// update item info
        ITEMINFO_OTHERPLAYER_EQUIPMENT, // 返回其他玩家装备// id is user id
        ITEMINFO_AUCTION,               // 拍买查看物品
     };

class CItem;
class CMsgItemInfo : public CNetMsg
{
public:
    CMsgItemInfo();
    virtual ~CMsgItemInfo();

    BOOL	Create		(CItem* pItem, int nAction = ITEMINFO_ADDITEM, OBJID idUser = ID_NONE);
    BOOL	Create		(CGameAuctionSystemData* pData, int nAction);
    BOOL	Create		(char* pMsgBuf, DWORD dwSize);
    void	Process		(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID		id;
        DWORD		dwType;
        USHORT		usAmount;
        USHORT		usAmountLimit;
        UCHAR		ucAction;
        UCHAR		ucIdent;
        UCHAR		ucPosition;
        UCHAR		ucGem1;
        UCHAR		ucGem2;
        UCHAR		ucMagic1;
        UCHAR		ucMagic2;
        UCHAR		ucMagic3;
        int			nData;
        //---jinggy---2004-11-19---begin
        DWORD		dwWarGhostExp; //战魂经验值
        DWORD		dwGemAtkType; //攻击类宝石的类型
        DWORD		dwAvailabeTime; //攻击类宝石可用次数
        //---jinggy---2004-11-19---end

        char		szBuf[1];		// 正常物品消息的时候，不包含附加的字符串
        // 当物品是幻兽的时候，第一个字符串就是幻兽名字
    } MSG_Info;

    MSG_Info*	m_pInfo;

private:
    CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGITEMINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
