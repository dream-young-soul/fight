
//**********************************************************
// 代码编辑器
//**********************************************************

// Check.h: interface for the CCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECK_H__BD357BB3_6170_4E69_8778_C55BCA1682B8__INCLUDED_)
#define AFX_CHECK_H__BD357BB3_6170_4E69_8778_C55BCA1682B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"

class CCheck
{
public:
    enum
    {
        _WS_MAINWINDOW_	= 0x00000001, // 主窗体
    };
    enum
    {
        _TYPE_WS				= 1, // 无双
        _TYPE_FY				= 2, // 风云
        _TYPE_USE_LIFE_MEDICINE	= 3, // 用红药
        _TYPE_USE_MANA_MEDICINE	= 4, // 用蓝药
    };
public:
    CCheck();
    virtual ~CCheck();
public:
    BOOL Process();
    void Reset();
    void DecLife();
    void DecMana();
    void AddLife();
    void AddMana();
private:
    DWORD	m_dwTimeCheck;				// 检测时间为启动后5－10分钟随机
    BOOL	m_bReportWSProcess;			// 每次启动只报告一次
    BOOL	m_bReportFYProcess;			// 每次启动只报告一次
    BOOL	m_bReportUseLifeMedicine;	// 每次启动只报告一次
    BOOL	m_bReportUseManaMedicine;	// 每次启动只报告一次
    int		m_nUseLifeMedicineAmount;
    int		m_nUseManaMedicineAmount;
    DWORD	m_dwLastDecLife;
    DWORD	m_dwLastDecMana;
    union
    {
        DWORD	m_dwData;
        struct
        {
            UCHAR	m_ucType; // 程序类型
        };
    };
};

#endif // !defined(AFX_CHECK_H__BD357BB3_6170_4E69_8778_C55BCA1682B8__INCLUDED_)
