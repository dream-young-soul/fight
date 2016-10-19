
//**********************************************************
// 代码编辑器
//**********************************************************

///////////////////////////////////////////////////////////
//
//		CMsgMagicEffect.h
//

#ifndef		_MSGMAGICEFFECT_H_
#define		_MSGMAGICEFFECT_H_

#include "NetMsg.h"


//////////////////////////////////////////////////////////////////////
enum
{
    MAGICSORT_ATTACK			= 1,
    MAGICSORT_RECRUIT			= 2,			// auto active too.
    MAGICSORT_CROSS				= 3,
    MAGICSORT_FAN				= 4,
    MAGICSORT_BOMB				= 5,
    MAGICSORT_ATTACHSTATUS		= 6,
    MAGICSORT_DETACHSTATUS		= 7,
    MAGICSORT_SQUARE			= 8,
    MAGICSORT_JUMPATTACK		= 9,			// move, a-lock
    MAGICSORT_RANDOMTRANS		= 10,			// move, a-lock
    MAGICSORT_DISPATCHXP		= 11,
    MAGICSORT_COLLIDE			= 12,			// move, a-lock & b-synchro
    MAGICSORT_SERIALCUT			= 13,			// auto active only.
    MAGICSORT_LINE				= 14,			// support auto active(random).
    MAGICSORT_ATKRANGE			= 15,			// auto active only, forever active.
    MAGICSORT_ATKSTATUS			= 16,			// support auto active, random active.
    MAGICSORT_CALLTEAMMEMBER	= 17,
    MAGICSORT_RECORDTRANSSPELL	= 18,
    MAGICSORT_TRANSFORM			= 19,
    MAGICSORT_ADDMANA			= 20,			// support self target only.
    MAGICSORT_LAYTRAP			= 21,
    MAGICSORT_DANCE				= 22,			// 跳舞(only use for client)
};

///////////////////////////////////////////////////////////
typedef struct
{
    DWORD	idRole;
    DWORD	dwData;
} EffectRoleStruct;

class CMsgMagicEffect : public CNetMsg
{
public:
    CMsgMagicEffect();
    virtual ~CMsgMagicEffect();

public:		//接口函数
    BOOL	Create(OBJID idUser, int nType, int nLevel, OBJID idTarget, DWORD dwData);
    BOOL	CreateByPos(OBJID idUser, int nType, int nLevel, int x, int y, OBJID idTarget = ID_NONE, DWORD dwPower = 0);		// 成片攻击都传坐标
    BOOL	CreateCollide(OBJID idUser, int nType, int nLevel, OBJID idTarget, DWORD dwData, int nCollideDir);
    BOOL	AppendRole(OBJID idRole, DWORD dwData);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(CGameSocket* pSocket);

private:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID				idUser;
        union
        {
            OBJID				idTarget;
            struct
            {
                USHORT				usPosX;
                USHORT				usPosY;
            };
            UCHAR				ucCollideDir;			// 8: no move
        };
        USHORT				usType;
        USHORT				usLevel;
        UCHAR				ucEffectNum;		// 可能为0
        EffectRoleStruct	setEffectRole[1];			// 变长
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif
