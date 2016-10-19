
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// ChatRoom.cpp: implementation of the CChatRoom class.
//
//////////////////////////////////////////////////////////////////////

//Create By:	Wen Yiku
//Create Date:	2005-6-29
#include "ChatRoom.h"
#include "GameMsg.h"
#include <time.h>

CChatRoom g_obj91Sdk;
uint32 OnEventNotifyCB( uint32 dwMsgID, uint32 wParam, uint32 lParam );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatRoom::CChatRoom()
{
    strcpy(m_strAccount , "100215");
    strcpy(m_strPasswd , "1");
    strcpy(m_strGameCode , "3");
    m_dwGroupID = 0;
    m_dwAttrib = 0;
    strcpy(m_strValue , "");
    strcpy(m_strGroupDomain , "");
    m_dwUin = 0;
    strcpy(m_strMsg , "");
    m_curGroup = NULL;
    Init();
}


CChatRoom::~CChatRoom()
{
    DeInit();
}

void CChatRoom::Login()
{
#ifdef _DEBUG
    char strTemp[256];
    sprintf(strTemp, "Login Time :%u", ::timeGetTime());
    ::OutputDebugString(strTemp);
#endif
    //	im_Login( m_strAccount, m_strGameCode, m_strPasswd, "", FALSE );
}

void CChatRoom::Logout()
{
    //	im_Logout();
}

void CChatRoom::Init()
{
    //	im_Init( OnEventNotifyCB );
}

void CChatRoom::Reset()
{
    strcpy(m_strAccount , "100215");
    strcpy(m_strPasswd , "1");
    strcpy(m_strGameCode , "3");
    m_dwGroupID = 0;
    m_dwAttrib = 0;
    strcpy(m_strValue , "");
    strcpy(m_strGroupDomain , "");
    m_dwUin = 0;
    strcpy(m_strMsg , "");
    m_curGroup = NULL;
    Init();
}

