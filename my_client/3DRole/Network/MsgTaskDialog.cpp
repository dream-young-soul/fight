
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgTaskDialog.cpp: implementation of the CMsgTaskDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgTaskDialog.h"
#include "Hero.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgTaskDialog::CMsgTaskDialog()
{
    Init();
    m_pInfo	= (MSG_Info* )m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgTaskDialog::~CMsgTaskDialog()
{
}

//--------------------------------------------------------------------
BOOL CMsgTaskDialog::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    if ((unsigned short)dwMsgSize != GetSize(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_TASKDIALOG != GetType(pbufMsg, dwMsgSize))
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}
//--------------------------------------------------------------------
BOOL CMsgTaskDialog::Create(int nIndex, char* pszStr, OBJID idAction, int nAction)
{
    // fill
    this->Init();
    m_pInfo->ucAction = nAction;
    m_pInfo->idxTask = nIndex;
    if (pszStr)
    {
        m_StrPacker.AddString(pszStr);
    }
    m_pInfo->idTask = idAction;
    m_pInfo->unMsgType	= _MSG_TASKDIALOG;
    m_pInfo->unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}
//--------------------------------------------------------------------
void CMsgTaskDialog::Process(void* pInfo)
{
    switch(m_pInfo->ucAction)
    {
    case MSGDIALOG_TEXT:
        {
            TaskDialogLine* pLine = new TaskDialogLine;
            MYASSERT(pLine);
            if (m_StrPacker.GetStrAmount() > 0)
            {
                m_StrPacker.GetString(0, pLine->szStr, sizeof(pLine->szStr));
                //				::ReplaceString(pLine->szStr, '~', ' ');
            }
            pLine->dwData = m_pInfo->usData;
            pLine->posLine.x = m_pInfo->usPosX;
            pLine->posLine.y = m_pInfo->usPosY;
            pLine->nType = _TASKDIALOG_TEXT;
            pLine->nIndex = m_pInfo->idxTask;
            g_objHero.AddTaskDialogLine(pLine);
        }
        break;
    case MSGDIALOG_LINK:
    case _TASKDIALOG_LIST:
        {
            TaskDialogLine* pLine = new TaskDialogLine;
            MYASSERT(pLine);
            if (m_StrPacker.GetStrAmount() > 0)
            {
                m_StrPacker.GetString(0, pLine->szStr, sizeof(pLine->szStr));
                //				::ReplaceString(pLine->szStr, '~', ' ');
            }
            pLine->dwData = m_pInfo->usData;
            pLine->posLine.x = m_pInfo->usPosX;
            pLine->posLine.y = m_pInfo->usPosY;
            pLine->nType = _TASKDIALOG_LINK;
            pLine->nIndex = m_pInfo->idxTask;
            g_objHero.AddTaskDialogLine(pLine);
        }
        break;
    case MSGDIALOG_EDIT:
        {
            TaskDialogLine* pLine = new TaskDialogLine;
            MYASSERT(pLine);
            if (m_StrPacker.GetStrAmount() > 0)
            {
                m_StrPacker.GetString(0, pLine->szStr, sizeof(pLine->szStr));
                //				::ReplaceString(pLine->szStr, '~', ' ');
            }
            pLine->dwData = m_pInfo->usData;
            pLine->posLine.x = m_pInfo->usPosX;
            pLine->posLine.y = m_pInfo->usPosY;
            pLine->nType = _TASKDIALOG_EDIT;
            pLine->nIndex = m_pInfo->idxTask;
            g_objHero.AddTaskDialogLine(pLine);
        }
        break;
    case MSGDIALOG_PIC:
        {
            TaskDialogLine* pLine = new TaskDialogLine;
            MYASSERT(pLine);
            if (m_StrPacker.GetStrAmount() > 0)
            {
                m_StrPacker.GetString(0, pLine->szStr, sizeof(pLine->szStr));
                //				::ReplaceString(pLine->szStr, '~', ' ');
            }
            pLine->dwData = m_pInfo->usData;
            pLine->posLine.x = m_pInfo->usPosX;
            pLine->posLine.y = m_pInfo->usPosY;
            pLine->nType = _TASKDIALOG_PIC;
            pLine->nIndex = m_pInfo->idxTask;
            g_objHero.AddTaskDialogLine(pLine);
        }
        break;
    case MSGDIALOG_CREATE:
        {
            TaskDialogLine* pLine = new TaskDialogLine;
            MYASSERT(pLine);
            if (m_StrPacker.GetStrAmount() > 0)
            {
                m_StrPacker.GetString(0, pLine->szStr, sizeof(pLine->szStr));
                //				::ReplaceString(pLine->szStr, '~', ' ');
            }
            pLine->dwData = m_pInfo->usData;
            pLine->posLine.x = m_pInfo->usPosX;
            pLine->posLine.y = m_pInfo->usPosY;
            pLine->nType = _TASKDIALOG_DEFAULT;
            pLine->nIndex = m_pInfo->idxTask;
            g_objHero.AddTaskDialogLine(pLine);
            g_objHero.CreateTaskDialog();
            ::PostCmd(CMD_TASKDIALOGOPEN);
        }
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------
