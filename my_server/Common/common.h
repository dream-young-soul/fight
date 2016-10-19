
//**********************************************************
// 代码编辑器
//**********************************************************

// 通用工具
// 仙剑修，2002.9.14
//////////////////////////////////////////////////////////////////////

#ifndef	COMMON_H
#define	COMMON_H

#include <windows.h>
#ifdef	_DEBUG
#include <vector>
#include <utility>
void* _cdecl operator new(size_t nSize, LPCSTR lpszFileName, int nLine);
void _cdecl operator delete(void* p, LPCSTR lpszFileName, int nLine);
#define new new(__FILE__,__LINE__)
#endif // _DEBUG
#include <time.h>
#include "logfile.h"

/////////////////////////////////////////////////////////////////////////////
// function
/////////////////////////////////////////////////////////////////////////////
bool SafeCopy(char* pTarget, const char* pSource, int nBufLen = 0);
#define SAFE_DELETE(ptr) { if (ptr){	try{ delete ptr; }catch(...){ LOGERROR("CATCH: *** SAFE_DELETE() crash! ***\n"); } ptr = 0; } }
#define SAFE_RELEASE(ptr) { if (ptr){ try{ ptr->Release(); }catch(...){ LOGERROR("CATCH: *** SAFE_RELEASE() crash! ***\n"); } ptr = 0; } }
#define SAFE_RELEASEBYOWNER(ptr) { if (ptr){ try{ ptr->ReleaseByOwner(); }catch(...){ LOGERROR("CATCH: *** SAFE_RELEASE() crash! ***\n"); } ptr = 0; } }

//////////////////////////////////////////////////////////////////////
// 其它
#include <assert.h>
#include "logfile.h"

inline void	log_assert(const char* type, const char* str, const char* file, int line) { LOGERROR("★%s(%s)★ in %s, %d", type, str, file, line); }

#ifdef	LOCAL_DEBUG
#include <assert.h>
#undef	ASSERT
#define		ASSERT(x)	(void)( (x) || (assert(!"(#x)"),log_assert("ASSERT", #x, __FILE__, __LINE__), 0) )
#define		CHECK(x)	do{ if (!(x)) { assert(!(#x)),log_assert("CHECK", #x, __FILE__, __LINE__); return; } }while(0)
#define		CHECKF(x)	do{ if (!(x)) { assert(!(#x)),log_assert("CHECKF", #x, __FILE__, __LINE__); return 0; } }while(0)
#define		IF_NOT(x)	if ( !(x) ? ( assert(!(#x)),log_assert("IF_NOT", #x, __FILE__, __LINE__),1 ) : 0 )
#define		IF_NOT_(x)	if ( !(x) ? ( assert(!"(#x)"),log_assert("IF_NOT_", #x, __FILE__, __LINE__),1 ) : 0 )
#define		IF_OK(x)	if ( (x) ? 1 : ( assert(!(#x)),log_assert("IF_OK", #x, __FILE__, __LINE__),0 ) )
#define		IF_OK_(x)	if ( (x) ? 1 : ( assert(!"(#x)"),log_assert("IF_OK_", #x, __FILE__, __LINE__),0 ) )
#define		PURE_VIRTUAL_FUNCTION_0		= 0;
#define		PURE_VIRTUAL_FUNCTION		= 0;
#define		PURE_VIRTUAL_FUNCTION_(x)	= 0;
#else // LOCAL_DEBUG
#undef	ASSERT
#define		ASSERT(x)	(void)( (x) || (log_assert("ASSERT", #x, __FILE__, __LINE__), 0) )
#define		CHECK(x)	do{ if (!(x)) { log_assert("CHECK", #x, __FILE__, __LINE__); return; } }while(0)
#define		CHECKF(x)	do{ if (!(x)) { log_assert("CHECKF", #x, __FILE__, __LINE__); return 0; } }while(0)
#define		IF_NOT(x)	if ( !(x) ? ( log_assert("IF_NOT", #x, __FILE__, __LINE__),1 ) : 0 )
#define		IF_NOT_(x)	if ( !(x) ? ( log_assert("IF_NOT_", #x, __FILE__, __LINE__),1 ) : 0 )
#define		IF_OK(x)	if ( (x) ? 1 : ( log_assert("IF_OK", #x, __FILE__, __LINE__),0 ) )
#define		IF_OK_(x)	if ( (x) ? 1 : ( log_assert("IF_OK_", #x, __FILE__, __LINE__),0 ) )
#define		PURE_VIRTUAL_FUNCTION_0		{ ASSERT(!"PURE_VIRTUAL_FUNCTION_0"); return 0; }
#define		PURE_VIRTUAL_FUNCTION		{ ASSERT(!"PURE_VIRTUAL_FUNCTION"); }
#define		PURE_VIRTUAL_FUNCTION_(x)	{ ASSERT(!"PURE_VIRTUAL_FUNCTION"); return x(); }
#endif // LOCAL_DEBUG


void		LogSave		(const char* fmt, ...);
#define		DEBUG_TRY		try{
#define		DEBUG_CATCH(x)	}catch(...){ ::LogSave("CATCH: *** " x " crash()! ***\n"); }
#define		DEBUG_CATCH2(x,y)	}catch(...){ ::LogSave((x),(y)); ::LogSave("CATCH: *** " x " crash()! ***\n"); }
#define		DEBUG_CATCH3(x,y,z)	}catch(...){ ::LogSave((x),(y),(z)); ::LogSave("CATCH: *** " x " crash()! ***\n"); }

#define		LOCKTHREAD		CSingleLock xLock(&m_xCtrl, true)

// int xCount = 0;
// while(...) { ...; DEAD_LOOP_BREAK(xCount, 10000) }
#define		DEAD_LOOP_BREAK(x,n)	{ if (++(x) > (n)){ ASSERT(!"DEAD_LOCK_BREAK"); break; } }

//#define		class	class

//////////////////////////////////////////////////////////////////////
template<class T>
inline void		safe_release(T*& pT)			{ pT->Release(); pT = NULL; }
#define			SafeRelease			safe_release


#endif // COMMON_H