uint32 OnEventNotifyCB( uint32 dwMsgID, uint32 wParam, uint32 lParam )
{
    switch( dwMsgID )
    {
    case EV_QUITGROUP://当前用户被强制退出群（wParam = IGroupInfo*）
        {
            g_obj91Sdk.Reset();
            ::Post91UCmd(CMD_91U_KICKOUT);
        }
        break;
    case EV_GROUPMEMBERENTRY://群成员进入(wParam = IGroupInfo*, lParam = IGroupMember*)
        {
            g_obj91Sdk.AddRoomMember((IGroupInfo*)wParam, (IGroupMember*)lParam);
            ::Post91UCmd(CMD_91U_MEMBERIN);
        }
        break;
    case EV_GROUPMEMBERKICK:////群成员踢出(wParam = IGroupInfo*, lParam = IGroupMember*)
        {
            g_obj91Sdk.DelRoomMember((IGroupInfo*)wParam, (IGroupMember*)lParam);
            ::Post91UCmd(CMD_91U_MEMBERKICKOUT);
        }
        break;
    case EV_GROUPMEMBERLEAVE://群成员退出(wParam = IGroupInfo*, lParam = IGroupMember*)
        {
            g_obj91Sdk.DelRoomMember((IGroupInfo*)wParam, (IGroupMember*)lParam);
            ::Post91UCmd(CMD_91U_MEMBEROUT);
        }
        break;
    case EV_GROUPENTRYSELF://自己进入群(wParam = IGroupInfo*)
        {
        }
        break;
    case EV_RECVGROUPMSG://接收群消息(wParam = IMsgPacket*)
    case EV_RECVGROUPA2AMSG://接收群私聊消息(wParam = IMsgBody*)
    case EV_RECVGSYSMSG://接收系统消息消息(wParam = IMsgBody*)
        {
            IMsgPacket* pIMsgPacket = (IMsgPacket*)wParam;
            UDATATYPE DataType;
            pIMsgPacket->Get( "dwGroupID", &DataType );
            uint32 dwGroupID = DataType.dwVal;
            pIMsgPacket->Get( _T("dwFriendUin"), &DataType );
            uint32 dwFrdUin = DataType.dwVal;
            string strNick;
            char strTemp[256];
            sprintf(strTemp, "%lu", dwFrdUin);
            strNick = strTemp;
            IGroupInfo* pGroupInfo = im_GetGroupInfo( dwGroupID );
            if( pGroupInfo )
            {
                IGroupMember* pGroupMember = pGroupInfo->MemberFind( dwFrdUin );
                if( pGroupMember )
                {
                    strNick = pGroupMember->GetString( _T("szNick"));
                }
            }
            pIMsgPacket->Get(_T("dwWhen"), &DataType );
            time_t long_time = DataType.dwVal;
            struct tm* newtime;
            newtime = localtime( &long_time );
            TCHAR szTime[50];
            memset( szTime, 0, sizeof( szTime ) );
            _tcsftime( szTime, sizeof( szTime ), _T("%H:%M:%S"), newtime);
            //nick+time
            //std::string strText;
            std::string strText;
            sprintf( strTemp , "%s (%s):\n", strNick, szTime);
            strText = strTemp;
            string strTxt;
            strTxt += pIMsgPacket->GetText();
            strText += strTxt.c_str();
            //			m_strMsg = strText;	//wyk
            //			g_obj91Sdk.SetMsg((char*)strText.c_str());
            //			g_obj91Sdk->m_strMsg=strText;
            //wyk		pMy91uSdkTest->m_strMsg = strText;
            //wyk		pMy91uSdkTest->UpdateData( FALSE );
            g_objGameMsg.AddMsg(strText.c_str());
            //销毁
            pIMsgPacket->Destroy();
        }
        break;
    case EV_MODIFYGROUPREGINFO://修改群信息(wParam = IGroupInfo*, lParam = enum CHATRM_ATTR)
        {
            IGroupInfo* pGroupInfo = (IGroupInfo*)wParam;
            if( pGroupInfo == NULL ) { return 0; }
            uint32 dwType = lParam;
            char strTemp[256];
            switch( dwType )
            {
            case CHATRM_NAME:////名称
                ::Post91UCmd(CMD_91U_MODIFYROOMNAME);
                sprintf(strTemp, "聊天室更名为[%s]", pGroupInfo->GetLong("szGroupName"));
                break;
            case CHATRM_BULLETIN://改公告
                ::Post91UCmd(CMD_91U_MODIFYROOMBULLETIN);
                sprintf(strTemp , "聊天室公告为[%s]", pGroupInfo->GetLong("szGroupBulletin"));
                break;
            case CHATRM_SYNOPSIS://群简介
                ::Post91UCmd(CMD_91U_MODIFYROOMSYNOPSIS);
                sprintf(strTemp , "聊天室简介为[%s]", pGroupInfo->GetLong("szGroupSynopsis"));
                break;
            default:
                return 0;
            }
            MessageBox(NULL, strTemp, "test", MB_OK );
        }
        break;
    case EV_SEARCHGROUP://查找群（wParam = GroupSummary[],lParam=个数）
        {
            int iCount = lParam;
            CGroupSummary* pGroupSummary = (CGroupSummary*)wParam;
            g_obj91Sdk.SetRoomListInfo(pGroupSummary, iCount);
            ::Post91UCmd(CMD_91U_SEARCHROOMBACK);
        }
        break;
    case EV_LOGIN://用户是否登录成功 (wParam:0 成功 其他值：不成功)
        switch( wParam )
        {
        case EVENTMSG_LOGINSUCCESS:
#ifdef _DEBUG
            char strTemp[256];
            sprintf(strTemp, "Login Suc Time :%u", ::timeGetTime());
            ::OutputDebugString(strTemp);
            MessageBox(NULL, "登陆成功！", "test", MB_OK );
#endif
            ::Post91UCmd(CMD_91U_LOGIN);
            g_obj91Sdk.GetRoomList();
            break;
        case EVENTMSG_EXIST_UIN://Uin在别处登录了
            ::Post91UCmd(CMD_91U_EXIST_UIN);
            //			MessageBox(NULL,"Uin在别处登录了！","test",MB_OK );
            break;
        case EVENTMSG_LOGINFAILURE://登录失败
            ::Post91UCmd(CMD_91U_LOGINFAILED);
            MessageBox(NULL, "登录失败！", "test", MB_OK );
            break;
        case EVENTMSG_STATUS_CHANGED://在线状态改变了
            ::Post91UCmd(CMD_91U_STATUSCHANGED);
            MessageBox(NULL, "在线状态改变了！", "test", MB_OK );
            break;
        case EVENTMSG_ABNORMAL_OFFLINE://不正常掉线了
            ::Post91UCmd(CMD_91U_OFFLINEERROR);
            MessageBox(NULL, "不正常掉线了！", "test", MB_OK );
            break;
        case EVENTMSG_OFFLINE://正常下线了
            ::Post91UCmd(CMD_91U_OFFLINE);
            MessageBox(NULL, "正常下线！", "test", MB_OK );
            break;
        }
        break;
    case EV_STATUS_CHANGE://用户状态的变化通知 例如：网络通,未收到KEEPLIVE等。WParam表示当前的状态。如果属于掉线等情况,客户需要重新调用登录接口。该状态另行定义。
        break;
    case EV_GETGROUPINFO://从服务端取群信息（wParam = IGroupInfo*）
        {
            IGroupInfo* pGroupInfo = (IGroupInfo*)wParam;
            if (pGroupInfo != NULL)
            {
                g_obj91Sdk.SetCurRoomInfo(pGroupInfo);
                ::Post91UCmd(CMD_91U_GETROOMINFOBACK);
            }
            else
            {
                return 0;
            }
        }
        break;
    case EV_SERVERREPLY://成员进入群是否成功
        {
            CGroupServerReply* pGroupServerReply = (CGroupServerReply*)lParam;
            if( pGroupServerReply->wAttr != 2 ) { return 0; }//不是聊天室
            if( wParam == TRUE )
            {
                g_obj91Sdk.m_dwGroupID = pGroupServerReply->dwGroupID;
                g_obj91Sdk.GetRoomInfo();
                ::Post91UCmd(CMD_91U_ENTERROOMOK);
                MessageBox(NULL, "进入群成功！", "test", MB_OK );
                return 0;
            }
            switch( pGroupServerReply->dwResult )
            {
            case 0x0021:
                ::Post91UCmd(CMD_91U_ROOMOUTAMOUNT);
                MessageBox(NULL, "超出群允许的最大成员数！", "test", MB_OK );
                break;
            case 0x0022:
                ::Post91UCmd(CMD_91U_HASENTERED);
                MessageBox(NULL, "该成员已经在群的列表之中", "test", MB_OK );
                break;
            case 0x0023:
                ::Post91UCmd(CMD_91U_NOTALLOW);
                MessageBox(NULL, "不允许添加群管理员到群中", "test", MB_OK );
                break;
            case 0x0024:
                ::Post91UCmd(CMD_91U_ENTEROUTTIME);
                MessageBox(NULL, "网络超时", "test", MB_OK );
                break;
            case 0x0020:
            default:
                ::Post91UCmd(CMD_91U_ENTERFAILED);
                MessageBox(NULL, "进入群失败", "test", MB_OK );
                break;
            }
        }
        break;
    case EV_GROUPMEMBERSTATUSMODIFY://成员状态改变(wParam = IGroupInfo*, lParam = IGroupMember*)
        {
            IGroupMember* pGroupMember = (IGroupMember*)lParam;
            if( pGroupMember == NULL ) { return 0; }
            if( pGroupMember->GetLong( "dwStatus") == STATUS_OFFLINE )
            {
                char str[256];
                sprintf(str, "[%s]掉线了", pGroupMember->GetString( "szNick") );
                MessageBox(NULL, str, "test", MB_OK );
                ::Post91UCmd(CMD_91U_MEMBEROFFLINE);
            }
            else
            {
                char str[256];
                sprintf(str, "[%s]在线了", pGroupMember->GetString( "szNick") );
                MessageBox(NULL, str, "test", MB_OK );
                ::Post91UCmd(CMD_91U_MEMBERONLINE);
            }
        }
        break;
    case EV_CHANGEGROUPMANAGER://改变群管理员(wParam = IGroupInfo*)
        {
            IGroupInfo* pGroupInfo = (IGroupInfo*)wParam;
            if( pGroupInfo == NULL ) { return 0; }
            CGroupServerReply* pGroupServerReply = (CGroupServerReply*)lParam;
            if( pGroupServerReply == NULL ) { return 0; }
            g_obj91Sdk.SetCurRoomInfo(pGroupInfo);
            char str[256];
            uint32 dwManager = pGroupInfo->GetLong("dwCreator");
            IGroupMember* pSrcMember = pGroupInfo->MemberFind( pGroupServerReply->dwUin );
            IGroupMember* pDstMember = pGroupInfo->MemberFind( pGroupServerReply->dwDstUin );
            ::Post91UCmd(CMD_91U_CHANGEGROUPMANAGER);
            char strSrc[256], strDst[256];
            switch( pGroupServerReply->dwResult )
            {
            case GroupAbandonManagement_Self://自己放弃
                sprintf(str, "[%s]放弃聊管，[%s]成为聊管", pSrcMember ? pSrcMember->GetString("szNick") : "无", pDstMember ? pDstMember->GetString("szNick") : "无" );
                break;
            case GroupAbandonManagement_System://系统放弃
                if( pSrcMember )
                {
                    sprintf(strSrc, "系统放弃[%s]聊管，", pSrcMember->GetString("szNick") );
                }
                sprintf(strDst, "[%s]成为聊管", pDstMember ? pDstMember->GetString("szNick") : "无");
                sprintf( str, "%s%s", strSrc, strDst);
                break;
            case GroupAbandonManagement_Quit://退出聊天室
                if( pSrcMember )
                {
                    sprintf(strSrc, "[%s]退出聊天室，", pSrcMember->GetString("szNick"));
                }
                else
                {
                    strcpy(strSrc , "聊管退出");
                }
                sprintf(strDst, "[%s]成为聊管", pDstMember ? pDstMember->GetString("szNick") : "无");
                sprintf( str, "%s%s", strSrc, strDst);
                break;
            case GroupAbandonManagement_Apply://请求聊管
                sprintf(str, "[%s]成为聊管", pDstMember ? pDstMember->GetString("szNick") : "无" );
                break;
            case GroupAbandonManagement_AbandonRequest://放弃请求
                if( pSrcMember )
                {
                    sprintf(str, "[%s]放弃聊管请求", pSrcMember ? pSrcMember->GetString("szNick") : "无" );
                }
                break;
            case GroupAbandonManagement_ApplyRequest://申请请求
                if( pSrcMember )
                {
                    sprintf(str, "[%s]申请聊管", pSrcMember ? pSrcMember->GetString("szNick") : "无" );
                }
                break;
            default:
                //				ShowMessageText( "系统有误" );
                MessageBox(NULL, "系统有误", "test", MB_OK );
                return 0;
            }
            MessageBox(NULL, str, "test", MB_OK );
        }
        break;
    case EV_APPLYMANAGER://请求成为管理员(wParam=dwGroupID, lParam=dwUin)
        {
            IGroupInfo* pGroupInfo = (IGroupInfo*)wParam;
            if( pGroupInfo == NULL ) { return 0; }
            uint32 dwUin = lParam;
            IGroupMember* pGroupMember = pGroupInfo->MemberFind( dwUin );
            if( pGroupMember )
            {
                char str[256];
                sprintf(str, "[%s]申请成为聊天室管理员", pGroupMember->GetString( "szNick") );
                MessageBox(NULL, str, "test", MB_OK );
            }
        }
        break;
    case EV_MODIFYGROUPREGMEMBERINFO://修改群成员信息(wParam = IGroupMember*, lParam = enum Enum_MemberChange)
        {
            IGroupMember* pGroupMember = (IGroupMember*)wParam;
            uint32 dwType = lParam;
            //			char str[256];
            char strTemp[256];
            switch( dwType )
            {
            case MemberChange_Rename://改名
                ::Post91UCmd(CMD_91U_ABANDONGROUPMANAGER);
                sprintf(strTemp, "[%lu]修改呢称[%s]", pGroupMember->GetLong("dwUin"), pGroupMember->GetString("szNick") );
                break;
            case MemberChange_Idiograph://玩家签名
                ::Post91UCmd(CMD_91U_ABANDONGROUPMANAGER);
                sprintf(strTemp, "[%lu]修改签名[%s]", pGroupMember->GetLong("dwUin"), pGroupMember->GetString("szIdiograph") );
                break;
            default:
                return 0;
            }
            MessageBox(NULL, strTemp, "test", MB_OK );
        }
        break;
    default:
        break;
    }
    return 0;
}

