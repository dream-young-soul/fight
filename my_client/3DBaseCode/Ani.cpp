
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#include "basefunc.H"
#include "nd_bitmap.H"
#include "ani.H"

// static
HANDLE	CAni::s_hHeap	= NULL;
UINT	CAni::s_uNumAllocsInHeap	= 0;
CAniFileSet CAni::s_setAniFile;
IDai* CAni::s_pDai = IDai::CreateNew();
IFrameSet* CAni::s_pFrameSet = IFrameSet::CreateNew();
CRITICAL_SECTION	sAniCritical;
//--------------------------------------------------------------
DWORD Str2ID( const char* str )
{
    int i;
    unsigned int v;
    static unsigned m[70];
    strncpy((char*)m, str, 256);
    for (i = 0; i < 256 / 4 && m[i]; i++) ;
    m[i++] = 0x9BE74448, m[i++] = 0x66F42C48;
    v = 0xF4FA8928;
    __asm
    {
        mov esi, 0x37A8470E		;//x0=0x37A8470E
        mov edi, 0x7758B42B		;//y0=0x7758B42B
        xor ecx, ecx
        _loop:
        mov ebx, 0x267B0B11		;//w=0x267B0B11
        rol v, 1
        lea eax, m
        xor ebx, v

        mov eax, [eax+ecx*4]
        mov edx, ebx
        xor esi, eax
        xor edi, eax

        add edx, edi
        or edx, 0x2040801		;//a=0x2040801
        and edx, 0xBFEF7FDF		;//c=0xBFEF7FDF

        mov eax, esi
        mul edx
        adc eax, edx
        mov edx, ebx
        adc eax, 0

        add edx, esi
        or edx, 0x804021		;//b=0x804021
        and edx, 0x7DFEFBFF		;//d=0x7DFEFBFF

        mov esi, eax
        mov eax, edi
        mul edx

        add edx, edx
        adc eax, edx
        jnc _skip
        add eax, 2
        _skip:
        inc ecx;
        mov edi, eax
        cmp ecx, i
        jnz _loop
        xor esi, edi
        mov v, esi
    }
    return v;
}

//--------------------------------------------------------------
void* CAni::operator new(size_t size)
{
    // got heap?
    if (!s_hHeap)
    {
        s_hHeap	=::HeapCreate(0, 0, 0);
        if (!s_hHeap)
        {
            return NULL;
        }
    }
    // alloc mem for new obj
    void* p	=::HeapAlloc(s_hHeap, HEAP_ZERO_MEMORY, size);
    if (p)
    {
        s_uNumAllocsInHeap++;
    }
    return p;
}

//--------------------------------------------------------------
void CAni::operator delete(void* p)
{
    // check param
    if (!s_hHeap || s_uNumAllocsInHeap <= 0)
    {
        return;
    }
    // free it...
    if (::HeapFree(s_hHeap, 0, p))
    {
        s_uNumAllocsInHeap--;
    }
    // time to destroy heap?
    if (s_uNumAllocsInHeap <= 0)
    {
        if (::HeapDestroy(s_hHeap))
        {
            s_hHeap	= NULL;
        }
    }
}

//--------------------------------------------------------------
BOOL CAni::IsValidPt(void* p)
{
    if (!s_hHeap || !p)
    {
        return false;
    }
    if ((DWORD)p < 0x00010000 || (DWORD)p >= 0x7FFEFFFF)	// user address range from 1M--2G-64k
    {
        return false;
    }
    if (::IsBadCodePtr((FARPROC)p))
    {
        return false;
    }
    return ::HeapValidate(s_hHeap, 0, p);
}

//--------------------------------------------------------------
void CAni::Init(void)
{
    m_nStatus	= ANISTATUS_INIT;
    m_bHalf		= false;
    m_iFrameAmount	= 0;
    for(int i = 0; i < _MAX_FRAME; i++)
    {
        m_pbmpFrame[i]	= NULL;
    }
    return;
}

//#define USE_ENCRYPTANI
#ifdef USE_ENCRYPTANI

