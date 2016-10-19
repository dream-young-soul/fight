
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgWeaponSkill.h: interface for the CMsgWeaponSkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGWEAPONSKILL_H__E72BC0A6_A34C_43BD_B06E_22AED0870AFF__INCLUDED_)
#define AFX_MSGWEAPONSKILL_H__E72BC0A6_A34C_43BD_B06E_22AED0870AFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgWeaponSkill  :	public CNetMsg
{
public:
    CMsgWeaponSkill();
    virtual ~CMsgWeaponSkill();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        USHORT		unMsgSize;
        USHORT		unMsgType;

        unsigned int	unType;
        unsigned short	usLevel;
        unsigned int	unExp;
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGWEAPONSKILL_H__E72BC0A6_A34C_43BD_B06E_22AED0870AFF__INCLUDED_)