#ifdef WM_91U_MESSAGE
void Post91UCmd(DWORD dwCommand, int nData)
{
    if(g_hGameWnd)
    {
        ::PostMessage(g_hGameWnd, WM_91U_MESSAGE, dwCommand, nData);
    }
}
#endif

void CChatRoom::DeInit()
{
    //	im_Deinit();
}

void CChatRoom::GetRoomList()
{
    //	im_GetChatRoomLst( 0, 0, atoi( m_strGameCode ));
}

void CChatRoom::SetRoomListInfo(CGroupSummary* pGroupSummary, int iCount)
{
    m_setGroupList.clear();
    if (pGroupSummary == NULL)
    {
        return;
    }
    if (iCount <= 0)
    {
        return;
    }
    for(int i = 0; i < iCount; i++)
    {
        CGroupSummary* pGroup = new CGroupSummary;
        MYASSERT (pGroup);
        *pGroup = pGroupSummary[i];
        m_setGroupList.push_back(pGroup);
    }
}

CGroupSummary* CChatRoom::GetRoomInfoList(int nIndex)
{
    int nAmount = m_setGroupList.size();
    if(nIndex >= 0 && nIndex < nAmount)
    {
        return m_setGroupList[nIndex];
    }
    return NULL;
}

CGroupSummary* CChatRoom::GetRoomInfoListByID(UINT GroupID)
{
    int nAmount = m_setGroupList.size();
    for(int i = 0; i < nAmount; i++)
    {
        CGroupSummary* pGroup = m_setGroupList[i];
        if(pGroup && pGroup->m_dwGroupID == GroupID)
        {
            return pGroup;
        }
    }
    return NULL;
}

