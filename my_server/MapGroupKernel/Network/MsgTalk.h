
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

MSGHEAD_DEFINE

// MsgTalk.h: interface for the CMsgTalk class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MSGTALK_H
#define _MSGTALK_H

#include "NetMsg.h"
#include "NetStringPacker.h"

class CMsgTalk : public CNetMsg
{
public:
    CMsgTalk();
    virtual ~CMsgTalk();

    BOOL	Create	(LPCTSTR pszWords)
    { return Create(SYSTEM_NAME, ALLUSERS_NAME, pszWords, NULL, 0xFFFFFF, _TXTATR_NORMAL); }
    BOOL	Create	(USHORT unAttribute, LPCTSTR pszWords)
    { return Create(SYSTEM_NAME, ALLUSERS_NAME, pszWords, NULL, 0xFFFFFF, unAttribute); }
    BOOL	Create	(LPCTSTR pszSpeaker, LPCTSTR pszHearer, LPCTSTR pszWords,
                     LPCTSTR pszEmotion = NULL, DWORD dwColor = 0xffffff,
                     USHORT unAttribute = _TXTATR_NORMAL, USHORT unStyle = _TXT_NORMAL);

public:
    BOOL			Create		(char* pbufMsg, DWORD dwMsgSize);
    void			Process		(void* pInfo);

private:
    typedef struct
    {
        MSGHEAD_DEFINE

        DWORD	dwWordsColor;
        USHORT	unTxtAttribute;
        USHORT	unTxtStyle;
        union
        {
            DWORD	dwTime;
            OBJID	idTransmit;
        };
        char	szBuf[1];
    } MSG_Info;

    MSG_Info*	m_pInfo;

private:
    CNetStringPacker	m_StrPacker;

};

#endif
