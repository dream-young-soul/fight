
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgMagicInfo.h: interface for the CMsgMagicInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGMAGICINFO_H__1188BAEC_D9BF_4F0D_8FEF_316D822FAD5E__INCLUDED_)
#define AFX_MSGMAGICINFO_H__1188BAEC_D9BF_4F0D_8FEF_316D822FAD5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgMagicInfo : public CNetMsg
{
public:
    CMsgMagicInfo();
    virtual ~CMsgMagicInfo();

public:
    BOOL	Create		(char* pMsgBuf, DWORD dwSize);
    void	Process		(void* pInfo);

protected:
    typedef struct
    {
        USHORT	unMsgSize;
        USHORT	unMsgType;

        OBJID			idOwner;	//魔法的所有者ID 用来区别是否是幻兽的技能
        DWORD			dwExp;
        unsigned short	usType;
        unsigned short  usLevel;

    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGMAGICINFO_H__1188BAEC_D9BF_4F0D_8FEF_316D822FAD5E__INCLUDED_)
