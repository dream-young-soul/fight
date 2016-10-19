
//**********************************************************
// 代码编辑器
//**********************************************************

// 3DRoleData.h: interface for the C3DRoleData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DBODY_H__E9D089A1_2D56_4A28_AF07_3BB4FCD5C818__INCLUDED_)
#define AFX_3DBODY_H__E9D089A1_2D56_4A28_AF07_3BB4FCD5C818__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <deque>
#include <vector>
#include "GameDataSet.h"
using namespace std;

typedef map<OBJID, DWORD>MAP_DATA;

struct AttackEffectInfo
{
    int   nAction;
    int	  nFrameIndex;
} ;

struct TrackInfo
{
    DWORD			dwID;
    int				nAction;
    int				nStartFrame;
    int				nEndFrame;
    int				nFrameInterval;
    int				nHeight;
    std::string		strEffect;
    std::string		strWave;
} ;

struct EffectIndex
{
    std::string		strEffect;
    std::string		strWave;
    DWORD			dwData;
} ;

struct LightInfoPart
{
    DWORD			dwTimeCost;
    unsigned char	ucAlphaRate;
    unsigned char	ucRedRate;
    unsigned char	ucGreenRate;
    unsigned char	ucBlueRate;
} ;

struct LightInfo
{
	LightInfo()
	{
		m_setLightPartInfo.clear();
	}
	~LightInfo()
	{
		for (DWORD i = 0; i < m_setLightPartInfo.size(); ++i)
		{
			SAFE_DELETE(m_setLightPartInfo[i]);
		}
		m_setLightPartInfo.clear();
	}

    vector<LightInfoPart*> m_setLightPartInfo;
} ;

struct PetInfo
{
	PetInfo()
	{
		nEnvMethod	= 0;
		nScale	= 0;
		nAttackRange	= 0;
		nAddSize	= 0;
		dwAttackInterval	= 0;

		setLocateTargetMethod.clear();
	}

    vector<int> setLocateTargetMethod;
    int			nEnvMethod;
    int			nScale;
    int			nAttackRange;
    int			nAddSize;
    DWORD		dwAttackInterval;
} ;

struct TextActionInfo
{
    string strAction;
    string strText;
} ;

struct TipsInfo
{
	TipsInfo()
	{
		m_setStr.clear();
	}
    vector<string>	m_setStr;
} ;

struct UserHelpInfo
{
	UserHelpInfo()
	{
		nProfession	= 0;
		nMinLevel	= 0;
		nMaxLevel	= 0;
		nSex		= 0;
		nMapId		= 0;
		nStatus		= 0;
		nStatusLevel	= 0;
		nItemType	= 0;
		nMagicId	= 0;
		nMagicLevel	= 0;

		m_setStr.clear();
	}

    int		nProfession;
    int		nMinLevel;
    int		nMaxLevel;
    int		nSex;
    int		nMapId;
    int     nStatus;
    int     nStatusLevel;
    int		nItemType;
    int     nMagicId;
    int		nMagicLevel;
    vector<string>m_setStr;
} ;

struct TerrainWeatherInfo
{
	TerrainWeatherInfo()
	{
		nTerrain	= 0;
		nType	= 0;
		nWindDir	= 0;
		nLevel	= 0;
		dwColor	= 0;
	}
    int	nTerrain;
    int	nType;
    int nWindDir;
    int nLevel;
    DWORD dwColor;
} ;

struct FlyingObjInfo
{
	FlyingObjInfo()
	{
		idType	= 0;
		idSimpleObj = 0;

		memset(szFlyEffect, 0, sizeof(szFlyEffect));
		memset(szFlySound, 0, sizeof(szFlySound));
		memset(szHitSound, 0, sizeof(szHitSound));
		memset(szTargetEffect, 0, sizeof(szTargetEffect));
	}
    OBJID idType;
    OBJID idSimpleObj;
    char  szFlyEffect[64];
    char  szFlySound[64];
    char  szHitSound[64];
    char  szTargetEffect[64];
} ;

struct MapMagicItemInfo
{
	MapMagicItemInfo()
	{
		idType = 0;
		memset(szStart, 0, sizeof(szStart));
		memset(szLast, 0, sizeof(szLast));
		memset(szEnd, 0, sizeof(szEnd));
	}

    OBJID	idType;
    char	szStart[64];
    char	szLast[64];
    char	szEnd[64];
} ;

