
//**********************************************************
// 代码编辑器
//**********************************************************

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2DEF01BC_6018_41FB_9A48_4D2196F1C7C1__INCLUDED_)
#define AFX_STDAFX_H__2DEF01BC_6018_41FB_9A48_4D2196F1C7C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define _WINSOCKAPI_		// 阻止加载winsock2.h

//#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

//#pragma comment (lib, "ws2_32.lib")
//#pragma comment (lib, "comctl32.lib")


#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//#ifndef FD_SETSIZE
#define	FD_SETSIZE			600		//??? 当客户端LOGIN连接数很大时，应增加该值
//#endif
#include <winsock2.h>		// MFC socket extensions	2.0版

#define MFC_VER_4
#define INDEX           register int
#define WINDEX          register WORD
#define LINDEX          register long
#define DWINDEX         register DWORD

typedef signed  char    INT8  ;
typedef unsigned char   UINT8 ;
typedef signed  short   INT16 ;
typedef unsigned short  UINT16;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2DEF01BC_6018_41FB_9A48_4D2196F1C7C1__INCLUDED_)
