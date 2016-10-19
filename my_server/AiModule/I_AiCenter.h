
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// AiCenter.h: interface for the CAiCenter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERFACE_AICENTER_H__D248B8B7_EB60_4EFE_9A6C_A2BD9E621F95__INCLUDED_)
#define AFX_INTERFACE_AICENTER_H__D248B8B7_EB60_4EFE_9A6C_A2BD9E621F95__INCLUDED_

#include "index.h"
#include "Vartype.h"

typedef	Index<String, VarType>		ARGUMENT_SET;
typedef	const ARGUMENT_SET&			ARGUMENT_SET_REF;
template<>
struct dump_traits<ARGUMENT_SET> { static String Dump(const ARGUMENT_SET& obj) { return obj.Dump(); } };
//////////////////////////////////////////////////////////////////////
class IDatabase;
class IAiCenterOwner
{
protected:
    IAiCenterOwner()			{}
    virtual ~IAiCenterOwner()	{}

public: // const
    virtual unsigned long	GetID()												PURE_VIRTUAL_FUNCTION_0
    virtual bool	CheckCondition(bool bLogicNot, int idxFactFunction, VarTypeSetRef setParam, ARGUMENT_SET* psetArgument)			PURE_VIRTUAL_FUNCTION_0
    virtual int		Priority2Durable(int nPriority)								PURE_VIRTUAL_FUNCTION_0
};

//////////////////////////////////////////////////////////////////////
class IAiCenter
{
protected:
    IAiCenter()				{}
    virtual ~IAiCenter()	{}

public: // static
    static IAiCenter* CreateNew();
    static bool	MatchParam(VarTypeSetRef setFactParam, VarTypeSetRef setRuleParam, ARGUMENT_SET* psetArgument);
    static VarTypeRef	GetValue(VarTypeRef cParam, ARGUMENT_SET* psetArgument);

public: // constuction
    virtual void	Release()													PURE_VIRTUAL_FUNCTION
    virtual bool	Create(IAiCenterOwner* pOwner, IDatabase* pDb, const Array<String>& setGameFunction, const Array<String>& setGameAction, int nTimeOfLife)				PURE_VIRTUAL_FUNCTION_0
    virtual void	OnTimer()													PURE_VIRTUAL_FUNCTION
    virtual void	Persistent(IDatabase* pDb)									PURE_VIRTUAL_FUNCTION

public: // application
    virtual void	AddFact(StringRef strFact, int nPriority)					PURE_VIRTUAL_FUNCTION
    virtual bool	FetchAction(int* pidxAction, VarTypeSet* psetParam)			PURE_VIRTUAL_FUNCTION_0

public: // constant
    virtual int		GetTimeOfLife()	const										PURE_VIRTUAL_FUNCTION_0
    virtual	int		Now()			const										PURE_VIRTUAL_FUNCTION_0
};



#endif // !defined(AFX_INTERFACE_AICENTER_H__D248B8B7_EB60_4EFE_9A6C_A2BD9E621F95__INCLUDED_)
