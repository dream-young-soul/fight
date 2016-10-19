
//**********************************************************
// 代码编辑器
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: MsgEudemonAttrib.h
// Create by: Huang Yuhang
// Create on: 2004/5/31 10:25
//--------------------------------------------------------------------------------------
#ifndef		_MSGEUDEMONATTRIB_H
#define		_MSGEUDEMONATTRIB_H

#include "NetMsg.h"
enum
{
    _EA_ACTION_NONE		= 0, // ERROR
    _EA_ACTION_QUERY	= 1, // 查询属性
    _EA_ACTION_ATTRIB	= 2, // 返回属性
};
enum
{
    _EUDEMONATTRIB_ATK_MAX			=	0,
    _EUDEMONATTRIB_ATK_MIN			=	1,
    _EUDEMONATTRIB_MAGICATK_MAX		=	2,
    _EUDEMONATTRIB_MAGICATK_MIN		=	3,
    _EUDEMONATTRIB_DEF				=	4,
    _EUDEMONATTRIB_MAGICDEF			=	5,
    _EUDEMONATTRIB_ATKSPEED			=	6,
    _EUDEMONATTRIB_LIFE				=	7,
};

typedef struct
{
    DWORD dwAttributeType;
    DWORD dwAttributeData;
} EudemonAttribInfo;


class CMsgEudemonAttrib : public CNetMsg
{
public:
    CMsgEudemonAttrib();
    virtual ~CMsgEudemonAttrib();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

private:

    typedef struct
    {
        unsigned short		unMsgSize;
        unsigned short		unMsgType;

        OBJID idEudemon;
        DWORD dwAttributeNum;
        EudemonAttribInfo setEudemonAttrib[1]; // 变长
    } MSG_Info;
    MSG_Info*	m_pInfo;
};

#endif
