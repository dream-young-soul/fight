
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DMapEffectNew.cpp: implementation of the C3DMapEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "3DMapEffectNew.h"
#include "InteractiveLayer.h"
#include "3Dgamemap.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void C3DMapEffectNew::SetScale(float x, float y, float z)
{
    m_fScaleX = x;
    m_fScaleY = y;
    m_fScaleZ = z;
}

void C3DMapEffectNew::Show(void* pInfo)
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
            m_objEffect.Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
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
            float fColor =(float)( 0.1 * (InfoColor.ucRed + InfoColor.ucGreen + InfoColor.ucBlue) / (255 * 3));
            m_objSimple.SetLightOffset(CRole::s_nShadowType, 300, -300, -1000, alphaShadow, fColor, fColor, fColor);
            m_objSimple.Rotate(m_fVertical, 0.0, m_fHorizontal);
            m_objSimple.Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
            m_objSimple.SetFrame(m_dwFrameIndex);
            m_objSimple.SetPos(posBG.x, posBG.y, 0, nRotate, 1.0f);
            m_objSimple.Draw2BG(nViewportBgX, nViewportBgY);
            m_objSimple.ClearMatrix();
        }
    }
}

C3DMapEffectNew* C3DMapEffectNew::CreateNew(CMyPos posWorld, char* pszIndex, BOOL bDelSelf, BOOL bSave)
{
    if (!pszIndex)
    {
        return NULL;
    }
    C3DMapEffectNew* pEffect = new C3DMapEffectNew;
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

void C3DMapEffectNew::LoadDataObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fread(m_szIndex, sizeof(char), 64, fp);
    // save posworld
    fread(&m_posWorld, sizeof(CMyPos), 1, fp);
    this->Create(m_posWorld, m_szIndex);
    fread(&(this->m_fVertical), sizeof (float), 1, fp);
    fread(&(this->m_fVertical), sizeof (float), 1, fp);
    fread(&(this->m_fHorizontal), sizeof (float), 1, fp);
    fread(&(this->m_fScaleX), sizeof (float), 1, fp);
    fread(&(this->m_fScaleY), sizeof (float), 1, fp);
    fread(&(this->m_fScaleZ), sizeof (float), 1, fp);
    if (!m_bSimpleObj)
    {
        m_objEffect.EverPlay();
    }
    m_bDelSelf	= true;
    m_bDie = false;
    m_nOffsetY = 0;
    m_bSave = true;
}

void C3DMapEffectNew::LoadTextObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fscanf(fp, "Index=%s\n", m_szIndex);
    fscanf(fp, "PosWorld=[%d,%d]\n", &m_posWorld.x, &m_posWorld.y);
    this->Create(m_posWorld, m_szIndex);
    fscanf(fp, "Angle=[%f,%f,%f]\n", &m_fVertical, &m_fVertical, &m_fHorizontal);
    fscanf(fp, "Scale=[%f,%f,%f]\n", &m_fScaleX, &m_fScaleY, &m_fScaleZ);
    if (!m_bSimpleObj)
    {
        m_objEffect.EverPlay();
    }
    m_bDelSelf	= true;
    m_bDie = false;
    m_nOffsetY = 0;
    m_bSave = true;
}

void C3DMapEffectNew::SaveDataObj(FILE* fp)
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
    fwrite(&(this->m_fVertical), sizeof (float), 1, fp);
    fwrite(&(this->m_fVertical), sizeof (float), 1, fp);
    fwrite(&(this->m_fHorizontal), sizeof (float), 1, fp);
    fwrite(&(this->m_fScaleX), sizeof (float), 1, fp);
    fwrite(&(this->m_fScaleY), sizeof (float), 1, fp);
    fwrite(&(this->m_fScaleZ), sizeof (float), 1, fp);
}

void C3DMapEffectNew::SaveTextObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fprintf(fp, "Index=%s\n", m_szIndex);
    CMyPos posWorld;
    g_objGameMap.Bg2World(m_posWorld.x, m_posWorld.y, posWorld.x, posWorld.y);
    fprintf(fp, "PosWorld=[%d,%d]\n", posWorld.x, posWorld.y);
    fprintf(fp, "Angle=[%f,%f,%f]\n", m_fVertical, m_fVertical, m_fHorizontal);
    fprintf(fp, "Scale=[%f,%f,%f]\n", m_fScaleX, m_fScaleY, m_fScaleZ);
}
