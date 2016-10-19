
//**********************************************************
// 代码编辑器
//**********************************************************

#include "DataControlAni.h"
#include "ani.h"
#include "gamedataset.h"

void CDataControlAni::Show(int nFrame, int nCurAction, int nX, int nY, int nAlpha)
{
    int nOffsetX, nOffsetY;
    if (nCurAction > (int)m_dequeAction.size())
    {
        nCurAction = m_dequeAction.size() - 1;
    }
    OFFSET* pOff = m_dequeAction[nCurAction];
    CAni* pAni = g_objGameDataSet.GetDataAni(m_szAniFilename, pOff->szSection, EXIGENCE_IMMEDIATE, 30 * 1000);
    if (!pAni)
    {
        return;
    }
    nOffsetX = pOff->nOffsetX[nFrame];
    nOffsetY = pOff->nOffsetY[nFrame];
    int nAniFrame = nFrame % pAni->GetFrameAmount();
    pAni->Show(nAniFrame, nX + nOffsetX, nY + nOffsetY, nAlpha);
}
int  CDataControlAni::GetControlFrame(int nAction)//得到NACTION的总帧数
{
    return m_dequeAction[nAction]->nFrame;
}

BOOL CDataControlAni::Create(const char* pszDataFile)
{
    m_dwBornTime = ::TimeGet();
    FILE* fp;
    fp	= fopen(pszDataFile, "rb");
    if (!fp)
    {
        return 0;
    }
    DWORD dwFileFlag;// = 0xaabbccdd;
    fread(&dwFileFlag, sizeof(DWORD), 1, fp);
    if (dwFileFlag != 0xaabbccdd)
    {
        return 0;
    }
    fread(m_szAniFilename, 256, 1, fp);
    fread(&m_bCanPlayRand, 1, 1, fp);
    int nTotalAction;
    fread(&nTotalAction, sizeof(int), 1, fp);
    for(int i = 0; i < nTotalAction; i++)
    {
        OFFSET* pOff = (OFFSET*)malloc(sizeof(OFFSET));
        fread(pOff->szSection, 32, 1, fp);
        fread(&pOff->nFrame, 1, 1, fp);
        pOff->nOffsetX = (int*)malloc(sizeof(int) * pOff->nFrame);
        pOff->nOffsetY = (int*)malloc(sizeof(int) * pOff->nFrame);
        for(int j = 0; j < pOff->nFrame; j++)
        {
            int x, y;
            fread(&x, sizeof(int), 1, fp);
            fread(&y, sizeof(int), 1, fp);
            pOff->nOffsetX[j] = x;
            pOff->nOffsetY[j] = y;
        }
        m_dequeAction.push_back(pOff);
    }
    fclose(fp);
    return 1;
}


void CDataControlAni::Destroy()
{
    for(int i = 0; i < (int)m_dequeAction.size(); i++)
    {
        if (m_dequeAction[i])
        {
            OFFSET* pOff = m_dequeAction[i] ;
            if (pOff->nOffsetX)
            {
                free(pOff->nOffsetX);
            }
            if (pOff->nOffsetY)
            {
                free(pOff->nOffsetY);
            }
            free(pOff);
        }
    }
    m_dequeAction.clear();
}


