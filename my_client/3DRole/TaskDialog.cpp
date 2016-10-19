
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TaskDialog.cpp: implementation of the CTaskDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "TaskDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskDialog::CTaskDialog()
{
    m_bCreate = false;
}

CTaskDialog::~CTaskDialog()
{
    this->Destroy();
}
//--------------------------------------------------------------------
void CTaskDialog::Destroy()
{
    int nAmount = m_setTaskDialogLine.size();
    for(int i = 0; i < nAmount; i++)
    {
        TaskDialogLine* pLine = m_setTaskDialogLine[i];
        SAFE_DELETE(pLine);
    }
    m_setTaskDialogLine.clear();
    m_bCreate = false;
}
//--------------------------------------------------------------------
TaskDialogLine*	CTaskDialog::GetLineByIndex(int nIndex)
{
    int nAmount = m_setTaskDialogLine.size();
    if ((nIndex >= nAmount) || (nIndex < 0))
    {
        return NULL;
    }
    return m_setTaskDialogLine[nIndex];
}
//--------------------------------------------------------------------
void CTaskDialog::AddLine(int nType, DWORD dwData, int nIndex, char* szStr, CMyPos posLine)
{
    if (m_bCreate)
    {
        m_bCreate = false;
        this->Destroy();
    }
    TaskDialogLine* pLine = new TaskDialogLine;
    MYASSERT(pLine);
    pLine->dwData = dwData;
    pLine->nIndex = nIndex;
    pLine->nType = nType;
    pLine->posLine = posLine;
    if (szStr || (strlen(szStr) < _MAX_STRING))
    {
        strcpy(pLine->szStr, szStr);
    }
    else
    {
        strcpy(pLine->szStr, "");
    }
    m_setTaskDialogLine.push_back(pLine);
}
//--------------------------------------------------------------------
void CTaskDialog::AddLine(TaskDialogLine* pLine)
{
    if (m_bCreate)
    {
        m_bCreate = false;
        this->Destroy();
    }
    if (pLine)
    {
        m_setTaskDialogLine.push_back(pLine);
    }
}
//--------------------------------------------------------------------
