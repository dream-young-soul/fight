
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// ChatRoomManager.cpp: implementation of the CChatRoomManager class.
//
//////////////////////////////////////////////////////////////////////

#include "ChatRoomManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatRoomManager g_objChatRoomManager;

CChatRoomManager::CChatRoomManager()
{
}

CChatRoomManager::~CChatRoomManager()
{
    Clear();
}

CChatRoomManager* CChatRoomManager::CreateNew()
{
    return new CChatRoomManager;
}

void CChatRoomManager::Init()
{
    m_nChatRoomNum = 5;		//案子中规定聊天室为5个
    CChatRoom* pChatRoom = NULL;
    char szName[MAX_PATH];
    for(int i = 0; i < m_nChatRoomNum; ++i)
    {
        pChatRoom = CChatRoom::CreateNew();
        CHECK(pChatRoom);
        //初始化聊天室属性
        pChatRoom->SetID(i + 1);
        sprintf(szName, STR_CHATROOM_NAME, i + 1);
        pChatRoom->SetName(szName);
        m_vtChatRoomSet.push_back(pChatRoom);
    }
}

void CChatRoomManager::Clear()
{
    CChatRoom* pChatRoom = NULL;
    for(ITCHATROOM itChatRoom = m_vtChatRoomSet.begin(); itChatRoom != m_vtChatRoomSet.end(); ++itChatRoom)
    {
        pChatRoom = (*itChatRoom);
        SAFE_RELEASE(pChatRoom);
    }
    m_vtChatRoomSet.clear();
}

void CChatRoomManager::OnTimer(time_t tCurr)
{
    CChatRoom* pChatRoom = NULL;
    for(ITCHATROOM itChatRoom = m_vtChatRoomSet.begin(); itChatRoom != m_vtChatRoomSet.end(); ++itChatRoom)
    {
        pChatRoom = (*itChatRoom);
        CHECK(pChatRoom);
        pChatRoom->OnTimer(tCurr);
    }
}

CChatRoom* CChatRoomManager::GetRoom(unsigned short usRoomID)
{
    CChatRoom* pChatRoom = NULL;
    for(ITCHATROOM itChatRoom = m_vtChatRoomSet.begin(); itChatRoom != m_vtChatRoomSet.end(); ++itChatRoom)
    {
        pChatRoom = (*itChatRoom);
        CHECKF(pChatRoom);
        if(pChatRoom->GetRoomID() == usRoomID)
        {
            return pChatRoom;
        }
    }
    return NULL;
}

CChatRoom* CChatRoomManager::GetRoomByPlayer(CPlayer* pPlayer)
{
    CChatRoom* pChatRoom = NULL;
    for(ITCHATROOM itChatRoom = m_vtChatRoomSet.begin(); itChatRoom != m_vtChatRoomSet.end(); ++itChatRoom)
    {
        pChatRoom = (*itChatRoom);
        CHECKF(pChatRoom);
        if(pChatRoom->IsExist(pPlayer))
        {
            return pChatRoom;
        }
    }
    return NULL;
}

CChatRoom* CChatRoomManager::GetRoom(const char* pBuff)
{
    CChatRoom* pChatRoom = NULL;
    for(ITCHATROOM itChatRoom = m_vtChatRoomSet.begin(); itChatRoom != m_vtChatRoomSet.end(); ++itChatRoom)
    {
        pChatRoom = (*itChatRoom);
        CHECKF(pChatRoom);
        if(strcmp(pChatRoom->GetRoomName(), pBuff) == 0)
        {
            return pChatRoom;
        }
    }
    return NULL;
}
