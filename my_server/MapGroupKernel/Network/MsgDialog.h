
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgDialog.h: interface for the CMsgDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MSGDIALOG_H
#define MSGDIALOG_H

#include "NetMsg.h"
#include "NetStringPacker.h"

enum
{
    MSGDIALOG_TEXT		= 1,
    MSGDIALOG_LINK,
    MSGDIALOG_EDIT,
    MSGDIALOG_PIC,						// data: npc face
    MSGDIALOG_LISTLINE,
    MSGDIALOG_CREATE	= 100,			// idxTask: default task
    MSGDIALOG_ANSWER	= 101,			// to server
    MSGDIALOG_TASKID	= 102,			// to server, launch task id by interface
};

class CMsgDialog : public CNetMsg
{
public:
    CMsgDialog();
    virtual ~CMsgDialog();

    BOOL	Create	(int nAction, LPCTSTR szText, int idxTask, int nData);
    BOOL	Create	(int nAction, int x, int y, int nPic, int idxTask);
    BOOL	Create	(int nAction, int idxTask);

public:
    BOOL			Create		(char* pbufMsg, DWORD dwMsgSize);
    void			Process		(void* pInfo);

private:
    typedef struct
    {
        MSGHEAD_DEFINE

        union
        {
            struct
            {
                USHORT	usPosX;
                USHORT	usPosY;
            };
            DWORD	idTask;			// panel task id, to server only
        };
        USHORT	usData;
        UCHAR	idxTask;
        UCHAR	ucAction;
        char	szBuf[1];
        // szText
    } MSG_Info;

    MSG_Info*	m_pInfo;

private:
    CNetStringPacker	m_StrPacker;

};

#endif