struct PkTitleInfo
{
	PkTitleInfo()
	{
		nMinPkValue = 0;
		nMaxPkValue = 0;
		memset(szTitle, 0, sizeof(szTitle));
	}
    int nMaxPkValue;
    int nMinPkValue;
    char szTitle[_MAX_NAMESIZE];
} ;

struct EffectInfo
{
	EffectInfo()
	{
		dwLoopTime	= 0;
		dwFrameInterval	= 0;
		dwShowWay	= 0;
		dwLoopInterval	= 0;
		dwDelay	= 0;
		nOffsetX	= 0;
		nOffsetY	= 0;
		nExigence	= 0;

		memset(szTitle, 0 ,sizeof(szTitle));
		memset(szAniTitle, 0 ,sizeof(szAniTitle));
	}

    char	szTitle[64];
    char	szAniTitle[64];
    DWORD	dwLoopTime;
    DWORD	dwFrameInterval;
    DWORD	dwShowWay;
    DWORD	dwLoopInterval;
    DWORD	dwDelay;
    int		nOffsetX;
    int		nOffsetY;
    int		nExigence;
} ;

struct TerrainNpcTypeInfo
{
	TerrainNpcTypeInfo()
	{
		nType = 0;
		memset(szNpcName, 0 ,sizeof(szNpcName));
		setStr.clear();
	}
    int				nType;
    char			szNpcName[_MAX_NAMESIZE];
    vector<string>	setStr;
} ;

struct NpcTypeInfo
{
	NpcTypeInfo()
	{
		nType		= 0;
		idSimpleObj		= 0;
		idMotionStandBy		= 0;
		idMotionBlaze		= 0;
		idMotionBlaze1		= 0;
		idMotionBlaze2		= 0;
		idMotionRest		= 0;
		nZoomPercent		= 0;
		bMouseSign		= 0;
		bChangeDir		= 0;
		nFront		= 0;
		nBack		= 0;
		nLeft		= 0;
		nRight		= 0;

		memset(szNpcName, 0 ,sizeof(szNpcName));
		memset(szEffectIndex, 0 ,sizeof(szEffectIndex));
		memset(szNpcTitle, 0 ,sizeof(szNpcTitle));
	}

    int		nType;
    char	szNpcName[_MAX_NAMESIZE];
    char	szEffectIndex[64];
    OBJID	idSimpleObj;
    OBJID	idMotionStandBy;
    OBJID	idMotionBlaze;
    OBJID	idMotionBlaze1;
    OBJID	idMotionBlaze2;
    OBJID	idMotionRest;
    char	szNpcTitle[128];
    int		nZoomPercent;
    BOOL	bMouseSign;
    BOOL	bChangeDir;
    int		nFront;
	int		nBack;
	int		nLeft;
	int		nRight;
} ;

struct Npc2DTypeInfo
{
	Npc2DTypeInfo()
	{
		nType = 0;
		memset(szNpcName, 0, sizeof(szNpcName));
		memset(pszScene, 0, sizeof(pszScene));
	}
    int		nType;
    char	szNpcName[_MAX_NAMESIZE];
    char*	pszScene[8];
} ;


struct RolePartInfo
{
	RolePartInfo()
	{
		idMesh	= 0;
		idTexture = 0;
		idTexture2 = 0;
		fMoveRateX = 0.f;
		fMoveRateY = 0.f;
	}
    //char szIndex[64];
    OBJID idMesh;
    OBJID idTexture;
    OBJID idTexture2;
    float fMoveRateX;
    float fMoveRateY;
} ;

enum
{
    ROLEPARTINFO_WEAPON	= 0,
    ROLEPARTINFO_ARMET	= 1,
    ROLEPARTINFO_AEMOR	= 2,
    ROLEPARTINFO_MANTLE	= 3,
    ROLEPARTINFO_MOUNT	= 4,
    ROLEPARTINFO_HEAD	= 5,
};
typedef map<OBJID, RolePartInfo>MAP_ROLEPARTINFO;

struct ActionInfo
{
	ActionInfo()
	{
		nActionIndex = 0;
		dwTimeInterval = 0;
	}
    int		nActionIndex;
    DWORD	dwTimeInterval;
} ;
typedef deque<ActionInfo*>DEQUE_ACTION;

struct ActionMapEffectInfo
{
	ActionMapEffectInfo()
	{
		idx	= 0;
		nShowTime = 0;
		bRotate = false;
		nType = 0;
		memset(szEffect, 0, sizeof(szEffect));
	}
    int		idx;
    int		nShowTime;
    BOOL	bRotate;
    char	szEffect[32];
    int		nType;
} ;
typedef deque<ActionMapEffectInfo*> DEQUE_ACTIONMAPEFFECT;


