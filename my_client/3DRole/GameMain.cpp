
//**********************************************************
// 代码编辑器
//**********************************************************

#include "m_map.h"
#include "basefunc.h"
#include "3dgamemap.h"
#include "role.h"
#include "gameplayerset.h"
#include "hero.h"
#include "allmsg.h"
#include "gamemsg.h"
#include "allchanel.h"
#include "GameDataSet.h"
#include "Rc5_321216.h"
#include "LocalEncrypt.h"

//#define _NETWORK_DISABLE
//#define _ACCOUNT_DISABLE
#define	REPLAY_OK_STR		"ANSWER_OK"			// 申请新帐号或LOGIN时返回的成功标志串。

// Global Variables:
HWND	g_hMyWnd	= NULL;
UINT	g_uStatus	= _STATUS_NONE;

// constant ...
const unsigned char RC5PASSWORD_KEY[] = {0x3C, 0xDC, 0xFE, 0xE8, 0xC4, 0x54, 0xD6, 0x7E, 0x16, 0xA6, 0xF8, 0x1A, 0xE8, 0xD0, 0x38, 0xBE};

//////////////////////////////////////////////////////////////////////
int GameLoginServerEx(unsigned long uType, const char* pszAccount, const char* pszPsw, const char* pszServer, const char* pszServerName)
{
    g_objHero.Reset();
    // init network
    if (!g_objNetwork.Init(pszServer, _PORT_ACCOUNTSERVER))
    {
        g_uStatus	= _STATUS_EXIT;
        char szMsg[256];
        sprintf(szMsg, g_objGameDataSet.GetStr(10397));
        g_objEntrance.AddMsg(szMsg);
        ::Sleep(5000);
        return -1;
    }
    char szPwdCopy[32];
    strcpy(szPwdCopy, pszPsw);
    CRc5_321216* pRc5 = new(CRc5_321216);
    if (!pRc5)
    {
        return -1;
    }
    pRc5->Rc5InitKey(RC5PASSWORD_KEY);
    pRc5->Rc5Encrypt(szPwdCopy, 32);
    delete(pRc5);
    pRc5 = NULL;
    const char szVersionFile[] = "res.dat";
    FILE* fp = fopen(szVersionFile, "r");
    char szRes[_MAX_NAMESIZE] = "ERROR";
    DWORD dwRes = 0;
    if (fp)
    {
        if (fscanf(fp, "%s\n", szRes))
        {
            fclose(fp);
        }
    }
    CMsgLoginAccountEx msg;
    if (msg.Create(uType, pszAccount, szPwdCopy, pszServerName, szRes))
    {
        msg.Send();
    }
    if (pszServerName)
    {
        strcpy(g_objHero.m_szServerName, pszServerName);
    }
    g_uStatus	= _STATUS_ACCOUNT;
    return 0;
}
//////////////////////////////////////////////////////////////////////
int GameLoginServer(const char* pszAccount, const char* pszPsw, const char* pszServer, const char* pszServerName)
{
    g_objHero.Reset();
#ifdef _ACCOUNT_DISABLE
    // init network
    if (!g_objNetwork.Init(pszServer, _PORT_GAMESERVER))
    {
        g_uStatus	= _STATUS_EXIT;
        char szMsg[256];
        sprintf(szMsg, "服务器连接错误, 可能是由于服务器维护或网络堵塞引起，请稍候再登录。");
        g_objEntrance.AddMsg(szMsg);
        return -1;
    }
    g_uStatus	= _STATUS_LOGIN;
    CMsgLogin msg;
    if (msg.Create(_VERSION, pszAccount, pszPsw))
    {
        msg.Send();
    }
    return 0;
#else//--------------------- account server ----------------------------
    // init network
    if (!g_objNetwork.Init(pszServer, _PORT_ACCOUNTSERVER))
    {
        g_uStatus	= _STATUS_EXIT;
        char szMsg[256];
        sprintf(szMsg, "服务器连接错误, 可能是由于服务器维护或网络堵塞引起，请稍候再登录。");
        g_objEntrance.AddMsg(szMsg);
        ::Sleep(5000);
        return -1;
    }
    char szPwdCopy[16];
    strcpy(szPwdCopy, pszPsw);
    CRc5_321216* pRc5 = new(CRc5_321216);
    if (!pRc5)
    {
        return -1;
    }
    pRc5->Rc5InitKey(RC5PASSWORD_KEY);
    pRc5->Rc5Encrypt(szPwdCopy, 16);
    delete(pRc5);
    pRc5 = NULL;
    CMsgAccount msg;
    if (msg.Create(pszAccount, szPwdCopy, pszServerName))
    {
        msg.Send();
    }
    g_uStatus	= _STATUS_ACCOUNT;
    return 0;
#endif
}


