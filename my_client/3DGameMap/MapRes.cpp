
//**********************************************************
// 代码编辑器
//**********************************************************

//code by xueliang 2005.5.12
#include "3dGameMap.h"
//const char szLoadFailed[] = "地图的资源配置文件%s,加载失败!";
//const char szLoadOk[]     = "地图的资源配置文件%s,加载成功!";

//-------------------------------------------------------------------------------------
CellRes* CGameMap::GetCellRes (DWORD x, DWORD y)
{
    __int64 i64Data = x;
    i64Data = (i64Data << 32) + y;
    MAP_CELLRES::iterator iter = m_setCellRes.find (i64Data);
    if (iter == m_setCellRes.end ())
    {
        return NULL;
    }
    else
    {
        return iter->second;
    }
}

void CGameMap::LoadMapRes(char* szFileName)
{
    if (!szFileName)
    {
        return;
    }
    char pszFileRes[MAX_PATH] = "";
    strncat(pszFileRes, szFileName, strlen(szFileName) - 5);
    strcat(pszFileRes, ".Mres");
    FILE* fp = fopen(pszFileRes, "r");
    if (!fp)
    {
        /*char szMsg[256]="";
        sprintf(szMsg,szLoadFailed,pszFileRes);
        ::MessageBox (NULL,szMsg,"提示!",0);*/
        return;
    }
    else
    {
        int nAmount = 0;
        fscanf (fp, "ResAmount=%d", &nAmount);
        if (nAmount <= 0)
        {
            return;
        }
        for(int i = 0; i < nAmount; i++)
        {
            int nX, nY;
            DWORD idTmp;
            fscanf (fp, "Cell[%d,%d]=%d\n", &nX, &nY, &idTmp);
            this->PutCellRes (nX, nY, idTmp);
        }
        fclose(fp);
        //		char szMsg[256]="";
        //		sprintf(szMsg,szLoadOk,pszFileRes);
        //		::MessageBox (NULL,szMsg,"提示!",0);
    }
}

void CGameMap::SaveMapRes(char* szFileName)
{
    if (!szFileName)
    {
        return;
    }
    char pszFileRes[MAX_PATH] = "";
    strncat (pszFileRes, szFileName, strlen(szFileName) - 5);
    strcat (pszFileRes, ".Mres");
    FILE* fp = fopen(pszFileRes, "w");
    if (!fp)
    {
        return;
    }
    int nAmount = this->m_setCellRes.size ();
    if (nAmount <= 0)
    {
        return;
    }
    // write amount ...
    fprintf(fp, "ResAmount=%d", nAmount);
    // write cell resource
    MAP_CELLRES::iterator iter = m_setCellRes.begin ();
    while (iter != m_setCellRes.end ())
    {
        CellRes* pCellRes = iter->second;
        if (pCellRes)
        {
            fprintf(fp, "Cell[%d,%d]=%d\n", pCellRes->X, pCellRes->Y, pCellRes->pResTmpID);
        }
        iter++;
    }
    fclose(fp);
}

void CGameMap::PutCellRes (DWORD x, DWORD y , DWORD idTmp)
{
    CellRes* pCellRes = this->GetCellRes (x, y);
    if (pCellRes)
    {
        pCellRes->pResTmpID = idTmp;
    }
    else
    {
        pCellRes = new CellRes;
        pCellRes->X = x;
        pCellRes->Y = y;
        pCellRes->pResTmpID = idTmp;
        __int64 i64Data = x;
        i64Data = (i64Data << 32) + y;
        m_setCellRes.insert (MAP_CELLRES::value_type(i64Data, pCellRes));
    }
}

void CGameMap::ClearRes()
{
    MAP_CELLRES::iterator iter = m_setCellRes.begin ();
    while(iter != m_setCellRes.end ())
    {
        CellRes* pCellRes = iter->second;
        SAFE_DELETE(pCellRes);
        iter++;
    }
    m_setCellRes.clear ();
}

void CGameMap::DelCellRes (DWORD x, DWORD y)
{
    __int64 i64Data = x;
    i64Data = (i64Data << 32) + y;
    MAP_CELLRES::iterator iter = m_setCellRes.find (i64Data);
    if (iter != m_setCellRes.end ())
    {
        CellRes* pCellRes = iter->second;
        SAFE_DELETE(pCellRes);
        m_setCellRes.erase(iter);
    }
}

void CGameMap::BuildMapResSql(const char* pszFileName, const char* pszTemplate)
{
    if (!pszFileName || !pszTemplate)
    {
        return;
    }
    int nAmount = m_setCellRes.size ();
    if (nAmount < 1 )
    {
        return;
    }
    FILE* fp = fopen(pszFileName, "w");
    if (!fp)
    {
        return;
    }
    MAP_CELLRES::iterator iter = m_setCellRes.begin ();
    while(iter != m_setCellRes.end ())
    {
        CellRes* pCellRes = iter->second;
        if (pCellRes)
        {
            fprintf(fp, pszTemplate, pCellRes->X, pCellRes->Y, pCellRes->pResTmpID);
        }
        iter++;
    }
    fclose(fp);
}
