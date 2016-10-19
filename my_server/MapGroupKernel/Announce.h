
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Announce.h: interface for the CAnnounce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANNOUNCE_H__9444E2C3_297A_482E_B31C_43A7B288E0C2__INCLUDED_)
#define AFX_ANNOUNCE_H__9444E2C3_297A_482E_B31C_43A7B288E0C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)
#include "define.h"
#include <deque>
#include "AnnounceData.h"
class IDatabase;
class IRole;
class CAnnounce
{
public:
    CAnnounce()  ;
    virtual ~CAnnounce();
public:
    void	Release()						{ delete this; }
    static CAnnounce*	CreateNew()		{ return new CAnnounce; }
    virtual bool Create(IDatabase* pDb);
    virtual bool Create(IDatabase* pDb, OBJID id);

    bool	CreateNewAnnounce(const ST_ANNOUNCE_DATA* pInfo, IDatabase* pDb);
    virtual bool	DeleteAnnounceByObjID(OBJID idAnnounce);
    virtual bool    DeleteAnnounceByUserID(OBJID id, bool isDelete);
    int		GetAnnounceAmount()					{ return m_setData.size(); }
    CAnnounceData*	QueryAnnounceDataByIndex(int nIdx);
    CAnnounceData*	QueryAnnounceDataByID(OBJID idAnnounce);
    CAnnounceData*	QueryAnnounceDataByOwner(OBJID idUser);

public:
    bool	SendAnnounceList(IRole* pRole, UCHAR usType, int nIndexBegin);
    bool	SendAnnounceInfo(IRole* pRole, OBJID idAnnounce);
    bool    SendAnnounceSelf(IRole* pRole/*, OBJID idAnnounce*/);
protected:
    typedef std::deque<CAnnounceData*>	ANNOUNCEDATA_SET;
    ANNOUNCEDATA_SET	m_setData;
    //	PROCESS_ID	m_idProcess;
};

#endif // !defined(AFX_ANNOUNCE_H__9444E2C3_297A_482E_B31C_43A7B288E0C2__INCLUDED_)



