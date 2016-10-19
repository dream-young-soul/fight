
//**********************************************************
// 代码编辑器
//**********************************************************

// SymbolTable.cpp: implementation of the CSymbolTable class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "SymbolTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSymbolTable::CSymbolTable()
{
}

CSymbolTable::~CSymbolTable()
{
}

bool CSymbolTable::AddSymbol(StringRef strSymbol)
{
    if (m_setSymbol.IsIn(strSymbol))								//??? 最好用无符号比较
    {
        return false;
    }
    OBJID	id = SpawnSymbolID();
    LOGDUMP("Add Symbol: %u, %s", id, (LPCTSTR)strSymbol);
    return m_setSymbol.Add(strSymbol, id);
}

LPCTSTR CSymbolTable::FindSymbolByID(OBJID id)
{
    for(SYMBOL_SET::Iterator iter = m_setSymbol.Begin(); iter != m_setSymbol.End(); iter++)
    {
        if (m_setSymbol.Data(iter) == id)
        {
            return m_setSymbol.Key(iter);
        }
    }
    return NULL;
}

bool CSymbolTable::Create(OBJID idSymbolBegin)
{
    m_idNext	= idSymbolBegin;
    return true;
}