struct WeaponCombinInfo
{
	WeaponCombinInfo()
	{
		idxRWeapon = 0;
		idxLWeapon = 0;
	}
    DWORD idxRWeapon;
    DWORD idxLWeapon;
} ;
typedef deque<WeaponCombinInfo*>DEQUE_WEAPONCOMBIN;

typedef map<__int64, string>MAP_GAME3DEFFECTINFO;
typedef map<__int64, string>MAP_GAMESOUNDINFO;

struct CMy3DEffectInfo
{
	CMy3DEffectInfo()
	{
		nAmount		= 0;
		nASB[8]		= 0;
		nADB[8]		= 0;
		nDelay		= 0;
		nLoopTime		= 0; 
		nFrameInterval		= 0;
		nLoopInterval		= 0;
		nOffsetX		= 0;
		nOffsetY		= 0;
		nOffsetZ		= 0;
		nShapeAir		= 0;
		memset(szIndex, 0, sizeof(szIndex));
		memset(szIndex, ID_NONE, sizeof(idEffect));
		memset(szIndex, ID_NONE, sizeof(idTexture));
	}
    char	szIndex[64];
    int		nAmount;
    OBJID	idEffect[8];
    OBJID	idTexture[8];
    int		nASB[8];
    int		nADB[8];
    int		nDelay;
	int		nLoopTime; 
	int		nFrameInterval;
	int		nLoopInterval;
    int		nOffsetX;
	int		nOffsetY;
	int		nOffsetZ;
	int		nShapeAir;
};
typedef deque<CMy3DEffectInfo*> DEQUE_MY3DEFFECTINFO;

struct CMonsterInfo
{
	CMonsterInfo()
	{
		idType = ID_NONE;
		nAddSize	= 0;
		nZoomPercent = 0;
		nMaxLife	= 0;
		nLevel		= 0;
		nWhiteBlack	= 0;

		memset(szMonster, 0, sizeof(szMonster));
		memset(szDisappearEffect, 0, sizeof(szDisappearEffect));
		memset(szDisappearSound, 0, sizeof(szDisappearSound));
	}
    OBJID	idType;
    int		nAddSize;
	int		nZoomPercent;
	int		nMaxLife;
	int		nLevel;
	int		nWhiteBlack;
	char	szMonster[16];
    char	szDisappearEffect[128];
    char	szDisappearSound[128];
};
typedef deque<CMonsterInfo*>DEQUE_MONSTERINFO;

struct CTransformInfo
{
    int		nIndex;
    int		nLook;
    int		nAddSize;
    BOOL	bCanJump;
    int		nScale;
};
typedef vector<CTransformInfo>VECTOR_TRANSFORMINFO;

CONST int _MAXPART_SIMPLEOBJ = 4;
struct C3DSimpleObjInfo
{
	C3DSimpleObjInfo()
	{
		idType		= ID_NONE;
		nParts		= 0;
		memset(idMesh, ID_NONE, sizeof(idMesh));
		memset(idTexture, ID_NONE, sizeof(idTexture));
	}

    OBJID	idType;
    int		nParts;
    OBJID	idMesh[_MAXPART_SIMPLEOBJ];
    OBJID	idTexture[_MAXPART_SIMPLEOBJ];
} ;

struct C3DItemInfo
{
	C3DItemInfo()
	{
		idMesh	= 0;
		idTexture	= 0;
		usPercentItemBox = 0;
		usPercentItemMap = 0;
	}

    OBJID idMesh;
    OBJID idTexture;
    unsigned short usPercentItemBox;
    unsigned short usPercentItemMap;
} ;

struct StatusEffectInfo
{
	StatusEffectInfo()
	{
		dwStatus = 0;
		memset(sz3DEffect, 0, sizeof(sz3DEffect));
		memset(sz2DEffect, 0, sizeof(sz2DEffect));
	}
    __int64	dwStatus;
    char	sz3DEffect[64];
    char	sz2DEffect[64];
} ;

// 动作节奏, 位移控制
struct CActionCtrlInfo
{
	enum { MAX_ACTION_SECTION = 8 };
	CActionCtrlInfo()
	{
		nSection	= 0;
		memset(nTimePercent, 0, sizeof(nTimePercent));
		memset(nMovePercent, 0, sizeof(nMovePercent));
	}
    
