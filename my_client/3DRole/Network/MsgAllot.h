
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAllot.h: interface for the CMsgAllot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGALLOT_H__FE7492C3_B6C2_4B8A_A441_CEA75DFBC4F1__INCLUDED_)
#define AFX_MSGALLOT_H__FE7492C3_B6C2_4B8A_A441_CEA75DFBC4F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgAllot : public CNetMsg
{
public:
    CMsgAllot();
    virtual ~CMsgAllot();
    BOOL Create(unsigned char ucForce, unsigned char ucSpeed, unsigned char ucHealth, unsigned char ucSoul);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);
    typedef struct
    {
        USHORT			unMsgSize;
        USHORT			unMsgType;
        unsigned char	ucForce;
        unsigned char	ucSpeed;
        unsigned char	ucHealth;
        unsigned char	ucSoul;
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGALLOT_H__FE7492C3_B6C2_4B8A_A441_CEA75DFBC4F1__INCLUDED_)
