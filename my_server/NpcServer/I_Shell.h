
//**********************************************************
// 代码编辑器
//**********************************************************

// NpcWorld.h: interface for the CNpcWorld class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(INTERFACE_HEAD_FILE)
#define INTERFACE_HEAD_FILE


#include "common.h"


#define	CONFIG_INI_FILENAME			"msg_config.ini"

// use in shell
class IDialog
{
public:
    virtual void	PrintText(LPCTSTR szMsg)			PURE_VIRTUAL_FUNCTION
    virtual void	CloseAll()							PURE_VIRTUAL_FUNCTION
    virtual void	SetState(LPCTSTR szState)			PURE_VIRTUAL_FUNCTION
    virtual	void	ChangeEncrypt(DWORD nKey)			PURE_VIRTUAL_FUNCTION
};

// for kernel
class IShell : public IDialog
{
public:
    virtual bool	SendPacket(const char* pack, int nLen, bool bFlush = false)			PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsConnectOK()		PURE_VIRTUAL_FUNCTION_0
    // 没有CloseSocket接口
};


// for shell
class IKernel;
extern IKernel*	CreateNewKernel();

class IKernel
{
public:
    virtual bool	Create(IShell* pShell)					PURE_VIRTUAL_FUNCTION_0
    virtual void	SetDisconnect()							PURE_VIRTUAL_FUNCTION
    virtual ULONG	Release()								PURE_VIRTUAL_FUNCTION_0
    virtual	void	ProcessMsg(OBJID idPacket, const char* pBuf, int nMsgSize, OBJID idNpc)	PURE_VIRTUAL_FUNCTION
    virtual	void	OnTimer(DWORD nCurr)					PURE_VIRTUAL_FUNCTION
    virtual void	ShellCommond(LPCTSTR szCmd)				PURE_VIRTUAL_FUNCTION
};


#endif // !defined(INTERFACE_HEAD_FILE)
