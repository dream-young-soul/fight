
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgMessageBoard.h: interface for the CMsgMessageBoard class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MESSAGEBOARD_H
#define MESSAGEBOARD_H

#include "NetMsg.h"
#include "NetStringPacker.h"
//#include "MessageBoard.h"

//class CMessageBoard;
//WORLDKERNEL_BEGIN

enum
{
    MESSAGEBOARD_DEL				= 1,			// to server					// no return
    MESSAGEBOARD_GETLIST			= 2,			// to server: index(first index)
    MESSAGEBOARD_LIST				= 3,			// to client: index(first index), name, words, time...
    MESSAGEBOARD_GETWORDS			= 4,			// to server: index(for get)	// return by MsgTalk
};

class CMsgMessageBoard : public CNetMsg
{
public:
    CMsgMessageBoard();
    virtual ~CMsgMessageBoard();

    //	BOOL	Create	(int nAction, int nChannel, int nIndex, CMessageBoard* pObj=NULL);

public:
    BOOL			Create		(char* pbufMsg, DWORD dwMsgSize);
    void			Process		(void* pInfo);

private:
    typedef struct
    {
        MSGHEAD_DEFINE

        USHORT	usIndex;
        USHORT	usChannel;
        UCHAR	ucAction;
        char	szBuf[1];
    } MSG_Info;

    MSG_Info*	m_pInfo;

private:
    CNetStringPacker	m_StrPacker;

};
//WORLDKERNEL_END

#endif
