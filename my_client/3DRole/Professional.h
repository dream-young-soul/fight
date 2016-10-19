
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Professional.h: interface for the CProfessional class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFESSIONAL_H__AA8D79D6_9AB1_498B_A2FC_ADAE266BBB44__INCLUDED_)
#define AFX_PROFESSIONAL_H__AA8D79D6_9AB1_498B_A2FC_ADAE266BBB44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"

typedef struct
{
    char	szProfessionalName[_MAX_NAMESIZE];
    DWORD	dwIndex;
} ProfessionalInfo;

#include <deque>
using namespace std;
typedef deque<ProfessionalInfo*>DEQUE_PROFESSIONAL;

class CProfessional
{
public:
    CProfessional();
    virtual ~CProfessional();

private:
    DEQUE_PROFESSIONAL m_setProfessional;
    char m_szError[_MAX_NAMESIZE];
private:
    void CreateProfessionList();
    void DestroyProfessionList();

public:
    char* GetProfessionalName(DWORD dwIndex);

};

#endif // !defined(AFX_PROFESSIONAL_H__AA8D79D6_9AB1_498B_A2FC_ADAE266BBB44__INCLUDED_)
