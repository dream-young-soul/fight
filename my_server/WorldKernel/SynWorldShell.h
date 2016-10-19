
//**********************************************************
// 代码编辑器
//**********************************************************

// SynShell.h: interface for the CSyndicateWorld class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNSHELL_H__8147FFBF_85B9_43E4_9D3A_B847BF31F088__INCLUDED_2)
#define AFX_SYNSHELL_H__8147FFBF_85B9_43E4_9D3A_B847BF31F088__INCLUDED_2

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "GameData.h"
#include "protocol.h"
#include "AutoPtr.h"
#include "MessageBoard.h"
#include <vector>
#include <string>
using namespace std;

//////////////////////////////////////////////////////////////////////
enum
{
    SYNFUNC_DEMISE = 1,
    SYNFUNC_SETTENET,
    SYNFUNC_SETLEADERTITLE,
    SYNFUNC_SETMEMBERTITLE,
    SYNFUNC_ADDDATA,
    SYNFUNC_SETDATA,
    SYNFUNC_PRESENT,
    SYNFUNC_ADD_MEMBER,
    SYNFUNC_DEL_MEMBER,
    SYNFUNC_UP_LEVEL,
    SYNFUNC_SETSYNNAME,
};

//////////////////////////////////////////////////////////////////////
enum SYNDATA
{
    SYNDATA_ID = 0,			// 仅用于INSERT

    SYNDATA_NAME = 1,			// 帮派名
    SYNDATA_TENET,			// 帮派宗旨
    SYNDATA_ANNOUNCE,		// 帮派公告
    SYNDATA_MEMBERTITLE,	// 帮派成员称号
    SYNDATA_LEADERID,		// 帮主ID
    SYNDATA_LEADERNAME,		// 帮主名字
    SYNDATA_MONEY,			// 帮派基金
    SYNDATA_FEALTY,			// Master ID
    SYNDATA_DEL_FLAG,
    SYNDATA_AMOUNT,			// 帮众数量
    SYNDATA_ENEMY0,			// 敌对帮派
    SYNDATA_ENEMY1,
    SYNDATA_ENEMY2,
    SYNDATA_ENEMY3,
    SYNDATA_ENEMY4,
    SYNDATA_ALLY0,			// 同盟帮派
    SYNDATA_ALLY1,
    SYNDATA_ALLY2,
    SYNDATA_ALLY3,
    SYNDATA_ALLY4,
    SYNDATA_RANK,			// 骑士团等级  1 ~ 5
    SYNDATA_SAINT,			// 圣骑士团    (0: NULL，1：圣)
    SYNDATA_MANTLE_FALG,	// 披风标志    add huang 2004.1.1
    SYNDATA_DISMISS_TIME,	// 自动解散开始时间   //add huang 2004.1.5
    SYNDATA_REPUTE,			// 帮派声望
    SYNDATA_PUBLISHTIME,	// 公告发布时间，格式：yyyymmdd
};
char	szID[];
char	szSynTable[];
typedef	CGameData<SYNDATA, szSynTable, szID>	CSynWorldBase;

//////////////////////////////////////////////////////////////////////
class ISynModify
{
public:
    virtual bool	Demise(OBJID idOldLeader, int nOldSynMemberLevel, OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave)		PURE_VIRTUAL_FUNCTION_0
    virtual bool	SetTenet(LPCTSTR szTenet, bool bSave)													PURE_VIRTUAL_FUNCTION_0
    virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave)										PURE_VIRTUAL_FUNCTION_0
    virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave)										PURE_VIRTUAL_FUNCTION_0
    virtual bool	AddData(SYNDATA idx, int nData, bool bSave)												PURE_VIRTUAL_FUNCTION_0
    virtual bool	SetData(SYNDATA idx, int nData, bool bSave)												PURE_VIRTUAL_FUNCTION_0
    virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave)										PURE_VIRTUAL_FUNCTION_0
    virtual bool	SetSynName(LPCTSTR szSynName, bool bSave = true)											PURE_VIRTUAL_FUNCTION_0
};

//////////////////////////////////////////////////////////////////////
// 所有成员函数为原子操作。
struct CPlayer;
class CSyndicateWorld : ISynModify
{
protected:
    CSyndicateWorld();
    virtual ~CSyndicateWorld();
public:
    static CSyndicateWorld* CreateNew()	{ return new CSyndicateWorld; }
    virtual ULONG	ReleaseByOwner()				{ delete this; return 0; }
    bool	Create(IRecordset* pRes);
    bool	Create(IRecordset* pRes, const CreateSyndicateInfo* pInfo, bool bSave);
    ISynModify*		QueryModify()					{ return (ISynModify*)this; }
    CMessageBoard*	QueryMessageBoard()				{ ASSERT(m_ptrMsgBd); return m_ptrMsgBd; }
    bool			ChangeSyndicate(const SynFuncInfo0* pFuncInfo);

public:
    OBJID	GetID()									{ return m_pData->GetKey(); }
    int		GetInt(SYNDATA idx)						{ return m_pData->GetInt(idx); }
    LPCTSTR	GetStr(SYNDATA idx)						{ return m_pData->GetStr(idx); }

protected: // syn member list --------------------------
    struct	MemberInfo
    {
        MemberInfo(LPCTSTR str, int nUserRank)
        {
            szName	= str;
            nRank	= nUserRank;
        }
        string	szName;
        int		nRank;	//玩家军衔
    };
    typedef	vector<MemberInfo>	MEMBER_SET;
public:
    int		GetSynRankShow(int nRank);
    //	void	SendMemberListSpecifySyn(CPlayer* pUser, int idxLast);
    CSyndicateWorld* GetMasterSyn();
    bool	AddMember(LPCTSTR szName, int nUserRank);
    bool	DelMember(LPCTSTR szName);
    int		GetMemberAmount()				{ return m_setMember.size(); }
    void	SendMemberList(CPlayer* pUser, int idx);
    void	CombineMemberList(const MEMBER_SET& setMember);
    MEMBER_SET&	QueryMemberList()						{ return m_setMember; }
protected:
    int		AppendMember(class CMsgName& msg, int idx, int idxFirst, int& nCount);
    //	int		AppendOnlineMember(class CMsgName& msg, int idxFirst);
    //	int		AppendOnlineMemberForAllMember(class CMsgName& msg, int idxFirst);
    //	int		AppendOfflineMember(class CMsgName& msg, int idxFirst, int nCurrCount=0);
    //	int		AppendOfflineMemberForAllMember(class CMsgName& msg, int idx, int nCount=0);
protected:
    MEMBER_SET	m_setMember;
    //	MEMBER_SET	m_setMemberTmp;

protected: // modify, mast be atom operator. test & set
    // bSave 必须为true，否则不会存盘
    virtual bool	Demise(OBJID idOldLeader, int nOldSynMemberLevel, OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave);
    virtual bool	SetTenet(LPCTSTR szTenet, bool bSave);
    virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave);
    virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave);
    virtual bool	AddData(SYNDATA idx, int nData, bool bSave);
    virtual bool	SetData(SYNDATA idx, int nData, bool bSave);
    virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave);
    virtual bool	SetSynName(LPCTSTR szSynName, bool bSave);

private:
    CSynWorldBase*	m_pData;

protected: // msgbd
    typedef	CAutoPtr<CMessageBoard>	MSGBD_PTR;
    MSGBD_PTR		m_ptrMsgBd;

protected: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_SYNSHELL_H__8147FFBF_85B9_43E4_9D3A_B847BF31F088__INCLUDED_2)
