
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Friend.h: interface for the CFriend class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRIEND_H__175B4F68_F766_4995_9E8A_414301B232E2__INCLUDED_)
#define AFX_FRIEND_H__175B4F68_F766_4995_9E8A_414301B232E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BaseFunc.h"

class CFriend
{
public:
    CFriend();
    virtual ~CFriend();

private:
    OBJID	m_idFriend;
    char	m_szName[_MAX_NAMESIZE];
    BOOL	m_bFlagOnline;
    BOOL	m_bFlagNewMsg;

    DWORD	m_dwFace;
    DWORD	m_dwLevel;
    DWORD	m_dwProfessional;
    int		m_nPK;
    DWORD	m_IDSyndicate;
    DWORD	m_dwRank;
    char	m_szMate[_MAX_NAMESIZE];
    DWORD	m_dwLook;
    DWORD	m_dwRace;
    UCHAR   m_ucNobilityRank;
    // the operate interface(s)
public:
    static CFriend* CreateNew(OBJID id, char* pszName, DWORD dwLookFace, BOOL m_bFlagOnline);
    // get...
    OBJID	GetID() {return m_idFriend;}
    char*	GetName() {return m_szName;}
    BOOL	IsOnline() {return m_bFlagOnline;}
    BOOL	HaveNewMsg() {return m_bFlagNewMsg;}

    DWORD	GetProfessional();
    DWORD	GetRace();
    DWORD	GetLook();
    DWORD	GetFace() {return m_dwFace;}
    DWORD	GetLevel() {return m_dwLevel;}
    int		GetPK() {return m_nPK;}
    OBJID	GetSynDicateID() {return m_IDSyndicate;}
    DWORD	GetRank() {return m_dwRank;}
    char*	GetMate() {return m_szMate;}
    UCHAR   GetNobilityRank() {return m_ucNobilityRank;}
    // additional get ...
    char*	GetProfessionalName();
    char*	GetSyndicateName();
    char*	GetRankName();

    // set...
    void	SetLook(int nLook) {m_dwLook = nLook;}
    void	SetRace(int nRace) {m_dwRace = nRace;}

    void	SetID(OBJID id) {m_idFriend = id;}
    void	SetName(char* pszName) {assert(pszName); strcpy(m_szName, pszName);}
    void	Online() {m_bFlagOnline = true;}
    void	Offline() {m_bFlagOnline = false;}
    void	SetNewMsg(BOOL bMsg) {m_bFlagNewMsg = bMsg;}
    void	SetFace(DWORD dwFace) {m_dwFace = dwFace;}
    void	SetLevel(DWORD dwLevel) {m_dwLevel = dwLevel;}
    void	SetProfessional(DWORD dwProfessional) {m_dwProfessional = dwProfessional;}
    void	SetPK(int nPK) {m_nPK = nPK;}
    void	SetSyndicateID(OBJID idSyndicate) {m_IDSyndicate = idSyndicate;}
    void	SetRank(DWORD dwRank) {m_dwRank = dwRank;}
    void	SetMate(char* pszMate) {strcpy(m_szMate, pszMate);}
    void	SetNobilityRank(UCHAR ucNobilityRank) {m_ucNobilityRank = ucNobilityRank;}
};
#include <deque>
using namespace std;
class CPlayer;
typedef deque<CFriend* >  DEQUE_FRIEND;
#endif // !defined(AFX_FRIEND_H__175B4F68_F766_4995_9E8A_414301B232E2__INCLUDED_)
