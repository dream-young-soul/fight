
//**********************************************************
// 代码编辑器
//**********************************************************

//-------------------------------------------------------------
//	3DRole.h
//-------------------------------------------------------------
#ifndef _3DROLE_H
#define _3DROLE_H
//-------------------------------------------------------------
#include "ND_Bitmap.h"
#include "3DWeapon.h"
#include "3DArmor.h"
#include "3DMount.h"
#include "3DArmet.h"
#include "IRoleView.h"
#include "3DMantle.h"
#include "3DHead.h"

enum RolePart
{
    RP_BODY = 0,		// 身体
    RP_ARMET,			// 头盔
    RP_BACK,			// 背后物件
    RP_L_WEAPON,		// 左手武器
    RP_R_WEAPON,		// 右手武器
    RP_L_SHIELD,		// 左手防具
    RP_R_SHIELD,		// 右手防具
    RP_L_SHOES,			// 左脚鞋
    RP_R_SHOES,			// 右脚鞋
};

#define		PART_MAX		5
class C3DRole : public IRoleView
{
public:
    C3DRole();
    virtual ~C3DRole ();

    // 这里的 x 对应屏幕坐标的 x, z 对应屏幕坐标的 y, 1 个单位就是 1 个象素
    // 移动
    void Move ( float x, float y, float z );
    // 旋转, 参数必须是弧度
    void Rotate	( float x, float y, float z );
    // 缩放， 1.0 就是原大小
    void Scale ( float x, float y, float z );

    // 设置当前帧数
    void SetFrame (DWORD dwFrame);

    // 下一帧 nStep 步幅 负直为倒播
    void NextFrame ( int nStep );

public:
    int			m_nPosX, m_nPosY, m_nPosH;
    int			m_nRotateX, m_nRotateY, m_nRotateZ;
    float		m_fScale;
    __int64		m_dwPlayerStatus;

    C3DArmor*	m_pArmor;		// 装甲
    C3DWeapon*  m_pLWeapon;		// 左武器
    C3DWeapon*  m_pRWeapon;		// 右武器
    C3DWeapon*  m_pLShield;		// 左护盾
    C3DWeapon*  m_pRShield;		// 右护盾
    C3DMount*	m_pMount;		// 坐骑
    C3DArmet*	m_pArmet;		// 头发，头盔
    C3DMantle*	m_pMantle;		// 披风
    C3DHead*	m_pHead;
    C3DArmor*	m_pArmorClone;	// 一份拷贝，用于幽灵、变身 etc.

    int				m_nLookClone;	// Look拷贝，用于幽灵、变身 etc.

    D3DXVECTOR3		m_LightOffset;
    int				m_nShadowType;
    int				m_nLook;
    int				m_nHair;
    int				m_nActionType;
    unsigned short	m_usStatus;

    float			m_a;
    float			m_r;
    float			m_g;
    float			m_b;
    D3DXVECTOR3		m_min, m_max;
public:
    C3DMotion*	GetWeaponMotion	(OBJID idWeapon, int nAction);
    C3DMotion*	m_pMotionArmor;
    C3DMotion*	m_pMotionArmet;
    C3DMotion*	m_pMotionWeaponR;
    C3DMotion*  m_pMotionWeaponL;
    C3DMotion*	m_pMotionMount;
    C3DMotion*	m_pMotionMantle;
    CGame3DEffectEx* m_pEffect;

    void		ResetMotion	(void);


public:
    DWORD	GetWeaponActionData (void);
    DWORD	GetMountActionData	(void);


    BOOL SetLWeapon	(OBJID idType);
    BOOL SetRWeapon	(OBJID idType);
    void SetArmor	(OBJID idType);
    void SetMount	(OBJID idType);
    void SetArmet	(OBJID idType);
    void SetMantle	(OBJID idMantle);
    void SetHead	(OBJID idFace);
    void SetLook	(int nLook);
    void SetHair	(int nHair);
    void SetStatus  (USHORT usStatus)	{m_usStatus = usStatus;}
    int  SetCloneAction  (int nActionType, BOOL bPlayerSound = false, int nWorldX = 0, int nWorldY = 0, BOOL bSetEffect = false);
    void Transform	(int nLook);
    void SetRotateX (int nRotateX) {m_nRotateX = nRotateX;}
    void SetGame3DEffectEx(CGame3DEffectEx* pEffect) {m_pEffect = pEffect;}

    void SetPlayerStatus (__int64 dwStatus);
    void ClrPlayerStatus (__int64 dwStatus);
    BOOL TestPlayerStatus (__int64 dwStatus);

public: // bascal class interface
    virtual DWORD   GetActionData(void);
    virtual BOOL	SetVariable(int nVariableType, DWORD dwData);
    virtual int		SetAction(int nActionType, BOOL bResetMotion = true, BOOL bPlayerSound = false, int nWorldX = 0, int nWorldY = 0, BOOL bSetEffect = false, DWORD dwTerrain = 0);
    virtual void	SetPos ( int nX, int nY, int nHeight, int nRotate, float fScale );
    virtual void	SetLightOffset( int type, float x, float y, float z, float a, float r, float g, float b );
    virtual void	SetRGBA ( float alpha, float red, float green, float blue );
    virtual BOOL	HitTest ( int nHitScreenX, int nHitScreenY, int nMapX, int nMapY );
    virtual BOOL	Draw ( int nMapX, int nMapY );
    virtual DWORD	GetVariable(int nVariableType);
    static IRoleView* CreateNewView();
};

#ifdef _ANALYSIS_ON
extern DWORD g_dw3DRoleDrawTime;
extern DWORD g_dw3DRoleDrawAmount;
extern DWORD g_dw3DRoleDrawTimeMax;
extern DWORD g_dw3DRoleDrawTimeFrame;
extern DWORD g_dw3DRoleDrawAmountFrame;
#endif

//-------------------------------------------------------------
#endif
