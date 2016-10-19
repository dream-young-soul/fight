
//**********************************************************
// 代码编辑器
//**********************************************************

#include <windows.h>

#include "mydatafile.H"
#include "basefunc.H"
#include "gamedataset.H"

// gobal
CGameDataSet g_objGameDataSet;

// const
const char	szCOMMONANI[]	= "ani/Common.ani";

const int   _MAX_ACTNAME    = 32;

const int	_MEM_CHECK_TIMER	= 2000;
const int	_LOW_MEM			= 4 * 1024 * 1024;
const int	_HIGH_MEM			= 20 * 1024 * 1024;
const int	_HIGHEST_DEADLINE	= 30000;
const int	_LOWEST_DEADLINE	= 3000;

const int	_3DOBJ_DEADLINE		= 30 * 1000;
/*const int	_3DMOTION_DEADLINE	= 10*1000;
const int	_3DTEXTURE_DEADLINE	= 10*1000;
const int	_3DEFFECT_DEADLINE	= 10*1000;
*/

// globle....
DWORD			g_dwDeadLine	=	10000;
// local...
CRITICAL_SECTION	csReadThread;
DEQUE_ANI			dequeDataToLoad;


HANDLE			hExitEvent	= NULL;		// exit event, set if u want terminate read process
HANDLE			hReadEvent	= NULL;		// read event, for process read data
HANDLE			hReadThread	= NULL;


#ifdef _ANALYSIS_ON
DWORD g_dw3DObj = 0, g_dw3DMotion = 0, g_dw3DWMotion = 0, g_dwTexture = 0, g_dw3DEffect = 0, g_dwDataAni = 0;

#endif
//--------------------------------------------------------------
void AddReadDeque(CAni* pAni, int nExigence)
{
    if (!pAni)
    {
        return;
    }
    ::EnterCriticalSection(&csReadThread);
    //------------------------------
    if (nExigence == EXIGENCE_NORMAL)
    {
        dequeDataToLoad.push_back(pAni);
    }
    else
    {
        dequeDataToLoad.push_front(pAni);
    }
    //------------------------------
    ::LeaveCriticalSection(&csReadThread);
    // come on, load my data now!
    ::SetEvent(hReadEvent);
}

//--------------------------------------------------------------
void UpPriority(CAni* pAni)
{
    if (!pAni)
    {
        return;
    }
    ::EnterCriticalSection(&csReadThread);
    //------------------------------
    if (pAni->GetStatus() == ANISTATUS_INIT)
    {
        int nAmount = dequeDataToLoad.size();
        for (int i = nAmount - 1; i > 0; i--)	// i>0
        {
            CAni* pTheAni = dequeDataToLoad[i];
            if (pTheAni == pAni)
            {
                dequeDataToLoad.erase(dequeDataToLoad.begin() + i);
                dequeDataToLoad.push_front(pAni);
                break;
            }
        }
    }
    //------------------------------
    ::LeaveCriticalSection(&csReadThread);
}


//--------------------------------------------------------------
// Name: DataReadThread
//
// Note: read thread process function
//
// In:	 none
//
// Return:	 DWORD type, always 0.
//--------------------------------------------------------------
DWORD WINAPI DataReadThread(LPVOID lpThreadParameter)
{
    HANDLE		eventHandles[2];
    eventHandles[0]	= hReadEvent;
    eventHandles[1] = hExitEvent;
    // loop waiting for player events. If the exit event is signaled
    // the thread will exit
    while (WaitForMultipleObjects(2, eventHandles, FALSE, INFINITE) == WAIT_OBJECT_0)
    {
        while(!dequeDataToLoad.empty())
        {
            CAni* pAni = NULL;
            try
            {
                ::EnterCriticalSection(&csReadThread);
                //------------------------------
                pAni = dequeDataToLoad.front();
                dequeDataToLoad.pop_front();
                pAni->SetStatus(ANISTATUS_LOADING);
                //------------------------------
                ::LeaveCriticalSection(&csReadThread);
                // loading.... cost lot's of time
                pAni->Load();
            }
            catch(...)
            {
                ::LogMsg("catch error in data load thread.");
                CDynamicAni* pDynaAni = (CDynamicAni*) (pAni);
                if (pDynaAni)
                {
                    ::LogMsg("crashed when loading %s :%s", pDynaAni->m_szIniFile, pDynaAni->m_szIndex);
                }
                else
                {
                    ::LogMsg("can't dynamic_cast the ani pointer");
                }
            }
        }
    }
    return 0;
}

//--------------------------------------------------------------
BOOL DataReadThreadInit(void)
{
    hReadEvent = CreateEvent	 (NULL,		// no security
                                  FALSE,	// auto reset
                                  FALSE,	// initial event reset
                                  NULL);	// no name
    if (!hReadEvent)
    {
        return false;
    }
    hExitEvent = CreateEvent  (NULL,		// no security
                               FALSE,	// auto reset
                               FALSE,	// initial event reset
                               NULL);	// no name
    if (!hExitEvent)
    {
        return false;
    }
    //---------------------------------
    dequeDataToLoad.clear();
    //---------------------------------
    DWORD dwIdThread;
    hReadThread		= CreateThread(NULL,				// default security
                                   0,				// default stack size
                                   DataReadThread,	// pointer to thread routine
                                   NULL,				// argument for thread
                                   0,				// start it right away
                                   &dwIdThread);
    if (!hReadThread)
    {
        return false;
    }
    SetThreadPriority(hReadThread, g_objGameDataSet.m_dwReadThreadPriority);
    return true;
}

//--------------------------------------------------------------
void DataReadThreadDestroy(void)
{
    while (true)
    {
        if (dequeDataToLoad.size() <= 0)
        {
            break;
        }
        Sleep(50);
    }
    if (hReadThread)
    {
        SetEvent(hExitEvent);
        Sleep(100);	// waiting for thread exit
        CloseHandle(hReadThread);
        hReadThread	= NULL;
    }
    if (hReadEvent)
    {
        CloseHandle(hReadEvent);
        hReadEvent	= NULL;
    }
    if (hExitEvent)
    {
        CloseHandle(hExitEvent);
        hExitEvent	= NULL;
    }
}


//--------------------------------------------------------------
// CGameDataSet
//--------------------------------------------------------------
CGameDataSet::CGameDataSet(void)
{
}

