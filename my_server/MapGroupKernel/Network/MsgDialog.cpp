
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgDialog.cpp: implementation of the CMsgDialog class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#ifdef	WORLD_KERNEL
#include "userlist.h"
#else
#pragma warning(disable:4786)
#include "mapgroup.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgDialog::CMsgDialog()
{
    Init();
    m_pInfo	= (MSG_Info* )m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgDialog::~CMsgDialog()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgDialog::Create	(int nAction, LPCTSTR szText, int idxTask, int nData)
{
    CHECKF(szText);
    // fill
    this->Init();
    // fill structure
    m_pInfo->idxTask	= idxTask;
    m_pInfo->usData		= nData;
    m_pInfo->usPosX		= 0;
    m_pInfo->usPosY		= 0;
    m_pInfo->ucAction	= nAction;
    BOOL ret = m_StrPacker.AddString(szText);
    m_unMsgType	= _MSG_DIALOG;
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return ret;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgDialog::Create	(int nAction, int x, int y, int nPic, int idxTask)
{
    CHECKF(nAction == MSGDIALOG_PIC);
    // fill
    this->Init();
    // fill structure
    m_pInfo->idxTask	= idxTask;
    m_pInfo->usData		= nPic;
    m_pInfo->usPosX		= x;
    m_pInfo->usPosY		= y;
    m_pInfo->ucAction	= nAction;
    m_unMsgType	= _MSG_DIALOG;
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgDialog::Create	(int nAction, int idxTask)
{
    CHECKF(nAction == MSGDIALOG_CREATE);
    // fill
    this->Init();
    // fill structure
    m_pInfo->idxTask	= idxTask;
    m_pInfo->usData		= 0;
    m_pInfo->usPosX		= 0;
    m_pInfo->usPosY		= 0;
    m_pInfo->ucAction	= nAction;
    m_unMsgType	= _MSG_DIALOG;
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgDialog::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgDialog::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogMsg("Process MsgDialog, Sender:%s, Receiver:%s, Words:%s",
             szSender,
             szReceiver,
             szWords);
#endif
    char* pAccept = "";
    char szText[MAX_PARAMSIZE];
    if (m_StrPacker.GetString(0, szText, sizeof(szText)))
    {
        pAccept	= szText;
    }
    CHECK(strlen(szText) < MAX_PARAMSIZE);
    CUserPtr pUser = UserManager()->GetUser(this);
    CHECK(pUser);
    if (!pUser->IsAlive())
    {
        pUser->SendSysMsg(STR_DIE);
        return ;
    }
    switch(m_pInfo->ucAction)
    {
    case	MSGDIALOG_ANSWER:
        {
            pUser->ProcessTask(m_pInfo->idxTask, pAccept);
        }
        break;
    case	MSGDIALOG_TASKID:
        {
            pUser->ProcessClientTask(m_pInfo->idTask, pAccept);
        }
        break;
    }
}
