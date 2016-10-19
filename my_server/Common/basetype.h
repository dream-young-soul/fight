
//**********************************************************
// 代码编辑器
//**********************************************************

// 通用工具
// 仙剑修，2002.9.14
//////////////////////////////////////////////////////////////////////

#ifndef	COMMON_TYPEDEF_HEADER
#define	COMMON_TYPEDEF_HEADER

#include "common.h"
#pragma warning(disable:4786)
#include <string>
using namespace std;

/*/////////////////////////////////////////////////////////////////////
template<class T, int SZ, int D=0>
struct Buffer{
	Buffer()					{ memset(m_buf, D, sizeof(T)*SZ); }
	T& operator[](int idx)		{ IF_OK(idx>=0 && idx<SZ); return m_buf[idx]; return m_buf[SZ]; }
protected:
	T		m_buf[SZ+1];
};*/

//////////////////////////////////////////////////////////////////////
struct true_type {};
struct false_type {};

//////////////////////////////////////////////////////////////////////
class String
{
public:
    String()												{ m_string = string(); }
    String(const char* str)									{ IF_OK(str) m_string = str; }
    String& operator=(const char* str)						{ IF_OK(str) m_string = str; else { m_string = ""; } return *this; }
    operator const char*() const							{ return m_string.c_str(); }
protected:
    String(string& s)										{ m_string = s; }

public:
    string&	QueryString()									{ return m_string; }
    bool	operator<(const String& rhs) const				{ return m_string < rhs.m_string; }
    bool	operator==(const String& rhs) const				{ return m_string == rhs.m_string; }
    String&	operator+=(const String& rhs)					{ m_string += rhs.m_string; return *this; }
    String&	operator+=(char ch) 							{ m_string += ch; return *this; }
    String	operator+(const String& rhs) const				{ return m_string + rhs.m_string; }
    const String&	FormatLong(long nNum)					{ char buf[] = "-2147483648"; m_string = ltoa(nNum, buf, 10); return *this; }
    const String&	FormatDword(DWORD nNum)					{ char buf[] = "-2147483648"; m_string = ultoa(nNum, buf, 10); return *this; }
    long	ConvertLong()									{ return atol(m_string.c_str()); }
    DWORD	ConvertDword()									{ return atol(m_string.c_str()); }

protected:
    string		m_string;
};


//////////////////////////////////////////////////////////////////////
template<class T>
struct dump_traits { static String Dump(const T& obj)		{ return "?"; } };

template<>
struct dump_traits<char> { static String Dump(const char& obj)		{ return String().FormatLong(obj); } };

template<>
struct dump_traits<short> { static String Dump(const short& obj)	{ return String().FormatLong(obj); } };

template<>
struct dump_traits<int> { static String Dump(const int& obj)		{ return String().FormatLong(obj); } };

template<>
struct dump_traits<long> { static String Dump(const long& obj)		{ return String().FormatLong(obj); } };

template<>
struct dump_traits<unsigned char> { static String Dump(const unsigned char& obj)	{ return String().FormatLong(obj); } };

template<>
struct dump_traits<unsigned short> { static String Dump(const unsigned short& obj)	{ return String().FormatLong(obj); } };

template<>
struct dump_traits<unsigned int> { static String Dump(const unsigned int& obj)		{ return String().FormatLong(obj); } };

template<>
struct dump_traits<unsigned long> { static String Dump(const unsigned long& obj)	{ return String().FormatLong(obj); } };

template<>
struct dump_traits<char*> { static String Dump(const char*& obj)		{ return String(obj); } };

template<>
struct dump_traits<const char*> { static String Dump(const const char*& obj)	{ return String(obj); } };

/*
template<>
struct dump_traits<XXX> { static String Dump(const T& obj) { return obj.Dump(); } };
*/
template<class T>
inline	String	DumpString(const T& obj)	{ return dump_traits<T>::Dump(obj); }

//-------------------------------------
typedef	const String&	StringRef;
template<>
struct dump_traits<String> { static String Dump(const String& obj) { return obj; } };


//////////////////////////////////////////////////////////////////////
template<class T>
class POINT_NO_RELEASE : public T
{
    virtual void	Release()			= 0;		// prevent call this function
    virtual void	RleaseByOwner()		= 0;		// prevent call this function
};

//////////////////////////////////////////////////////////////////////
template<int I>
struct auto_long_
{
    auto_long_() : hide_data(I) {}
    auto_long_<I>& operator =(long n)	{ hide_data = n; return *this; }
    operator long() const	{ return hide_data; }

    long	hide_data;
};

typedef	auto_long_<0>	auto_long;

//////////////////////////////////////////////////////////////////////
template<class T>
class hide_type
{
    T	hide_data;
public:
    T&	HideData()		{ return hide_data; }
};


#endif // COMMON_TYPEDEF_HEADER