CGameDataSet::~CGameDataSet(void)
{
    this->Destroy();
    //------------------------------------
    ::DeleteCriticalSection(&csReadThread);
    extern CRITICAL_SECTION	sAniCritical;
    ::DeleteCriticalSection(&sAniCritical);
}

//--------------------------------------------------------------
void CGameDataSet::Init(void)
{
    DWORD m_dw3DMotionLife			=	30 * 1000;
    DWORD m_dw3DTextureLife			=	30 * 1000;
    DWORD m_dw3DEffectLife			=	30 * 1000;
    DWORD m_dwPuzzleLife			=	30 * 1000;
    DWORD m_dwPuzzlePreloadSize		=	1;
    DWORD m_dwReadThreadPriority	=	THREAD_PRIORITY_NORMAL;
    this->InitSystemPolicy();
    m_mapDataAni.clear();
    m_set3DObj.clear();
    m_set3DMotion.clear();
    m_set3DWeaponMotion.clear();
    m_set3DMantleMotion.clear();
    m_set3DMountMotion.clear();
    m_set3DTexture.clear();
    m_set3DEffect.clear();
    m_setEmotion.clear ();
    this->LoadRes("ini/3dobj.ini", m_mapResMesh);
    this->LoadRes("ini/3dtexture.ini", m_mapResTexture);
    this->LoadRes("ini/3dmotion.ini", m_mapResMotion);
    this->LoadRes("ini/3deffectobj.ini", m_mapResEffectObj);
    this->LoadRes("ini/weaponmotion.ini", m_mapResWeaponMotion);
    this->LoadRes("ini/mantlemotion.ini", m_mapResMantleMotion);
    this->LoadRes("ini/mountmotion.ini", m_mapResMountMotion);
    this->LoadRes("ini/sound.ini", m_mapSound);
    this->CreateItemEffectInfo();
    ::InitializeCriticalSection(&csReadThread);
    extern CRITICAL_SECTION	sAniCritical;
    ::InitializeCriticalSection(&sAniCritical);
    if (!::DataReadThreadInit())
    {
        ::ErrorOut("动态数据读入线程初始化错误！");
    }
    m_timerCheck = ::TimeGet();
    g_dwDeadLine	= 10000;
    // open pack data file
    this->InitDnp();
    m_objStrRes.Init();
    this->CreateEmotionInfo ();
}

//--------------------------------------------------------------
void CGameDataSet::InitSystemPolicy(void)
{
    // get total memory
    MEMORYSTATUS stat;
    ::GlobalMemoryStatus(&stat);
    char szTitle[64];
    if (stat.dwTotalPhys >= 400 * 1000 * 1000)
    {
        strcpy(szTitle, "MEM512");
    }
    else if (stat.dwTotalPhys >= 192 * 1000 * 1000)
    {
        strcpy(szTitle, "MEM256");
    }
    else
    {
        strcpy(szTitle, "MEM128");
    }
    const char szCommon[] = "ini/common.ini";
    char szSubtitle[128];
    int nData;
    strcpy(szSubtitle, "3DMotionLife");
    if (::IniDataGet(szCommon, szTitle, szSubtitle, nData))
    {
        m_dw3DMotionLife = nData;
    }
    strcpy(szSubtitle, "3DTextureLife");
    if (::IniDataGet(szCommon, szTitle, szSubtitle, nData))
    {
        m_dw3DTextureLife = nData;
    }
    strcpy(szSubtitle, "3DEffectLife");
    if (::IniDataGet(szCommon, szTitle, szSubtitle, nData))
    {
        m_dw3DEffectLife = nData;
    }
    strcpy(szSubtitle, "PuzzleLife");
    if (::IniDataGet(szCommon, szTitle, szSubtitle, nData))
    {
        m_dwPuzzleLife = nData;
    }
    strcpy(szSubtitle, "PuzzlePreloadSize");
    if (::IniDataGet(szCommon, szTitle, szSubtitle, nData))
    {
        m_dwPuzzlePreloadSize = nData;
    }
    strcpy(szSubtitle, "ReadThreadPriority");
    if (::IniDataGet(szCommon, szTitle, szSubtitle, nData))
    {
        switch(nData)
        {
        case -1:
            m_dwReadThreadPriority = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case 0:
            m_dwReadThreadPriority = THREAD_PRIORITY_NORMAL;
            break;
        case 1:
            m_dwReadThreadPriority = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        default:
            m_dwReadThreadPriority = THREAD_PRIORITY_NORMAL;
            break;
        }
    }
}
//--------------------------------------------------------------
void CGameDataSet::Destroy	(void)
{
    // got anyting to load by loading thread?
    ::DataReadThreadDestroy();
    // free pack data file
    ::MyDataFileClose();
    this->DestroyDataAni();
    this->Destroy3DMotion();
    this->Destroy3DObj();
    this->Destroy3DTexture();
    this->Destroy3DEffect();
    this->Destroy3DWeaponMotion();
    this->Destroy3DMantleMotion();
    this->DestroyItemEffectInfo();
    // res
    m_mapResMesh.clear();
    m_mapResMotion.clear();
    m_mapResTexture.clear();
    m_mapResEffectObj.clear();
    m_mapResWeaponMotion.clear();
    m_mapResMantleMotion.clear();
    m_objStrRes.Destroy();
    m_mapSound.clear();
    m_setEmotion.clear ();
    // quit 3d device now
    CMyBitmap::Quit3D();
    SAFE_DELETE(CAni::s_pDai);
    SAFE_DELETE(CAni::s_pFrameSet);
}

//--------------------------------------------------------------
void CGameDataSet::LoadRes(const char* pszResFile, MAP_RES& mapRes)
{
    if (!pszResFile)
    {
        return;
    }
    FILE* fp = fopen(pszResFile, "r");
    if (!fp)
    {
        ::ErrorOut("can't open file:%s！", pszResFile);
        return;
    }
    mapRes.clear();
    while(true)
    {
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        OBJID id = ID_NONE;
        char szFile[256]	= "";
        if (2 == sscanf(szLine, "%u=%s", &id, szFile))
        {
            string strFile = szFile;
            mapRes[id] = strFile;
        }
    }
    fclose(fp);
}

//--------------------------------------------------------------
void CGameDataSet::Process(void)
{
    this->ProcessDataAni();
    this->Process3DMotionSet();
    this->Process3DWeaponMotionSet();
    this->Process3DMantalMotionSet();
    //this->Process3DObjSet();
    this->Process3DTextureSet();
    this->Process3DEffectSet();
}

