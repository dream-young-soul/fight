
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgItemInfoEx.h: interface for the CMsgItemInfoEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGITEMINFOEX_H__605ACCB5_24D4_4DB0_9449_C42A2F15BBD1__INCLUDED_)
#define AFX_MSGITEMINFOEX_H__605ACCB5_24D4_4DB0_9449_C42A2F15BBD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"

enum
{
    ITEMINFOEX_NONE = 0,
    ITEMINFOEX_BOOTH,					// for booth
    ITEMINFOEX_EQUIPMENT,				// for update equipment info
};

class CMsgItemInfoEx   : public CNetMsg
{
public:
    CMsgItemInfoEx();
    virtual ~CMsgItemInfoEx();

public:
    BOOL	Create		(char* pMsgBuf, DWORD dwSize);
    void	Process		(void* pInfo);

protected:
    typedef struct
    {
        USHORT		unMsgSize;
        USHORT		unMsgType;

        OBJID		id;
        OBJID		idOwner;
        DWORD		dwPrice;
        DWORD		dwType;
        USHORT		usAmount;
        USHORT		usAmountLimit;
        UCHAR		ucAction;
        UCHAR		ucStatus;
        UCHAR		ucPosition;
        UCHAR		ucGem1;
        UCHAR		ucGem2;
        UCHAR		ucMagic1;
        UCHAR		ucMagic2;
        UCHAR		ucMagic3;
        int			nData;

        DWORD dwWarGhostExp; //战魂经验值
        DWORD dwGemAtkType; //攻击类宝石的类型
        DWORD dwAvailabeTime; //攻击类宝石可用次数

        char szBuf[1]; // 正常物品消息的时候，不包含附加的字符串
        // 当物品是幻兽的时候，第一个字符串就是幻兽名字

    } MSG_Info;

    MSG_Info*	m_pInfo;
private:
    CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGITEMINFOEX_H__605ACCB5_24D4_4DB0_9449_C42A2F15BBD1__INCLUDED_)
