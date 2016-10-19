
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Friend.cpp: implementation of the CFriend class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "Friend.h"
#include "User.h"

MYHEAP_IMPLEMENTATION(CFriend, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFriend::CFriend()
{
}

//////////////////////////////////////////////////////////////////////
CFriend::~CFriend()
{
}

//////////////////////////////////////////////////////////////////////
CFriend* CFriend::CreateNewFriend(const FriendInfoStruct* pInfo)
{
    CHECKF(pInfo);
    CFriendPtr	pFriend = CFriend::CreateNew();
    if (pFriend)
    {
        memcpy(&pFriend->m_Info, pInfo, sizeof(FriendInfoStruct));
    }
    return pFriend;
}






