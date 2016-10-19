
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// FactSet.cpp: implementation of the CFactSet class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "Fact.h"
#include "FactSet.h"
#include "i_mydb.h"
#include "SymbolTable.h"
#include "Syntax.h"

//////////////////////////////////////////////////////////////////////
inline bool IsSymbol(char ch)
{
    if (ch == UNDERLINE_CHAR || isalpha(ch))
    {
        return true;
    }
    return false;
}

inline bool IsNum(char ch)
{
    if (ch == NEGATE_CHAR || isdigit(ch))
    {
        return true;
    }
    return false;
}

inline bool IsIdentChar(char ch)
{
    if (ch == UNDERLINE_CHAR || isalnum(ch))		// || ch == NEGATE_CHAR
    {
        return true;
    }
    return false;
}

inline void IgnoreSpace(const char*& ptr)
{
    // space ------------------------------------
    while(isspace(*ptr))
    {
        ptr++;
    }
}

//////////////////////////////////////////////////////////////////////
bool GetWord(const char*& ptr, char* bufWord)
{
    // space ------------------------------------
    IgnoreSpace(ptr);
    // get string
    char* pBuf = bufWord;
    if (*ptr == STRING_CHAR)
    {
        do
        {
            *(pBuf++)	= *(ptr++);
        }
        while(*ptr && *ptr != STRING_CHAR);
        if (*ptr)
        {
            *(pBuf++)	= *(ptr++);
        }
    }
    else
    {
        if (*ptr == NEGATE_CHAR && isdigit(*(ptr + 1)))
        {
            *(pBuf++)	= *(ptr++);
        }
        while(IsIdentChar(*ptr))
        {
            *(pBuf++)	= *(ptr++);
        }
    }
    *pBuf	= 0;
    return bufWord[0] != 0;
}

//////////////////////////////////////////////////////////////////////
inline void CopyKeyParam(const char*& ptr, char*& bufWord)
{
    while(*ptr && *ptr != KEY_PARAM_CHAR2)
    {
        *(bufWord++)	= *(ptr++);
    }
    if (*ptr)
    {
        *(bufWord++)	= *(ptr++);
    }
}

inline void CopyDataParam(const char*& ptr, char*& bufWord)
{
    while(*ptr && *ptr != DATA_PARAM_CHAR2)
    {
        *(bufWord++)	= *(ptr++);
    }
    if (*ptr)
    {
        *(bufWord++)	= *(ptr++);
    }
}

// don't include leader char (") in source string
inline void CopyString(const char*& ptr, char*& bufWord)
{
    while(*ptr && *ptr != STRING_CHAR)
    {
        *(bufWord++)	= *(ptr++);
    }
    if (*ptr)
    {
        *(bufWord++)	= *(ptr++);
    }
}

//////////////////////////////////////////////////////////////////////
bool GetFact(const char*& ptr, char* bufWord)
{
    // space ------------------------------------
    IgnoreSpace(ptr);
    char* pBuf = bufWord;
    while(*ptr && *ptr != SEPARATOR_CHAR)
    {
        if (*ptr == KEY_PARAM_CHAR)
        {
            *(pBuf++)	= *(ptr++);
            CopyKeyParam(ptr, pBuf);
        }
        else if (*ptr == DATA_PARAM_CHAR)
        {
            *(pBuf++)	= *(ptr++);
            CopyDataParam(ptr, pBuf);
        }
        else if (*ptr == STRING_CHAR)
        {
            *(pBuf++)	= *(ptr++);
            CopyString(ptr, pBuf);
        }
        else
        {
            *(pBuf++)	= *(ptr++);
        }
    }
    if (*ptr)
    {
        ptr++;
    }
    *pBuf	= 0;
    return bufWord[0] != 0;
}

