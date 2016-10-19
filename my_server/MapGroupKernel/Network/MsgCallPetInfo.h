
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgCallPetInfo.h: interface for the CMsgCallPetInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGCALLPETINFO_H__50F4AAD1_342A_4D6C_A161_33CDE0158D5B__INCLUDED_)
#define AFX_MSGCALLPETINFO_H__50F4AAD1_342A_4D6C_A161_33CDE0158D5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "NetMsg.h"
#include "Npc.h"

class CNpc;
class CMsgCallPetInfo  : public CNetMsg
{
public:
    CMsgCallPetInfo();
    virtual ~CMsgCallPetInfo();

    BOOL	Create(OBJID id, DWORD dwLookFace, int nAIType, int usPosX, int usPosY, OBJID idItem = ID_NONE);
public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(CGameSocket* pSocket);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID			id;
        DWORD			dwLookFace;
        int				nAIType;
        unsigned short	usPosX;
        unsigned short	usPosY;
        OBJID			idItem;

    } MSG_Info;
    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGCALLPETINFO_H__50F4AAD1_342A_4D6C_A161_33CDE0158D5B__INCLUDED_)
