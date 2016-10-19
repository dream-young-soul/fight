
//**********************************************************
// 代码编辑器
//**********************************************************

// 类COM支持
// paled, 2003.5.25

#ifndef	MY_COM_H
#define	MY_COM_H

#include "common.h"

////////////////////////////////////////////////////////////////
#define	NO_COPY(T)	\
protected:	\
	T(const T&) {}	\
	T& operator=(const T&)	{}


////////////////////////////////////////////////////////////////
#define	NEW_RELEASE(T)	\
public:	\
	static T* CreateNew() 	{ return new T; }	\
	void Release() 		{ delete this; }	\
	void ReleaseByOwner() 		{ delete this; }	\
protected:


////////////////////////////////////////////////////////////////
#define	COM_STYLE(T)	\
	NEW_RELEASE(T)	\
	NO_COPY(T)	\
protected:


////////////////////////////////////////////////////////////////
#define	COM_STYLE_HEAP(T)	\
	COM_STYLE(T)	\
protected:	\
	T() {}	\
	virtual ~T() {}	\
protected:


////////////////////////////////////////////////////////////////
#define	COM_STYLE_STACK(T)	\
	COM_STYLE(T)	\
public:	\
	T() {}	\
	virtual ~T() {}	\
protected:


////////////////////////////////////////////////////////////////
// typedef
//////////////////////////////////////////////////////////////////////
#define	TYPE_OF(x)			((x**)0)

//////////////////////////////////////////////////////////////////////
template<class T>
struct guid_traits {};
#define GUID_DECLARE(T,i)	template<> struct guid_traits< T > { enum{ id=(i) }; };
#define	GUID_OF(T)			(guid_traits< T >::id)

//////////////////////////////////////////////////////////////////////
#define DEFINE_QI(T,v)		T* QueryInterface(T** ppT) { ASSERT(v); if (ppT) *ppT = v; return v; }

//////////////////////////////////////////////////////////////////////
template<class T, class X>
inline T* Cast(X* pX) { if (pX) { return pX->QueryInterface((T**)0); } return NULL; }

template<class T, class X>
inline bool Cast(T** ppT, X* pX) { CHECKF(ppT); if (pX) { return pX->QueryInterface(ppT) != NULL; } return *ppT = NULL, false; }

//////////////////////////////////////////////////////////////////////
// 对象组合
// paled, 2004.1.17
//////////////////////////////////////////////////////////////////////

// --------------- SUB OBJ ---------------//
// 会修改PUBLIC
#define	PUBLIC_OBJ(T,V)	\
	public:		T*	Query##V()		{ return hide_p##V; }	\
	protected:	CAutoPtr< T >		hide_p##V;
#define	INCLUDE_OBJ(T,V)	\
	protected:	T*	Query##V()		{ return hide_p##V; }	\
	protected:	CAutoPtr< T >		hide_p##V;
#define	INCLUDE_OBJ_SIMPLE(T,V)	\
	protected:	T*	Query##V()		{ return &hide_c##V; }	\
	protected:	T					hide_c##V;

// 允许重复调用CREATE_OBJ
#define CREATE_OBJ(T,V)			\
	(hide_p##V ? hide_p##V.pop()->Release() : 0),	\
	(hide_p##V = T::CreateNew()),	\
	hide_p##V->Create


// --------------- SUB OBJ SET ---------------//
// 会修改PUBLIC
#define	PUBLIC_OBJSET(S,V,T,V1)	\
	public:		S*	Query##V()			{ CHECKF(hide_p##V); return hide_p##V; }	\
	public:		T*	Query##V1(OBJID id)	{ CHECKF(hide_p##V); return hide_p##V->GetObj(id); }	\
	protected:	CAutoPtr< S >			hide_p##V;
#define	INCLUDE_OBJSET(S,V,T,V1)	\
	protected:	S*	Query##V()			{ CHECKF(hide_p##V); return hide_p##V; }	\
	protected:	T*	Query##V1(OBJID id)	{ CHECKF(hide_p##V); return hide_p##V->GetObj(id); }	\
	protected:	CAutoPtr< S >			hide_p##V;
#define	INCLUDE_OBJSET_SIMPLE(S,V,T,V1)	\
	protected:	S*	Query##V()			{ return &hide_c##V; }	\
	protected:	T&	Query##V1(OBJID id)	{ return hide_c##V[id]; }	\
	protected:	S						hide_c##V;

#define CREATENEW_OBJSET(S,V)	\
	(ASSERT(!hide_p##V)),	\
	(hide_p##V = S::CreateNew())
#define CREATE_OBJSET(S,V)	\
	(ASSERT(!hide_p##V)),	\
	(hide_p##V = S::CreateNew()),	\
	hide_p##V->Create

// --------------- AUTOLINK ---------------//
// 会修改PUBLIC
#define	PUBLIC_LINK(T,V)	\
	public:		T*	Query##V()		{ return hide_p##V; }	\
	protected:	CAutoLink< T >		hide_p##V;
#define	AUTOLINK_OBJ(T,V)	\
	protected:	T*	Query##V()		{ return hide_p##V; }	\
	protected:	CAutoLink< T >		hide_p##V;


// 允许重复调用INIT_OBJ
#define INIT_OBJ(V,D)	\
	(hide_p##V = (D))

// --------------- REFERENCE ---------------//
// 会修改PUBLIC
#define	PUBLIC_REFERENCE(T,V)	\
	public:		T*	Query##V()		{ return hide_p##V; }	\
	protected:	T*					hide_p##V;
#define	REFERENCE_OBJ(T,V)	\
	protected:	T*	Query##V()		{ return hide_p##V; }	\
	protected:	T*					hide_p##V;


// --------------- INT ---------------//
// 会修改PUBLIC
#define	PUBLIC_INT(V)	\
	public:		int		Get##V()		{ return hide_p##V; }	\
	protected:	void	Set##V(int n)	{ hide_p##V=n; }	\
	protected:	auto_long				hide_p##V;
#define	DEFINE_INT(V)	\
	protected:	int		Get##V()		{ return hide_p##V; }	\
	protected:	void	Set##V(int n)	{ hide_p##V=n; }	\
	protected:	auto_long				hide_p##V;
#define	PUBLIC_SET_INT(V)	\
	public:		int		Get##V()		{ return hide_p##V; }	\
	public:		void	Set##V(int n)	{ hide_p##V=n; }	\
	protected:	auto_long				hide_p##V;


#endif // MY_COM_H
