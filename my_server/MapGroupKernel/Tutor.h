
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Tutor.h: interface for the CTutor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TUTOR_H__B3DCB235_513A_4B49_A461_01438966E2D7__INCLUDED_)
#define AFX_TUTOR_H__B3DCB235_513A_4B49_A461_01438966E2D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TutorData.h"
#include "Myheap.h"


class CTutor : public CTutorData
{
public:
    CTutor();
    virtual ~CTutor();

    ULONG	ReleaseByOwner()		{ delete this; return 0; }

public:
    static CTutor*	CreateNewTutor(const TutorInfoStruct* pInfo);
    static CTutor*	CreateNew()		{ return new CTutor; }

protected:
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_TUTOR_H__B3DCB235_513A_4B49_A461_01438966E2D7__INCLUDED_)