    int		nSection;
    int		nTimePercent[MAX_ACTION_SECTION];
    int		nMovePercent[MAX_ACTION_SECTION];
} ;

// 动作延时
struct CActionDelayInfo
{
	CActionDelayInfo()
	{
		tmWoundDelay = 0;
		tmBlockDelay = 0;
		tmDieDelay = 0;
	}

    DWORD	tmWoundDelay;
    DWORD	tmBlockDelay;
    DWORD	tmDieDelay;
} ;

// 武器打击效果
struct CWeaponInfo
{
	CWeaponInfo()
	{}

    std::string		strHitEffect;
    std::string		strHitSound;
    std::string		strBlkEffect;
    std::string		strBlkSound;
} ;

//根据face重新设定光效的x, y ,z
struct EffectLookSet
{
	EffectLookSet()
	{
		memset(szEffect, 0, sizeof(szEffect));
		nX	= 0;
		nY	= 0;
		nZ	= 0;
	}

    char szEffect[32];
    int	 nX;
    int  nY;
    int  nZ;
} ;

struct EffectLookSetInfo
{
	EffectLookSetInfo()
	{
		dwLook		= 0;
		nDefaultX	= 0;
		nDefaultY	= 0;
		nDefaultZ	= 0;
		nAmount		= 0;
		setEffect.clear();
	}

    DWORD dwLook;
    int	  nDefaultX;	//默认
    int   nDefaultY;	//默认
    int   nDefaultZ;	//默认
    int	  nAmount;
    vector<EffectLookSet> setEffect;
} ;

class C3DRoleData
{
public:
    C3DRoleData();
    virtual ~C3DRoleData();

public:
    void	Init(void);

    CONST CActionCtrlInfo* GetActionCtrlInfo (DWORD dwKey) CONST;
    DWORD GetActionTimeInterval(DWORD dwActionIndex);
    const ActionMapEffectInfo* GetActionMapEffect (int nLook, int nAction, int nTerrain) const;
    const char* GetRolePart3DEffect(DWORD dwLook, DWORD idxWeapon, DWORD idxAction, DWORD idxVar);
    BOOL  CanWeaponCombin(DWORD idxRWeapon, DWORD idxLWeapon);
    const char* GetActionSound (DWORD dwTerrain, DWORD idxLook, DWORD idxWeapon, DWORD idxAction);
    const CMonsterInfo* GetMonsterInfo(const char* pszMonster);
    const CMonsterInfo* GetMonsterInfo(DWORD idType);   //因为要支持幻兽改名所以用idtype做索引
    int		GetTransformAdditiveSize(int nLook);
    BOOL	GetTransformJumpAble(int nLook);
    CTransformInfo*	GetTransformInfo(int nIndex);

    const CMy3DEffectInfo* GetMy3DEffectInfo(const char* pszIndex);


    C3DSimpleObjInfo*	Get3DSimpleObjInfo(OBJID idType);
    void				DestroySimpleObjInfo();

    NpcTypeInfo*		GetNpcTypeInfo(OBJID idType);
    Npc2DTypeInfo&		GetNpc2DTypeInfo(OBJID idType);
    EffectInfo* GetEffectInfo(char* pszIndex);
    void		CreateEffectInfo();
    MapMagicItemInfo*			GetMapMagicItemInfo(int nType);
    FlyingObjInfo*				GetFlyingObjInfo(OBJID idItem);

    StatusEffectInfo*			GetStatusEffectInfoByIndex(int nIndex);
    DWORD						GetStatusEffectInfoAmount();

    const char*					GetUserHelpInfoLineByIndex(int nProfessional, int nLevel, int nSex, int nMapId, int nStatus, int nStatusLevel, int nItemType, int nMagicId, int nIndex);
    int							GetUserHelpInfoLineAmount(int nProfessional, int nLevel, int nSex, int nMapId, int nStatus, int nStatusLevel, int nItemType, int nMagicId);
    int							GetUserHelpInfoAmount();
    UserHelpInfo*				GetUserHelpInfoByIndex(int nIndex);

    TerrainNpcTypeInfo*			GetTerrainNpcInfo(OBJID idType);

    int							GetTipsAmount();
    int							GetTipLineAmount(int nIndex);
    char*						GetTipLine(int nTipIndex, int nLineIndex);

    int							GetTextActionAmount();
    char*						GetTextActionTitle(int nIndex);
    char*						GetTextActionText(int nIndex);

    PetInfo*					GetPetInfo(int nIndex);