DWORD g_dwEgLoad = 0, g_dwImLoad = 0, g_dwNmLoad = 0;

//--------------------------------------------------------------
//--------------------------------------------------------------
CAni* CGameDataSet::GetDataAni	(const char* pszFileName, const char* pszTitle, int nExigence, DWORD dwLife)
{
    // check ...
    if ((!pszFileName || strlen(pszFileName) < 1) || (!pszTitle || strlen(pszTitle) < 1) )
    {
        return NULL;
    }
    // CreateID
    static char szTemp[256];
    strcpy(szTemp, pszFileName);
    strcat((char*)szTemp, pszTitle);
    extern DWORD Str2ID( const char * str );
    OBJID idAni = Str2ID(szTemp);
    // search ...
    MAP_DYNAMICANI::iterator iter = m_mapDataAni.find(idAni);
    if (iter != m_mapDataAni.end())
    {
        CDynamicAni* pDAni = iter->second;
        if (pDAni)
        {
            // found it!
            switch (pDAni->GetStatus())
            {
            case ANISTATUS_INIT:
            case ANISTATUS_LOADING:
                {
                    if (nExigence == EXIGENCE_IMMEDIATE)
                    {
                        g_dwEgLoad ++;
                        // still waiting load?
                        if (ANISTATUS_INIT == pDAni->GetStatus())
                        {
                            ::UpPriority(pDAni);
                        }
                        // we must waiting the accomplish of data loading
                        DWORD timeBegin = ::TimeGet();
                        while (ANISTATUS_INIT == pDAni->GetStatus()
                                || ANISTATUS_LOADING == pDAni->GetStatus())
                        {
                            ::Sleep(30);
                            if (::TimeGet() >= timeBegin + 3000)
                            {
                                ::LogMsg("Error: can't complete ani loading in 3 sec. ani(%s :%s)", pDAni->m_szIniFile, pDAni->m_szIndex);
                                timeBegin = ::TimeGet();
                            }
                        }
                        // loading complished
                        if (ANISTATUS_LOADED == pDAni->GetStatus())
                        {
                            pDAni->ResetLife();
                            return pDAni;
                        }
                        else
                        {
                            return NULL;
                        }
                    }
                    else
                    {
                        return NULL;
                    }
                }
                break;
            case ANISTATUS_LOADED:
                {
                    pDAni->ResetLife();
                    return pDAni;
                }
                break;
            case ANISTATUS_FAILED:
                {
#ifdef _DEBUG
                    //					::ErrorOut("ERROR: hahaha Ani:[%s] of [%s] load failed", pszTitle, pszFileName);
#endif
                    m_mapDataAni.erase(iter);
                    SAFE_DELETE (pDAni);
                    return NULL;
                }
                break;
            default:
                {
                    ::ErrorOut("ERROR: Ani:[%s] of [%s] got invalid status:%u CGameDataSet::GetDataAni()", pszTitle, pszFileName, pDAni->GetStatus());
                    m_mapDataAni.erase(iter);
                    SAFE_DELETE (pDAni);
                    return NULL;
                }
                break;
            }
        }
        else
        {
            ::LogMsg("Error: found invalid data ani in CGameDataSet::GetDataAni()");
            m_mapDataAni.erase(iter);
        }
    }
    // ani data not found, so we must create new one...
    //CAni::s_setAniFile.AddAniFile((char*)pszFileName);
    CAni::s_pDai->Load(pszFileName, false);
    CDynamicAni* pNewDAni = new CDynamicAni(pszFileName, pszTitle);
    if (!pNewDAni)
    {
        return NULL;
    }
    pNewDAni->SetLife(dwLife);
    if (EXIGENCE_IMMEDIATE == nExigence)
    {
        g_dwImLoad ++;
        if (pNewDAni->Load() && ANISTATUS_LOADED == pNewDAni->GetStatus())
        {
            m_mapDataAni[pNewDAni->m_idAni] = pNewDAni;
            return pNewDAni;
        }
        else
        {
#ifdef _DEBUG
            ::ErrorOut("ERROR: Ani:[%s] not found in [%s]", pszTitle, pszFileName);
#endif
            // Test if the resource is the second resource
            if ( strcmp ( pszFileName, "ani/Control2.Ani" ) == 0 )
            {
                ::PostCmd(CMD_RESOURCE_LOSE) ;
            }
            SAFE_DELETE (pNewDAni) ;
            return NULL;
        }
    }
    else
    {
        g_dwNmLoad ++;
        // set status
        pNewDAni->SetStatus(ANISTATUS_INIT);
        // keep in deque
        m_mapDataAni[pNewDAni->m_idAni] = pNewDAni;
        // add to read list
        ::AddReadDeque(pNewDAni, nExigence);
        return NULL;
    }
}

//--------------------------------------------------------------

void CGameDataSet::ProcessDataAni ()
{
    MAP_DYNAMICANI::iterator iter = m_mapDataAni.begin();
    while(iter != m_mapDataAni.end())
    {
        CDynamicAni* pAni = iter->second;
        if (pAni && ::TimeGet() - pAni->GetTimeBorn() >= pAni->GetLife() &&
                (ANISTATUS_LOADED == pAni->GetStatus() || ANISTATUS_FAILED == pAni->GetStatus()))
        {
            SAFE_DELETE(pAni);
            MAP_DYNAMICANI::iterator iterCopy = iter;
            iterCopy ++;
            m_mapDataAni.erase(iter);
            iter = iterCopy;
        }
        else
        {
            iter ++;
        }
    }
}

//--------------------------------------------------------------
void CGameDataSet::Destroy3DObj()
{
    MAP_3DOBJEX::iterator iter = m_set3DObj.begin();
    while(iter != m_set3DObj.end())
    {
        if (iter->second)
        {
            SAFE_DELETE(iter->second->p3DObj);
            SAFE_DELETE(iter->second->pDefaultMotion);
            SAFE_DELETE(iter->second);
        }
        iter ++;
    }
    m_set3DObj.clear();
}

//--------------------------------------------------------------
void CGameDataSet::Destroy3DMotion()
{
    MAP_3DMOTIONEX::iterator iter = m_set3DMotion.begin();
    while(iter != m_set3DMotion.end())
    {
        if (iter->second)
        {
            SAFE_DELETE(iter->second->p3DMotion);
            SAFE_DELETE(iter->second);
        }
        iter ++;
    }
    m_set3DMotion.clear();
}

//--------------------------------------------------------------
void CGameDataSet::Destroy3DWeaponMotion()
{
    MAP_3DMOTIONEX::iterator iter = m_set3DWeaponMotion.begin();
    while(iter != m_set3DWeaponMotion.end())
    {
        if (iter->second)
        {
            SAFE_DELETE(iter->second->p3DMotion);
            SAFE_DELETE(iter->second);
        }
        iter ++;
    }
    m_set3DWeaponMotion.clear();
}
//--------------------------------------------------------------
void CGameDataSet::Destroy3DMantleMotion()
{
    MAP_3DMOTIONEX::iterator iter = m_set3DMantleMotion.begin();
    while(iter != m_set3DMantleMotion.end())
    {
        if (iter->second)
        {
            SAFE_DELETE(iter->second->p3DMotion);
            SAFE_DELETE(iter->second);
        }
        iter ++;
    }
    m_set3DMantleMotion.clear();
}
//--------------------------------------------------------------
void CGameDataSet::Destroy3DMountMotion()
{
    MAP_3DMOTIONEX::iterator iter = m_set3DMountMotion.begin();
    while(iter != m_set3DMountMotion.end())
    {
        if (iter->second)
        {
            SAFE_DELETE(iter->second->p3DMotion);
            SAFE_DELETE(iter->second);
        }
        iter ++;
    }
    m_set3DMountMotion.clear();
}
//--------------------------------------------------------------
void CGameDataSet::Destroy3DTexture()
{
    MAP_3DTEXTUREEX::iterator iter = m_set3DTexture.begin();
    while(iter != m_set3DTexture.end())
    {
        if (iter->second)
        {
            SAFE_DELETE(iter->second->p3DTexture);
            SAFE_DELETE(iter->second);
        }
        iter ++;
    }
    m_set3DTexture.clear();
}
//--------------------------------------------------------------
void CGameDataSet::Destroy3DEffect()
{
    MAP_3DEFFECTEX::iterator iter = m_set3DEffect.begin();
    while(iter != m_set3DEffect.end())
    {
        if (iter->second)
        {
            SAFE_DELETE(iter->second->p3DEffect);
            SAFE_DELETE(iter->second);
        }
        iter ++;
    }
    m_set3DEffect.clear();
    int nAmount = m_set3DEffect.size();
}
//--------------------------------------------------------------
void CGameDataSet::DestroyDataAni()
{
    MAP_DYNAMICANI::iterator iter = m_mapDataAni.begin();
    while(iter != m_mapDataAni.end())
    {
        if (iter->second)
        {
            SAFE_DELETE(iter->second);
        }
        iter ++;
    }
    m_mapDataAni.clear();
}

DWORD g_dwMeshTime = 0;
DWORD g_dwMeshLoad = 0;
//--------------------------------------------------------------
C3DObj* CGameDataSet::Get3DObj	(OBJID id)
{
    MYASSERT (id != ID_NONE);
    // search first
    MAP_3DOBJEX::iterator iterF = m_set3DObj.find(id);
    if (iterF != m_set3DObj.end())
    {
        C3DObjInfo* pInfo = iterF->second;
        if (pInfo)
        {
            pInfo->timeBirth = ::TimeGet();
            return pInfo->p3DObj;
        }
    }
    // create new obj
    map<OBJID, string>::iterator iter = m_mapResMesh.find(id);
    if (iter == m_mapResMesh.end())
    {
        const OBJID id3DObjError = 500006;
        ::ErrorOut(g_objGameDataSet.GetStr(id3DObjError), id);
        return NULL;
    }
    char szFileName[_MAX_PATH] = "";
    strcpy(szFileName, iter->second.c_str());
    DWORD time0 = ::TimeGet();
    C3DObj* p3DObj = new C3DObj;
    MYASSERT (p3DObj);
    if (!p3DObj->Create(szFileName))
    {
        SAFE_DELETE(p3DObj);
        const OBJID idMeshError = 500007;
        ::ErrorOut(g_objGameDataSet.GetStr(idMeshError), szFileName);
        return NULL;
    }
    g_dwMeshTime += (::TimeGet() - time0);
    g_dwMeshLoad++;
    C3DMotion* p3DMotion = this->CreateNew3DMotion(szFileName);
    if (!p3DMotion)
    {
        SAFE_DELETE(p3DObj);
        const OBJID idMotionError = 500008;
        ::ErrorOut(g_objGameDataSet.GetStr(idMotionError), szFileName);
        return NULL;
    }
    // set default motion
    p3DObj->SetMotion(p3DMotion);
    // 3d obj info
    C3DObjInfo* pInfo = new C3DObjInfo;
    MYASSERT (pInfo);
    pInfo->id				= id;
    pInfo->p3DObj			= p3DObj;
    pInfo->pDefaultMotion	= p3DMotion;
    pInfo->timeBirth		= ::TimeGet();
    // add to set
    m_set3DObj[pInfo->id] = pInfo;
    return p3DObj;
}

//--------------------------------------------------------------
C3DObjInfo* CGameDataSet::Get3DObjInfo (OBJID id)
{
    MYASSERT (id != ID_NONE);
    MAP_3DOBJEX::iterator iter = m_set3DObj.find(id);
    if (iter != m_set3DObj.end())
    {
        C3DObjInfo* pInfo = iter->second;
        if (pInfo)
        {
            pInfo->timeBirth = ::TimeGet();
            return pInfo;
        }
    }
    return NULL;
}
//--------------------------------------------------------------
C3DMotion*  CGameDataSet::Get3DMantleMotion (OBJID id)
{
    if (id == ID_NONE)
    {
        return NULL;
    }
    MAP_3DMOTIONEX::iterator iter = m_set3DMantleMotion.find(id);
    if (m_set3DMantleMotion.end() != iter)
    {
        C3DMotionInfo* pInfo = iter->second;
        if (pInfo)
        {
            pInfo->timeBirth = ::TimeGet();
            return pInfo->p3DMotion;
        }
    }
    // create new
    C3DMotion* p3DObj = this->CreateNew3DMantleMotion(id);
    if (!p3DObj)
    {
        return NULL;
    }
    // create motion info
    C3DMotionInfo* pInfo = new C3DMotionInfo;
    if (!pInfo)
    {
        SAFE_DELETE(p3DObj);
        return NULL;
    }
    pInfo->id		 = id;
    pInfo->p3DMotion = p3DObj;
    pInfo->timeBirth = ::TimeGet();
    // add to set
    m_set3DMantleMotion[pInfo->id] = pInfo;
    return p3DObj;
}
//--------------------------------------------------------------
C3DMotion*  CGameDataSet::Get3DMountMotion (OBJID id)
{
    if (id == ID_NONE)
    {
        return NULL;
    }
    MAP_3DMOTIONEX::iterator iter = m_set3DMountMotion.find(id);
    if (m_set3DMountMotion.end() != iter)
    {
        C3DMotionInfo* pInfo = iter->second;
        if (pInfo)
        {
            pInfo->timeBirth = ::TimeGet();
            return pInfo->p3DMotion;
        }
    }
    // create new
    C3DMotion* p3DObj = this->CreateNew3DMountMotion(id);
    if (!p3DObj)
    {
        return NULL;
    }
    // create motion info
    C3DMotionInfo* pInfo = new C3DMotionInfo;
    if (!pInfo)
    {
        SAFE_DELETE(p3DObj);
        return NULL;
    }
    pInfo->id		 = id;
    pInfo->p3DMotion = p3DObj;
    pInfo->timeBirth = ::TimeGet();
    // add to set
    m_set3DMountMotion[pInfo->id] = pInfo;
    return p3DObj;
}

//--------------------------------------------------------------
C3DMotion*  CGameDataSet::Get3DWeaponMotion (OBJID id)
{
    if (id == ID_NONE)
    {
        return NULL;
    }
    MAP_3DMOTIONEX::iterator iter = m_set3DWeaponMotion.find(id);
    if (m_set3DWeaponMotion.end() != iter)
    {
        C3DMotionInfo* pInfo = iter->second;
        if (pInfo)
        {
            pInfo->timeBirth = ::TimeGet();
            return pInfo->p3DMotion;
        }
    }
    // create new
    C3DMotion* p3DObj = this->CreateNew3DWeaponMotion(id);
    if (!p3DObj)
    {
        return NULL;
    }
    // create motion info
    C3DMotionInfo* pInfo = new C3DMotionInfo;
    if (!pInfo)
    {
        SAFE_DELETE(p3DObj);
        return NULL;
    }
    pInfo->id		 = id;
    pInfo->p3DMotion = p3DObj;
    pInfo->timeBirth = ::TimeGet();
    // add to set
    m_set3DWeaponMotion[pInfo->id] = pInfo;
    return p3DObj;
}
//--------------------------------------------------------------

C3DMotion* CGameDataSet::Get3DMotion(OBJID id)
{
    if (id == ID_NONE)
    {
        return NULL;
    }
    MAP_3DMOTIONEX::iterator iter = m_set3DMotion.find(id);
    if (m_set3DMotion.end() != iter)
    {
        C3DMotionInfo* pInfo = iter->second;
        if (pInfo)
        {
            pInfo->timeBirth = ::TimeGet();
            return pInfo->p3DMotion;
        }
    }
    // create new
    C3DMotion* p3DObj = this->CreateNew3DMotion(id);
    if (!p3DObj)
    {
        return NULL;
    }
    // create motion info
    C3DMotionInfo* pInfo = new C3DMotionInfo;
    if (!pInfo)
    {
        SAFE_DELETE(p3DObj);
        return NULL;
    }
    pInfo->id		 = id;
    pInfo->p3DMotion = p3DObj;
    pInfo->timeBirth = ::TimeGet();
    // add to set
    m_set3DMotion[pInfo->id]	=	pInfo;
    return p3DObj;
}

//--------------------------------------------------------------
C3DTexture* CGameDataSet::Get3DTexture(OBJID id)
{
    // search first
    MAP_3DTEXTUREEX::iterator iter = m_set3DTexture.find(id);
    if (iter != m_set3DTexture.end())
    {
        C3DTextureInfo* pObj = iter->second;
        if (pObj)
        {
            pObj->timeBirth = ::TimeGet();
            return pObj->p3DTexture;
        }
    }
    // create new
    C3DTexture* p3DObj = this->CreateNew3DTexture(id);
    if (!p3DObj)
    {
        return NULL;
    }
    // add to set
    C3DTextureInfo* pInfo = new C3DTextureInfo;
    if (!pInfo)
    {
        SAFE_DELETE(p3DObj);
        return NULL;
    }
    pInfo->id		  = id;
    pInfo->p3DTexture = p3DObj;
    pInfo->timeBirth  = ::TimeGet();
    m_set3DTexture[pInfo->id]	=	pInfo;
    return p3DObj;
}
//--------------------------------------------------------------
C3DEffect* CGameDataSet::Get3DEffect(OBJID id)
{
    // search first
    MAP_3DEFFECTEX::iterator iter = m_set3DEffect.find(id);
    if (iter != m_set3DEffect.end())
    {
        C3DEffectInfo* pObj = iter->second;
        if (pObj)
        {
            pObj->timeBirth = ::TimeGet();
            return pObj->p3DEffect;
        }
    }
    // create new
    C3DEffect* p3DObj = this->CreateNew3DEffect(id);
    if (!p3DObj)
    {
        return NULL;
    }
    // add to set
    C3DEffectInfo* pInfo = new C3DEffectInfo;
    if (!pInfo)
    {
        SAFE_DELETE(p3DObj);
        return NULL;
    }
    pInfo->id = id;
    pInfo->p3DEffect = p3DObj;
    pInfo->timeBirth = ::TimeGet();
    m_set3DEffect[pInfo->id]	=	pInfo;
    return p3DObj;
}
//--------------------------------------------------------------
C3DMotion* CGameDataSet::CreateNew3DMotion(OBJID id)
{
    if (id == ID_NONE)
    {
        return NULL;
    }
    map<OBJID, string>::iterator iter = m_mapResMotion.find(id);
    if (iter == m_mapResMotion.end())
    {
        /*#ifdef _DEBUG
        		const OBJID idActionError = 500009;
        		::ErrorMsg(g_objGameDataSet.GetStr(idActionError), id);
        #else*/
        const OBJID idActionError = 500009;
        ::LogMsg(g_objGameDataSet.GetStr(idActionError), id);
        //#endif
        return NULL;
    }
    return this->CreateNew3DMotion(iter->second.c_str());
}
//--------------------------------------------------------------
C3DMotion* CGameDataSet::CreateNew3DMountMotion(OBJID id)
{
    if (id == ID_NONE)
    {
        return NULL;
    }
    map<OBJID, string>::iterator iter = m_mapResMountMotion.find(id);
    if (iter == m_mapResMountMotion.end())
    {
        return NULL;
    }
    return this->CreateNew3DMotion(iter->second.c_str());
}
//--------------------------------------------------------------
C3DMotion* CGameDataSet::CreateNew3DMantleMotion(OBJID id)
{
    if (id == ID_NONE)
    {
        return NULL;
    }
    map<OBJID, string>::iterator iter = m_mapResMantleMotion.find(id);
    if (iter == m_mapResMantleMotion.end())
    {
        //::ErrorMsg("错误：没有发现id[%u]的武器动作！", id);
        return NULL;
    }
    return this->CreateNew3DMotion(iter->second.c_str());
}
//--------------------------------------------------------------
C3DMotion* CGameDataSet::CreateNew3DWeaponMotion(OBJID id)
{
    if (id == ID_NONE)
    {
        return NULL;
    }
    map<OBJID, string>::iterator iter = m_mapResWeaponMotion.find(id);
    if (iter == m_mapResWeaponMotion.end())
    {
        //::ErrorMsg("错误：没有发现id[%u]的武器动作！", id);
        return NULL;
    }
    return this->CreateNew3DMotion(iter->second.c_str());
}

//--------------------------------------------------------------
C3DMotion* CGameDataSet::CreateNew3DMotion(const char* pszFileName)
{
    if (!pszFileName)
    {
        return NULL;
    }
    C3DMotion* pObj	= new C3DMotion;
    if (!pObj)
    {
        return NULL;
    }
    if (!pObj->Create((char*)pszFileName))
    {
        const OBJID idMotionError = 500010;
        ::ErrorMsg(g_objGameDataSet.GetStr(idMotionError), pszFileName);
        SAFE_DELETE(pObj);
        return NULL;
    }
    return pObj;
}

//--------------------------------------------------------------
C3DTexture* CGameDataSet::CreateNew3DTexture(OBJID id)
{
    if (id == ID_NONE)
    {
        return NULL;
    }
    map<OBJID, string>::iterator iter = m_mapResTexture.find(id);
    if (iter == m_mapResTexture.end())
    {
        const OBJID idTextureError = 500011;
        ::ErrorMsg(g_objGameDataSet.GetStr(idTextureError), id);
        return NULL;
    }
    const char* pszFileName = iter->second.c_str();
    MYASSERT (pszFileName);
    C3DTexture* pTexture = new C3DTexture;
    if (!pTexture)
    {
        return NULL;
    }
    if (!pTexture->Create(pszFileName))
    {
        SAFE_DELETE(pTexture);
        const OBJID idTextureError = 500012;
        ::ErrorOut(g_objGameDataSet.GetStr(idTextureError), pszFileName);
        return NULL;
    }
    return pTexture;
}

//--------------------------------------------------------------
C3DEffect* CGameDataSet::CreateNew3DEffect(OBJID id)
{
    if (id == ID_NONE)
    {
        return NULL;
    }
    map<OBJID, string>::iterator iter = m_mapResEffectObj.find(id);
    if (iter == m_mapResEffectObj.end())
    {
        const OBJID idEffect = 500013;
        ::ErrorMsg(g_objGameDataSet.GetStr(idEffect), id);
        return NULL;
    }
    char szFileName[_MAX_PATH] = "";
    strcpy(szFileName, iter->second.c_str());
    C3DEffect* pEffect = new C3DEffect;
    if (!pEffect)
    {
        return NULL;
    }
    if (!pEffect->Load(szFileName))
    {
        ::ErrorMsg("Error: CreateNew3DEffect(%s) 失败. ", szFileName);
        SAFE_DELETE(pEffect);
        return NULL;
    }
    return pEffect;
}

//--------------------------------------------------------------
void CGameDataSet::Process3DObjSet(void)
{
    MAP_3DOBJEX::iterator iter = m_set3DObj.begin();
    while(iter != m_set3DObj.end())
    {
        C3DObjInfo* pInfo = iter->second;
        if (!pInfo || ::TimeGet() - pInfo->timeBirth > _3DOBJ_DEADLINE)
        {
            if (pInfo)
            {
                SAFE_DELETE(pInfo->p3DObj);
                SAFE_DELETE(pInfo->pDefaultMotion);
                SAFE_DELETE(pInfo);
            }
            MAP_3DOBJEX::iterator iterCopy = iter;
            iterCopy ++;
            m_set3DObj.erase(iter);
            iter = iterCopy;
        }
        else
        {
            iter ++;
        }
    }
}
//--------------------------------------------------------------
void CGameDataSet::Process3DMotionSet(void)
{
    MAP_3DMOTIONEX::iterator iter = m_set3DMotion.begin();
    while(iter != m_set3DMotion.end())
    {
        C3DMotionInfo* pInfo = iter->second;
        if (!pInfo || ::TimeGet() - pInfo->timeBirth > m_dw3DMotionLife)
        {
            if (pInfo)
            {
                SAFE_DELETE(pInfo->p3DMotion);
                SAFE_DELETE(pInfo);
            }
            MAP_3DMOTIONEX::iterator iterCopy = iter;
            iterCopy ++;
            m_set3DMotion.erase(iter);
            iter = iterCopy;
        }
        else
        {
            iter ++;
        }
    }
}
//--------------------------------------------------------------
void CGameDataSet::Process3DWeaponMotionSet(void)
{
    MAP_3DMOTIONEX::iterator iter = m_set3DWeaponMotion.begin();
    while(iter != m_set3DWeaponMotion.end())
    {
        C3DMotionInfo* pInfo = iter->second;
        if (!pInfo || ::TimeGet() - pInfo->timeBirth > m_dw3DMotionLife)
        {
            if (pInfo)
            {
                SAFE_DELETE(pInfo->p3DMotion);
                SAFE_DELETE(pInfo);
            }
            MAP_3DMOTIONEX::iterator iterCopy = iter;
            iterCopy ++;
            m_set3DWeaponMotion.erase(iter);
            iter = iterCopy;
        }
        else
        {
            iter ++;
        }
    }
}
//--------------------------------------------------------------
void		CGameDataSet::Process3DMountMotionSet	(void)
{
    MAP_3DMOTIONEX::iterator iter = m_set3DMountMotion.begin();
    while(iter != m_set3DMountMotion.end())
    {
        C3DMotionInfo* pInfo = iter->second;
        if (!pInfo || ::TimeGet() - pInfo->timeBirth > m_dw3DMotionLife)
        {
            if (pInfo)
            {
                SAFE_DELETE(pInfo->p3DMotion);
                SAFE_DELETE(pInfo);
            }
            MAP_3DMOTIONEX::iterator iterCopy = iter;
            iterCopy ++;
            m_set3DMountMotion.erase(iter);
            iter = iterCopy;
        }
        else
        {
            iter ++;
        }
    }
}
//--------------------------------------------------------------
void		CGameDataSet::Process3DMantalMotionSet	(void)
{
    MAP_3DMOTIONEX::iterator iter = m_set3DMantleMotion.begin();
    while(iter != m_set3DMantleMotion.end())
    {
        C3DMotionInfo* pInfo = iter->second;
        if (!pInfo || ::TimeGet() - pInfo->timeBirth > m_dw3DMotionLife)
        {
            if (pInfo)
            {
                SAFE_DELETE(pInfo->p3DMotion);
                SAFE_DELETE(pInfo);
            }
            MAP_3DMOTIONEX::iterator iterCopy = iter;
            iterCopy ++;
            m_set3DMantleMotion.erase(iter);
            iter = iterCopy;
        }
        else
        {
            iter ++;
        }
    }
}
//--------------------------------------------------------------
void CGameDataSet::Process3DTextureSet(void)
{
    MAP_3DTEXTUREEX::iterator iter = m_set3DTexture.begin();
    while(iter != m_set3DTexture.end())
    {
        C3DTextureInfo* pInfo = iter->second;
        if (!pInfo || ::TimeGet() - pInfo->timeBirth > m_dw3DTextureLife)
        {
            if (pInfo)
            {
                SAFE_DELETE(pInfo->p3DTexture);
                SAFE_DELETE(pInfo);
            }
            MAP_3DTEXTUREEX::iterator iterCopy = iter;
            iterCopy ++;
            m_set3DTexture.erase(iter);
            iter = iterCopy;
        }
        else
        {
            iter ++;
        }
    }
}
//--------------------------------------------------------------
void CGameDataSet::Process3DEffectSet(void)
{
    MAP_3DEFFECTEX::iterator iter = m_set3DEffect.begin();
    while(iter != m_set3DEffect.end())
    {
        C3DEffectInfo* pInfo = iter->second;
        if (!pInfo || ::TimeGet() - pInfo->timeBirth > m_dw3DEffectLife)
        {
            if (pInfo)
            {
                SAFE_DELETE(pInfo->p3DEffect);
                SAFE_DELETE(pInfo);
            }
            MAP_3DEFFECTEX::iterator iterCopy = iter;
            iterCopy ++;
            m_set3DEffect.erase(iter);
            iter = iterCopy;
        }
        else
        {
            iter ++;
        }
    }
}
//--------------------------------------------------------------
OBJID CGameDataSet::Get3DTextureID(C3DTexture* pTexture)
{
    /*
    	int nAmount = m_set3DTexture.size();
    	for(int i = 0; i < nAmount; i++)
    	{
    		C3DTextureInfo* pInfo = m_set3DTexture[i];
    		if (pInfo->p3DTexture == pTexture)
    			return pInfo->id;
    	}*/
    return ID_NONE;
}
//--------------------------------------------------------------
CAni* CGameDataSet::GetItemMinIcon(OBJID idItemType, int nExigence)
{
    if (idItemType == ID_NONE)
    {
        return NULL;
    }
    else
    {
        char szAniFile[] = "ani/ItemMinIcon.Ani";
        char szTitle[64];
        sprintf(szTitle, "Item%d", idItemType);
        CAni* pAni = g_objGameDataSet.GetDataAni(szAniFile, szTitle, nExigence);
        if (!pAni)
        {
            pAni = g_objGameDataSet.GetDataAni(szAniFile, "ItemDefault", nExigence);
        }
        return pAni;
    }
}
//--------------------------------------------------------------
CAni* CGameDataSet::GetPlayerFaceIcon(int nFace, int nLook, int nExigence)
{
    char szAniFile[] = "ani/PlayerFace.Ani";
    char szTitle[64];
    int nRealFace = 10000 * (nFace + 1) + nLook;
    sprintf(szTitle, "Face%d", nRealFace);
    return g_objGameDataSet.GetDataAni(szAniFile, szTitle, nExigence);
}
//--------------------------------------------------------------
CAni* CGameDataSet::GetPetFaceIcon(int nFace, int nExigence)
{
    char szAniFile[] = "ani/PetFace.Ani";
    char szTitle[64];
    sprintf(szTitle, "Face%d", nFace);
    return g_objGameDataSet.GetDataAni(szAniFile, szTitle, nExigence);
}
//--------------------------------------------------------------
CAni* CGameDataSet::GetNpcFaceIcon(int nFace, int nExigence)
{
    char szAniFile[] = "ani/NpcFace.Ani";
    char szTitle[64];
    sprintf(szTitle, "Face%d", nFace);
    return g_objGameDataSet.GetDataAni(szAniFile, szTitle, nExigence);
}
//--------------------------------------------------------------
void CGameDataSet::GetObjAmount(DWORD& dw3DObj, DWORD& dw3DMotion, DWORD& dw3DWMotion, DWORD& dwTexture, DWORD& dw3DEffect, DWORD& dwDataAni)
{
    dw3DObj = m_set3DObj.size();
    dw3DMotion = m_set3DMotion.size();
    dw3DWMotion = m_set3DWeaponMotion.size();
    dwTexture = m_set3DTexture.size();
    dw3DEffect = m_set3DEffect.size();
    dwDataAni = m_mapDataAni.size();
}
//--------------------------------------------------------------
const char* CGameDataSet::GetSound(OBJID idSound)
{
    map<OBJID, string>::iterator iter = m_mapSound.find(idSound);
    if (iter != m_mapSound.end())
    {
        return iter->second.c_str();
    }
    return NULL;
}
//--------------------------------------------------------------
CAni* CGameDataSet::GetMedal(int nMedal, int nExigence)
{
    char szAniFile[] = "ani/Medal.Ani";
    char szTitle[64];
    sprintf(szTitle, "Medal%d", nMedal);
    return g_objGameDataSet.GetDataAni(szAniFile, szTitle, nExigence);
}
//--------------------------------------------------------------
void CGameDataSet::InitDnp(void)
{
    /*
    	const char szDisperseFile[] = "ini/DisperseFile.ini";
    	char szLine[512];
    	FILE* fp = fopen(szDisperseFile, "r");
    	if (!fp)
    		return;
    	while(true)
    	{
    		if (EOF == fscanf(fp, "%s\n", szLine))
    			break;
    		else
    			::Common_AddDnpDisperseFile(szLine);
    	}
    	fclose(fp);*/
    const char szDnpFile[] = "ini/dnpFile.ini";
    char szLine[512];
    FILE* 	fp = fopen(szDnpFile, "r");
    if (!fp)
    {
        return;
    }
    while(true)
    {
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        else
        {
            ::MyDnpFileOpen(szLine);
        }
    }
}
//--------------------------------------------------------------
CAni*	CGameDataSet::GetEmblemIcon(int nIndex, int nExigence)
{
    char szAniFile[] = "ani/Emblem.Ani";
    char szTitle[64];
    sprintf(szTitle, "Emblem%d", nIndex);
    return g_objGameDataSet.GetDataAni(szAniFile, szTitle, nExigence);
}
//--------------------------------------------------------------
void	CGameDataSet::CreateItemEffectInfo()
{
    char szIniFile[] = "ini/itemeffect.ini";
    FILE* fp = fopen(szIniFile, "r");
    if (fp)
    {
        char szLine[1024];
        while(fgets(szLine, 1023, fp))
        {
            OBJID id;
            char szEffect0[256];
            char szEffect1[256];
            char szEffect2[256];
            char szWaveFile[_MAX_PATH];
            if (5 == sscanf(szLine, "%u %s %s %s %s\n", &id, szEffect0, szEffect1, szEffect2,
                            szWaveFile))
            {
                ItemEffectInfo Info;
                Info.strItemUseEffect = szEffect0;
                Info.strMapItemEffect = szEffect1;
                Info.strItemBgEffect = szEffect2;
                Info.strItemDropSound = szWaveFile;
                m_mapItemEffectInfo[id] = Info;
            }
        }
        fclose(fp);
    }
}
//--------------------------------------------------------------
void	CGameDataSet::DestroyItemEffectInfo()
{
    m_mapItemEffectInfo.clear();
}
//--------------------------------------------------------------
ItemEffectInfo*		CGameDataSet::GetItemEffectInfo(OBJID id)
{
    std :: map<OBJID, ItemEffectInfo>::iterator iter = m_mapItemEffectInfo.find(id);
    if (iter != m_mapItemEffectInfo.end())
    {
        return &iter->second;
    }
    return NULL;
}
//--------------------------------------------------------------
void CGameDataSet::CreateEmotionInfo ()
{
    char szIniFile[] = "ini/EmotionIco.ini";
    char szLine[1024];
    FILE* fp = fopen(szIniFile, "r");
    if (fp)
    {
        while(fgets(szLine, 1023, fp))
        {
            int nIndex = 0;
            char szShutCut[128];
            if (2 == sscanf(szLine, "%d %s\n", &nIndex, szShutCut))
            {
                string str = szShutCut;
                m_setEmotion.push_back (str);
            }
        }
        fclose(fp);
    }
}
//--------------------------------------------------------------
int CGameDataSet::GetEmotionAmount ()
{
    return m_setEmotion.size ();
}
//--------------------------------------------------------------
const char* CGameDataSet::GetEmotionShutCut (int nIndex)
{
    int nAmount = m_setEmotion.size ();
    if (nIndex >= nAmount || nIndex < 0)
    {
        return NULL;
    }
    else
    {
        return m_setEmotion[nIndex].c_str();
    }
}
//--------------------------------------------------------------
CAni*	CGameDataSet::GetEmotionIcon(int nIndex, int nExigence)
{
    char szAniFile[] = "ani/EmotionIco.Ani";
    char szTitle[64];
    sprintf(szTitle, "ICON%d", nIndex);
    return g_objGameDataSet.GetDataAni(szAniFile, szTitle, nExigence);
}
//--------------------------------------------------------------

