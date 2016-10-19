
//**********************************************************
// 代码编辑器
//**********************************************************

// Account.cpp: implementation of the CAccount class.
// 修改：仙剑修，2001.12.12
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <stdio.h>
#include <assert.h>
//#include <windows.h>

#include "define.h"
//#include "basefunc.h"
#include "Account.h"
#include "Logfile.h"	//paled
#include "typedef.h"

//const
//? const char	SQL_ACCOUNT_STMT[]			="SELECT * FROM account WHERE name = '%s'";
//? const char	SQL_ACCOUNTID_STMT[]			="SELECT * FROM account WHERE id = %u";	//paled
//#ifdef	DATABASE_PASSWORD
//	const char	SQL_ACCOUNT_STMT[]			="SELECT id,name,type,point,pointtime,online,licence,netbar_ip,ip_mask FROM account WHERE name='%s' AND password=PASSWORD('%s')";
//#else
//	const char	SQL_ACCOUNT_STMT[]			="SELECT id,name,type,point,pointtime,online,licence,netbar_ip,ip_mask FROM account WHERE name='%s' AND password='%s'";
//	const char	SQL_ACCOUNT_STMT2[]			="SELECT id,name,type,point,pointtime,online,licence,netbar_ip,ip_mask FROM account WHERE name='%s'";
//#endif
//const char	SQL_ACCOUNTID_STMT[]			="SELECT id,name,type,point,pointtime,online,licence,netbar_ip,ip_mask FROM account WHERE id = %u";	//paled
//const char	SQL_ACCOUNTISP_STMT[]			="SELECT id,name,type,point,pointtime,online,licence,netbar_ip,ip_mask FROM account WHERE type=%d AND netbar_ip='%s'";	//paled
////const char	SQL_ACCOUNT_INSERT_STMT[]	="INSERT INTO account VALUES(0, '%s', %d, %d, '%s', '%s', '%s', '%s', '%s')";
////const char	SQL_ACCOUNT_DELETE_STMT[]	="DELETE FROM account WHERE id = %u";
//const char	SQL_ACCOUNT_UPDATEPOINT_STMT[]	="UPDATE account SET point=point-1 WHERE id=%u AND point>0 LIMIT 1";	//paled
#ifdef	DATABASE_PASSWORD
const char	SQL_ACCOUNT_STMT[]			= "SELECT id,name,type,point,pointtime,online,licence,netbar_ip,ip_mask FROM account as t1 left join %s as t2 on t2.account_id = t1.id WHERE name = '%s' AND password = PASSWORD('%s')";
#else
const char	SQL_ACCOUNT_STMT[]			= "SELECT id,name,type,point,pointtime,online,licence,netbar_ip,ip_mask FROM account as t1 left join %s as t2 on t2.account_id = t1.id WHERE name = '%s' AND password = '%s'";
const char	SQL_ACCOUNT_STMT2[]			= "SELECT id,name,type,point,pointtime,online,licence,netbar_ip,ip_mask FROM account as t1 left join %s as t2 on t2.account_id = t1.id WHERE name = '%s'";
#endif
const char	SQL_ACCOUNTID_STMT[]			= "SELECT id,name,type,point,pointtime,online,licence,netbar_ip,ip_mask FROM account as t1 left join %s as t2 on t2.account_id = t1.id WHERE id = %u";	//paled
const char	SQL_ACCOUNTISP_STMT[]			= "SELECT id,name,type,point,pointtime,online,licence,netbar_ip,ip_mask FROM account as t1 left join %s as t2 on t2.account_id = t1.id WHERE type=%d AND netbar_ip='%s'";	//paled
//const char	SQL_ACCOUNT_INSERT_STMT[]	="INSERT INTO account VALUES(0, '%s', %d, %d, '%s', '%s', '%s', '%s', '%s')";
//const char	SQL_ACCOUNT_DELETE_STMT[]	="DELETE FROM account WHERE id = %u";
const char	SQL_ACCOUNT_UPDATEPOINT_STMT[]	= "UPDATE %s SET point=point-1 WHERE account_id=%u AND point>0 LIMIT 1";	//paled

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CAccount::CAccount()
{
    m_pRes = NULL;
    memset(&m_Info, 0L, sizeof(m_Info));
}

