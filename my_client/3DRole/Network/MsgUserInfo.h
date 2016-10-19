
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

class CMsgUserInfo : public CNetMsg
{
public:
    CMsgUserInfo();
    virtual ~CMsgUserInfo();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        unsigned short unMsgSize;
        unsigned short unMsgType;

        OBJID id;
        DWORD dwLookFace; // 外貌
        USHORT usHair; // 头发
        char cLength; // 高度
        char cFat; // 胖瘦

        DWORD dwMoney;
        LONG nExp;

        //LONG lDeed;
        //DWORD dwVirtue;

        int	  dwTutorExp;	//	导师经验
        DWORD dwMercenaryExp;	// 佣兵经验


        int	   nPotential; // 潜能
        USHORT usForce; // 力量
        USHORT usConstitution; // 体质
        USHORT usDexterity; // 敏捷
        USHORT usSpeed; // 速度 -- 客户端的移动速度
        USHORT usHealth; // 耐力
        USHORT usSoul; // 精神

        USHORT usAdditionalPoint;

        USHORT usLife;
        USHORT usMaxLife; // 客户端放弃计算最大生命了，改由服务器提供
        USHORT usMana;
        SHORT  sPk;

        UCHAR ucLevel;
        UCHAR ucProfession;
        UCHAR ucNobility; // to be deleted
        UCHAR ucMetempsychosis; // to be deleted
        UCHAR ucAutoAllot;

        UCHAR ucTutorLevel; // 导师等级
        UCHAR ucMercenaryLevel; // 佣兵等级

        UCHAR ucNobilityRank; // 爵位
        USHORT usMaxEudemon; // 最大可召唤幻兽数
        DWORD dwExploit; // 功勋值

        // for str packer
        char szBuf[1];
    } MSG_Info;


    MSG_Info*	m_pInfo;

private:
    CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGUSERINFO_H__DFC94B21_4F65_11D5_A212_0000E8E7F527__INCLUDED_)
