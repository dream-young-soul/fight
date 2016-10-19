
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgName.h: interface for the CMsgName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGNAME_H__62DB03E2_7185_4BDF_B9B0_5AE64B03DFDB__INCLUDED_)
#define AFX_MSGNAME_H__62DB03E2_7185_4BDF_B9B0_5AE64B03DFDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"

enum
{
    NAMEACT_NONE				= 0,
    NAMEACT_FIREWORKS			= 1,
    NAMEACT_CREATE_SYN			= 2,
    NAMEACT_CHANGE_SYN			= 3,
    NAMEACT_CHANGE_SYNTITLE		= 4,
    NAMEACT_DELROLE				= 5,
    NAMEACT_MATE				= 6,
    NAMEACT_QUERY_NPC			= 7,
    NAMEACT_WANTED				= 8,
    NAMEACT_MAPEFFECT			= 9,
    NAMEACT_ROLEEFFECT			= 10,
    NAMEACT_MEMBERLIST			= 11,		//dwData==-1 表示第一次QUERY
    MANEACT_KICKOUT_SYNMEM		= 12,
    NAMEACT_QUERY_WANTED		= 13,
    NAMEACT_QUERY_POLICEWANTED	= 14,
    NAMEACT_POLICEWANTED		= 15,
    NAMEACT_QUERY_MATE			= 16,

    NAMEACT_ADDDICE_PLAYER		= 17, //ToClient 增加骰子玩家// dwData为骰子摊ID // To Server 加入 需要原消息返回
    NAMEACT_DELDICE_PLAYER		= 18, //ToClient 删除骰子玩家// dwData为骰子摊ID // To Server 离开 需要原消息返回
    NAMEACT_DICE_BONUS			= 19, // 报骰子奖金 玩家// dwData为Money

    NAMEACT_PLAYER_WAVE			= 20, // 播音效 // 如果usPosX = usPosY = 0; 非地图音效

    NAMEACT_MEMBERLIST_SPECIFYSYN = 21,			//查询指定帮派的成员列表
    //		NAMEACT_SYN_FRIEND			= 21, 				//
    NAMEACT_SYN_HOSTILE			= 22, 				//

    NAMEACT_PLAYERTASKLIST		= 23, // 任务查询返回
    NAMEACT_CHANGE_EUDEMON_NAME = 24, // 幻兽改名。idTarget=幻兽物品ID，第一个字符串为幻兽新名字
};

class CMsgName : public CNetMsg
{
public:
    CMsgName();
    virtual ~CMsgName();

    BOOL	Create(int nType, const char* pszName, DWORD dwData = 0);
    BOOL	Create(int nType, const char* pszName, USHORT usPosX, USHORT usPosY);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

private:
    typedef struct
    {
        USHORT	unMsgSize;
        USHORT	unMsgType;

        union
        {
            OBJID	idTarget;
            DWORD	dwData;
            struct
            {
                USHORT  usPosX, usPosY;
            };
        };
        UCHAR	ucType;
        char	szBuf[1];
    } MSG_Info;

    MSG_Info*	m_pInfo;
private:
    CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGNAME_H__62DB03E2_7185_4BDF_B9B0_5AE64B03DFDB__INCLUDED_)
