
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgDataArray.h: interface for the CMsgCharArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGCHARARRAY_H__9C42BA31_AE9D_493E_9336_BD1BC2615921__INCLUDED_)
#define AFX_MSGCHARARRAY_H__9C42BA31_AE9D_493E_9336_BD1BC2615921__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum
{
    _DATAARRAY_ACTION_MEDAL				=	0,	//	unsigned char	// 勋章
    _DATAARRAY_ACTION_HONORTITLE		=	1,	//	unsigned char	// 称号

    //---jinggy---2004-11-9---铸造系统---begin
    _DATAARRAY_ACTION_UPGRADE_QUALITY		= 2, //升级装备品质
    _DATAARRAY_ACTION_UPGRADE_SUPERADDITION = 3, //升级装备追加
    _DATAARRAY_ACTION_UPGRADE_EQUIPLEVEL	= 4, //升级装备等级
    _DATAARRAY_ACTION_EMBEDGEM				= 5, //对装备嵌入宝石
    //---jinggy---2004-11-9---铸造系统---end

    // add time -2005-3-21-Xliang
    _DATAARRAY_ACTION_SETSYN_DRESS	= 50,	//客户端向服务器。设置帮派服装，一共四个等级的服装,参数：ucAmount，usShort
    _DATAARRAY_SETITEMREBATE	= 60,  //设置物品的折扣，参数：idNPC，nRebate
};

#define MAX_LEVEL_SUPERADDITION 9 //指追加属性的最高级别

class CMsgDataArray   : public CNetMsg
{
public:
    CMsgDataArray();
    virtual ~CMsgDataArray();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

    BOOL	Create(int nAction, int nAmount, char setChar[]);
    BOOL	Create(int nAction, int nAmount, short setShort[]);
    BOOL	Create(int nAction, int nAmount, long setLong[]);

protected:
    typedef struct
    {
        USHORT			unMsgSize;
        USHORT			unMsgType;
        unsigned char	ucAction;
        unsigned char	ucAmount;
        union
        {
            unsigned char	ucChar[1];
            char			cChar[1];
            unsigned short	usShort[1];
            short			sShort[1];
            unsigned long	dwLong[1];
            long			nLong[1];
        };
    } MSG_Info;
    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGCHARARRAY_H__9C42BA31_AE9D_493E_9336_BD1BC2615921__INCLUDED_)
