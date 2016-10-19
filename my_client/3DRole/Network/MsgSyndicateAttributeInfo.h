
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgSyndicateAttributeInfo.h: interface for the CMsgSyndicateAttributeInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSYNDICATEATTRIBUTEINFO_H__BEACEA82_5569_4645_BC2D_A5BB94E67C57__INCLUDED_)
#define AFX_MSGSYNDICATEATTRIBUTEINFO_H__BEACEA82_5569_4645_BC2D_A5BB94E67C57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"


class CMsgSyndicateAttributeInfo  : public CNetMsg
{
public:
    CMsgSyndicateAttributeInfo();
    virtual ~CMsgSyndicateAttributeInfo();
public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);
protected:
    typedef struct
    {
        USHORT	unMsgSize;
        USHORT	unMsgType;
        OBJID	idSyndicate;				// 帮派id
        int		nProffer;					// 贡献度
        DWORD	dwSyndicateFund;			// 帮派基金
        DWORD	dwSyndicatePopulation;		// 帮派人口
        DWORD   dwSyndicateFamer;			// 帮派声望
        USHORT	usMantle;					// 团标
        UCHAR	ucSynRank;					//帮派级别
        UCHAR	ucRank;						// 帮派职称
        UCHAR	ucLevel;					// 帮派等级
        char	szLeader[_MAX_NAMESIZE];	// 帮主
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGSYNDICATEATTRIBUTEINFO_H__BEACEA82_5569_4645_BC2D_A5BB94E67C57__INCLUDED_)
