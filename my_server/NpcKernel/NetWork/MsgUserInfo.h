
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgUserInfo.h: interface for the CMsgUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGUSERINFO_H__DFC94B21_4F65_11D5_A212_0000E8E7F527__INCLUDED_)
#define AFX_MSGUSERINFO_H__DFC94B21_4F65_11D5_A212_0000E8E7F527__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

struct UserInfoStruct;
class CMsgUserInfo : public CNetMsg
{
public:
    CMsgUserInfo();
    virtual ~CMsgUserInfo();

public:
    BOOL			Create		(OBJID idUser, const UserInfoStruct* pInfo);
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID id;
        DWORD dwLookFace;		// 外貌
        USHORT usHair;			// 头发
        char cLength;			// 高度 -- zlong 2004-02-03
        char cFat;				// 胖瘦 -- zlong 2004-02-03
        DWORD dwMoney;			// 金钱
        LONG nExp;				// 经验

        int		nTutorExp;			// 导师经验
        DWORD	dwMercenaryExp;		// 佣兵经验

        int    nPotential;			// 潜能
        USHORT usForce;				// 力量
        USHORT usConstitution;		// 体质
        USHORT usDexterity;			// 敏捷
        USHORT usSpeed;				// 速度 -- 客户端用的移动速度
        USHORT usHealth;			// 耐力
        USHORT usSoul;				// 精神

        USHORT usAdditionalPoint;

        USHORT usLife;
        USHORT usMaxLife;
        USHORT usMana;
        SHORT sPk;

        UCHAR ucLevel;
        UCHAR ucProfession;
        UCHAR ucNobility;
        UCHAR ucMetempsychosis;
        UCHAR ucAutoAllot;

        UCHAR ucTutorLevel;		// 导师等级
        UCHAR ucMercenaryRank;	// 佣兵等级

        UCHAR ucNobilityRank;	// 爵位
        DWORD dwExploit;		// 功勋值

        // for str packer
        char			szBuf[1];
        //	name[16]
        //	mate[16]
    } MSG_Info;
    MSG_Info*	m_pInfo;

private:
    CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGUSERINFO_H__DFC94B21_4F65_11D5_A212_0000E8E7F527__INCLUDED_)
