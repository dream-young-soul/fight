
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef _MSGPACKET_H
#define _MSGPACKET_H
//-------------------------------------------------------

const UINT _MAX_PACKETSIZE	= 1024;

class CMsgPacket
{
//----------------------------------------------------------------
public:
    static HANDLE	s_hHeap;
    static UINT		s_uNumAllocsInHeap;

    static BOOL		IsValidPt(void* p);

//----------------------------------------------------------------
public:
    void*	operator new		(size_t size);
    void	operator delete		(void* p);

//----------------------------------------------------------------
public:
    CMsgPacket(void);
    virtual ~CMsgPacket(void) {}

public:
    BOOL    Create      (void* pbuf, int nSize);
    int		AddContent	(void* pbuf, int nSize);
    void	Reset		(void);

    int     GetSizeofSize   (void)  {return sizeof(m_unMsgSize);}

    char*	GetBuf		(void)	{return m_bufMsg;}
    int		GetSize		(void)	{return m_unMsgSize;}
    int		GetType		(void)	{return m_unMsgType;}

    void	Dump		(void);
public:
    DWORD   m_dwAnyData;

public:
    int		m_nCurSize;
    union
    {
        char	m_bufMsg[_MAX_PACKETSIZE];
        struct
        {
            unsigned short	m_unMsgSize;
            unsigned short	m_unMsgType;
        };
    };
};

#include <deque>
using namespace std;
typedef deque<CMsgPacket* >  DEQUE_MSGPACKET;

const UINT _MAX_CACHESIZE	= 128;
//-------------------------------------------------------
class CMsgPacketCache
{
public:
    CMsgPacketCache();
    virtual ~CMsgPacketCache();

    BOOL		AddPacket	(const void* pbuf, DWORD dwSize);

    CMsgPacket*	GetFirstPacket	(void);
    void		DelFirstPacket  (void);

    void		SetInterval	(DWORD dwInterval)	{m_dwTimeInterval = dwInterval;}

    int			GetPacketAmount	(void)	{return m_dequePacketToSend.size();}

private:
    DEQUE_MSGPACKET	m_dequePacketToSend;

    DWORD			m_dwTimeLastSend;
    DWORD			m_dwTimeInterval;
};


//-------------------------------------------------------
#endif

