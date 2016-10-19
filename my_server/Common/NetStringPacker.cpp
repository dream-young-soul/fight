
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NetStringPacker.cpp: implementation of the CNetStringPacker class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "NetStringPacker.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetStringPacker::CNetStringPacker()
{
    m_pBuf	= NULL;
}

CNetStringPacker::~CNetStringPacker()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CNetStringPacker::AddString(const char* pszString)
{
    /*
    if (!pszString)
    	return false;
    */
    if (!m_pBuf)
    {
        return false;
    }
    if (pszString)
    {
        UINT uLen	= strlen(pszString);
        if (uLen > 255)
        {
            return false;
        }
        UINT uCurSize	= this->GetSize();
        if (m_uBufSize < uLen + uCurSize)
        {
            return false;
        }
        UCHAR* pBuf	= (UCHAR*)m_pBuf;
        *pBuf	= (UCHAR)(this->GetStrAmount() + 1);
        pBuf	+= uCurSize;
        *pBuf	= uLen;
        pBuf++;
        memcpy(pBuf, pszString, uLen);
    }
    else
    {
        UINT uSize	= this->GetSize();
        UCHAR* pBuf	= (UCHAR*)m_pBuf;
        *pBuf	= (UCHAR)(this->GetStrAmount() + 1);
        pBuf	+= uSize;
        *pBuf	= 0;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CNetStringPacker::SetBuf(char* pBuf, UINT uBufSize)
{
    m_pBuf		= pBuf;
    m_uBufSize	= uBufSize;
    memset(m_pBuf, 0L, m_uBufSize);
}


//////////////////////////////////////////////////////////////////////
BOOL CNetStringPacker::GetString(UINT uIndex, char* pszStrBuf, UINT uBufSize)
{
    memset(pszStrBuf, 0L, uBufSize);
    UCHAR*	pBuf	= (UCHAR*)m_pBuf;
    pBuf++;
    UINT uAmount	= this->GetStrAmount();
    if (uIndex >= uAmount)
    {
        return false;
    }
    for(UINT i = 0; i < uAmount; i++)
    {
        UCHAR ucStrSize	= *pBuf;	// get size...
        if (uIndex == i)	// got it...
        {
            if (uBufSize <= (UINT)ucStrSize)
            {
                return false;
            }
            else
            {
                memcpy(pszStrBuf, pBuf + 1, (int)ucStrSize);
                return true;
            }
        }
        else			// next string
        {
            pBuf	+= (1 + ucStrSize);
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
UINT CNetStringPacker::GetStrAmount(void)
{
    UCHAR*	pBuf	= (UCHAR*)m_pBuf;
    return *pBuf;
}

//////////////////////////////////////////////////////////////////////
UINT CNetStringPacker::GetSize(void)
{
    UCHAR*	pBuf	= (UCHAR*)m_pBuf;
    pBuf++;
    UINT uObjSize	= 1;
    UINT uAmount	= this->GetStrAmount();
    for(UINT i = 0; i < uAmount; i++)
    {
        UCHAR ucStrSize	= *pBuf;
        pBuf	+= (1 + ucStrSize);
        uObjSize	+= (1 + ucStrSize);
    }
    return uObjSize;
}
