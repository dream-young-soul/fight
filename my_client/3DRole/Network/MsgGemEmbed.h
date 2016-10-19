
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgGemEmbed.h: interface for the CMsgGemEmbed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGGEMEMBED_H__230BC8E1_26D1_486F_B289_BB989ACFA59D__INCLUDED_)
#define AFX_MSGGEMEMBED_H__230BC8E1_26D1_486F_B289_BB989ACFA59D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum { GEM_EMBED = 0, GEM_TAKEOUT };

class CMsgGemEmbed : public CNetMsg
{
public:
    CMsgGemEmbed();
    virtual ~CMsgGemEmbed();

    BOOL Create(OBJID idUser, OBJID idItem, OBJID idGem, int nPos, int nAction = GEM_EMBED);

public:
    BOOL	Create		(char* pMsgBuf, DWORD dwSize);
    void	Process		(void* pInfo);

protected:
    typedef struct
    {
        USHORT	unMsgSize;
        USHORT	unMsgType;

        OBJID	idUser;
        OBJID	idItem;
        OBJID	idGem;
        USHORT	usPos;
        USHORT	usAction;
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGGEMEMBED_H__230BC8E1_26D1_486F_B289_BB989ACFA59D__INCLUDED_)
