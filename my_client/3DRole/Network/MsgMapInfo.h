
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgMapInfo.h: interface for the CMsgMapInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGMAPINFO_H__29E692EB_4845_46DA_8E50_60561AF26347__INCLUDED_)
#define AFX_MSGMAPINFO_H__29E692EB_4845_46DA_8E50_60561AF26347__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgMapInfo : public CNetMsg
{
public:
    CMsgMapInfo();
    virtual ~CMsgMapInfo();

public:
    BOOL	Create		(char* pMsgBuf, DWORD dwSize);
    void	Process		(void* pInfo);

protected:
    typedef struct
    {
        USHORT	unMsgSize;
        USHORT	unMsgType;

        OBJID	idMap;
        OBJID	idDoc;
        DWORD	dwType;

    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGMAPINFO_H__29E692EB_4845_46DA_8E50_60561AF26347__INCLUDED_)
