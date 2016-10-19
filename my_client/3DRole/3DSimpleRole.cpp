
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DSimpleRole.cpp: implementation of the C3DSimpleRole class.
//
//////////////////////////////////////////////////////////////////////

#include "3DSimpleRole.h"
#include "GamedataSet.h"
#include "3dgamemap.h"
#include "Role.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DSimpleRole::C3DSimpleRole()
{
    m_nIndex			= -1;
    m_nLook				= -1;
    m_dwBegin			= ::TimeGet();
    m_idSimpleObj		= ID_NONE;
    m_idMotionStandBy	= ID_NONE;
    m_idMotionBlaze		= ID_NONE;
    m_bBlaze			= false;
    m_bSetFlash			= false;
    m_obj3DSimpleObj.SetOblique(false);
    m_obj3DSimpleObj.SetShadowType(CRole::SHADOW_NONE);
    m_pSound			= NULL;
    //	m_objEffectB.SetOblique(false);
    //	m_objEffectF.SetOblique(false);
}

C3DSimpleRole::~C3DSimpleRole()
{
}

//--------------------------------------------------------------------------
void C3DSimpleRole::Blaze()
{
    m_dwBegin			= ::TimeGet();	// reset timer...
    m_bBlaze			= true;			// blaze it ...
    const char szIniFile[] = "ini/3DSimpleRole.ini";
    char szTitle[64], szSubtitle[64];
    sprintf(szTitle, "Role%d", m_nIndex);
    // sound ...
    char szSoundFile[_MAX_PATH];
    strcpy(szSubtitle, "BlazeSound");
    if (m_pSound)
    {
        DXClose2DSound(m_pSound);
        m_pSound = NULL;
    }
    if (::IniStrGet(szIniFile, szTitle, szSubtitle, szSoundFile))
    {
        m_pSound = (CSound*)::DXPlaySound(szSoundFile);
    }
}
//--------------------------------------------------------------------------
void C3DSimpleRole::SetLook(int nLook)
{
    m_obj3DRole.SetLook(nLook);
}
//--------------------------------------------------------------------------
void C3DSimpleRole::SetHead(OBJID idHead)
{
    m_obj3DRole.SetHead(idHead);
}
//--------------------------------------------------------------------------
void C3DSimpleRole::Show(int nIndex, CMyPos posScr, int nZoomPercent, int nRotateX, int nStatus)
{
    switch(nIndex)
    {
    case _MALE_ROLE_A:
    case _MALE_ROLE_B:
    case _MALE_ROLE_C:
    case _FEMALE_ROLE_A:
    case _FEMALE_ROLE_B:
    case _FEMALE_ROLE_C:
        this->ShowRole(nIndex, posScr, nZoomPercent, nRotateX, nStatus);
        return;
    }
    if (nIndex < 0)
    {
        return;
    }
    if (nIndex != m_nIndex)
    {
        m_bBlaze = true;
        // select a new role ...
        m_dwBegin			= ::TimeGet();	// reset timer...
        m_nIndex			= nIndex;		// set new index ...
        // get more info from ini file
        const char szIniFile[] = "ini/3DSimpleRole.ini";
        char szTitle[64], szSubtitle[64];
        sprintf(szTitle, "Role%d", m_nIndex);
        strcpy(szSubtitle, "3DSimpleObjID");
        int nData = 0;
        MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, nData));
        m_idSimpleObj = nData;
        strcpy(szSubtitle, "3DStandByMotion");
        MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, nData));
        m_idMotionStandBy = nData;
        strcpy(szSubtitle, "3DBlazeMotion");
        MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, nData));
        m_idMotionBlaze = nData;
        // sound ...
        char szSoundFile[_MAX_PATH];
        strcpy(szSubtitle, "BlazeSound");
        if (m_pSound)
        {
            DXClose2DSound(m_pSound);
            m_pSound = NULL;
        }
        if (::IniStrGet(szIniFile, szTitle, szSubtitle, szSoundFile))
        {
            m_pSound = (CSound*)::DXPlaySound(szSoundFile);
        }
        // effect ...
        m_objEffectF.Clear();
        char szIndex[_MAX_STRING];
        strcpy(szSubtitle, "F3DEffect");
        if (::IniStrGet(szIniFile, szTitle, szSubtitle, szIndex))
        {
            m_objEffectF.Add(szIndex, false, 0, this->m_obj3DRole.m_nLook);
        }
        // effect ...
        m_objEffectB.Clear();
        szIndex[_MAX_STRING];
        strcpy(szSubtitle, "B3DEffect");
        if (::IniStrGet(szIniFile, szTitle, szSubtitle, szIndex))
        {
            m_objEffectB.Add(szIndex, false, 0, this->m_obj3DRole.m_nLook);
        }
        // create the 3DSimpleObj...
        m_obj3DSimpleObj.Create(m_idSimpleObj);
    }
    // begin show ...
    CMySize sizeCamera = {_SCR_WIDTH, _SCR_HEIGHT};
    CMyBitmap::GameCameraBuild(sizeCamera);
    CMyBitmap::GameCameraSet((float)(_SCR_WIDTH / 2),(float)( _SCR_HEIGHT / 2));
    // something like process...
    const DWORD dwFrameInterval = 33;
    if (m_bBlaze)
    {
        C3DMotion* p3DMotion = g_objGameDataSet.Get3DMotion(m_idMotionBlaze);
        m_obj3DSimpleObj.SetMotion(p3DMotion);
        int nFrameAmount = p3DMotion->GetFrameAmount();
        int nCurrentFrame = (::TimeGet() - m_dwBegin) / dwFrameInterval;
        if (nCurrentFrame >= nFrameAmount)
        {
            nCurrentFrame = nFrameAmount - 1;
            m_bBlaze = false;
            m_dwBegin = ::TimeGet();
        }
        m_obj3DSimpleObj.SetFrame(nCurrentFrame);
    }
    else
    {
        C3DMotion* p3DMotion = g_objGameDataSet.Get3DMotion(m_idMotionStandBy);
        m_obj3DSimpleObj.SetMotion(p3DMotion);
        int nFrameAmount = p3DMotion->GetFrameAmount();
        int nCurrentFrame = (::TimeGet() - m_dwBegin) / dwFrameInterval;
        m_obj3DSimpleObj.SetFrame(nCurrentFrame);
    }
    m_objEffectB.Process();
    m_objEffectB.Show(posScr);
    m_obj3DSimpleObj.SetPos(posScr.x, posScr.y, 0, nRotateX, nZoomPercent / 100.0f);
    m_obj3DSimpleObj.Draw2BG(0, 0);
    m_obj3DSimpleObj.ClearMatrix();
    m_objEffectF.Process();
    posScr.y += 10;
    m_objEffectF.Show(posScr);
}
//--------------------------------------------------------------------------
void C3DSimpleRole::SetFlashEffect()
{
    m_bSetFlash = true;
}
//--------------------------------------------------------------------------
BOOL C3DSimpleRole::ShowPet(int nIndex, CMyPos posScr, int nRotateX, int nStatus)
{
    if (m_nIndex != nIndex)
    {
        m_nIndex = nIndex;
        m_bBlaze = true;
        const char szIniFile[] = "ini/3DSimpleRole.ini";
        char szTitle[64], szSubtitle[64];
        sprintf(szTitle, "Pet%d", m_nIndex);
        strcpy(szSubtitle, "Look");
        int nData;
        MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, nData));
        m_idSimpleObj = nData;
        strcpy(szSubtitle, "Size");
        MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, nData));
        m_nZoomPercent = nData;
        strcpy(szSubtitle, "XPos");
        MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, nData));
        m_XPos = nData;
        strcpy(szSubtitle, "YPos");
        MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, nData));
        m_YPos = nData;
        strcpy(szSubtitle, "3DStandByMotion");
        MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, nData));
        m_idMotionStandBy = nData;
        strcpy(szSubtitle, "3DBlazeMotion");
        MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, nData));
        m_idMotionBlaze = nData;
        // create the 3DSimpleObj...
        m_obj3DSimpleObj.Create(m_idSimpleObj);
        C3DMotion* p3DMotion = g_objGameDataSet.Get3DMotion(m_idMotionStandBy);
        m_obj3DSimpleObj.SetMotion(p3DMotion);
        m_dwFrameAmount = p3DMotion->GetFrameAmount();
        m_dwFlashDelayTime = 0;
    }
    // begin show ...
    // something like process...
    BOOL bReturn = false;
    const DWORD dwFrameInterval = 33;
    DWORD dwFrame = (::TimeGet() - m_dwBegin) / dwFrameInterval;
    if (dwFrame > m_dwFrameAmount)
    {
        dwFrame = 0;
        m_dwBegin = ::TimeGet();
        C3DMotion* p3DMotion = NULL;
        if (!m_bBlaze)
        {
            p3DMotion = g_objGameDataSet.Get3DMotion(m_idMotionBlaze);
            m_obj3DSimpleObj.SetMotion(p3DMotion);
            m_bBlaze = true;
        }
        else
        {
            p3DMotion = g_objGameDataSet.Get3DMotion(m_idMotionStandBy);
            m_obj3DSimpleObj.SetMotion(p3DMotion);
            m_bBlaze = false;
        }
        m_dwFrameAmount = p3DMotion->GetFrameAmount();
    }
    m_obj3DSimpleObj.SetFrame(dwFrame);
    CMyPos posWorld, posMap;
    g_objGameMap.Screen2World(posScr.x - m_XPos, posScr.y - m_YPos, posWorld.x, posWorld.y);
    g_objGameMap.World2Bg(posWorld.x, posWorld.y, posMap.x, posMap.y);
    m_obj3DSimpleObj.SetPos(posMap.x, posMap.y, 0, nRotateX, m_nZoomPercent / 100.0f);
    CMyPos posView;
    g_objGameMap.GetViewPos(posView);
    int nViewportBgX, nViewportBgY;
    g_objGameMap.World2Bg(posView.x, posView.y, nViewportBgX, nViewportBgY);
    if (m_bSetFlash)
    {
        const char szIniFile[] = "ini/3DSimpleRole.ini";
        char szTitle[64], szSubtitle[64];
        sprintf(szTitle, "Pet%d", m_nIndex);
        // effect ...
        m_objEffectF.Clear();
        char szIndex[_MAX_STRING];
        strcpy(szSubtitle, "F3DEffect");
        if (::IniStrGet(szIniFile, szTitle, szSubtitle, szIndex))
        {
            m_objEffectF.Add(szIndex, false, 0, this->m_obj3DRole.m_nLook);
        }
        m_bSetFlash = FALSE;
        m_dwFlashDelayTime = ::TimeGet();
    }
    int PET_FLASHDELAY_TIME = 1000;
    if (m_dwFlashDelayTime != 0 && (int)(::TimeGet() - m_dwFlashDelayTime) >= PET_FLASHDELAY_TIME )
    {
        m_dwFlashDelayTime  = 0;
    }
    if (m_dwFlashDelayTime == 0)
    {
        m_obj3DSimpleObj.Draw2BG(nViewportBgX, nViewportBgY);
    }
    m_objEffectF.Process();
    m_objEffectF.Show(posMap);
    m_obj3DSimpleObj.ClearMatrix();
    return bReturn;
}
//--------------------------------------------------------------------------
BOOL C3DSimpleRole::ShowRole(int nIndex, CMyPos posScr, int nZoomPercent, int nRotateX, int nStatus)
{
    if (m_nIndex != nIndex)
    {
        m_nIndex = nIndex;
        m_bBlaze = true;
        const char szIniFile[] = "ini/3DSimpleRole.ini";
        char szTitle[64], szSubtitle[64];
        sprintf(szTitle, "Role%d", m_nIndex);
        strcpy(szSubtitle, "Look");
        int nData;
        MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, nData));
        m_obj3DRole.SetLook(nData);
        strcpy(szSubtitle, "Hair");
        MYASSERT(::IniDataGet(szIniFile, szTitle, szSubtitle, nData));
        m_obj3DRole.SetArmet(nData);
        strcpy(szSubtitle, "Armor");
        if (::IniDataGet(szIniFile, szTitle, szSubtitle, nData))
        {
            m_obj3DRole.SetArmor(nData);
        }
        m_nSimpRoleAction = _ACTION_STANDBY;
        m_dwFrameAmount = m_obj3DRole.SetAction(nIndex * 1000 + m_nSimpRoleAction);
        // effect ...
        m_objEffectF.Clear();
        char szIndex[_MAX_STRING];
        strcpy(szSubtitle, "F3DEffect");
        if (::IniStrGet(szIniFile, szTitle, szSubtitle, szIndex))
        {
            m_objEffectF.Add(szIndex, false, 0, this->m_obj3DRole.m_nLook);
        }
        // effect ...
        m_objEffectB.Clear();
        szIndex[_MAX_STRING];
        strcpy(szSubtitle, "B3DEffect");
        if (::IniStrGet(szIniFile, szTitle, szSubtitle, szIndex))
        {
            m_objEffectB.Add(szIndex, false, 0, this->m_obj3DRole.m_nLook);
        }
    }
    // begin show ...
    CMySize sizeCamera = {_SCR_WIDTH, _SCR_HEIGHT};
    CMyBitmap::GameCameraBuild(sizeCamera);
    CMyBitmap::GameCameraSet((float)(_SCR_WIDTH / 2), (float)(_SCR_HEIGHT / 2));
    // something like process...
    BOOL bReturn = false;
    m_SimpleRolePos.x = posScr.x;
    m_SimpleRolePos.y = posScr.y;
    const DWORD dwFrameInterval = 33;
    int nZoomSpeed = 0;
    DWORD dwFrame = (::TimeGet() - m_dwBegin) / dwFrameInterval;
    if (dwFrame >= m_dwFrameAmount)
    {
        switch(nStatus)
        {
        case 1:
            if (m_nSimpRoleAction == _ACTION_STANDBY)
            {
                m_nSimpRoleRotate = nRotateX;
                m_nSimpRoleZoom = nZoomPercent;
                m_nSimpRoleAction = _ACTION_STANDBY;
                m_dwFrameAmount = m_obj3DRole.SetAction(nIndex * 1000 + m_nSimpRoleAction);
            }
            break;
        case 2:
            if (m_nSimpRoleAction == _ACTION_STANDBY)
            {
                m_nSimpRoleAction = _ACTION_DIEFLY;
                m_dwFrameAmount = m_obj3DRole.SetAction(nIndex * 1000 + m_nSimpRoleAction);
            }
            else if (m_nSimpRoleAction == _ACTION_DIEFLY)
            {
                bReturn = true;
                m_nSimpRoleAction = _ACTION_DIEFLYEND;
                m_dwFrameAmount = m_obj3DRole.SetAction(nIndex * 1000 + m_nSimpRoleAction);
            }
            else if (m_nSimpRoleAction == _ACTION_DIEFLYEND)
            {
                m_nSimpRoleRotate = 0;
                m_dwFrameAmount = m_obj3DRole.SetAction(nIndex * 1000 + m_nSimpRoleAction);
            }
            break;
        case 3:
            if (m_nSimpRoleAction == _ACTION_WALKBACK)
            {
                bReturn = true;
            }
            m_nSimpRoleAction = _ACTION_WALKBACK;
            m_dwFrameAmount = m_obj3DRole.SetAction(nIndex * 1000 + m_nSimpRoleAction);
            break;
        default:
            break;
        }
        m_dwBegin = ::TimeGet();
        dwFrame = 0;
    }
    m_dwFrameAmount = m_obj3DRole.SetAction(m_obj3DRole.m_nActionType);
    m_obj3DRole.SetFrame(dwFrame);
    m_obj3DRole.SetPos(posScr.x , posScr.y , 0, nRotateX, nZoomPercent * 1.0f / 100);
    m_obj3DRole.SetRotateX(0);
    m_obj3DRole.SetLightOffset(CRole::SHADOW_NONE, 300, -300, -1000, 0.8f, 0.1f, 0.1f, 0.1f);
    m_obj3DRole.Draw(posScr.x , posScr.y);
    //	posScr.y += 10;
    return bReturn;
}

//--------------------------------------------------------------------------
