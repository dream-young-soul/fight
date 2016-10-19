
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
    BOOL	Create(OBJID idUser, int nType, int nLevel, OBJID idTarget, UCHAR ucDir);
    BOOL	Create(OBJID idUser, int nType, int nLevel, OBJID idTarget, DWORD dwData, UCHAR ucDir);
    BOOL	CreateByPos(OBJID idUser, int nType, int nLevel, int x, int y, UCHAR ucDir, OBJID idTarget = ID_NONE, DWORD dwPower = 0);		// 成片攻击都传坐标
    BOOL	CreateCollide(OBJID idUser, int nType, int nLevel, OBJID idTarget, DWORD dwData, int nCollideDir, UCHAR ucDir);
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
        UCHAR				ucDir;
        UCHAR				ucEffectNum;		// 可能为0
        EffectRoleStruct	setEffectRole[1];			// 变长
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif
