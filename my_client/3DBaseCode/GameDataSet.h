
//**********************************************************
// 代码编辑器
//**********************************************************

#ifndef _GAMEDATASET_H
#define _GAMEDATASET_H

//-------------------------------------------------------------------
#include "Ani.h"
#include "ND_Bitmap.h"
#include "3DEffect.h"
#include "StrRes.h"
//-------------------------------------------------------------------

#include <map>
#include <deque>
#include <string>
using namespace std;

// type define
typedef struct
{
    std::string		strMapItemEffect;
    std::string		strItemUseEffect;
    std::string		strItemBgEffect;
    std::string		strItemDropSound;
} ItemEffectInfo;

typedef struct
{
    OBJID	id;
    C3DObj*	p3DObj;
    C3DMotion* pDefaultMotion;
    DWORD	timeBirth;
} C3DObjInfo;

typedef struct
{
    CHAR	szFile[128];
} CResourceInfo;

typedef struct
{
    OBJID	id;
    C3DMotion* p3DMotion;
    DWORD	timeBirth;
} C3DMotionInfo;

typedef struct
{
    OBJID	id;
    C3DTexture* p3DTexture;
    DWORD	timeBirth;
} C3DTextureInfo;

typedef struct
{
    OBJID	id;
    C3DEffect* p3DEffect;
    DWORD	timeBirth;
} C3DEffectInfo;

#include <deque>
using namespace std;

typedef map<OBJID, C3DObjInfo* >	MAP_3DOBJEX;
typedef map<OBJID, C3DMotionInfo* >	MAP_3DMOTIONEX;
typedef map<OBJID, C3DTextureInfo* >	MAP_3DTEXTUREEX;
typedef map<OBJID, C3DEffectInfo* >	MAP_3DEFFECTEX;

typedef map<OBJID, string>		MAP_RES;
class CAni;
typedef deque<CAni* >  DEQUE_ANI;

//--------------------------------------------------
enum {EXIGENCE_NORMAL = 0, EXIGENCE_ABOVENORMAL, EXIGENCE_IMMEDIATE};
//---------------------------------------------------

class CGameDataSet
{
public:
    CGameDataSet(void);
    ~CGameDataSet(void);

    void Init			(void);
    void Destroy		(void);
    void Process		(void);
    void InitDnp		(void);
    CAni*		GetItemMinIcon		(OBJID idType, int nExigence = EXIGENCE_NORMAL);
    CAni*		GetPlayerFaceIcon	(int nFace, int nLook, int nExigence = EXIGENCE_NORMAL);
    CAni*		GetEmblemIcon		(int nIndex, int nExigence = EXIGENCE_NORMAL);

    CAni*		GetPetFaceIcon		(int nFace, int nExigence = EXIGENCE_NORMAL);
    CAni*		GetNpcFaceIcon		(int nFace, int nExigence = EXIGENCE_NORMAL);
    CAni*		GetDataAni			(const char* pszFileName, const char* pszTitle, int nExigence = EXIGENCE_NORMAL, DWORD dwLife = 30 * 1000);
    CAni*		GetMedal			(int nMedal, int nExigence = EXIGENCE_NORMAL);

    C3DObj*		Get3DObj	(OBJID id);
    C3DMotion*	Get3DMotion (OBJID id);
    C3DMotion*  Get3DWeaponMotion (OBJID id);
    C3DMotion*  Get3DMantleMotion (OBJID id);
    C3DMotion*  Get3DMountMotion (OBJID id);

    C3DTexture* Get3DTexture(OBJID id);
    C3DEffect*	Get3DEffect	(OBJID id);

    C3DObjInfo*	Get3DObjInfo(OBJID id);
    OBJID		Get3DTextureID	(C3DTexture* pTexture);

    C3DEffect*  CreateNew3DEffect(OBJID id);
    const char*		GetSound(OBJID idSound);
    ItemEffectInfo*		GetItemEffectInfo(OBJID id);

    const char*    GetEmotionShutCut(int nIndex);
    int			   GetEmotionAmount();
    CAni*		GetEmotionIcon(int nIndex, int nExigence = EXIGENCE_NORMAL);

    void		ShowInfoBack(int x, int y, int nWidth, int nHeight);
    const char*	GetVersionInfo	(void);

private:
    MAP_3DOBJEX		m_set3DObj;
    MAP_3DMOTIONEX		m_set3DMotion, m_set3DWeaponMotion, m_set3DMantleMotion, m_set3DMountMotion;
    MAP_3DTEXTUREEX		m_set3DTexture;
    MAP_3DEFFECTEX		m_set3DEffect;
    MAP_DYNAMICANI		m_mapDataAni;

    DWORD				m_timerCheck;
    MAP_RES				m_mapResMesh;
    MAP_RES				m_mapResMotion;
    MAP_RES				m_mapResTexture;
    MAP_RES				m_mapResEffectObj;
    MAP_RES				m_mapResWeaponMotion;
    MAP_RES				m_mapResMantleMotion;
    MAP_RES				m_mapResMountMotion;
    MAP_RES				m_mapSound;
    CStrRes				m_objStrRes;
    std::map<OBJID, ItemEffectInfo> m_mapItemEffectInfo;

    std::vector<string> m_setEmotion;

public:
    void	LoadRes(const char* pszResFile, MAP_RES& mapRes);
    const	char* GetStr(OBJID idRes) {return m_objStrRes.GetStr(idRes);}
private:
    void		CreateItemEffectInfo();
    void		DestroyItemEffectInfo();
    C3DMotion*	CreateNew3DMotion(OBJID id);

    C3DMotion*	CreateNew3DWeaponMotion(OBJID id);
    C3DMotion*	CreateNew3DMantleMotion(OBJID id);
    C3DMotion*	CreateNew3DMountMotion(OBJID id);

    C3DMotion*	CreateNew3DMotion(const char* pszFileName);
    C3DTexture* CreateNew3DTexture(OBJID id);

    void        CreateEmotionInfo();

    void		DestroyDataAni				(void);
    void		Destroy3DObj				(void);
    void		Destroy3DTexture			(void);
    void		Destroy3DMotion				(void);
    void		Destroy3DWeaponMotion		(void);
    void		Destroy3DMantleMotion		(void);
    void		Destroy3DMountMotion		(void);
    void		Destroy3DEffect				(void);

    void		ProcessDataAni				(void);
    void		Process3DObjSet				(void);
    void		Process3DMotionSet			(void);
    void		Process3DWeaponMotionSet	(void);
    void		Process3DMantalMotionSet	(void);
    void		Process3DMountMotionSet	(void);
    void		Process3DTextureSet			(void);
    void		Process3DEffectSet			(void);
    // 系统策略...
private:
    void		InitSystemPolicy(void);

public:
    DWORD m_dw3DMotionLife;
    DWORD m_dw3DTextureLife;
    DWORD m_dw3DEffectLife;
    DWORD m_dwPuzzleLife;
    DWORD m_dwPuzzlePreloadSize;
    DWORD m_dwReadThreadPriority;
public:
    void GetObjAmount(DWORD& dw3DObj, DWORD& dw3DMotion, DWORD& dw3DWMotion, DWORD& dwTexture, DWORD& dw3DEffect, DWORD& dwDataAni);
};
#ifdef _ANALYSIS_ON
extern DWORD g_dw3DObj, g_dw3DMotion, g_dw3DWMotion, g_dwTexture, g_dw3DEffect, g_dwDataAni;
#endif
//-------------------------------------------------------------------
// extenal...
extern CGameDataSet g_objGameDataSet;

#endif
