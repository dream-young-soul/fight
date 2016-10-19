
//**********************************************************
// 代码编辑器
//**********************************************************

///////////////////////////////////////////////////////////
//
//		CMsgUserAttrib.h
//

#ifndef		_MSGUSERSTATUS_H_
#define		_MSGUSERSTATUS_H_

#include "NetMsg.h"


// 取消32位的掩码限制
//const _MAX_MSGATRRIB	=32;			// ★：由于取数值属性时使用32位整数作为DIRTY掩码，故该值不能超过32。

enum {  _USERATTRIB_LIFE				= 0,
        _USERATTRIB_MAXLIFE,
        _USERATTRIB_MANA				= 2,
        _USERATTRIB_MAXMANA,
        _USERATTRIB_MONEY				= 4,
        _USERATTRIB_EXP,
        _USERATTRIB_PK					= 6,
        _USERATTRIB_PORFESSION,
        _USERATTRIB_SIZEADD				= 8,
        _USERATTRIB_ENERGY,
        _USERATTRIB_MONEYSAVED			= 10,
        _USERATTRIB_ADDPOINT,
        _USERATTRIB_LOOKFACE			= 12,
        _USERATTRIB_LEV,
        _USERATTRIB_SOUL				= 14,
        _USERATTRIB_HEALTH,
        _USERATTRIB_FORCE				= 16,
        _USERATTRIB_SPEED,						// 速度 -- 客户端用的，与敏捷没有任何关系
        _USERATTRIB_NOBILITYRANK		= 18,	// 爵位
        //_USERATTRIB_DEGREE				= 18,
        _USERATTRIB_POSE,
        _USERATTRIB_PROFFER				= 20,
        _USERATTRIB_LENGTH,						// zlong 2004-02-03
        _USERATTRIB_SUPERMAP_ORDER		= 22,
        //23
        _USERATTRIB_EXPLOIT				= 24,	// 功勋值
        _USERATTRIB_MERCENARYRANK,				// 佣兵等级
        _USERATTRIB_KEEPEFFECT			= 26,	// 低32位的魔法状态
        _USERATTRIB_HAIR,
        _USERATTRIB_XP					= 28,
        _USERATTRIB_ACTIONSPEED			= 29,	// 行动速度  zlong 2004.09.18
        _USERATTRIB_MERCENARYEXP		= 30,	// 佣兵经验
        _USERATTRIB_FAT					= 31,	// zlong 2004-02-03
        _USERATTRIB_TUTOR_EXP			= 32,	// 导师等级
        _USERATTRIB_TUTOR_LEVEL			= 33,	// 导师经验
        _USERATTRIB_DEX					= 34,	// 敏捷
        _USERATTRIB_POTENTIAL			= 35,	// 潜能
        _USERATTRIB_KEEPEFFECT2			= 36,	// 高32位的魔法状态
        _USERATTRIB_MAXENERGY			= 37,	// 最大体力
        _USERATTRIB_ARMORTYPE			= 38,	// 帮派成员时，改变玩家的身体的颜色
        _USERATTRIB_MAXEUDEMON			= 39,	// 最大可召唤幻兽数

        _USERATTRIB_NONE	= -1,			// 非法值
     };

typedef struct
{
    DWORD	ucAttributeType;
    DWORD	dwAttributeData;
} UserAttribStruct;

class CMsgUserAttrib : public CNetMsg
{
public:
    CMsgUserAttrib();
    virtual ~CMsgUserAttrib();

public:		//接口函数
    //	BOOL	Create(OBJID idUser, int nType, DWORD dwData);
    BOOL	Create(OBJID idUser, int nType, I64 i64Data);

    //	BOOL	Append(int nType, DWORD dwData);
    BOOL	Append(int nType, I64 i64Data);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(CGameSocket* pSocket);

private:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID				idUser;
        DWORD				dwAttributeNum;
        UserAttribStruct	setUserAttrib[1];			// 变长
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif
