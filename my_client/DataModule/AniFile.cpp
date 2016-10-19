
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: AniFile.cpp
// Create by: Huang Yuhang
// Create on: 2003/9/24 14:40
//--------------------------------------------------------------------------------------
#pragma warning(disable:4786)
#include "AniFile.h"
#include "DataAniModule.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAniFile::CAniFile()
{
    m_pAniFile = NULL;
    m_pDaiFile = NULL;
    m_pAniFileByOrder = NULL;
    m_pDaiFileByOrder = NULL;
    m_uLastErrorLine = 0;
}

CAniFile::~CAniFile()
{
    this->Clear();
}

//--------------------------------------------------------------------------------------

unsigned long	CAniFile::Str2Id(const char* pszStr)
{
    int i;
    unsigned int v;
    strncpy((char*)m_idGen, pszStr, 256);
    for (i = 0; i < 256 / 4 && m_idGen[i]; i++) ;
    m_idGen[i++] = 0x9BE74448, m_idGen[i++] = 0x66F42C48;
    v = 0xF4FA8928;
    unsigned* pm = (unsigned*)&m_idGen;
    __asm
    {
        mov esi, 0x37A8470E		;//x0=0x37A8470E
        mov edi, 0x7758B42B		;//y0=0x7758B42B
        xor ecx, ecx
        _loop:
        mov ebx, 0x267B0B11		;//w=0x267B0B11
        rol v, 1
        mov eax, pm
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

//--------------------------------------------------------------------------------------
void CAniFile::Clear()
{
    if (m_pAniFile)
    {
        map<unsigned long, AniInfo*>::iterator iter = m_pAniFile->setAniInfo.begin();
        while(iter != m_pAniFile->setAniInfo.end())
        {
            AniInfo* pInfo = iter->second;
            if (pInfo)
            {
                int nStrAmounat = pInfo->setFrame.size();
                for(int i = 0; i < nStrAmounat; i++)
                {
                    char* pStr = pInfo->setFrame[i];
                    if (pStr)
                    {
                        delete[] pStr;
                    }
                }
                pInfo->setFrame.clear();
                delete(pInfo);
                pInfo = NULL;
            }
            iter ++;
        }
        m_pAniFile->setAniInfo.clear();
        delete m_pAniFile;
        m_pAniFile = NULL;
    }
    if (m_pDaiFile)
    {
        map<unsigned long, DaiInfo*>::iterator iter = m_pDaiFile->setDaiInfo.begin();
        while(iter != m_pDaiFile->setDaiInfo.end())
        {
            DaiInfo* pDaiInfo = iter->second;
            if (pDaiInfo)
            {
                if (pDaiInfo->pFrameInfo)
                {
                    delete[] pDaiInfo->pFrameInfo;
                }
                delete pDaiInfo;
                pDaiInfo = NULL;
            }
            iter ++;
        }
        m_pDaiFile->setDaiInfo.clear();
        int nDirAmount = m_pDaiFile->setDir.size();
        int i;
        for(i = 0; i < nDirAmount; i ++)
        {
            char* pszStr = m_pDaiFile->setDir[i];
            if (pszStr)
            {
                delete[] pszStr;
            }
        }
        m_pDaiFile->setDir.clear();
        int nExtAmount = m_pDaiFile->setExt.size();
        for(i = 0; i < nExtAmount; i ++)
        {
            char* pszStr = m_pDaiFile->setExt[i];
            if (pszStr)
            {
                delete[] pszStr;
            }
        }
        m_pDaiFile->setExt.clear();
        int nStrAmount = m_pDaiFile->setStr.size();
        for(i = 0; i < nStrAmount; i ++)
        {
            char* pszStr = m_pDaiFile->setStr[i];
            if (pszStr)
            {
                delete[] pszStr;
            }
        }
        m_pDaiFile->setStr.clear();
        delete m_pDaiFile;
        m_pDaiFile = NULL;
    }
    if (m_pAniFileByOrder)
    {
        int nAmount = m_pAniFileByOrder->setAniInfo.size();
        for(int j = 0; j < nAmount; j ++)
        {
            AniInfoByOrder* pInfo = m_pAniFileByOrder->setAniInfo[j];
            if (pInfo)
            {
                int nStrAmounat = pInfo->setFrame.size();
                for(int i = 0; i < nStrAmounat; i++)
                {
                    char* pStr = pInfo->setFrame[i];
                    if (pStr)
                    {
                        delete[] pStr;
                    }
                }
                pInfo->setFrame.clear();
                if (pInfo->pszIndex)
                {
                    delete[] pInfo->pszIndex;
                }
                delete(pInfo);
                pInfo = NULL;
            }
        }
        m_pAniFileByOrder->setAniInfo.clear();
        delete m_pAniFileByOrder;
        m_pAniFileByOrder = NULL;
    }
    if (m_pDaiFileByOrder)
    {
        int nAmount = m_pDaiFileByOrder->setDaiInfo.size();
        for(int j = 0; j < nAmount; j ++)
        {
            DaiInfo* pDaiInfo = m_pDaiFileByOrder->setDaiInfo[j];
            if (pDaiInfo)
            {
                if (pDaiInfo->pFrameInfo)
                {
                    delete[] pDaiInfo->pFrameInfo;
                }
                delete pDaiInfo;
                pDaiInfo = NULL;
            }
        }
        m_pDaiFileByOrder->setDaiInfo.clear();
        int nDirAmount = m_pDaiFileByOrder->setDir.size();
        int i;
        for(i = 0; i < nDirAmount; i ++)
        {
            char* pszStr = m_pDaiFileByOrder->setDir[i];
            if (pszStr)
            {
                delete[] pszStr;
            }
        }
        m_pDaiFileByOrder->setDir.clear();
        int nExtAmount = m_pDaiFileByOrder->setExt.size();
        for(i = 0; i < nExtAmount; i ++)
        {
            char* pszStr = m_pDaiFileByOrder->setExt[i];
            if (pszStr)
            {
                delete[] pszStr;
            }
        }
        m_pDaiFileByOrder->setExt.clear();
        int nStrAmount = m_pDaiFileByOrder->setStr.size();
        for(i = 0; i < nStrAmount; i ++)
        {
            char* pszStr = m_pDaiFileByOrder->setStr[i];
            if (pszStr)
            {
                delete[] pszStr;
            }
        }
        m_pDaiFileByOrder->setStr.clear();
        delete m_pDaiFileByOrder;
        m_pDaiFileByOrder = NULL;
    }
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFile::Load(const char* pszFile)
{
    if (!pszFile)
    {
        return IDai::CODE_EXTERIOR_ERROR;
    }
    // assay file name ...
    char szDir[_MAX_DIR]		=	"";
    char szDrive[_MAX_PATH]		=	"";
    char szFileName[_MAX_FNAME]	=	"";
    char szExt[_MAX_EXT]			=	"";
    _splitpath(pszFile, szDrive, szDir, szFileName, szExt);
    strcpy(m_infoIndex.szFile, szFileName);
    _strupr(m_infoIndex.szFile);
    strcpy(m_infoIndex.szPath, szDir);
    _strupr(m_infoIndex.szPath);
    _strupr(szExt);
    if (strcmp(szExt, ".DAI") == 0)
    {
        return this->LoadDai(pszFile);
    }
    else if (strcmp(szExt, ".ANI") == 0)
    {
        return this->LoadAni(pszFile);
    }
    return IDai::CODE_UNKNOW_FEXT;
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFile::LoadAni(const char* pszFile)
{
    this->Clear();
    if (!pszFile)
    {
        return IDai::CODE_EXTERIOR_ERROR;
    }
    FILE* fp = fopen(pszFile, "r");
    if (!fp)
    {
        return IDai::CODE_OPENFILE_FALSE;
    }
    // create buffer to hold ani info
    m_pAniFile = new AniFileInfo;
    if (!m_pAniFile)
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    int nLineCounter = 0;
    while(true)
    {
        // scan title
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            fclose(fp);
            return IDai::SUCCESS;
        }
        // inc the line counter ...
        nLineCounter ++;
        // check whether current line is title, if title assay else skip ...
        static char szTitle[MAX_PATH] = "";
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
            this->Clear();
            m_uLastErrorLine = nLineCounter;
            return IDai::CODE_TITLEFORMAT_ERROE;
        }
        // get frame amount now
        int nFrameAmount = 0;
        if (1 != fscanf(fp, "FrameAmount=%d\n", &nFrameAmount))
        {
            fclose(fp);
            this->Clear();
            m_uLastErrorLine = nLineCounter;
            return IDai::CODE_FRAMEAMOUNT_ERROR;
        }
        if (nFrameAmount > MAX_ANIFRAMEAMOUNT && nFrameAmount < 0)
        {
            fclose(fp);
            this->Clear();
            m_uLastErrorLine = nLineCounter;
            return IDai::CODE_FRAMEAMOUNT_OVERFLOW;
        }
        // create new ani buffer to hold the frames ...
        AniInfo* pFrameInfo = new AniInfo;
        if (!pFrameInfo)
        {
            fclose(fp);
            this->Clear();
            m_uLastErrorLine = nLineCounter;
            return IDai::CODE_INTERIOR_ERROR;
        }
        unsigned long idIndex = this->Str2Id(szTitle);
        m_pAniFile->setAniInfo[idIndex] = pFrameInfo;
        // get frame now
        for(int i = 0; i < nFrameAmount; i ++)
        {
            static char szFormat[MAX_PATH] = "";
            sprintf(szFormat, "Frame%d=%s\n", i, "%s");
            char szFile[MAX_PATH] = "";
            if (1 != fscanf(fp, szFormat, szFile))
            {
                fclose(fp);
                this->Clear();
                m_uLastErrorLine = nLineCounter;
                return IDai::CODE_FRAMEINFO_ERROR;
            }
            char* pszFile = new char[strlen(szFile)+1];
            if (!pszFile)
            {
                fclose(fp);
                this->Clear();
                m_uLastErrorLine = nLineCounter;
                return IDai::CODE_INTERIOR_ERROR;
            }
            strcpy(pszFile, szFile);
            pFrameInfo->setFrame.push_back(pszFile);
        }
    }
    fclose(fp);
    return IDai::SUCCESS;
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFile::LoadDai(const char* pszFile)
{
    this->Clear();
    if (!pszFile)
    {
        return IDai::CODE_EXTERIOR_ERROR;
    }

    FILE* fp = fopen(pszFile, "rb");
    if (!fp)
    {
        return IDai::CODE_OPENFILE_FALSE;
    }
    m_pDaiFile = new DaiFileInfo;
    if (!m_pDaiFile)
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    char szTitle[8];
    DWORD dwVersion;
    fread(szTitle, sizeof(char), 8, fp);
    if (strcmp(szTitle, "ND_DAI") != 0)
    {
        return IDai::CODE_UNKNOW_FILEHEAD;
    }
    fread(&dwVersion, sizeof(DWORD), 1, fp);
    if (dwVersion != 1000)
    {
        return IDai::CODE_UNKNOW_VERSION;
    }
    // read path ...
  
    unsigned short usPathAmount;
    fread(&usPathAmount, sizeof(unsigned short), 1, fp);
    for(unsigned short i = 0; i < usPathAmount; i ++)
    {
        unsigned short usLength;
        fread(&usLength, sizeof(unsigned short), 1, fp);
        char* pszStr = new char[usLength];
        if (!pszStr)
        {
            fclose(fp);
            this->Clear();
            return IDai::CODE_INTERIOR_ERROR;
        }
        fread(pszStr, sizeof(char), usLength, fp);
        m_pDaiFile->setDir.push_back(pszStr);
    }
    // read Ext ...
    unsigned char ucExtAmount;
    fread(&ucExtAmount, sizeof(unsigned char), 1, fp);
    for(unsigned char i = 0; i < ucExtAmount; i ++)
    {
        unsigned short usLength;
        fread(&usLength, sizeof(unsigned short), 1, fp);
        char* pszStr = new char[usLength];
        if (!pszStr)
        {
            fclose(fp);
            this->Clear();
            return IDai::CODE_INTERIOR_ERROR;
        }
        fread(pszStr, sizeof(char), usLength, fp);
        m_pDaiFile->setExt.push_back(pszStr);
    }
    // read Str ...
    unsigned short usStrAmount;
    fread(&usStrAmount, sizeof(unsigned short), 1, fp);
    for(unsigned short i = 0; i < usStrAmount; i ++)
    {
        unsigned short usLength;
        fread(&usLength, sizeof(unsigned short), 1, fp);
        char* pszStr = new char[usLength];
        if (!pszStr)
        {
            fclose(fp);
            this->Clear();
            return IDai::CODE_INTERIOR_ERROR;
        }
        fread(pszStr, sizeof(char), usLength, fp);
        m_pDaiFile->setStr.push_back(pszStr);
    }
    // read Ani Amount ...
    unsigned long uAniAmount;
    fread(&uAniAmount, sizeof(DWORD), 1, fp);
    // read Ani Info...
    for(unsigned long i = 0; i < uAniAmount; i ++)
    {
        // frame amount  ...
        unsigned char ucFrameAmount;
        fread(&ucFrameAmount, sizeof(unsigned char), 1, fp);
        int nIndexLength = strlen(m_infoIndex.szFile);
        _strupr(m_infoIndex.szFile);
        ucFrameAmount = ucFrameAmount ^ m_infoIndex.szFile[i%nIndexLength];
        // new buffer ...
        DaiInfo* pAniInfo = new  DaiInfo;
        if (!pAniInfo)
        {
            fclose(fp);
            this->Clear();
            return IDai::CODE_INTERIOR_ERROR;
        }
        pAniInfo->pFrameInfo = NULL;
        pAniInfo->pFrameInfo = new DaiFrameInfo[ucFrameAmount];
        if (!pAniInfo->pFrameInfo)
        {
            fclose(fp);
            this->Clear();
            return IDai::CODE_INTERIOR_ERROR;
        }
        unsigned short	usIndexNameIndex;
        char			cIndexNameAdjust1;
        char			cIndexNameAdjust2;
        fread(&usIndexNameIndex, sizeof(unsigned short), 1, fp);
        fread(&cIndexNameAdjust1, sizeof(char), 1, fp);
        fread(&cIndexNameAdjust2, sizeof(char), 1, fp);
        // assign DAI member info ...
        pAniInfo->usIndexNameIndex	=	usIndexNameIndex;
        pAniInfo->cIndexNameAdjust1 =	cIndexNameAdjust1;
        pAniInfo->cIndexNameAdjust2	=	cIndexNameAdjust2;
        pAniInfo->ucFrameAmount		=	ucFrameAmount;
        // generate id
        char szStr[256];
        unsigned long uCode = this->UnZipStr(m_pDaiFile->setStr, szStr, 256, pAniInfo->usIndexNameIndex,
                                             pAniInfo->cIndexNameAdjust1, pAniInfo->cIndexNameAdjust2);
        if (uCode != IDai::SUCCESS)
        {
            fclose(fp);
            this->Clear();
            return uCode;
        }
        unsigned long uId = this->Str2Id(szStr);
        m_pDaiFile->setDaiInfo[uId] = pAniInfo;
        fread(pAniInfo->pFrameInfo, sizeof(DaiFrameInfo), ucFrameAmount, fp);
    }
    fclose(fp);


	//µ¼³öani
	/*std::string str[] = {"ani//bg.DAI","ani//cartoon.DAI","ani//cartoonwater.DAI","ani//champaign.DAI","ani//cloudworld.DAI","ani//Common.DAI","ani//Control.DAI","ani//curtain.DAI","ani//dragonridge.DAI","ani//effect.DAI","ani//EmotionIco.DAI","ani//family.DAI","ani//fenland.DAI","ani//gobiland.DAI","ani//iceland.DAI","ani//island.DAI","ani//ItemMinIcon.DAI","ani//lovelake.DAI","ani//Magic.DAI","ani//MapItemIcon.DAI","ani//mapscene-scene.DAI","ani//mapscene.DAI","ani//market.DAI","ani//Medal.DAI","ani//mine.DAI","ani//MiniMap.DAI","ani//new.DAI","ani//NpcFace.DAI","ani//PetFace.DAI","ani//play.DAI","ani//PlayerFace.DAI","ani//prison.DAI","ani//room.DAI","ani//Server.DAI","ani//treecity.DAI","ani//volcano.DAI","ani//weather.DAI","ani//wetland.DAI"};
	
	for(int i = 0;i < 38;i++)
	{
		pszFile = str[i].c_str();
		this->LoadDaiByOrder(pszFile);
		char aniPath[256] = {0};
		sprintf(aniPath,"%s.ANI",pszFile);
	
		this->SaveDai2Ani(aniPath);
	}*/
	
	
	//char aniPath[256] = {0};
	//sprintf(aniPath,"%s.ANI",pszFile);
	//
	//this->SaveDai2Ani(aniPath);
    return IDai::SUCCESS;
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFile::UnLoad()
{
    this->Clear();
    return IDai::SUCCESS;
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFile::LoadAniByOrder(const char* pszFile)
{
    this->Clear();
    if (!pszFile)
    {
        return IDai::CODE_EXTERIOR_ERROR;
    }
    FILE* fp = fopen(pszFile, "r");
    if (!fp)
    {
        return IDai::CODE_OPENFILE_FALSE;
    }
    // create buffer to hold ani info
    m_pAniFileByOrder = new AniFileInfoByOrder;
    if (!m_pAniFileByOrder)
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    int nLineCounter = 0;
    while(true)
    {
        // scan title
        static char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            fclose(fp);
            return IDai::SUCCESS;
        }
        // inc the line counter ...
        nLineCounter ++;
        // check whether current line is title, if title assay else skip ...
        static char szTitle[MAX_PATH] = "";
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
            this->Clear();
            m_uLastErrorLine = nLineCounter;
            return IDai::CODE_TITLEFORMAT_ERROE;
        }
        // get frame amount now
        int nFrameAmount = 0;
        if (1 != fscanf(fp, "FrameAmount=%d\n", &nFrameAmount))
        {
            fclose(fp);
            this->Clear();
            m_uLastErrorLine = nLineCounter;
            return IDai::CODE_FRAMEAMOUNT_ERROR;
        }
        if (nFrameAmount > MAX_ANIFRAMEAMOUNT && nFrameAmount < 0)
        {
            fclose(fp);
            this->Clear();
            m_uLastErrorLine = nLineCounter;
            return IDai::CODE_FRAMEAMOUNT_OVERFLOW;
        }
        // create new ani buffer to hold the frames ...
        AniInfoByOrder* pFrameInfo = new AniInfoByOrder;
        if (!pFrameInfo)
        {
            fclose(fp);
            this->Clear();
            m_uLastErrorLine = nLineCounter;
            return IDai::CODE_INTERIOR_ERROR;
        }
        pFrameInfo->pszIndex = new char[strlen(szTitle)+1];
        strcpy(pFrameInfo->pszIndex, szTitle);
        m_pAniFileByOrder->setAniInfo.push_back(pFrameInfo);
        // get frame now
        for(int i = 0; i < nFrameAmount; i ++)
        {
            static char szFormat[MAX_PATH] = "";
            sprintf(szFormat, "Frame%d=%s\n", i, "%s");
            char szFile[MAX_PATH] = "";
            if (1 != fscanf(fp, szFormat, szFile))
            {
                fclose(fp);
                this->Clear();
                m_uLastErrorLine = nLineCounter;
                return IDai::CODE_FRAMEINFO_ERROR;
            }
            char* pszFile = new char[strlen(szFile)+1];
            if (!pszFile)
            {
                fclose(fp);
                this->Clear();
                m_uLastErrorLine = nLineCounter;
                return IDai::CODE_INTERIOR_ERROR;
            }
            strcpy(pszFile, szFile);
            pFrameInfo->setFrame.push_back(pszFile);
        }
    }
    fclose(fp);
    return IDai::SUCCESS;
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFile::LoadDaiByOrder(const char* pszFile)
{
    this->Clear();
    if (!pszFile)
    {
        return IDai::CODE_EXTERIOR_ERROR;
    }
    FILE* fp = fopen(pszFile, "rb");
    if (!fp)
    {
        return IDai::CODE_OPENFILE_FALSE;
    }
    m_pDaiFileByOrder = new DaiFileInfoByOrder;
    if (!m_pDaiFileByOrder)
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    char szTitle[8];
    DWORD dwVersion;
    fread(szTitle, sizeof(char), 8, fp);
    if (strcmp(szTitle, "ND_DAI") != 0)
    {
        return IDai::CODE_UNKNOW_FILEHEAD;
    }
    fread(&dwVersion, sizeof(DWORD), 1, fp);
    if (dwVersion != 1000)
    {
        return IDai::CODE_UNKNOW_VERSION;
    }
    // read path ...
   
    unsigned short usPathAmount;
    fread(&usPathAmount, sizeof(unsigned short), 1, fp);
    for(unsigned short i = 0; i < usPathAmount; i ++)
    {
        unsigned short usLength;
        fread(&usLength, sizeof(unsigned short), 1, fp);
        char* pszStr = new char[usLength];
        if (!pszStr)
        {
            fclose(fp);
            this->Clear();
            return IDai::CODE_INTERIOR_ERROR;
        }
        fread(pszStr, sizeof(char), usLength, fp);
        m_pDaiFileByOrder->setDir.push_back(pszStr);
    }
    // read Ext ...
    unsigned char ucExtAmount;
    fread(&ucExtAmount, sizeof(unsigned char), 1, fp);
    for(unsigned char i = 0; i < ucExtAmount; i ++)
    {
        unsigned short usLength;
        fread(&usLength, sizeof(unsigned short), 1, fp);
        char* pszStr = new char[usLength];
        if (!pszStr)
        {
            fclose(fp);
            this->Clear();
            return IDai::CODE_INTERIOR_ERROR;
        }
        fread(pszStr, sizeof(char), usLength, fp);
        m_pDaiFileByOrder->setExt.push_back(pszStr);
    }
    // read Str ...
    unsigned short usStrAmount;
    fread(&usStrAmount, sizeof(unsigned short), 1, fp);
    for(unsigned short i = 0; i < usStrAmount; i ++)
    {
        unsigned short usLength;
        fread(&usLength, sizeof(unsigned short), 1, fp);
        char* pszStr = new char[usLength];
        if (!pszStr)
        {
            fclose(fp);
            this->Clear();
            return IDai::CODE_INTERIOR_ERROR;
        }
        fread(pszStr, sizeof(char), usLength, fp);
        m_pDaiFileByOrder->setStr.push_back(pszStr);
    }
    // read Ani Amount ...
    unsigned long uAniAmount;
    fread(&uAniAmount, sizeof(DWORD), 1, fp);
    // read Ani Info...
    // assay file name ...
    static char szDir[_MAX_DIR]		=	"";
    static char szDrive[_MAX_PATH]		=	"";
    static char szFileName[_MAX_FNAME]	=	"";
    static char szExt[_MAX_EXT]			=	"";
    _splitpath(pszFile, szDrive, szDir, szFileName, szExt);
    strcpy(m_infoIndex.szFile, szFileName);
    _strupr(m_infoIndex.szFile);
    strcpy(m_infoIndex.szPath, szDir);
    _strupr(m_infoIndex.szPath);
    for(unsigned long i = 0; i < uAniAmount; i ++)
    {
        // frame amount  ...
        unsigned char ucFrameAmount;
        fread(&ucFrameAmount, sizeof(unsigned char), 1, fp);
        int nIndexLength = strlen(m_infoIndex.szFile);
        ucFrameAmount = ucFrameAmount ^ m_infoIndex.szFile[i%nIndexLength];
        // new buffer ...
        DaiInfo* pAniInfo = new  DaiInfo;
        if (!pAniInfo)
        {
            fclose(fp);
            this->Clear();
            return IDai::CODE_INTERIOR_ERROR;
        }
        pAniInfo->pFrameInfo = NULL;
        pAniInfo->pFrameInfo = new DaiFrameInfo[ucFrameAmount];
        if (!pAniInfo->pFrameInfo)
        {
            fclose(fp);
            this->Clear();
            return IDai::CODE_INTERIOR_ERROR;
        }
        unsigned short	usIndexNameIndex;
        char			cIndexNameAdjust1;
        char			cIndexNameAdjust2;
        fread(&usIndexNameIndex, sizeof(unsigned short), 1, fp);
        fread(&cIndexNameAdjust1, sizeof(char), 1, fp);
        fread(&cIndexNameAdjust2, sizeof(char), 1, fp);
        // assign DAI member info ...
        pAniInfo->usIndexNameIndex	=	usIndexNameIndex;
        pAniInfo->cIndexNameAdjust1 =	cIndexNameAdjust1;
        pAniInfo->cIndexNameAdjust2	=	cIndexNameAdjust2;
        pAniInfo->ucFrameAmount		=	ucFrameAmount;
        // generate id
        m_pDaiFileByOrder->setDaiInfo.push_back(pAniInfo);
        fread(pAniInfo->pFrameInfo, sizeof(DaiFrameInfo), ucFrameAmount, fp);
    }
    fclose(fp);
    return IDai::SUCCESS;
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFile::SaveDai2Ani(const char* pszFile)
{
    if (!pszFile)
    {
        return IDai::CODE_EXTERIOR_ERROR;
    }
    if (!m_pDaiFileByOrder)
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    FILE* fp = fopen(pszFile, "wb");
    if (!fp)
    {
        return IDai::CODE_OPENFILE_FALSE;
    }
    int nRecordAmount = m_pDaiFileByOrder->setDaiInfo.size();
    for(int i = 0; i < nRecordAmount; i ++)
    {
        DaiInfo* pInfo = m_pDaiFileByOrder->setDaiInfo[i];
        if (pInfo)
        {
            // generate index
            int nStrAmount = m_pDaiFileByOrder->setStr.size();
            if (pInfo->usIndexNameIndex >= nStrAmount && pInfo->usIndexNameIndex != 65535
                    && pInfo->usIndexNameIndex != 65534)
            {
                fclose(fp);
                this->Clear();
                return IDai::CODE_ANIINDEX_OVERFLOW;
            }
            char szStr[_MAX_PATH];
            this->UnZipStr(m_pDaiFileByOrder->setStr, szStr, _MAX_PATH, pInfo->usIndexNameIndex,
                           pInfo->cIndexNameAdjust1, pInfo->cIndexNameAdjust2);
            fprintf(fp, "[%s]\n", szStr);
            // frame amount ...
            fprintf(fp, "FrameAmount=%d\n", pInfo->ucFrameAmount);
            // frame info ...
            // generate frame string ...
            if (!pInfo->pFrameInfo)
            {
                fclose(fp);
                this->Clear();
                return IDai::CODE_INTERIOR_ERROR;
            }
            for(int j = 0; j <  pInfo->ucFrameAmount; j ++)
            {
                DaiFrameInfo* pFrameInfo = pInfo->pFrameInfo + j;
                //static char szFrame[512];
                int nDirAmount = m_pDaiFileByOrder->setDir.size();
                if (pFrameInfo->usPath >= nDirAmount)
                {
                    fclose(fp);
                    this->Clear();
                    return IDai::CODE_DIRINDEX_OVERFLOW;
                }
                int nExtAmount = m_pDaiFileByOrder->setExt.size();
                if (pFrameInfo->ucExtendName >= nExtAmount)
                {
                    fclose(fp);
                    this->Clear();
                    return IDai::CODE_EXTINDEX_OVERFLOW;
                }
                int nStrAmount = m_pDaiFileByOrder->setStr.size();
                if (pFrameInfo->usFileNameIndex >= nStrAmount && pFrameInfo->usFileNameIndex != 65535 && pFrameInfo->usFileNameIndex != 65534)
                {
                    fclose(fp);
                    this->Clear();
                    return IDai::CODE_STRINDEX_OVERFLOW;
                }
                static char szFrameInfo[512];
                unsigned long uCode = this->UnZipFrame(m_pDaiFileByOrder->setStr, m_pDaiFileByOrder->setExt,
                                                       m_pDaiFileByOrder->setDir, szFrameInfo, 512, pFrameInfo->usPath, pFrameInfo->ucExtendName,
                                                       pFrameInfo->usFileNameIndex, pFrameInfo->cFileNameAdjust1, pFrameInfo->cFileNameAdjust2);
                if (uCode != IDai::SUCCESS)
                {
                    this->Clear();
                    fclose(fp);
                    return uCode;
                }
                fprintf(fp, "Frame%d=%s\n", j, szFrameInfo);
            }
            fprintf(fp, "\n");
        }
        else
        {
            fclose(fp);
            this->Clear();
            return IDai::CODE_INTERIOR_ERROR;
        }
    }
    fclose(fp);
    return IDai::SUCCESS;
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFile::SaveAni2Dai(const char* pszFile)
{
    // clear first ...
    if (!pszFile)
    {
        return IDai::CODE_EXTERIOR_ERROR;
    }
    if (!m_pAniFileByOrder)
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    if (m_pDaiFileByOrder)
    {
        int nAmount = m_pDaiFileByOrder->setDaiInfo.size();
        for(int j = 0; j < nAmount; j ++)
        {
            DaiInfo* pDaiInfo = m_pDaiFileByOrder->setDaiInfo[j];
            if (pDaiInfo)
            {
                if (pDaiInfo->pFrameInfo)
                {
                    delete[] pDaiInfo->pFrameInfo;
                }
                delete pDaiInfo;
                pDaiInfo = NULL;
            }
        }
        m_pDaiFileByOrder->setDaiInfo.clear();
        int nDirAmount = m_pDaiFileByOrder->setDir.size();
        int i;
        for(i = 0; i < nDirAmount; i ++)
        {
            char* pszStr = m_pDaiFileByOrder->setDir[i];
            if (pszStr)
            {
                delete[] pszStr;
            }
        }
        m_pDaiFileByOrder->setDir.clear();
        int nExtAmount = m_pDaiFileByOrder->setExt.size();
        for(i = 0; i < nExtAmount; i ++)
        {
            char* pszStr = m_pDaiFileByOrder->setExt[i];
            if (pszStr)
            {
                delete[] pszStr;
            }
        }
        m_pDaiFileByOrder->setExt.clear();
        int nStrAmount = m_pDaiFileByOrder->setStr.size();
        for(i = 0; i < nStrAmount; i ++)
        {
            char* pszStr = m_pDaiFileByOrder->setStr[i];
            if (pszStr)
            {
                delete[] pszStr;
            }
        }
        m_pDaiFileByOrder->setStr.clear();
        delete m_pDaiFileByOrder;
        m_pDaiFileByOrder = NULL;
    }
    m_pDaiFileByOrder = new DaiFileInfoByOrder;
    if (!m_pDaiFileByOrder)
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    FILE* fp = fopen(pszFile, "wb");
    if (!fp)
    {
        return IDai::CODE_OPENFILE_FALSE;
    }
    int nRecordAmount = m_pAniFileByOrder->setAniInfo.size();
   
    for(int i = 0; i < nRecordAmount; i++)
    {
        AniInfoByOrder* pAniInfo = m_pAniFileByOrder->setAniInfo[i];
        if (!pAniInfo)
        {
            return IDai::CODE_INTERIOR_ERROR;
        }
        DaiInfo* pInfo = new DaiInfo;
        if (!pInfo)
        {
            return IDai::CODE_INTERIOR_ERROR;
        }
        m_pDaiFileByOrder->setDaiInfo.push_back(pInfo);
        if (!pAniInfo->pszIndex)
        {
            return IDai::CODE_INTERIOR_ERROR;
        }
        this->ZipStr(m_pDaiFileByOrder->setStr, pAniInfo->pszIndex, pInfo->usIndexNameIndex,
                     pInfo->cIndexNameAdjust1, pInfo->cIndexNameAdjust2);
        // frame ...
        pInfo->ucFrameAmount = pAniInfo->setFrame.size();
        if (pInfo->ucFrameAmount >= 255)
        {
            return IDai::CODE_FRAMEAMOUNT_OVERFLOW;
        }
        pInfo->pFrameInfo = new DaiFrameInfo[pInfo->ucFrameAmount];
        if (!pInfo->pFrameInfo)
        {
            return IDai::CODE_INTERIOR_ERROR;
        }
        for(int j = 0; j < pInfo->ucFrameAmount; j ++)
        {
            char* pszFrame = pAniInfo->setFrame[j];
            if (!pszFrame)
            {
                return IDai::CODE_INTERIOR_ERROR;
            }
            DaiFrameInfo* pCurrentInfo = pInfo->pFrameInfo + j;
            this->ZipFrame(m_pDaiFileByOrder->setStr, m_pDaiFileByOrder->setExt, m_pDaiFileByOrder->setDir,
                           pszFrame, pCurrentInfo->usPath, pCurrentInfo->ucExtendName, pCurrentInfo->usFileNameIndex,
                           pCurrentInfo->cFileNameAdjust1, pCurrentInfo->cFileNameAdjust2);
        }
    }
    // now save ...
    char szTitle[8] = "ND_DAI";
    DWORD dwVersion = 1000;
    fwrite(szTitle, sizeof(char), 8, fp);
    fwrite(&dwVersion, sizeof(DWORD), 1, fp);
    // path...
    unsigned short usPathAmount = m_pDaiFileByOrder->setDir.size();
    fwrite(&usPathAmount, sizeof(unsigned short), 1, fp);
    for(unsigned short i = 0; i < usPathAmount; i ++)
    {
        char* pszDir = m_pDaiFileByOrder->setDir[i];
        if (!pszDir)
        {
            fclose(fp);
            return IDai::CODE_INTERIOR_ERROR;
        }
        unsigned short usLength = strlen(pszDir) + 1;
        fwrite(&usLength, sizeof(unsigned short), 1, fp);
        fwrite(pszDir, sizeof(char), usLength, fp);
    }
    // save Ext ...
    unsigned char ucExtAmount = m_pDaiFileByOrder->setExt.size();
    fwrite(&ucExtAmount, sizeof(unsigned char), 1, fp);
    for(unsigned char i = 0; i < ucExtAmount; i ++)
    {
        char* pszExt = m_pDaiFileByOrder->setExt[i];
        if (!pszExt)
        {
            fclose(fp);
            return IDai::CODE_INTERIOR_ERROR;
        }
        unsigned short usLength = strlen(pszExt) + 1;
        fwrite(&usLength, sizeof(unsigned short), 1, fp);
        fwrite(pszExt, sizeof(char), usLength, fp);
    }
    // save Str ...
    unsigned short usStrAmount = m_pDaiFileByOrder->setStr.size();
    fwrite(&usStrAmount, sizeof(unsigned short), 1, fp);
    for(unsigned short i = 0; i < usStrAmount; i ++)
    {
        char* pszStr = m_pDaiFileByOrder->setStr[i];
        if (!pszStr)
        {
            fclose(fp);
            return IDai::CODE_INTERIOR_ERROR;
        }
        unsigned short usLength = strlen(pszStr) + 1;
        fwrite(&usLength, sizeof(unsigned short), 1, fp);
        fwrite(pszStr, sizeof(char), usLength, fp);
    }
    // get filename
    // save record
    unsigned long uAniAmount = m_pDaiFileByOrder->setDaiInfo.size();
    fwrite(&uAniAmount, sizeof(DWORD), 1, fp);
    char szDir[_MAX_DIR]		=	"";
    char szDrive[_MAX_PATH]		=	"";
    char szFileName[_MAX_FNAME]	=	"";
    char szExt[_MAX_EXT]		=	"";
    _splitpath(pszFile, szDrive, szDir, szFileName, szExt);
    for(unsigned long i = 0; i < uAniAmount; i ++)
    {
        DaiInfo* pInfo = m_pDaiFileByOrder->setDaiInfo[i];
        if (!pInfo)
        {
            fclose(fp);
            return IDai::CODE_INTERIOR_ERROR;
        }
        if (!pInfo->pFrameInfo)
        {
            fclose(fp);
            return IDai::CODE_INTERIOR_ERROR;
        }
        // encrypt ...
        int nLength = strlen(szFileName);
        _strupr(szFileName);
        unsigned char ucFrameAmount = pInfo->ucFrameAmount ^ szFileName[i%nLength];
        fwrite(&ucFrameAmount, sizeof(unsigned char), 1, fp);
        fwrite(&pInfo->usIndexNameIndex, sizeof(unsigned short), 1, fp);
        fwrite(&pInfo->cIndexNameAdjust1, sizeof(char), 1, fp);
        fwrite(&pInfo->cIndexNameAdjust2, sizeof(char), 1, fp);
        fwrite(pInfo->pFrameInfo, sizeof(DaiFrameInfo), pInfo->ucFrameAmount, fp);
    }
    fclose(fp);
    return IDai::SUCCESS;
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFile::ZipStr(deque<char*>& setStr, const char* pszStr, unsigned short& usStrIndex, char& cAdjustA, char& cAdjustB)
{
    if (!pszStr)
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    DWORD dwLength = strlen(pszStr);
    if (dwLength == 1)
    {
        usStrIndex = 65535;
        cAdjustA = pszStr[0];
        cAdjustB = '\0';
        return IDai::SUCCESS;
    }
    if (dwLength == 2)
    {
        usStrIndex = 65534;
        cAdjustA = pszStr[0];
        cAdjustB = pszStr[1];
        return IDai::SUCCESS;
    }
    // get index
    char szIndex[_MAX_PATH];
    strcpy(szIndex, pszStr);
    szIndex[dwLength-2] = '\0';
    int nAmount = setStr.size();
    int i = 0;
    for(; i < nAmount; i ++)
    {
        char* pStr = setStr[i];
        if (pStr	&& strcmp(pStr, szIndex) == 0)
        {
            // found ...
            usStrIndex = i;
            break;
        }
    }
    if (i == nAmount)
    {
        if (i >= 65533)
        {
            return IDai::CODE_STRINDEX_OVERFLOW;
        }
        char* pszNewIndex = new char[strlen(szIndex)+1];
        if (!pszNewIndex)
        {
            return IDai::CODE_INTERIOR_ERROR;
        }
        strcpy(pszNewIndex, szIndex);
        setStr.push_back(pszNewIndex);
        usStrIndex = setStr.size() - 1;
    }
    cAdjustA = pszStr[dwLength-2];
    cAdjustB = pszStr[dwLength-1];
    return IDai::SUCCESS;
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFile::ZipFrame(deque<char*>& setStr, deque<char*>& setExt, deque<char*>& setDir,
                                   const char* pszFrame, unsigned short& usPathIndex, unsigned char& ucExtIndex,
                                   unsigned short& usNameIndex, char& cNameAdjustA, char& cNameAdjustB)
{
    if (!pszFrame)
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    // assay path
    char szDir[_MAX_DIR]			=	"";
    char szDrive[_MAX_PATH]		=	"";
    char szFileName[_MAX_FNAME]	=	"";
    char szExt[_MAX_EXT]			=	"";
    _splitpath(pszFrame, szDrive, szDir, szFileName, szExt);
    // continue ...
    int i;
    // direction ...
    int nDirAmount = setDir.size();
    for(i = 0; i < nDirAmount; i ++)
    {
        char* pszDir = setDir[i];
        if (pszDir && (strcmp(szDir, pszDir) == 0))
        {
            usPathIndex = i;
            break;
        }
    }
    if (i == nDirAmount)
    {
        if (i >= 65535)
        {
            return IDai::CODE_DIRINDEX_OVERFLOW;
        }
        char* pszNewDir = new char[strlen(szDir)+1];
        if (!pszNewDir)
        {
            return IDai::CODE_INTERIOR_ERROR;
        }
        strcpy(pszNewDir, szDir);
        setDir.push_back(pszNewDir);
        usPathIndex = setDir.size() - 1;
    }
    // ext name ...
    int nExtNameAmount = setExt.size();
    for(i = 0;  i < nExtNameAmount; i ++)
    {
        char* pszExt = setExt[i];
        if (pszExt && strcmp(pszExt, szExt) == 0)
        {
            ucExtIndex = i;
            break;
        }
    }
    if (i == nExtNameAmount)
    {
        if (i >= 255)
        {
            return IDai::CODE_EXTINDEX_OVERFLOW;
        }
        char* pszNewExt = new char[strlen(szExt)+1];
        if (!pszNewExt)
        {
            return IDai::CODE_INTERIOR_ERROR;
        }
        strcpy(pszNewExt, szExt);
        setExt.push_back(pszNewExt);
        ucExtIndex = setExt.size() - 1;
    }
    // file name ...
    unsigned long uCode = this->ZipStr(setStr, szFileName, usNameIndex, cNameAdjustA, cNameAdjustB);
    if (IDai::SUCCESS != uCode)
    {
        return uCode;
    }
    return IDai::SUCCESS;
}
//--------------------------------------------------------------------------------------
bool	CAniFile::CheckFile(const char* pszDir, const char* pszFileName)
{
    if (!pszDir || !pszFileName)
    {
        return false;
    }
    if (strcmp(this->m_infoIndex.szPath, pszDir) == 0 && strcmp(this->m_infoIndex.szFile, pszFileName) == 0)
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFile::UnZipStr(deque<char*>& setStr, char* pszBuffer, unsigned long uBufferSize,
                                   unsigned short& usStrIndex, char& cAdjustA, char& cAdjustB)
{
    if (!pszBuffer)
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    int nStrAmount = setStr.size();
    // special string index ...
    if (uBufferSize < 3)
    {
        return IDai::CODE_STRINGBUFFER_OVERFLOW;
    }
    if (usStrIndex == 65535)
    {
        pszBuffer[0] = cAdjustA;
        pszBuffer[1] = '\0';
        return IDai::SUCCESS;
    }
    if (usStrIndex == 65534)
    {
        pszBuffer[0] = cAdjustA;
        pszBuffer[1] = cAdjustB;
        pszBuffer[2] = '\0';
        return IDai::SUCCESS;
    }
    // check index
    int nAmount = setStr.size();
    if (usStrIndex >= nAmount)
    {
        return IDai::CODE_STRINDEX_OVERFLOW;
    }
    if (!setStr[usStrIndex])
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    // check size
    unsigned long uSize = 1 + 1 + 1 + sizeof(setStr[usStrIndex]);
    if (uSize > uBufferSize)
    {
        return IDai::CODE_STRINGBUFFER_OVERFLOW;
    }
    strcpy(pszBuffer, setStr[usStrIndex]);
    // adjust ...
    int nLength = strlen(pszBuffer);
    pszBuffer[nLength] = cAdjustA;
    pszBuffer[nLength+1] = cAdjustB;
    pszBuffer[nLength+2] = '\0';
    return IDai::SUCCESS;
}
//--------------------------------------------------------------------------------------

unsigned long	CAniFile::UnZipFrame(deque<char*>& setStr, deque<char*>& setExt, deque<char*>& setDir,
                                     char* pszBuffer, unsigned long uBufferSize, unsigned short& usPathIndex, unsigned char& ucExtIndex,
                                     unsigned short& usNameIndex, char& cNameAdjustA, char& cNameAdjustB)
{
    if (!pszBuffer)
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    // check index
    int nExtAmount = setExt.size();
    if (ucExtIndex >= nExtAmount)
    {
        return IDai::CODE_EXTINDEX_OVERFLOW;
    }
    int nDirAmount = setDir.size();
    if (usPathIndex >= nDirAmount)
    {
        return IDai::CODE_DIRINDEX_OVERFLOW;
    }
    if (!setExt[ucExtIndex] || !setDir[usPathIndex])
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    static char szFileName[512];
    unsigned long uCode = this->UnZipStr(setStr, szFileName, 512, usNameIndex, cNameAdjustA, cNameAdjustB);
    if (uCode != IDai::SUCCESS)
    {
        return uCode;
    }
    // check size
    unsigned long uSize = strlen(szFileName) + strlen(setExt[ucExtIndex]) + strlen(setDir[usPathIndex]) + 1;
    if (uBufferSize < uSize)
    {
        return IDai::CODE_STRINGBUFFER_OVERFLOW;
    }
    sprintf(pszBuffer, "%s%s%s", setDir[usPathIndex], szFileName, setExt[ucExtIndex]);
    return IDai::SUCCESS;
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFile::GetAni(const char* pszIndex, IFrameSet* pInfoFrame)
{
    if (!pszIndex && ! pInfoFrame)
    {
        return IDai::CODE_EXTERIOR_ERROR;
    }
    if (!m_pAniFile && !m_pDaiFile)
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    unsigned long uKey = this->Str2Id(pszIndex);
    // clear buffer if not empty
    pInfoFrame->Clear();
    if (m_pDaiFile)
    {
        // search ...
        map<unsigned long, DaiInfo*>::iterator iter =	m_pDaiFile->setDaiInfo.find(uKey);
        if (iter == m_pDaiFile->setDaiInfo.end())
        {
            return IDai::CODE_INFO_NOTFOUND;
        }
        // found ...
        DaiInfo* pInfo = iter->second;
        if (!pInfo)
        {
            return IDai::CODE_INTERIOR_ERROR;
        }
        for(int i = 0; i < pInfo->ucFrameAmount; i++)
        {
            if (!pInfo)
            {
                return IDai::CODE_INTERIOR_ERROR;
            }
            DaiFrameInfo* pFrameInfo =  pInfo->pFrameInfo + i;
            char szFrame[256];
            unsigned long uCode;
            uCode = this->UnZipFrame(m_pDaiFile->setStr, m_pDaiFile->setExt, m_pDaiFile->setDir,
                                     szFrame, 256, pFrameInfo->usPath, pFrameInfo->ucExtendName,
                                     pFrameInfo->usFileNameIndex, pFrameInfo->cFileNameAdjust1,
                                     pFrameInfo->cFileNameAdjust2);
            pInfoFrame->InsertFrame(szFrame);
            if (uCode != IDai::SUCCESS)
            {
                return uCode;
            }
        }
        return IDai::SUCCESS;
    }
    else if (m_pAniFile)
    {
        // search ...
        map<unsigned long, AniInfo*>::iterator iter =	m_pAniFile->setAniInfo.find(uKey);
        if (iter == m_pAniFile->setAniInfo.end())
        {
            return IDai::CODE_INFO_NOTFOUND;
        }
        AniInfo* pAniInfo = iter->second;
        if (!pAniInfo)
        {
            return IDai::CODE_INTERIOR_ERROR;
        }
        // found ...
        int nFrameAmount = 	pAniInfo->setFrame.size();
        for(int i = 0; i < nFrameAmount; i ++)
        {
            char* pszFile = pAniInfo->setFrame[i];
            if (!pszFile)
            {
                return IDai::CODE_INTERIOR_ERROR;
            }
            pInfoFrame->InsertFrame(pszFile);
        }
        return IDai::SUCCESS;
    }
    return IDai::CODE_INFO_NOTFOUND;
}
//--------------------------------------------------------------------------------------
