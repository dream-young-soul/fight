
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// FactSet.h: interface for the CFactSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNTAX_H__C444973E_E282_4312_81CF_A23CFDA0660B__INCLUDED_)
#define AFX_SYNTAX_H__C444973E_E282_4312_81CF_A23CFDA0660B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mycom.h"
#include "T_SingleMap.h"
#include "VarType.h"
#include "I_AiCenter.h"	// Added by ClassView
#include "TickOver.h"
#include "autoptr.h"

//////////////////////////////////////////////////////////////////////
#include "ctype.h"
const char	LOGIC_NOT_CHAR				= '!';
const char	KEY_PARAM_CHAR				= '<';
const char	KEY_PARAM_CHAR2				= '>';
const char	DATA_PARAM_CHAR				= '(';
const char	DATA_PARAM_CHAR2			= ')';
const char	SEPARATOR_CHAR				= ',';
const char	NEGATE_CHAR					= '-';
const char	STRING_CHAR					= '"';
const char	UNDERLINE_CHAR				= '_';

const char	ANY_PARAM_STRING[]			= "_";

//////////////////////////////////////////////////////////////////////

bool AnalyzeFact(CSymbolTable* pSymbolTable, StringRef strMode, CFact* pFact, CFact* pPattern = NULL);
bool AnalyzeFactSet(CSymbolTable* pSymbolTable, StringRef strMode, CFactArray* psetFacts);

bool ReplaceSymbol(CFact* pFact, ARGUMENT_SET_REF setArgument);

#endif // !defined(AFX_SYNTAX_H__C444973E_E282_4312_81CF_A23CFDA0660B__INCLUDED_)
