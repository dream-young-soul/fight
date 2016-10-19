
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgNpcInfo.h: interface for the CMsgNpcInfoEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGNPCINFO_H__50F4AAD1_342A_4D6C_A161_33CDE0158D5B__INCLUDED_EX)
#define AFX_MSGNPCINFO_H__50F4AAD1_342A_4D6C_A161_33CDE0158D5B__INCLUDED_EX

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "NetMsg.h"
#include "Npc.h"
#include "NetStringPacker.h"


class CNpc;
class CMsgNpcInfoEx  : public CNetMsg
{
public:
    CMsgNpcInfoEx();
    virtual ~CMsgNpcInfoEx();

    BOOL	Create	(CNpc* pNpc);
    BOOL	Create(OBJID id, DWORD dwMaxLife, DWORD dwLife, int nType, int nSort,
                   int nLookType, int nCellX, int nCellY, int nLength, int nFat, const char* pszName = NULL);
public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(CGameSocket* pSocket);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID		id;						// lookface for statuary
        DWORD		dwMaxLife;				// the max life of npc
        DWORD		dwLife;					// the current life of npc
        unsigned	short usCellX;
        unsigned	short usCellY;
        USHORT		usLook;					// dir for statuary
        USHORT		usType;					// frame for statuary
        USHORT		usSort;					// pose for statuary
        char		cLength;				// ¸ß°«
        char		cFat;					// ÅÖÊÝ
        char		szBuf[1];
    } MSG_Info;

    MSG_Info*	m_pInfo;

private:
    CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGNPCINFO_H__50F4AAD1_342A_4D6C_A161_33CDE0158D5B__INCLUDED_EX)