//--------------------------------------------------------------
BOOL CAni::Create(const char* pszDataFile, const char* pszIndex, BOOL bHalf, DWORD dwLoadWay)
{
    if (!pszDataFile || !pszIndex)
    {
        return false;
    }
    if (m_nStatus == ANISTATUS_LOADED)
    {
        Destroy();
    }
    m_bHalf = bHalf;
    FILE* fp = fopen(pszDataFile, "rb");
    if (!fp)
    {
        m_nStatus	= ANISTATUS_FAILED;
        return false;
    }
    WORD wFileFlag = 0;
    fread(&wFileFlag, sizeof(WORD), 1, fp);
    WORD ANIFILE_FLAG  =  0x8888;
    if (wFileFlag != ANIFILE_FLAG)
    {
        return false;
    }
    int nTotal = 0;
    fread(&nTotal, sizeof(int), 1, fp);
    typedef struct CSection
    {
        DWORD dwID;
        int   nFrame;
        int   nOffSet;
    } Section;
    Section* pTitle = (Section*)calloc(sizeof(Section), nTotal);
    if (!pTitle)
    {
        return false;
    }
    fread(pTitle, sizeof(Section), nTotal, fp);
    // search the titile
    DWORD dwIDSearch = String2ID( pszIndex );
    int i = 0, nfind = -1;
    Section* pTitleFind = NULL;
    for (i = 0; i < nTotal; i++)
    {
        pTitleFind = pTitle + i;
        if (pTitleFind->dwID == dwIDSearch)
        {
            nfind = i;
            break;
        }
    }
    if (nfind == -1)
    {
        m_nStatus	= ANISTATUS_FAILED;
        fclose(fp);
        return false;
    }
    else
    {
        pTitleFind = pTitle + nfind;
    }
    fseek(fp, 256 * pTitleFind->nOffSet, SEEK_CUR);
    m_iFrameAmount	= pTitleFind->nFrame % _MAX_FRAME;
    // get frame file name & load it...
    for( i = 0; i < m_iFrameAmount; i ++)
    {
        char szFrame[MAX_PATH];
        if (!feof(fp))
        {
            fread(szFrame, 256, 1, fp);
        }
        m_pbmpFrame[i]	= new CMyBitmap;
        assert(m_pbmpFrame[i]);
        if ((i != 0) && (i % 2 == 0) && m_bHalf)
        {
            m_pbmpFrame[i] = m_pbmpFrame[i-1];
        }
        else if (!m_pbmpFrame[i]->Load(szFrame, dwLoadWay))	// frame load error
        {
            for(int j = 0; j < i + 1; j++)
            {
                if (m_pbmpFrame[j])
                {
                    delete m_pbmpFrame[j];
                }
                m_pbmpFrame[j]	= NULL;
            }
            fclose(fp);
            m_iFrameAmount	= 0;
            m_nStatus	= ANISTATUS_FAILED;
            return false;
        }
    }
    fclose(fp);
    m_nStatus	= ANISTATUS_LOADED;
    return true;
}

//--------------------------------------------------------------
#else
//--------------------------------------------------------------
BOOL CAni::Create(const char* pszDataFile, const char* pszIndex, BOOL bHalf, DWORD dwLoadWay)
{
    // param check
    ::EnterCriticalSection(&sAniCritical);
    if (!pszDataFile || !pszIndex)
    {
        return false;
    }
    // release mem if data have loaded.
    if (m_nStatus == ANISTATUS_LOADED)
    {
        Destroy();
    }
    m_bHalf = bHalf;
    if (s_pDai->Locate(pszDataFile, pszIndex, CAni::s_pFrameSet) == IDai::SUCCESS)
    {
        BOOL bReturn = this->Create(CAni::s_pFrameSet);
        ::LeaveCriticalSection(&sAniCritical);
        return bReturn;
    }
    else
    {
        m_nStatus	= ANISTATUS_FAILED;
        ::LeaveCriticalSection(&sAniCritical);
        return false;
    }
    ::LeaveCriticalSection(&sAniCritical);
    return false;
}
//--------------------------------------------------------------
#endif
//--------------------------------------------------------------
BOOL CAni::Create(AniIndexInfo* pInfo)
{
    if (!pInfo)
    {
        m_nStatus	= ANISTATUS_FAILED;
        return false;
    }
    m_iFrameAmount = pInfo->setAniLine.size() - 1;
    for(int i = 0; i < m_iFrameAmount; i ++)
    {
        char* pszFile = pInfo->setAniLine[i+1];
        m_pbmpFrame[i]	= new CMyBitmap;
        assert(m_pbmpFrame[i]);
        if ((i != 0) && (i % 2 == 0) && m_bHalf)
        {
            m_pbmpFrame[i] = m_pbmpFrame[i-1];
        }
        else if (!pszFile || !m_pbmpFrame[i]->Load(pszFile))	// frame load error
        {
            for(int j = 0; j < i + 1; j++)
            {
                if (m_pbmpFrame[j])
                {
                    delete m_pbmpFrame[j];
                }
                m_pbmpFrame[j]	= NULL;
            }
            m_iFrameAmount	= 0;
            m_nStatus	= ANISTATUS_FAILED;
            //::ErrorOut("Error: %s load failed in CAni::Create().", szFrame);
            return false;
        }
    }
    m_nStatus	= ANISTATUS_LOADED;
    return true;
}

