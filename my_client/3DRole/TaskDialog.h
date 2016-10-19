
//**********************************************************
// 代码编辑器
//**********************************************************

// TaskDialog.h: interface for the CTaskDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKDIALOG_H__9E7E3639_6462_47E2_B177_8B81CFAF046B__INCLUDED_)
#define AFX_TASKDIALOG_H__9E7E3639_6462_47E2_B177_8B81CFAF046B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"

// 任务显示的文本，一般是提出问题// szStr 文本
const int _TASKDIALOG_TEXT		= 0;	// 文本

// 回答选项，点击以后回答 //  szStr 选项 // nIndex 选项号
const int _TASKDIALOG_LINK		= 1;	// 回答

// 回答输入筐 // szStr 输入筐标题 // nIndex 选项号 //  dwData 允许最大长度
const int _TASKDIALOG_EDIT		= 2;	// 输入筐

// Npc 头像 // dwData 头像编号
const int _TASKDIALOG_PIC		= 3;	// 头像

// 直接关闭对话筐 // nIndex 选项号
const int _TASKDIALOG_DEFAULT	= 4;	// 没有回答

// 列表项 // szStr 文本
const int _TASKDIALOG_LIST	= 5;	// 回答列表项

typedef struct
{
    int		nType;	// 类型
    DWORD	dwData; // 数据
    int		nIndex; // 返回
    char	szStr[_MAX_STRING]; // 字符串
    CMyPos	posLine; // 预留
} TaskDialogLine;

#include <deque>
using namespace std;
typedef deque<TaskDialogLine*> DEQUE_TASKDIALOGLINE;

class CTaskDialog
{
public:
    CTaskDialog();
    virtual ~CTaskDialog();
private:
    BOOL	m_bCreate;
    DEQUE_TASKDIALOGLINE	m_setTaskDialogLine;
public:
    void			AddLine(TaskDialogLine* pLine);
    void			Destroy();
    int				GetLineAmount() {return m_setTaskDialogLine.size();}
    TaskDialogLine*	GetLineByIndex(int nIndex);
    void			AddLine(int nType, DWORD dwData, int nIndex, char* szStr, CMyPos posLine);
    void			Create() {m_bCreate = true;}
};

#endif // !defined(AFX_TASKDIALOG_H__9E7E3639_6462_47E2_B177_8B81CFAF046B__INCLUDED_)