void CChatRoom::ClearRoomListInfo()
{
    m_setGroupList.clear();
}

void CChatRoom::GetRoomInfo()
{
    //	im_GetChatRoomInfo( m_dwGroupID, m_strGroupDomain );
}

void CChatRoom::EnterRoom(int nIndex)
{
    CGroupSummary* pGroup = this->GetRoomInfoList(nIndex);
    if (pGroup == NULL)
    {
        return ;
    }
    if (m_dwGroupID == pGroup->m_dwGroupID && m_curGroup && m_curGroup->GetLong("dwGroupID") == pGroup->m_dwGroupID)
    {
        return;    //已经进入了当前聊天室
    }
    //	im_EnterChatRoom( pGroup->m_dwGroupID, pGroup->m_strDomain.c_str() );
}

void CChatRoom::EnterRoomByID(UINT GroupID)
{
    if (GroupID == 0 )
    {
        return;
    }
    if (m_dwGroupID == GroupID && m_curGroup && m_curGroup->GetLong("dwGroupID") == GroupID)
    {
        return;    //已经进入了当前聊天室
    }
    if (this->GetRoomInfoListByID(GroupID))
    {
        //		im_EnterChatRoom( GroupID, this->GetRoomInfoListByID(GroupID)->m_strDomain.c_str() );
    }
}

