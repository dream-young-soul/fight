
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// EncryptData.h: interface for the CEncryptData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENCRYPTDATA_H__F5417883_9631_4AF1_8829_1A4ABD409790__INCLUDED_)
#define AFX_ENCRYPTDATA_H__F5417883_9631_4AF1_8829_1A4ABD409790__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"

class CEncryptData
{
public:
    CEncryptData();
    virtual ~CEncryptData();
public:
    DWORD*	set_key(const DWORD in_key[], const DWORD key_len);
	void	Reset();

public:
    void	SetValue(int nData);
    int		GetValue(void);
private:
    // rc6 ³­À´µÄ...
    DWORD  l_key[44];
    inline void	encrypt(const DWORD in_blk[4], DWORD out_blk[4]);
    inline void	decrypt(const DWORD in_blk[4], DWORD out_blk[4]);

private:
    DWORD	m_dwValue[4];
};

#endif // !defined(AFX_ENCRYPTDATA_H__F5417883_9631_4AF1_8829_1A4ABD409790__INCLUDED_)
