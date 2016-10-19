
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgItemInfo.h: interface for the CMsgItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGITEMINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_EX)
#define AFX_MSGITEMINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_EX

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "Item.h"

enum {	ITEMINFOEX_NONE = 0,
        ITEMINFOEX_BOOTH,					// for booth
        ITEMINFOEX_EQUIPMENT,				// equipment update
     };

class CMsgItemInfoEx : public CNetMsg
{
public:
    CMsgItemInfoEx();
    virtual ~CMsgItemInfoEx();

    BOOL	Create		(CItem* pItem, OBJID idOwner, int nCost, int nAction = ITEMINFOEX_BOOTH);
    BOOL	Create		(char* pMsgBuf, DWORD dwSize);
    void	Process		(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID		id;
        OBJID		idOwner;
        DWORD		dwPrice;
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

        DWORD		dwWarGhostExp;	//战魂经验值
        DWORD		dwGemAtkType;	//攻击类宝石的类型
        DWORD		dwAvailabeTime;	//攻击类宝石可用次数

        char		szBuf[1];		// 正常物品消息的时候，不包含附加的字符串
        // 当物品是幻兽的时候，第一个字符串就是幻兽名字
    } MSG_Info;

    MSG_Info*	m_pInfo;

private:
    CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGITEMINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_EX)