//////////////////////////////////////////////////////////////////////
void GameScreenMsg(const char* pszMsg)
{
    if (!pszMsg)
    {
        return;
    }
    int nScrWidth = 0, nScrHeight = 0;
    CMyBitmap::GetScreenSize(nScrWidth, nScrHeight);
    int nFontWidth = 0, nFontHeight = 0;
    nFontHeight	= CMyBitmap::GetFontSize();
    nFontWidth	= nFontHeight / 2;
    // show time
    CMyBitmap::Begin3D ();
    CMyBitmap::ClearBuffer ( true, true, D3DCOLOR_XRGB ( 0, 0, 0 ) );
    int nShowX	= (nScrWidth - strlen(pszMsg) * nFontWidth) / 2;
    int nShowY	= (nScrHeight - nFontHeight) / 2;
    CMyBitmap::ShowString(nShowX, nShowY, 0x00ffffff, pszMsg);
    CMyBitmap::End3D ();
    CMyBitmap::Flip ();
}

//////////////////////////////////////////////////////////////////////
BOOL GameInit(HWND hWnd)
{
    g_hMyWnd	= hWnd;
    // init 3D graphic
    if (!CMyBitmap::Init3DEx (	hWnd,
                                _SCR_WIDTH,
                                _SCR_HEIGHT,
                                1,			// 1 - window mode  0 - full screen
                                1))			// back buffer count
    {
        return false;
    }
    // init font
    MYASSERT (CMyBitmap::GameFontCreate());
    // create hero
    //g_objHero.Create(2);
#ifdef _NETWORK_DISABLE
    MYASSERT (g_objGameMap.LoadDataMap ("map/map/desert.DMap"));
    g_objGameMap.m_idDoc = 1000;
    g_objHero.SetPos(501, 645);
    g_objGameMap.AddInteractiveObj(&g_objHero);
#endif
    g_objGameMsg.SetShowLines(6);
    return true;
}


