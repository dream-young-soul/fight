
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// PoliceWanted.h: interface for the CPoliceWanted class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLICEWANTED_H__3E6401B7_C9EA_48D6_8781_EC563A1DF9C8__INCLUDED_)
#define AFX_POLICEWANTED_H__3E6401B7_C9EA_48D6_8781_EC563A1DF9C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <deque>
#include <string>
using namespace std;

typedef struct
{
    OBJID		idUser;
    string		strName;
    string		strSynName;
    int			nPk;
    int			nLev;
} PoliceWantedStruct;

const int PKVALUE_BADGUY	= 1000;

class CCriticalSection;
class CUser;
class CPoliceWanted
{
public:
    CPoliceWanted();
    virtual ~CPoliceWanted();

    // interface
    bool	AddWanted	(CUser* pUser);
    bool	DelWanted	(OBJID idUser);

    PoliceWantedStruct*	GetWantedByIndex(int idx);
    PoliceWantedStruct*	GetWanted	(OBJID idUser);
    int		GetWantedAmount	(void)	{ return m_setWanted.size(); }

    static CPoliceWanted		s_objPoliceWanted;

protected:
    deque<PoliceWantedStruct> m_setWanted;

    // static
    static CCriticalSection		s_xCtrl;
};

extern CPoliceWanted& PoliceWanted(void);

#endif // !defined(AFX_POLICEWANTED_H__3E6401B7_C9EA_48D6_8781_EC563A1DF9C8__INCLUDED_)
