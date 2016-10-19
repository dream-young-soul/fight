
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgSyndicateInfo.h: interface for the CMsgSyndicateInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSYNDICATEINFO_H__B5C77080_A52F_4E64_91B5_9F5ADCF073B2__INCLUDED_)
#define AFX_MSGSYNDICATEINFO_H__B5C77080_A52F_4E64_91B5_9F5ADCF073B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NetMsg.h"

class CMsgSyndicateInfo : public CNetMsg
{
public:
    CMsgSyndicateInfo();
    virtual ~CMsgSyndicateInfo();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    BOOL			Create		(OBJID idSyn);
    void			Process		(void* pInfo);
protected:
    typedef struct
    {
        USHORT	unMsgSize;
        USHORT	unMsgType;

        OBJID		idSyn;						//
        OBJID		idFealty;
        DWORD		dwSyndicateFund;			// 帮派基金
        DWORD		dwSyndicatePopulation;		// 帮派人口
        UCHAR		ucRank;						// 等级
        UCHAR		ucSaint;
        char		szName[_MAX_NAMESIZE];		//
        char		szLeader[_MAX_NAMESIZE];	// 帮主
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGSYNDICATEINFO_H__B5C77080_A52F_4E64_91B5_9F5ADCF073B2__INCLUDED_)
