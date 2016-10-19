
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgWeaponSkill.h: interface for the CMsgWeaponSkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGWEAPONSKILL_H__200717E7_2D49_4493_ADAA_9F5F096E7C10__INCLUDED_)
#define AFX_MSGWEAPONSKILL_H__200717E7_2D49_4493_ADAA_9F5F096E7C10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgWeaponSkill : public CNetMsg
{
public:
    CMsgWeaponSkill();
    virtual ~CMsgWeaponSkill();

public:
    BOOL			Create		(int nType, int nLevel, int nExp);

    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        unsigned int	unType;
        unsigned short	usLevel;
        unsigned int	unExp;
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGWEAPONSKILL_H__200717E7_2D49_4493_ADAA_9F5F096E7C10__INCLUDED_)