    LightInfo*					GetLightInfoByIndex(int nIndex);
    DWORD						GetRolePartLightInfo(OBJID idType);

    BOOL						Get3DItem(OBJID idItemType, OBJID& idMesh, OBJID& idTexture,
                                          unsigned short& usBoxSize, unsigned short& usMapSize);
    const char*					GetHonorTitle(int nIndex);
    const char*					GetNobilityTitle(int nIndex);
    const char*					GetMedal(int nIndex);

    CONST CActionDelayInfo*		GetActionDelayInfo	(DWORD dwKey) CONST;
    CONST CWeaponInfo*			GetWeaponInfo		(DWORD dwKey) CONST;

    BOOL			GetEffectLookSetInfo(DWORD dwLook, char* szEffect, int& x, int& y, int& z) ;

private:
    map<OBJID, EffectLookSetInfo*>	m_setEffectLookSetInfo;
    MAP_DATA					m_setActionInfo;
    DEQUE_ACTIONMAPEFFECT		m_setMapEffectInfo;
    DEQUE_WEAPONCOMBIN			m_setWeaponCombinInfo;
    MAP_GAME3DEFFECTINFO		m_setGame3DEffectInfo;
    MAP_GAMESOUNDINFO			m_setSoundInfo;
    DEQUE_MONSTERINFO			m_setMonsterInfo;
    VECTOR_TRANSFORMINFO		m_setTransFormInfo;
    DEQUE_MY3DEFFECTINFO		m_setMy3DEffectInfo;

    MAP_ROLEPARTINFO			m_setArmor;
    MAP_ROLEPARTINFO			m_setArmet;
    MAP_ROLEPARTINFO			m_setWeapon;
    MAP_ROLEPARTINFO			m_setMantle;
    MAP_ROLEPARTINFO			m_setMount;
    MAP_ROLEPARTINFO			m_setHead;

    int							m_nMaleFaceAmount;
    int							m_nFemaleFaceAmount;
    int							m_nEmblemAmount;

    vector<TextActionInfo*>		m_setTextAction;
    vector<TipsInfo*>			m_setTipsInfo;
    vector<TerrainNpcTypeInfo*> m_setTerrainNpcTypeInfo;
    vector<C3DSimpleObjInfo*>	m_vct3DSimpleObjInfo;
    vector<NpcTypeInfo*>		m_setNpcType;
    vector<Npc2DTypeInfo*>		m_set2DNpcType;
    vector<PkTitleInfo*>		m_setPkTitle;
    vector<EffectInfo*>			m_setEffectInfo;
    vector<MapMagicItemInfo*>	m_setMapMagicItem;
    vector<FlyingObjInfo*>		m_setFlyObjInfo;
    vector<TerrainWeatherInfo*> m_setTerrainWeatherInfo;
    vector<StatusEffectInfo*>	m_setStatusEffectInfo;
    vector<UserHelpInfo*>		m_setUserHelpInfo;
    vector<PetInfo*>			m_setPetInfo;
    vector<LightInfo*>			m_setLightInfo;
    vector<string>				m_setHonorTitle;
    vector<string>				m_setNobilityTitle;
    vector<string>				m_setMedal;

    map<DWORD, CActionCtrlInfo >	m_setActionCtrlInfo;
    MAP_DATA						m_setRolePartLightInfo;
    map<OBJID, C3DItemInfo>			m_set3DItemInfo;
    map<OBJID, TrackInfo*>			m_setTrackInfo;
    map<OBJID, EffectIndex>			m_setEffectIndexInfo;
    map<OBJID, AttackEffectInfo*>	m_setAttackEffectInfo;
    int							m_nLeaderScale;
    int							m_nAssistantScale;
    int							m_nSubLeaderScale;
    int							m_nLeaderMaxLife;
    int							m_nAssistantMaxLife;
    int							m_nSubLeaderMaxLife;
private:
    BOOL	CreateActionDelayInfo();
    void	DestroyActionDelayInfo();
    std::map< DWORD, CActionDelayInfo > m_setActionDelayInfo;

    BOOL	CreateWeaponInfo();
    void	DestoryWeaponInfo();
    std::map< DWORD, CWeaponInfo > m_setWeaponInfo;

    BOOL    CreateActionCtrlInfo();
    void    DestroyActionCtrlInfo();

    BOOL	CreateAttackEffectInfo();
    void	DestroyAttackEffectInfo();

    BOOL	CreateTrackInfo();
    void	DestroyTrackInfo();

