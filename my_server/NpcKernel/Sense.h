
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Sense.h: interface for the CSense class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SENSE_H__AAC1DEEB_7413_42DE_9BA7_4DDFC3A245C0__INCLUDED_)
#define AFX_SENSE_H__AAC1DEEB_7413_42DE_9BA7_4DDFC3A245C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mycom.h"
#include "VarType.h"
#include "I_AiCenter.h"
#include "TimeOut.h"

class CAgent;
class CSense
{
    COM_STYLE(CSense)
protected:
    CSense();
    virtual ~CSense();

public: // construction
    bool	Create(CAgent* pOwner);

public: // interface
    bool	CheckCondition(int idxFactFunction, VarTypeSetRef setParam, ARGUMENT_SET* psetArgument);

public: // const

public: // application
    void OnTimer();
    void TeamApply(OBJID idUser);
    void Reborn();
    void Recruit(int nLife);
    void BeKill(IRole* pRole);
    void BeAttack(IRole* pRole);
    void Die();

protected:
    CAgent*		m_pOwner;
    CTimeOut	m_tLook;
};

#endif // !defined(AFX_SENSE_H__AAC1DEEB_7413_42DE_9BA7_4DDFC3A245C0__INCLUDED_)
