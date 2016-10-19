
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgSchoolMember.h: interface for the CMsgSchoolMember class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSCHOOLMEMBER_H__82E6220E_E387_4CA2_A66E_51466E54A627__INCLUDED_)
#define AFX_MSGSCHOOLMEMBER_H__82E6220E_E387_4CA2_A66E_51466E54A627__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum
{
    MESSAGEBOARD_DELMEMBER		= 1, //删除
    MESSAGEBOARD_ADDMEMBER		= 2, //添加
    MESSAGEBOARD_UPDATEMEMBER	= 3, //更新
};

enum
{
    RELATION_TEACHER = 1,		// 导师
    RELATION_STUDENT,			// 徒弟
    RELATION_STUDENTOFSTUDENT,	// 徒弟的徒弟(徒孙)
};

enum
{
    MEMBERSTATUS_OFFLINE = 0,	// 离线状态
    MEMBERSTATUS_ONLINE,		// 上线状态
};

typedef struct
{
    OBJID	idMember;				// 成员ID
    char	szName[_MAX_NAMESIZE];	// 成员名字
    char	szMate[_MAX_NAMESIZE];	// 配偶名字
    UCHAR	ucLevel;				// 等级
    UCHAR	ucProfession;			// 职业编号
    USHORT	usPkValue;				// PK值
    UCHAR	ucNobilityRank;			// 身份（爵位）
    UCHAR	ucRelation;				// 和玩家关系
    UCHAR	ucStatus;				// 状态 —— 如在线、离线
    DWORD	dwSynID_Rank;			// 帮派ID & 职位 (与MsgPlayer一样，高8位为职位编号，低24位为帮派ID)
} ST_MEMBERINFO;

class CMsgSchoolMember : public CNetMsg
{
public:
    CMsgSchoolMember();
    virtual ~CMsgSchoolMember();

public:
    BOOL	Create	(UCHAR ucAction, ST_MEMBERINFO* pMember, UCHAR ucAmount);
    BOOL	Append	(UCHAR ucRelation, UCHAR ucStatus, OBJID idMember, const char* pszName = NULL);
    BOOL	Append	(UCHAR ucRelation, UCHAR ucStatus, CUser* pUser);
    BOOL	Create	(char* pMsgBuf, DWORD dwSize);
    void	Process	(CGameSocket* pSocket);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        UCHAR		ucAction;			// 行为模式
        UCHAR		ucAmount;			// 数量
        ST_MEMBERINFO	setMember[1];	// 成员列表
    } MSG_Info;
    MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGSCHOOLMEMBER_H__82E6220E_E387_4CA2_A66E_51466E54A627__INCLUDED_)
