
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgAnnounceInfo.h: interface for the CMsgAnnounceInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGANNOUNCEINFO_H__A8C4D920_B8F1_4651_A8F2_7E915AFC7F9F__INCLUDED_)
#define AFX_MSGANNOUNCEINFO_H__A8C4D920_B8F1_4651_A8F2_7E915AFC7F9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NetMsg.h"
#include "AnnounceData.h"

enum {_ANNOUNCE_ISSUE = 0,       //发布公告to server
      _ANNOUNCE_CHECK,            //查看公告to client
      _ANNOUNCE_SLEF_CHECK,       //查看自己的公告to client
      _ANNOUNCE_USER_INFO,        //to server/client在发布时，客户端要用到的信息。 返回客户端的:等级，
      //，导师等级   信息结构中char	szBuf[1]的内容为：姓名，职业
      _ANNOUNCE_CANCEL,           //取消公告
     };

class CMsgAnnounceInfo : public CNetMsg
{
public:
    CMsgAnnounceInfo();
    virtual ~CMsgAnnounceInfo();
    BOOL            Create      (CAnnounceData* pData, int type);
    BOOL            Create      (int level, int teacher_level, const char* name, int profession, int type);
public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);
protected:
    typedef struct
    {
        MSGHEAD_DEFINE
        int usType;
        OBJID	idAnnounce;
        OBJID   user_id;
        int     level;
        int     teacher_level;
        int     profession;        //导师等级
        char	szBuf[1];
    } MSG_Info;

    MSG_Info*	m_pInfo;
private:
    CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGANNOUNCEINFO_H__A8C4D920_B8F1_4651_A8F2_7E915AFC7F9F__INCLUDED_)
