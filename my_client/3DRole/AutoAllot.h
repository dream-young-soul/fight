
//**********************************************************
// 代码编辑器
//**********************************************************

// AutoAllot.h: interface for the CAutoAlloc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOALLOC_H__D1ACB192_904A_4723_835D_541423CBAEB7__INCLUDED_)
#define AFX_AUTOALLOC_H__D1ACB192_904A_4723_835D_541423CBAEB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"

// 四围定义
typedef struct
{
    DWORD dwForce;
    DWORD dwSpeed;
    DWORD dwHealth;
    DWORD dwSoul;
} FourAttribInfo;

class CAutoAllot
{
public:
    CAutoAllot();
    virtual ~CAutoAllot();
private:
    DWORD			m_dwLevelAmount;				// 总共有多少个等级
    DWORD			m_dwProfessionalAmount;			// 总共有多少个职业
    DWORD*			m_pdwProfessionalIndex;			// 职业索引

private:
    void	BulidIndex();				// 创建索引

public:
    BOOL	GetAllot(DWORD dwLevel, DWORD dwProfessional, FourAttribInfo& infoAttrib);
};

#endif // !defined(AFX_AUTOALLOC_H__D1ACB192_904A_4723_835D_541423CBAEB7__INCLUDED_)