int nFrameRate	= 0;
int GameMain()
{
    if (_STATUS_EXIT == g_uStatus)
    {
        const char* pszMsg	= g_objEntrance.GetLastMsg();
        if (pszMsg)
        {
            if (0 == _stricmp(REPLAY_OK_STR, pszMsg))
            {
                g_uStatus	= _STATUS_NONE;
            }
            else
            {
                ::MessageBox(NULL, pszMsg, g_objGameDataSet.GetStr(100001), MB_OK | MB_ICONERROR);
                g_uStatus	= _STATUS_DESTROY;
            }
        }
        return g_uStatus;
    }
    else if (_STATUS_REGISTER == g_uStatus)
    {
        GameScreenMsg(g_objGameDataSet.GetStr(100002));
        g_objNetwork.ProcessNetMsg();
        const char* pszMsg	= g_objRegister.GetLastMsg();
        if (pszMsg)
        {
            g_uStatus	= _STATUS_EXIT;
            g_objEntrance.AddMsg(pszMsg);
        }
        return g_uStatus;
    }
    else if (_STATUS_ACCOUNT == g_uStatus)
    {
        GameScreenMsg(g_objGameDataSet.GetStr(100003));
        g_objNetwork.ProcessNetMsg();
        return g_uStatus;
    }
    else if (_STATUS_LOGIN == g_uStatus)
    {
        GameScreenMsg(g_objGameDataSet.GetStr(100004));
        g_objNetwork.ProcessNetMsg();
        const char* pszMsg	= g_objEntrance.GetLastMsg();
        if (pszMsg)
        {
            if (0 != _stricmp(REPLAY_OK_STR, pszMsg))
            {
                g_objEntrance.AddMsg(pszMsg);
                g_uStatus	= _STATUS_EXIT;
            }
        }
        return g_uStatus;
    }
    // frame rate
    static DWORD timeLoop	= 0;
    DWORD timeNow	=::TimeGet();
    if (timeNow < timeLoop + 30)
    {
        ::Sleep(timeLoop + 30 - timeNow);
    }
    nFrameRate	= (nFrameRate + 1000 / __max(1, int(::TimeGet() - timeLoop))) / 2;
    timeLoop	=::TimeGet();
    // input process
    int nMouseX, nMouseY;
    int nEvent	=::MouseCheck(nMouseX, nMouseY);
    int nMapX, nMapY;
    g_objGameMap.Screen2Cell(nMouseX, nMouseY, nMapX, nMapY);
    if (nEvent)
    {
        /*
        CCommand cmd;
        //cmd.iType		=_COMMAND_WALK;
        cmd.iType		=_COMMAND_JUMP;
        cmd.iStatus		=_CMDSTATUS_BEGIN;
        cmd.posTarget.x	=nMapX;
        cmd.posTarget.y	=nMapY;

        g_objHero.GetClosestTargetPos(cmd.posTarget);
        g_objHero.SetCommand(&cmd);
        */
    }
    /*
    // npc ai
    for (int i=0; i<_MAX_NPC; i++)
    {
    	if (::RandGet(300) == 1)
    	{
    		int nMapX, nMapY;
    		g_objNpc[i].GetPos(nMapX, nMapY);

    		nMapX	+=(::RandGet(10)-5);
    		nMapY	+=(::RandGet(10)-5);

    		CCommand cmd;
    		cmd.iType		=_COMMAND_WALK;
    		cmd.iStatus		=_CMDSTATUS_BEGIN;
    		cmd.posTarget.x	=nMapX;
    		cmd.posTarget.y	=nMapY;

    		g_objNpc[i].GetClosestTargetPos(cmd.posTarget);
    		g_objNpc[i].SetCommand(&cmd);
    	}
    }
    */
    // process player set
    CMyPos posHeroAlign	= g_objHero.GetAlignPos();
    g_objPlayerSet.Process(posHeroAlign.x, posHeroAlign.y);
    // process game map
    g_objGameMap.Process(NULL);
    // view port
    CMyPos posWorld;
    g_objHero.GetWorldPos(posWorld);
    posWorld.x	-= _SCR_WIDTH / 2;
    posWorld.y	-= _SCR_HEIGHT / 2;
    g_objGameMap.SetViewPos(posWorld);
    // reset mouse event now
    ::MouseProcess();
    // show time
    CMyBitmap::Begin3D ();
    CMyBitmap::ClearBuffer ( true, false, D3DCOLOR_XRGB ( 255, 0, 0 ) );
    // show game objs
    g_objGameMap.Show();
    // show x?
    g_objPlayerSet.ShowX();
    // show game msg
    g_objGameMsg.Show();
    // show infos
    CMyPos posView;
    g_objGameMap.GetBgViewport(posView.x, posView.y);
    CMyPos posHero;
    g_objHero.GetPos(posHero);
    CMyPos posAlign	= g_objHero.GetAlignPos();
    char szMsg[256] = "";
    sprintf(szMsg, "Hero(%03d,%03d [%03d, %03d]), Dir:%d, Viewport(%03d,%03d)",
            posHero.x, posHero.y, posAlign.x, posAlign.y,
            g_objHero.GetDir(),
            posView.x, posView.y);
    CMyBitmap::ShowString(10, 10, 0xffffff, szMsg);
    extern DWORD g_dwEgLoad, g_dwImLoad, g_dwNmLoad;
    sprintf(szMsg, "FrameRate:%02d, Player:%02d, [%03d,%03d], Eg:%u, Im:%u, Nm:%u",
            nFrameRate,
            g_objPlayerSet.GetPlayerAmount(),
            nMapX, nMapY,
            g_dwEgLoad,
            g_dwImLoad,
            g_dwNmLoad);
    CMyBitmap::ShowString(10, 30, 0xffffff, szMsg);
    // show mouse pos
    int nScrX, nScrY;
    g_objGameMap.Cell2Screen(nMapX, nMapY, nScrX, nScrY);
    CMyBitmap::ShowLine(nScrX, nScrY - 16, nScrX - 32, nScrY, 0xff000000);
    CMyBitmap::ShowLine(nScrX, nScrY - 16, nScrX + 32, nScrY, 0xff000000);
    CMyBitmap::ShowLine(nScrX, nScrY + 16, nScrX - 32, nScrY, 0xff000000);
    CMyBitmap::ShowLine(nScrX, nScrY + 16, nScrX + 32, nScrY, 0xff000000);
    CMyBitmap::End3D ();
    CMyBitmap::Flip ();
    // process data set
    g_objGameDataSet.Process();
#ifndef _NETWORK_DISABLE
    // process net msg
    g_objNetwork.ProcessNetMsg();
#endif
    return g_uStatus;
}
