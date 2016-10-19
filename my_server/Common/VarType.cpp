
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// VarType.cpp: implementation of the VarType class.
//
//////////////////////////////////////////////////////////////////////

#include "VarType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VarType& VarType::operator=(const VarType& obj)
{
    if (this == &obj)
    {
        return *this;
    }
    if (obj.m_ptr)
    {
        if (!m_ptr)
        {
            m_ptr = new String;
        }
        *m_ptr = *obj.m_ptr;
    }
    else
    {
        if (m_ptr)
        {
            delete m_ptr;
            m_ptr = NULL;
        }
    }
    m_int		= obj.m_int;
    m_nType		= obj.m_nType;
    return *this;
}

bool VarType::operator==(const VarType& obj) const
{
    if (!IsValid() || !obj.IsValid())
    {
        return false;
    }
    if (IsStringType())
    {
        if (obj.IsStringType() && *m_ptr == *obj.m_ptr)
        {
            return true;
        }
    }
    else if (IsIntType())
    {
        if (obj.IsIntType() && m_int == obj.m_int)
        {
            return true;
        }
    }
    return false;
}
