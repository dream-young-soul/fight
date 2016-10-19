
//**********************************************************
// 代码编辑器
//**********************************************************

#include "floorlayer.h"
#include "ani.h"
#include "3dgamemap.h"

CFloorLayer* CFloorLayer::CreateNew()
{
    CFloorLayer* p = new CFloorLayer();
    if (p)
    {
        return p;
    }
    else
    {
        return NULL;
    }
}


void CFloorLayer::AddObj(CMapObj* obj)
{
    m_dequeMapObj.push_back(obj);
}


void CFloorLayer::ClearObj()
{
    int nSize = m_dequeMapObj.size();
    for(int i = 0; i < nSize; i++)
    {
        SafeDelete(m_dequeMapObj[i]);
    }
    m_dequeMapObj.clear();
}

void CFloorLayer::Show(CMyPos posShow)
{
    m_ObjRose.Show();
    m_ObjPatternRose.Show();
    int nAmount = m_dequeMapObj.size();
    for(int i = 0; i < nAmount; i++)
    {
        m_dequeMapObj[i]->Show(NULL);
    }
}

void CFloorLayer::Process(void* pInfo)
{
    int nAmount = m_dequeMapObj.size();
    for(int i = 0; i < nAmount; i++)
    {
        m_dequeMapObj[i]->Process(pInfo);
    }
    return;
}

void CFloorLayer::CreateRose(int iWorldX, int iWorldY, int nType, int nColor) // nType图案 0 全屏 1 --- 5+ 外部文件图案数 1:红色,2:黄色,3:玫瑰色 ,4:三色花
{
    if (nType)
    {
        m_ObjPatternRose.Create(iWorldX, iWorldY, nType, nColor);
    }
    else
    {
        m_ObjRose.Create( iWorldX, iWorldY, nColor);
    }
}

void CFloorLayer::Destory()
{
    m_ObjRose.Destroy();
    m_ObjPatternRose.Destroy();
    ClearObj();
}


//--------------------------------------------------------------
BOOL CFloorObj::Create(const char* pszDataFile, const char* pszIndex, int nWorldX, int nWorldY, int nLifeTime)
{
    if (m_pAni)
    {
        SAFE_DELETE(m_pAni);
    }
    m_pAni	= new CAni;
    if (!m_pAni)
    {
        return false;
    }
    if (!m_pAni->Create(pszDataFile, pszIndex))
    {
        return 0;
    }
    m_nWorldX		= nWorldX;
    m_nWorldY		= nWorldY;
    m_nLifeTime		= nLifeTime;
    m_nCurrentFrame	= 0;
    return 1;
}

//--------------------------------------------------------------
void CFloorObj::Destroy(void)
{
    SAFE_DELETE(m_pAni);
}

//--------------------------------------------------------------

void CFloorObj::Show(void* pInfo)
{
    if (!m_nLifeTime)
    {
        return;
    }
    int x, y;
    g_objGameMap.World2Screen(m_nWorldX, m_nWorldY, x, y);
    m_pAni->Show(m_nCurrentFrame++, x, y);
    if (m_nCurrentFrame >= m_pAni->GetFrameAmount())
    {
        m_nCurrentFrame = 0;
    }
}

void CFloorObj::Process(void* pInfo)
{
    if (m_nLifeTime != -1)
    {
        m_nLifeTime--;
    }
}
