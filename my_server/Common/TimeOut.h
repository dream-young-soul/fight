
//**********************************************************
// 代码编辑器
//**********************************************************

// TimeOut.h: interface for the CTimeOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEOUT_H__059AED43_D21C_4072_A172_20A7D8B6DF52__INCLUDED_)
#define AFX_TIMEOUT_H__059AED43_D21C_4072_A172_20A7D8B6DF52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <time.h>
#include "BaseFunc.h"

//////////////////////////////////////////////////////////////////////
// 超时类，以“秒”为单位
template<int I>
class CTimeOut_
{
public:
    CTimeOut_(int nIntervalSecs = I) { m_nInterval = nIntervalSecs; m_tUpdateTime = 0; }		// =0 : 用于数组初始化
    //	virtual ~CTimeOut_() {}

public:
    // 设置时间标签
    void	Update		()						{ m_tUpdateTime = (clock() / 1000); }
    // 检查是否超时
    bool	IsTimeOut	()						{ return (clock() / 1000) >= m_tUpdateTime + m_nInterval; }
    // 用于定时触发的连续时钟。每次触发时为true。
    bool	ToNextTime	()						{ if (IsTimeOut()) { return Update(), true; } else { return false; } }
    // 初始化时间片。(但不启动)
    void	SetInterval	(int nSecs)				{ m_nInterval = nSecs; }

    // 开始启动时钟(同SetInterval)
    void	Startup		(int nSecs)				{ m_nInterval = nSecs; Update(); }
    // 时钟是否触发(只触发一次)，与Startup配合使用，可实现只触发一次的时钟。
    bool	TimeOver	()						{ if (IsActive() && IsTimeOut()) { return Clear(), true; } return false; }
    void	Clear		()						{ m_nInterval = 0; m_tUpdateTime = 0; }
    bool	IsActive	()						{ return m_tUpdateTime != 0; }
    void	IncInterval	(int nSecs, int nLimit)	{ m_nInterval = ::CutOverflow(m_nInterval + nSecs, nLimit); }
    void	DecInterval	(int nSecs)				{ m_nInterval = ::CutTrail(m_nInterval - nSecs, 0); }

    // 检查是否超时了指定时间片。用于同一时间标签检查多个不同超时事件。(可实现一个对象控制多个时钟)
    bool	IsTimeOut	(int nSecs)				{ return (clock() / 1000) >= m_tUpdateTime + nSecs; }
    // 用于变长的超时事件。
    bool	ToNextTime	(int nSecs)				{ if (IsTimeOut(nSecs)) { return Update(), true; } else { return false; } }
    // 时钟是否触发(只触发一次)，与Startup配合使用，可实现只触发一次的时钟。
    bool	TimeOver	(int nSecs)					{ if (IsActive() && IsTimeOut(nSecs)) { return Clear(), true; } return false; }
    // 时钟会积累，但积累值不超过间隔值。其它同ToNextTime(...)
    bool	ToNextTick	(int nSecs);
    //	void	AppendInterval	(int nSecs)			{ if (ToNextTime()) m_nInterval=nSecs; else m_nInterval+=nSecs; }	// ToNextTime(): 保证至少有nSecs秒

public: // get
    int		GetRemain()							{ return m_tUpdateTime ? ::CutRange<time_t>(m_nInterval - ((clock() / 1000) - (int)m_tUpdateTime), 0, m_nInterval) : 0; }
    int		GetInterval()						{ return m_nInterval; }

protected:
    int		m_nInterval;
    time_t	m_tUpdateTime;
};
typedef	CTimeOut_<0>	CTimeOut;

//////////////////////////////////////////////////////////////////////
// 超时类，以“毫秒”为单位
template<int I>
class CTimeOutMS_
{
public:
    CTimeOutMS_(int nIntervalSecs = I) { m_nInterval = nIntervalSecs; m_tUpdateTime = 0; }		// =0 : 用于数组初始化
    //	virtual ~CTimeOutMS_() {}

public:
    void	Update		()						{ m_tUpdateTime = clock(); }
    bool	IsTimeOut	()						{ return clock() >= m_tUpdateTime + m_nInterval; }
    bool	ToNextTime	()						{ if (IsTimeOut()) { return Update(), true; } else { return false; } }
    void	SetInterval	(int nMilliSecs)		{ m_nInterval = nMilliSecs; }

    void	Startup		(int nMilliSecs)		{ m_nInterval = nMilliSecs; Update(); }
    bool	TimeOver	()						{ if (IsActive() && IsTimeOut()) { return Clear(), true; } return false; }
    void	Clear		()						{ m_nInterval = m_tUpdateTime = 0; }
    bool	IsActive	()						{ return m_tUpdateTime != 0; }
    void	IncInterval	(int nMilliSecs, int nLimit)		{ m_nInterval = ::CutOverflow(m_nInterval + nMilliSecs, nLimit); }
    void	DecInterval	(int nMilliSecs)		{ m_nInterval = ::CutTrail(m_nInterval - nMilliSecs, 0); }

    bool	IsTimeOut	(int nMilliSecs)		{ return clock() >= m_tUpdateTime + nMilliSecs; }
    bool	ToNextTime	(int nMilliSecs)		{ if (IsTimeOut(nMilliSecs)) { return Update(), true; } else { return false; } }
    bool	TimeOver	(int nMilliSecs)		{ if (IsActive() && IsTimeOut(nMilliSecs)) { return Clear(), true; } return false; }
    bool	ToNextTick	(int nMilliSecs);
    //	void	AppendInterval	(int nMilliSecs)	{ if (ToNextTime()) m_nInterval=nMilliSecs; else m_nInterval+=nMilliSecs; }	// ToNextTime(): 保证至少有nSecs秒

public: // get
    int		GetRemain()							{ return m_tUpdateTime ? ::CutRange<time_t>(m_nInterval - (clock() - (int)m_tUpdateTime), 0, m_nInterval) : 0; }
    int		GetInterval()						{ return m_nInterval; }

protected:
    int		m_nInterval;
    clock_t	m_tUpdateTime;
};
typedef	CTimeOutMS_<0>	CTimeOutMS;

#endif // !defined(AFX_TIMEOUT_H__059AED43_D21C_4072_A172_20A7D8B6DF52__INCLUDED_)
