
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgSynAttrInfo.h: interface for the CMsgSynAttrInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSYNATTRINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
#define AFX_MSGSYNATTRINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "SynAttr.h"


class CMsgSynAttrInfo : public CNetMsg
{
public:
    CMsgSynAttrInfo();
    virtual ~CMsgSynAttrInfo();

    BOOL	Create		(SynAttrInfoStruct* pInfo, CSyndicate* pSyn);
    BOOL	Create		(char* pMsgBuf, DWORD dwSize);
    void	Process		(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID		idSyn;						// 职位
        int 		nProffer;					// 贡献度
        DWORD		dwSyndicateFund;			// 帮派基金
        DWORD		dwSyndicatePopulation;		// 帮派人口
        DWORD		dwSyndicateRepute;			// 帮派声望
        USHORT		usMantle;					// 团标
        UCHAR		ucSynRank;					// 帮派级别
        UCHAR		ucRankShow;					// RankShow
        UCHAR		ucLevel;					// 成员级别
        char		szLeader[_MAX_NAMESIZE];	// 帮主
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGSYNATTRINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
