
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// StrRes.h: interface for the CStrRes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRRES_H__B92BDD57_47DD_4856_B21B_58FCFC32B256__INCLUDED_)
#define AFX_STRRES_H__B92BDD57_47DD_4856_B21B_58FCFC32B256__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"
#include <map>
using namespace std;
typedef map<OBJID, char*>MAP_STRRES;
const char g_szStrResError[] = "ERROR";
class CStrRes
{
public:
    CStrRes();
    virtual ~CStrRes();

public:
    const	char*	GetStr(OBJID idRes);

private:
    MAP_STRRES		m_setStr;

public:
    void	Init();
    void	Destroy();
};

#endif // !defined(AFX_STRRES_H__B92BDD57_47DD_4856_B21B_58FCFC32B256__INCLUDED_)
