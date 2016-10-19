
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// FriendData.h: interface for the CFriendData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRIENDDATA_H__B256E96D_8D10_4E17_8572_0726F0249539__INCLUDED_)
#define AFX_FRIENDDATA_H__B256E96D_8D10_4E17_8572_0726F0249539__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameObj.h"
#include "I_mydb.h"

struct FriendInfoStruct
{
    OBJID		id;
    OBJID		idUser;
    OBJID		idFriend;
    NAMESTR		szFriendName;
};

class CFriendData : public CGameObj
{
protected:
    CFriendData();
    virtual ~CFriendData();

public:
    bool	Create			(IRecordset* pRes);
    bool	Create			(OBJID id, IDatabase* pDb);
    FriendInfoStruct*	GetInfo	(void)		{return &m_Info;}

    bool	LoadInfo		(IRecord* pRes);

public: // get
    OBJID		GetID()						{ return m_Info.id; }
    OBJID		GetUserID()					{ return m_Info.idUser; }
    OBJID		GetFriendID()				{ return m_Info.idFriend; }
    LPCTSTR		GetFriendName()				{ return m_Info.szFriendName; }

public: // add del
    OBJID		CreateRecord(OBJID idUser, OBJID idFriend, LPCTSTR szFriendName, IDatabase* pDb);
    bool		DeleteRecord(IDatabase* pDb);

protected:
    FriendInfoStruct	m_Info;

public:	// ±íÄ£°å
    static bool			Init(IDatabase* pDb);
    static void			Final();
protected:
    //	static IRecord*		m_pDefaultRecord;
};


#endif // !defined(AFX_FRIENDDATA_H__B256E96D_8D10_4E17_8572_0726F0249539__INCLUDED_)
