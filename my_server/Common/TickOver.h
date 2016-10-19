
//**********************************************************
// 代码编辑器
//**********************************************************

// TickOver.h: interface for the CTickOver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TICKOVER_H__059AED43_D21C_4072_A172_20A7D8B6DF52__INCLUDED_)
#define AFX_TICKOVER_H__059AED43_D21C_4072_A172_20A7D8B6DF52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"

//////////////////////////////////////////////////////////////////////
// 超时类，以“秒”为单位
class CTickOver
{
public:
    CTickOver() { m_tTickEnd = 0; m_tTickBegin = 0; }
    //	virtual ~CTickOver() {}

public:
    // 开始启动时钟
    void	Startup		(int nNow, int nDur)	{ ASSERT(nDur > 0); m_tTickBegin = nNow; m_tTickEnd = nNow + nDur; }
    // 检查是否超时了指定时间片
    bool	IsTickOver	(int nNow)	const		{ return nNow >= m_tTickEnd; }
    // 用于定时触发的连续时钟。每次触发时为true。
    bool	ToNextTime	(int nNow)				{ if (!IsTickOver(nNow)) { return false; } int nDur = m_tTickEnd - m_tTickBegin; m_tTickBegin += nDur; m_tTickEnd += nDur; return true; }

public: // get 用于存盘
    int		GetBegin()				const		{ return m_tTickBegin; }
    int		GetEnd()				const		{ return m_tTickEnd; }
    int		GetDurable()			const		{ return m_tTickEnd - m_tTickBegin; }
    int		GetDurable(int nNow)	const		{ return m_tTickEnd - nNow; }

protected:
    int		m_tTickBegin;
    int		m_tTickEnd;
};


#endif // !defined(AFX_TICKOVER_H__059AED43_D21C_4072_A172_20A7D8B6DF52__INCLUDED_)
