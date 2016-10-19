
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Account.h: interface for the CAccount class.
// ÐÞ¸Ä£ºÏÉ½£ÐÞ£¬2001.12.12
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCOUNT_H__629DF181_66F0_11D5_8BDA_5254AB3634F5__INCLUDED_)
#define AFX_ACCOUNT_H__629DF181_66F0_11D5_8BDA_5254AB3634F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "mydb.h"

// const
const int _MAX_PHONESIZE	=	16;
const int _MAX_EMAILSIZE	=	128;
const int _MAX_ADDRESSSIZE	=	128;
const int _MAX_IDNUMBERSIZE	=	32;

typedef struct
{
    OBJID	id;
    char	szName[_MAX_NAMESIZE];
    int 	nSex;
    int		nAge;
    char 	szPhone[_MAX_PHONESIZE];
    char	szEmail[_MAX_EMAILSIZE];
    char	szAddress[_MAX_ADDRESSSIZE];
    char 	szIdNumber[_MAX_IDNUMBERSIZE];
    char	szPassword[_MAX_NAMESIZE];
    int		nType;
    int		nPoint;
    int		nPointTime;
    int		nOnline;
    int		nLicence;
    char	dwNetBarIP[_MAX_NAMESIZE];
    char	dwIPMask[_MAX_NAMESIZE];
} AccountInfoStruct, *pAccountInfoStruct;

void InsertBackslash(char* bufTarget, const char* pszSource);

class CAccount
{
public:
    CAccount();
    virtual ~CAccount();

    //---------- static -----------
    static CAccount* CreateNew	(const char* pszName, const char* pszPassword);
//	static OBJID CreateRecord	(AccountInfoStruct * pAccountInfo);
//	static BOOL	DeleteRecord	(OBJID id);

    //---------- common -----------
    BOOL	Create	(const char* pszName, const char* pszPassword);
    BOOL	Create	(OBJID	idAccount);
    BOOL	Create	(int nType, const char* pszIPAddr);
    void	Destroy	(void);

    BOOL	CheckPassword	(const char* pszPsw);
    AccountInfoStruct*	GetInfo	(void)	{return &m_Info;}

    //---------- info  -----------
    OBJID	GetID()	{return m_Info.id;}

//	BOOL	SaveInfo(void);
    BOOL	LoadInfo(void);

    char*	GetName	(BOOL bUpdate = false);
    void	SetName (const char* pszName, BOOL bUpdate = false);

    int		GetSex		(BOOL bUpdate = false);
    void	SetSex		(const int nSex, BOOL bUpdate = false);

    int		GetAge(BOOL bUpdate = false);
    void	SetAge(int nLife, BOOL bUpdate = false);

    char*	GetPhone(BOOL bUpdate = false);
    void	SetPhone(const char* pszSex, BOOL bUpdate = false);

    char*	GetEmail(BOOL bUpdate = false);
    void	SetEmail(const char* pszSex, BOOL bUpdate = false);

    char*	GetAddress(BOOL bUpdate = false);
    void	SetAddress(const char* pszSex, BOOL bUpdate = false);

    char*	GetIdNumber	(BOOL bUpdate = false);
    void	SetIdNumber(const char* pszSex, BOOL bUpdate = false);

//	char*	GetPassword(BOOL bUpdate=false);
//	void	SetPassword(const char* pszPsw, BOOL bUpdate=false);

    int		GetType(BOOL bUpdate = false);
//	void	SetType(int nType, BOOL bUpdate=false);

    int		GetPoint(BOOL bUpdate = false);
//	void	SetPoint(int Point, BOOL bUpdate=false);

    int		GetPointTime(BOOL bUpdate = false);
//	void	SetPointTime(int nPointTime, BOOL bUpdate=false);

    int		GetOnline(BOOL bUpdate = false);

    int		GetLicence(BOOL bUpdate = false);
    LPCTSTR		GetNetBarIP(BOOL bUpdate = false);
    LPCTSTR		GetIPMask(BOOL bUpdate = false);

    bool	DecPoint(OBJID idAccount);
protected:
    AccountInfoStruct	m_Info;
    CMyRecordset*		m_pRes;
    char				m_szSQL[256];
};

#endif // !defined(AFX_ACCOUNT_H__629DF181_66F0_11D5_8BDA_5254AB3634F5__INCLUDED_)
