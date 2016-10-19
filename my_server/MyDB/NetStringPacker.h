
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NetStringPacker.h: interface for the CNetStringPacker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETSTRINGPACKER_H__185B6BE4_A9BA_463E_B618_7260C0B8CD1A__INCLUDED_)
#define AFX_NETSTRINGPACKER_H__185B6BE4_A9BA_463E_B618_7260C0B8CD1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNetStringPacker
{
public:
    CNetStringPacker();
    virtual ~CNetStringPacker();

    BOOL	AddString		(const char* pszString);

    BOOL	GetString		(UINT uIndex, char* pszStrBuf, UINT uBufSize);
    UINT	GetStrAmount	(void);
    UINT	GetSize			(void);

    void	SetBuf			(char* pBuf, UINT uBufSize);


private:
    char*	m_pBuf;
    UINT	m_uBufSize;
};

#endif // !defined(AFX_NETSTRINGPACKER_H__185B6BE4_A9BA_463E_B618_7260C0B8CD1A__INCLUDED_)
