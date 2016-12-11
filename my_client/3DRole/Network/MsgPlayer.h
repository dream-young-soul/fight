
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

class CMsgPlayer : public CNetMsg
{
public:
    CMsgPlayer();
    virtual ~CMsgPlayer();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        unsigned short unMsgSize;
        unsigned short unMsgType;

        OBJID id;
        DWORD dwLookFace;
       // union
       // {
            DWORD dwStatus[2];
      //      struct
      //      {
                USHORT usStatuaryLife; // 雕像生命
                USHORT usStatuaryFrame;// 雕像帧数
      //      };
       // };

      //  union
      //  {
            DWORD dwSynID_Rank; // 非幻兽有效
            OBJID idOwner; // 幻兽有效
       // };

        DWORD dwArmorType;
        DWORD dwWeaponRType;
      //  union
      //  {
            DWORD dwMantleType;
       //     struct
       //     {
                USHORT	usMaxLife;		// 当Player为Monster的时候，表示最大生命
                USHORT	usMonsterLife;			// 当Player为Monster的时候，表示当前生命
       //     };
      //  };

       // union
       // {
            DWORD dwMountType;
       //     struct
       //     {
                USHORT usLife;		//	怪物有效
                USHORT usLevel;		//  怪物等级
       //     };
       // };

        USHORT usPosX, usPosY;
        USHORT usHair;

        char cLength; // 高矮
        char cFat; // 胖瘦


        UCHAR ucDir;
        UCHAR ucPose;

       // union
        //{
        //    struct
        //    {
                UCHAR ucActionSpeed; // 行动速度

                unsigned char ucTutorLevel; // 导师等级
                unsigned char ucMercenaryLevel;	// 佣兵等级
                unsigned char ucNobilityRank; // 爵位系统
				 USHORT flag; //补位
        //    };
            DWORD dwMonsterType;
       // };
        // for str packer
        char szBuf[1];
    } MSG_Info;



    MSG_Info*	m_pInfo;

private:
    CNetStringPacker	m_StrPacker;

};

#endif // !defined(AFX_MSGPLAYER_H__298DE32A_21B9_41E0_8728_CD895F36A959__INCLUDED_)
