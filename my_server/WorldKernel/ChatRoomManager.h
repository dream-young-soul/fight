
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// ChatRoomManager.h: interface for the CChatRoomManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATROOMMANAGER_H__70C811B9_1A5A_4573_B970_174A08CFB962__INCLUDED_)
#define AFX_CHATROOMMANAGER_H__70C811B9_1A5A_4573_B970_174A08CFB962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ChatRoom.h"

class CChatRoomManager
{
    typedef std::vector<CChatRoom*>::iterator ITCHATROOM;
private:
    std::vector<CChatRoom*> m_vtChatRoomSet;
    int m_nChatRoomNum;		//聊天室的个数
public:
    CChatRoomManager();
    virtual ~CChatRoomManager();
public:
    void Init();
public:
    static CChatRoomManager* CreateNew();
public:
    CChatRoom* GetRoom(const char* pBuff);
    CChatRoom* GetRoomByPlayer(CPlayer* pPlayer);
    CChatRoom* GetRoom(unsigned short usRoomID);
    void OnTimer(time_t tCurr);
    void Clear();

};

#endif // !defined(AFX_CHATROOMMANAGER_H__70C811B9_1A5A_4573_B970_174A08CFB962__INCLUDED_)
