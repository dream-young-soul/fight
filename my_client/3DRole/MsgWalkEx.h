
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgWalkEx.h: interface for the CMsgWalkEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGWALKEX_H__72591628_31A5_40C1_9872_A9D57D0A0768__INCLUDED_)
#define AFX_MSGWALKEX_H__72591628_31A5_40C1_9872_A9D57D0A0768__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgWalkEx  : public CNetMsg
{
public:
    CMsgWalkEx();
    virtual ~CMsgWalkEx();
public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    BOOL			Create		(OBJID idPlayer, int usPosX, int usPosY, int nDir, int uMode = 0);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        USHORT		unMsgSize;
        USHORT		unMsgType;

        OBJID		idUser;
        USHORT		usPosX;
        USHORT		usPosY;
        UCHAR		ucDir;
        UCHAR		ucMode;
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGWALKEX_H__72591628_31A5_40C1_9872_A9D57D0A0768__INCLUDED_)
