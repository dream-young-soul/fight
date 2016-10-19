
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgItemAttrib.h: interface for the CMsgItemAttrib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGITEMATTRIB_H__256183C8_E4BC_4A5D_8A8A_F9EC4EB9117A__INCLUDED_)
#define AFX_MSGITEMATTRIB_H__256183C8_E4BC_4A5D_8A8A_F9EC4EB9117A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum
{
    _ITEMATTRIB_AMOUNT,			_ITEMATTRIB_FIDELITY		= _ITEMATTRIB_AMOUNT,
    _ITEMATTRIB_AMOUNT_LIMIT,	_ITEMATTRIB_EUDEMON_LEVEL	= _ITEMATTRIB_AMOUNT_LIMIT,
    _ITEMATTRIB_WARGHOSTEXP,	_ITEMATTRIB_EUDEMON_LIFE	= _ITEMATTRIB_WARGHOSTEXP,
    _ITEMATTRIB_GEMTYPE,		_ITEMATTRIB_EUDEMON_EXP		= _ITEMATTRIB_GEMTYPE,

    _ITEMATTRIB_NONE = -1,
};

typedef struct
{
    DWORD	ucAttributeType;
    DWORD	dwAttributeData;
} ST_ITEMATTRIB;


class CMsgItemAttrib : public CNetMsg
{
public:
    CMsgItemAttrib();
    virtual ~CMsgItemAttrib();

    BOOL	Create(OBJID idItem, int nType, DWORD dwData);
    BOOL	Append(int nType, DWORD dwData);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(CGameSocket* pSocket);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID			idItem;
        DWORD			dwAttributeNum;
        ST_ITEMATTRIB	setItemAttrib[1];			// ±ä³¤
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGITEMATTRIB_H__256183C8_E4BC_4A5D_8A8A_F9EC4EB9117A__INCLUDED_)
