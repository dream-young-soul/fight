
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Register.cpp: implementation of the CRegister class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "Register.h"

// gloable
CRegister	g_objRegister;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegister::CRegister()
{
    m_TxtMsgSet.ClearMsg();
}

CRegister::~CRegister()
{
}
