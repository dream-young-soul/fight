
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgSynAttrInfo.h: interface for the CMsgSynInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSYNINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
#define AFX_MSGSYNINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "Syndicate.h"


class CMsgSynInfo : public CNetMsg
{
public:
    CMsgSynInfo();
    virtual ~CMsgSynInfo();

    BOOL	Create		(CSyndicate* pSyn);
    BOOL	Create		(char* pMsgBuf, DWORD dwSize);
    void	Process		(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

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

#endif // !defined(AFX_MSGSYNINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
