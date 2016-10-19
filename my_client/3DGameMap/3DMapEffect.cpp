
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DMapEffect.cpp: implementation of the C3DMapEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "3DMapEffect.h"
#include "InteractiveLayer.h"
#include "3Dgamemap.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL C3DMapEffect::s_bStroke = false;
C3DMapEffect::C3DMapEffect()
{
    m_bDie = false;
    m_nOffsetY = 0;
    strcpy(m_szIndex, "");
    m_fHorizontal	= 0.0;
    m_fVertical		= 0.0;
    m_bOver			= false;
    m_nDelay		= 0;
    m_bSimpleObj	= false;
    m_dwFrameIndex	= 0;
}

C3DMapEffect::~C3DMapEffect()
{
}

//-------------------------------------------------------------------
void C3DMapEffect::Show(void* pInfo)
{
    if (m_bDie)
    {
        return;
    }
    if (!m_bSimpleObj)
    {
        m_objEffect.SetHeight((float)m_nOffsetY);
    }
    CMyPos posScr, posWorld;
    if (s_bStroke)
    {
        g_objGameMap.Bg2World(m_posWorld.x, m_posWorld.y, posWorld.x, posWorld.y);
        g_objGameMap.World2Screen(posWorld.x, posWorld.y, posScr.x, posScr.y);
        CMyBitmap::ShowBlock(posScr.x - 32, posScr.y - 32, posScr.x + 32, posScr.y + 32, 0xff000000);
    }
    if (!m_bDie)
    {
        if (!m_bSimpleObj)
        {
            m_objEffect.Rotate(m_fVertical, 0.0, m_fHorizontal);
            m_objEffect.Show(m_posWorld);
        }
        else
        {
            CMyPos posBG = {m_posWorld.x, m_posWorld.y};
            //g_objGameMap.World2Bg(m_posWorld.x, m_posWorld.y, posBG.x, posBG.y);
            CMyPos posView;
            g_objGameMap.GetViewPos(posView);
            int nViewportBgX, nViewportBgY;
            int nRotate	= 0;
            g_objGameMap.World2Bg(posView.x, posView.y, nViewportBgX, nViewportBgY);
            //	this->IsFocus();
            union
            {
                DWORD dwColor;
                struct
                {
                    unsigned char ucAlpha;
                    unsigned char ucRed;
                    unsigned char ucGreen;
                    unsigned char ucBlue;
                } InfoColor;
            };
            float alphaShadow	= 0.8f;
            dwColor = g_objGameMap.GetARGB();
            float fColor = (float)(0.1 * (InfoColor.ucRed + InfoColor.ucGreen + InfoColor.ucBlue) / (255 * 3));
            m_objSimple.SetLightOffset(CRole::s_nShadowType, 300, -300, -1000, alphaShadow, fColor, fColor, fColor);
            m_objSimple.Rotate(m_fVertical, 0.0, m_fHorizontal);
            m_objSimple.SetFrame(m_dwFrameIndex);
            m_objSimple.SetPos(posBG.x, posBG.y, 0, nRotate, 1.0f);
            m_objSimple.Draw2BG(nViewportBgX, nViewportBgY);
            m_objSimple.ClearMatrix();
        }
    }
}
//-------------------------------------------------------------------
void C3DMapEffect::SetRotate(float fHorizontal, float fVertical)
{
    m_fHorizontal = fHorizontal;
    m_fVertical = fVertical;
}
//-------------------------------------------------------------------
void C3DMapEffect::Process(void* pInfo)
{
    m_dwFrameIndex ++;
    BOOL* pbOver = (BOOL*)pInfo;
    if (m_bDie)
    {
        m_nDelay ++;
        if (m_nDelay > 10)
        {
            *pbOver = true;
        }
        else
        {
            *pbOver = false;
        }
        return;
    }
    if (m_bSimpleObj)
    {
        m_bOver = false;
        *pbOver = false;
    }
    else
    {
        *pbOver = m_objEffect.Process();
        m_bOver = *pbOver;
    }
    CMyPos posCell;
    CMyPos posWorld;
    g_objGameMap.Bg2World(m_posWorld.x, m_posWorld.y, posWorld.x, posWorld.y);
    g_objGameMap.World2Cell(posWorld.x, posWorld.y, posCell.x, posCell.y);
    this->SetPos(posCell);
    if ((!*pbOver && g_objGameMap.IsPosVisible(posCell)) || (g_objGameMap.IsPosVisible(posCell) && !m_bDelSelf))
    {
        CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
        if (pLayer)
        {
            pLayer->ApplyShow(this);
        }
    }
    if (!m_bDelSelf)
    {
        *pbOver = false;
    }
}
//-------------------------------------------------------------------
C3DMapEffect* C3DMapEffect::CreateNew(CMyPos posWorld, char* pszIndex, BOOL bDelSelf, BOOL bSave)
{
    if (!pszIndex)
    {
        return NULL;
    }
    C3DMapEffect* pEffect = new C3DMapEffect;
    if (!pEffect)
    {
        return NULL;
    }
    if (!pEffect->Create(posWorld, pszIndex, bDelSelf, bSave))
    {
        SAFE_DELETE(pEffect);
        return NULL;
    }
    CMyPos posCell;
    g_objGameMap.World2Cell(posWorld.x, posWorld.y, posCell.x, posCell.y);
    if (pEffect)
    {
        pEffect->SetPos(posCell);
    }
    return pEffect;
}
//-------------------------------------------------------------------
BOOL C3DMapEffect::Create(CMyPos posWorld, char* pszIndex, BOOL bDelSelf, BOOL bSave)
{
    if (!pszIndex)
    {
        return false;
    }
    if (strlen(pszIndex) == 0)
    {
        return false;
    }
    int nBgX, nBgY;
    g_objGameMap.World2Bg(posWorld.x, posWorld.y, nBgX, nBgY);
    m_posWorld.x = nBgX;
    m_posWorld.y = nBgY;
    m_bDelSelf = bDelSelf;
    strcpy(m_szIndex, pszIndex);
    m_bSave = bSave;
    if (pszIndex[0] == '$')
    {
        m_bSimpleObj = true;
        DWORD dwData;
        if (1 != sscanf(pszIndex, "$%u", &dwData))
        {
            return false;
        }
        m_objSimple.Create(dwData);
    }
    else
    {
        m_objEffect.Add(pszIndex);
        if (bSave)
        {
            m_objEffect.EverPlay();
        }
    }
    return true;
}
//-------------------------------------------------------------------
void C3DMapEffect::GetWorldPos(CMyPos& posWorld)
{
    posWorld = m_posWorld;
}
//-------------------------------------------------------------------
void C3DMapEffect::SetWorldPos(CMyPos posWorld)
{
    int nBgX, nBgY;
    g_objGameMap.World2Bg(posWorld.x, posWorld.y, nBgX, nBgY);
    m_posWorld.x = nBgX;
    m_posWorld.y = nBgY;
}
//-------------------------------------------------------------------
void C3DMapEffect::LoadDataObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fread(m_szIndex, sizeof(char), 64, fp);
    // save posworld
    fread(&m_posWorld, sizeof(CMyPos), 1, fp);
    this->Create(m_posWorld, m_szIndex);
    if (!m_bSimpleObj)
    {
        m_objEffect.EverPlay();
    }
    m_bDelSelf	= true;
    m_bDie = false;
    m_nOffsetY = 0;
    m_bSave = true;
}
//-------------------------------------------------------------------
void C3DMapEffect::LoadTextObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fscanf(fp, "Index=%s\n", m_szIndex);
    fscanf(fp, "PosWorld=[%d,%d]\n", &m_posWorld.x, &m_posWorld.y);
    this->Create(m_posWorld, m_szIndex);
    if (!m_bSimpleObj)
    {
        m_objEffect.EverPlay();
    }
    m_bDelSelf	= true;
    m_bDie = false;
    m_nOffsetY = 0;
    m_bSave = true;
}
//-------------------------------------------------------------------
void C3DMapEffect::SaveDataObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    // save index
    fwrite(m_szIndex, sizeof(char), 64, fp);
    // save posworld
    CMyPos posWorld;
    g_objGameMap.Bg2World(m_posWorld.x, m_posWorld.y, posWorld.x, posWorld.y);
    fwrite(&posWorld, sizeof(CMyPos), 1, fp);
}
//-------------------------------------------------------------------
void C3DMapEffect::SaveTextObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fprintf(fp, "Index=%s\n", m_szIndex);
    CMyPos posWorld;
    g_objGameMap.Bg2World(m_posWorld.x, m_posWorld.y, posWorld.x, posWorld.y);
    fprintf(fp, "PosWorld=[%d,%d]\n", posWorld.x, posWorld.y);
}
//-------------------------------------------------------------------
BOOL C3DMapEffect::IsFocus()
{
    CMyPos posScr, posWorld;
    if (s_bStroke)
    {
        g_objGameMap.Bg2World(m_posWorld.x, m_posWorld.y, posWorld.x, posWorld.y);
        g_objGameMap.World2Screen(posWorld.x, posWorld.y, posScr.x, posScr.y);
        CMyPos posMouse;
        ::MouseCheck(posMouse.x, posMouse.y);
        if ((posMouse.x > posScr.x - 32) &&
                (posMouse.y > posScr.y - 32) &&
                (posMouse.x < posScr.x + 32) &&
                (posMouse.y < posScr.y + 32))
        {
            return true;
        }
    }
    return false;
}
//-------------------------------------------------------------------
void C3DMapEffect::SetDir(int nDir)
{
    nDir = nDir % 8;
    float fPei = (float)3.14159;
    switch(nDir)
    {
    case 0:
        m_fHorizontal = fPei * 5 / 4;
        break;
    case 1:
        m_fHorizontal = fPei;
        break;
    case 2:
        m_fHorizontal = fPei * 3 / 4;
        break;
    case 3:
        m_fHorizontal = fPei / 2;
        break;
    case 4:
        m_fHorizontal = fPei / 4;
        break;
    case 5:
        m_fHorizontal = 0;
        break;
    case 6:
        m_fHorizontal = -fPei / 4;
        break;
    case 7:
        m_fHorizontal = -fPei / 2;
        break;
    }
}
//-------------------------------------------------------------------
