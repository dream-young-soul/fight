
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgNpcInfoEX.h: interface for the CMsgNpcInfoEX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGNPCINFOEX_H__8CE4D354_B8B9_44BC_B2D2_D2776C160CEF__INCLUDED_)
#define AFX_MSGNPCINFOEX_H__8CE4D354_B8B9_44BC_B2D2_D2776C160CEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"

class CMsgNpcInfoEX  : public CNetMsg
{
public:
    CMsgNpcInfoEX();
    virtual ~CMsgNpcInfoEX();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        USHORT		unMsgSize;
        USHORT		unMsgType;
        OBJID		id;						// lookface for statuary
        DWORD		dwMaxLife;				// the max life of npc
        DWORD		dwLife;					// the life of npc
        unsigned	short usCellX;
        unsigned	short usCellY;
        USHORT		usLook;					// dir for statuary
        USHORT		usType;					// frame for statuary
        USHORT		usSort;					// pose for statuary
        char		cLength;				// ¸ß°«
        char		cFat;					// ÅÖÊÝ
        char		szName[1];				// name
    } MSG_Info;
    MSG_Info*	m_pInfo;
private:
    CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGNPCINFOEX_H__8CE4D354_B8B9_44BC_B2D2_D2776C160CEF__INCLUDED_)
