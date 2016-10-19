
//**********************************************************
// 代码编辑器
//**********************************************************

// 安全指针模板
// 仙剑修，2001.12.15


#ifndef	T_SAFEPTR
#define T_SAFEPTR

#pragma warning(disable:4786)
#include "common.h"

/*
#define	LogSave	printf		//sys
#define LF	"\n"					//
#define SHOWINC	printf("[+%d+]", m_nCount)		//
#define SHOWDEC printf("[-%d-]", m_nCount)		//
//*/
//*else
//#define	LogSave	printf		//sys
#define LF	// "\n"					
#define SHOWINC	// printf("[+%d+]", m_nCount)		
#define SHOWDEC // printf("[-%d-]", m_nCount)		
//*/


//////////////////////////////////////////////////////////////////////////////////////////////////
template < class T, unsigned long MYID >
class	TSafePtr
{
public:
    TSafePtr() { m_ptr = NULL; m_nCount++; SHOWINC; }
    TSafePtr(T* ptr)
    {
        if (ptr != m_pDefault)	{ m_ptr = ptr; }
        else { m_ptr = NULL; }		//? 无意义。不会为m_pDefault
        m_nCount++;
        SHOWINC;
    }		//? 未检测参数是否有效
    TSafePtr(T& ref)
    {
        if (ref.m_ptr != m_pDefault)	{ m_ptr = ref.m_ptr; }
        else { m_ptr = NULL; }		//? 无意义。不会为m_pDefault
        m_nCount++;
        SHOWINC;
    }
    ~TSafePtr()
    {
        m_nCount--;
        SHOWDEC;
        if (!m_nCount && m_pDefault)
        {
#ifdef	SAFEPTR_COMSTYLE
            m_pDefault->ReleaseByOwner();
#else
            delete m_pDefault;
#endif
            m_pDefault = NULL;
        }
    }
public:
    //	T & operator =(T * ptr) { m_ptr = ptr; }		//? 未检测参数是否有效
    //T* operator T * () { Valid(); return m_ptr; }		//? 为保证NULL指针语义正确，不转换为缺省参数
    bool operator == (T* ptr) { Valid(); return (m_ptr == ptr); }
    T* operator ->() { if (Valid(false)) { return m_ptr; } else { return Default(); } }
private:
    bool	Valid(bool bEnableNull = true);		// 通常允许空指针
    T* 		Default();
protected:
    T* 	m_ptr;
    static T* 	m_pDefault;
    static int	m_nCount;		// TSafePtr对象的个数
};

/////////////////////////////////////////////////////////////////////////////////
template < class T, unsigned long MYID >
T* TSafePtr<T, MYID>::m_pDefault = NULL;

template < class T, unsigned long MYID >
int	TSafePtr<T, MYID>::m_nCount = 0;

/////////////////////////////////////////////////////////////////////////////////
// 检查指针有有效性，并将无效指针(尾随引用)改为NULL。
template < class T, unsigned long MYID >
bool	TSafePtr<T, MYID>::Valid(bool bEnableNull /*= true*/)
{
    if (bEnableNull && !m_ptr)
    {
        return true;
    }
    if (m_ptr)
    {
        try
        {
            unsigned long nID = m_ptr->GetObjType();			//??? 直接使用函数，而非从模板参数中传递。
            if (nID == MYID)
            {
                return true;    // 正常返回
            }
            else
            {
                LOGERROR("ERROR: TSafePointer<%d>::operator->() type dismatch to %d." LF, MYID, nID);
                ASSERT(!"TSafePtr");
            }
        }
        catch(...)
        {
            m_ptr = NULL;
            LOGERROR("CATCH ERROR: TSafePointer<%d>::operator->() can't access object. follow access." LF, MYID);
            ASSERT(!"TSafePtr");
        }
    }
    else
    {
        LOGERROR("ERROR: TSafePointer<%d>::operator->() null pointer access.uninitial pointer." LF, MYID);
        ASSERT(!"TSafePtr");
    }
    return false;
}

template < class T, unsigned long MYID >
T* 		TSafePtr<T, MYID>::Default()
{
    // 出错处理
    try
    {
#ifdef	SAFEPTR_COMSTYLE
        m_pDefault->ReleaseByOwner();
#else
        delete m_pDefault;			//??? 直接使用函数，而非从模板参数中传递。
#endif
    }
    catch(...)
    {
        LOGERROR("CATCH ERROR: TSafePointer<%d>::operator->() can't access default object." LF, MYID);
        ASSERT(!"TSafePtr");
        //?????			m_nCount = 0;
    }
#ifdef	SAFEPTR_COMSTYLE
    m_pDefault = T::CreateNew();
#else
    m_pDefault = new T;
#endif
    return m_pDefault;
}


#endif	// T_SAFEPTR
