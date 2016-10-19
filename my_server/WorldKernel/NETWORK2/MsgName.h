
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgName.h: interface for the CMsgName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGNAME_H__62DB03E2_7185_4BDF_B9B0_5AE64B03DFDB__INCLUDED_2)
#define AFX_MSGNAME_H__62DB03E2_7185_4BDF_B9B0_5AE64B03DFDB__INCLUDED_2

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

WORLDKERNEL_BEGIN
enum {	NAMEACT_NONE			= 0,
        NAMEACT_FIREWORKS		= 1,
        NAMEACT_CREATE_SYN		= 2, 				// 改到TALK中
        NAMEACT_SYNDICATE		= 3, 				// 无法实现修改帮派名，仅下传
        NAMEACT_CHANGE_TITLE	= 4,
        NAMEACT_DELROLE			= 5,				// 无意义
        NAMEACT_MATE			= 6,
        NAMEACT_QUERY_NPC		= 7,				// to server，to client
        NAMEACT_WANTED			= 8,				// to client
        NAMEACT_MAPEFFECT		= 9,				// to client
        NAMEACT_ROLEEFFECT		= 10,
        NAMEACT_MEMBERLIST		= 11,				// to server/client, dwData==-1 表示第一次QUERY

        NAMEACT_MEMBERLIST_SPECIFYSYN = 21,			//查询指定帮派的成员列表
     };

class CMsgName : public CNetMsg
{
public:
    CMsgName();
    virtual ~CMsgName();

    BOOL	Create(int nType, const char* pszName, DWORD dwData = 0);
    BOOL	CreateHead(int nType, DWORD dwData);
    BOOL	AppendString(const char* pString, DWORD dwData);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

private:
    typedef struct
    {
        MSGHEAD_DEFINE

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
WORLDKERNEL_END

#endif // !defined(AFX_MSGNAME_H__62DB03E2_7185_4BDF_B9B0_5AE64B03DFDB__INCLUDED_)
