
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MagicEffectManager.cpp: implementation of the CMagicEffectManager class.
//
//////////////////////////////////////////////////////////////////////

#include "MagicEffectManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMagicEffectManager::CMagicEffectManager()
{
}

CMagicEffectManager::~CMagicEffectManager()
{
}

//-------------------------------------------------------------------
void CMagicEffectManager::Destroy()
{
    int nAmount = m_setMagicEffect.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMagicEffect* pEffect = m_setMagicEffect[i];
        SAFE_DELETE(pEffect);
    }
    m_setMagicEffect.clear();
}
//-------------------------------------------------------------------
void CMagicEffectManager::Process()
{
    int nAmount = m_setMagicEffect.size();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        CMagicEffect* pEffect = m_setMagicEffect[i];
        if (pEffect && pEffect->Process())
        {
            SAFE_DELETE(pEffect);
            m_setMagicEffect.erase(m_setMagicEffect.begin() + i);
        }
    }
}
//-------------------------------------------------------------------
void CMagicEffectManager::AddMagicEffect(CMagicEffect* pEffect)
{
    if (!pEffect)
    {
        return;
    }
    m_setMagicEffect.push_back(pEffect);
}
//-------------------------------------------------------------------
