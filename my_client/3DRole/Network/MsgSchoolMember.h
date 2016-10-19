
//**********************************************************
// 代码编辑器
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: MsgSchoolMember.h
// Create by: Huang Yuhang
// Create on: 2004/9/4 15:53
//--------------------------------------------------------------------------------------
#ifndef _MSGSCHOOLMEMBER_H
#define _MSGSCHOOLMEMBER_H

#include "NetMsg.h"
#include "NetStringPacker.h"
enum
{
    MESSAGEBOARD_DELMEMBER				= 1,	//删除
    MESSAGEBOARD_ADDMEMBER				= 2,	//添加
    MESSAGEBOARD_UPDATEMEMBER			= 3,	//更新
};

class CMsgSchoolMember : public CNetMsg
{
public:
    CMsgSchoolMember();
    virtual ~CMsgSchoolMember();


public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);
    //	BOOL			Create		(unsigned short usIndex, unsigned char ucAction, unsigned short usChannel, char* pszStr = NULL);
    typedef struct
    {
        OBJID idMember; // 成员ID
        char szName[_MAX_NAMESIZE]; // 成员名字
        char szMate[_MAX_NAMESIZE]; // 配偶名字
        UCHAR ucLevel; // 等级
        UCHAR ucProfession; // 职业编号
        USHORT usPkValue; // PK值
        UCHAR ucNobilityRank; // 身份（爵位）
        UCHAR ucRelation; // 和玩家关系
        UCHAR ucStatus; // 状态 —— 如在线、离线
        DWORD dwSynID_Rank; // 帮派ID & 职位 (与MsgPlayer一样，高8位为职位编号，低24位为帮派ID)
    } ST_MEMBERINFO;

    typedef struct
    {
        USHORT unMsgSize;
        USHORT unMsgType;

        UCHAR ucAction; // 行为模式
        UCHAR ucAmount; // 数量
        ST_MEMBERINFO setMember[1]; // 成员列表
    } MSG_Info;
    MSG_Info*	m_pInfo;
};

#endif
