
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgAnnounceInfo.h: interface for the CMsgAnnounceInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGANNOUNCEINFO_H__B692E49E_1A88_4A84_BB46_E8BAAC61FFC6__INCLUDED_)
#define AFX_MSGANNOUNCEINFO_H__B692E49E_1A88_4A84_BB46_E8BAAC61FFC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "Hero.h"

class CMsgAnnounceInfo : public CNetMsg
{
public:
    CMsgAnnounceInfo();
    virtual ~CMsgAnnounceInfo();
public:
    BOOL QueryMyAnnounceInfo();
    BOOL QueryAnnounceInfo(OBJID idAnnounce);
    BOOL CancelAnnounce(OBJID idAnnounce);
    BOOL Create (char* pMsgBuf, DWORD dwSize);
    BOOL UpAnnounce (const char* szTitle, const char* szDesc);
    void Process (void* pInfo);
protected:
    typedef struct
    {
        unsigned short	unMsgSize;
        unsigned short	unMsgType;
        int				usType;
        OBJID			idAnnounce;
        OBJID			user_id;
        int				level;// 发布公告人的等级
        int				teacher_level;// 导师等级
        int				profession; //导师职业
        char			szBuf[1];//to client 姓名，公告标题，公告内容
    } MSG_Info;

    MSG_Info* m_pInfo;
private:
    CNetStringPacker m_StrPacker;
};

#endif // !defined(AFX_MSGANNOUNCEINFO_H__B692E49E_1A88_4A84_BB46_E8BAAC61FFC6__INCLUDED_)