void CChatRoom::LeaveRoom()
{
    //	im_LeaveChatRoom( m_dwGroupID, m_strGroupDomain );
}

void CChatRoom::KickMember()
{
    //	im_KickMember( m_dwGroupID, m_strGroupDomain, m_dwUin );
}

void CChatRoom::ApplyManager()
{
    //	im_RequestMgrRole( m_dwGroupID, m_strGroupDomain );
}

void CChatRoom::AbandonManger()
{
    //	im_RequestGiveupMgrRole( m_dwGroupID, m_strGroupDomain );
}

void CChatRoom::ChangeMyInfo()
{
    //	im_ChangeMyInfo( m_dwGroupID, m_strGroupDomain, (Enum_MemberChange)m_dwAttrib, m_strValue ); ///(LPTSTR)(LPCTSTR)m_strValue
}

void CChatRoom::ChangeRoomInfo()
{
    //	im_ChangeChatRoomInfo( m_dwGroupID, m_strGroupDomain, (CHATRM_ATTR)m_dwAttrib,m_strValue ); //(LPTSTR)(LPCTSTR)m_strValue
}

void CChatRoom::SendMsg(UINT dwFriendID)
{
    /*	IMsgPacket* pObj;
    	im_CreateMsgPacket( &pObj ) ;

    	pObj->InsertText( m_strMsg );

    //	UDATATYPE DataType;
    	DataType.dwVal = m_dwGroupID;
    	pObj->Set( "dwGroupID", &DataType );

    	DataType.dwVal = 0;
    	pObj->Set( "dwFriendUin", &DataType );

    	DataType.pszVal = m_strGroupDomain; //(LPCTSTR)m_strGroupDomain;
    	pObj->Set( "szDomain", &DataType );

    	DataType.dwVal = ChatMsgType_GroupMsg;
    	pObj->Set( "dwMsgType", &DataType );

    //	im_SendMsgPacket( pObj );*/
}