//--------------------------------------------------------------
BOOL	CAni::Create(IFrameSet* pFrameSet)
{
    if (!pFrameSet)
    {
        m_nStatus	= ANISTATUS_FAILED;
        return false;
    }
    m_iFrameAmount = pFrameSet->GetFrameAmount();
    for(int i = 0; i < m_iFrameAmount; i ++)
    {
        char* pszFile = pFrameSet->GetFrameByIndex(i);
        m_pbmpFrame[i]	= new CMyBitmap;
        assert(m_pbmpFrame[i]);
        if ((i != 0) && (i % 2 == 0) && m_bHalf)
        {
            m_pbmpFrame[i] = m_pbmpFrame[i-1];
        }
        else if (!pszFile || !m_pbmpFrame[i]->Load(pszFile))	// frame load error
        {
            for(int j = 0; j < i + 1; j++)
            {
                if (m_pbmpFrame[j])
                {
                    delete m_pbmpFrame[j];
                }
                m_pbmpFrame[j]	= NULL;
            }
            m_iFrameAmount	= 0;
            m_nStatus	= ANISTATUS_FAILED;
            //::ErrorOut("Error: %s load failed in CAni::Create().", szFrame);
            return false;
        }
    }
    m_nStatus	= ANISTATUS_LOADED;
    return true;
}
//--------------------------------------------------------------
// return true means is the last frame
BOOL CAni::Show(UINT uFrame, int x, int y, int alpha, DWORD dwShowWay) const
{
    if (m_nStatus != ANISTATUS_LOADED || m_iFrameAmount <= 0)
    {
        return false;
    }
    uFrame	%= m_iFrameAmount;
    m_pbmpFrame[uFrame]->Show(x, y, alpha, dwShowWay);
    if (uFrame + 1 >= (UINT)m_iFrameAmount)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//--------------------------------------------------------------
BOOL CAni::ShowEx(UINT uFrame, int x, int y, RECT* lpSrc, DWORD dwWidth, DWORD dwHeight, int alpha, DWORD dwShowWay)
{
    if (m_nStatus != ANISTATUS_LOADED || m_iFrameAmount <= 0)
    {
        return false;
    }
    uFrame	%= m_iFrameAmount;
    m_pbmpFrame[uFrame]->ShowEx(x, y, lpSrc, dwWidth, dwHeight, alpha, dwShowWay);
    if (uFrame + 1 >= (UINT)m_iFrameAmount)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//--------------------------------------------------------------
CMyBitmap* CAni::GetFrame(UINT uFrameIndex) const
{
    if (m_nStatus != ANISTATUS_LOADED || m_iFrameAmount <= 0)
    {
        return NULL;
    }
    return m_pbmpFrame[uFrameIndex%m_iFrameAmount];
}

//--------------------------------------------------------------
void CAni::Destroy(void)
{
    if (m_nStatus != ANISTATUS_LOADED)
    {
        return;
    }
    for(int i = 0; i < m_iFrameAmount; i++)
    {
        if (m_bHalf)
        {
            if ((i == 0) || (i % 2 != 0))
            {
                SAFE_DELETE(m_pbmpFrame[i]);
            }
        }
        else
        {
            SAFE_DELETE(m_pbmpFrame[i]);
        }
        m_pbmpFrame[i]	= NULL;
    }
    m_iFrameAmount	= 0;
    m_nStatus	= ANISTATUS_INIT;
    return;
}

//--------------------------------------------------------------
DWORD CAni::GetSize() const
{
    DWORD dwAniSize = 0;
    if (m_nStatus == ANISTATUS_LOADED)
    {
        int nAmount = this->GetFrameAmount();
        for(int i = 0; i < nAmount; i ++)
        {
            if (m_bHalf)
            {
                if ((i != 0) && (i % 2 == 0))
                {
                    continue;
                }
            }
            CMyBitmap* pBmp = this->GetFrame(i);
            if (pBmp)
            {
                dwAniSize += pBmp->GetSize();
            }
        }
    }
    return dwAniSize;
}

//--------------------------------------------------------------
CMySize CAni::GetSize(UINT uFrame) const
{
    CMySize sizeAni = {0, 0};
    CMyBitmap* pBmp = this->GetFrame(uFrame);
    if (pBmp)
    {
        pBmp->GetSize(sizeAni.iWidth, sizeAni.iHeight);
    }
    return sizeAni;
}

//--------------------------------------------------------------
BOOL CAni::CheckAni(const char* pszDataFile, const char* pszIndex)
{
    // param check
    if (!pszDataFile || !pszIndex)
    {
        return false;
    }
    // open data file
    FILE* fp	= fopen(pszDataFile, "r");
    if (!fp)
    {
        return false;
    }
    // search the titile
    char szTitle[256];
    sprintf(szTitle, "[%s]", pszIndex);
    while(true)
    {
        char szLine[1024];
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            fclose(fp);
            return false;
        }
        if (strstr(szLine, szTitle))
        {
            fclose(fp);
            return true;
        }
    }
}

//--------------------------------------------------------------
//	CDynamicAni
//--------------------------------------------------------------
CDynamicAni::CDynamicAni(const char* pszIniFile, const char* pszIndex, BOOL bHalf)
{
    m_bHalf = bHalf;
    m_dwTimeBorn = ::TimeGet();
    this->SetAniFilename(pszIniFile, pszIndex);
}


//--------------------------------------------------------------
BOOL CDynamicAni::Load(void)
{
    try
    {
        this->ResetLife();
        return this->Create(m_szIniFile, m_szIndex, m_bHalf);
    }
    catch(...)
    {
        ::LogMsg("Error: exception catch when loading %s :%s", m_szIniFile, m_szIndex);
        return false;
    }
}

BOOL CDynamicAni::SetAniFilename(const char* pszDataFile, const char* pszIndex)
{
    if (!pszDataFile || !pszIndex)
    {
        return false;
    }
    strcpy(m_szIniFile, pszDataFile);
    strcpy(m_szIndex, pszIndex);
    static char szTmep[256];
    strcpy(szTmep, pszDataFile);
    strcat((char*)szTmep, pszIndex);
    m_idAni = Str2ID(szTmep);
    return true;
}

BOOL CDynamicAni::CheckAniByFilename(const char* pszDataFile, const char* pszIndex) const
{
    if (!pszDataFile || !pszIndex)
    {
        return false;
    }
    if ((strcmp(pszIndex, m_szIndex) == 0) &&
            (strcmp(pszDataFile, m_szIniFile) == 0))
    {
        return true;
    }
    return false;
}

//#########################################################################

//--------------------------------------------------------------
CAniFile::CAniFile()
{
}
//--------------------------------------------------------------
CAniFile::~CAniFile()
{
    this->Destroy();
}
//--------------------------------------------------------------
void CAniFile::Destroy()
{
    map<OBJID, AniIndexInfo>::iterator iter;
    for(iter = m_setAniIndexInfo.begin(); iter != m_setAniIndexInfo.end();  iter++)
    {
        this->DestroyAniIndexInfo(iter->second);
    }
    m_setAniIndexInfo.clear();
}
//--------------------------------------------------------------
AniIndexInfo* CAniFile::CreateNewAniIndexInfo()
{
    AniIndexInfo* pInfo = new AniIndexInfo;
    MYASSERT(pInfo);
    return pInfo;
}
//--------------------------------------------------------------
void CAniFile::DestroyAniIndexInfo(AniIndexInfo& pInfo)
{
    int nAmount = pInfo.setAniLine.size();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        char* pszStr = pInfo.setAniLine[i];
        if (pszStr)
        {
            delete[] pszStr;
            pszStr = NULL;
        }
    }
    pInfo.setAniLine.clear();
}
//--------------------------------------------------------------
CAniFile* CAniFile::CreateNew(char* pszAniFile)
{
    if (!pszAniFile)
    {
        return NULL;
    }
    CAniFile* pAniFile = new CAniFile;
    MYASSERT(pAniFile);
    if (!pAniFile->Create(pszAniFile))
    {
        SAFE_DELETE(pAniFile);
        return NULL;
    }
    strcpy(pAniFile->m_szFileName, pszAniFile);
    return pAniFile;
}
//--------------------------------------------------------------
BOOL CAniFile::Create(char* pszAniFile)
{
    if (!pszAniFile)
    {
        return false;
    }
    FILE* fp = fopen(pszAniFile, "r");
    if (!fp)
    {
        return false;
    }
    while(true)
    {
        // scan title
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            fclose(fp);
            return true;
        }
        char szTitle[MAX_PATH] = "";
        if (1 != sscanf(szLine, "[%s", szTitle))
        {
            continue;
        }
        char* p = strchr(szTitle, ']');
        if (p)
        {
            *p = '\0';
        }
        else
        {
            fclose(fp);
            ::ErrorOut("Error Title line %s in %s", szLine, pszAniFile);
            return false;
        }
        AniIndexInfo infoAni;
        this->AddAniIndexLine(&infoAni, szTitle);
        // get frame amount
        int nFrameAmount = 0;
        if (1 != fscanf(fp, "FrameAmount=%d\n", &nFrameAmount))
        {
            fclose(fp);
            ::ErrorOut("FrameAmount error in [%s] of %s", szTitle, pszAniFile);
            return false;
        }
        nFrameAmount %= _MAX_FRAME;
        // get frame file name
        for (int i = 0; i < nFrameAmount; i++)
        {
            char szFormat[MAX_PATH] = "";
            sprintf(szFormat, "Frame%d=%s\n", i, "%s");
            char szFile[MAX_PATH] = "";
            if (1 != fscanf(fp, szFormat, szFile))
            {
                fclose(fp);
                ::ErrorOut("%s error in [%s] of %s", szFormat, szTitle, pszAniFile);
                return false;
            }
            // yeah
            this->AddAniIndexLine(&infoAni, szFile);
        }
        OBJID id  = Str2ID(szTitle);
        m_setAniIndexInfo[id] = infoAni;
    }
    return true;
}
//--------------------------------------------------------------
AniIndexInfo* CAniFile::GetAniIndexInfo(char* pszIndex)
{
    if (!pszIndex)
    {
        return NULL;
    }
    OBJID id = Str2ID(pszIndex);
    map<OBJID, AniIndexInfo>::iterator iter = m_setAniIndexInfo.find(id);
    if (iter == m_setAniIndexInfo.end())
    {
        return NULL;
    }
    return &iter->second;
}
//--------------------------------------------------------------
void CAniFile::AddAniIndexLine(AniIndexInfo* pInfo, char* pszLine)
{
    if (!pInfo || !pszLine)
    {
        return;
    }
    char* pszStr = new char[strlen(pszLine)+1];
    strcpy(pszStr, pszLine);
    pInfo->setAniLine.push_back(pszStr);
}
//--------------------------------------------------------------

