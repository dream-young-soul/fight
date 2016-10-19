
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgMagicIntone.h: interface for the MsgMagicIntone class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGMAGICINTONE_H__CA975F3E_ECD3_4C5D_97BB_077A791AC3AC__INCLUDED_)
#define AFX_MSGMAGICINTONE_H__CA975F3E_ECD3_4C5D_97BB_077A791AC3AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NetMsg.h"
#include "Magic.h"


enum MAGIC_GROUP_TYPE
{
    MAGICGROUP_NONE	= 0,
    MAGICGROUP_ATTACK_NOMOVE,	// 合击1，幻兽吟唱原地攻击
    MAGICGROUP_ATTACK_MOVE,		// 合击2，幻兽跑位吟唱攻击
    MAGICGROUP_ATTACK_RAND,		// 幻兽攻击玩家合法范围内的某一随机对象
    MAGICGROUP_ATTACK_SAME,		// 幻兽攻击与玩家相同的目标
};

typedef struct
{
    DWORD	idEudemon;
    USHORT	usOffsetX, usOffsetY;
} EudemonPos_ST;


class CMsgMagicIntone  : public CNetMsg
{
public:
    CMsgMagicIntone();
    virtual ~CMsgMagicIntone();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

private:
    typedef struct
    {
        USHORT	unMsgSize;
        USHORT	unMsgType;

        OBJID	idUser;
        USHORT	usPosX, usPosY;

        USHORT	usMagicType;
        USHORT	usMagicLevel;
        UCHAR	ucIntoneType;
        UCHAR	ucDir;
        UCHAR	ucEudemonNum;		// 可能为0
        EudemonPos_ST	setEudemonPos[1];	// 变长
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGMAGICINTONE_H__CA975F3E_ECD3_4C5D_97BB_077A791AC3AC__INCLUDED_)
