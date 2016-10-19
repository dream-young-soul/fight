
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Body.h: interface for the CBody class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BODY_H__BAE033E5_0C32_4D5C_9AD3_DF4607475280__INCLUDED_)
#define AFX_BODY_H__BAE033E5_0C32_4D5C_9AD3_DF4607475280__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mycom.h"
#include "VarType.h"

class CAgent;
class CBody
{
    COM_STYLE(CBody)
protected:
    CBody();
    virtual ~CBody();

public:
    bool ProcessAction(int idxAction, VarTypeSetRef setParam);
    bool Create(CAgent* pOwner);
    void OnTimer();

protected:
    CAgent*		m_pOwner;
};

#endif // !defined(AFX_BODY_H__BAE033E5_0C32_4D5C_9AD3_DF4607475280__INCLUDED_)