//#########################################################################

//--------------------------------------------------------------
CAniFileSet::CAniFileSet()
{
}
//--------------------------------------------------------------
CAniFileSet::~CAniFileSet()
{
    this->Destroy();
}
//--------------------------------------------------------------
void CAniFileSet::Destroy()
{
    int nAmount = m_setAniFile.size();
    for(int i = 0; i < nAmount; i++)
    {
        SAFE_DELETE(m_setAniFile[i]);
    }
    m_setAniFile.clear();
}
//--------------------------------------------------------------
void CAniFileSet::AddAniFile(char* pszAniFile)
{
    if (!pszAniFile)
    {
        return;
    }
    int nAmount = m_setAniFile.size();
    for(int i = 0; i < nAmount; i++)
    {
        CAniFile* pFile = m_setAniFile[i];
        if (pFile && (strcmp(pFile->m_szFileName, pszAniFile) == 0))
        {
            return;
        }
    }
    CAniFile* pFile = CAniFile::CreateNew(pszAniFile);
    if (pFile)
    {
        m_setAniFile.push_back(pFile);
    }
}
//--------------------------------------------------------------
void CAniFileSet::DelAniFile(char* pszAniFile)
{
    if (!pszAniFile)
    {
        return;
    }
    int nAmount = m_setAniFile.size();
    for(int i = 0; i < nAmount; i++)
    {
        CAniFile* pFile = m_setAniFile[i];
        if (pFile && (strcmp(pFile->m_szFileName, pszAniFile) == 0))
        {
            SAFE_DELETE(pFile);
            m_setAniFile.erase(m_setAniFile.begin() + i);
            return;
        }
    }
}
//--------------------------------------------------------------
AniIndexInfo* CAniFileSet::GetAniIndexInfo(char* pszAniFile, char* pszAniIndex)
{
    if (!pszAniFile || !pszAniIndex)
    {
        return NULL;
    }
    int nAmount = m_setAniFile.size();
    for(int i = 0; i < nAmount; i++)
    {
        CAniFile* pFile = m_setAniFile[i];
        if (pFile && (strcmp(pFile->m_szFileName, pszAniFile) == 0))
        {
            return pFile->GetAniIndexInfo(pszAniIndex);
        }
    }
    return NULL;
}
//--------------------------------------------------------------