CAccount::~CAccount()
{
    Destroy();
}

//////////////////////////////////////////////////////////////////////
BOOL CAccount::CheckPassword (const char* pszPsw)
{
    if (!m_pRes)
    {
        return false;
    }

    if (0 == strcmp(pszPsw, m_Info.szPassword))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
void InsertBackslash(char* bufTarget, const char* pszSource)
{
    const char* pSour = pszSource;
    char* pTarg = bufTarget;
    while(*pSour)
    {
        if (*pSour == '\\' || *pSour == '\'')
        {
            *(pTarg++) = '\\';
        }

        *(pTarg++) = *(pSour++);
    }
    *pTarg = 0;
}

//////////////////////////////////////////////////////////////////////

BOOL CAccount::Create(const char* pszName, const char* pszPassword)
{
    try
    {
        if (!pszName || _MAX_NAMESIZE <= strlen(pszName))
        {
            return false;
        }

		if (!pszPassword || _MAX_PSWSIZE <= strlen(pszPassword) || strlen(pszPassword) == 0)
        {
            return false;
        }
		
        if (m_pRes != NULL)
        {
            ::LogSave("WARNING: CAccount::Create() m_pRes not NULL.");
            this->Destroy();
        }

        char bufName[256];
        InsertBackslash(bufName, pszName);
//		if (strchr(pszName, '\'') || strchr(pszName, '\\'))			// 单引号(')常量和反斜杠(\)常量
        {
//			::LogSave("ERROR: Login with illegal username [%s]. can't login.", pszName);	//★ DEBUG
//			return false;
        }

        char bufPassword[256];
        InsertBackslash(bufPassword, pszPassword);
//		if (strchr(pszPassword, '\'') || (strlen(pszPassword) && pszPassword[strlen(pszPassword)-1] == '\\'))	// 单引号(')常量和反斜杠(\)常量
//		if (strchr(pszPassword, '\'') || strchr(pszPassword, '\\'))	// 单引号(')常量和反斜杠(\)常量
        {
//			::LogSave("ERROR: Login with illegal password [%s]. can't login.", pszPassword);	//★ DEBUG
//			return false;
        }

        m_pRes = new CMyRecordset(&g_db);
        if (!m_pRes)
        {
            ::LogSave("ERROR: CAccount::Create() can't new CMyRecordset object.");
            return false;
        }
        if (bufPassword[0])
        {
            //sprintf(m_szSQL, SQL_ACCOUNT_STMT, POINTTABLE, bufName, bufPassword);
			sprintf(m_szSQL, "SELECT id,name,type,point,pointtime,online,licence,netbar_ip,ip_mask FROM account where name='%s' and password='%s'", bufName,bufPassword);
        }
        else
        {
            sprintf(m_szSQL, SQL_ACCOUNT_STMT2, POINTTABLE, bufName);
        }

        if (!m_pRes->Open(m_szSQL))
        {
            ::LogSave("ERROR: CAccount::Create(char*) can't Open() database for [%s]", m_szSQL);
            delete m_pRes;
            m_pRes = NULL;
            return false;
        }

        if (m_pRes->RecordCount() == 0)
        {
            m_pRes->Close();
            delete m_pRes;
            m_pRes = NULL;
            return false;
        }

        if (!this->LoadInfo())
        {
            ::LogSave("ERROR: CAccount::Create() can't LoadInfo()");
            return false;
        }

        return true;
    }
    catch(...)
    {
        ::LogSave("exception catch at CAccount::Create(%s).", pszName);
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
BOOL CAccount::Create(OBJID	idAccount)		//paled add
{
    try
    {
        if (m_pRes != NULL)
        {
            this->Destroy();
        }

        m_pRes = new CMyRecordset(&g_db);
        sprintf(m_szSQL, SQL_ACCOUNTID_STMT, POINTTABLE, idAccount);

        if (!m_pRes->Open(m_szSQL))
        {
            ::LogSave("ERROR: CAccount::Create(OBJID) can't Open() database for [%s]", m_szSQL);
            delete m_pRes;
            m_pRes = NULL;
            return false;
        }

        if (m_pRes->RecordCount() == 0)
        {
            m_pRes->Close();
            delete m_pRes;
            m_pRes = NULL;
            return false;
        }

        if (!this->LoadInfo())
        {
            return false;
        }

        return true;
    }
    catch(...)
    {
        ::LogSave("exception catch at CAccount::Create(%04X).", idAccount);
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
BOOL CAccount::Create(int nType, const char* pszIPAddr)		//paled add
{
    try
    {
        if (!pszIPAddr)
        {
            return false;
        }
//		ASSERT(nType == c_typeISP);

        if (m_pRes != NULL)
        {
            this->Destroy();
        }

        m_pRes = new CMyRecordset(&g_db);
        sprintf(m_szSQL, SQL_ACCOUNTISP_STMT, POINTTABLE, nType, pszIPAddr);

        if (!m_pRes->Open(m_szSQL))
        {
//			::LogSave("ERROR: CAccount::Create(OBJID) can't Open() database for [%s].", m_szSQL);
            delete m_pRes;
            m_pRes = NULL;
            return false;
        }

        if (m_pRes->RecordCount() == 0)
        {
            m_pRes->Close();
            delete m_pRes;
            m_pRes = NULL;
            return false;
        }

        if (!this->LoadInfo())
        {
            return false;
        }

        return true;
    }
    catch(...)
    {
        ::LogSave("exception catch at CAccount::Create(%d, %s)。", nType, pszIPAddr);
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

void CAccount::Destroy(void)
{
    if (m_pRes != NULL)
    {
        m_pRes->Close();
        delete m_pRes;
        m_pRes = NULL;
    }

    memset(&m_Info, 0L, sizeof(m_Info));
}

//////////////////////////////////////////////////////////////////////
BOOL CAccount::LoadInfo(void)
{
    m_Info.id	= (int)m_pRes->Fields("id");
//?	m_Info.nSex = m_pRes->Fields("sex");
//?	m_Info.nAge = m_pRes->Fields("age");
    m_Info.nType		= (int)m_pRes->Fields("type");
    m_Info.nPoint		= (int)m_pRes->Fields("point");
    m_Info.nPointTime	= (int)m_pRes->Fields("pointtime");
    m_Info.nOnline		= (int)m_pRes->Fields("online");
    m_Info.nLicence		= (int)m_pRes->Fields("licence");

    strcpy(m_Info.szName, m_pRes->Fields("name"));
    strcpy(m_Info.dwNetBarIP, m_pRes->Fields("netbar_ip"));
    strcpy(m_Info.dwIPMask, m_pRes->Fields("ip_mask"));
//?	strcpy(m_Info.szPhone, m_pRes->Fields("phone"));
//?	strcpy(m_Info.szEmail, m_pRes->Fields("email"));
//?	strcpy(m_Info.szAddress, m_pRes->Fields("address"));
//?	strcpy(m_Info.szIdNumber, m_pRes->Fields("idnumber"));
//	strcpy(m_Info.szPassword, m_pRes->Fields("password"));
    return true;
}
/*
//////////////////////////////////////////////////////////////////////
BOOL CAccount::SaveInfo(void)
{
	assert(m_pRes);

	m_pRes->Edit();

	m_pRes->Fields("id") = (int)m_Info.id;
	m_pRes->Fields("age") = m_Info.nAge;
	m_pRes->Fields("sex") = m_Info.nSex;
	m_pRes->Fields("type")		= m_Info.nType;
	m_pRes->Fields("point")		= m_Info.nPoint;
	m_pRes->Fields("pointtime")	= m_Info.nPointTime;
	m_pRes->Fields("licence")	= m_Info.nLicence;

	strcpy(m_pRes->Fields("name"), m_Info.szName);
	strcpy(m_pRes->Fields("phone"),m_Info.szPhone);
	strcpy(m_pRes->Fields("email"),m_Info.szEmail);
	strcpy(m_pRes->Fields("address"),m_Info.szAddress);
	strcpy(m_pRes->Fields("idnumber"),m_Info.szIdNumber);
	strcpy(m_pRes->Fields("password"),m_Info.szPassword);

	m_pRes->Update();
	return true;
}
*/
//////////////////////////////////////////////////////////////////////

char* CAccount::GetName(BOOL bUpdate)
{
    assert(m_pRes);
    if (bUpdate)
    {
        strcpy(m_Info.szName, m_pRes->Fields("name"));
    }
    return m_Info.szName;
}

//////////////////////////////////////////////////////////////////////
void CAccount::SetName(const char* pszName, BOOL bUpdate)
{
    assert(m_pRes);
    strcpy(m_Info.szName, pszName);

    if (bUpdate)
    {
        m_pRes->Edit();
        strcpy(m_pRes->Fields("name"), pszName);
        m_pRes->Update();
    }
}

//////////////////////////////////////////////////////////////////////
/*
char* CAccount::GetPassword(BOOL bUpdate)
{
	assert(m_pRes);
	if (bUpdate)
	{
		strcpy(m_Info.szPassword, m_pRes->Fields("password"));
	}

	return m_Info.szPassword;
}

//////////////////////////////////////////////////////////////////////
void CAccount::SetPassword(const char* pszPsw, BOOL bUpdate)
{
	assert(m_pRes);
	strcpy(m_Info.szPassword, pszPsw);

	if (bUpdate)
	{
		m_pRes->Edit();
		strcpy(m_pRes->Fields("password"), pszPsw);
		m_pRes->Update();
	}
}
*/
//////////////////////////////////////////////////////////////////////

int CAccount::GetType(BOOL bUpdate)
{
    assert(m_pRes);
    if (bUpdate)
    {
        m_Info.nType = m_pRes->Fields("type");
    }

    return m_Info.nType;
}
/*
//////////////////////////////////////////////////////////////////////
void CAccount::SetType(int nType, BOOL bUpdate)
{
	assert(m_pRes);
	m_Info.nType = nType;

	if (bUpdate)
	{
		m_pRes->Edit();
		m_pRes->Fields("type") = nType;
		m_pRes->Update();
	}
}
*/
//////////////////////////////////////////////////////////////////////

int CAccount::GetPoint(BOOL bUpdate)
{
    assert(m_pRes);
    if (bUpdate)
    {
        m_Info.nPoint = m_pRes->Fields("point");
    }

    return m_Info.nPoint;
}
/*
//////////////////////////////////////////////////////////////////////
void CAccount::SetPoint(int nPoint, BOOL bUpdate)
{
	assert(m_pRes);
	m_Info.nPoint = nPoint;

	if (bUpdate)
	{
		m_pRes->Edit();
		m_pRes->Fields("point") = nPoint;
		m_pRes->Update();
	}
}
*/
//////////////////////////////////////////////////////////////////////

int CAccount::GetPointTime(BOOL bUpdate)
{
    assert(m_pRes);
    if (bUpdate)
    {
        m_Info.nPointTime = (int)m_pRes->Fields("pointtime");
    }

    return m_Info.nPointTime;
}
/*
//////////////////////////////////////////////////////////////////////
void CAccount::SetPointTime(int nPointTime, BOOL bUpdate)
{
	assert(m_pRes);
	m_Info.nPointTime = nPointTime;

	if (bUpdate)
	{
		m_pRes->Edit();
		m_pRes->Fields("pointtime") = nPointTime;
		m_pRes->Update();
	}
}
*/
//////////////////////////////////////////////////////////////////////
int CAccount::GetOnline(BOOL bUpdate)
{
    assert(m_pRes);
    if (bUpdate)
    {
        m_Info.nOnline = (int)m_pRes->Fields("online");
    }

    return m_Info.nOnline;
}

//////////////////////////////////////////////////////////////////////
int CAccount::GetLicence(BOOL bUpdate)
{
    assert(m_pRes);
    if (bUpdate)
    {
        m_Info.nLicence = (int)m_pRes->Fields("licence");
    }

    return m_Info.nLicence;
}

//////////////////////////////////////////////////////////////////////
LPCTSTR CAccount::GetNetBarIP(BOOL bUpdate)
{
    assert(m_pRes);
    if (bUpdate)
    {
        strcpy(m_Info.dwNetBarIP, m_pRes->Fields("netbar_ip"));
    }

    return m_Info.dwNetBarIP;
}

//////////////////////////////////////////////////////////////////////
LPCTSTR CAccount::GetIPMask(BOOL bUpdate)
{
    assert(m_pRes);
    if (bUpdate)
    {
        strcpy(m_Info.dwIPMask, m_pRes->Fields("ip_mask"));
    }

    return m_Info.dwIPMask;
}

//////////////////////////////////////////////////////////////////////


int CAccount::GetSex(BOOL bUpdate)
{

    assert(m_pRes);
    if (bUpdate)
    {
        m_Info.nSex = m_pRes->Fields("sex");
    }

    return m_Info.nSex;
}

//////////////////////////////////////////////////////////////////////

void CAccount::SetSex(const int nSex, BOOL bUpdate)
{
    assert(m_pRes);
    m_Info.nSex = nSex;

    if (bUpdate)
    {
        m_pRes->Edit();
        m_pRes->Fields("sex") = m_Info.nSex;
        m_pRes->Update();
    }
}

//////////////////////////////////////////////////////////////////////

char* CAccount::GetPhone(BOOL bUpdate)
{
    assert(m_pRes);
    if (bUpdate)
    {
        strcpy(m_Info.szPhone, m_pRes->Fields("phone"));
    }
    return m_Info.szPhone;
}

//////////////////////////////////////////////////////////////////////

void CAccount::SetPhone(const char* pszPhone, BOOL bUpdate)
{
    assert(m_pRes);
    strcpy(m_Info.szPhone, pszPhone);

    if (bUpdate)
    {
        m_pRes->Edit();
        strcpy(m_pRes->Fields("phone"), pszPhone);
        m_pRes->Update();
    }
}

//////////////////////////////////////////////////////////////////////
char* CAccount::GetEmail(BOOL bUpdate)
{
    assert(m_pRes);
    if (bUpdate)
    {
        strcpy(m_Info.szEmail, m_pRes->Fields("email"));
    }
    return m_Info.szEmail;
}

//////////////////////////////////////////////////////////////////////
void CAccount::SetEmail(const char* pszEmail, BOOL bUpdate)
{
    assert(m_pRes);
    strcpy(m_Info.szEmail, pszEmail);

    if (bUpdate)
    {
        m_pRes->Edit();
        strcpy(m_pRes->Fields("email"), pszEmail);
        m_pRes->Update();
    }
}

//////////////////////////////////////////////////////////////////////
char* CAccount::GetAddress(BOOL bUpdate)
{
    assert(m_pRes);
    if (bUpdate)
    {
        strcpy(m_Info.szAddress, m_pRes->Fields("address"));
    }
    return m_Info.szAddress;
}

//////////////////////////////////////////////////////////////////////
void CAccount::SetAddress(const char* pszAddress, BOOL bUpdate)
{
    assert(m_pRes);
    strcpy(m_Info.szAddress, pszAddress);
    if (bUpdate)
    {
        m_pRes->Edit();
        strcpy(m_pRes->Fields("address"), pszAddress);
        m_pRes->Update();
    }
}

//////////////////////////////////////////////////////////////////////
char* CAccount::GetIdNumber(BOOL bUpdate)
{
    assert(m_pRes);
    if (bUpdate)
    {
        strcpy(m_Info.szIdNumber, m_pRes->Fields("idnumber"));
    }
    return m_Info.szIdNumber;
}

//////////////////////////////////////////////////////////////////////
void CAccount::SetIdNumber(const char* pszIdNumber, BOOL bUpdate)
{
    assert(m_pRes);
    strcpy(m_Info.szIdNumber, pszIdNumber);
    if (bUpdate)
    {
        m_pRes->Edit();
        strcpy(m_pRes->Fields("idnumber"), pszIdNumber);
        m_pRes->Update();
    }
}

//////////////////////////////////////////////////////////////////////
int	CAccount::GetAge(BOOL bUpdate)
{
    assert(m_pRes);
    if (bUpdate)
    {
        m_Info.nAge = m_pRes->Fields("age");
    }
    return m_Info.nAge;
}

//////////////////////////////////////////////////////////////////////
void CAccount::SetAge(int nAge, BOOL bUpdate)
{
    assert(m_pRes);
    m_Info.nAge = nAge;
    if (bUpdate)
    {
        m_pRes->Edit();
        m_pRes->Fields("age") = nAge;
        m_pRes->Update();
    }
}

//////////////////////////////////////////////////////////////////////
// static functions
//////////////////////////////////////////////////////////////////////
CAccount* CAccount::CreateNew (const char* pszName, const char* pszPassword)
{
    CAccount* pAccount	= new CAccount;
    if (!pAccount)
    {
        return NULL;
    }

    char	bufName[256];
    InsertBackslash(bufName, pszName);
    char	bufPassword[256];
    InsertBackslash(bufPassword, pszPassword);
    if (!pAccount->Create(bufName, bufPassword))
    {
        delete pAccount;
        return NULL;
    }

    return pAccount;
}
/*
//////////////////////////////////////////////////////////////////////
OBJID CAccount::CreateRecord(AccountInfoStruct * pAccountInfo)
{
	if (!pAccountInfo)
		return ID_NONE;

	char szSQL[256];
	sprintf( szSQL, SQL_ACCOUNT_INSERT_STMT,
					pAccountInfo->szName,
					pAccountInfo->nSex,
					pAccountInfo->nAge,
					pAccountInfo->szPhone,
					pAccountInfo->szEmail,
					pAccountInfo->szAddress,
					pAccountInfo->szIdNumber,
					pAccountInfo->szPassword);

	if (g_db.ExecuteSQL(szSQL,NULL))
		return g_db.GetInsertId();
	else
		return ID_NONE;
}

//////////////////////////////////////////////////////////////////////
BOOL CAccount::DeleteRecord(OBJID id)
{
	if (id == ID_NONE)
		return false;

	char szSQL[256];
	sprintf( szSQL, SQL_ACCOUNT_DELETE_STMT, id);

	if (!g_db.ExecuteSQL(szSQL,NULL))
		return false;

	return true;
}
*/
//////////////////////////////////////////////////////////////////////
bool	CAccount::DecPoint(OBJID idAccount)	//paled
{
    char szSQL[256];
    sprintf( szSQL, SQL_ACCOUNT_UPDATEPOINT_STMT, POINTTABLE, idAccount);

    return g_db.ExecuteSQL(szSQL, NULL);		//?? 还应该检查修改行的数量，以确定是否被更新。当未更新时返回FALSE，以便再试一次。
}



