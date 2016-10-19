
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TutorData.h: interface for the CTutorData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TUTORDATA_H__B21C4B4C_0011_45BA_AC0E_5F6F501A7EE7__INCLUDED_)
#define AFX_TUTORDATA_H__B21C4B4C_0011_45BA_AC0E_5F6F501A7EE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameObj.h"
#include "I_mydb.h"

struct TutorInfoStruct
{
    OBJID	id;
    OBJID	idUser;
    OBJID	idTutor;
    NAMESTR	szUserName;
    NAMESTR	szTutorName;
};


class CTutorData : public CGameObj
{
public:
    CTutorData();
    virtual ~CTutorData();

public:
    bool	Create			(IRecordset* pRes);
    bool	Create			(OBJID id, IDatabase* pDb);
    TutorInfoStruct*	GetInfo	(void)		{return &m_Info;}

    bool	LoadInfo		(IRecord* pRes);

public:	// get
    OBJID	GetID()				{ return m_Info.id; }
    OBJID	GetUserID()			{ return m_Info.idUser; }
    OBJID	GetTutorID()		{ return m_Info.idTutor; }
    LPCTSTR	GetUserName()		{ return m_Info.szUserName; }
    LPCTSTR	GetTutorName()		{ return m_Info.szTutorName; }

public:	// add del
    OBJID	CreateRecord(OBJID idUser,  OBJID idTutor, LPCTSTR szUserName, LPCTSTR szTutorName, IDatabase* pDb);
    bool	DeleteRecord(IDatabase* pDb);

protected:
    TutorInfoStruct		m_Info;
};

#endif // !defined(AFX_TUTORDATA_H__B21C4B4C_0011_45BA_AC0E_5F6F501A7EE7__INCLUDED_)
