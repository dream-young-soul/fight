
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAllot.h: interface for the CMsgAllot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGALLOT_H__EC294340_42D1_4A51_B115_38641893A817__INCLUDED_)
#define AFX_MSGALLOT_H__EC294340_42D1_4A51_B115_38641893A817__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgAllot : public CNetMsg
{
public:
    CMsgAllot();
    virtual ~CMsgAllot();

    BOOL			Create		(	unsigned char ucForce, unsigned char ucSpeed,
                                    unsigned char ucHealth, unsigned char ucSoul);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

    struct MSG_Info
    {
        MSGHEAD_DEFINE

        unsigned char	ucForce;
        unsigned char	ucDexterity;
        unsigned char	ucHealth;
        unsigned char	ucSoul;
    };

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGALLOT_H__EC294340_42D1_4A51_B115_38641893A817__INCLUDED_)
