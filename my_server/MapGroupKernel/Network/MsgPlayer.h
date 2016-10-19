
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgPlayer.h: interface for the CMsgPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGPLAYER_H__298DE32A_21B9_41E0_8728_CD895F36A959__INCLUDED_)
#define AFX_MSGPLAYER_H__298DE32A_21B9_41E0_8728_CD895F36A959__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"
#include "AiNpc.h"


//////////////////////////////////////////////////////////////////////
// 用于dwSynID_Rank
const DWORD	MASK_SYNID			= 0x00FFFFFF;
const DWORD	MASK_RANK_SHIFT		= 24;

class CUser;
class CMsgPlayer : public CNetMsg
{
public:
    CMsgPlayer();
    virtual ~CMsgPlayer();

public:
    //	BOOL			Create		(CBooth* pBooth);
    BOOL			Create		(IRole* pRole);

    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID id;
        DWORD dwLookFace;
        union
        {
            //I64 i64Status;
            DWORD	dwStatus[2];
            struct
            {
                USHORT usStatuaryLife; // 雕像生命
                USHORT usStatuaryFrame;// 雕像帧数
            };
        };

        union
        {
            DWORD	dwSynID_Rank;		// 非幻兽有效
            OBJID	idOwner;			// 幻兽有效
        };

        DWORD dwArmorType;
        DWORD dwWeaponRType;

        union
        {
            DWORD	dwMantleType;
            struct
            {
                USHORT	usMaxLife;				// 当Player为Monster的时候，表示最大生命
                USHORT	usMonsterLife;			// 当Player为Monster的时候，表示当前生命
            };
        };

        union
        {
            DWORD dwMountType;
            struct
            {
                USHORT usLife;		//	怪物有效
                USHORT usLevel;		//  怪物等级
            };
        };

        //unsigned short usMantleType;

        USHORT usPosX, usPosY;
        USHORT usHair;

        // zlong 2004-02-03
        char cLength;
        char cFat;

        UCHAR ucDir;
        UCHAR ucPose;

        union
        {
            struct
            {
                UCHAR ucActionSpeed;		// 行动速度
                UCHAR ucTutorLevel;			// 导师等级
                UCHAR ucMercenaryRank;		// 佣兵等级
                UCHAR ucNobilityRank;		// 爵位
            };
            OBJID	idMonsterType;			// 怪物和幻兽有效
        };
        // for str packer
        char szBuf[1];
    } MSG_Info;
    MSG_Info*	m_pInfo;

private:
    CNetStringPacker	m_StrPacker;

};

#endif // !defined(AFX_MSGPLAYER_H__298DE32A_21B9_41E0_8728_CD895F36A959__INCLUDED_)