void CGameDataSet::ShowInfoBack(int x, int y, int nWidth, int nHeight)
{
    CAni* BackAni = g_objGameDataSet.GetDataAni ( "ani/control.ani",
                    "InfoBack",
                    EXIGENCE_IMMEDIATE ) ;
    RECT rect1 = { 2, 2, 30, 30 } ;
    RECT rect2 = { 0, 0, 2, 2 } ;
    RECT rect3 = { 30, 0, 32, 2 } ;
    RECT rect4 = { 0, 30, 2, 32 } ;
    RECT rect5 = { 30, 30, 32, 32 } ;
    RECT rect6 = { 2, 0, 30, 2 } ;
    RECT rect7 = { 0, 2, 2, 30 } ;
    if (BackAni != NULL)
    {
        BackAni->ShowEx( 0,	x, y,
                         &rect1,	nWidth, nHeight);
        BackAni->ShowEx( 0,	x, y,
                         &rect6, nWidth, 2);
        BackAni->ShowEx( 0,	x, y + nHeight,
                         &rect6,	nWidth, 2);
        BackAni->ShowEx( 0,	x, y,
                         &rect7,	2, nHeight);
        BackAni->ShowEx( 0,	x + nWidth, y,
                         &rect7,	2, nHeight);
        BackAni->ShowEx( 0,	x, y,
                         &rect2,	2, 2);
        BackAni->ShowEx( 0,	x + nWidth - 2 , y ,
                         &rect3,	2, 2);
        BackAni->ShowEx( 0,	x, y + nHeight - 2,
                         &rect4,	2, 2);
        BackAni->ShowEx( 0,	x + nWidth - 2, y + nHeight - 2,
                         &rect5,	2, 2);
    }
}

//--------------------------------------------------------------
const char* CGameDataSet::GetVersionInfo(void)
{
#ifdef _ENGLISH_VERSION
    return "English";
#else
    return this->GetStr(10426);
#endif
}
