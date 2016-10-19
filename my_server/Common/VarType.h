
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// VarType.h: interface for the VarType class.
// paled, 2003.8.5
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARTYPE_H__D9E42F53_A4BD_4BB9_9745_D3FE8666BA41__INCLUDED_)
#define AFX_VARTYPE_H__D9E42F53_A4BD_4BB9_9745_D3FE8666BA41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "basetype.h"
#pragma warning(disable:4786)
#include <string>
using namespace std;

//////////////////////////////////////////////////////////////////////
class VarType
{
public: // type
    enum TYPE { TYPE_NONE, TYPE_INT, TYPE_STRING, TYPE_SYMBOL, TYPE_OBJID };
public: // constructor
    VarType() : m_ptr(NULL), m_int(0), m_nType(TYPE_NONE)		{}
    VarType(const VarType& obj)	: m_int(obj.m_int), m_nType(obj.m_nType)					{ if (obj.m_ptr) { m_ptr = new String(*obj.m_ptr); } else { m_ptr = NULL; } }
    VarType(const char* str, TYPE type = TYPE_STRING)	: m_int(0), m_nType(type)			{ ASSERT(type == TYPE_STRING || type == TYPE_SYMBOL); ASSERT(str); m_ptr = new String(str); }
    VarType(int data, TYPE type = TYPE_INT)			: m_ptr(NULL), m_int(data), m_nType(type)		{ ASSERT(type == TYPE_INT || type == TYPE_OBJID); }
    ~VarType()												{ delete m_ptr; }
    VarType& operator=(const VarType& obj);
    //	VarType& operator=(const char* str)						{ ASSERT(str); if (!m_ptr) m_ptr=new String; *m_ptr=String(str); m_int=0; m_nType=TYPE_STRING; return *this; }
    //	VarType& operator=(int data)							{ if (m_ptr) delete m_ptr; m_ptr=NULL; m_int=data; m_nType=TYPE_INT; return *this; }

public:
    bool	operator==(const VarType& obj) const;
    bool	operator!=(const VarType& obj) const			{ return !(operator==(obj)); }
    //operator String()	const								{ ASSERT(m_ptr); return *m_ptr; }
    operator const String&()	const						{ ASSERT(m_ptr); return *m_ptr; }
    operator int()		const								{ ASSERT(!m_ptr); return m_int; }
    //	operator String()										{ ASSERT(m_ptr); return *m_ptr; }
    //	operator int()											{ ASSERT(!m_ptr); return m_int; }
    void	Clear()											{ delete m_ptr; m_nType = TYPE_NONE; }

public: // is sort
    //	bool	IsSameType(const VarType& obj) const			{ return m_nType == obj.m_nType; }
    bool	IsStringType() const							{ return m_nType == TYPE_STRING; }
    bool	IsIntType() const								{ return m_nType == TYPE_INT; }
    bool	IsSymbolType() const							{ return m_nType == TYPE_SYMBOL; }
    bool	IsOBJIDType() const								{ return m_nType == TYPE_OBJID; }
    bool	IsValid() const									{ return m_nType != TYPE_NONE; }

public: // dump
    String	Dump()	const									{ if (!m_ptr) { return String().FormatLong(m_int); } return *m_ptr; }

protected:
    String*			m_ptr;					// NULL: int type or no init
    int				m_int;
    TYPE			m_nType;				// may be optimize this
};
typedef	const VarType&	VarTypeRef;
template<>
struct dump_traits<VarType> { static String Dump(const VarType& obj)		{ return obj.Dump(); } };

#include "Array.h"
typedef Array<VarType>	VarTypeSet;
typedef	const VarTypeSet&	VarTypeSetRef;
template<>
struct dump_traits<VarTypeSet> { static String Dump(const VarTypeSet& obj)		{ return obj.Dump(); } };

#endif // !defined(AFX_VARTYPE_H__D9E42F53_A4BD_4BB9_9745_D3FE8666BA41__INCLUDED_)
