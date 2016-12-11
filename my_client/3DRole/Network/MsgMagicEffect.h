
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgMagicEffect.h: interface for the CMsgMagicEffect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGMAGICEFFECT_H__C7006740_20DD_4149_A29C_4CE4006E7FC7__INCLUDED_)
#define AFX_MSGMAGICEFFECT_H__C7006740_20DD_4149_A29C_4CE4006E7FC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MagicEffect.h"
#include "NetMsg.h"

class CMsgMagicEffect : public CNetMsg
{
public:
    CMsgMagicEffect();
    virtual ~CMsgMagicEffect();

public:
    BOOL	Create		(char* pMsgBuf, DWORD dwSize);
    void	Process		(void* pInfo);

protected:
    typedef struct
    {
        USHORT	unMsgSize;
        USHORT	unMsgType;

        OBJID				idUser;
       // union
       // {
            OBJID				idTarget;
       //     struct
       //     {
                USHORT				usPosX;
                USHORT				usPosY;
       //     };
       // };
        USHORT				usType;
        USHORT				usLevel;
        UCHAR				ucDir;
        UCHAR				ucEffectNum;				// 可能为0
		USHORT		flag;
        MagicEffectRoleInfo	setEffectRole[1];			// 变长

    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGMAGICEFFECT_H__C7006740_20DD_4149_A29C_4CE4006E7FC7__INCLUDED_)