//////////////////////////////////////////////////////////////////////
bool AnalyzeFact(CSymbolTable* pSymbolTable, StringRef strFact,				// in
                 bool* pbLogicNot, OBJID* pidType, VarTypeSet* psetParam, VarTypeSet* pKeyParam/*=NULL*/)	// out
{
    CHECKF(pSymbolTable && pbLogicNot && pidType && psetParam);
    DEBUG_TRY //VVVVVVVVVVVVVVVVVVVVVVVV
    FIELDBUF	buf;
    const char* ptr = strFact;
    // space ------------------------------------
    IgnoreSpace(ptr);
    *pbLogicNot = (*ptr == LOGIC_NOT_CHAR);
    if (*pbLogicNot)
    {
        ptr++;
    }
    // space ------------------------------------
    IgnoreSpace(ptr);
    // fact type string
    IF_NOT(GetWord(ptr, buf) && isalpha(*buf))
    {
        LOGERROR("Syntax error: [%s] near [%s]", strFact, ptr);
        return false;
    }
    *pidType	= pSymbolTable->GetSymbolID(buf);
    // space ------------------------------------
    IgnoreSpace(ptr);
    // key param
    if (*ptr == KEY_PARAM_CHAR)
    {
        ptr++;
        while(*ptr)
        {
            // key param string
            IF_NOT(GetWord(ptr, buf))
            {
                LOGERROR("Syntax error: [%s] near [%s]", strFact, ptr);
                return false;
            }
            VarType	vData;
            if (IsSymbol(*buf))
            {
                vData = VarType(buf, VarType::TYPE_SYMBOL);
            }
            else if (IsNum(*buf))
            {
                vData = VarType(atoi(buf), VarType::TYPE_INT);
            }
            else if (*buf == STRING_CHAR)
            {
                if (strlen(buf) >= 2 && buf[strlen(buf)-1] == STRING_CHAR)
                {
                    buf[strlen(buf)-1]	= 0;
                }
                vData = VarType(buf + 1, VarType::TYPE_STRING);
            }
            else
            {
                LOGERROR("Syntax error: [%s] near [%s]", strFact, ptr);
                return false;
            }
            psetParam->Push(vData);
            if (pKeyParam)
            {
                pKeyParam->Push(vData);
            }
            // space ------------------------------------
            IgnoreSpace(ptr);
            // break while
            if (*ptr == SEPARATOR_CHAR)
            {
                ptr++;
            }
            else
            {
                break;
            }
        } // while
        IF_NOT(*ptr == KEY_PARAM_CHAR2)
        {
            LOGERROR("Syntax error: [%s] near [%s]", strFact, ptr);
            return false;
        }
        if (*ptr == KEY_PARAM_CHAR2)
        {
            ptr++;
        }
    }
    // data param
    if (*ptr == DATA_PARAM_CHAR)
    {
        ptr++;
        while(*ptr)
        {
            // key param string
            IF_NOT(GetWord(ptr, buf))
            {
                LOGERROR("Syntax error: [%s] near [%s]", strFact, ptr);
                return false;
            }
            VarType	vData;
            if (IsSymbol(*buf))
            {
                vData = VarType(buf, VarType::TYPE_SYMBOL);
            }
            else if (IsNum(*buf))
            {
                vData = VarType(atoi(buf), VarType::TYPE_INT);
            }
            else if (*buf == STRING_CHAR)
            {
                if (strlen(buf) >= 2 && buf[strlen(buf)-1] == STRING_CHAR)
                {
                    buf[strlen(buf)-1]	= 0;
                }
                vData = VarType(buf + 1, VarType::TYPE_STRING);
            }
            else
            {
                LOGERROR("Syntax error: [%s] near [%s]", strFact, ptr);
                return false;
            }
            psetParam->Push(vData);
            if (pKeyParam)
            {
                pKeyParam->Push(VarType());
            }
            // space ------------------------------------
            IgnoreSpace(ptr);
            // break while
            if (*ptr == SEPARATOR_CHAR)
            {
                ptr++;
            }
            else
            {
                break;
            }
        } // while
        IF_NOT(*ptr == DATA_PARAM_CHAR2)
        {
            LOGERROR("Syntax error: [%s] near [%s]", strFact, ptr);
            return false;
        }
        if (*ptr == DATA_PARAM_CHAR2)
        {
            ptr++;
        }
    }
    // space ------------------------------------
    IgnoreSpace(ptr);
    IF_NOT(*ptr == 0)
    {
        LOGERROR("Syntax error: [%s] near [%s]", strFact, ptr);
    }
    return *ptr == 0;
    DEBUG_CATCH("AnalyzeFact(1)")	// AAAAAAAAAAAAAAAAAAAAA
    return false;
}

bool AnalyzeFact(CSymbolTable* pSymbolTable, StringRef strFact,		// in
                 CFact* pFact, CFact* pPattern/*=NULL*/)			// out
{
    CHECKF(pSymbolTable && pFact);
    DEBUG_TRY //VVVVVVVVVVVVVVVVVVVVVVVV
    bool		bLogicNot;
    OBJID		idType;
    VarTypeSet	setParam;
    VarTypeSet	setPattern;
    if (AnalyzeFact(pSymbolTable, strFact, &bLogicNot, &idType, &setParam, &setPattern))
    {
        pFact->Create(bLogicNot, idType, setParam);
        if (pPattern)
        {
            pPattern->Create(true, idType, setPattern);
        }
        return true;
    }
    DEBUG_CATCH("AnalyzeFact(2)")	// AAAAAAAAAAAAAAAAAAAAA
    return false;
}

bool AnalyzeFactSet(CSymbolTable* pSymbolTable, StringRef strFact,	// in
                    CFactArray* psetFacts)			// out
{
    CHECKF(pSymbolTable && psetFacts);
    DEBUG_TRY //VVVVVVVVVVVVVVVVVVVVVVVV
    const char*	ptr	= strFact;
    FIELDBUF	buf;
    while(GetFact(ptr, buf))
    {
        bool		bLogicNot;
        OBJID		idType;
        VarTypeSet	setParam;
        ASSERT(setParam.Size() == 0);
        if (AnalyzeFact(pSymbolTable, buf, &bLogicNot, &idType, &setParam, NULL))
        {
            CFact fact;
            if (fact.Create(bLogicNot, idType, setParam))
            {
                psetFacts->Push(fact);
            }
        }
    }
    // space ------------------------------------
    IgnoreSpace(ptr);
    IF_NOT(*ptr == 0)
    {
        LOGERROR("Syntax error: [%s] near [%s]", strFact, ptr);
    }
    return *ptr == 0;
    DEBUG_CATCH("AnalyzeFactSet()")	// AAAAAAAAAAAAAAAAAAAAA
    return false;
}

//////////////////////////////////////////////////////////////////////
bool ReplaceSymbol(CFact* pFact,						// in out
                   ARGUMENT_SET_REF setArgument)		// in
{
    VarTypeSet&	setParam = pFact->GetParamSet();
    VarTypeSet::Iterator pParam = setParam.NewEnum();
    while(pParam.Next())
    {
        if (pParam->IsSymbolType())
        {
            String strSymbol = static_cast<String>(*pParam);
            IF_NOT(setArgument.IsIn(strSymbol))
            return false;
            *pParam	= setArgument[strSymbol];
        }
    }
    return true;
}
