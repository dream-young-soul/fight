
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgSynMemberInfo.h: interface for the CMsgSynMemberInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSYNMEMBERINFO_H__CB607DAF_CB16_40F2_A459_E8963D4B91AD__INCLUDED_)
#define AFX_MSGSYNMEMBERINFO_H__CB607DAF_CB16_40F2_A459_E8963D4B91AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NetMsg.h"


class CMsgSynMemberInfo  : public CNetMsg
{
public:
    CMsgSynMemberInfo();
    virtual ~CMsgSynMemberInfo();
public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);
    BOOL			Create		(char* pszName);
protected:
    typedef struct
    {
        USHORT	unMsgSize;
        USHORT	unMsgType;
        int		nProffer; // 贡献度
        UCHAR	ucRank; // 职位
        UCHAR	ucProfession; // 职业
        UCHAR	ucNobilityRank; // 爵位
        UCHAR	ucLevel; // 等级
        SHORT	sPk; // PK值
        DWORD	dwLookFace; // 头像
        char	szName[_MAX_NAMESIZE]; // 名字
        char	szMate[_MAX_NAMESIZE]; // 配偶名字

    } MSG_Info;

    MSG_Info*	m_pInfo;
};


#endif // !defined(AFX_MSGSYNMEMBERINFO_H__CB607DAF_CB16_40F2_A459_E8963D4B91AD__INCLUDED_)
