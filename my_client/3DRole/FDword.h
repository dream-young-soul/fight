
//**********************************************************
// 代码编辑器
//**********************************************************

// FDword.h: interface for the CFDword class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FDWORD_H__FF01F7F6_C985_4B2D_BD75_C9AB9DF11CC6__INCLUDED_)
#define AFX_FDWORD_H__FF01F7F6_C985_4B2D_BD75_C9AB9DF11CC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"

class CFDword
{
public:
    CFDword();
    virtual ~CFDword();
private:
    int		m_nOrder;	// 正序，逆序，偶数在前，奇数在前
    DWORD	m_dwSize;	// the size od block
    DWORD*	m_pData;	// brock ptr
public:
    BOOL			Create(int nOrder, DWORD dwSize);
    static CFDword*	CreateNew(int nOrder, DWORD dwSize);// size is by dword
    void		SetValue(DWORD dwValue, int nIndex);
    DWORD	GetValue(int nIndex);

private:
    inline DWORD		Index2Order	(int nIndex);
    inline void			CheckOrder	(int nOrder);
    inline void			Encrypt		(DWORD& dwData, int nOrder);
    inline void			DecryptFile	(DWORD& dwData, int nOrder);
};

#endif // !defined(AFX_FDWORD_H__FF01F7F6_C985_4B2D_BD75_C9AB9DF11CC6__INCLUDED_)