BOOL CChatRoom::AddRoomMember(IGroupInfo* pGroupInfo, IGroupMember* pGroupMember)
{
    if( pGroupInfo == NULL ) { return false; }
    if (pGroupMember == NULL ) { return false; }
    if (!this->UpdateRoomMember(pGroupMember))
    {
        GroupMember* pMember = new GroupMember;
        MYASSERT (pMember);
        pMember->dwUin = pGroupMember->GetLong("dwUin");
        pMember->dwStatus = pGroupMember->GetLong("dwStatus");
        pMember->dwEntryTime = pGroupMember->GetLong("dwEnterTime");
        strcpy(pMember->szNick, pGroupMember->GetString("szNick"));
        strcpy(pMember->szDomain, pGroupMember->GetString("szDomain"));
        strcpy(pMember->szIdiograph, pGroupMember->GetString("szIdiograph"));
        m_setGroupMember.push_back(pMember);
    }
    return TRUE;
}

BOOL CChatRoom::UpdateRoomMember(IGroupMember* pGroupMember)
{
    if (pGroupMember == NULL ) { return false; }
    GroupMember* pMember = this->GetRoomMemberByUin(pGroupMember->GetLong("dwUin"));
    if (pMember)
    {
        pMember->dwUin = pGroupMember->GetLong("dwUin");
        pMember->dwStatus = pGroupMember->GetLong("dwStatus");
        pMember->dwEntryTime = pGroupMember->GetLong("dwEnterTime");
        strcpy(pMember->szNick, pGroupMember->GetString("szNick"));
        strcpy(pMember->szDomain, pGroupMember->GetString("szDomain"));
        strcpy(pMember->szIdiograph, pGroupMember->GetString("szIdiograph"));
        return TRUE;
    }
    return FALSE;
}
BOOL CChatRoom::DelRoomMember(IGroupInfo* pGroupInfo, IGroupMember* pGroupMember)
{
    if( pGroupInfo == NULL ) { return false; }
    if (m_curGroup->GetLong("dwGroupID") == pGroupInfo->GetLong("dwGroupID"))
    {
        return false;
    }
    if (pGroupMember == NULL ) { return false; }
    int nAmount = m_setGroupMember.size();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        GroupMember* pMember = this->GetRoomMemberByIndex(pGroupMember->GetLong("dwUin"));
        if (pMember)
        {
            pMember = NULL;
            m_setGroupMember.erase(m_setGroupMember.begin() + i);
            return TRUE;
        }
    }
    return false;
}

GroupMember* CChatRoom::GetRoomMemberByIndex(int nIndex)
{
    int nAmount  = m_setGroupMember.size();
    if (nIndex >= 0 && nIndex < nAmount)
    {
        return m_setGroupMember[nIndex];
    }
    return NULL;
}

GroupMember* CChatRoom::GetRoomMemberByUin(int nUin)
{
    int nAmount  = m_setGroupMember.size();
    for(int i = 0; i < nAmount; i++)
    {
        GroupMember* pMember = this->GetRoomMemberByIndex(i);
        if (pMember && pMember->dwUin == nUin)
        {
            return pMember;
        }
    }
    return NULL;
}

void CChatRoom::ClearRoomMembers()
{
    int nAmount = m_setGroupMember.size();
    for(int i = 0; i < nAmount; i++)
    {
        GroupMember* pMember = m_setGroupMember[i];
        pMember = NULL;
        //		SAFE_DELETE(pMember);
    }
    m_setGroupMember.clear();
}