    void	CreateEffectLookSetInfo();
    void	DestroyEffectLookSetInfo();

    BOOL	CreateEffectIndexInfo();
    void	DestroyEffectIndexInfo();

    BOOL	CreateMedalInfo();
    void	DestroyMedalInfo();

    BOOL	CreateHonorTitleInfo();
    void	DestroyHonorTitleInfo();

    BOOL	CreateNobilityTitleInfo();
    void	DestroyNobilityTitleInfo();

    BOOL	Create3DItemInfo();
    void	Destroy3DItemInfo();

    BOOL	CreateLightLInfo();
    void	DestroyLightInfo();

    BOOL	CreateRolePartLightInfo();
    void	DestroyRolePartLightInfo();

    BOOL	CreatePetInfo();
    void	DestroyPetInfo();


    BOOL	CreateTextAction();
    void	DestroyTextAction();

    BOOL	CreateTipsInfo();
    void	DestroyTipsInfo();

    BOOL	CreateTerrainNpcTypeInfo();
    void	DestroyTerrainNpcTypeInfo();

    BOOL	CreateUserHelpInfo();
    void	DestroyUserHelpInfo();

    BOOL	CreateStatusEffectInfo();
    void	DestroyStatusEffectInfo();

    BOOL	CreateTerrainWeatherInfo();
    void	DestroyTerrainWeatherInfo();

    BOOL	CreateFlyObjInfo();
    void	DestroyFlyObjInfo();

    void	DestroyEffectInfo();

    BOOL	CreateMapMagicItemInfo();
    void	DestroyMapMagicItemInfo();

    BOOL	CreatePkTitleInfo();
    void	DestroyPkTitleInfo();

    BOOL	CreateNpcTypeInfo();
    void	DestroyNpcTypeInfo();

    BOOL	Create2DNpcTypeInfo();
    void	Destroy2DNpcTypeInfo();

    BOOL	CreateActionInfo();
    void	DestroyActionInfo();

    BOOL	CreateActionMapEffectInfo();
    void	DestroyActionMapEffectInfo();

    BOOL	CreateWeaponCombinInfo();
    void	DestroyWeaponCombinInfo();

    BOOL	Create3DEffectInfo();
    void	Destroy3DEffectInfo();

    void	CreateSoundInfo();
    void	DestroySoundInfo();

    BOOL	CreateTransFormInfo();
    void	DestroyTransFormInfo();

    BOOL	CreateMonsterInfo();
    void	DestroyMonsterInfo();

    void	CreateMy3DEffectInfo();
    void	DestroyMy3DEffectInfo();

    void	CreateRolePartInfo();
    void	DestroyRolePartInfo();

    void	CreateRolePartInfo(MAP_ROLEPARTINFO& m_setInfo, const char* pszFilename);
    void	DestroyRolePartInfo(MAP_ROLEPARTINFO& m_setInfo);
    RolePartInfo* GetRoleRartInfo(MAP_ROLEPARTINFO& m_setInfo, const char* pszTitle);

    void	LoadCommonIni();

    void	Create3DSimpleObjInfo();
public:
    EffectIndex* GetEffectIndexInfo(OBJID id);
    TrackInfo*	GetTrackInfo(OBJID id);
    AttackEffectInfo* GetAttackEffectInfo(int nMount, int nLook, int nAction, int nWeapon);
    void	UpdateMy3DEffectInfo();
    TerrainWeatherInfo* GetWeatherInfo(int nTerrain);
    RolePartInfo* GetRoleRartInfo(int nRolePartIndex, const char* pszTitle);
    int	GetMaleFaceAmount() {return m_nMaleFaceAmount;}
    int GetFemaleFaceAmount() {return m_nFemaleFaceAmount;}
    int GetEmblemAmount() {return m_nEmblemAmount;}

    int GetLeaderScale() {return m_nLeaderScale;}
    int GetAssistantScale() {return m_nAssistantScale;}
    int GetSubLeaderScale() {return m_nSubLeaderScale;}

    int GetLeaderMaxLife() {return m_nLeaderMaxLife;}
    int GetAssistantMaxLife() {return m_nAssistantMaxLife;}
    int GetSubLeaderMaxLife() {return m_nSubLeaderMaxLife;}


public:
    char* GetPkTitle(int nPkValue);
};
extern C3DRoleData g_obj3DRoleData;

#endif // !defined(AFX_3DBODY_H__E9D089A1_2D56_4A28_AF07_3BB4FCD5C818__INCLUDED_)
