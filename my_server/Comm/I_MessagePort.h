
//**********************************************************
// 代码编辑器
//**********************************************************

// 接口类基类，用于派生接口类。接口：基于消息通讯机制的线程间通讯接口。
// 仙剑修，2002.8.28

#ifndef	I_MESSAGEPORT_H
#define I_MESSAGEPORT_H

#include <windows.h>
#include "common.h"
#include "LogFile.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
// 外部定义
//#define	LOGERROR	LogError
//void	LogError(const char* szFormat, ...);
/////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
// 用于隐藏内部名空间
#define MSGPORT_BEGIN	namespace message_port {
#define MSGPORT_END		};


/////////////////////////////////////////////////////////////////////////////////////////////////
#define	MAX_PORT_ID			59999
#define	MAX_PACKET_ID		59999
#define	MAX_MSG_TYPE		59999

/////////////////////////////////////////////////////////////////////////////////////////////////
enum VAR_TYPE
{
    VARTYPE_NONE = MAX_MSG_TYPE + 1,			// 没有一个结构尺寸会大于这个值，小于该值表示结构
    VARTYPE_CHAR, VARTYPE_UCHAR, VARTYPE_SHORT, VARTYPE_USHORT,
    VARTYPE_LONG, VARTYPE_ULONG, VARTYPE_INT, VARTYPE_UINT,
    VARTYPE_FLOAT, VARTYPE_DOUBLE
};
#define	STRUCT_TYPE(x)							((VAR_TYPE)sizeof(x))
#define	STRING_TYPE(x)							((VAR_TYPE)(strlen(x)+1))
#define	BUFFER_TYPE(x)							((VAR_TYPE)(x))

/////////////////////////////////////////////////////////////////////////////////////////////////
const int	PORT_ANY		= -1;				// 正常PORT范围，0 - XXX
const int	INVALID_PORT	= MAX_PORT_ID + 1;	// 正常PORT，不会发送
const int	PACKET_ANY		= -1;				// 正常PACKET范围，1 - XXX

enum {	STATUS_FLAG_OK			= 0,				// 没有错误
        STATUS_FLAG_CLOSE,							// 连接正常断开，对方对象不接收消息。
        STATUS_FLAG_ERROR,							// 连接发生错误。溢出？
        STATUS_FLAG_FATAL,							// 致命错误。
     };

/////////////////////////////////////////////////////////////////////////////////////////////////
struct	CMessageStatus
{
    int		m_nPortFrom;
    int		m_nPacket;
    VAR_TYPE		m_nVarType;
    int 	m_nError;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
class	IMessagePort
{
public:
    IMessagePort			() {}
    virtual ~IMessagePort	() {}

    virtual bool	IsOpen	() PURE_VIRTUAL_FUNCTION_0;

public:
    // 取本接口的ID号
    virtual int		GetID	() PURE_VIRTUAL_FUNCTION_0;
    // 取接口数量
    virtual int		GetSize	() PURE_VIRTUAL_FUNCTION_0;

    // 初始化，设置接口ID号，开始接收消息。可重复调用(PORT_ID不能改变)。
    virtual bool	Open	() PURE_VIRTUAL_FUNCTION_0;
    // 关闭接口，不再接收消息。可重复调用。
    virtual bool	Close	() PURE_VIRTUAL_FUNCTION_0;

    // 发送消息到指定接口。包含消息ID、数据类型、数据。return false: 发送不成功，或已经关闭
    virtual bool	Send	(int nPort, int nPacket, VAR_TYPE nVarType, const void* buf) PURE_VIRTUAL_FUNCTION_0;
    // 接收指定接口(或所有接口)发来的消息。可指定消息ID，也可不指定。return false: 没有收到数据
    virtual bool	Recv	(int nPort, int nPacket, VAR_TYPE nVarType, void* buf, CMessageStatus* pStatus) PURE_VIRTUAL_FUNCTION_0;

    // 检查消息栈中有没有消息，如没有消息则等待。return false: 超时或错误。
    virtual bool	WaitMsg	(int nMilliSec = INFINITE) PURE_VIRTUAL_FUNCTION_0;
};


#endif // I_MESSAGEPORT_H

/////////////////////////////////////////////////////////////////////////////////////////////////
// 限制：
//		port		0-59999
//		packet		1-59999
//		msg_len		0-59999












