
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#include "skylayer.h"
#include "skyrose.h"
#include "patternrose.h"


void CSkyLayer::AddObj(CMapObj* obj)
{
    m_dequeMapObj.push_back(obj);
}

CSkyLayer* CSkyLayer::CreateNew()
{
    CSkyLayer* p = new CSkyLayer();
    if (p)
    {
        return p;
    }
    else
    {
        return NULL;
    }
}

void CSkyLayer::Process(void* pInfo)
{
    for(int i = 0; i < (int)m_dequeMapObj.size(); i++)
    {
        CMapObj* obj =	m_dequeMapObj[i];
        if (obj->GetObjType() == MAP_SCENE)
        {
            CSkyRose* p = (CSkyRose*)obj;
            if (p->GetStatus() == PATTERNROSE_DESTROY)
            {
                SafeDelete(p);
                m_dequeMapObj.erase(m_dequeMapObj.begin() + i);
                i--;
            }
            else
            {
                obj->Process(pInfo);
            }
        }
        else
        {
            obj->Process(pInfo);
        }
    }
}

void CSkyLayer::Show(CMyPos posShow)
{
    m_objfirework.Process() ;
    m_objCharFirework.Process();
    m_objWeather.EnvShow();
    m_objAirBubble.Show();
    int nSize = m_dequeMapObj.size();
    for(int i = 0; i < nSize; i++)
    {
        CMapObj* obj =	m_dequeMapObj[i];
        obj->Show(NULL);
    }
}

void CSkyLayer::Destroy()
{
    m_objfirework.Destroy();
    m_objCharFirework.Destroy();
    m_objWeather.Destroy();
    m_objAirBubble.Destroy();
    int nSize = m_dequeMapObj.size();
    for(int i = 0; i < nSize; i++)
    {
        SafeDelete(	m_dequeMapObj[i]);
    }
    m_dequeMapObj.clear();
}

void CSkyLayer::ClearObj()
{
    int nSize = m_dequeMapObj.size();
    for(int i = 0; i < nSize; i++)
    {
        SafeDelete(	m_dequeMapObj[i]);
    }
    m_dequeMapObj.clear();
}




