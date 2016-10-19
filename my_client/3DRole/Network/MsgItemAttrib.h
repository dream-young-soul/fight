
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgItemAttrib.h: interface for the CMsgItemAttrib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGITEMATTRIB_H__A0E1757F_D22F_420C_851A_A724EDC5819B__INCLUDED_)
#define AFX_MSGITEMATTRIB_H__A0E1757F_D22F_420C_851A_A724EDC5819B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"


enum
{
    _ITEMATTRIB_AMOUNT,  _ITEMATTRIB_EUDEMON_FIDELITY = _ITEMATTRIB_AMOUNT,
    _ITEMATTRIB_AMOUNT_LIMIT, _ITEMATTRIB_EUDEMON_LEVEL = _ITEMATTRIB_AMOUNT_LIMIT,
    _ITEMATTRIB_WARGHOSTEXP, _ITEMATTRIB_EUDEMON_LIFE = _ITEMATTRIB_WARGHOSTEXP,
    _ITEMATTRIB_GEMTYPE,	_ITEMATTRIB_EUDEMON_EXP	 = _ITEMATTRIB_GEMTYPE,
    _ITEMATTRIB_NONE = -1,
};

typedef struct
{
    DWORD ucAttributeType;
    DWORD dwAttributeData;
} ST_ITEMATTRIB;

class CMsgItemAttrib : public CNetMsg
{
public:
    CMsgItemAttrib();
    virtual ~CMsgItemAttrib();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:

    typedef struct
    {
        unsigned short unMsgSize;
        unsigned short unMsgType;

        OBJID idItem;
        DWORD dwAttributeNum;
        ST_ITEMATTRIB setItemAttrib[1]; // ±ä³¤
    } MSG_Info;

    MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGITEMATTRIB_H__A0E1757F_D22F_420C_851A_A724EDC5819B__INCLUDED_)
