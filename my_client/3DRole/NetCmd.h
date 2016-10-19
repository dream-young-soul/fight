
//**********************************************************
// 代码编辑器
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: NetCmd.h
// Create by: Huang Yuhang
// Create on: 2004/1/14 10:28
//--------------------------------------------------------------------------------------
#ifndef _NETCMD_H
#define _NETCMD_H

#include "BaseFunc.h"

class CNetCmd
{
private:
    enum
    {
        _ID_BEGIN						= 1500000,				// 非法ID	// 客户端起始ID // 不使用
        _ID_SYN_DONATE					= _ID_BEGIN + 1,		// 捐款		// 钱数量
        _ID_SYN_DEMISE					= _ID_BEGIN + 2,		// 禅让		// 帮众名字
        _ID_SYN_UNAPPOINT				= _ID_BEGIN + 3,		// 解职		// 帮众名字
        _ID_SYN_CREATE_SUBSYN			= _ID_BEGIN + 4,		// 创子帮派	// 子帮主id  帮派名字(长度不大于2个汉字)
        _ID_SYN_UPDATE_SUBSYN_LEADER	= _ID_BEGIN + 5,		// 更新子帮派的帮主 // 子帮主id 帮派名字
        _ID_SYN_ANTAGONIZE				= _ID_BEGIN + 6,		// 树敌		// 帮派名字
        _ID_SYN_UNANTAGONIZE			= _ID_BEGIN + 7,		// 取消树敌	// 帮派名字
        _ID_SYN_ALLY					= _ID_BEGIN + 8,		// 结盟		// 帮派名字
        _ID_SYN_UNALLY					= _ID_BEGIN + 9,		// 取消结盟	// 帮派名字
        _ID_SYN_FIRE_MEMBER				= _ID_BEGIN + 10,		// 开除帮众 // 帮众名字
        _ID_SYN_MOVE_MEMBER				= _ID_BEGIN + 11,		// 堂口转移 // 玩家ID 目标堂口名字
        _ID_SYN_BUY_EMBLEM				= _ID_BEGIN + 12,		// 买团标   // 团标编号
    };
private:
    static void SendCmd(int nIndex, const char* fmt, ...);

public:
    static BOOL SynDonate(DWORD dwMoney);
    static BOOL SynDemise(const char* pszName);
    static BOOL SynUnAppoint(const char* pszName);
    static BOOL SynCreateSubsyn(OBJID idSubLeader, const char* pszSynName);
    static BOOL SynUpdateSubsyn(OBJID idSubLeader, const char* pszSynName);
    static BOOL SynAntagonize(const char* pszSynName);
    static BOOL SynUnAntagonize(const char* pszSynName);
    static BOOL SynAlly(const char* pszSynName);
    static BOOL SynUnAlly(const char* pszSynName);
    static BOOL SynFireMember(const char* pszMemberName);
    static BOOL SynMoveMember(const OBJID idMember, const char* pszSubSynName);
    static BOOL SynBuyEmblem(DWORD dwIndex);
};

#endif
